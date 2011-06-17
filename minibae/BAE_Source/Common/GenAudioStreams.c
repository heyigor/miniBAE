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
/***************************************************************************/
/*
**	"GenAudioStreams.c"
**
**	This implements multi source audio streaming code.
**
**	й Copyright 1993-2001 Beatnik, Inc, All Rights Reserved.
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
**	10/5/96		Created
**	11/8/96		Added GM_AudioStreamGetStereoPosition
**	12/30/96	Changed copyright
**	1/24/97		Added fade API & GM_SetAudioStreamFadeRate
**				Changed GM_AudioStreamSetVolume to return 0
**				when stream is dead
**	3/25/97		Fixed a bug when servicing streams and it gets released
**				Removed hard coded constants for stream mode and replaced
**				with way spiffy defines
**	4/24/97		Fixed bug with STREAM_DESTROY that caused recursive callbacks
**	5/3/97		Fixed a few potential problems that the MOT compiler found
**	5/15/97		Reworked file streaming to support compressed streams
**	5/20/97		Fixed a memory leak with GM_AudioStreamFileStart
**	5/21/97		Renamed GM_AudioStreamStart to GM_AudioStreamSetup
**				Seperated load and starting of a stream into two seperate functions:
**				now you call GM_AudioStreamFileSetup OR GM_AudioStreamSetup then
**				call GM_AudioStreamStart
**	6/3/97		Added GM_AudioStreamGetReference
**				Modified GM_AudioStreamStop to not STREAM_DESTROY imeadiately. Instead
**				it calls it at the next GM_AudioServiceStream. This makes sure that
**				the right thread context is setup for the deallocation of memory
**	6/4/97		Changed GM_AudioStreamFileSetup & GM_AudioStreamSetup & 
**				GM_AudioStreamGetData & GM_AudioStreamService to pass in a thread
**				context
**	6/16/97		Added GM_AudioStreamSetVolumeAll
**	6/25/97		Removed extra commented out code in GM_AudioStreamSetup
**				Redid setup and handling of small buffer sizes when getting the first and
**				second buffer. Fixed a bug in which the buffer would stop because the first
**				buffer get was smaller than the actual buffer allocated.
**	6/26/97		Modified PV_StartThisBufferPlaying to not shutdown the buffer if the oppisite
**				buffer length equals zero.
**				Fixed a bug with PV_FileStreamCallback that included extra data after
**				the sample end of file, so it caused playback noise.
**	7/16/97		Incorporated changes from David
**	7/22/97		Changed GM_AudioStreamSetVolumeAll to accecpt -1 as a volume change
**				to reset all volumes and recalculate all volumes
**	8/18/97		Added GM_AudioStreamFlush & GM_AudioStreamGetSampleOffset
**				Eliminated the 10k limit in GM_AudioStreamSetup to new streams buffer sizes.
**				This will have the effect of skipping if the buffer is too small or there is
**				not enough CPU bandwidth to process samples.
**	9/15/97		Added GM_AudioStreamGetReverb
**	9/25/97		Added some underflow condition code to stream engine
**	9/30/97		Eliminated lastAudioStreamErr, and changed GM_AudioStreamError to take
**				a stream reference to get the last error
**	10/13/97	Added new code to handle the underflow condition for the first two buffers
**				of GM_AudioStreamSetup. In that case its handle inside of GM_AudioStreamService.
**	10/16/97	Changed GM_AudioStreamError to return NO_ERR instead of NOT_SETUP if the stream
**				is invalid.
**				Cleaned out old commented out code from PV_AudioBufferCallback
**	11/10/97	Changed some preprocessor tests and flags to explicity test for flags rather
**				than assume
**	11/17/97	Added GM_AudioStreamGetSamplesPlayed & GM_AudioStreamUpdateSamplesPlayed from Kara
**	11/18/97	Fixed more underflow problems
**				Modified GM_AudioStreamPause & GM_AudioStreamResume to work with a particular
**				stream
**				Added GM_AudioStreamResumeAll & GM_AudioStreamPauseAll
**	11/19/97	Fixed a bug in GM_AudioStreamService::STREAM_MODE_DEAD that forgot to set the dataLength
**				Fixed bug with GM_AudioStreamSetup in which the dataLength (in frames) was setup as bytes
**				and fixed a potential bug in which bufferSize was less than round value and it rounded
**				into a negitive (large) number
**	11/24/97	Added GM_AudioStreamFlush
**	12/4/97		Modifed PV_StartThisBufferPlaying to set a flush flag to FALSE. Modified GM_AudioStream to
**				included a flush flag when GM_AudioStreamFlush is called. Modified 
**				GM_AudioStreamService::STREAM_MODE_STOP_STREAM to detect the flush flag and free the stream
**				if the stream is already flushed.
**	12/8/97		Modified GM_AudioStreamStop to pass in a context
**				Changed GM_IsAudioStreamPlaying to check for the stream playing.
**	12/16/97	Moe: removed compiler warnings
**	1/29/98		Added defer parameter to GM_AudioStreamSetVolume
**	2/11/98		Modified for support of Q_48K & Q_24K & Q_8K
**	3/11/98		Added in PV_FreeStream to clear out streamID
**	3/12/98		Modified PV_StartThisBufferPlaying and various API's to implement a callback
**				when the sample is stopped to clear our playbackReference for a particular
**				voice
**	5/7/98		Changed GM_AudioStreamFileSetup to handle error codes
**	7/6/98		Changed usage of streamPlaybackOffset to use constant STREAM_OFFSET_UNSET
**
**	JAVASOFT
**	02.13.98	$$kk: set playbackReference = -1 whenever we call GM_EndSample to avoid jmf stream crossing
**	05.20.98	$$kk: changed GM_AudioStreamStop to handle the shutdown case for a paused stream.
**				otherwise, if the stream is paused, it'll never get shut down properly in
**				GM_AudioStreamService.  need to set up the state so that it can shut down.
**	???			$$kk: added GM_AudioStreamDrain
**	???			$$kk: changed GM_AudioStreamService to allow streams to exist in the engine until all samples played
**	???			$$kk: fixed bugs in GM_AudioStreamUpdateSamplesPlayed
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
**	9/9/98		Modified various routines that use the GM_Waveform structure to use the new elements
**				for file position
**	09.10.98	$$kk: added	STREAM_MODE_INACTIVE mode
**	09.23.98	$$kk: added GM_EventStatus struct and code to generate callbacks whenever
**				playback of a stream starts or stops in response to start(), stop(), or EOM.
**	9/10/98		Fixed a problem when USE_HIGHLEVEL_FILE_API is not defined
**	9/26/98		Changed GM_AudioStreamFileSetup & PV_FileStreamCallback to handle a change in a 
**				Gen API call for block allocations of custom file decoders
**				Fixed a bug with GM_AudioStreamGetFileSamplePosition that returned bytes rather
**				than samples
**	12/3/98		Added GM_GetStreamReverbAmount & GM_SetStreamReverbAmount
**	2/12/99		Renamed USE_BAE_FOR_MPEG to USE_MPEG_DECODER
**	2/25/99		Changed PV_AudioBufferCallback to use XPTR instead of G_PTR
**				Changed playbackReference to use VOICE_REFERENCE
**				Changed callbacks to use a context
**				Removed extra code in GM_AudioStreamStart
**	3/2/99		Changed all stream references to STREAM_REFERENCE rather than
**				the blank 'long'
**				Renamed pStream->reference to pStream->userReference
**	3/3/99		Changed to the new way of starting samples. First a setup, then a start.
**				Changed PV_StartThisBufferPlaying to set streamPlaybackOffset only when actaully
**				starting the voice used by the stream.
**				Added GM_AudioStreamGetFrequencyFilter & GM_AudioStreamSetFrequencyFilter 
**						GM_AudioStreamGetResonanceFilter & GM_AudioStreamSetResonanceFilter 
**						GM_AudioStreamGetLowPassAmountFilter & GM_AudioStreamSetLowPassAmountFilter
**	3/4/99		Broke PV_StartThisBufferPlaying into two functions: PV_PrepareThisBufferForPlaying & 
**				PV_StartStreamBuffers. One for setting up everything and one for actaully starting the
**				playback at the closest point inside of BAE. Required for Group streaming
**	3/5/99		Changed context to threadContext to reflect what is really is used
**				in the system.
**	7/13/99		Renamed HAE to BAE
**	7/27/99		Changed signed var in GM_AudioStreamUpdateSamplesPlayed to unsigned
**				to fix some warnings.
**	1/26/00		Changed PV_FreeStream to free the passed in GM_AudioStream
**				Modified PV_FileStreamCallback to open an mpeg stream
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	10/26/2000	sh	Added GM_AudioStreamSetLoop & GM_AudioStreamGetLoop.
**	12/13/2000	sh	Changed PV_FileStreamCallback to call GM_CreateFileState &
**					GM_DisposeFileState to preserve calls between stream decodes.
**	1/3/2001	sh	Changed PV_FileStreamCallback to use a new function to
**					reposition a stream.
**					Added GM_AudioStreamSetFileSamplePosition, which in turn
**					adds a new message to handle reposition.
**	1/16/2001	sh	Fixed crash in PV_FileStreamCallback.
**	1/17/2001	sh	Changed messages STREAM_MODE_STOP_STREAM & STREAM_MODE_FREE_STREAM
**					inside of GM_AudioStreamService to not actaully kill the
**					stream until its finished playing out.
**				sh	Added GM_AudioStreamGetPlaybackSamplePosition
**					Changed message in STREAM_GET_DATA, function PV_FileStreamCallback
**					to correctly handle the playback position and update it when
**					it passes the end of the file. This is the loop case.
**	1/23/2001	sh	inital pass as position code.
**	1/24/2001	sh	Modified PV_StartStreamBuffers to call PV_RestoreFeatureSettings
**					fix a bug in which a stream lost its attributes (verb, etc)
**	1/25/2001	sh	Fixed stream position display, seeking can still cause
**					this to wig out.
**				sh	Fixed a low buffer condition when looping that might cause
**					extra data to be played at the end of the file.
**	2/15/2001	sh	Initialized pStream in GM_AudioStreamSetup.
**	3/30/2001	sh	Added conditionals for REVERB_DISABLED
**	6/4/2001	sh	Added BAE_PRINTF debug settings.
*/
/***************************************************************************/


// GM_AudioStreamSetup has a bug in which a bad file will now return a NULL
// stream reference but there's no valid error code returned. Can be fixed
// by passing in a error code pointer.

//#define DEBUG_STREAMS				1
//#define TEST_UNDERFLOW_CODE		1


#include "X_API.h"
#include "X_Formats.h"
#include "GenSnd.h"
#include "GenPriv.h"
#include "BAE_API.h"
#include "X_Assert.h"

/* THINGS TO DO ееееееееееееееееееееееееееееееееееееееееее

*/

/*
	Description of use:

	filter controls		GM_AudioStreamGetFrequencyFilter GM_AudioStreamSetFrequencyFilter 
						GM_AudioStreamGetResonanceFilter GM_AudioStreamSetResonanceFilter 
						GM_AudioStreamGetLowPassAmountFilter GM_AudioStreamSetLowPassAmountFilter


	SYNC START
	To start streams at the same time, call one of the GM_AudioStreamSetup or GM_AudioStreamFileSetup functions 
	then call GM_AudioStreamPreroll to get all data ready. Then call GM_SetSyncAudioStreamReference
	with a unique reference. The reference can be a pointer to a local structure. Its not used as anything other
	that common reference for all streams that you need to start at the moment. After they are started it is ignored.
	Then GM_SyncAudioStreamStart to actaully activate the streams. They will start at the next 11 ms slice. Be careful
	using these functions directly because they don't wait for the mixer slice to be ready, so you might actaully
	start streams between 11ms slices. The best way to insure it is to use the linked streams below. Those function
	use the ones described.

	LINKED STREAMS
	Call one of the GM_AudioStreamSetup or GM_AudioStreamFileSetup functions in the various standard ways, to get an 
	allocate stream then call GM_AudioStreamPreroll then call GM_NewLinkedStreamList. Then add it to your maintained 
	top list of linked voices with by calling GM_AddLinkedStream. Use GM_FreeLinkedStreamList to delete an entire list, 
	or GM_RemoveLinkedStream to just one link.

	Then you can call GM_StartLinkedStreams to start them all at the same time, or GM_EndLinkedStreams
	to end them all, or GM_SetLinkedStreamVolume, GM_SetLinkedStreamRate, and GM_SetLinkedStreamPosition
	set parameters about them all.

	management			GM_NewLinkedStreamList GM_FreeLinkedStreamList GM_AddLinkedStream GM_RemoveLinkedStream

	info				GM_GetLinkedStreamPlaybackReference

	control				GM_StartLinkedStreams GM_EndLinkedStreams

	sync control		GM_SetLinkedStreamVolume GM_SetLinkedStreamRate GM_SetLinkedStreamPosition


*/


#if USE_STREAM_API == TRUE
#define STREAM_ID						FOUR_CHAR('L','I','V','E')	//	'LIVE' id for verification of valid stream

#define MAX_SAMPLE_OVERSAMPLE			4		// number of samples extra per buffer

// Stream modes
#define STREAM_MODE_INTERRUPT_ACTIVE	0x80	// this value is or'd onto the overall value
#define STREAM_MODE_DEAD				0x00
#define STREAM_MODE_START_BUFFER_1		0x01	// has to be 1
#define STREAM_MODE_START_BUFFER_2		0x02	// has to be 2
#define STREAM_MODE_STOP_STREAM			0x03
#define STREAM_MODE_FREE_STREAM			0x04

// $$kk: 09.10.98: added this mode.
// we want to be able to PV_AddStream even if we underflowed in  GM_AudioStreamSetup;
// otherwise GM_AudioStreamStart fails with PARAM_ERR because  PV_AudioStreamGetFromReference
// fails.  but we need a mode which allows us to go through the service loop (so we can free
// the stream) without trying to play sound.
#define STREAM_MODE_INACTIVE			0x05


/* Used for file playback */
struct GM_AudioStreamFileInfo
{
	XFILENAME				playbackFile;
	XFILE					fileOpenRef;
	unsigned long			fileStartPosition;		// units are in bytes but as a complete decoded sample
	unsigned long			filePlaybackPosition;	// for example: fileEndPosition for a MP3 file might be 40 MB.
	unsigned long			fileEndPosition;		// these variables are used for positioning and control.
	XBOOL					loopFile;

	GM_SoundDoneCallbackPtr	doneCallback;
	void					*doneCallbackReference;
#if USE_HIGHLEVEL_FILE_API != FALSE
	AudioFileType			fileType;
#endif
	long					formatType;				// typed file compression mode

	XPTR					pBlockBuffer;			// used for decompression
	unsigned long			blockSize;				// used for decompression
};
typedef struct GM_AudioStreamFileInfo GM_AudioStreamFileInfo;

// $$kk: 09.23.98: added this block ->

/* Status of playback events.  Playback events occur when rendering
 * at the _device_ starts or stops.  
 * When an action for which a playback event will be generate happens
 * (start, stop, pause, resume), the event is put in the EVENT_PENDING
 * state.  When it is detected as having occurred in GM_AudioStreamUpdateSamplesPlayed,
 * it is marked EVENT_DETECTED.  When the event representing it is 
 * dispatched in GM_AudioStreamService, it is marked EVENT_RESOLVED.
 */
typedef enum
{
	EVENT_RESOLVED				= 0,
	EVENT_PENDING,
	EVENT_DETECTED
} GM_EventStatus;

/*
 * Describes a playback event: its status and the stream playback frame position 
 * when it occurred.
 */
struct GM_PlaybackEvent
{
	GM_EventStatus status;
	unsigned long framePosition;	
};
typedef struct GM_PlaybackEvent GM_PlaybackEvent;
// $$kk: 09.23.98: end changes <-

#define STREAM_OFFSET_UNSET		(unsigned long)0xFFFFFFFFL

// this structure, once allocated, becomes a STREAM_REFERENCE
struct GM_AudioStream
{
	long					userReference;
	long					streamID;
	VOICE_REFERENCE			playbackReference;	// voice reference to live mixer voice. It
												// will be DEAD_VOICE if not active

	OPErr					startupStatus;		// error return before startup
	short int				startupBufferFullCount;

	GM_StreamObjectProc		streamCallback;
	GM_StreamData			streamData;
	void					*pStreamBuffer;
	unsigned long			streamBufferLength;

	unsigned long			streamPreRollBufferSize;	// size in frames of how many sample frames
														// are prerolled prior to playback. This is
														// set after stream is created. Its used as
														// an offset to streamPlaybackPosition
	unsigned long			streamOrgLength1;
	unsigned long			streamOrgLength2;

	void					*pStreamData1;
	void					*pStreamData2;
	unsigned long			streamLength1;
	unsigned long			streamLength2;
	XBYTE					streamMode;					// Stream modes
	XBYTE					lastStreamBufferPlayed;

	unsigned long			streamPlaybackResetAtPosition;	//  in samples
	unsigned long			streamPlaybackResetToThisPosition;	// in samples

	unsigned long			streamPlaybackPosition;		// in samples; samples in this stream processed by engine
    unsigned long			streamPlaybackOffset;		// in samples; total samples processed by engine when this stream starts
    unsigned long           samplesWritten;             // update in GM_AudioStreamService. total number of samples
    unsigned long           samplesPlayed;              // update in GM_AudioStreamUpdateSamplesPlayed. total number of samples played

// $$kk: 08.12.98 merge: added this field   
	unsigned long           residualSamples;			// if we're underflowing, then get more data and reset streamPlaybackOffset
														// before playing all samples, need to record that we can still play these
														// samples before reaching the new streamPlaybackOffset.

// $$kk: 09.23.98: added these two variables ->
	GM_PlaybackEvent		startEvent;					
	GM_PlaybackEvent		stopEvent;					
// $$kk: 09.23.98: end changes <-
	
	XBOOL					streamPrerolled;				// will be true, if PV_PrepareThisBufferForPlaying has been called
	XBOOL					streamActive;
	XBOOL					streamShuttingDown;
	XBOOL					streamPaused;
	XBOOL					streamFirstTime;			// first time active
	XBOOL					streamUnderflow;
	XBOOL					streamFlushed;				// only set to TRUE when flush is called. Reset to FALSE at start

	XFIXED					streamFadeRate;				// when non-zero fading is enabled
	XFIXED					streamFixedVolume;			// inital volume level that will be changed by streamFadeRate
	short int				streamFadeMaxVolume;		// max volume
	short int				streamFadeMinVolume;		// min volume
	XBOOL					streamEndAtFade;

	// state
#if REVERB_USED != REVERB_DISABLED
	XBOOL					streamUseReverb;
	short int				streamReverbAmount;
#endif
	short int				streamVolume;
	short int				streamStereoPosition;
	short int				streamFrequencyFilter;
	short int				streamLowPassAmountFilter;
	short int				streamResonanceFilter;

	OPErr					streamErr;

	GM_AudioStreamFileInfo	*pFileStream;				// if not NULL, then streaming file
	struct GM_AudioStream	*pNext;
};
typedef struct GM_AudioStream GM_AudioStream;

// linked list of all active streams. Required for servicing. Call GM_AudioStreamService() to process
// all the streams, for fades, callbacks, reads, etc
static GM_AudioStream	*theStreams = NULL;

// verify reference is a valid audio stream structure
static GM_AudioStream * PV_AudioStreamGetFromReference(STREAM_REFERENCE reference)
{
	GM_AudioStream *pStream;
	GM_AudioStream *next;
	
	pStream = (GM_AudioStream *)reference;
	next = theStreams;
	while ( next != NULL )
	{
		if (next == pStream)
		{
			if (pStream->streamID == STREAM_ID)
			{
				return pStream;
			}
		}
		next = next->pNext;
	}
	return NULL;
}

// add a valid stream to the global stream list
static void PV_AddStream(GM_AudioStream *next)
{
	GM_AudioStream *last;

	if (next)
	{
		next->streamID = STREAM_ID;
		if (theStreams == NULL)
		{
			theStreams = next;
		}
		else
		{
			last = theStreams;
			while (last->pNext)
			{
				last = last->pNext;
			}
			last->pNext = next;
		}
		next->pNext = NULL;
	}
}

// remove a stream from the global stream list
static void PV_FreeStream(GM_AudioStream *found)
{
	GM_AudioStream *next, *last;

	last = found;	// preserve in case we fail the valid test
	found = PV_AudioStreamGetFromReference((STREAM_REFERENCE)found);		// verify as valid
	if (found)
	{
		if (found->streamID == STREAM_ID)
		{
			last = next = theStreams;
			while (next)
			{
				if (next == found)						// found object in list?
				{
					if (next == theStreams)				// is object the top object
					{
						theStreams = next->pNext;		// yes, change to next object
					}
					else
					{
						if (last)						// no, change last to point beyond next
						{
							last->pNext = next->pNext;
						}
					}
					if (next->pFileStream)
					{
						XDisposePtr(next->pFileStream->pBlockBuffer);
						XDisposePtr(next->pFileStream);
					}
					next->streamID = 0;
					XDisposePtr(next);					// clean up
					break;
				}
				last = next;
				next = next->pNext;
			}
		}
	}
	else
	{
		// not in list, so just delete the memory
		XDisposePtr((XPTR)last);
	}
}

// Get an empty GM_AudioStream. Will return DEAD_STREAM if can't allocate stream
static STREAM_REFERENCE PV_GetEmptyAudioStream(void)
{
	GM_AudioStream	*pStream;
	STREAM_REFERENCE	ref;

	ref = DEAD_STREAM;
	pStream = (GM_AudioStream *)XNewPtr((long)sizeof(GM_AudioStream));
	if (pStream)
	{
		pStream->userReference = 0;
		pStream->playbackReference = DEAD_VOICE;
		ref = (STREAM_REFERENCE)pStream;
	}
	return ref;
}


static unsigned long PV_GetSampleSizeInBytes(GM_StreamData * pAS)
{
	return pAS->channelSize * (pAS->dataBitSize / 8);
}


static void PV_FillBufferEndWithSilence(char *pDest, GM_StreamData * pAS)
{
	unsigned long	bufferSize, blockSize;
	unsigned long	count;
	short int		*pWData;

	if (pDest)
	{
		blockSize = MAX_SAMPLE_OVERSAMPLE * PV_GetSampleSizeInBytes(pAS);
		bufferSize = (pAS->dataLength * PV_GetSampleSizeInBytes(pAS));

		pDest += bufferSize;
		if (pAS->dataBitSize == 8)
		{
			for (count = 0; count < blockSize; count++)
			{
				*pDest++ = (char)0x80;
			}
		}
		else
		{
			pWData = (short int *)pDest;
			blockSize /= 2;
			for (count = 0; count < blockSize; count++)
			{
				*pWData++ = 0;
			}
		}
	}
}

static void PV_CopyLastSamplesToFirst(char *pSource, char *pDest, GM_StreamData * pAS)
{
	unsigned long	bufferSize, blockSize;

	if (pAS->dataLength && pSource && pDest)
	{
		blockSize = MAX_SAMPLE_OVERSAMPLE * PV_GetSampleSizeInBytes(pAS);
		bufferSize = (pAS->dataLength * PV_GetSampleSizeInBytes(pAS));
		if (bufferSize)
		{
			XBlockMove(pSource + bufferSize, pDest, blockSize);
		}
	}
}

// this callback will only be called in extreme cases in which the voice is starved to death
// and when the stream is forced to stop
static void PV_AudioBufferFinished(void *context)
{
	GM_AudioStream	*pStream;

	pStream = (GM_AudioStream *)context;

// This is the same thing as the above line, just faster.
//	pStream = PV_AudioStreamGetFromReference(context);

	if (pStream)
	{
		pStream->playbackReference = DEAD_VOICE;	// kill our reference to this voice
	}
}

static void PV_AudioBufferCallback(void *context, XPTR pWhichBufferFinished, INT32 *pBufferSize_IN_OUT)
{
	GM_AudioStream			*pStream;
	GM_AudioStreamFileInfo	*pASInfo;

//#pragma unused (pWhichBufferFinished)
	pWhichBufferFinished = pWhichBufferFinished;

	pStream = (GM_AudioStream *)context;

// This is the same thing as the above line, just faster.
//	pStream = PV_AudioStreamGetFromReference(context);

	if (pStream)
	{
		#if DEBUG_STREAMS && 0
			BAE_PRINTF("Playback Pos %ld Length %ld\r", pStream->streamPlaybackPosition, *pBufferSize_IN_OUT);
		#endif
		#if DEBUG_STREAMS && 0
			BAE_PRINTF("buffer done %lx size %ld", pWhichBufferFinished, *pBufferSize_IN_OUT);
		#endif
		pStream->streamPlaybackPosition += *pBufferSize_IN_OUT;

		pASInfo = pStream->pFileStream;
		if (pASInfo)
		{
			// ok, now this checks the playback position and resets it if
			// we're looping. We can't do this at the loop because we're
			// reading ahead.
			if (pASInfo->loopFile)
			{
				if (pStream->streamPlaybackResetAtPosition)
				{
					if (pStream->streamPlaybackPosition >= pStream->streamPlaybackResetAtPosition)
					{
						pStream->streamPlaybackPosition =  pStream->streamPlaybackResetToThisPosition;
						pStream->streamPlaybackResetAtPosition = 0;	// off
						pStream->streamPlaybackResetToThisPosition = 0; // off
					}
				}
			}
		}

		switch (pStream->streamMode & (~STREAM_MODE_INTERRUPT_ACTIVE))
		{
			default:
				#if DEBUG_STREAMS
				BAE_PRINTF("Bad case in PV_AudioBufferCallback");
				#endif
				break;
			case STREAM_MODE_STOP_STREAM:
				#if DEBUG_STREAMS
				BAE_PRINTF("PV_AudioBufferCallback::STREAM_MODE_STOP_STREAM");
				#endif
				pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_FREE_STREAM;		// end
				pStream->streamShuttingDown = TRUE;
				*pBufferSize_IN_OUT = 0;
				break;
			// buffer 1 ends playback
			case STREAM_MODE_START_BUFFER_1:	// start buffer 2 playing
				#if DEBUG_STREAMS
				BAE_PRINTF("PV_AudioBufferCallback::STREAM_MODE_START_BUFFER_1");
				#endif
				if (pStream->streamFirstTime)
				{
					// copy end of buffer 2 into the start of buffer 1
					// This only needs to happen once at the start because the buffers are different in the begining
					PV_CopyLastSamplesToFirst((char *)pStream->pStreamData2, (char *)pStream->pStreamData1, &pStream->streamData);
					pStream->streamFirstTime = FALSE;
				}

				*pBufferSize_IN_OUT = pStream->streamLength2;
				if (pStream->streamShuttingDown)
				{
					pStream->streamShuttingDown = TRUE;
					#if DEBUG_STREAMS
					BAE_PRINTF("    End of BUFFER_1");
					#endif
					if (pStream->streamLength2)
					{
						pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_STOP_STREAM;		// end
					}
					else
					{
						pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_FREE_STREAM;		// end
					}
				}
				else
				{
					pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_START_BUFFER_2;		// buffer1 read and playing2
				}
				break;
			// buffer 2 ends playback
			case STREAM_MODE_START_BUFFER_2:
				#if DEBUG_STREAMS
				BAE_PRINTF("PV_AudioBufferCallback::STREAM_MODE_START_BUFFER_2");
				#endif
				*pBufferSize_IN_OUT = pStream->streamLength1;
				if (pStream->streamShuttingDown)
				{
					pStream->streamShuttingDown = TRUE;
					#if DEBUG_STREAMS
					BAE_PRINTF("    End of BUFFER_2");
					#endif
					if (pStream->streamLength1)
					{
						pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_STOP_STREAM;		// end
					}
					else
					{
						pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_FREE_STREAM;		// end
					}
				}
				else
				{
					pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_START_BUFFER_1;		// buffer2 read and playing1
				}
				break;
		}
	}
}

// restore active voice to this streams specific attributes.
static void PV_RestoreFeatureSettings(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		GM_AudioStreamSetLowPassAmountFilter(reference, GM_AudioStreamGetLowPassAmountFilter(reference));
		GM_AudioStreamSetResonanceFilter(reference, GM_AudioStreamGetResonanceFilter(reference));
		GM_AudioStreamSetFrequencyFilter(reference, GM_AudioStreamGetFrequencyFilter(reference));
		GM_AudioStreamSetFrequencyFilter(reference, GM_AudioStreamGetFrequencyFilter(reference));
#if REVERB_USED != REVERB_DISABLED
		GM_SetStreamReverbAmount(reference, GM_GetStreamReverbAmount(reference));
		GM_AudioStreamReverb(reference, GM_AudioStreamGetReverb(reference));
#endif
	}		
}

// Call this after calling PV_PrepareThisBufferForPlaying. All this function does is start
// the voice playing and record its start time in the stream. Its already allocated and ready.
static void PV_StartStreamBuffers(GM_AudioStream * pStream)
{
	if (pStream)
	{
		if (pStream->streamPrerolled)
		{
		    // if it is currently unset (STREAM_OFFSET_UNSET value), set the streamPlaybackOffset 
		    // to the current position of the engine in samples
		    if (pStream->streamPlaybackOffset == STREAM_OFFSET_UNSET)
		    {
		        pStream->streamPlaybackOffset = MusicGlobals->samplesWritten;
			}
			if (GM_StartSample(pStream->playbackReference) == NO_ERR)
			{
				PV_RestoreFeatureSettings((STREAM_REFERENCE)pStream);
			}
		}
	}
}

// setup and allocate an BAE voice for a playing buffer. Account for MAX_SAMPLE_OVERSAMPLE in length
static XBOOL PV_PrepareThisBufferForPlaying(GM_AudioStream * pStream, XBYTE bufferNumber)
{
	XBYTE	mode;

	pStream->streamPrerolled = FALSE;
// $$kk: 08.12.98 merge: added this block 
	// $$kk: we use (pStream->streamPlaybackOffset == STREAM_OFFSET_UNSET) to indicate that the
	// value needs to be set.  this is the case 1) when the stream is just
	// starting up and 2) when the stream has shut down (probably due to
	// underflow).  this is now the centralized location for setting
	// pStream->streamPlaybackOffset to STREAM_OFFSET_UNSET during play.  (before, there were
	// potential timing problems, and we didn't necessarily count all the
	// committed samples....

	if (GM_IsSoundDone(pStream->playbackReference))
	{
		// if we were set to a valid offset before (i.e. we're not just starting)
		// record any residual samples that we should count.
		if (pStream->streamPlaybackOffset != STREAM_OFFSET_UNSET)
		{
			pStream->residualSamples = pStream->samplesWritten - pStream->samplesPlayed;
		}
		pStream->streamPlaybackOffset = STREAM_OFFSET_UNSET;
	}
// $$kk: 08.12.98 merge: end new block  

	if (pStream->streamShuttingDown == FALSE)
	{
		switch (bufferNumber)
		{
			case 0:	// don't know which buffer to play, figure it out, eh?
				DEBUG_STR("PV_PrepareThisBufferForPlaying:0");
				if (pStream->lastStreamBufferPlayed)
				{
					mode = pStream->streamMode &= (~STREAM_MODE_INTERRUPT_ACTIVE);
					if (mode != pStream->lastStreamBufferPlayed)
					{
						DEBUG_STR("DIFFERENT!!");
						mode = pStream->lastStreamBufferPlayed;
					}
					pStream->streamPrerolled = PV_PrepareThisBufferForPlaying(pStream, mode);	// play last successfull buffer played
				}
				break;

			case STREAM_MODE_START_BUFFER_1:
				DEBUG_STR("PV_PrepareThisBufferForPlaying:STREAM_MODE_START_BUFFER_1");
				if (pStream->streamLength1)
				{
					pStream->playbackReference =
						GM_SetupSampleDoubleBuffer(	(XPTR)pStream->pStreamData1,
												(XPTR)pStream->pStreamData2,
												pStream->streamLength1,
												pStream->streamData.sampleRate,
												pStream->streamData.dataBitSize, pStream->streamData.channelSize,
												pStream->streamVolume,
												pStream->streamStereoPosition,
												(void *)pStream,
												PV_AudioBufferCallback,
												PV_AudioBufferFinished);
					if (pStream->playbackReference != DEAD_VOICE)	// successfull?
					{
						pStream->lastStreamBufferPlayed = STREAM_MODE_START_BUFFER_1;
						pStream->streamPrerolled = TRUE;
/*
						// if it is currently unset (STREAM_OFFSET_UNSET value), set the streamPlaybackOffset 
						// to the current position of the engine in samples
						if (pStream->streamPlaybackOffset == STREAM_OFFSET_UNSET)
						{
							pStream->streamPlaybackOffset = MusicGlobals->samplesWritten;
						}
						GM_StartSample(pStream->playbackReference);
*/
					}
					pStream->streamFlushed = FALSE;
				}
				break;
			case STREAM_MODE_START_BUFFER_2:
				DEBUG_STR("PV_PrepareThisBufferForPlaying:STREAM_MODE_START_BUFFER_2");
				if (pStream->streamLength2)
				{
					pStream->playbackReference =
						GM_SetupSampleDoubleBuffer(	(XPTR)pStream->pStreamData2,
												(XPTR)pStream->pStreamData1,
												pStream->streamLength2,
												pStream->streamData.sampleRate,
												pStream->streamData.dataBitSize, pStream->streamData.channelSize,
												pStream->streamVolume,
												pStream->streamStereoPosition,
												(void *)pStream,
												PV_AudioBufferCallback,
												PV_AudioBufferFinished);
					if (pStream->playbackReference != DEAD_VOICE)	// successfull?
					{
						pStream->lastStreamBufferPlayed = STREAM_MODE_START_BUFFER_2;
						pStream->streamPrerolled = TRUE;
/*
		                // if it is currently unset (STREAM_OFFSET_UNSET value), set the streamPlaybackOffset 
		                // to the current position of the engine in samples
		                if (pStream->streamPlaybackOffset == STREAM_OFFSET_UNSET)
		                {
		                    pStream->streamPlaybackOffset = MusicGlobals->samplesWritten;
						}
						GM_StartSample(pStream->playbackReference);
*/
					}
					pStream->streamFlushed = FALSE;
				}
				break;
		}
	}
	return pStream->streamPrerolled;
}

//$$kk: 10.22.97
// i am taking out these pause/resume methods and replacing them with the
// last version; these for some reasons cause synch to be off ON WIN32 ONLY
// after pause/resume

// Pause this particular audio stream
// $$kk: 08.12.98 merge: changed this function to avoid stream crossing
void GM_AudioStreamPause(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive && (!pStream->streamPaused))
		{
			//  $$kk: 02.13.98: set playbackReference = DEAD_VOICE whenever we call GM_EndSample to avoid jmf stream crossing
			VOICE_REFERENCE playbackReference = pStream->playbackReference;

// $$kk: 09.23.98: added this ->
			pStream->stopEvent.status = EVENT_PENDING;
// $$kk: 09.23.98: end changes <-

			pStream->streamPaused = TRUE;

			pStream->playbackReference = DEAD_VOICE;
			GM_EndSample(playbackReference);
		}
	}
}

// Resume this particular audio stream
// $$kk: 08.12.98 merge: changed this function to avoid stutter
void GM_AudioStreamResume(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive && pStream->streamPaused)
		{
			pStream->streamPaused = FALSE;

			// $$kk: 02.20.98
			// if the stream is flushed and STREAM_MODE_DEAD, should not start playing here!
			// this change should fix the JMF stutter problem, bug #4098200
			if (!(pStream->streamFlushed))
			{
				if (PV_PrepareThisBufferForPlaying(pStream, (XBYTE)(pStream->streamMode & (~STREAM_MODE_INTERRUPT_ACTIVE))))
				{
					PV_StartStreamBuffers(pStream);
				}
			}
		}
	}
}



// Pause all audio streams
// $$kk: 08.12.98 merge: changed this function to avoid stream crossing
void GM_AudioStreamPauseAll(void)
{
	GM_AudioStream	*pStream;

	pStream = theStreams;
	while (pStream)
	{
		if (pStream->streamActive && (!pStream->streamPaused))
		{
			//  $$kk: 02.13.98: set playbackReference = DEAD_VOICE whenever we call GM_EndSample to avoid jmf stream crossing
			VOICE_REFERENCE playbackReference = pStream->playbackReference;

			pStream->streamPaused = TRUE;

			pStream->playbackReference = DEAD_VOICE;
			GM_EndSample(playbackReference);
		}
		pStream = pStream->pNext;
	}
}

// Resume all audio streams
void GM_AudioStreamResumeAll(void)
{
	GM_AudioStream	*pStream;

	pStream = theStreams;
	while (pStream)
	{
		if (pStream->streamActive && pStream->streamPaused)
		{
			pStream->streamPaused = FALSE;
			if (PV_PrepareThisBufferForPlaying(pStream, (XBYTE)(pStream->streamMode & (~STREAM_MODE_INTERRUPT_ACTIVE))))
			{
				PV_StartStreamBuffers(pStream);
			}
		}
		pStream = pStream->pNext;
	}
}

OPErr GM_AudioStreamError(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;
	OPErr				theErr;

	theErr = NO_ERR;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		theErr = pStream->streamErr;
	}
	return theErr;
}

#if USE_HIGHLEVEL_FILE_API
// streaming file callback. Used for GM_AudioStreamFileStart to decode typed files.
static OPErr PV_FileStreamCallback(void *context, GM_StreamMessage message, GM_StreamData *pAS)
{
	OPErr					error;
	unsigned long			bufferSize, fileSize, outputBufferSize;
	GM_AudioStreamFileInfo	*pASInfo;
	GM_AudioStream			*pStream;
	short int				blockAlign;

#if TEST_UNDERFLOW_CODE
	static int				pv_count = 0;
	static int				pv_count2 = 0;
#endif

	context = context;
	error = NO_ERR;
	switch (message)
	{
		default:
			error = NOT_SETUP;
			break;
		case STREAM_CREATE:
			#if DEBUG_STREAMS
			BAE_PRINTF("PV_FileStreamCallback::STREAM_CREATE\r");
			#endif
			pStream = (GM_AudioStream *)pAS->streamReference;
			pASInfo = (GM_AudioStreamFileInfo *)pAS->userReference;
			pStream->pFileStream = pASInfo;

			switch (pASInfo->fileType)
			{
#if USE_MPEG_DECODER != 0
				case FILE_MPEG_TYPE:
					pASInfo->fileOpenRef = XFileOpenForRead(&pASInfo->playbackFile);
					if (pASInfo->fileOpenRef)
					{
						XMPEGDecodedData *pFileStream;

						pFileStream = XOpenMPEGStreamFromXFILE(pASInfo->fileOpenRef, &error);
						pASInfo->pBlockBuffer = (XPTR)pFileStream;

						// remember to take in account that the data length passed is always in audio frames not bytes
						bufferSize = pAS->dataLength * pAS->channelSize * (pAS->dataBitSize / 8);
						pAS->pData = XNewPtr(bufferSize);
						if (pAS->pData)
						{
							error = NO_ERR;
						}
						else
						{
							error = MEMORY_ERR;
						}
					}
					else
					{
						error = GENERAL_BAD;
					}
					break;
#endif
				default:
					pASInfo->fileOpenRef = XFileOpenForRead(&pASInfo->playbackFile);
					if (pASInfo->fileOpenRef)
					{
						pASInfo->pBlockBuffer = GM_CreateFileState(pASInfo->fileType);

						// remember to take in account that the data length passed is always in audio frames not bytes
						bufferSize = pAS->dataLength * pAS->channelSize * (pAS->dataBitSize / 8);

						pAS->pData = XNewPtr(bufferSize);
						if (pAS->pData)
						{
							error = NO_ERR;
							XFileSetPosition(pASInfo->fileOpenRef, pASInfo->filePlaybackPosition);
						}
						else
						{
							error = MEMORY_ERR;
						}
					}
					else
					{
						error = GENERAL_BAD;
					}
			}
			break;
		case STREAM_DESTROY:
			#if DEBUG_STREAMS
			BAE_PRINTF("PV_FileStreamCallback::STREAM_DESTROY\r");
			#endif
			pASInfo = (GM_AudioStreamFileInfo *)pAS->userReference;

			switch (pASInfo->fileType)
			{
#if USE_MPEG_DECODER != 0
				case FILE_MPEG_TYPE:
					if (pASInfo->fileOpenRef)
					{
						XCloseMPEGStream((XMPEGDecodedData *)pASInfo->pBlockBuffer);					
						XFileClose(pASInfo->fileOpenRef);
						pASInfo->fileOpenRef = 0;
					}
					break;
#endif
				default:
					GM_DisposeFileState(pASInfo->fileType, (void *)pASInfo->pBlockBuffer);
					if (pASInfo->fileOpenRef)
					{
						XFileClose(pASInfo->fileOpenRef);
						pASInfo->fileOpenRef = 0;
					}
					break;
			}
			if (pASInfo->doneCallback)
			{
				(*pASInfo->doneCallback)(pASInfo->doneCallbackReference);
			}
			XDisposePtr((XPTR)pAS->pData);
			pAS->pData = NULL;
			break;

		case STREAM_SET_POSITION:
			{
				unsigned long	samplePosition;

				pStream = (GM_AudioStream *)pAS->streamReference;
				pASInfo = (GM_AudioStreamFileInfo *)pAS->userReference;
				blockAlign = (short int)PV_GetSampleSizeInBytes(pAS);
				samplePosition = pAS->framePosition * blockAlign;	// convert from bytes to samples
				if (samplePosition < pASInfo->fileEndPosition)
				{
					pASInfo->filePlaybackPosition = pASInfo->fileStartPosition + samplePosition;

					// ok now we know we're in range, so seek the file
					error = GM_RepositionFileStream(pASInfo->fileOpenRef,
															pASInfo->fileType,
															pASInfo->formatType,
															pASInfo->pBlockBuffer,
															pASInfo->blockSize,
															pAS->channelSize,
															pAS->dataBitSize,
															pAS->framePosition,
															pASInfo->fileStartPosition,
															&pASInfo->filePlaybackPosition);
				}
				else
				{
					error = OUT_OF_RANGE;
				}
			}
			break;

		case STREAM_GET_DATA:
#if TEST_UNDERFLOW_CODE
			pv_count++;
			if (pv_count == 0)
			{
				pAS->dataLength = 0;
				DEBUG_STR("Fake underflow!");
				return NO_ERR;
			}
#endif
			pStream = (GM_AudioStream *)pAS->streamReference;
			pASInfo = (GM_AudioStreamFileInfo *)pAS->userReference;

			blockAlign = (short int)PV_GetSampleSizeInBytes(pAS);
			if (pAS->pData)
			{
				// get the desired length, and account for stereo and bit size
				error  = GM_ReadAndDecodeFileStream(	pASInfo->fileOpenRef,
															pASInfo->fileType,
															pASInfo->formatType,
															pASInfo->pBlockBuffer,
															pASInfo->blockSize,
															(XPTR)pAS->pData,
															pAS->dataLength,
															pAS->channelSize,
															pAS->dataBitSize,
															&outputBufferSize,
															&fileSize);

				#if DEBUG_STREAMS && 1
					BAE_PRINTF("STREAM_GET_DATA::frames %ld outputBufferSize %ld fileSize %ld", pAS->dataLength, outputBufferSize, fileSize);
				#endif
				// NOTE: we are incrementing the file position by the buffer size rather
				// than the file size because our units are in audio bytes.
				pASInfo->filePlaybackPosition += outputBufferSize;

				// return actual length, if this is not done, then it will
				// continue to play the last buffer size
				// NOTE: we're returned the number of audio frames, not the actual byte size
				pAS->dataLength = outputBufferSize / blockAlign;
			
				if (pASInfo->filePlaybackPosition >= pASInfo->fileEndPosition)
				{
					// how much over are we? Our fileEndPosition is at the exact end of the file,
					// so if we're over then there's extra data after the sample data. We don't want to
					// include that in the playback because its noise.
					fileSize = pASInfo->filePlaybackPosition - pASInfo->fileEndPosition;
					pASInfo->filePlaybackPosition -= fileSize;

					fileSize /= blockAlign;		// convert to frames
					if (fileSize > pAS->dataLength)
					{
						pAS->dataLength = 0;
					}
					else
					{
						pAS->dataLength -= fileSize;
					}

					error = NO_ERR;
					if (pASInfo->loopFile)
					{
						unsigned long	savePos;

						pStream->streamPlaybackResetAtPosition = (pASInfo->filePlaybackPosition -
																	pASInfo->fileStartPosition) / blockAlign;
						pStream->streamPlaybackResetToThisPosition = 0;
/*
						pStream->streamPlaybackResetAtPosition = (pASInfo->filePlaybackPosition -
																	pASInfo->fileStartPosition) / blockAlign;

						pStream->streamPlaybackResetToThisPosition = pASInfo->fileStartPosition / blockAlign;
*/
						savePos = pStream->streamPlaybackPosition;
						error = GM_AudioStreamSetFileSamplePosition((STREAM_REFERENCE)pAS->streamReference,
 										pStream->streamPlaybackResetToThisPosition);
						pStream->streamPlaybackPosition = savePos;


						switch (pASInfo->fileType)
						{
#if USE_MPEG_DECODER != 0
							case FILE_MPEG_TYPE:
								// do nothing, because in this case pBlockBuffer is our mpeg stream
								break;
#endif
							default:
								// clear transient data
								XSetMemory(pASInfo->pBlockBuffer, pASInfo->blockSize, 0);
								break;
						}
					}
					else
					{
						error = STREAM_STOP_PLAY;	// we've hit the end of the file, so stop
					}
				}
				else
				{
					error = NO_ERR;
				}
			}
			else
			{
				error = GENERAL_BAD;
			}
			break;
	}
	return error;
}
#endif	// USE_HIGHLEVEL_FILE_API

#if USE_HIGHLEVEL_FILE_API != FALSE
// Set the done callback and reference flag of a audio stream
void GM_AudioStreamSetDoneCallback(STREAM_REFERENCE reference, GM_SoundDoneCallbackPtr doneCallback, void *doneCallbackReference)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->pFileStream)
		{
			pStream->pFileStream->doneCallback = doneCallback;
			pStream->pFileStream->doneCallbackReference = doneCallbackReference;
		}
	}
}

// Get the done callback and reference flag of a audio stream
void * GM_AudioStreamGetDoneCallback(STREAM_REFERENCE reference, GM_SoundDoneCallbackPtr *pDoneCallback)
{
	GM_AudioStream			*pStream;
	GM_SoundDoneCallbackPtr	doneCallback;
	void					*doneCallbackReference;

	doneCallback = NULL;
	reference = NULL;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->pFileStream)
		{
			doneCallback = pStream->pFileStream->doneCallback;
			doneCallbackReference = pStream->pFileStream->doneCallbackReference;
		}
	}
	if (pDoneCallback)
	{
		*pDoneCallback = doneCallback;
	}
	return doneCallbackReference;
}
#endif	//#if USE_HIGHLEVEL_FILE_API != FALSE

#if USE_HIGHLEVEL_FILE_API != FALSE
// Set the loop flag of a audio stream
void GM_AudioStreamSetLoop(STREAM_REFERENCE reference, XBOOL loopFile)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->pFileStream)
		{
			pStream->pFileStream->loopFile = loopFile;
		}
	}
}

// Get the loop flag of a audio stream
XBOOL GM_AudioStreamGetLoop(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	XBOOL			loopFile;

	loopFile = FALSE;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->pFileStream)
		{
			loopFile = pStream->pFileStream->loopFile;
		}
	}
	return loopFile;
}
#endif

#if USE_HIGHLEVEL_FILE_API != FALSE
// setup streaming a file and place it into pause mode. Don't start
STREAM_REFERENCE GM_AudioStreamFileSetup(void *threadContext,
									XFILENAME *file, AudioFileType fileType,
									unsigned long bufferSize, GM_Waveform *pFileInfo,
									XBOOL loopFile)
{
	STREAM_REFERENCE		reference;
	GM_Waveform				*pWaveform;
	GM_AudioStreamFileInfo	*pStream;
	long					format;
	unsigned long			blockSize;
	OPErr					err;
	void					*blockPtr;

	reference = DEAD_STREAM;
	blockPtr = NULL;
	pWaveform = GM_ReadFileInformation(file, fileType, &format, &blockPtr, &blockSize, &err);
	if (pWaveform && (err == NO_ERR))
	{
		pStream = (GM_AudioStreamFileInfo *)XNewPtr((long)sizeof(GM_AudioStreamFileInfo));
		if (pStream)
		{
			pStream->playbackFile = *file;
			pStream->loopFile = loopFile;
			pStream->formatType = format;
			pStream->fileType = fileType;
			pStream->blockSize = blockSize;
			if (blockPtr)
			{
				pStream->pBlockBuffer = blockPtr;
			}
			else if (blockSize)
			{
				pStream->pBlockBuffer = XNewPtr(blockSize);
			}
			// now the file is positioned right at the data block
			pStream->filePlaybackPosition = pWaveform->currentFilePosition;
			pStream->fileStartPosition = pWaveform->currentFilePosition;
			// we want the byte size
			pStream->fileEndPosition = pWaveform->waveSize + pStream->fileStartPosition;

			if (pFileInfo)
			{
				*pFileInfo = *pWaveform;
			}
			if (blockSize)
			{
				bufferSize = (bufferSize * blockSize) / blockSize;	// round down to blockSize
			}
			reference = GM_AudioStreamSetup(threadContext, (long)pStream, PV_FileStreamCallback,
												bufferSize,
												pWaveform->sampledRate,
												pWaveform->bitSize,
												pWaveform->channels);
		}
		XDisposePtr(pWaveform);
	}
	return reference;
}
#endif // USE_HIGHLEVEL_FILE_API

// This will start a streaming audio object.
//
// INPUT:
//	userReference	This is a reference value that will be returned and should be passed along to all GM_AudioStream
//					functions.
//
//	pProc		is a GM_StreamObjectProc proc pointer. At startup of the streaming the proc will be called
//				with STREAM_CREATE, then followed by two STREAM_GET_DATA calls to get two buffers of data,
//				and finally STREAM_DESTROY when finished.
//
// OUTPUT:
//	long			This is an audio reference number. Will be non-zero for valid stream

STREAM_REFERENCE GM_AudioStreamSetup(void *threadContext, long userReference, GM_StreamObjectProc pProc, 						
							unsigned long bufferSize, 
							XFIXED sampleRate,	// Fixed 16.16 sample rate
							char dataBitSize,		// 8 or 16 bit data
							char channelSize)		// 1 or 2 channels of date
{
	STREAM_REFERENCE	reference;
	GM_AudioStream		*pStream;
	GM_StreamData		ssData;
	OPErr				theErr;
	unsigned long		byteLength;

	reference = DEAD_STREAM;
	theErr = NO_ERR;
	pStream = NULL;
	#if DEBUG_STREAMS
		BAE_PRINTF("StreamSetup: Starting.\n");
	#endif
	if (MusicGlobals->MaxEffects)
	{
//		if (bufferSize >= 10000L)
		{
			if ( (pProc) && ( (channelSize >= 1) || (channelSize <= 2) ) && ( (dataBitSize == 8) || (dataBitSize == 16) ) )
			{
				reference = PV_GetEmptyAudioStream();
				if (reference)
				{
					pStream = (GM_AudioStream *)reference;
					pStream->streamCallback = pProc;
					pStream->streamUnderflow = FALSE;
					pStream->userReference = userReference;
					pStream->streamPlaybackPosition = 0;
					pStream->streamPlaybackResetAtPosition = 0;
					pStream->streamPlaybackResetToThisPosition = 0;
					pStream->streamPlaybackOffset = STREAM_OFFSET_UNSET;     // not known
					pStream->samplesWritten = 0;
					pStream->samplesPlayed = 0;

					// $$kk: 08.12.98 merge 
					// $$kk: added residualSamples
					pStream->residualSamples = 0; 

// $$kk: 09.23.98: added this block ->
					pStream->startEvent.status = EVENT_RESOLVED;
					pStream->startEvent.framePosition = 0;
					pStream->stopEvent.status = EVENT_RESOLVED;
					pStream->stopEvent.framePosition = 0;
// $$kk: 09.23.98: end changes <-

					pStream->streamShuttingDown = FALSE;
					pStream->streamVolume = MAX_NOTE_VOLUME;
					pStream->streamStereoPosition = 0;
					ssData.pData = NULL;
					ssData.userReference = pStream->userReference;
					ssData.streamReference = (STREAM_REFERENCE)pStream;
					ssData.sampleRate = sampleRate;
					ssData.dataBitSize = dataBitSize;
					ssData.channelSize = channelSize;
					if (bufferSize > 1000)
					{
						bufferSize -= (bufferSize % 1000L);		// round down to the nearest 100
					}
					// add over sample extra
					bufferSize += PV_GetSampleSizeInBytes(&ssData) * MAX_SAMPLE_OVERSAMPLE;
					pStream->streamBufferLength = bufferSize;

					// convert from bytes to frames
					ssData.dataLength = bufferSize / PV_GetSampleSizeInBytes(&ssData);

					theErr = (*pProc)(threadContext, STREAM_CREATE, &ssData);

					if (theErr == NO_ERR)
					{
						pStream->streamData = ssData;
						pStream->pStreamBuffer = ssData.pData;
						pStream->pStreamData1 = pStream->streamData.pData;

						byteLength = ssData.dataLength * PV_GetSampleSizeInBytes(&ssData);
						pStream->pStreamData2 = (char *)pStream->streamData.pData + (byteLength / 2);
						pStream->streamLength1 = ssData.dataLength / 2;
						pStream->streamLength2 = ssData.dataLength / 2;

						pStream->streamOrgLength1 = pStream->streamLength1;
						pStream->streamOrgLength2 = pStream->streamLength2;
						pStream->streamMode = STREAM_MODE_DEAD;
						pStream->streamUnderflow = FALSE;
						pStream->startupBufferFullCount = 0;

						// ok, fill first buffer
						ssData.userReference = pStream->userReference;
						ssData.streamReference = (STREAM_REFERENCE)pStream;
						ssData.pData = pStream->pStreamData1;
						// get the full amount this buffer only
						ssData.dataLength = pStream->streamLength1;
						ssData.dataBitSize = dataBitSize;
						ssData.channelSize = channelSize;
						ssData.sampleRate = sampleRate;
						#if DEBUG_STREAMS
							BAE_PRINTF("StreamSetup: Call callback with GET_DATA\n");
						#endif
						theErr = (*pProc)(threadContext, STREAM_GET_DATA, &ssData);

#if DEBUG_STREAMS
						if( theErr)
						{
							BAE_PRINTF("StreamSetup: Return from 1st callback with an error\n");
						}
						else
						{
							BAE_PRINTF("StreamSetup: Return 1st from callback o.k.\n");
						}
#endif
						pStream->streamLength1 = ssData.dataLength;			// just in case it changes

						pStream->streamPreRollBufferSize = pStream->streamLength1;

						if (pStream->streamLength1 == 0)
						{	// underflow, get this buffer again
							pStream->streamUnderflow = TRUE;

							// $$kk: 09.10.98: added these two lines.  need to PV_AddStream; otherwise AudioStreamStart 
							// will fail (cannot get stream from reference: PARAM_ERR).  we use STREAM_MODE_INACTIVE
							// to avoid trying to play sound in GM_AudioStreamService.  GM_AudioStreamStart will set
							// STREAM_MODE_DEAD; then we'll start trying to read data.
							pStream->streamMode = STREAM_MODE_INACTIVE;
							PV_AddStream(pStream);		// ok add stream
						
						}
						else
						{
							pStream->startupBufferFullCount++;	// first buffer full
							if ( (theErr == NO_ERR) || (theErr == STREAM_STOP_PLAY) )
							{
								if (theErr == NO_ERR)
								{
									// update count of samples written
									pStream->samplesWritten += pStream->streamLength1;

									if (ssData.dataLength < MAX_SAMPLE_OVERSAMPLE)
									{
										ssData.dataLength += MAX_SAMPLE_OVERSAMPLE;		// going to click for sure
									}
									else
									{
										ssData.dataLength -= MAX_SAMPLE_OVERSAMPLE;
									}
									// copy end of buffer 1 into the start of buffer 2
									PV_CopyLastSamplesToFirst((char *)pStream->pStreamData1, (char *)pStream->pStreamData2, &ssData);

									// ok, now fill second buffer
									ssData.userReference = pStream->userReference;
									ssData.streamReference = (STREAM_REFERENCE)pStream;

									// now, push second pointer out for oversampling, and get fewer bytes for this buffer
									ssData.pData = (char *)pStream->pStreamData2 + (PV_GetSampleSizeInBytes(&ssData) * MAX_SAMPLE_OVERSAMPLE);
									ssData.dataLength = pStream->streamLength2 - MAX_SAMPLE_OVERSAMPLE;

									theErr = (*pProc)(threadContext, STREAM_GET_DATA, &ssData);

									pStream->streamLength2 = ssData.dataLength;			// just in case it changes

									// update count of samples written
									pStream->samplesWritten += pStream->streamLength2;
									pStream->streamPreRollBufferSize += pStream->streamLength1;

									if (pStream->streamLength2 == 0)
									{	// underflow, get this buffer again
										pStream->streamUnderflow = TRUE;
									}
									else
									{
										pStream->startupBufferFullCount++;	// second buffer full
									}
								}
								else
								{
									pStream->streamLength2 = 0;
								}

								pStream->startupStatus = theErr;
								theErr = NO_ERR;	// we don't want to return a stop fail here

								// Ok, start the sample playback
								pStream->streamData = ssData;
								PV_AddStream(pStream);		// ok add stream
							}
						}
						if ( (theErr != NO_ERR) && (theErr != STREAM_STOP_PLAY) )
						{	// we've got to dispose of the data now
							ssData.userReference = pStream->userReference;
							ssData.streamReference = (STREAM_REFERENCE)pStream;
							ssData.pData = pStream->pStreamBuffer;
							ssData.dataLength = pStream->streamBufferLength;
							ssData.sampleRate = sampleRate;
							ssData.dataBitSize = dataBitSize;
							ssData.channelSize = channelSize;
							pStream->streamCallback = NULL;
							(*pProc)(threadContext, STREAM_DESTROY, &ssData);
							PV_FreeStream(pStream);
							reference = (STREAM_REFERENCE)0;
						}
					}
				}
			}
			else
			{
				theErr = PARAM_ERR;
			}
		}
	}
	else
	{
		theErr = NO_FREE_VOICES;
	}

#if DEBUG_STREAMS
	if (theErr)
	{
		BAE_PRINTF("Exiting stream create with an error.\n");
	}
	else
	{
		BAE_PRINTF("Exiting stream create NO error.\n");
	}
#endif
	if (pStream)
	{
		pStream->streamErr = theErr;
	}

	return reference;
}

OPErr GM_AudioStreamPreroll(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;
	OPErr				theErr;

	theErr = NO_ERR;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		// Ok, start the sample playback
		pStream->streamActive = TRUE;
		pStream->streamPaused = FALSE;
		pStream->streamFirstTime = TRUE;

// $$kk: 09.23.98: added this ->
		pStream->startEvent.status = EVENT_PENDING;
// $$kk: 09.23.98: end changes <-


		if (pStream->startupBufferFullCount == 0)
		{	// underflow on first buffer
			pStream->streamMode = STREAM_MODE_DEAD;
			//theErr = NOT_READY;
		}
		else
		{	// everythings ok, or at least the first buffer has data
			pStream->streamMode = STREAM_MODE_START_BUFFER_1;
			if (PV_PrepareThisBufferForPlaying(pStream, STREAM_MODE_START_BUFFER_1) == FALSE)
			{
				theErr = NOT_READY;
			}
			//{
			//	PV_StartStreamBuffers(pStream);
			//}
		}
		if (pStream->startupStatus == STREAM_STOP_PLAY)
		{
			pStream->streamShuttingDown = TRUE;
			if (pStream->startupBufferFullCount == 1)
			{
				pStream->streamLength2 = 0;
			}
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}

// set all the streams you want to start at the same time the same syncReference. Then call GM_SyncAudioStreamStart
// to start the sync start. Will return an error (not NO_ERR) if its an invalid reference, or syncReference is NULL.
OPErr GM_SetSyncAudioStreamReference(STREAM_REFERENCE reference, void *syncReference)
{
	GM_AudioStream		*pStream;
	OPErr				err;

	err = NO_ERR;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		err = GM_SetSyncSampleStartReference(pStream->playbackReference, syncReference);
	}
	return err;
}

// Once you have called GM_SetSyncAudioStreamReference on all the streams, this will set them to start at the next
// mixer slice. Will return an error (not NO_ERR) if its an invalid reference, or syncReference is NULL.
OPErr GM_SyncAudioStreamStart(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;
	OPErr				err;

	err = NO_ERR;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		err = GM_SyncStartSample(pStream->playbackReference);
	}
	else
	{
		err = NOT_SETUP;
	}
	return err;
}


OPErr GM_AudioStreamStart(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;
	OPErr				theErr;

	theErr = NO_ERR;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamPrerolled == FALSE)
		{
			theErr = GM_AudioStreamPreroll(reference);
		}
		if (theErr == NO_ERR)
		{
			PV_StartStreamBuffers(pStream);
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}

long GM_AudioStreamGetReference(STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;
	long				userReference;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		userReference = pStream->userReference;
	}
	return userReference;
}

void GM_AudioStreamStopAll(void *threadContext)
{
	GM_AudioStream	*pStream;

	pStream = theStreams;
	while (pStream)
	{
		if (pStream->streamActive)
		{
			GM_AudioStreamStop(threadContext, (STREAM_REFERENCE)pStream);
		}
		pStream = pStream->pNext;
	}
}

// This will stop a streaming audio object.
//
// INPUT:
//	This is the reference number returned from AudioStreamStart.
//
static OPErr PV_AudioStreamStopAndFreeNow(void *threadContext, STREAM_REFERENCE reference)
{
	GM_AudioStream		*pStream;
	GM_StreamData		ssData;
	short int			theErr;
	GM_StreamObjectProc	pProc;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			// $$kk: 08.12.98 merge 
			//  $$kk: 02.13.98: set playbackReference = DEAD_VOICE whenever we call GM_EndSample to avoid jmf stream crossing
			VOICE_REFERENCE playbackReference = pStream->playbackReference;

			pStream->streamLength1 = 0;
			pStream->streamLength2 = 0;		// don't play next buffer.

			pStream->playbackReference = DEAD_VOICE;
			GM_EndSample(playbackReference);

			pStream->streamActive = FALSE;
			pStream->streamShuttingDown = FALSE;
		}
		if (pStream->streamCallback)
		{
			ssData = pStream->streamData;

			ssData.userReference = pStream->userReference;
			ssData.streamReference = (STREAM_REFERENCE)pStream;
			ssData.pData = pStream->pStreamBuffer;
			ssData.dataLength = pStream->streamBufferLength;
			pProc = pStream->streamCallback;
			pStream->streamCallback = NULL;		// prevent recursive callbacks
			theErr = (*pProc)(threadContext, STREAM_DESTROY, &ssData);
			pStream->userReference = 0;
		}
		PV_FreeStream(pStream);
	}
	return NO_ERR;
}

// This will stop a streaming audio object, during the next GM_AudioServiceStream
//
// INPUT:
//	This is the reference number returned from AudioStreamStart.
//
// $$kk: 08.12.98 merge: changed this function 
OPErr GM_AudioStreamStop(void *threadContext, STREAM_REFERENCE reference)
{
/*
	PV_AudioStreamStopAndFreeNow(threadContext, reference);
*/
/*
	GM_AudioStream		*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		// Set various flags to iniate a shutdown of the stream from inside GM_AudioStreamService
		pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_STOP_STREAM;		// end

		// if audio data is already dead, then
		if (GM_IsSoundDone(pStream->playbackReference))
		{	// we've already flushed the audio data, so just free it now
			PV_AudioStreamStopAndFreeNow(threadContext, (long)pStream);
			pStream->streamMode = STREAM_MODE_DEAD;
		}
	}
*/

// $$kk: 01.06.98
// want stream to stay in engine until all samples played.
// we make it stop playing and mark it to be freed; it should not actually
// get freed until all samples have been played.
//	PV_AudioStreamStopAndFreeNow(threadContext, reference);

	GM_AudioStream		*pStream;

	threadContext;
	pStream = PV_AudioStreamGetFromReference(reference);

	if (pStream)
	{

		// $$kk: 05.20.98: if the stream is paused, it'll never get shut down properly in
		// GM_AudioStreamService.  need to set up the state so that it can shut down.
		if (pStream->streamPaused)
		{
			//PV_AudioStreamStopAndFreeNow(threadContext, reference);
			//return NO_ERR;
			GM_AudioStreamFlush(reference);
			GM_AudioStreamResume(reference);
		}
		
		if (pStream->streamActive)
		{
			//  $$kk: 02.13.98: set playbackReference = DEAD_VOICE whenever we call GM_EndSample to avoid jmf stream crossing
			VOICE_REFERENCE playbackReference = pStream->playbackReference;

			// stop current voice
			pStream->playbackReference = DEAD_VOICE;

			pStream->streamLength1 = 0;
			pStream->streamLength2 = 0;		// don't play next buffer.

			GM_EndSample(playbackReference);
		}

		pStream->streamShuttingDown = TRUE;
		pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_FREE_STREAM;		// end

		// $$kk: 11.03.98: added this  ->
		if (pStream->samplesWritten > 0)
		{

		// $$kk: 09.23.98: added this  ->
			pStream->stopEvent.status = EVENT_PENDING;
		// $$kk: 09.23.98: end changes <-
		} else 
		{
			// $$kk: 11.03.98: need to handle the case where a stream was started,
			// but no data was ever delivered to it.  in this case, we must mark
			// the start and stop events, which will never be generated, resolved.
			pStream->startEvent.status = EVENT_RESOLVED;
			pStream->stopEvent.status = EVENT_RESOLVED;
		}
// $$kk: 11.03.98: end changes <-
	}
	return NO_ERR;
}

// get the position of samples played in a stream. This will be as close as
// posible to realtime. Probably off by engine latency.
unsigned long GM_AudioStreamGetPlaybackSamplePosition(STREAM_REFERENCE reference)
{
#if 0
	GM_AudioStream			*pStream;
	unsigned long			samplePosition;

	samplePosition = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			samplePosition = GM_AudioStreamGetFileSamplePosition(reference) - 
									pStream->streamPreRollBufferSize;
		}
		if (pStream->streamShuttingDown)
		{
			// ok, stream as play out last of what's in the file, so at this point
			// we're coasting on what's left to play.
			samplePosition = pStream->streamPlaybackPosition;
		}
		else
		{
			samplePosition += GM_GetSamplePlaybackPosition(pStream->playbackReference);
		}
	}
	return samplePosition;
#else
	GM_AudioStream			*pStream;
	unsigned long			samplePosition;

	samplePosition = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			samplePosition = pStream->streamPlaybackPosition;	// no conversion because its in samples

			// now that we have the stream position, add in the playback of the current chunk of stream
			// for an actual position
			samplePosition += GM_GetSamplePlaybackPosition(pStream->playbackReference);
		}
	}
	return samplePosition;
#endif
}

// Get the file position of a audio stream, in samples. This
// value is the current file track position. This does not equal what has
// been played. Typically it will be ahead of real time.
unsigned long GM_AudioStreamGetFileSamplePosition(STREAM_REFERENCE reference)
{
	GM_AudioStreamFileInfo	*pInfo;
	GM_AudioStream			*pStream;
	unsigned long			samplePosition;
	short int				blockAlign;

	samplePosition = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			blockAlign = (short int)PV_GetSampleSizeInBytes(&pStream->streamData);
			pInfo = pStream->pFileStream;
			if (pInfo)
			{
				samplePosition = pInfo->filePlaybackPosition / blockAlign;	// convert from bytes to samples
			}
			else
			{
				samplePosition = pStream->streamPlaybackPosition;	// no conversion because its in samples
			}
		}
	}
	return samplePosition;
}

// Set the file position of a audio stream, in samples
OPErr GM_AudioStreamSetFileSamplePosition(STREAM_REFERENCE reference, unsigned long framePos)
{
	GM_AudioStream			*pStream;
	GM_AudioStreamFileInfo	*pFileInfo;
	OPErr					err;
	GM_StreamData			ssData;

	err = NOT_SETUP;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			pFileInfo = pStream->pFileStream;
			if (pFileInfo)
			{
				if (pStream->streamPlaybackResetAtPosition == 0)
				{
					GM_AudioStreamFlush(reference);	// flush current data
				}
				pStream->streamPlaybackPosition = framePos;
				ssData = pStream->streamData;
				ssData.userReference = pStream->userReference;
				ssData.streamReference = (STREAM_REFERENCE)pStream;
				ssData.framePosition = framePos;
				err = (*pStream->streamCallback)(NULL, STREAM_SET_POSITION, &ssData);
			}
		}
	}
	return err;
}

OPErr GM_AudioStreamGetData(void *threadContext, STREAM_REFERENCE reference, unsigned long startFrame, unsigned long stopFrame,
									XPTR pBuffer, unsigned long bufferLength)
{
	GM_AudioStream	*pStream;
	OPErr			theErr;
	GM_StreamData	ssData;

	theErr = NOT_SETUP;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream && pBuffer)
	{
		if (pStream->streamCallback)
		{
			ssData = pStream->streamData;
			ssData.dataLength = bufferLength / ssData.channelSize / (ssData.dataBitSize / 8);
			ssData.pData = (char *)pBuffer;
			ssData.userReference = pStream->userReference;
			ssData.streamReference = (STREAM_REFERENCE)pStream;
			ssData.startSample = startFrame;
			ssData.endSample = stopFrame;
			theErr = (*pStream->streamCallback)(threadContext, STREAM_GET_SPECIFIC_DATA, &ssData);
		}
	}
	return theErr;
}


// Set the stereo position of a audio stream
void GM_AudioStreamSetStereoPosition(STREAM_REFERENCE reference, short int stereoPosition)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamStereoPosition = stereoPosition;
		GM_ChangeSampleStereoPosition(pStream->playbackReference, stereoPosition);
	}
}

// Get the stereo position of a audio stream
short int GM_AudioStreamGetStereoPosition(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	short int		stereoPosition;

	stereoPosition = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		stereoPosition = pStream->streamStereoPosition;
	}
	return stereoPosition;
}

// Get the playback offset in samples for the stream.
// This is the offset between the number of samples processed
// by the mixer and the number of samples processed from this stream.
unsigned long GM_AudioStreamGetSampleOffset(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
    unsigned long offset = 0;


	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
        offset = pStream->streamPlaybackOffset;
    }
    return offset;
}

// Get the engine's count of samples from this stream actually
// played through the device.
unsigned long GM_AudioStreamGetSamplesPlayed(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
    unsigned long samplesPlayed = 0;


	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
        samplesPlayed = pStream->samplesPlayed;
    }

    return samplesPlayed;
}

// $$kk: 08.12.98 merge: added this method
// Drain this stream
void GM_AudioStreamDrain(void *threadContext, STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	unsigned long	samplesWritten;

	// get the samples written.
	// we have to drain until samples played reaches this value.
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			samplesWritten = pStream->samplesWritten;
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	//fprintf(stderr, "samplesWritten = %d\n", samplesWritten);

	while(1)
	{
		pStream = PV_AudioStreamGetFromReference(reference);
		if (pStream)
		{
			if (pStream->streamActive)
			{
				unsigned newSamplesWritten;

				newSamplesWritten = pStream->samplesWritten;
				if (newSamplesWritten < samplesWritten)
				{
					samplesWritten =  newSamplesWritten;
				}

				if (pStream->samplesPlayed >= samplesWritten)
				{
					// done!
					//fprintf(stderr, "GM_AudioStreamDrain: played all samples\n");
					return;
				}
			}
			else
			{
				// stream no longer active
				//fprintf(stderr, "GM_AudioStreamDrain: stream no longer active\n");
				return;
			}
		}
		else
		{
			// stream no longer valid
			//fprintf(stderr, "GM_AudioStreamDrain: stream no longer valid\n");
			return;
		}

		// if we get here, the stream is still valid and active and has samples still to play.
		// wait 10 milliseconds
		GM_AudioStreamService(threadContext);
		XWaitMicroseocnds(10000);
	}
}



// Flush this stream
// $$kk: 08.12.98 merge: changed to avoid stream crossing
void GM_AudioStreamFlush(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (pStream->streamActive)
		{
			//  $$kk: 02.13.98: set playbackReference = DEAD_VOICE whenever we call GM_EndSample to avoid jmf stream crossing
			VOICE_REFERENCE playbackReference = pStream->playbackReference;

			// stop current voice
			pStream->playbackReference = DEAD_VOICE;
			GM_EndSample(playbackReference);

			// set to a dead voice, which will force a new complete buffer read before starting next buffer

			if (pStream->streamShuttingDown == TRUE)
			{
				pStream->streamMode = STREAM_MODE_STOP_STREAM;
			}
			else
			{
				pStream->streamMode = STREAM_MODE_DEAD;
			}

			pStream->streamUnderflow = TRUE;	// induce underflow
			pStream->streamFlushed = TRUE;

			// here we reduce the count of samples written to the number which we will
			// actually end up playing.  (the difference between the old and new values
			// is the amount of data flushed!)
			pStream->samplesWritten = GM_AudioStreamGetFileSamplePosition(reference);
        }
    }
}


// Set the volume level of a audio stream
void GM_AudioStreamSetVolume(STREAM_REFERENCE reference, short int newVolume, XBOOL defer)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamVolume = newVolume;
		if (defer == FALSE)
		{
			GM_ChangeSampleVolume(pStream->playbackReference, newVolume);
		}
	}
}

// Get the volume level of a audio stream
short int GM_AudioStreamGetVolume(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	short int		volume;

	volume = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		volume = pStream->streamVolume;
	}
	return volume;
}

// set the volume level of all open streams. Scale is 0 to MAX_NOTE_VOLUME. If you pass
// -1, then it will reset all volumes. This is used to grab the master volume changes.
void GM_AudioStreamSetVolumeAll(short int newVolume)
{
	GM_AudioStream	*pStream;
	short int		thisVolume;

	pStream = theStreams;
	while (pStream)
	{
		if (newVolume == -1)
		{
			thisVolume = GM_GetSampleVolumeUnscaled(pStream->playbackReference);
		}
		else
		{
			thisVolume = newVolume;
		}
		pStream->streamVolume = thisVolume;
		GM_ChangeSampleVolume(pStream->playbackReference, thisVolume);
		pStream = pStream->pNext;
	}
}


// Set stream fade rate. Its a 16.16 fixed value
// Input:	reference	stream to affect
//			fadeRate	amount to change every 11 ms
//						example:	FLOAT_TO_XFIXED(2.2) will decrease volume
//									FLOAT_TO_XFIXED(2.2) * DEAD_VOICE will increase volume
//			minVolume	lowest volume level fade will go
//			maxVolume	highest volume level fade will go
void GM_SetAudioStreamFadeRate(STREAM_REFERENCE reference, XFIXED fadeRate, 
							INT16 minVolume, INT16 maxVolume, XBOOL endStream)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamFixedVolume = LONG_TO_XFIXED(pStream->streamVolume);
		pStream->streamFadeMaxVolume = maxVolume;
		pStream->streamFadeMinVolume = minVolume;
		pStream->streamEndAtFade = endStream;
		pStream->streamFadeRate = fadeRate;

	}
}

#if REVERB_USED != REVERB_DISABLED
// Enable/Disable reverb on this particular audio stream
void GM_AudioStreamReverb(STREAM_REFERENCE reference, XBOOL useReverb)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamUseReverb = useReverb;
		GM_ChangeSampleReverb(pStream->playbackReference, useReverb);
	}
}
#endif

#if REVERB_USED != REVERB_DISABLED
XBOOL GM_AudioStreamGetReverb(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	XBOOL			verb;

	verb = FALSE;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		verb = pStream->streamUseReverb;
	}
	return verb;
}
#endif

#if REVERB_USED != REVERB_DISABLED
void GM_SetStreamReverbAmount(STREAM_REFERENCE reference, short int reverbAmount)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamReverbAmount = reverbAmount;
		GM_SetSampleReverbAmount(pStream->playbackReference, reverbAmount);
	}
}
#endif

#if REVERB_USED != REVERB_DISABLED
short int GM_GetStreamReverbAmount(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	short int		verbAmount;

	verbAmount = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		verbAmount = pStream->streamReverbAmount;
	}
	return verbAmount;
}
#endif

// Set the filter frequency of a audio stream
// Range is 512 to 32512
void GM_AudioStreamSetFrequencyFilter(STREAM_REFERENCE reference, short int frequency)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamFrequencyFilter = frequency;
		GM_SetSampleFrequencyFilter(pStream->playbackReference, frequency);
	}
}

// Get the filter frequency of a audio stream
// Range is 512 to 32512
short int GM_AudioStreamGetFrequencyFilter(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	short int		frequency;

	frequency = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		frequency = pStream->streamFrequencyFilter;
	}
	return frequency;
}

// Set the filter resonance of a audio stream
// Range is 0 to 256
void GM_AudioStreamSetResonanceFilter(STREAM_REFERENCE reference, short int resonance)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamResonanceFilter = resonance;
		GM_SetSampleResonanceFilter(pStream->playbackReference, resonance);
	}
}

// Get the filter resonance of a audio stream
// Range is 0 to 256
short int GM_AudioStreamGetResonanceFilter(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	short int		resonance;

	resonance = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		resonance = pStream->streamResonanceFilter;
	}
	return resonance;
}

// get/set filter low pass amount of a audio stream
// lowPassAmount range is -255 to 255
short int GM_AudioStreamGetLowPassAmountFilter(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	short int		lowpass;

	lowpass = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		lowpass = pStream->streamLowPassAmountFilter;
	}
	return lowpass;
}

// lowPassAmount range is -255 to 255
void GM_AudioStreamSetLowPassAmountFilter(STREAM_REFERENCE reference, short int lowPassAmount)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamLowPassAmountFilter = lowPassAmount;
		GM_SetSampleLowPassAmountFilter(pStream->playbackReference, lowPassAmount);
	}
}

// Set the sample rate of a audio stream
void GM_AudioStreamSetRate(STREAM_REFERENCE reference, XFIXED newRate)
{
	GM_AudioStream	*pStream;

	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		pStream->streamData.sampleRate = newRate;
		GM_ChangeSamplePitch(pStream->playbackReference, newRate);
	}
}


// Get the sample rate of a audio stream
XFIXED GM_AudioStreamGetRate(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	XFIXED			rate;

	rate = 0;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		rate = pStream->streamData.sampleRate;
	}
	return rate;
}

// Returns TRUE or FALSE if a given GM_AudioStream is still active
XBOOL GM_IsAudioStreamPlaying(STREAM_REFERENCE reference)
{
	GM_AudioStream	*pStream;
	XBOOL			active;


	active = FALSE;
	pStream = PV_AudioStreamGetFromReference(reference);
	if (pStream)
	{
		if (GM_IsSoundDone(pStream->playbackReference) == FALSE)
		{
			if (pStream->streamActive)
			{
				active = TRUE;
			}
		}
	}
	return active;
}

// Returns TRUE or FALSE if a given reference is still valid    
// $$kk: 08.12.98 merge: changed to avoid stream crossing
XBOOL GM_IsAudioStreamValid(STREAM_REFERENCE reference)
{
	return (PV_AudioStreamGetFromReference(reference)) ? TRUE : FALSE;
}

// Process any fading streams
void PV_ServeStreamFades(void)
{
	GM_AudioStream	*pStream;
	long			value;

	pStream = theStreams;
	while (pStream)
	{
		if ((pStream->streamActive) && (pStream->streamPaused == FALSE))
		{
			if (pStream->streamFadeRate)
			{
				pStream->streamFixedVolume -= pStream->streamFadeRate;
				value = XFIXED_TO_LONG(pStream->streamFixedVolume);
				if (value > pStream->streamFadeMaxVolume)
				{
					value = pStream->streamFadeMaxVolume;
					pStream->streamFadeRate = 0;
				}
				if (value < pStream->streamFadeMinVolume)
				{
					value = pStream->streamFadeMinVolume;
					pStream->streamFadeRate = 0;
				}
				pStream->streamVolume = (short)value;

				GM_ChangeSampleVolume(pStream->playbackReference, (INT16)value);

				if ((pStream->streamFadeRate == 0) && pStream->streamEndAtFade)
				{
					//  $$kk: 02.13.98: set playbackReference = DEAD_VOICE whenever we call GM_EndSample to avoid jmf stream crossing
					VOICE_REFERENCE playbackReference = pStream->playbackReference;

					pStream->playbackReference = DEAD_VOICE;
					GM_EndSample(playbackReference);

					pStream->streamMode = STREAM_MODE_INTERRUPT_ACTIVE | STREAM_MODE_FREE_STREAM;		// end
				}
			}
		}
		pStream = pStream->pNext;
	}
}

// This is the streaming audio service routine. Call this as much as possible, but not during an
// interrupt. This is a very quick routine. A good place to call this is in your main event loop.
// $$kk: 08.12.98 merge: changed this method to allow streams to exist in the engine until all samples played
void GM_AudioStreamService(void *threadContext)
{
	GM_AudioStream		*pStream, *pNext;
	GM_StreamData		ssData;
	GM_StreamObjectProc	theProc;
	XBOOL				done;
	OPErr				theErr;

	pStream = theStreams;
	while (pStream)
	{

// $$kk: 09.23.98: added this block ->

		if (pStream->startEvent.status == EVENT_DETECTED)
		{			
			pStream->startEvent.status = EVENT_RESOLVED;
			theProc = pStream->streamCallback;

			if (theProc)
			{
				ssData = pStream->streamData;
				// do i need the rest of this ssData setup??
				ssData.userReference = pStream->userReference;
				ssData.startSample = pStream->startEvent.framePosition;
				
				theErr = (*theProc)(threadContext, STREAM_START, &ssData);
			}
		}
		if (pStream->stopEvent.status == EVENT_DETECTED)
		{
			pStream->stopEvent.status = EVENT_RESOLVED;
			theProc = pStream->streamCallback;

			if (theProc)
			{
				ssData = pStream->streamData;
				// do i need the rest of this ssData setup??
				ssData.userReference = pStream->userReference;
				ssData.startSample = pStream->stopEvent.framePosition;
				if (pStream->startupStatus == STREAM_STOP_PLAY)
				{
					theErr = (*theProc)(threadContext, STREAM_EOM, &ssData);
				}
				else 
				{
					theErr = (*theProc)(threadContext, STREAM_STOP, &ssData);
				}
			}
		}
// $$kk: 09.23.98: end changes <-


		pNext = pStream->pNext;
		if ((pStream->streamActive) && (pStream->streamPaused == FALSE))
		{
			done = GM_IsSoundDone(pStream->playbackReference);
			// voice has shutdown, either from a voice setup change or some other problem, restart
			// the stream from the last place that we're aware of.
			if ( (done) && (pStream->streamShuttingDown == FALSE) )
			{
				pStream->streamMode = STREAM_MODE_DEAD;
			}
		}

		if ( ((pStream->streamActive) &&
			  (pStream->streamMode & STREAM_MODE_INTERRUPT_ACTIVE) &&
			  (pStream->streamPaused == FALSE)) ||
			  // $$kk: added check for pStream->streamPaused == FALSE
			   (pStream->streamUnderflow && (pStream->streamPaused == FALSE)))
		{
			theProc = pStream->streamCallback;
			
			if (theProc)
			{
				pStream->streamMode &= (~STREAM_MODE_INTERRUPT_ACTIVE);
				ssData = pStream->streamData;
				switch (pStream->streamMode)
				{
					default:
						#if DEBUG_STREAMS
							BAE_PRINTF("Bad case in GM_AudioStreamService");
						#endif
						break;

					case STREAM_MODE_FREE_STREAM:
						#if DEBUG_STREAMS
							BAE_PRINTF("GM_AudioStreamService::STREAM_MODE_FREE_STREAM");
						#endif
// stream stays in engine until all samples played.
						if ((pStream->playbackReference == DEAD_VOICE) || 
							GM_IsSoundDone(pStream->playbackReference))
						{

							//fprintf(stderr, "GM_AudioStreamService::STREAM_MODE_FREE_STREAM %d\n", pStream);
							//fprintf(stderr, "pStream->samplesPlayed: %d, sample position: %d \n", pStream->samplesPlayed, GM_AudioStreamGetFileSamplePosition((long)pStream));

							// figure out whether all the samples have been played out through the device
							// before freeing the stream

							if (pStream->stopEvent.status == EVENT_RESOLVED)
							{
								// This will free the stream
								//fprintf(stderr, "freeing stream %d\n", pStream);
								PV_AudioStreamStopAndFreeNow(threadContext, (STREAM_REFERENCE)pStream);
							}
							else
							{
								// keep coming back through this loop to check again
								pStream->streamMode |= (STREAM_MODE_INTERRUPT_ACTIVE);
							}
						}
						break;
					case STREAM_MODE_STOP_STREAM:
// stream stays in engine until all samples played.
						if ((pStream->playbackReference == DEAD_VOICE) || 
							GM_IsSoundDone(pStream->playbackReference))
						{
							XBOOL	doLoop;
							
							doLoop = FALSE;
							if (pStream->pFileStream)
							{
								if (pStream->pFileStream->loopFile)
								{
									// we want to loop the entire file
									pStream->streamShuttingDown = FALSE;
								}
							}
							if (doLoop == FALSE)
							{
								//fprintf(stderr, "GM_AudioStreamService::STREAM_MODE_STOP_STREAM %d\n", pStream);
								GM_AudioStreamStop(threadContext, (STREAM_REFERENCE)pStream);

/*
								if (pStream->streamFlushed)
								{	// we've already flushed the audio data, so just free it now
									PV_AudioStreamStopAndFreeNow(threadContext, (long)pStream);
								}
								// otherwise we do nothing, because the audio callback will set the streamMode to STREAM_MODE_FREE_STREAM
								// when the samples are finished playing
*/
							}
						}
						break;
					case STREAM_MODE_DEAD:	// first buffer failed to fill
						#if DEBUG_STREAMS
							BAE_PRINTF("GM_AudioStreamService::STREAM_MODE_DEAD");
						#endif
						ssData.dataLength = pStream->streamOrgLength1;
						ssData.pData = pStream->pStreamData1;
						ssData.userReference = pStream->userReference;
						ssData.streamReference = (STREAM_REFERENCE)pStream;

// $$kk: 09.23.98: changed this ->
//						if ((*theProc)(threadContext, STREAM_GET_DATA, &ssData) != NO_ERR)
						pStream->startupStatus = (*theProc)(threadContext, STREAM_GET_DATA, &ssData);
// $$kk: 09.23.98: end changes <-
						if (pStream->startupStatus != NO_ERR)
						{
							#if DEBUG_STREAMS
								BAE_PRINTF("    STOP!");
							#endif
							pStream->streamShuttingDown = TRUE;
							pStream->streamLength2 = 0;
							PV_FillBufferEndWithSilence((char *)ssData.pData, &ssData);
						}
						pStream->streamLength1 = ssData.dataLength;			// just in case it changes
						if (ssData.dataLength < MAX_SAMPLE_OVERSAMPLE)
						{
							ssData.dataLength += MAX_SAMPLE_OVERSAMPLE;		// going to click for sure
						}
						else
						{
							ssData.dataLength -= MAX_SAMPLE_OVERSAMPLE;
						}
						// copy end of buffer 1 into the start of buffer 2
						PV_CopyLastSamplesToFirst((char *)pStream->pStreamData1, (char *)pStream->pStreamData2, &ssData);

						if (pStream->streamLength1 == 0)
						{	// underflow, get this buffer again
							pStream->streamUnderflow = TRUE;
						}
						else
						{

							// $$kk: added this
							// update count of samples written
							pStream->samplesWritten += pStream->streamLength1;

							// ok, now fill second buffer
							ssData.userReference = pStream->userReference;
							ssData.streamReference = (STREAM_REFERENCE)pStream;

							// now, push second pointer out for oversampling, and get fewer bytes for this buffer
							ssData.pData = (char *)pStream->pStreamData2 + (PV_GetSampleSizeInBytes(&ssData) * MAX_SAMPLE_OVERSAMPLE);
							ssData.dataLength = pStream->streamOrgLength2 - MAX_SAMPLE_OVERSAMPLE;

							theErr = (*theProc)(threadContext, STREAM_GET_DATA, &ssData);

// $$kk: 09.23.98: added this ->
							pStream->startupStatus = theErr;
// $$kk: 09.23.98: end changes <-

// $$kk: 11.03.98: note that if we return STREAM_STOP_PLAY here and never again, the stream does *not* shut down!!
// this means that short files with only one buffer worth of data are hosed here.

							pStream->streamLength2 = ssData.dataLength;			// just in case it changes
							if (pStream->streamLength2 == 0)
							{	// underflow, get this buffer again
								pStream->streamUnderflow = TRUE;
							}
							else
							{
								// $$kk: added this
								// update count of samples written
								pStream->samplesWritten += pStream->streamLength2;

								pStream->streamUnderflow = FALSE;
							}

							pStream->streamMode = STREAM_MODE_START_BUFFER_1;
							done = GM_IsSoundDone(pStream->playbackReference);
							if (done)
							{
								if (PV_PrepareThisBufferForPlaying(pStream, STREAM_MODE_START_BUFFER_1))
								{
									PV_StartStreamBuffers(pStream);
								}
							}
							pStream->streamData = ssData;
						}
						break;
					case STREAM_MODE_START_BUFFER_2:		// read buffer 1 into memory
						#if DEBUG_STREAMS
							BAE_PRINTF("GM_AudioStreamService::STREAM_MODE_START_BUFFER_2");
						#endif
						if (pStream->streamShuttingDown == FALSE)
						{
							ssData.dataLength = pStream->streamOrgLength1 - MAX_SAMPLE_OVERSAMPLE;
							ssData.pData = (char *)pStream->pStreamData1 + (PV_GetSampleSizeInBytes(&ssData) * MAX_SAMPLE_OVERSAMPLE);
							ssData.userReference = pStream->userReference;
							ssData.streamReference = (STREAM_REFERENCE)pStream;

// $$kk: 09.23.98: changed this ->
//							if ((*theProc)(threadContext, STREAM_GET_DATA, &ssData) != NO_ERR)
							
							pStream->startupStatus = (*theProc)(threadContext, STREAM_GET_DATA, &ssData);														
// $$kk: 09.23.98: end changes <-
							if (pStream->startupStatus != NO_ERR)
							{
								#if DEBUG_STREAMS
									BAE_PRINTF("    STOP!");
								#endif
								pStream->streamShuttingDown = TRUE;
								pStream->streamLength2 = 0;
								PV_FillBufferEndWithSilence((char *)ssData.pData, &ssData);
							}
							pStream->streamLength1 = ssData.dataLength;			// just in case it changes
							// copy end of buffer 1 into the start of buffer 2
							PV_CopyLastSamplesToFirst((char *)pStream->pStreamData1, (char *)pStream->pStreamData2, &ssData);

                            if (pStream->streamLength1 == 0)
							{	// underflow, get this buffer again
								pStream->streamUnderflow = TRUE;
							}
							else
							{
								// update count of samples written
								pStream->samplesWritten += pStream->streamLength1;

								// did our last buffer underflow?
								if (pStream->streamUnderflow)
								{
									done = GM_IsSoundDone(pStream->playbackReference);
									// voice has shutdown from running out of data, restart previous buffer.
									if (done)
									{
										if (PV_PrepareThisBufferForPlaying(pStream, STREAM_MODE_START_BUFFER_1))
										{
											PV_StartStreamBuffers(pStream);
										}
									}
								}
								pStream->streamUnderflow = FALSE;
							}
						}
						#if DEBUG_STREAMS && 0
							BAE_PRINTF("B1-> %ld len %ld", pStream->streamPlaybackPosition, pStream->streamLength1);
						#endif
						break;
					case STREAM_MODE_START_BUFFER_1:		// read buffer 2 into memory
						#if DEBUG_STREAMS
							BAE_PRINTF("GM_AudioStreamService::STREAM_MODE_START_BUFFER_1");
						#endif
						if (pStream->streamShuttingDown == FALSE)
						{
							ssData.dataLength = pStream->streamOrgLength2 - MAX_SAMPLE_OVERSAMPLE;
							ssData.pData = (char *)pStream->pStreamData2 + (PV_GetSampleSizeInBytes(&ssData) * MAX_SAMPLE_OVERSAMPLE);
							ssData.userReference = pStream->userReference;
							ssData.streamReference = (STREAM_REFERENCE)pStream;
// $$kk: 09.23.98: changed this ->
//							if ((*theProc)(threadContext, STREAM_GET_DATA, &ssData) != NO_ERR)
							pStream->startupStatus = (*theProc)(threadContext, STREAM_GET_DATA, &ssData);
							
							// $$kk: 09.23.98: end changes <-
							if (pStream->startupStatus != NO_ERR)
							{
								#if DEBUG_STREAMS
									BAE_PRINTF("    STOP!");
								#endif
								pStream->streamShuttingDown = TRUE;
								pStream->streamLength1 = 0;
								PV_FillBufferEndWithSilence((char *)ssData.pData, &ssData);
							}
							pStream->streamLength2 = ssData.dataLength;
							// copy end of buffer 2 into the start of buffer 1
							PV_CopyLastSamplesToFirst((char *)pStream->pStreamData2, (char *)pStream->pStreamData1, &ssData);

							if (pStream->streamLength2 == 0)
							{	// underflow, get this buffer again
								pStream->streamUnderflow = TRUE;
							}
							else
							{
								// update count of samples written
								pStream->samplesWritten += pStream->streamLength2;

								// did our last buffer underflow?
								if (pStream->streamUnderflow)
								{
									done = GM_IsSoundDone(pStream->playbackReference);
									// voice has shutdown from running out of data, restart previous buffer.
									if (done)
									{
										if (PV_PrepareThisBufferForPlaying(pStream, STREAM_MODE_START_BUFFER_2))
										{
											PV_StartStreamBuffers(pStream);
										}
									}
								}
								pStream->streamUnderflow = FALSE;
							}
						}
						#if DEBUG_STREAMS && 0
							BAE_PRINTF("B2-> %ld len %ld", pStream->streamPlaybackPosition, pStream->streamLength2);
						#endif
						break;
				}
			}
		}
		pStream = pNext;
	}
}

// update number of samples played for each stream
// delta is number of samples engine advanced, in its format
// $$kk: 08.12.98 merge: changed this method to allow streams to exist in the engine until all samples played
void GM_AudioStreamUpdateSamplesPlayed(unsigned long delta) 
{
	GM_AudioStream		*pStream, *pNext;
    unsigned long		outputSampleRate;

	//$$kk: 05.08.98: adding this variable and removing the next one
	unsigned long		streamSampleRate;
    //XFIXED				scaling;

    unsigned long		streamDelta;    // delta in stream-format samples
	unsigned long		samplesCommitted = 0;

	pStream = theStreams;

	while (pStream)
	{

		pNext = pStream->pNext;

        // convert from engine samples to stream samples
		// $$kk: 05.08.98: the outputSampleRate we get here is a regular sample rate, not a fixed value.
		// i am changing this to account for this.
		// scaling = delta * pStream->streamData.sampleRate / outputSampleRate;
		// streamDelta = XFIXED_TO_LONG_ROUNDED(scaling);
		outputSampleRate = GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate);
		streamSampleRate = XFIXED_TO_LONG_ROUNDED(pStream->streamData.sampleRate);
		streamDelta = delta * streamSampleRate / outputSampleRate;

		// $$kk: we can't count higher than the number of samples processed by the engine for this stream
		samplesCommitted = GM_AudioStreamGetFileSamplePosition((STREAM_REFERENCE)pStream);


		// $$kk: 03.05.98: at the end of the stream, GM_GetSamplePlaybackPosition remains at its last (highest) value even though
		// pStream->streamPlaybackPosition already equals samplesWritten, so GM_GetSamplePlaybackPosition returns an impossibly high
		// value.  make sure we don't use it....
		if (samplesCommitted > pStream->samplesWritten)
		{
				samplesCommitted = pStream->samplesWritten;
		}

		// do we know our offset, and are we in play range?
        if ((pStream->streamPlaybackOffset != STREAM_OFFSET_UNSET) &&
        	(MusicGlobals->samplesPlayed > pStream->streamPlaybackOffset)) 
        {
            // we need to handle the case where the stream may just be starting up again
            if ((unsigned long)(MusicGlobals->samplesPlayed -
            					pStream->streamPlaybackOffset) < streamDelta) 
            {
                streamDelta = MusicGlobals->samplesPlayed - pStream->streamPlaybackOffset;
            }

/*
            // are we incrementing normally?
            if ((pStream->samplesPlayed + streamDelta) <= samplesCommitted)
            {
                pStream->samplesPlayed += streamDelta;
            }
            // no, actual underflow is occurring
            else
            {
                pStream->samplesPlayed = samplesCommitted;
			}
*/

            // are we incrementing normally?
            if ((pStream->samplesPlayed + streamDelta) < samplesCommitted)
            {
// $$kk: 09.23.98: changed this block ->
				if (pStream->startEvent.status == EVENT_PENDING)
				{
					pStream->startEvent.status = EVENT_DETECTED;
					pStream->startEvent.framePosition = pStream->samplesPlayed;
				}

                pStream->samplesPlayed += streamDelta;
            }
            // no, actual underflow is occurring
            else
            {
				if (samplesCommitted > pStream->samplesPlayed)
				{
					if (pStream->startEvent.status == EVENT_PENDING)
					{
						pStream->startEvent.status = EVENT_DETECTED;
						pStream->startEvent.framePosition = pStream->samplesPlayed;
					}
	
					pStream->samplesPlayed = samplesCommitted;
				}
				else 
				{
					if (pStream->stopEvent.status == EVENT_PENDING)
					{
						pStream->stopEvent.status = EVENT_DETECTED;
						pStream->stopEvent.framePosition = pStream->samplesPlayed;
					}

				}
// $$kk: 09.23.98: end changes <-
			}
		}


// $$kk: 09.21.98: i *think* we shouldn't need this as long as we are making the stream stay in the engine???
/*		
		else 
		{ 
			// $$kk: here we are trying to make sure that we keep incrementing the
			// sample count for a stream which has a playback reference of DEAD_VOICE, but
			// is still playing out....
			if (pStream->residualSamples > 0)								  
			{
				if (pStream->residualSamples < streamDelta)
				{
					streamDelta = pStream->residualSamples;
				}
				if (streamDelta > (samplesCommitted - pStream->samplesPlayed))
				{
					streamDelta = samplesCommitted - pStream->samplesPlayed;
				}
				pStream->samplesPlayed += streamDelta;
				pStream->residualSamples -= streamDelta;
			}
		}

		// $$kk: 04.28.98: should not need this with new Headspace code.
		// 03.09.98: this is a *hack* to get around the stream crossing that can happen if a stream plays out and
		// its id becomes available to another stream, but the first one still holds a reference to it.
		//if ( GM_IsSoundDone(pStream->playbackReference) )
		//{
		//	pStream->playbackReference = DEAD_VOICE;
		//}
*/
		pStream = pNext; 
	} // while
}

// LINKED STREAMS

// group functions
// new
// free
// add GM_Waveform
// add double buffer
// remove
// start
// stop
// setvolume

// Group samples
//
// USE:
//
// linked streams
// Call GM_AudioStreamSetup, to get an allocate stream then call GM_NewLinkedStreamList. Then add it to your 
// maintained top list of linked streams by calling GM_AddLinkedStream. Use GM_FreeLinkedStreamList to delete an entire list, 
// or GM_RemoveLinkedStream to just one link.
//
// Then you can call GM_StartLinkedStreams to start them all at the same time, or GM_EndLinkedStreams
// to end them all, or GM_SetLinkedStreamVolume, GM_SetLinkedStreamRate, and GM_SetLinkedStreamPosition
// set parameters about them all.

// private structure of linked streams
struct GM_LinkedStream
{
	STREAM_REFERENCE			playbackReference;
	void						*threadContext;
	struct GM_LinkedStream		*pNext;
};
typedef struct GM_LinkedStream GM_LinkedStream;


LINKED_STREAM_REFERENCE GM_NewLinkedStreamList(STREAM_REFERENCE reference, void *threadContext)
{
	GM_LinkedStream	*pNew;

	pNew = NULL;
	if (GM_IsAudioStreamValid(reference))
	{
		pNew = (GM_LinkedStream *)XNewPtr(sizeof(GM_LinkedStream));
		if (pNew)
		{
			pNew->playbackReference	= reference;
			pNew->pNext = NULL;
			pNew->threadContext = threadContext;
		}
	}
	return (LINKED_STREAM_REFERENCE)pNew;
}

// Given a top link, deallocates the linked list. DOES NOT deallocate the streams.
void GM_FreeLinkedStreamList(LINKED_STREAM_REFERENCE pTop)
{
	GM_LinkedStream	*pNext, *pLast;

	pNext = (GM_LinkedStream *)pTop;
	while (pNext)
	{
		pLast = pNext;
		pNext = pNext->pNext;
		XDisposePtr((XPTR)pLast);
	}
}

// Given a top link, and a new link this will add to a linked list, and return a new top
// if required.
LINKED_STREAM_REFERENCE GM_AddLinkedStream(LINKED_STREAM_REFERENCE pTop, LINKED_STREAM_REFERENCE pEntry)
{
	GM_LinkedStream	*pNext;

	if (pEntry)
	{
		pNext = (GM_LinkedStream *)pTop;
		while (pNext)
		{
			if (pNext->pNext == NULL)
			{
				break;
			}
			else
			{
				pNext = pNext->pNext;
			}
		}
		if (pNext == NULL)
		{
			pTop = pEntry;
		}
		else
		{
			pNext->pNext = (GM_LinkedStream *)pEntry;
		}
	}
	return pTop;
}

// Given a top link and an link to remove this will disconnect the link from the list and
// return a new top if required.
LINKED_STREAM_REFERENCE GM_RemoveLinkedStream(LINKED_STREAM_REFERENCE pTop, LINKED_STREAM_REFERENCE pEntry)
{
	GM_LinkedStream	*pNext, *pLast;

	if (pEntry)
	{
		pLast = pNext = (GM_LinkedStream *)pTop;
		while (pNext)
		{
			if (pNext == (GM_LinkedStream *)pEntry)								// found object in list?
			{
				if (pNext == (GM_LinkedStream *)pTop)								// is object the top object
				{
					pTop = (LINKED_STREAM_REFERENCE)pNext->pNext;						// yes, change to next object
				}
				else
				{
					if (pLast)									// no, change last to point beyond next
					{
						pLast->pNext = pNext->pNext;
					}
				}
				break;
			}
			pLast = pNext;
			pNext = pNext->pNext;
		}
	}
	return pTop;
}

STREAM_REFERENCE GM_GetLinkedStreamPlaybackReference(LINKED_STREAM_REFERENCE pLink)
{
	STREAM_REFERENCE			reference;

	reference = DEAD_STREAM;
	if (pLink)
	{
		reference = ((GM_LinkedStream *)pLink)->playbackReference;
		if (GM_IsAudioStreamValid(reference) == FALSE)
		{
			reference = DEAD_STREAM;
		}
	}
	return reference;
}

void * GM_GetLinkedStreamThreadContext(LINKED_STREAM_REFERENCE pLink)
{
	void	*threadContext;

	threadContext = NULL;
	if (pLink)
	{
		threadContext = ((GM_LinkedStream *)pLink)->threadContext;
	}
	return threadContext;
}


OPErr GM_StartLinkedStreams(LINKED_STREAM_REFERENCE pTop)
{
	GM_LinkedStream		*pNext;
	OPErr				err;

	err = NO_ERR;	// ok, until proved otherwise
	if (MusicGlobals)
	{
		// set sync reference. Use our group because its easy and will be unique
		pNext = (GM_LinkedStream *)pTop;
		while (pNext)
		{
			err = GM_SetSyncAudioStreamReference(((GM_LinkedStream *)pNext)->playbackReference, (void *)pTop);
			pNext = pNext->pNext;
		}
		if (err == NO_ERR)
		{
			// ok, now wait for mixer to be free
			while (MusicGlobals->insideAudioInterrupt)
			{
				XWaitMicroseocnds(BAE_GetSliceTimeInMicroseconds());
			}
			pNext = (GM_LinkedStream *)pTop;
			while (pNext)
			{
				err = GM_SyncAudioStreamStart(((GM_LinkedStream *)pNext)->playbackReference);
				pNext = pNext->pNext;
			}
		}
	}
	else
	{
		err = NOT_SETUP;
	}
	return err;
}

// end in unison the samples for all the linked streams
void GM_EndLinkedStreams(LINKED_STREAM_REFERENCE pTop)
{
	GM_LinkedStream	*pNext;

	pNext = (GM_LinkedStream *)pTop;
	while (pNext)
	{
		GM_AudioStreamStop(pNext->threadContext, pNext->playbackReference);
		pNext = pNext->pNext;
	}
}

// Volume range is from 0 to MAX_NOTE_VOLUME
// set in unison the sample volume for all the linked streams
void GM_SetLinkedStreamVolume(LINKED_STREAM_REFERENCE pTop, INT16 sampleVolume, XBOOL defer)
{
	GM_LinkedStream	*pNext;

	pNext = (GM_LinkedStream *)pTop;
	while (pNext)
	{
		GM_AudioStreamSetVolume(pNext->playbackReference, sampleVolume, defer);
		pNext = pNext->pNext;
	}
}

// set in unison the sample rate for all the linked streams
void GM_SetLinkedStreamRate(LINKED_STREAM_REFERENCE pTop, XFIXED theNewRate)
{
	GM_LinkedStream	*pNext;

	pNext = (GM_LinkedStream *)pTop;
	while (pNext)
	{
		GM_AudioStreamSetRate(pNext->playbackReference, theNewRate);
		pNext = pNext->pNext;
	}
}


// set in unison the sample position for all the linked streams
// range from -63 to 63
void GM_SetLinkedStreamPosition(LINKED_STREAM_REFERENCE pTop, INT16 newStereoPosition)
{
	GM_LinkedStream	*pNext;

	pNext = (GM_LinkedStream *)pTop;
	while (pNext)
	{
		GM_AudioStreamSetStereoPosition(pNext->playbackReference, newStereoPosition);
		pNext = pNext->pNext;
	}
}


#endif	// USE_STREAM_API

// EOF of GenAudioStreams.c

