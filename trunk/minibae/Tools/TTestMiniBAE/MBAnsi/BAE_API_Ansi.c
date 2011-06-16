/*****************************************************************************/
/*
**	AE_API_Ansi.c
**
**	This provides platform specfic functions for ANSI C compilation
**
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
**	11/21/00	tom		Created
**  11/28/00    tom     build with USE_WINDOWS_IO = 0 (unix build)
**  12/01/00    tom     include three (3) i/o options: Windows, Unix, Ansi C
**  02/23/01    tom     in HAE_FileCreate, add fclose() in USE_ANSI_IO option
**						in HAE_WriteFile, add fflush() before fwrite() to workaround VC++ stdio shortcoming
*/
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "BAE_API.h"

#define BUFFER_SIZE   896  

// only one of these can be true
#define USE_ANSI_IO			0
#define USE_UNIX_IO			0
#define USE_WINDOWS_IO		1


// includes for USE_WINDOWS_IO 
#if USE_WINDOWS_IO
	#include <windows.h>
	#include <mmsystem.h>
	#include <windowsx.h>
	#include <assert.h>
	#include <io.h>
#endif

// includes for USE_UNIX_IO
#if USE_UNIX_IO
	#include <fcntl.h>
#endif

static unsigned long		g_memory_buoy = 0;			// amount of memory allocated at this moment
static unsigned long		g_memory_buoy_max = 0;

static short int			g_balance = 0;				// balance scale -256 to 256 (left to right)
static short int			g_unscaled_volume = 256;	// hardware volume in HAE scale

static long					g_audioByteBufferSize;			// size of audio buffers in bytes

static long					g_shutDownDoubleBuffer;
static long					g_activeDoubleBuffer;

// $$kk: 05.06.98: made lastPos a global variable
static long					g_lastPos;

 // number of samples per audio frame to generate
long						g_audioFramesToGenerate;

// How many audio frames to generate at one time 
static unsigned int			g_synthFramesPerBlock;	// setup upon runtime


// **** System setup and cleanup functions
// Setup function. Called before memory allocation, or anything serious. Can be used to 
// load a DLL, library, etc.
// return 0 for ok, or -1 for failure
int HAE_Setup(void)
{
	return 0;
}

// Cleanup function. Called after all memory and the last buffers are deallocated, and
// audio hardware is released. Can be used to unload a DLL, library, etc.
// return 0 for ok, or -1 for failure
int HAE_Cleanup(void)
{
	return 0;
}

// **** Memory management
// allocate a block of locked, zeroed memory. Return a pointer
void * HAE_Allocate(unsigned long size)
{
	void *data = NULL;
	if (size)
	{
		data = (char *)malloc(size);
		if (data)
		{
			memset(data, 0, size);
		}
	}
	return data;
}

// dispose of memory allocated with HAE_Allocate
void HAE_Deallocate(void * memoryBlock)
{
	if (memoryBlock)
	{
		free(memoryBlock);
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
int HAE_IsBadReadPointer(void *memoryBlock, unsigned long size)
{
	// return (IsBadReadPtr(memoryBlock, size)) ? 1 : 0;
	return 2;
}

// this will return the size of the memory pointer allocated with HAE_Allocate. Return
// 0 if you don't support this feature
unsigned long HAE_SizeOfPointer(void * memoryBlock)
{
	return 0;
}

// block move memory. This is basicly memmove, but its exposed to take advantage of
// special block move speed ups, various hardware has available.
// NOTE:	Must use a function like memmove that insures a valid copy in the case
//			of overlapping memory blocks.
void HAE_BlockMove(void * source, void * dest, unsigned long size)
{
	if (source && dest && size)
	{
		memmove(dest, source, size);
	}
}

// **** Audio Card modifiers
// Return 1 if stereo hardware is supported, otherwise 0.
int HAE_IsStereoSupported(void)
{
	return 0;
}

// Return 1, if sound hardware support 16 bit output, otherwise 0.
int HAE_Is16BitSupported(void)
{
	return 1;
}

// Return 1, if sound hardware support 8 bit output, otherwise 0.
int HAE_Is8BitSupported(void)
{
	return 1;
}

// returned balance is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
short int HAE_GetHardwareBalance(void)
{
	return g_balance;
}

// 'balance' is in the range of -256 to 256. Left to right. If you're hardware doesn't support this
// range, just scale it.
void HAE_SetHardwareBalance(short int balance)
{

}

// returned volume is in the range of 0 to 256
short int HAE_GetHardwareVolume(void)
{
	return 1;
}

// newVolume is in the range of 0 to 256
void HAE_SetHardwareVolume(short int newVolume)
{

}



// **** Timing services
// return microseconds
unsigned long HAE_Microseconds(void)
{
#if USE_WINDOWS_IO
	static unsigned long	starttick = 0;
	static char				firstTime = TRUE;
	static char				QPClockSupport = FALSE;
	static unsigned long	clockpusu = 0;	// clocks per microsecond
	unsigned long			time;
	LARGE_INTEGER			p;

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
#else
	return 0;
#endif
}

// wait or sleep this thread for this many microseconds
// CLS??: If this function is called from within the frame thread and
// JAVA_THREAD is non-zero, we'll probably crash.
void HAE_WaitMicroseconds(unsigned long waitAmount)
{
#if USE_WINDOWS_IO
	unsigned long	ticks;

	ticks = HAE_Microseconds() + waitAmount;
	while (HAE_Microseconds() < ticks) 
	{
		Sleep(0);	// Give up the rest of this time slice to other threads
	}
#endif
}

// If no thread support, this will be called during idle times. Used for host
// rendering without threads.
void HAE_Idle(void *userContext)
{
	userContext;
}

// **** File support
// Create a file, delete orignal if duplicate file name.
// Return -1 if error

// Given the fileNameSource that comes from the call HAE_FileXXXX, copy the name
// in the native format to the pointer passed fileNameDest.
void HAE_CopyFileNameNative(void *fileNameSource, void *fileNameDest)
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

long HAE_FileCreate(void *fileName)
{

	
#if USE_UNIX_IO
	int file;
	file = _open((char *)fileName, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY);
	if (file != -1)
	{
		_close(file);
	}
	return (file != -1) ? 0 : -1;
#elif USE_ANSI_IO
	FILE *fp = fopen((char *)fileName, "wb");
	return (long)fp;
	if(fp)
	{
		fclose(fp);
	}
#elif USE_WINDOWS_IO
	HANDLE	file;

	file = CreateFile((LPCTSTR)fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, 
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
									NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		DWORD	lastErr = GetLastError();
		return -1;
	}
	CloseHandle(file);
	return file ? 0 : -1;
#endif
}

long HAE_FileDelete(void *fileName)
{
#if USE_ANSI_IO
	remove ((char *)fileName);
#elif USE_WINDOWS_IO

	if (fileName)
	{
		if (DeleteFile((char *)fileName))
		{
			return 0;
		}
	}
#endif
	return -1;
}


// Open a file
// Return -1 if error, otherwise file handle
long HAE_FileOpenForRead(void *fileName)
{

	if (fileName)
	{
#if USE_UNIX_IO
	   return _open((char *)fileName, _O_RDONLY | _O_BINARY);
#elif USE_ANSI_IO
       FILE *fp = fopen((char *)fileName, "rb");
       return (long)fp;
#elif USE_WINDOWS_IO
		HANDLE	file;

		file = CreateFile((LPCTSTR)fileName, GENERIC_READ, 
									//FILE_SHARE_READ | FILE_SHARE_WRITE,
									FILE_SHARE_READ,
									NULL, OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY | 
									FILE_FLAG_RANDOM_ACCESS,
									NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			DWORD	lastErr = GetLastError();
			return -1;
		}
		return (long)file;
#endif
	}
	return -1;
}

long HAE_FileOpenForWrite(void *fileName)
{

	if (fileName)
	{
#if USE_UNIX_IO
		return _open((char *)fileName, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY);
#elif USE_ANSI_IO
		FILE *fp = fopen((char *)fileName, "wb");
		return (long)fp;;
#elif USE_WINDOWS_IO
		HANDLE	file;

		file = CreateFile((LPCTSTR)fileName, GENERIC_WRITE, 0, NULL, CREATE_NEW | TRUNCATE_EXISTING,
									FILE_FLAG_RANDOM_ACCESS,
									NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			DWORD	lastErr = GetLastError();
			return -1;
		}
		return (long)file;

#endif
	}
	return -1;
}

long HAE_FileOpenForReadWrite(void *fileName)
{
	if (fileName)
	{
#if USE_UNIX_IO
		return _open((char *)fileName, _O_RDWR | _O_BINARY);
#elif USE_ANSI_IO
		FILE *fp = fopen((char *)fileName, "r+b" /*"arb"*/ /*"wrb"*/);
		return (long)fp;
#elif USE_WINDOWS_IO
		HANDLE	file;

		file = CreateFile((LPCTSTR)fileName, GENERIC_READ | GENERIC_WRITE, 
									FILE_SHARE_READ, 
									NULL, 
									OPEN_EXISTING,
									FILE_FLAG_RANDOM_ACCESS,
									NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			DWORD	lastErr = GetLastError();
			return -1;
		}
		return (long)file;

#endif
	}
	
	return -1;
}

// Close a file
void HAE_FileClose(long fileReference)
{
#if  USE_UNIX_IO
	_close(fileReference);
#elif USE_ANSI_IO
	int val = fclose((FILE *)fileReference);
#elif USE_WINDOWS_IO
	CloseHandle((HANDLE)fileReference);
#endif
}

// Read a block of memory from a file.
// Return -1 if error, otherwise length of data read.
long HAE_ReadFile(long fileReference, void *pBuffer, long bufferLength)
{
	if (pBuffer && bufferLength)
	{
#if  USE_UNIX_IO
		return _read(fileReference, (char *)pBuffer, bufferLength);
#elif USE_ANSI_IO
		long bytesRead = 0;
		bytesRead = fread( (char *)pBuffer, 1, bufferLength, (FILE *)fileReference); 
		return (bytesRead <= 0) ? -1: bytesRead;
#elif USE_WINDOWS_IO
		{
			DWORD	readFromBuffer;
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
long HAE_WriteFile(long fileReference, void *pBuffer, long bufferLength)
{
	if (pBuffer && bufferLength)
	{
#if  USE_UNIX_IO
		return _write(fileReference, (char *)pBuffer, bufferLength);
#elif USE_ANSI_IO
		long bytesWritten = 0;
		int val = ftell((FILE *)fileReference);
		fflush((FILE *)fileReference);
		bytesWritten = fwrite( (char *)pBuffer, 1, bufferLength, (FILE *)fileReference);
		fflush((FILE *)fileReference);
		return (bytesWritten <= 0) ? -1: bytesWritten;
#elif USE_WINDOWS_IO
		{
			DWORD	writtenFromBuffer;
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
long HAE_SetFilePosition(long fileReference, unsigned long filePosition)
{
#if  USE_UNIX_IO
	return (_lseek(fileReference, filePosition, SEEK_SET) == -1) ? -1 : 0;
#elif USE_ANSI_IO
	int val = 0;
	val = fseek((FILE *)fileReference, filePosition, SEEK_SET);
	return (val == 0) ? 0 : -1;
#elif USE_WINDOWS_IO
	return (
			(SetFilePointer((HANDLE)fileReference, filePosition, NULL, FILE_BEGIN) == 0xFFFFFFFFL)
			 ? -1 : 0);
#endif
}

// get file position in absolute file bytes
unsigned long HAE_GetFilePosition(long fileReference)
{
#if USE_UNIX_IO
	return _lseek(fileReference, 0, SEEK_CUR);
#elif USE_ANSI_IO
	return ftell((FILE *)fileReference);
#elif USE_WINDOWS_IO
	return SetFilePointer((HANDLE)fileReference, 0, NULL, FILE_CURRENT);
#endif
}

// get length of file
unsigned long HAE_GetFileLength(long fileReference)
{
	unsigned long pos = 0;
	int val = 0;

#if  USE_UNIX_IO
	pos = _lseek(fileReference, 0, SEEK_END);
	_lseek(fileReference, 0, SEEK_SET);
#elif USE_ANSI_IO
	fseek((FILE *)fileReference, 0, SEEK_END);
	pos = ftell((FILE *)fileReference);
	fseek((FILE *)fileReference, 0, SEEK_SET);
#elif USE_WINDOWS_IO
	pos = GetFileSize((HANDLE)fileReference, NULL);
	if (pos == 0xFFFFFFFFL)
	{
		pos = 0;
	}
#endif
	return pos;
}

// set the length of a file. Return 0, if ok, or -1 for error
int HAE_SetFileLength(long fileReference, unsigned long newSize)
{
#if USE_UNIX_IO
	return _chsize(fileReference, newSize);
#elif USE_ANSI_IO
	return -1;
#elif USE_WINDOWS_IO
	int error;
	
	error = -1;
	if (HAE_SetFilePosition(fileReference, newSize) == 0)
	{
		error = SetEndOfFile((HANDLE)fileReference) ? 0 : -1;
	}
	return error;
#endif
}


// Return the number of 11 ms buffer blocks that are built at one time.
int HAE_GetAudioBufferCount(void)
{
	return g_synthFramesPerBlock;
}

// Return the number of bytes used for audio buffer for output to card
long HAE_GetAudioByteBufferSize(void)
{
	return g_audioByteBufferSize;
}



// **** Audio card support
// Aquire and enabled audio card
// return 0 if ok, -1 if failed
int HAE_AquireAudioCard(void *threadContext, unsigned long sampleRate, unsigned long channels, unsigned long bits)
{
	// need to set callback which will in turn call BuildMixerSlice every so often
	g_audioByteBufferSize = BUFFER_SIZE;

	return 0;
}

// Release and free audio card.
// return 0 if ok, -1 if failed.
int HAE_ReleaseAudioCard(void *threadContext)
{
	return 0;
}

// return device position in samples since the device was opened
unsigned long HAE_GetDeviceSamplesPlayedPosition(void)
{
	unsigned long pos = 0;

	return pos;
}

// number of devices. ie different versions of the HAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long HAE_MaxDevices(void)
{
	return 1;
}

// set the current device. device is from 0 to HAE_MaxDevices()
// NOTE:	This function needs to function before any other calls may have happened.
//			Also you will need to call HAE_ReleaseAudioCard then HAE_AquireAudioCard
//			in order for the change to take place.
void HAE_SetDeviceID(long deviceID, void *deviceParameter)
{

}

// return current device ID
// NOTE: This function needs to function before any other calls may have happened.
long HAE_GetDeviceID(void *deviceParameter)
{
	return 1;
}

// NOTE:	This function needs to function before any other calls may have happened.
//			Format of string is a zero terminated comma delinated C string.
//			"platform,method,misc"
//	example	"MacOS,Sound Manager 3.0,SndPlayDoubleBuffer"
//			"WinOS,DirectSound,multi threaded"
//			"WinOS,waveOut,multi threaded"
//			"WinOS,VxD,low level hardware"
//			"WinOS,plugin,Director"
void HAE_GetDeviceName(long deviceID, char *cName, unsigned long cNameLength)
{

}


// EOF of BAE_API_ANSI.c
