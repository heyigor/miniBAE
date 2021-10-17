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
**  "GenCache.h"
**
**  Generalized Music Synthesis package. Part of SoundMusicSys.
**
**  © Copyright 2000 Beatnik, Inc, All Rights Reserved.
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
**  This contains the header for code to  manage the mixer's sample cache.
**
**
**  Modification History:
**
**  2000.04.21 AER  File Created
**
******************************************************************************/


#ifndef G_CACHE
#define G_CACHE

#include "X_API.h"
#include "GenPriv.h"

#ifdef __cplusplus
    extern "C" {
#endif

// Prototypes (Documentation in .c file)
GM_SampleCacheEntry * GMCache_BuildSampleCacheEntry(GM_Mixer * pMixer,
                                                    const XSampleID theID,
                                                    const XBankToken bankToken,
                                                    const XPTR useThisSnd,
                                                    OPErr * pErr);
OPErr GMCache_IncrCacheEntryRef(const GM_Mixer * pMixer,
                                GM_SampleCacheEntry * pCache);
OPErr GMCache_DecrCacheEntryRef(GM_Mixer * pMixer,
                                GM_SampleCacheEntry * pCache);
OPErr GMCache_ClearSampleCache(GM_Mixer * pMixer);



XBOOL GMCache_IsIDInCache(const GM_Mixer * pMixer,
                          const XSampleID theID,
                          const XBankToken bankToken);
GM_SampleCacheEntry * GMCache_GetCachePtrFromID(const GM_Mixer * pMixer,
                                                const XSampleID theID,
                                                const XBankToken bankToken,
                                                OPErr * pErr);
GM_SampleCacheEntry * GMCache_GetCachePtrFromPtr(const GM_Mixer * pMixer,
                                                 const XPTR pSample,
                                                 OPErr * pErr);
XPTR GMCache_GetSamplePtr(const GM_SampleCacheEntry * pCache,
                          OPErr * pErr);


#ifdef __cplusplus
    }
#endif

#endif  //  G_CACHE





