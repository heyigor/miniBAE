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
**  BAE_API_IOS.c
**
**  This provides platform specfic functions for the Macintosh OS. This interface
**  for BAE is for the Sound Manager and sends buffer slices
**  through the multimedia system.
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
**  7/22/97     Created
**  11/11/97    Added BAE_MaxDevices & BAE_SetDeviceID & BAE_GetDeviceID & BAE_GetDeviceName
**  11/14/97    Removed multiple defined variable "mAudioFramesToGenerate"
**  12/16/97    Modified BAE_GetDeviceID and BAE_SetDeviceID to pass a device parameter pointer
**              that is specific for that device.
**  1/9/98      Added BAE_FileDelete
**  2/13/98     Modified BAE_AquireAudioCard to handle different sample rates
**  3/17/98     Added BAE_Is8BitSupported
**  3/23/98     Fixed a bug in BAE_GetDeviceName that wrote the name into space. Thanks DavidZ
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
**  12/17/98    Added BAE_GetHardwareBalance & BAE_SetHardwareBalance
**  3/5/99      Changed context to threadContext to reflect what is really is used
**              in the system.
**  3/31/99     Added constant for Netscape plugin memory.
**  5/7/99      Added some post flight memory tests to keep us from the hairy
**              edge of MacOS lala land.
**  8/23/99     MSD:  fixed terminating zero bug in BAE_GetDeviceName()
**  9/3/99      Changed the BAE_FRAMES_PER_BLOCK to 2, in case Apple changes to
**              smaller buffers in the Sound Manager.
**  2/1/2000    Added new MacOS X "approved" sound double buffering. Set
**              USE_MAC_OS_X to TRUE to use the new callback method.
**              Note: its slower.
**  2/2/2000    Added a buffer clear function to eliminate a startup click. Changed
**              name in BAE_GetDeviceName to support new method of playback.
**  7/5/2000    Changed BAE_Allocate/BAE_Deallocate to use MacOS temporary
**              memory. This has the effect of expanding the current application
**              heap.
**              Fixed a recursive bug in BAE_Allocate. oops.
**  7/25/2000   Changed BAE_SizeOfPointer to use the new way to get size of temp
**              handles.
**              Placed a USE_TEMP_MEMORY flag to control if we use the temp memory
**              API instead of allocating through our natural heap.
**              Added PV_IsVirtualMemoryAvailable & PV_LockMemory & PV_UnlockMemory
**  9/29/2000   Added condition compiler flags for support on MacOS X
**  10/2/2000   Set default condition to not use SndDoubleBuffer. This allows us to
**              to run under X without recompiling. We run in the "classic" mode.
**  10/5/2000   Fixed a function name change in BAE_AquireAudioCard
**  2/14/2001   sh  When compiled for Carbon (MacOS9/X) BAE_GetDeviceName now
**                  reports a better name that's not specific to OS9/X
*/
/*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <assert.h>
#include <malloc/malloc.h>

#include <AudioUnit/AudioUnit.h>
#include <CoreFoundation/CFURL.h>

#include "BAE_API.h"


#undef FALSE
#undef TRUE
#define TRUE                            1
#define FALSE                           0


#define BAE_FRAMES_PER_BLOCK            1 // how much ms latancy can we handle (in ms)

typedef struct
{
    // hardware volume in BAE scale
    short int                       mUnscaledVolume;
    // balance scale -256 to 256 (left to right)
    short int                       mBalance;
    
    // size of audio buffers in bytes
    long                            mAudioByteBufferSize;
    
    char                            mDataReady;
    char                            mDonePlaying;
    char                            mShutdownDoubleBuffer;
    
    // number of samples per audio frame to generate
    long                            mAudioFramesToGenerate;
    
    // How many audio frames to generate at one time
    unsigned int                    mSynthFramesPerBlock;
    unsigned long                   mSamplesPlayed;

    AudioUnit                       mAudioUnit;
    AudioStreamBasicDescription     mFormat;

} AudioControlData;

static AudioControlData* sHardwareChannel;

// This file contains API's that need to be defined in order to get BAE (IgorAudio)
// to link and compile.

// **** System setup and cleanup functions
// Setup function. Called before memory allocation, or anything serious. Can be used to
// load a DLL, library, etc.
// return 0 for ok, or -1 for failure
int BAE_Setup(void)
{
   return(0);
}

// Cleanup function. Called after all memory and the last buffers are deallocated, and
// audio hardware is released. Can be used to unload a DLL, library, etc.
// return 0 for ok, or -1 for failure
int BAE_Cleanup(void)
{
   return(0);
}

// **** Memory management
// allocate a block of locked, zeroed memory. Return a pointer
void *BAE_Allocate(unsigned long size)
{
    assert(size > 0);
    void* data = (void*)calloc(1, size);
    return data;
}

// dispose of memory allocated with BAE_Allocate
void BAE_Deallocate(void *memoryBlock)
{
    free(memoryBlock);
}

// return memory used
unsigned long BAE_GetSizeOfMemoryUsed(void)
{
//  return g_memory_buoy;
   return(0);
}

// return max memory used
unsigned long BAE_GetMaxSizeOfMemoryUsed(void)
{
//  return g_memory_buoy_max;
   return(0);
}

// Given a memory pointer and a size, validate of memory pointer is a valid memory address
// with at least size bytes of data avaiable from the pointer.
// This is used to determine if a memory pointer and size can be accessed without
// causing a memory protection
// fault.
// return 0 for valid, or 1 for bad pointer, or 2 for not supported.
int BAE_IsBadReadPointer(void *memoryBlock, unsigned long size)
{
   memoryBlock = memoryBlock;
   size        = size;
   return(2);           // not supported, so this assumes that we don't have memory protection and will
   // not get an access violation when accessing memory outside of a valid memory block
}

// this will return the size of the memory pointer allocated with BAE_Allocate. Return
// 0 if you don't support this feature
unsigned long BAE_SizeOfPointer(void *memoryBlock)
{
    return malloc_size(memoryBlock);
}

// block move memory. This is basicly memmove, but its exposed to take advantage of
// special block move speed ups, various hardware has available.
void BAE_BlockMove(void *source, void *dest, unsigned long size)
{
    assert(dest != NULL && source != NULL);
    memmove(dest, source, size);
}

// **** Audio Card modifiers
// Return 1 if stereo hardware is supported, otherwise 0.
int BAE_IsStereoSupported(void)
{
    return 1;
}

// Return 1, if sound hardware support 16 bit output, otherwise 0.
int BAE_Is16BitSupported(void)
{
    return 1;
}

// Return 1, if sound hardware support 8 bit output, otherwise 0.
int BAE_Is8BitSupported(void)
{
    return 1;
}

// returned balance is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
short int BAE_GetHardwareBalance(void)
{
    if (sHardwareChannel)
    {
        return sHardwareChannel->mBalance;
    }
    return 0;
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
   if (sHardwareChannel)
   {
       sHardwareChannel->mBalance = balance;
       BAE_SetHardwareVolume(sHardwareChannel->mUnscaledVolume);
   }
}

// returned volume is in the range of 0 to 256
short int BAE_GetHardwareVolume(void)
{
    if (sHardwareChannel)
    {
        return sHardwareChannel->mUnscaledVolume;
    }
    return 256;
}

// theVolume is in the range of 0 to 256
void BAE_SetHardwareVolume(short int newVolume)
{
   unsigned long volume;
   short int     lbm, rbm;

   // pin volume
   if (newVolume > 256)
   {
      newVolume = 256;
   }
   if (newVolume < 0)
   {
      newVolume = 0;
   }
    sHardwareChannel->mUnscaledVolume = newVolume;

   // calculate balance multipliers
   if (sHardwareChannel->mBalance > 0)
   {
      lbm = 256 - sHardwareChannel->mBalance;
      rbm = 256;
   }
   else
   {
      lbm = 256;
      rbm = 256 + sHardwareChannel->mBalance;
   }

   volume = (((newVolume * lbm) / 256) << 16L) | ((newVolume * rbm) / 256);             // keep scale at 0 to 256
#warning BAE_SetHardwareVolume not implemented
}

// **** Timing services
// return microseconds
unsigned long BAE_Microseconds(void)
{
   static int           firstTime = TRUE;
   static unsigned long offset    = 0;
   struct timeval       tv;

   if (firstTime)
   {
      gettimeofday(&tv, NULL);
      offset    = tv.tv_sec;
      firstTime = FALSE;
   }
   gettimeofday(&tv, NULL);
   return(((tv.tv_sec - offset) * 1000000UL) + tv.tv_usec);
}

// wait or sleep this thread for this many microseconds
void BAE_WaitMicroseconds(unsigned long usec)
{
   usleep(usec);
}

int BAE_NewMutex(BAE_Mutex* lock, char *name, char *file, int lineno)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t *) BAE_Allocate(sizeof(pthread_mutex_t));
    pthread_mutexattr_t attrib;
    pthread_mutexattr_init(&attrib);
    pthread_mutexattr_settype(&attrib, PTHREAD_MUTEX_RECURSIVE);
    // Create reentrant (within same thread) mutex.
    pthread_mutex_init(pMutex, &attrib);
    pthread_mutexattr_destroy(&attrib);
    *lock = (BAE_Mutex) pMutex;
    return 1; // ok
}

void BAE_AcquireMutex(BAE_Mutex lock)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t*) lock;
    pthread_mutex_lock(pMutex);
}

void BAE_ReleaseMutex(BAE_Mutex lock)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t*) lock;
    pthread_mutex_unlock(pMutex);
}

void BAE_DestroyMutex(BAE_Mutex lock)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t*) lock;
    pthread_mutex_destroy(pMutex);
    BAE_Deallocate(pMutex);
}


// If no thread support, this will be called during idle times. Used for host
// rendering without threads.
void BAE_Idle(void *userContext)
{
//  userContext = userContext;
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
    
    if ((fileNameSource) && (fileNameDest))
    {
        dest = (char *)fileNameDest;
        src  = (char *)fileNameSource;
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
    int file;
    
    file = open((char *)fileName, O_CREAT | O_TRUNC | O_RDWR);
    if (file != -1)
    {
        close(file);
    }
    return((file != -1) ? 0 : -1);  
}

long BAE_FileDelete(void *fileName)
{
    if (fileName)
    {
        if (remove(fileName))
        {
            return(0);
        }
    }
    return(-1);
}


// Open a file
// Return -1 if error, otherwise file handle
long BAE_FileOpenForRead(void *fileName)
{
    CFURLRef dataURL;
    CFStringRef cfResourceName;
    char fullPath[PATH_MAX];
    Boolean ok = FALSE;

    if (fileName)
    {
        fullPath[0] = 0;
        cfResourceName = CFStringCreateWithCString(NULL, fileName, kCFStringEncodingASCII);
        dataURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), cfResourceName, NULL, NULL);
        if (dataURL)
        {
            ok = CFURLGetFileSystemRepresentation(dataURL, TRUE, (UInt8*)fullPath, PATH_MAX);
            CFRelease(dataURL); 
        }
        CFRelease(cfResourceName);

        if ((ok == TRUE) && (fullPath[0]))
        {
            return (long)open(fullPath, O_RDONLY);
        }
    }
    return -1;
}

long BAE_FileOpenForWrite(void *fileName)
{
    if (fileName)
    {
        return(open((char *)fileName, O_WRONLY | O_CREAT | O_TRUNC));
    }
    return(-1);
}

long BAE_FileOpenForReadWrite(void *fileName)
{
    if (fileName)
    {
        return(open((char *)fileName, O_RDWR));
    }
    return(-1);
}

// Close a file
void BAE_FileClose(long fileReference)
{
    close(fileReference);
}

// Read a block of memory from a file.
// Return -1 if error, otherwise length of data read.
long BAE_ReadFile(long fileReference, void *pBuffer, long bufferLength)
{
    if ((pBuffer) && (bufferLength))
    {
        return(read(fileReference, (char *)pBuffer, bufferLength));
    }
    return(-1);
}

// Write a block of memory from a file
// Return -1 if error, otherwise length of data written.
long BAE_WriteFile(long fileReference, void *pBuffer, long bufferLength)
{
    if ((pBuffer) && (bufferLength))
    {
        return(write(fileReference, (char *)pBuffer, bufferLength));
    }
    return(-1);
}

// set file position in absolute file byte position
// Return -1 if error, otherwise 0.
long BAE_SetFilePosition(long fileReference, unsigned long filePosition)
{
    return((lseek(fileReference, filePosition, SEEK_SET) == -1) ? -1 : 0);
}

// get file position in absolute file bytes
unsigned long BAE_GetFilePosition(long fileReference)
{
    return(lseek(fileReference, 0, SEEK_CUR));
}

// get length of file
unsigned long BAE_GetFileLength(long fileReference)
{
    unsigned long pos;

    pos = lseek(fileReference, 0, SEEK_END);
    lseek(fileReference, 0, SEEK_SET);
    return(pos);
}

// set the length of a file. Return 0, if ok, or -1 for error
int BAE_SetFileLength(long fileReference, unsigned long newSize)
{
    return -1;
}

// This function is called at render time with w route bus flag. If there's
// no change, return currentRoute, other wise return one of audiosys.h route values.
// This will change an active rendered's voice placement.
void BAE_ProcessRouteBus(int currentRoute, long *pChannels, int count)
{
}

static void PV_ClearOutputBuffer(void *pBuffer, short int channels, short int bits, unsigned long frames)
{
    short int count;
    char      *dest8;
    short int *dest16;
    
    if (bits == 16)
    {
        // use 16 bit output
        dest16 = (short int *)pBuffer;
        for (count = 0; count < frames / 4; count++)
        {
            *dest16++ = 0;
            *dest16++ = 0;
            *dest16++ = 0;
            *dest16++ = 0;
            if (channels == 2)
            {
                // this is a slow way to do this!
                *dest16++ = 0;
                *dest16++ = 0;
                *dest16++ = 0;
                *dest16++ = 0;
            }
        }
    }
    else
    {
        // use 8 bit output
        dest8 = (char *)pBuffer;
        for (count = 0; count < frames / 4; count++)
        {
            *dest8++ = 0x80;
            *dest8++ = 0x80;
            *dest8++ = 0x80;
            *dest8++ = 0x80;
            if (channels == 2)
            {
                // this is a slow way to do this!
                *dest8++ = 0x80;
                *dest8++ = 0x80;
                *dest8++ = 0x80;
                *dest8++ = 0x80;
            }
        }
    }
}


// Return the number of 11 ms buffer blocks that are built at one time.
int BAE_GetAudioBufferCount(void)
{
   return(sHardwareChannel->mSynthFramesPerBlock);
}

// Return the number of bytes used for audio buffer for output to card
long BAE_GetAudioByteBufferSize(void)
{
   return(sHardwareChannel->mAudioByteBufferSize);
}

// Mute/unmute audio. Shutdown amps, etc.
// return 0 if ok, -1 if failed
int BAE_Mute(void)
{
   return(0);
}

int BAE_Unmute(void)
{
   return(0);
}

static OSStatus PV_AudioUnitOutputCallback(void *inUserData, 
                                       AudioUnitRenderActionFlags *ioActionFlags, 
                                       const AudioTimeStamp *inTimeStamp, 
                                       UInt32 inBusNumber, 
                                       UInt32 inNumberFrames, 
                                       AudioBufferList *ioData)
{
    AudioControlData *THIS = (AudioControlData*)inUserData;

    THIS->mSamplesPlayed += sHardwareChannel->mAudioFramesToGenerate;
    if (THIS->mShutdownDoubleBuffer == FALSE)
    {
        if (THIS->mDataReady)
        {
            THIS->mDonePlaying = FALSE;
            int maxBuffers = inNumberFrames / THIS->mAudioFramesToGenerate;
            if ((maxBuffers * THIS->mAudioFramesToGenerate) != inNumberFrames)
            {
                printf("Trying to generate %ld frames, but only can generate %ld\n", inNumberFrames,
                       THIS->mAudioFramesToGenerate);
                assert(TRUE);
            }

            for (int count = 0; count < maxBuffers; count++)
            {
                char* buffer = (char*)ioData->mBuffers[0].mData;
                buffer += (count * THIS->mAudioByteBufferSize);
                // Generate one frame audio
                BAE_BuildMixerSlice(NULL, buffer, THIS->mAudioByteBufferSize, THIS->mAudioFramesToGenerate);
            }
            ioData->mBuffers[0].mDataByteSize = THIS->mAudioByteBufferSize * maxBuffers;
        }
        else
        {
            PV_ClearOutputBuffer(ioData->mBuffers[0].mData, THIS->mFormat.mChannelsPerFrame,
                                 THIS->mFormat.mBitsPerChannel, inNumberFrames);
            ioData->mBuffers[0].mDataByteSize = inNumberFrames;
        }
    }
    return 0;
}

const int kOutputBus = 0;
const int kInputBus = 1;


// **** Audio card support
// Aquire and enabled audio card
// return 0 if ok, -1 if failed
int BAE_AquireAudioCard(void *threadContext, unsigned long sampleRate, unsigned long channels, unsigned long bits)
{
    int           flag;
    long          bufferSize;
    OSStatus      err = noErr;

    sHardwareChannel = BAE_Allocate(sizeof(AudioControlData));
    if (sHardwareChannel == NULL)
    {
        return -1;
    }

    flag = -1;                                   // failure
    sHardwareChannel->mShutdownDoubleBuffer = FALSE;
    sHardwareChannel->mBalance = 0;
    sHardwareChannel->mUnscaledVolume = 256;

    sHardwareChannel->mAudioFramesToGenerate = BAE_GetMaxSamplePerSlice();        // get number of frames per sample rate slice
    switch (sampleRate)
    {
        default:
            assert(TRUE);
            break;
        case 48000:
        case 44100:
        case 24000:
        case 22050:
        case 11025:
        case 16000:
        case 8000:
            break;
    }
    OSStatus status = 0;
    if ((sHardwareChannel->mAudioFramesToGenerate) && ((err == noErr)))
    {
        bufferSize  = (bits == 8) ? sizeof(char) : sizeof(short int);
        bufferSize *= channels;

        // Describe audio component
        AudioComponentDescription desc;
        desc.componentType = kAudioUnitType_Output;
        desc.componentSubType = kAudioUnitSubType_RemoteIO;
        desc.componentFlags = 0;
        desc.componentFlagsMask = 0;
        desc.componentManufacturer = kAudioUnitManufacturer_Apple;
        
        AudioComponent comp = AudioComponentFindNext(NULL, &desc);
        AudioComponentInstanceNew(comp, &sHardwareChannel->mAudioUnit);

        // Enable IO for playback
        status = AudioUnitSetProperty(sHardwareChannel->mAudioUnit, 
                                      kAudioOutputUnitProperty_EnableIO, 
                                      kAudioUnitScope_Output, 
                                      kOutputBus,
                                      &flag, 
                                      sizeof(flag));

        // Describe format
        sHardwareChannel->mFormat.mSampleRate       = sampleRate;
        sHardwareChannel->mFormat.mFormatID         = kAudioFormatLinearPCM;
        sHardwareChannel->mFormat.mFormatFlags      = kAudioFormatFlagIsPacked;
        if (bits != 8)
        {
            sHardwareChannel->mFormat.mFormatFlags  |= kAudioFormatFlagIsSignedInteger;
        }
        sHardwareChannel->mFormat.mFramesPerPacket  = 1;
        sHardwareChannel->mFormat.mChannelsPerFrame = channels;
        sHardwareChannel->mFormat.mBitsPerChannel   = bits;
        sHardwareChannel->mFormat.mBytesPerPacket   = channels * (bits / 8);
        sHardwareChannel->mFormat.mBytesPerFrame    = channels * (bits / 8);
        
        // Apply format
        status = AudioUnitSetProperty(sHardwareChannel->mAudioUnit, 
                                      kAudioUnitProperty_StreamFormat, 
                                      kAudioUnitScope_Output, 
                                      kInputBus, 
                                      &sHardwareChannel->mFormat, 
                                      sizeof(AudioStreamBasicDescription));
        status = AudioUnitSetProperty(sHardwareChannel->mAudioUnit, 
                                      kAudioUnitProperty_StreamFormat, 
                                      kAudioUnitScope_Input, 
                                      kOutputBus, 
                                      &sHardwareChannel->mFormat, 
                                      sizeof(AudioStreamBasicDescription));
        
        // Set output callback
        AURenderCallbackStruct proc;
        proc.inputProc = PV_AudioUnitOutputCallback;
        proc.inputProcRefCon = sHardwareChannel;
        status = AudioUnitSetProperty(sHardwareChannel->mAudioUnit, 
                                      kAudioUnitProperty_SetRenderCallback, 
                                      kAudioUnitScope_Global, 
                                      kOutputBus,
                                      &proc, 
                                      sizeof(AURenderCallbackStruct));

        
        bufferSize *= sHardwareChannel->mAudioFramesToGenerate;
        // store just the size of the audio buffer frame
        sHardwareChannel->mAudioByteBufferSize = bufferSize;

        // set max buffer slice size
        status = AudioUnitSetProperty(sHardwareChannel->mAudioUnit, 
                                      kAudioUnitProperty_MaximumFramesPerSlice, 
                                      kAudioUnitScope_Global, 0, 
                                      (UInt32*)&sHardwareChannel->mAudioFramesToGenerate, 
                                      sizeof(UInt32));
        
        // we're going to build many buffers at a time
        sHardwareChannel->mSynthFramesPerBlock = BAE_FRAMES_PER_BLOCK;
        sHardwareChannel->mSamplesPlayed = 0;
        sHardwareChannel->mDataReady = FALSE;
        sHardwareChannel->mDonePlaying = FALSE;

        status = AudioUnitInitialize(sHardwareChannel->mAudioUnit);
                
        flag = 0;                      // ok
        sHardwareChannel->mDataReady = TRUE;
        status = AudioOutputUnitStart(sHardwareChannel->mAudioUnit);
        if (status)
        {
            printf("not started\n");
            flag = 1;
        }
    }

    if (flag)
    {
        // something failed
        BAE_ReleaseAudioCard(threadContext);
    }
    return(flag);
}

// Release and free audio card.
// return 0 if ok, -1 if failed.
int BAE_ReleaseAudioCard(void *threadContext)
{
    //  Kill sounds currently playing in this channel
    if (sHardwareChannel)
    {
        sHardwareChannel->mShutdownDoubleBuffer = TRUE;
        sHardwareChannel->mDataReady = FALSE;

        AudioOutputUnitStop(sHardwareChannel->mAudioUnit);
        AudioUnitUninitialize(sHardwareChannel->mAudioUnit);
        
        BAE_Deallocate(sHardwareChannel);
        sHardwareChannel = NULL;
    }
    return 0;
}

// return device position in samples
unsigned long BAE_GetDeviceSamplesPlayedPosition(void)
{
   return(sHardwareChannel->mSamplesPlayed);
}


// number of devices. ie different versions of the BAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_MaxDevices(void)
{
   return(1);
}

// set the current device. device is from 0 to BAE_MaxDevices()
// NOTE:    This function needs to function before any other calls may have happened.
//          Also you will need to call BAE_ReleaseAudioCard then BAE_AquireAudioCard
//          in order for the change to take place.
void BAE_SetDeviceID(long deviceID, void *deviceParameter)
{
   deviceID;
   deviceParameter;
}

// return current device ID
// NOTE: This function needs to function before any other calls may have happened.
long BAE_GetDeviceID(void *deviceParameter)
{
   deviceParameter;
   return(0);
}

// get deviceID name
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
   static char id[] =
   {
      "MacOS,Cocoa,AudioQueue"
   };
   unsigned long length;

   if ((cName) && (cNameLength))
   {
      cName[0] = 0;
      if (deviceID == 0)
      {
         length = sizeof(id) + 1;
         if (length > cNameLength)
         {
            length = cNameLength;
         }
         BAE_BlockMove((void *)id, (void *)cName, length);
         cName[length - 1] = '\0';
      }
   }
}


// EOF of BAE_API_MacOS.c
