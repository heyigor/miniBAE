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
/*****************************************************************************/
/*
**  BAE_API_WinOS.c
**
**  This provides platform specfic functions for Windows 95/NT. This interface
**  for BAE is for Windows 95/NT and uses the waveOut API or DirectSound to send 
**  buffer slices through the multimedia system.
**
**  Overview:
**      This works by creating another thread, aquires the sound card via
**      the WinOS waveOutOpen call. Then allocating enough buffers, preps then,
**      and build buffers depending upon how much data has been written to the
**      audio device. The thread polls the current position in the audio stream
**      that has been written and when it falls below a buffer or two, more
**      are built in that thread.
**
**  © Copyright 1995-2000 Beatnik, Inc, All Rights Reserved.
**  Written by Steve Hales
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
**  History -
**  7/14/97     Created
**  7/18/97     Added Win32 file i/o functions rather than odd unix style ones
**  9/2/97      Fixed typo that referenced samples rather than bytes
**  9/3/97      Added support to detect stereo or 16 bit support
**  11/11/97    Added support for DirectSound.
**              Added BAE_MaxDevices & BAE_SetDeviceID & BAE_GetDeviceID & BAE_GetDeviceName
**  11/20/97    Modifed DirectSound implementation to handle differences for NT DirectSound
**  12/8/97     Added BAE_GetDeviceSamplesPlayedPosition
**  12/16/97    Modified device system to be real device numbers, and allowed for the user
**              to pass back and forth device speicific info, like the window
**  2/13/98     Modified BAE_AquireAudioCard to handle different sample rates
**  3/2/98      Changed BAE_WAVEOUT_FRAMES_PER_BLOCK to 6 from 5 to support even blocks for NT
**              Put an extra waveOutReset in PV_AudioWaveOutFrameThread
**  3/3/98      Changed BAE_Is16BitSupported & BAE_IsStereoSupported to walk through
**              valid devices rather than use the WAVE_MAPPER constant
**  3/17/98     Added BAE_Is8BitSupported
**  4/27/98     Added new compile time flag USE_DIRECTSOUND to eliminate DirectSound
**              define the USE_DIRECTSOUND flag as 0 and DirectSound will be compiled out
**
**  6/5/98      Jim Nitchals RIP    1/15/62 - 6/5/98
**              I'm going to miss your irreverent humor. Your coding style and desire
**              to make things as fast as possible. Your collaboration behind this entire
**              codebase. Your absolute belief in creating the best possible relationships 
**              from honesty and integrity. Your ability to enjoy conversation. Your business 
**              savvy in understanding the big picture. Your gentleness. Your willingness 
**              to understand someone else's way of thinking. Your debates on the latest 
**              political issues. Your generosity. Your great mimicking of cartoon voices. 
**              Your friendship. - Steve Hales
**
**  8/12/98     Fixed a compiler warning with BAE_WriteFile & BAE_ReadFile & PV_AudioWaveOutFrameThread
**
**  JAVASOFT
**  04.16.98    $$kk: NOTE: *must* define USE_DIRECTSOUND 0 in makefile rather than defining NO_DIRECT_SOUND 
**              def to javasound win32 makefile.to def out direct sound stuff.  we *cannot* link with dsound.dll 
**              or we get an unsatisfied link error on machines that don't have in on the system (#4110266).
**
**  05.06.98:   $$kk: made g_lastPos global and updated in BAE_AquireAudioCard to deal with
**              bug #4135246.
**  9/3/98      Renamed lastPos global to g_lastPos. Changed BAE_Is8BitSupported to actaully
**              test the audio hardware for 8 bit support.
**  9/13/98     Fixed a oversight in PV_IsNT that never cached the result
**  12/17/98    Added BAE_GetHardwareBalance & BAE_SetHardwareBalance
**  3/5/99      Changed context to threadContext to reflect what is really is used
**              in the system.
**  3/19/99     Put in some more error checking for files.
**  5/28/99     Implemented a third type of DirectSound support. Primary buffer, and 
**              secondary buffer.
**              Changed BAE_BlockMove to use memmove for overlapping memory support.
**  6/4/99      MOE:  Changed the DirectSound code to use the COM object APIs
**              rather than DirectSoundCreate().  This means that BAE can now
**              reliably figure out whether DirectSound is available.  If
**              it isn't, WaveOut will be used.  dsound.dll will no longer
**              be loaded at BAE's load-time.
**              BUILD NOTE: BAE must now be linked with ole32.lib, instead of dsound.lib.
**  6/4/99      MOE:  Created USE_COM_DIRECTSOUND compiler flag, which
**              determines whether to use the new DirectSound code.
**  6/4/99      MOE:  Created BAE_DIRECTSOUND_DEFAULT macro, which determines
**              what flavor of DirectSound buffers to use, primary or secondary.
**  6/7/99      MOE:  Changed BAE_ReleaseAudioCard() to NOT loop on waveOutClose()
**  6/8/99      Added DirectSound debugging tools
**  6/10/99     Changed PV_DirectSoundRelease & PV_DirectSoundCreate to be static defined
**  6/11/99     Changed BAE_BlockMove to use the Win32 API MoveMemory
**  7/7/99      Enabled DSBCAPS_GLOBALFOCUS on all buffers
**  7/12/99     Fixed spelling in BAE_WaitMicroseconds
**              Now when ever you select the BAE_DIRECTSOUND_SECONDARY_BUFFER for DirectSound,
**              I first try to allocate a primary buffer to force the DS mixer to be set to
**              something real,then I drop back to the secondary buffer.
**  7/14/99     Removed the X_Assert stuff because it included X_API.h not everyone has
**              access to that file from here.
**  7/21/99     Changed PV_SetupDirectSound & PV_ReleaseDirectSound to completely
**              allocate and deallocate the COM DirectSound object. Previous versions
**              still held onto the DirectSound system and prevented others from accessing
**              it. Also reworked the callers of these two functions to call them directly
**              all the time, and removed the cached results of the max number of devices
**              so we can always check our sound enviroment to handle changes on the fly.
**  8/2/99      MOE: now includes "BAE_API.h" instead of "BAE_API.h"
**  8/18/99     Added a pref change feature that reads from a file to control the device
**              to play on, and timing information
**  8/26/99     MSD: Fixed zero termination problem in BAE_GetDeviceName()
**              Fixed issues of negative device ID's in BAE_GetDeviceName() and BAE_SetDeviceID()
**  8/28/99     Changed BAE_Microseconds to use a higher quality timer API, if available.
**              Changed BAE_AquireAudioCard & PV_AudioWaveOutFrameThread to use the global
**              sleep variable; g_audioPeriodSleepTime
**  8/28/99     Added BAEWinOSParameters, and changed BAE_SetDeviceID & BAE_GetDeviceID
**              to read from that structure for the Windows platform
**  11/4/99     Muaaaahaaaaahaaaaaa Finally I get my hands on the engine!!
**              Switched the logic of BAE_Setup around and added a check for
**              the version of DirectX... Now, we only use DSound if we find
**              version 5 or higher (e.g., skip version 4) (AER)
**  12/16/99    Changed BAE_FileOpenForReadWrite share permissions.
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  2/5/2000    Fixed compiler warning in PV_ReadFromPrefs
**  2/22/2000   Removed the test for NT 4.0 running DirectSound to set the
**              latancy. Now if we're running DirectSound we assume good
**              latancy. We never run DirectSound on NT 4.0. This solves a
**              problem for Windows 2000 (NT 5.0)
**  3/23/2000   Changed PV_MonitorDirectSoundPosition to check the bDone flag
**              before waiting for the render thread. Changed PV_CleanupDirectSound
**              to set the bDone flag to allow the PV_MonitorDirectSoundPosition thread
**              to die naturally.
**  4/24/2000   msd: added NULL checks in BAE_GetDeviceSamplesPlayedPosition()
**  8/29/2000   sh  Changed PV_AudioWaveOutFrameThread to allocate waveHeader
**                  instead of staticly defining its size on the local stack.
**                  Changed g_audioBufferBlock to allocate dynamicly rather
**                  than a fixed size.
**  9/3/2000    sh  Changed PV_DirectSoundCreate to call GetDXVersion to verify
**                  the version number of DirectSound.
**                  Changed BAE_MaxDevices & BAE_Setup to call PV_DirectSoundCreate 
**                  correctly.
**  2001.01.30 AER  Removed the GetDXVersion code and replaced with COM-based
**                      function, PV_IsDX5OrHigher().
*/
/*****************************************************************************/

#define USE_PREFS   1

#ifndef USE_DIRECTSOUND
    #define USE_DIRECTSOUND             1
    #ifdef _DEBUG
        #define DEBUG_DIRECTSOUND
    #endif
#endif

#if USE_DIRECTSOUND
#define USE_COM_DIRECTSOUND     1   // set to 0 if linking with dsound.lib
#else
#define USE_COM_DIRECTSOUND     0
#endif

#if USE_COM_DIRECTSOUND
//#define   INITGUID    1   // affects <compobjs.h> (see comment in <basetyps.h>)
// Not used. Just include the library dxguid.lib to get this to work.
#endif

#ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#pragma warning(disable: 4100)  // L4: unreferenced formal parameter
#pragma warning(disable: 4244)  // L3/4: conversion from <sometype> to <another type>, possible loss of data
// windows.h mucks with the C4201 warning, so include it first and 
//  then set the pragma before mmsystem.h
#include <windows.h>
#pragma warning(disable: 4201)  // L4: nonstandard extension used : nameless struct/union (mmsystem.h)
#include <mmsystem.h>

#if USE_DIRECTSOUND
// objbase.h mucks with the C4201 warning, so include it explicitly and 
//  then set the pragma again
#pragma warning(disable: 4115)  // L1/3: name type definition in parentheses (rpcasync.h included by objbase.h)
#include <objbase.h>
#pragma warning(default: 4115)  // L1/3: name type definition in parentheses (rpcasync.h)
#pragma warning(disable: 4201)  // L4: nonstandard extension used : nameless struct/union (directx headers)
#include <dsound.h>
#endif // USE_DIRECTSOUND

#include <assert.h>
#include <stdio.h>

#include "BAE_API.h"

enum
{
    BAE_NO_DEVICE = -1,
    BAE_WAVEOUT = 0,
    BAE_DIRECTSOUND_PRIMARY_BUFFER,
    BAE_DIRECTSOUND_SECONDARY_BUFFER
};
typedef long BAEDeviceID;

#define BAE_DIRECTSOUND_DEFAULT     BAE_DIRECTSOUND_SECONDARY_BUFFER
//#define   BAE_DIRECTSOUND_DEFAULT     BAE_DIRECTSOUND_PRIMARY_BUFFER

static BAEDeviceID          g_lastDeviceID = BAE_NO_DEVICE;
#if USE_DIRECTSOUND
#if USE_COM_DIRECTSOUND
static int                  g_maxDeviceCount = 0;   // calculate later
#else
static int                  g_maxDeviceCount = BAE_DIRECTSOUND_SECONDARY_BUFFER + 1;
#endif
static BAEDeviceID          g_currentDeviceID = BAE_DIRECTSOUND_DEFAULT;
#else
static int                  g_maxDeviceCount = BAE_WAVEOUT + 1;
static BAEDeviceID          g_currentDeviceID = BAE_WAVEOUT;
#endif

static unsigned long        g_memory_buoy = 0;          // amount of memory allocated at this moment
static unsigned long        g_memory_buoy_max = 0;

static short int            g_balance = 0;              // balance scale -256 to 256 (left to right)
static short int            g_unscaled_volume = 256;    // hardware volume in BAE scale

#define BAE_WAVEOUT_FRAMES_PER_BLOCK    6               // how much ms latancy can we handle (in ms)
#define BAE_WAVEOUT_SOUND_PERIOD        11              // sleep period between position checks (in ms)

static void                 **g_audioBufferBlock;
static long                 g_audioByteBufferSize;          // size of audio buffers in bytes

static HWAVEOUT             g_waveDevice = 0;
static long                 g_shutDownDoubleBuffer;
static long                 g_activeDoubleBuffer;

// $$kk: 05.06.98: made lastPos a global variable
static long                 g_lastPos;

// Format of output buffer
static WAVEFORMATEX         g_waveFormat;

// How many audio frames to generate at one time 
#define BAE_DIRECTSOUND_FRAMES_PER_BLOCK    8
#define BAE_DIRECTSOUND_FRAMES_PER_BLOCK_NT 26      //24//16    //NOTE: 24 seems to work better with
                                                    //      secondary buffers
// How long in milliseconds to sleep between polling DirectSound play position?
#define BAE_DIRECTSOUND_SOUND_PERIOD        6
#define BAE_DIRECTSOUND_SOUND_PERIOD_NT     9

 // number of samples per audio frame to generate
long                        g_audioFramesToGenerate;

// How many audio frames to generate at one time 
static unsigned int         g_synthFramesPerBlock;  // setup upon runtime
static unsigned int         g_audioPeriodSleepTime; // setup upon runtime

static unsigned int         g_userShadowSynthFramesPerBlock = 0;    // setup by user
static unsigned int         g_userShadowAudioPeriodSleepTime = 0;   // setup by user


#if USE_DIRECTSOUND

#ifdef DEBUG_DIRECTSOUND
    static void PV_DisplayDirectSoundError(HRESULT error);
#endif

#if USE_COM_DIRECTSOUND
static  BOOL                g_comInitialized = FALSE;
#endif

// Structure to describe output sound buffer topology
struct DXSOUNDPOSDATA
{
    unsigned long   uiBufferSize;           // Size of DX sound play buffer
    unsigned long   uiBufferBlocks;         // Number of sub blocks in play buffer
    unsigned long   uiWriteBlock;           // Current block to fill with audio samples
    unsigned long   uiDestPos;              // current destination buffer offset
    unsigned long   uiBlockSize;            // size of data block to copy
    unsigned long   uiSynthFrameBytes;      // How many bytes per audio frame
    unsigned long   uiSynthFramesPerBlock;  // How many frames to audio process at once
    BOOL            bWaveEnd;               // Signal to stop processing audio
    BOOL            bDone;                  // Has last generated frame been played
};
typedef struct DXSOUNDPOSDATA   DXSOUNDPOSDATA;

// Output buffer sub block info
struct SOUNDPOSBLOCK
{
    unsigned long   dwStart;                // Offset of block start
    unsigned long   dwEnd;                  // Offset to block end
    BOOL            bEndingBlock;           // Does this block contain final samples?
};
typedef struct SOUNDPOSBLOCK    SOUNDPOSBLOCK;

static LPDIRECTSOUND        g_pDirectSoundObject = NULL;
static LPDIRECTSOUNDBUFFER  g_pDirectSoundBuffer = NULL;
static LPDIRECTSOUNDBUFFER  g_pDirectSoundPrimaryBuffer = NULL;

// Thread used to poll DirectSound play position
static HANDLE               g_hthreadSoundMon = NULL;

// Window handle of Client application
static HWND                 g_directSoundWindow = NULL;
// How is the output buffer divided for purposes of monitoring the play position
static long                 g_nBufferSegments = 2;

 // Output sound buffer record
static DXSOUNDPOSDATA       g_SPD;
 // Are we currently generating audio frames?
static BOOL                 g_bProcessingFrame = FALSE;
// Events to signal the start and end of a batch audio frame processing
static HANDLE               g_heventProcessFrameStart = NULL;
static HANDLE               g_heventProcessFrameFinish = NULL;

#endif  // USE_DIRECTSOUND


// This file contains API's that need to be defined in order to get BAE (IgorAudio)
// to link and compile.


#if USE_DIRECTSOUND

static void PV_DirectSoundRelease(void)
{
#if USE_COM_DIRECTSOUND
    if (g_pDirectSoundObject)
    {
        IDirectSound_Release(g_pDirectSoundObject);
        g_pDirectSoundObject = NULL;
    }
    if (g_comInitialized)
    {
        CoUninitialize();
        g_comInitialized = FALSE;
    }
#endif
}

static BOOL PV_IsDX5OrHigher()
{
    BOOL                    isOK                = FALSE;
    HRESULT                 result;

    LPDIRECTDRAW            pDDraw              = NULL;
    DDSURFACEDESC           desc;
    LPDIRECTDRAWSURFACE     pSurf               = NULL;
    LPDIRECTDRAWSURFACE3    pSurf3              = NULL;

    result = CoInitialize(NULL);
    if (FAILED(result))
        return FALSE;

    result = CoCreateInstance(&CLSID_DirectDraw,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              &IID_IDirectDraw,
                              &pDDraw);
    if (FAILED(result))
        goto theEnd;

    //  BoundsChecker doesn't like this, but it's what is called for by MS.
    //      NULL means 'active display driver'... Seach not for this, but
    //      IDirectDraw2_Initialize in MSDN or online...
    result = IDirectDraw_Initialize(pDDraw, NULL);
    if (FAILED(result))
        goto theEnd;

    ZeroMemory(&desc, sizeof(desc));
    desc.dwSize         = sizeof(desc);
    desc.dwFlags        = DDSD_CAPS;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    result = IDirectDraw_SetCooperativeLevel(pDDraw, NULL, DDSCL_NORMAL);
    if (FAILED(result))
        goto theEnd;

    result = IDirectDraw_CreateSurface(pDDraw, &desc, &pSurf, NULL);
    if (FAILED(result))
        goto theEnd;
    
    result = IDirectDrawSurface_QueryInterface(pSurf,
                                               &IID_IDirectDrawSurface3,
                                               (LPVOID *) &pSurf3);
    if (FAILED(result))
        goto theEnd;
    else
        isOK = TRUE;
    
    
theEnd:
    if (pSurf3)
        result = IDirectDrawSurface3_Release(pSurf3);
    if (pSurf)
        result = IDirectDrawSurface_Release(pSurf);
    if (pDDraw)
        result = IDirectDraw_Release(pDDraw);
    CoUninitialize();

    return isOK;
}

static BOOL PV_DirectSoundCreate(void)
{
#if USE_COM_DIRECTSOUND
    // Check to see if we have a DriectX version of 5.0 or higher...
    if (PV_IsDX5OrHigher())
    {
        // go ahead and test and dynamicly change the max number of devices
        // because our audio enviroment may have changed.
        if (g_comInitialized == FALSE)
        {
            CoInitialize(NULL);
            g_comInitialized = TRUE;
            if (!FAILED(CoCreateInstance(&CLSID_DirectSound, NULL,
                                            CLSCTX_INPROC_SERVER, &IID_IDirectSound,
                                            &g_pDirectSoundObject)))
            {
                HRESULT     result;

                result = IDirectSound_Initialize(g_pDirectSoundObject, NULL);
                if ( (result == DS_OK) || (result == DSERR_ALREADYINITIALIZED))
                {
                    assert(BAE_DIRECTSOUND_SECONDARY_BUFFER > BAE_DIRECTSOUND_PRIMARY_BUFFER);
                    assert(BAE_DIRECTSOUND_PRIMARY_BUFFER > BAE_WAVEOUT);
                    g_maxDeviceCount = BAE_DIRECTSOUND_SECONDARY_BUFFER + 1;
                    return TRUE;
                }
    #ifdef DEBUG_DIRECTSOUND
                else
                {
                    PV_DisplayDirectSoundError(result);
                }
    #endif
            }
            else
            {
                g_pDirectSoundObject = NULL;        // just to be sure
            }
            PV_DirectSoundRelease();
        }
        else
        {
            // already setup
            return TRUE;
        }
    }
    g_maxDeviceCount = BAE_WAVEOUT + 1;         // direct sound not available;
    return FALSE;
#else
    assert(g_maxDeviceCount == BAE_DIRECTSOUND_SECONDARY_BUFFER + 1);
    return TRUE;    // just assume that direct sound available
#endif
}

#endif  // USE_DIRECTSOUND

// **** System setup and cleanup functions
// Setup function. Called before memory allocation, or anything serious. Can be used to 
// load a DLL, library, etc.
// return 0 for ok, or -1 for failure
int BAE_Setup(void)
{
#if USE_COM_DIRECTSOUND
    if (PV_DirectSoundCreate())
    {
        g_currentDeviceID = BAE_DIRECTSOUND_DEFAULT;
    }
    else
    {
        g_currentDeviceID = BAE_WAVEOUT;
    }
#endif
    g_lastDeviceID = g_currentDeviceID;
    return 0;
}

// Cleanup function. Called after all memory and the last buffers are deallocated, and
// audio hardware is released. Can be used to unload a DLL, library, etc.
// return 0 for ok, or -1 for failure
int BAE_Cleanup(void)
{
#if USE_COM_DIRECTSOUND
    PV_DirectSoundRelease();
#endif
    return 0;
}

// **** Memory management
// allocate a block of locked, zeroed memory. Return a pointer
void * BAE_Allocate(unsigned long size)
{
    void    *data;

    data = 0;
    if (size)
    {
        // the GHND flag includes the GMEM_ZEROINIT flag
        data = (void *)GlobalAllocPtr(GHND, size);
    }
    if (data)
    {
        g_memory_buoy += size;
        if (g_memory_buoy > g_memory_buoy_max)
        {
            g_memory_buoy_max = g_memory_buoy;
        }
    }
    return data;
}

// dispose of memory allocated with BAE_Allocate
void BAE_Deallocate(void * memoryBlock)
{
    if (memoryBlock)
    {
        g_memory_buoy -= BAE_SizeOfPointer(memoryBlock);
        GlobalFreePtr(memoryBlock);
    }
}

// return memory used
unsigned long BAE_GetSizeOfMemoryUsed(void)
{
    return g_memory_buoy;
}

// return max memory used
unsigned long BAE_GetMaxSizeOfMemoryUsed(void)
{
    return g_memory_buoy_max;
};

// Given a memory pointer and a size, validate of memory pointer is a valid memory address
// with at least size bytes of data avaiable from the pointer.
// This is used to determine if a memory pointer and size can be accessed without 
// causing a memory protection
// fault.
// return 0 for valid, or 1 for bad pointer, or 2 for not supported. 
int BAE_IsBadReadPointer(void *memoryBlock, unsigned long size)
{
    return (IsBadReadPtr(memoryBlock, size)) ? 1 : 0;
}

// this will return the size of the memory pointer allocated with BAE_Allocate. Return
// 0 if you don't support this feature
unsigned long BAE_SizeOfPointer(void * memoryBlock)
{
    unsigned long   size;
    HANDLE          hData;
            
    size = 0;
    if (memoryBlock)
    {
        hData = GlobalPtrHandle(memoryBlock);
        if (hData)
        {
            size = GlobalSize(hData);
        }
    }
    return size;
}

// block move memory. This is basicly memmove, but its exposed to take advantage of
// special block move speed ups, various hardware has available.
// NOTE:    Must use a function like memmove that insures a valid copy in the case
//          of overlapping memory blocks.
void BAE_BlockMove(void * source, void * dest, unsigned long size)
{
    if (source && dest && size)
    {
        MoveMemory(dest, source, size);
    }
}

// **** Audio Card modifiers
// Return 1 if stereo hardware is supported, otherwise 0.
int BAE_IsStereoSupported(void)
{
    MMRESULT    theErr;
    int         status;
    WAVEOUTCAPS info;
    UINT        count, maxDevs;
    static int  cachedStatus = -1;

    if (cachedStatus == -1)
    {
        status = 0;
        maxDevs = waveOutGetNumDevs();
        for (count = 0; count < maxDevs; count++)
        {
            theErr = waveOutGetDevCaps(count, &info, sizeof(WAVEOUTCAPS));
            if (theErr == MMSYSERR_NOERROR)
            {
                if (info.wChannels > 1)
                {
                    status = 1; // stereo supported
                }
            }
        }
        cachedStatus = status;
    }
    return cachedStatus;
}

// Return 1, if sound hardware support 16 bit output, otherwise 0.
int BAE_Is16BitSupported(void)
{
    MMRESULT    theErr;
    int         status, fcount;
    UINT        count, maxDevs;
    WAVEOUTCAPS info;
    static int  caps[] = {  WAVE_FORMAT_1M16, WAVE_FORMAT_1S16, WAVE_FORMAT_2M16,
                            WAVE_FORMAT_2S16, WAVE_FORMAT_4M16, WAVE_FORMAT_4S16};
    static int  cachedStatus = -1;

    if (cachedStatus == -1)
    {
        status = 0;
        maxDevs = waveOutGetNumDevs();
        for (count = 0; count < maxDevs; count++)
        {
            theErr = waveOutGetDevCaps(count, &info, sizeof(WAVEOUTCAPS));
            if (theErr == MMSYSERR_NOERROR)
            {
                for (fcount = 0; fcount < 6; fcount++)
                {
                    if (info.dwFormats & caps[fcount])
                    {
                        status = 1;     // 16 bit supported
                        break;
                    }
                }
            }
        }
        cachedStatus = status;
    }
    return cachedStatus;
}

// Return 1, if sound hardware support 8 bit output, otherwise 0.
int BAE_Is8BitSupported(void)
{
    MMRESULT    theErr;
    int         status, fcount;
    UINT        count, maxDevs;
    WAVEOUTCAPS info;
    static int  caps[] = {  WAVE_FORMAT_1M08, WAVE_FORMAT_1S08, WAVE_FORMAT_2M08,
                            WAVE_FORMAT_2S08, WAVE_FORMAT_4M08, WAVE_FORMAT_4S08};
    static int  cachedStatus = -1;

    if (cachedStatus == -1)
    {
        status = 0;
        maxDevs = waveOutGetNumDevs();
        for (count = 0; count < maxDevs; count++)
        {
            theErr = waveOutGetDevCaps(count, &info, sizeof(WAVEOUTCAPS));
            if (theErr == MMSYSERR_NOERROR)
            {
                for (fcount = 0; fcount < 6; fcount++)
                {
                    if (info.dwFormats & caps[fcount])
                    {
                        status = 1;     // 8 bit supported
                        break;
                    }
                }
            }
        }
        cachedStatus = status;
    }
    return cachedStatus;
}

// returned balance is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
short int BAE_GetHardwareBalance(void)
{
    return g_balance;
}

// 'balance' is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
void BAE_SetHardwareBalance(short int balance)
{
    // pin balance to box
    if (balance > 256)
    {
        balance = 256;
    }
    if (balance < -256)
    {
        balance = -256;
    }
    g_balance = balance;
    BAE_SetHardwareVolume(g_unscaled_volume);
}

// returned volume is in the range of 0 to 256
short int BAE_GetHardwareVolume(void)
{
// Note this might be an implementation issue. One way (A), just returns the passed in volume the
// BAE user has set, the other way (B) always looks at the hardware to adjust for outside changes.
// B has the disadvantage of scaling problems with repeated calls to get and set.
#if 1
// A
    return g_unscaled_volume;
#else
// B
    MMRESULT    theErr;
    DWORD       volume;

    volume = 256;
    theErr = waveOutGetVolume((HWAVEOUT)WAVE_MAPPER, &volume);
    if (theErr == MMSYSERR_NOERROR)
    {
        volume = (volume & 0xFFFF) / 256;   // scale down to 0 to 256
    }
    g_unscaled_volume = volume;
    return (short int)volume;
#endif
}

// newVolume is in the range of 0 to 256
void BAE_SetHardwareVolume(short int newVolume)
{
    MMRESULT        theErr;
    unsigned long   volume;
    unsigned long   lbm, rbm;

    // pin volume
    if (newVolume > 256)
    {
        newVolume = 256;
    }
    if (newVolume < 0)
    {
        newVolume = 0;
    }
    g_unscaled_volume = newVolume;

    // calculate balance multipliers
    if (g_balance > 0)
    {
        lbm = 256 - g_balance;
        rbm = 256;
    }
    else
    {
        lbm = 256;
        rbm = 256 + g_balance;
    }
    rbm = (((unsigned long)newVolume * rbm) * 65535) / 65536;   // scale down to 0 to 65535
    lbm = (((unsigned long)newVolume * lbm) * 65535) / 65536;   // scale down to 0 to 65535

    volume = (rbm << 16L) | lbm;
    theErr = waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, volume);
}

// **** Timing services
// return microseconds
unsigned long BAE_Microseconds(void)
{
    static unsigned long    starttick = 0;
    static char             firstTime = TRUE;
    static char             QPClockSupport = FALSE;
    static unsigned long    clockpusu = 0;  // clocks per microsecond
    unsigned long           time;
    LARGE_INTEGER           p;

    if (firstTime)
    {
        if (QueryPerformanceFrequency(&p))
        {
            QPClockSupport = TRUE;
            clockpusu = (unsigned long)(p.QuadPart / 1000000L);
        }
        firstTime = FALSE;
    }
    if (QPClockSupport)
    {
        QueryPerformanceCounter(&p);
        time = (unsigned long)(p.QuadPart / clockpusu);
    }
    else
    {
        time = timeGetTime() * 1000L;
    }

    if (starttick == 0)
    {
        starttick = time;
    }
    return (time - starttick);
}

// wait or sleep this thread for this many microseconds
// CLS??: If this function is called from within the frame thread and
// JAVA_THREAD is non-zero, we'll probably crash.
void BAE_WaitMicroseconds(unsigned long waitAmount)
{
    unsigned long   ticks;

    ticks = BAE_Microseconds() + waitAmount;
    while (BAE_Microseconds() < ticks) 
    {
        Sleep(0);   // Give up the rest of this time slice to other threads
    }
}

// If no thread support, this will be called during idle times. Used for host
// rendering without threads.
void BAE_Idle(void *userContext)
{
    userContext;
}

// **** File support
// Create a file, delete orignal if duplicate file name.
// Return -1 if error

// Given the fileNameSource that comes from the call BAE_FileXXXX, copy the name
// in the native format to the pointer passed fileNameDest.
void BAE_CopyFileNameNative(void *fileNameSource, void *fileNameDest)
{
    char *dest;
    char *src;

    if (fileNameSource && fileNameDest)
    {
        dest = (char *)fileNameDest;
        src = (char *)fileNameSource;
        if (src == NULL)
        {
            src = "";
        }
        if (dest)
        {
            while (*src)
            {
                *dest++ = *src++;
            }
            *dest = 0;
        }
    }
}

long BAE_FileCreate(void *fileName)
{
#if USE_WIN32_FILE_IO == 0
    int     file;

    file = _creat((char *)fileName, _S_IREAD | _S_IWRITE | _O_RDWR | _O_TRUNC);
    if (file != -1)
    {
        _close(file);
    }
    return (file != -1) ? 0 : -1;
#else
    HANDLE  file;

    file = CreateFile((LPCTSTR)fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, 
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
                                    NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        DWORD   lastErr = GetLastError();
        return -1;
    }
    CloseHandle(file);
    return file ? 0 : -1;
#endif
}

long BAE_FileDelete(void *fileName)
{
    if (fileName)
    {
        if (DeleteFile((LPCTSTR)fileName))
        {
            return 0;
        }
    }
    return -1;
}


// Open a file
// Return -1 if error, otherwise file handle
long BAE_FileOpenForRead(void *fileName)
{
    if (fileName)
    {
#if USE_WIN32_FILE_IO == 0
        return _open((char *)fileName, _O_RDONLY | _O_BINARY);
#else
        HANDLE  file;

        file = CreateFile((LPCTSTR)fileName, GENERIC_READ, 
                                    //FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    FILE_SHARE_READ,
                                    NULL, OPEN_EXISTING,
                                    FILE_ATTRIBUTE_READONLY | 
                                    FILE_FLAG_RANDOM_ACCESS,
                                    NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            DWORD   lastErr = GetLastError();
            return -1;
        }
        return (long)file;
#endif
    }
    return -1;
}

long BAE_FileOpenForWrite(void *fileName)
{
    if (fileName)
    {
#if USE_WIN32_FILE_IO == 0
        return _open((char *)fileName, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY);
#else
        HANDLE  file;

        file = CreateFile((LPCTSTR)fileName, GENERIC_WRITE, 0, NULL, CREATE_NEW | TRUNCATE_EXISTING,
                                    FILE_FLAG_RANDOM_ACCESS,
                                    NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            DWORD   lastErr = GetLastError();
            return -1;
        }
        return (long)file;

#endif
    }
    return -1;
}

long BAE_FileOpenForReadWrite(void *fileName)
{
    if (fileName)
    {
#if USE_WIN32_FILE_IO == 0
        return _open((char *)fileName, _O_RDWR | _O_BINARY);
#else
        HANDLE  file;

        file = CreateFile((LPCTSTR)fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
                                    OPEN_EXISTING,
                                    FILE_FLAG_RANDOM_ACCESS,
                                    NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            DWORD   lastErr = GetLastError();
            return -1;
        }
        return (long)file;

#endif
    }
    return -1;
}

// Close a file
void BAE_FileClose(long fileReference)
{
#if USE_WIN32_FILE_IO == 0
    _close(fileReference);
#else
    CloseHandle((HANDLE)fileReference);
#endif
}

// Read a block of memory from a file.
// Return -1 if error, otherwise length of data read.
long BAE_ReadFile(long fileReference, void *pBuffer, long bufferLength)
{
    if (pBuffer && bufferLength)
    {
#if USE_WIN32_FILE_IO == 0
        return _read(fileReference, (char *)pBuffer, bufferLength);
#else
        {
            DWORD   readFromBuffer;
            return ReadFile((HANDLE)fileReference, (LPVOID)pBuffer, 
                                        bufferLength, &readFromBuffer,
                                        NULL) ? (long)readFromBuffer : -1;
        }
#endif
    }
    return -1;
}

// Write a block of memory from a file
// Return -1 if error, otherwise length of data written.
long BAE_WriteFile(long fileReference, void *pBuffer, long bufferLength)
{
    if (pBuffer && bufferLength)
    {
#if USE_WIN32_FILE_IO == 0
        return _write(fileReference, (char *)pBuffer, bufferLength);
#else
        {
            DWORD   writtenFromBuffer;
            return WriteFile((HANDLE)fileReference, (LPVOID)pBuffer, 
                                        bufferLength, &writtenFromBuffer,
                                        NULL) ? (long)writtenFromBuffer : -1;
        }
#endif
    }
    return -1;
}

// set file position in absolute file byte position
// Return -1 if error, otherwise 0.
long BAE_SetFilePosition(long fileReference, unsigned long filePosition)
{
#if USE_WIN32_FILE_IO == 0
    return (_lseek(fileReference, filePosition, SEEK_SET) == -1) ? -1 : 0;
#else
    return (
            (SetFilePointer((HANDLE)fileReference, filePosition, NULL, FILE_BEGIN) == 0xFFFFFFFFL)
             ? -1 : 0);
#endif
}

// get file position in absolute file bytes
unsigned long BAE_GetFilePosition(long fileReference)
{
#if USE_WIN32_FILE_IO == 0
    return _lseek(fileReference, 0, SEEK_CUR);
#else
    return SetFilePointer((HANDLE)fileReference, 0, NULL, FILE_CURRENT);
#endif
}

// get length of file
unsigned long BAE_GetFileLength(long fileReference)
{
    unsigned long pos;

#if USE_WIN32_FILE_IO == 0
    pos = _lseek(fileReference, 0, SEEK_END);
    _lseek(fileReference, 0, SEEK_SET);
#else
    pos = GetFileSize((HANDLE)fileReference, NULL);
    if (pos == 0xFFFFFFFFL)
    {
        pos = 0;
    }
#endif
    return pos;
}

// set the length of a file. Return 0, if ok, or -1 for error
int BAE_SetFileLength(long fileReference, unsigned long newSize)
{
#if USE_WIN32_FILE_IO == 0
    return _chsize(fileReference, newSize);
#else
    int error;
    
    error = -1;
    if (BAE_SetFilePosition(fileReference, newSize) == 0)
    {
        error = SetEndOfFile((HANDLE)fileReference) ? 0 : -1;
    }
    return error;
#endif
}

static int PV_IsNT(void)
{
    static int  firstTime = TRUE;
    static int  underNT;

    if (firstTime)
    {
        underNT = (GetVersion() < 0x80000000) ? TRUE : FALSE;
        firstTime = FALSE;
    }
    return underNT;
}

// When using DirectSound, we need a window handle (!). So this function looks
// around and waits around until it can find an active window handle.
// This has to be done for Windows only, because the message system needs
// a window handle to process messages. If we can't find one, bail with an
// error.
#if USE_DIRECTSOUND
static HWND PV_GetMostActiveWindow(void)
{
    short int   count;
    HWND        theWindow;

    theWindow = NULL;
    // we're going to count to 100, and wait for a window to become active by
    // sleeping in this thread then checking to see if anything has changed.
    for (count = 0; count < 100; count++)
    {
        theWindow = GetForegroundWindow();
        if (theWindow == NULL)
        {
            theWindow = GetActiveWindow();
        }
        if (theWindow == NULL)
        {
            theWindow = GetFocus();
        }
        if (theWindow)
        {   // found something, so break out of here
            break;
        }
        Sleep(10);  // sleep 10 milliseconds
    }
    return theWindow;
}
#endif  // USE_DIRECTSOUND

static void PV_AudioWaveOutFrameThread(void* threadContext)
{
    WAVEHDR         *waveHeader;
    long            waveHeaderCount;
    MMTIME          audioStatus;

    // $$kk: 08.12.98 merge: changed g_lastPos to global variable
    //$$kk: 05.06.98: i am making g_lastPos global to fix P1 bug #4135246, in
    // which the jdk samples sometimes do not get audio.  depending
    // on the order of events when the audio device is opened and
    // closed, the frame thread may get stuck in the wait().
    long            count, currentPos, error;

    char            *pFillBuffer;

    waveHeader = (WAVEHDR *)BAE_Allocate(sizeof(WAVEHDR) * g_synthFramesPerBlock);
    if (waveHeader)
    {
        waveOutReset(g_waveDevice);     // stop all audio before preparing headers

        memset(&audioStatus, 0, (long)sizeof(MMTIME));
        audioStatus.wType = TIME_BYTES; // get byte position

        error = waveOutGetPosition(g_waveDevice, &audioStatus, sizeof(MMTIME));
        g_lastPos = audioStatus.u.cb - (g_audioByteBufferSize * g_synthFramesPerBlock * 2);

        // now write out all of the data built
        for (count = 0; count < (long)g_synthFramesPerBlock; count++)
        {
            waveHeader[count].lpData = (char *)g_audioBufferBlock[count];
            waveHeader[count].dwBufferLength = g_audioByteBufferSize * g_synthFramesPerBlock;
            waveHeader[count].dwFlags       = 0;
            waveHeader[count].dwLoops       = 0;
            error = waveOutPrepareHeader(g_waveDevice, &waveHeader[count], (long)sizeof(WAVEHDR));
        }

        waveHeaderCount = 0;
        while ( (g_activeDoubleBuffer) && (g_shutDownDoubleBuffer == FALSE) )
        {
            pFillBuffer = (char *)g_audioBufferBlock[waveHeaderCount];
            for (count = 0; count < (long)g_synthFramesPerBlock; count++)
            {
                // Generate one frame audio
                BAE_BuildMixerSlice(threadContext, pFillBuffer, g_audioByteBufferSize,
                                                            g_audioFramesToGenerate);
                pFillBuffer += g_audioByteBufferSize;

                if (g_shutDownDoubleBuffer)
                {
                        break;  // time to quit
                }
            }

            if (g_shutDownDoubleBuffer == FALSE)
            {
                if (PV_IsNT() == FALSE)     // under Win95?
                {
                    waveHeader[waveHeaderCount].dwFlags &= WHDR_DONE;   // must do, or buffers stop on Win95
                }
                error = waveOutWrite(g_waveDevice, &waveHeader[waveHeaderCount], (long)sizeof(WAVEHDR));

                // I am incrementing g_lastPos right after the write operation
                // to lessen the chance of a device open/close messing us up in the wait loop.
                g_lastPos += (g_audioByteBufferSize * g_synthFramesPerBlock);

                waveHeaderCount++;
                if (waveHeaderCount == (long)g_synthFramesPerBlock)
                {
                    waveHeaderCount = 0;
                }
                error = waveOutGetPosition(g_waveDevice, &audioStatus, sizeof(MMTIME));
                currentPos = audioStatus.u.cb;
                while ((currentPos < g_lastPos) && (g_shutDownDoubleBuffer == FALSE))
                {
                    BAE_SleepFrameThread(threadContext, g_audioPeriodSleepTime);        // in ms

                    error = waveOutGetPosition(g_waveDevice, &audioStatus, sizeof(MMTIME));
                    currentPos = audioStatus.u.cb;
                }
            }
        }
        waveOutReset(g_waveDevice);     // stop all audio before unpreparing headers

        for (count = 0; count < (long)g_synthFramesPerBlock; count++)
        {
            error = waveOutUnprepareHeader(g_waveDevice, &waveHeader[count], (long)sizeof(WAVEHDR));
        }
        BAE_Deallocate((void *)waveHeader);
    }
    g_activeDoubleBuffer = FALSE;
}


// Return the number of 11 ms buffer blocks that are built at one time.
int BAE_GetAudioBufferCount(void)
{
    return g_synthFramesPerBlock;
}

// Return the number of bytes used for audio buffer for output to card
long BAE_GetAudioByteBufferSize(void)
{
    return g_audioByteBufferSize;
}


#if USE_DIRECTSOUND
#ifdef DEBUG_DIRECTSOUND
static void PV_ConvertDSErrorCodeToSomethingReal(HRESULT hr, char *cString)
{
    switch (hr)
    {
        default:
            sprintf(cString, "general problem (%lx)", (long)hr);
            break;
        case DS_OK:
            break;
        case DSERR_ALLOCATED:
            strcpy(cString, "The call failed because resources (such as a priority level) were already being used by another caller.");
            break;
        case DSERR_CONTROLUNAVAIL:
            strcpy(cString, "The control (vol,pan,etc.) requested by the caller is not available.");
            break;
        case DSERR_INVALIDPARAM:
            strcpy(cString, "An invalid parameter was passed to the returning function");
            break;
        case DSERR_INVALIDCALL:
            strcpy(cString, "This call is not valid for the current state of this object");
            break;
        case DSERR_GENERIC:
            strcpy(cString, "An undetermined error occured inside the DirectSound subsystem");
            break;
        case DSERR_PRIOLEVELNEEDED:
            strcpy(cString, "The caller does not have the priority level required for the function to succeed.");
            break;
        case DSERR_OUTOFMEMORY:
            strcpy(cString, "Not enough free memory is available to complete the operation");
            break;
        case DSERR_BADFORMAT:
            strcpy(cString, "The specified WAVE format is not supported");
            break;
        case DSERR_UNSUPPORTED:
            strcpy(cString, "The function called is not supported at this time");
            break;
        case DSERR_NODRIVER:
            strcpy(cString, "No sound driver is available for use");
            break;
        case DSERR_ALREADYINITIALIZED:
            strcpy(cString, "This object is already initialized");
            break;
        case DSERR_NOAGGREGATION:
            strcpy(cString, "This object does not support aggregation");
            break;
        case DSERR_BUFFERLOST:
            strcpy(cString, "The buffer memory has been lost, and must be restored.");
            break;
        case DSERR_OTHERAPPHASPRIO:
            strcpy(cString, "Another app has a higher priority level, preventing this call from succeeding.");
            break;
        case DSERR_UNINITIALIZED:
            strcpy(cString, "This object has not been initialized");
            break;
        case DSERR_NOINTERFACE:
            strcpy(cString, "The requested COM interface is not available");
            break;
    }
}

void PV_DisplayDirectSoundError(HRESULT error)
{
    char    buffer[1024];
    
    PV_ConvertDSErrorCodeToSomethingReal(error, buffer);

    MessageBox((HWND)NULL, buffer, "DirectSound Error", MB_OK | MB_ICONWARNING);
}
#endif
#endif

// DWORD PV_AudioDirectSoundFrameThread (LPVOID lpv)
//
// This is the thread function that fills the current block of the
// output buffer.
//
// This thread will sleep until the "g_heventProcessFrameStart" event
// is set by the PV_MonitorDirectSoundPosition thread.  After the current
// block has been filled, the "g_heventProcessFrameFinish" event is
// set and the thread goes back to sleep.
//
// The call to ProcessSampleFrame is what drives the synth engine.
// Each call to this function generates "g_SPD.uiSynthFrameBytes"
// bytes of audio data.  BAE_BuildMixerSlice will be called
// g_SPD.uiSynthFramesPerBlock times to fill the current block.
//
// If the flag "g_SPD.bWaveEnd" is set then PV_AudioDirectSoundFrameThread will fill
// the current block with silence instead of calling generating
// more audio samples.
#if USE_DIRECTSOUND
static void PV_AudioDirectSoundFrameThread(void* threadContext)
{
    HRESULT         error;
    LPVOID          lpWrite;
    DWORD           dwWriteLen;
    DWORD           count;
    char            *pFillBuffer;
    unsigned long   dwPlayPos, dwWritePos;

    // get byte position
    IDirectSoundBuffer_GetCurrentPosition(g_pDirectSoundBuffer, &dwPlayPos, &dwWritePos);
    g_lastPos = dwPlayPos - (g_audioByteBufferSize * g_synthFramesPerBlock * 2);

    while ( (g_activeDoubleBuffer) && (g_shutDownDoubleBuffer == FALSE) )
    {
        g_bProcessingFrame = FALSE;
        // Wait for someone to wake us up.
        WaitForSingleObject(g_heventProcessFrameStart, INFINITE);
        g_bProcessingFrame = TRUE;

        // Lock the current block of the DirectSound buffer.
        error = IDirectSoundBuffer_Lock(g_pDirectSoundBuffer,
                g_SPD.uiDestPos, g_SPD.uiBlockSize,
                &lpWrite, &dwWriteLen, NULL, NULL, 0);

        if (error == DS_OK)
        {
            // If we're to generate more audio samples
            if (!g_SPD.bWaveEnd)
            {
                // For each frame of the current block
                for (count = 0; count < g_SPD.uiSynthFramesPerBlock; count++)
                {
                    pFillBuffer = ((char *)lpWrite) + count * g_SPD.uiSynthFrameBytes;

                    // Generate new audio samples, putting the directly
                    // into the output buffer.
                    BAE_BuildMixerSlice(threadContext, pFillBuffer, g_audioByteBufferSize, g_audioFramesToGenerate);

                    // I am incrementing g_lastPos right after the write operation
                    // to lessen the chance of a device open/close messing us up in the wait loop.
                    g_lastPos += (g_audioByteBufferSize * g_synthFramesPerBlock);

                    if (g_shutDownDoubleBuffer)
                    {
                        break;
                    }
                }
            }
            else
            {
                // Fill the current block with appropriate silence value/
                memset(lpWrite, (g_waveFormat.wBitsPerSample == 16) ? 0 : 0x80, dwWriteLen);
            }
            // Unlock the block of SoundBuffer were we writing.
            error = IDirectSoundBuffer_Unlock(g_pDirectSoundBuffer,
                                                    lpWrite, dwWriteLen, NULL, 0);

            // Advance to the next block in the output buffer
            g_SPD.uiWriteBlock++;
            // We're circular, so when we reach the end of the buffer,
            // start back at the beginning.
            if (g_SPD.uiWriteBlock > (g_SPD.uiBufferBlocks - 1))
            {
                g_SPD.uiWriteBlock = 0;
            }
            // Calculate the new offsets.
            g_SPD.uiDestPos = g_SPD.uiWriteBlock * g_SPD.uiBlockSize;
        }

        // Set the ProcessFrameFinish event before going back to sleep.
        SetEvent(g_heventProcessFrameFinish);
    }

    g_activeDoubleBuffer = FALSE;
}



// DWORD PV_MonitorDirectSoundPosition(LPVOID lpv)
//
// This is the main thread routine for our engine.
//
// Here's a synopsis of what's going on:
// - Divide the output buffer into g_nBufferSegments.
// - Poll the DirectSound play cursor.
// - When the play cursor crosses the transition
//   from iPrevBlock to iCurBlock, wake up the PV_AudioDirectSoundFrameThread thread
//   to fill the next block.
// - Wait for PV_AudioDirectSoundFrameThread to finish
// - Sleep so that we aren't hogging the CPU with our polling
// - Wake up and poll the play cursor again.
//
// When the g_SPD.bWaveEnd flag is set, indicating that we
// should stop:
// - Mark the block we were polling in when the flag was set.
// - Wake up the PV_AudioDirectSoundFrameThread thread to fill the last block.
// - Let the polling engine know that the last block has been
//   filled by setting the bWaitingForLastBlock flag.
// - Wait for the last block to be played by continuing to poll
//   until the play cursor has passed through the last block.
//
// Note:
//   The block indecies used in this polling thread are not the same
//   as the indecies used in the PV_AudioDirectSoundFrameThread thread.  They are
//   offset by one because the first block of the sound buffer is
//   filled with silence.  This allows the engine to generate the
//   first audio frames while this silent block is playing.
//
//   There is a problem with this design though.  If PV_AudioDirectSoundFrameThread takes
//   more time to process the current block than it takes to play the
//   previous block, the polling loop thread will have missed the next
//   block transition.  This will result in having to wait until the same
//   transition comes around again before more audio is generated.  The
//   blocks after the missed transition will not be filled with new data
//   therefore old audio blocks will be replayed.

static DWORD WINAPI PV_MonitorDirectSoundPosition(LPVOID lpv)
{
    BOOL            bWaitingForLastBlock;
    DWORD           dwPlayPos, dwWritePos;
    int             iPrevBlock;
    int             iCurBlock;
    SOUNDPOSBLOCK   *pSoundBlock;
    DWORD           dwCurPos;
    DWORD           count;

    lpv = lpv;
    dwCurPos = 0;
    iPrevBlock = 0;
    iCurBlock = 0;
    bWaitingForLastBlock = FALSE;
    pSoundBlock = (SOUNDPOSBLOCK *)BAE_Allocate(sizeof(SOUNDPOSBLOCK) * g_SPD.uiBufferBlocks);
    if (pSoundBlock)
    {
        // Calculate the boundaries for each of the blocks
        for (count = 0; count < g_SPD.uiBufferBlocks; count++)
        {
            pSoundBlock[count].dwStart = dwCurPos;
            pSoundBlock[count].dwEnd = dwCurPos + g_SPD.uiBlockSize;
            pSoundBlock[count].bEndingBlock = FALSE;
            dwCurPos += g_SPD.uiBlockSize;
        }

        // While the engine is not done
        while(!g_SPD.bDone)
        {
            // Give up some of our time slice
            Sleep(g_audioPeriodSleepTime);

            // Get the current play position
            IDirectSoundBuffer_GetCurrentPosition(g_pDirectSoundBuffer, &dwPlayPos, &dwWritePos);

            // If the play position has entered the current block and it hasn't
            // reached the end of the block yet
            if (dwPlayPos >= pSoundBlock[iCurBlock].dwStart &&
                dwPlayPos < pSoundBlock[iCurBlock].dwEnd)
            {
                // Wake up PV_AudioDirectSoundFrameThread to generate a block of audio
                SetEvent(g_heventProcessFrameStart);
                // Wait for PV_AudioDirectSoundFrameThread to finish only if
                // we're not shutting down.
                if (g_SPD.bDone == FALSE)
                {
                    WaitForSingleObject(g_heventProcessFrameFinish, INFINITE);
                }
                // If the this is the last block to be generated
                // and we haven't marked it yet
                if (g_SPD.bWaveEnd && !bWaitingForLastBlock)
                {
                    // Mark the current block as the last block
                    pSoundBlock[iCurBlock].bEndingBlock = TRUE;
                    bWaitingForLastBlock = TRUE;
                }

                // If we're waiting for the last block to play
                if (bWaitingForLastBlock)
                {
                    iPrevBlock = iCurBlock - 1;
                    if (iPrevBlock < 0)
                    {
                        iPrevBlock = g_SPD.uiBufferBlocks - 1;
                    }
                    // If the current block is the last block
                    if (pSoundBlock[iPrevBlock].bEndingBlock)
                    {
                        // End the thread and stop playing sound
                        g_SPD.bDone = TRUE;
                        IDirectSoundBuffer_Stop(g_pDirectSoundBuffer);
                    }
                }
                // Go to the next block.
                iCurBlock++;
                // If it's the last in the list wrap back to the first.
                if ((UINT)iCurBlock > g_SPD.uiBufferBlocks - 1)
                {
                    iCurBlock = 0;
                }
            }
        }
    }

    // Get rid of our list of sound blocks
    BAE_Deallocate(pSoundBlock);
    return 0;
}

// void PV_ClearDirectSoundBuffer ()
//
// Files the DirectSound buffer with silence.

static void PV_ClearDirectSoundBuffer(void)
{
    HRESULT     error;
    LPVOID      lpWrite;
    DWORD       dwWriteLen;

    if (g_pDirectSoundBuffer)
    {
        error = IDirectSoundBuffer_Lock(g_pDirectSoundBuffer, 0,
                                        g_SPD.uiBlockSize * g_SPD.uiBufferBlocks,
                                        &lpWrite, &dwWriteLen, NULL, NULL,
                                        0);
        if (error == DS_OK)
        {
            memset(lpWrite, (g_waveFormat.wBitsPerSample == 16) ? 0 : 0x80, dwWriteLen);
            error = IDirectSoundBuffer_Unlock(g_pDirectSoundBuffer,
                                                    lpWrite, dwWriteLen, NULL, 0);
        }
    }
}

// int PV_CreateDirectSoundThreads()
//
// Inititialize the SOUNDPOSDATA structure and create the threads
// and events for our engine
// returns -1 if failure, or 0 for success
static int PV_CreateDirectSoundThreads(void* threadContext)
{
    int     status;
    DWORD   threadID;

    status = -1;
    g_heventProcessFrameStart = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (g_heventProcessFrameStart)
    {
        g_heventProcessFrameFinish = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (g_heventProcessFrameFinish)
        {
            // Create the monitor thread
            g_hthreadSoundMon = CreateThread(NULL, 0L,
                                                PV_MonitorDirectSoundPosition, 0L,
                                                0L, &threadID);
            if (g_hthreadSoundMon)
            {
                if (BAE_CreateFrameThread(threadContext, PV_AudioDirectSoundFrameThread) >= 0)
                {
                    // THREAD_PRIORITY_HIGHEST
                    // THREAD_PRIORITY_NORMAL
                    // THREAD_PRIORITY_TIME_CRITICAL
                    SetThreadPriority(g_hthreadSoundMon, THREAD_PRIORITY_TIME_CRITICAL);
                    status = 0;
                }
            }
        }
    }
    return status;
}

// This function will allocate and setup a DirectSound Primary buffer. It assmes
// the global variables g_directSoundWindow are set a valid HWND, and 
// g_pDirectSoundObject has a valid object returned from DirectSoundCreate.
// The global g_pDirectSoundPrimaryBuffer should not have been allocated prior
// to calling this function.
//
// NOTE: By calling this, DirectSound will not be able to share audio buffers.
//
static HRESULT PV_AllocateDirectSoundPrimaryBuffer(WAVEFORMATEX *pFormat)
{
    DSBUFFERDESC    dsbd;
    HRESULT         error;

    if (g_pDirectSoundObject && g_directSoundWindow)
    {
        g_pDirectSoundPrimaryBuffer = NULL; // should be NULL already, but makes it clear
        // First thing that must be done after creating object
        // Set level to PRIORITY so that we can change format of primary buffer
        error = IDirectSound_SetCooperativeLevel(g_pDirectSoundObject,
                                            g_directSoundWindow, DSSCL_PRIORITY);
        if (error == DS_OK)
        {
            // Create a primary buffer and set its format to that of our synth engine
            dsbd.dwSize = sizeof(dsbd);
            dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;

            dsbd.dwBufferBytes = 0;
            dsbd.dwReserved = 0;
            dsbd.lpwfxFormat = NULL;
            error = IDirectSound_CreateSoundBuffer(g_pDirectSoundObject, &dsbd, &g_pDirectSoundPrimaryBuffer, NULL);

            if (error == DS_OK)
            {
                if (g_pDirectSoundPrimaryBuffer)
                {
                    // By setting the primary buffer to have the same format as our generated
                    // synth data, there will be no extra overhead when DirectSound mixes
                    // our "Secondary" buffer into the "Primary" buffer.
                    error = IDirectSoundBuffer_SetFormat(g_pDirectSoundPrimaryBuffer,
                                                                            &g_waveFormat);
                }
            }
        }
    }
    else
    {
        error = -1;
    }
    return error;
}

// setup direct sound, create a primary buffer and set format to match.
// Return 0 if success, or -1 if it fails
static int PV_SetupDirectSound(void* threadContext, int sampleRate, 
                                int channels, int bitSize, BOOL createThreads)
{
    int             status;
    HRESULT         error;
    DSBUFFERDESC    dsbd;

    status = -1;
    memset(&g_waveFormat, 0, (DWORD)sizeof(WAVEFORMATEX));
    error = 0;
    if (g_directSoundWindow == NULL)
    {
        g_directSoundWindow = PV_GetMostActiveWindow();
    }
    if (g_directSoundWindow)
    {
        // Use standard PCM audio samples
        //g_waveFormat.cbSize = 0;
        g_waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        g_waveFormat.nSamplesPerSec = sampleRate;
        g_waveFormat.nChannels = channels;
        g_waveFormat.wBitsPerSample = bitSize;
        // Calculate size in bytes of an audio sample
        g_waveFormat.nBlockAlign =  g_waveFormat.nChannels * g_waveFormat.wBitsPerSample / 8;
        g_waveFormat.nAvgBytesPerSec = g_waveFormat.nSamplesPerSec * g_waveFormat.nBlockAlign;

        // Try and create a DirectSound object
#if !USE_COM_DIRECTSOUND
// we're linking with dsound.lib
        if (DirectSoundCreate(NULL, &g_pDirectSoundObject, NULL) == DS_OK)
#else
// use COM
        if (PV_DirectSoundCreate())
#endif
        {
            assert(g_pDirectSoundObject);
            if (g_currentDeviceID == BAE_DIRECTSOUND_PRIMARY_BUFFER)
            {
                // ok, we want a primary buffer, so this will allocate one, set it
                // format to the correct one, and lock out any one else using DirectSound.
                error = PV_AllocateDirectSoundPrimaryBuffer(&g_waveFormat);
            }
            else
            {
                // We're secondary, so try first to allocate a primary buffer then throw it away. We do this
                // to force the primary buffer to be allocated nicely. This will take over the sound 
                // device only if no one else has it and set it to the format that we want. 
                // Otherwise we get the yucky default 22k 8-bit mono mode. If it fails we don't
                // care because its been allocated by someone else anyway, and we have to live in the
                // format presented.
                error = PV_AllocateDirectSoundPrimaryBuffer(&g_waveFormat);
                if (g_pDirectSoundPrimaryBuffer)
                {
                    IDirectSoundBuffer_Release(g_pDirectSoundPrimaryBuffer);
                    g_pDirectSoundPrimaryBuffer = NULL;
                }

                // First thing that must be done after creating object
                // Set level to PRIORITY to a normal condition because we're just
                // using secondary buffers.
                error = IDirectSound_SetCooperativeLevel(g_pDirectSoundObject,
                                                    g_directSoundWindow, DSSCL_PRIORITY);   // DSSCL_NORMAL
            }

            // At this point all buffers created will be secondary buffers. If we allocated
            // a primary it will be in the format we asked for, otherwise DirectSound
            // will translate into whatever the current DirectSound primary buffer is
            // set up via IDirectSound_SetFormat.
            if (error == DS_OK)
            {
                // Now we can create the secondary sound buffer we will use to output sound
                dsbd.dwSize = sizeof(dsbd);
                dsbd.dwFlags = DSBCAPS_STICKYFOCUS;
                dsbd.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;
                dsbd.dwFlags |= DSBCAPS_GLOBALFOCUS;

                dsbd.dwBufferBytes = g_waveFormat.nBlockAlign *
                                        g_audioFramesToGenerate *
                                        g_synthFramesPerBlock *
                                        g_nBufferSegments;
                dsbd.dwReserved = 0;
                dsbd.lpwfxFormat = &g_waveFormat;

                error = IDirectSound_CreateSoundBuffer(g_pDirectSoundObject, &dsbd, &g_pDirectSoundBuffer, NULL);
                if (error == DS_OK)
                {
                    // Make sure we start at the beginning of the buffer
                    error = IDirectSoundBuffer_SetCurrentPosition(g_pDirectSoundBuffer, 0);
                    if (error == DS_OK)
                    {
                        // Start by writing an empty block
                        PV_ClearDirectSoundBuffer();

                        g_SPD.uiSynthFramesPerBlock = g_synthFramesPerBlock;
                        // Number of bytes per audio frame
                        g_SPD.uiSynthFrameBytes = g_audioFramesToGenerate * g_waveFormat.nBlockAlign;
                        // Initial output buffer offset to start wrtting audio data
                        g_SPD.uiDestPos = g_SPD.uiSynthFrameBytes * g_SPD.uiSynthFramesPerBlock;
                        // Total number of bytes generated on each pass through PV_AudioDirectSoundFrameThread
                        g_SPD.uiBlockSize = g_SPD.uiSynthFrameBytes * g_SPD.uiSynthFramesPerBlock;
                        // Total number of blocks in output buffer
                        g_SPD.uiBufferBlocks = g_nBufferSegments;
                        // Total number of bytes in output buffer
                        g_SPD.uiBufferSize = g_SPD.uiBlockSize * g_nBufferSegments;
                        // Index of next block to we written
                        g_SPD.uiWriteBlock = 1;
                        g_SPD.bWaveEnd = FALSE;
                        g_SPD.bDone = FALSE;

                        if (createThreads)
                        {
                            // Create thread to poll sound play position
                            status = PV_CreateDirectSoundThreads(threadContext);
                            if (status == 0)
                            {
                                // Start playing whatever is in the output buffer (silence initially)
                                error = IDirectSoundBuffer_Play(g_pDirectSoundBuffer,
                                                                        0, 0, DSBPLAY_LOOPING);
                                if (error == DS_OK)
                                {
                                    status = 0;
                                }
                                else
                                {
                                    status = -1;
                                }
                            }
                        }
                        else
                        {
                            status = 0;
                        }
                    }
                }
            }
        }
    }
    #ifdef DEBUG_DIRECTSOUND
    if (error)
    {
        PV_DisplayDirectSoundError(error);
    }
    #endif
    return status;
}

static void PV_CleanupDirectSound(void)
{
    long        status;

    // Did we create the SoundMon thread?
    if (g_hthreadSoundMon)
    {
        // Signal thread to stop generating audio
        g_SPD.bWaveEnd = TRUE;
        g_SPD.bDone = TRUE;

        // wake up threads
        SetEvent(g_heventProcessFrameStart);
        SetEvent(g_heventProcessFrameFinish);

        // Start by writing an empty block
        PV_ClearDirectSoundBuffer();
        // wait for thread to end and only wait 500 ms
        status = WaitForSingleObject(g_hthreadSoundMon, 500);
        // If thread takes too long, kill it
        if (status == WAIT_TIMEOUT)
        {
            TerminateThread(g_hthreadSoundMon, 0);
            g_hthreadSoundMon = NULL;
        }
        CloseHandle(g_hthreadSoundMon);
        g_hthreadSoundMon = NULL;
    }
    if (g_pDirectSoundBuffer)
    {
        IDirectSoundBuffer_Stop(g_pDirectSoundBuffer);
    }
    if (g_pDirectSoundPrimaryBuffer)
    {
        IDirectSoundBuffer_Release(g_pDirectSoundPrimaryBuffer);
        g_pDirectSoundPrimaryBuffer = NULL;
    }
    if (g_pDirectSoundBuffer)
    {
        IDirectSoundBuffer_Release(g_pDirectSoundBuffer);
        g_pDirectSoundBuffer = NULL;
    }
#if !USE_COM_DIRECTSOUND
    if (g_pDirectSoundObject)
    {
        IDirectSound_Release(g_pDirectSoundObject);
        g_pDirectSoundObject = NULL;
    }
#else
    // using COM
    PV_DirectSoundRelease();
#endif
    if (g_heventProcessFrameStart)
    {
        CloseHandle(g_heventProcessFrameStart);
        g_heventProcessFrameStart = NULL;
    }
    if (g_heventProcessFrameFinish)
    {
        CloseHandle(g_heventProcessFrameFinish);
        g_heventProcessFrameFinish = NULL;
    }
    g_directSoundWindow = NULL;
}
#endif  // USE_DIRECTSOUND

#if USE_PREFS
// reads a file at "<WINDOWS>bae_setup.txt" that contains three numbers:
/*
4 6 2 - 95/98

26 9 2 - NT

    First number is how many DirectSound buffers to build. Reasonable range is 2 to 50.
    Windows 98 this number is 9. 25 is what I used to use for NT, and its not working. 
    My guess is NT wants a number between 25 and 31.

    Second number is how many milliseconds to sleep or let some other thing do
    its thing. Resonable range is 3 to 15. On Windows this number is 6. NT its 5. 
    Typically this is set to half of 11 because it when audio is playing out of the 
    audio buffer it takes 11 ms to finish, so we need to be looking at when to build 
    the next buffer sooner than it ends. Larger numbers will cause more breakups.

    Third number is the device ID. 0 is waveOut, 1 is primary direct sound, 2 is
    secondary direct sound.
*/

static BOOL PV_ReadFromPrefs(BAEDeviceID *pDeviceID, unsigned int *pSynthFramesPerBlock, unsigned int *pAudioPeriodSleepTime)
{
    long            ref;
    char            data[2048];
    unsigned long   frames, sleep, device;
    int             ret;
    char            fileName[MAX_PATH];
    BOOL            found;

    found = FALSE;
    GetWindowsDirectory(fileName, MAX_PATH);
    strcat(fileName, "\\bae_setup.txt");

    ref = BAE_FileOpenForRead(fileName);
    if (ref != -1)
    {
        BAE_ReadFile(ref, data, 2048L);
        ret = sscanf(data, "%ld %ld %ld", &frames, &sleep, &device);
        if (ret == 3)   // we got three paramters
        {
            found = TRUE;
            if (pSynthFramesPerBlock)
            {
                if ((frames > 1) && (frames < 50))
                {
                    *pSynthFramesPerBlock = (unsigned int)frames;
                }
            }
            if (pAudioPeriodSleepTime)
            {
                if ((sleep > 2) && (sleep < 15))
                {
                    *pAudioPeriodSleepTime = (unsigned int)sleep;
                }
            }
            if (pDeviceID)
            {
                if ((device >= 0) && (device < (unsigned long)BAE_MaxDevices()))
                {
                    *pDeviceID = (BAEDeviceID)device;
                }
            }
        }

        BAE_FileClose(ref);
    }
    return found;
}
#endif

static void PV_SetTimingDefaults(int reset)
{
    switch(g_currentDeviceID)
    {
        case BAE_WAVEOUT:
            // use waveOut API
            // we're going to build this many buffers at a time
            g_synthFramesPerBlock = BAE_WAVEOUT_FRAMES_PER_BLOCK;
            g_audioPeriodSleepTime = BAE_WAVEOUT_SOUND_PERIOD;
            if (reset)
            {
                g_userShadowSynthFramesPerBlock = g_synthFramesPerBlock;
                g_userShadowAudioPeriodSleepTime = g_audioPeriodSleepTime;
            }
            break;

#if USE_DIRECTSOUND
        case BAE_DIRECTSOUND_PRIMARY_BUFFER:
        case BAE_DIRECTSOUND_SECONDARY_BUFFER:
            // use DirectSound API
            // we're going to build this many buffers at a time
            g_synthFramesPerBlock = BAE_DIRECTSOUND_FRAMES_PER_BLOCK;
            g_audioPeriodSleepTime = BAE_DIRECTSOUND_SOUND_PERIOD;

            if (reset)
            {
                g_userShadowSynthFramesPerBlock = g_synthFramesPerBlock;
                g_userShadowAudioPeriodSleepTime = g_audioPeriodSleepTime;
            }
            break;
#endif
    }
}

// **** Audio card support
// Aquire and enabled audio card
// return 0 if ok, -1 if failed
int BAE_AquireAudioCard(void *threadContext, unsigned long sampleRate, unsigned long channels, unsigned long bits)
{
    int             flag;
    short int       count;
    long            bufferSize;
    long            error;
    WAVEFORMATEX    waveFormat;
    #if USE_PREFS
    BOOL            validPrefs;
    BAEDeviceID     deviceID;
    unsigned int    synthFramesPerBlock;
    unsigned int    audioPeriodSleepTime;
    #endif

    flag = 0;
    g_activeDoubleBuffer = FALSE;
    g_shutDownDoubleBuffer = TRUE;

    g_audioFramesToGenerate = BAE_GetMaxSamplePerSlice();   // get number of frames per sample rate slice

    #if USE_PREFS
    validPrefs = FALSE;
    if (PV_ReadFromPrefs(&deviceID, &synthFramesPerBlock, &audioPeriodSleepTime))
    {
        g_currentDeviceID = deviceID;
        validPrefs = TRUE;
    }
    #endif
    // set default timings for current device
    PV_SetTimingDefaults(FALSE);
    // use DirectSound or waveOut API
    if (g_currentDeviceID == BAE_WAVEOUT)
    {   // use waveOut API
        if (g_userShadowSynthFramesPerBlock)
        {
            g_synthFramesPerBlock = g_userShadowSynthFramesPerBlock;
        }
        if (g_userShadowAudioPeriodSleepTime)
        {
            g_audioPeriodSleepTime = g_userShadowAudioPeriodSleepTime;
        }

        #if USE_PREFS
        if (validPrefs)
        {
            g_synthFramesPerBlock = synthFramesPerBlock;
            g_audioPeriodSleepTime = audioPeriodSleepTime;
        }
        #endif

        // Use waveOut API
        waveFormat.wFormatTag = WAVE_FORMAT_PCM;

        waveFormat.nSamplesPerSec = sampleRate;
        waveFormat.nChannels = (unsigned short)channels;
        waveFormat.wBitsPerSample = (unsigned short)bits;

        // Calculate size in bytes of an audio sample
        waveFormat.nBlockAlign =  waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
        waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
        waveFormat.cbSize = 0;
        g_waveFormat = waveFormat;

        if (waveFormat.wBitsPerSample == 8)
        {
            bufferSize = (sizeof(char) * g_audioFramesToGenerate);
        }
        else
        {
            bufferSize = (sizeof(short int) * g_audioFramesToGenerate);
        }
        bufferSize *= waveFormat.nChannels;
        g_audioByteBufferSize = bufferSize;


        // allocate buffer blocks
        flag = TRUE;
        g_audioBufferBlock = BAE_Allocate(sizeof(void *) * g_synthFramesPerBlock);
        for (count = 0; count < (long)g_synthFramesPerBlock; count++)
        {
            g_audioBufferBlock[count] = BAE_Allocate(g_audioByteBufferSize * g_synthFramesPerBlock);
            if (g_audioBufferBlock[count] == NULL)
            {
                flag = FALSE;
                break;
            }
        }

        // try to open wave device
        if (flag)
        {
            error = waveOutOpen(&g_waveDevice,  WAVE_MAPPER, &waveFormat,
                                0L, 0L,         CALLBACK_NULL
                                //              | WAVE_FORMAT_DIRECT
                                //              | WAVE_ALLOWSYNC
                                //              | WAVE_MAPPED
                                );
            if (error == 0)
            {
                g_shutDownDoubleBuffer = FALSE;
                g_activeDoubleBuffer = TRUE;    // must enable process, before thread begins


                // $$kk: 08.12.98 merge 
                // $$kk: 05.06.98: added this whole block.
                // we need to reset the g_lastPos each time the device gets acquired.
                // otherwise we may get stuck in the wait loop because we never count
                // up to the right sample position.
                {
                    MMTIME          audioStatus;
                    memset(&audioStatus, 0, (long)sizeof(MMTIME));
                    audioStatus.wType = TIME_BYTES; // get byte position

                    error = waveOutGetPosition(g_waveDevice, &audioStatus, sizeof(MMTIME));

                    g_lastPos = audioStatus.u.cb - (g_audioByteBufferSize * g_synthFramesPerBlock * 2);
                }


                // create thread that builds and sends buffers to the wave device
                if (BAE_CreateFrameThread(threadContext, PV_AudioWaveOutFrameThread) >= 0)
                {
                    flag = 0;
                }
                else
                {
                    flag = 1;
                    g_activeDoubleBuffer = FALSE;
                }
            }
            else
            {
                flag = 1;
            }
        }
    }
#if USE_DIRECTSOUND
    else
    {   // use DirectSound API
        if (g_userShadowSynthFramesPerBlock)
        {
            g_synthFramesPerBlock = g_userShadowSynthFramesPerBlock;
        }
        if (g_userShadowAudioPeriodSleepTime)
        {
            g_audioPeriodSleepTime = g_userShadowAudioPeriodSleepTime;
        }

        #if USE_PREFS
        if (validPrefs)
        {
            g_synthFramesPerBlock = synthFramesPerBlock;
            g_audioPeriodSleepTime = audioPeriodSleepTime;
        }
        #endif

        g_shutDownDoubleBuffer = FALSE;
        g_activeDoubleBuffer = TRUE;    // must enable process, before thread begins
        g_audioByteBufferSize = g_audioFramesToGenerate * channels * (bits / 8);

        // create direct sound buffers, and control threads
        flag = PV_SetupDirectSound(threadContext, sampleRate, channels, bits, TRUE);
        if (flag != 0)
        {
            // something failed
            flag = 1;
            g_activeDoubleBuffer = FALSE;
        }
    }
#endif  // USE_DIRECTSOUND
    if (flag)
    {   // something failed
        BAE_ReleaseAudioCard(threadContext);
    }
    else
    {
        g_lastDeviceID = g_currentDeviceID;
    }
    return flag;
}

// Release and free audio card.
// return 0 if ok, -1 if failed.
int BAE_ReleaseAudioCard(void *threadContext)
{
    long        count;

    g_shutDownDoubleBuffer = TRUE;  // signal the frame thread to stop
#if USE_DIRECTSOUND
    if (g_currentDeviceID != BAE_WAVEOUT)
    {   // DirectSound
        PV_CleanupDirectSound();
    }
#endif  // USE_DIRECTSOUND
    BAE_DestroyFrameThread(threadContext);

    // use DirectSound or waveOut API
    if (g_currentDeviceID == BAE_WAVEOUT)
    {   // waveOut
        if (g_waveDevice)
        {
            waveOutReset(g_waveDevice);
            while (waveOutClose(g_waveDevice) == WAVERR_STILLPLAYING)
            {
                waveOutReset(g_waveDevice);
                //MOE: Looping here doesn't help.  By the specs and observation
                // waveOut is hosed if we get here and will never "close".
                // We might as well just leave.  It would be good if we
                // could return an error code here.
                break;
            }
            g_waveDevice = 0;
        }
        if (g_audioBufferBlock)
        {
            for (count = 0; count < (long)g_synthFramesPerBlock; count++)
            {
                BAE_Deallocate(g_audioBufferBlock[count]);
                g_audioBufferBlock[count] = NULL;
            }
            BAE_Deallocate(g_audioBufferBlock);
            g_audioBufferBlock = NULL;
        }
    }
    BAE_SleepFrameThread(threadContext, 20);    // wait 20 ms to let system settle down.

    return 0;
}

// return device position in samples since the device was opened
unsigned long BAE_GetDeviceSamplesPlayedPosition(void)
{
    int             error, frameSize;
    MMTIME          audioStatus;
    unsigned long   pos;

    pos = 0;
    frameSize = g_waveFormat.nChannels * g_waveFormat.wBitsPerSample / 8;
#if USE_DIRECTSOUND
    if (g_currentDeviceID != BAE_WAVEOUT)
    {   // DirectSound
        if (g_pDirectSoundBuffer)
        {
            unsigned long   dwPlayPos, dwWritePos;
    
            // get byte position
            IDirectSoundBuffer_GetCurrentPosition(g_pDirectSoundBuffer, &dwPlayPos, &dwWritePos);
            // GetCurrentPosition return the postion of the current buffer, so we have to add
            // it to our global position to get an actual position in samples.
            pos = (g_lastPos + dwPlayPos) / frameSize;
        }
    }
    else
#endif  // USE_DIRECTSOUND
    {   // waveOut
        if (g_pDirectSoundBuffer)
        {
            memset(&audioStatus, 0, (long)sizeof(MMTIME));
            audioStatus.wType = TIME_BYTES; // get byte position
    
            error = waveOutGetPosition(g_waveDevice, &audioStatus, sizeof(MMTIME));
            // return in samples
            pos = audioStatus.u.cb / frameSize;
        }
    }
    return pos;
}

// number of devices. ie different versions of the BAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_MaxDevices(void)
{
#if USE_DIRECTSOUND
    PV_DirectSoundCreate();
#else
    assert(g_maxDeviceCount == BAE_WAVEOUT + 1);
#endif
    return g_maxDeviceCount;
}

// set the current device. device is from 0 to BAE_MaxDevices()
// NOTE:    This function needs to function before any other calls may have happened.
//          Also you will need to call BAE_ReleaseAudioCard then BAE_AquireAudioCard
//          in order for the change to take place.
void BAE_SetDeviceID(long deviceID, void *deviceParameter)
{
    BAEWinOSParameters  *parms;

    parms = (BAEWinOSParameters *)deviceParameter;
    if (deviceID < BAE_MaxDevices())
    {
        switch ((BAEDeviceID)deviceID)
        {
            case BAE_WAVEOUT:       // waveout
                if (parms)
                {
                    g_userShadowSynthFramesPerBlock = parms->synthFramesPerBlock;
                    g_userShadowAudioPeriodSleepTime = parms->audioPeriodSleepTime;
                }
                break;
#if USE_DIRECTSOUND
            case BAE_DIRECTSOUND_PRIMARY_BUFFER:    // directsound
            case BAE_DIRECTSOUND_SECONDARY_BUFFER:
                if (parms)
                {
                    g_userShadowSynthFramesPerBlock = parms->synthFramesPerBlock;
                    g_userShadowAudioPeriodSleepTime = parms->audioPeriodSleepTime;
                    g_directSoundWindow = (HWND)parms->windowHandle;
                }
                break;
#endif
            default :
                assert(FALSE);
                break;
        }

        g_currentDeviceID = (BAEDeviceID)deviceID;
        // check to see if we've changed devices. If we have then reset to defaults.
        if (g_lastDeviceID != g_currentDeviceID)
        {
            PV_SetTimingDefaults(TRUE);
        }
    }
}

// return current device ID
// NOTE: This function needs to function before any other calls may have happened.
long BAE_GetDeviceID(void *deviceParameter)
{
    BAEWinOSParameters  *parms;

    parms = (BAEWinOSParameters *)deviceParameter;
    if (parms)
    {
        parms->windowHandle = NULL;
        parms->synthFramesPerBlock = g_synthFramesPerBlock;
        parms->audioPeriodSleepTime = g_audioPeriodSleepTime;
    }

    switch (g_currentDeviceID)
    {
        case BAE_WAVEOUT:       // waveout
            break;
#if USE_DIRECTSOUND
        case BAE_DIRECTSOUND_PRIMARY_BUFFER:    // directsound
        case BAE_DIRECTSOUND_SECONDARY_BUFFER:
            if (parms)
            {
                parms->windowHandle = (void *)g_directSoundWindow;
            }
            break;
#endif
        default :
            assert(FALSE);
            break;
    }

    return g_currentDeviceID;
}

// NOTE:    This function needs to function before any other calls may have happened.
//          Format of string is a zero terminated comma delinated C string.
//          "platform,method,misc"
//  example "MacOS,Sound Manager 3.0,SndPlayDoubleBuffer"
//          "WinOS,DirectSound,multi threaded"
//          "WinOS,waveOut,multi threaded"
//          "WinOS,VxD,low level hardware"
//          "WinOS,plugin,Director"
void BAE_GetDeviceName(long deviceID, char *cName, unsigned long cNameLength)
{
    char        *data;
    static char *names[] =
            {   "WinOS,waveOut,multi threaded"
#if USE_DIRECTSOUND
                ,"WinOS,DirectSound Primary Buffer,multi threaded"
                ,"WinOS,DirectSound Secondary Buffer,multi threaded"
#endif
            };
    if (cName && cNameLength)
    {
        if ((deviceID < BAE_MaxDevices()) && (deviceID >= 0))
        {
            // can't do string functions here, it might be bad to do.
            data = names[deviceID];
            while (*data && (cNameLength > 1))
            {
                *cName = *data;
                cName++;
                data++;
                cNameLength--;
            }
        }
        *cName = 0;
    }
}


// EOF of BAE_API_WinOS.c
