/*****************************************************************************/
/*
**	XMPEG_BAE_API.h
**
**	Layer between BAE and MPEG decoder.
**
**	© Copyright 2000-2001 Beatnik, Inc, All Rights Reserved.
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
**
**	4/15/2000	Created
**	5/7/2000	Start of a file encoder
**	06.21.2000 msd	Added MPG_EncodeSetRefillCallback(), which enables
**					The MPEG encoder to stream encoding rather than
**					having all it's data ready at the start.
**	7/25/2000	sh	Added a new parameter to XGetClosestMPEGSampleRate.
**	8/30/2000	sh	Added XGetMPEGCompressionType.
**	1/3/2001	sh	Added MPG_GetEncodedFrameBufferSizeInBytes.
**	1/17/2001	sh	Renamed MPG_GetEncodedFrameBufferSizeInBytes to 
**					MPG_GetFrameBufferSizeInBytes.
*/
/*****************************************************************************/

#ifndef BAE_MPEG
#define HAE_MPEG
#define BAE_MPEG

#include "GenSnd.h"
#include "X_API.h"
#include "X_Formats.h"

#ifdef __cplusplus
	extern "C" {
#endif

//#### decoder API

// create new mpeg stream from vaild MPG file. Returns NULL if failed.
void * MPG_NewStream(void *fileName_in);
void * MPG_NewStreamXFILENAME(XFILENAME *fileName_in);
void * MPG_NewStreamXFILE(XFILE file);
void * MPG_NewStreamFromMemory(void *mpeg_stream, unsigned long mpeg_stream_length);

// free vaild stream
void MPG_FreeStream(void *stream);

// return the number of bytes filled when calling MPG_FillBuffer
int MPG_GetBufferSize(void *reference);

// return the size in bytes of an encoded frame block
int MPG_GetFrameBufferSizeInBytes(void *reference);

// reposition stream, start reading from newPos.
int MPG_SeekStream(void *reference, unsigned long newPos);

// return the max number of frames
unsigned long MPG_GetMaxBuffers(void *reference);

// Given a valid stream, and a buffer fill the pcm audio samples from the stream
int MPG_FillBuffer(void *stream, void *buffer);

// get the number of channels used for this stream.
int MPG_GetChannels(void *stream);

// get the bit size 8, 16 from the stream
int MPG_GetBitSize(void *stream);

// get the encoded bitrate of the stream, in bits/sec
int MPG_GetBitrate(void *stream);

// get the sample rate of the resulting pcm audio data from the stream
int MPG_GetSampleRate(void *stream);

// return the number of samples in this stream
unsigned long MPG_GetNumberOfSamples(void *stream);

// return the size in bytes of the complete uncompressed MPEG file
unsigned long MPG_GetSizeInBytes(void *reference);

//#### encoder API

// create new mpeg stream to prepare for memory based encoding. Returns NULL if failed.
void * MPG_EncodeNewStream(unsigned long encodeRate,
							unsigned long sampleRate, unsigned long channels,
							XPTR data, unsigned long dataLength);

// compress frame
// returns number of samples processed
// returns *pLastFrame TRUE if last mpeg frame
int MPG_EncodeProcess(void *stream, XPTR *pReturnedBuffer, unsigned long *pReturnedSize, XBOOL *pLastFrame);

// free vaild stream and return compressed MPG stream m_results
void MPG_EncodeFreeStream(void *stream);

// get max mpeg frames we're going to encode
unsigned long MPG_EncodeMaxFrames(void *stream);

// get max size of an mpeg frame in bytes
unsigned long MPG_EncodeMaxFrameSize(void *stream);

// Use these to make your buffer ('data' in MPG_EncodeNewStream) refillable
// by having this callback refill the 'data' buffer when called.
// MPEGFillBufferFn() returns TRUE if the buffer has been refilled
typedef XBOOL (*MPEGFillBufferFn)(void *buffer, void *userRef);
void MPG_EncodeSetRefillCallback(void *stream, MPEGFillBufferFn callback, void *userRef);


// Convert a SndCompressionType to an XMPEGEncodeRate
XMPEGEncodeRate XGetMPEGEncodeRate(SndCompressionType type);
SndCompressionType XGetMPEGCompressionType(XMPEGEncodeRate rate);

// Given a MPEG bit rate, return the closest one this encoder supports.
XMPEGEncodeRate XGetClosestMPEGEncodeRate(unsigned int bitrate);

// Given a sample rate, and a MPEG compression sub type, return the closest
// sample rate supported by this encoder.
XFIXED XGetClosestMPEGSampleRate(XFIXED sourceRate, SndCompressionSubType subType);

// given a sample rate, an bit encode rate and MPEG compression sub type
// calulate a new sample rate, new encode rate and return them in
// *outSampleRate & *outEncodeRate
void XGetClosestMPEGSampleRateAndEncodeRate(XFIXED inSampleRate, 
											XMPEGEncodeRate inEncodeRate,
											XFIXED *outSampleRate,
											XMPEGEncodeRate *outEncodeRate,
											SndCompressionSubType subType);

#ifdef __cplusplus
	}
#endif


#endif	// HAE_MPEG

