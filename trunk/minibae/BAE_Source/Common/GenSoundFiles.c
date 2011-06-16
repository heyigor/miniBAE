/*****************************************************************************/
/*
**	GenSoundFiles.c
**
**	Reads AIFF, WAVE, MP3, Sun AU sound files
**
**	© Copyright 1989-2001 Beatnik, Inc, All Rights Reserved.
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
**	10/1/96		Created
**	10/13/96	Added GM_ReadIntoMemoryWaveFile & GM_ReadIntoMemoryAIFFFile
**	11/5/96		Changed WaveformInfo to GM_Waveform
**				Added more error checking
**				Added GM_FreeWaveform
**	12/30/96	Changed copyright
**	1/20/97		Added support for Sun AU files
**	1/23/97		More memory fail checking code
**	1/29/97		Changed PV_ConvertFromIeeeExtended to return a XFIXED
**				instead of a double
**	2/4/97		Fixed problem with PV_ReadSunAUFile. Didn't define the procptr
**	2/5/97		Added GM_ReadFileIntoMemoryFromMemory with the ability to parse a
**				memory mapped file as a fileType
**	3/19/97		Fixed a positioning and platform word swap bug 
**				with PV_ReadIntoMemorySunAUFile
**	3/29/97		Removed 4 character constants and changed to use macro FOUR_CHAR
**	5/3/97		Fixed a few potential problems that the MOT compiler found
**	5/15/97		Put in code for ADPCM decoding for WAVE files & AIFF files
**	6/2/97		Finished up GM_ReadAndDecodeFileStream to support AU streaming files
**	6/9/97		Added support for X_WAVE_FORMAT_ALAW & X_WAVE_FORMAT_MULAW for wave files
**	7/8/97		Fixed divide by zero when reading WAVE files.
**	7/25/97		Added PV_ConvertToIeeeExtended
**	11/10/97	Changed some preprocessor tests and flags to explicity test for flags rather
**				than assume
**	12/18/97	Cleaned up some warnings
**	3/20/98	MOE: Changed code that deals with XExpandAiffImaStream(),
**					which now accepts a short[2] parameter
**					PV_ReadAIFFAndDecompressIMA() and
**					PV_ReadIntoMemoryAIFFFile() were altered
**	3/20/98	MOE: Change name of call to XExpandWavIma()
**	3/23/98		Fixed warnings in PV_ReadAIFFAndDecompressIMA
**				Renamed PV_ConvertToIeeeExtended to XConvertToIeeeExtended and moved into
**				X_Formats.h. Renamed PV_ConvertFromIeeeExtended to XConvertFromIeeeExtended and
**				moved into X_Formats.h
**	5/5/98		Changed PV_ReadSunAUFile & PV_ReadAIFFAndDecompressIMA to return an OPErr.
**				Added OPErr parameter to PV_ReadIntoMemorySunAUFile & PV_ReadIntoMemoryAIFFFile &
**				PV_ReadAIFFAndDecompressIMA
**				Modified all the IFF_ functions to return OPErr's correctly
**	5/7/98		Changed GM_ReadFileInformation & GM_ReadFileIntoMemory & GM_ReadFileIntoMemoryFromMemory
**				to set an error code if failure
**	5/12/98		MOE: Changed PV_ReadAIFFAndDecompressIMA() to use short[] predictor
**				array rather than int[] index array.
**	5/14/98		Added support for loop points in WAV files. See IFF_GetWAVLoopPoints
**	5/21/98		Changed IFF_GetAIFFSampleSize to use AIFF_IMA_BLOCK_FRAMES constant instead of 64.
**				Changed all structures to be typedef structures
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
**	7/6/98		Fixed a compiler warning in PV_ReadWAVEAndDecompressIMA about testing for a negitive number
**				with an unsigned value (destLength < 0)
**	9/9/98		Added support for MPEG stream decode
**	9/10/98		Changed various routines that passed the file position in the GM_Waveform structure
**				in the waveformID element to now use the currentFilePosition element
**	9/14/98		Added PV_ReadIntoMemoryMPEGFile
**	2/12/99		Renamed USE_HAE_FOR_MPEG to USE_MPEG_DECODER
**	6/11/99		Added FILE_NOT_FOUND to PV_ReadIntoMemoryWaveFile & PV_ReadIntoMemoryMPEGFile &
**				PV_ReadIntoMemorySunAUFile & PV_ReadIntoMemoryAIFFFile
**	6/28/99		Fixed bug in PV_ReadIntoMemoryMPEGFile inwhich the file was opened, but not closed.
**	8/1/99		Changed structure XSampleLoop to define loops[] as loops[1]
**	8/3/99		Added the ability to write RAW wave files from a GM_Waveform. The infrastructure is
**				in place to add more types as required.
**	8/5/99		Fixed bug in PV_WriteFromMemoryWaveFile() affecting
**				8 bit data on Motorola-byte-order platforms. Fixed bug in IFF_WriteSize in which data
**				was not being written out in the correct order.
**	8/9/99		Moved error transfer in PV_ReadIntoMemoryAIFFFile to end of function.
**	8/28/99		Added a default to GM_WriteFileFromMemory
**	9/8/99		MOE: Changed several uses of XSwapShort() in loops to XSwapShorts()
**	9/8/99		MOE: Added decodeData parameter to GM_ReadFileIntoMemory(),
**				GM_ReadFileIntoMemoryFromMemory(), and related PV_ functions
**	10/21/99	MSD: Added PV_WriteFromMemoryAiffFile()
**	10/26/99	MSD: Added (SBYTE*) cast to wave->theWaveform in PV_ReadIntoMemoryMPEGFile()
**				to appease certain compilers (visual, CW Solaris)
**	10/28/99	MSD: Added PV_WriteFromMemorySunAUFile()
**	1/26/00		Changed PV_ReadIntoMemoryMPEGFile to close the mpeg stream after getting info
**				and setting variables up that are used for streaming.
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	2/14/2000	Fixed an endian bug in PV_ReadSunAUFile()
**	2/17/2000	Put creation wrappers around GM_WriteFileFromMemory and its associated code.
**	4/5/2000	Removed warnings in PV_ReadIntoMemoryWaveFile & PV_ReadIntoMemorySunAUFile
**	2000.04.24 msd	Added GM_FinalizeFileHeader() to support writing output to file
**	2000.04.25 msd	added GM_ReadRawAudioIntoMemoryFromMemory() to support MiniBAE API lego.
**	2000.04.27 msd	Fixed problem of not closing AU file after saving.
**					Added AU and AIFF support to writing output to file
**					Encapsulated writing mixer slices to audio files in GM_WriteAudioBufferToFile()
**	2000.05.08 msd	Fixed problem in writing AIFF header in GM_FinalizeFileHeader()
**	8/28/2000	sh	Changed the way PV_ReadIntoMemoryAIFFFile gets loop points and base pitch. Now
**					correctly follows the AIFF spec.
**	8/30/2000	sh	Fixed bug in IFF_GetAIFFMarkerValue that forgot to check for the number
**					of markers.
**	12/13/2000	sh	Added GM_CreateFileState & GM_DisposeFileState so that we can stream data
**					for AU files and preserve state between calls to GM_ReadAndDecodeFileStream.
**					Changed PV_ReadSunAUFile & PV_UnpackInput to handle passed in state. Eliminated
**					use of global variables.
**	12/29/2000	sh	Added IFF_WriteWAVLoopPoints, and modifed PV_WriteFromMemoryWaveFile to call 
**					IFF_WriteWAVLoopPoints when writing out WAV data.
**					Added IFF_WriteAIFFLoopPoints and modified PV_WriteFromMemoryAiffFile to call
**					it.
**				sh	Fixed two problems with AIF and the - 8 issue. 1) Turns out we were writing
**					an extra 8 bytes when writing out the X_SoundData chunk without increasing
**					the size of the block written, so it was wrong in length. The extra 8 bytes
**					are an offset and a alignment value. 2) we thinking wrongly about why we
**					need to subtract 8 when writing out the total lenght. The 8 is that case
**					is not the 8 for the samples, but for the FORM/TYPE header size.
**	1/3/2001	sh	Added GM_RepositionFileStream for support in repositioning a stream.
**	1/12/2001	sh	Modifed PV_ReadIntoMemorySunAUFile to set the GM_Waveform member waveFrames,
**					correctly.
**	1/23/2001	sh	inital pass as position code
**	1/25/2001	sh	fixed decoding of AU files, it was skipping frames when the size
**					of the decode was not the entire file.
**	1/26/2001	sh	Fixed a problem with uncompressed samples and GM_ReadAndDecodeFileStream.
**					We were reading past the end of the file and responding to and error
**					and ignoring the last buffer of data. This was for AIF, and WAV only.
**	2/2/2001	sh	Changed IFF_PutChunk & IFF_WriteBlock to return an OPErr. Just a type change.
**					Changed IFF_WriteAIFFLoopPoints to pass through error codes.
**	3/19/2001	sh	Fixed a few warnings.
**	5/20/2001	sh	New structure packing stuff, and now compiling more code even when
**					USE_HIGHLEVEL_FILE_API is FALSE
*/
/*****************************************************************************/

//#define USE_DEBUG	2

#include "X_API.h"
#include "X_Formats.h"
#include "X_Assert.h"
#include "GenSnd.h"

#if USE_HIGHLEVEL_FILE_API == TRUE
	#include <math.h>
	// The functions: 	XConvertFromIeeeExtended & XConvertToIeeeExtended
	//					rely on floating point. This needs to be changed in order to
	//					read AIF files.
#endif
#if USE_MPEG_DECODER != 0
	#include "XMPEG_BAE_API.h"
#endif

#define ODD(x)			((long)(x) & 1L)

#undef X_PACK_FAST
#include "X_PackStructures.h"

/**********************- WAVe Defines -**************************/
//
//  extended waveform format structure used for all non-PCM formats. this
//  structure is common to all non-PCM formats.
//
typedef struct
{
    XWORD		wFormatTag;			/* format type */
    XWORD		nChannels;			/* number of channels (i.e. mono, stereo...) */
    XDWORD		nSamplesPerSec;		/* sample rate */
    XDWORD		nAvgBytesPerSec;	/* for buffer estimation */
    XWORD		nBlockAlign;		/* block size of data */
    XWORD		wBitsPerSample;		/* number of bits per sample of mono data */
    XWORD		cbSize;				/* the count in bytes of the size of */
									/* extra information (after cbSize) */
} X_PACKBY1 XWaveHeader;
 

//
//  IMA endorsed ADPCM structure definitions--note that this is exactly
//  the same format as Intel's DVI ADPCM.
//
//      for WAVE_FORMAT_IMA_ADPCM   (0x0011)
//
//
typedef struct 
{
        XWaveHeader		wfx;
        XWORD			wSamplesPerBlock;
} X_PACKBY1 XWaveHeaderIMA;




typedef struct 
{
  XDWORD			dwIdentifier;		// a unique number (ie, different than the ID number of any other SampleLoop structure). This field may
										// correspond with the dwIdentifier field of some CuePoint stored in the Cue chunk. In other words, the CuePoint structure which has the
										// same ID number would be considered to be describing the same loop as this SampleLoop structure. Furthermore, this field corresponds to
										// the dwIndentifier field of any label stored in the Associated Data List. In other words, the text string (within some chunk in the Associated
										// Data List) which has the same ID number would be considered to be this loop's "name" or "label".

  XDWORD			dwType;				// the loop type (ie, how the loop plays back) as so:
										// 0 - Loop forward (normal)
										// 1 - Alternating loop (forward/backward)
										// 2 - Loop backward
										// 3-31 - reserved for future standard types
										// 32-? - sampler specific types (manufacturer defined)   

  XDWORD			dwStart;			// the startpoint of the loop. In sample frames
  XDWORD 			dwEnd;				// the endpoint of the loop. In sample frames
  XDWORD 			dwFraction;			// allows fine-tuning for loop fractional areas between samples. Values range from 0x00000000 to 0xFFFFFFFF. A
										// value of 0x80000000 represents 1/2 of a sample length.
  XDWORD			dwPlayCount;		// number of times to play the loop. A value of 0 specifies an infinite sustain loop (ie, the wave keeps looping
										// until some external force interrupts playback, such as the musician releasing the key that triggered that wave's playback).
} X_PACKBY1 XSampleLoop;

typedef struct 
{
  XDWORD			dwManufacturer;		// the MMA Manufacturer code for the intended sampler
  XDWORD			dwProduct;			// Product code (ie, model ID) of the intended sampler for the dwManufacturer.

  XDWORD			dwSamplePeriod;		// specifies the period of one sample in nanoseconds (normally 1/nSamplesPerSec from the Format chunk. But
										// note that this field allows finer tuning than nSamplesPerSec). For example, 44.1 KHz would be specified as 22675 (0x00005893).

  XDWORD			dwMIDIUnityNote;	// MIDI note number at which the instrument plays back the waveform data without pitch modification
										// (ie, at the same sample rate that was used when the waveform was created). This value ranges 0 through 127, inclusive. Middle C is 60

  XDWORD			dwMIDIPitchFraction;// specifies the fraction of a semitone up from the specified dwMIDIUnityNote. A value of 0x80000000 is
										// 1/2 semitone (50 cents); a value of 0x00000000 represents no fine tuning between semitones.

  XDWORD			dwSMPTEFormat;		// specifies the SMPTE time format used in the dwSMPTEOffset field. Possible values are:
										//	0  = no SMPTE offset (dwSMPTEOffset should also be 0)
										//	24 = 24 frames per second
										//	25 = 25 frames per second
										//	29 = 30 frames per second with frame dropping ('30 drop')
										//	30 = 30 frames per second       

  XDWORD			dwSMPTEOffset;		// specifies a time offset for the sample if it is to be syncronized or calibrated according to a start time other than
										// 0. The format of this value is 0xhhmmssff. hh is a signed Hours value [-23..23]. mm is an unsigned Minutes value [0..59]. ss is
										// unsigned Seconds value [0..59]. ff is an unsigned value [0..( - 1)]. 

  XDWORD			cSampleLoops;		// number (count) of SampleLoop structures that are appended to this chunk. These structures immediately
										// follow the cbSamplerData field. This field will be 0 if there are no SampleLoop structures.

  XDWORD			cbSamplerData;		// The cbSamplerData field specifies the size (in bytes) of any optional fields that an application wishes to append to this chunk.

  XSampleLoop		loops[1];
} X_PACKBY1 XSamplerChunk;

// WAVE form wFormatTag IDs

// supported 
enum 
{
	X_WAVE_FORMAT_PCM					=	0x0001,
	X_WAVE_FORMAT_ALAW					=	0x0006,	/*  Microsoft Corporation  */
	X_WAVE_FORMAT_MULAW					=	0x0007,	/*  Microsoft Corporation  */
	X_WAVE_FORMAT_DVI_ADPCM				=	0x0011,	/*  Intel Corporation  */
	X_WAVE_FORMAT_IMA_ADPCM				=	0x0011	/*  Intel Corporation  */
};

// not supported yet.
enum 
{
	X_WAVE_FORMAT_UNKNOWN				=	0x0000,	/*  Microsoft Corporation  */
	X_WAVE_FORMAT_ADPCM					=	0x0002,	/*  Microsoft Corporation  */
	X_WAVE_FORMAT_IBM_CVSD				=	0x0005,	/*  IBM Corporation  */
	X_WAVE_FORMAT_OKI_ADPCM				=	0x0010,	/*  OKI  */
	X_WAVE_FORMAT_MEDIASPACE_ADPCM		=	0x0012,	/*  Videologic  */
	X_WAVE_FORMAT_SIERRA_ADPCM			=	0x0013,	/*  Sierra Semiconductor Corp  */
	X_WAVE_FORMAT_G723_ADPCM			=	0x0014,	/*  Antex Electronics Corporation  */
	X_WAVE_FORMAT_DIGISTD				=	0x0015,	/*  DSP Solutions, Inc.  */
	X_WAVE_FORMAT_DIGIFIX				=	0x0016,	/*  DSP Solutions, Inc.  */
	X_WAVE_FORMAT_DIALOGIC_OKI_ADPCM	=	0x0017,	/*  Dialogic Corporation  */
	X_WAVE_FORMAT_YAMAHA_ADPCM			=	0x0020,	/*  Yamaha Corporation of America  */
	X_WAVE_FORMAT_SONARC				=	0x0021,	/*  Speech Compression  */
	X_WAVE_FORMAT_DSPGROUP_TRUESPEECH	=	0x0022,	/*  DSP Group, Inc  */
	X_WAVE_FORMAT_ECHOSC1				=	0x0023,	/*  Echo Speech Corporation  */
	X_WAVE_FORMAT_AUDIOFILE_AF36		=	0x0024,	/*    */
	X_WAVE_FORMAT_APTX					=	0x0025,	/*  Audio Processing Technology  */
	X_WAVE_FORMAT_AUDIOFILE_AF10		=	0x0026,	/*    */
	X_WAVE_FORMAT_DOLBY_AC2				=	0x0030,	/*  Dolby Laboratories  */
	X_WAVE_FORMAT_GSM610				=	0x0031,	/*  Microsoft Corporation  */
	X_WAVE_FORMAT_ANTEX_ADPCME			=	0x0033,	/*  Antex Electronics Corporation  */
	X_WAVE_FORMAT_CONTROL_RES_VQLPC		=	0x0034,	/*  Control Resources Limited  */
	X_WAVE_FORMAT_DIGIREAL				=	0x0035,	/*  DSP Solutions, Inc.  */
	X_WAVE_FORMAT_DIGIADPCM				=	0x0036,	/*  DSP Solutions, Inc.  */
	X_WAVE_FORMAT_CONTROL_RES_CR10		=	0x0037,	/*  Control Resources Limited  */
	X_WAVE_FORMAT_NMS_VBXADPCM			=	0x0038,	/*  Natural MicroSystems  */
	X_WAVE_FORMAT_CS_IMAADPCM			=	0x0039,	/* Crystal Semiconductor IMA ADPCM */
	X_WAVE_FORMAT_G721_ADPCM			=	0x0040,	/*  Antex Electronics Corporation  */
	X_WAVE_FORMAT_MPEG					=	0x0050,	/*  Microsoft Corporation  */
	X_WAVE_FORMAT_CREATIVE_ADPCM		=	0x0200,	/*  Creative Labs, Inc  */
	X_WAVE_FORMAT_CREATIVE_FASTSPEECH8	=	0x0202,	/*  Creative Labs, Inc  */
	X_WAVE_FORMAT_CREATIVE_FASTSPEECH10	=	0x0203,	/*  Creative Labs, Inc  */
	X_WAVE_FORMAT_FM_TOWNS_SND			=	0x0300,	/*  Fujitsu Corp.  */
	X_WAVE_FORMAT_OLIGSM				=	0x1000,	/*  Ing C. Olivetti & C., S.p.A.  */
	X_WAVE_FORMAT_OLIADPCM				=	0x1001,	/*  Ing C. Olivetti & C., S.p.A.  */
	X_WAVE_FORMAT_OLICELP				=	0x1002,	/*  Ing C. Olivetti & C., S.p.A.  */
	X_WAVE_FORMAT_OLISBC				=	0x1003,	/*  Ing C. Olivetti & C., S.p.A.  */
	X_WAVE_FORMAT_OLIOPR				=	0x1004	/*  Ing C. Olivetti & C., S.p.A.  */
};

enum 
{
	X_WAVE			= FOUR_CHAR('W','A','V','E'),		//		'WAVE'
	X_RIFF			= FOUR_CHAR('R','I','F','F'),		//		'RIFF'
	X_DATA			= FOUR_CHAR('d','a','t','a'),		//		'data'
	X_FMT			= FOUR_CHAR('f','m','t',' '),		//		'fmt '
	X_SMPL			= FOUR_CHAR('s','m','p','l')		//		'smpl'
};

/**********************- AIFF Defines -**************************/

enum 
{
	X_AIFF			= FOUR_CHAR('A','I','F','F'),		//		'AIFF'
	X_AIFC			= FOUR_CHAR('A','I','F','C'),		//		'AIFC'
	X_Common		= FOUR_CHAR('C','O','M','M'),		//		'COMM'
	X_SoundData		= FOUR_CHAR('S','S','N','D'),		//		'SSND'
	X_Marker		= FOUR_CHAR('M','A','R','K'),		//		'MARK'
	X_Instrument	= FOUR_CHAR('I','N','S','T'),		//		'INST'
	X_FORM			= FOUR_CHAR('F','O','R','M'),		//		'FORM'
	X_CAT			= FOUR_CHAR('C','A','T',' '),		//		'CAT '
	X_LIST			= FOUR_CHAR('L','I','S','T'),		//		'LIST'
	X_BODY			= FOUR_CHAR('B','O','D','Y')		//		'BODY'
};

typedef struct
{
	long	ckID;	   /* ID */
	long	ckSize;    /* size */
	long	ckData;
} X_PACKBY1 XIFFChunk;

typedef struct
{
	short int		numChannels;
	unsigned long	numSampleFrames;
	short int		sampleSize;
	unsigned char	sampleRate[10];
} X_PACKBY1 XAIFFHeader;

typedef struct
{
	short int		numChannels;
	unsigned long	numSampleFrames;
	short int		sampleSize;
	unsigned char	sampleRate[10];
	unsigned long	compressionType;
	char			compressionName[256];			/* variable length array, Pascal string */
} X_PACKBY1 XAIFFExtenedHeader;

typedef struct
{
	unsigned short	numMarkers;	// 2
	short int		id1;		// 0
	unsigned long	position1;
	char			name1[8];	// 076265674C6F6F70	'begLoop'
	short int		id2;		// 1
	unsigned long	position2;
	char			name2[8];	// 07656E644C6F6F70	'endLoop'
} X_PACKBY1 XSingleLoopMarker;

typedef struct
{
	unsigned char	baseFrequency;
	unsigned char	detune;
	unsigned char	lowFrequency;
	unsigned char	highFrequency;
	unsigned char	lowVelocity;
	unsigned char	highVelocity;
	short int		gain;

	short int		sustainLoop_playMode;
	short int		sustainLoop_beginLoop;
	short int		sustainLoop_endLoop;
	short int		releaseLoop_beginLoop;
	short int		releaseLoop_endLoop;
	short int		extra;
} X_PACKBY1 XInstrumentHeader;

typedef struct
{
	unsigned long	offset;
	unsigned long	blockSize;
} X_PACKBY1 XSoundData;

/**********************- AU Defines -**************************/
	
#include "g72x.h"

typedef struct 
{
	unsigned long magic;          // magic number 
	unsigned long hdr_size;       // size of the whole header, including optional comment.
	unsigned long data_size;      // optional data size - usually unusalble. 
	unsigned long encoding;       // format of data contained in this file 
	unsigned long sample_rate;    // sample rate of data in this file 
	unsigned long channels;       // numbder of interleaved channels (usually 1 or 2) 
} X_PACKBY1 SunAudioFileHeader;

typedef struct
{
	struct g72x_state	state;
	unsigned int		buffer;
	int					bits;
} X_PACKBY1 SunDecodeState;

// Note these are defined for big-endian architectures 
#define SUN_AUDIO_FILE_MAGIC_NUMBER (0x2E736E64L)		// '.snd'

// Define the encoding fields 
#define	SUN_AUDIO_FILE_ENCODING_MULAW_8			(1)		// 8-bit ISDN u-law 
#define	SUN_AUDIO_FILE_ENCODING_LINEAR_8		(2)		// 8-bit linear PCM 
#define	SUN_AUDIO_FILE_ENCODING_LINEAR_16		(3)		// 16-bit linear PCM 
#define	SUN_AUDIO_FILE_ENCODING_LINEAR_24		(4)		// 24-bit linear PCM 
#define	SUN_AUDIO_FILE_ENCODING_LINEAR_32		(5)		// 32-bit linear PCM 
#define	SUN_AUDIO_FILE_ENCODING_FLOAT			(6)		// 32-bit IEEE floating point 
#define	SUN_AUDIO_FILE_ENCODING_DOUBLE			(7)		// 64-bit IEEE floating point 
#define	SUN_AUDIO_FILE_ENCODING_ADPCM_G721		(23)	// 4-bit CCITT g.721 ADPCM 
#define	SUN_AUDIO_FILE_ENCODING_ADPCM_G722		(24)	// CCITT g.722 ADPCM 
#define	SUN_AUDIO_FILE_ENCODING_ADPCM_G723_3	(25)	// CCITT g.723 3-bit ADPCM 
#define	SUN_AUDIO_FILE_ENCODING_ADPCM_G723_5	(26)	// CCITT g.723 5-bit ADPCM 
#define SUN_AUDIO_FILE_ENCODING_ALAW_8			(27) 	// 8-bit ISDN A-law 

#include "X_UnpackStructures.h"

// internal structures
typedef struct
{
	long			formType;				//	either X_RIFF, or X_FORM;
	long			headerType;
	long			formPosition;			//	position in file of current FORM
	long			formLength;				//	length of FORM
	OPErr			lastError;
	XFILE			fileReference;
} X_IFF;

#if USE_HIGHLEVEL_FILE_API == TRUE

// IFF functions

#if 0
	#pragma mark ## IFF/RIFF read and general scan code ##
#endif

static OPErr IFF_Error(X_IFF *pIFF)
{
	if (pIFF)
	{
		return pIFF->lastError;
	}
	return NO_ERR;
}

static void IFF_SetFormType(X_IFF *pIFF, long formType)
{
	if (pIFF)
	{
		pIFF->formType = formType;
	}
}


/* -1 is error */
static long IFF_GetNextGroup(X_IFF *pIFF, XIFFChunk *pChunk)
{
	long	err = 0;

	if (XFileRead(pIFF->fileReference, pChunk, (long)sizeof(XIFFChunk) - sizeof(long)) != -1)	// get chunk ID
	{
		if (pIFF->formType == X_RIFF)
		{
			#if X_WORD_ORDER == FALSE	// motorola?
			pChunk->ckSize = XSwapLong(pChunk->ckSize);
			#endif
		}
		else
		{
			pChunk->ckSize = XGetLong(&pChunk->ckSize);
		}
		pChunk->ckData = 0L;
		pChunk->ckID = XGetLong(&pChunk->ckID);		// swap if not motorola
		switch(pChunk->ckID)
		{
			case X_FORM:
			case X_RIFF:
			case X_LIST:
			case X_CAT:
				pIFF->formPosition = XFileGetPosition(pIFF->fileReference);	 /* get current pos */
				pIFF->formLength = pChunk->ckSize;

				if (XFileRead(pIFF->fileReference, &pChunk->ckData, (long)sizeof(long)) == -1)
				{
					pIFF->lastError = BAD_FILE;
				}
				pChunk->ckData = XGetLong(&pChunk->ckData);
				break;
		}
	}
	else
	{
		pIFF->lastError = BAD_FILE;
		err = -1;
	}
	return err;
}


/******- Determine if a file is a IFF type -******************************/

static long IFF_FileType(X_IFF *pIFF)
{
	XIFFChunk type;

	if (pIFF)
	{
		XFileSetPosition(pIFF->fileReference, 0L);	/* set to begining of file */

		pIFF->formPosition = 0;
		pIFF->formLength = 0;
		XSetMemory(&type, (long)sizeof(XIFFChunk), 0);
		IFF_GetNextGroup(pIFF, &type);
		return( (type.ckID == pIFF->formType) ? type.ckData : -1L);
	}
	return -1L;
}

/*- scan past nested FORM's -*/
static long IFF_NextBlock(X_IFF *pIFF, long blockID)
{
	long saveFORM, saveFLEN;
	XIFFChunk type;
	long flag;

	flag = -1;
	while (XFileGetPosition(pIFF->fileReference) < (pIFF->formPosition + pIFF->formLength))
	{
		 saveFORM = pIFF->formPosition;
		 saveFLEN = pIFF->formLength;
		 if (IFF_GetNextGroup(pIFF, &type) == -1)	/* error? */
		 {
			break;
		 }
		 if (type.ckID == pIFF->formType)
		 {
			type.ckSize -= 4L;
		 }
		 pIFF->formPosition = saveFORM;
		 pIFF->formLength = saveFLEN;
		 if (type.ckID != blockID)
		 {
		 	if (XFileSetPositionRelative(pIFF->fileReference, type.ckSize + (type.ckSize&1)))
	 		{
	  			  flag = -1;  /* error */
	  			  break;
			}
		 }
		 else
		 {
	 		flag = 0;	/* ok, found block ID */
			break;
		 }
	}
	if (flag == -1)
	{
		pIFF->lastError = BAD_FILE;
	}
	return flag;
}


/******- Scan a FORM for a 4 letter block -*******************************/
static long IFF_ScanToBlock(X_IFF *pIFF, long block)
{
	if (XFileSetPosition(pIFF->fileReference, pIFF->formPosition + 4) == 0)		// set to inside of FORM
 	{
		return IFF_NextBlock(pIFF, block);
	}
	else
	{
		pIFF->lastError = BAD_FILE;
		return -1;
	}
}

/******************- Return Chunk size -**********************************/
static long IFF_ChunkSize(X_IFF *pIFF, long block)
{
	long size;

	// must back up. Variable length change based upon block ID
	if (IFF_ScanToBlock(pIFF, block) == -1L)
	{
		return -1;	// bad
	}
	XFileSetPositionRelative(pIFF->fileReference, -4L);		// back-up and get size
	if (XFileRead(pIFF->fileReference, &size, 4L) == -1)
	{
		pIFF->lastError = BAD_FILE;
		return -1;
	}
	if (pIFF->formType == X_RIFF)
	{
		#if X_WORD_ORDER == FALSE	// motorola?
		size = XSwapLong(size);
		#endif
	}
	return size;
}


/*- go inside a FORM that has been found -*/
static long IFF_NextForm(X_IFF *pIFF)
{
	XIFFChunk type;

	XFileSetPosition(pIFF->fileReference, pIFF->formPosition);	// set to inside of FORM
	
	if (IFF_GetNextGroup(pIFF, &type) != -1)
	{
		if (type.ckID == pIFF->formType)
		{
			return type.ckData;
		}
	}
	pIFF->lastError =  BAD_FILE;
	return -1;
}

static long IFF_CurrentForm(X_IFF *pIFF)
{
	pIFF->formPosition = XFileGetPosition(pIFF->fileReference);
	return IFF_NextForm(pIFF);
}

static long IFF_ReadBlock(X_IFF *pIFF, XPTR pData, long Length)
{
	if (XFileRead(pIFF->fileReference, pData, Length) == -1)
	{
		pIFF->lastError = BAD_FILE;
	}
	return Length;
}


static long IFF_GetChunk(X_IFF *pIFF, long block, long size, XPTR p)
{
	if (IFF_ScanToBlock(pIFF, block) == -1L)
	{
		 return(-1); /* bad */
	}
	if (size == -1L)	/* size not known? */
	{
		XFileSetPositionRelative(pIFF->fileReference, -4L);		// back-up and get size
		if (XFileRead(pIFF->fileReference, &size, (long)sizeof(long)) == -1)
		{
			pIFF->lastError = BAD_FILE;
		}
		if (pIFF->formType == X_RIFF)
		{
			#if X_WORD_ORDER == FALSE	// motorola?
			size = XSwapLong(size);
			#endif
		}
		else
		{
			size = XGetLong(&size);
		}
	}

	IFF_ReadBlock(pIFF, p, size);	/* read block */
	pIFF->lastError = NO_ERR;
	if (size&1) /* odd? */
	{
		XFileSetPositionRelative(pIFF->fileReference, 1L);		// skip one byte
	}
	return pIFF->lastError;
}


static long IFF_NextChunk(X_IFF *pIFF, long block, long size, XPTR p)
{
	if (IFF_NextBlock(pIFF, block) == -1)
	{
		 return(-1); /* bad */
	}
	if (size == -1L)	/* size not known? */
	{
		XFileSetPositionRelative(pIFF->fileReference, -4L);		// back-up and get size
		if (XFileRead(pIFF->fileReference, &size, (long)sizeof(long)) == -1)
		{
			pIFF->lastError = BAD_FILE;
		}
		if (pIFF->formType == X_RIFF)
		{
			#if X_WORD_ORDER == FALSE	// motorola?
			size = XSwapLong(size);
			#endif
		}
		else
		{
			size = XGetLong(&size);
		}
	}
	IFF_ReadBlock(pIFF, p, size);	/* read block */
	if (size&1) /* odd? */
	{
		XFileSetPositionRelative(pIFF->fileReference, 1L);		// skip one byte
	}
	return pIFF->lastError;
}

#if 0
	#pragma mark ## IFF/RIFF write code ##
#endif

#if USE_CREATION_API == TRUE
static void IFF_WriteType(X_IFF *pIFF, unsigned long type)
{
	unsigned long	theType;

	theType = type;
	XPutLong(&theType, type);		// makes sure its motorola order
	XFileWrite(pIFF->fileReference, &theType, sizeof(long));
}

static OPErr IFF_WriteBlock(X_IFF *pIFF, XPTR pData, unsigned long Length)
{
	pIFF->lastError = NO_ERR;
	if (XFileWrite(pIFF->fileReference, pData, (long)Length) == -1)
	{
		pIFF->lastError = BAD_FILE;
	}
	return pIFF->lastError;
}

// write size of block, but order in the format particulars
static long IFF_WriteSize(X_IFF *pIFF, unsigned long size)
{
	unsigned long theSize;

	theSize = size;
	if (pIFF->formType == X_RIFF)
	{
		#if X_WORD_ORDER == FALSE	// motorola?
		theSize = XSwapLong(size);
		#endif
	}
	else if (pIFF->formType == X_FORM)
	{
		#if X_WORD_ORDER == TRUE
		theSize = XSwapLong(size);
		#endif
	}
	else
	{
		XPutLong(&theSize, size);		// makes sure its motorola order
	}
	return IFF_WriteBlock(pIFF, &theSize, (long)sizeof(unsigned long));
}
#endif

#if USE_CREATION_API == TRUE
static OPErr IFF_PutChunk(X_IFF *pIFF, long block, long unsigned size, XPTR p)
{
	OPErr err;

	pIFF->lastError = NO_ERR;
	IFF_WriteType(pIFF, block);

	// SSND chunk in AIFF requires 8 bytes between size and sample data.
	if (pIFF->formType == X_FORM && block == X_SoundData)
	{
		long tmp = 0;

		IFF_WriteSize(pIFF, size + 8);
		IFF_WriteBlock(pIFF, (XPTR)&tmp, sizeof(long));		// offset to first sample
		IFF_WriteBlock(pIFF, (XPTR)&tmp, sizeof(long));		// alignment block
	}
	else
	{
		IFF_WriteSize(pIFF, size);
	}
 	err = IFF_WriteBlock(pIFF, p, size);
	if (err == NO_ERR)
	{
		 if (ODD(size)) /* is it odd? */
		 {
			size = 0L;
			XFileWrite(pIFF->fileReference, &size, (long)sizeof(char));
		}
	}
	return pIFF->lastError;
}
#endif

// Sun AU file code
#if 0
	#pragma mark ## Sun AU file code ##
#endif

// Unpack input codes and pass them back as bytes. Returns 1 if there is residual input, returns -1 if eof, else returns 0.
static int PV_UnpackInput(XFILE fileReference, unsigned int *in_buffer, int *in_bits, 
									unsigned char *code, int bits)
{
	unsigned char		in_byte;

	if (*in_bits < bits) 
	{
		if (XFileRead(fileReference, &in_byte, 1L) == -1)
		{
			*code = 0;
			return -1;
		}
		*in_buffer |= (in_byte << *in_bits);
		*in_bits += 8;
	}
	*code = *in_buffer & ((1 << bits) - 1);
	*in_buffer >>= bits;
	*in_bits -= bits;
	return (*in_bits > 0);
}

#define MAX_AU_DECODE_BLOCK_SIZE		1024L
// decode a sun file. If pState is non-zero then this is used between calls, otherwise it
// uses clean state on the stack.
static OPErr PV_ReadSunAUFile(	long encoding,
								XFILE fileReference, 
								void *pSample,
								unsigned long sampleByteLength,
								unsigned long *pBufferLength,
								SunDecodeState *pState
								)
{
	OPErr				err;
	short				sample;
	unsigned char		code;
	int					count;
	int					(*dec_routine)(int i, int out_coding, struct g72x_state *state_ptr);
	int					dec_bits;
	short int			*pSample16;
	unsigned char		*pSample8;
	unsigned char		codeBlock[MAX_AU_DECODE_BLOCK_SIZE];
	unsigned long		writeLength;
	SunDecodeState		state;

	writeLength = 0;
	err = NO_ERR;
	dec_bits = 0;
	pSample8 = (unsigned char *)pSample;
	pSample16 = (short int *)pSample;
	switch (encoding)
	{
		case SUN_AUDIO_FILE_ENCODING_LINEAR_16:
			writeLength = sampleByteLength;
			XFileRead(fileReference, pSample, writeLength);

			#if X_WORD_ORDER != FALSE	// intel?
				XSwapShorts(pSample16, writeLength/2);
			#endif

			break;
		case SUN_AUDIO_FILE_ENCODING_LINEAR_8:
			writeLength = sampleByteLength;
			XFileRead(fileReference, pSample, writeLength);
			break;
		case SUN_AUDIO_FILE_ENCODING_MULAW_8:
#if 0	// slow
			while (sampleByteLength > 0)
			{
				XFileRead(fileReference, codeBlock, 1);
				*pSample16++ = ulaw2linear(codeBlock[0]);
				writeLength += 2;
				sampleByteLength -= 2;
			}
#else
// fast				
			sampleByteLength = sampleByteLength / (MAX_AU_DECODE_BLOCK_SIZE*2);
			while (sampleByteLength > 0)
			{
				XFileRead(fileReference, codeBlock, MAX_AU_DECODE_BLOCK_SIZE);
				for (count = 0; count < MAX_AU_DECODE_BLOCK_SIZE; count++)
				{
					*pSample16++ = ulaw2linear(codeBlock[count]);
					writeLength += 2;
				}
				sampleByteLength--;
			}
#endif
			break;
		case SUN_AUDIO_FILE_ENCODING_ALAW_8:
			sampleByteLength = sampleByteLength / (MAX_AU_DECODE_BLOCK_SIZE*2);
			while (sampleByteLength > 0)
			{
				XFileRead(fileReference, codeBlock, MAX_AU_DECODE_BLOCK_SIZE);
				for (count = 0; count < MAX_AU_DECODE_BLOCK_SIZE; count++)
				{
					*pSample16++ = alaw2linear(codeBlock[count]);
					writeLength += 2;
				}
				sampleByteLength--;
			}
			break;

		case SUN_AUDIO_FILE_ENCODING_ADPCM_G721:
			dec_routine = g721_decoder;
			dec_bits = 4;
			goto decode_adpcm;
		case SUN_AUDIO_FILE_ENCODING_ADPCM_G723_3:
			dec_routine = g723_24_decoder;
			dec_bits = 3;
			goto decode_adpcm;
		case SUN_AUDIO_FILE_ENCODING_ADPCM_G723_5:
			dec_routine = g723_40_decoder;
			dec_bits = 5;

decode_adpcm:
			if (pState == NULL)
			{	// a one time call, so create a new state
				g72x_init_state(&state.state);
				state.bits = 0;
				state.buffer = 0;
				pState = &state;
			}
			/* Read and unpack input codes and process them */
			while (PV_UnpackInput(fileReference, &pState->buffer, &pState->bits, &code, dec_bits) >= 0) 
			{
				sample = (*dec_routine)(code, AUDIO_ENCODING_LINEAR, &pState->state);
				*pSample16++ = sample;
				writeLength += sizeof(short);
				if (sampleByteLength > 0)
				{
					sampleByteLength -= sizeof(short);
				}
				else
				{
					break;
				}
			}
			break;
	}
	if (pBufferLength)
	{
		*pBufferLength = writeLength;
	}
	return err;
}






// WAVE functions
#if 0
	#pragma mark ## WAVE read functions ##
#endif

static long IFF_GetWAVFormatTag(X_IFF *pIFF)
{
	long		theErr;
	XWaveHeader	header;

	theErr = IFF_GetChunk(pIFF, X_FMT, (long)sizeof(XWaveHeader), (void *)&header);

	#if X_WORD_ORDER == FALSE	// motorola?
		header.wFormatTag = XSwapShort(header.wFormatTag);
	#endif
	return (long)header.wFormatTag;
}

static long IFF_GetWAVHeader(X_IFF *pIFF, XWaveHeader * pHeaderInfo)
{
	long	theErr;

	theErr = IFF_GetChunk(pIFF, X_FMT, (long)sizeof(XWaveHeader), (void *)pHeaderInfo);

	#if X_WORD_ORDER == FALSE	// motorola?
		pHeaderInfo->nSamplesPerSec = XSwapLong(pHeaderInfo->nSamplesPerSec);
		pHeaderInfo->nAvgBytesPerSec = XSwapLong(pHeaderInfo->nAvgBytesPerSec);
		pHeaderInfo->wFormatTag = XSwapShort(pHeaderInfo->wFormatTag);
		pHeaderInfo->nChannels = XSwapShort(pHeaderInfo->nChannels);
		pHeaderInfo->nBlockAlign = XSwapShort(pHeaderInfo->nBlockAlign);
		pHeaderInfo->wBitsPerSample = XSwapShort(pHeaderInfo->wBitsPerSample);
		pHeaderInfo->cbSize = XSwapShort(pHeaderInfo->cbSize);
	#endif
	return theErr;
}

static long IFF_GetWAVIMAHeader(X_IFF *pIFF, XWaveHeaderIMA * pHeaderInfo)
{
	long	theErr;

	theErr = IFF_GetChunk(pIFF, X_FMT, (long)sizeof(XWaveHeaderIMA), (void *)pHeaderInfo);

	#if X_WORD_ORDER == FALSE	// motorola?
		pHeaderInfo->wfx.nSamplesPerSec = XSwapLong(pHeaderInfo->wfx.nSamplesPerSec);
		pHeaderInfo->wfx.nAvgBytesPerSec = XSwapLong(pHeaderInfo->wfx.nAvgBytesPerSec);
		pHeaderInfo->wfx.wFormatTag = XSwapShort(pHeaderInfo->wfx.wFormatTag);
		pHeaderInfo->wfx.nChannels = XSwapShort(pHeaderInfo->wfx.nChannels);
		pHeaderInfo->wfx.nBlockAlign = XSwapShort(pHeaderInfo->wfx.nBlockAlign);
		pHeaderInfo->wfx.wBitsPerSample = XSwapShort(pHeaderInfo->wfx.wBitsPerSample);
		pHeaderInfo->wfx.cbSize = XSwapShort(pHeaderInfo->wfx.cbSize);
		pHeaderInfo->wSamplesPerBlock = XSwapShort(pHeaderInfo->wSamplesPerBlock);
	#endif
	return theErr;
}

// Get compressed and uncompressed size. Return 0 if successful, 1 if failure
static long IFF_GetWAVSampleSize(X_IFF *pIFF, unsigned long *pUncompressedSize, unsigned long *pCompressedSize)
{
	long			size, error;
	XWaveHeaderIMA	header;

	error = 0;
	size = 0;
	if ( (IFF_GetWAVIMAHeader(pIFF, &header) == 0) && pUncompressedSize && pCompressedSize)
	{
		if (IFF_ScanToBlock(pIFF, X_DATA) == 0)	/* skip to body */
		{
			XFileSetPositionRelative(pIFF->fileReference, -4L);		// back-up and get size
			if (XFileRead(pIFF->fileReference, &size, (long)sizeof(long)) == -1)
			{
				pIFF->lastError = BAD_FILE;
			}
			if (pIFF->formType == X_RIFF)
			{
				#if X_WORD_ORDER == FALSE	// motorola?
					size = XSwapLong(size);
				#endif
			}
			*pCompressedSize = size;
			switch(header.wfx.wFormatTag)
			{
				default:
					size = 0;
					error = 1;
					break;
				case X_WAVE_FORMAT_ALAW:
				case X_WAVE_FORMAT_MULAW:
					size *= 2;	// double size
					break;
				case X_WAVE_FORMAT_PCM:
					// size will be valid
					break;
				// calculate the uncompressed byte size
				case X_WAVE_FORMAT_IMA_ADPCM:
					size *= 4;
/*
					{
					    unsigned long	cBlocks;
					    unsigned long	cbBytesPerBlock;

		                //
		                //  how many destination PCM bytes are needed to hold
		                //  the decoded ADPCM data
		                //
		                //  always round UP
		                //
		                cBlocks = size / header.wfx.nBlockAlign;
		                if (cBlocks)
		                {
			                cbBytesPerBlock = header.wSamplesPerBlock * 2; // dest block aline

			                if ( ! ((0xFFFFFFFFL / cbBytesPerBlock) < cBlocks) )
			                {
				                if ((size % header.wfx.nBlockAlign) == 0)
				                {
				                    size = cBlocks * cbBytesPerBlock;
				                }
				                else
				                {
				                    size = (cBlocks + 1) * cbBytesPerBlock;
			    	            }
							}
							else
							{
								size = 0;	// out of range
								error = 1;
							}
						}
						else
						{
							size = 0;	// out of range
							error = 1;
						}
					}
*/
					break;
			}
			*pUncompressedSize = size;
		}
	}
	else
	{
		error = 1;
	}
	return error;
}

#if USE_CREATION_API == TRUE
// Create and write out loop points if there's any in the waveform.
static OPErr IFF_WriteWAVLoopPoints(X_IFF *pIFF, GM_Waveform const* pWaveform)
{
	XSamplerChunk	sample;

	if (pWaveform)
	{
		if (pWaveform->startLoop && pWaveform->endLoop)
		{
			XSetMemory(&sample, (long)sizeof(XSamplerChunk), 0);

			// this rather complex process in setting these values makes sure the values in the structure are intel ordered, reguardless
			// of platform we're on. The structure pWaveform is always host ordered.
			XPutLong(&sample.cSampleLoops, XSwapLong(1));	// one loop point

			XPutLong(&sample.dwMIDIUnityNote, XSwapLong(pWaveform->baseMidiPitch));	// MIDI note number

			XPutLong(&sample.loops[0].dwStart, XSwapLong(pWaveform->startLoop));	// the startpoint of the loop. In sample frames
			XPutLong(&sample.loops[0].dwEnd, XSwapLong(pWaveform->endLoop));		// the endpoint of the loop. In sample frames
			XPutLong(&sample.loops[0].dwPlayCount, XSwapLong(pWaveform->numLoops));

			// write WAVE loop block
			return IFF_PutChunk(pIFF, X_SMPL, (long)sizeof(XSamplerChunk), (XPTR)&sample);
		}
	}
	return NO_ERR;
}
#endif

// Returns WAV loop points, if there. Return 0 if successful, -1 if failure
static long IFF_GetWAVLoopPoints(X_IFF *pIFF, unsigned long *pLoopStart, unsigned long *pLoopEnd, unsigned long *pLoopCount)
{
	XSamplerChunk	*pSampler;
	long			theErr;
	unsigned long	size;

	*pLoopStart = 0;
	*pLoopEnd = 0;
	theErr = 0;
	if (IFF_ScanToBlock(pIFF, X_SMPL) == 0)	/* skip to body */
	{
		XFileSetPositionRelative(pIFF->fileReference, -4L);		// back-up and get size
		if (XFileRead(pIFF->fileReference, &size, (long)sizeof(long)) == -1)
		{
			pIFF->lastError = BAD_FILE;
			theErr = -1;
		}
		if (pIFF->formType == X_RIFF)
		{
			#if X_WORD_ORDER == FALSE	// motorola?
				size = XSwapLong(size);
			#endif
		}
		if (theErr == 0)
		{
			pSampler = (XSamplerChunk *)XNewPtr(size);
			if (pSampler)
			{
				if (XFileRead(pIFF->fileReference, pSampler, size) == -1)
				{
					pIFF->lastError = BAD_FILE;
					theErr = -1;
				}
				else
				{	// ok
					#if X_WORD_ORDER == FALSE	// motorola?
					if (XSwapLong(pSampler->cSampleLoops))
					#else
					if (pSampler->cSampleLoops)
					#endif
					{
						#if X_WORD_ORDER == FALSE	// motorola?
						if (XSwapLong(pSampler->loops[0].dwType) == 0)
						#else
						if (pSampler->loops[0].dwType == 0)
						#endif
						{
							// WAV loop points are in bytes not in frames
							#if X_WORD_ORDER == FALSE	// motorola?
							*pLoopStart = XSwapLong(pSampler->loops[0].dwStart);
							*pLoopEnd = XSwapLong(pSampler->loops[0].dwEnd);
							#else
							*pLoopStart = pSampler->loops[0].dwStart;
							*pLoopEnd = pSampler->loops[0].dwEnd;
							#endif
							#if X_WORD_ORDER == FALSE	// motorola?
							*pLoopCount = XSwapLong(pSampler->loops[0].dwPlayCount);
							#else
							*pLoopCount = pSampler->loops[0].dwPlayCount;
							#endif
						}
					}
				}
			}
			XDisposePtr(pSampler);
		}
		
	}
	return theErr;
}


// return NO_ERR if successfull
static OPErr PV_ReadWAVEAndDecompressIMA(XFILE fileReference, unsigned long sourceLength,
											char *pDestSample, unsigned long destLength,
											char outputBitSize, char channels,
											XPTR pBlockBuffer, unsigned long blockSize,
											unsigned long *pBufferLength)
{
	unsigned long			writeBufferLength, size, offset;
	OPErr					error;
	XBOOL					customBlockBuffer;
	
	error = MEMORY_ERR;
	writeBufferLength = 0;
	customBlockBuffer = FALSE;
	if (pBlockBuffer == NULL)
	{
		pBlockBuffer = XNewPtr(blockSize);
		customBlockBuffer = TRUE;
	}
	if (pBlockBuffer)
	{
		error = NO_ERR;
		while (sourceLength > 0)
		{
			if (sourceLength >= blockSize)
			{
				size = XFileGetPosition(fileReference);	 /* get current pos */
				if (XFileRead(fileReference, pBlockBuffer, blockSize) == -1)
				{
					error = BAD_FILE;
				}
				size = XFileGetPosition(fileReference) - size;
			}
			else
			{
				// last block, just stop
				size = 0;
			}
			offset = 0;
			if (size)
			{
				offset = XExpandWavIma((XBYTE const*)pBlockBuffer, blockSize,
										pDestSample, outputBitSize,
										size, channels);
			}
			if (offset == 0)
			{
				// we're done
				break;
			}
			if (destLength < offset)	// done filling?
			{
				break;
			}
			sourceLength -= blockSize;
			destLength -= offset;
			pDestSample += offset;
			writeBufferLength += offset;
		}
	}
	if (customBlockBuffer)
	{
		XDisposePtr(pBlockBuffer);
	}
	if (pBufferLength)
	{
		*pBufferLength = writeBufferLength;
	}
	return error;
}


// This will read into memory the entire wave file and return a GM_Waveform structure.
// When disposing make sure and dispose of both the GM_Waveform structure and the
// theWaveform inside of that structure with XDisposePtr
static GM_Waveform* PV_ReadIntoMemoryWaveFile(XFILE file, XBOOL decodeData,
												long *pFormat, unsigned long *pBlockSize,
												OPErr *pError)
{
	GM_Waveform			*wave;
	X_IFF				*pIFF;

	BAE_ASSERT(file);
	BAE_ASSERT(pError);
	wave = NULL;

	pIFF = (X_IFF*)XNewPtr(sizeof(X_IFF));
	if (pIFF)
	{
		IFF_SetFormType(pIFF, X_RIFF);
		pIFF->fileReference = file;

		wave = (GM_Waveform*)XNewPtr(sizeof(GM_Waveform));
		if (wave)
		{
		XWaveHeaderIMA		waveHeader;
		unsigned long		size, sourceLength;

			if ((IFF_FileType(pIFF) == X_WAVE) &&
				(IFF_GetWAVIMAHeader(pIFF, &waveHeader) == 0))
			{
				wave->channels = (UBYTE)waveHeader.wfx.nChannels;
				wave->bitSize = (UBYTE)waveHeader.wfx.wBitsPerSample;
				wave->sampledRate = waveHeader.wfx.nSamplesPerSec << 16L;
				wave->baseMidiPitch = 60;
				wave->compressionType = C_NONE;

				IFF_GetWAVLoopPoints(pIFF, &wave->startLoop, &wave->endLoop, &wave->numLoops);

				// we want the byte size
				size = 0;
				if (IFF_GetWAVSampleSize(pIFF, &size, &sourceLength) != 0)
				{
					BAE_ASSERT(FALSE);
					pIFF->lastError = BAD_FILE_TYPE;
				}
				else
				{
					switch (waveHeader.wfx.wFormatTag)
					{
					case X_WAVE_FORMAT_IMA_ADPCM:
						wave->bitSize = 16;
						break;
					case X_WAVE_FORMAT_ALAW:
					case X_WAVE_FORMAT_MULAW:
						wave->bitSize = 16;
						break;
					}

					// file is positioned at the sample data

					wave->waveSize = size;
					wave->waveFrames = size / (wave->channels * (wave->bitSize / 8));

					if (pBlockSize)
					{
						wave->currentFilePosition = XFileGetPosition(pIFF->fileReference);
						BAE_ASSERT(pFormat);
						*pFormat = waveHeader.wfx.wFormatTag;
						*pBlockSize = waveHeader.wfx.nBlockAlign;
						// don't read/decode any data, streaming code will do it later
					}
					else	// if (decodeData) // for now
					{
						wave->theWaveform = (SBYTE *)XNewPtr(size);
						if (wave->theWaveform)
						{
							switch(waveHeader.wfx.wFormatTag)
							{
							case X_WAVE_FORMAT_ALAW:
								pIFF->lastError =
									PV_ReadSunAUFile(SUN_AUDIO_FILE_ENCODING_ALAW_8,
														pIFF->fileReference, 
														wave->theWaveform,
														wave->waveSize,
														&size,
														NULL);
								break;
							case X_WAVE_FORMAT_MULAW:
								pIFF->lastError =
									PV_ReadSunAUFile(SUN_AUDIO_FILE_ENCODING_MULAW_8,
														pIFF->fileReference, 
														wave->theWaveform,
														wave->waveSize,
														&size, 
														NULL);
								break;
							case X_WAVE_FORMAT_PCM:
								pIFF->lastError = NO_ERR;
								if (XFileRead(pIFF->fileReference, 
												wave->theWaveform,
												wave->waveSize) != -1)
								{
// now, if the file is 16 bit sample on a motorola ordered system, swap the bytes
#if X_WORD_ORDER == FALSE
									if (wave->bitSize == 16)
									{
										XSwapShorts((short*)wave->theWaveform,
													wave->waveFrames * wave->channels);
									}
#endif
								}
								else
								{
									BAE_ASSERT(FALSE);
									pIFF->lastError = BAD_FILE;
								}
								break;
							case X_WAVE_FORMAT_IMA_ADPCM:
								pIFF->lastError =
									PV_ReadWAVEAndDecompressIMA(pIFF->fileReference, 
																sourceLength,
																(char *)wave->theWaveform,
																wave->waveSize,
																wave->bitSize,
																wave->channels,
																NULL,	// allocate block buffer
																waveHeader.wfx.nBlockAlign,
																NULL);
								break;
							default:
								BAE_ASSERT(FALSE);
								pIFF->lastError = BAD_FILE_TYPE;
								break;
							}
						}
						else
						{
							pIFF->lastError = MEMORY_ERR;
						}
					}
				}
			}
			else
			{
				pIFF->lastError = BAD_FILE_TYPE;
			}
			
			if (pIFF->lastError)
			{
				if (wave) XDisposePtr(wave->theWaveform);
				XDisposePtr(wave);
				wave = NULL;
			}
		}
		else
		{
			pIFF->lastError = MEMORY_ERR;
		}

		*pError = pIFF->lastError;
		XDisposePtr((XPTR)pIFF);
	}
	else
	{
		*pError = MEMORY_ERR;
	}
	
	return wave;
}

#if USE_CREATION_API == TRUE
static OPErr PV_WriteFromMemoryWaveFile(XFILENAME *file, GM_Waveform const* pAudioData, XWORD formatTag)
{
	X_IFF			*pIFF;
	XWaveHeader		waveHeader;
	OPErr			err;

	err = NO_ERR;
	if (file && pAudioData && (formatTag == X_WAVE_FORMAT_PCM))
	{
		if (pAudioData->compressionType != C_NONE)
		{
			BAE_ASSERT(FALSE);	// for now
			//MOE: should decompress here and use below
			return PARAM_ERR;
		}

		pIFF = (X_IFF *)XNewPtr((long)sizeof(X_IFF));
		if (pIFF)
		{
			IFF_SetFormType(pIFF, X_RIFF);
			pIFF->fileReference = XFileOpenForWrite(file, TRUE);
			if (pIFF->fileReference)
			{
				// write form type
				pIFF->formPosition = XFileGetPosition(pIFF->fileReference);	 // get current pos
				IFF_WriteType(pIFF, X_RIFF);
				IFF_WriteSize(pIFF, -1);	// we come back to this and rewrite it after completely done
				pIFF->formLength = -1;

				IFF_WriteType(pIFF, X_WAVE);
				// setup header. values need to be stored in intel order.
				#if X_WORD_ORDER != FALSE	// intel
					waveHeader.wFormatTag = formatTag;
					waveHeader.nChannels = pAudioData->channels;
					waveHeader.wBitsPerSample = pAudioData->bitSize;
					waveHeader.nSamplesPerSec = XFIXED_TO_UNSIGNED_LONG(pAudioData->sampledRate);
					waveHeader.nBlockAlign = pAudioData->bitSize / 8 * pAudioData->channels;
					waveHeader.nAvgBytesPerSec = waveHeader.nSamplesPerSec * waveHeader.nBlockAlign;
					waveHeader.cbSize = 0;
				#else
					// wave files require data to be intel ordered
					waveHeader.wFormatTag = XSwapShort(formatTag);
					waveHeader.nChannels = XSwapShort(pAudioData->channels);
					waveHeader.wBitsPerSample = XSwapShort(pAudioData->bitSize);
					waveHeader.nSamplesPerSec = XSwapLong(XFIXED_TO_UNSIGNED_LONG(pAudioData->sampledRate));
					waveHeader.nBlockAlign = pAudioData->bitSize / 8 * pAudioData->channels;
					waveHeader.nAvgBytesPerSec = XSwapLong(XFIXED_TO_UNSIGNED_LONG(pAudioData->sampledRate) * waveHeader.nBlockAlign);
					waveHeader.nBlockAlign = XSwapShort(waveHeader.nBlockAlign);
					waveHeader.cbSize = XSwapShort(0);
				#endif

				// write wave header block
				if (IFF_PutChunk(pIFF, X_FMT, (long)sizeof(XWaveHeader), (XPTR)&waveHeader) == NO_ERR)
				{
					#if X_WORD_ORDER == FALSE	// motorola?
					if (pAudioData->bitSize == 16)
					{
						// swap to intel format
						XSwapShorts((short int*)pAudioData->theWaveform, pAudioData->waveFrames * pAudioData->channels);
					}
					#endif
					// write out loop points
					err = IFF_WriteWAVLoopPoints(pIFF, pAudioData);
					if (err == NO_ERR)
					{
						// write out sample data
						if (IFF_PutChunk(pIFF, X_DATA, pAudioData->waveSize, pAudioData->theWaveform) == NO_ERR)
						{
							unsigned long	end;
							unsigned long	size;
							
							// write end
							end = XFileGetPosition(pIFF->fileReference);	 // get current pos
							XFileSetPosition(pIFF->fileReference, pIFF->formPosition + 4);
							size = end - pIFF->formPosition - 8;
							IFF_WriteSize(pIFF, size);
						}
						else
						{
							err = pIFF->lastError;
						}
					}

					#if X_WORD_ORDER == FALSE	// motorola?
					if (pAudioData->bitSize == 16)
					{
						// put back the way we found it
						XSwapShorts((short int*)pAudioData->theWaveform, pAudioData->waveFrames * pAudioData->channels);
					}
					#endif
				}
				else
				{
					err = pIFF->lastError;
				}
				XFileClose(pIFF->fileReference);
			}
			else
			{
				err = FILE_NOT_FOUND;
			}
			XDisposePtr((XPTR)pIFF);
		}
		else
		{
			err = MEMORY_ERR;
		}
	}
	else
	{
		BAE_ASSERT(FALSE);
		err = PARAM_ERR;
	}
	return err;
}

static OPErr PV_WriteFromMemorySunAUFile(XFILENAME *file, GM_Waveform const* pAudioData, XWORD formatTag)
{
	SunAudioFileHeader	auHeader;
	OPErr				err = NO_ERR;
	XFILE				theFile;

	if (file && pAudioData && (formatTag == X_WAVE_FORMAT_PCM))
	{
		if (pAudioData->compressionType != C_NONE)
		{
			return PARAM_ERR;
		}

		auHeader.magic = SUN_AUDIO_FILE_MAGIC_NUMBER;
		auHeader.hdr_size = sizeof(SunAudioFileHeader);
		auHeader.data_size = pAudioData->waveSize;
		switch (pAudioData->bitSize)
		{
			case 8:
				auHeader.encoding = SUN_AUDIO_FILE_ENCODING_LINEAR_8;
				break;
			case 16:
				auHeader.encoding = SUN_AUDIO_FILE_ENCODING_LINEAR_16;
				break;
			default:
				return PARAM_ERR;
		}
		auHeader.sample_rate = XFIXED_TO_UNSIGNED_LONG(pAudioData->sampledRate);
		auHeader.channels = pAudioData->channels;

		#if X_WORD_ORDER != FALSE // intel?
			auHeader.magic = XSwapLong(auHeader.magic);
			auHeader.hdr_size = XSwapLong(auHeader.hdr_size);
			auHeader.data_size = XSwapLong(auHeader.data_size);
			auHeader.encoding = XSwapLong(auHeader.encoding);
			auHeader.sample_rate = XSwapLong(auHeader.sample_rate);
			auHeader.channels = XSwapLong(auHeader.channels);
		#endif


		theFile = XFileOpenForWrite(file, TRUE);
		if (theFile)
		{
			if (XFileWrite(theFile, &auHeader, sizeof(SunAudioFileHeader)) == -1)
				return PARAM_ERR;

			
			if (pAudioData->bitSize == 8)
			{
				// 8 bit .AU data is signed, but internal engine 8 bit data is unsigned.
				XPhase8BitWaveform((XBYTE*)pAudioData->theWaveform, pAudioData->waveSize);
			}

			#if X_WORD_ORDER != FALSE	// intel?
			if (pAudioData->bitSize == 16)
			{
				XSwapShorts((short int*)pAudioData->theWaveform, pAudioData->waveFrames * pAudioData->channels);
			}
			#endif

			if (XFileWrite(theFile, pAudioData->theWaveform, pAudioData->waveSize) == -1)
				return PARAM_ERR;

			if (pAudioData->bitSize == 8)
			{
				// undo the switch
				XPhase8BitWaveform((XBYTE*)pAudioData->theWaveform, pAudioData->waveSize);
			}

			#if X_WORD_ORDER != FALSE	// intel?
			if (pAudioData->bitSize == 16)
			{
				XSwapShorts((short int*)pAudioData->theWaveform, pAudioData->waveFrames * pAudioData->channels);
			}
			#endif

			XFileClose(theFile);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = PARAM_ERR;
	}
	return err;
}

#endif	// USE_CREATION_API == TRUE

static float pv_pow(float x, int n)
{
	float	answer;
	int		count;
	int		sign;

	sign = (n > 0) ? 1 : -1;
	n = ABS(n);
	answer = 1.0;
	if (sign > 0)
	{
		for (count = 0; count < n; count++)
		{
			answer *= x;
		}
	}
	else
	{
		for (count = 0; count < n; count++)
		{
			answer /= x;
		}
	}
	return answer;
}

// implement ldexp. This is the brute force way of doing this, but its not used very often.
static float pv_ldexp(float x, int y)
{
	return x * pv_pow(2, y);
}

/*
 * C O N V E R T   F R O M   I E E E   E X T E N D E D  
 */

/* 
 * Copyright (C) 1988-1991 Apple Computer, Inc.
 * All rights reserved.
 *
 * Machine-independent I/O routines for IEEE floating-point numbers.
 *
 * NaN's and infinities are converted to HUGE_VAL or HUGE, which
 * happens to be infinity on IEEE machines.  Unfortunately, it is
 * impossible to preserve NaN's in a machine-independent way.
 * Infinities are, however, preserved on IEEE machines.
 *
 * These routines have been tested on the following machines:
 *    Apple Macintosh, MPW 3.1 C compiler
 *    Apple Macintosh, THINK C compiler
 *    Silicon Graphics IRIS, MIPS compiler
 *    Cray X/MP and Y/MP
 *    Digital Equipment VAX
 *
 *
 * Implemented by Malcolm Slaney and Ken Turkowski.
 *
 * Malcolm Slaney contributions during 1988-1990 include big- and little-
 * endian file I/O, conversion to and from Motorola's extended 80-bit
 * floating-point format, and conversions to and from IEEE single-
 * precision floating-point format.
 *
 * In 1991, Ken Turkowski implemented the conversions to and from
 * IEEE double-precision format, added more precision to the extended
 * conversions, and accommodated conversions involving +/- infinity,
 * NaN's, and denormalized numbers.
 */
#define UNSIGNED_TO_FLOAT(u)         (((float)((long)(u - 2147483647L - 1))) + 2147483648.0)

/****************************************************************
 * Extended precision IEEE floating-point conversion routine.
 ****************************************************************/
XFIXED XConvertFromIeeeExtended(unsigned char *bytes)
{
	float			f;
	int				expon;
	unsigned long	hiMant, loMant;
	XFIXED			ieeeRate;

	expon = ((bytes[0] & 0x7F) << 8L) | (bytes[1] & 0xFF);
	hiMant	=    ((unsigned long)(bytes[2] & 0xFF) << 24L)
			|    ((unsigned long)(bytes[3] & 0xFF) << 16L)
			|    ((unsigned long)(bytes[4] & 0xFF) << 8L)
			|    ((unsigned long)(bytes[5] & 0xFF));
	loMant	=    ((unsigned long)(bytes[6] & 0xFF) << 24L)
			|    ((unsigned long)(bytes[7] & 0xFF) << 16L)
			|    ((unsigned long)(bytes[8] & 0xFF) << 8L)
			|    ((unsigned long)(bytes[9] & 0xFF));

	if (expon == 0 && hiMant == 0 && loMant == 0) 
	{
		f = 0;
	}
	else 
	{
		if (expon == 0x7FFF) 
		{    /* Infinity or NaN */
//#ifndef HUGE_VAL
//	#define HUGE_VAL HUGE
//#endif
//			f = HUGE_VAL;
			f = 0.0;
		}
        else 
		{
			expon -= 16383;
			expon -= 31;
			f  = pv_ldexp(UNSIGNED_TO_FLOAT(hiMant), expon);
			expon -= 32;
			f += pv_ldexp(UNSIGNED_TO_FLOAT(loMant), expon);
		}
	}

	if (bytes[0] & 0x80)
	{
		f *= -1;
	}
	ieeeRate = FLOAT_TO_XFIXED(f);
	return ieeeRate;
}

#if USE_CREATION_API == TRUE
/*
 * C O N V E R T   T O   I E E E   E X T E N D E D
 */

/* Copyright (C) 1988-1991 Apple Computer, Inc.
 * All rights reserved.
 *
 * Machine-independent I/O routines for IEEE floating-point numbers.
 *
 * NaN's and infinities are converted to HUGE_VAL or HUGE, which
 * happens to be infinity on IEEE machines.  Unfortunately, it is
 * impossible to preserve NaN's in a machine-independent way.
 * Infinities are, however, preserved on IEEE machines.
 *
 * These routines have been tested on the following machines:
 *    Apple Macintosh, MPW 3.1 C compiler
 *    Apple Macintosh, THINK C compiler
 *    Silicon Graphics IRIS, MIPS compiler
 *    Cray X/MP and Y/MP
 *    Digital Equipment VAX
 *
 *
 * Implemented by Malcolm Slaney and Ken Turkowski.
 *
 * Malcolm Slaney contributions during 1988-1990 include big- and little-
 * endian file I/O, conversion to and from Motorola's extended 80-bit
 * floating-point format, and conversions to and from IEEE single-
 * precision floating-point format.
 *
 * In 1991, Ken Turkowski implemented the conversions to and from
 * IEEE double-precision format, added more precision to the extended
 * conversions, and accommodated conversions involving +/- infinity,
 * NaN's, and denormalized numbers.
 */

#define FLOAT_TO_UNSIGNED(f)      ((unsigned long)(((long)(f - 2147483648.0)) + 2147483647L + 1))

void XConvertToIeeeExtended(XFIXED ieeeFixedRate, unsigned char *bytes)
{
	int				sign;
	int				expon;
	double			fMant, fsMant;
	unsigned long	hiMant, loMant;
	double			num;

	num = XFIXED_TO_FLOAT(ieeeFixedRate);
    if (num < 0) 
    {
        sign = 0x8000;
        num *= -1;
    } 
    else 
    {
        sign = 0;
    }

    if (num == 0) 
    {
        expon = 0; hiMant = 0; loMant = 0;
    }
    else 
    {
        fMant = frexp(num, &expon);
        if ((expon > 16384) || !(fMant < 1)) 
        {    /* Infinity or NaN */
            expon = sign|0x7FFF; 
            hiMant = 0; 
            loMant = 0; /* infinity */
        }
        else 
        {    /* Finite */
            expon += 16382;
            if (expon < 0) 
            {    /* denormalized */
                fMant = ldexp(fMant, expon);
                expon = 0;
            }
            expon |= sign;
            fMant = ldexp(fMant, 32);          
            fsMant = floor(fMant); 
            hiMant = FLOAT_TO_UNSIGNED(fsMant);
            fMant = ldexp(fMant - fsMant, 32); 
            fsMant = floor(fMant); 
            loMant = FLOAT_TO_UNSIGNED(fsMant);
        }
    }
    
    bytes[0] = expon >> 8;
    bytes[1] = expon;
    bytes[2] = (char)(hiMant >> 24);
    bytes[3] = (char)(hiMant >> 16);
    bytes[4] = (char)(hiMant >> 8);
    bytes[5] = (char)hiMant;
    bytes[6] = (char)(loMant >> 24);
    bytes[7] = (char)(loMant >> 16);
    bytes[8] = (char)(loMant >> 8);
    bytes[9] = (char)loMant;
}
#endif

#if 0
	#pragma mark ## AIFF read functions ##
#endif
// get AIFF header. Returns 0 if ok, -1 if failed
static long IFF_GetAIFFHeader(X_IFF *pIFF, XAIFFHeader * pHeaderInfo)
{
	long	theErr;

	theErr = IFF_GetChunk(pIFF, X_Common, (long)sizeof(XAIFFHeader), (void *)pHeaderInfo);

	#if X_WORD_ORDER != FALSE	// intel?
		pHeaderInfo->numChannels = XSwapShort(pHeaderInfo->numChannels);
		pHeaderInfo->numSampleFrames = XSwapLong(pHeaderInfo->numSampleFrames);
		pHeaderInfo->sampleSize = XSwapShort(pHeaderInfo->sampleSize);
	#endif
	return theErr;
}

// get AIFF extended header. Returns 0 if ok, -1 if failed
static long IFF_GetAIFFExtenedHeader(X_IFF *pIFF, XAIFFExtenedHeader * pHeaderInfo)
{
	long	theErr;
	char	size;

	theErr = IFF_GetChunk(pIFF, X_Common, (long)sizeof(XAIFFHeader) + sizeof(long), (void *)pHeaderInfo);

	#if X_WORD_ORDER != FALSE	// intel?
		pHeaderInfo->numChannels = XSwapShort(pHeaderInfo->numChannels);
		pHeaderInfo->numSampleFrames = XSwapLong(pHeaderInfo->numSampleFrames);
		pHeaderInfo->sampleSize = XSwapShort(pHeaderInfo->sampleSize);
		pHeaderInfo->compressionType = XSwapLong(pHeaderInfo->compressionType);
	#endif
	
	theErr = XFileRead(pIFF->fileReference, &size, 1L);
	pHeaderInfo->compressionName[0] = size;
	theErr = XFileRead(pIFF->fileReference, &pHeaderInfo->compressionName[1], size);
	return theErr;
}

static long IFF_GetAIFFInstrument(X_IFF *pIFF, XInstrumentHeader * pInstrumentInfo)
{
	long	theErr;

	theErr = IFF_GetChunk(pIFF, X_Instrument, (long)sizeof(XInstrumentHeader), (void *)pInstrumentInfo);

	#if X_WORD_ORDER != FALSE	// intel?
		pInstrumentInfo->gain = XSwapShort(pInstrumentInfo->gain);
		pInstrumentInfo->sustainLoop_playMode = XSwapShort(pInstrumentInfo->sustainLoop_playMode);
		pInstrumentInfo->sustainLoop_beginLoop = XSwapShort(pInstrumentInfo->sustainLoop_beginLoop);
		pInstrumentInfo->sustainLoop_endLoop = XSwapShort(pInstrumentInfo->sustainLoop_endLoop);
		pInstrumentInfo->releaseLoop_beginLoop = XSwapShort(pInstrumentInfo->releaseLoop_beginLoop);
		pInstrumentInfo->releaseLoop_endLoop = XSwapShort(pInstrumentInfo->releaseLoop_endLoop);
		pInstrumentInfo->extra = XSwapShort(pInstrumentInfo->extra);
	#endif
	return theErr;
}

/*
	unsigned short					numMarkers;			// 2 markers
	short							id
	unsigned long					position;			// 1
	pstring							name;				// begloop
	short							id
	unsigned long					position;			// 2
	pstring							name;				// endloop
*/
// searches for MARK and pulls the ID marker's value
static XBOOL IFF_GetAIFFMarkerValue(X_IFF *pIFF, short int ID, unsigned long *pMarkerValue)
{
	unsigned char	loopMark[1024];
	long			theErr;
	unsigned char	*pData, *pEnd;
	unsigned short	len;
	
	*pMarkerValue = 0;
	theErr = IFF_GetChunk(pIFF, X_Marker, 1023L, loopMark);
	if (theErr == 0)
	{
		pData = loopMark;
		pEnd = &loopMark[1024];
		len = XGetShort(pData);
		pData += 2;				// skip marker count
		if (len > 1)
		{
			while (pData < pEnd)
			{
				if (XGetShort(pData) == ID)
				{
					pData += 2;				// skip marker id
					*pMarkerValue = XGetLong(pData);
					return TRUE;
				}
				pData += 2;				// skip marker id
				pData += 4;				// skip past value

				len = *pData;
				pData += len + 1;		// walk past first string
				pData++;
			}
		}
	}
	return FALSE;
}

// Returns AIFF loop points, if there. Return 0 if successful, -1 if failure
static long IFF_GetAIFFLoopPoints(X_IFF *pIFF, unsigned long *pLoopStart, unsigned long *pLoopEnd)
{
	XInstrumentHeader	inst;
	long				err;

	*pLoopStart = 0;
	*pLoopEnd = 0;
	err = IFF_GetAIFFInstrument(pIFF, &inst);
	if (err == 0)
	{
		if (IFF_GetAIFFMarkerValue(pIFF, inst.sustainLoop_beginLoop, pLoopStart))
		{
			IFF_GetAIFFMarkerValue(pIFF, inst.sustainLoop_endLoop, pLoopEnd);
		}
	}
	return err;
}

#if USE_CREATION_API == TRUE
// Create and write out loop points if there's any in the waveform.
static OPErr IFF_WriteAIFFLoopPoints(X_IFF *pIFF, GM_Waveform const* pWaveform)
{
	XSingleLoopMarker	loop;
	XInstrumentHeader	inst;
	static char			mBegLoop[] = {0x07, 0x62, 0x65, 0x67, 0x4C, 0x6F, 0x6F, 0x70};	//'begLoop'
	static char			mEndLoop[] = {0x07, 0x65, 0x6E, 0x64, 0x4C, 0x6F, 0x6F, 0x70};	//'endLoop'
	OPErr				err;

	err = NO_ERR;
	if (pWaveform)
	{
		if (pWaveform->startLoop && pWaveform->endLoop)
		{
			XSetMemory(&inst, (long)sizeof(XInstrumentHeader), 0);
			XSetMemory(&loop, (long)sizeof(XSingleLoopMarker), 0);

			XPutShort(&loop.numMarkers, 2);
			XPutShort(&loop.id1, 0);		// ID 1
			XPutShort(&loop.id2, 1);		// ID 2
			XBlockMove(mBegLoop, &loop.name1, 8L);
			XBlockMove(mEndLoop, &loop.name2, 8L);
			XPutLong(&loop.position1, pWaveform->startLoop);
			XPutLong(&loop.position2, pWaveform->endLoop);

			inst.baseFrequency = (unsigned char)pWaveform->baseMidiPitch;
			XPutShort(&inst.sustainLoop_playMode, 1);	// play forward
			XPutShort(&inst.sustainLoop_beginLoop, 0);	// ID 1
			XPutShort(&inst.sustainLoop_endLoop, 1);	// ID 2

			err = IFF_PutChunk(pIFF, X_Instrument, (long)sizeof(XInstrumentHeader), (XPTR)&inst);
			if (err == NO_ERR)
			{
				err = IFF_PutChunk(pIFF, X_Marker, (long)sizeof(XSingleLoopMarker), (XPTR)&loop);
			}
		}
	}
	return err;
}
#endif

// Returns AIFF base pitch, if there. Return 0 if successful, -1 if failure
static long IFF_GetAIFFBasePitch(X_IFF *pIFF, XWORD *pBasePitch)
{
	XInstrumentHeader	inst;
	long				err;

	err = IFF_GetAIFFInstrument(pIFF, &inst);
	if (err == 0)
	{
		*pBasePitch = inst.baseFrequency;
	}
	return err;
}

// Get compressed and uncompressed size. Return 0 if successful, -1 if failure
static long IFF_GetAIFFSampleSize(X_IFF *pIFF, long *pUncompressedSize, long *pCompressedSize)
{
	long				size, error;
	XAIFFExtenedHeader	header;

	size = 0L;
	error = 0;
	if (pUncompressedSize && pCompressedSize)
	{
		switch (pIFF->headerType)
		{
			case X_AIFF:
				error = IFF_GetAIFFHeader(pIFF, (XAIFFHeader *)&header);
				size = header.numSampleFrames * header.numChannels * (header.sampleSize / 8);
				break;
			case X_AIFC:
				error = IFF_GetAIFFExtenedHeader(pIFF, &header);
				switch (header.compressionType)
				{
					default:
						BAE_ASSERT(FALSE);
						pIFF->lastError = BAD_FILE_TYPE;
						// fail because we don't know how to decompress
						break;
					case X_NONE:
						size = header.numSampleFrames * header.numChannels * (header.sampleSize / 8);
						break;
					case X_IMA4:
						// Sound Manager defines 64 samples per packet number of sample frames
						size = header.numSampleFrames * AIFF_IMA_BLOCK_FRAMES * header.numChannels * (header.sampleSize / 8);
						break;
				}
				break;
		}
	}

	if (size)
	{
		// now position right to data block
		if (IFF_ScanToBlock(pIFF, X_SoundData))	// skip to body
		{
			error = -1;	// failed
		}
		else
		{
			XFileSetPositionRelative(pIFF->fileReference, -4L);		// back-up and get size
			if (XFileRead(pIFF->fileReference, pCompressedSize, (long)sizeof(long)) == -1)
			{
				pIFF->lastError = BAD_FILE;
				error = -1;
			}
			BAE_ASSERT(pCompressedSize);
			*pCompressedSize = XGetLong(pCompressedSize);
//			XFileSetPositionRelative(pIFF->fileReference, sizeof(long) * 2L);
		}
	}

	BAE_ASSERT(pUncompressedSize);
	*pUncompressedSize = size;
	return error;
}

#if USE_CREATION_API == TRUE
static OPErr PV_WriteFromMemoryAiffFile(XFILENAME *file, GM_Waveform const* pAudioData, XWORD formatTag)
{
	X_IFF			*pIFF;
	XAIFFHeader		aiffHeader;
	OPErr			err;

	err = NO_ERR;
	if (file && pAudioData && (formatTag == X_WAVE_FORMAT_PCM))
	{
		if (pAudioData->compressionType != C_NONE)
		{
			return PARAM_ERR;
		}

		pIFF = (X_IFF *)XNewPtr((long)sizeof(X_IFF));
		if (pIFF)
		{
			IFF_SetFormType(pIFF, X_FORM);
			pIFF->fileReference = XFileOpenForWrite(file, TRUE);
			if (pIFF->fileReference)
			{
				// write form type
				pIFF->formPosition = XFileGetPosition(pIFF->fileReference);	 // get current pos
				IFF_WriteType(pIFF, X_FORM);
				IFF_WriteSize(pIFF, -1);	// we come back to this and rewrite it after completely done
				pIFF->formLength = -1;

				IFF_WriteType(pIFF, X_AIFF);
				// setup header. values need to be stored in motorola order.
				#if X_WORD_ORDER != FALSE	// intel
					// aiff files require data to be motorola ordered
					aiffHeader.numChannels = XSwapShort(pAudioData->channels);
					aiffHeader.numSampleFrames = XSwapLong(pAudioData->waveFrames);
					aiffHeader.sampleSize = XSwapShort(pAudioData->bitSize);
					XConvertToIeeeExtended(pAudioData->sampledRate, aiffHeader.sampleRate);
				#else
					aiffHeader.numChannels = pAudioData->channels;
					aiffHeader.numSampleFrames = pAudioData->waveFrames;
					aiffHeader.sampleSize = pAudioData->bitSize;
					XConvertToIeeeExtended(pAudioData->sampledRate, aiffHeader.sampleRate);
				#endif

				// write aiff header block
				if (IFF_PutChunk(pIFF, X_Common, (long)sizeof(XAIFFHeader), (XPTR)&aiffHeader) == NO_ERR)
				{
					#if X_WORD_ORDER != FALSE	// intel?
					if (pAudioData->bitSize == 16)
					{
						// swap to motorola format
						XSwapShorts((short int*)pAudioData->theWaveform, pAudioData->waveFrames * pAudioData->channels);
					}
					#endif

					if (pAudioData->bitSize == 8)
					{
						XPhase8BitWaveform((XBYTE*)pAudioData->theWaveform, pAudioData->waveSize);
					}

					// write out loop points					
					err = IFF_WriteAIFFLoopPoints(pIFF, pAudioData);
					if (err == NO_ERR)
					{
						if (IFF_PutChunk(pIFF, X_SoundData, pAudioData->waveSize, pAudioData->theWaveform) == NO_ERR)
						{
							unsigned long	end;
							unsigned long	size;
							
							// write end
							end = XFileGetPosition(pIFF->fileReference);	 // get current pos
							XFileSetPosition(pIFF->fileReference, pIFF->formPosition + 4);
							size = end - pIFF->formPosition;
							size -= 8;	// subtract format header and type from total length
							IFF_WriteSize(pIFF, size);
						}
						else
						{
							err = pIFF->lastError;
						}
					}
					// put back the way we found it
					if (pAudioData->bitSize == 8)
					{
						XPhase8BitWaveform((XBYTE*)pAudioData->theWaveform, pAudioData->waveSize);
					}

					#if X_WORD_ORDER != FALSE	// intel?
					if (pAudioData->bitSize == 16)
					{
						XSwapShorts((short int*)pAudioData->theWaveform, pAudioData->waveFrames * pAudioData->channels);
					}
					#endif
				}
				else
				{
					err = pIFF->lastError;
				}
				XFileClose(pIFF->fileReference);
			}
			else
			{
				err = FILE_NOT_FOUND;
			}
			XDisposePtr((XPTR)pIFF);
		}
		else
		{
			err = MEMORY_ERR;
		}
	}
	else
	{
		BAE_ASSERT(FALSE);
		err = PARAM_ERR;
	}
	return err;
}
#endif	// #if USE_CREATION_API == TRUE

#define AIFF_IMA_BUFFER_SIZE	AIFF_IMA_BLOCK_BYTES * 40

static OPErr PV_ReadAIFFAndDecompressIMA(XFILE fileReference, long sourceLength,
											unsigned char *pDestSample, long destLength,
											char outputBitSize, char channels,
											unsigned long *pBufferLength,
											short predictorCache[2])
{
	XBYTE		codeBlock[AIFF_IMA_BUFFER_SIZE];
	long		writeBufferLength, size, offset;
	OPErr		err;

	err = NO_ERR;
	writeBufferLength = 0;
	sourceLength -= sourceLength % AIFF_IMA_BUFFER_SIZE;	// round to block size

	#if USE_DEBUG && 0
	{
		char text[256];
		
		sprintf(text, "sourceLength %ld, AIFF_IMA_BUFFER_SIZE %ld", sourceLength, AIFF_IMA_BUFFER_SIZE);
		DEBUG_STR(XCtoPstr(text));
	}
	#endif

	while (sourceLength > 0)
	{
		if (sourceLength > AIFF_IMA_BUFFER_SIZE)
		{
			size = XFileGetPosition(fileReference);	 /* get current pos */
			XFileRead(fileReference, codeBlock, AIFF_IMA_BUFFER_SIZE);
			size = XFileGetPosition(fileReference) - size;
		}
		else
		{
			// last block so just stop
			size = 0;
		}
		sourceLength -= AIFF_IMA_BUFFER_SIZE;
		offset = 0;
		if (size)
		{
			offset = XExpandAiffImaStream(codeBlock, AIFF_IMA_BLOCK_BYTES,
											pDestSample, outputBitSize,
											size, channels, predictorCache);
			if (offset == 0)
			{
				// we're done
				break;
			}
			destLength -= offset;
			if (destLength < 0)
			{
				// time to quit, we've hit the end
				break;
			}
			else
			{
				pDestSample += offset;
				writeBufferLength += offset;
			}
		}
	}
	if (pBufferLength)
	{
		*pBufferLength = writeBufferLength;
	}
	return err;
}

// This will read into memory the entire AIFF file and return a GM_Waveform structure.
// When disposing make sure and dispose of both the GM_Waveform structure and the
// theWaveform inside of that structure with XDisposePtr
static GM_Waveform * PV_ReadIntoMemoryAIFFFile(XFILE file, XBOOL decodeData,
												long *pFormat, unsigned long *pBlockSize,
												OPErr *pError)
{
GM_Waveform			*wave;
X_IFF				*pIFF;

	BAE_ASSERT(file);
	BAE_ASSERT(pError);

	wave = NULL;
	
	pIFF = (X_IFF*)XNewPtr(sizeof(X_IFF));
	if (pIFF)
	{
		IFF_SetFormType(pIFF, X_FORM);
		pIFF->fileReference = file;

		wave = (GM_Waveform*)XNewPtr(sizeof(GM_Waveform));
		if (wave)
		{
		long				type;

			type = IFF_FileType(pIFF);
			pIFF->headerType = type;

			if ((type != X_AIFF) && (type != X_AIFC))
			{
				BAE_ASSERT(FALSE);
				pIFF->lastError = BAD_FILE_TYPE;
			}
			else
			{
			XAIFFExtenedHeader	aiffHeader;
			long				size;
			long				sourceLength;

				XSetMemory(&aiffHeader, sizeof(XAIFFExtenedHeader), 0);

				if (type == X_AIFF)
				{
					IFF_GetAIFFHeader(pIFF, (XAIFFHeader*)&aiffHeader);
					aiffHeader.compressionType = X_NONE;
				}
				else
				{
					IFF_GetAIFFExtenedHeader(pIFF, &aiffHeader);
				}


				wave->channels = (UBYTE)aiffHeader.numChannels;
				wave->bitSize = (UBYTE)aiffHeader.sampleSize;
				wave->baseMidiPitch = 60;
				wave->compressionType = C_NONE;

				// get loop points, if any
				IFF_GetAIFFLoopPoints(pIFF, &wave->startLoop, &wave->endLoop);
				IFF_GetAIFFBasePitch(pIFF, &wave->baseMidiPitch);

				// Convert the ieee number into a 16.16 fixed value
				wave->sampledRate = XConvertFromIeeeExtended(aiffHeader.sampleRate);
				if (wave->sampledRate == 0)
				{
					BAE_ASSERT(FALSE);
					pIFF->lastError = BAD_SAMPLE;
				}
				else if (IFF_GetAIFFSampleSize(pIFF, &size, &sourceLength) == 0)
				{
					BAE_ASSERT(size != 0);
					wave->waveSize = size;
					wave->waveFrames = wave->waveSize / (wave->channels * (wave->bitSize / 8));

					XFileSetPositionRelative(pIFF->fileReference, sizeof(long) * 2L);
					// now the file is positioned right at the data block

					if (pBlockSize)
					{
						wave->currentFilePosition = XFileGetPosition(pIFF->fileReference);
						BAE_ASSERT(pFormat);
						*pFormat = aiffHeader.compressionType;
						*pBlockSize = sizeof(long) * 2;	//MOE: Isn't this too small for efficient streaming?
						// don't read/decode data, other streaming code will do it
					}
					else
					{
						switch (aiffHeader.compressionType)
						{
						case X_NONE:
							wave->theWaveform = (SBYTE*)XNewPtr(size);
							if (wave->theWaveform)
							{
								if (XFileRead(pIFF->fileReference, wave->theWaveform, size) != -1)
								{
#if X_WORD_ORDER != FALSE	// intel?
									if (wave->bitSize == 16)
									{
										XSwapShorts((short*)wave->theWaveform,
													wave->waveFrames * wave->channels);
									}
#endif
								}
								else
								{
									BAE_ASSERT(FALSE);
									pIFF->lastError = BAD_FILE;
								}
							}
							else
							{
								pIFF->lastError = MEMORY_ERR;
							}
							break;
						case X_IMA4:
							if (decodeData)
							{
								wave->theWaveform = (SBYTE*)XNewPtr(size);
								if (wave->theWaveform)
								{
								short		predictorCache[2];
								
									predictorCache[0] = 0;
									predictorCache[1] = 0;
									pIFF->lastError =
										PV_ReadAIFFAndDecompressIMA(pIFF->fileReference,
																	sourceLength,
																	(XBYTE*)wave->theWaveform,
																	wave->waveSize,
																	wave->bitSize,
																	wave->channels,
																	NULL,
																	predictorCache);
								}
								else
								{
									pIFF->lastError = MEMORY_ERR;
								}
							}
							else
							{
							XDWORD		const imaBlocks =
											(wave->waveFrames + AIFF_IMA_BLOCK_FRAMES - 1) /
											AIFF_IMA_BLOCK_FRAMES;
								
								wave->waveSize = imaBlocks * wave->channels * AIFF_IMA_BLOCK_BYTES;
								BAE_ASSERT(wave->waveSize > 0);
								wave->theWaveform = (SBYTE*)XNewPtr(wave->waveSize);
								if (wave->theWaveform)
								{
									if (XFileRead(file, wave->theWaveform, wave->waveSize))
									{
										BAE_ASSERT(FALSE);	// file probably wasn't long enough
										pIFF->lastError = BAD_FILE;
									}
									wave->compressionType = C_IMA4;
								}
								else
								{
									pIFF->lastError = MEMORY_ERR;
								}
							}
							break;
						default:
							BAE_ASSERT(FALSE);
							pIFF->lastError = BAD_FILE_TYPE;
							break;
						}

						if ((pIFF->lastError == NO_ERR) && ((long)wave->waveSize == size))
						{
							// now, if the file is 8 bit sample, change the sample phase
							if (wave->bitSize == 8)
							{
								BAE_ASSERT(wave->theWaveform);
								XPhase8BitWaveform((XBYTE*)wave->theWaveform, wave->waveSize);
							}
						}
					}
				}
			}
		}
		else
		{
			pIFF->lastError = MEMORY_ERR;
		}
		
		if (pIFF->lastError != NO_ERR)
		{
			if (wave) XDisposePtr(wave->theWaveform);
			XDisposePtr(wave);
			wave = NULL;
		}
		*pError = pIFF->lastError;
		XDisposePtr(pIFF);
	}
	else
	{
		*pError = MEMORY_ERR;
	}
	return wave;
}

// This will read into memory the entire Sun AU file and return a GM_Waveform structure.
// When disposing make sure and dispose of both the GM_Waveform structure and the
// theWaveform inside of that structure with XDisposePtr
static GM_Waveform * PV_ReadIntoMemorySunAUFile(XFILE file, XBOOL decodeData,
												long *pFormat, unsigned long *pBlockSize,
												OPErr *pError)
{
GM_Waveform*		wave;
OPErr				err;
SunAudioFileHeader	sunHeader;

	BAE_ASSERT(file);
	BAE_ASSERT(pError);

	wave = NULL;
	err = NO_ERR;

	if (XFileRead(file, &sunHeader, (long)sizeof(SunAudioFileHeader)) == 0)
	{
	long				size;
	long				filePos;
	long				originalLength;
	
		// now skip past any info string
		size = XGetLong(&sunHeader.hdr_size) - (long)sizeof(SunAudioFileHeader);
		filePos = XFileGetPosition(file) + size;
		originalLength = XFileGetLength(file) - size + sizeof(SunAudioFileHeader);

		XFileSetPosition(file, filePos);	// go back to ending
	    // Make sure we've got a legitimate audio file
	    if (XGetLong(&sunHeader.magic) == SUN_AUDIO_FILE_MAGIC_NUMBER)
	    {
		long				const encoding = XGetLong(&sunHeader.encoding);
		long				waveLength = 0;
		short int			bits = 0;
		
			switch (encoding)
			{
			case SUN_AUDIO_FILE_ENCODING_LINEAR_8:
				waveLength = originalLength;
				bits = 8;
				break;
			case SUN_AUDIO_FILE_ENCODING_MULAW_8:
			case SUN_AUDIO_FILE_ENCODING_ALAW_8:
				waveLength = originalLength * 2;
				bits = 16;
				break;
			case SUN_AUDIO_FILE_ENCODING_LINEAR_16:
				waveLength = originalLength;
				bits = 16;
				break;
			case SUN_AUDIO_FILE_ENCODING_ADPCM_G721:
				waveLength = originalLength * 4;
				bits = 16;
				break;
			case SUN_AUDIO_FILE_ENCODING_ADPCM_G723_3:
			case SUN_AUDIO_FILE_ENCODING_ADPCM_G723_5:
				waveLength = originalLength * 4;
				bits = 16;
				break;
			default :
				BAE_ASSERT(FALSE);
				err = BAD_FILE_TYPE;
				break;
			}

			if (err == NO_ERR)
			{
				wave = (GM_Waveform*)XNewPtr(sizeof(GM_Waveform));
				if (wave)
				{
					wave->channels = (UBYTE)XGetLong(&sunHeader.channels);
					wave->baseMidiPitch = 60;
					wave->bitSize = (UBYTE)bits;
					wave->sampledRate = XGetLong(&sunHeader.sample_rate) << 16L;
					// we want the byte size
					wave->waveSize = waveLength;
					wave->waveFrames = wave->waveSize / wave->channels;
					if (wave->bitSize > 8)
					{
						wave->waveFrames /= 2;
					}
					wave->compressionType = C_NONE;
					
					// now the file is positioned right at the data block. So let's allocate it and read
					// it into memory
					if (pBlockSize)
					{
						// now the file is positioned right at the data block
						wave->currentFilePosition = XFileGetPosition(file);
						BAE_ASSERT(pFormat);
						*pFormat = encoding;
						*pBlockSize = 0;	//MOE: Won't this cause the streaming code to fail?
						// don't read/decode any data, streaming code will do it later
					}
					else	// if (decodeData) // for now
					{
						wave->theWaveform = (SBYTE*)XNewPtr(wave->waveSize);
						if (wave->theWaveform)
						{
							err = PV_ReadSunAUFile(encoding, file,
													wave->theWaveform, wave->waveSize, NULL, NULL);
							if (err == 0)
							{
								// we don't need to byte swap these based upon platform because
								// we generate samples from runtime code, so the results are in
								// native format already

								// now, if the file is 8 bit sample, change the sample phase
								if (wave->bitSize == 8)
								{
									XPhase8BitWaveform((unsigned char *)wave->theWaveform, wave->waveSize);
								}
							}
						}
						else
						{
							err = MEMORY_ERR;
						}
					}

					if (err != NO_ERR)
					{
						XDisposePtr((XPTR)wave->theWaveform);
						XDisposePtr((XPTR)wave);
						wave = NULL;
					}
				}
				else
				{
					BAE_ASSERT(FALSE);
					err = BAD_FILE_TYPE;
				}
			}
		}
		else
		{
			BAE_ASSERT(FALSE);
			err = BAD_FILE_TYPE;
		}
	}
	else
	{
		BAE_ASSERT(FALSE);
		err = BAD_FILE;
	}

	*pError = err;
	return wave;
}


#if USE_MPEG_DECODER != 0
static
GM_Waveform* PV_ReadIntoMemoryMPEGFile(XFILE file, XBOOL decodeData,
										long *pFormat, void **ppBlockPtr, unsigned long *pBlockSize,
										OPErr *pError)
{
GM_Waveform*		wave;
OPErr				err;
XMPEGDecodedData*	stream;

	BAE_ASSERT(file);
	BAE_ASSERT(pError);

	err = NO_ERR;
	wave = NULL;

	stream = XOpenMPEGStreamFromXFILE(file, &err);
	if (stream && (err == NO_ERR))
	{
		wave = (GM_Waveform*)XNewPtr(sizeof(GM_Waveform));
		if (wave)
		{
			wave->channels = (UBYTE)stream->channels;
			wave->bitSize = (UBYTE)stream->bitSize;
			wave->baseMidiPitch = 60;
			wave->sampledRate = stream->sampleRate;
			// we want the byte size
			wave->waveSize = stream->lengthInBytes;	// currently is usually too big
			wave->waveFrames = stream->lengthInSamples;
			wave->compressionType = C_NONE;

			// now the file is positioned right at the data block.
			if (pBlockSize)
			{
				// we're only interested in the bare info. We're about to
				// start streaming this data now.
				wave->compressionType = XGetMPEGBitrateType(stream->bitrate);
				*pFormat = wave->compressionType;

				*ppBlockPtr = (void *)-1L;
				*pBlockSize = stream->frameBufferSize;
				XCloseMPEGStream(stream);
				*pError = err;
				return wave;
			}

			// So let's allocate it and read it into memory
			if (decodeData)
			{
			UINT32		const decodingBytes = stream->maxFrameBuffers * stream->frameBufferSize;
			
				BAE_ASSERT(wave->waveSize <= decodingBytes);
				wave->theWaveform = (SBYTE*)XNewPtr(decodingBytes);
				if (wave->theWaveform)
				{
					// now decode the mpeg sample and store into the resulting buffer
					{
					SBYTE*			data;
					UINT32			count;
					UINT32			usefulBytes;
					
						data = (SBYTE *)wave->theWaveform;
						count = 0;
						while (count < stream->maxFrameBuffers)
						{
						XBOOL			done;

							done = FALSE;
							err = XFillMPEGStreamBuffer(stream, data, &done);
							if ((err != NO_ERR) || done)
							{
								break;
							}
							data += stream->frameBufferSize;
							count++;
						}
						
						usefulBytes = data - (SBYTE*)wave->theWaveform;
						if (usefulBytes < wave->waveSize)
						{
							wave->waveSize = usefulBytes;
							wave->waveFrames = usefulBytes / (wave->channels * (wave->bitSize / 8));
						}
						data = (SBYTE*)XResizePtr(wave->theWaveform, wave->waveSize);
						if (data) wave->theWaveform = data;
					}
				}
				else
				{
					err = MEMORY_ERR;
				}
			}
			else
			{
			UINT32		const encodedBytes = XFileGetLength(file);
			
				wave->waveSize = encodedBytes;
				wave->theWaveform = (SBYTE*)XNewPtr(encodedBytes);
				if (wave->theWaveform)
				{
					if (XFileSetPosition(file, 0) ||
						XFileRead(file, wave->theWaveform, encodedBytes))
					{
						err = BAD_FILE;
					}
					wave->compressionType = XGetMPEGBitrateType(stream->bitrate);
				}
				else
				{
					err = MEMORY_ERR;
				}
			}

			if (err != NO_ERR)
			{
				XDisposePtr(wave->theWaveform);
				XDisposePtr(wave);
				wave = NULL;
			}
		}
		else
		{
			err = MEMORY_ERR;
		}

		BAE_ASSERT(stream);
		XCloseMPEGStream(stream);
	}
	else
	{
		err = BAD_FILE_TYPE;
	}
		
	*pError = err;
	return wave;
}
#endif	// USE_MPEG_DECODER != FALSE

// functions used with GM_ReadAndDecodeFileStream to preserve state between decode calls.
void * GM_CreateFileState(AudioFileType fileType)
{
	void	*state;

	state = NULL;
	switch (fileType)
	{
		case FILE_AU_TYPE:
			state = (void *)XNewPtr(sizeof(SunDecodeState));
			if (state)
			{
				g72x_init_state(&((SunDecodeState *)state)->state);
				((SunDecodeState *)state)->buffer = 0;
				((SunDecodeState *)state)->bits = 0;
			}
			break;
	}
	return state;
}

void GM_DisposeFileState(AudioFileType fileType, void *state)
{
	switch (fileType)
	{
		case FILE_AU_TYPE:
			XDisposePtr((XPTR)state);
			break;
	}
}

// Read into memory a file
GM_Waveform* GM_ReadFileIntoMemory(XFILENAME *filename, AudioFileType fileType,
									XBOOL decodeData, OPErr *pErr)
{
XFILE			file;
OPErr			err;
GM_Waveform		*waveform;

	waveform = NULL;
	file = XFileOpenForRead(filename);
	if (file)
	{
		switch (fileType)
		{
		case FILE_WAVE_TYPE:
			waveform = PV_ReadIntoMemoryWaveFile(file, decodeData, NULL, NULL, &err);
			break;
		case FILE_AIFF_TYPE:
			waveform = PV_ReadIntoMemoryAIFFFile(file, decodeData, NULL, NULL, &err);
			break;
		case FILE_AU_TYPE:
			waveform = PV_ReadIntoMemorySunAUFile(file, decodeData, NULL, NULL, &err);
			break;
	#if USE_MPEG_DECODER != FALSE
		case FILE_MPEG_TYPE:
			waveform = PV_ReadIntoMemoryMPEGFile(file, decodeData, NULL, NULL, NULL, &err);
			break;
	#endif
		default :
			err = PARAM_ERR;
			break;
		}

		XFileClose(file);
	}
	else
	{
		err = FILE_NOT_FOUND;
	}

	if (pErr)
	{
		*pErr = err;
	}
	return waveform;
}

// Read into memory a file
GM_Waveform* GM_ReadFileIntoMemoryFromMemory(void *pFileBlock, unsigned long fileBlockSize,
												AudioFileType fileType, XBOOL decodeData,
												OPErr *pErr)
{
XFILE			file;
OPErr			err;
GM_Waveform		*waveform;

	waveform = NULL;
	file = XFileOpenForReadFromMemory(pFileBlock, fileBlockSize);
	if (file)
	{
		switch (fileType)
		{
		case FILE_WAVE_TYPE:
			waveform = PV_ReadIntoMemoryWaveFile(file, decodeData, NULL, NULL, &err);
			break;
		case FILE_AIFF_TYPE:
			waveform = PV_ReadIntoMemoryAIFFFile(file, decodeData, NULL, NULL, &err);
			break;
		case FILE_AU_TYPE:
			waveform = PV_ReadIntoMemorySunAUFile(file, decodeData, NULL, NULL, &err);
			break;
	#if USE_MPEG_DECODER != FALSE
		case FILE_MPEG_TYPE:
			waveform = PV_ReadIntoMemoryMPEGFile(file, decodeData, NULL, NULL, NULL, &err);
			break;
	#endif
		default :
			err = PARAM_ERR;
			break;
		}

		XFileClose(file);
	}
	else
	{
		err = FILE_NOT_FOUND;
	}
	
	if (pErr)
	{
		*pErr = err;
	}
	return waveform;
}

// Read file information from file, which is a fileType file. If pFormat is not NULL, then
// store format specific format type
GM_Waveform * GM_ReadFileInformation(XFILENAME *filename, AudioFileType fileType, 
											long *pFormat, 
											void **ppBlockPtr, unsigned long *pBlockSize, 
											OPErr *pErr)
{
XFILE			file;
OPErr			err;
GM_Waveform*	pWave = NULL;

	if (pBlockSize)
	{
		*pBlockSize = 0;
	}
	file = XFileOpenForRead(filename);
	if (file)
	{
		switch (fileType)
		{
		case FILE_WAVE_TYPE:
			pWave = PV_ReadIntoMemoryWaveFile(file, TRUE, pFormat, pBlockSize, &err);
			break;
		case FILE_AIFF_TYPE:
			pWave = PV_ReadIntoMemoryAIFFFile(file, TRUE, pFormat, pBlockSize, &err);
			break;
		case FILE_AU_TYPE:
			pWave = PV_ReadIntoMemorySunAUFile(file, TRUE, pFormat, pBlockSize, &err);
			break;
#if USE_MPEG_DECODER != FALSE
		case FILE_MPEG_TYPE:
			pWave = PV_ReadIntoMemoryMPEGFile(file, FALSE, pFormat, ppBlockPtr, pBlockSize, &err);
			break;
#endif
		default :
			BAE_ASSERT(FALSE);
			err = PARAM_ERR;
			pWave = NULL;
			break;
		}
		XFileClose(file);
	}
	else
	{
		err = FILE_NOT_FOUND;
	}

	if (pErr)
	{
		*pErr = err;
	}
	return pWave;
}

// given an open file, format types, and a sample position in frames, reseek the file
OPErr GM_RepositionFileStream(XFILE fileReference,
										AudioFileType fileType, long format,
										XPTR pBlockBuffer, unsigned long blockSize,
										short int channels, short int bitSize,
										unsigned long newSampleFramePosition,
										unsigned long firstSampleInFileOffsetInBytes,
										unsigned long *pOuputNewPlaybackPositionInBytes)
{
	OPErr			fileError;
	XBOOL			reSeek;
	short int		frameBlockSize, decodeBlockSize;
	unsigned long	filePos;

	reSeek = FALSE;
	fileError = NO_ERR;
	filePos = 0;
	frameBlockSize = channels * (bitSize / 8);
	decodeBlockSize = 0;
	switch (fileType)
	{
		default:
			fileError = BAD_FILE_TYPE;
			break;
		case FILE_AU_TYPE:
			switch(format)
			{
				default:
					fileError = BAD_FILE_TYPE;
					break;

				case SUN_AUDIO_FILE_ENCODING_LINEAR_16:
				case SUN_AUDIO_FILE_ENCODING_LINEAR_8:
				case SUN_AUDIO_FILE_ENCODING_MULAW_8:
				case SUN_AUDIO_FILE_ENCODING_ALAW_8:
					decodeBlockSize = 2;
					reSeek = TRUE;
					break;

				case SUN_AUDIO_FILE_ENCODING_ADPCM_G721:
				case SUN_AUDIO_FILE_ENCODING_ADPCM_G723_3:
				case SUN_AUDIO_FILE_ENCODING_ADPCM_G723_5:
					decodeBlockSize = 4;
					reSeek = TRUE;
					break;
			}
			break;

#if USE_MPEG_DECODER != 0
		case FILE_MPEG_TYPE:
			if (pBlockBuffer && blockSize)
			{
				XMPEGDecodedData	*pMPG;
				int					bufferSize;

				pMPG = (XMPEGDecodedData *)pBlockBuffer;
				decodeBlockSize = MPG_GetFrameBufferSizeInBytes(pMPG->stream);

				filePos = firstSampleInFileOffsetInBytes + 
								(newSampleFramePosition * frameBlockSize);
				bufferSize = MPG_GetBufferSize(pMPG->stream);

				filePos = (filePos / bufferSize) * decodeBlockSize;
				MPG_SeekStream(pMPG->stream, filePos);
				if (pOuputNewPlaybackPositionInBytes)
				{
					*pOuputNewPlaybackPositionInBytes = firstSampleInFileOffsetInBytes + 
														(newSampleFramePosition * frameBlockSize);
				}
				reSeek = FALSE;
			}
			break;
#endif
		case FILE_AIFF_TYPE:
			switch(format)
			{
				default:
					fileError = BAD_FILE_TYPE;
					break;
				case X_NONE:
					decodeBlockSize = 1;
					reSeek = TRUE;
					break;
				case X_IMA4:
					decodeBlockSize = 4;
					reSeek = TRUE;
					break;
			}
			break;
		case FILE_WAVE_TYPE:				
			switch(format)
			{
				default:
					fileError = BAD_FILE_TYPE;
					break;
				case X_WAVE_FORMAT_PCM:			// normal PCM data
					decodeBlockSize = 1;
					reSeek = TRUE;
					break;
				case X_WAVE_FORMAT_ALAW:
					decodeBlockSize = 2;
					reSeek = TRUE;
					break;
				case X_WAVE_FORMAT_MULAW:
					decodeBlockSize = 2;
					reSeek = TRUE;
					break;
				case X_WAVE_FORMAT_IMA_ADPCM:	// IMA 4 to 1 compressed data
					decodeBlockSize = 4;
					reSeek = TRUE;
					break;
			}
			break;
	}
	if (reSeek)
	{
		unsigned long	pos;

		pos = newSampleFramePosition * frameBlockSize;
		filePos = firstSampleInFileOffsetInBytes + (pos / decodeBlockSize);

		XFileSetPosition(fileReference, filePos);
		if (pOuputNewPlaybackPositionInBytes)
		{
			*pOuputNewPlaybackPositionInBytes = firstSampleInFileOffsetInBytes + pos;
		}
	}
	return fileError;
}

// Read a block of data, based apon file type and format, decode and store into a buffer.
// Return length of buffer stored or 0 if error.
OPErr GM_ReadAndDecodeFileStream(XFILE fileReference, 
										AudioFileType fileType, long format, 
										XPTR pBlockBuffer, unsigned long blockSize,
										XPTR pBuffer, unsigned long bufferFrames,
										short int channels, short int bitSize,
										unsigned long *pStoredBufferLength,
										unsigned long *pReadBufferLength)
{
	unsigned long			returnedLength, writeLength, bufferSize;
	OPErr					fileError;
	long					filePosition;
#if USE_MPEG_DECODER != 0
	long					count, frames;
#endif
	XBOOL					calculateFileSize;

	returnedLength = 0;
	writeLength = 0;
	calculateFileSize = TRUE;
	filePosition = 0;
	fileError = NO_ERR;
	if (pBuffer)
	{
		if (fileReference)
		{
			filePosition = XFileGetPosition(fileReference);
		}
		bufferSize = bufferFrames * channels * (bitSize / 8);
		switch (fileType)
		{
			case FILE_AU_TYPE:
				if (pBlockBuffer)
				{
					fileError = PV_ReadSunAUFile(format, fileReference,
											pBuffer, bufferSize, &writeLength, (SunDecodeState *)pBlockBuffer);
				
					calculateFileSize = FALSE;
					returnedLength = writeLength;
					// adjust for size actually read into the buffer
//					returnedLength = XFileGetPosition(fileReference) - filePosition;	// length of data read from the file
				}
				break;
#if USE_MPEG_DECODER != 0
			case FILE_MPEG_TYPE:
				if (pBlockBuffer)
				{
					XMPEGDecodedData	*pMPG;
					XBOOL				mpegDone;
					char				*pcmAudio;					

					pMPG = (XMPEGDecodedData *)pBlockBuffer;
					frames = bufferSize / pMPG->frameBufferSize;
					pcmAudio = (char *)pBuffer;
					for (count = 0; count < frames; count++)
					{
						fileError = XFillMPEGStreamBuffer(pMPG, pcmAudio, &mpegDone);
						if (mpegDone == FALSE)
						{
							pcmAudio += pMPG->frameBufferSize;
						}
						else
						{	// done
							fileError = BAD_FILE;
							break;
						}
					}
					calculateFileSize = FALSE;
					writeLength = pMPG->frameBufferSize * frames;
					returnedLength = writeLength;
				}
				break;
#endif
			case FILE_AIFF_TYPE:
				switch(format)
				{
					default:
						calculateFileSize = FALSE;
						break;
					case X_NONE:
						// go ahead and read, this call fails when hitting the end,
						// but we don't care because we'll get as much as possible
						// and then calculate the data read by reading the last known
						// position.
						XFileRead(fileReference, 
											pBuffer,
											bufferSize);
						// now, if the file is 8 bit sample, change the sample phase
						if (bitSize == 8)
						{
							XPhase8BitWaveform((unsigned char *)pBuffer, bufferSize);
						}
						#if X_WORD_ORDER != FALSE	// intel?
						// now, if the file is 16 bit sample on a intel ordered system, swap the bytes
						else if (bitSize == 16)
						{
							XSwapShorts((short*)pBuffer, bufferFrames * channels);
						}
						#endif
						break;
					case X_IMA4:
						if (pBlockBuffer)
						{
							fileError = PV_ReadAIFFAndDecompressIMA(fileReference,
														bufferSize / 4,
														(unsigned char *)pBuffer,
														bufferSize,
														(char)bitSize,
														(char)channels,
														(unsigned long *)&writeLength,
														(short*)pBlockBuffer);

							returnedLength = bufferSize / 4;
						}
						else
						{
							writeLength = 0;
							returnedLength = 0;
						}
						calculateFileSize = FALSE;
						break;
				}
				break;
			case FILE_WAVE_TYPE:				
				switch(format)
				{
					default:
						calculateFileSize = FALSE;
						fileError = BAD_FILE_TYPE;
						break;
					case X_WAVE_FORMAT_PCM:			// normal PCM data
						// go ahead and read, this call fails when hitting the end,
						// but we don't care because we'll get as much as possible
						// and then calculate the data read by reading the last known
						// position.
						XFileRead(fileReference, 
												pBuffer,
												bufferSize);
						#if X_WORD_ORDER == FALSE	// motorola?
						// now, if the file is 16 bit sample on a intel ordered system, swap the bytes
						if (bitSize == 16)
						{
							XSwapShorts((short int *)pBuffer, bufferFrames * channels);
						}
						#endif
						break;

					case X_WAVE_FORMAT_ALAW:
						fileError = PV_ReadSunAUFile(SUN_AUDIO_FILE_ENCODING_ALAW_8,
											fileReference, 
											(char *)pBuffer,
											bufferSize,
											&writeLength,
											NULL);
						break;
					case X_WAVE_FORMAT_MULAW:
						fileError = PV_ReadSunAUFile(SUN_AUDIO_FILE_ENCODING_MULAW_8,
											fileReference, 
											(char *)pBuffer,
											bufferSize,
											&writeLength,
											NULL);
						break;
					case X_WAVE_FORMAT_IMA_ADPCM:	// IMA 4 to 1 compressed data
						if (pBlockBuffer)
						{
							fileError = PV_ReadWAVEAndDecompressIMA(fileReference, 
													bufferSize / 4,
													(char *)pBuffer,
													bufferSize,
													(char)bitSize,
													(char)channels,
													pBlockBuffer,
													blockSize,
													(unsigned long *)&writeLength);
							returnedLength = bufferSize / 4;
							// since we decode the samples at runtime, we don't have
							// to byte swap the words.
						}
						else
						{
							writeLength = 0;
							returnedLength = 0;
						}
						calculateFileSize = FALSE;
						break;
				}
				break;
		}
		if (calculateFileSize && fileReference)
		{
			// adjust for size actually read into the buffer
			returnedLength = XFileGetPosition(fileReference) - filePosition;	// length of data read from the file
			writeLength = returnedLength;										// length of data created for audio buffer

			#if USE_DEBUG && 0
			{
				char text[256];
				
				sprintf(text, "%ld %ld", returnedLength, bufferLength);
				DEBUG_STR(XCtoPstr(text));
			}
			#endif
		}
		*pReadBufferLength = returnedLength;
		*pStoredBufferLength = writeLength;
	}
	else
	{
		fileError = PARAM_ERR;
	}
	return fileError;
}


// write memory to a file
OPErr GM_WriteFileFromMemory(XFILENAME *file, GM_Waveform const* pAudioData, AudioFileType fileType)
{
#if USE_CREATION_API == TRUE
	OPErr	err;

	file;
	pAudioData;
	err = NO_ERR;
	switch (fileType)
	{
		case FILE_WAVE_TYPE:
			err = PV_WriteFromMemoryWaveFile(file, pAudioData, X_WAVE_FORMAT_PCM);
			break;
		case FILE_AIFF_TYPE:
			err = PV_WriteFromMemoryAiffFile(file, pAudioData, X_WAVE_FORMAT_PCM);
			break;
		case FILE_AU_TYPE:
			err = PV_WriteFromMemorySunAUFile(file, pAudioData, X_WAVE_FORMAT_PCM);
			break;
#if USE_MPEG_DECODER != FALSE
		case FILE_MPEG_TYPE:
#endif
		default:
			err = NOT_SETUP;
			break;
	}
	return err;
#else
	return NOT_SETUP;
#endif
}



// GM_FinalizeFileHeader()
// ------------------------
// Given an open XFILE and its file type, this will fill in header
// information.  Used when writing output to file and you don't know
// the file size until you're done writing.  This function will fill in
// size fields in the header.
//
OPErr GM_FinalizeFileHeader(XFILE file, AudioFileType fileType)
{
	OPErr err;
	unsigned long chunk;
	unsigned long fileSize;
	unsigned long tmp;
	XERR xerr;

	err = NO_ERR;
	chunk = 0;
	
	if (file)
	{
		switch (fileType)
		{
			case FILE_WAVE_TYPE:
				fileSize = XFileGetLength(file);
				
				XFileSetPosition(file, 0);
				XFileRead(file, &chunk, sizeof(chunk));
				if (XGetLong(&chunk) == X_RIFF)
				{
					XFileSetPosition(file, 8);
					XFileRead(file, &chunk, sizeof(chunk));
					if (XGetLong(&chunk) == X_WAVE)
					{
						XFileSetPosition(file, 4);
						tmp = fileSize-8;
#if X_WORD_ORDER == FALSE // motorola
						tmp = XSwapLong(tmp);
#endif
						XFileWrite(file, &tmp, sizeof(tmp));

						while (XGetLong(&chunk) != X_DATA)
						{
							xerr = XFileSetPositionRelative(file, -3);
							if (xerr != 0)
							{
								err = BAD_FILE;
							}

							xerr = XFileRead(file, &chunk, sizeof(chunk));
							if (xerr != 0)
							{
								err = BAD_FILE;
							}
						}
						tmp = fileSize - XFileGetPosition(file) - sizeof(chunk);
#if X_WORD_ORDER == FALSE // motorola
						tmp = XSwapLong(tmp);
#endif
						xerr = XFileWrite(file, &tmp, sizeof(tmp));
						if (xerr != 0)
						{
							err = BAD_FILE;
						}
					}
				}

			
			case FILE_AIFF_TYPE:
			{
				unsigned long size;
				unsigned short channels;
				unsigned long pos;
				unsigned short bits;
				unsigned long frames;

				fileSize = XFileGetLength(file);
				
				XFileSetPosition(file, 0);
				XFileRead(file, &chunk, sizeof(chunk));
				if (XGetLong(&chunk) == X_FORM)
				{
					XFileSetPosition(file, 8);
					XFileRead(file, &chunk, sizeof(chunk));
					if (XGetLong(&chunk) == X_AIFF)
					{
						XFileSetPosition(file, 4);
						tmp = fileSize;
#if X_WORD_ORDER == TRUE // intel
						tmp = XSwapLong(tmp);
#endif
						XFileWrite(file, &tmp, sizeof(tmp));

						while (XGetLong(&chunk) != X_Common)
						{
							XFileSetPositionRelative(file, -3);
							XFileRead(file, &chunk, sizeof(chunk));
						}
						XFileRead(file, &size, sizeof(size));
						XFileRead(file, &channels, sizeof(channels));
						pos = XFileGetPosition(file); // go back here later to fill in...
						XFileSetPositionRelative(file, 4);
						XFileRead(file, &bits, sizeof(bits));
#if X_WORD_ORDER == TRUE // intel
						size = XSwapLong(size);
						channels = XSwapShort(channels);
						bits = XSwapShort(bits);
#endif

						while (XGetLong(&chunk) != X_SoundData)
						{
							xerr = XFileSetPositionRelative(file, -3);
							if (xerr != 0)
							{
								err = BAD_FILE;
							}

							xerr = XFileRead(file, &chunk, sizeof(chunk));
							if (xerr != 0)
							{
								err = BAD_FILE;
							}
						}
						tmp = fileSize - XFileGetPosition(file) - sizeof(chunk);
						tmp -= 8;	// subtract format header and type from total length
						frames = tmp /*bytes*/ / (bits/8) / channels;
#if X_WORD_ORDER == TRUE // intel
						tmp = XSwapLong(tmp);
#endif
						xerr = XFileWrite(file, &tmp, sizeof(tmp));
						if (xerr != 0)
						{
							err = BAD_FILE;
						}

						XFileSetPosition(file, pos);
#if X_WORD_ORDER == TRUE // intel
						frames = XSwapLong(frames);
#endif
						xerr = XFileWrite(file, &frames, sizeof(frames));
						if (xerr != 0)
						{
							err = BAD_FILE;
						}
					}
				}
			} break;

			case FILE_AU_TYPE:
			{
				unsigned long headerSize;

				fileSize = XFileGetLength(file);
				
				XFileSetPosition(file, 0);
				XFileRead(file, &chunk, sizeof(chunk));
				if (XGetLong(&chunk) == SUN_AUDIO_FILE_MAGIC_NUMBER)
				{
					headerSize = XFileRead(file, &headerSize, sizeof(headerSize));
#if X_WORD_ORDER == TRUE // intel
					headerSize = XSwapLong(headerSize);
#endif
					tmp = fileSize - headerSize;
#if X_WORD_ORDER
					tmp = XSwapLong(tmp);
#endif
					xerr = XFileWrite(file, &tmp, sizeof(tmp));
					if (xerr != 0)
					{
						err = BAD_FILE;
					}
				}
				else
				{
					err = BAD_FILE;
				}
			} break;

			default:
				err = BAD_FILE_TYPE;
				break;
		}
	}
	else
	{
		err = PARAM_ERR;
	}

	return err;
}
#endif	// #if USE_HIGHLEVEL_FILE_API == TRUE

// for 16 bit data. samples are from -32767 to 32768 and 0 is silent
// for 8 bit data. samples are from -127 to 128 and 0 is silent. This is oppsite for BAE
GM_Waveform * GM_ReadRawAudioIntoMemoryFromMemory(void * sampleData,				// pointer to audio data
										unsigned long frames, 			// number of frames of audio
										unsigned short int bitSize, 	// bits per sample 8 or 16
										unsigned short int channels, 	// mono or stereo 1 or 2
										XFIXED rate,					// 16.16 fixed sample rate
										unsigned long loopStart, 		// loop start in frames
										unsigned long loopEnd,			// loop end in frames
										OPErr *pErr)
{
	GM_Waveform 	*pWave = NULL;
	long			size;
	OPErr			err;
	void			*copySampleData;

	err = NO_ERR;
	if (sampleData)
	{
		pWave = GM_NewWaveform();
		if (pWave)
		{
			size = frames * (bitSize / 8) * channels;
			copySampleData = XNewPtr(size);
			if (copySampleData != NULL)
			{
				XBlockMove(sampleData, copySampleData, size);
				pWave->waveSize = size;
				pWave->waveFrames = frames;
				pWave->startLoop = loopStart;
				pWave->endLoop = loopEnd;
				pWave->baseMidiPitch = 60;
				pWave->bitSize = (unsigned char)bitSize;
				pWave->channels = (unsigned char)channels;
				pWave->sampledRate = rate;
				pWave->theWaveform = copySampleData;

				if (bitSize == 8)
				{
					// 8 bit passed in is signed, but internal engine 8 bit data is unsigned.
					XPhase8BitWaveform((XBYTE*)pWave->theWaveform, pWave->waveSize);
				}
			}
			else
			{
				err = MEMORY_ERR;
				XDisposePtr(pWave);
				pWave = NULL;
			}
		}
		else
		{
			err = MEMORY_ERR;
		}
	}
	else
	{
		err = NOT_SETUP;
	}
	if (pErr) *pErr = err;
	return pWave;
}

#if USE_HIGHLEVEL_FILE_API == TRUE
#if USE_CREATION_API == TRUE
OPErr GM_WriteAudioBufferToFile(XFILE file, AudioFileType type, void *buffer, long size, long channels, long sampleSize)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (file)
	{
		switch (type)
		{
			case FILE_WAVE_TYPE:
#if X_WORD_ORDER == FALSE // motorola
				if (sampleSize == 2) // if 16-bit data, we need to flip to little endian
				{
					XSwapShorts((short *)buffer, (long)(size / sampleSize));
				}
#endif
				if (XFileWrite(file, buffer, size) == -1)
				{
					theErr = BAD_FILE;
				}
				break;
						
			case FILE_AIFF_TYPE:
			case FILE_AU_TYPE:
#if X_WORD_ORDER == TRUE // intel?
				if (sampleSize == 2) // if 16-bit data, we need to flip to big endian
				{
					// swap to motorola format
					XSwapShorts((short *)buffer, (long)(size / sampleSize));
				}
#endif
				if (sampleSize == 1) // 8-bit data
				{
					XPhase8BitWaveform((unsigned char*)buffer, size);
				}

				if (XFileWrite(file, buffer, size) == -1)
				{
					theErr = BAD_FILE;
				}
				break;

			default:
				theErr = PARAM_ERR;
				break;
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}
#endif
#endif	// USE_HIGHLEVEL_FILE_API

// EOF of GenSoundFiles.c
