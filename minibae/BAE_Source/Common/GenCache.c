/*
    Copyright (c) 2009 Beatnik, Inc All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
    
    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
    
    Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    
    Neither the name of the Beatnik, Inc nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
    TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************
**
**  "GenCache.c"
**
**  Generalized Music Synthesis package. Part of SoundMusicSys.
**
**  © Copyright 2000-2001 Beatnik, Inc, All Rights Reserved.
**  Written by Steve Hales, Mark Deggeler, Andrew Rostaing
**
**  Beatnik products contain certain trade secrets and confidential and
**  proprietary information of Beatnik.  Use, reproduction, disclosure
**  and distribution by any means are prohibited, except pursuant to
**  a written license from Beatnik. Use of copyright notice is
**  precautionary and does not imply publication or disclosure.
**
**  Restricted Rights Legend:
**  Use, duplication, or disclosure by the Government is subject to
**  restrictions as set forth in subparagraph (c)(1)(ii) of The
**  Rights in Technical Data and Computer Software clause in DFARS
**  252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
**  Computer Software--Restricted Rights at 48 CFR 52.227-19, as
**  applicable.
**
**  Confidential-- Internal use only
**
**
**  Overview
**  This contains code to  manage the mixer's sample cache.
**
**
**  Modification History:
**
**  2000.04.21 AER  File Created
**  2000.05.17 AER  Made some minor modifications for Mac compiles
**  2000.05.18 AER  D'oh! One little mistake, and the sample cache goes unused:
**                      fixed massive massive massive memory leak
**                  Fixed improper return value when cache pointer isn't found
**                      from sample pointer
**  2000.07.14 AER  Modified access to XBankTokens for new system
**
******************************************************************************/

#include "GenCache.h"
#include "X_API.h"
#include "X_Formats.h"
#include "X_Assert.h"
#include "GenPriv.h"
#include "GenSnd.h"

//#define DISPLAY_INSTRUMENTS   1

// Private Prototypes
static OPErr PV_FreeCacheEntry(GM_Mixer * pMixer, GM_SampleCacheEntry * pCache);
UINT32 PV_GetCacheIndexFromCachePtr(GM_Mixer * pMixer,
                                    GM_SampleCacheEntry * pCache,
                                    OPErr * pErr);
OPErr PV_PlaceSampleInCache(GM_Mixer * pMixer, GM_SampleCacheEntry * pCache);

// if CONFORM_SAMPLES is 1, then sample data is modified to match, as closely as possible
// the hardware output. Sample rate conversion is not appiled.

#define CONFORM_SAMPLES     1


#if CONFORM_SAMPLES
#if USE_STEREO_OUTPUT == FALSE
static XPTR PV_ConvertToMono(XPTR pSamples, SampleDataInfo *pInfo)
{
    unsigned long   sizeb, count;
    XPTR            newData = NULL;
    XWORD           *src16, *dest16;
    XBYTE           *src8, *dest8;

    if (pSamples && pInfo)
    {
        if (pInfo->channels > 1)
        {
            sizeb = pInfo->frames * ((pInfo->bitSize == 16) ? sizeof(short) : sizeof(char));
            newData = XNewPtr((long)sizeb);
            if (newData)
            {
                if (pInfo->bitSize == 16)
                {
                    dest16 = (XWORD *)newData;
                    src16 = (XWORD *)pSamples;
                    for (count = 0; count < pInfo->frames; count++)
                    {
                        dest16[count] = (src16[count] + src16[count+1]) / 2;
                        dest16++;
                        src16 += 2;
                    }
                }
                else
                {
                    dest8 = (XBYTE *)newData;
                    src8 = (XBYTE *)pSamples;
                    for (count = 0; count < pInfo->frames; count++)
                    {
                        dest8[count] = (src8[count] + src8[count+1]) / 2;
                        dest8++;
                        src8 += 2;
                    }
                }
                pInfo->channels = 1;
                pInfo->size = sizeb;
            }
        }
    }
    return newData;
}
#endif

#if USE_16_BIT_OUTPUT == FALSE
static XPTR PV_ConvertTo8Bit(XPTR pSamples, SampleDataInfo *pInfo)
{
    XPTR    newData = NULL;

    if (pSamples && pInfo)
    {
        if (pInfo->bitSize != 8)
        {
            newData = XConvert16BitTo8Bit((XWORD *)pSamples, pInfo->frames, pInfo->channels);
            if (newData)
            {
                XDisposePtr(pSamples);
                pInfo->bitSize = 8;
                pInfo->size = pInfo->frames * sizeof(char);
            }
        }
    }
    return newData;
}
#endif
#endif

/******************************************************************************
*******************************************************************************
*******************************************************************************
**
**  Functions which directly manipulate the Sample Cache
**
*******************************************************************************
*******************************************************************************
******************************************************************************/


/******************************************************************************
**
**  GMCache_BuildSampleCacheEntry (previously PV_GetSampleData and
**                                            GMCache_BuildCacheSample)
**
**  This function:
**      * Takes either an XSampleID/XBankToken pair or a PCM sample pointer,
**      * Creates and fills out a cache entry,
**      * Places it in the cache (so it has a reference count of one), and
**      * Returns a pointer to that entry.
**  The cache entry data structure contains, among other things, a pointer to
**      the PCM sound data.
**  If a pointer to PCM data is passed (UseThisSnd), it is not disposed after
**      use, so you must dispose of it.
**
**  ????.??.?? ???  Function created
**  2000.05.02 AER  Renamed function BuildCacheSampleData, since that's what
**                      it really does, after all...
**                  Funtion previously returned redundant data -- the out
**                      parameter pCache contained theSound as a member...
**                      Now, return an OPErr, and have caller use pCache
**
******************************************************************************/
GM_SampleCacheEntry * GMCache_BuildSampleCacheEntry(GM_Mixer * pMixer,
                                                    const XSampleID theID,
                                                    const XBankToken bankToken,
                                                    const XPTR useThisSnd,
                                                    OPErr * pErr)
{
    XPTR                    theData, thePreSound;
    GM_SampleCacheEntry *   pCache;
    SampleDataInfo          newSoundInfo;
    INT32                   size;

    *pErr = NO_ERR;
    pCache = NULL;

    //  First, gripe if it already exists in the cache...
    if (GMCache_IsIDInCache(pMixer, theID, bankToken) == TRUE)
    {
        *pErr = ALREADY_EXISTS;
        return NULL;
    }

    if (useThisSnd)
    {
        theData = useThisSnd;
    }
    else
    {
        theData = XGetSoundResourceByID(theID, &size);
    }
    if (theData)
    {
        // convert snd resource into a simple pointer of data with information
        thePreSound = XGetSamplePtrFromSnd(theData, &newSoundInfo);

        if (newSoundInfo.pMasterPtr != theData)
        {   // this means that XGetSamplePtrFromSnd created a new sample
            XDisposePtr(theData);
        }

        #if CONFORM_SAMPLES
        // modify samples, so that we don't waste memory.
        // stereo to mono, if we can only play mono output
        // 16 to 8 bit, if we are only 8 bit output
            #if USE_STEREO_OUTPUT == FALSE
                if (newSoundInfo.channels > 1)
                {
                    thePreSound = PV_ConvertToMono(thePreSound, &newSoundInfo);
                }
            #endif

            #if USE_16_BIT_OUTPUT == FALSE
                if (newSoundInfo.bitSize == 16)
                {
                    thePreSound = PV_ConvertTo8Bit(thePreSound, &newSoundInfo);
                }
            #endif
        #endif
        if (thePreSound)
        {
            pCache = (GM_SampleCacheEntry *) XNewPtr(sizeof(GM_SampleCacheEntry));
            if (pCache)
            {
                // validate loop points
                if ((newSoundInfo.loopStart > newSoundInfo.loopEnd) ||
                    (newSoundInfo.loopEnd > newSoundInfo.frames) ||
                    ((newSoundInfo.loopEnd - newSoundInfo.loopStart) < MIN_LOOP_SIZE) )
                {
                    // disable loops
                    newSoundInfo.loopStart = 0;
                    newSoundInfo.loopEnd = 0;
                }
                pCache->theID = theID;
                // 2000.04.28 AER   Use a bank token to assist in uniquely
                //                      identifying samples in the cache...
                pCache->bankToken = bankToken;
                pCache->referenceCount = 0;
                pCache->waveSize = newSoundInfo.size;
                pCache->waveFrames = newSoundInfo.frames;
                pCache->loopStart = newSoundInfo.loopStart;
                pCache->loopEnd = newSoundInfo.loopEnd;
                pCache->baseKey = newSoundInfo.baseKey;
                pCache->bitSize = (char)newSoundInfo.bitSize;
                pCache->channels = (char)newSoundInfo.channels;
                pCache->rate = newSoundInfo.rate;

#if DISPLAY_INSTRUMENTS
                BAE_PRINTF(
                       "---->Getting 'snd' ID %ld rate %ld loopstart %ld loopend %ld basekey %ld\n",
                       (long)theID,
                       XFIXED_TO_LONG(pCache->rate),
                       pCache->loopStart,
                       pCache->loopEnd,
                       (long)pCache->baseKey);
#endif
                pCache->pSampleData = thePreSound;
                pCache->pMasterPtr = newSoundInfo.pMasterPtr;

                PV_PlaceSampleInCache(pMixer, pCache);
            }
            else
            {
                *pErr = MEMORY_ERR;
            }
        }
        else
        {
            *pErr = MEMORY_ERR;
        }
    }
    else
    {
        *pErr = BAD_SAMPLE;
    }
    return pCache;
}


/******************************************************************************
**
**  PV_PlaceSampleInCache (previously GMCache_PlaceSampleInCache)
**
**  Places a sample in the cache if it is not there already.
**  The function takes over ownership of the cache entry sent (Callee-Owns)
**
**  2000.03.08 AER  Function created
**  2000.03.16 AER  Now returns an error if user tries to enter a sample more
**                      than once (only if Mixer is caching samples)
**  2000.03.17 AER  Added Token checking to prevent conflicts
**                  NOTE: This function should really scan the cache for an
**                      ID and a token, return an error if it finds them,
**                      and place the sample if it doesn't
**  2000.03.21 AER  Removed query for cache samples flag
**  2000.03.28 AER  Function now sets the reference count for the cache
**  2000.05.02 AER  Imported from MiniBAE
**  2000.05.16 AER  Killed Caller-Owns handling of function and made private
**
******************************************************************************/
OPErr PV_PlaceSampleInCache(GM_Mixer * pMixer, GM_SampleCacheEntry * pCache)
{
    register INT16          count;
    OPErr                   pErr;

    pErr = GENERAL_BAD;

    for (count = 0; count < MAX_SAMPLES; count++)
    {
        if (pMixer->sampleCaches[count] == NULL)
        {
            if (pCache)
            {
                pMixer->sampleCaches[count] = pCache;
                pMixer->sampleCaches[count]->referenceCount = 1;
                pErr = NO_ERR;
                break;
            }
            else
            {
                pErr = MEMORY_ERR;
                break;
            }
        }
        else if (pMixer->sampleCaches[count]->theID == pCache->theID &&
                 AreBankTokensIdentical(pMixer->sampleCaches[count]->bankToken, pCache->bankToken))
        {
            pErr = ALREADY_EXISTS;
            BAE_ASSERT(FALSE);
            break;
        }
    }
    return pErr;
}


/******************************************************************************
**
**  GMCache_IncrCacheEntryRef
**
**  Increments a cache's reference count
**
**  2000.03.08 AER  Function created
**  2000.05.09 AER  Imported from MiniBAE (was named GM_CacheAddRef)
**
******************************************************************************/
OPErr GMCache_IncrCacheEntryRef(const GM_Mixer * pMixer,
                                GM_SampleCacheEntry * pCache)
{
#ifdef DISPLAY_CACHE_SAVINGS
    char                foo[255];
    static UINT32       byteCount = 0;
#endif // DISPLAY_CACHE_SAVINGS

    if (pMixer && pCache)
    {
        pCache->referenceCount++;
#ifdef DISPLAY_CACHE_SAVINGS
        byteCount += pCache->waveSize;
        sprintf(foo, "Added a cache entry of %d bytes\n", pCache->waveSize);
        OutputDebugString(foo);
        sprintf(foo, "Total Savings of %d bytes so far...\n", byteCount);
        OutputDebugString(foo);
#endif // DISPLAY_CACHE_SAVINGS
        return NO_ERR;
    }
    return PARAM_ERR;
}


/******************************************************************************
**
**  GMCache_DecrCacheEntryRef
**
**  Decrements a cache's reference count
**  Removes the cache entry should its reference count hit zero
**
**  2000.03.08 AER  Function created but not implemented
**  2000.03.29 AER  Function completed and integrated
**  2000.05.09 AER  Imported from MiniBAE (was named GM_CacheRelease)
**
******************************************************************************/
OPErr GMCache_DecrCacheEntryRef(GM_Mixer * pMixer,
                                GM_SampleCacheEntry * pCache)
{
    OPErr               pErr;

    if (pMixer && pCache)
    {
        pCache->referenceCount--;
        if (pCache->referenceCount == 0)
        {
            pErr = PV_FreeCacheEntry(pMixer, pCache);
        }
        return NO_ERR;
    }
    return PARAM_ERR;
}


/******************************************************************************
**
**  GMCache_ClearSampleCache
**
**  Forces the Mixer to remove all entries from the sample cache
**  NOTE:   Do not abuse this function... It can obscure instrument resource
**          leaks that may be important to track otherwise
**
**  2000.03.29 AER  Function (reluctantly) created
**  2000.05.08 AER  Function imported from MiniBAE
**
******************************************************************************/
OPErr GMCache_ClearSampleCache(GM_Mixer * pMixer)
{
    register INT16      count;

    BAE_ASSERT(pMixer);

    if (pMixer)
    {
        for (count = 0; count < MAX_SAMPLES; count++)
        {
            if (pMixer->sampleCaches[count])
            {
                PV_FreeCacheEntry(pMixer, pMixer->sampleCaches[count]);
            }
        }
        return NO_ERR;
    }
    return PARAM_ERR;
}


/******************************************************************************
**
**  PV_FreeCacheEntry
**
**  Disposes of a cache entry's memory and place in the mixer's list of entries
**
**  ????.??.?? ???  Function created
**  2000.05.15 AER  Function modified to NULL out entry in mixer list
**
******************************************************************************/
static OPErr PV_FreeCacheEntry(GM_Mixer * pMixer, GM_SampleCacheEntry * pCache)
{
    UINT32              entryLoc;
    OPErr               pErr;

    if (pCache)
    {
        if (pCache->pSampleData)
        {
            XDisposePtr(pCache->pMasterPtr);
        }
        XDisposePtr(pCache);
    }
    else
    {
        return RESOURCE_NOT_FOUND;
    }

    entryLoc = PV_GetCacheIndexFromCachePtr(pMixer, pCache, &pErr);
    if (pErr != NO_ERR)
    {
        return RESOURCE_NOT_FOUND;
    }
    pMixer->sampleCaches[entryLoc] = NULL;
    return NO_ERR;
}


/******************************************************************************
*******************************************************************************
*******************************************************************************
**
**  Functions to find cache entries
**
*******************************************************************************
*******************************************************************************
******************************************************************************/


/******************************************************************************
**
**  GM_IsIDInCache
**
**  Given a song resource ID, this will return whether there's a cache entry
**      matching the ID
**  This functionality was split off from FindCacheFromID.... That function
**      now returns an error if it cannot find the entry
**
**  2000.03.15 AER  Function created
**  2000.03.17 AER  Adapted function to require a token
**  2000.05.09 AER  Imported from MiniBAE
**
******************************************************************************/
XBOOL GMCache_IsIDInCache(const GM_Mixer * pMixer,
                          const XSampleID theID,
                          const XBankToken bankToken)
{
    register INT16          count;
    GM_SampleCacheEntry *   pCache;

    BAE_ASSERT(pMixer);

    if (pMixer)
    {
        for (count = 0; count < MAX_SAMPLES; count++)
        {
            pCache = pMixer->sampleCaches[count];
            if (pCache)
            {
                if (pCache->theID == theID &&
                    AreBankTokensIdentical(pCache->bankToken, bankToken))
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}


/******************************************************************************
**
**  GMCache_GetCachePtrFromID (previously PV_FindCacheFromID)
**
**  Returns a pointer to a mixer cache entry (or NULL) given an ID and bank
**
**  ????.??.?? ???  Function created
**  2000.05.12 AER  Renamed function GMCache_GetCachePtrFromID
**  2000.05.15 AER  Added OPErr out parameter
**
******************************************************************************/
GM_SampleCacheEntry * GMCache_GetCachePtrFromID(const GM_Mixer * pMixer,
                                                const XSampleID theID,
                                                const XBankToken bankToken,
                                                OPErr * pErr)
{
    register UINT16         count;
    GM_SampleCacheEntry *   pCache;

    if (pMixer)
    {
        for (count = 0; count < MAX_SAMPLES; count++)
        {
            pCache = pMixer->sampleCaches[count];
            if (pCache)
            {
                if (pCache->theID == theID &&
                    AreBankTokensIdentical(pCache->bankToken, bankToken))
                {
                    *pErr = NO_ERR;
                    return pCache;
                }
            }
        }
        *pErr = RESOURCE_NOT_FOUND;
        return NULL;
    }
    *pErr = PARAM_ERR;
    return NULL;
}


/******************************************************************************
**
**  GMCache_GetCachePtrFromPtr
**
**  Returns a pointer to a mixer cache entry (or NULL) given a pointer to
**      sample data
**
**  2000.05.15 AER  Function created
**
******************************************************************************/
GM_SampleCacheEntry * GMCache_GetCachePtrFromPtr(const GM_Mixer * pMixer,
                                                 const XPTR pSample,
                                                 OPErr * pErr)
{
    register UINT16         count;
    GM_SampleCacheEntry *   pCache;

    if (pMixer)
    {
        for (count = 0; count < MAX_SAMPLES; count++)
        {
            pCache = pMixer->sampleCaches[count];
            if (pCache)
            {
                if (pCache->pSampleData == pSample)
                {
                    *pErr = NO_ERR;
                    return pCache;
                }
            }
        }
        *pErr = RESOURCE_NOT_FOUND;
        return NULL;
    }
    *pErr = PARAM_ERR;
    return NULL;
}


/******************************************************************************
**
**  GMCache_GetSamplePtr
**
**  Returns a pointer to the sample data of a mixer cache entry (or NULL),
**      given an pointer to the cache entry
**
**  2000.05.15 AER  Function created
**
******************************************************************************/
XPTR GMCache_GetSamplePtr(const GM_SampleCacheEntry * pCache,
                          OPErr * pErr)
{
    if (pCache)
    {
        *pErr = NO_ERR;
        return pCache->pSampleData;
    }
    *pErr = PARAM_ERR;
    return NULL;
}


/******************************************************************************
**
**  PV_GetCacheIndexFromCachePtr
**
**  Returns the index of a cache entry in the mixer's array of entries
**
**  2000.05.15 AER  Function created
**
******************************************************************************/
UINT32 PV_GetCacheIndexFromCachePtr(GM_Mixer * pMixer,
                                    GM_SampleCacheEntry * pCache,
                                    OPErr * pErr)
{
    register UINT32     count;

    if (pMixer && pCache)
    {
        for (count = 0; count < MAX_SAMPLES; count++)
        {
            if (pCache == pMixer->sampleCaches[count])
            {
                *pErr = NO_ERR;
                return count;
            }
        }
        *pErr = RESOURCE_NOT_FOUND;
        return 0;
    }
    *pErr = PARAM_ERR;
    return 0;
}
