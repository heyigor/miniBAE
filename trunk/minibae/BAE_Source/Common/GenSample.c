/*****************************************************************************/
/*
** "GenSample.c"
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
**	4/15/97		Moved sample API functions to GenSample.c
**				Added code to handle sample callbacks
**				Added GM_AddSampleOffsetCallback & GM_RemoveSampleOffsetCallback
**	5/1/97		Added startOffsetFrame to GM_BeginSampleFromInfo
**	5/7/97		Changed various functions that wanted stereo pan to be in the range
**				of -255 to 255, to now accecpt the natural range of -63 to 63
**	5/8/97		Fixed GM_ChangeSampleVolume & GM_GetSampleVolume to scale the volume
**				correctly based upon the current effectsVolume
**	5/21/97		Added GM_GetSamplePlaybackPosition
**	6/17/97		Modified GM_BeginSample & GM_BeginDoubleBuffer to save the volume
**				level unscaled in the voice allocated for later use
**				Moved GM_GetEffectsVolume & GM_SetEffectsVolume from GenSetup.c and
**				modified GM_SetEffectsVolume to scale volume levels based upon
**				masterVolume and effectsVolume using the unscaled sample volume
**	6/25/97		Added GM_SetSampleLoopPoints
**	7/22/97		Added GM_GetSampleVolumeUnscaled
**	9/15/97		Added GM_GetSampleReverb
**	10/26/97	Fixed a bug with GM_BeginDoubleBuffer that forgot to look for
**				MusicGlobals being deallocated
**	11/19/97	Removed zero volume check in GM_BeginDoubleBuffer & GM_BeginSample
**	12/16/97	Moe: removed compiler warnings
**	1/12/98		Modified GM_IsSoundDone to pass TRUE/FALSE correctly
**	1/14/98		kk: added number of loops to GM_BeginSample calls
**	2/8/98		Changed BOOL_FLAG to XBOOL
**	2/10/98		Changed GM_BeginSample to use max duration count
**	2/11/98		Put code wrappers around functions not used for WebTV
**	3/5/98		Changed GM_BeginDoubleBuffer && GM_BeginSample to not play samples
**				out of MAX_SAMPLE_FRAMES range
**	3/12/98		Modified GM_BeginDoubleBuffer to include a sample done callback
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
**	7/6/98		Fixed a compiler warning with GM_BeginSampleFromInfo
**	7/28/98		Renamed inst_struct to pInstrument
**	7/30/98		Fixed GM_ChangeSampleReverb to set the reverbLevel for new verbs when
**				enabling verb on sound effects
**	8/5/98		Changed GM_BeginDoubleBuffer & GM_BeginSample to allocate the voice
**				before it starts filling in parameters for control.
**	8/6/98		Changed the order of voice allocate in GM_BeginDoubleBuffer & GM_BeginSample
**				from the last voice to the first voice.
**	8/11/98		Added to GM_ChangeSampleReverb an amount of 25 plus the enable threshold to
**				actaully active the verb. Also optimized GM_ChangeSampleReverb a little.
**	9/12/98		Added GM_GetSamplePlaybackPointer
**	10/17/98	Fixed almost all (!) the functions that change sample related parameters to
**				didn't allow a voice sample in voice 0 to be modified.
**	11/9/98		Renamed NoteDur to voiceMode
**	11/24/98	Added GM_GetSampleReverbAmount & GM_SetSampleReverbAmount
**	3/1/99		Added VOICE_REFERENCE and changed all sample API's to use this new voice
**				reference type.
**				Changed NoteRefNum to NoteContext
**	3/3/99		Renamed myMusicGlobals to pMixer
**				Changed GM_IsSoundDone to scan all voices rather the limited range
**				Changed GM_EndSample to use function rather than direct reference
**				Removed all extra references to MusicGlobals
**				Added GM_GetSampleStartTimeStamp
**				Renamed GM_BeginSample to GM_SetupSample, GM_BeginDoubleBuffer to GM_SetupSampleDoubleBuffer,
**				GM_BeginSampleFromInfo to GM_SetupSampleFromInfo, and added GM_StartSample
**				Added GM_GetSampleFrequencyFilter GM_SetSampleFrequencyFilter GM_GetSampleResonanceFilter 
**				GM_SetSampleResonanceFilter GM_GetSampleLowPassAmountFilter GM_SetSampleLowPassAmount
**	3/5/99		Added threadContext to PV_ServeEffectCallbacks
**				Added GM_SetSyncSampleStartReference & GM_SyncStartSample
**	3/6/99		Added documentation
**	3/8/99		Renamed GM_EndSoundEffects to GM_EndAllSamples
**	3/12/99		Put in support for different loops
**	5/28/99		MOE:  Changed GM_SetSampleReverbAmount() to eliminate
**				data-truncation warning for "amount"
**	7/13/99		Renamed HAE to BAE
**	10/19/99	MSD: switched to REVERB_USED and LOOPS_USED
**	11/4/99		MSD: fixed bug with NoteNextSize in GM_ChangeSamplePitch(); needed to calculate
**				number of samples in a slice.
**	11/10/99	Added ifdefs to handle the case REVERB_USED == REVERB_DISABLED
**	1/31/2000	Added GM_SetWaveformLoopPoints(), GM_GetWaveformLoopPoints()
**				Added GM_SetWaveformByteSize(), GM_GetWaveformByteSize()
**				Added GM_SetWaveformNumFrames(), GM_GetWaveformNumFrames()
**				Added GM_SetWaveformBitDepth(), GM_GetWaveformBitDepth()
**				Added GM_SetWaveformNumChannels(), GM_GetWaveformNumChannels()
**				Added GM_SetWaveformSampleRate(), GM_GetWaveformSampleRate()
**				Added GM_SetWaveformBaseMidiPitch(), GM_GetWaveformBaseMidiPitch()
**				Added GM_SetWaveformSampleData(), GM_GetWaveformSampleData()
**				Added GM_NewWaveform()
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	2/5/2000	Fixed compiler warning in GM_SetWaveformNumChannels & GM_SetWaveformBitDepth
**	4/19/2000	Fixed bug in GM_SetupSampleFromInfo in which the loop points were not being
**				shifted when an starting offset was supplied.
**	5/8/2000	jsc: changed GM_SetSampleLoopPoints to correctly handle invalid
**				or all-zero loop points.
**	7/11/2000	DS:	Added code to correctly offset voice samplePosition when start frame
**					is non-zero.  Added startFrame arg to GM_SetupSample.  Removed
**					4/19 fix in the process.
**	1/2/2001	sh	Happy New Space Odyssey 2001! Added GM_SetSamplePlaybackPosition.
**					Changed copyright.
**	4/30/2001	sh	Added code to start a voice on DSP.
**	5/1/2001	sh	Fixed warnings.
**	5/1/2001	se	Fixed pointer dereference checks.
**	1/27/2002	sh	Removed warnings.
**	10/21/2002	sh	Added PV_FindFreeSampleVoice, and moved all voice aquiring methods
**					to use it.
*/
/*****************************************************************************/
#include "GenSnd.h"
#include "GenPriv.h"
#include "BAE_API.h"

/*
	Description of use:
	
	Call GM_SetEffectsVolume and GM_GetEffectsVolume to control the overal mix level of sound effects into
	the mixer.
	
	In general, all of these functions create, manipulate and dispose of a VOICE_REFERENCE. The VOICE_REFERENCE
	is a direct connect to an active or about to be active voice in the mixer. There are a limited number of
	them. MAX_VOICES to be exact.
	
	There are 3 ways to get a VOICE_REFERENCE. Call GM_SetupSample, GM_SetupSampleDoubleBuffer, or GM_SetupSampleFromInfo
	based upon your particular needs.

	This will allocate a VOICE_REFERENCE and preserve a voice in the mixer. You then need to call GM_StartSample
	to start the voice. Once the voice finishes it will be marked VOICE_UNUSED. You can also call GM_EndSample
	or GM_EndAllSamples.
	
	MODIFIERS:
	Modifier functions of active VOICE_REFERENCE's. All functions will effect the voice within 11 ms
	of changing values.

	sample rate			GM_ChangeSamplePitch GM_GetSamplePitch

	volume				GM_ChangeSampleVolume GM_GetSampleVolumeUnscaled GM_GetSampleVolume GM_SetSampleFadeRate

	filter controls		GM_GetSampleFrequencyFilter GM_SetSampleFrequencyFilter GM_GetSampleResonanceFilter 
						GM_SetSampleResonanceFilter GM_GetSampleLowPassAmountFilter GM_SetSampleLowPassAmountFilter

	loop points			GM_SetSampleLoopPoints

	stereo placement	GM_ChangeSampleStereoPosition GM_GetSampleStereoPosition

	reverb controls		GM_GetSampleReverbAmount GM_SetSampleReverbAmount GM_ChangeSampleReverb GM_GetSampleReverb

	position			GM_GetSamplePlaybackPointer GM_GetSamplePlaybackPosition

	information			GM_GetSampleStartTimeStamp GM_IsSoundDone

	callbacks			GM_SetSampleOffsetCallbackLinks GM_AddSampleOffsetCallback GM_RemoveSampleOffsetCallback
						GM_SetSampleDoneCallback


	SYNC START
	To start samples at the same time, call one of the GM_Setup... functions then call GM_SetSyncSampleStartReference
	to set a unique refernece. The reference can be a pointer to a local structure. Its not used as anything other
	that common reference for all voices that you need to start at the moment. After they are started it is ignored.
	Then GM_SyncStartSample to actaully activate the voices. They will start at the next 11 ms slice. Be careful
	using these functions directly because they don't wait for the mixer slice to be ready, so you might actaully
	start voices between 11ms slices. The best way to insure it is to use the linked voices below. Those function
	use the ones described.

	LINKED VOICES
	Call one of the GM_SetupSample... functions in the various standard ways, to get an allocate voice
	then call GM_NewLinkedSampleList. Then add it to your maintained top list of linked voices with 
	by calling GM_AddLinkedSample. Use GM_FreeLinkedSampleList to delete an entire list, 
	or GM_RemoveLinkedSample to just one link.

	Then you can call GM_StartLinkedSamples to start them all at the same time, or GM_EndLinkedSamples
	to end them all, or GM_SetLinkedSampleVolume, GM_SetLinkedSampleRate, and GM_SetLinkedSamplePosition
	set parameters about them all.

	management			GM_NewLinkedSampleList GM_FreeLinkedSampleList GM_AddLinkedSample GM_RemoveLinkedSample

	info				GM_GetLinkedSamplePlaybackReference

	control				GM_StartLinkedSamples GM_EndLinkedSamples

	sync control		GM_SetLinkedSampleVolume GM_SetLinkedSampleRate GM_SetLinkedSamplePosition

*/


#if X_PLATFORM != X_WEBTV
// Process any fading effects voices
static void PV_ServeEffectsFades(void)
{
	long		count, minValue, maxValue;
	GM_Voice	*pVoice;
	long		value;
	GM_Mixer	*pMixer;

	pMixer = GM_GetCurrentMixer();
	if (pMixer)
	{
		minValue = pMixer->MaxNotes;					// only look this voice range
		maxValue =  minValue + (pMixer->MaxEffects-1);
		for (count = maxValue; count >= minValue; count--)
		{
			pVoice = &pMixer->NoteEntry[count];
			if (pVoice->voiceMode != VOICE_UNUSED)
			{
				if (pVoice->soundFadeRate)
				{
					pVoice->soundFixedVolume -= pVoice->soundFadeRate;
					value = XFIXED_TO_LONG(pVoice->soundFixedVolume);
					if (value > pVoice->soundFadeMaxVolume)
					{
						value = pVoice->soundFadeMaxVolume;
						pVoice->soundFadeRate = 0;
					}
					if (value < pVoice->soundFadeMinVolume)
					{
						value = pVoice->soundFadeMinVolume;
						pVoice->soundFadeRate = 0;
					}
					pVoice->NoteVolume = (INT32)value;
					pVoice->NoteMIDIVolume = (INT16)value;
					if ((pVoice->soundFadeRate == 0) && pVoice->soundEndAtFade)
					{
						GM_EndSample((VOICE_REFERENCE)count);
					}
				}
			}
		}
	}
}
#endif

#if USE_CALLBACKS
static void PV_ServeEffectCallbacks(void *threadContext)
{
	long					count, minValue, maxValue;
	unsigned long			offsetStart, offsetEnd;
	GM_Voice				*pVoice;
	GM_SampleCallbackEntry	*pCallbackEntry;
	GM_Mixer				*pMixer;

	pMixer = GM_GetCurrentMixer();
	if (pMixer)
	{
		minValue = pMixer->MaxNotes;					// only look this voice range
		maxValue =  minValue + (pMixer->MaxEffects-1);
		for (count = maxValue; count >= minValue; count--)
		{
			pVoice = &pMixer->NoteEntry[count];
			if (pVoice->voiceMode != VOICE_UNUSED)
			{
				pCallbackEntry = pVoice->pSampleMarkList;
				if (pCallbackEntry)
				{
					// get current position of sample
					offsetStart = PV_GetPositionFromVoice(pVoice);

					// get size of 1 time slice for this sample (11ms) and compute end point
					offsetEnd = offsetStart + pVoice->NoteNextSize;
					while (pCallbackEntry)
					{
						if ((pCallbackEntry->frameOffset >= offsetStart) && (pCallbackEntry->frameOffset <= offsetEnd) )
						{
							if (pCallbackEntry->pCallback)
							{
								(*pCallbackEntry->pCallback)(threadContext, pCallbackEntry->reference, PV_GetPositionFromVoice(pVoice));
								break;
							}
						}
						pCallbackEntry = pCallbackEntry->pNext;
					}
				}
			}
		}
	}
}
#endif

// process all sample events. Fades, callbacks, etc
void PV_ProcessSampleEvents(void *threadContext)
{
	PV_ServeEffectsFades();
	#if USE_CALLBACKS
		PV_ServeEffectCallbacks(threadContext);
	#endif
}


#if X_PLATFORM != X_WEBTV
// range is 0 to MAX_MASTER_VOLUME (256). Note volume is from 0 to MAX_NOTE_VOLUME (127)
void GM_SetEffectsVolume(INT16 newVolume)
{
	register GM_Mixer		*pMixer;
	register LOOPCOUNT		count;
	register GM_Voice		*pVoice;
	short int				minValue, maxValue;

	if (newVolume < 0)
	{
		newVolume = 0;
	}
	if (newVolume > MAX_MASTER_VOLUME * 5)
	{
		newVolume = MAX_MASTER_VOLUME * 5;
	}
	pMixer = GM_GetCurrentMixer();
	if (pMixer)
	{
		minValue = pMixer->MaxNotes;
		maxValue =  minValue + pMixer->MaxEffects;

		pMixer->effectsVolume = newVolume;
		newVolume = (newVolume * MAX_NOTE_VOLUME) / MAX_MASTER_VOLUME; // scale

		// update the current notes playing to the new volume
		for (count = minValue; count < maxValue; count++)
		{
			pVoice = &pMixer->NoteEntry[count];
			if (pVoice->voiceMode != VOICE_UNUSED)
			{
				if (pVoice->NoteChannel == SOUND_EFFECT_CHANNEL)
				{
					// make sure and set the channel volume not scaled, because its scaled later
					if (newVolume == 0)
					{
						pVoice->voiceMode = VOICE_RELEASING;
						pVoice->NoteDecay = 0;
						pVoice->volumeADSRRecord.ADSRTime[0] = 1;
						pVoice->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
						pVoice->volumeADSRRecord.ADSRLevel[0] = 0;	// just in case
					}
					// now calculate the new volume based upon the current channel volume and
					// the unscaled note volume
					newVolume = (pVoice->NoteMIDIVolume * pMixer->MasterVolume) / MAX_MASTER_VOLUME;
					newVolume = (newVolume * pMixer->effectsVolume) / MAX_MASTER_VOLUME;
					pVoice->NoteVolume = newVolume;
				}
			}
		}
	}
}

INT16 GM_GetEffectsVolume(void)
{
	INT16	volume;

	volume = MAX_MASTER_VOLUME;
	if (MusicGlobals)
	{
		volume = MusicGlobals->effectsVolume;
	}
	return volume;
}
#endif

static GM_Voice * PV_FindFreeSampleVoice(GM_Mixer *pMixer, VOICE_REFERENCE *pOutputIndex)
{
	long		min, max, count;
	GM_Voice	*pVoice;

	pVoice = NULL;
	if (pOutputIndex)
	{
		*pOutputIndex = 0;
	}
	if (pMixer)
	{
		min = pMixer->MaxNotes;				// only pick a new voice within this range
		max =  min + pMixer->MaxEffects;
		for (count = min; count < max; count++)
		{
			if (pMixer->NoteEntry[count].voiceMode == VOICE_UNUSED)
			{
				pVoice = &pMixer->NoteEntry[count];
				*pOutputIndex = (VOICE_REFERENCE)count;
				break;
			}
		}
	//	printf("audio::sample found free voice %ld\n", pVoice - &pMixer->NoteEntry[0]);
	}
	return pVoice;
}


// given a VOICE_REFERENCE returned from GM_Begin... this will return TRUE, if voice is
// valid
XBOOL GM_IsSoundReferenceValid(VOICE_REFERENCE reference)
{
	XBOOL	goodVoice;
	
	goodVoice = FALSE;
	if (MusicGlobals)
	{
		if ( ((long)reference >= 0) && ((long)reference < (MusicGlobals->MaxNotes+MusicGlobals->MaxEffects)) )
		{
			goodVoice = TRUE;
		}
	}
	return goodVoice;
}

// given a VOICE_REFERENCE, this will return the GM_Voice associated to the reference in question
GM_Voice * PV_GetVoiceFromSoundReference(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;

	pVoice = NULL;
	if (GM_IsSoundReferenceValid(reference))
	{
		pVoice = &MusicGlobals->NoteEntry[(long)reference];
		// is voice alive?
		if (pVoice->voiceMode == VOICE_UNUSED)
		{
			pVoice = NULL;
		}		
	}
	return pVoice;
}

XBOOL GM_IsSoundDone(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	return (pVoice) ? FALSE : TRUE;
}                


// scan through fast, and get the largest peak
static void PV_CalculateLargestPeak(GM_Voice *pVoice)
{
	short 			min = 0;
	short 			max = 0;
	short 			sample;
	unsigned long	noiseMin, noiseMax;
	char 			*pStartB;
	short			*pStartW;
	char 			*pEndB;
	short			*pEndW;
	int				step;

	if (pVoice)
	{
		pStartW = (short *)pVoice->NotePtr;
		pStartB = (char *)pVoice->NotePtr;
		pEndW = (short *)pVoice->NotePtrEnd;
		pEndB = (char *)pVoice->NotePtrEnd;
		pVoice->largestPeak = 0;

		if (pStartW < pEndW)
		{
			step = 16 * pVoice->channels;		// frames to skip
			if (pVoice->bitSize == 8)
			{
				// 8 bit data is unsigned
				while (pStartB < pEndB)
				{
					sample = (short)*pStartB;
					sample = (sample - 128) * 256;		// convert to 16 bit
					if (sample > max)
					{
						max = sample;
					}
					if (sample < min)
					{
						min = sample;
					}
					pStartB += step;
				}
			}
			else
			{
				// 16 bit data is signed
				while (pStartW < pEndW)
				{
					sample = (short)*pStartW;
					if (sample > max)
					{
						max = sample;
					}
					if (sample < min)
					{
						min = sample;
					}
					pStartW += step;
				}
			}
			noiseMin = ((long)min + 32768L);
			noiseMax = ((long)32768L - max);
			if (noiseMin < noiseMax)
			{
				pVoice->largestPeak = noiseMin;
			}
			else
			{
				pVoice->largestPeak = noiseMax;
			}
			pVoice->largestPeak *= 1000;
		}
	}
}

#if USE_CALLBACKS
// setup a double buffer sound effect
// stereoPosition is in the range 63 to -63
VOICE_REFERENCE GM_SetupSampleDoubleBuffer(XPTR pBuffer1, XPTR pBuffer2, UINT32 theSize, XFIXED theRate,
							INT16 bitSize, INT16 channels,
							INT32 sampleVolume, INT16 stereoPosition,
							void *context,
							GM_DoubleBufferCallbackPtr bufferCallback,
							GM_SoundDoneCallbackPtr doneCallbackProc)
{
	register GM_Mixer	*pMixer;
	register GM_Voice	*pVoice;
	VOICE_REFERENCE		count;

	pMixer = GM_GetCurrentMixer();
#if LOOPS_USED == LIMITED_LOOPS
	if (pMixer && (pMixer->MaxEffects > 0) && (theSize < MAX_SAMPLE_FRAMES))
#else
	if (pMixer && (pMixer->MaxEffects > 0))
#endif
	{
		pVoice = PV_FindFreeSampleVoice(pMixer, &count);
		if ((pVoice != NULL) && (pVoice->voiceMode == VOICE_UNUSED))
		{
			pVoice->voiceMode = VOICE_ALLOCATED;		// allocate voice so no one else can grab it.
			PV_CleanNoteEntry(pVoice);					// fill with all zero's except voiceMode field.
			pVoice->noteSamplePitchAdjust = XFIXED_1;	// 1.0
			pVoice->doubleBufferProc = bufferCallback;
			pVoice->NotePtr = (UBYTE *) pBuffer1;
			pVoice->NoteStartFrame = 0;				// ALWAYS, for this case
			pVoice->NotePtrEnd = (XBYTE *) pBuffer1 + theSize;

			pVoice->doubleBufferPtr1 = (UBYTE *) pBuffer1;
			pVoice->doubleBufferPtr2 = (UBYTE *) pBuffer2;

			// loop defaults to entire sample length
			pVoice->NoteLoopPtr = pVoice->NotePtr;
			pVoice->NoteLoopEnd = pVoice->NotePtrEnd;

			pVoice->NotePitch = (XFIXED)theRate / 22050;
			pVoice->NoteLoopProc = NULL;

			pVoice->NoteEndCallback = doneCallbackProc;
			pVoice->NoteProgram = -1;      
			pVoice->stereoPosition = stereoPosition;
			pVoice->bitSize = (UBYTE)bitSize;
			pVoice->channels = (UBYTE)channels;
			pVoice->avoidReverb = TRUE;
			pVoice->soundFadeRate = 0;

			pVoice->NoteMIDIVolume = (INT16)sampleVolume;	// save unscaled
			sampleVolume = (sampleVolume * pMixer->effectsVolume) / MAX_MASTER_VOLUME;
			sampleVolume = (sampleVolume * pMixer->MasterVolume) / MAX_MASTER_VOLUME;

			pVoice->NoteVolume = sampleVolume;
			pVoice->NoteVolumeEnvelope = VOLUME_RANGE;
			pVoice->volumeADSRRecord.ADSRLevel[0] = VOLUME_RANGE;
			pVoice->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
			pVoice->volumeADSRRecord.currentLevel = VOLUME_RANGE;
			pVoice->volumeADSRRecord.currentPosition = 0;
			pVoice->volumeADSRRecord.sustainingDecayLevel = XFIXED_1;

			pVoice->NoteChannel = SOUND_EFFECT_CHANNEL;
			pVoice->NoteDecay = 0x7FFF;		// never release
			pVoice->NoteContext = context;
			pVoice->sustainMode = SUS_NORMAL;
// is there an initial volume level in the ADSR record that starts at time=0?  If so, don't interpolate the
// note's volume up from 0 to the first target level.  Otherwise, it's a traditional ramp-up from 0.
			if (pMixer->generateStereoOutput)
			{
				PV_CalculateStereoVolume(pVoice, &pVoice->lastAmplitudeL, &pVoice->lastAmplitudeR);
			}
			else
			{
				PV_CalculateMonoVolume(pVoice, &pVoice->lastAmplitudeL);
			}
			//pVoice->voiceStartTimeStamp = XMicroseconds();
			//pVoice->voiceMode = VOICE_SUSTAINING;
			return (VOICE_REFERENCE)count;
		}
	}
	return DEAD_VOICE;
}
#endif

// stereoPosition is in the range 63 to -63
VOICE_REFERENCE GM_SetupSample(XPTR theData, UINT32 startFrame, UINT32 frames, XFIXED theRate, 
				UINT32 startLoopFrame, UINT32 endLoopFrame, UINT32 theLoopTarget,
				INT32 sampleVolume, INT32 stereoPosition,
				void *context, INT16 bitSize, INT16 channels, 
				GM_LoopDoneCallbackPtr theLoopContinueProc,
				GM_SoundDoneCallbackPtr theCallbackProc)
{
	register GM_Mixer	*pMixer;
	register GM_Voice	*pVoice;
	VOICE_REFERENCE		count;

	pMixer = GM_GetCurrentMixer();
#if LOOPS_USED == LIMITED_LOOPS
	if (pMixer && (pMixer->MaxEffects > 0) && (frames < MAX_SAMPLE_FRAMES))
#else
	if (pMixer && (pMixer->MaxEffects > 0))
#endif
	{
		pVoice = PV_FindFreeSampleVoice(pMixer, &count);
		if ((pVoice != NULL) && (pVoice->voiceMode == VOICE_UNUSED))
		{
			pVoice->voiceMode = VOICE_ALLOCATED;	// allocate voice so no one else can grab it.
			PV_CleanNoteEntry(pVoice);				// zeroes ALL entries, except voiceMode
			PV_SetPositionFromVoice(pVoice, startFrame);	// set start offset
			pVoice->noteSamplePitchAdjust = XFIXED_1;

			pVoice->NotePtr = (UBYTE *) theData;
			pVoice->NotePtrEnd = (UBYTE *) theData + frames;
			pVoice->NoteStartFrame = startFrame;	// store this -- needed?
			pVoice->NotePitch = (XFIXED)theRate / 22050;

			// reset the NoteLoopCount
			pVoice->NoteLoopCount = 0;

#if USE_CALLBACKS
			pVoice->NoteLoopProc = theLoopContinueProc;
#endif
			// loop start and end pointers are set independently of the sample start
			if ( (startLoopFrame < endLoopFrame) && (endLoopFrame - startLoopFrame > MIN_LOOP_SIZE) )
			{
				pVoice->NoteLoopPtr = (UBYTE *)theData + startLoopFrame;
				pVoice->NoteLoopEnd = (UBYTE *)theData + endLoopFrame;
			
				// set the target number of loops
				pVoice->NoteLoopTarget = theLoopTarget;
			}
#if USE_CALLBACKS
			pVoice->NoteEndCallback = theCallbackProc;
			pVoice->NoteContext = context;
#endif
			pVoice->NoteProgram = -1;      
			pVoice->stereoPosition = (INT16)stereoPosition;
			pVoice->bitSize = (UBYTE)bitSize;
			pVoice->channels = (UBYTE)channels;
			pVoice->avoidReverb = TRUE;
			pVoice->LFORecordCount = 0;
			pVoice->pInstrument = NULL;
			pVoice->soundFadeRate = 0;
			pVoice->NoteMIDIVolume = (INT16)sampleVolume;	// save unscaled
			sampleVolume = (sampleVolume * pMixer->effectsVolume) / MAX_MASTER_VOLUME;
			sampleVolume = (sampleVolume * pMixer->MasterVolume) / MAX_MASTER_VOLUME;

			pVoice->NoteVolume = sampleVolume;
			pVoice->NoteVolumeEnvelope = VOLUME_RANGE;
			pVoice->volumeADSRRecord.ADSRLevel[0] = VOLUME_RANGE;
			pVoice->volumeADSRRecord.currentLevel = VOLUME_RANGE;
			pVoice->volumeADSRRecord.currentPosition = 0;
			pVoice->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
			pVoice->volumeADSRRecord.mode = ADSR_TERMINATE;
			pVoice->volumeADSRRecord.sustainingDecayLevel = XFIXED_1;
			pVoice->NoteChannel = SOUND_EFFECT_CHANNEL;
			pVoice->sustainMode = SUS_NORMAL;
			pVoice->sampleAndHold = 1;
			pVoice->NoteDecay = 32767;		// never release
// is there an initial volume level in the ADSR record that starts at time=0?  If so, don't interpolate the
// note's volume up from 0 to the first target level.  Otherwise, it's a traditional ramp-up from 0.

			if (pMixer->generateStereoOutput)
			{
				PV_CalculateStereoVolume(pVoice, &pVoice->lastAmplitudeL, &pVoice->lastAmplitudeR);
			}
			else
			{
				PV_CalculateMonoVolume(pVoice, &pVoice->lastAmplitudeL);
			}
			PV_CalculateLargestPeak(pVoice);

			//pVoice->voiceStartTimeStamp = XMicroseconds();
			//pVoice->voiceMode = VOICE_SUSTAINING;
			return (VOICE_REFERENCE)count;
		}
	}
	return DEAD_VOICE;
}

#if X_PLATFORM != X_WEBTV
// stereoPosition is in the range 63 to -63
VOICE_REFERENCE GM_SetupSampleFromInfo(GM_Waveform *pSample, void *context,
								INT32 sampleVolume, INT32 stereoPosition,
								GM_LoopDoneCallbackPtr theLoopContinueProc,
								GM_SoundDoneCallbackPtr theCallbackProc,
								UINT32 startOffsetFrame)
{
	if (pSample)
	{
		if (startOffsetFrame > pSample->waveFrames)
		{
			startOffsetFrame = 0;
		}
		return GM_SetupSample(  pSample->theWaveform,
								startOffsetFrame,
								pSample->waveFrames,
								pSample->sampledRate,
								pSample->startLoop,
								pSample->endLoop,
								pSample->numLoops,
								sampleVolume, stereoPosition, context,
								pSample->bitSize,
								pSample->channels,
								theLoopContinueProc, theCallbackProc);
	}							
	return DEAD_VOICE;
}
#endif

// set all the voices you want to start at the same time the same syncReference. Then call GM_SyncStartSample
// to start the sync start. Will return an error if its an invalid reference, or syncReference is NULL.
OPErr GM_SetSyncSampleStartReference(VOICE_REFERENCE reference, void *syncReference)
{
	GM_Voice	*pVoice;
	OPErr		err;

	err = NO_ERR;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (syncReference)
		{
			pVoice->syncVoiceReference = syncReference;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NOT_SETUP;
	}
	return err;
}

// Once you have called GM_SetSyncSampleStartReference on all the voices, this will set them to start at the next
// mixer slice. Will return an error if its an invalid reference, or syncReference is NULL.
OPErr GM_SyncStartSample(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;
	OPErr		err;

	err = NO_ERR;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (pVoice->syncVoiceReference)
		{
			pVoice->voiceMode = VOICE_ALLOCATED_READY_TO_SYNC_START;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NOT_SETUP;
	}
	return err;
}

// after voice is setup, call this to start it playing now. returns 0 if started
OPErr GM_StartSample(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;
	OPErr		err;

	err = NOT_SETUP;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->voiceStartTimeStamp = XMicroseconds();
#ifdef BAE_MCU
		if (GM_InitVoiceOnDSP(pVoice) == NO_ERR)
		{
			pVoice->voiceMode = VOICE_SUSTAINING;
		}
		else
		{
			pVoice->voiceMode = VOICE_UNUSED;
		}
#else
		pVoice->voiceMode = VOICE_SUSTAINING;
#endif
		err = NO_ERR;
	}
	return err;
}

void GM_EndSample(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->largestPeak = 0;
#if USE_CALLBACKS
		PV_DoCallBack(pVoice);
#endif

		pVoice->voiceMode = VOICE_UNUSED;
#ifndef BAE_COMPLETE
		GM_KillVoiceOnDSP(pVoice);
#endif
	}
}

XBOOL GM_IsSampleProcessing(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;
	XBOOL		process = FALSE;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		process = pVoice->processingSlice;
	}
	return process;
}


// Stop just sound effects
void GM_EndAllSamples(void)
{           
	register LOOPCOUNT count;
	register GM_Voice *pVoice;

	if (MusicGlobals)
	{
		for (count = MusicGlobals->MaxNotes; count < MusicGlobals->MaxNotes + MusicGlobals->MaxEffects; count++)
		{
			pVoice = &MusicGlobals->NoteEntry[count];
			if (pVoice->voiceMode != VOICE_UNUSED)
			{
#if USE_CALLBACKS
				PV_DoCallBack(pVoice);
#endif
				pVoice->voiceMode = VOICE_UNUSED;
#ifndef BAE_COMPLETE
				GM_KillVoiceOnDSP(pVoice);
#endif
			}
		}
	}
}


#if X_PLATFORM != X_WEBTV

unsigned long GM_GetSampleStartTimeStamp(VOICE_REFERENCE reference)
{
	GM_Voice		*pVoice;
	unsigned long	time;

	time = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		time = pVoice->voiceStartTimeStamp;
	}
	return time;
}

// given a valid voice, return the current playback position
unsigned long GM_GetSamplePlaybackPosition(VOICE_REFERENCE reference)
{
	unsigned long	position;
	GM_Voice		*pVoice;

	position = 0L;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		// get current position of sample
		position = PV_GetPositionFromVoice(pVoice);
	}
	return position;
}

// given a valid voice, set the position of the current playback
OPErr GM_SetSamplePlaybackPosition(VOICE_REFERENCE reference, unsigned long framePos)
{
	GM_Voice		*pVoice;
	OPErr			err;

	err = NO_ERR;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		// set current position of sample
		PV_SetPositionFromVoice(pVoice, framePos);
	}
	else
	{
		err = NOT_SETUP;
	}
	return err;
}

void * GM_GetSamplePlaybackPointer(VOICE_REFERENCE reference, unsigned long *outFrameLength)
{
	void			*pointer;
	GM_Voice		*pVoice;

	pointer = NULL;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice && outFrameLength)
	{
		// get current pointer
		pointer = (void *)pVoice->NotePtr;
		*outFrameLength = pVoice->NotePtrEnd - pVoice->NotePtr;
	}
	return pointer;
}
#endif

#if USE_CALLBACKS
void GM_SetSampleDoneCallback(VOICE_REFERENCE reference, GM_SoundDoneCallbackPtr theCallbackProc, void *context)
{
	GM_Voice		*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->NoteContext = context;
		pVoice->NoteEndCallback = theCallbackProc;
	}
}
#endif

#if X_PLATFORM != X_WEBTV
XFIXED GM_GetSamplePitch(VOICE_REFERENCE reference)
{
	XFIXED		rate;
	GM_Voice	*pVoice;

	rate = LONG_TO_XFIXED(22050L);
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		rate = pVoice->NotePitch * 22050L;
	}
	return rate;
}

void GM_ChangeSamplePitch(VOICE_REFERENCE reference, XFIXED theNewRate)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->NotePitch = theNewRate / 22050L;
		// Recalculate number of samples in a slice
		pVoice->NoteNextSize = 0;
	}
}

void GM_SetSampleRouteBus(VOICE_REFERENCE reference, short int routeBus)
{
	GM_Voice	*pVoice;
	GM_Mixer	*pMixer;

	pMixer = GM_GetCurrentMixer();
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->routeBus = routeBus;
		if (pMixer->generateStereoOutput)
		{
			PV_CalculateStereoVolume(pVoice, &pVoice->lastAmplitudeL, &pVoice->lastAmplitudeR);
		}
		else
		{
			PV_CalculateMonoVolume(pVoice, &pVoice->lastAmplitudeL);
		}
	}
}

// amount range is -255 to 255
void GM_SetSampleLowPassAmountFilter(VOICE_REFERENCE reference, short int amount)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->LPF_lowpassAmount = (INT32)amount;
		pVoice->LPF_base_lowpassAmount = (INT32)amount;
	}
}

// amount range is -255 to 255
short int GM_GetSampleLowPassAmountFilter(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;
	short int	amount;

	amount = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		amount = (short int)pVoice->LPF_lowpassAmount;
	}
	return amount;
}

// resonance range is 0 to 256
void GM_SetSampleResonanceFilter(VOICE_REFERENCE reference, short int resonance)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->LPF_resonance = (INT32)resonance;
		pVoice->LPF_base_resonance = (INT32)resonance;
	}
}

// resonance range is 0 to 256
short int GM_GetSampleResonanceFilter(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;
	short int		resonance;

	resonance = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		resonance = (short int)pVoice->LPF_resonance;
	}
	return resonance;
}

// frequency range is 512 to 32512
void GM_SetSampleFrequencyFilter(VOICE_REFERENCE reference, short int frequency)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->LPF_frequency = (INT32)frequency;
		pVoice->LPF_base_frequency = (INT32)frequency;
	}
}

// frequency range is 512 to 32512
short int GM_GetSampleFrequencyFilter(VOICE_REFERENCE reference)
{
	GM_Voice	*pVoice;
	short int	frequency;

	frequency = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		frequency = (short int)pVoice->LPF_frequency;
	}
	return frequency;
}


#endif


#if REVERB_USED != REVERB_DISABLED
// return the current amount of reverb mix. 0-127 is the range.
short int GM_GetSampleReverbAmount(VOICE_REFERENCE reference)
{
	short int	amount;
	GM_Voice	*pVoice;

	amount = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		amount = pVoice->reverbLevel;
	}
	return amount;
}

// set amount of reverb to mix. 0-127 is the range.
//MOE: "amount" should be typed UBYTE
void GM_SetSampleReverbAmount(VOICE_REFERENCE reference, short int amount)
{
	GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (reference)
		{
			GM_ChangeSampleReverb(reference, TRUE);		// force on

			//MOE: This cast should eventually be eliminated.
			pVoice->reverbLevel = (UBYTE)amount;
		}
		else
		{	// off
			GM_ChangeSampleReverb(reference, FALSE);	// force off
		}
	}
}

// Get current status of reverb. On or off
XBOOL GM_GetSampleReverb(VOICE_REFERENCE reference)
{
	XBOOL 		enable;
	GM_Voice	*pVoice;

	enable = FALSE;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (pVoice->avoidReverb == FALSE)
		{
			enable = TRUE;
		}
	}
	return enable;
}

// change status of reverb. Force on, or off
void GM_ChangeSampleReverb(VOICE_REFERENCE reference, XBOOL enable)
{
	GM_Voice		*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (enable)
		{	// verb on
			pVoice->avoidReverb = FALSE;
			pVoice->reverbLevel = GM_GetReverbEnableThreshold() + 25;
		}
		else
		{	// verb off
			pVoice->avoidReverb = TRUE;
			pVoice->reverbLevel = 0;
		}
	}
}
#endif

INT16 GM_GetSampleVolume(VOICE_REFERENCE reference)
{
	INT16		volume;
	GM_Voice	*pVoice;

	volume = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (pVoice->voiceMode != VOICE_UNUSED)
		{
			volume = (pVoice->NoteVolume * MAX_MASTER_VOLUME) / MusicGlobals->effectsVolume;
			volume = (volume * MAX_MASTER_VOLUME) / MusicGlobals->MasterVolume;
		}
	}
	return volume;
}

// Return volume from a sample that is not scaled
INT16 GM_GetSampleVolumeUnscaled(VOICE_REFERENCE reference)
{
	INT16		volume;
	GM_Voice	*pVoice;

	volume = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if (pVoice->voiceMode != VOICE_UNUSED)
		{
			volume = pVoice->NoteMIDIVolume;
		}
	}
	return volume;
}

// Volume range is from 0 to MAX_NOTE_VOLUME
void GM_ChangeSampleVolume(VOICE_REFERENCE reference, INT16 sampleVolume)
{
	register GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->NoteMIDIVolume = sampleVolume;	// save unscaled
		sampleVolume = (sampleVolume * MusicGlobals->effectsVolume) / MAX_MASTER_VOLUME;
		sampleVolume = (sampleVolume * MusicGlobals->MasterVolume) / MAX_MASTER_VOLUME;
		pVoice->NoteVolume = sampleVolume;
	}
}

#if X_PLATFORM != X_WEBTV
void GM_SetSampleLoopPoints(VOICE_REFERENCE reference, unsigned long start, unsigned long end)
{
	register GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		if ( (start < end) && (end - start > MIN_LOOP_SIZE) )
		{
			pVoice->NoteLoopPtr = (UBYTE *)pVoice->NotePtr + start;
			pVoice->NoteLoopEnd = (UBYTE *)pVoice->NotePtr + end;
		}
		else
		{
			pVoice->NoteLoopPtr = 0;
			pVoice->NoteLoopEnd = 0;
		}
	}
}
#endif


OPErr GM_SetWaveformLoopPoints(GM_Waveform *pWave, unsigned long start, unsigned long end)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (start > end)
		{
			theErr = PARAM_ERR;
		}
		else if (end > pWave->waveFrames)
		{
			theErr = PARAM_ERR;
		}
		else
		{
			// params in range.
			if (start == end)
			{
				// then we're going to shut the loop point off
				start = 0;
				end = 0;
			}
			else
			{
				if ( (end - start) < MIN_LOOP_SIZE)
				{
					theErr = BUFFER_TO_SMALL;
				}
			}
		}

		if (theErr == NO_ERR)
		{
			pWave->startLoop = start;
			pWave->endLoop = end;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}


OPErr GM_GetWaveformLoopPoints(GM_Waveform *pWave, unsigned long *outStart, unsigned long *outEnd)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outStart && outEnd)
		{
			*outStart = pWave->startLoop;
			*outEnd = pWave->endLoop;
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


OPErr GM_SetWaveformByteSize(GM_Waveform *pWave, XDWORD byteSize)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->waveSize = byteSize;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformByteSize(GM_Waveform *pWave, XDWORD *outByteSize)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outByteSize)
		{
			*outByteSize = pWave->waveSize;
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


OPErr GM_SetWaveformNumFrames(GM_Waveform *pWave, XDWORD numFrames)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->waveFrames = numFrames;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformNumFrames(GM_Waveform *pWave, XDWORD *outNumFrames)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outNumFrames)
		{
			*outNumFrames = pWave->waveFrames;
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

OPErr GM_SetWaveformBitDepth(GM_Waveform *pWave, XWORD bitDepth)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->bitSize = (XBYTE)bitDepth;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformBitDepth(GM_Waveform *pWave, XWORD *outBitDepth)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outBitDepth)
		{
			*outBitDepth = pWave->bitSize;
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


OPErr GM_SetWaveformNumChannels(GM_Waveform *pWave, XWORD numChannels)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->channels = (XBYTE)numChannels;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformNumChannels(GM_Waveform *pWave, XWORD *outNumChannels)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outNumChannels)
		{
			*outNumChannels = pWave->channels;
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


OPErr GM_SetWaveformSampleRate(GM_Waveform *pWave, XFIXED sampleRate)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->sampledRate = sampleRate;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformSampleRate(GM_Waveform *pWave, XFIXED *outSampleRate)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outSampleRate)
		{
			*outSampleRate = pWave->sampledRate;
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


OPErr GM_SetWaveformBaseMidiPitch(GM_Waveform *pWave, XWORD baseMidiPitch)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->baseMidiPitch = baseMidiPitch;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformBaseMidiPitch(GM_Waveform *pWave, XWORD *outBaseMidiPitch)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outBaseMidiPitch)
		{
			*outBaseMidiPitch = pWave->baseMidiPitch;
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


OPErr GM_SetWaveformSampleData(GM_Waveform *pWave, XPTR sampleData)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		pWave->theWaveform = sampleData;
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return theErr;
}

OPErr GM_GetWaveformSampleData(GM_Waveform *pWave, XPTR *outSampleData)
{
	OPErr theErr;

	theErr = NO_ERR;
	if (pWave)
	{
		if (outSampleData)
		{
			*outSampleData = pWave->theWaveform;
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



// Set sample fade rate. Its a 16.16 fixed value
// Input:	reference		sound to affect
//			fadeRate	amount to change every 11 ms
//						example:	FLOAT_TO_XFIXED(2.2) will decrease volume
//									FLOAT_TO_XFIXED(2.2) * -1 will increase volume
//			minVolume	lowest volume level fade will go
//			maxVolume	highest volume level fade will go
#if X_PLATFORM != X_WEBTV
void GM_SetSampleFadeRate(VOICE_REFERENCE reference, XFIXED fadeRate, 
							INT16 minVolume, INT16 maxVolume, XBOOL endSample)
{
	register GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->soundFadeMaxVolume = maxVolume * 4;
		pVoice->soundFadeMinVolume = minVolume * 4;
		pVoice->soundFixedVolume = LONG_TO_XFIXED(pVoice->NoteVolume);
		pVoice->soundEndAtFade = endSample;
		pVoice->soundFadeRate = fadeRate;
	}
}
#endif

#if X_PLATFORM != X_WEBTV
// range from -63 to 63
INT16 GM_GetSampleStereoPosition(VOICE_REFERENCE reference)
{
	INT16		pos;
	GM_Voice	*pVoice;

	pos = 0;
	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pos = pVoice->stereoPosition;
	}
	return pos;
}

// range from -63 to 63
void GM_ChangeSampleStereoPosition(VOICE_REFERENCE reference, INT16 newStereoPosition)
{
	register GM_Voice	*pVoice;

	pVoice = PV_GetVoiceFromSoundReference(reference);
	if (pVoice)
	{
		pVoice->stereoPosition = newStereoPosition;
	}
}
#endif

GM_Waveform *GM_NewWaveform(void)
{
	GM_Waveform *result;

	result = (GM_Waveform *)XNewPtr(sizeof(GM_Waveform));

	return result;
}

// Frees memory associated with GM_Waveform structure and the structure itself. Don't
// call this on a GM_Waveform that has been allocated on the stack.
void GM_FreeWaveform(GM_Waveform *pWaveform)
{
	if (pWaveform)
	{
		XDisposePtr((XPTR)pWaveform->theWaveform);
		XDisposePtr((XPTR)pWaveform);
	}
}

#if USE_CALLBACKS
void GM_SetSampleOffsetCallbackLinks(VOICE_REFERENCE reference, GM_SampleCallbackEntry *pTopEntry)
{
	GM_Voice				*pVoice;

	if (pTopEntry)
	{
		pVoice = PV_GetVoiceFromSoundReference(reference);
		if (pVoice)
		{
			pVoice->pSampleMarkList = pTopEntry;
		}
	}
}



void GM_AddSampleOffsetCallback(VOICE_REFERENCE reference, GM_SampleCallbackEntry *pEntry)
{
	GM_Voice				*pVoice;
	GM_SampleCallbackEntry	*pNext;

	if (pEntry)
	{
		pVoice = PV_GetVoiceFromSoundReference(reference);
		if (pVoice)
		{
			pNext = pVoice->pSampleMarkList;
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
				pVoice->pSampleMarkList = pEntry;
			}
			else
			{
				pNext->pNext = pEntry;
			}
		}
	}
}

void GM_RemoveSampleOffsetCallback(VOICE_REFERENCE reference, GM_SampleCallbackEntry *pEntry)
{
	GM_Voice				*pVoice;
	GM_SampleCallbackEntry	*pNext, *pLast;

	if (pEntry)
	{
		pVoice = PV_GetVoiceFromSoundReference(reference);
		if (pVoice)
		{
			pLast = pNext = pVoice->pSampleMarkList;
			while (pNext)
			{
				if (pNext == pEntry)								// found object in list?
				{
					if (pNext == pVoice->pSampleMarkList)			// is object the top object
					{
						pVoice->pSampleMarkList = pNext->pNext;		// yes, change to next object
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
	}
}
#endif


#if X_PLATFORM != X_WEBTV
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
// linked samples
// Call one of the GM_SetupSample... functions in the various standard ways, to get an allocate voice
// then call GM_NewLinkedSampleList. Then add it to your maintained top list of linked voices with 
// by calling GM_AddLinkedSample. Use GM_FreeLinkedSampleList to delete an entire list, 
// or GM_RemoveLinkedSample to just one link.
//
// Then you can call GM_StartLinkedSamples to start them all at the same time, or GM_EndLinkedSamples
// to end them all, or GM_SetLinkedSampleVolume, GM_SetLinkedSampleRate, and GM_SetLinkedSamplePosition
// set parameters about them all.

// private structure of linked voices
struct GM_LinkedVoice
{
	VOICE_REFERENCE			playbackReference;
	struct GM_LinkedVoice	*pNext;
};
typedef struct GM_LinkedVoice GM_LinkedVoice;


LINKED_VOICE_REFERENCE GM_NewLinkedSampleList(VOICE_REFERENCE reference)
{
	GM_LinkedVoice	*pNew;

	pNew = NULL;
	if (GM_IsSoundReferenceValid(reference))
	{
		pNew = (GM_LinkedVoice *)XNewPtr(sizeof(GM_LinkedVoice));
		if (pNew)
		{
			pNew->playbackReference	= reference;
			pNew->pNext = NULL;
		}
	}
	return (LINKED_VOICE_REFERENCE)pNew;
}

void GM_FreeLinkedSampleList(LINKED_VOICE_REFERENCE pTop)
{
	GM_LinkedVoice	*pNext, *pLast;

	pNext = (GM_LinkedVoice *)pTop;
	while (pNext)
	{
		pLast = pNext;
		pNext = pNext->pNext;
		XDisposePtr((XPTR)pLast);
	}
}

// Given a top link, and a new link this will add to a linked list, and return a new top
// if required.
LINKED_VOICE_REFERENCE GM_AddLinkedSample(LINKED_VOICE_REFERENCE pTop, LINKED_VOICE_REFERENCE pEntry)
{
	GM_LinkedVoice	*pNext;

	if (pEntry)
	{
		pNext = (GM_LinkedVoice *)pTop;
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
			pNext->pNext = (GM_LinkedVoice *)pEntry;
		}
	}
	return pTop;
}

// Given a top link and an link to remove this will disconnect the link from the list and
// return a new top if required.
LINKED_VOICE_REFERENCE GM_RemoveLinkedSample(LINKED_VOICE_REFERENCE pTop, LINKED_VOICE_REFERENCE pEntry)
{
	GM_LinkedVoice	*pNext, *pLast;

	if (pEntry)
	{
		pLast = pNext = (GM_LinkedVoice *)pTop;
		while (pNext)
		{
			if (pNext == (GM_LinkedVoice *)pEntry)				// found object in list?
			{
				if (pNext == (GM_LinkedVoice *)pTop)			// is object the top object
				{
					pTop = pNext->pNext;						// yes, change to next object
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

VOICE_REFERENCE GM_GetLinkedSamplePlaybackReference(LINKED_VOICE_REFERENCE pLink)
{
	VOICE_REFERENCE			reference;

	reference = DEAD_VOICE;
	if (pLink)
	{
		reference = ((GM_LinkedVoice *)pLink)->playbackReference;
		if (GM_IsSoundReferenceValid(reference) == FALSE)
		{
			reference = DEAD_VOICE;
		}
	}
	return reference;
}


OPErr GM_StartLinkedSamples(LINKED_VOICE_REFERENCE pTop)
{
	GM_LinkedVoice		*pNext;
	OPErr				err;

	err = NO_ERR;	// ok, until proved otherwise
	if (MusicGlobals)
	{
		// set sync reference. Use our group because its easy and will be unique
		pNext = (GM_LinkedVoice *)pTop;
		while (pNext)
		{
			err = GM_SetSyncSampleStartReference(((GM_LinkedVoice *)pNext)->playbackReference, (void *)pTop);
			pNext = pNext->pNext;
		}
		if (err == NO_ERR)
		{
			// ok, now wait for mixer to be free
			while (MusicGlobals->insideAudioInterrupt)
			{
				XWaitMicroseocnds(BAE_GetSliceTimeInMicroseconds());
			}
			pNext = (GM_LinkedVoice *)pTop;
			while (pNext)
			{
				err = GM_SyncStartSample(((GM_LinkedVoice *)pNext)->playbackReference);
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


// end in unison the samples for all the linked samples
void GM_EndLinkedSamples(LINKED_VOICE_REFERENCE pTop)
{
	GM_LinkedVoice	*pNext;

	pNext = (GM_LinkedVoice *)pTop;
	while (pNext)
	{
		GM_EndSample(pNext->playbackReference);
		pNext = pNext->pNext;
	}
}

// Volume range is from 0 to MAX_NOTE_VOLUME
// set in unison the sample volume for all the linked samples
void GM_SetLinkedSampleVolume(LINKED_VOICE_REFERENCE pTop, INT16 sampleVolume)
{
	GM_LinkedVoice	*pNext;

	pNext = (GM_LinkedVoice *)pTop;
	while (pNext)
	{
		GM_ChangeSampleVolume(pNext->playbackReference, sampleVolume);
		pNext = pNext->pNext;
	}
}

// set in unison the sample rate for all the linked samples
void GM_SetLinkedSampleRate(LINKED_VOICE_REFERENCE pTop, XFIXED theNewRate)
{
	GM_LinkedVoice	*pNext;

	pNext = (GM_LinkedVoice *)pTop;
	while (pNext)
	{
		GM_ChangeSamplePitch(pNext->playbackReference, theNewRate);
		pNext = pNext->pNext;
	}
}


// set in unison the sample position for all the linked samples
// range from -63 to 63
void GM_SetLinkedSamplePosition(LINKED_VOICE_REFERENCE pTop, INT16 newStereoPosition)
{
	GM_LinkedVoice	*pNext;

	pNext = (GM_LinkedVoice *)pTop;
	while (pNext)
	{
		GM_ChangeSampleStereoPosition(pNext->playbackReference, newStereoPosition);
		pNext = pNext->pNext;
	}
}

#endif


// EOF of GenSample.c


