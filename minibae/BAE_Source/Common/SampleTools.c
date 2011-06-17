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
**	SampleTools.c
**
**	Tools for manipulating 'snd' resources.
**
**	© Copyright 1989-2000 Beatnik, Inc, All Rights Reserved.
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
**	1/31/93		Created
**	6/23/95		Integrated into SoundMusicSys Library
**	11/22/95	Added MACE support for CreateSampleFromSnd
**	12/15/95	Changed memory allocation to X_API
**				Pulled out most Mac specific code
**	1/18/96		Spruced up for C++ extra error checking
**	2/3/96		Removed extra includes
**	2/11/96		No longer relies on MacOS Sound.h
**	2/12/96		Changed Boolean to SMSBoolean
**	2/17/96		Added more platform compile arounds
**	3/28/96		Added XPutShort & XPutLong
**	3/29/96		Tried to figure out ima 4 to 1 and failed
**	4/2/96		Added more snd tools to set loop points, rates, and basekey
**	4/9/96		Changed AIFFSampleRate from extended80 to char [10]
**	4/21/96		Removed CPU edian issues by use XGetShort & XGetLong
**				Removed register usage in parameters
**	4/25/96		Added PV_Swap16BitSamplesIfIntel to deal with 16 bit sample on non 68k ordered platforms
**	5/9/96		Fixed byte swap problem
**	6/30/96		Changed font and re tabbed
**	7/6/96		Fixed possible problem with XGetSamplePtrFromSnd by no zeroing pSampleData
**	7/14/96		Fixed structure alignment problem for Pentium compilers
**	9/22/96		Added the ability to select an IMA 16 bit decompression, or an 8 bit version
**	11/14/96	Removed dependancy on MacSpecificSMS.h
**	12/15/96	Removed last remants of MacOS code and moved it all in MacSpecificSMS.c
**				Moved all snd resource types and structs into X_Formats.h
**	12/30/96	Changed copyright
**	1/30/97		Changed all '' constants to defines
**	5/3/97		Fixed a few potential problems that the MOT compiler found
**	6/20/97		Added XGetSampleNameFromID
**	7/17/97		Wrapped X_WORD_ORDER macros around PV_Swap16BitSamplesIfIntel. Saved code size,
**				and fixed potential bug because its word order based, not platform based
**	7/24/97		Added XCreateSoundObjectFromData & XGetSoundEmbeddedStatus & 
**				XSetSoundEmbeddedStatus
**	8/6/97		Changed XGetSampleNameFromID to be smarter about how many types
**				to search through.
**				Added XCreateAudioObjectFromData
**	8/7/97		Added XTranslateFromWaveformToSampleData
**	9/3/97		Wrapped a few more functions around USE_CREATION_API == TRUE
**	12/16/97	Moe: removed compiler warnings
**	1/31/98		Modified XGetSamplePtrFromSnd to handle the case of samples that
**				are ordered a motorola way on an intel platform. Modify them in place
**				and set a flag that allows for the next reference to be correct
**	2/11/98		Modified XGetSamplePtrFromSnd to elimate code to support MacOS MACE compression
**				Put code wrappers around XPhase8BitWaveform to eliminate it from the compile
**				if its not being used
**				Reduced code size of XGetSamplePtrFromSnd when decoding IMA samples, and fixed
**				a memory bug in which too much data was being allocated.
**	3/5/98		Fixed XCreateSoundObjectFromData to make sure the data created was motorola ordered
**	3/23/98		MOE: Changed name of call to XExpandAiffIma(),
**					also added block size parameter
**				Changed XGetSamplePtrFromSnd to use IMA constant. Changed XCreateSoundObjectFromData
**				to support compression types
**	3/23/98		MOE: fixed XCreateSoundObjectFromData() and consolidated its code a bit
**	3/24/98		Fixed some warnings with XCreateSoundObjectFromData
**	5/12/98		MOE: Changed XCreateSoundObjectFromData() to encode IMA4 with proper "sampleSize" and
**				"forceSample8bit" values  (Since Beatnik ignores the former and MacOS ignores the latter,
**				this wasn't causing an audible bug.)
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
**	7/6/98		Fixed a compiler warning with XCreateSoundObjectFromData
**	7/7/98		Modified XGetSampleNameFromID to use new XLongResourceID type
**	9/29/98		Renamed firstSoundFormat & secondSoundFormat to XFirstSoundFormat & XSecondSoundFormat
**				Added support for new 'snd ' XThirdSoundFormat
**				Renamed stdSH, extSH, cmpSH	to XStandardHeader, XExtendedHeader, XCompressedHeader
**				Changed parameter type of XSetSoundSampleRate
**	9/30/98		Added XTranslateFromSampleDataToWaveform
**	10/2/98		Added support in XCreateSoundObjectFromData to create ALAW/ULAW/ and MPEG compressed
**				Snd resources
**	10/9/98		Changed XCreateSoundObjectFromData to convert 8 bit to 16 bit data before compressing
**				in ulaw/alaw/mpeg formats
**	10/15/98	Added PV_GetClosestMPEGSampleRate which looks for the closest MPEG rate to use for encoding
**	2/12/99		Renamed USE_HAE_FOR_MPEG to USE_MPEG_DECODER
**	2/23/99		Fixed a bug in XGetSamplePtrFromSnd in which a bad decode type would return an invalid sample
**				block
**	3/8/99		MOE: Added MPEGEncodeRateElement{},mpegEncodeRateTable[],
**							PV_GetMPEGEncodeRate(), PV_GetClosestMPEGEncodeRate()
**				Changed encoder code to use table and scale output bitrate by the ratio:
**				input-sample-rate-declared-to-MPEG-encoder/actual-sample-rate-of-input-data
**				Changed PV_GetClosestMPEGSampleRate() to find closest, not nearest below
**	3/25/99		MOE:  Added procData parameter to functions using XCompressStatusProc
**	4/2/99		Fixed bug in XGetSamplePtrFromSnd near XCompressedHeader, in which a off word memory
**				access caused a violation on Solaris
**	4/26/99		Optimized PV_Swap16BitSamplesIfIntel to inline swap the bytes
**	4/29/99		Fixed bug in PV_Swap16BitSamplesIfIntel. Change...debug...refine...repeat...infinate series...
**	7/20/99		"control, the eagle has landed, on tranquility base" 7/20/69
**				Changed XGetSoundEmbeddedStatus to always report embedded for old style
**				8-bit samples.
**	9/9/99		MOE:  Changed PV_Swap16BitSamplesIfIntel()'s parameters
**	9/9/99		MOE:  Created the non-static XSwapShorts() for use in other files
**	9/9/99		MOE:  Changed XTranslateFromWaveformToSampleData() to use the new
**				compressionType field in GM_Waveform{}
**	9/9/99		MOE:  Moved PV_GetMPEGEncodeRate(), PV_GetClosestMPEGEncodeRate(), and
**				PV_GetClosestMPEGSampleRate() to XMPEGFiles.c since they are
**				MPEG-encoder-dependent
**	10/26/99	MSD:  Added X_PLATFORM != X_BE around ROM case in XGetSamplePtrFromSnd()
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	2/5/2000	Fixed compiler warning in XCreateSoundObjectFromData
**	2/17/2000	Moved XTranslateFromWaveformToSampleData && XTranslateFromSampleDataToWaveform
**				into an area that is compiled reguardless of options.
**	3/19/2000	Fixed bug in XConvert8BitTo16Bit. Would not convert stereo samples correctly. Only
**				half the samples got processed.
**	3/22/2000	Commented XGetSamplePtrFromSnd. Added XGetSampleInfoFromSnd
**	2000.04.14 AER	Promoted XConvert8BitTo16Bit to a non-private function
**	8/28/2000	sh	Modified XCreateSoundObjectFromData to ignore the ID3 tag of an incoming
**					MPEG file.
**	9/7/2000	sh	Modified XCreateSoundObjectFromData to tweak the block count values
**					for pre encoded MPEG data when creating an 'snd' resource.
**	9/20/2000	sh	Modified XCreateSoundObjectFromData to tweak the block count values
**					for BAE encoded MPEG data when creating an 'snd' resource.
**  9/30/2000   tl  in  XGetSamplePtrFromSnd, changed XNewPtr(encodedBytes) to XNewPtr(info->size)
*/
/*****************************************************************************/

#include "X_API.h"
#include "X_Formats.h"
#include "X_Assert.h"


// NOTE: This samples parameter is not guarenteed to be word aligned.
void XSwapShorts(short* shortArray, long count)
{
UBYTE*			byteArray;
UBYTE			data;

	byteArray = (UBYTE*)shortArray;
	while (--count >= 0)
	{
		data = byteArray[0];
		byteArray[0] = byteArray[1];// get this read for free, possibly cached already
		byteArray[1] = data;
		byteArray += 2;
	}
}
#if X_WORD_ORDER != FALSE
// This is used to convert samples that are ordered in a 68k way to the native platform.
#define PV_Swap16BitSamplesIfIntel(samples, count)		XSwapShorts(samples, count)
#else
#define PV_Swap16BitSamplesIfIntel(samples, count)		((void)0)
#endif

// This is used on 8 bit samples that are sign encoded. We want 0 to 255 not -128 to 127
void XPhase8BitWaveform(unsigned char * pByte, long size)
{
	while (size--)
	{
		*pByte++ -= 128;
	}
}

// Given a Mac snd pointer, this will return the encoding type, and a pointer to a SoundHeader structure
static void * PV_GetSoundHeaderPtr(XPTR pRes, short int *pEncode)
{
	XSoundHeader	*pSndBuffer;
	short int		soundFormat;
	short int		numSynths, numCmds;
	long			offset;
	char			*pSndFormat;

	numSynths = 0;
	numCmds = 0;
	pSndBuffer = NULL;
	if (pRes && pEncode)
	{
		*pEncode = -1;

		pSndFormat = (char *)pRes;
		soundFormat = XGetShort(pSndFormat);
		switch (soundFormat)
		{
			case XFirstSoundFormat:
				// look inside the format 1 resource and decode offsets
				numSynths = XGetShort(pSndFormat + 2);
				numCmds = XGetShort(pSndFormat + 4 + numSynths * 6);		// get number of commands

				// compute address of sound header.
				offset = 6 + 6 * numSynths + 8 * numCmds;
				pSndBuffer = (XSoundHeader *) ((char *)pRes + offset);
				*pEncode = pSndBuffer->encode & 0xFF;
				break;
			case XSecondSoundFormat:
				numSynths = 0;		// format 2 has none
				numCmds = XGetShort(pSndFormat + 4);

				// compute address of sound header.
				offset = 6 + 6 * numSynths + 8 * numCmds;
				pSndBuffer = (XSoundHeader *) ((char *)pRes + offset);
				*pEncode = pSndBuffer->encode & 0xFF;
				break;
			case XThirdSoundFormat:
				numSynths = 0;		// format 3 has none
				numCmds = 0;
				pSndBuffer = (XSoundHeader *) ((char *)pRes + sizeof(short));
				*pEncode = XType3Header;
				break;
			default:
				soundFormat = -1;
				break;
		}
	}
	return pSndBuffer;
}


// Given a Mac sample, and loop points, this will change the data associated with it
#if USE_CREATION_API == TRUE
void XSetSoundLoopPoints(XPTR pRes, long loopStart, long loopEnd)
{
	register XSoundHeader		*pSndBuffer;
	register XCmpSoundHeader	*pCmpBuffer;
	register XExtSoundHeader	*pExtBuffer;
	register XSoundHeader3		*pType3Buffer;
	short int					encode, count;

	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	if (pSndBuffer)	/* did we get the right format? */
	{
		switch (encode)
		{
			case XStandardHeader:	// standard header
				XPutLong(&pSndBuffer->loopStart, loopStart);
				XPutLong(&pSndBuffer->loopEnd, loopEnd);
				break;

			case XExtendedHeader:	// extended header
				pExtBuffer = (XExtSoundHeader *)pSndBuffer;
				XPutLong(&pExtBuffer->loopStart, loopStart);
				XPutLong(&pExtBuffer->loopEnd, loopEnd);
				break;

			case XCompressedHeader:	// compressed header
				pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
				XPutLong(&pCmpBuffer->loopStart, loopStart);
				XPutLong(&pCmpBuffer->loopEnd, loopEnd);
				break;

			case XType3Header:		// type 3 header
				pType3Buffer = (XSoundHeader3 *)pSndBuffer;
				for (count = 0; count < pType3Buffer->channels; count++)
				{
					XPutLong(&pType3Buffer->loopStart[count], loopStart);
					XPutLong(&pType3Buffer->loopEnd[count], loopEnd);
				}
				break;
		}
	}
}

void XSetSoundEmbeddedStatus(XPTR pRes, XBOOL soundEmbedded)
{
	register XSoundHeader		*pSndBuffer;
	register XCmpSoundHeader	*pCmpBuffer;
	register XExtSoundHeader	*pExtBuffer;
	register XSoundHeader3		*pType3Buffer;
	short int					encode;

	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	if (pSndBuffer)	/* did we get the right format? */
	{
		switch (encode)
		{
//			case XStandardHeader:	// standard header
//				break;

			case XExtendedHeader:	// extended header
				pExtBuffer = (XExtSoundHeader *)pSndBuffer;
				pExtBuffer->soundIsEmbedded = soundEmbedded;
				break;

			case XCompressedHeader:	// compressed header
				pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
				pCmpBuffer->soundIsEmbedded = soundEmbedded;
				break;

			case XType3Header:		// type 3 header
				pType3Buffer = (XSoundHeader3 *)pSndBuffer;
				pType3Buffer->isEmbedded = soundEmbedded;
				break;
		}
	}
}

XBOOL XGetSoundEmbeddedStatus(XPTR pRes)
{
	register XSoundHeader		*pSndBuffer;
	register XCmpSoundHeader	*pCmpBuffer;
	register XExtSoundHeader	*pExtBuffer;
	register XSoundHeader3		*pType3Buffer;
	short int					encode;
	XBOOL						soundEmbedded;

	soundEmbedded = FALSE;
	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	if (pSndBuffer)	/* did we get the right format? */
	{
		switch (encode)
		{
			default:
				break;
			case XStandardHeader:	// standard header
				soundEmbedded = TRUE;	// since this format is an 8-bit format and we don't
										// create it, we're going to assume its an embedded format
				break;

			case XExtendedHeader:	// extended header
				pExtBuffer = (XExtSoundHeader *)pSndBuffer;
				soundEmbedded = pExtBuffer->soundIsEmbedded;
				break;

			case XCompressedHeader:	// compressed header
				pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
				soundEmbedded = pCmpBuffer->soundIsEmbedded;
				break;

			case XType3Header:		// type 3 header
				pType3Buffer = (XSoundHeader3 *)pSndBuffer;
				soundEmbedded = pType3Buffer->isEmbedded;
				break;
		}
	}
	return soundEmbedded;
}

// Given a Mac sample, and a sample rate, this will change the data associated with it
void XSetSoundSampleRate(XPTR pRes, XFIXED sampleRate)
{
	register XSoundHeader		*pSndBuffer;
	register XCmpSoundHeader	*pCmpBuffer;
	register XExtSoundHeader	*pExtBuffer;
	register XSoundHeader3		*pType3Buffer;
	short int					encode;

	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	if (pSndBuffer)	/* did we get the right format? */
	{
		switch (encode)
		{
			case XStandardHeader:	// standard header
				XPutLong(&pSndBuffer->sampleRate, sampleRate);
				break;

			case XExtendedHeader:	// extended header
				pExtBuffer = (XExtSoundHeader *)pSndBuffer;
				XPutLong(&pExtBuffer->sampleRate, sampleRate);
				break;

			case XCompressedHeader:	// compressed header
				pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
				XPutLong(&pCmpBuffer->sampleRate, sampleRate);
				break;

			case XType3Header:		// type 3 header
				pType3Buffer = (XSoundHeader3 *)pSndBuffer;
				XPutLong(&pType3Buffer->sampleRate, sampleRate);
				break;
		}
	}
}

// Given a Mac sample, and a sample rate, this will change the data associated with it
void XSetSoundBaseKey(XPTR pRes, short int baseKey)
{
	register XSoundHeader		*pSndBuffer;
	register XCmpSoundHeader	*pCmpBuffer;
	register XExtSoundHeader	*pExtBuffer;
	register XSoundHeader3		*pType3Buffer;
	short int					encode;

	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	if (pSndBuffer)	/* did we get the right format? */
	{
		switch (encode)
		{
			case XStandardHeader:	// standard header
				pSndBuffer->baseFrequency = (unsigned char)baseKey;
				break;

			case XExtendedHeader:	// extended header
				pExtBuffer = (XExtSoundHeader *)pSndBuffer;
				pExtBuffer->baseFrequency = (unsigned char)baseKey;
				break;

			case XCompressedHeader:	// compressed header
				pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
				pCmpBuffer->baseFrequency = (unsigned char)baseKey;
				break;

			case XType3Header:		// type 3 header
				pType3Buffer = (XSoundHeader3 *)pSndBuffer;
				pType3Buffer->baseKey = (unsigned char)baseKey;
				break;
		}
	}
}

short int XGetSoundBaseKey(XPTR pRes)
{
	register XSoundHeader		*pSndBuffer;
	register XCmpSoundHeader	*pCmpBuffer;
	register XExtSoundHeader	*pExtBuffer;
	register XSoundHeader3		*pType3Buffer;
	short int					encode;
	short int					baseKey;

	baseKey = kMiddleC;
	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	if (pSndBuffer)	/* did we get the right format? */
	{
		switch (encode)
		{
			case XStandardHeader:	// standard header
				baseKey = pSndBuffer->baseFrequency;
				break;

			case XExtendedHeader:	// extended header
				pExtBuffer = (XExtSoundHeader *)pSndBuffer;
				baseKey = pExtBuffer->baseFrequency;
				break;

			case XCompressedHeader:	// compressed header
				pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
				baseKey = pCmpBuffer->baseFrequency;
				break;

			case XType3Header:		// type 3 header
				pType3Buffer = (XSoundHeader3 *)pSndBuffer;
				baseKey = pType3Buffer->baseKey;
				break;
		}
	}
	return baseKey;
}
#endif	// USE_CREATION_API == TRUE

//MOE: should probably eliminate "encodedData" and "encodedBytes" and take
//	   these data from "info".  Maybe alter "info" with new data afterwards,
//	   or fill "dstInfo" structure.
OPErr XDecodeSampleData(GM_Waveform const* src, UINT32 startFrame, GM_Waveform* dst)
{
UINT32			const bytesPerFrame = src->channels * (src->bitSize / 8);
UINT32			const startByte = startFrame * bytesPerFrame;
UINT32			const decodingFrames = src->waveFrames + startFrame;
UINT32			const decodingBytes = decodingFrames * bytesPerFrame;

	BAE_ASSERT(src);
	BAE_ASSERT(dst);
	*dst = *src;
	dst->waveSize = dst->waveFrames * bytesPerFrame;
	dst->theWaveform = NULL;
	dst->compressionType = C_NONE;

	switch (src->compressionType)
	{
	case C_NONE:	// raw pcm audio
		BAE_ASSERT(startFrame == 0);	// wouldn't make sense to skip frames
	case C_IMA4:	// IMA 4:1 - AIFF-file flavor
	case C_IMA4_WAV:// IMA 4:1 - WAV-file flavor
	case C_ALAW:	// ALAW 2:1
	case C_ULAW:	// ULAW 2:1
		dst->theWaveform = XNewPtr(decodingBytes);
		if (!dst->theWaveform)
		{
			return MEMORY_ERR;
		}

		switch (src->compressionType)
		{
		case C_NONE :
			BAE_ASSERT(src->waveSize == decodingBytes);
			XBlockMove(src->theWaveform, dst->theWaveform, decodingBytes);
			break;
		case C_IMA4 :
			XExpandAiffIma((XBYTE*)src->theWaveform, AIFF_IMA_BLOCK_BYTES,
							dst->theWaveform, dst->bitSize,
							decodingFrames, dst->channels);
			break;
		case C_IMA4_WAV :
			XExpandWavIma((XBYTE*)src->theWaveform, AIFF_IMA_BLOCK_BYTES,
							dst->theWaveform, dst->bitSize,
							src->waveSize, dst->channels);
			break;
		case C_ALAW :
			XExpandALawto16BitLinear((XBYTE*)src->theWaveform,
										(short*)dst->theWaveform,
										decodingFrames, dst->channels);
			break;
		case C_ULAW :
			XExpandULawto16BitLinear((XBYTE*)src->theWaveform,
										(short*)dst->theWaveform,
										decodingFrames, dst->channels);
			break;
		}
		break;

#if USE_MPEG_DECODER != 0
	case C_MPEG_32:
	case C_MPEG_40:
	case C_MPEG_48:
	case C_MPEG_56:
	case C_MPEG_64:
	case C_MPEG_80:
	case C_MPEG_96:
	case C_MPEG_112:
	case C_MPEG_128:
	case C_MPEG_160:
	case C_MPEG_192:
	case C_MPEG_224:
	case C_MPEG_256:
	case C_MPEG_320:
		return XExpandMPEG(src, startFrame, dst);
#endif

#if X_PLATFORM == X_MACINTOSH_9
	case C_MACE3 :
	case C_MACE6 :
	{
	void		(*maceFunction)(void *inBuffer, void *outBuffer, unsigned long cnt, 
								void * inState, void * outState, 
								unsigned long numChannels, unsigned long whichChannel);
	XDWORD		maceFrames;
	
		maceFunction = (src->compressionType == C_MACE3) ? XExpandMace1to3
														 : XExpandMace1to6;
		maceFrames = decodingFrames / 6;
		
		dst->theWaveform = XNewPtr(dst->waveSize);
		if (!dst->theWaveform)
		{
			return MEMORY_ERR;
		}

		if (dst->channels == 1)
		{
			(*maceFunction)(src->theWaveform, dst->theWaveform, maceFrames, NULL, NULL, 1, 1);
		}
		else
		{
		XPTR		rightData;
		XBYTE*		pLeft;
		XBYTE*		pRight;
		XDWORD		i;
		
			rightData = XNewPtr(dst->waveSize / 2);
			if (!rightData)
			{
				XDisposePtr(dst->theWaveform);
				dst->theWaveform = NULL;
				return MEMORY_ERR;
			}

			(*maceFunction)(src->theWaveform, dst->theWaveform, maceFrames, NULL, NULL, 2, 1);
			(*maceFunction)(src->theWaveform, rightData, maceFrames, NULL, NULL, 2, 2);
			
			pLeft = (XBYTE*)dst->theWaveform;
			pRight = (XBYTE*)rightData;
			i = decodingFrames - 1;
			// interleave the data into a stereo sample block
			// must copy backwards!
			for (i = decodingFrames - 1; i >= 0; i--)
			{
				pLeft[i*2+0] = pLeft[i];
				pLeft[i*2+1] = pRight[i];
			}
			XDisposePtr(rightData);
		}
		break;
	}
#endif
	default:
		BAE_ASSERT(FALSE);	// bad type
		return PARAM_ERR;
	}
	
	if (startByte > 0)
	{
	XPTR			resizedData;

		BAE_ASSERT(dst->waveSize == decodingBytes - startByte);	
		XBlockMove((XBYTE*)dst->theWaveform + startByte, dst->theWaveform, dst->waveSize);

		//MOE: we should really get XResizePtr() working efficiently.
		resizedData = XResizePtr(dst->theWaveform, dst->waveSize);
		if (resizedData) dst->theWaveform = resizedData;
	}

	BAE_ASSERT(dst->theWaveform);
	return NO_ERR;
}

// Given a ID_SND resource, parse through and return in *pOutInfo the information
// about the sample resource. The pMasterPtr will be NULL.
//
// Returns 	0 	if no problems, and fills *pOutInfo will details about the sample resource.
//			-1	if parameter err
//			-2	badly formatted resource
//			-3	not supported format type
#if USE_CREATION_API == TRUE
XERR XGetSampleInfoFromSnd(XPTR pResource, SampleDataInfo *pOutInfo)
{
	XERR				err;
	XSoundHeader*		header;
	short				headerType;

	err = -1;
	if (pOutInfo && pResource)
	{
		pOutInfo->size = 0;
		pOutInfo->frames = 0;
		pOutInfo->rate = rate22khz;
		pOutInfo->loopStart = 0;
		pOutInfo->loopEnd = 0;
		pOutInfo->baseKey = kMiddleC;
		pOutInfo->bitSize = 8;
		pOutInfo->channels = 1;
		pOutInfo->compressionType = C_NONE;
		pOutInfo->pMasterPtr = NULL;

		header = (XSoundHeader*)PV_GetSoundHeaderPtr(pResource, &headerType);
		BAE_ASSERT(header);
		if (header)	/* did we get the right format? */
		{
			err = 0;	// ok
			switch (headerType)
			{
				case XStandardHeader:	// standard header
					pOutInfo->frames = XGetLong(&header->length);
					pOutInfo->loopStart = XGetLong(&header->loopStart);
					pOutInfo->loopEnd = XGetLong(&header->loopEnd);
					pOutInfo->baseKey = header->baseFrequency;
					pOutInfo->rate = XGetLong(&header->sampleRate);
					pOutInfo->channels = 1;
					pOutInfo->bitSize = 8;			// defaults for standard headers
					break;

				case XExtendedHeader :	// extended header
				{
				XExtSoundHeader*	headerExt;

					headerExt = (XExtSoundHeader*)header;
					pOutInfo->channels = (short)XGetLong(&headerExt->numChannels);
					pOutInfo->bitSize = XGetShort(&headerExt->sampleSize);
					pOutInfo->frames = XGetLong(&headerExt->numFrames);
					pOutInfo->loopStart = XGetLong(&headerExt->loopStart);
					pOutInfo->loopEnd = XGetLong(&headerExt->loopEnd);
					pOutInfo->baseKey = headerExt->baseFrequency;
					pOutInfo->rate = XGetLong(&headerExt->sampleRate);
					break;
				}

				case XCompressedHeader:	// compressed header
				{
				XCmpSoundHeader*	headerCmp;
				short				compressionID;

					headerCmp = (XCmpSoundHeader*)header;
					pOutInfo->channels = (short)XGetLong(&headerCmp->numChannels);
					pOutInfo->bitSize = XGetShort(&headerCmp->sampleSize);
					pOutInfo->frames = XGetLong(&headerCmp->numFrames);
					pOutInfo->loopStart = XGetLong(&headerCmp->loopStart);
					pOutInfo->loopEnd = XGetLong(&headerCmp->loopEnd);
					pOutInfo->baseKey = headerCmp->baseFrequency;
					pOutInfo->rate = XGetLong(&headerCmp->sampleRate);

					compressionID = XGetShort(&headerCmp->compressionID);
					switch(compressionID)
					{
					case fixedCompression:
						pOutInfo->compressionType = XGetLong(&headerCmp->format);
						switch (pOutInfo->compressionType)
						{
						case C_IMA4 :	// IMA 4:1
							pOutInfo->size = pOutInfo->frames * AIFF_IMA_BLOCK_BYTES * pOutInfo->channels;
							pOutInfo->frames *= AIFF_IMA_BLOCK_FRAMES;
							pOutInfo->bitSize = (headerCmp->forceSample8bit & 0x80) ? 8 : 16;
							break;
						case C_ALAW :	// alaw 2 : 1
						case C_ULAW :	// ulaw 2 : 1
							pOutInfo->size = pOutInfo->frames * pOutInfo->channels;
							pOutInfo->bitSize = 16;				// it's stored as 8 bit
							break;
						default :
							BAE_ASSERT(FALSE);
							err = -3;
						}
						break;
			#if X_PLATFORM == X_MACINTOSH_9
					case threeToOne:
						pOutInfo->compressionType = C_MACE3;
						pOutInfo->size = pOutInfo->frames * pOutInfo->channels;
						pOutInfo->frames *= 6;			// adjust the frame count to equal the real frames
						break;
					case sixToOne:
						pOutInfo->compressionType = C_MACE6;
						pOutInfo->size = pOutInfo->frames * pOutInfo->channels;
						pOutInfo->frames *= 6;			// adjust the frame count to equal the real frames
						break;
			#endif
					default :
						BAE_ASSERT(FALSE);
						err = -3;
						break;
					}
					break;
				}

				case XType3Header:
				{
				XSoundHeader3*		header3;

					header3 = (XSoundHeader3*)header;
					pOutInfo->size = XGetLong(&header3->encodedBytes);
					pOutInfo->rate = XGetLong(&header3->sampleRate);
					pOutInfo->channels = header3->channels;
					pOutInfo->bitSize = header3->bitSize;
					pOutInfo->frames = XGetLong(&header3->frameCount);
					pOutInfo->loopStart = XGetLong(&header3->loopStart[0]);
					pOutInfo->loopEnd = XGetLong(&header3->loopEnd[0]);
					pOutInfo->baseKey = header3->baseKey;
					pOutInfo->compressionType = XGetLong(&header3->subType);

					switch (pOutInfo->compressionType)
					{
					case C_NONE:	// raw pcm audio
						break;
					case C_IMA4_WAV:	// IMA, WAV-file flavor
					case C_IMA4:		// IMA 4:1
						BAE_ASSERT(FALSE);	// nothing wrong, but we should look the first time this happens
						break;
			#if USE_MPEG_DECODER != 0
					case C_MPEG_32:
					case C_MPEG_40:
					case C_MPEG_48:
					case C_MPEG_56:
					case C_MPEG_64:
					case C_MPEG_80:
					case C_MPEG_96:
					case C_MPEG_112:
					case C_MPEG_128:
					case C_MPEG_160:
					case C_MPEG_192:
					case C_MPEG_224:
					case C_MPEG_256:
					case C_MPEG_320:
						BAE_ASSERT(pOutInfo->bitSize == 16);	//MOE: shouldn't it already be set thus?
						pOutInfo->bitSize = 16;
						
						// header's "frameCount" is garbage (was once MPEG block count) recalculate
						pOutInfo->frames = (XDWORD)XGetLong(&header3->decodedBytes) / pOutInfo->channels / 2;
						break;
			#endif
					default:
						BAE_PRINTF("Unsupported codec %ld\n", pOutInfo->compressionType);
						BAE_ASSERT(FALSE);
						err = -3;
						break;
					}
					break;
				}
			}

			// check byte size
			if (pOutInfo->compressionType == C_NONE)
			{
				pOutInfo->size = pOutInfo->frames * pOutInfo->channels * (pOutInfo->bitSize / 8);
			}

			// verify loop points
			if (((long)pOutInfo->loopStart < 0) ||
				(pOutInfo->loopStart > pOutInfo->loopEnd) ||
				(pOutInfo->loopEnd > pOutInfo->frames))
			{
				pOutInfo->loopStart = 0;
				pOutInfo->loopEnd = 0;
			}
		}
		else
		{
			err = -2;
		}
	}
	return err;
}
#endif	// USE_CREATION_API == TRUE

// Given a ID_SND resource, parse through and return in *pInfo the information
// about the sample resource. The pMasterPtr will be set to the resource passed in (pRes).
//
// Returns	a pointer that is either 
//			1) an offset from pRes which is the actaul PCM data
//			2) a new allocated pointer which is the decoded data from its compressed form.
//				Deallocate this pointer with XDisposePtr.
XPTR XGetSamplePtrFromSnd(XPTR pRes, SampleDataInfo* info)
{
#if TRUE
XSoundHeader*		header;
short				headerType;
XPTR				encodedData;
UINT32				startFrame;
XBYTE*				inIntelOrder;
XPTR				sampleData;
XBYTE				order = X_WORD_ORDER;

	inIntelOrder = &order;
	info->size = 0;			//MOE: used outside as a signal of failure?
	info->frames = 0;
	info->rate = rate22khz;
	info->loopStart = 0;
	info->loopEnd = 0;
	info->baseKey = kMiddleC;
	info->bitSize = 8;
	info->channels = 1;
	info->compressionType = C_NONE;
	info->pMasterPtr = pRes;

	header = (XSoundHeader*)PV_GetSoundHeaderPtr(pRes, &headerType);
	BAE_ASSERT(header);
	if (!header)	/* did we get the right format? */
	{
		return NULL;
	}

	encodedData = NULL;
	startFrame = 0;

	switch (headerType)
	{
	case XStandardHeader:	// standard header
		encodedData = (char *)&header->sampleArea[0];
		info->frames = XGetLong(&header->length);
		info->loopStart = XGetLong(&header->loopStart);
		info->loopEnd = XGetLong(&header->loopEnd);
		info->baseKey = header->baseFrequency;
		info->rate = XGetLong(&header->sampleRate);
		info->channels = 1;
		info->bitSize = 8;			// defaults for standard headers
		break;

	case XExtendedHeader :	// extended header
	{
	XExtSoundHeader*	headerExt;

		headerExt = (XExtSoundHeader*)header;
		encodedData = (char *)&headerExt->sampleArea[0];
		info->channels = (short)XGetLong(&headerExt->numChannels);
		info->bitSize = XGetShort(&headerExt->sampleSize);
		info->frames = XGetLong(&headerExt->numFrames);
		info->loopStart = XGetLong(&headerExt->loopStart);
		info->loopEnd = XGetLong(&headerExt->loopEnd);
		info->baseKey = headerExt->baseFrequency;
		info->rate = XGetLong(&headerExt->sampleRate);
		inIntelOrder = (XBYTE*)&headerExt->sampleIsIntelOrder;
		break;
	}

	case XCompressedHeader:	// compressed header
	{
	XCmpSoundHeader*	headerCmp;
	short				compressionID;

		headerCmp = (XCmpSoundHeader*)header;
		encodedData = (XPTR)XGetLong(&headerCmp->samplePtr);
		if (!encodedData)	/* get ptr to sample data */
		{
			encodedData = headerCmp->sampleArea;
		}
		info->channels = (short)XGetLong(&headerCmp->numChannels);
		info->bitSize = XGetShort(&headerCmp->sampleSize);
		info->frames = XGetLong(&headerCmp->numFrames);
		info->loopStart = XGetLong(&headerCmp->loopStart);
		info->loopEnd = XGetLong(&headerCmp->loopEnd);
		info->baseKey = headerCmp->baseFrequency;
		info->rate = XGetLong(&headerCmp->sampleRate);

		compressionID = XGetShort(&headerCmp->compressionID);
		switch(compressionID)
		{
		case fixedCompression:
			info->compressionType = XGetLong(&headerCmp->format);
			switch (info->compressionType)
			{
			case C_IMA4 :	// IMA 4:1
				info->size = info->frames * AIFF_IMA_BLOCK_BYTES * info->channels;
				info->frames *= AIFF_IMA_BLOCK_FRAMES;
				info->bitSize = (headerCmp->forceSample8bit & 0x80) ? 8 : 16;
				break;
			case C_ALAW :	// alaw 2 : 1
			case C_ULAW :	// ulaw 2 : 1
				info->size = info->frames * info->channels;
				info->bitSize = 16;				// it's stored as 8 bit
				break;
			default :
				BAE_ASSERT(FALSE);
				return NULL;
			}
			break;
#if X_PLATFORM == X_MACINTOSH_9
		case threeToOne:
			info->compressionType = C_MACE3;
			info->size = info->frames * info->channels;
			info->frames *= 6;			// adjust the frame count to equal the real frames
			break;
		case sixToOne:
			info->compressionType = C_MACE6;
			info->size = info->frames * info->channels;
			info->frames *= 6;			// adjust the frame count to equal the real frames
			break;
#endif
		default :
			BAE_ASSERT(FALSE);
			return NULL;
		}
		break;
	}
	
	case XType3Header:
	{
	XSoundHeader3*		header3;

		header3 = (XSoundHeader3*)header;
		encodedData = &header3->sampleArea[0];
		startFrame = XGetLong(&header3->startFrame);

		info->size = XGetLong(&header3->encodedBytes);
		info->rate = XGetLong(&header3->sampleRate);
		info->channels = header3->channels;
		info->bitSize = header3->bitSize;
		info->frames = XGetLong(&header3->frameCount);
		info->loopStart = XGetLong(&header3->loopStart[0]);
		info->loopEnd = XGetLong(&header3->loopEnd[0]);
		info->baseKey = header3->baseKey;
		info->compressionType = XGetLong(&header3->subType);

		switch (info->compressionType)
		{
		case C_NONE:	// raw pcm audio
			BAE_ASSERT(startFrame == 0);
			startFrame = 0;		// just in case
			inIntelOrder = &header3->isSampleIntelOrder;
			break;
		case C_IMA4_WAV:	// IMA, WAV-file flavor
		case C_IMA4:		// IMA 4:1
			BAE_ASSERT(FALSE);	// nothing wrong, but we should look the first time this happens
			break;
#if USE_MPEG_DECODER != 0
		case C_MPEG_32:
		case C_MPEG_40:
		case C_MPEG_48:
		case C_MPEG_56:
		case C_MPEG_64:
		case C_MPEG_80:
		case C_MPEG_96:
		case C_MPEG_112:
		case C_MPEG_128:
		case C_MPEG_160:
		case C_MPEG_192:
		case C_MPEG_224:
		case C_MPEG_256:
		case C_MPEG_320:
			BAE_ASSERT(info->bitSize == 16);	//MOE: shouldn't it already be set thus?
			info->bitSize = 16;
			
			// header's "frameCount" is garbage (was once MPEG block count) recalculate
			info->frames = (XDWORD)XGetLong(&header3->decodedBytes) / info->channels / 2;
			startFrame /= info->channels;			// header3->startFrame counts samples, not frames (for MPEG only)
			break;
#endif
		default:
			BAE_PRINTF("Unsupported codec %ld\n", info->compressionType);
			BAE_ASSERT(FALSE);
			return NULL;
		}
		break;
	}
	
	}

	BAE_ASSERT(encodedData);
	if (info->compressionType == C_NONE)
	{
		info->size = info->frames * info->channels * (info->bitSize / 8);
		sampleData = encodedData;
		// This is the tricky special case of this function.
		// Only here do we return the source data pointer rather than
		// allocating and filling a new block of sample data.
		// info->pMasterPtr is used later to determine whether the pointer
		// should be disposed.  (a bit hacky)
		
#if X_WORD_ORDER != FALSE
		// if we're on a non motorola platform, swap sample data for 16 bit samples because its stored in a motorola way
		if ((info->bitSize == 16) && !*inIntelOrder)
		{
			// if we're reading from a memory file, check to see if the samples are intel ordered
			// if not, then swap them and set this flag. This will fail if we're working from
			// ROM. If its ROM then we need to copy the data.
			*inIntelOrder = TRUE;	// try and modify copy
									// next time sample is read it will be right

#if X_PLATFORM != X_WIN95 && X_PLATFORM != X_BE
			if (!*inIntelOrder)
			{
				// since we are reading from ROM, we need to copy the data
				// before we can swap it.
				sampleData = XNewPtr(info->size);  // 09.30.00 tl. this was XNewPtr(encodedBytes);
				if (!sampleData)
				{
					return NULL;
				}
				info->pMasterPtr = sampleData;
				XBlockMove(encodedData, sampleData, info->size);
			}
#endif
			XSwapShorts((short*)sampleData, info->size / 2);
		}
#endif
	}
	else
	{
	GM_Waveform		encoded;
	GM_Waveform		decoded;
	
		XTranslateFromSampleDataToWaveform(info, &encoded);
		encoded.theWaveform = encodedData;
		
		if (XDecodeSampleData(&encoded, startFrame, &decoded) != NO_ERR)
		{
			return NULL;
		}
		XTranslateFromWaveformToSampleData(&decoded, info);
		// must preserve compression type for later use. This matches the data compression
		// type on disk, not in memory.
		info->compressionType = encoded.compressionType;	
		sampleData = info->pMasterPtr;
	}
	
	if (((long)info->loopStart < 0) ||
		(info->loopStart > info->loopEnd) ||
		(info->loopEnd > info->frames))
	{
		info->loopStart = 0;
		info->loopEnd = 0;
	}

	return sampleData;
	
#else

XPTR				decodedData;
register XSoundHeader		*pSndBuffer;
register XCmpSoundHeader	*pCmpBuffer;
register XExtSoundHeader	*pExtBuffer;
register XSoundHeader3		*pType3Buffer;
long						offset;
char						*sampleData;
short int					encode;
#if X_PLATFORM == X_MACINTOSH
XPTR						rightData;
long						count;
char						*pLeft, *pRight;
#endif

	sampleData = NULL;
	info->size = 0;		// if left alone, then wrong type of resource
	info->frames = 0;
	info->rate = rate22khz;
	info->loopStart = 0;
	info->loopEnd = 0;
	info->baseKey = kMiddleC;
	info->bitSize = 8;
	info->channels = 1;
	info->compressionType = C_NONE;

	pSndBuffer = (XSoundHeader *)PV_GetSoundHeaderPtr(pRes, &encode);
	BAE_ASSERT(pSndBuffer);
	if (!pSndBuffer)	/* did we get the right format? */
	{
		return NULL;
	}
	
	switch (encode)
	{
	case XStandardHeader:	// standard header
		sampleData = (char *)&pSndBuffer->sampleArea[0];
		info->size = XGetLong(&pSndBuffer->length);
		info->frames = info->size;
		info->loopStart = XGetLong(&pSndBuffer->loopStart);
		info->loopEnd = XGetLong(&pSndBuffer->loopEnd);
		info->baseKey = pSndBuffer->baseFrequency;
		info->rate = XGetLong(&pSndBuffer->sampleRate);
		info->channels = 1;
		info->bitSize = 8;			// defaults for standard headers
		info->pMasterPtr = pRes;
		break;

	case XType3Header:
		pType3Buffer = (XSoundHeader3*)pSndBuffer;
		sampleData = (char *)&pType3Buffer->sampleArea[0];

		info->rate = XGetLong(&pType3Buffer->sampleRate);
		info->channels = pType3Buffer->channels;
		info->bitSize = pType3Buffer->bitSize;
		info->size = XGetLong(&pType3Buffer->decodedBytes);
		info->frames = XGetLong(&pType3Buffer->frameCount);
		info->loopStart = XGetLong(&pType3Buffer->loopStart[0]);
		info->loopEnd = XGetLong(&pType3Buffer->loopEnd[0]);
		info->baseKey = pType3Buffer->baseKey;
		info->compressionType = XGetLong(&pType3Buffer->subType);

		switch (info->compressionType)
		{
		case C_NONE:	// raw pcm audio
			info->size = XGetLong(&pType3Buffer->encodedBytes);	// decodedBytes not stored correctly!
			#if X_WORD_ORDER != FALSE
			// if we're on a non motorola platform, swap sample data for 16 bit samples because its stored in a motorola way
			if (info->bitSize == 16)
			{
				// if we're reading from a memory file, check to see if the samples are intel ordered
				// if not, then swap them and set this flag. This will fail if we're working from
				// ROM. If its ROM then we need to copy the data.
				if (pType3Buffer->isSampleIntelOrder == FALSE)
				{
					pType3Buffer->isSampleIntelOrder = TRUE;	// try and modify copy
															// next time sample is read it will be right
					if (pType3Buffer->isSampleIntelOrder)
					{	// Not ROM, must be a memory file, or a local copy
						PV_Swap16BitSamplesIfIntel((short*)sampleData,
											info->frames * info->channels);
					}
					else
					{
						// since we are reading from ROM, we need to copy the data
						// before we can swap it.
						decodedData = XNewPtr(info->size);
						info->pMasterPtr = decodedData;
						if (decodedData)
						{
							XBlockMove(sampleData, decodedData, info->size);
							PV_Swap16BitSamplesIfIntel((short*)decodedData,
												info->frames *info->channels);
						}
						sampleData = (char *)decodedData;
					}
				}
			}
			#endif
			break;
#if USE_MPEG_DECODER != 0
		// Beatnik MPEG
		case C_MPEG_32:
		case C_MPEG_40:
		case C_MPEG_48:
		case C_MPEG_56:
		case C_MPEG_64:
		case C_MPEG_80:
		case C_MPEG_96:
		case C_MPEG_112:
		case C_MPEG_128:
		case C_MPEG_160:
		case C_MPEG_192:
		case C_MPEG_224:
		case C_MPEG_256:
		case C_MPEG_320:
		{
		XDWORD		startFrame;
		XDWORD		encodedBytes;
		OPErr		err;
		
			startFrame = XGetLong(&pType3Buffer->startFrame) / info->channels;
			encodedBytes = XGetLong(&pType3Buffer->encodedBytes);

			err = XExpandMPEG(&decodedData, sampleData, encodedBytes, startFrame, info->size);
			info->pMasterPtr = decodedData;
			sampleData = (char*)decodedData;

			BAE_ASSERT(info->bitSize == 16);	//MOE: shouldn't it already be set thus?
			info->bitSize = 16;
			info->frames = info->size / info->channels / 2;
			break;
		}
#endif
		default:
			BAE_ASSERT(FALSE);
			sampleData = NULL;	// bad type
			break;
		}
		break;

	case XExtendedHeader:	// extended header
		pExtBuffer = (XExtSoundHeader *)pSndBuffer;
		sampleData = (char *)&pExtBuffer->sampleArea[0];
		info->channels = (short)XGetLong(&pExtBuffer->numChannels);
		info->bitSize = XGetShort(&pExtBuffer->sampleSize);
		info->frames = XGetLong(&pExtBuffer->numFrames);
		info->size = info->frames * (info->channels) * (info->bitSize / 8);
		info->loopStart = XGetLong(&pExtBuffer->loopStart);
		info->loopEnd = XGetLong(&pExtBuffer->loopEnd);
		info->baseKey = pExtBuffer->baseFrequency;
		info->rate = XGetLong(&pExtBuffer->sampleRate);
		info->pMasterPtr = pRes;

		#if X_WORD_ORDER != FALSE
		// if we're on a non motorola platform, swap sample data for 16 bit samples because its stored in a motorola way
		if (info->bitSize == 16)
		{
			// if we're reading from a memory file, check to see if the samples are intel ordered
			// if not, then swap them and set this flag. This will fail if we're working from
			// ROM. If its ROM then we need to copy the data.
			if (pExtBuffer->sampleIsIntelOrder == FALSE)
			{
				pExtBuffer->sampleIsIntelOrder = TRUE;	// try and modify copy
														// next time sample is read it will be right
				if (pExtBuffer->sampleIsIntelOrder)
				{	// Not ROM, must be a memory file, or a local copy
					PV_Swap16BitSamplesIfIntel((short *)sampleData,
										info->frames * info->channels);
				}
				else
				{
					// since we are reading from ROM, we need to copy the data
					// before we can swap it.
					decodedData = XNewPtr(info->size);
					info->pMasterPtr = decodedData;
					if (decodedData)
					{
						XBlockMove(sampleData, decodedData, info->size);
						PV_Swap16BitSamplesIfIntel((short *)decodedData,
											info->frames * info->channels);
					}
					sampleData = (char *)decodedData;
				}
			}
		}
		#endif
		break;

	case XCompressedHeader:	// compressed header
		pCmpBuffer = (XCmpSoundHeader *)pSndBuffer;
		sampleData = (char *)XGetLong(&pCmpBuffer->samplePtr);
		if (sampleData == NULL)	/* get ptr to sample data */
		{
			sampleData = (char *) pCmpBuffer->sampleArea;
		}
		info->channels = (short)XGetLong(&pCmpBuffer->numChannels);
		info->bitSize = XGetShort(&pCmpBuffer->sampleSize);
		BAE_ASSERT((info->bitSize == 8) || (info->bitSize == 16));
		info->frames = XGetLong(&pCmpBuffer->numFrames);
		info->loopStart = XGetLong(&pCmpBuffer->loopStart);
		info->loopEnd = XGetLong(&pCmpBuffer->loopEnd);
		info->baseKey = pCmpBuffer->baseFrequency;
		info->rate = XGetLong(&pCmpBuffer->sampleRate);

		encode = XGetShort(&pCmpBuffer->compressionID);
		switch(encode)
		{
		default:
			DEBUG_STR("\nInvalid compression ID");
			break;

		case fixedCompression:
			info->compressionType = XGetLong(&pCmpBuffer->format);
			switch (info->compressionType)
			{
//			case C_IMA4_WAV:	// IMA, WAV-file flavor
//				should be fairly simple to implement with XExpandWavIma()
			case C_IMA4:	// IMA 4:1
				info->frames *= AIFF_IMA_BLOCK_FRAMES;
				info->size = info->frames * info->channels;
				if ((pCmpBuffer->forceSample8bit & 0x80) == 0)
				{
				// do a 16 bit decompression. As we decompress the IMA we build a 16 bit sample
					info->size *= 2;
					info->bitSize = 16;			// must change to final output size
				}
				else
				{
				// do a 8 bit decompression. As we decompress the IMA we build a 8 bit sample
					info->bitSize = 8;			// must change to final output size
				}
				decodedData = XNewPtr(info->size);
				if (decodedData)
				{
					XExpandAiffIma((XBYTE const*)sampleData, AIFF_IMA_BLOCK_BYTES,
									decodedData, info->bitSize,
									info->frames,info->channels);
				}
				break;
			case C_ALAW:	// alaw 2 : 1
				info->bitSize = 16;								// must change, its stored as 8 bit
				info->size = info->frames * info->channels * 2;	// always 16 bit
				decodedData = XNewPtr(info->size);
				if (decodedData)
				{
					XExpandALawto16BitLinear((XBYTE*)sampleData,
												(short int *)decodedData,
												info->frames, info->channels);
				}
				break;
			case C_ULAW:	// ulaw 2 : 1
				info->bitSize = 16;								// must change, its stored as 8 bit
				info->size = info->frames * info->channels * 2;	// always 16 bit
				decodedData = XNewPtr(info->size);
				if (decodedData)
				{
					XExpandULawto16BitLinear((XBYTE*)sampleData,
												(short int *)decodedData,
												info->frames, info->channels);
				}
				break;
			}
			info->pMasterPtr = decodedData;
			sampleData = (char*)decodedData;
			break;
		case threeToOne:
#if X_PLATFORM != X_MACINTOSH
			BAE_ASSERT(FALSE);
			decodedData = NULL;	// don't support this format
#else
			info->frames *= 6;			// adjust the frame count to equal the real frames
			info->size = info->frames * (info->channels) * (info->bitSize / 8);
			// 2 bytes at 3:1 is 6 bytes for a packet, 1 byte at 6:1 is 6 bytes too
			decodedData = XNewPtr(info->size);
			if (decodedData)
			{
				if (info->channels == 1)
				{
					XExpandMace1to3(sampleData, decodedData, info->frames, NULL, NULL, 1, 1);
					info->frames *= 6;			// adjust the frame count to equal the real frames
				}
				else
				{
					rightData = XNewPtr(info->size / 2);
					if (rightData)
					{
						XExpandMace1to3(sampleData, decodedData, info->frames, NULL, NULL, info->channels, 1);
						XExpandMace1to3(sampleData, rightData, info->frames, NULL, NULL, info->channels, 2);
						pLeft = (char *)decodedData;
						pRight = (char *)rightData;
						info->frames *= 6;			// adjust the frame count to equal the real frames
						offset = info->frames - 1;
						// copy the data into a stereo sample block, copy backwards so that we don't have to create
						// two blocks of data
						for (count = offset; count >= 0; count--)
						{
							pLeft[count*2+0] = pLeft[count];
							pLeft[count*2+1] = pRight[count];
						}
						XDisposePtr(rightData);
					}
				}
			}
#endif
			info->compressionType = C_MACE3;
			info->pMasterPtr = decodedData;
			sampleData = (char *)decodedData;
			break;
		case sixToOne:
#if X_PLATFORM != X_MACINTOSH
			BAE_ASSERT(FALSE);
			decodedData = NULL;	// don't support this format
#else
			info->frames *= 6;			// adjust the frame count to equal the real frames
			info->size = info->frames * (info->channels) * (info->bitSize / 8);
			// 2 bytes at 3:1 is 6 bytes for a packet, 1 byte at 6:1 is 6 bytes too
			decodedData = XNewPtr(info->size);
			if (decodedData)
			{
				if (info->channels == 1)
				{
					XExpandMace1to6(sampleData, decodedData, info->frames, NULL, NULL, 1, 1);
				}
				else
				{
					rightData = XNewPtr(info->size / 2);
					if (rightData)
					{
						XExpandMace1to6(sampleData, decodedData, info->frames, NULL, NULL, info->channels, 1);
						XExpandMace1to6(sampleData, rightData, info->frames, NULL, NULL, info->channels, 2);
						pLeft = (char *)decodedData;
						pRight = (char *)rightData;
						info->frames *= 6;			// adjust the frame count to equal the real frames
						offset = info->frames - 1;
						// copy the data into a stereo sample block, copy backwards so that we don't have to create
						// two blocks of data
						for (count = offset; count >= 0; count--)
						{
							pLeft[count*2+0] = pLeft[count];
							pLeft[count*2+1] = pRight[count];
						}
						XDisposePtr(rightData);
					}
				}
			}
#endif
			info->compressionType = C_MACE6;
			info->pMasterPtr = decodedData;
			sampleData = (char *)decodedData;
			break;
		}
		break;
	}

	if (((long)info->loopStart < 0) ||
		(info->loopStart > info->loopEnd) ||
		(info->loopEnd > info->frames))
	{
		info->loopStart = 0;
		info->loopEnd = 0;
	}
	return sampleData;
#endif
}

#if USE_CREATION_API == TRUE
// Given a sample ID, this will search through sample types and return a 'C' string
// of the resource name of the currently open resource files
XBOOL XGetSampleNameFromID(XLongResourceID sampleSoundID, char cName[256])
{
	static XResourceType	sampleType[] = {ID_CSND, ID_ESND, ID_SND};
	short int				count;
	XBOOL					bad;

	bad = FALSE;
	if (cName)
	{
		count = 0;
		while (TRUE)
		{
			if (count >= sizeof(sampleType) / sizeof(XResourceType))
			{
				bad = TRUE;
				break;
			}
			if (XGetResourceName(sampleType[count], sampleSoundID, cName))
			{
				break;
			}
			count++;
		}
	}
	return bad;		//MOE: Why are we returning TRUE for failure?
}
#endif

#if USE_CREATION_API == TRUE
// given 8 bit data, convert this to 16 bit data
XWORD * XConvert8BitTo16Bit(XBYTE * p8BitPCMData, unsigned long frames, unsigned long channels)
{
	XWORD			*newData;
	unsigned long	count, ccount;
	short int		sample;

	ccount = frames * channels;
	newData = (XWORD *)XNewPtr(ccount * sizeof(short));
	if (newData)
	{
		for (count = 0; count < ccount; count++)
		{
			sample = (p8BitPCMData[count] - 128) << 8;
			sample += XRandomRange(3);		// add a little noise. This is a trick of the ear. This doesn't make
											// sense, but the brain filters the noise and the 8 bit conversion sounds
											// less fuzzy.
			newData[count] = sample;
		}

	}
	return newData;
}
#endif

// given 16 bit data, convert this to 8 bit data
XBYTE * XConvert16BitTo8Bit(XWORD * p16BitPCMData, unsigned long frames, unsigned long channels)
{
	XBYTE			*newData = NULL;
	unsigned long	count, ccount;
	XBYTE			sample;

	if (p16BitPCMData)
	{
		ccount = frames * channels;
		newData = (XBYTE *)XNewPtr(ccount * sizeof(char));
		if (newData)
		{
			for (count = 0; count < ccount; count++)
			{
				sample = (p16BitPCMData[count] >> 8) + 128;
				newData[count] = sample;
			}
		}
	}
	return newData;
}


#if USE_CREATION_API == TRUE
// Given a data stream in (dst). This can be unformatted PCM data, or a compressed stream,
// a GM_Waveform (src) describing the data 
// Pass in type a compression type use C_NONE for no compression, and CS_NONE for sub type.
// The compression sub type is used as a modifer of the (dstCompression). It is used to
// control somthing specific with the encoder. Most of the time, you'll pass in CS_NONE.
// Pass in a function that will be called proc during compression only
// Pass in a void* parameter to be passed to the proc
OPErr XCreateSoundObjectFromData(XPTR* dst,
									GM_Waveform const* srcWave,
									SndCompressionType dstCompression,
									SndCompressionSubType dstCompressionSubType,
									XCompressStatusProc proc, void* procData)
{
GM_Waveform			src;
XPTR				intermediateData;
XSoundFormat1*		header;

	BAE_ASSERT(dst);
	BAE_ASSERT(srcWave);
	BAE_ASSERT(srcWave->theWaveform);
	BAE_ASSERT(srcWave->waveSize > 0);
#if USE_MPEG_ENCODER == 0
	proc;			// suppress unreferenced parameter warning
	procData;		// suppress unreferenced parameter warning
#endif

	*dst = NULL;

	src = *srcWave;
	intermediateData = NULL;
/*
	if ((src.compressionType == C_IMA4_WAV) && (dstCompression == C_IMA4))
	{
		*** convert WAV-style IMA to AIFF-style IMA
		src.theWaveform =
		src.waveSize =
		src.compressionType = C_IMA4;
	}
	else
*/
	if ((src.compressionType != (XDWORD)C_NONE) && (src.compressionType != (XDWORD)dstCompression))
	{
	GM_Waveform		decoded;
	OPErr			err;

		//MOE: if data is C_IMA4, we should suppress decoding into 8 bits
		//     if dstCompression != C_NONE
		err = XDecodeSampleData(&src, 0, &decoded);
		if (err != NO_ERR)
		{
			return err;
		}
		src = decoded;
		intermediateData = src.theWaveform;
	}
	
	switch (dstCompression)
	{
	default:
	case C_MACE3 :
	case C_MACE6 :
	case C_IMA4_WAV :
		BAE_ASSERT(FALSE);
		XDisposePtr(intermediateData);
		return PARAM_ERR;

	case C_NONE:
	{
	XExtSndHeader1*	snd;

		*dst = XNewPtr(sizeof(XExtSndHeader1) + src.waveSize);
		if (!*dst)
		{
			XDisposePtr(intermediateData);
			return MEMORY_ERR;
		}

		snd = (XExtSndHeader1*)*dst;

		XPutLong(&snd->sndBuffer.samplePtr, 0L);
		XPutLong(&snd->sndBuffer.numChannels, src.channels);
		XPutShort(&snd->sndBuffer.sampleSize, src.bitSize);
		XPutLong(&snd->sndBuffer.sampleRate, src.sampledRate);
		XConvertToIeeeExtended(src.sampledRate,
								(unsigned char*)snd->sndBuffer.AIFFSampleRate);
		XPutLong(&snd->sndBuffer.loopStart, src.startLoop);
		XPutLong(&snd->sndBuffer.loopEnd, src.endLoop);
		snd->sndBuffer.encode = XExtendedHeader;		// extended header
		snd->sndBuffer.baseFrequency = (unsigned char)src.baseMidiPitch;
		XPutLong(&snd->sndBuffer.numFrames, src.waveFrames);
		XBlockMove(src.theWaveform, &snd->sndBuffer.sampleArea, src.waveSize);
		if (src.bitSize == 16)
		{
			PV_Swap16BitSamplesIfIntel((short*)&snd->sndBuffer.sampleArea,
										src.waveFrames * src.channels);
		}

		header = &snd->sndHeader;
		break;
	}

	case C_IMA4:
	{
	XDWORD			const imaBlocks =
						(src.waveFrames + AIFF_IMA_BLOCK_FRAMES - 1) / AIFF_IMA_BLOCK_FRAMES;
	XDWORD			const imaBytes =
						imaBlocks * src.channels * AIFF_IMA_BLOCK_BYTES;
	XCmpSndHeader1*	snd;

		*dst = XNewPtr(sizeof(XCmpSndHeader1) + imaBytes);
		if (!*dst) 
		{
			XDisposePtr(intermediateData);
			return MEMORY_ERR;
		}

		snd = (XCmpSndHeader1*)*dst;

		XPutLong(&snd->sndBuffer.samplePtr, 0L);
		XPutLong(&snd->sndBuffer.numChannels, src.channels);
		XPutShort(&snd->sndBuffer.sampleSize, 2);
		if (src.bitSize == 8)
		{
			snd->sndBuffer.forceSample8bit |= 0x80;
		}

		XPutLong(&snd->sndBuffer.sampleRate, src.sampledRate);
		XConvertToIeeeExtended(src.sampledRate, (unsigned char *)snd->sndBuffer.AIFFSampleRate);

		XPutLong(&snd->sndBuffer.loopStart, src.startLoop);
		XPutLong(&snd->sndBuffer.loopEnd, src.endLoop);

		snd->sndBuffer.encode = XCompressedHeader;		// compressed header
		XPutShort(&snd->sndBuffer.compressionID, (unsigned short)fixedCompression);
		XPutLong(&snd->sndBuffer.format, C_IMA4);
		snd->sndBuffer.baseFrequency = (unsigned char)src.baseMidiPitch;

		XPutLong(&snd->sndBuffer.numFrames, imaBlocks);
		XPutShort(&snd->sndBuffer.packetSize, 0);
		XPutShort(&snd->sndBuffer.snthID, 0);

		if (src.compressionType == C_NONE)
		{
			XCompressAiffIma(src.theWaveform, src.bitSize, snd->sndBuffer.sampleArea,
								src.waveFrames, src.channels);
		}
		else
		{
			BAE_ASSERT(src.compressionType == C_IMA4);
			BAE_ASSERT(src.waveSize <= imaBytes);
			if (src.waveSize > imaBytes) src.waveSize = imaBytes;
			XBlockMove(src.theWaveform, snd->sndBuffer.sampleArea, src.waveSize);
		}
		
		header = &snd->sndHeader;
		break;
	}

	case C_ULAW:
	case C_ALAW:
	{
	XDWORD			const lawBlocks = src.waveFrames / 2;
	XDWORD			const lawBytes = lawBlocks * sizeof(short) * src.channels;
	XCmpSndHeader1	*snd;

		if ((src.bitSize != 16) && (src.compressionType == C_NONE))
		{
		XPTR		wordData;
		
			// convert to 16 bit data first, then compress
			wordData = XConvert8BitTo16Bit((XBYTE*)src.theWaveform,
											src.waveFrames, src.channels);
			XDisposePtr(intermediateData);
			if (!wordData)
			{
				return MEMORY_ERR;
			}
			intermediateData = wordData;
			src.theWaveform = wordData;
			src.bitSize = 16;
			src.waveSize *= 2;
		}

		*dst = XNewPtr(sizeof(XCmpSndHeader1) + lawBytes);
		if (!*dst)
		{
			XDisposePtr(intermediateData);
			return MEMORY_ERR;
		}

		snd = (XCmpSndHeader1*)*dst;

		XPutLong(&snd->sndBuffer.samplePtr, 0L);
		XPutLong(&snd->sndBuffer.numChannels, src.channels);
		XPutShort(&snd->sndBuffer.sampleSize, 2);

		XPutLong(&snd->sndBuffer.sampleRate, src.sampledRate);
		XConvertToIeeeExtended(src.sampledRate, (unsigned char *)snd->sndBuffer.AIFFSampleRate);

		XPutLong(&snd->sndBuffer.loopStart, src.startLoop);
		XPutLong(&snd->sndBuffer.loopEnd, src.endLoop);

		snd->sndBuffer.encode = XCompressedHeader;		// compressed header
		XPutShort(&snd->sndBuffer.compressionID, (unsigned short)fixedCompression);
		XPutLong(&snd->sndBuffer.format, dstCompression);
		snd->sndBuffer.baseFrequency = (unsigned char)src.baseMidiPitch;

		XPutLong(&snd->sndBuffer.numFrames, src.waveFrames);
		XPutShort(&snd->sndBuffer.packetSize, 0);
		XPutShort(&snd->sndBuffer.snthID, 0);

		if (src.compressionType == C_NONE)
		{
			XCompressLaw(dstCompression, (short int*)src.theWaveform,
							(char*)snd->sndBuffer.sampleArea,
							src.waveFrames, src.channels);
		}
		else
		{
			BAE_ASSERT(src.compressionType == (XDWORD)dstCompression);
			BAE_ASSERT(src.waveSize <= lawBytes);
			if (src.waveSize > lawBytes) src.waveSize = lawBytes;
			XBlockMove(src.theWaveform, snd->sndBuffer.sampleArea, src.waveSize);
		}

		header = &snd->sndHeader;
		break;
	}

#if USE_MPEG_ENCODER != 0
	// Beatnik MPEG
	case C_MPEG_32:
	case C_MPEG_40:
	case C_MPEG_48:
	case C_MPEG_56:
	case C_MPEG_64:
	case C_MPEG_80:
	case C_MPEG_96:
	case C_MPEG_112:
	case C_MPEG_128:
	case C_MPEG_160:
	case C_MPEG_192:
	case C_MPEG_224:
	case C_MPEG_256:
	case C_MPEG_320:
	{
	UINT32			decodedBytes;
	UINT32			startFrame;
	UINT32			blockBytes;
	UINT32			blockCount;
	OPErr			err;
	XSndHeader3*	snd;

		if (src.compressionType == C_NONE)
		{
		XPTR			encodedData;
		UINT32			encodedBytes;

			if ((src.bitSize != 16) && (src.compressionType == C_NONE))
			{
			XPTR			wordData;
			
				// convert to 16 bit data first, then compress
				wordData = XConvert8BitTo16Bit((XBYTE*)src.theWaveform,
												src.waveFrames, src.channels);
				XDisposePtr(intermediateData);
				if (!wordData)
				{
					return MEMORY_ERR;
				}
				intermediateData = wordData;
				src.theWaveform = wordData;
				src.bitSize = 16;
				src.waveSize *= 2;
			}

			err = XCompressMPEG(&src, dstCompression, dstCompressionSubType,
								proc, procData,
								&encodedData, &encodedBytes,
								&blockCount, &blockBytes, &startFrame);
			XDisposePtr(intermediateData);
			if (err != NO_ERR)
			{
				return err;
			}
			intermediateData = encodedData;
			src.theWaveform = encodedData;
			decodedBytes = src.waveSize;
			src.waveSize = encodedBytes;
			blockCount += 2;						// the plus 2 compensates for BAE 1.5
													// needing an extra buffer.
													// What's important is that the decodedBytes
													// be less than blockCount * blockBytes.
		}
		else
		{
			XMPEGDecodedData*	stream;
			XBYTE				data[10];
		
			// we need to check for the ID3 format tag and skip it.
			// see http://www.id3.org/ for details.
			XBlockMove(src.theWaveform, data, 10L);
			if ( (data[0] == 0x49) &&
				 (data[1] == 0x44) &&
				 (data[2] == 0x33))
			{
				unsigned long tagSize;
				// inital tag ok. Now try to get size.
				
				// The ID3v2 tag size is encoded with four bytes where the most significant bit (bit 7) 
				// is set to zero in every byte, making a total of 28 bits. The zeroed bits are ignored, 
				// so a 257 bytes long tag is represented as $00 00 02 01. 
				
				tagSize = data[6] * 128;
				tagSize += data[7] * 128;
				tagSize += data[8] * 128;
				tagSize += data[9];
				tagSize += 10;	// to account for header
				
				// ok, now adjust our local copy of the data stream.
				src.waveSize -= tagSize;
				src.theWaveform = (char *)src.theWaveform + tagSize;
			}
		
			// Have to open the stream simply to find out how many blocks
			// This is only necessary to be compatible with players
			// using BAE 1.51 and earlier code.  Could be taken out some day.
			stream = XOpenMPEGStreamFromMemory(src.theWaveform, src.waveSize, &err);
			if (!stream)
			{
				return err;
			}
			blockBytes = stream->frameBufferSize;
			blockCount = stream->maxFrameBuffers + 2;
													// the plus 2 compensates for BAE 1.5
													// needing an extra buffer.
													// What's important is that the decodedBytes
													// be less than blockCount * blockBytes.
			XCloseMPEGStream(stream);
			decodedBytes = src.waveFrames * src.channels * sizeof(short);
			startFrame = 0;
		}

		*dst = XNewPtr(sizeof(XSndHeader3) + src.waveSize);
		if (!*dst)
		{
			XDisposePtr(intermediateData);
			return MEMORY_ERR;
		}

		snd = (XSndHeader3*)*dst;
		XPutShort(&snd->type, XThirdSoundFormat);
		XPutLong(&snd->sndBuffer.subType, dstCompression);
		XPutLong(&snd->sndBuffer.sampleRate, src.sampledRate);
		XPutLong(&snd->sndBuffer.frameCount, blockCount);	//MOE: This is a misuse of "frameCount" but we're stuck with it
		XPutLong(&snd->sndBuffer.blockBytes, blockBytes);	//MOE: This is only read by the old XGetSamplePtrFromSnd()
		XPutLong(&snd->sndBuffer.startFrame, startFrame * src.channels);
		XPutLong(&snd->sndBuffer.encodedBytes, src.waveSize);
		XPutLong(&snd->sndBuffer.decodedBytes, decodedBytes);
		XPutLong(&snd->sndBuffer.loopStart[0], src.startLoop);
		XPutLong(&snd->sndBuffer.loopEnd[0], src.endLoop);
		snd->sndBuffer.baseKey = (XBYTE)src.baseMidiPitch;
		snd->sndBuffer.channels = (XBYTE)src.channels;
		snd->sndBuffer.bitSize = 16;
		XBlockMove(src.theWaveform, snd->sndBuffer.sampleArea, src.waveSize);

		XDisposePtr(intermediateData);
		return err;
	}
#endif

	}	// switch

	XPutShort(&header->type, XFirstSoundFormat);
	XPutShort(&header->numModifiers, 1);
	XPutShort(&header->modNumber, 5);
	XPutLong(&header->modInit, 224);
	XPutShort(&header->numCommands, 1);
	XPutShort(&header->cmd, 0x8000 + bufferCmd);
	XPutShort(&header->param1, 0);
	XPutLong(&header->param2, sizeof(XSoundFormat1));

	XDisposePtr(intermediateData);
	return NO_ERR;
}

// Given a structure filled with no compression, create a AudioResource object.
// Will return NULL if out of memory, failed to get the right info from the
// SampleDataInfo structure, and if pPCMData is NULL.
// pSampleInfo->pMasterPtr is ignored, pPCMData points to the PCM data formated
// the way pSampleInfo describes
static AudioResource *XCreateAudioObjectFromData(XPTR pPCMData, SampleDataInfo *pSampleInfo)
{
	AudioResource	*pNewSampleObject;

	pNewSampleObject = NULL;
	if (pSampleInfo && pPCMData && pSampleInfo->size)
	{
		pNewSampleObject = (AudioResource *)XNewPtr(sizeof(AudioResource) + pSampleInfo->size);
		if (pNewSampleObject)
		{
			XPutLong(&pNewSampleObject->version, AUDIO_OBJECT_VERSION);
			XPutLong(&pNewSampleObject->dataLength, pSampleInfo->size);
			XPutLong(&pNewSampleObject->sampleFrames, pSampleInfo->frames);
			XPutLong(&pNewSampleObject->dataOffset, sizeof(AudioResource));
			XPutLong(&pNewSampleObject->audioType, AUDIO_SND);
			XPutLong(&pNewSampleObject->nameResourceType, ID_NULL);
			XPutLong(&pNewSampleObject->sampleRate, pSampleInfo->rate);
			XPutLong(&pNewSampleObject->loopStart, pSampleInfo->loopStart);
			XPutLong(&pNewSampleObject->loopEnd, pSampleInfo->loopEnd);
			XPutShort(&pNewSampleObject->baseMidiKey, pSampleInfo->baseKey);
			XPutShort(&pNewSampleObject->bitSize, pSampleInfo->channels);
			XPutShort(&pNewSampleObject->channels, pSampleInfo->bitSize);
			XBlockMove(pPCMData, (char *)(&pNewSampleObject->firstSampleFiller) + sizeof(long),
										pSampleInfo->size);
		}
	}
	else
	{
		BAE_ASSERT(FALSE);
	}
	return pNewSampleObject;
}

static XPTR XGetAudioObjectFromMemory(AudioResource *pAudioObject, SampleDataInfo *pInfo)
{
	XPTR				pSampleData;
	unsigned long		size;

	pSampleData = NULL;
	pInfo->size = 0;		// if left alone, then wrong type of resource
	pInfo->frames = 0;
	pInfo->rate = rate22khz;
	pInfo->loopStart = 0;
	pInfo->loopEnd = 0;
	pInfo->baseKey = kMiddleC;
	pInfo->bitSize = 8;
	pInfo->channels = 1;
	pInfo->compressionType = C_NONE;
	if (pAudioObject)
	{
		if (XGetLong(&pAudioObject->version) == AUDIO_OBJECT_VERSION)
		{
			switch (XGetLong(&pAudioObject->audioType))
			{
				case AUDIO_SND:
					size = XGetLong(&pAudioObject->dataLength);
					if (size)
					{
						pInfo->size = size;
						pInfo->frames = XGetLong(&pAudioObject->sampleFrames);
						pInfo->rate = XGetLong(&pAudioObject->sampleRate);
						pInfo->loopStart = XGetLong(&pAudioObject->loopStart);
						pInfo->loopEnd = XGetLong(&pAudioObject->loopEnd);
						pInfo->baseKey = XGetShort(&pAudioObject->baseMidiKey);
						pInfo->bitSize = XGetShort(&pAudioObject->bitSize);
						pInfo->channels = XGetShort(&pAudioObject->channels);
						pSampleData = XGetSamplePtrFromSnd((XPTR)((char *)(&pAudioObject->firstSampleFiller) + sizeof(long)), pInfo);
					}
					break;
			}
		}
	}
	return (XPTR)pSampleData;
}
#endif	// USE_CREATION_API

// Translate a GM_Waveform structure into a SampleDataInfo structure
void XTranslateFromWaveformToSampleData(GM_Waveform const* pSource, SampleDataInfo *pDest)
{
	if (pSource && pDest)
	{
		pDest->rate = pSource->sampledRate;
		pDest->frames = pSource->waveFrames;
		pDest->size = pSource->waveSize;
		pDest->loopStart = pSource->startLoop;
		pDest->loopEnd = pSource->endLoop;
		pDest->bitSize = pSource->bitSize;
		pDest->channels = pSource->channels;
		pDest->baseKey = pSource->baseMidiPitch;
		pDest->theID = (short)pSource->waveformID;
		pDest->compressionType = pSource->compressionType;
		pDest->pMasterPtr = pSource->theWaveform;
	}
	else
	{
		BAE_ASSERT(FALSE);
	}
}
// Translate a SampleDataInfo structure into a GM_Waveform structure
void XTranslateFromSampleDataToWaveform(SampleDataInfo const* pSource, GM_Waveform *pDest)
{
	if (pSource && pDest)
	{
		pDest->sampledRate = pSource->rate;
		pDest->waveFrames = pSource->frames;
		pDest->waveSize = pSource->size;
		pDest->startLoop = pSource->loopStart;
		pDest->endLoop = pSource->loopEnd;
		pDest->bitSize = (XBYTE)pSource->bitSize;
		pDest->channels = (XBYTE)pSource->channels;
		pDest->baseMidiPitch = (XBYTE)pSource->baseKey;
		pDest->waveformID = pSource->theID;
		pDest->compressionType = pSource->compressionType;
		pDest->theWaveform = (SBYTE *)pSource->pMasterPtr;
	}
	else
	{
		BAE_ASSERT(FALSE);
	}
}


/* EOF of SampleTools.c
*/

