/*****************************************************************************/
/*
** "GenSetup.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	© Copyright 1993-2001 Beatnik, Inc, All Rights Reserved.
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
**
** Overview
**	General purpose Music Synthesis software, C-only implementation
**	(no assembly language optimizations made)
**
** Modification History:
**
**	1/10/96		Split from GenSynth.c
**	1/12/96		Changed GM_ChangeSystemVoices to support 32 voices instead of 31
**	1/18/96		Spruced up for C++ extra error checking
**				Fixed bug with GM_FinisGeneralSound referencing GM_Mixer before its allocated
**	1/19/96		Changed GM_BeginSample to support bitsize and channels
**	2/5/96		Working towards multiple songs
**	2/12/96		Moved cleaning of external midi queue to Init code
**	3/6/96		Eliminated the global songVolume
**	4/10/96		Changed min loop size from 100 to use define 'MIN_LOOP_SIZE'
**				Eliminated the 'can't use 0 as loop start' in GM_BeginSample
**	4/21/96		Removed register usage in parameters
**				Put test around ReverbBuffer being NULL
**	4/25/96		Removed SwapLong & SwapShort. Use XGetLong & XGetShort instead
**	7/4/96		Changed font and re tabbed
**	7/31/96		Fixed PV_ChangeSustainedNotes to do pedal changes as 127=on,
**				anything else as off
**	10/11/96	Added GM_BeginSampleFromInfo
**	10/23/96	Removed reference to BYTE and changed them all to UBYTE or SBYTE
**	11/3/96		Fixed a bug in GM_BeginDoubleBuffer. Changed currentPosition to
**				currentLevel
**				Changed the way the expression controller calculates extra volume,
**				now done in PV_ScaleVolumeFromChannelAndSong
**	11/8/96		Added GM_GetSampleVolume & GM_GetSamplePitch & GM_GetSampleStereoPosition
**	11/21/96	Reconfigured GM_InitGeneralSound to check parmeters in a clearer way.
**				Also prevents possible compiler problems
**	12/17/96	Fixed bug in GM_InitGeneralSound that would return an error for 32 voices
**	12/30/96	Changed copyright
**	1/12/97		Changed maxNormalizedVoices to mixLevel
**	1/22/97		Added GM_SetSampleDoneCallback
**	1/23/97		Fixed a bug with PV_ScaleVolumeFromChannelAndSong that prevent direct midi
**				control from changing the master volume
**				Changed GM_GetSampleVolume to return zero if sample reference is bad
**	1/27/97		Added fade sample engine via PV_ServeEffectsFades
**	1/28/97		Added the ability to end a sample when fade is reached
**	2/12/97		Added support for X_WIN_HARDWARE
**	2/13/97		Added GM_GetSystemVoices
**	2/20/97		Added support for TYPE 7 and 8 reverb in GM_SetReverbType
**				Fixed a bug with GM_SetMasterVolume that doesn't kill audio when
**				volume is 0
**	3/3/97		Reversed stereo placement in PV_CalculateStereoVolume
**	3/24/97		Put code in to prevent changing the voices unless they are
**				different. See GM_ChangeSystemVoices
**	4/1/97		Fixed a memory leak. Forgot to free reverb memory in GM_FinisGeneralSound
**	4/9/97		Added sampleExpansion factor
**	4/15/97		Moved sample API functions to GenSample.c
**	5/3/97		Fixed a few potential problems that the MOT compiler found
**	5/5/97		Changed PV_CalculateStereoVolume to handle the fractional volume case
**				at 1.
**	6/4/97		Renamed InitSoundManager to GM_StartHardwareSoundManager, and
**				renamed FinsSoundManager to GM_StopHardwareSoundManager, and
**				now pass in a thread context
**				Changed GM_InitGeneralSound & GM_FinisGeneralSound & GM_ChangeAudioModes &
**				GM_PauseGeneralSound & GM_ResumeGeneralSound to pass in a thread context
**				that is passed to the low level hardware API
**	6/17/97		Changed GM_SetMasterVolume to reset all songs and samples to rescale
**				current volumes
**	7/22/97		Moved GM_SetAudioTask & GM_SetAudioOutput & GM_GetAudioTask &
**				GM_GetAudioOutput from various platform specific files to GenSetup.c
**				Changed GM_SetMasterVolume to include streams calls
**	7/28/97		In GM_SetMasterVolume, wrapped a define around the stream call
**	8/15/97		Removed reference of pMixer->channelCallbackProc. Now using new
**				memory efficant controler callback
**	8/18/97		Changed X_WIN_BAE to USE_BAE_EXTERNAL_API
**	8/19/97		Seperated GM_StartHardwareSoundManager from being called inside of
**				GM_InitGeneralSound. Now it has to be called by the caller. Likewise
**				GM_StopHardwareSoundManager is no longer being called within GM_FinisGeneralSound.
**	10/12/97	Fixed SetChannelVolume & PV_ChangeSustainedNotes & SetChannelStereoPosition &
**				SetChannelModWheel & SetChannelPitchBend to check for a particular song before
**				changing playing voices. This fixed a bug in which a pitch bend from one song
**				would affect another
**	10/16/97	Removed lame instrument cache support from GM_InitGeneralSound
**	11/12/97	Added GM_MaxDevices & GM_SetDeviceID & GM_GetDeviceID & GM_GetDeviceName
**	12/4/97		Added GM_GetDeviceTimeStamp & GM_UpdateSamplesPlayed
**				Added calculation of sampleFrameSize to GM_InitGeneralSound
**				Renamed GM_Mixer to GM_Mixer
**	12/16/97	Modified GM_GetDeviceID and GM_SetDeviceID to pass a device parameter pointer
**				that is specific for that device.
**	12/16/97	Moe: removed compiler warnings
**	1/20/98		Modified SetChannelVolume to allow for zero volume levels without killing
**				note
**	2/3/98		Added GM_SetupReverb & GM_CleanupReverb
**	2/8/98		Changed BOOL_FLAG to XBOOL
**	2/10/98		kcr		initialize new effect types (new reverb, chorus, etc.)
**	2/11/98		Added support for Q_48K & Q_24K & Q_8K & Q_22K_TERP_44K & Q_11K_TERP_22K
**				Fixed bug in GM_ChangeAudioModes that changed the verb mode
**				Added GM_ConvertFromOutputQualityToRate
**	2/20/98		kcr		properly dispose of reverb memory
**	2/23/98		Removed last of old variable reverb code
**	3/12/98		Renamed myMusicGlobals to pMixer. Started implementing verb config code. 
**				Redid and moved some reverb setup code into GenReverb.c
**	3/18/98		Changed GM_InitGeneralSound & GM_ChangeAudioModes to fail on an 8 bit
**				audio card if there's no support.
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
**	7/28/98		Modified PV_CalculateStereoVolume to handle master song pan placement
**	7/30/98		Removed some duplicated code that cleared the active note list
**	8/5/98		Put wrappers around GM_ChangeSystemVoices & GM_ChangeAudioModes to 
**				protect against MusicGlobals not being setup
**
**
**	JAVASOFT
**	03.17.98	$$kk: GM_InitGeneralSound and GM_ChangeAudioModes: left my checks for 8 and 16 bit support
**				For some sound cards, we support 16 bit but *not* 8 bit....  I also left
**				my UNSUPPORTED_HARDWARE error because I want to differentiate between this (failure
**				to acquire the device because we don't support it at all) and failure to acquire
**				the device for some random reason that may not be true later, like someone else is using it.
**	01.26.98:	$$kk: changed GM_ChangeAudioModes to only start and stop the device 
**				if it is opened
** ??			$$kk: changes to count in samples, not bytes, in GM_UpdateSamplesPlayed
**	10/6/98		Fixed bug with PV_ChangeSustainedNotes that changed a pedal down note's state
**				to the wrong state if a another pedal down event happens. Result is a stuck note.
**	11/9/98		Renamed NoteDur to voiceMode
**	12/22/98	Removed old USE_SEQUENCER flag
**	1/12/99		Added some Katmai code and dynamic flags
**	3/8/99		Renamed GM_EndSoundEffects to GM_EndAllSamples
**	3/12/99		Put in support for different loop types
**	4/8/99		Changed Katmai ID code
**	5/28/99		Changed GM_GetSyncTimeStamp to get around an odd compiler bug
**	6/8/99		Removed references to USE_BAE_EXTERNAL_API. We now require BAE_API
**	7/13/99		Renamed HAE to BAE
**	10/19/99	MSD: switched to REVERB_USED and LOOPS_USED
**	10/30/99	Added support for Q_16K
**	11/10/99	Added ifdefs to handle the case REVERB_USED == REVERB_DISABLED
**	11/30/99	Fixed the Q_8K case in PV_SetSampleSliceSize
**	1/31/2000	Added GM_IsGeneralSoundPaused()
**				Added GM_Generate16bitOutP(), GM_GenerateStereoOutP()
**				Added GM_GetInterpolationMode(), GM_GetQuality()
**				Changed GM_InitGeneralSound() to return the resulting GM_Mixer in an outParameter
**	2/3/00		Added GM_Mixer parameter to GM_FinisGeneralSound()
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	3/7/2000	Referenced passed in mixer in GM_FinisGeneralSound
**	4/25/2000	msd: added support for 32kHz and 40kHz output.
**	2000.05.16 AER	Completed modifications for new sample cache
**	2000.05.18 msd	now calculate maxChunkSize programatically, rather than
**					as compile time #defines in PV_SetSampleSliceSize().
**	5/28/2000	sh	Added GM_GetCurrentMixer
**	9/6/2000	sh	Removed default Katmai logic from Setup and Change modes
**					and placed into the main BAE.cpp file.
**	11/2/2000	sh	Changed GM_InitGeneralSound to continue setting up mixer
**					even if there was an error condition. This handles the case
**					in which XIs16BitSupported Fails, but we still need the mixer alive.
**	2/14/2001	sh	Set enableDriftFixer to TRUE in GM_InitGeneralSound. This enables
**					an automatic fix to events being passed into the system and queued.
**					The internal clock will never drift beyond 1ms of realtime.
**	4/23/2001	sh	Added PV_CalculateMonoVolume
**	8/10/2002		Added GM_ConvertFromOutputQualityToPerceivedRate
*/
/*****************************************************************************/
#include "X_API.h"
#include "GenSnd.h"
#include "GenPriv.h"
#include "GenCache.h"
#include "BAE_API.h"
#include "X_Assert.h"


// Add rates here, to allow use
static XBOOL PV_ValidateRate(Rate theRate)
{
	XBOOL	ok = FALSE;

	if ((theRate > Q_RATE_7K) && (theRate < Q_RATE_48K))
	{
		ok = TRUE;
	}
	return ok;
}


// convert GenSynth Rate to actual sample rate used
unsigned long GM_ConvertFromOutputRateToRate(Rate rate)
{
	unsigned long sampleRate;

	sampleRate = 0;
	if (PV_ValidateRate(rate))
	{
		switch (rate)
		{
			default:
				sampleRate = (unsigned long)rate;
				break;
			case Q_RATE_22K_TERP_44K:
				sampleRate = 44100;
				break;
			case Q_RATE_11K_TERP_22K:
				sampleRate = 22050;
				break;
		}
	}
	return sampleRate;
}

OPErr GM_PauseGeneralSound(void *threadContext)
{
	OPErr	theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (MusicGlobals->systemPaused == FALSE)
		{
			GM_PauseSequencer(FALSE);						// don't kill voices
			MusicGlobals->systemPaused = TRUE;
			GM_StopHardwareSoundManager(threadContext);		// disengage from hardware
		}
		else
		{
			theErr = ALREADY_PAUSED;
		}
	}
	return theErr;
}

OPErr GM_ResumeGeneralSound(void *threadContext)
{
	OPErr	theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (MusicGlobals->systemPaused)
		{
			if (GM_StartHardwareSoundManager(threadContext))			// reconnect to hardware
            {
			    MusicGlobals->systemPaused = FALSE;
			    GM_ResumeSequencer();
            }
            else
            {
                theErr = DEVICE_UNAVAILABLE;
            }
		}
		else
		{
			theErr = ALREADY_RESUMED;
		}
	}
	return theErr;
}

OPErr GM_IsGeneralSoundPaused(XBOOL *outIsPaused)
{
	OPErr	theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (outIsPaused)
		{
			*outIsPaused = MusicGlobals->systemPaused;
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

void GM_GetSystemVoices(INT16 *pMaxSongVoices, INT16 *pMixLevel, INT16 *pMaxEffectVoices)
{
	if (MusicGlobals && pMaxSongVoices && pMixLevel && pMaxEffectVoices)
	{
		*pMaxSongVoices = MusicGlobals->MaxNotes;
		*pMixLevel = MusicGlobals->mixLevel;
		*pMaxEffectVoices = MusicGlobals->MaxEffects;
	}
}

OPErr GM_ChangeSystemVoices(INT16 maxSongVoices, INT16 mixLevel, INT16 maxEffectVoices)
{
	OPErr	theErr;
	XBOOL	change;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if ( (maxSongVoices >= 0) &&
			(mixLevel > 0) &&
			(maxEffectVoices >= 0) &&
			((maxEffectVoices+maxSongVoices) > 0) &&
			((maxEffectVoices+maxSongVoices) <= MAX_VOICES) )
		{
			change = FALSE;
			if (MusicGlobals->MaxNotes != maxSongVoices)
			{
				change = TRUE;
			}
			if (MusicGlobals->mixLevel != mixLevel)
			{
				change = TRUE;
			}
			if (MusicGlobals->MaxEffects != maxEffectVoices)
			{
				change = TRUE;
			}
			if (change)
			{
				MusicGlobals->MaxNotes = maxSongVoices;
				MusicGlobals->mixLevel = mixLevel;
				MusicGlobals->MaxEffects = maxEffectVoices;

				PV_CalcScaleBack();
			}
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

// Set the master volume, and recalculate all volumes. Scale is 0 to MAX_MASTER_VOLUME
void GM_SetMasterVolume(INT32 theVolume)
{
	if (MusicGlobals)
	{
		MusicGlobals->MasterVolume = (INT16)theVolume;
		PV_CalcScaleBack();

#if USE_STREAM_API
		GM_AudioStreamSetVolumeAll(-1);	// recalculate stream volumes
#endif
		{
			short int	count;
			GM_Song		*pSong;

			// reset volumes for sound effects
			GM_SetEffectsVolume(GM_GetEffectsVolume());

			// walk through songs and reset volumes
			for (count = 0; count < MAX_SONGS; count++)
			{
				pSong = MusicGlobals->pSongsToPlay[count];
				if (pSong)
				{
					GM_SetSongVolume(pSong, GM_GetSongVolume(pSong));
				}
			}
		}
	}
}

INT32 GM_GetMasterVolume(void)
{
	if (MusicGlobals)
	{
		return MusicGlobals->MasterVolume;
	}
	else
	{
		return MAX_MASTER_VOLUME;
	}
}


// Return the number of microseconds of real time that will be generated when calling
// BAE_BuildMixerSlice.
unsigned long BAE_GetSliceTimeInMicroseconds(void)
{
	return MusicGlobals->bufferTime;
}

// Based upon the sample rate, setup the unrolled inner loop counters, and
// number of samples per frame count
//
// These variables control the number of loops to process deep in the inner loops
// of the interpolaters.
//
// For example:
//		One_Slice is how many samples to process assuming you're doing 1 sample at a time.
//		Two_Loop is how many samples to process assuming you're doing 2 samples at a time.
//		Four_Loop is how many samples to process assuming you're doing 4 samples at a time.
//		Sixteen_Loop is how many samples to process assuming you're doing 16 samples at a time.
//
// MAX_CHUNK_SIZE must be divisible by 16 for this reason, otherwise gaps in the audio will
// generate noise.
//
// This assumes that the pMixer and theRate are valid, no exceptions.
static void PV_SetSampleSliceSize(GM_Mixer *pMixer, Rate theRate)
{
	unsigned long maxChunkSize;
	unsigned long rate;

	pMixer->maxChunkSize = MAX_CHUNK_SIZE;
	pMixer->One_Slice = MAX_CHUNK_SIZE;
	pMixer->outputRate = theRate;

	rate = GM_ConvertFromOutputRateToRate(theRate);

	maxChunkSize = (rate*BUFFER_SLICE_TIME)/1000000;
	if (maxChunkSize % 16 != 0)
	{
		// round up to next multiple of 16
		maxChunkSize = 16 * ((maxChunkSize / 16)+1);
	}
	
	// calulcate microseconds per buffer
	pMixer->bufferTime = (maxChunkSize * 1000000) / rate;

	// calulcate microseconds per buffer used for lfo changes
	pMixer->lfoBufferTime = XFixedMultiply(BUFFER_SLICE_TIME, pMixer->bufferTime);
	pMixer->lfoBufferTime = XFixedDivide(pMixer->lfoBufferTime, FIXED_BUFFER_SLICE_TIME) - 610;

	pMixer->maxChunkSize = (short)maxChunkSize;
	pMixer->One_Slice = pMixer->maxChunkSize;

	if (theRate == Q_RATE_22K_TERP_44K || theRate == Q_RATE_11K_TERP_22K)
	{
		pMixer->One_Slice /= 2;
	}

	// set control loops
	pMixer->One_Loop = pMixer->One_Slice;
	pMixer->Two_Loop = pMixer->One_Slice/2;
	pMixer->Four_Loop = pMixer->One_Slice/4;
	pMixer->Sixteen_Loop = pMixer->One_Slice/16;
	pMixer->sampleExpansion = 1;
}

// Returns the global GM_Mixer pointer
struct GM_Mixer * GM_GetCurrentMixer(void)
{
	return MusicGlobals;
}

// allocate and setup the mixer, but don't active the hardware, yet.
//
// This allocates MusicGlobals, which is the common GM_Mixer structure.
OPErr GM_InitGeneralSound(void *threadContext, Rate theRate, TerpMode theTerp, AudioModifiers theMods,
				INT16 maxVoices, INT16 normVoices, INT16 maxEffects, GM_Mixer **outMixer)
{
	register GM_Mixer	*pMixer;
	register INT32		count;
	OPErr				theErr;

	threadContext = threadContext;
	theErr = NO_ERR;

// Check out parameter
	if (outMixer == NULL)
	{
		theErr = PARAM_ERR;
	}

	count = maxVoices + maxEffects;
	if (count <= MAX_VOICES)
	{
		if (normVoices > MAX_VOICES)
		{
			if ((normVoices/100) > count)
			{
				theErr = PARAM_ERR;
			}
		}
		else if (normVoices > count)
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
// Check terp mode
	switch (theTerp)
	{
		case E_AMP_SCALED_DROP_SAMPLE:
		case E_2_POINT_INTERPOLATION:
		case E_LINEAR_INTERPOLATION:
		case E_LINEAR_INTERPOLATION_FLOAT:
		case E_LINEAR_INTERPOLATION_U3232:
			break;
		default:
			theErr = PARAM_ERR;
			break;
	}
	if (PV_ValidateRate(theRate) == FALSE)
	{
		theErr = PARAM_ERR;
	}
	if (theErr == NO_ERR)
	{
		// call setup before any memory allocation happens
		if (BAE_Setup())
		{
			theErr = MEMORY_ERR;
		}
	}

	pMixer = NULL;
	if (theErr == NO_ERR)
	{
// Allocate MusicGlobals
		MusicGlobals = (GM_Mixer *)XNewPtr( (long)sizeof(GM_Mixer) );
		pMixer = MusicGlobals;
		if (pMixer)
		{
			// Turn off all notes!
			for (count = 0; count < MAX_VOICES; count++)
			{
				pMixer->NoteEntry[count].voiceMode = VOICE_UNUSED;
			}
			pMixer->interpolationMode = theTerp;
		
			pMixer->MasterVolume = MAX_MASTER_VOLUME;
			pMixer->effectsVolume = MAX_MASTER_VOLUME * 2 * 4;

			// set control loops
			PV_SetSampleSliceSize(pMixer, theRate);
		
			// Don't generate 16 bit output, unless hardware can do it. 
			if ( (theMods & M_USE_16) == M_USE_16)
			{
				pMixer->generate16output = XIs16BitSupported();
			}
			else
			{
				// $$kk: 08.12.98 merge: left my code here.
				// $$kk: 03.17.98: some solaris audio drivers do not support linear 8 bit output
				// so we need to check for mono support
				pMixer->generate16output = !XIs8BitSupported();
			}

			// $$kk: 03.17.98: now if whatever is set is not supported, we are hosed
			if (pMixer->generate16output)
			{
				if (!XIs16BitSupported())
				{
					theErr = UNSUPPORTED_HARDWARE;
				}
			}
			else
			{
				if (!XIs8BitSupported())
				{
					theErr = UNSUPPORTED_HARDWARE;
				}
			}

			// double check users request for Stereo output. Make sure the hardware can play it
			if ( (theMods & M_USE_STEREO) == M_USE_STEREO)
			{
				pMixer->generateStereoOutput = XIsStereoSupported();
			}
			else
			{
				pMixer->generateStereoOutput = FALSE;
			}

			pMixer->stereoFilter = ( (pMixer->generateStereoOutput) &&
												 ((theMods & M_STEREO_FILTER) == M_STEREO_FILTER) ) ? TRUE : FALSE;
			pMixer->MaxNotes = maxVoices;
			pMixer->mixLevel = normVoices;
			pMixer->MaxEffects = maxEffects;
#if REVERB_USED != REVERB_DISABLED
			pMixer->reverbPtr = 0;
			pMixer->reverbBuffer = NULL;
			pMixer->reverbUnitType = REVERB_NO_CHANGE;
			pMixer->reverbTypeAllocated = REVERB_NO_CHANGE;
			pMixer->reverbBufferSize = 0;

			if ( (theMods & M_DISABLE_REVERB) != M_DISABLE_REVERB)
			{
				GM_SetupReverb();
			}
			GM_SetReverbType(DEFAULT_REVERB_TYPE);		// default reverb
#endif
			GM_EndAllNotes();

// Compute volume multiplier for mix-level
			PV_CalcScaleBack();

#ifndef _DEBUG
			GMCache_ClearSampleCache(pMixer);	//	Replaces manual modification
#endif

			// 2000.05.02 AER	Mixer always caches now...
			// pMixer->cacheSamples = FALSE;		// don't cache samples
			pMixer->cacheInstruments = FALSE;	// not used
		}
		else
		{
			theErr = MEMORY_ERR;
		}
	}
	if (pMixer)
	{
		pMixer->insideAudioInterrupt = 0;
		pMixer->enableDriftFixer = TRUE;	// always fix drift for realtime vs parsed events.
		pMixer->syncCount = XMicroseconds();
        pMixer->samplesPlayed = 0;
        pMixer->samplesWritten = 0;
        pMixer->lastSamplePosition = 0;
		pMixer->sequencerPaused = TRUE;
		pMixer->systemPaused = TRUE;
		BAE_NewMutex(&pMixer->queueLock, "bae", "seqq", __LINE__);
		PV_CleanExternalQueue(pMixer);

		// calculate sample size for conversion of bytes to sample frames
		pMixer->sampleFrameSize = 1;

		if (pMixer->generate16output)
		{
			pMixer->sampleFrameSize *= 2;
		}
		if (pMixer->generateStereoOutput)
		{
			pMixer->sampleFrameSize *= 2;
		}

		// since we don't call GM_StartHardwareSoundManager, we start the engine
		// up paused. You'll need to call GM_ResumeGeneralSound to start the
		// engine
		
// This code is here for reference only.
//		pMixer->sequencerPaused = FALSE;
//		pMixer->systemPaused = FALSE;
//		if (GM_StartHardwareSoundManager(threadContext) == FALSE)
//		{
//			theErr = DEVICE_UNAVAILABLE;
//		}
	}

	*outMixer = pMixer;
	return theErr;
}

// $$kk: 08.12.98 merge: changed this method
// $$kk: 01.26.98: changed this to only start and stop the device if it is opened,
#if X_PLATFORM != X_WEBTV
OPErr GM_ChangeAudioModes(void *threadContext, 
							Rate theRate, TerpMode theTerp, AudioModifiers theMods)
{
	register GM_Mixer	*pMixer;
	OPErr				theErr;
#if REVERB_USED != REVERB_DISABLED
	ReverbMode			verb = REVERB_TYPE_1;
#endif
	// $$kk: 01.26.98: added this var reacquireDevice 
	XBOOL			reacquireDevice = FALSE;

	theErr = NO_ERR;
	pMixer = MusicGlobals;
	if (pMixer)
	{
	// Check terp mode
		switch (theTerp)
		{
			case E_AMP_SCALED_DROP_SAMPLE:
			case E_2_POINT_INTERPOLATION:
			case E_LINEAR_INTERPOLATION:
			case E_LINEAR_INTERPOLATION_FLOAT:
			case E_LINEAR_INTERPOLATION_U3232:
				break;
			default:
				theErr = PARAM_ERR;
				break;
		}
		if (PV_ValidateRate(theRate) == FALSE)
		{
			theErr = PARAM_ERR;
		}
		if (theErr == NO_ERR)
		{
			// $$kk: 01.26.98: add this check
			if (pMixer->systemPaused == FALSE) 
			{		
				GM_StopHardwareSoundManager(threadContext);
				reacquireDevice = TRUE; // reopen the device when we're done configuring
			}

			if ( (theMods & M_USE_16) == M_USE_16)
			{
				pMixer->generate16output = XIs16BitSupported();
			}
			else
			{
				// $$kk: 03.17.98: some solaris audio drivers do not support linear 8 bit output
				// so we need to check for mono support
				pMixer->generate16output = !XIs8BitSupported();
			}

			// $$kk: 03.17.98: now if whatever is set is not supported, we are hosed
			if (pMixer->generate16output)
			{
				if (!XIs16BitSupported())
				{
					theErr = UNSUPPORTED_HARDWARE;
				}
			}
			else
			{
				if (!XIs8BitSupported())
				{
					theErr = UNSUPPORTED_HARDWARE;
				}
			}

			// double check users request for Stereo output. Make sure the hardware can play it
			if ( (theMods & M_USE_STEREO) == M_USE_STEREO)
			{
				pMixer->generateStereoOutput = XIsStereoSupported();
			}
			else
			{
				pMixer->generateStereoOutput = FALSE;
			}
			pMixer->stereoFilter = ( (pMixer->generateStereoOutput) &&
												 ((theMods & M_STEREO_FILTER) == M_STEREO_FILTER) ) ? TRUE : FALSE;

#if REVERB_USED != REVERB_DISABLED
			verb = GM_GetReverbType();	// preserve current
			if ( (theMods & M_DISABLE_REVERB) == M_DISABLE_REVERB)
			{
				// cleanup the verb buffers
				GM_CleanupReverb();
			}
			else
			{
				GM_SetupReverb();
			}
			GM_SetReverbType(verb);		// restore verb
#endif

			// set control loops
			PV_SetSampleSliceSize(pMixer, theRate);

#if LOOPS_USED != LIMITED_LOOPS
			// if we've changed terp modes translate the all the active voices
			// sample position
			if (pMixer->interpolationMode != theTerp)
			{
				INT32	count;
				UINT32	pos;

				for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
				{
					GM_Voice *pVoice;
					
					pVoice = &pMixer->NoteEntry[count];
					pos = PV_GetPositionFromVoice(pVoice);
					PV_SetPositionFromVoice(pVoice, pos);
					pVoice->NoteNextSize = 0;	// force a recalculate slice size for voice
				}
			}
#endif
			pMixer->interpolationMode = theTerp;
	// Recompute mix level
			PV_CalcScaleBack();

			// $$kk: 01.26.98: add this check
			if (reacquireDevice == TRUE) 
			{
				if (GM_StartHardwareSoundManager(threadContext) == FALSE)
				{
					theErr = MEMORY_ERR;
				}
			}
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}
#endif	// X_PLATFORM != X_WEBTV


void GM_FinisGeneralSound(void *threadContext, GM_Mixer *mixer)
{
	threadContext = threadContext;
	BAE_ASSERT(mixer == MusicGlobals);
	if (mixer)
	{
		mixer->systemPaused = TRUE;
		BAE_DestroyMutex(mixer->queueLock);
		GM_FreeSong(threadContext, NULL);		// free all songs

		// Close up sound manager BEFORE releasing memory!
//		GM_StopHardwareSoundManager(threadContext);

#if REVERB_USED != REVERB_DISABLED
		// clean up the verb buffers
		GM_CleanupReverb();
#endif

		XDisposePtr((XPTR)mixer);
		MusicGlobals = NULL;
	}

	BAE_Cleanup();
}

UINT32 PV_ScaleVolumeFromChannelAndSong(GM_Song *pSong, INT16 channel, UINT32 volume)
{
	register UINT32		newVolume;

	// scale song volume based upon master song volume, only if a song channel
	if (channel != SOUND_EFFECT_CHANNEL)
	{
		if (pSong)
		{
			if (pSong->channelExpression[channel])
			{
				// for now, let's just scale up the volume level of the channel
				// Say 127 is 10.2% higher
				volume = (volume * (UINT32)pSong->channelExpression[channel]) / (MAX_NOTE_VOLUME - 12);
			}

			// scale note velocity via current channel volume
			newVolume = (volume * (UINT32)pSong->channelVolume[channel]) / MAX_NOTE_VOLUME;

			// scale note velocity via current song volume
			newVolume = (newVolume * (UINT32)pSong->songVolume) / MAX_SONG_VOLUME;
		}
		else
		{
			newVolume = volume;
		}
	}
	else
	{
		// scale note velocity via current master effects volume
		newVolume = (volume * (UINT32)MusicGlobals->effectsVolume) / MAX_MASTER_VOLUME;
	}
	return newVolume;
}


// ------------------------------------------------------------------------------------------------------//

/*
static const UBYTE stereoPanRamp[] =
{
0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13,
14, 14, 15, 15, 16, 16, 17, 17, 18, 19, 19, 20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27,
28, 29, 30, 30, 31, 32, 33, 33, 34, 35, 36, 37, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 62, 63, 64, 66, 67, 69, 70, 72, 73, 75, 77, 79, 81,
83, 85, 87, 89, 91, 94, 96, 99, 102, 105, 108, 111, 115, 119, 123, 127, 132, 138, 144, 151, 159,
169, 180, 195, 216, 252, 253
};
*/
#define USE_GS_RAMP		0

#if USE_GS_RAMP
// new GS ramp * 32. Divide by 32 after using this factor
static const short newStereoPanRamp[] = 
{
0, 0, 32, 63, 95, 135, 167, 198, 230, 270, 302, 341, 373, 413, 445, 484, 516, 
556, 595, 635, 667, 706, 746, 786, 825, 857, 897, 937, 976, 1016, 1056, 1095, 
1135, 1175, 1214, 1254, 1294, 1333, 1373, 1421, 1461, 1500, 1540, 1580, 1619, 
1659, 1699, 1738, 1778, 1826, 1865, 1905, 1945, 1984, 2024, 2064, 2104, 2143, 
2183, 2223, 2262, 2302, 2342, 2373, 2413, 2453, 2492, 2532, 2564, 2603, 2643, 
2675, 2715, 2746, 2786, 2818, 2857, 2889, 2929, 2961, 2992, 3024, 3064, 3096, 
3127, 3159, 3191, 3223, 3254, 3278, 3310, 3342, 3365, 3397, 3429, 3453, 3477, 
3508, 3532, 3556, 3588, 3612, 3635, 3659, 3683, 3699, 3723, 3747, 3770, 3786, 
3810, 3826, 3842, 3866, 3882, 3897, 3913, 3929, 3945, 3961, 3977, 3992, 4008, 
4016, 4032, 4040, 4056, 4064, 
}; 
static void PV_RemapMidiPan(INT32 stereoPosition, UINT32 *pLeft, UINT32 *pRight)
{
	UINT32	left, right;

	stereoPosition >>= 1;
	left = newStereoPanRamp[63 - stereoPosition] / 16;
	right = newStereoPanRamp[stereoPosition + 63] / 16;
	*pLeft = left;
	*pRight = right;
}
#endif

#if USE_DLS
static const UBYTE stereoPanRamp[] = 
{
0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6,
6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 12, 12,
12, 13, 13, 13, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19,
20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 26, 26, 27, 27, 28, 28, 29,
30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
46, 47, 49, 50, 51, 53, 54, 56, 58, 60, 62, 64, 67, 70, 73, 76, 80, 85, 91,
98, 109, 127
};
static void PV_RemapMidiPan(INT32 stereoPosition, UINT32 *pLeft, UINT32 *pRight)
{
	UINT32	left, right;

	stereoPosition >>= 1;
	left = stereoPanRamp[63 - stereoPosition] * 5;
	right = stereoPanRamp[stereoPosition + 63] * 5;
	*pLeft = left;
	*pRight = right;
}
#endif
#if USE_DLS == 0 && USE_GS_RAMP == 0
static void PV_RemapMidiPan(INT32 stereoPosition, UINT32 *pLeft, UINT32 *pRight)
{
	UINT32	left, right;

	if (stereoPosition)
	{
		if (stereoPosition < 0)	// left changes
		{
			right = (MAX_NOTE_VOLUME-1) + stereoPosition;
//			left = (MAX_NOTE_VOLUME-1) - stereoPosition;		// new
			left = (MAX_NOTE_VOLUME-1);
		}
		else
		{					// right changes
			right = (MAX_NOTE_VOLUME-1);
//			right = (MAX_NOTE_VOLUME-1) + stereoPosition;		// new
			left = (MAX_NOTE_VOLUME-1) - stereoPosition;
		}
	}
	else
	{
		left = MAX_NOTE_VOLUME;
		right = MAX_NOTE_VOLUME;
	}
	*pLeft = left;
	*pRight = right;
}
#endif

void PV_CalculateMonoVolume(GM_Voice *pVoice, INT32 *pVolume)
{
	UINT32	noteVolume;
	long	channels[2];

	// scale new volume based up channel volume, song volume, and current note volume
//	noteVolume = PV_ScaleVolumeFromChannelAndSong(pVoice->pSong, pVoice->NoteChannel, pVoice->NoteVolume);
	noteVolume = pVoice->NoteVolume;
	noteVolume = (noteVolume * (UINT32)pVoice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;

	channels[0] = (long)noteVolume;
	channels[1] = (long)noteVolume;

	// with the voice route bus, modify the left and right channel volume
	BAE_ProcessRouteBus(pVoice->routeBus, channels, 2);
	noteVolume = (UINT32)channels[0];
	*pVolume = noteVolume;
}

// Given a stereo position from -63 to 63, return a volume level
void PV_CalculateStereoVolume(GM_Voice *pVoice, INT32 *pLeft, INT32 *pRight)
{
	INT32	stereoPosition;
	UINT32	left, right;
	UINT32	noteVolume;
	long	channels[2];

	stereoPosition = pVoice->stereoPosition + pVoice->stereoPanBend;

	// multplex the master song pan
	if (pVoice->pSong)
	{
		stereoPosition += (pVoice->pSong->songMasterStereoPlacement * 2);
	}
	stereoPosition *= -2;	// reverse left and right, and increase range

	// clip to absolute ranges
	if (stereoPosition >= (MAX_PAN_RIGHT * 2))
	{
		stereoPosition = MAX_PAN_RIGHT * 2;
	}
	if (stereoPosition <= (MAX_PAN_LEFT * 2))
	{
		stereoPosition = MAX_PAN_LEFT * 2;
	}

	if (pVoice->NoteChannel != SOUND_EFFECT_CHANNEL)
	{
		// map pan for music channels
		PV_RemapMidiPan(stereoPosition, &left, &right);
	}
	else
	{
		if (stereoPosition)
		{
			// map pan for effects channels
			if (stereoPosition < 0)	// left changes
			{
				right = (MAX_NOTE_VOLUME-1) + stereoPosition;
				left = (MAX_NOTE_VOLUME-1) - stereoPosition;		// new
			}
			else
			{					// right changes
				right = (MAX_NOTE_VOLUME-1) + stereoPosition;		// new
				left = (MAX_NOTE_VOLUME-1) - stereoPosition;
			} 
		}
		else
		{
			left = MAX_NOTE_VOLUME;
			right = MAX_NOTE_VOLUME;
		}
	}

	// scale new volume based up channel volume, song volume, and current note volume
//	noteVolume = PV_ScaleVolumeFromChannelAndSong(pVoice->pSong, pVoice->NoteChannel, pVoice->NoteVolume);
	noteVolume = pVoice->NoteVolume;
	noteVolume = (noteVolume * (UINT32)pVoice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;

	left = (left * noteVolume) / MAX_NOTE_VOLUME;
	right = (right * noteVolume) / MAX_NOTE_VOLUME;

	// min out values to handle the fractional case
	if (left <= 1)
	{
		left = 0;
	}
	if (right <= 1)
	{
		right = 0;
	}
	
	channels[0] = (long)left;
	channels[1] = (long)right;

	// with the voice route bus, modify the left and right channel volume
	BAE_ProcessRouteBus(pVoice->routeBus, channels, 2);
	*pLeft = (UINT32)channels[0];
	*pRight = (UINT32)channels[1];
}


void SetChannelVolume(GM_Song *pSong, INT16 the_channel, INT16 newVolume)
{
	register GM_Mixer		*pMixer;
	register LOOPCOUNT		count;
	register GM_Voice		*theNote;

	pMixer = MusicGlobals;
	// update the current notes playing to the new volume
	for (count = 0; count < pMixer->MaxNotes; count++)
	{
		theNote = &pMixer->NoteEntry[count];
		if ( (theNote->voiceMode != VOICE_UNUSED) && (theNote->pSong == pSong) )
		{
			if (theNote->NoteChannel == the_channel)
			{
// performance tweak. Kills note if volume level reaches zero, but this is bad
// for content that uses volume level to shape output
#if 0
				if (newVolume == 0)
				{
					theNote->voiceMode = VOICE_RELEASING;
					theNote->NoteDecay = 0;
					theNote->volumeADSRRecord.ADSRTime[0] = 1;
					theNote->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
					theNote->volumeADSRRecord.ADSRLevel[0] = 0;	// just in case
				}
#endif
				// now calculate the new volume based upon the current channel volume and
				// the unscaled note volume
				newVolume = (INT16)PV_ScaleVolumeFromChannelAndSong(theNote->pSong, the_channel, theNote->NoteMIDIVolume);
				//CLS:  Do we not want to use a 32-bit intermediate value here?
				newVolume = (INT16)((newVolume * pMixer->scaleBackAmount) >> 8);
				theNote->NoteVolume = newVolume;
			}
		}
	}
}


// Put all notes that have been in 'SUS_ON_NOTE' mode into their normal decay release mode
void PV_ChangeSustainedNotes(GM_Song *pSong, INT16 the_channel, INT16 data)
{
	register GM_Mixer		*pMixer;
	register LOOPCOUNT		count;
	register GM_Voice		*theNote;

	pMixer = MusicGlobals;
	for (count = 0; count < pMixer->MaxNotes; count++)
	{
		theNote = &pMixer->NoteEntry[count];
		if ( (theNote->voiceMode != VOICE_UNUSED) && (theNote->pSong == pSong) )
		{
			if (theNote->NoteChannel == the_channel)
			{
				if (data < 64)	// release. ( 0-63 off, 64-127 on)
				{
					// the note has been released by the fingers, so release note
					if (theNote->sustainMode == SUS_ON_NOTE_OFF)
					{
						theNote->voiceMode = VOICE_RELEASING;	// decay note out to prevent clicks
					}
					theNote->sustainMode = SUS_NORMAL;
				}
				else
				{	// change status
					// only do this if we're not sustaining a note already
					if (theNote->sustainMode != SUS_ON_NOTE_OFF)
					{
						theNote->sustainMode = SUS_ON_NOTE_ON;
					}
				}
			}
		}
	}
}

// Set stereo position from control values of 0-127. This will translate into values of 63 to -63
INT16 SetChannelStereoPosition(GM_Song *pSong, INT16 the_channel, UINT16 newPosition)
{
	register GM_Mixer		*pMixer;
	register LOOPCOUNT		count;
	register GM_Voice *	theNote;
	register INT16			newLogPosition;
	static char stereoScale[] =
	{
		63, 58, 55, 52, 50, 47, 45, 43,		41, 39, 37, 35, 33, 32, 30, 29,
		27, 26, 25, 23, 22, 21, 20, 19, 	18, 17, 17, 16, 15, 14, 14, 13, 
		12, 12, 11, 11, 10, 10,  9,  9,		 8,  8,  7,  7,  7,  6,  6,  6, 
		 6,  5,  5,  5,  5,  4,  4,  4,		 4,  4,  3,  3,  3,  2,  1,  0,
		 0,
		-1, -2, -3, -3, -3, -4, -4, -4, 	-4, -4, -5, -5, -5, -5, -6, -6, 
		-6, -6, -7, -7, -7, -8, -8, -9, 	-9,-10,-10,-11,-11,-12,-12,-13, 
	   -14,-14,-15,-16,-17,-17,-18,-19,    -20,-21,-22,-23,-25,-26,-27,-29, 
	   -30,-32,-33,-35,-37,-39,-41,-43,    -45,-47,-50,-52,-55,-58,-63
	};

	pMixer = MusicGlobals;
	// make sure and set the channel stereo position
	newLogPosition = stereoScale[newPosition];
	// update the current notes playing to the new stereo position. It will get incorporated into the mix at the
	// next audio frame
	for (count = 0; count < pMixer->MaxNotes; count++)
	{
		theNote = &pMixer->NoteEntry[count];
		if ( (theNote->voiceMode != VOICE_UNUSED) && (theNote->pSong == pSong) )
		{
			if (theNote->NoteChannel == the_channel)
			{
				theNote->stereoPosition = newLogPosition;
			}
		}
	}
	return newLogPosition;
}

// Set mod wheel position from control values of 0-127.
void SetChannelModWheel(GM_Song *pSong, INT16 the_channel, UINT16 value)
{
	register GM_Mixer		*pMixer;
	register LOOPCOUNT		count;
	register GM_Voice		*theNote;

	pMixer = MusicGlobals;

	// update the current notes playing to the new MOD wheel setting
	for (count = 0; count < pMixer->MaxNotes; count++)
	{
		theNote = &pMixer->NoteEntry[count];
		if ( (theNote->voiceMode != VOICE_UNUSED) && (theNote->pSong == pSong) )
		{
			if (theNote->NoteChannel == the_channel)
			{
				theNote->ModWheelValue = value;
			}
		}
	}
}


// Change pitch all notes playing on this channel, and for new notes on this channel
INT16 SetChannelPitchBend(GM_Song *pSong, INT16 the_channel, UBYTE bendRange, UBYTE bendMSB, UBYTE bendLSB)
{
	register LOOPCOUNT		count;
	register GM_Mixer		*pMixer;
	register long			bendAmount, the_pitch_bend;
	register GM_Voice		*pNote;

	pMixer = MusicGlobals;
	// Convert LSB & MSB into values from -8192 to 8191
	the_pitch_bend = (((bendMSB * 128) + bendLSB) - 8192);

	// Scale values from -8192 to 8192 to -bend to bend in 8.8 fixed point
	bendAmount = bendRange * 256;
	the_pitch_bend = (the_pitch_bend * bendAmount) / 8192;

	// update the current note playing to the new bend value
	for (count = 0; count < pMixer->MaxNotes; count++)
	{
		pNote = &pMixer->NoteEntry[count];
		if ( (pNote->voiceMode != VOICE_UNUSED) && (pNote->pSong == pSong) )
		{
			if (pNote->NoteChannel == the_channel)
			{
				pNote->NotePitchBend = (INT16)the_pitch_bend;
			}
		}
	}
	return (INT16)the_pitch_bend;
}




UINT32 GM_GetSamplesPlayed(void) 
{
	if (MusicGlobals)
	{
	    return MusicGlobals->samplesPlayed;
	}
	return 0L;
}

#if USE_CALLBACKS
void GM_SetAudioTask(GM_AudioTaskCallbackPtr pTaskProc, void *taskReference)
{
	if (MusicGlobals)
	{
		MusicGlobals->pTaskProc = pTaskProc;
		MusicGlobals->taskReference = taskReference;
	}
}

void GM_SetAudioOutput(GM_AudioOutputCallbackPtr pOutputProc)
{
	if (MusicGlobals)
	{
		MusicGlobals->pOutputProc = pOutputProc;
	}
}

GM_AudioTaskCallbackPtr GM_GetAudioTask(void)
{
	if (MusicGlobals)
	{
		return MusicGlobals->pTaskProc;
	}
	return NULL;
}

GM_AudioOutputCallbackPtr GM_GetAudioOutput(void)
{
	if (MusicGlobals)
	{
		return MusicGlobals->pOutputProc;
	}
	return NULL;
}
#endif	// #if USE_CALLBACKS

// Connect to hardware.
//
// Global variables must be set before this can be called.
//
//	MusicGlobals->generate16output
//	MusicGlobals->generateStereoOutput
//	MusicGlobals->outputRate
//
// Return FALSE if failure, otherwise TRUE

#ifdef BAE_COMPLETE
XBOOL GM_StartHardwareSoundManager(void *threadContext)
{
	long	sampleRate;
	int		ok;

	if (MusicGlobals)
	{
		sampleRate = (long)GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate);

		ok = BAE_AquireAudioCard(threadContext, sampleRate,
									(MusicGlobals->generateStereoOutput) ? 2 : 1,
									(MusicGlobals->generate16output) ? 16 : 8);
		return (ok == 0) ? TRUE : FALSE;
	}
	return FALSE;
}
#endif

// Stop generating samples and shutdown
#ifdef BAE_COMPLETE
void GM_StopHardwareSoundManager(void *threadContext)
{
	// need this so that we can properly update samplesPlayed to the total written
	// to the *device* when we close the audio device.  UpdateSamplesPlayed() needs
	// to take the current device playback position as its argument.  when we shut
	// down the device, we want to move it to represent everything submitted.  if the
	// device has been opened and closed before, the device-end-position is different
	// than MusicGlobals->samplesWritten.
	static UINT32 lastSamplesWritten = 0;

	// everything that's going to play has been played; samples played by mixer
	// should equal samples submitted.  if we don't synch these here, our count
	// of samples played and samples submitted to device diverge after closing
	// and reopening the device.

	BAE_ReleaseAudioCard(threadContext);
	if (MusicGlobals)
	{
		GM_UpdateSamplesPlayed((MusicGlobals->samplesWritten - lastSamplesWritten));
		lastSamplesWritten = MusicGlobals->samplesWritten;
	}
}
#endif

// Get current audio time stamp in microseconds; this is the
// microseconds' worth of samples that have passed through the
// audio device.  it never decreases.
// $$kk: this and all the time stamp methods should move into a common file
// CLS:  copied this function in from Kara's
UINT32 GM_GetDeviceTimeStamp(void)
{
	UINT16	sampleRate;

	if (MusicGlobals)
	{
		// convert from samples into microseconds
		sampleRate = (UINT16)GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate);
#if USE_FLOAT == FALSE
		return (MusicGlobals->samplesPlayed * 1000000) / sampleRate;
#else
		return (UINT32)(((float) MusicGlobals->samplesPlayed / sampleRate) * 1000000);
#endif
	}
	return 0L;
}

// Update count of samples played.  This function caluculates from number of bytes,
// given the sample frame size from the mixer variables
// $$kk: 08.12.98 merge: changed this function
// $$kk: no, we're getting the currentPos in SAMPLES, not bytes, from BAE_GetDeviceSamplesPlayedPosition().
void GM_UpdateSamplesPlayed(unsigned long currentPos)
{
	unsigned long delta;

	if (currentPos >= MusicGlobals->lastSamplePosition)
	{
		delta = (currentPos - MusicGlobals->lastSamplePosition);
	}
	else
	{
		delta = currentPos;
	}

	MusicGlobals->lastSamplePosition = currentPos;

	// update mixer samples played
	MusicGlobals->samplesPlayed += delta;
#if USE_STREAM_API == TRUE
	// update samples played for each stream
	GM_AudioStreamUpdateSamplesPlayed(delta);
#endif
}

// number of devices. ie different versions of the BAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long GM_MaxDevices(void)
{
	return BAE_MaxDevices();
}

// set the current device. device is from 0 to GM_MaxDevices()
// NOTE:	This function needs to function before any other calls may have happened.
//			Also you will need to call BAE_ReleaseAudioCard then BAE_AquireAudioCard
//			in order for the change to take place.
void GM_SetDeviceID(long deviceID, void *deviceParameter)
{
	BAE_SetDeviceID(deviceID, deviceParameter);
}

// return current device ID
// NOTE: This function needs to function before any other calls may have happened.
long GM_GetDeviceID(void *deviceParameter)
{
	return BAE_GetDeviceID(deviceParameter);
}

// get deviceID name
// NOTE:	This function needs to function before any other calls may have happened.
//			Format of string is a zero terminated comma delinated C string.
//			"platform,method,misc"
//	example	"MacOS,Sound Manager 3.0,SndPlayDoubleBuffer"
//			"WinOS,DirectSound,multi threaded"
//			"WinOS,waveOut,multi threaded"
//			"WinOS,VxD,low level hardware"
//			"WinOS,plugin,Director"
void GM_GetDeviceName(long deviceID, char *cName, unsigned long cNameLength)
{
	BAE_GetDeviceName(deviceID, cName, cNameLength);
}

// Get current audio time stamp based upon the audio built interrupt
UINT32 GM_GetSyncTimeStamp(void)
{
	UINT32	ticks;

	ticks = 0L;
	if (MusicGlobals)
	{
		ticks = MusicGlobals->syncCount;
	}
	return ticks;
}

long GM_GetAudioBufferOutputSize(void)
{
	return BAE_GetAudioByteBufferSize();
}

// Get current audio time stamp based upon the audio built interrupt, but ahead in time and quantized for
// the particular OS
UINT32 GM_GetSyncTimeStampQuantizedAhead(void)
{
	return GM_GetSyncTimeStamp() + (BAE_GetSliceTimeInMicroseconds() * BAE_GetAudioBufferCount());
}

OPErr GM_Generate16bitOutP(XBOOL *outGenerate16)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (outGenerate16)
		{
			*outGenerate16 = MusicGlobals->generate16output;
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GenerateStereoOutP(XBOOL *outGenerateStereo)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (outGenerateStereo)
		{
			*outGenerateStereo = MusicGlobals->generateStereoOutput;
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}


OPErr GM_GetInterpolationMode(TerpMode *outTerpMode)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (outTerpMode)
		{
			*outTerpMode = MusicGlobals->interpolationMode;
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetRate(Rate *outRate)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (MusicGlobals)
	{
		if (outRate)
		{
			*outRate = MusicGlobals->outputRate;
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}


/* EOF of GenSetup.c
*/

