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
**	BAE_API.h
**
**	This provides platform specfic functions for BAE
**
**	© Copyright 1995-2000 Beatnik, Inc, All Rights Reserved.
**	Written by Steve Hales
**
**	Beatnik products contain certain trade secrets and confidential and
**	proprietary information of Beatnik.  Use, reproduction, disclosure
**	and distribution by any means are prohibited, except pursuant to
**	a written license from Beatnik. Use of copyright notice is
**	precautionary and does not imply publication or disclosure.
**
**	Restricted Rights Legend:
**	Use, duplication, or disclosure by the Government is subject to
**	restrictions as set forth in subparagraph (c)(1)(ii) of The
**	Rights in Technical Data and Computer Software clause in DFARS
**	252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
**	Computer Software--Restricted Rights at 48 CFR 52.227-19, as
**	applicable.
**
**	Confidential-- Internal use only
**
**	History	-
**	7/14/97		Created
**	7/22/97		Added BAE_GetAudioByteBufferSize
**	11/11/97	Added BAE_MaxDevices & BAE_SetDeviceID & BAE_GetDeviceID & BAE_GetDeviceName
**	12/16/97	Modified BAE_GetDeviceID and BAE_SetDeviceID to pass a device parameter pointer
**				that is specific for that device.
**	1/9/98		Added BAE_FileDelete
**	2/13/98		Changed BAE_AquireAudioCard to use unsigned longs
**	3/17/98		Added BAE_Is8BitSupported
**
**	6/5/98		Jim Nitchals RIP	1/15/62 - 6/5/98
**				I'm going to miss your irreverent humor. Your coding style and desire
**				to make things as fast as possible. Your collaboration behind this entire
**				codebase. Your absolute belief in creating the best possible relationships 
**				from honesty and integrity. Your ability to enjoy conversation. Your business 
**				savvy in understanding the big picture. Your gentleness. Your willingness 
**				to understand someone else's way of thinking. Your debates on the latest 
**				political issues. Your generosity. Your great mimicking of cartoon voices. 
**				Your friendship. - Steve Hales
**
**	6/28/98		Added Capture API
**	7/23/98		Added a new parameter to BAE_AquireAudioCapture
**				Added BAE_PauseAudioCapture & BAE_ResumeAudioCapture
**	7/31/98		Added BAE_GetCaptureDeviceName
**	8/13/98		Changed BAE_CaptureDone proc pointer to be void * parameters rather than
**				the direct objects.
**
** JAVASOFT
** 10.09.98		Changed BAE_StartAudioCapture parameters
** 10.13.98		added BAE_GetCaptureBufferSizeInFrames
** 10.14.98		added BAE_GetDeviceSamplesCapturedPosition (need to implement!)
** 10.15.98		added BAE_GetCaptureBufferCount
** 
**	12/17/98	Added BAE_GetHardwareBalance & BAE_SetHardwareBalance
**	3/5/99		Changed context to threadContext to reflect what is really is used
**				in the system.
**	5/28/99		Changed comment on BAE_BlockMove.
**	7/12/99		Fixed spelling in BAE_WaitMicroseconds
**	7/13/99		Renamed BAE to BAE
**	7/14/99		Added backward compatibility macros to BAE_ API
**	8/28/99		Added BAEWinOSParameters, and changed BAE_SetDeviceID & BAE_GetDeviceID
**				to read from that structure for the Windows platform
**	11/27/99	Added BAE_GetSizeOfMemoryUsed
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	2/28/00		Added BAE_GetMaxSizeOfMemoryUsed
**	10/17/2000	Added BAE_Idle
*/
/*****************************************************************************/

#ifndef BAE_API
#define BAE_API

#ifdef __cplusplus
	extern "C" {
#endif

// This file contains API's that need to be defined in order to get BAE (IgorAudio)
// to link and compile.

// Debug tool
void BAE_PrintHexDump(void *address, long length);

// display memory allocated and other info.
// detailLevel of 0 should just display size, 1 should display links and addresses
void BAE_DisplayMemoryUsage(int detailLevel);

// **** System setup and cleanup functions
// Setup function. Called before memory allocation, or anything serious. Can be used to 
// load a DLL, library, etc.
// return 0 for ok, or -1 for failure
int BAE_Setup(void);

// Cleanup function. Called after all memory and the last buffers are deallocated, and
// audio hardware is released. Can be used to unload a DLL, library, etc.
// return 0 for ok, or -1 for failure
int BAE_Cleanup(void);

// **** Memory management
// allocate a block of locked, zeroed memory. Return a pointer
void *BAE_Allocate(unsigned long size);

// dispose of memory allocated with BAE_Allocate
void BAE_Deallocate(void *memoryBlock);

// debug = 0, off debug = 1, on
void BAE_AllocDebug(int debug);

// return total number of bytes allocate at this moment
unsigned long BAE_GetSizeOfMemoryUsed(void);

// return maximum number of bytes allocated at any time
unsigned long BAE_GetMaxSizeOfMemoryUsed(void);

// Given a memory pointer and a size, validate of memory pointer is a valid memory address
// with at least size bytes of data avaiable from the pointer.
// This is used to determine if a memory pointer and size can be accessed without 
// causing a memory protection fault.
// return 0 for valid, or 1 for bad pointer, or 2 for not supported. 
int BAE_IsBadReadPointer(void *memoryBlock, unsigned long size);

// this will return the size of the memory pointer allocated with BAE_Allocate. Return
// 0 if you don't support this feature
unsigned long BAE_SizeOfPointer(void *memoryBlock);

// block move memory. This is basicly memmove, but its exposed to take advantage of
// special block move speed ups, various hardware has available.
// NOTE:	Must use a function like memmove that insures a valid copy in the case
//			of overlapping memory blocks.
void BAE_BlockMove(void *source, void *dest, unsigned long size);

// **** Audio Card modifiers
// Return 1 if stereo hardware is supported, otherwise 0.
int BAE_IsStereoSupported(void);

// Return 1, if sound hardware support 8 bit output, otherwise 0.
int BAE_Is8BitSupported(void);

// Return 1, if sound hardware support 16 bit output, otherwise 0.
int BAE_Is16BitSupported(void);

// returned volume is in the range of 0 to 256. If you're hardware doesn't support this
// range, just scale it.
short int BAE_GetHardwareVolume(void);

// theVolume is in the range of 0 to 256. If you're hardware doesn't support this
// range, just scale it.
void BAE_SetHardwareVolume(short int theVolume);

// returned balance is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
short int BAE_GetHardwareBalance(void);

// 'balance' is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
void BAE_SetHardwareBalance(short int balance);

// **** Timing services
// return microseconds, preferably quantized better than 1000 microseconds, but can live
// with it being as bad as 11000 microseconds.
unsigned long BAE_Microseconds(void);

// wait or sleep this thread for this many microseconds
void BAE_WaitMicroseconds(unsigned long wait);

// **** File support
// NOTE: about the fileName parameters. For "C" string path name based OS's the name
// will be passed in from the C++ API. For MacOS, or structure based file systems, the
// native structure will be passed in. For example: on a MacOS a FSSpec pointer is passed 
// from the C++ API to these various functions.

// Given the fileNameSource that comes from the call BAE_FileXXXX, copy the name
// in the native format to the pointer passed fileNameDest.
void BAE_CopyFileNameNative(void *fileNameSource, void *fileNameDest);

// Create a file, delete orignal if duplicate file name.
// Return -1 if error
long BAE_FileCreate(void *fileName);

// Delete a file. Returns -1 if there's an error, or 0 if ok.
long BAE_FileDelete(void *fileName);

// Open a file
// Return -1 if error, otherwise file handle
long BAE_FileOpenForRead(void *fileName);
long BAE_FileOpenForWrite(void *fileName);
long BAE_FileOpenForReadWrite(void *fileName);

// Close a file
void BAE_FileClose(long fileReference);

// Read a block of memory from a file
// Return -1 if error, otherwise length of data read.
long BAE_ReadFile(long fileReference, void *pBuffer, long bufferLength);

// Write a block of memory from a file
// Return -1 if error, otherwise length of data written.
long BAE_WriteFile(long fileReference, void *pBuffer, long bufferLength);

// set file position in absolute file byte position
long BAE_SetFilePosition(long fileReference, unsigned long filePosition);

// get file position in absolute file bytes
unsigned long BAE_GetFilePosition(long fileReference);

// get length of file
unsigned long BAE_GetFileLength(long fileReference);

// set the length of a file. Return 0, if ok, or -1 for error
int BAE_SetFileLength(long fileReference, unsigned long newSize);

// **** Audio card support
// Aquire and enabled audio card. sampleRate is 44100, 22050, or 11025; channels is 1 or 2;
// bits is 8 or 16.
// return 0 if ok, -1 if failed
int BAE_AquireAudioCard(void *threadContext, unsigned long sampleRate, unsigned long channels, unsigned long bits);

// Release and free audio card.
// return 0 if ok, -1 if failed.
int BAE_ReleaseAudioCard(void *threadContext);

// Mute/unmute audio. Shutdown amps, etc.
// return 0 if ok, -1 if failed
int BAE_Mute(void);
int BAE_Unmute(void);

// returns 0 if not muted, 1 if muted
int BAE_IsMuted(void);

// This function is called at render time with w route bus flag, and the channel volumes.
// If there's no change, do nothing, othewise modify the channel volumes.
// This will change an active rendered's voice placement.

void BAE_ProcessRouteBus(int currentRoute, long *pChannels, int count);

// If no thread support, this will be called during idle times. Used for host
// rendering without threads.
void BAE_Idle(void *userContext);

// Function to enable paused audio rendering thread.
// unlock
void BAE_UnlockAudioFrameThread(void);
// lock
void BAE_LockAudioFrameThread(void);
// block
void BAE_BlockAudioFrameThread(void);

unsigned long BAE_GetDeviceSamplesPlayedPosition(void);

// number of devices. ie different versions of the BAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_MaxDevices(void);


// NOTE:
//		BAE_SetDeviceID & BAE_GetDeviceID take and accept a platform specific pointer for
//		each device type. Currently there are 3 devices:
//			0,	BAE_WAVEOUT							- uses the waveOut device
//			1,	BAE_DIRECTSOUND_PRIMARY_BUFFER		- uses DirectSound with a primary buffer
//			2,	BAE_DIRECTSOUND_SECONDARY_BUFFER	- uses DirectSound with a secondary buffer

//		For Windows 95/98/NT the deviceParameter is defined as
//		WINDOWS
struct BAEWinOSParameters
{
	// actaully a HWND. Only usefull for DirectSound devices
	void			*windowHandle;

	// How many audio frames to generate at one time 
	unsigned long	synthFramesPerBlock;	// NOTE: the waveOut device ignore this parameter
	// How much time to sleep in milliseconds between buffer builds
	unsigned long	audioPeriodSleepTime;

	unsigned long	undefined[32];			// used for future expansion
};
typedef struct BAEWinOSParameters BAEWinOSParameters;

// set the current device. device is from 0 to BAE_MaxDevices()
// NOTE:	This function needs to function before any other calls may have happened.
//			Also you will need to call BAE_ReleaseAudioCard then BAE_AquireAudioCard
//			in order for the change to take place. deviceParameter is a device specific
//			pointer. Pass NULL if you don't know what to use.
void BAE_SetDeviceID(long deviceID, void *deviceParameter);

// return current device ID, and fills in the deviceParameter with a device specific
// pointer. It will pass NULL if there is nothing to use.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_GetDeviceID(void *deviceParameter);

// get deviceID name 
// NOTE:	This function needs to function before any other calls may have happened.
//			Format of string is a zero terminated comma delinated C string.
//			"platform,method,misc"
//	example	"MacOS,Sound Manager 3.0,SndPlayDoubleBuffer"
//			"WinOS,DirectSound,multi threaded"
//			"WinOS,waveOut,multi threaded"
//			"WinOS,VxD,low level hardware"
//			"WinOS,plugin,Director"
void BAE_GetDeviceName(long deviceID, char *cName, unsigned long cNameLength);

// Return the number of microseconds of real time that will be generated when calling
// BAE_BuildMixerSlice.
unsigned long BAE_GetSliceTimeInMicroseconds(void);

// Return the number of buffer blocks that are built at one time. The value returned
// from this function and BAE_GetSliceTimeInMicroseconds will give you the amount
// of lantancy
int BAE_GetAudioBufferCount(void);

// Return the number of bytes used for audio buffer for output to card
long BAE_GetAudioByteBufferSize(void);

// **** Audio Engine feedback functions. These functions are used to direct or get
//		information about the engine.
//
// NOTE:	These are external functions that your audio card code calls to process 
//			a mixer buffer. You call this, don't define it.

// Based upon sample rate, channels, and bit size, build 11 milliseconds worth of mixer
// output, and store it in pAudioBuffer.
// bufferByteLength is assumed to be 256, 512, 1024, or 2048 only. The value is ignored
// at the moment. sampleFrames is how many sample frames. These two values should match
// ie. sampleFrames = bufferByteLength / channels / bitsize / 8
extern void BAE_BuildMixerSlice(void *threadContext, void *pAudioBuffer, 
					long bufferByteLength,
					long sampleFrames);

// Return the maximumn number of samples for 11 milliseconds worth of 44 khz data.
// Typically this is 512. Use this in your calculation of audio buffers
extern short BAE_GetMaxSamplePerSlice(void);

// MUTEX

typedef void* BAE_Mutex;

// returns 1 if ok.
int BAE_NewMutex(BAE_Mutex* lock, char *name, char *file, int lineno);
void BAE_AcquireMutex(BAE_Mutex mutex);
void BAE_ReleaseMutex(BAE_Mutex mutex);
void BAE_DestroyMutex(BAE_Mutex mutex);

//CLS:  THREADING API:

// the type of function called by the frame thread
// "threadContext" is a platform-specific value or (#if JAVA_SOUND) a JNIEnv* pointer
typedef void (*BAE_FrameThreadProc)(void* threadContext);

// Create and start the frame thread
extern int	BAE_CreateFrameThread(void* threadContext, BAE_FrameThreadProc proc);

extern int BAE_SetFrameThreadPriority(void *threadContext, int priority);

// Stop and destroy the frame thread
extern int	BAE_DestroyFrameThread(void* threadContext);

// Make the frame thread sleep for the given number of milliseconds
extern int	BAE_SleepFrameThread(void* threadContext, long msec);

// CAPTURE API

// Aquire and capture audio. sampleRate is 48000 to 2000. Will fail if device doesn't support it
// bits is 8 or 16; channels is 1 or 2;
// *pCaptureHandle will contain a device dependent ID for the capture device.
// pCaptureHandle can be NULL.
// return 0 if ok, -1 if failed
int BAE_AquireAudioCapture(void *threadContext, unsigned long sampleRate, unsigned long channels, unsigned long bits,
							unsigned long *pCaptureHandle);

// Release and free audio capture device
// return 0 if ok, -1 if failed.
int BAE_ReleaseAudioCapture(void *threadContext);

typedef enum
{
	OPEN_CAPTURE = 0,
	CLOSE_CAPTURE,
	DATA_READY_CAPTURE
} BAECaptureMessage;

// Called when BAE_StartAudioCapture is called. 
// 	OPEN_CAPTURE
//		parmeter1 and parmeter2 are NULL.

// Called when BAE_StopAudioCapture is called. 
//	CLOSE_CAPTURE
//		parmeter1 and parmeter2 are NULL.

// Called when the audio buffer has been filled by the capture audio subsystem.
//	DATA_READY_CAPTURE
//		parmeter1 is a pointer to an char * which is the pointer to the audio block
//		parmeter2 is a pointer to an unsigned long which is the length of buffer

typedef void (*BAE_CaptureDone)(void *callbackContext, BAECaptureMessage message, void *parmeter1, void *parameter2);

// Given the capture hardware is working, fill a buffer with some data. This call is
// asynchronous. When this buffer is filled, the function done will be called.
// $$kk: 10.09.98: changed this
// $$kk: 10.15.98: should go back and make buffer size configurable later!
//int BAE_StartAudioCapture(void *buffer, unsigned long bufferSize, BAE_CaptureDone done, void *callbackContext);
int BAE_StartAudioCapture(BAE_CaptureDone done, void *callbackContext);

// stop the capture hardware
int BAE_StopAudioCapture(void);

// Pause and resume the audio capture. You might lose a buffer doing this.
int BAE_PauseAudioCapture(void);
int BAE_ResumeAudioCapture(void);

// number of capture devices
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_MaxCaptureDevices(void);

// set the current device. device is from 0 to BAE_MaxCaptureDevices()
// NOTE:	This function needs to function before any other calls may have happened.
//			Also you will need to call BAE_ReleaseAudioCapture then BAE_AquireAudioCapture
//			in order for the change to take place. deviceParameter is a device specific
//			pointer. Pass NULL if you don't know what to use.
void BAE_SetCaptureDeviceID(long deviceID, void *deviceParameter);

// return current device ID, and fills in the deviceParameter with a device specific
// pointer. It will pass NULL if there is nothing to use.
// NOTE: This function needs to function before any other calls may have happened.
long BAE_GetCaptureDeviceID(void *deviceParameter);

// get deviceID name 
// NOTE:	This function needs to function before any other calls may have happened.
//			Format of string is a zero terminated comma delinated C string.
//			"platform,method,misc"
//	example	"MacOS,Sound Manager 3.0,SndPlayDoubleBuffer"
//			"WinOS,DirectSound,multi threaded"
//			"WinOS,waveIn,multi threaded"
//			"WinOS,VxD,low level hardware"
//			"WinOS,plugin,Director"
void BAE_GetCaptureDeviceName(long deviceID, char *cName, unsigned long cNameLength);

// return the number of frames in the capture buffer
// $$kk: 10.13.98: added this
unsigned long BAE_GetCaptureBufferSizeInFrames();

// return the number of buffers used in capture.  the capture latency can be 
// determined from this and BAE_GetCaptureBufferSizeInFrames.
// $$kk: 10.15.98: added this
int BAE_GetCaptureBufferCount();

// Get the count of samples captured at the capture device
// $$kk: 10.14.98: added this; need to implement!
unsigned long BAE_GetDeviceSamplesCapturedPosition();

// backward compatibility
#define HAE_WaitMicroseocnds				BAE_WaitMicroseconds
#define HAE_Setup							BAE_Setup
#define HAE_Cleanup							BAE_Cleanup
#define HAE_Allocate						BAE_Allocate
#define HAE_Deallocate						BAE_Deallocate
#define HAE_IsBadReadPointer				BAE_IsBadReadPointer
#define HAE_SizeOfPointer					BAE_SizeOfPointer
#define HAE_BlockMove						BAE_BlockMove
#define HAE_IsStereoSupported				BAE_IsStereoSupported
#define HAE_Is8BitSupported					BAE_Is8BitSupported
#define HAE_Is16BitSupported				BAE_Is16BitSupported
#define HAE_GetHardwareVolume				BAE_GetHardwareVolume
#define HAE_SetHardwareVolume				BAE_SetHardwareVolume
#define HAE_GetHardwareBalance				BAE_GetHardwareBalance
#define HAE_SetHardwareBalance				BAE_SetHardwareBalance
#define HAE_Microseconds					BAE_Microseconds
#define HAE_WaitMicroseconds				BAE_WaitMicroseconds
#define HAE_WaitMicroseocnds				BAE_WaitMicroseconds
#define HAE_CopyFileNameNative				BAE_CopyFileNameNative
#define HAE_FileCreate						BAE_FileCreate
#define HAE_FileDelete						BAE_FileDelete
#define HAE_FileOpenForRead					BAE_FileOpenForRead
#define HAE_FileOpenForWrite				BAE_FileOpenForWrite
#define HAE_FileOpenForReadWrite			BAE_FileOpenForReadWrite
#define HAE_FileClose						BAE_FileClose
#define HAE_ReadFile						BAE_ReadFile
#define HAE_WriteFile						BAE_WriteFile
#define HAE_SetFilePosition					BAE_SetFilePosition
#define HAE_GetFilePosition					BAE_GetFilePosition
#define HAE_GetFileLength					BAE_GetFileLength
#define HAE_SetFileLength					BAE_SetFileLength
#define HAE_AquireAudioCard					BAE_AquireAudioCard
#define HAE_ReleaseAudioCard				BAE_ReleaseAudioCard
#define HAE_Idle							BAE_Idle
#define HAE_GetDeviceSamplesPlayedPosition	BAE_GetDeviceSamplesPlayedPosition
#define HAE_MaxDevices						BAE_MaxDevices
#define HAE_SetDeviceID						BAE_SetDeviceID
#define HAE_GetDeviceID						BAE_GetDeviceID
#define HAE_GetDeviceName					BAE_GetDeviceName
#define HAE_GetSliceTimeInMicroseconds		BAE_GetSliceTimeInMicroseconds
#define HAE_GetAudioBufferCount				BAE_GetAudioBufferCount
#define HAE_GetAudioByteBufferSize			BAE_GetAudioByteBufferSize
#define HAE_BuildMixerSlice					BAE_BuildMixerSlice
#define HAE_GetMaxSamplePerSlice			BAE_GetMaxSamplePerSlice
#define HAE_FrameThreadProc					BAE_FrameThreadProc
#define HAE_CreateFrameThread				BAE_CreateFrameThread
#define HAE_DestroyFrameThread				BAE_DestroyFrameThread
#define HAE_SleepFrameThread				BAE_SleepFrameThread

#ifdef __cplusplus
	}
#endif

#endif	// BAE_API

