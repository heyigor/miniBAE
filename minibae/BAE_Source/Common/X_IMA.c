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
**	X_IMA.c
**
**	Functions that...
**		compress 16-bit linear audio data into Mac IMA (ADPCM) format
**		decompress Mac IMA audio data to 16-bit linear
**		decompress Microsoft IMA audio data to 16-bit linear
**
**	© Copyright 1996-2000 Beatnik, Inc, All Rights Reserved.
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
**	3/12/98		Created (by Moe)
**	3/12/98		MOE: Added XAllocateCompressedAiffIma() and XCompressAiffIma()
**	3/12/98		MOE: Re-wrote XExpandMacADPCMtoXBitLinear()
**	3/18/98		Igor: Added some wrappers around functions, and enabled the old
**				decompressor because its currently broken for stereo samples
**	3/20/98		MOE: Re-wrote XExpandIMAtoXBitLinearStreamed()
**				set USE_NEW_EXPAND_CODE to FALSE to get back to old code
**	3/20/98		MOE: Re-wrote XExpandIMAtoXBitLinearStreamed()
**	3/20/98		MOE: Renamed XExpandIMAtoXBitLinearStreamed()-->XExpandAiffImaStream()
**	3/20/98		MOE: Renamed XExpandMacADPCMtoXBitLinear()-->XExpandAiffIma()
**	3/20/98		MOE: Renamed XDecodeIMA4Bit()-->XExpandWavIma()
**	3/23/98		MOE: Lots of changes made, comments deleted by source code control
**	5/12/98		MOE: Fixed XCompressAiffIma() to encode the proper "predictor"
**				bytes.
**	5/12/98		MOE: Changed all the "ExpandAiffIma" functions to accept a
**				predictorCache[] parameter, rather than an indexCache[], taking
**				advantage of new knowledge of how Apple's IMA data is formatted.
**				(We COULD get rid of all cache data entirely, but the precision
**				 of predictor bytes would be limited to 9 bits.)
**				NOTE: We really need to clean up bad-format-error-handling in
**				the functions in GenSoundFiles() that call the expansion
**				functions here.
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
**	8/4/98		Fixed >= bug in PV_ExpandIma8 for MacOS platform.
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	3/7/2000	Made XExpandWavIma compile in all cases.
**	10/20/2000	Fixed two data access bugs in PV_ExpandIma8 & PV_ExpandIma16 in which
**				an odd address accessing a long word caused an violation.
**	1/27/2002	sh Fixed warnings.
*/
/*****************************************************************************/


#include "X_API.h"
#include "X_Formats.h"


///////////////////////////////////////////////// DOCUMENTATION:

// Apple's AIFF format documentation (typos/grammar fixed-up a bit by Moe):
//
// AIFF IMA data is formatted in a stream of 64 byte blocks, with a 2 byte predictor. 
// This allows random access playback of the data. The predictor is the same value 
// computed for the block if it were compressed in a streaming fashion.  But 
// saving this value in each block allows random starting points, so long as 
// you start at a block boundary.
//
// In our IMA data, you get the following 
// compression info when calling GetCompressionInfo().
//
// 64 samplesPerPacket
// 34 bytesPerPacket, 68 bytesPerPacket for stereo
// 2 bytesPerSample
//
// In the CmdSoundHeader, you'll find the field numFrames, which is the 
// number of blocks/frames/packets, each containing 64 decompressed samples.
//
// Moe's notes:
//	Stereo data is stored by interleaving left and right blocks.
//	Though 64 frames-per-block is specd, my expansion code handles any passed-in block length.
//	The compression code compresses 64-frames-per-block
//	the 2-byte predictor is in (of course) Motorola byte-order.  It's bits are allocated:
//		15:8 - most significant 9 bits of the predictor value
//		 7:0 - index into imaStepTable
//		(This data from http://developer.apple.com/technotes/tn/tn1081.html)

// WAV IMA data is formatted in blocks of any (constant) size.  
// Mono blocks have a 4-byte header, followed by IMA nybbles in exactly the
// same format as AIFF IMA data.
// Stereo blocks have a 4-byte header for each channel, followed by 4-bytes of
// left-channel nybbles, followed by 4-bytes of right channel nybbles, and so on.
// (The left channel's header comes first.)
// The header has the following structure:
//	2bytes - the predictor sample, in Intel byte-order
//	1byte  - the initial index value
//  1byte  - unused
// Unlike in AIFF IMA data, the predictor sample is stored as the first sample in a block.


///////////////////////////////////////////////// SWITCHES:

#define	USE_NEW_EXPAND_CODE		TRUE	// set to FALSE to get Steve's and Microsoft's old code
#define	EXPAND_PARTIAL_BLOCKS	FALSE	// whether to handle the data in incomplete blocks
#define	FRAMES_IN_EIGHTS		FALSE	// whether frame counts are always in multiples of eight


///////////////////////////////////////////////// STATIC DATA:

static char				const imaIndexIncrementTable[16] =
{
	-1,-1,-1,-1, 2, 4, 6, 8,
	-1,-1,-1,-1, 2, 4, 6, 8
};
static unsigned short	const imaStepTable[89] =
{	
	7,   8,   9,   10,  11,  12,  13, 14,
	16,  17,  19,  21,  23,  25,  28,
	31,  34,  37,  41,  45,  50,  55,
	60,  66,  73,  80,  88,  97, 107,
	118, 130, 143, 157, 173, 190, 209,
	230, 253, 279, 307, 337, 371, 408,
	449, 494, 544, 598, 658, 724, 796,
	876, 963,1060,1166,1282,1411,1552,
  	1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,
	4428,4871,5358,5894,6484,7132,7845,8630,
	9493,10442,11487,12635,13899,15289,16818,
	18500,20350,22385,24623,27086,29794,32767
};


///////////////////////////////////////////////// MACROS:

#define PROCESS_IMA_NYBBLE(nybble, sample, index)			\
{															\
unsigned int	const step = imaStepTable[index];			\
INT32			nextSample;									\
															\
	/* nextSample = (nybble + .5) * step / 4.0 */			\
	nextSample = step >> 3;									\
	if ((nybble) & 0x1) nextSample += step >> 2;			\
	if ((nybble) & 0x2) nextSample += step >> 1;			\
	if ((nybble) & 0x4) nextSample += step;					\
	if ((nybble) & 0x8) nextSample = -nextSample;			\
	nextSample += sample;									\
	if ((INT32)(short)nextSample != nextSample)				\
	{	/* cute way to test for 16-bit overflow */			\
		if (nextSample > 32767) nextSample = 32767;			\
		else nextSample = -32768;							\
	}														\
	sample = (short)nextSample;								\
															\
	index += imaIndexIncrementTable[nybble];				\
	if (index < 0) index = 0;								\
	if (index > 88) index = 88;								\
}


///////////////////////////////////////////////// FORWARD DECLARATIONS:

static void PV_CompressImaBlock(XBYTE const* src8, INT16 const* src16, XBYTE* dst,
								XDWORD sampleCount, XDWORD channelCount,
								short* predictorCache, short* indexCache);

static void PV_ExpandAiffIma(XBYTE const* src, XDWORD srcBytesPerBlock,
								void *dst, XDWORD dstBytesPerSample,
								XDWORD frameCount, XDWORD channelCount, 
								short predictorCache[2]);
static void PV_ExpandIma8(XBYTE const* src, XBYTE* dst8,
							XDWORD sampleCount, XDWORD channelCount,
							short* predictorCache, short index);
static void PV_ExpandIma16(XBYTE const* src, INT16* dst16,
							XDWORD sampleCount, XDWORD channelCount,
							short* predictorCache, short index);

static XDWORD PV_ExpandWavImaStereo8(XBYTE const* src, XBYTE* dst8, XDWORD sampleCount);
static XDWORD PV_ExpandWavImaStereo16(XBYTE const* src, INT16* dst16, XDWORD sampleCount);


///////////////////////////////////////////////// MAC-IMA COMPRESSION FUNCTIONS:
#if USE_CREATION_API == TRUE

XPTR XAllocateCompressedAiffIma(void const* src, XDWORD srcBitsPerSample,
								XDWORD frameCount, XDWORD channelCount)
{
XDWORD		blockCount;
XDWORD		bytes;
XPTR		data;

	blockCount = (frameCount + AIFF_IMA_BLOCK_FRAMES - 1) / AIFF_IMA_BLOCK_FRAMES;
	bytes = blockCount * (AIFF_IMA_BLOCK_FRAMES / 2 + 2);
	bytes *= channelCount;

	data = XNewPtr(bytes);	// we rely on the fact that the new memory contains zeroes
	if (data)
	{
		XCompressAiffIma(src, srcBitsPerSample, (XBYTE *)data, frameCount, channelCount);
	}
	return data;
}
void XCompressAiffIma(void const* src, XDWORD srcBitsPerSample,
						XBYTE* dst, XDWORD frameCount, XDWORD channelCount)
{
int				const srcIncrement = (AIFF_IMA_BLOCK_FRAMES - 1) * channelCount;
short			predictorCache[2];	// to allow for compression of more than 2
short			indexCache[2];		// channels, increase these array sizes
XBYTE const*	src8;
INT16 const*	src16;

	predictorCache[0] = 0;
	predictorCache[1] = 0;
	indexCache[0] = 0;
	indexCache[1] = 0;

	if (srcBitsPerSample == 8)
	{
		src8 = (XBYTE const*)src;
		src16 = NULL;
	}
	else
	{
		src8 = NULL;
		src16 = (INT16 const*)src;
	}

	while (frameCount > 0)
	{
	XDWORD		const blockFrameCount = XMIN(frameCount, AIFF_IMA_BLOCK_FRAMES);
	XDWORD		channel;

		channel = 0;
		do
		{
			XPutShort(dst, (unsigned short)((predictorCache[channel] & 0xFF80) | indexCache[channel]));
			dst += AIFF_IMA_HEADER_BYTES;

			PV_CompressImaBlock(src8, src16, dst,
								blockFrameCount, channelCount,
								&predictorCache[channel], &indexCache[channel]);
			dst += AIFF_IMA_BLOCK_BYTES - AIFF_IMA_HEADER_BYTES;
			if (src16)
			{
				src16++;
			}
			else
			{
				src8++;
			}
			channel++;
		}
		while (channel < channelCount);
		
		if (src16)
		{
			src16 += srcIncrement;
		}
		else
		{
			src8 += srcIncrement;
		}
		frameCount -= blockFrameCount;
	}
}	

void PV_CompressImaBlock(XBYTE const* src8, INT16 const* src16, XBYTE* dst,
								XDWORD sampleCount, XDWORD channelCount,
								short* predictorCache, short* indexCache)
{
INT32		sample;
short		index;
XDWORD		sampleIndex;

	sample = *predictorCache;
	index = *indexCache;

	sampleIndex = 0;
	while (sampleIndex < sampleCount)
	{
	XDWORD			const step = imaStepTable[index];
	XDWORD			delta;
	INT32			sampleOffset;
	unsigned int	nybble;

		if (src16)
		{
			delta = (INT32)*src16 - sample;
			src16 += channelCount;
		}
		else
		{
			delta = ((INT32)(*src8 - 128) << 8) - sample;
			src8 += channelCount;
		}

		// compute next data-nybble and sample offset
		sampleOffset = step >> 3;
		nybble = 0x0;
		if ((INT32)delta < 0)
		{
			delta = -(INT32)delta;
			nybble |= 0x8;
		}
		if (delta >= step)
		{
			delta -= step;
			sampleOffset += step;
			nybble |= 0x4;
		}
		if (delta >= step >> 1)
		{
			delta -= step >> 1;
			sampleOffset += step >> 1;
			nybble |= 0x2;
		}
		if (delta >= step >> 2)
		{
			sampleOffset += step >> 2;
			nybble |= 0x1;
		}
		if (nybble & 0x8)
		{
			sampleOffset = -sampleOffset;
		}
		
		sample += sampleOffset;
		if ((INT32)(short)sample != sample)	// cute way to test for 16-bit overflow
		{
			if (sample > 32767) sample = 32767;
			else sample = -32768;
		}

		index += imaIndexIncrementTable[nybble];
		if (index < 0) index = 0;
		if (index > 88) index = 88;

		if ((sampleIndex & 1) == 0)
		{
			*dst = (XBYTE)nybble;
		}
		else
		{
			*dst |= (XBYTE)(nybble << 4);
			dst++;
		}
		sampleIndex++;
	}

	*predictorCache = (short)sample;
	*indexCache = index;
}

#endif	// USE_CREATION_API == TRUE


#if	USE_NEW_EXPAND_CODE == TRUE


///////////////////////////////////////////////// AIFF-IMA DECOMPRESSION FUNCTIONS:

void XExpandAiffIma(XBYTE const* src, XDWORD srcBytesPerBlock,
					void* dst, XDWORD dstBitsPerSample,
					XDWORD frameCount, XDWORD channelCount)
{
short		predictorCache[2];

	predictorCache[0] = 0;
	predictorCache[1] = 0;

	frameCount &= ~1;	// frame count cannot be odd
	
	PV_ExpandAiffIma(src, srcBytesPerBlock,
						dst, dstBitsPerSample / 8,
						frameCount, channelCount, predictorCache);
}

void PV_ExpandAiffIma(XBYTE const* src, XDWORD srcBytesPerBlock,
						void *dst, XDWORD dstBytesPerSample,
						XDWORD frameCount, XDWORD channelCount, 
						short predictorCache[2])
{
XDWORD		const framesPerBlock =
					(srcBytesPerBlock - AIFF_IMA_HEADER_BYTES) * 2;
XDWORD		const dstIncrement = (framesPerBlock - 1) * channelCount;

	if (dstBytesPerSample == 1)
	{
	XBYTE*		dst8;

		dst8 = (XBYTE*)dst;
		while ((INT32)frameCount > 0)
		{
#if EXPAND_PARTIAL_BLOCKS
		XDWORD		const blockFrameCount = XMIN(frameCount, framesPerBlock);
#else
		XDWORD		const blockFrameCount = framesPerBlock;
#endif
		XDWORD		channel;

			channel = 0;
			do
			{
			unsigned short	index;

				index = XGetShort(src) & 0x7F;	// discard predictor bits
				if ((unsigned short)index > 88)
				{
					//??STEVE:  I want to return an error code here but your code above won't support it
					index = 88;
				}
				src += AIFF_IMA_HEADER_BYTES;		// skip block header
				PV_ExpandIma8(src, dst8,
								blockFrameCount, channelCount,
								&predictorCache[channel], index);
				src += srcBytesPerBlock - AIFF_IMA_HEADER_BYTES;
				dst8++;
				channel++;
			}
			while (channel < channelCount);

			dst8 += dstIncrement;
			frameCount -= blockFrameCount;
		}
	}
	else
	{
	INT16*		dst16;

		dst16 = (INT16*)dst;
		while ((INT32)frameCount > 0)
		{
#if EXPAND_PARTIAL_BLOCKS
		XDWORD		const blockFrameCount = XMIN(frameCount, framesPerBlock);
#else
		XDWORD		const blockFrameCount = framesPerBlock;
#endif
		XDWORD		channel;

			channel = 0;
			do
			{
			unsigned short	index;

				index = XGetShort(src) & 0x7F;	// discard predictor bits
				if ((unsigned short)index > 88)
				{
					//??STEVE:  I want to return an error code here but your code above won't support it
					index = 88;
				}
				src += AIFF_IMA_HEADER_BYTES;		// skip block header
				PV_ExpandIma16(src, dst16,
								blockFrameCount, channelCount,
								&predictorCache[channel], index);
				src += srcBytesPerBlock - AIFF_IMA_HEADER_BYTES;
				dst16++;
				channel++;
			}
			while (channel < channelCount);
			
			dst16 += dstIncrement;
			frameCount -= blockFrameCount;
		}
	}
}

void PV_ExpandIma8(XBYTE const* src, XBYTE* dst8,
					XDWORD sampleCount, XDWORD channelCount,
					short* predictorCache, short index)
{
short			sample;

	sample = *predictorCache;

#if X_WORD_ORDER == TRUE	// intel byte order
	while (sampleCount > 0)
	{
	XDWORD		nybbles;
	int			nybbleCount;

		nybbles = XGetLongIntel(src);	// this is required because some CPU's
										// cannot access data as a long on odd addresses
		src += sizeof(XDWORD);
#if FRAMES_IN_EIGHTS == TRUE
		nybbleCount = 8;
#else
		nybbleCount = XMIN(sampleCount, 8);
#endif
		while (--nybbleCount >= 0)
		{
			PROCESS_IMA_NYBBLE(nybbles & 0x0F, sample, index)
			*dst8 = (XBYTE)((sample >> 8) + 128);	// make unsigned
			dst8 += channelCount;
			nybbles >>= 4;
		}
	
		sampleCount -= 8;
	}
#else	// X_WORD_ORDER == TRUE
	while ((long)(sampleCount -= 2) >= 0)
	{
	unsigned short	nybbles;

		nybbles = *src++;
		nybbles |= 0x1000;	// loop flag, shifted right to zero
		while (nybbles & 0xFF00)
		{
			PROCESS_IMA_NYBBLE(nybbles & 0xF, sample, index)
			*dst8 = (XBYTE)((sample >> 8) + 128);	// make unsigned
			dst8 += channelCount;
			nybbles >>= 4;
		}
	}
#endif	// X_WORD_ORDER == TRUE

	*predictorCache = sample;
}
void PV_ExpandIma16(XBYTE const* src, INT16* dst16,
					XDWORD sampleCount, XDWORD channelCount,
					short* predictorCache, short index)
{
short			sample;

	sample = *predictorCache;

#if X_WORD_ORDER == TRUE	// intel byte order
	while (sampleCount > 0)
	{
	XDWORD		nybbles;
	int			nybbleCount;

		nybbles = XGetLongIntel(src);	// this is required because some CPU's
										// cannot access data as a long on odd addresses
		src += sizeof(XDWORD);
#if FRAMES_IN_EIGHTS == TRUE
		nybbleCount = 8;
#else
		nybbleCount = XMIN(sampleCount, 8);
#endif
		while (--nybbleCount >= 0)
		{
			PROCESS_IMA_NYBBLE(nybbles & 0x0F, sample, index)
			*dst16 = sample;
			dst16 += channelCount;
			nybbles >>= 4;
		}
	
		sampleCount -= 8;
	}
#else	// X_WORD_ORDER == TRUE
	while ((long)(sampleCount -= 2) >= 0)
	{
	unsigned short	nybbles;

		nybbles = *src++;
		nybbles |= 0x1000;	// loop flag, shifted right to zero
		while (nybbles & 0xFF00)
		{
			PROCESS_IMA_NYBBLE(nybbles & 0xF, sample, index)
			*dst16 = sample;
			dst16 += channelCount;
			nybbles >>= 4;
		}
	}
#endif	// X_WORD_ORDER == TRUE

	*predictorCache = sample;
}


///////////////////////////////////////////////// AIFF-IMA STREAMING DECOMPRESSION FUNCTIONS:
#if USE_HIGHLEVEL_FILE_API

// returns the number of bytes stored at dst
XDWORD XExpandAiffImaStream(XBYTE const* src, XDWORD srcBytesPerBlock,
							void *dst, XDWORD dstBitsPerSample,
							XDWORD srcBytes, XDWORD channelCount,
							short predictorCache[2])
{
XDWORD		const framesPerBlock =
				(srcBytesPerBlock - AIFF_IMA_HEADER_BYTES) * 2;
XDWORD		const blockCount = srcBytes / srcBytesPerBlock;
XDWORD		const dstBytesPerSample = dstBitsPerSample / 8;
XDWORD		frameCount;
XDWORD		dstBytes;

	if (!((dstBitsPerSample == 8) || (dstBitsPerSample == 16)) ||
		!((channelCount == 1) || (channelCount == 2)))
	{
		//?? STEVE:  The code that calls this doesn't see this return value as an error!
		return 0;	// incompatible format
	}
	
	frameCount = (blockCount / channelCount) * framesPerBlock;
#if EXPAND_PARTIAL_BLOCKS
	if (channelCount == 1)
	{
	XDWORD		const extraBytes = srcBytes % srcBytesPerBlock;
	
		if (extraBytes > AIFF_IMA_HEADER_BYTES)
		{
			frameCount += (extraBytes - AIFF_IMA_HEADER_BYTES) * 2;
		}
	}
#endif
	dstBytes = frameCount * dstBytesPerSample * channelCount;

	PV_ExpandAiffIma(src, srcBytesPerBlock,
						dst, dstBytesPerSample,
						frameCount, channelCount, predictorCache);

	return dstBytes;
}

#endif	// USE_HIGHLEVEL_FILE_API


///////////////////////////////////////////////// WAV-IMA DECOMPRESSION FUNCTIONS:
#if 1
// USE_HIGHLEVEL_FILE_API
// returns the number of bytes stored at dst
XDWORD XExpandWavIma(XBYTE const* src, XDWORD srcBytesPerBlock,
						void* dst, XDWORD dstBitsPerSample,
						XDWORD srcBytes, XDWORD channelCount)
{
XDWORD		const framesPerBlock =
	1 + (srcBytesPerBlock / channelCount - WAV_IMA_HEADER_BYTES) * 2;
XDWORD		const blockCount = srcBytes / srcBytesPerBlock;
XDWORD		const dstBytesPerSample = dstBitsPerSample / 8;
XDWORD		frameCount;
XDWORD		dstBytes;
	
	if (!((dstBitsPerSample == 8) || (dstBitsPerSample == 16)) ||
		!((channelCount == 1) || (channelCount == 2)))
	{
		return 0;	// incompatible format
	}
	
	frameCount = blockCount * framesPerBlock;
#if EXPAND_PARTIAL_BLOCKS
	{
	XDWORD		extraBytes;

		extraBytes = (srcBytes % srcBytesPerBlock) / channelCount;
		if (extraBytes >= WAV_IMA_HEADER_BYTES)
		{
			extraBytes -= WAV_IMA_HEADER_BYTES;
			if (channelCount == 2)
			{
				extraBytes -= extraBytes % sizeof(XDWORD);
			}
			frameCount += 1 + extraBytes * 2;
	}
#endif
	dstBytes = frameCount * dstBytesPerSample * channelCount;

	if (channelCount == 1)
	{
		if (dstBytesPerSample == 1)
		{
		XBYTE*		dst8;
	
			dst8 = (XBYTE*)dst;
			while ((INT32)frameCount > 0)
			{
#if EXPAND_PARTIAL_BLOCKS
			XDWORD			const blockFrameCount = XMIN(frameCount, framesPerBlock);
#else
			XDWORD			const blockFrameCount = framesPerBlock;
#endif
			short			predictor;
			unsigned short	index;
		
				predictor = XGetShortIntel(src);
				index  = (unsigned short)src[sizeof(short)];
				if ((unsigned short)index > 88)
				{
					return 0;	// invalid data
				}
				src += WAV_IMA_HEADER_BYTES;
				
				*dst8++ = (XBYTE)(predictor >> 8) + 128;	// first sample in each block is predictor
				PV_ExpandIma8(src, dst8,
								blockFrameCount - 1, 1,
								&predictor, index);
				src += srcBytesPerBlock - WAV_IMA_HEADER_BYTES;
				dst8 += blockFrameCount - 1;

				frameCount -= blockFrameCount;
			}
		}
		else
		{
		INT16*		dst16;
	
			dst16 = (INT16*)dst;
			while ((INT32)frameCount > 0)
			{
#if EXPAND_PARTIAL_BLOCKS
			XDWORD			const blockFrameCount = XMIN(frameCount, framesPerBlock);
#else
			XDWORD			const blockFrameCount = framesPerBlock;
#endif
			short			predictor;
			unsigned short	index;
		
				predictor = XGetShortIntel(src);
				index  = (unsigned short)src[sizeof(short)];
				if ((unsigned short)index > 88)
				{
					return 0;	// invalid data
				}
				src += WAV_IMA_HEADER_BYTES;
				
				*dst16++ = predictor;	// first sample in each block is predictor
				PV_ExpandIma16(src, dst16,
								blockFrameCount - 1, 1,
								&predictor, index);
				src += srcBytesPerBlock - WAV_IMA_HEADER_BYTES;
				dst16 += blockFrameCount - 1;

				frameCount -= blockFrameCount;
			}
		}
	}
	else	// (channelCount == 1)
	{
		if (WAV_IMA_HEADER_BYTES != sizeof(XDWORD))
		{
			return 0;	// adding WAV_IMA_HEADER_BYTES to src to access right channel won't work!
		}

		if (dstBytesPerSample == 1)
		{
		XBYTE*		dst8;
	
			dst8 = (XBYTE*)dst;
			while ((INT32)frameCount > 0)
			{
#if EXPAND_PARTIAL_BLOCKS
			XDWORD		const blockFrameCount = XMIN(frameCount, framesPerBlock);
#else
			XDWORD		const blockFrameCount = framesPerBlock;
#endif
				if (PV_ExpandWavImaStereo8(src, dst8, blockFrameCount) == 0)
				{
					return 0;
				}
				if (PV_ExpandWavImaStereo8(src + WAV_IMA_HEADER_BYTES,
											dst8 + 1, blockFrameCount) == 0)
				{
					return 0;
				}
				src += srcBytesPerBlock;
				dst8 += 2 * blockFrameCount;

				frameCount -= blockFrameCount;
			}
		}
		else
		{
		INT16*		dst16;
	
			dst16 = (INT16*)dst;
			while ((INT32)frameCount > 0)
			{
#if EXPAND_PARTIAL_BLOCKS
			XDWORD		const blockFrameCount = XMIN(frameCount, framesPerBlock);
#else
			XDWORD		const blockFrameCount = framesPerBlock;
#endif
				if (PV_ExpandWavImaStereo16(src, dst16, blockFrameCount) == 0)
				{
					return 0;
				}
				if (PV_ExpandWavImaStereo16(src + WAV_IMA_HEADER_BYTES,
											dst16 + 1, blockFrameCount) == 0)
				{
					return 0;
				}
				src += srcBytesPerBlock;
				dst16 += 2 * blockFrameCount;

				frameCount -= blockFrameCount;
			}
		}
	}
	return dstBytes;
}

XDWORD PV_ExpandWavImaStereo8(XBYTE const* src, XBYTE* dst8, XDWORD sampleCount)
{
short		sample;
short		index;

	sample = XGetShortIntel(src);
	index  = src[sizeof(short)];
	if ((unsigned short)index > 88)
	{
		return 0;	// invalid data
	}
	src += 2 * WAV_IMA_HEADER_BYTES;	// skip other channel's header

	*dst8 = (XBYTE)(sample >> 8) + 128;
	dst8 += 2;	// skip other channel's first sample
	sampleCount--;
	
	while ((long)(sampleCount -= 8) >= 0)
	{
	XDWORD		nybbles;
	int			nybbleCount;

		nybbles = XGetLongIntel(src);
		src += 2 * sizeof(XDWORD);	// skip other channel's dword

		nybbleCount = 8;
		while (--nybbleCount >= 0)
		{
			PROCESS_IMA_NYBBLE(nybbles & 0x0F, sample, index)
			*dst8 = (XBYTE)((sample >> 8) + 128);	// make unsigned
			dst8 += 2;	// skip other channel's sample
			nybbles >>= 4;
		}
	}
	return 1;
}
XDWORD PV_ExpandWavImaStereo16(XBYTE const* src, INT16* dst16, XDWORD sampleCount)
{
short		sample;
short		index;

	sample = XGetShortIntel(src);
	index  = src[sizeof(short)];
	if ((unsigned short)index > 88)
	{
		return 0;	// invalid data
	}
	src += 2 * WAV_IMA_HEADER_BYTES;	// skip other channel's header

	*dst16 = sample;
	dst16 += 2;	// skip other channel's first sample
	sampleCount--;

	while ((long)(sampleCount -= 8) >= 0)
	{
	XDWORD		nybbles;
	int			nybbleCount;

		nybbles = XGetLongIntel(src);
		src += 2 * sizeof(XDWORD);	// skip other channel's dword

		nybbleCount = 8;
		while (--nybbleCount >= 0)
		{
			PROCESS_IMA_NYBBLE(nybbles & 0x0F, sample, index)
			*dst16 = sample;
			dst16 += 2;	// skip other channel's sample
			nybbles >>= 4;
		}
	}
	return 1;
}

#endif	// USE_HIGHLEVEL_FILE_API


/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
#else	// USE_NEW_EXPAND_CODE
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////


///////////////////////////////////////////////// AIFF-IMA DECOMPRESSION FUNCTIONS:

void XExpandAiffIma(XBYTE const* src, XDWORD srcBytesPerBlock,
					void* dst, XDWORD dstBitsPerSample,
					XDWORD frameCount, XDWORD channelCount)
{
char const*		pSourceL;
char const*		pSourceR;
short int		*pDest16;
char			*pDest8;
long			stepL, indexL;
long			diffL, predsampleL;
char			codebufL, codeL;
long			stepR, indexR;
long			diffR, predsampleR;
char			codebufR, codeR;
XDWORD			count;
char			macBlockOffset;

	if ( (dstBitsPerSample == 8) || (dstBitsPerSample == 16) )
	{
		// setup inital predictor and index
		stepL = 7;
		predsampleL = 0;
		indexL = 0;
		codebufL = 0;
		pDest16 = (short int *)dst;
		pDest8 = (char *)dst;
		if (channelCount == 1)
		{
			macBlockOffset = 32;	// skip first two predictor bytes
			for (count = 0; count < frameCount; count++)
			{
				if (macBlockOffset == 32)
				{
					macBlockOffset = 0;
					src += 2;	// skip next predictor bytes
				}

				if	(count & 1)							/* two samples per inbuf char */
				{
					codeL = (codebufL >> 4) & 0x0F;
				}
				else	
				{
					codebufL = *src++;			/* buffer two ADPCM nibbles */
					codeL = codebufL & 0x0F;
					macBlockOffset++;
				}
				/* compute new sample estimate predsample */
				 diffL = 0;
				 if (codeL & 4) 
				 	diffL += stepL;
				 if (codeL & 2) 
				 	diffL += stepL >> 1;
				 if (codeL & 1) 
				 	diffL += stepL >> 2;
				 diffL += stepL >> 3;
				 if (codeL & 8)
				 	diffL = -diffL;
				predsampleL += diffL;

				 if (predsampleL > 32767)				/* check for overflow */
				 	predsampleL = 32767;
				 else if (predsampleL < -32768)
				 	predsampleL = -32768;

				/* store estimate to output buffer */
				if (dstBitsPerSample == 16)
				{
					*pDest16++ = (short int)predsampleL;		
				}
				else
				{
					*pDest8++ = (predsampleL >> 8L) - 128;
				}
				/* compute new stepsize step */
				indexL += imaIndexIncrementTable[codeL];
				if(indexL < 0) indexL = 0;
				else if(indexL > 88) indexL = 88;
				stepL = imaStepTable[indexL];
			}
		}
		else
		{	// STEREO DECODER
			pSourceL = src - 34;
			pSourceR = src;		// handle the starting case
			stepR = 7;
			predsampleR = 0;
			indexR = 0;
			codebufR = 0;
			macBlockOffset = 32;	// skip first two predictor bytes

			for (count = 0; count < frameCount; count++)
			{
				if (macBlockOffset == 32)
				{
					macBlockOffset = 0;
					pSourceL += 2 + 34;	// skip next predictor bytes
					pSourceR += 2 + 34;	// skip next predictor bytes
				}

				if	(count & 1)							/* two samples per inbuf char */
				{
					codeL = (codebufL >> 4) & 0x0F;
					codeR = (codebufR >> 4) & 0x0F;
				}
				else	
				{
					codebufL = *pSourceL++;			/* buffer two ADPCM nibbles */
					codeL = codebufL & 0x0F;
					codebufR = *pSourceR++;			/* buffer two ADPCM nibbles */
					codeR = codebufR & 0x0F;
					macBlockOffset++;
				}
				/* compute new sample estimate predsample */
				 diffL = 0;
				 if (codeL & 4) 
				 	diffL += stepL;
				 if (codeL & 2) 
				 	diffL += stepL >> 1;
				 if (codeL & 1) 
				 	diffL += stepL >> 2;
				 diffL += stepL >> 3;
				 if (codeL & 8)
				 	diffL = -diffL;
				predsampleL += diffL;

				 diffR = 0;
				 if (codeR & 4) 
				 	diffR += stepR;
				 if (codeR & 2) 
				 	diffR += stepR >> 1;
				 if (codeR & 1) 
				 	diffR += stepR >> 2;
				 diffR += stepR >> 3;
				 if (codeR & 8)
				 	diffR = -diffR;
				predsampleR += diffR;

				 if (predsampleL > 32767)				/* check for overflow */
				 	predsampleL = 32767;
				 else if (predsampleL < -32768)
				 	predsampleL = -32768;

				 if (predsampleR > 32767)				/* check for overflow */
				 	predsampleR = 32767;
				 else if (predsampleR < -32768)
				 	predsampleR = -32768;

				/* store estimate to output buffer */
				if (dstBitsPerSample == 16)
				{
					*pDest16++ = (short int)predsampleL;
					*pDest16++ = (short int)predsampleR;
				}
				else
				{
					*pDest8++ = (predsampleL >> 8L) - 128;
					*pDest8++ = (predsampleR >> 8L) - 128;
				}
				/* compute new stepsize step */
				indexL += imaIndexIncrementTable[codeL];
				if(indexL < 0) indexL = 0;
				else if(indexL > 88) indexL = 88;
				stepL = imaStepTable[indexL];

				indexR += imaIndexIncrementTable[codeR];
				if(indexR < 0) indexR = 0;
				else if(indexR > 88) indexR = 88;
				stepR = imaStepTable[indexR];
			}
		}
	}
}


///////////////////////////////////////////////// AIFF-IMA STREAMING DECOMPRESSION FUNCTIONS:
#if USE_HIGHLEVEL_FILE_API

XDWORD XExpandAiffImaStream(XBYTE const* src, XDWORD srcBytesPerBlock,
							void *dst, XDWORD dstBitsPerSample,
							XDWORD srcBytes, XDWORD channelCount,
							short predictorCache[2])
{
	XBYTE const*	pSourceL;
	XBYTE const*	pSourceR;
	INT16			*pDest16;
	XBYTE			*pDest8;
	int				stepL, indexL;
	INT32			predsampleL;
	char			codebufL, codeL;
	int				stepR, indexR;
	long			diffL;
	long			diffR;
	INT32			predsampleR;
	char			codebufR, codeR;
	unsigned long	count, srcIndex;

	predsampleL = predictorCache[0];
	predsampleR = predictorCache[1];

	pDest16 = (short int *)dst;
	pDest8 = (char *)dst;
	if ( (dstBitsPerSample == 8) || (dstBitsPerSample == 16) )
	{
		if (channelCount == 1)
		{
			pSourceL = src;
			indexL = XGetShort(pSourceL) & 0x7F;	// discard predictor bits
			if (indexL > 88) indexL == 88;

			for (srcIndex = 0; srcIndex < srcBytes;)
			{
				pSourceL += AIFF_IMA_HEADER_BYTES;
				srcIndex += AIFF_IMA_HEADER_BYTES;

				for (count = 0; count < AIFF_IMA_BLOCK_FRAMES; count++)
				{
					if	(count & 1)							/* two samples per inbuf char */
					{
						codeL = (codebufL >> 4) & 0x0F;
					}
					else	
					{
						codebufL = *pSourceL++;			/* buffer two ADPCM nibbles */
						codeL = codebufL & 0x0F;
					}
					stepL = imaStepTable[indexL];
#if 0
					predsampleL = imaadpcmSampleDecode(codeL, predsampleL, stepL);
					indexL  = imaadpcmNextStepIndex(codeL, indexL);
#else
					/* compute new sample estimate predsample */
					 diffL = stepL >> 3;
					 if (codeL & 1) diffL += stepL >> 2;
					 if (codeL & 2) diffL += stepL >> 1;
					 if (codeL & 4) diffL += stepL;
					 if (codeL & 8) diffL = -diffL;
					predsampleL += diffL;

					 if (predsampleL > 32767)				// check for overflow
					 	predsampleL = 32767;
					 else if (predsampleL < -32768)
					 	predsampleL = -32768;

					// compute new stepsize step
					indexL += imaIndexIncrementTable[codeL];
					if(indexL < 0) indexL = 0;
					else if(indexL > 88) indexL = 88;
#endif
					// store estimate to output buffer
					if (dstBitsPerSample == 16)
					{
						*pDest16++ = (short)predsampleL;		
					}
					else
					{
						*pDest8++ = (predsampleL >> 8L) - 128;
					}
				}
				srcIndex += AIFF_IMA_BLOCK_BYTES - AIFF_IMA_HEADER_BYTES;
			}
		}
		else
		{	// STEREO DECODER
		XDWORD		const framesPerBlock =
							(srcBytesPerBlock - AIFF_IMA_HEADER_BYTES) * 2;
		XDWORD		const blockCount = srcBytes / srcBytesPerBlock;
		XDWORD		frameCount = (blockCount / channelCount) * framesPerBlock;

			pSourceL = src;	// handle the starting case
			pSourceR = src + srcBytesPerBlock;

			for (srcIndex = 0; srcIndex < srcBytes;)
			{
				srcIndex += srcBytesPerBlock * 2;

				indexL = XGetShort(pSourceL) & 0x7F;	// discard predictor bits
				if (indexL > 88) indexL == 88;
				pSourceL += AIFF_IMA_HEADER_BYTES;
				indexR = XGetShort(pSourceR) & 0x7F;	// discard predictor bits
				if (indexR > 88) indexR == 88;
				pSourceR += AIFF_IMA_HEADER_BYTES;

				for (count = 0; count < framesPerBlock; count++)
				{
					if	(count & 1)							/* two samples per inbuf char */
					{
						codeL = (codebufL >> 4) & 0x0F;
						codeR = (codebufR >> 4) & 0x0F;
					}
					else	
					{
						codebufL = *pSourceL++;			/* buffer two ADPCM nibbles */
						codeL = codebufL & 0x0F;
						codebufR = *pSourceR++;			/* buffer two ADPCM nibbles */
						codeR = codebufR & 0x0F;
					}
					stepL = imaStepTable[indexL];
					stepR = imaStepTable[indexR];
#if 0
					predsampleL = imaadpcmSampleDecode(codeL, predsampleL, stepL);
					predsampleR = imaadpcmSampleDecode(codeR, predsampleR, stepR);

					indexL  = imaadpcmNextStepIndex(codeL, indexL);
					indexR  = imaadpcmNextStepIndex(codeR, indexR);
#else
					/* compute new sample estimate predsample */
					diffL = stepL >> 3;
					if (codeL & 1) diffL += stepL >> 2;
					if (codeL & 2) diffL += stepL >> 1;
					if (codeL & 4) diffL += stepL;
					if (codeL & 8) diffL = -diffL;
					predsampleL += diffL;
					if (predsampleL > 32767)				// check for overflow
					 	predsampleL = 32767;
					else if (predsampleL < -32768)
					 	predsampleL = -32768;

					diffR = stepR >> 3;
					if (codeR & 1) diffR += stepR >> 2;
					if (codeR & 2) diffR += stepR >> 1;
					if (codeR & 4) diffR += stepR;
					if (codeR & 8) diffR = -diffR;
					predsampleR += diffR;
					if (predsampleR > 32767)				// check for overflow
					 	predsampleR = 32767;
					else if (predsampleR < -32768)
					 	predsampleR = -32768;

					/* compute new stepsize step */
					indexL += imaIndexIncrementTable[codeL];
					if(indexL < 0) indexL = 0;
					else if(indexL > 88) indexL = 88;

					indexR += imaIndexIncrementTable[codeR];
					if(indexR < 0) indexR = 0;
					else if(indexR > 88) indexR = 88;
#endif

					/* store estimate to output buffer */
					if (dstBitsPerSample == 16)
					{
						*pDest16++ = (short)predsampleL;
						*pDest16++ = (short)predsampleR;
					}
					else
					{
						*pDest8++ = (predsampleL >> 8) + 128;
						*pDest8++ = (predsampleR >> 8) + 128;
					}
				}
			}
		}
	}

	predictorCache[0] = predsampleL;
	predictorCache[1] = predsampleR;
	if (dstBitsPerSample == 16)
	{
		count = (XBYTE *)pDest16 - (XBYTE *)dst;
	}
	else
	{
		count = pDest8 - (XBYTE *)dst;
	}
	return count;
}

#endif	// USE_HIGHLEVEL_FILE_API


///////////////////////////////////////////////// WAV-IMA DECOMPRESSION FUNCTIONS:
#if USE_HIGHLEVEL_FILE_API

//==========================================================================;
//
//	  DECODE ROUTINES
//
//==========================================================================;

//--------------------------------------------------------------------------;
//  
//  int imaadpcmSampleDecode
//  
//  Description:
//	  This routine decodes a single ADPCM sample.  For efficiency, it is
//	  declared INLINE.  Note that, depending on the optimization flags,
//	  it may not actually be implemented as INLINE.  Optimizing for speed
//	  (-Oxwt) will generally obey the INLINE specification.
//  
//  Arguments:
//	  int nEncodedSample:  The sample to be decoded.
//	  int nPredictedSample:  The predicted value of the sample (in PCM).
//	  int nStepSize:  The quantization step size used to encode the sample.
//  
//  Return (int):  The decoded PCM sample.
//  
//--------------------------------------------------------------------------;

static INLINE int imaadpcmSampleDecode
(
	int				 nEncodedSample,
	int				 nPredictedSample,
	int				 nStepSize
)
{
	long			lDifference;
	long			lNewSample;

	//
	//  calculate difference:
	//
	//	  lDifference = (nEncodedSample + 1/2) * nStepSize / 4
	//
	lDifference = nStepSize>>3;

	if (nEncodedSample & 1) 
		lDifference += nStepSize>>2;

	if (nEncodedSample & 2) 
		lDifference += nStepSize>>1;

	if (nEncodedSample & 4) 
		lDifference += nStepSize;

	//
	//  If the 'sign bit' of the encoded nibble is set, then the
	//  difference is negative...
	//
	if (nEncodedSample & 8)
		lDifference = -lDifference;

	//
	//  adjust predicted sample based on calculated difference
	//
	lNewSample = nPredictedSample + lDifference;

	//
	//  check for overflow and clamp if necessary to a 16 signed sample.
	//  Note that this is optimized for the most common case, when we
	//  don't have to clamp.
	//
	if( (long)(short)lNewSample == lNewSample )
	{
		return (int)lNewSample;
	}

	//
	//  Clamp.
	//
	if( lNewSample < -32768 )
		return (int)-32768;
	else
		return (int)32767;
}


//--------------------------------------------------------------------------;
//  
//  int imaadpcmNextStepIndex
//  
//  Description:
//	  This routine calculates the step index value to use for the next
//	  encode, based on the current value of the step index and the current
//	  encoded sample.  For efficiency, it is declared INLINE.  Note that,
//	  depending on the optimization flags, it may not actually be 
//	  implemented as INLINE.  Optimizing for speed (-Oxwt) will generally 
//	  obey the INLINE specification.
//  
//  Arguments:
//	  int nEncodedSample:  The current encoded ADPCM sample.
//	  int nStepIndex:  The step index value used to encode nEncodedSample.
//  
//  Return (int):  The step index to use for the next sample.
//  
//--------------------------------------------------------------------------;

static INLINE int imaadpcmNextStepIndex
(
	int					 nEncodedSample,
	int					 nStepIndex
)
{
	//
	//  compute new stepsize step
	//
	nStepIndex += imaIndexIncrementTable[nEncodedSample];

	if (nStepIndex < 0)
		nStepIndex = 0;
	else if (nStepIndex > 88)
		nStepIndex = 88;

	return (nStepIndex);
}



//--------------------------------------------------------------------------;
//  
//  BOOL imaadpcmValidStepIndex
//  
//  Description:
//	  This routine checks the step index value to make sure that it is
//	  within the legal range.
//  
//  Arguments:
//	  
//	  int nStepIndex:  The step index value.
//  
//  Return (BOOL):  TRUE if the step index is valid; FALSE otherwise.
//  
//--------------------------------------------------------------------------;

static INLINE XBOOL imaadpcmValidStepIndex
(
	int					 nStepIndex
)
{

	if( nStepIndex >= 0 && nStepIndex <= 88 )
		return TRUE;
	else
		return FALSE;
}


//--------------------------------------------------------------------------;
//  
//  XDWORD imaadpcmDecode4Bit_M08
//  XDWORD imaadpcmDecode4Bit_M16
//  XDWORD imaadpcmDecode4Bit_S08
//  XDWORD imaadpcmDecode4Bit_S16
//  
//  Description:
//	  These functions decode a buffer of data from ADPCM to PCM in the
//	  specified format.  The appropriate function is called once for each
//	  ACMDM_STREAM_CONVERT message received.  Note that since these
//	  functions must share the same prototype as the encoding functions
//	  (see acmdStreamOpen() and acmdStreamConvert() in codec.c for more
//	  details), not all the parameters are used by these routines.
//  
//  Arguments:
//	  XBYTE* pbSrc:  Pointer to the source buffer (ADPCM data).
//	  XDWORD cbSrcLength:  The length of the source buffer (in bytes).
//	  XBYTE* pbDst:  Pointer to the destination buffer (PCM data).  Note
//					  that it is assumed that the destination buffer is
//					  large enough to hold all the encoded data; see
//					  acmdStreamSize() in codec.c for more details.
//	  unsigned int nBlockAlignment:  The block alignment of the ADPCM data (in
//					  bytes).
//  
//  Return (XDWORD):  The number of bytes used in the destination buffer.
//  
//--------------------------------------------------------------------------;

static INLINE XDWORD imaadpcmDecode4Bit_M08
(
	XBYTE const*			pbSrc,
	XDWORD					cbSrcLength,
	XBYTE*			pbDst,
	unsigned int			nBlockAlignment
)
{
	XBYTE*			pbDstStart;
	unsigned int			cbHeader;
	unsigned int			cbBlockLength;
	XBYTE					bSample;
	int						nStepSize;

	int						nEncSample;
	int						nPredSample;
	int						nStepIndex;

	
	pbDstStart = pbDst;
	cbHeader = WAV_IMA_HEADER_BYTES * 1;  //  1 = number of channels.


//	DPF(3,"Starting imaadpcmDecode4Bit_M08().");


	//
	//
	//
	while (cbSrcLength >= cbHeader)
	{
		XDWORD	   dwHeader;

		cbBlockLength  = (unsigned int)XMIN(cbSrcLength, nBlockAlignment);
		cbSrcLength   -= cbBlockLength;
		cbBlockLength -= cbHeader;

		//
		//  block header
		//
		dwHeader = *(XDWORD *)pbSrc;
		#if X_WORD_ORDER == FALSE	// motorola?
			dwHeader = XSwapLong(dwHeader);
		#endif
		pbSrc   += sizeof(XDWORD);
		nPredSample = (int)(short)XLOWORD(dwHeader);
		nStepIndex  = (int)(XBYTE)XHIWORD(dwHeader);

		if( !imaadpcmValidStepIndex(nStepIndex) ) {
			//
			//  The step index is out of range - this is considered a fatal
			//  error as the input stream is corrupted.  We fail by returning
			//  zero bytes converted.
			//
//			DPF(1,"imaadpcmDecode4Bit_M08: invalid step index.");
			return 0;
		}
		

		//
		//  write out first sample
		//
		*pbDst++ = (XBYTE)((nPredSample >> 8) + 128);


		//
		//
		//
		while (cbBlockLength--)
		{
			bSample = *pbSrc++;

			//
			//  sample 1
			//
			nEncSample  = (bSample & (XBYTE)0x0F);
			nStepSize   = imaStepTable[nStepIndex];
			nPredSample = imaadpcmSampleDecode(nEncSample, nPredSample, nStepSize);
			nStepIndex  = imaadpcmNextStepIndex(nEncSample, nStepIndex);

			//
			//  write out sample
			//
			*pbDst++ = (XBYTE)((nPredSample >> 8) + 128);

			//
			//  sample 2
			//
			nEncSample  = (bSample >> 4);
			nStepSize   = imaStepTable[nStepIndex];
			nPredSample = imaadpcmSampleDecode(nEncSample, nPredSample, nStepSize);
			nStepIndex  = imaadpcmNextStepIndex(nEncSample, nStepIndex);

			//
			//  write out sample
			//
			*pbDst++ = (XBYTE)((nPredSample >> 8) + 128);
		}
	}

	//
	//  We return the number of bytes used in the destination.  This is
	//  simply the difference in bytes from where we started.
	//
	return (XDWORD)(pbDst - pbDstStart);

} // imaadpcmDecode4Bit_M08()



//--------------------------------------------------------------------------;
//--------------------------------------------------------------------------;

static INLINE XDWORD imaadpcmDecode4Bit_M16
(
	XBYTE const*			pbSrc,
	XDWORD					cbSrcLength,
	XBYTE*			pbDst,
	unsigned int			nBlockAlignment
)
{
	XBYTE*				  pbDstStart;
	unsigned int					cbHeader;
	unsigned int					cbBlockLength;
	XBYTE					bSample;
	int					 nStepSize;

	int					 nEncSample;
	int					 nPredSample;
	int					 nStepIndex;

	
	pbDstStart = pbDst;
	cbHeader = WAV_IMA_HEADER_BYTES * 1;  //  1 = number of channels.


//	DPF(3,"Starting imaadpcmDecode4Bit_M16().");


	//
	//
	//
	while (cbSrcLength >= cbHeader)
	{
		XDWORD	   dwHeader;

		cbBlockLength  = (unsigned int)XMIN(cbSrcLength, nBlockAlignment);
		cbSrcLength   -= cbBlockLength;
		cbBlockLength -= cbHeader;

		//
		//  block header
		//
		dwHeader = *(XDWORD *)pbSrc;
		#if X_WORD_ORDER == FALSE	// motorola?
			dwHeader = XSwapLong(dwHeader);
		#endif
		pbSrc   += sizeof(XDWORD);
		nPredSample = (int)(short)XLOWORD(dwHeader);
		nStepIndex  = (int)(XBYTE)XHIWORD(dwHeader);

		if( !imaadpcmValidStepIndex(nStepIndex) ) {
			//
			//  The step index is out of range - this is considered a fatal
			//  error as the input stream is corrupted.  We fail by returning
			//  zero bytes converted.
			//
//			DPF(1,"imaadpcmDecode4Bit_M16: invalid step index.");
			return 0;
		}
		

		//
		//  write out first sample
		//
		*(short *)pbDst = (short)nPredSample;
		pbDst += sizeof(short);


		//
		//
		//
		while (cbBlockLength--)
		{
			bSample = *pbSrc++;

			//
			//  sample 1
			//
			nEncSample  = (bSample & (XBYTE)0x0F);
			nStepSize   = imaStepTable[nStepIndex];
			nPredSample = imaadpcmSampleDecode(nEncSample, nPredSample, nStepSize);
			nStepIndex  = imaadpcmNextStepIndex(nEncSample, nStepIndex);

			//
			//  write out sample
			//
			*(short *)pbDst = (short)nPredSample;
			pbDst += sizeof(short);

			//
			//  sample 2
			//
			nEncSample  = (bSample >> 4);
			nStepSize   = imaStepTable[nStepIndex];
			nPredSample = imaadpcmSampleDecode(nEncSample, nPredSample, nStepSize);
			nStepIndex  = imaadpcmNextStepIndex(nEncSample, nStepIndex);

			//
			//  write out sample
			//
			*(short *)pbDst = (short)nPredSample;
			pbDst += sizeof(short);
		}
	}

	//
	//  We return the number of bytes used in the destination.  This is
	//  simply the difference in bytes from where we started.
	//
	return (XDWORD)(pbDst - pbDstStart);

} // imaadpcmDecode4Bit_M16()



//--------------------------------------------------------------------------;
//--------------------------------------------------------------------------;

static INLINE XDWORD imaadpcmDecode4Bit_S08
(
	XBYTE const*			pbSrc,
	XDWORD					cbSrcLength,
	XBYTE*			pbDst,
	unsigned int			nBlockAlignment
)
{
	XBYTE*				  pbDstStart;
	unsigned int					cbHeader;
	unsigned int					cbBlockLength;
	int					 nStepSize;
	XDWORD				   dwHeader;
	XDWORD				   dwLeft;
	XDWORD				   dwRight;
	int					 i;

	int					 nEncSampleL;
	int					 nPredSampleL;
	int					 nStepIndexL;

	int					 nEncSampleR;
	int					 nPredSampleR;
	int					 nStepIndexR;

	
	pbDstStart = pbDst;
	cbHeader = WAV_IMA_HEADER_BYTES * 2;  //  2 = number of channels.


//	DPF(3,"Starting imaadpcmDecode4Bit_S08().");


	//
	//
	//
	while( 0 != cbSrcLength )
	{
		//
		//  The data should always be block aligned.
		//
		cbBlockLength  = nBlockAlignment;
		cbSrcLength   -= cbBlockLength;
		cbBlockLength -= cbHeader;


		//
		//  LEFT channel header
		//
		dwHeader = *(XDWORD *)pbSrc;
		#if X_WORD_ORDER == FALSE	// motorola?
			dwHeader = XSwapLong(dwHeader);
		#endif
		pbSrc   += sizeof(XDWORD);
		nPredSampleL = (int)(short)XLOWORD(dwHeader);
		nStepIndexL  = (int)(XBYTE)XHIWORD(dwHeader);

		if( !imaadpcmValidStepIndex(nStepIndexL) ) {
			//
			//  The step index is out of range - this is considered a fatal
			//  error as the input stream is corrupted.  We fail by returning
			//  zero bytes converted.
			//
//			DPF(1,"imaadpcmDecode4Bit_S08: invalid step index (L).");
			return 0;
		}
		
		//
		//  RIGHT channel header
		//
		dwHeader = *(XDWORD *)pbSrc;
		#if X_WORD_ORDER == FALSE	// motorola?
			dwHeader = XSwapLong(dwHeader);
		#endif
		pbSrc   += sizeof(XDWORD);
		nPredSampleR = (int)(short)XLOWORD(dwHeader);
		nStepIndexR  = (int)(XBYTE)XHIWORD(dwHeader);

		if( !imaadpcmValidStepIndex(nStepIndexR) ) {
			//
			//  The step index is out of range - this is considered a fatal
			//  error as the input stream is corrupted.  We fail by returning
			//  zero bytes converted.
			//
//			DPF(1,"imaadpcmDecode4Bit_S08: invalid step index (R).");
			return 0;
		}
		

		//
		//  write out first sample
		//
		*pbDst++ = (XBYTE)((nPredSampleL >> 8) + 128);
		*pbDst++ = (XBYTE)((nPredSampleR >> 8) + 128);


		//
		//  The first XDWORD contains 4 left samples, the second XDWORD
		//  contains 4 right samples.  We process the source in 8-byte
		//  chunks to make it easy to interleave the output correctly.
		//
		while( 0 != cbBlockLength )
		{
			cbBlockLength -= 8;

			dwLeft   = *(XDWORD *)pbSrc;
			pbSrc   += sizeof(XDWORD);
			dwRight  = *(XDWORD *)pbSrc;
			pbSrc   += sizeof(XDWORD);
			#if X_WORD_ORDER == FALSE	// motorola?
				dwLeft = XSwapLong(dwLeft);
				dwRight = XSwapLong(dwRight);
			#endif

			for( i=8; i>0; i-- )
			{
				//
				//  LEFT channel
				//
				nEncSampleL  = (int)(dwLeft & 0x0F);
				nStepSize	= imaStepTable[nStepIndexL];
				nPredSampleL = imaadpcmSampleDecode(nEncSampleL, nPredSampleL, nStepSize);
				nStepIndexL  = imaadpcmNextStepIndex(nEncSampleL, nStepIndexL);

				//
				//  RIGHT channel
				//
				nEncSampleR  = (int)(dwRight & 0x0F);
				nStepSize	= imaStepTable[nStepIndexR];
				nPredSampleR = imaadpcmSampleDecode(nEncSampleR, nPredSampleR, nStepSize);
				nStepIndexR  = imaadpcmNextStepIndex(nEncSampleR, nStepIndexR);

				//
				//  write out sample
				//
				*pbDst++ = (XBYTE)((nPredSampleL >> 8) + 128);
				*pbDst++ = (XBYTE)((nPredSampleR >> 8) + 128);

				//
				//  Shift the next input sample into the low-order 4 bits.
				//
				dwLeft  >>= 4;
				dwRight >>= 4;
			}
		}
	}

	//
	//  We return the number of bytes used in the destination.  This is
	//  simply the difference in bytes from where we started.
	//
	return (XDWORD)(pbDst - pbDstStart);

} // imaadpcmDecode4Bit_S08()



//--------------------------------------------------------------------------;
//--------------------------------------------------------------------------;

static INLINE XDWORD imaadpcmDecode4Bit_S16
(
	XBYTE const*			pbSrc,
	XDWORD					cbSrcLength,
	XBYTE*			pbDst,
	unsigned int			nBlockAlignment
)
{
	XBYTE*		pbDstStart;
	unsigned int		cbHeader;
	unsigned int		cbBlockLength;
	int					nStepSize;
	XDWORD				dwHeader;
	XDWORD				dwLeft;
	XDWORD				dwRight;
	int					i;

	int					nEncSampleL;
	int					nPredSampleL;
	int					nStepIndexL;

	int					nEncSampleR;
	int					nPredSampleR;
	int					nStepIndexR;

	
	pbDstStart = pbDst;
	cbHeader = WAV_IMA_HEADER_BYTES * 2;  //  2 = number of channels.


//	DPF(3,"Starting imaadpcmDecode4Bit_S16().");


	//
	//
	//
	while( 0 != cbSrcLength )
	{
		//
		//  The data should always be block aligned.
		//
		cbBlockLength  = nBlockAlignment;
		cbSrcLength   -= cbBlockLength;
		cbBlockLength -= cbHeader;


		//
		//  LEFT channel header
		//
		dwHeader = *(XDWORD *)pbSrc;
		#if X_WORD_ORDER == FALSE	// motorola?
			dwHeader = XSwapLong(dwHeader);
		#endif
		pbSrc   += sizeof(XDWORD);
		nPredSampleL = (int)(short)XLOWORD(dwHeader);
		nStepIndexL  = (int)(XBYTE)XHIWORD(dwHeader);

		if( !imaadpcmValidStepIndex(nStepIndexL) ) {
			//
			//  The step index is out of range - this is considered a fatal
			//  error as the input stream is corrupted.  We fail by returning
			//  zero bytes converted.
			//
//			DPF(1,"imaadpcmDecode4Bit_S16: invalid step index %u (L).", nStepIndexL);
			return 0;
		}
		
		//
		//  RIGHT channel header
		//
		dwHeader = *(XDWORD *)pbSrc;
		#if X_WORD_ORDER == FALSE	// motorola?
			dwHeader = XSwapLong(dwHeader);
		#endif
		pbSrc   += sizeof(XDWORD);
		nPredSampleR = (int)(short)XLOWORD(dwHeader);
		nStepIndexR  = (int)(XBYTE)XHIWORD(dwHeader);

		if( !imaadpcmValidStepIndex(nStepIndexR) ) {
			//
			//  The step index is out of range - this is considered a fatal
			//  error as the input stream is corrupted.  We fail by returning
			//  zero bytes converted.
			//
//			DPF(1,"imaadpcmDecode4Bit_S16: invalid step index %u (R).",nStepIndexR);
			return 0;
		}
		

		//
		//  write out first sample
		//
		*(XDWORD *)pbDst = XMAKELONG(nPredSampleL, nPredSampleR);
		pbDst += sizeof(XDWORD);


		//
		//  The first XDWORD contains 4 left samples, the second XDWORD
		//  contains 4 right samples.  We process the source in 8-byte
		//  chunks to make it easy to interleave the output correctly.
		//
		while( 0 != cbBlockLength )
		{
			cbBlockLength -= 8;

			dwLeft   = *(XDWORD *)pbSrc;
			pbSrc   += sizeof(XDWORD);
			dwRight  = *(XDWORD *)pbSrc;
			pbSrc   += sizeof(XDWORD);
			#if X_WORD_ORDER == FALSE	// motorola?
				dwLeft = XSwapLong(dwLeft);
				dwRight = XSwapLong(dwRight);
			#endif

			for( i=8; i>0; i-- )
			{
				//
				//  LEFT channel
				//
				nEncSampleL  = (int)(dwLeft & 0x0F);
				nStepSize	= imaStepTable[nStepIndexL];
				nPredSampleL = imaadpcmSampleDecode(nEncSampleL, nPredSampleL, nStepSize);
				nStepIndexL  = imaadpcmNextStepIndex(nEncSampleL, nStepIndexL);

				//
				//  RIGHT channel
				//
				nEncSampleR  = (int)(dwRight & 0x0F);
				nStepSize	= imaStepTable[nStepIndexR];
				nPredSampleR = imaadpcmSampleDecode(nEncSampleR, nPredSampleR, nStepSize);
				nStepIndexR  = imaadpcmNextStepIndex(nEncSampleR, nStepIndexR);

				//
				//  write out sample
				//
				*(XDWORD *)pbDst = XMAKELONG(nPredSampleL, nPredSampleR);
				pbDst += sizeof(XDWORD);

				//
				//  Shift the next input sample into the low-order 4 bits.
				//
				dwLeft  >>= 4;
				dwRight >>= 4;
			}
		}
	}

	//
	//  We return the number of bytes used in the destination.  This is
	//  simply the difference in bytes from where we started.
	//
	return (XDWORD)(pbDst - pbDstStart);

} // imaadpcmDecode4Bit_S16()

XDWORD XExpandWavIma(XBYTE const* src, XDWORD srcBytesPerBlock,
						void* pbDst, XDWORD dstBitsPerSample,
						XDWORD srcBytes, XDWORD channelCount)
{
	if ( (channelCount == 1) && (dstBitsPerSample == 8) )
	{
		return imaadpcmDecode4Bit_M08(src, srcBytes, (XBYTE*)pbDst, srcBytesPerBlock);
	}
	if ( (channelCount == 1) && (dstBitsPerSample == 16) )
	{
		return imaadpcmDecode4Bit_M16(src, srcBytes, (XBYTE*)pbDst, srcBytesPerBlock);
	}
	if ( (channelCount == 2) && (dstBitsPerSample == 8) )
	{
		return imaadpcmDecode4Bit_S08(src, srcBytes, (XBYTE*)pbDst, srcBytesPerBlock);
	}
	if ( (channelCount == 2) && (dstBitsPerSample == 16) )
	{
		return imaadpcmDecode4Bit_S16(src, srcBytes, (XBYTE*)pbDst, srcBytesPerBlock);
	}
	return 0L;
}
#endif	// USE_HIGHLEVEL_FILE_API


//==========================================================================;


#endif	// USE_NEW_EXPAND_CODE


// EOF of X_IMA.c
