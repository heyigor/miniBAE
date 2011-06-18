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
** "GenSnd.h"
**
**  Generalized Music Synthesis package. Part of SoundMusicSys.
**
**  © Copyright 1993-2001 Beatnik, Inc, All Rights Reserved.
**  Written by Jim Nitchals and Steve Hales
**
**  Beatnik products contain certain trade secrets and confidential and
**  proprietary information of Beatnik.  Use, reproduction, disclosure
**  and distribution by any means are prohibited, except pursuant to
**  a written license from Beatnik. Use of copyright notice is
**  precautionary and does not imply publication or disclosure.
**
**  Restricted Rights Legend:
**  Use, duplication, or disclosure by the Government is subject to
**  restrictions as set forth in subparagraph (c)(1)(ii) of The
**  Rights in Technical Data and Computer Software clause in DFARS
**  252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
**  Computer Software--Restricted Rights at 48 CFR 52.227-19, as
**  applicable.
**
**  Confidential-- Internal use only
**
** Overview
**  The purpose of this layer of code is to remove Macintosh Specific code.
**  No file, or memory access. All functions are passed pointers to data
**  that needs to be passed into the mixer, and MIDI sequencer
**
** Modification History
**
**  4/6/93      Created
**  4/12/93     First draft ready
**  4/14/93     Added Waveform structure
**  7/7/95      Added Instrument API
**  11/7/95     Major changes, revised just about everything.
**  11/11/95    Added external queued midi links
**  11/20/95    Removed the BF_ flags, now you must walk through the union structure
**              Remove bit fields. BIT FIELDS DON'T WORK WITH MPW!!!!
**  12/5/95     Added avoidReverb into instrument field; removed drop sample case
**  12/6/95     Move REVERB_TYPE from GENPRIV.H
**              Added GM_SetReverbType; removed extern references
**              Added ReverbType to GM_Song structure
**              Removed defaultPlaybackRate & defaultInterpolationMode from the GM_Song
**              structure
**  12/7/95     Moved DEFAULT_REVERB_TYPE from GenSnd.c
**              Added GM_GetReverbType
**  1/4/96      Added GM_ChangeSampleReverb for sound effects
**  1/7/96      Changed GM_BeginDoubleBuffer to use a 32 bit value for volume
**              Added GM_SetEffectsVolume & GM_GetEffectsVolume
**  1/13/96     Added extendedFormat bit to internal instrument format
**  1/18/96     Spruced up for C++ extra error checking
**  1/19/96     Changed GM_BeginSample to support bitsize and channels
**  1/29/96     Changed WaveformInfo to support XFIXED for sample rate
**              Added useSampleRate factor for playback of instruments & sampleAndHold bits
**  2/4/96      Added songMidiTickLength to GM_Song
**  2/5/96      Moved lots of variables from the GM_Mixer structure into
**              the GM_Song structure.
**              Changed GM_EndSong & GM_SongTicks & GM_IsSongDone & 
**              GM_SetMasterSongTempo to pass in a GM_Song pointer
**              Added GM_GetSongTickLength
**  2/12/96     Added GM_SetSongTickPosition
**              Added songMicrosecondLength to GM_Song structure
**  2/14/96     Added GM_StartLiveSong
**  2/18/96     Added panPlacement to the GM_Instrument structure
**  2/29/96     Added trackMuted to GM_Song structure
**  3/5/96      Added MAX_SONG_VOLUME
**  4/15/96     Added support to interpret SONG resource via GM_MergeExternalSong
**  4/20/96     Added defines for max lfos, and max curves, MAX_LFOS & MAX_CURVES
**  4/21/96     Added GM_GetRealtimeAudioInformation
**  6/7/96      Increased MAX_TRACKS to 65 to include tempo track and 64 tracks
**  6/9/96      Added GM_GetUserReference & GM_SetUserReference
**  6/30/96     Changed font and retabbed
**  7/2/96      Added packing pragmas
**              Removed usage of Machine.h. Now merged into X_API.h
**  7/3/96      Added support for one level of loop save and playback
**              Required for Beatnik support
**  7/5/96      Added GM_KillAllNotes
**  7/6/96      Added GM_GetSyncTimeStamp
**  7/14/96     Fixed structure alignment issue for PowerPC
**  7/23/96     Changed QGM_ functions to use an unsigned long for time stamp
**  7/25/96     Added GM_GetSyncTimeStampQuantizedAhead
**  8/15/96     Added constant for LOW_PASS_AMOUNT & LPF_DEPTH
**  8/19/96     Added GM_SetAudioTask
**  9/10/96     Added GM_NoteOn & GM_NoteOff & GM_ProgramChange & GM_PitchBend & 
**              GM_Controller & GM_AllNotesOff for direct control to bypass the queue
**  9/17/96     Added GM_LoadSongInstrument & GM_UnloadSongInstrument
**              Added Q_GET_TICK
**  9/18/96     Changed GM_SongCallbackProcPtr to pass a GM_Song structure 
**              rather than an ID
**  9/19/96     Added GM_GetSongTempo
**  9/20/96     Added GM_SetSongTempo & GM_ResumeSong & GM_PauseSong
**  9/23/96     Added GM_MuteTrack & GM_UnmuteTrack & GM_MuteChannel & GM_UnmuteChannel
**  9/24/96     Added GM_SetSongTempInBeatsPerMinute & GM_GetSongTempoInBeatsPerMinute
**              Added GM_SoloTrack & GM_SoloChannel
**              Added GM_GetSongPitchOffset & GM_SetSongPitchOffset
**  9/25/96     Added GM_GetChannelMuteStatus & GM_GetTrackMuteStatus
**              Added controller 90 to change the global reverb type
**              Added GM_EndSongNotes
**  10/8/96     Removed pascal from GM_SongCallbackProcPtr
**  10/11/96    Added GM_BeginSampleFromInfo
**  10/13/96    Changed QGM_AllNotesOff to work with the queue and post an event
**              Added GM_ReadIntoMemoryWaveFile & GM_ReadIntoMemoryAIFFFile
**  10/18/96    Made WaveformInfo smaller
**  10/23/96    Removed reference to BYTE and changed them all to XBYTE or XSBYTE
**              Added defines for MOD_WHEEL_CONTROL = 'MODW & SAMPLE_NUMBER = 'SAMP'
**              Added more defines for instrument types
**  10/28/96    Modified QGM_NoteOn & QGM_NoteOff & QGM_ProgramChange & 
**              QGM_PitchBend & QGM_Controller & QGM_AllNotesOff to accept a GM_Song *
**  10/31/96    Changed trackMuted and channelMuted to be bit flag based
**              Added soloMuted bit array for solo control
**  10/31/96    Added GM_IsInstrumentLoaded
**  11/3/96     Added midiNote to GM_AudioInfo structure
**  11/5/96     Changed WaveformInfo to GM_Waveform
**              Added GM_ReadFileInformation and GM_FreeWaveform
**  11/6/96     Added GM_UnsoloTrack, GM_GetTrackSoloStatus
**  11/7/96     Added GM_SongTimeCallbackProcPtr & GM_SetSongTimeCallback
**  11/8/96     Added GM_GetSampleVolume & GM_GetSamplePitch & GM_GetSampleStereoPosition
**  11/9/96     Added GM_KillSongNotes
**  11/11/96    Added more error codes and removed extra ones
**  11/14/96    Added pSong reference in GM_GetRealtimeAudioInformation
**  11/19/96    Changed MAX_CHANNELS to 17, 16 for Midi, 1 for sfx
**              Added a GM_Song structure to GM_ConvertPatchBank and removed bank
**  11/21/96    Removed GM_ConvertPatchBank
**  11/26/96    Changed MAX_BANKS to 6
**              Added GM_GetControllerValue
**  12/2/96     Added MOD file code API
**  12/9/96     Added GM_LoadInstrumentFromExternal
**  12/15/96    Added controls for DEFAULT_VELOCITY_CURVE
**  12/19/96    Added Sparc pragmas
**  1/2/97      Moved USE_MOD_API and USE_STREAM_API into X_API.h
**  1/12/97     Changed maxNormalizedVoices to mixLevel
**  1/16/97     Changed GM_LFO to LFORecords
**  1/22/97     Added GM_SetSampleDoneCallback
**  1/23/97     Added M_STEREO_FILTER
**              Added GM_SetAudioOutput
**  1/24/97     Added GM_SetSongFadeRate
**              Added GM_SetModLoop & GM_GetModTempoBPM & GM_SetModTempoBPM
**              Changed disposeMidiDataWhenDone to disposeSongDataWhenDone
**              Added GM_SetAudioStreamFadeRate
**  1/28/97     Added more parmeters to GM_SetSongFadeRate to support async
**              ending of song
**  1/28/97     Eliminated terminateDecay flag. Not used anymore
**  1/30/97     Changed SYMPHONY_SIZE to MAX_VOICES
**  2/1/97      Added support for pitch offset control on a per channel basis
**              Added GM_DoesChannelAllowPitchOffset & GM_AllowChannelPitchOffset
**  2/2/97      Tightened up GM_Song and GM_Instrument a bit
**  2/13/97     Added GM_GetSystemVoices    
**  2/20/97     Added support for TYPE 7 and 8 reverb in GM_SetReverbType
**  3/12/97     Added a REVERB_NO_CHANGE reverb mode. This means don't change the
**              current mixer state
**  3/20/97     Added GM_SetSongMicrosecondPosition & GM_GetSongMicrosecondLength
**  3/27/97     Added channelChorus
**  3/27/97     Changed all 4 character constants to use the FOUR_CHAR macro
**  4/14/97     Changed KeymapSplit to GM_KeymapSplit
**  4/20/97     Added GM_SetSongMetaEventCallback
**  4/21/97     Removed enableInterpolate from GM_Instrument structure
**  5/1/97      Added startOffsetFrame to GM_BeginSampleFromInfo
**  5/19/97     Added GM_ReadAndDecodeFileStream
**  6/10/97     Added GM_SetModReverbStatus to allow for verb enabled Mod files
**  6/16/97     Added GM_AudioStreamSetVolumeAll
**  6/25/97     Added GM_SetSampleLoopPoints
**  7/8/97      Added or changed GM_GetSongLoopFlag & GM_SetSongLoopMax & 
**              GM_SetSongLoopFlag & GM_GetSongLoopMax
**  7/15/97     Added GM_GetAudioOutput & GM_GetAudioTask & GM_GetAudioBufferOutputSize
**  7/17/97     Aligned GM_Song and GM_Instrument structures to 8 bytes
**  7/21/97     Put mins and maxs on GM_SetSongTempInBeatsPerMinute. 0 and 500
**              Changed GM_SetSongTempInBeatsPerMinute & GM_GetSongTempoInBeatsPerMinute
**              GM_SetSongTempo & GM_GetSongTempo & GM_SetMasterSongTempo to all be unsigned longs
**              Changed key calculations to floating point to test for persicion loss. Use
**              USE_FLOAT in GenSnd.h to control this change.
**  7/22/97     Removed MicroJif from the GM_Mixer structure. Now using constant BUFFER_SLICE_TIME
**              Added GM_GetSampleVolumeUnscaled
**  7/28/97     Moved GM_FreeWaveform so it is compiled in all platform cases.
**              Removed define wrapper from errors codes in defining OPErr.
**              Fixed a praga pack problem for Sun
**  8/6/97      Moved USE_FLOAT to X_API.h
**  8/06/97     (ddz) Added PgmEntry structure def. Added midiSize, TSNumerator, TSDenominator,
**              pgmChangeInfo, and trackcumuticks[] fields to GM_Song structure for
**              implementing the Song Program List capability
**              Changed GM_SongMetaCallbackProcPtr and added currenTrack
**  8/13/97     Renamed GM_GetSongProgramChanges to GM_GetSongInstrumentChanges and changed
**              Byte reference to XBYTE
**  8/27/97     Moved GM_StartHardwareSoundManager & GM_StopHardwareSoundManager from
**              GenPriv.h
**  9/15/97     Added GM_GetSampleReverb && GM_AudioStreamGetReverb
**              Added PatchInfo structure to combine some of instrument scan variables into one ptr
**              Changed PgmEntry to InstrumentEntry, added bank select fields
**  9/25/97     Added TrackStatus to represent track status rather than 'R' and 'F'. Symbolic use.
**  10/15/97    Added processingSlice to GM_Song and GM_Instrument to handle threading release issues.
**  10/16/97    Changed GM_LoadSong parmeters to include an option to ignore bad instruments
**              when loading.
**              Changed GM_LoadSongInstruments to use a XBOOL for a flag rather than an int
**              Added GM_AnyStereoInstrumentsLoaded
**              Added GM_CacheSamples
**  11/12/97    Added GM_MaxDevices & GM_SetDeviceID & GM_GetDeviceID & GM_GetDeviceName
**  11/17/97    Added GM_AudioStreamGetSamplesPlayed & GM_AudioStreamUpdateSamplesPlayed from Kara
**  11/18/97    Added GM_AudioStreamResumeAll & GM_AudioStreamPauseAll
**  12/16/97    Modified GM_GetDeviceID and GM_SetDeviceID to pass a device parameter pointer
**              that is specific for that device.
**  12/16/97    Moe: removed compiler warnings
**  1/14/98     kk: added numLoops field to GM_Waveform struct.
**              added theLoopTarget to GM_BeginSample
**  1/21/98     Added GM_SetChannelVolume & GM_GetChannelVolume
**  1/29/98     Added new defer parameter to GM_AudioStreamSetVolume
**  2/2/98      Added GM_SetVelocityCurveType
**  2/3/98      Added GM_SetupReverb & GM_CleanupReverb
**  2/8/98      Changed BOOL_FLAG to XBOOL
**  2/11/98     Added Q_48K, Q_24K, Q_8K and GM_ConvertFromOutputQualityToRate
**  2/15/98     Removed songMicrosecondIncrement from the GM_Song structure. Not used
**  2/23/98     Removed GM_InitReverbTaps & GM_GetReverbTaps & GM_SetReverbTaps
**  3/4/98      Added constant MAX_SAMPLE_FRAMES that represents the mixer limit
**  3/12/98     Modified GM_BeginDoubleBuffer to include a sample done callback
**  3/16/98     Added GM_ProcessReverb && GM_GetReverbEnableThreshold
**  3/23/98     Added MAX_SAMPLE_RATE and moved some fields around in GM_Instrument
**              for alignment
**              Added MIN_SAMPLE_RATE
**  3/23/98     MOE: Changed MAX_SAMPLE_RATE and MIN_SAMPLE_RATE to be unsigned
**  3/24/98     Changed a code wrapper USE_STREAM_API to USE_HIGHLEVEL_FILE_API
**  5/4/98      Eliminated neverInterpolate & enablePitchRandomness from the 
**              GM_Instrument structure. Its not used. Also got rid of
**              enablePitchRandomness & disableClickRemoval in the GM_Song structure.
**  5/5/98      Changed GM_ReadAndDecodeFileStream to return an OPErr
**  5/7/98      Changed GM_ReadFileInformation & GM_ReadFileIntoMemory & GM_ReadFileIntoMemoryFromMemory
**              to set an error code if failure
**              Added GM_SetChannelReverb & GM_GetChannelReverb
**  5/15/98     Added trackStatusSave to the GM_Song structure will helps when doing loopstart/loopend
**
**  6/5/98      Jim Nitchals RIP    1/15/62 - 6/5/98
**              I'm going to miss your irreverent humor. Your coding style and desire
**              to make things as fast as possible. Your collaboration behind this entire
**              codebase. Your absolute belief in creating the best possible relationships 
**              from honesty and integrity. Your ability to enjoy conversation. Your business 
**              savvy in understanding the big picture. Your gentleness. Your willingness 
**              to understand someone else's way of thinking. Your debates on the latest 
**              political issues. Your generosity. Your great mimicking of cartoon voices. 
**              Your friendship. - Steve Hales
**
**  6/19/98     Added message STREAM_HAVE_DATA to GM_StreamMessage
**  6/26/98     Added GM_IsReverbFixed
**  6/30/98     Changed songID from XSWORD to XSDWORD in GM_Song structure
**  7/1/98      Changed various API to use the new XResourceType and XLongResourceID or XShortResourceID
**  7/6/98      Added GM_IsSongInstrumentLoaded
**              Fixed type problems with GM_LoadSong
**  7/28/98     Added songMasterStereoPlacement to GM_Song
**              Added GM_SetSongStereoPosition & GM_GetSetStereoPosition
**
**  JAVASOFT
**  03.17.98:   kk: added UNSUPPORTED_HARDWARE to OPErr
**  ??          kk: added GM_AudioStreamDrain()
**  08.17.98:   kk: put USE_STREAM_API back rather than USE_HIGHLEVEL_FILE_API;
**              otherwise cannot compile.
**  8/13/98     Added GM_VoiceType to GM_AudioInfo structure
**              Added GM_GetMixerUsedTime
**  8/14/98     Added GM_GetMixerUsedTimeInPercent
**  9/8/98      Added SAMPLE_TO_LARGE to error codes
**  9/10/98     Added two new fields to GM_Waveform to handle streams and custom
**              object pointers.
**  9/12/98     Added GM_GetSamplePlaybackPointer
**  9/22/98     Added BAD_SAMPLE_RATE
**  9/25/98     Added new parameter to GM_ReadFileInformation to handle block
**              allocation by file types
**  10/27/98    Moved MIN_LOOP_SIZE to GenPriv.h
**  11/6/98     Removed noteDecayPref from the GM_Waveform structure.
**  11/24/98    Added GM_GetSampleReverbAmount & GM_SetSampleReverbAmount
**              Added NOT_READY as a new OPErr
**  11/30/98    Added support for omni mode in GM_Song
**              Added GM_EndSongChannelNotes to support omni mode
**  12/3/98     Added GM_GetStreamReverbAmount & GM_SetStreamReverbAmount
**  12/9/98     Added GM_GetPitchBend
**  12/18/98    Changed GM_BeginSong to include a new parameter for autoLevel
**  1/13/99     Added a dynamic Katmai flag and some voice calculation flags
**  1/14/99     Added GM_LoadInstrumentFromExternalData
**  2/12/99     Renamed USE_HAE_FOR_MPEG to USE_MPEG_DECODER
**  2/18/99     Changed GM_LoadSong & GM_CreateLiveSong to pass in a context
**              Added GM_GetSongContext & GM_SetSongContext
**              Removed GM_GetUserReference & GM_SetUserReference
**  2/24/99     Removed songEndCallbackReference1 & songEndCallbackReference2 from
**              GM_Song structure
**              Removed the extra reference in GM_SetSongMetaEventCallback and the
**              GM_Song structure
**  2/25/99     Increased MAX_SONGS to 16 from 8.
**  3/2/99      Changed all stream references to STREAM_REFERENCE rather than
**              the blank 'long'
**  3/3/99      Added GM_GetSampleStartTimeStamp
**              Renamed GM_BeginSample to GM_SetupSample, GM_BeginDoubleBuffer to GM_SetupSampleDoubleBuffer,
**              GM_BeginSampleFromInfo to GM_SetupSampleFromInfo, and added GM_StartSample
**              Added GM_GetSampleFrequencyFilter GM_SetSampleFrequencyFilter GM_GetSampleResonanceFilter 
**              GM_SetSampleResonanceFilter GM_GetSampleLowPassAmountFilter GM_SetSampleLowPassAmount
**  3/5/99      Added GM_SetSyncSampleStartReference & GM_SyncStartSample
**  3/8/99      Renamed GM_EndSoundEffects to GM_EndAllSamples
**  3/11/99     Renamed ADSRRecord to GM_ADSR. Renamed LFORecord to GM_LFO. Renamed CurveRecord to GM_TieTo.
**  3/12/99     Put in support for different loop types
**  3/24/99     Added ABORTED_PROCESS
**  5/16/99     Added GM_PrerollSong & GM_IsSongPaused
**  5/19/99     Added FILE_NOT_FOUND error code
**  5/28/99     MOE:  Moved DEFAULT_REVERB_LEVEL, DEFAULT_REVERB_LEVEL,
**              DEFAULT_REVERB_LEVEL from GenPriv.h
**  6/8/99      Added channelMonoMode in GM_Song
**  6/15/99     Added GM_EndSongButKeepActive
**  7/9/99      Modified GM_AudioTaskCallbackPtr to use a reference
**  7/19/99     Renamed UBYTE to XBYTE. Renamed INT16 to XSWORD. Renamed INT32 to XSDWORD.
**              Renamed UINT32 to XDWORD. Renamed SBYTE to XSBYTE. Renamed UINT16 to XWORD
**  8/3/99      Added GM_WriteFileFromMemory
**              Changed pragma settings for X_BE
**  9/9/99      MOE: Added GM_Waveform.compressionType
**  10/8/99     Added GM_BANK_TO_IGOR_BANK
**  10/30/99    Added Q_16K
**  11/4/99     Added GM_KillSongEventsFromQueue
**  11/9/99     Added songPriority to GM_Song structure. Added GM_SetSongPriority & GM_GetSongPriority
**  1/12/2000   Added GM_IsAudioActive
**  1/13/2000   Added GM_AreEventsPending
**  1/31/00     Added GM_Generate16bitOutP(), GM_GenerateStereoOutP(), GM_GetQuality()
**              Added GM_GetInterpolationMode(), GM_IsGeneralSoundPaused(), GM_NewWaveform()
**              Added GenAPI functions to set/get GM_Sound structure
**              Added GM_SetDisposeSongDataWhenDoneFlag(), GM_GetDisposeSongDataWhenDoneFlag()
**              Added GM_ChangeSongVoices(), GM_GetSongVoices()
**              Added new error codes NULL_OBJECT, RESOURCE_NOT_FOUND
**  2/3/00      Added GM_Mixer parameter to GM_FinisGeneralSound()
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  2/26/2000   Added GM_GetSongMixer & GM_SetSongMixer.
**              Added a GM_Mixer reference to GM_Song
**              Added GM_Mixer parameter to GM_LoadSong
**  3/3/2000    Removed extra array instrumentRemap from the GM_Song structure. Added
**              flag checkedForAliases to determine if the remap array has been setup.
**              Added GM_SetupSongRemaps & GM_GetSongInstrumentReamp
**  3/7/2000    Added GM_IsSongInstrumentRemapped
**              Added UNIT_TYPE and commented places where code footprint can change.
**  4/3/2000    Changed GM_GetSongInstrumentReamp to GM_GetSongInstrumentRemap
**  2000.04.24 AER  Moved GM_SetCacheSamples and GM_GetCacheSamples to
**                      GenCache.h
**  2000.04.24 msd  Added GM_FinalizeFileHeader() to support writing output to file
**  2000.04.25 msd  Added support for 32kHz and 40kHz output.
**                  Added GM_GetProgramBank() and GM_ReadRawAudioIntoMemoryFromMemory()
**                    to support MiniBAE API lego.
**  2000.04.27 msd  Reordered Quality to be the same as BAEQuality
**                  Added GM_WriteAudioBufferToFile()
**  2000.05.16 AER  Completed modifications for new sample cache
**  2000.05.22 sh   Changed sustainingDecayLevel to XFIXED, which is what it is.
**                  Added GM_GetSustainDecayLevelInTime & GM_SetSustainDecayLevelInTime
**  2000.05.24 sh   Added metaLoopDisabled in GM_Song. Added bitfields in various XBOOL's in
**                  GM_Song & GM_Instrument structures.
**  2000.05.25  sh  Added GM_GetSongMetaLoopFlag & GM_SetSongMetaLoopFlag.
**  2000.05.28  sh  Added GM_GetCurrentMixer
**  07/11/2000  DS: Added start frame offset argument to GM_SetupSample.
**  2000.07.14 AER  Moved XBankToken to X_API
**  8/27/2000   sh  Added MAX_TRACKS_EXCEEDED to OPErr
**  9/7/2000    sh  Increased patch in GM_AudioInfo to a XLongResourceID to match
**                  what's in the GM_Voice
**  9/7/2000    sh  Added GM_KillSongInstrument. Changed patch in GM_AudioInfo 
**                  to a XLongResourceID to better reflect the value.
**  10/26/2000  sh  Added GM_AudioStreamSetLoop & GM_AudioStreamGetLoop
**  11/1/2000   sh  Added a flag USE_MEMORY_OPTS to control a memory tweak feature.
**                  Fixed various enum that were matched to the wrong equivalent
**                  enums that caused sustains in envelopes to disappear.
**  12/7/2000   sh  Fixed comment in GM_Song for variables songMidiTickLength &
**                  songMicrosecondLength. 0 means not calculated not -1.
**                  Changed songMidiTickLength & songMicrosecondLength from the GM_Song
**                  structure to be UFLOAT's. So that we preserve long files. This will
**                  later fail when we deliver to the client a UINT32, but at least internaly
**                  we're ok.
**  12/12/2000  sh  Added GM_ResetTempoToDefault
**  1/2/2001    sh  Happy New Space Odyssey 2001! Added GM_SetSamplePlaybackPosition.
**                  Changed copyright. Added GM_AudioStreamSetFileSamplePosition.
**                  Added new message and element in GM_StreamData to handle
**                  reposition.
**  1/17/2001   sh  Added GM_AudioStreamGetPlaybackSamplePosition
**  1/25/2001   sh  Added GM_IsSongPrerolled
**  4/17/2001   sh  Started partitioning code for MCU/DSP split. Added BAE_BuildMCUSlice &
**                  GM_ProcessSyncUpdateFromDSP. Added synth loop API's for sending
**                  over samples.
**  4/19/2001   sh  Added GM_SendVoiceSamplesToDSP16 & GM_SendVoiceSamplesToDSP8 &
**                  GM_UpdateVoiceOnDSPNoFilter & GM_UpdateVoiceOnDSPOnePole &
**                  GM_UpdateVoiceOnDSPDelayLine
**  4/20/2001   sh  Modified GM_SendVoiceSamplesToDSP16 & GM_SendVoiceSamplesToDSP8
**                  to accept a channels parameter for stereo data
**                  Added GM_StartVoiceOnDSP, changed parmeters of GM_KillVoiceOnDSP
**  4/26/2001   sh  Renamed GM_StartVoiceOnDSP to GM_InitVoiceOnDSP
**  5/15/2001   sh  Removed bitfields. Fails on gcc.
**  6/18/2001       Added midi command defines
**  1/28/2004   sh  Replaced Quality enum with Rate. Changed all API's to support
**                  a more sane approach to sample rates.
*/
/*****************************************************************************/

#ifndef G_SOUND
#define G_SOUND

#ifndef __X_API__
    #include "X_API.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif


/* System defines */


/* Used in InitGeneralSound */

// Supported sample rates
typedef enum
{
    Q_RATE_INVALID      =   0,
    Q_RATE_7K           =   7813L,          // 7.813 kHz
    Q_RATE_8K           =   8000L,          // 8 kHz
    Q_RATE_8270         =   8270L,          // 8.270 kHz
    Q_RATE_10K          =   10417L,         // 10.417 kHz
    Q_RATE_10677        =   10677L,         // 10.677 kHz
    Q_RATE_11K          =   11025L,         // 11 kHz
    Q_RATE_11027        =   11027L,         // 11.027 kHz
    Q_RATE_11K_TERP_22K =   -11025L,        // 11 kHz interpolated to 22 kHz
    Q_RATE_15K          =   15625L,         // 15.625 kHz
    Q_RATE_16K          =   16000L,         // 16 kHz
    Q_RATE_16540        =   16540L,         // 16.540 kHz
    Q_RATE_20K          =   20833L,         // 20.833 kHz
    Q_RATE_22K          =   22050L,         // 22 kHz
    Q_RATE_22K_TERP_44K =   -22050L,        // 22 kHz interpolated to 44 kHz
    Q_RATE_22053        =   22053L,         // 22.053 kHz
    Q_RATE_24K          =   24000L,         // 24 kHz
    Q_RATE_32K          =   32000L,         // 32 kHz
    Q_RATE_40K          =   40000L,         // 40 kHz
    Q_RATE_44K          =   44100L,         // 44 kHz
    Q_RATE_48K          =   48000           // 48 kHz
} Rate;


// Modifier types
#define M_NONE              0L
#define M_USE_16            (1<<0L)
#define M_USE_STEREO        (1<<1L)
#define M_DISABLE_REVERB    (1<<2L)
#define M_STEREO_FILTER     (1<<3L)
typedef long AudioModifiers;

// Interpolation types
enum
{
    E_AMP_SCALED_DROP_SAMPLE = 0,
    E_2_POINT_INTERPOLATION,
    E_LINEAR_INTERPOLATION,
    E_LINEAR_INTERPOLATION_FLOAT,
    E_LINEAR_INTERPOLATION_U3232
};
typedef long TerpMode;

// verb types
enum 
{
    REVERB_NO_CHANGE = 0,   // Don't change current mixer settings
    REVERB_TYPE_1 = 1,      // None
    REVERB_TYPE_2,          // Igor's Closet
    REVERB_TYPE_3,          // Igor's Garage
    REVERB_TYPE_4,          // Igor's Acoustic Lab
    REVERB_TYPE_5,          // Igor's Dungeon
    REVERB_TYPE_6,          // Igor's Cavern
    REVERB_TYPE_7,          // Small reflections Reverb used for WebTV
    REVERB_TYPE_8,          // Early reflections (variable verb)
    REVERB_TYPE_9,          // Basement (variable verb)
    REVERB_TYPE_10,         // Banquet hall (variable verb)
    REVERB_TYPE_11          // Catacombs (variable verb)
};
typedef char ReverbMode;
#define MAX_REVERB_TYPES    12

typedef void (*GM_ReverbProc)(ReverbMode which);

typedef struct
{
    ReverbMode      type;
    XBYTE           thresholdEnableValue;   // 0 for variable, value to enable fixed
    XBOOL           isFixed;
    unsigned long   globalReverbUsageSize;  // GM_Mixer->reverbBuffer size
    GM_ReverbProc   pMonoRuntimeProc;
    GM_ReverbProc   pStereoRuntimeProc;
} GM_ReverbConfigure;


typedef enum 
{
    SCAN_NORMAL = 0,        // normal Midi scan
    SCAN_SAVE_PATCHES,      // save patches during Midi scan. Fast
    SCAN_DETERMINE_LENGTH,  // calculate tick length. Slow
    SCAN_FIND_PATCHES,      // mode for scanning to find the program changes
    SCAN_COUNT_PATCHES      // mode for counting the number of program changes
} ScanMode;

typedef enum 
{
    SAFE_TO_ACCESS = 0,     // memory structure is safe to access
    ASK_FOR_ACCESS,         // ask for access
    ACKNOWLEGE_ACCESS       // acknowlege access
} AccessSemaphore;

// used in GM_Song->trackon to track progress of a midi file
enum 
{
    TRACK_OFF = 0,
    TRACK_FREE,
    TRACK_RUNNING
};
typedef unsigned char TrackStatus;

enum
{
    VELOCITY_CURVE_1 = 0,   // default S curve
    VELOCITY_CURVE_2,       // more peaky S curve
    VELOCITY_CURVE_3,       // inward curve centered around 95 (used for WebTV)
    VELOCITY_CURVE_4,       // two time exponential
    VELOCITY_CURVE_5        // two times linear
};
typedef unsigned char VelocityCurveType;


#define MAX_VOICES              64      // max voices at once
#define MAX_INSTRUMENTS         128     // MIDI number of programs per patch bank
#define MAX_BANKS               6       // three GM banks; three user banks
#define MAX_TRACKS              65      // max MIDI file tracks to process (64 + tempo track)
#define MAX_CHANNELS            17      // max MIDI channels + one extra for sound effects
#define MAX_CONTROLLERS         128     // max MIDI controllers
#define MAX_SONG_VOLUME         127
#define MAX_NOTE_VOLUME         127     // max note volume
#define MAX_CURVES              4       // max curve entries in instruments
#define MAX_LFOS                6       // max LFO's, make sure to add one extra for MOD wheel support
#define MAX_MASTER_VOLUME       256     // max volume level for master volume level
#define MAX_SAMPLES             256     // max number of samples that can be loaded
#define MAX_SONGS               16      // max number of songs that can play at one time
#define PERCUSSION_CHANNEL      9       // which channel (zero based) is the default percussion channel
#define MAX_SAMPLE_FRAMES       1048576 // max number of sample frames that we can play in one voice
                                        // 1024 * 1024 = 1MB. This limit exisits only in DROP_SAMPLE, TERP1, TERP2 cases
#define MIN_LOOP_SIZE           20      // min number of loop samples that can be processed

#define MIN_SAMPLE_RATE         ((unsigned long)1L)     // min sample rate. 1.5258789E-5 kHz
#define MAX_SAMPLE_RATE         rate48khz   // max sample rate  48 kHz
#define MAX_PAN_LEFT            (-63)   // max midi pan to the left
#define MAX_PAN_RIGHT           63      // max midi pan to the right

#define DEFAULT_PITCH_RANGE         2       // number of semi-tones that change with pitch bend wheel
#define DEFAULT_CHORUS_LEVEL        0       // value used for chorus-enabled instruments
#define DEFAULT_REVERB_LEVEL        40      // value used for reverb-enabled instruments
#define DEFAULT_REVERB_TYPE         REVERB_TYPE_4
#define REVERB_CONTROLER_THRESHOLD  13      // past this value via controlers, reverb is enabled. only for fixed reverb
#define DEFAULT_VELOCITY_CURVE      VELOCITY_CURVE_1
#define DEFAULT_PATCH               0
#define DEFAULT_BANK                0

#define GM_BANK_TO_IGOR_BANK        256     // An Igor bank value is two GM banks. One for pitched instruments
                                            // and one for percussion instruments. Use this constant to
                                            // convert a Midi bank value into an Igor instrument starting range.

// These are the MAX_BANK bank values. To get the bank number for Midi:
// use  (instrument / GM_BANK_TO_IGOR_BANK) = midi bank
//      (instrument / 128) = midi program
#define GM_PITCHED_BANK             0
#define GM_PERCUSSION_BANK          1
#define SPECIAL_PITCHED_BANK        2
#define SPECIAL_PERCUSSION_BANK     3
#define USER_PITCHED_BANK           4
#define USER_PERCUSSION_BANK        5

/* Common errors returned from the system */
typedef enum
{
    NO_ERR = 0,
    PARAM_ERR,
    MEMORY_ERR,
    BAD_SAMPLE,
    BAD_INSTRUMENT,
    BAD_MIDI_DATA,
    ALREADY_PAUSED,
    ALREADY_RESUMED,
    DEVICE_UNAVAILABLE,
    NO_SONG_PLAYING,
    STILL_PLAYING,
    NO_VOLUME,
    TOO_MANY_SONGS_PLAYING,
    BAD_FILE,
    NOT_REENTERANT,
    NOT_SETUP,
    BUFFER_TO_SMALL,
    NO_FREE_VOICES,
    OUT_OF_RANGE,
    INVALID_REFERENCE,
    STREAM_STOP_PLAY,
    BAD_FILE_TYPE,
    GENERAL_BAD,
    SAMPLE_TO_LARGE,
    BAD_SAMPLE_RATE,
    NOT_READY,
    UNSUPPORTED_HARDWARE,
    ABORTED_PROCESS,
    FILE_NOT_FOUND,
    RESOURCE_NOT_FOUND,
    ALREADY_EXISTS,
    NULL_OBJECT,
    MAX_TRACKS_EXCEEDED
} OPErr;

// Need a forward reference to the GM_Song struct to keep
// our compiler from complaining.
struct GM_Song;
struct GM_Mixer;

// Cache data types
typedef XLongResourceID XSampleID;

// sample and instrument callbacks
typedef XBOOL       (*GM_LoopDoneCallbackPtr)(void *context);
typedef void        (*GM_DoubleBufferCallbackPtr)(void *context, XPTR pWhichBufferFinished, XSDWORD *pBufferSize);
typedef void        (*GM_SoundDoneCallbackPtr)(void *context);
typedef void        (*GM_SampleFrameCallbackPtr)(void *threadContext, XSDWORD reference, XSDWORD sampleFrame);

// sequencer callbacks
typedef void        (*GM_ControlerCallbackPtr)(void *threadContext, struct GM_Song *pSong, void * reference, short int channel, short int track, short int controler, short int value);
typedef void        (*GM_SongCallbackProcPtr)(void *threadContext, struct GM_Song *pSong, void *reference);
typedef void        (*GM_SongTimeCallbackProcPtr)(void *threadContext, struct GM_Song *pSong, XDWORD currentMicroseconds, XDWORD currentMidiClock);
typedef void        (*GM_SongMetaCallbackProcPtr)(void *threadContext, struct GM_Song *pSong, char markerType, void *pMetaText, long metaTextLength, XSWORD currentTrack);

// mixer callbacks
typedef void        (*GM_AudioTaskCallbackPtr)(void *threadContext, void *reference);
typedef void        (*GM_AudioOutputCallbackPtr)(void *threadContext, void *samples, long sampleSize, long channels, unsigned long lengthInFrames);

#define X_PACK_FAST
#include "X_PackStructures.h"

struct GM_SampleCallbackEntry
{
    unsigned long                   frameOffset;
    GM_SampleFrameCallbackPtr       pCallback;
    long                            reference;
    struct GM_SampleCallbackEntry   *pNext;
};
typedef struct GM_SampleCallbackEntry GM_SampleCallbackEntry;

// Flags for embedded midi objects. (eMidi support)
enum
{
    EM_FLUSH_ID                 =   FOUR_CHAR('F','L','U','S'), //  'FLUS'      // immediate command
    EM_CACHE_ID                 =   FOUR_CHAR('C','A','C','H'), //  'CACH'      // immediate command
    EM_DATA_ID                  =   FOUR_CHAR('D','A','T','A')  //  'DATA'      // block resources
};


// Set this flag to 1 to enabled to small file to memory translator. This converts
// 4 byte tags stored in the HIRF resource format into local runtime smaller values.
#define USE_MEMORY_OPTS     1

// small footprint identifier. These represent the larger file based tags. This can be smaller,
// but every where the UNIT_TYPE is used is must be changed to the samller units.
#if USE_MEMORY_OPTS == 1
typedef short int   UNIT_TYPE;
#else
typedef long        UNIT_TYPE;
#endif

// Flags for ADSR module. GM_ADSR.ADSRFlags
enum
{
    ADSR_OFF_LONG               =   0,
    ADSR_LINEAR_RAMP_LONG       =   FOUR_CHAR('L','I','N','E'), //  'LINE'
    ADSR_SUSTAIN_LONG           =   FOUR_CHAR('S','U','S','T'), //  'SUST'
    ADSR_TERMINATE_LONG         =   FOUR_CHAR('L','A','S','T'), //  'LAST'
    ADSR_GOTO_LONG              =   FOUR_CHAR('G','O','T','O'), //  'GOTO'
    ADSR_GOTO_CONDITIONAL_LONG  =   FOUR_CHAR('G','O','S','T'), //  'GOST'
    ADSR_RELEASE_LONG           =   FOUR_CHAR('R','E','L','S'), //  'RELS'

    ADSR_STAGES                 =   8,      // max number of ADSR stages

#if USE_MEMORY_OPTS == 1
    // small footprint translations. These represent the larger file based tags.
    // Used as a UNIT_TYPE. See PV_TranslateFromFileToMemoryID in GenPatch.c
    ADSR_OFF                    =   0,
    ADSR_LINEAR_RAMP            =   1,
    ADSR_SUSTAIN                =   2,
    ADSR_TERMINATE              =   3,
    ADSR_GOTO                   =   4,
    ADSR_GOTO_CONDITIONAL       =   5,
    ADSR_RELEASE                =   6
#else
    ADSR_OFF                    =   ADSR_OFF_LONG,
    ADSR_LINEAR_RAMP            =   ADSR_LINEAR_RAMP_LONG,
    ADSR_SUSTAIN                =   ADSR_SUSTAIN_LONG,
    ADSR_TERMINATE              =   ADSR_TERMINATE_LONG,
    ADSR_GOTO                   =   ADSR_GOTO_LONG,
    ADSR_GOTO_CONDITIONAL       =   ADSR_GOTO_CONDITIONAL_LONG,
    ADSR_RELEASE                =   ADSR_RELEASE_LONG
#endif
};


struct GM_ADSR
{
    XSDWORD         currentTime;
    XSDWORD         currentLevel;
    XSDWORD         previousTarget;
    XFIXED          sustainingDecayLevel;
    XSDWORD         ADSRLevel[ADSR_STAGES];
    XSDWORD         ADSRTime[ADSR_STAGES];
    UNIT_TYPE       ADSRFlags[ADSR_STAGES];
    UNIT_TYPE       mode;
    XBYTE           currentPosition;    //  ranges from 0 to ADSR_STAGES
};
typedef struct GM_ADSR GM_ADSR;

// kinds of LFO modules. GM_LFO.where_to_feed & GM_TieTo.tieTo
enum
{
    VOLUME_LFO_LONG         =   FOUR_CHAR('V','O','L','U'), // 'VOLU'
    PITCH_LFO_LONG          =   FOUR_CHAR('P','I','T','C'), // 'PITC'
    STEREO_PAN_LFO_LONG     =   FOUR_CHAR('S','P','A','N'), // 'SPAN'
    STEREO_PAN_NAME2_LONG   =   FOUR_CHAR('P','A','N',' '), // 'PAN '
    LPF_FREQUENCY_LONG      =   FOUR_CHAR('L','P','F','R'), // 'LPFR'
    LPF_DEPTH_LONG          =   FOUR_CHAR('L','P','R','E'), // 'LPRE'
    LOW_PASS_AMOUNT_LONG    =   FOUR_CHAR('L','P','A','M'), // 'LPAM'

#if USE_MEMORY_OPTS == 1
    // small footprint translations. These represent the larger file based tags.
    // Used as a UNIT_TYPE. See PV_TranslateFromFileToMemoryID in GenPatch.c
    LOW_PASS_AMOUNT         =   10,
    VOLUME_LFO              =   11,
    PITCH_LFO               =   12,
    STEREO_PAN_LFO          =   13,
    STEREO_PAN_NAME2        =   14,
    LPF_FREQUENCY           =   15,
    LPF_DEPTH               =   16
#else
    LOW_PASS_AMOUNT         =   LOW_PASS_AMOUNT_LONG,
    VOLUME_LFO              =   VOLUME_LFO_LONG,
    PITCH_LFO               =   PITCH_LFO_LONG,
    STEREO_PAN_LFO          =   STEREO_PAN_LFO_LONG,
    STEREO_PAN_NAME2        =   STEREO_PAN_NAME2_LONG,
    LPF_FREQUENCY           =   LPF_FREQUENCY_LONG,
    LPF_DEPTH               =   LPF_DEPTH_LONG
#endif
};

// kinds of LFO wave shapes. GM_LFO.waveShape parameter
enum
{
    // 4 byte file codes
    SINE_WAVE_LONG          =       FOUR_CHAR('S','I','N','E'), // 'SINE'
    TRIANGLE_WAVE_LONG      =       FOUR_CHAR('T','R','I','A'), // 'TRIA'
    SQUARE_WAVE_LONG        =       FOUR_CHAR('S','Q','U','A'), // 'SQUA'
    SQUARE_WAVE2_LONG       =       FOUR_CHAR('S','Q','U','2'), // 'SQU2'
    SAWTOOTH_WAVE_LONG      =       FOUR_CHAR('S','A','W','T'), // 'SAWT'
    SAWTOOTH_WAVE2_LONG     =       FOUR_CHAR('S','A','W','2'), // 'SAW2'

#if USE_MEMORY_OPTS == 1
    // small footprint translations. These represent the larger file based tags.
    // Used as a UNIT_TYPE. See PV_TranslateFromFileToMemoryID in GenPatch.c
    SINE_WAVE               =       20,
    TRIANGLE_WAVE           =       21,
    SQUARE_WAVE             =       23,
    SQUARE_WAVE2            =       24,
    SAWTOOTH_WAVE           =       25,
    SAWTOOTH_WAVE2          =       26
#else
    SINE_WAVE               =       SINE_WAVE_LONG,
    TRIANGLE_WAVE           =       TRIANGLE_WAVE_LONG,
    SQUARE_WAVE             =       SQUARE_WAVE_LONG,
    SQUARE_WAVE2            =       SQUARE_WAVE2_LONG,
    SAWTOOTH_WAVE           =       SAWTOOTH_WAVE_LONG,
    SAWTOOTH_WAVE2          =       SAWTOOTH_WAVE2_LONG
#endif
};

// Additional elements used by Curve functions. GM_TieTo.tieTo parameter
enum 
{
    VOLUME_LFO_FREQUENCY_LONG   =   FOUR_CHAR('V','O','L','F'), // 'VOLF'
    PITCH_LFO_FREQUENCY_LONG    =   FOUR_CHAR('P','I','T','F'), // 'PITF'
    NOTE_VOLUME_LONG            =   FOUR_CHAR('N','V','O','L'), // 'NVOL'
    VOLUME_ATTACK_TIME_LONG     =   FOUR_CHAR('A','T','I','M'), // 'ATIM'
    VOLUME_ATTACK_LEVEL_LONG    =   FOUR_CHAR('A','L','E','V'), // 'ALEV'
    SUSTAIN_RELEASE_TIME_LONG   =   ADSR_SUSTAIN_LONG,
    SUSTAIN_LEVEL_LONG          =   FOUR_CHAR('S','L','E','V'), // 'SLEV'
    RELEASE_TIME_LONG           =   ADSR_RELEASE_LONG,
    WAVEFORM_OFFSET_LONG        =   FOUR_CHAR('W','A','V','E'), // 'WAVE'
    SAMPLE_NUMBER_LONG          =   FOUR_CHAR('S','A','M','P'), // 'SAMP'
    MOD_WHEEL_CONTROL_LONG      =   FOUR_CHAR('M','O','D','W'), // 'MODW'

#if USE_MEMORY_OPTS == 1
    VOLUME_LFO_FREQUENCY    =   40,
    PITCH_LFO_FREQUENCY     =   41,
    NOTE_VOLUME             =   42,
    VOLUME_ATTACK_TIME      =   43,
    VOLUME_ATTACK_LEVEL     =   44,
    SUSTAIN_RELEASE_TIME    =   ADSR_SUSTAIN,
    SUSTAIN_LEVEL           =   46,
    RELEASE_TIME            =   ADSR_RELEASE,
    WAVEFORM_OFFSET         =   48,
    SAMPLE_NUMBER           =   49,
    MOD_WHEEL_CONTROL       =   50
#else
    VOLUME_LFO_FREQUENCY    =   VOLUME_LFO_FREQUENCY_LONG,
    PITCH_LFO_FREQUENCY     =   PITCH_LFO_FREQUENCY_LONG,
    NOTE_VOLUME             =   NOTE_VOLUME_LONG,
    VOLUME_ATTACK_TIME      =   VOLUME_ATTACK_TIME_LONG,
    VOLUME_ATTACK_LEVEL     =   VOLUME_ATTACK_LEVEL_LONG,
    SUSTAIN_RELEASE_TIME    =   SUSTAIN_RELEASE_TIME_LONG,
    SUSTAIN_LEVEL           =   SUSTAIN_LEVEL_LONG,
    RELEASE_TIME            =   RELEASE_TIME_LONG,
    WAVEFORM_OFFSET         =   WAVEFORM_OFFSET_LONG,
    SAMPLE_NUMBER           =   SAMPLE_NUMBER_LONG,
    MOD_WHEEL_CONTROL       =   MOD_WHEEL_CONTROL_LONG
#endif
};

// these are used in the raw instrument format to determine a top level command and 
// structure of the following date
enum
{
    DEFAULT_MOD_LONG        =   FOUR_CHAR('D','M','O','D'),     //  'DMOD'
    ADSR_ENVELOPE_LONG      =   FOUR_CHAR('A','D','S','R'),     //  'ADSR'
    EXPONENTIAL_CURVE_LONG  =   FOUR_CHAR('C','U','R','V'),     //  'CURV'
    LOW_PASS_FILTER_LONG    =   FOUR_CHAR('L','P','G','F'),     //  'LPGF'

// These must be defined as 4 byte identifiers because they are used to parse the
// instrument from a file
    INST_ADSR_ENVELOPE      =   ADSR_ENVELOPE_LONG,
    INST_EXPONENTIAL_CURVE  =   EXPONENTIAL_CURVE_LONG,
    INST_LOW_PASS_FILTER    =   LOW_PASS_FILTER_LONG,
    INST_DEFAULT_MOD        =   DEFAULT_MOD_LONG,
    INST_PITCH_LFO          =   PITCH_LFO_LONG,
    INST_VOLUME_LFO         =   VOLUME_LFO_LONG,
    INST_STEREO_PAN_LFO     =   STEREO_PAN_LFO_LONG,
    INST_STEREO_PAN_NAME2   =   STEREO_PAN_NAME2_LONG,
    INST_LOW_PASS_AMOUNT    =   LOW_PASS_AMOUNT_LONG,
    INST_LPF_DEPTH          =   LPF_DEPTH_LONG,
    INST_LPF_FREQUENCY      =   LPF_FREQUENCY_LONG
};

// Controls for registered parameter status
enum
{
    USE_NO_RP = 0,      // no registered parameters selected
    USE_NRPN,           // use non registered parameters
    USE_RPN             // use registered parameters
};

// Controls for channelBankMode
enum
{
    USE_GM_DEFAULT = 0,         // this is default behavior
                                // normal bank for channels 1-9 and 11-16
                                // percussion for channel 10
    USE_NON_GM_PERC_BANK,       // will force the use of the percussion
                                // bank reguardless of channel and allow program
                                // changes to reflect the percussion instrments and
                                // allow you to change them with normal notes
    USE_GM_PERC_BANK,           // will force the use of the percussion
                                // bank reguardless of channel and act just like a GM
                                // percussion channel. ie. midi note represents the instrument
                                // to play
    USE_NORM_BANK               // will force the use of the normal
                                // bank reguardless of channel
    
};

struct GM_LFO
{
    XSDWORD     DC_feed;        // DC feed amount
    XSDWORD     level;
    XSDWORD     period;
    XSDWORD     currentTime;
    XSDWORD     LFOcurrentTime;
    XSDWORD     currentWaveValue;
    UNIT_TYPE   where_to_feed;
    UNIT_TYPE   waveShape;
    UNIT_TYPE   mode;
    GM_ADSR     a;
};
typedef struct GM_LFO GM_LFO;

// possible tieFrom values      to_Scalar range     from_Value range
//  PITCH_LFO                   | 
//  VOLUME_LFO                  | 
//  MOD_WHEEL_CONTROL           | 
//  SAMPLE_NUMBER               | 

// possible tieTo values        to_Scalar range     from_Value range
//  LPF_FREQUENCY               | 
//  NOTE_VOLUME                 | 
//  SUSTAIN_RELEASE_TIME        | 
//  SUSTAIN_LEVEL               | 
//  RELEASE_TIME                | 
//  VOLUME_ATTACK_TIME          | 
//  VOLUME_ATTACK_LEVEL         | 
//  WAVEFORM_OFFSET             | 
//  LOW_PASS_AMOUNT             | 
//  PITCH_LFO                   | 
//  VOLUME_LFO                  | 
//  PITCH_LFO_FREQUENCY         | 
//  VOLUME_LFO_FREQUENCY        | 

struct GM_TieTo
{
    XSWORD      to_Scalar[MAX_CURVES];
    XBYTE       from_Value[MAX_CURVES];     // midi range 0 to 127
    UNIT_TYPE   tieFrom;
    UNIT_TYPE   tieTo;
    XSWORD      curveCount;
};
typedef struct GM_TieTo GM_TieTo;



struct GM_KeymapSplit
{
    XBYTE                   lowMidi;
    XBYTE                   highMidi;
    XSWORD                  miscParameter1;     // can be smodParmeter1 if enableSoundModifier
                                                // enabled, otherwise its a replacement
                                                // rootKey for sample
    XSWORD                  miscParameter2;
    struct GM_Instrument    *pSplitInstrument;
};
typedef struct GM_KeymapSplit GM_KeymapSplit;

struct GM_KeymapSplitInfo
{
    XShortResourceID    defaultInstrumentID;
    XWORD               KeymapSplitCount;
    GM_KeymapSplit      keySplits[1];
};
typedef struct GM_KeymapSplitInfo GM_KeymapSplitInfo;

// main structure for all waveform objects
//MOE:  This structure should eventually be changed and merged with
//      SampleDataInfo{}, which is almost identical
//Changes:
//  take out currentFilePosition and keep track of streaming with
//      a different structure, like as is the case with MPEG
//  rename waveSize, theWaveform -> dataBytes, data
//  rename waveFrames -> frames
//  rename startLoop, endLoop -> loopStart, loopEnd
//  rename sampledRate -> sampleRate
//  add XPTR allocatedData field that is always XDisposed when a GM_Waveform
//      is "destroyed".  It may be the same as data and maybe not.
//      the data field itself is never disposed.  This provides a functionality
//      needed in SampleDataInfo{}, and may also be useful in representing
//      samples in ROM, or samples stored elsewhere
struct GM_Waveform
{
    XLongResourceID     waveformID;         // extra specific data
    XDWORD              currentFilePosition;// if used will be an file byte position
    XDWORD              compressionType;    // format of "theWaveform" (SndCompressionType)
    XWORD               baseMidiPitch;      // base Midi pitch of recorded sample ie. 60 is middle 'C'
    XBYTE               bitSize;            // number of bits per sample
    XBYTE               channels;           // number of channels
    XDWORD              waveSize;           // total waveform size in bytes
    XDWORD              waveFrames;         // number of frames
    XDWORD              startLoop;          // start loop point offset
    XDWORD              endLoop;            // end loop point offset
    XDWORD              numLoops;           // number of loops between loop points before continuing to end of sample
    XFIXED              sampledRate;        // FIXED_VALUE 16.16 value for recording
    XPTR                theWaveform;        // array data that morphs into what ever you need
};
typedef struct GM_Waveform GM_Waveform;

// Internal Instrument structure
// aligned structure to 8 bytes
struct GM_Instrument
{
    AccessSemaphore     accessStatus;
    XSWORD              masterRootKey;
    XShortResourceID    smodResourceID;
    XSWORD              miscParameter1;
    XSWORD              miscParameter2;

    XBOOL               disableSndLooping;      // Disable waveform looping
    XBOOL               playAtSampledFreq;      // Play instrument at sampledRate only
    XBOOL               doKeymapSplit;          // If TRUE, then this instrument is a keysplit defination
    XBOOL               notPolyphonic;          // if FALSE, then instrument is a mono instrument
    XBOOL               enableSoundModifier;
    XBOOL               extendedFormat;         // extended format instrument
    XBOOL               sampleAndHold;
    XBOOL               useSampleRate;          // factor in sample rate into pitch calculation

    XBOOL               processingSlice;
    XBOOL               useSoundModifierAsRootKey;
#if REVERB_USED != REVERB_DISABLED
    XBOOL               avoidReverb;            // if TRUE, this instrument is not mixed into reverb unit
#endif
    XBYTE               usageReferenceCount;    // number of references this instrument is associated to

    XBYTE               LFORecordCount;
    XBYTE               curveRecordCount;

    XSWORD              panPlacement;           // inital stereo pan placement of this instrument

    XSDWORD             LPF_frequency;
    XSDWORD             LPF_resonance;
    XSDWORD             LPF_lowpassAmount;

    GM_LFO              LFORecords[MAX_LFOS];
    GM_ADSR             volumeADSRRecord;
    GM_TieTo            curve[MAX_CURVES];
    union
    {
        GM_KeymapSplitInfo  k;
        GM_Waveform         w;
    } u;
};
typedef struct GM_Instrument GM_Instrument;

struct GM_ControlCallback
{
    // these pointers are NULL until used, then they are allocated
    GM_ControlerCallbackPtr callbackProc[MAX_CONTROLLERS];      // current controller callback functions
    void                    *callbackReference[MAX_CONTROLLERS];
};
typedef struct GM_ControlCallback GM_ControlCallback;
typedef struct GM_ControlCallback * GM_ControlCallbackPtr;


// sequence pointer type
typedef enum
{
    SEQ_MIDI = 0,           // sequenceData is a MIDI formatted stream
} SequenceType;

// Internal Song structure
// aligned structure to 8 bytes
struct GM_Song
{
    struct GM_Mixer     *pMixer;    // mixer associated to this song

    XShortResourceID    songID;
    XSWORD              maxSongVoices;
    XSWORD              mixLevel;
    XSWORD              maxEffectVoices;
    XSWORD              routeBus;

    // various values calculated during first scan of midi file. The one's marked
    // with a '*' are actaully usefull.
    XSWORD              averageTotalVoices;
    XSWORD              averageActiveVoices;
    XSWORD              voiceCount, voiceSustain;
    XSWORD              averageVoiceUsage;      // * average voice usage over time
    XSWORD              maxVoiceUsage;          // * max number of voices used

    XFIXED              MasterTempo;            // master midi tempo (fixed point)
    XWORD               songTempo;              // tempo (16667 = 1.0)
    XSWORD              songPitchShift;         // master pitch shift

    XWORD               allowPitchShift[(MAX_CHANNELS / 16) + 1];       // allow pitch shift

    void                *context;               // context of song creation. C++ 'this' pointer, thread, etc
    long                userReference;          // user reference. Can be anything

    GM_SongCallbackProcPtr      songEndCallbackPtr;     // called when song ends/stops/free'd up
    void                        *songEndCallbackReference;

    GM_SongTimeCallbackProcPtr  songTimeCallbackPtr;    // called every slice to pass the time
    long                        songTimeCallbackReference;

    GM_SongMetaCallbackProcPtr  metaEventCallbackPtr;   // called during playback with current meta events
    long                        metaEventCallbackReference;

    // these pointers are NULL until used, then they are allocated
    GM_ControlCallbackPtr       controllerCallback;     // called during playback with controller info

#if REVERB_USED != REVERB_DISABLED
    ReverbMode          defaultReverbType;
#endif
    VelocityCurveType   velocityCurveType;          // which curve to use. (Range is 0 to 2)

    ScanMode            AnalyzeMode;                // analyze mode (Byte)
    XBOOL               ignoreBadInstruments;       // allow bad patches. Don't fail because it can't load

    XBOOL               allowProgramChanges;
    XBOOL               loopSong;                   // loop song when done
    XBOOL               metaLoopDisabled;           // if TRUE, then meta loopstart/loopend are disabled
    XBOOL               disposeSongDataWhenDone;    // if TRUE, then free midi data
    XBOOL               SomeTrackIsAlive;           // song still alive
    XBOOL               songFinished;               // TRUE at start of song, FALSE and end
    XBOOL               processingSlice;            // TRUE if processing slice of this song
    XBOOL               omniModeOn;                 // if TRUE, then omni mode is on
    
    XBOOL               hasPercData;                    // if TRUE, then no channel 9 data
    XBOOL               hasAutoGenerateData;            // if TRUE, then has auto generate data

    XBOOL               songPaused;                 // if TRUE, sequencer is paused.
    XBOOL               songPrerolled;              // if TRUE, instruments are loaded, midi queued
    XBOOL               checkedForAliases;          // if FALSE, then aliases from bank have not been
                                                    // incorporated into the remapArray yet.

    XBOOL               songEndAtFade;              // when true, stop song at end of fade
    XFIXED              songFadeRate;               // when non-zero fading is enabled
    XFIXED              songFixedVolume;            // inital volume level that will be changed by songFadeRate
    XSWORD              songFadeMaxVolume;          // max volume
    XSWORD              songFadeMinVolume;          // min volume

    XSWORD              songPriority;               // higher values determine note stealing level

    XSWORD              songVolume;
    XSWORD              songMasterStereoPlacement;  // master stereo placement (-8192 to 8192)

    XSWORD              defaultPercusionProgram;    // default percussion program for percussion channel. 
                                                    // -1 means GM style bank select, -2 means allow program changes on percussion

    XSWORD              songLoopCount;              // current loop counter. Starts at 0
    XSWORD              songMaxLoopCount;           // when songLoopCount reaches songMaxLoopCount it will be set to 0

    UFLOAT              songMidiTickLength;         // song midi tick length. 0 not calculated yet.
    UFLOAT              songMicrosecondLength;      // song microsecond length. 0 not calculated yet.

    SequenceType        seqType;
    void                *sequenceData;              // sequence pointer data for this song
    XDWORD              sequenceDataSize;           // sequence size of data

    XDWORD              titleOffset;                // offset in bytes of midi file
    XDWORD              titleLength;                // for title=
    //  instrument array. These are the instruments that are used by just this song
    GM_Instrument       *instrumentData[MAX_INSTRUMENTS*MAX_BANKS];
    XLongResourceID     remapArray[MAX_INSTRUMENTS*MAX_BANKS];

    void                *pUsedPatchList;                // This is NULL most of the time, only
                                                        // GM_LoadSongInstruments sets it
                                                        // instruments by notes
                                                        // This should be about 4096 bytes
                                                        // during preprocess of the midi file, this will be
                                                        // the instruments that need to be loaded
                                                        // total divided by 8 bits. each bit represents an instrument

    XSBYTE              firstChannelBank[MAX_CHANNELS];     // set during preprocess. this is the program
    XSWORD              firstChannelProgram[MAX_CHANNELS];  // to be set at the start of a song
    XSWORD              firstNoteOnChannel;                 // first channel a noteon event occurs

// channel based controler values
    XSBYTE              channelWhichParameter[MAX_CHANNELS];            // 0 for none, 1 for RPN, 2 for NRPN
    XSBYTE              channelRegisteredParameterLSB[MAX_CHANNELS];    // Registered Parameter least signifcant byte
    XSBYTE              channelRegisteredParameterMSB[MAX_CHANNELS];    // Registered Parameter most signifcant byte
    XSBYTE              channelNonRegisteredParameterLSB[MAX_CHANNELS]; // Non-Registered Parameter least signifcant byte
    XSBYTE              channelNonRegisteredParameterMSB[MAX_CHANNELS]; // Non-Registered Parameter most signifcant byte
    XBYTE               channelBankMode[MAX_CHANNELS];                  // channel bank mode
    XBYTE               channelSustain[MAX_CHANNELS];                   // sustain pedal on/off
    XBYTE               channelVolume[MAX_CHANNELS];                    // current channel volume
    XBYTE               channelExpression[MAX_CHANNELS];                // current channel expression
    XBYTE               channelPitchBendRange[MAX_CHANNELS];            // current bend range in half steps
#if REVERB_USED != REVERB_DISABLED
    XBYTE               channelReverb[MAX_CHANNELS];                    // current channel reverb
    XBYTE               channelChorus[MAX_CHANNELS];                    // current channel chorus
#endif
    XBYTE               channelModWheel[MAX_CHANNELS];                  // Mod wheel (primarily affects pitch bend)
    XBYTE               channelLowPassAmount[MAX_CHANNELS];             // low pass amount controller (NOT CONNECTED as of 3.8.99)
    XBYTE               channelResonanceFilterAmount[MAX_CHANNELS];     // Resonance amount controller (NOT CONNECTED as of 3.8.99)
    XBYTE               channelFrequencyFilterAmount[MAX_CHANNELS];     // Frequency amount controller (NOT CONNECTED as of 3.8.99)
    XBYTE               channelMonoMode[MAX_CHANNELS];                  // boolean for mono mode being on or off (NOT CONNECTED as of 6.8.99)
    XSWORD              channelBend[MAX_CHANNELS];                      // MUST BE AN XSWORD!! current amount to bend new notes
    XSWORD              channelProgram[MAX_CHANNELS];                   // current channel program
    XSBYTE              channelBank[MAX_CHANNELS];                      // current bank
    XSWORD              channelStereoPosition[MAX_CHANNELS];            // current channel stereo position

// mute controls for tracks, channels, and solos
// NOTE: Do not access these directly. Use XSetBit & XClearBit & XTestBit
    XDWORD              trackMuted[(MAX_TRACKS / 32) + 1];          // track mute control bits 
    XDWORD              soloTrackMuted[(MAX_TRACKS / 32) + 1];      // solo track mute control bits 
    XWORD               channelMuted[(MAX_CHANNELS / 16) + 1];      // current channel muted status
    XWORD               soloChannelMuted[(MAX_CHANNELS / 16) + 1];  // current channel muted status

// internal timing variables for sequencer
    UFLOAT              UnscaledMIDITempo;
    UFLOAT              MIDITempo;
    UFLOAT              MIDIDivision;
    UFLOAT              UnscaledMIDIDivision;
    UFLOAT              CurrentMidiClock;
    UFLOAT              songMicroseconds;

// storage for loop playback
    XBOOL               loopbackSaved;
    XBYTE               *pTrackPositionSave[MAX_TRACKS];
    IFLOAT              trackTicksSave[MAX_TRACKS];     // must be signed
    TrackStatus         trackStatusSave[MAX_TRACKS];
    UFLOAT              currentMidiClockSave;
    UFLOAT              songMicrosecondsSave;
    XSBYTE              loopbackCount;

// internal position variables for sequencer. Set after inital preprocess
    TrackStatus         trackon[MAX_TRACKS];            // track playing? TRACK_FREE is free, TRACK_RUNNING is playing
    XDWORD              tracklen[MAX_TRACKS];           // length of track in bytes
    XBYTE               *ptrack[MAX_TRACKS];            // current position in track
    XBYTE               *trackstart[MAX_TRACKS];        // start of track
    XBYTE               runningStatus[MAX_TRACKS];      // midi running status
    IFLOAT              trackticks[MAX_TRACKS];         // current position of track in ticks. must be signed
//  XSDWORD             trackcumuticks[MAX_TRACKS];     // current number of beat ticks into track
};
typedef struct GM_Song GM_Song;

#include "X_UnpackStructures.h"

// Functions


/**************************************************/
/*
** FUNCTION InitGeneralSound(Rate theRate, 
**                              TerpMode theTerp, XSWORD maxVoices,
**                              XSWORD normVoices, XSWORD maxEffects,
**                              XSWORD maxChunkSize)
**
** Overvue --
**  This will setup the sound system, allocate memory, and such,
**  so that any calls to play effects or songs can happen right
**  away.
**
** Private notes:
**  This code will preinitialize the MIDI sequencer, allocate
**  amplitude scaling buffer & init it, and init the
**  General Magic sound system.
**
**  INPUT   --  theRate
**                  Q_RATE_11K  Sound ouput is 11025
**                  Q_RATE_22K  Sound output is 22050
**          --  theTerp
**                  Interpolation type
**          --  use16bit
**                  If true, then hardware will be setup for 16 bit output
**          --  maxVoices
**                  maximum voices
**          --  normVoices
**                  number of voices normally. ie a gain
**          --  maxEffects
**                  number of voices to be used as effects
**
**  OUTPUT  --  
**
** NOTE:    
**  Only call this once.
*/
/**************************************************/
OPErr GM_InitGeneralSound(void *threadContext, Rate theRate, TerpMode theTerp, AudioModifiers theMods,
                XSWORD maxVoices, XSWORD normVoices, XSWORD maxEffects, struct GM_Mixer **outMixer);

OPErr GM_Generate16bitOutP(XBOOL *outGenerate16);
OPErr GM_GenerateStereoOutP(XBOOL *outGenerateStereo);
OPErr GM_GetRate(Rate *outRate);
OPErr GM_GetInterpolationMode(TerpMode *outTerpMode);

// convert GenSynth Rate to actual sample rate used
unsigned long GM_ConvertFromOutputRateToRate(Rate rate);

/**************************************************/
/*
** FUNCTION FinisGeneralSound;
**
** Overvue --
**  This will release any memory allocated by InitGeneralSound, clean up.
**
**  INPUT   --  
**  OUTPUT  --  
**
** NOTE:    
**  Only call this once.
*/
/**************************************************/
void GM_FinisGeneralSound(void *threadContext, struct GM_Mixer *mixer);

// Returns the global GM_Mixer pointer
struct GM_Mixer * GM_GetCurrentMixer(void);

// get calculated microsecond time different between mixer slices.
unsigned long GM_GetMixerUsedTime(void);

// Get CPU load in percent. This function is realtime and assumes the mixer has been allocated
unsigned long GM_GetMixerUsedTimeInPercent(void);

// return time in microseconds for the decay during sustain system. Values passed in are the stored values
// for ADSRLevel.
//  15 000 000 = 15 seconds
//   1 500 000 = 1.5 seconds
//     150 000 = 15 microseconds
unsigned long GM_GetSustainDecayLevelInTime(long storedValue);
// given a timeInMicroseconds range check between 50 microseconds and 16 seconds, and return the storable
// value for ADSRLevel.
long GM_SetSustainDecayLevelInTime(unsigned long timeInMicroseconds);

#if REVERB_USED != REVERB_DISABLED
// allocate the reverb buffers and enable them.
void GM_SetupReverb(void);
// deallocate the reverb buffers
void GM_CleanupReverb(void);
// Is current reverb fixed (old style)?
XBOOL GM_IsReverbFixed(void);
// get highest MIDI verb amount required to activate verb
XBYTE GM_GetReverbEnableThreshold(void);
// Set the global reverb type
void GM_SetReverbType(ReverbMode theReverbMode);
// Get the global reverb type
ReverbMode GM_GetReverbType(void);

// process the verb. Only call on data currently in the mix bus
void GM_ProcessReverb(void);
#endif

void GM_TestTone(XBOOL toneStatus);
void GM_TestToneFrequency(XFIXED freq);


/* Sound hardware specific
*/
XBOOL       GM_StartHardwareSoundManager(void *threadContext);
void        GM_StopHardwareSoundManager(void *threadContext);

// number of devices. ie different versions of the HAE connection. DirectSound and waveOut
// return number of devices. ie 1 is one device, 2 is two devices.
// NOTE: This function needs to function before any other calls may have happened.
long GM_MaxDevices(void);

// set the current device. device is from 0 to HAE_MaxDevices()
// NOTE:    This function needs to function before any other calls may have happened.
//          Also you will need to call HAE_ReleaseAudioCard then HAE_AquireAudioCard
//          in order for the change to take place. deviceParameter is a device specific
//          pointer. Pass NULL if you don't know what to use.
void GM_SetDeviceID(long deviceID, void *deviceParameter);

// return current device ID, and fills in the deviceParameter with a device specific
// pointer. It will pass NULL if there is nothing to use.
// NOTE: This function needs to function before any other calls may have happened.
long GM_GetDeviceID(void *deviceParameter);

// get deviceID name 
// NOTE:    This function needs to function before any other calls may have happened.
//          Format of string is a zero terminated comma delinated C string.
//          "platform,method,misc"
//  example "MacOS,Sound Manager 3.0,SndPlayDoubleBuffer"
//          "WinOS,DirectSound,multi threaded"
//          "WinOS,waveOut,multi threaded"
//          "WinOS,VxD,low level hardware"
//          "WinOS,plugin,Director"
void GM_GetDeviceName(long deviceID, char *cName, unsigned long cNameLength);

void GM_GetSystemVoices(XSWORD *pMaxSongVoices, XSWORD *pMixLevel, XSWORD *pMaxEffectVoices);

OPErr GM_ChangeSystemVoices(XSWORD maxVoices, XSWORD mixLevel, XSWORD maxEffects);

OPErr GM_ChangeAudioModes(void *threadContext, Rate theRate, TerpMode theTerp, AudioModifiers theMods);

/**************************************************/
/*
** FUNCTION PauseGeneralSound;
**
** Overvue --
**  This is used to pause the system, and release hardware for other tasks to
**  play around. Call ResumeGeneralSound when ready to resume working with
**  the system.
**
**  INPUT   --  
**  OUTPUT  --  OPErr   Errors in pausing system.
**
** NOTE:    
*/
/**************************************************/
OPErr GM_PauseGeneralSound(void *threadContext);
OPErr GM_IsGeneralSoundPaused(XBOOL *outIsPaused);

// Pause all songs
void GM_PauseSequencer(XBOOL endVoices);
// resume all songs
void GM_ResumeSequencer(void);

// Pause just this song
void GM_PauseSong(GM_Song *pSong, XBOOL endVoices);
// Resume just this song
void GM_ResumeSong(GM_Song *pSong);
XBOOL GM_IsSongPaused(GM_Song *pSong);

char GM_GetControllerValue(GM_Song *pSong, XSWORD channel, XSWORD controller);
// return pitch bend for channel
void GM_GetPitchBend(GM_Song *pSong, XSWORD channel, unsigned char *pLSB, unsigned char *pMSB);

OPErr GM_GetProgramBank(GM_Song *pSong, XSWORD channel, XSWORD *outProgram, XSWORD *outBank);

/**************************************************/
/*
** FUNCTION ResumeGeneralSound;
**
** Overvue --
**  This is used to resume the system, and take over the sound hardware. Call this
**  after calling PauseGeneralSound.
**
**  INPUT   --  
**  OUTPUT  --  OPErr   Errors in resuming. Continue to call until errors have
**                      stops, or alert user that something is still holding
**                      the sound hardware.
**
** NOTE:    
*/
/**************************************************/
OPErr GM_ResumeGeneralSound(void *threadContext);

/**************************************************/
/*
** FUNCTION BeginSong(GM_Song *theSong, GM_SongCallbackProcPtr theCallbackProc);
**
** Overvue --
**  This will start a song, given the data structure that contains the midi data,
**  instrument data, and specifics about playback of this song.
**
**  INPUT   --  theSong,            contains pointers to the song data, midi data,
**                                  all instruments used in this song.
**              theCallbackProc,    when the song is done, even in the looped
**                                  case, this procedure will be called.
**              useEmbeddedMixerSettings
**                                  when TRUE the mixer will be reconfigured
**                                  to use the embedded song settings. If FALSE
**                                  then the song will use the current settings
**  OUTPUT  --  
**
** NOTE:    
*/
/**************************************************/

// return preroll status
XBOOL GM_IsSongPrerolled(GM_Song *pSong);
// Call GM_PreollSong to allocate the song in the song buss, configure the sequencer
// but don't start the sequence
OPErr GM_PrerollSong(GM_Song *pSong, GM_SongCallbackProcPtr theCallbackProc, 
                    XBOOL useEmbeddedMixerSettings, XBOOL autoLevel);
// Start the song. Will call GM_PrerollSong if not already done.
OPErr GM_BeginSong(GM_Song *theSong, GM_SongCallbackProcPtr theCallbackProc, XBOOL useEmbeddedMixerSettings, XBOOL autoLevel);

// Set song priority level. Higher values will allow for better note stealing.
OPErr GM_SetSongPriority(GM_Song *pSong, XSWORD songPriority);
// Get song priority level. Higher values will allow for better note stealing.
XSWORD GM_GetSongPriority(GM_Song *pSong);

XSWORD GM_GetSongRouteBus(GM_Song *pSong);
OPErr GM_SetSongRouteBus(GM_Song *pSong, XSWORD routeBus);

// return valid context for song that was pass in when calling GM_LoadSong or GM_CreateLiveSong
void * GM_GetSongContext(GM_Song *pSong);
// set valid context for song
void GM_SetSongContext(GM_Song *pSong, void *context);

// return associated Mixer from Song
struct GM_Mixer * GM_GetSongMixer(GM_Song *pSong);
// Associate a Mixer to a Song
OPErr GM_SetSongMixer(GM_Song *pSong, struct GM_Mixer *pMixer);

// Load the SongID from an external SONG resource and or a extneral midi resource.
//
//  pMixer              When the song is created this is the mixer it will be associated
//                      with during playback. Can be NULL, if you're not loading
//                      instrments ie. loadInstruments = FALSE
//
//  threadContext       context of thread. passed all the way down to callbacks
//  context             context of song creation. C++ 'this' pointer, etc. this is a user variable
//                      Its just stored in the GM_Song->context variable
//  songID              will be the ID used during playback
//  theExternalSong     standard SONG resource structure

//  theExternalMidiData if not NULL, then will use this midi data rather than what is found in external SONG resource
//  midiSize            size of midi data if theExternalMidiData is not NULL
//                      theExternalMidiData and midiSize is not used if the songType is RMF

//  pInstrumentArray    array, if not NULL will be filled with the instruments that need to be loaded.
//  loadInstruments     if not zero, then instruments and samples will be loaded
//  pErr                pointer to an OPErr
GM_Song * GM_LoadSong(struct GM_Mixer *pMixer,
                      void *threadContext,
                      void *context,
                      XShortResourceID songID,
                      void *theExternalSong,
                      void *theExternalMidiData,    //MOE: This parameter should be const!
                      long midiSize,
                      XShortResourceID *pInstrumentArray,
                      XBOOL loadInstruments,
                      XBOOL ignoreBadInstruments,
                      XBankToken bankToken,
                      OPErr *pErr);
// Create Song with no midi data associated. Used for direct control of a synth object
GM_Song * GM_CreateLiveSong(void *context, XShortResourceID songID);
OPErr GM_StartLiveSong(GM_Song *pSong, XBOOL loadPatches, XBankToken bankToken);

// return note offset in semi tones (12 is down an octave, -12 is up an octave)
long GM_GetSongPitchOffset(GM_Song *pSong);
// set note offset in semi tones    (12 is down an octave, -12 is up an octave)
void GM_SetSongPitchOffset(GM_Song *pSong, long offset);
// If allowPitch is FALSE, then "GM_SetSongPitchOffset" will have no effect on passed 
// channel (0 to 15)
void GM_AllowChannelPitchOffset(GM_Song *pSong, unsigned short int channel, XBOOL allowPitch);
// Return if the passed channel will allow pitch offset
XBOOL GM_DoesChannelAllowPitchOffset(GM_Song *pSong, unsigned short int channel);

// Stop this song playing, or if NULL stop all songs playing.
// This removes the Song from the mixer, so you can no longer send
// midi events to the song for processing.
void GM_EndSong(void *threadContext, GM_Song *pSong);

// Stop this song playing, or if NULL stop all songs playing.
// This does NOT remove the Song from the mixer, so you can send
// midi events to the song for processing.
void GM_EndSongButKeepActive(void *threadContext, GM_Song *pSong);

OPErr GM_FreeSong(void *threadContext, GM_Song *theSong);

void GM_MergeExternalSong(void *theExternalSong, XShortResourceID theSongID, GM_Song *theSong);

// Walk through the current queue and invalidate all events that are associated to a specific
// GM_Song.
void GM_KillSongEventsFromQueue(GM_Song *pSong);

// Return TRUE if there are events pending for the passed in song.
XBOOL GM_AreEventsPending(GM_Song *pSong);

/**************************************************/
/*
** FUNCTION SongTicks;
**
** Overvue --
**  This will return in 1/60th of a second, the count since the start of the song
**  currently playing.
**
**  INPUT   --  
**  OUTPUT  --  XSDWORD,        returns ticks since BeginSong.
**                      returns 0 if no song is playing.
**
** NOTE:    
*/
/**************************************************/
XDWORD GM_SongTicks(GM_Song *pSong);

// Return the length in MIDI ticks of the song passed
//  pSong   GM_Song structure. Data will be cloned for this function.
//  pErr        OPErr error type
XDWORD GM_GetSongTickLength(GM_Song *pSong, OPErr *pErr);
OPErr GM_SetSongTickPosition(GM_Song *pSong, XDWORD songTickPosition);

// scan through song and build data structure with all program changes
OPErr GM_GetSongInstrumentChanges(void *theSongResource, GM_Song **outSong, XBYTE **outTrackNames);

XDWORD GM_SongMicroseconds(GM_Song *pSong);
XDWORD GM_GetSongMicrosecondLength(GM_Song *pSong, OPErr *pErr);
// Set the song position in microseconds
OPErr GM_SetSongMicrosecondPosition(GM_Song *pSong, XDWORD songMicrosecondPosition);


// Get current audio time stamp based upon the audio built interrupt
XDWORD GM_GetSyncTimeStamp(void);

// Get current audio time stamp in microseconds; this is the
// microseconds' worth of samples that have passed through the
// audio device.  it never decreases.
XDWORD GM_GetDeviceTimeStamp(void);

// Update count of samples played.  This function caluculates from number of bytes,
// given the sample frame size from the mixer variables, and the bytes of data written
void GM_UpdateSamplesPlayed(unsigned long currentPos);

// Get current audio time stamp based upon the audio built interrupt, but ahead in time 
// and quantized for the particular OS
XDWORD GM_GetSyncTimeStampQuantizedAhead(void);

// Get current number of samples played; this is the
// number of samples that have passed through the
// audio device.  it never decreases.
XDWORD GM_GetSamplesPlayed(void);

// Return the used patch array of instruments used in the song passed.
//  theExternalSong standard SONG resource structure
//  theExternalMidiData if not NULL, then will use this midi data rather than what is found in external SONG resource
//  midiSize            size of midi data if theExternalMidiData is not NULL
//  pInstrumentArray    array, if not NULL will be filled with the instruments that need to be loaded.
//  pErr                pointer to an OPErr
XSDWORD GM_GetUsedPatchlist(void *theExternalSong, void *theExternalMidiData, long midiSize, 
                    XShortResourceID *pInstrumentArray, OPErr *pErr);

// set key velocity curve type
void GM_SetVelocityCurveType(GM_Song *pSong, VelocityCurveType velocityCurveType);

/**************************************************/
/*
** FUNCTION IsSongDone;
**
** Overvue --
**  This will return a XBOOL if a song is done playing or not.
**
**  INPUT   --  
**  OUTPUT  --  XBOOL,  returns TRUE if song is done playing,
**                          or FALSE if not playing.
**
** NOTE:    
*/
/**************************************************/
XBOOL GM_IsSongDone(GM_Song *pSong);

// Mute and unmute tracks (0 to 64)
void GM_MuteTrack(GM_Song *pSong, short int track);
void GM_UnmuteTrack(GM_Song *pSong, short int track);
// Get mute status of all tracks. pStatus should be an array of 65 bytes
void GM_GetTrackMuteStatus(GM_Song *pSong, char *pStatus);

void GM_SoloTrack(GM_Song *pSong, short int track);
void GM_UnsoloTrack(GM_Song *pSong, short int track);
// will write only MAX_TRACKS bytes for MAX_TRACKS Midi tracks
void GM_GetTrackSoloStatus(GM_Song *pSong, char *pStatus);

// Mute and unmute channels (0 to 15)
void GM_MuteChannel(GM_Song *pSong, short int channel);
void GM_UnmuteChannel(GM_Song *pSong, short int channel);
// Get mute status of all channels. pStatus should be an array of 16 bytes
void GM_GetChannelMuteStatus(GM_Song *pSong, char *pStatus);

void GM_SoloChannel(GM_Song *pSong, short int channel);
void GM_UnsoloChannel(GM_Song *pSong, short int channel);
void GM_GetChannelSoloStatus(GM_Song *pSong, char *pStatus);

/*************************************************/
/*
** FUNCTION SetMasterVolume;
**
** Overvue --
**  This will set the master output volume of the mixer.
**
**  INPUT   --  theVolume,  0-256 which is the master volume.
**  OUTPUT  --  
**
** NOTE:    
**  This is different that the hardware volume. This will scale the output by
**  theVolume factor.
**  There is somewhat of a CPU hit, while calulating the new scale buffer.
*/
/*************************************************/
void GM_SetMasterVolume(XSDWORD theVolume);
XSDWORD GM_GetMasterVolume(void);


// This is an active voice reference that represents a valid/active voice.
// Used in various functions that need to return and reference a voice.
#define DEAD_VOICE          (void *)-1L             // this represents a dead or invalid voice
typedef void *              VOICE_REFERENCE;        // reference returned that is a allocated active voice
#define DEAD_LINKED_VOICE   (void *)0L              // this represents a dead or invalid voice
typedef void *              LINKED_VOICE_REFERENCE; // this represents a series of linked VOICE_REFERENCE's

/**************************************************/
/*
**  The functions:
**          GM_SetupSample, GM_SetupSampleDoubleBuffer, GM_SetupSampleFromInfo
**
**          Overall these functions are the same. They take various parmeters
**          to allocate a voice. The VOICE_REFERENCE then can be used to start
**          the voice playing and to modify various realtime parmeters.
**
**          Keep in mind that the VOICE_REFERENCE is an active voice in the mixer. 
**          You can run out of active voices. The total number is allocate when you
**          call GM_InitGeneralSound.
**
**          If the GM_Setup... functions to return DEAD_VOICE this means that
**          the total number of voices available from the mixer have all been allocated.
**          The functions do NOT steal voices, and do NOT allocate from MIDI voices.
*/
/**************************************************/
VOICE_REFERENCE GM_SetupSample(XPTR theData, XDWORD startFrame, XDWORD frames, XFIXED theRate, 
                        XDWORD startLoopFrame, XDWORD endLoopFrame, XDWORD theLoopTarget,
                        XSDWORD sampleVolume, XSDWORD stereoPosition,
                        void *context, XSWORD bitSize, XSWORD channels, 
                        GM_LoopDoneCallbackPtr theLoopContinueProc,
                        GM_SoundDoneCallbackPtr theCallbackProc);

// given a VOICE_REFERENCE returned from GM_Begin... this will tell return TRUE, if voice is
// valid
XBOOL GM_IsSoundReferenceValid(VOICE_REFERENCE reference);

VOICE_REFERENCE GM_SetupSampleDoubleBuffer(XPTR pBuffer1, XPTR pBuffer2, XDWORD theSize, XFIXED theRate,
                            XSWORD bitSize, XSWORD channels,
                            XSDWORD sampleVolume, XSWORD stereoPosition,
                            void *context,
                            GM_DoubleBufferCallbackPtr bufferCallback,
                            GM_SoundDoneCallbackPtr doneCallbackProc);

VOICE_REFERENCE GM_SetupSampleFromInfo(GM_Waveform *pSample, void *context,
                                XSDWORD sampleVolume, XSDWORD stereoPosition,
                                GM_LoopDoneCallbackPtr theLoopContinueProc,
                                GM_SoundDoneCallbackPtr theCallbackProc,
                                XDWORD startOffsetFrame);

// set all the voices you want to start at the same time the same syncReference. Then call GM_SyncStartSample
// to start the sync start. Will return an error if its an invalid reference, or syncReference is NULL.
OPErr GM_SetSyncSampleStartReference(VOICE_REFERENCE reference, void *syncReference);
// Once you have called GM_SetSyncSampleStartReference on all the voices, this will set them to start at the next
// mixer slice. Will return an error if its an invalid reference, or syncReference is NULL.
OPErr GM_SyncStartSample(VOICE_REFERENCE reference);
// after voice is setup, call this to start it playing now. returns 0 if started
OPErr GM_StartSample(VOICE_REFERENCE reference);

GM_Waveform *GM_NewWaveform(void);
void GM_FreeWaveform(GM_Waveform *pWaveform);

void GM_SetSampleDoneCallback(VOICE_REFERENCE reference, GM_SoundDoneCallbackPtr theCallbackProc, void *context);

// This will end the sample by the reference number that is passed.
void GM_EndSample(VOICE_REFERENCE reference);

// Returns TRUE, if voice is being rendered
XBOOL GM_IsSampleProcessing(VOICE_REFERENCE reference);

// This will return status of a sound that is being played.
XBOOL GM_IsSoundDone(VOICE_REFERENCE reference);

void GM_ChangeSamplePitch(VOICE_REFERENCE reference, XFIXED theNewRate);
XFIXED GM_GetSamplePitch(VOICE_REFERENCE reference);

void GM_SetSampleRouteBus(VOICE_REFERENCE reference, short int routeBus);

void GM_ChangeSampleVolume(VOICE_REFERENCE reference, XSWORD newVolume);
XSWORD GM_GetSampleVolumeUnscaled(VOICE_REFERENCE reference);
XSWORD GM_GetSampleVolume(VOICE_REFERENCE reference);
void GM_SetSampleFadeRate(VOICE_REFERENCE reference, XFIXED fadeRate,
                            XSWORD minVolume, XSWORD maxVolume, XBOOL endSample);

// get/set frequency filter amount. Range is 512 to 32512
short int GM_GetSampleFrequencyFilter(VOICE_REFERENCE reference);
void GM_SetSampleFrequencyFilter(VOICE_REFERENCE reference, short int frequency);

// get/set resonance filter amount. Range is 0 to 256
short int GM_GetSampleResonanceFilter(VOICE_REFERENCE reference);
void GM_SetSampleResonanceFilter(VOICE_REFERENCE reference, short int resonance);

// get/set low pass filter amount. Range is -255 to 255
short int GM_GetSampleLowPassAmountFilter(VOICE_REFERENCE reference);
void GM_SetSampleLowPassAmountFilter(VOICE_REFERENCE reference, short int amount);

void GM_SetSampleLoopPoints(VOICE_REFERENCE reference, unsigned long start, unsigned long end);

OPErr   GM_SetWaveformLoopPoints(GM_Waveform *pWave, unsigned long start, unsigned long end);
OPErr   GM_GetWaveformLoopPoints(GM_Waveform *pWave, unsigned long *outStart, unsigned long *outEnd);

OPErr   GM_SetWaveformByteSize(GM_Waveform *pWave, XDWORD byteSize);
OPErr   GM_GetWaveformByteSize(GM_Waveform *pWave, XDWORD *outByteSize);

OPErr   GM_SetWaveformNumFrames(GM_Waveform *pWave, XDWORD numFrames);
OPErr   GM_GetWaveformNumFrames(GM_Waveform *pWave, XDWORD *outNumFrames);

OPErr   GM_SetWaveformBitDepth(GM_Waveform *pWave, XWORD bitDepth);
OPErr   GM_GetWaveformBitDepth(GM_Waveform *pWave, XWORD *outBitDepth);

OPErr   GM_SetWaveformNumChannels(GM_Waveform *pWave, XWORD numChannels);
OPErr   GM_GetWaveformNumChannels(GM_Waveform *pWave, XWORD *outNumChannels);

OPErr   GM_SetWaveformSampleRate(GM_Waveform *pWave, XFIXED sampleRate);
OPErr   GM_GetWaveformSampleRate(GM_Waveform *pWave, XFIXED *outSampleRate);

OPErr   GM_SetWaveformBaseMidiPitch(GM_Waveform *pWave, XWORD baseMidiPitch);
OPErr   GM_GetWaveformBaseMidiPitch(GM_Waveform *pWave, XWORD *outBaseMidiPitch);

OPErr   GM_SetWaveformSampleData(GM_Waveform *pWave, XPTR sampleData);
OPErr   GM_GetWaveformSampleData(GM_Waveform *pWave, XPTR *outSampleData);

unsigned long GM_GetSampleStartTimeStamp(VOICE_REFERENCE reference);

// given a valid voice, return the current playback position
unsigned long GM_GetSamplePlaybackPosition(VOICE_REFERENCE reference);
// given a valid voice, set the position of the current playback
OPErr GM_SetSamplePlaybackPosition(VOICE_REFERENCE reference, unsigned long framePos);

void * GM_GetSamplePlaybackPointer(VOICE_REFERENCE reference, unsigned long *outFrameLength);

void GM_ChangeSampleStereoPosition(VOICE_REFERENCE reference, XSWORD newStereoPosition);
XSWORD GM_GetSampleStereoPosition(VOICE_REFERENCE reference);

#if REVERB_USED != REVERB_DISABLED
// return the current amount of reverb mix. 0-127 is the range.
short int GM_GetSampleReverbAmount(VOICE_REFERENCE reference);
// set amount of reverb to mix. 0-127 is the range.
void GM_SetSampleReverbAmount(VOICE_REFERENCE reference, short int amount);
// change status of reverb. Force on, or off
void GM_ChangeSampleReverb(VOICE_REFERENCE reference, XBOOL enable);
// Get current status of reverb. On or off
XBOOL GM_GetSampleReverb(VOICE_REFERENCE reference);
#endif

void GM_SetSampleOffsetCallbackLinks(VOICE_REFERENCE reference, GM_SampleCallbackEntry *pTopEntry);
void GM_AddSampleOffsetCallback(VOICE_REFERENCE reference, GM_SampleCallbackEntry *pEntry);
void GM_RemoveSampleOffsetCallback(VOICE_REFERENCE reference, GM_SampleCallbackEntry *pEntry);

// This will end all sound effects from the system.  It does not shut down sound hardware 
// or deallocate memory used by the music system.
void GM_EndAllSamples(void);
void GM_EndAllNotes(void);
void GM_KillAllNotes(void);
// kills notes that use a particular instrument. The instrument passed is the combined bank/program
// instrument.
void GM_KillSongInstrument(GM_Song *pSong, XLongResourceID instrument);

void GM_EndSongNotes(GM_Song *pSong);
void GM_KillSongNotes(GM_Song *pSong);
// stop notes for a song and channel passed. This will put the note into release mode.
void GM_EndSongChannelNotes(GM_Song *pSong, short int channel);

/**************************************************/
/*
** FUNCTION SetMasterSongTempo(XSDWORD newTempo);
**
** Overvue --
**  This will set the master tempo for the currently playing song.
**
**  INPUT   --  newTempo is in microseconds per MIDI quater-note.
**              Another way of looking at it is, 24ths of a microsecond per
**              MIDI clock.
**  OUTPUT  --  NO_SONG_PLAYING is returned if there is no song playing
**
** NOTE:    
*/
/**************************************************/
OPErr GM_SetMasterSongTempo(GM_Song *pSong, XFIXED newTempo);
XFIXED GM_GetMasterSongTempo(GM_Song *pSong);

// Sets tempo in microsecond per quarter note
void GM_SetSongTempo(GM_Song *pSong, XDWORD newTempo);
// returns tempo in microsecond per quarter note
XDWORD GM_GetSongTempo(GM_Song *pSong);

// sets tempo in beats per minute
void GM_SetSongTempInBeatsPerMinute(GM_Song *pSong, XDWORD newTempoBPM);
// returns tempo in beats per minute
XDWORD GM_GetSongTempoInBeatsPerMinute(GM_Song *pSong);

// Instrument API
// Given an instrument number from 0 to MAX_INSTRUMENTS*MAX_BANKS, this will load that instrument into the musicvars globals, including
// splits. The instrument is assumed to be the real instrument. No remaps or aliases are taken care of here
OPErr GM_LoadInstrument(GM_Song *pSong,
                        XLongResourceID instrument,
                        XBankToken bankToken);
OPErr GM_LoadInstrumentFromExternalData(GM_Song *pSong,
                                        XLongResourceID instrument,
                                        XBankToken bankToken,
                                        void *theX,
                                        UINT32 theXPatchSize);
XBOOL   GM_AnyStereoInstrumentsLoaded(GM_Song *pSong);


// Given an instrument number from 0 to MAX_INSTRUMENTS*MAX_BANKS, this will unload that instrument including
// splits. The instrument is assumed to be the real instrument. No remaps or aliases are taken care of here
// Can return STILL_PLAYING if instrument fails to unload
OPErr   GM_UnloadInstrument(GM_Song *pSong, XLongResourceID instrument);

OPErr   GM_SetupSongRemaps(GM_Song *pSong, XBOOL checkForAliases);
OPErr   GM_GetSongInstrumentRemap(GM_Song *pSong, XLongResourceID fromInstrument, XLongResourceID *pToInstrument);
OPErr   GM_RemapInstrument(GM_Song *pSong, XLongResourceID from, XLongResourceID to);

//// Pass TRUE to cache samples, and share them. FALSE to create new copy for each sample
//void GM_SetCacheSamples(GM_Song *pSong, XBOOL cacheSamples);
//XBOOL GM_GetCacheSamples(GM_Song *pSong);

// returns TRUE if instrument is loaded, FALSE if otherwise
XBOOL   GM_IsInstrumentLoaded(GM_Song *pSong, XLongResourceID instrument);

/**************************************************/
/*
** FUNCTION GM_LoadSongInstruments(GM_Song *theSong)
**
** Overvue --
**  This will load the instruments required for this song to play
**
**  INPUT   --  theSong,    a pointer to the GM_Song data containing the MIDI Data
**          --  pArray, an array that will be filled with the instruments that are
**                      loaded, if not NULL.
**          --  loadInstruments, if TRUE will load instruments and samples
**
**  OUTPUT  --  OPErr,      an error will be returned if the song
**                          cannot be loaded
**
** NOTE:    
*/
/**************************************************/
// Will follow remaps or instrument aliases.
OPErr GM_LoadSongInstruments(GM_Song *theSong,
                             XShortResourceID *pArray,
                             XBankToken bankToken,
                             XBOOL loadInstruments);

// Will unload all instruments assoicated to this song. Will follow remaps or instrument aliases.
// can return STILL_PLAYING if instruments are still in process. Call again to clear
OPErr   GM_UnloadSongInstruments(GM_Song *theSong);

// Will load an instrument to this song. Will follow remaps or instrument aliases.
OPErr GM_LoadSongInstrument(GM_Song *pSong,
                            XLongResourceID instrument,
                            XBankToken bankToken);

// Will unload an instrument from this song. Will follow remaps or instrument aliases.
// can return STILL_PLAYING if instruments are still in process. Call again to clear
OPErr   GM_UnloadSongInstrument(GM_Song *pSong, XLongResourceID instrument);

// Will check to see if an instrument is loaded into this song. Will follow remaps or instrument aliases.
// Returns TRUE if instrument is loaded, otherwise FALSE
XBOOL GM_IsSongInstrumentLoaded(GM_Song *pSong, XLongResourceID instrument);

// Will check to see if an instrument has got a remap.
// Returns TRUE if instrument is remapped, otherwise FALSE
XBOOL   GM_IsSongInstrumentRemapped(GM_Song *pSong, XLongResourceID instrument);

void    GM_SetSongLoopFlag(GM_Song *theSong, XBOOL loopSong);
XBOOL   GM_GetSongLoopFlag(GM_Song *theSong);

// pass TRUE to enabled meta loops for a song, FALSE to not loop
void    GM_SetSongMetaLoopFlag(GM_Song *theSong, XBOOL loopSong);
// return the meta loop status for a song
XBOOL   GM_GetSongMetaLoopFlag(GM_Song *theSong);

short int GM_GetSongLoopMax(GM_Song *theSong);
void    GM_SetSongLoopMax(GM_Song *theSong, short int maxLoopCount);

short int GM_GetChannelVolume(GM_Song *theSong, short int channel);
void    GM_SetChannelVolume(GM_Song *theSong, short int channel, short int volume, XBOOL updateNow);

OPErr   GM_SetDisposeSongDataWhenDoneFlag(GM_Song *pSong, XBOOL disposeData);
OPErr   GM_GetDisposeSongDataWhenDoneFlag(GM_Song *pSong, XBOOL *outDisposeData);

OPErr   GM_ChangeSongVoices(GM_Song *pSong, INT16 maxSongVoices, INT16 mixLevel, INT16 maxEffectVoices);
OPErr   GM_GetSongVoices(GM_Song *pSong, INT16 *pMaxSongVoices, INT16 *pMixLevel, INT16 *pMaxEffectVoices);

#if REVERB_USED != REVERB_DISABLED
// set reverb of a channel of a current song. If updateNow is active and the song is playing
// the voice will up updated
void    GM_SetChannelReverb(GM_Song *theSong, short int channel, XBYTE reverbAmount, XBOOL updateNow);

// Given a song and a channel, this will return the current reverb level
short int GM_GetChannelReverb(GM_Song *theSong, short int channel);
#endif

// Given a song and a new volume set/return the master volume of the song
// Range is 0 to 127. You can overdrive
void    GM_SetSongVolume(GM_Song *theSong, XSWORD newVolume);
XSWORD  GM_GetSongVolume(GM_Song *theSong);

// set/get song position. Range is MAX_PAN_LEFT to MAX_PAN_RIGHT
void GM_SetSongStereoPosition(GM_Song *theSong, XSWORD newStereoPosition);
XSWORD GM_GetSetStereoPosition(GM_Song *theSong);

void    GM_SetSongFadeRate(GM_Song *pSong, XFIXED fadeRate,
                            XSWORD minVolume, XSWORD maxVolume, XBOOL endSong);


// range is 0 to MAX_MASTER_VOLUME (256)
void    GM_SetEffectsVolume(XSWORD newVolume);
XSWORD  GM_GetEffectsVolume(void);

XBOOL GM_IsInstrumentRangeUsed(GM_Song *pSong, XLongResourceID thePatch, XSWORD theLowKey, XSWORD theHighKey);
XBOOL GM_IsInstrumentUsed(GM_Song *pSong, XLongResourceID thePatch, XSWORD theKey);
void GM_SetUsedInstrumentRange(GM_Song *pSong, XLongResourceID thePatch, int start, int end, XBOOL used);
void GM_SetUsedInstrument(GM_Song *pSong, XLongResourceID thePatch, XSWORD theKey, XBOOL used);
void GM_SetInstrumentUsedRange(GM_Song *pSong, XLongResourceID thePatch, XSBYTE *pUsedArray);
void GM_GetInstrumentUsedRange(GM_Song *pSong, XLongResourceID thePatch, XSBYTE *pUsedArray);

// resets the tempo to the inital default state
void GM_ResetTempoToDefault(GM_Song *pSong);

void GM_SetSongCallback(GM_Song *theSong, GM_SongCallbackProcPtr songEndCallbackPtr, void *reference);
void GM_SetSongTimeCallback(GM_Song *theSong, GM_SongTimeCallbackProcPtr songTimeCallbackPtr, long reference);
void GM_SetSongMetaEventCallback(GM_Song *theSong, GM_SongMetaCallbackProcPtr theCallback, long reference);

void GM_SetControllerCallback(GM_Song *theSong, void * reference, GM_ControlerCallbackPtr controllerCallback, short int controller);

// Display
XSWORD GM_GetAudioSampleFrame(XSWORD *pLeft, XSWORD *pRight);

// This will check active voices and look at a sub sample of the audio output to
// determine if there's any audio still playing
XBOOL GM_IsAudioActive(void);

typedef enum
{
    MIDI_PCM_VOICE = 0,     // Voice is a PCM voice used by MIDI
    SOUND_PCM_VOICE         // Voice is a PCM sound effect used by BAESound/BAESoundStream
} GM_VoiceType;

struct GM_AudioInfo
{
    XSWORD          maxNotesAllocated;
    XSWORD          maxEffectsAllocated;
    XSWORD          mixLevelAllocated;
    XSWORD          voicesActive;               // number of voices active
    XLongResourceID patch[MAX_VOICES];          // current patches (program, and bank)
    XSWORD          volume[MAX_VOICES];         // current volumes
    XSWORD          scaledVolume[MAX_VOICES];   // current scaled volumes
    XSWORD          channel[MAX_VOICES];        // current channel
    XSWORD          midiNote[MAX_VOICES];       // current midi note
    XSWORD          voice[MAX_VOICES];          // voice index
    GM_VoiceType    voiceType[MAX_VOICES];      // voice type
    GM_Song         *pSong[MAX_VOICES];         // song associated with voice
};
typedef struct GM_AudioInfo GM_AudioInfo;

void GM_GetRealtimeAudioInformation(GM_AudioInfo *pInfo);


/** Standard Midi constants.
 */
enum
{
    
    /** MIDI status commands most significant bit is 1 */
    B_NOTE_OFF                  =   0x80,
    B_NOTE_ON                   =   0x90,
    B_POLY_AFTERTOUCH           =   0xA0,
    B_CONTROL_CHANGE            =   0xB0,
    B_PROGRAM_CHANGE            =   0xC0,
    B_CHANNEL_AFTERTOUCH        =   0xD0,
    B_PITCH_BEND                =   0xE0,
    B_SYSTEM_EXCLUSIVE          =   0xF0,
    B_SYSTEM_EXCLUSIVE_CONT     =   0xF7,
    
    /** Controller values. Obtained from:
     http://www.midi.org/about-midi/table3.shtml
     */
    B_BANK_MSB                  =   0,
    B_MODULATION_MSB            =   1,
    B_DATA_MSB                  =   6,
    B_VOLUME_MSB                =   7,
    B_BALANCE_MSB               =   8,
    B_PAN_MSB                   =   10,
    B_EXPRESSION_MSB            =   11,
    
    B_BANK_LSB                  =   32,
    B_MODULATION_LSB            =   33,
    B_DATA_LSB                  =   38,
    B_VOLUME_LSB                =   39,
    B_BALANCE_LSB               =   40,
    B_PAN_LSB                   =   42,
    B_EXPRESSION_LSB            =   43,
    
    B_SUSTAIN                   =   64,
    B_SOFT_PEDAL                =   67,

    B_REVERB_TYPE               =   90, // non-standard
    B_REVERB_SEND               =   91,
    B_TREMOLO_LEVEL             =   92,
    B_CHROUS_SEND_LEVEL         =   93,
    B_DETUNE_DEPTH              =   94,
    B_PHASER_DEPTH              =   95,

    B_INCREMENT_DATA            =   96,
    B_DECREMENT_DATA            =   97,
    B_NRPN_LSB                  =   98,
    B_NRPN_MSB                  =   99,
    B_RPN_LSB                   =   100,
    B_RPN_MSB                   =   101,
    
    B_ALL_NOTES_OFF_CHANNEL     =   120,
    B_RESET_ALL_CONTROLLERS     =   121,
    B_ALL_NOTES_OFF             =   123,
    
    /** Midi specific RPN's 
     To set or change the value of a Registered Parameter:
     
     1. Send two Control Change messages using Control Numbers 101
     (65H)  and 100 (64H) to select the desired Registered Parameter
     Number, as per  the following table.
     
     2. To set the selected Registered Parameter to a specific value,
     send  a Control Change messages to the Data Entry MSB controller
     (Control  Number 6). If the selected Registered Parameter
     requires the LSB to be  set, send another Control Change message
     to the Data Entry LSB  controller (Control Number 38).
     
     3. To make a relative adjustment to the selected Registered 
     Parameter's current value, use the Data Increment or Data
     Decrement  controllers (Control Numbers 96 and 97).
     */
    /** MSB = +/- semitones LSB =+/--cents */
    B_RPN_PITCH_BEND_SENSITIVITY_LSB    =   0,
    B_RPN_PITCH_BEND_SENSITIVITY_MSB    =   0,
    
    
    /** Standard MIDI Files meta event definitions */
    B_META_EVENT                =   0xFF,
};


// External MIDI links
#define Q_GET_TICK  0L      // if you pass this constant for timeStamp it will get the current
                            // tick
void QGM_NoteOn(GM_Song *pSong, XDWORD timeStamp, XSWORD channel, XSWORD note, XSWORD velocity);
void QGM_NoteOff(GM_Song *pSong, XDWORD timeStamp, XSWORD channel, XSWORD note, XSWORD velocity);
void QGM_ProgramChange(GM_Song *pSong, XDWORD timeStamp, XSWORD channel, XSWORD program);
void QGM_PitchBend(GM_Song *pSong, XDWORD timeStamp, XSWORD channel, XBYTE valueMSB, XBYTE valueLSB);
void QGM_Controller(GM_Song *pSong, XDWORD timeStamp, XSWORD channel, XSWORD controller, XSWORD value);
void QGM_AllNotesOff(GM_Song *pSong, XDWORD timeStamp);
void QGM_LockExternalMidiQueue(void);
void QGM_UnlockExternalMidiQueue(void);

// External MIDI Links using a GM_Song. Will be unstable unless called via GM_AudioTaskCallbackPtr
void GM_NoteOn(GM_Song *pSong, XSWORD channel, XSWORD note, XSWORD velocity);
void GM_NoteOff(GM_Song *pSong, XSWORD channel, XSWORD note, XSWORD velocity);
void GM_ProgramChange(GM_Song *pSong, XSWORD channel, XSWORD program);
void GM_PitchBend(GM_Song *pSong, XSWORD channel, XBYTE valueMSB, XBYTE valueLSB);
void GM_Controller(GM_Song *pSong, XSWORD channel, XSWORD controller, XSWORD value);
void GM_AllNotesOff(GM_Song *pSong);

// mixer callbacks and tasks
void GM_SetAudioTask(GM_AudioTaskCallbackPtr pTaskProc, void *taskReference);
void GM_SetAudioOutput(GM_AudioOutputCallbackPtr pOutputProc);
GM_AudioOutputCallbackPtr GM_GetAudioOutput(void);
GM_AudioTaskCallbackPtr GM_GetAudioTask(void);

long GM_GetAudioBufferOutputSize(void);

#if USE_HIGHLEVEL_FILE_API == TRUE
typedef enum
{
    FILE_INVALID_TYPE = 0,
    FILE_AIFF_TYPE = 1,
    FILE_WAVE_TYPE,
    FILE_AU_TYPE
#if USE_MPEG_DECODER != FALSE
    ,
    FILE_MPEG_TYPE
#endif
} AudioFileType;

// This will read into memory the entire file and return a GM_Waveform structure.
// To dispose of a GM_Waveform structure, call GM_FreeWaveform
GM_Waveform * GM_ReadFileIntoMemory(XFILENAME *file, AudioFileType fileType, XBOOL decodeData, OPErr *pErr);

// write memory to a file
OPErr GM_WriteFileFromMemory(XFILENAME *file,
                                GM_Waveform const* pAudioData, AudioFileType fileType);

OPErr GM_WriteAudioBufferToFile(XFILE file, AudioFileType type, void *buffer, long size, long channels, long sampleSize);

// fill in empty fields in the file header.
OPErr GM_FinalizeFileHeader(XFILE file, AudioFileType fileType);

// This will read from a block of memory an entire file and return a GM_Waveform structure.
// Assumes that the block of memory is formatted as a fileType
// To dispose of a GM_Waveform structure, call GM_FreeWaveform
GM_Waveform* GM_ReadFileIntoMemoryFromMemory(void *pFileBlock, unsigned long fileBlockSize, 
                                                AudioFileType fileType,
                                                XBOOL decodeSamples, OPErr *pErr);

GM_Waveform * GM_ReadRawAudioIntoMemoryFromMemory(void *sampleData,         // pointer to audio data
                                                    unsigned long frames,       // number of frames of audio
                                                    unsigned short int bitSize, // bits per sample 8 or 16
                                                    unsigned short int channels,// mono or stereo 1 or 2
                                                    XFIXED rate,                // 16.16 fixed sample rate
                                                    unsigned long loopStart,    // loop start in frames
                                                    unsigned long loopEnd,      // loop end in frames
                                                    OPErr *pErr);

// This will read into memory just the information about the file and return a 
// GM_Waveform structure.
// Read file information from file, which is a fileType file. If pFormat is not NULL, then
// store format specific format type
// To dispose of a GM_Waveform structure, call GM_FreeWaveform
// If pBlockPtr is a void *, then allocate a *pBlockSize buffer, otherwise do nothing
GM_Waveform * GM_ReadFileInformation(XFILENAME *file, AudioFileType fileType, long *pFormat, 
                                    void **pBlockPtr, unsigned long *pBlockSize, OPErr *pErr);

// functions used with GM_ReadAndDecodeFileStream to preserve state between decode calls.
void * GM_CreateFileState(AudioFileType fileType);
void GM_DisposeFileState(AudioFileType fileType, void *state);

// Read a block of data, based apon file type and format, decode and store into a buffer.
// Return length of buffer stored and return an OPErr. NO_ERR if successfull.
OPErr GM_ReadAndDecodeFileStream(XFILE fileReference, 
                                        AudioFileType fileType, long format, 
                                        XPTR pBlockBuffer, unsigned long blockSize,
                                        XPTR pBuffer, unsigned long bufferLength,
                                        short int channels, short int bitSize,
                                        unsigned long *pStoredBufferLength,
                                        unsigned long *pReadBufferLength);

// given an open file, format types, and a sample position in frames, reseek the file
OPErr GM_RepositionFileStream(XFILE fileReference,
                                        AudioFileType fileType, long format,
                                        XPTR pBlockBuffer, unsigned long blockSize,
                                        short int channels, short int bitSize,
                                        unsigned long newSampleFramePosition,
                                        unsigned long firstSampleInFileOffsetInBytes,
                                        unsigned long *pOuputNewFilePosition);

#endif  // USE_HIGHLEVEL_FILE_API

#if USE_STREAM_API == TRUE
// Audio Sample Data Format. (ASDF)
// Support for 8, 16 bit data, mono and stereo. Can be extended for multi channel beyond 2 channels, but
// not required at the moment.
//
//  DATA BLOCK
//      8 bit mono data
//          ZZZZZZZ...
//              Where Z is signed 8 bit data
//
//      16 bit mono data
//          WWWWW...
//              Where W is signed 16 bit data
//
//      8 bit stereo data
//          ZXZXZXZX...
//              Where Z is signed 8 bit data for left channel, and X is signed 8 bit data for right channel.
//
//      16 bit stereo data
//          WQWQWQ...
//              Where W is signed 16 bit data for left channel, and Q is signed 16 bit data for right channel.
//



typedef enum
{
    STREAM_CREATE               =   1,
    STREAM_DESTROY,
    STREAM_GET_DATA,
    STREAM_GET_SPECIFIC_DATA,
    STREAM_HAVE_DATA,
    STREAM_SET_POSITION,

    // $$kk: 09.21.98: i am adding these 
    STREAM_START,
    STREAM_STOP,
    STREAM_EOM
} GM_StreamMessage;

// The GM_StreamObjectProc callback is called to allocate buffer memory, get the next block of data to stream and
// mix into the final audio output, and finally dispose of the memory block. All messages will happen at 
// non-interrupt time. The structure GM_StreamData will be passed into all messages.
//
// INPUT:
// Message
//  STREAM_CREATE
//      Use this message to create a block a data with a length of (dataLength). Keep in mind that dataLength
//      is always total number of samples,  not bytes allocated. Allocate the block of data into the Audio Sample 
//      Data Format based upon (dataBitSize) and (channelSize). Store the pointer into (pData).
//
//  STREAM_DESTROY
//      Use this message to dispose of the memory allocated. (pData) will contain the pointer allocated.
//      (dataLength) will be the sample size not the buffer size. ie. for 8 bit data use (dataLength), 
//      for 16 bit mono data double (dataLength).
//
//  STREAM_GET_DATA
//      This message is called whenever the streaming object needs a new block of data. Right after STREAM_CREATE
//      is called, STREAM_GET_DATA will be called twice. Fill (pData) with the new data to be streamed.
//      Set (dataLength) to the amount of data put into (pData). This message is called in a linear fashion.
//
//  STREAM_GET_SPECIFIC_DATA
//      This message is optional. It will be called when a specific sample frame and length needs to be captured.
//      The function GM_AudioStreamGetData will call this message. If you do not want to implement this message
//      return an error of NOT_SETUP. Fill (pData) with the new sample data betweem sample frames (startSample)
//      and (endSample). Set (dataLength) to the amount of data put into (pData).
//      Note: this message will should not be called to retrive sample data for streaming. Its only used to capture
//      a range of data inside of a stream for display or other processes.
//
//
// OUTPUT:
// returns
//  NO_ERR
//      Everythings ok
//
//  STREAM_STOP_PLAY
//      Everything is fine, but stop playing stream
//
//  MEMORY_ERR
//      Couldn't allocate memory for buffers.
//
//  PARAM_ERR
//      General purpose error. Something wrong with parameters passed.
//
//  NOT_SETUP
//      If message STREAM_GET_SPECIFIC_DATA is called and it is not implemented you should return this error.
//

#define DEAD_STREAM 0L              // this represents a dead or invalid stream
typedef long        STREAM_REFERENCE;
typedef long        LINKED_STREAM_REFERENCE;

struct GM_StreamData
{
    STREAM_REFERENCE    streamReference;    // IN for all messages
    long                userReference;      // IN for all messages. userReference is passed in at AudioStreamStart
    void                *pData;             // OUT for STREAM_CREATE, IN for STREAM_DESTROY and STREAM_GET_DATA and STREAM_GET_SPECIFIC_DATA
    unsigned long       dataLength;         // OUT for STREAM_CREATE, IN for STREAM_DESTROY. IN and OUT for STREAM_GET_DATA and STREAM_GET_SPECIFIC_DATA
    XFIXED              sampleRate;         // IN for all messages. Fixed 16.16 value
    char                dataBitSize;        // IN for STREAM_CREATE only.
    char                channelSize;        // IN for STREAM_CREATE only.
    unsigned long       startSample;        // IN for STREAM_GET_SPECIFIC_DATA only.
    unsigned long       endSample;          // IN for STREAM_GET_SPECIFIC_DATA only.
    unsigned long       framePosition;      // IN for STREAM_SET_POSITION only.
};
typedef struct GM_StreamData    GM_StreamData;


// CALLBACK FUNCTIONS TYPES

// AudioStream object callback
typedef OPErr (*GM_StreamObjectProc)(void *threadContext, GM_StreamMessage message, GM_StreamData *pAS);



// Multi source user config based streaming
// This will setup a streaming audio object.
//
// INPUT:
//  userReference   This is a reference value that will be returned and should be passed along to all AudioStream
//                  functions.
//
//  pProc           is a GM_StreamObjectProc proc pointer. At startup of the streaming the proc will be called
//                  with STREAM_CREATE, then followed by two STREAM_GET_DATA calls to get two buffers of data,
//                  and finally STREAM_DESTROY when finished.
//
// OUTPUT:
//  long            This is an audio stream reference number. Will be 0 if error
STREAM_REFERENCE    GM_AudioStreamSetup(    void *threadContext,                    // platform threadContext
                                    long userReference,             // user reference
                                    GM_StreamObjectProc pProc,      // control callback
                                    unsigned long bufferSize,       // buffer size 
                                    XFIXED sampleRate,          // Fixed 16.16
                                    char dataBitSize,               // 8 or 16 bit data
                                    char channelSize);              // 1 or 2 channels of date

#if USE_HIGHLEVEL_FILE_API
// setup a streaming file
// OUTPUT:
//  long            This is an audio stream reference number. Will be 0 if error
STREAM_REFERENCE    GM_AudioStreamFileSetup(    void *threadContext,                // platform threadContext
                                        XFILENAME *file,            // file name
                                        AudioFileType fileType,     // type of file
                                        unsigned long bufferSize,   // temp buffer to read file
                                        GM_Waveform *pFileInfo,
                                        XBOOL loopFile);            // TRUE will loop file
#endif  // USE_HIGHLEVEL_FILE_API

#if USE_HIGHLEVEL_FILE_API
// Set the loop flag of a audio stream
void GM_AudioStreamSetLoop(STREAM_REFERENCE reference, XBOOL loopFile);
// Get the loop flag of a audio stream
XBOOL GM_AudioStreamGetLoop(STREAM_REFERENCE reference);
// Get the done callback and reference flag of a audio stream
void * GM_AudioStreamGetDoneCallback(STREAM_REFERENCE reference, GM_SoundDoneCallbackPtr *pDoneCallback);
// Set the done callback and reference flag of a audio stream
void GM_AudioStreamSetDoneCallback(STREAM_REFERENCE reference, GM_SoundDoneCallbackPtr doneCallback, void *doneCallbackReference);
#endif

// Get the file position of a audio stream, in samples. This
// value is the current file track position. This does not equal what has
// been played. Typically it will be ahead of real time.
unsigned long GM_AudioStreamGetFileSamplePosition(STREAM_REFERENCE reference);
// Set the file position of a audio stream, in samples
OPErr       GM_AudioStreamSetFileSamplePosition(STREAM_REFERENCE reference, unsigned long framePos);

// get the position of samples played in a stream. This will be as close as
// posible to realtime. Probably off by engine latency.
unsigned long GM_AudioStreamGetPlaybackSamplePosition(STREAM_REFERENCE reference);

// call this to preroll everything and allocate a voice in the mixer, then call
// GM_AudioStreamStart to start it playing
OPErr       GM_AudioStreamPreroll(STREAM_REFERENCE reference);
// once a stream has been setup, call this function
OPErr       GM_AudioStreamStart(STREAM_REFERENCE reference);

// set all the streams you want to start at the same time the same syncReference. Then call GM_SyncAudioStreamStart
// to start the sync start. Will return an error (not NO_ERR) if its an invalid reference, or syncReference is NULL.
OPErr       GM_SetSyncAudioStreamReference(STREAM_REFERENCE reference, void *syncReference);

// Once you have called GM_SetSyncAudioStreamReference on all the streams, this will set them to start at the next
// mixer slice. Will return an error (not NO_ERR) if its an invalid reference, or syncReference is NULL.
OPErr       GM_SyncAudioStreamStart(STREAM_REFERENCE reference);

long        GM_AudioStreamGetReference(STREAM_REFERENCE reference);

OPErr       GM_AudioStreamGetData(  void *threadContext,                    // platform threadContext
                                    STREAM_REFERENCE reference, 
                                    unsigned long startFrame, 
                                    unsigned long stopFrame,
                                    XPTR pBuffer, unsigned long bufferLength);

// This will stop a streaming audio object and free any memory.
//
// INPUT:
//  reference   This is the reference number returned from AudioStreamStart.
//
OPErr       GM_AudioStreamStop(void *threadContext, STREAM_REFERENCE reference);

// This will return the last error of this stream
OPErr       GM_AudioStreamError(STREAM_REFERENCE reference);

// This will stop all streams that are current playing and free any memory.
void        GM_AudioStreamStopAll(void *threadContext);

// This is the streaming audio service routine. Call this as much as possible, but not during an
// interrupt. This is a very quick routine. A good place to call this is in your main event loop.
void        GM_AudioStreamService(void *threadContext);

// Returns TRUE or FALSE if a given AudioStream is still active
XBOOL       GM_IsAudioStreamPlaying(STREAM_REFERENCE reference);

// Returns TRUE if a given AudioStream is valid
XBOOL       GM_IsAudioStreamValid(STREAM_REFERENCE reference);

// Set the volume level of a audio stream
void        GM_AudioStreamSetVolume(STREAM_REFERENCE reference, short int newVolume, XBOOL defer);

// set the volume level of all open streams
void        GM_AudioStreamSetVolumeAll(short int newVolume);

// Get the volume level of a audio stream
short int   GM_AudioStreamGetVolume(STREAM_REFERENCE reference);

// start a stream fading
void        GM_SetAudioStreamFadeRate(STREAM_REFERENCE reference, XFIXED fadeRate, 
                            XSWORD minVolume, XSWORD maxVolume, XBOOL endStream);

// Set the sample rate of a audio stream
void        GM_AudioStreamSetRate(STREAM_REFERENCE reference, XFIXED newRate);

// Get the sample rate of a audio stream
XFIXED      GM_AudioStreamGetRate(STREAM_REFERENCE reference);

// Set the stereo position of a audio stream
void        GM_AudioStreamSetStereoPosition(STREAM_REFERENCE reference, short int stereoPosition);

// Get the stereo position of a audio stream
short int   GM_AudioStreamGetStereoPosition(STREAM_REFERENCE reference);

// Get the offset, in mixer-format samples, between the mixer play 
// position and the stream play position.
unsigned long GM_AudioStreamGetSampleOffset(STREAM_REFERENCE reference);

// Update the number of samples played from each stream, in the
// format of the stream.  delta is given in engine-format samples.
void        GM_AudioStreamUpdateSamplesPlayed(unsigned long delta);

// get number of samples played for this stream
unsigned long GM_AudioStreamGetSamplesPlayed(STREAM_REFERENCE reference);

// $$kk: 08.12.98 merge: added this 
// Drain this stream
void        GM_AudioStreamDrain(void *threadContext, STREAM_REFERENCE reference);

// Get the number of samples actually played through the device
// from this stream, in stream-format samples.
// Flush this stream
void        GM_AudioStreamFlush(STREAM_REFERENCE reference);

// Enable/Disable reverb on this particular audio stream
void        GM_AudioStreamReverb(STREAM_REFERENCE reference, XBOOL useReverb);
XBOOL       GM_AudioStreamGetReverb(STREAM_REFERENCE reference);
// get/set reverb mix level
void        GM_SetStreamReverbAmount(STREAM_REFERENCE reference, short int reverbAmount);
short int   GM_GetStreamReverbAmount(STREAM_REFERENCE reference);

// get/set filter frequency of a audio stream
// Range is 512 to 32512
void        GM_AudioStreamSetFrequencyFilter(STREAM_REFERENCE reference, short int frequency);
short int   GM_AudioStreamGetFrequencyFilter(STREAM_REFERENCE reference);
// get/set filter resonance of a audio stream
// Range is 0 to 256
short int   GM_AudioStreamGetResonanceFilter(STREAM_REFERENCE reference);
void        GM_AudioStreamSetResonanceFilter(STREAM_REFERENCE reference, short int resonance);
// get/set filter low pass amount of a audio stream
// lowPassAmount range is -255 to 255
short int   GM_AudioStreamGetLowPassAmountFilter(STREAM_REFERENCE reference);
void        GM_AudioStreamSetLowPassAmountFilter(STREAM_REFERENCE reference, short int lowPassAmount);


// Pause or resume this particular audio stream
void        GM_AudioStreamResume(STREAM_REFERENCE reference);
void        GM_AudioStreamPause(STREAM_REFERENCE reference);

// Pause or resume all audio streams
void        GM_AudioStreamResumeAll(void);
void        GM_AudioStreamPauseAll(void);

LINKED_STREAM_REFERENCE GM_NewLinkedStreamList(STREAM_REFERENCE reference, void *threadContext);

// Given a top link, deallocates the linked list. DOES NOT deallocate the streams.
void GM_FreeLinkedStreamList(LINKED_STREAM_REFERENCE pTop);

// Given a top link, and a new link this will add to a linked list, and return a new top
// if required.
LINKED_STREAM_REFERENCE GM_AddLinkedStream(LINKED_STREAM_REFERENCE pTop, LINKED_STREAM_REFERENCE pEntry);

// Given a top link and an link to remove this will disconnect the link from the list and
// return a new top if required.
LINKED_STREAM_REFERENCE GM_RemoveLinkedStream(LINKED_STREAM_REFERENCE pTop, LINKED_STREAM_REFERENCE pEntry);

STREAM_REFERENCE GM_GetLinkedStreamPlaybackReference(LINKED_STREAM_REFERENCE pLink);
void * GM_GetLinkedStreamThreadContext(LINKED_STREAM_REFERENCE pLink);

OPErr GM_StartLinkedStreams(LINKED_STREAM_REFERENCE pTop);

// end in unison the samples for all the linked streams
void GM_EndLinkedStreams(LINKED_STREAM_REFERENCE pTop);

// Volume range is from 0 to MAX_NOTE_VOLUME
// set in unison the sample volume for all the linked streams
void GM_SetLinkedStreamVolume(LINKED_STREAM_REFERENCE pTop, XSWORD sampleVolume, XBOOL defer);

// set in unison the sample rate for all the linked streams
void GM_SetLinkedStreamRate(LINKED_STREAM_REFERENCE pTop, XFIXED theNewRate);

// set in unison the sample position for all the linked streams
// range from -63 to 63
void GM_SetLinkedStreamPosition(LINKED_STREAM_REFERENCE pTop, XSWORD newStereoPosition);

#endif  // USE_STREAM_API

// linked samples
// Call one of the GM_SetupSample... functions in the various standard ways, to get an allocate voice
// then call GM_NewLinkedSampleList. Then add it to your maintained top list of linked voices with 
// by calling GM_AddLinkedSample. Use GM_FreeLinkedSampleList to delete an entire list, 
// or GM_RemoveLinkedSample to just one link.
//
// Then you can call GM_StartLinkedSamples to start them all at the same time, or GM_EndLinkedSamples
// to end them all, or GM_SetLinkedSampleVolume, GM_SetLinkedSampleRate, and GM_SetLinkedSamplePosition
// set parameters about them all.

LINKED_VOICE_REFERENCE GM_NewLinkedSampleList(VOICE_REFERENCE reference);

void GM_FreeLinkedSampleList(LINKED_VOICE_REFERENCE reference);

// Given a top link, and a new link this will add to a linked list, and return a new top
// if required.
LINKED_VOICE_REFERENCE GM_AddLinkedSample(LINKED_VOICE_REFERENCE pTop, LINKED_VOICE_REFERENCE pEntry);

// Given a top link and an link to remove this will disconnect the link from the list and
// return a new top if required.
LINKED_VOICE_REFERENCE GM_RemoveLinkedSample(LINKED_VOICE_REFERENCE pTop, LINKED_VOICE_REFERENCE pEntry);

VOICE_REFERENCE GM_GetLinkedSamplePlaybackReference(LINKED_VOICE_REFERENCE pLink);

OPErr GM_StartLinkedSamples(LINKED_VOICE_REFERENCE reference);

// end in unison the samples for all the linked samples
void GM_EndLinkedSamples(LINKED_VOICE_REFERENCE reference);

// Volume range is from 0 to MAX_NOTE_VOLUME
// set in unison the sample volume for all the linked samples
void GM_SetLinkedSampleVolume(LINKED_VOICE_REFERENCE reference, XSWORD sampleVolume);

// set in unison the sample rate for all the linked samples
void GM_SetLinkedSampleRate(LINKED_VOICE_REFERENCE reference, XFIXED theNewRate);

// set in unison the sample position for all the linked samples
// range from -63 to 63
void GM_SetLinkedSamplePosition(LINKED_VOICE_REFERENCE reference, XSWORD newStereoPosition);


#ifdef BAE_MCU
// build next frame. Causes messages to be sent to the DSP. Calls GM_ProcessSyncUpdateFromDSP
void BAE_BuildMCUSlice(void * threadContext, XDWORD dspTime);

// process next frame. Causes messages to be sent to the DSP
OPErr GM_ProcessSyncUpdateFromDSP(XDWORD dspTime);

typedef struct GM_Voice GM_Voice;

// 8 bit, stereo
void GM_DSPMix8SFullBuffer(GM_Voice *this_voice);
void GM_DSPMix8SPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 8 bit, mono
void GM_DSPMix8MFullBuffer(GM_Voice *this_voice);
void GM_DSPMix8MPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 16 bit, stereo
void GM_DSPMix16SFullBuffer(GM_Voice *this_voice);
void GM_DSPMix16SPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 16 bit, mono
void GM_DSPMix16MFullBuffer(GM_Voice *this_voice);
void GM_DSPMix16MPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 8 bit, stereo, filters
void GM_DSPMix8SFilterFullBuffer(GM_Voice *this_voice);
void GM_DSPMix8SFilterPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 8 bit, mono, filters
void GM_DSPMix8MFilterFullBuffer(GM_Voice *this_voice);
void GM_DSPMix8MFilterPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 16 bit, stereo, filters
void GM_DSPMix16SFilterFullBuffer(GM_Voice *this_voice);
void GM_DSPMix16SFilterPartialBuffer(GM_Voice *this_voice, XBOOL looping);

// 16 bit, mono, filters
void GM_DSPMix16MFilterFullBuffer(GM_Voice *this_voice);
void GM_DSPMix16MFilterPartialBuffer(GM_Voice *this_voice, XBOOL looping);



// dsp transfer functions

// given a voice, and samples and the number of samples, send them
// over to the dsp for futhar processing.
// assumes samples are 16 bit.
OPErr GM_SendVoiceSamplesToDSP16(XWORD voice, XSWORD *samples, XWORD channels, XWORD frames);

// given a voice, and samples and the number of samples, send them
// over to the dsp for futhar processing.
// assumes samples are 8 bit.
OPErr GM_SendVoiceSamplesToDSP8(XWORD voice, XBYTE *samples, XWORD channels, XWORD frames);

// update voice given a voice, pos, and amplitude envelope vectors
// no filters only interpolator
OPErr GM_UpdateVoiceOnDSPNoFilter(XWORD voice, XDWORD  waveInc, XDWORD ampEnvL, XDWORD ampEnvR);

// update voice given a voice, pos, and amplitude envelope vectors
// low pass filter and interpolator
OPErr GM_UpdateVoiceOnDSPOnePole(XWORD voice, XDWORD  waveInc, XDWORD ampEnvL, XDWORD ampEnvR,
                                    XDWORD filterTapZero, XDWORD filterTapOne);

// update voice given a voice, pos, and amplitude envelope vectors
// low pass filter and interpolator and resonant delay line
OPErr GM_UpdateVoiceOnDSPDelayLine(XWORD voice, XDWORD  waveInc, XDWORD ampEnvL, XDWORD ampEnvR,
                                    XDWORD filterTapZero, XDWORD filterTapOne,
                                    XDWORD filterTapTwo, XDWORD filterDelayLength);

// init a voice using the given parmeters on the DSP.
OPErr GM_InitVoiceOnDSP(GM_Voice *pVoice);

// given a voice, kill a voice on the DSP.
OPErr GM_KillVoiceOnDSP(GM_Voice *pVoice);

#endif

#ifdef __cplusplus
    }
#endif

#endif /* GenSnd.h */



