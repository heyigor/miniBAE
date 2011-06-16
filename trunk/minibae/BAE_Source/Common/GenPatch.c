/*****************************************************************************/
/*
** "GenPatch.c"
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
**	This contains code to load an maintain patches (instruments). This code library is specific
**	to the platform, but the API is independent in nature.
**
**
** Modification History:
**
**	7/7/95		Created
**	11/7/95		Major changes, revised just about everything.
**	11/20/95	Removed the BF_ flags, now you must walk through the union structure
**				Remove bit fields. BIT FIELDS DON'T WORK WITH MPW!!!!
**   12/6/95	Added reverb on/off bit
**	12/11/95	Enlarged PV_GetEnvelopeData to include the parsing for the external instrument
**				format
**	12/14/95	Modified PV_GetSampleData to accept an external snd handle and cache/convert
**				that into a sample
**	1/13/96		Modified PV_GetInstrument to only run SMOD on 8 bit mono data
**				Modified PV_GetEnvelopeData to support the extended format bit in the internal instrument
**	1/18/96		Spruced up for C++ extra error checking
**				Added MIN_LOOP_SIZE
**				Added GM_SetUsedInstrument & GM_IsInstrumentUsed & GM_IsInstrumentRangeUsed
**				and changed the way InstUseList is built. Now its only built with GM_LoadSongInstruments
**				and only loads the samples needed in the splits
**	1/28/96		Removed all uses of Resource Manager
**	1/29/96		Added useSampleRate factor for playback of instruments & sampleAndHold bits
**				Changed PV_CreateInstrumentFromResource to propigate features to splits
**	2/3/96		Removed extra includes
**	2/5/96		Removed unused variables. Working towards multiple songs
**	2/18/96		Added panPlacement to the GM_Instrument structure
**	2/21/96		Changed to support the change in XGetResourceAndDetach
**	3/25/96		Removed private GetLong to XGetLong
**	3/28/96		Changed PV_GetInstrument to support external instruments
**				Added PV_SetSampleIntoCache
**	4/10/96		Reworked the sample cache system to not clone the sample data
**	4/20/96		Added defines for max lfos, and max curves, MAX_LFOS & MAX_CURVES
**	4/21/96		Removed CPU edian issues by use XGetShort & XGetLong
**	5/12/96		Removed some warnings by fixing cast errors
**	5/18/96		More error correction in GM_LoadSongInstruments
**	5/19/96		Fixed an error condition in GM_UnloadInstrument
**	5/30/96		Added ignoreBadInstruments code
**	6/7/96		Added code to support the case inwhich there are no program changes
**				Added new version of GM_IsInstrumentUsed. Thanks Mark!
**	6/28/96		Changed PV_GetSoundResource to search for CSND first before SND
**	6/30/96		Fixed bug with PV_GetSoundResource (!)
**				Changed font and re tabbed
**	7/5/96		Fixed thread order problem with GM_UnloadInstrument & GM_UnloadSongInstruments
**	10/23/96	Removed reference to BYTE and changed them all to UBYTE or SBYTE
**				Added defines for instrument types
**				Changed GetKeySplitFromPtr to XGetKeySplitFromPtr
**	10/31/96	Added GM_IsInstrumentLoaded
**	12/9/96		Added GM_LoadInstrumentFromExternal
**	12/10/96	Fixed a ignoreBadInstruments bug that caused certain instruments to fail even though
**				the flag was set
**	12/30/96	Changed copyright
**	1/16/97		Changed GM_LFO to LFORecords
**	1/28/97		Removed STAND_ALONE define
**	1/30/97		Changed PV_GetSoundResource to search for ESND also
**				Moved PV_GetSoundResource to DriverTools.c and renamed XGetSoundResourceByID
**	3/10/97		Added code to cache samples and not to remove them before the referenceCount
**				reaches zero
**	4/14/97		Changed KeymapSplit to GM_KeymapSplit
**	4/20/97		Changed PV_MusicIRQ to PV_ProcessMidiSequencerSlice
**	4/21/97		Added volume levels per keysplit
**	4/24/97		Fixed volume bug when value is 0. Now defaults to 100.
**	7/8/97		Discovered a bug in which a non GM instrument tries to load but fails, and the
**				fall back to GM is suppose to select a GM instrument and load it, but it fails
**				because GM_LoadInstrument knows its not used from the sequencer scan. Need to
**				set the instrument used flags. NOTE: DID NOT FIX.
**	8/26/97		Fixed various compiler warnings for Sun
**	10/15/97	Modified GM_UnloadInstrument to return an error if instrument is still in use
**				at the other thread level.
**				Eliminated the reserved_1 from PV_CreateInstrumentFromResource & PV_GetEnvelopeData &
**				PV_GetInstrument
**				Added OPErr to GM_FlushInstrumentCache so return STILL_PLAYING if instrument
**				is busy
**	10/16/97	Changed GM_LoadSongInstruments to use a XBOOL for a flag rather than an int
**				Renamed ignoreBadPatches to ignoreBadInstruments
**				Removed lame support for instrument caching from GM_UnloadInstrument
**	10/27/97	Changed GM_UnloadSongInstruments to handle errors better
**	10/28/97	Changed PV_FindCacheFromID, theID parameter from short to long
**				Reduced code size a bit in PV_GetSampleFromID
**	12/1/97		Fixed a memory leak with GM_UnloadSongInstruments. Loop broke when
**				no error happened. Needed to wrap test around break.
**	1/20/98		Fixed GM_LoadSongInstruments to return the correct error code when failing
**	2/5/98		Added a GM_Song pointer to PV_SetSampleIntoCache
**	2/8/98		Changed BOOL_FLAG to XBOOL
**	5/4/98		Eliminated neverInterpolate & enablePitchRandomness from the 
**				GM_Instrument structure and all code that used it. Its not used.
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
**	6/18/98		Fixed problem with GM_LoadSongInstruments when using ignoreBadInstruments
**				it would still fail on certain types of percussion instruments
**	6/27/98		Fixed a sign problem when loading samples in PV_GetInstrument
**				and tweaked again.
**	7/1/98		Changed various API to use the new XResourceType and XLongResourceID
**	7/7/98		Modified PV_GetSampleData & PV_GetSampleFromID & & PV_ProcessSampleWithSMOD &
**				PV_CreateInstrumentFromResource to use new types
**	11/4/98		Changed PV_GetInstrument to handle the case in which useSoundModifierAsRootKey
**				is being used and the instrument tries to scale 8 bit samples.
**	11/6/98		Removed noteDecayPref from the GM_Waveform structure and changed various API's
**				to reflect that.
**	1/14/99		Added GM_LoadInstrumentFromExternalData
**	3/11/99		Renamed ADSRRecord to GM_ADSR. Renamed LFORecord to GM_LFO. Renamed CurveRecord to GM_TieTo.
**	5/10/99		Changed PV_GetInstrument & PV_SetSampleIntoCache & PV_GetSampleFromID to return error codes
**				Changed GM_LoadSongInstruments to fail on serious errors, while ignoring, if asked to, the
**				BAD_INSTRUMENT (not found) error.
**	5/14/99		Fixed problem with GM_LoadInstrument in which a BAD_SAMPLE was being returned when loading
**				a instrument with keysplits, and we need to ignore it.
**	6/15/99		Simplfied some references to the MusicGlobals object
**	6/22/99		Removed casting in PV_GetSampleData for theID
**	7/19/99		Bulletproof functions that assume a mixer is valid
**	10/19/99	MSD: Removed FAR macro
**	10/30/99	Modified PV_GetSampleData to remove unused field
**	11/4/99		MSD: Fixed potential memory leak in GM_UnloadSongInstruments() where it
**				would break out of the for loop if an instrument didn't unload.
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	3/3/2000	Moved GM_RemapInstrument to GenSong.c
**				Changed GM_GetInstrumentUsedRange & GM_SetInstrumentUsedRange to check for a usedPatch array
**				pointer prior to doing any work.
**				Removed extra instrumentReamp array from GM_LoadInstrumentFromExternalData & GM_LoadInstrument.
**				Changed GM_LoadSongInstruments to support the new call GM_SetupSongRemaps that sets up
**				remaps from an external resource ID_ALIAS
**	3/4/2000	Added PV_TranslateFromFileToMemoryID and changed various functions to use this new
**				way of parsing the instrument information.
**	3/27/2000	Changed PV_TranslateFromFileToMemoryID to work back the old way.
**				Fixed PV_TranslateFromFileToMemoryID. Was walking past the table
**				during its compare tests.
**				Fixed bug in PV_GetEnvelopeData. Was using PV_TranslateFromFileToMemoryID
**				on the filter values! Bad, Igor, bad...
**	4/3/2000	Added PV_UnloadInstrumentData & PV_UnloadSongInstrument and changed
**				GM_UnloadInstrument to call PV_UnloadSongInstrument.
**				Changed GM_GetSongInstrumentReamp to GM_GetSongInstrumentRemap.
**				Changed GM_UnloadSongInstruments to follow reamps when unload instruments.
**	4/15/2000	Fixed problem decoding instruments in INST_EXPONENTIAL_CURVE it used
**				the wrong constant for decoding. Now used ADSR_STAGES
**	2000.05.16 AER	Completed modifications for new sample cache
**	2000.05.17 AER	Made some minor modifications for Mac compile errors
**	2000.05.19 AER	Fixed problem in the fallback code that attempted to load
**						instruments that had failed their initial load. The
**						behavior would cause a resource leak by incrementing
**						the usageReferenceCount improperly
**					Remove bob -- oops ;)
**	2000.05.20 sh	Changed reference to 65536 to XFIXED_1 for sustainingDecayLevel
**	2000.05.25 sh	Use GM_SetSongLoopFlag/GM_GetSongLoopFlag instead of touching 
**					the structure GM_Song directly.
**	2000.05.28 sh	Made PV_UnloadInstrumentData semi public
**	6/26/2000  DS	Added missing line to copy parent instument pan into splits.
**	9/7/2000	sh	Modified PV_UnloadSongInstrument to make sure the passed
**					in instrument isn't currently ringing in the mixer.
**	11/1/2000	sh	In reading INST_EXPONENTIAL_CURVE, fixed a potential bug when filling
**					in last entry.
**				sh	Rewrote PV_TranslateFromFileToMemoryID because it was broken
**					and translated sustain tag flags out.
**	12/12/2000	sh	Changed GM_LoadSongInstruments to call GM_ResetTempoToDefault 
**					after pre scanning midi file.
**	5/29/2001	sh	Added new debugging system with BAE_PRINTF
**	7/5/2001	sh	Enabled use of GM_IsInstrumentRangeUsed in PV_GetInstrument
**	2/22/2002	sh	Fixed bug in GM_IsInstrumentUsed, in which the testing of the bits
**					didn't match the setting of the bits. Yerg, the pain.
**	5/15/2003	sh	In GM_LoadSongInstruments, when checking firstChannelProgram
**					after the midi prescan, actaully just change the channels
**					with no program changes to the DEFAULT_PATCH and load that
**					instrument. Much better handling of the case of midi data
**					with a note before a program change, and a note without
**					one.
*/
/*****************************************************************************/

#include "X_API.h"
#include "X_Formats.h"
#include "GenSnd.h"
#include "GenPriv.h"
#include "X_Assert.h"
#include "GenCache.h"
#include "X_Assert.h"


#define DEBUG_DISPLAY_PATCHES	1

#if BAE_NOT_USED
#include "SMOD.h"

// SMOD jump table
static void (*smod_functions[])(unsigned char *pSample, long length, long param1, long param2) =
{
	VolumeAmpScaler,		//	Amplifier/Volume Scaler
	NULL,
	NULL,
	NULL
};
#endif

// Private Functions


/******************************************************************************
**
**	GMCache_GetSamplePtr
**
**	Process a sample pointer with a SMOD
**
**	????.??.?? ???	Function created
**	2000.05.16 AER	Modified to stop using the old cache API and work hard-coded
**						until it is finally put out of its misery
**					(Accommodating this sort of thing would have bankrupted the
**						integrity of the new cache system)
**
******************************************************************************/
#if BAE_NOT_USED
static void PV_ProcessSampleWithSMOD(void *pSample,
									 long length,
									 long masterID,
									 XShortResourceID smodID,
									 short param1,
									 short param2)
{
	register INT16		count;

	if ( (smodID < SMOD_COUNT) && smod_functions[smodID])
	{
		for (count = 0; count < MAX_SAMPLES; count++)
		{
			if (MusicGlobals->sampleCaches[count])
			{
				if (MusicGlobals->sampleCaches[count]->pSampleData == pSample)
				{
					// Find sample, and remove its ID number so that other instrument that try to share this one
					// will force a new copy to be loaded.
					MusicGlobals->sampleCaches[count]->theID = -masterID;		// SMOD's sounds are negitive of their masters

					(*smod_functions[smodID])((unsigned char *)pSample, length, param1, param2);
					return;
				}
			}
		}
		//Can only get here if it's not in the cache...
		BAE_PRINTF("Something is wrong with sample cache. Can't find sample.\n");
	}
//	short int	sampleIndex;
//
//	if ( (smodID < SMOD_COUNT) && smod_functions[smodID])
//	{
//		sampleIndex = GM_FindCacheIndexFromPtr(pSample);
//		if (sampleIndex != -1)
//		{
//			// Find sample, and remove its ID number so that other instrument that try to share this one
//			// will force a new copy to be loaded.
//			MusicGlobals->sampleCaches[sampleIndex]->theID = -masterID;		// SMOD's sounds are negitive of their masters
//
//			(*smod_functions[smodID])((unsigned char *)pSample, length, param1, param2);
//		}
//		else
//		{
//			//DEBUG_STR("\pSomething is wrong with sample cache. Can't find sample.");
//		}
//	}
}
#endif


// All instrument control identifiers are stored in the file as 4 bytes. This function
// translates between the larger 4 byte id into a smaller 1 byte ID for in memory use.
static UNIT_TYPE PV_TranslateFromFileToMemoryID(XDWORD fileUnitType)
{
#if USE_MEMORY_OPTS == 0
	return fileUnitType;
#else
	typedef struct
	{
		UNIT_TYPE	memoryType;
		XDWORD		fileType;
	} MemoryToFileXlate;
	short int	count;

	MemoryToFileXlate types[] =
	{
		{ADSR_OFF,ADSR_OFF_LONG},
		{ADSR_LINEAR_RAMP,ADSR_LINEAR_RAMP_LONG},
		{ADSR_SUSTAIN,ADSR_SUSTAIN_LONG},
		{ADSR_TERMINATE,ADSR_TERMINATE_LONG},
		{ADSR_GOTO,ADSR_GOTO_LONG},
		{ADSR_GOTO_CONDITIONAL,ADSR_GOTO_CONDITIONAL_LONG},
		{ADSR_RELEASE,ADSR_RELEASE_LONG},

		{VOLUME_LFO_FREQUENCY,VOLUME_LFO_FREQUENCY_LONG},
		{PITCH_LFO_FREQUENCY,PITCH_LFO_FREQUENCY_LONG},
		{NOTE_VOLUME,NOTE_VOLUME_LONG},
		{VOLUME_ATTACK_TIME,VOLUME_ATTACK_TIME_LONG},
		{VOLUME_ATTACK_LEVEL,VOLUME_ATTACK_LEVEL_LONG},
		{SUSTAIN_RELEASE_TIME,SUSTAIN_RELEASE_TIME_LONG},
		{SUSTAIN_LEVEL,SUSTAIN_LEVEL_LONG},
		{RELEASE_TIME,RELEASE_TIME_LONG},
		{WAVEFORM_OFFSET,WAVEFORM_OFFSET_LONG},
		{SAMPLE_NUMBER,SAMPLE_NUMBER_LONG},
		{MOD_WHEEL_CONTROL,MOD_WHEEL_CONTROL_LONG},

		{SINE_WAVE,SINE_WAVE_LONG},
		{TRIANGLE_WAVE,TRIANGLE_WAVE_LONG},
		{SQUARE_WAVE,SQUARE_WAVE_LONG},
		{SQUARE_WAVE2,SQUARE_WAVE2_LONG},
		{SAWTOOTH_WAVE,SAWTOOTH_WAVE_LONG},
		{SAWTOOTH_WAVE2,SAWTOOTH_WAVE2_LONG},

		{VOLUME_LFO,VOLUME_LFO_LONG},
		{PITCH_LFO,PITCH_LFO_LONG},
		{STEREO_PAN_LFO,STEREO_PAN_LFO_LONG},
		{STEREO_PAN_NAME2,STEREO_PAN_NAME2_LONG},
		{LPF_FREQUENCY,LPF_FREQUENCY_LONG},
		{LPF_DEPTH,LPF_DEPTH_LONG},
		{LOW_PASS_AMOUNT,LOW_PASS_AMOUNT_LONG}
	};

	for (count = 0; count < (short int)(sizeof(types) / sizeof(MemoryToFileXlate)); count++)
	{
		if (fileUnitType == types[count].fileType)
		{
			return types[count].memoryType;
		}
	}
	//BAE_ASSERT(FALSE);	
	// should never get here, but if we did, then its a bad instrument, or some part of it
	// it not valid. Just return 0, to ignore.
	return 0;
#endif
}

// Given an external instrument resource and an internal resource type fill the envelope data
// and if not there, place a default envelope
static void PV_GetEnvelopeData(InstrumentResource	*theX, GM_Instrument *theI, long theXSize)
{
	long					count, count2, lfoCount;
	long					size, unitCount, unitType, unitSubCount;
	unsigned short int		data;
	register char 			*pData, *pData2;
	register char 			*pUnit;
	register KeySplit 		*pSplits;
	register GM_LFO			*pLFO;
	register GM_ADSR		*pADSR;
	register GM_TieTo		*pCurve;
	XBOOL					disableModWheel;

	disableModWheel = FALSE;
	theI->volumeADSRRecord.currentTime = 0;
	theI->volumeADSRRecord.currentPosition = 0;
	theI->volumeADSRRecord.currentLevel = 0;
	theI->volumeADSRRecord.previousTarget = 0;
	theI->volumeADSRRecord.mode = 0;
	theI->volumeADSRRecord.sustainingDecayLevel = XFIXED_1;

	theI->LPF_frequency = 0;
	theI->LPF_resonance = 0;
	theI->LPF_lowpassAmount = 0;

	// fill default
	theI->volumeADSRRecord.ADSRLevel[0] = VOLUME_RANGE;
	theI->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
	theI->volumeADSRRecord.ADSRTime[0] = 0;
	theI->LFORecordCount = 0;
	theI->curveRecordCount = 0;
	theI->extendedFormat = FALSE;
	pUnit = NULL;
	size = theXSize;
	if (theX && size)
	{
		if (theX->flags1 & ZBF_extendedFormat)
		{
			// search for end of tremlo data $8000. If not there, don't walk past end of instrument
			pSplits = (KeySplit *) ( ((char *)&theX->keySplitCount) + sizeof(short));
			count = XGetShort(&theX->keySplitCount);
			pData = (char *)&pSplits[count];
			pData2 = (char *)theX;
			size -= (pData - pData2);
			for (count = 0; count < size; count++)
			{
				data = XGetShort(&pData[count]);
				if (data == 0x8000)
				{
					count += 4;								// skip past end token and extra word
					data = (unsigned short)pData[count] + 1;			// get first string length;
					count2 = (long)pData[count+data] + 1;			// get second string length
					pUnit = (char *) (&pData[count + data + count2]);
					// NOTE: src will be non aligned, possibly on a byte boundry.
					break;
				}
			}
			if (pUnit)
			{
				theI->extendedFormat = TRUE;
				pUnit += 12;		// reserved global space

				unitCount = *pUnit;		// how many unit records?
				pUnit++;					// byte
				if (unitCount)
				{
					lfoCount = 0;
					for (count = 0; count < unitCount; count++)
					{
						unitType = XGetLong(pUnit) & 0x5F5F5F5F;
						pUnit += 4;	// long
						switch (unitType)
						{
							case INST_EXPONENTIAL_CURVE:
								if (theI->curveRecordCount >= MAX_CURVES)	// can only handle 4
								{
									goto bailoninstrument;
								}
								pCurve = &theI->curve[theI->curveRecordCount];
								theI->curveRecordCount++;
								pCurve->tieFrom = PV_TranslateFromFileToMemoryID(XGetLong(pUnit) & 0x5F5F5F5F); 
								pUnit += 4;
								pCurve->tieTo = PV_TranslateFromFileToMemoryID(XGetLong(pUnit) & 0x5F5F5F5F);
								pUnit += 4;
								unitSubCount = *pUnit++;
								if (unitSubCount > ADSR_STAGES)
								{
									goto bailoninstrument;
								}
								pCurve->curveCount = (short int)unitSubCount;
								for (count2 = 0; count2 < unitSubCount; count2++)
								{
									pCurve->from_Value[count2] = *pUnit++;
									pCurve->to_Scalar[count2] = XGetShort(pUnit);
									pUnit += 2;
								}
								// there's one extra slot in the definition to allow for this PAST the end of the 8 possible entries
								pCurve->from_Value[count2] = 127;
								pCurve->to_Scalar[count2] = pCurve->to_Scalar[count2-1];
								break;
							case INST_ADSR_ENVELOPE:
								unitSubCount = *pUnit;		// how many unit records?
								pUnit++;					// byte
								if (unitSubCount > ADSR_STAGES)
								{	// can't have more than ADSR_STAGES stages
									goto bailoninstrument;
								}
								pADSR = &theI->volumeADSRRecord;
								for (count2 = 0; count2 < unitSubCount; count2++)
								{
									pADSR->ADSRLevel[count2] = XGetLong(pUnit);
									pUnit += 4;

									pADSR->ADSRTime[count2] = XGetLong(pUnit);
									pUnit += 4;

									pADSR->ADSRFlags[count2] = PV_TranslateFromFileToMemoryID(XGetLong(pUnit) & 0x5F5F5F5F);
									pUnit += 4;
								}
								break;

							case INST_LOW_PASS_FILTER:		// low pass global filter parameters
								theI->LPF_frequency = XGetLong(pUnit);
								pUnit += 4;
								theI->LPF_resonance = XGetLong(pUnit);
								pUnit += 4;
								theI->LPF_lowpassAmount = XGetLong(pUnit);
								pUnit += 4;
								break;
								
							case INST_DEFAULT_MOD:		// default mod wheel hookup
								disableModWheel = TRUE;
								break;									

							// LFO types
							case INST_PITCH_LFO:
							case INST_VOLUME_LFO:
							case INST_STEREO_PAN_LFO:
							case INST_STEREO_PAN_NAME2:
							case INST_LOW_PASS_AMOUNT:
							case INST_LPF_DEPTH:
							case INST_LPF_FREQUENCY:
								if (lfoCount > MAX_LFOS)
								{
									goto bailoninstrument;
								}
								unitSubCount = *pUnit;		// how many unit records?
								pUnit++;					// byte
								if (unitSubCount > ADSR_STAGES)
								{	// can't have more than ADSR_STAGES stages
									//DEBUG_STR("\p too many stages");
									goto bailoninstrument;
								}
								pLFO = &theI->LFORecords[lfoCount];
								for (count2 = 0; count2 < unitSubCount; count2++)
								{
									pLFO->a.ADSRLevel[count2] = XGetLong(pUnit);
									pUnit += 4;
									pLFO->a.ADSRTime[count2] = XGetLong(pUnit);
									pUnit += 4;
									pLFO->a.ADSRFlags[count2] = PV_TranslateFromFileToMemoryID(XGetLong(pUnit) & 0x5F5F5F5F);
									pUnit += 4;
								}
								pLFO->where_to_feed = PV_TranslateFromFileToMemoryID(unitType & 0x5F5F5F5F);

								pLFO->period = XGetLong(pUnit);
								pUnit += 4;
								pLFO->waveShape = PV_TranslateFromFileToMemoryID(XGetLong(pUnit));
								pUnit += 4;
								pLFO->DC_feed = XGetLong(pUnit);
								pUnit += 4;
								pLFO->level = XGetLong(pUnit);
								pUnit += 4;

								pLFO->currentWaveValue = 0;
								pLFO->currentTime = 0;
								pLFO->LFOcurrentTime = 0;
								pLFO->a.currentTime = 0;
								pLFO->a.currentPosition = 0;
								pLFO->a.currentLevel = 0;
								pLFO->a.previousTarget = 0;
								pLFO->a.mode = 0;
								pLFO->a.sustainingDecayLevel = XFIXED_1;
								lfoCount++;
								break;
						}
					}

					if ((lfoCount < (MAX_LFOS-1)) && (theI->curveRecordCount < MAX_CURVES) && (disableModWheel == FALSE))
					{
						pCurve = &theI->curve[theI->curveRecordCount];
						theI->curveRecordCount++;
						
						// Create a straight-line curve function to tie mod wheel to pitch LFO
						pCurve->tieFrom = MOD_WHEEL_CONTROL;
						pCurve->tieTo = PITCH_LFO;
						pCurve->curveCount = 2;
						pCurve->from_Value[0] = 0;
						pCurve->to_Scalar[0] = 0;
						pCurve->from_Value[1] = 127;
						pCurve->to_Scalar[1] = 256;
						pCurve->from_Value[2] = 127;
						pCurve->to_Scalar[2] = 256;

						// Create a default pitch LFO unit to tie the MOD wheel to.
						pLFO = &theI->LFORecords[lfoCount];
						pLFO->where_to_feed = PITCH_LFO;
						pLFO->period = 180000;
						pLFO->waveShape = SINE_WAVE;
						pLFO->DC_feed = 0;
						pLFO->level = 64;
						pLFO->currentWaveValue = 0;
						pLFO->currentTime = 0;
						pLFO->LFOcurrentTime = 0;
						pLFO->a.ADSRLevel[0] = 65536;
						pLFO->a.ADSRTime[0] = 0;
						pLFO->a.ADSRFlags[0] = ADSR_TERMINATE;
						pLFO->a.currentTime = 0;
						pLFO->a.currentPosition = 0;
						pLFO->a.currentLevel = 0;
						pLFO->a.previousTarget = 0;
						pLFO->a.mode = 0;
						pLFO->a.sustainingDecayLevel = XFIXED_1;
						lfoCount++;
					}

					theI->LFORecordCount = (unsigned char)lfoCount;
bailoninstrument:
					;
				}
			}
		}
	}
}

/******************************************************************************
**
**	PV_CreateInstrumentFromResource
**
**	Create instrument from a raw 'snd' resource ID
**
**	????.??.?? ???	Function created
**	2000.05.10 AER	Rewrote sample cache code to work with improved cache code
**
******************************************************************************/
static GM_Instrument * PV_CreateInstrumentFromResource(GM_Instrument *theMaster,
													   XSampleID theID,
													   XBankToken bankToken,
													   OPErr *pErr)
{
	GM_Instrument *			theI;
	XPTR					theSound;
	GM_SampleCacheEntry *	sndInfo;
	GM_Mixer *				pMixer;

	theI = NULL;
	pMixer = MusicGlobals;

	//	First, if there is no entry in the cache for this ID, create it.
	//	Next, increment refcount and grab it's pointer.
	if (GMCache_IsIDInCache(pMixer, theID, bankToken) != TRUE)
	{
		sndInfo = GMCache_BuildSampleCacheEntry(pMixer,
												theID,
												bankToken,
												NULL,
												pErr);
	}
	else
	{
		sndInfo = GMCache_GetCachePtrFromID(pMixer, theID, bankToken, pErr);
		if (*pErr != NO_ERR) return theI;
		*pErr = GMCache_IncrCacheEntryRef(pMixer, sndInfo);
	}
	if (*pErr != NO_ERR) return theI;
	theSound = GMCache_GetSamplePtr(sndInfo, pErr);

	if (theSound)
	{
		theI = (GM_Instrument *)XNewPtr((long)sizeof(GM_Instrument));
		if (theI)
		{
			theI->u.w.theWaveform = (SBYTE *)theSound;

			if (theMaster)
			{
				theI->disableSndLooping = theMaster->disableSndLooping;
				theI->playAtSampledFreq = theMaster->playAtSampledFreq;
				theI->doKeymapSplit = FALSE;
				theI->notPolyphonic = theMaster->notPolyphonic;
#if REVERB_USED != REVERB_DISABLED
				theI->avoidReverb = theMaster->avoidReverb;
#endif
				theI->useSampleRate = theMaster->useSampleRate;
				theI->sampleAndHold = theMaster->sampleAndHold;
			}
			else
			{
				theI->disableSndLooping = FALSE;
				theI->playAtSampledFreq = FALSE;
				theI->doKeymapSplit = FALSE;
				theI->notPolyphonic = FALSE;
#if REVERB_USED != REVERB_DISABLED
				theI->avoidReverb = FALSE;
#endif
				theI->useSampleRate = FALSE;
				theI->sampleAndHold = FALSE;
			}
			theI->u.w.bitSize = sndInfo->bitSize;
			theI->u.w.channels = sndInfo->channels;
			theI->u.w.waveformID = sndInfo->theID;
			theI->u.w.waveSize = sndInfo->waveSize;
			theI->u.w.waveFrames = sndInfo->waveFrames;
			theI->u.w.startLoop = sndInfo->loopStart;
			theI->u.w.endLoop = sndInfo->loopEnd;
			theI->u.w.baseMidiPitch = (unsigned char)sndInfo->baseKey;
			theI->u.w.sampledRate = sndInfo->rate;
		}
		else
		{
			if (pErr)
			{
				*pErr = MEMORY_ERR;
			}
		}
	}
	return theI;
}

/******************************************************************************
**
**	PV_GetInstrument
**
**	This will create an internal instrument from and external instrument.
**	If theX is non-null then it will use that data to create the GM_Instrument
**	Instruments 0 to MAX_INSTRUMENTS * MAX_BANKS are the standard MIDI
**		instrument placements.
**
**	????.??.?? ???	Function created
**	2000.05.10 AER	Rewrote sample cache code to work with improved cache code
**
******************************************************************************/
GM_Instrument * PV_GetInstrument(GM_Mixer *pMixer, GM_Song *pSong, 
								 XLongResourceID theID,
								 XBankToken bankToken,
								 void *theExternalX,
								 long patchSize,
								 OPErr *pErr)
{
	GM_Instrument *			theI;
	GM_Instrument *			theS;
	InstrumentResource *	theX;
	long					size;
	short int 				count;
	XPTR					theSound;
	KeySplit				theXSplit;
	GM_SampleCacheEntry *	sndInfo;
	LOOPCOUNT				i;
	XSampleID				theSampleID;

	theI = NULL;
	theX = (InstrumentResource *)theExternalX;

	if (theExternalX == NULL)
	{
		theX = (InstrumentResource *)XGetAndDetachResource(ID_INST, theID, &patchSize);
	}
	if (theX)
	{
		if (XGetShort(&theX->keySplitCount) < 2)	// if its 1, then it has no splits
		{
			// get the sample ID from a short, values can be negative
			// then allow conversion to take place.
			
			// NOTE:	I know this is awfull, but if you change it things will break. The
			//			internal ID values are all 32 bit signed, and some of the external
			//			file structures are 16 bit signed.
			theSampleID = (long) ((short) XGetShort(&theX->sndResourceID));

			//	First, if there is no entry in the cache for this ID, create it.
			//	Next, increment refcount and grab it's pointer.
			if (GMCache_IsIDInCache(pMixer, theSampleID, bankToken) != TRUE)
			{
				sndInfo = GMCache_BuildSampleCacheEntry(pMixer,
														theSampleID,
														bankToken,
														NULL,
														pErr);
			}
			else
			{
				sndInfo = GMCache_GetCachePtrFromID(pMixer,
													theSampleID,
													bankToken,
													pErr);
				if (*pErr != NO_ERR) return theI;
				*pErr = GMCache_IncrCacheEntryRef(pMixer, sndInfo);
			}
			if (*pErr != NO_ERR) return theI;
			theSound = GMCache_GetSamplePtr(sndInfo, pErr);
			
			if (theSound)
			{
				theI = (GM_Instrument *)XNewPtr((long)sizeof(GM_Instrument));
				if (theI)
				{
					theI->u.w.theWaveform = (SBYTE *)theSound;

					theI->disableSndLooping = TEST_FLAG_VALUE(theX->flags1, ZBF_disableSndLooping);
					theI->playAtSampledFreq = TEST_FLAG_VALUE(theX->flags2, ZBF_playAtSampledFreq);
					theI->doKeymapSplit = FALSE;
					theI->notPolyphonic = TEST_FLAG_VALUE(theX->flags2, ZBF_notPolyphonic);
#if REVERB_USED != REVERB_DISABLED
					theI->avoidReverb = TEST_FLAG_VALUE(theX->flags1, ZBF_avoidReverb);
#endif
					theI->useSampleRate = TEST_FLAG_VALUE(theX->flags1, ZBF_useSampleRate);
					theI->sampleAndHold = TEST_FLAG_VALUE(theX->flags1, ZBF_sampleAndHold);
					theI->useSoundModifierAsRootKey = TEST_FLAG_VALUE(theX->flags2, ZBF_useSoundModifierAsRootKey);
					PV_GetEnvelopeData(theX, theI, patchSize);		// get envelope

					theI->u.w.bitSize = sndInfo->bitSize;
					theI->u.w.channels = sndInfo->channels;
					theI->u.w.waveformID = XGetShort(&theX->sndResourceID);
					theI->u.w.waveSize = sndInfo->waveSize;
					theI->u.w.waveFrames = sndInfo->waveFrames;
					theI->u.w.startLoop = sndInfo->loopStart;
					theI->u.w.endLoop = sndInfo->loopEnd;

					theI->masterRootKey = XGetShort(&theX->midiRootKey);
					theI->panPlacement = theX->panPlacement;
					theI->u.w.baseMidiPitch = (unsigned char)sndInfo->baseKey;
					theI->u.w.sampledRate = sndInfo->rate;

					// NOTE!! If ZBF_useSoundModifierAsRootKey is TRUE, then we are using
					// the Sound Modifier data blocks as a root key replacement for samples in
					// the particular split
					theI->miscParameter1 = XGetShort(&theX->miscParameter1);
					theI->miscParameter2 = XGetShort(&theX->miscParameter2);
					if (theI->useSoundModifierAsRootKey)
					{
						theI->enableSoundModifier = FALSE;
						if (theI->miscParameter2 == 0)		// I hate this hack, but I fucked up and didn't set the default to 100
						{
							theI->miscParameter2 = 100;
						}
					}
					else
					{
						theI->enableSoundModifier = TEST_FLAG_VALUE(theX->flags2, ZBF_enableSoundModifier);
						theI->smodResourceID = theX->smodResourceID;

#if BAE_NOT_USED
						// Process sample in place
						if ( (theI->enableSoundModifier) && (theI->u.w.bitSize == 8) && (theI->u.w.channels == 1) )
						{
					#if DEBUG_DISPLAY_PATCHES
							BAE_PRINTF("---->Processing instrument %ld with SMOD %ld\n", (long)theID, (long)theI->smodResourceID);
					#endif
							PV_ProcessSampleWithSMOD(theI->u.w.theWaveform,
													theI->u.w.waveSize,
													theI->u.w.waveformID,
													theI->smodResourceID,
													theI->miscParameter1,
													theI->miscParameter2);
						}
#endif
					}
				}
				else
				{
					*pErr = MEMORY_ERR;
				}
			}
		}
		else
		{	// Keysplits
		#if DEBUG_DISPLAY_PATCHES
			BAE_PRINTF("----->Processing %ld keysplits\n", (long)XGetShort(&theX->keySplitCount));
		#endif
			size = XGetShort(&theX->keySplitCount) * sizeof(GM_KeymapSplit);
			size += sizeof(GM_KeymapSplitInfo);

			theI = (GM_Instrument *)XNewPtr(size + sizeof(GM_Instrument));
			if (theI)
			{
				theI->disableSndLooping = TEST_FLAG_VALUE(theX->flags1, ZBF_disableSndLooping);
				theI->doKeymapSplit = TRUE;
				theI->notPolyphonic = TEST_FLAG_VALUE(theX->flags2, ZBF_notPolyphonic);
#if REVERB_USED != REVERB_DISABLED
				theI->avoidReverb = TEST_FLAG_VALUE(theX->flags1, ZBF_avoidReverb);
#endif
				theI->useSampleRate = TEST_FLAG_VALUE(theX->flags1, ZBF_useSampleRate);
				theI->sampleAndHold = TEST_FLAG_VALUE(theX->flags1, ZBF_sampleAndHold);
				theI->playAtSampledFreq = TEST_FLAG_VALUE(theX->flags2, ZBF_playAtSampledFreq);
				theI->useSoundModifierAsRootKey = TEST_FLAG_VALUE(theX->flags2, ZBF_useSoundModifierAsRootKey);
				PV_GetEnvelopeData(theX, theI, patchSize);		// get envelope

				theI->u.k.KeymapSplitCount = XGetShort(&theX->keySplitCount);
				theI->u.k.defaultInstrumentID = (XShortResourceID)XGetShort(&theX->sndResourceID);

				theI->masterRootKey = XGetShort(&theX->midiRootKey);
				theI->panPlacement = theX->panPlacement;

				// NOTE!! If ZBF_useSoundModifierAsRootKey is TRUE, then we are using
				// the Sound Modifier data blocks as a root key replacement for samples in
				// the particular split
				theI->miscParameter1 = XGetShort(&theX->miscParameter1);
				theI->miscParameter2 = XGetShort(&theX->miscParameter2);
				if (theI->useSoundModifierAsRootKey)
				{
					theI->enableSoundModifier = FALSE;
					if (theI->miscParameter2 == 0)		// I hate this hack, but I fucked up and didn't set the default to 100
					{
						theI->miscParameter2 = 100;
					}
				}
				else
				{
					theI->enableSoundModifier = TEST_FLAG_VALUE(theX->flags2, ZBF_enableSoundModifier);
					theI->smodResourceID = theX->smodResourceID;
				}

				for (count = 0; count < theI->u.k.KeymapSplitCount; count++)
				{
					XGetKeySplitFromPtr(theX, count, &theXSplit);
					theI->u.k.keySplits[count].lowMidi = theXSplit.lowMidi;
					theI->u.k.keySplits[count].highMidi = theXSplit.highMidi;
					theI->u.k.keySplits[count].miscParameter1 = theXSplit.miscParameter1;
					if (theI->useSoundModifierAsRootKey && (theXSplit.miscParameter2 == 0))		// I hate this hack, but I fucked up and didn't set the default to 100
					{
						theXSplit.miscParameter2 = 100;
					}
					theI->u.k.keySplits[count].miscParameter2 = theXSplit.miscParameter2;

					if (GM_IsInstrumentRangeUsed(pSong, theID, (INT16)theXSplit.lowMidi, (INT16)theXSplit.highMidi))
					{
					#if DEBUG_DISPLAY_PATCHES
						BAE_PRINTF("------->Keysplit %ld low %ld high %ld\n", (long)count, 
																	(long)theXSplit.lowMidi, 
																	(long)theXSplit.highMidi);
					#endif
						theS =  PV_CreateInstrumentFromResource(theI,
															    (XSampleID) theXSplit.sndResourceID,
																bankToken,
																pErr);
						theI->u.k.keySplits[count].pSplitInstrument = theS;
						if (theS)
						{
							theS->useSoundModifierAsRootKey = theI->useSoundModifierAsRootKey;
							theS->miscParameter1 = theXSplit.miscParameter1;
							if (theS->useSoundModifierAsRootKey && (theXSplit.miscParameter2 == 0))		// I hate this hack, but I fucked up and didn't set the default to 100
							{
								theXSplit.miscParameter2 = 100;
							}
							theS->miscParameter2 = theXSplit.miscParameter2;

							theS->masterRootKey = theI->masterRootKey;
							theS->panPlacement = theI->panPlacement;
#if REVERB_USED != REVERB_DISABLED
							theS->avoidReverb = theI->avoidReverb;
#endif
							theS->volumeADSRRecord = theI->volumeADSRRecord;
							for (i = 0; i < theI->LFORecordCount; i++)
							{
								theS->LFORecords[i] = theI->LFORecords[i];
							}
							theS->LFORecordCount = theI->LFORecordCount;
							for (i = 0; i < theI->curveRecordCount; i++)
							{
								theS->curve[i] = theI->curve[i];
							}
							theS->curveRecordCount = theI->curveRecordCount;
							theS->LPF_frequency = theI->LPF_frequency;
							theS->LPF_resonance = theI->LPF_resonance;
							theS->LPF_lowpassAmount = theI->LPF_lowpassAmount;

#if BAE_NOT_USED
							if (theS->useSoundModifierAsRootKey == FALSE)
							{
								// Process sample in place
								if ( (theS->enableSoundModifier) && (theS->u.w.bitSize == 8) && (theS->u.w.channels == 1) )
								{
								#if DEBUG_DISPLAY_PATCHES
									BAE_PRINTF("----->Processing instrument %ld with SMOD %ld\n", (long)theID, (long)theI->smodResourceID);
								#endif
									PV_ProcessSampleWithSMOD(	theS->u.w.theWaveform,
															theS->u.w.waveSize,
															theXSplit.sndResourceID,
															theS->smodResourceID,
															theS->miscParameter1,
															theS->miscParameter2);
								}
							}
#endif
						}
					}
				}
			#if DEBUG_DISPLAY_PATCHES
				BAE_PRINTF("-------->INST info: masterRootKey %ld\n", (long)theI->masterRootKey);
			#endif
			}
			else
			{
				*pErr = MEMORY_ERR;
			#if DEBUG_DISPLAY_PATCHES
				BAE_PRINTF("Can't allocate instrument\n");
			#endif
			}
		}
		if (theExternalX == NULL)
		{
			XDisposePtr((XPTR)theX);
		}
	}
	else
	{
		*pErr = BAD_INSTRUMENT;
	}
	return theI;
}

XBOOL GM_AnyStereoInstrumentsLoaded(GM_Song *pSong)
{
	register GM_Instrument	*theI;
	register short int		instrument;
	XBOOL				stereoLoaded;

	stereoLoaded = FALSE;
	if (pSong)
	{
		for (instrument = 0; instrument < (MAX_INSTRUMENTS*MAX_BANKS); instrument++)
		{
			theI = pSong->instrumentData[instrument];
			if (theI)
			{
				if (theI->doKeymapSplit == FALSE)	// only look at wave data
				{
					if (theI->u.w.channels > 1)
					{
						stereoLoaded = TRUE;
						break;
					}
				}
			}
		}
	}
	return stereoLoaded;
}

XBOOL GM_IsInstrumentLoaded(GM_Song *pSong, XLongResourceID instrument)
{
	if ( pSong && (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)) )
	{
		if (pSong->instrumentData[instrument])
		{
			return TRUE;
		}
	}
	return FALSE;
}

// Load an instrument based from a memory definition, and assign it to the instrument ID passed.
// This will unload the instrument if its already loaded.
OPErr GM_LoadInstrumentFromExternalData(GM_Song *pSong,
										XLongResourceID instrument,
										XBankToken bankToken,
										void *theX,
										UINT32 theXPatchSize)
{
	register GM_Instrument	*theI;
	OPErr					theErr;

	theErr = MEMORY_ERR;
	if ( (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)) )
	{
		if (pSong)
		{
			theErr = NO_ERR;

			theI = pSong->instrumentData[instrument];
			if (theI)
			{
				GM_UnloadInstrument(pSong, instrument);
			}
			theI = PV_GetInstrument(GM_GetCurrentMixer(), pSong, instrument,
									bankToken,
									theX,
									theXPatchSize,
									&theErr);

			if (theI)
			{
				theI->usageReferenceCount++;		// increment reference count
				pSong->instrumentData[instrument] = theI;
			}
		}
		else
		{
			theErr = NOT_SETUP;
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}


// Given an instrument number from 0 to MAX_INSTRUMENTS*MAX_BANKS, this will load that instrument into the musicvars globals, including
// splits. The instrument is assumed to be the real instrument. No remaps or aliases are taken care of here
OPErr GM_LoadInstrument(GM_Song *pSong,
						XLongResourceID instrument,
						XBankToken bankToken)
{
	register GM_Instrument	*theI;
	OPErr					theErr;

	theErr = MEMORY_ERR;
	if ( (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)) )
	{
		if (pSong)
		{
			theErr = NO_ERR;

			theI = pSong->instrumentData[instrument];
			// use cached instrument, if its not there, then load it
			if (theI == NULL)
			{
				theI = PV_GetInstrument(GM_GetCurrentMixer(), pSong, instrument, bankToken, NULL, 0, &theErr);
			}
			if (theErr == BAD_SAMPLE)
			{
				// everythings ok, we've allocated an instrument, but we'll just ignore the bad sample
				theErr = NO_ERR;
			}
			if (theI)
			{
				theI->usageReferenceCount++;		// increment reference count
				pSong->instrumentData[instrument] = theI;
			}
			else
			{
				theErr = BAD_INSTRUMENT;
			}
		}
		else
		{
			theErr = NOT_SETUP;
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}

// unload an instrument and remove all of its memory and optionally the samples
OPErr PV_UnloadInstrumentData(GM_Instrument *theI, GM_Mixer *pMixer, XBOOL freeSamples)
{
	register GM_KeymapSplit	*k;
	OPErr					theErr;
	register short int		splitCount;
	GM_SampleCacheEntry *	pCache;

	theErr = NO_ERR;
	if (theI)
	{
		if (theI->processingSlice == FALSE)
		{
			theI->usageReferenceCount--;		// decrement reference count
			if (theI->usageReferenceCount == 0)
			{
				if (theI->doKeymapSplit)
				{
					k = theI->u.k.keySplits;
					for (splitCount = 0; splitCount < theI->u.k.KeymapSplitCount; splitCount++)
					{
						if (k->pSplitInstrument)
						{
							if (freeSamples)
							{
								if (k->pSplitInstrument->u.w.theWaveform)
								{
									pCache = GMCache_GetCachePtrFromPtr(pMixer,
																		k->pSplitInstrument->u.w.theWaveform,
																		&theErr);
									if (pCache && theErr == NO_ERR)
									{
										GMCache_DecrCacheEntryRef(pMixer, pCache);
									}
								}
							}
							XDisposePtr(k->pSplitInstrument);
						}
						k++;
					}
				}
				else
				{
					if (freeSamples)
					{
						if (theI->u.w.theWaveform)
						{
							pCache = GMCache_GetCachePtrFromPtr(pMixer,
																theI->u.w.theWaveform,
																&theErr);
							if (pCache && theErr == NO_ERR)
							{
								GMCache_DecrCacheEntryRef(pMixer, pCache);
							}
						}
					}
				}
				XDisposePtr((void *)theI);
			}
			else
			{ 	// duplicate reference
				theErr = NO_ERR;
			}
		}
		else
		{
			theErr = STILL_PLAYING;
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}

// Given a valid GM_Song pointer and an instrument this will release
// an instrument. The freeSamples flag will release samples
static OPErr PV_UnloadSongInstrument(GM_Song *pSong, XLongResourceID instrument,
											XBOOL freeSamples)
{
	register GM_Instrument		*theI;
	register OPErr				theErr;

	theErr = BAD_INSTRUMENT;
	if ( (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)) )
	{
		if (pSong)
		{
			// make sure this instrumen isn't playing anywhere
			GM_KillSongInstrument(pSong, instrument);

			theErr = NO_ERR;
			theI = pSong->instrumentData[instrument];
			if (theI)
			{
				if (theI->usageReferenceCount == 1)
				{
					pSong->instrumentData[instrument] = NULL;
				}
				theErr = PV_UnloadInstrumentData(theI, pSong->pMixer, freeSamples);
			}
		}
		else
		{
			theErr = NOT_SETUP;
		}
	}
	else
	{
		theErr = PARAM_ERR;
	}
	return theErr;
}

// Given an instrument number from 0 to MAX_INSTRUMENTS*MAX_BANKS, this will unload that instrument including
// splits. The instrument is assumed to be the real instrument. No remaps or aliases are taken care of here
// Can return STILL_PLAYING if instrument fails to unload
OPErr GM_UnloadInstrument(GM_Song *pSong, XLongResourceID instrument)
{
	return PV_UnloadSongInstrument(pSong, instrument, TRUE);
}

// Scan the midi file and determine which instrument that need to be loaded and load them.
OPErr GM_LoadSongInstruments(GM_Song *theSong,
							 XShortResourceID *pArray,
							 XBankToken bankToken,
							 XBOOL loadInstruments)
{
	register long		count, instCount;
	XLongResourceID		realInstrument, fallbackInstrument;
	XBOOL				loopSongSave;
	OPErr				theErr;
	XBOOL				emptyStart;
	SBYTE				remapUsedSaved[MAX_INSTRUMENTS];
	SBYTE				remapUsed[MAX_INSTRUMENTS];

	if (theSong->seqType != SEQ_MIDI)
	{
		return NOT_SETUP;
	}
	// Set the sequencer to mark instruments only
	theErr = NO_ERR;
	theSong->pUsedPatchList = (SBYTE *)XNewPtr((MAX_INSTRUMENTS*MAX_BANKS*128L) / 8);
	if (theSong->pUsedPatchList)
	{
		GM_SetupSongRemaps(theSong, TRUE);

		if (pArray)
		{
			for (count = 0; count < MAX_INSTRUMENTS*MAX_BANKS; count++)
			{
				pArray[count] = (XShortResourceID)-1;
			}
		}
		theSong->firstNoteOnChannel = -1;
		for (count = 0; count < MAX_CHANNELS; count++)
		{
			theSong->firstChannelBank[count] = 0;
			theSong->firstChannelProgram[count] = -1;
		}
		theErr = PV_ConfigureMusic(theSong);
		if (theErr == NO_ERR)
		{
			if (theSong->defaultPercusionProgram == -1)
			{
				theSong->channelBank[PERCUSSION_CHANNEL] = 0;
				theSong->firstChannelBank[PERCUSSION_CHANNEL] = 0;
			}
			else
			{
				if (theSong->defaultPercusionProgram)
				{
					theSong->firstChannelProgram[PERCUSSION_CHANNEL] = theSong->defaultPercusionProgram;
					GM_SetUsedInstrument(theSong, (XLongResourceID)theSong->defaultPercusionProgram, 60, TRUE);
				}
			}

			theSong->AnalyzeMode = SCAN_SAVE_PATCHES;
			theSong->SomeTrackIsAlive = TRUE;

			loopSongSave = GM_GetSongLoopFlag(theSong);
			GM_SetSongLoopFlag(theSong, FALSE);
			while (theSong->SomeTrackIsAlive)
			{
				theErr = PV_ProcessMidiSequencerSlice(NULL, theSong);
				if (theErr)
				{
					break;
				}
			}
			theSong->AnalyzeMode = SCAN_NORMAL;
			GM_SetSongLoopFlag(theSong, loopSongSave);
			GM_ResetTempoToDefault(theSong);

			if (theErr == NO_ERR)
			{
				// are we trying to load any instruments? 
				// This is for the case were there are no program changes. We must do something
				emptyStart = FALSE;
				for (count = 0; count < MAX_CHANNELS; count++)
				{
					if (count != PERCUSSION_CHANNEL)	// only look at non percussion channels
					{
						if (theSong->firstChannelProgram[count] == -1)
						{
							emptyStart = TRUE;
							theSong->firstChannelProgram[count] = DEFAULT_PATCH;
							theSong->channelProgram[count] = DEFAULT_PATCH;
						}
					}
				}
				if (emptyStart != FALSE)
				{	// there have been no program changes. So set up just the piano in all channels					
					GM_SetUsedInstrument(theSong, DEFAULT_PATCH, -1L, TRUE);		// load the entire piano
				}
			#if DEBUG_DISPLAY_PATCHES
				BAE_PRINTF("Loading instruments:\n");
			#endif
				instCount = 0;
				for (count = 0; count < MAX_INSTRUMENTS*MAX_BANKS; count++)
				{
					// instrument needs to be loaded
					if (GM_IsInstrumentUsed(theSong, (XLongResourceID)count, -1))
					{
						if (GM_GetSongInstrumentRemap(theSong, (XLongResourceID)count, &realInstrument) != NO_ERR)
						{
							realInstrument = (XLongResourceID)count;	// failed, no alias, so try to load instrument passed
						}

						if (pArray)
						{
							pArray[instCount++] = (XShortResourceID)realInstrument;
						}
	
						if (loadInstruments)
						{
							if (realInstrument != count)
							{
								GM_GetInstrumentUsedRange(theSong, realInstrument, remapUsedSaved);		// save
								GM_GetInstrumentUsedRange(theSong, (XLongResourceID)count, remapUsed);
								GM_SetInstrumentUsedRange(theSong, realInstrument, remapUsed);
							}

							theErr = GM_LoadInstrument(theSong, realInstrument, bankToken);

							// Fallback Code
							if (theErr == BAD_INSTRUMENT)
							{	// if the instrument is some other bank, then go back to the standard GM bank before failing
								if (realInstrument > MAX_INSTRUMENTS)
								{
								#if DEBUG_DISPLAY_PATCHES
									BAE_PRINTF("Failed loading extra bank instrument %ld, falling back to GM.\n", realInstrument);
								#endif
									fallbackInstrument = (realInstrument % MAX_INSTRUMENTS);
									fallbackInstrument += ((realInstrument / MAX_INSTRUMENTS) & 1) * MAX_INSTRUMENTS;
									realInstrument = fallbackInstrument;
								
								#if DEBUG_DISPLAY_PATCHES
									BAE_PRINTF("Trying to load instrument %ld\n", realInstrument);
								#endif
									// 2000.05.19 AER	If the instrument had
									//					already been loaded, we
									//					leaked the inst because
									//					the refcount was for the
									//					real, not fallback inst
									if (TRUE != GM_IsInstrumentLoaded(theSong, realInstrument))
									{
										theErr = GM_LoadInstrument(theSong,
																   realInstrument,
																   bankToken);
										if (theSong->ignoreBadInstruments)
										{
											if (theErr == BAD_INSTRUMENT)
											{
												theErr = NO_ERR;
											}
										}
									}
									else
									{
										theErr = NO_ERR;
									}
								}
								else
								{	// we are in GM, so check our ignore flag
									if (theSong->ignoreBadInstruments)
									{
										theErr = NO_ERR;
									}
								}
							}
							if (realInstrument != count)
							{
								GM_SetInstrumentUsedRange(theSong, realInstrument, remapUsedSaved);		// save
							}
							if (theErr)
							{
							#if DEBUG_DISPLAY_PATCHES
								BAE_PRINTF("Failed to load instrument %ld (%ld)\n", (long)realInstrument, (long)theErr);
							#endif
								break;
							}
							// we are in GM, so check our ignore flag
							if (theSong->ignoreBadInstruments)
							{
								if (theErr == BAD_INSTRUMENT)
								{
									theErr = NO_ERR;
								}
							}
						}
					}
				}
			}
		}	
	
		if (theErr != NO_ERR)
		{
			GM_UnloadSongInstruments(theSong);		// ignore error
		}
		XDisposePtr(theSong->pUsedPatchList);
		theSong->pUsedPatchList = NULL;
	}
	else
	{
		theErr = MEMORY_ERR;
	}
	return theErr;
}

// unload all instruments associated to this song. Follow remaps.
OPErr GM_UnloadSongInstruments(GM_Song *pSong)
{
	short int		count;
	OPErr			err, lastErr;
	XLongResourceID	realInstrument;

	err = NO_ERR;
	if (pSong)
	{
		for (count = 0; count < (MAX_INSTRUMENTS*MAX_BANKS); count++)
		{
			if (GM_GetSongInstrumentRemap(pSong, (XLongResourceID)count, &realInstrument) != NO_ERR)
			{
				realInstrument = (XLongResourceID)count;	// failed, no alias, so try to load instrument passed
			}
			err = GM_UnloadInstrument(pSong, realInstrument);
			if (err)
			{
				lastErr = err;
			}
		}
	}
	return err;
}

void GM_SetUsedInstrumentRange(GM_Song *pSong, XLongResourceID thePatch, int start, int end, XBOOL used)
{
	int count, tmp;

	if (pSong)
	{
		if (start < 0)
		{
			start = 0;
		}
		if (end > 127)
		{
			end = 127;
		}
		if (start > end)
		{
			tmp = start;
			start = end;
			end = tmp;
		}
	
		for (count = start; count < end; count++)
		{
			GM_SetUsedInstrument(pSong, thePatch, count, used);
		}
	}
}

// Set the patch & key used bit. Pass -1 in theKey to set all the keys in that patch
void GM_SetUsedInstrument(GM_Song *pSong, XLongResourceID thePatch, INT16 theKey, XBOOL used)
{
	unsigned long	bit, count;

	if (pSong && pSong->pUsedPatchList)
	{
	#if DEBUG_DISPLAY_PATCHES
	//	BAE_PRINTF("Set used INST %ld\n", thePatch);
	#endif
		if (theKey != -1)
		{
			bit = ((long)thePatch * 128L) + (long)theKey;
//			if (bit < (MAX_INSTRUMENTS*MAX_BANKS*128L))
			{
				if (used)
				{
					XSetBit((void *)pSong->pUsedPatchList, bit);
				}
				else
				{
					XClearBit((void *)pSong->pUsedPatchList, bit);
				}
			}
		}
		else
		{
			for (count = 0; count < MAX_INSTRUMENTS; count++)
			{
				bit = ((long)thePatch * 128L) + count;
//				if (bit < (MAX_INSTRUMENTS*MAX_BANKS*128L))
				{
					if (used)
					{
						XSetBit((void *)pSong->pUsedPatchList, bit);
					}
					else
					{
						XClearBit((void *)pSong->pUsedPatchList, bit);
					}
				}
			}
		}
	}
}


XBOOL GM_IsInstrumentUsed(GM_Song *pSong, XLongResourceID thePatch, INT16 theKey)
{
#if 1
// faster code
	if (pSong && pSong->pUsedPatchList)
	{
		register unsigned long   bit, count,byteIndex,bitIndex;
		register unsigned char  *patchlist = (unsigned char*)pSong->pUsedPatchList;

		bit = (long)thePatch << 7;
		if (theKey != -1)
		{
			bit += theKey;
			if (bit < (MAX_INSTRUMENTS*MAX_BANKS*128L))
			{
				byteIndex = bit >> 3;
				bitIndex = bit & 7;
				return ( (*(patchlist + byteIndex) & (1<<(7-bitIndex))) != 0);
			}
		}
		else
		{
			for (count = 0; count < MAX_INSTRUMENTS; count++, bit++ )
			{
				if (bit < (MAX_INSTRUMENTS*MAX_BANKS*128L))
				{
					byteIndex = bit >> 3;
					bitIndex = bit & 7;
					if ( *(patchlist + byteIndex) & (1<<(7-bitIndex)) )
					{
						return TRUE;
					}
				}
			}
		}
		return FALSE;
	}
	return TRUE;
#else
	register unsigned long	bit, count;
	register XBOOL		used;

	used = FALSE;
	if (pSong && pSong->pUsedPatchList)
	{
		if (theKey != -1)
		{
			bit = ((long)thePatch * 128L) + (long)theKey;
//			if (bit < (MAX_INSTRUMENTS*MAX_BANKS*128L))
			{
				used = XTestBit((void *)pSong->pUsedPatchList, bit);
			}
		}
		else
		{
			for (count = 0; count < MAX_INSTRUMENTS; count++)
			{
				bit = ((long)thePatch * 128L) + count;
//				if (bit < (MAX_INSTRUMENTS*MAX_BANKS*128L))
				{
					used = XTestBit((void *)pSong->pUsedPatchList, bit);
					if (used)
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		used = TRUE;
	}
	return used;
#endif
}

void GM_GetInstrumentUsedRange(GM_Song *pSong, XLongResourceID thePatch, SBYTE *pUsedArray)
{
	register unsigned long bit, count;

	if (pSong)
	{
		if (pSong->pUsedPatchList)
		{
			for (count = 0; count < MAX_INSTRUMENTS; count++)
			{
				bit = ((long)thePatch * 128L) + count;
				pUsedArray[count] = XTestBit((void *)pSong->pUsedPatchList, bit);
			}
		}
	}
}

void GM_SetInstrumentUsedRange(GM_Song *pSong, XLongResourceID thePatch, SBYTE *pUsedArray)
{
	register INT16		count;

	if (pSong)
	{
		if (pSong->pUsedPatchList)
		{
			for (count = 0; count < MAX_INSTRUMENTS; count++)
			{
				GM_SetUsedInstrument(pSong, thePatch, count, pUsedArray[count]);
			}
		}
	}
}


XBOOL GM_IsInstrumentRangeUsed(GM_Song *pSong, XLongResourceID thePatch, INT16 theLowKey, INT16 theHighKey)
{
#if 0
	// enable this path to always load samples from a particular instrument
	return TRUE;
#else
	register XBOOL	used;
	register INT16		count;

	used = FALSE;
	if (pSong)
	{
		for (count = theLowKey; count <= theHighKey; count++)
		{
			used = GM_IsInstrumentUsed(pSong, thePatch, count);
			if (used)
			{
				break;
			}
		}
	}

#if DEBUG_DISPLAY_PATCHES
	BAE_PRINTF("---->Testing INST %ld - key range (%ld to %ld) = %s\n", (long)thePatch,
																			(long)theLowKey,
																			(long)theHighKey,
																			(used) ? "VALID" : "FAILED");
#endif
	return used;
#endif
}


// EOF of GenPatch.c
