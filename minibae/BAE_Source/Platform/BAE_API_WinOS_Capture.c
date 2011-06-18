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
**  BAE_API_WinOS_Capture.c
**
**  This provides platform specfic functions for Windows 95/NT. This interface
**  for BAE is for Windows 95/NT and uses the waveIn API to capture audio
**  buffer slices through the multimedia system.
**
**  Overview:
**
**  © Copyright 1995-1999 Beatnik, Inc, All Rights Reserved.
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
**  6/19/98     Created
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
**  7/23/98     Added a new parameter to BAE_AquireAudioCapture
**              Added BAE_PauseAudioCapture & BAE_ResumeAudioCapture
**  8/3/98      Added support for multi devices and control which device is active
**              Fixed some type casting
** 
**  JAVASOFT
**  10.14.98    $$kk: messed with this file utterly.  changed callback mechanism to
**              run a separate thread for callbacks (analogous with output) and not
**              use windows callbacks: 1) dangerous from java because you can't pass
**              JNIEnv pointers between threads and 2) on WinNT, waveInAddBuffer called
**              on windows callback thread locks forever.
**  3/5/99      Changed context to threadContext to reflect what is really is used
**              in the system.
*/
/*****************************************************************************/

#ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <mmsystem.h>
#include <windowsx.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include "BAE_API.h"

#ifndef USE_DIRECTSOUND
    #define USE_DIRECTSOUND             1
#endif

#if USE_DIRECTSOUND
    #include <dsound.h>
#endif


#define BAE_WAVEIN_NUM_BUFFERS              3           // number of capture buffers
#define BAE_WAVEIN_SOUND_PERIOD             11          // sleep period between position checks (in ms)
#define BAE_WAVEIN_FRAMES_PER_BUFFER        16384       // how many frames of audio data per capture buffer?
                                                        // $$kk: 10.13.98: need to decide whether / how to let user configure this;
                                                        // what is a good default??


static void             *g_audioBufferBlock[BAE_WAVEIN_NUM_BUFFERS];        // actual data buffers
static long             g_audioBytesPerBuffer;                              // size of each audio buffer in bytes

static HWAVEIN          g_captureSound = NULL;          // the capture device
static BOOL             g_captureShutdown;              // false if capture active, otherwise true

long                    g_audioFramesPerBuffer;         // size in sample frames of each capture buffer
                                                        // $$kk: 10.13.98: right now this is fixed as BAE_WAVEIN_FRAMES_PER_BUFFER
                                                        // $$kk: 10.13.98: need to figure out how to configure this

static short int        g_bitSize;                      // bit size of current capture format
static short int        g_channels;                     // number of channels in current capture format
static short int        g_sampleRate;                   // sample rate of current capture format

static short int        g_soundDeviceIndex = 0;         // if non zero then use this device to open

static BAE_CaptureDone  g_captureDoneProc;
static void             *g_captureDoneContext;


/*
static void CALLBACK PV_AudioCaptureCallback(HWAVEIN captureHandle, UINT message, 
                                            DWORD platformContext, DWORD dwParam1, DWORD dwParam2)
{
    dwParam1;
    dwParam2;
    platformContext;
    switch (message)
    {
        case WIM_OPEN:

            if (g_captureDoneProc)
            {
                (*g_captureDoneProc)(g_captureDoneContext, OPEN_CAPTURE, NULL, NULL);
            }
            
              break;
        case WIM_CLOSE:

            if (g_captureDoneProc)
            {
                (*g_captureDoneProc)(g_captureDoneContext, CLOSE_CAPTURE, NULL, NULL);
            }

            break;
        case WIM_DATA:
            
            fprintf(stderr, "\nWIM_DATA\n");
            break;

            if ((g_captureShutdown == FALSE) && (g_captureSound == captureHandle))
            {
                LPWAVEHDR hdr = (LPWAVEHDR)dwParam1;

                fprintf(stderr, "\nWIM_DATA (1)\n");

                if (g_captureDoneProc)
                {
                    // $$kk: 10.08.98: added check for bytes recorded in LPWAVEHDR
                    //(*g_captureDoneProc)(g_captureDoneContext, DATA_READY_CAPTURE, &g_captureBuffer, &g_captureBufferSize);

                    DWORD dwBytesRecorded = hdr->dwBytesRecorded;
                    LPSTR lpData = hdr->lpData;

                    fprintf(stderr, "\nWIM_DATA (2)\n");

                    (*g_captureDoneProc)(g_captureDoneContext, DATA_READY_CAPTURE, &lpData, (void *)&dwBytesRecorded);
                }
            }
            break;
    }
}
*/


// this is run by a java thread; the context needs to be the JNI environment
// pointer valid for the thread.
static void PV_AudioWaveInFrameThread(void* threadContext)
{
    WAVEHDR         waveHeader[BAE_WAVEIN_NUM_BUFFERS];
    MMTIME          audioStatus;

    long            count, currentPos, lastPos, framesToRead, bytesToRead, buffersToRead, error;

    long            waveHeaderCount;    // current index in the array of waveheaders
    LPWAVEHDR       pCurrentWaveHdr;

    bytesToRead = g_audioBytesPerBuffer; 
    framesToRead = g_audioFramesPerBuffer;  

    memset(&waveHeader, 0, sizeof(WAVEHDR) * BAE_WAVEIN_NUM_BUFFERS);
    memset(&audioStatus, 0, (long)sizeof(MMTIME));
    audioStatus.wType = TIME_BYTES; // get byte position
    
    error = waveInGetPosition(g_captureSound, &audioStatus, sizeof(MMTIME));
    currentPos = audioStatus.u.cb;
    lastPos = currentPos + g_audioBytesPerBuffer;

    // set up all the capture buffers
    for (count = 0; count < BAE_WAVEIN_NUM_BUFFERS; count++)
    {
        waveHeader[count].lpData = (char *)g_audioBufferBlock[count];
        waveHeader[count].dwBufferLength = g_audioBytesPerBuffer;
        waveHeader[count].dwFlags       = 0;
        waveHeader[count].dwLoops       = 0;
        error = waveInPrepareHeader(g_captureSound, &waveHeader[count], (long)sizeof(WAVEHDR));
    }

    // add all the capture buffers
    for (count = 0; count < BAE_WAVEIN_NUM_BUFFERS; count++)
    {
        error = waveInAddBuffer(g_captureSound, &waveHeader[count], sizeof(WAVEHDR));
    }


    // now run this loop to do the capture.  
    // we wait for enough samples to be captured to fill one capture buffer,
    // callback with the captured data, and put the buffer back in the queue.

    waveHeaderCount = 0; // which buffer we're processing
    while (g_captureShutdown == FALSE)
    {   
        // wait for the device to record enough data to fill our capture buffer

        while (currentPos < lastPos)
        {
            if (g_captureShutdown == TRUE)
            {
                break;
            }

            BAE_SleepFrameThread(threadContext, BAE_WAVEIN_SOUND_PERIOD);       // in ms
                
            error = waveInGetPosition(g_captureSound, &audioStatus, sizeof(MMTIME));
            currentPos = audioStatus.u.cb;
        }

        lastPos += bytesToRead;


        if (g_captureShutdown == FALSE)
        {
            // then process the captured data

            // this is the data buffer for the current capture buffer
            pCurrentWaveHdr = &waveHeader[waveHeaderCount];

            if (pCurrentWaveHdr->dwFlags & WHDR_DONE)
            {
                DWORD dwBytesRecorded = pCurrentWaveHdr->dwBytesRecorded;
                LPSTR lpData = pCurrentWaveHdr->lpData;

                // callback with the captured data
                (*g_captureDoneProc)(threadContext, DATA_READY_CAPTURE, &lpData, (void *)&dwBytesRecorded);

                // add the buffer back into the queue
                error = waveInAddBuffer(g_captureSound, pCurrentWaveHdr, sizeof(WAVEHDR));

                // increment to the next wavehdr
                waveHeaderCount++;
                if (waveHeaderCount == BAE_WAVEIN_NUM_BUFFERS)
                {
                    waveHeaderCount = 0;
                }
            }
        }
    } // while

    waveInReset(g_captureSound);        // stop all audio before unpreparing headers

    // unprepare headers
    for (count = 0; count < BAE_WAVEIN_NUM_BUFFERS; count++)
    {
        error = waveInUnprepareHeader(g_captureSound, &waveHeader[count], (long)sizeof(WAVEHDR));
    }
    // do this here, when we can't call it anymore.
    g_captureDoneProc = NULL;
}

    
// Aquire and enabled audio card
// return 0 if ok, -1 if failed
int BAE_AquireAudioCapture(void *threadContext, unsigned long sampleRate, unsigned long channels, unsigned long bits,
                            unsigned long *pCaptureHandle)
{
    MMRESULT        theErr;
    WAVEINCAPS      caps;
    WAVEFORMATEX    format;
    UINT            deviceID;

    g_bitSize = bits;
    g_channels = channels;
    g_sampleRate = sampleRate;

    if (pCaptureHandle)
    {
        *pCaptureHandle = 0L;
    }
    deviceID = WAVE_MAPPER;
    if (g_soundDeviceIndex)
    {
        deviceID = g_soundDeviceIndex - 1;
    }
    
    theErr = waveInGetDevCaps(deviceID, &caps, sizeof(WAVEINCAPS));
    
    if (theErr == MMSYSERR_NOERROR)
    {           
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nSamplesPerSec = sampleRate;
        format.wBitsPerSample = (WORD)bits;
        format.nChannels = (WORD)channels;

        format.nBlockAlign = (WORD)((format.wBitsPerSample * format.nChannels) / 8);
        format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
        format.cbSize = 0;
    
        theErr = waveInOpen(&g_captureSound, deviceID, &format, 
                                0L/*(DWORD)PV_AudioCaptureCallback*/, (DWORD)threadContext, CALLBACK_NULL /*CALLBACK_FUNCTION*/);
        
        if (theErr == MMSYSERR_NOERROR)
        {           
            g_captureShutdown = FALSE;

            if (pCaptureHandle)
            {
                *pCaptureHandle = (unsigned long)g_captureSound;
            }
        }
        else 
        {
            BAE_ReleaseAudioCapture(threadContext);
        }
    }

    return (theErr == MMSYSERR_NOERROR) ? 0 : -1;
}

// Given the capture hardware is working, fill a buffer with some data. This call is
// asynchronous. When this buffer is filled, the function done will be called.
// returns 0 for success, -1 for failure
//int BAE_StartAudioCapture(void *buffer, unsigned long bufferSize, BAE_CaptureDone done, void *callbackContext)
int BAE_StartAudioCapture(BAE_CaptureDone done, void *callbackContext)
{
    long error = 0;
    int i;

    // start capture
    g_captureDoneProc = done;
    g_captureDoneContext = callbackContext;
        
    // our capture buffer will hold this many frames of sampled audio data
    g_audioFramesPerBuffer = BAE_WAVEIN_FRAMES_PER_BUFFER;  

    // calculate the number of bytes per capture buffer
    if (g_bitSize == 8)
    {
        g_audioBytesPerBuffer = (sizeof(char) * g_audioFramesPerBuffer);
    }
    else
    {
        g_audioBytesPerBuffer = (sizeof(short int) * g_audioFramesPerBuffer);
    }
    g_audioBytesPerBuffer *= g_channels;

    // allocate the capture data buffers
    // this could really be done in initialiation...?
    for (i = 0; i < BAE_WAVEIN_NUM_BUFFERS; i++)                                        
    {         
        g_audioBufferBlock[i] = BAE_Allocate(g_audioBytesPerBuffer);
        if (g_audioBufferBlock[i] == NULL)
        {
            error = -1; // something is wrong
            break; 
        }
    } // for

    if (error == 0)
    {
        // create thread to manage audio capture
        error = BAE_CreateFrameThread(callbackContext, PV_AudioWaveInFrameThread);

        if (error == 0)
        {
            error = BAE_ResumeAudioCapture();
        }
    }

    if (error == 0)
    {
        // $$kk: 10.12.98: added this so we can restart capture
        g_captureShutdown = FALSE;
    }

    return (error == 0) ? 0 : -1;
}


// stop the capture hardware
int BAE_StopAudioCapture(void)
{
    MMRESULT    theErr;
    int i;

    if (g_captureSound)
    {
        g_captureShutdown = TRUE;

        // stop streaming data
        theErr = BAE_PauseAudioCapture();

        // release all buffers
        theErr = waveInReset(g_captureSound);

        // deallocate the capture data buffers
        // this could really be done in initialiation...?
        // is waveInReset synchronous?  otherwise something bad could happen....
        for (i = 0; i < BAE_WAVEIN_NUM_BUFFERS; i++)                                        
        {         
            BAE_Deallocate(g_audioBufferBlock[i]);
        } // for
    }
    return 0;
}

int BAE_PauseAudioCapture(void)
{
    if (g_captureSound)
    {
        // stop streaming data
        waveInStop(g_captureSound);
    }
    return 0;
}

int BAE_ResumeAudioCapture(void)
{
    if (g_captureSound)
    {
        // start streaming data
        waveInStart(g_captureSound);
    }
    return 0;
}



// Release and free audio card.
// return 0 if ok, -1 if failed.
int BAE_ReleaseAudioCapture(void *threadContext)
{
    threadContext;
    if (g_captureSound)
    {
        //BAE_StopAudioCapture();
        
        // stop streaming data
        waveInStop(g_captureSound);

        // release all buffers
        waveInReset(g_captureSound);

        while (waveInClose(g_captureSound) == WAVERR_STILLPLAYING)
        {
            ;
            /*BAE_StopAudioCapture();*/
        }
        g_captureSound = NULL;
    }
    return 0;
}


// number of devices. ie different versions of the BAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_MaxCaptureDevices(void)
{
    return waveInGetNumDevs();
}


// set the current device. device is from 0 to BAE_MaxDevices()
// NOTE:    This function needs to function before any other calls may have happened.
//          Also you will need to call BAE_ReleaseAudioCard then BAE_AquireAudioCard
//          in order for the change to take place.
void BAE_SetCaptureDeviceID(long deviceID, void *deviceParameter)
{
    if (deviceID < BAE_MaxCaptureDevices())
    {
        g_soundDeviceIndex = deviceID + 1;
    }
    deviceParameter;
}


// return current device ID
// NOTE: This function needs to function before any other calls may have happened.
long BAE_GetCaptureDeviceID(void *deviceParameter)
{
    deviceParameter;
    if (g_soundDeviceIndex)
    {   // if its non-zero then we've set it, so return value - 1
        return g_soundDeviceIndex - 1;
    }
    return 1;
}


// get deviceID name 
// NOTE:    This function needs to function before any other calls may have happened.
//          The names returned in this function are platform specific.
void BAE_GetCaptureDeviceName(long deviceID, char *cName, unsigned long cNameLength)
{
    WAVEINCAPS      caps;
    MMRESULT        theErr;

    if (deviceID < BAE_MaxCaptureDevices())
    {
        if (deviceID == 0)
        {
            deviceID = (long)WAVE_MAPPER;
        }

        theErr = waveInGetDevCaps((UINT)deviceID, &caps, sizeof(WAVEINCAPS));
        if ((theErr == MMSYSERR_NOERROR) && cName && cNameLength)
        {
            strncpy(cName, caps.szPname, cNameLength-1);                                  
            cName[cNameLength] = 0;
        }
    }
}


// return the number of frames in the capture buffer
// (should make this settable?)
unsigned long BAE_GetCaptureBufferSizeInFrames()
{
    return BAE_WAVEIN_FRAMES_PER_BUFFER;
}


// return the number of buffers used.
int BAE_GetCaptureBufferCount()
{
    return BAE_WAVEIN_NUM_BUFFERS;
}


// return the number of samples captured at the device
// $$kk: 10.15.98: need to implement!
unsigned long BAE_GetDeviceSamplesCapturedPosition()
{
    return 0L;
}

// EOF of BAE_API_WinOS_Capture.c
