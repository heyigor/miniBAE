/*****************************************************************************/
/*
**	XMPEGFiles.c
**
**	Decodes/Encodes MPEG sound files
**
**	й Copyright 1998-2000 Beatnik, Inc, All Rights Reserved.
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
**	9/3/98		Created
**	11/20/98	Changed conditional compiler flag
**	2/12/99		Renamed USE_BAE_FOR_MPEG to USE_MPEG_DECODER
**	2/21/99		Changed table in PV_find_bitrate_index to ints
**	2/24/99		Changed XExpandMPEG to decode one less buffer. This may or may not
**				be a mistake, but it was overwriting memory at the end. This
**				seems to be a decoder problem
**	3/15/99		MOE: Fixed XExpandMPEG().  It was not testing whether allocated
**				buffer was large enough properly, it was skipping the first
**				buffer if there was no skipping to do, and it was ignoring
**				the last buffer (since 2/24/99).
**	3/22/99		MOE:  Changed the names of the parameters of XCompressMPEG()
**				to more closely reflect what they mean.
**				MOE:  Made PV_ScanForAudioDataFromMPEG() correct the frame
**				buffer count of encoded data using its decode stream
**				MOE:  Changed parameters of PV_ScanForAudioData(), fixed a bug
**				in it wherein it wouldn't detect stereo audio with perfectly
**				out-of-phase signals, and made it return the full buffer
**				length if no audio data detected.
**	3/25/99		MOE:  Added procData parameter to functions using XCompressStatusProc
**	6/23/99		MOE: Changed meaning of startFrame parameter to XExpandMPEG()
**				and XCompressMPEG().  Made it refer to frames, not samples
**	6/23/99		MOE: XExpandMPEG(): Now allocates the decoded data itself
**						Handles any number of skipped frames
**						Fixed a bug that would cause MPEGs encoded with zero
**							skipped frames to lose the first block on decode
**	6/28/99		MOE: eliminated XOpenMPEGStreamFromXFILENAME() since it wasn't
**				being called and it didn't clean up its XFILE at all
**	6/28/99		MOE: added XGetMPEGBitrateType()
**	7/2/99		MOE:  Moved PV_GetMPEGEncodeRate(), PV_GetClosestMPEGEncodeRate(), and
**				PV_GetClosestMPEGSampleRate() from SampleTools.c since they are
**				MPEG-encoder-dependent
**	7/2/99		MOE:  Changed the parameters of XCompressMPEG() and XExpandMPEG()
**	9/28/99		Changed instances of 1152 to MAX_MPEG_BUFFER_SIZE
**	10/19/99	In XProcessMPEGEncoder(), set the size of writeBuffer[] with 
**				the MAX_MPEG_BUFFER_SIZE macro
**	11/8/99		Added conditionals around MPEG code.
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	3/9/2000	Fixed bug in XExpandMPEG. The last buffer was not being copied
**				or moved if it needed to be offset. Now checking for last buffer
**				after all that.
**	4/26/2000	Renamed PV_GetMPEGEncodeRate to XGetMPEGEncodeRate and moved to
**				XMPEG_BAE_API.c. Renamed PV_GetClosestMPEGSampleRate to
**				XGetClosestMPEGSampleRate and moved to XMPEG_BAE_API.c.
**				Renamed PV_GetClosestMPEGEncodeRate to
**				XGetClosestMPEGEncodeRate and moved to XMPEG_BAE_API.c.
**	5/1/2000	Incorprated new mpeg encoder/decoder package.
**	5/9/2000	Enabled "scan for blank code" in the mpeg encoder.
**	5/11/2000	Now preserve any silence in the orignal sample prior to encoding with mpeg
**	5/12/2000	Added XIsValidMPEGSampleRateAndEncodeRate
**	7/25/2000	sh	Added parameter to XCompressMPEG to handle SndCompressionSubType
**	7/26/2000	sh	Changed PV_ScanForAudioDataFromMPEG to return errors.
**	7/27/2000	sh	Changed temp buffer size in PV_ScanForAudioDataFromMPEG
**	7/28/3000	sh	Changed XCompressMPEG to use the maxFrameBuffers instead of currentFrameBuffer
**					when calculating the total number of mpeg frames to store.
**	8/30/2000	sh	Changed XIsValidMPEGSampleRateAndEncodeRate to build a stream then test it.
**					Added channel parmeter to XIsValidMPEGSampleRateAndEncodeRate.
*/
/*****************************************************************************/

//#define USE_DEBUG	2

#include "X_API.h"
#include "X_Formats.h"
#include "X_Assert.h"
#include "GenSnd.h"
#include "GenPriv.h"
#include <math.h>		// cos()

#if (USE_MPEG_DECODER != 0) || (USE_MPEG_ENCODER != 0)
#include "XMPEG_BAE_API.h"

#define WRITE_FILE	0

#if WRITE_FILE
static XFILE	fileOut = 0;
#endif


#if WRITE_FILE
static void PV_SetupWriteFile(void)
{
	XFILENAME	fileOutName;

	fileOut = 0;
	if (XGetTempXFILENAME(&fileOutName) == 0)
	{
		fileOut = XFileOpenForWrite(&fileOutName, TRUE);
	}
}

static void PV_CleanupWriteFile(void)
{
	if (fileOut)
	{
		XFileClose(fileOut);
		fileOut = 0;
	}
}

static void PV_WriteToFile(char *buffer, unsigned long size)
{
	if (fileOut)
	{
		XFileWrite(fileOut, buffer, size);
	}
}
#endif

#if USE_MPEG_DECODER != 0
#if 0
	#pragma mark ееееееееееееее MPEG DECODER еееееееееееееееее
#endif

XMPEGDecodedData * XOpenMPEGStreamFromXFILE(XFILE file, OPErr *pErr)
{
	XMPEGDecodedData	*pStream;

#if WRITE_FILE
	PV_SetupWriteFile();
#endif
	pStream = NULL;
	if (file && pErr)
	{
		*pErr = NO_ERR;
		pStream = (XMPEGDecodedData *)XNewPtr(sizeof(XMPEGDecodedData));
		if (pStream)
		{
			pStream->stream = MPG_NewStreamXFILE(file);
			if (pStream->stream)
			{
				pStream->sampleRate = UNSIGNED_LONG_TO_XFIXED(MPG_GetSampleRate(pStream->stream));
				pStream->bitSize = (XBYTE)MPG_GetBitSize(pStream->stream);
				pStream->channels = (XBYTE)MPG_GetChannels(pStream->stream);
				pStream->bitrate = MPG_GetBitrate(pStream->stream);

				pStream->lengthInBytes = MPG_GetSizeInBytes(pStream->stream);
				pStream->lengthInSamples = MPG_GetNumberOfSamples(pStream->stream);

				pStream->frameBufferSize = (unsigned long)MPG_GetBufferSize(pStream->stream);
				pStream->maxFrameBuffers = (unsigned long)MPG_GetMaxBuffers(pStream->stream);

			}
			else
			{
				BAE_ASSERT(FALSE);
				*pErr = BAD_FILE;
				XDisposePtr((XPTR)pStream);
				pStream = NULL;
			}
		}
		else
		{
			BAE_ASSERT(FALSE);
			*pErr = MEMORY_ERR;
		}
	}
	return pStream;
}

XMPEGDecodedData * XOpenMPEGStreamFromMemory(XPTR pBlock, unsigned long blockSize, OPErr *pErr)
{
	XMPEGDecodedData	*pStream;

	if (!pBlock || !blockSize || !pErr)
	{
		BAE_ASSERT(FALSE);
		return NULL;
	}
	
#if WRITE_FILE
	PV_SetupWriteFile();
#endif
	pStream = (XMPEGDecodedData *)XNewPtr(sizeof(XMPEGDecodedData));
	if (pStream)
	{
		pStream->stream = MPG_NewStreamFromMemory(pBlock, blockSize);
		if (pStream->stream)
		{
			pStream->sampleRate = UNSIGNED_LONG_TO_XFIXED(MPG_GetSampleRate(pStream->stream));
			pStream->bitSize = (XBYTE)MPG_GetBitSize(pStream->stream);
			pStream->channels = (XBYTE)MPG_GetChannels(pStream->stream);
			pStream->bitrate = MPG_GetBitrate(pStream->stream);

			pStream->lengthInBytes = MPG_GetSizeInBytes(pStream->stream);
			pStream->lengthInSamples = MPG_GetNumberOfSamples(pStream->stream);

			pStream->frameBufferSize = (unsigned long)MPG_GetBufferSize(pStream->stream);
			pStream->maxFrameBuffers = (unsigned long)MPG_GetMaxBuffers(pStream->stream);
			*pErr = NO_ERR;
		}
		else
		{
			BAE_ASSERT(FALSE);
			XDisposePtr((XPTR)pStream);
			pStream = NULL;
			*pErr = BAD_FILE;
		}
	}
	else
	{
		BAE_ASSERT(FALSE);
		*pErr = MEMORY_ERR;
	}
	return pStream;
}

OPErr XCloseMPEGStream(XMPEGDecodedData *pStream)
{
	OPErr	err;

#if WRITE_FILE
	PV_CleanupWriteFile();
#endif
	err = NO_ERR;
	if (pStream)
	{
		if (pStream->stream)
		{
			MPG_FreeStream(pStream->stream);
		}
		XDisposePtr((XPTR)pStream);
	}
	else
	{
		BAE_ASSERT(FALSE);
		err = PARAM_ERR;
	}
	return err;
}

OPErr XFillMPEGStreamBuffer(XMPEGDecodedData *pStream, void *pcmAudioBuffer, XBOOL *pDone)
{
	OPErr	err;

	err = NO_ERR;
	if (pStream && pcmAudioBuffer && pDone)
	{
		*pDone = FALSE;
		if (MPG_FillBuffer(pStream->stream, pcmAudioBuffer) == -1)
		{
			*pDone = TRUE;
		}
#if WRITE_FILE
		PV_WriteToFile(pcmAudioBuffer, pStream->frameBufferSize);
#endif
	}
	else
	{
		BAE_ASSERT(FALSE);
		err = PARAM_ERR;
	}
	return err;
}

// given a MPEG stream and an offset, decode the mpeg stream into memory
OPErr XExpandMPEG(GM_Waveform const* src, UINT32 startFrame, GM_Waveform* dst)
{
OPErr				err;
XMPEGDecodedData*	stream;

	BAE_ASSERT(dst);
	BAE_ASSERT(src);
	*dst = *src;
	dst->theWaveform = NULL;
	dst->compressionType = C_NONE;
	
	stream = XOpenMPEGStreamFromMemory(src->theWaveform, src->waveSize, &err);
	if (stream)
	{
	UINT32		decodingBytes;
	XPTR		decodingData;

		decodingBytes = stream->maxFrameBuffers * stream->frameBufferSize;
		decodingData = XNewPtr(decodingBytes);
		if (decodingData)
		{
		UINT32			const bytesPerFrame = dst->channels * sizeof(short);
		XBYTE*			data;
		UINT32			startByte;
		UINT32			count;

			data = (XBYTE*)decodingData;
			startByte = startFrame * bytesPerFrame;
			count = 0;
			while (count < stream->maxFrameBuffers)
			{
			XBOOL		done;
				
				err = XFillMPEGStreamBuffer(stream, data, &done);

				if (startByte == 0)
				{
					data += stream->frameBufferSize;
				}
				else if (startByte < stream->frameBufferSize)
				{
					XBlockMove(data + startByte, data,
								stream->frameBufferSize - startByte);
					data += stream->frameBufferSize - startByte;
					startByte = 0;
				}
				else
				{
					// don't increment "data", skip the buffer
					startByte -= stream->frameBufferSize;
				}
				if ((err != NO_ERR) || done)
				{
					break;
				}
				count++;
			}
			
			if (err == NO_ERR)
			{
			UINT32			decodedBytes;
			UINT32			definedBytes;

				decodedBytes = data - (XBYTE*)decodingData;
				definedBytes = dst->waveFrames * bytesPerFrame;
				if (decodedBytes < definedBytes)
				{
					//MOE: This is a little weird that the number of frames decoded is less
					//     When ascertaining the true number of frames is better (with a
					//	   better MPEG decoder), we shouldn't see this happening.
					definedBytes = decodedBytes;
					dst->waveFrames = decodedBytes / bytesPerFrame;
				}
				
				if (definedBytes < decodingBytes)
				{
				XPTR			trimmedData;
				
					trimmedData = XResizePtr(decodingData, definedBytes);
					BAE_ASSERT(trimmedData);	// making memory block smaller shouldn't fail
					if (trimmedData)
					{
						decodingData = trimmedData;
					}
				}
				
				dst->theWaveform = (XSBYTE *)decodingData;
				dst->waveSize = definedBytes;
			}
			else
			{
				BAE_ASSERT(FALSE);	// check "err"
				XDisposePtr(decodingData);
				decodingData = NULL;
			}
		}
		else
		{
			err = MEMORY_ERR;
		}
		XCloseMPEGStream(stream);
	}
	return err;
}

#endif	// USE_MPEG_DECODER

#if USE_MPEG_ENCODER != 0
#if 0
	#pragma mark ееееееееееееее MPEG ENCODER еееееееееееееееее
#endif
// scan audio data for first non-silent frame from an mpeg stream
static unsigned long PV_ScanForAudioData(short int* data, unsigned long dataBytes,
											unsigned long channelCount)
{
	unsigned long		dataWords;
	unsigned long		offset;

	dataWords = dataBytes / 2;
	offset = 0;
	while (offset < dataWords)
	{
		if (data[offset] != 0)
		{
			break;
		}
		offset++;
	}
	
	return offset / channelCount;
}

// XProcessMPEGEncoder() seems to prepend some number zero samples to the waveform.
// Scan the first frame buffer for the first nonzero sample and
// set *startFrame to the offset to it.
static unsigned long PV_ScanForAudioDataFromMPEG(XPTR pMPEGStream,
													unsigned long mpegStreamSize,
													unsigned long *pFrameBufferCount,
													OPErr *pErr)
{
unsigned long		firstNonSilentFrame;
XMPEGDecodedData	*stream;
OPErr				err;
char				*tempBuffer;
XBOOL				done;

	if (pErr)
	{
		*pErr = NO_ERR;
	}
	firstNonSilentFrame = 0;
	stream = XOpenMPEGStreamFromMemory(pMPEGStream, mpegStreamSize, &err);
	if (stream)
	{
		if (pFrameBufferCount)
		{
			if (stream->maxFrameBuffers > *pFrameBufferCount)
			{
				*pFrameBufferCount = stream->maxFrameBuffers;
			}
		}
		
		tempBuffer = (char *)XNewPtr(stream->frameBufferSize * sizeof(long));
		if (tempBuffer)
		{
			err = XFillMPEGStreamBuffer(stream, tempBuffer, &done);
			if (err == NO_ERR)
			{
				firstNonSilentFrame = PV_ScanForAudioData((short int*)tempBuffer, 
												stream->frameBufferSize, 
												(unsigned long)stream->channels);
			}
			XDisposePtr(tempBuffer);
		}

		XCloseMPEGStream(stream);
	}
	if (pErr)
	{
		*pErr = err;
	}
#if 0
	if (firstNonSilentFrame < 448)	//481
	{
		return 448;
	}
#endif
	return firstNonSilentFrame;
}

struct MPEGEncoderPrivate
{
	void				*encoder;
	XPTR				pCompressedAudio;
	unsigned long		compressedAudioSizeInBytes;
	unsigned long		compressedAudioPosition;
};
typedef struct MPEGEncoderPrivate MPEGEncoderPrivate;
#define MPEG_ENCODE_PRIVATE(x)	((MPEGEncoderPrivate *)(x->pPrivateData))

XMPEGEncodeData *	XOpenMPEGEncodeStreamFromMemory(GM_Waveform *pAudio, 
														XMPEGEncodeRate encodeRate,
														OPErr *pErr)
{
	XMPEGEncodeData		*encode;
	OPErr				theErr;
	MPEGEncoderPrivate	*pPrivate;

	theErr = NO_ERR;
	encode = NULL;
	if (pAudio && pErr)
	{
		if ((pAudio->bitSize == 16) && (pAudio->channels <= 2))
		{
			encode = (XMPEGEncodeData *)XNewPtr((long)sizeof(XMPEGEncodeData));
			if (encode)
			{
				encode->pAudio = pAudio;
				encode->encodeRate = encodeRate;
				encode->pPrivateData = XNewPtr((long)sizeof(MPEGEncoderPrivate));
				if (encode->pPrivateData)
				{
					pPrivate = MPEG_ENCODE_PRIVATE(encode);

					// create new mpeg stream to prepare for encoding. Returns NULL if failed.
					pPrivate->encoder = MPG_EncodeNewStream(encodeRate,
											XFIXED_TO_UNSIGNED_LONG(pAudio->sampledRate), pAudio->channels,
											pAudio->theWaveform, pAudio->waveFrames);

					if (pPrivate->encoder)
					{
						encode->maxFrameBuffers = MPG_EncodeMaxFrames(pPrivate->encoder);
						encode->frameBufferSizeInBytes = MPG_EncodeMaxFrameSize(pPrivate->encoder);
						encode->frameBufferSize = 0;
						encode->currentFrameBuffer = 0;
						pPrivate->compressedAudioPosition = 0;
						pPrivate->compressedAudioSizeInBytes = encode->maxFrameBuffers * encode->frameBufferSizeInBytes;
						pPrivate->pCompressedAudio = XNewPtr(pPrivate->compressedAudioSizeInBytes);
						if (pPrivate->pCompressedAudio == NULL)
						{
							BAE_ASSERT(FALSE);
							theErr = MEMORY_ERR;
						}
					}
					else
					{
						BAE_ASSERT(FALSE);
						theErr = PARAM_ERR;
					}
				}
				else
				{
					BAE_ASSERT(FALSE);
					theErr = MEMORY_ERR;
				}
				if (theErr)
				{
					XDisposePtr((XPTR)encode);
					encode = NULL;
				}
			}
			else
			{
				BAE_ASSERT(FALSE);
				theErr = MEMORY_ERR;
			}
		}
		else
		{
			BAE_ASSERT(FALSE);
			theErr = BAD_SAMPLE;
		}
	}
	else
	{
		BAE_ASSERT(FALSE);
		theErr = PARAM_ERR;
	}
	if (pErr)
	{
		*pErr = theErr;
	}
	return encode;
}


OPErr XProcessMPEGEncoder(XMPEGEncodeData *stream)
{
	OPErr				theErr;
	MPEGEncoderPrivate	*pPrivate;
	XPTR				encodedBuffer;
	unsigned long		encodedLength;
	int					result;
	char				*resultBuffer;
	XBOOL				lastFrame;

	theErr = NO_ERR;
	if (stream)
	{
		pPrivate = MPEG_ENCODE_PRIVATE(stream);
		if (pPrivate->pCompressedAudio && pPrivate->encoder)
		{
			result = MPG_EncodeProcess(pPrivate->encoder, &encodedBuffer, &encodedLength, &lastFrame);
			if (lastFrame)
			{
				theErr = STREAM_STOP_PLAY;	// we're done. compress this last buffer and stop
			}
			if (encodedLength)
			{
				resultBuffer = (char *)pPrivate->pCompressedAudio + pPrivate->compressedAudioPosition;
				XBlockMove(encodedBuffer, (XPTR)(resultBuffer), encodedLength);
				pPrivate->compressedAudioPosition += encodedLength;
				stream->currentFrameBuffer++;
			}
			stream->pFrameBuffer = resultBuffer;
			stream->frameBufferSize = encodedLength;
		}
		else
		{
			BAE_ASSERT(FALSE);
			theErr = PARAM_ERR;
		}
	}
	return theErr;
}


OPErr XCloseMPEGEncodeStream(XMPEGEncodeData *stream, XPTR *pReturnedBuffer, unsigned long *pReturnedSize)
{
	MPEGEncoderPrivate	*pPrivate;
	OPErr				err;

	err = NO_ERR;
	if (stream)
	{
		pPrivate = MPEG_ENCODE_PRIVATE(stream);
		if (pPrivate->encoder)
		{
			MPG_EncodeFreeStream(pPrivate->encoder);
		}
		if (pReturnedSize && pReturnedBuffer)
		{
			*pReturnedBuffer = pPrivate->pCompressedAudio;
			*pReturnedSize = pPrivate->compressedAudioPosition;
		}
		XDisposePtr((XPTR)stream->pPrivateData);
		XDisposePtr((XPTR)stream);
	}
	else
	{
		BAE_ASSERT(FALSE);
		err = PARAM_ERR;
	}
	return err;
}

// Given an mpeg bit encode rate, and a sample rate, this will return TRUE if
// this encoder can encode, or FALSE if it will not work.
XBOOL XIsValidMPEGSampleRateAndEncodeRate(XMPEGEncodeRate encodeRate, XFIXED sampleRate, 
		SndCompressionSubType subType, short int numChannels)
{
	XBOOL				safe;
	int					count;
	const int			TEMP_BUFFER_FRAMES = 2500;
	const int			TEMP_BUFFER_SIZE = TEMP_BUFFER_FRAMES * 2;
	XPTR				tempBuffer;
	GM_Waveform			*test;
	OPErr				err;
	XMPEGDecodedData	*stream;
	XFIXED				newSampleRate;

	numChannels; // we igore stereo samples for now.
	
	safe = FALSE;
	if (subType == CS_MPEG1)
	{
		newSampleRate = XGetClosestMPEGSampleRate(sampleRate, subType);
		if (sampleRate < newSampleRate)
		{
			return safe;
		}
	}

	tempBuffer = XNewPtr(TEMP_BUFFER_SIZE);
	if (tempBuffer)
	{
		for (count = 0; count < TEMP_BUFFER_SIZE; count++)
		{
			((XBYTE *)tempBuffer)[count] = (XBYTE)(127.0 * cos(count));
		}

		test = GM_NewWaveform();
		if (test)
		{
			XPTR			encodedData;
			UINT32			encodedBytes;

			test->theWaveform = tempBuffer;
			test->bitSize = 16;
			test->channels = 1;
			test->compressionType = C_NONE;
			test->sampledRate = sampleRate;
			test->waveSize = TEMP_BUFFER_SIZE;
			test->waveFrames = TEMP_BUFFER_FRAMES;

			err = XCompressMPEG(test, 
					XGetMPEGCompressionType(encodeRate),
					subType,
					NULL, NULL,
					&encodedData, &encodedBytes,
					NULL, NULL, NULL);

			if (err == NO_ERR)
			{
				// Have to open the stream simply to find out if the stream we just built is
				// valid.
				stream = XOpenMPEGStreamFromMemory(encodedData, encodedBytes, &err);
				if (stream && (err == NO_ERR))
				{
					safe = TRUE;
				}
				XCloseMPEGStream(stream);
			}
			XDisposePtr(encodedData);
			test->theWaveform = NULL;
			GM_FreeWaveform(test);
		}
		XDisposePtr(tempBuffer);
	}
	return safe;
}

// This will encode an MPEG stream from a formatted GM_Waveform
OPErr XCompressMPEG(GM_Waveform const* wave, 
					SndCompressionType compressionType,
					SndCompressionSubType compressionSubType,
					XCompressStatusProc proc, void* procData,
					XPTR* pCompressedData, XDWORD* pCompressedBytes,
					XDWORD* pFrameBufferCount, XDWORD* pFrameBufferBytes,	//MOE: we don't really need these
					XDWORD* startFrame)
{
XMPEGEncodeRate		const encodeRate = XGetMPEGEncodeRate(compressionType);
GM_Waveform			hackedWave;
XMPEGEncodeRate		hackedEncodeRate;
OPErr				err;
XMPEGEncodeData*	mpegEncode;
unsigned long		originalOffset;

	// capture any original silence prior to encoding
	originalOffset = PV_ScanForAudioData((short int *)wave->theWaveform,
										wave->waveSize,
										wave->channels);

	hackedWave = *wave;
	XGetClosestMPEGSampleRateAndEncodeRate(wave->sampledRate, encodeRate,
											&hackedWave.sampledRate,
											&hackedEncodeRate,
											compressionSubType
											);

	// MOE: The above code depends on the fact that XMPEGEncodeRate{}s are defined to
	// equal their bitrates divided by 1024 (defined in X_Formats.h).

	*pCompressedData = NULL;
	mpegEncode = XOpenMPEGEncodeStreamFromMemory(&hackedWave, hackedEncodeRate, &err);
	if (mpegEncode)
	{
	OPErr				closeErr;
		
		while (TRUE)
		{
			err = XProcessMPEGEncoder(mpegEncode);
			if (err != NO_ERR)
			{
				if (err == STREAM_STOP_PLAY) err = NO_ERR;
				else BAE_ASSERT(FALSE);
				break;
			}
			
			if (proc && (*proc)(procData,
								mpegEncode->currentFrameBuffer,
								mpegEncode->maxFrameBuffers))
			{
				err = ABORTED_PROCESS;
				break;
			}
		}
		
		if (pFrameBufferBytes)
		{
			*pFrameBufferBytes = mpegEncode->frameBufferSizeInBytes;
		}
		if (pFrameBufferCount)
		{
			*pFrameBufferCount = mpegEncode->maxFrameBuffers;
		}

		closeErr = XCloseMPEGEncodeStream(mpegEncode, pCompressedData, pCompressedBytes);
		if (err == NO_ERR)
		{
			err = closeErr;
		}
		if ((err == NO_ERR) && startFrame)
		{
			// XProcessMPEGEncoder() seems to prepend some number zero samples to the waveform.
			// Scan the first frame buffer for the first nonzero sample and
			// set *startFrame to the offset to it.
#if 0
			*startFrame = 0;
#else
			*startFrame =
				PV_ScanForAudioDataFromMPEG(*pCompressedData, *pCompressedBytes,
											pFrameBufferCount, &err);
			if (*startFrame > originalOffset)
			{
				*startFrame -= originalOffset;
			}
#endif
		}

		if (err != NO_ERR)
		{
			XDisposePtr(*pCompressedData);
			*pCompressedData = NULL;
		}
	}
	return err;
}

#endif


#endif	// USE_HIGHLEVEL_FILE_API


