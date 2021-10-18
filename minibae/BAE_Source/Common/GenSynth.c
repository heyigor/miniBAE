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
** "GenSynth.c"
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
**  General purpose Music Synthesis software, C-only implementation 
**  (no assembly language optimizations made)
**
** Modification History:
**
**  6/7/93      begin real serious work
**  6/8/93      make a stab at getting MIDI performance to work
**  8/17/93     improved note prioritization
**  8/18/93     Implement complete API; do interpolated versions of loops and amp. scaling
**  8/21/93     Fixed bug with NoteDur processing samples after note has released
**  8/21/93     Fixed bug with NoteNextSize not being setup correctly inside of ServeMidiNote
**  8/22/93     Incorporated Windows changes for Microsoft C++ Compiler
**  8/23/93     Added new parameters to InitGeneralSound
**  8/24/93     Added even more new parameters to InitGeneralSound
**  8/24/93     Verified Cross platform compiled code
**  10/12/94    fixed pitch tables
**  11/7/95     Major changes, revised just about everything.
**  11/14/95    Fixed volume scale problem. Forgot to scale volume based upon song level
**  11/20/95    Removed the BF_ flags, now you must walk through the union structure
**              Forgot to set songVolume
**              Remove bit fields. BIT FIELDS DON'T WORK WITH MPW!!!!
**  11/24/95    Touched up GM_EndSample & GM_IsSoundDone & GM_BeginDoubleBuffer a bit
**   12/95      upgraded mixing bus to 32 bit; improved scaleback resolution; added reverb unit; first pass at volume ADSR
**  12/6/95     Moved function GM_GetAudioSampleFrame into GenSynth.c
**              Added GM_SetReverbType
**  12/7/95     Added reverb enable via channel controlers in PV_StartMIDINote
**              Removed some INTEL stuff
**              Moved DEFAULT_REVERB_TYPE to GENSND.H
**              Added GM_GetReverbType
**              Added some more behavior for note removal for sustaining notes
**  1/4/96      Added GM_ChangeSampleReverb for sound effects
**              Changed behavior for setting sample volumes
**  1/7/96      Changed GM_BeginDoubleBuffer to use a 32 bit value for volume
**              Changed PV_ScaleVolumeFromChannelAndSong to correct bug with sound effects
**              and song volume
**              Changed GM_BeginDoubleBuffer & GM_BeginSample to support effectsVolume
**  1/10/96     Split bulk of init code into GenSetup.c
**  1/18/96     Spruced up for C++ extra error checking
**              Added MIN_LOOP_SIZE
**  1/21/96     Changed references to 'true' & 'false' to TRUE and FALSE
**  1/28/96     Fixed 32 bit value bug in function PV_StartMIDINote
**  1/29/96     Added useSampleRate factor for playback of instruments
**  1/30/96     Fixed useSampleRate in PV_StartMIDINote
**  2/5/96      Removed unused variables. Working towards multiple songs
**  2/11/96     Removed FixedDivide & FixedMultiply. Use XFixedDivide & XFixedMultiply
**  2/13/96     Moved MusicIRQ into its own function, PV_ProcessSequencerEvents, and now called it
**              from ProcessSampleFrame.
**              Renamed MusicIRQ to PV_MusicIRQ;
**  2/18/96     Added pInstrument->panPlacement use in ServeMidiNote
**  3/1/96      Changed static variables to static const
**  3/6/96      Fixed a divide by zero error in PV_ADSRModule
**  4/6/96      Added default velocity translation curve
**  4/20/96     Moved myFixedMultiply into X_API.c and moved all references to XFixedMultiply
**              Moved myFixedDivide into X_API.c and moved all references to XFixedDivide
**  4/21/96     Removed register usage in parameters
**  4/25/96     Removed SwapWord. Use XGetShort instead
**              Added a test for NULL pInstrument in PV_ServeInstrumentCurves
**              Fixed looping sample bug that caused the sample to stop playing after looping for 8.9 minutes!
**  5/19/96     Removed some compiler warnings
**  6/18/96     Changed behavior of GM_EndAllNotes. Now puts playing notes into release mode rather than killing them.
**  7/4/96      Changed font and re tabbed
**              Fixed logLookupTable entry for -3
**  7/5/96      Added GM_KillAllNotes
**  7/8/96      Improved enveloping and wave shaping code
**  7/9/96      Fixed casting problem that created a bug with Windows compilers
**  7/10/96     Fixed reverb unit to be able to be turned off, in PV_StartMIDINote
**  9/25/96     Added GM_EndSongNotes
**  9/27/96     Added more parameters to PV_StartMIDINote & PV_StopMIDINote
**  10/1/96     Fixed GM_KillAllNotes & GM_EndSongNotes & GM_EndAllNotes to terminate notes better
**  10/23/96    Added defines for MOD_WHEEL_CONTROL = 'MODW & SAMPLE_NUMBER = 'SAMP'
**  11/9/96     Added GM_KillSongNotes
**  11/18/96    Added more velocity curves
**  12/3/96     Added 2times linear curve
**  12/4/96     Added 2.5 times linear curve
**  12/6/96     Added cool CBM curve
**  12/10/96    Added yet another CBM curve
**  12/15/96    Added controls for DEFAULT_VELOCITY_CURVE
**  12/30/96    Changed copyright
**  1/12/97     Changed maxNormalizedVoices to mixLevel
**  1/16/97     Changed GM_LFO to LFORecords
**  1/23/97     Added support for PV_PostFilterStereo
**              Changed PV_CleanNoteEntry to use XSetMemory
**  2/1/97      Fixed PV_StopMIDINote to support note offset and look for notes to turn off
**              based upon the starting offset
**  2/13/97     Added some INLINE's to help code size and performance
**  3/14/97     Fixed GM_GetAudioSampleFrame to display mono output correctly
**  3/16/97     Modified PV_StartMIDINote to support root key replacement in
**              keysplits
**  3/18/97     Fixed a bug with new root key replacement. I forgot to handle the
**              no split case
**  4/14/97     Changed KeymapSplit to GM_KeymapSplit
**  4/21/97     Modified PV_StartMIDINote to support volume levels on a per split basis.
**              Fixed a bug in PV_ServeInstrumentCurves. In the LOWPASS_AMOUNT case
**              it multiplied the orignal and lost the original settings. So when
**              midi data changed it would eventually end in 0.
**  6/10/97     Moved MOD output stage to before Midi output stage to allow for reverb
**              of MOD files to be included in mix.
**              Reworked ProcessSampleFrame to seperate clearing of output buffer
**              and mixing of active voices so that we can mix verb into MOD files
**  6/20/97     Added support for new 16 bit drop sample cases
**  7/18/97     Added BAE_BuildMixerSlice & BAE_GetMaxSamplePerSlice
**              Changed ProcessSampleFrame to reference a register version of MusicGlobals,
**              and removed reference to FAR
**  7/22/97     Changed SYNC_BUFFER_TIME to BUFFER_SLICE_TIME
**  7/28/97     Sun removed an extra ; from GetWaveShape
**  8/18/97     Changed X_WIN_BAE to USE_BAE_EXTERNAL_API
**  8/25/97     Commentted out DEBUG_STR
**              Added optional tone code to BAE_BuildBufferSlice
**  10/3/97     Changed PV_CalcScaleBack to use MAX_VOICES constant instead of fixed value
**  10/15/97    Added processingSlice flag to PV_ServeThisInstrument to help with thread issues
**  12/16/97    Moe: removed compiler warnings
**  1/14/98     $$kk: changes to allow a sample to start, loop NoteLoopTarget between the loop points, and then play to the end 
**  1/16/98     Modifed PV_StopMIDINote to only stop one note instead of all with the same
**              play/stop criteria
**  1/22/98     Renamed LOWPASS_AMOUNT to LOW_PASS_AMOUNT
**  1/29/98     Put more code wrappers around verb type 8
**  2/3/98      Renamed songBufferLeftMono to songBufferDry
**  2/8/98      Changed BOOL_FLAG to XBOOL
**  2/10/98     Fixed problem with PV_ServeThisInstrument that allowed a looping sample
**              to stop looping after time
**  2/11/98     Put code wrappers around functions not used for WebTV
**              Renamed GetWaveShape to PV_GetWaveShape
**  2/20/98     kcr     more fully integrate new variable reverbs and variable chorus
**  2/23/98     Put more wrappers around MONO only code and STEREO only code
**  3/16/98     Changed PV_ServeStereoInstruments & PV_ServeMonoInstruments on the way it
**              handles reverb.
**  4/2/98      MOE: Hacked ServeMidiNote() to set avoidReverb and reverbLevel in
**              such a way that changing reverb type in the middle of a long note
**              kind-of works.
**  5/7/98      Changed PV_StartMIDINote to correctly set the verb amount so that it fixes
**              the changing of verb in the middle of a note
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
**  6/18/98     Modified PV_StartMIDINote to scale the pan from the unmodified pan stored
**              during playback
**  6/29/98     Fixed reverb problem. All verb was enabled, rather than working with
**              the instrument designers choices.
**  7/7/98      Removed usage of reverbIsVariable and now use GM_IsReverbFixed instead.
**  7/28/98     Renamed inst_struct to pInstrument
**  8/5/98      Modified PV_DoubleBufferCallbackAndSwap to do the callback before setting
**              the voice to be finished.
**  8/12/98     Made PV_ModifyVelocityFromCurve public
**  8/13/98     Added to BAE_BuildMixerSlice calculation of pMixer->timeSliceDifference
**              Added GM_GetMixerUsedTime
**  8/14/98     Added GM_GetMixerUsedTimeInPercent
**  11/6/98     Removed noteDecayPref from the GM_Waveform structure and changed various API's
**              to reflect that.
**  11/9/98     Renamed NoteDur to voiceMode. Renamed this_voice to pVoice. Formatted.
**              Changed resonantFilterLookup from a short to a char to save space
**  11/10/98    Removed CLIP macro
**  11/30/98    Added GM_EndSongChannelNotes to support omni mode
**  12/7/98     Changed PV_CleanNoteEntry to preserve voiceMode during clean. Fixed a bug
**              in PV_StartMIDINote that would ignore sustaining notes.
**  12/22/98    Removed old USE_SEQUENCER flag
**  1/14/99     Fixed a bug in PV_StartMIDINote in which it ignore the force verb off in
**              the variable verbs
**  3/2/99      Changed NoteRefNum to NoteContext
**  3/4/99      Fixed PV_StartMIDINote to handle samples with loop points starting at 0.
**  3/5/99      Added threadContext to PV_ProcessSampleFrame and renamed from ProcessSampleFrame
**  3/9/99      Renamed myMusicGlobals to pMixer
**              Changed parameter of PV_ADSRModule to use a XBOOL rather than a INT32
**  3/11/99     Renamed ADSRRecord to GM_ADSR. Renamed LFORecord to GM_LFO. Renamed CurveRecord to GM_TieTo.
**  3/12/99     Put in support for different loop types
**  3/31/99     Renamed ServeMIDINote to PV_StartMIDINote, renamed StopMIDINote to PV_StopMIDINote
**  5/27/99     Fixed potenial bug with PV_ServeInstrumentCurves in which an access to an pInstrument
**              was used before checking for it being valid
**  5/28/99     MOE:  Changed calls to PV_ADSRModule() to eliminate data-truncation warnings
**  6/1/99      Fixed logLookupTable entry 1. Added an extra zero to time length.
**              General formating changes.
**  6/8/99      Removed references to USE_BAE_EXTERNAL_API. We now require BAE_API
**  6/15/99     Removed references to MusicGlobals
**  7/6/99      Changed the NextNoteSize calculation to calculate the exact number of samples
**              required rather than guessing.
**  7/9/99      Modified BAE_BuildMixerSlice to pass in taskReference during callback
**  7/13/99     Renamed HAE to BAE
**  10/19/99    MSD: removed FP_OFF(x) macro
**  10/19/99    MSD: switched to REVERB_USED and LOOPS_USED
**  10/30/99    Added Q_16K
**  11/4/99     Fixed in PV_ServeThisInstrument a situation in which the process that
**              determines if to process a partial buffer of sample data, or a full buffer
**              get confused and process a full buffer. It has to do with the current point
**              exceeding the end point.
**  11/7/99     Corrected PV_ClearReverbBuffer in which the REVERB_USED flag was in the wrong
**              direction, so the verb buffer was not being cleared.
**  11/9/99     Added to PV_StartMIDINote the ability to kill notes based upon a priority
**  11/10/99    Added ifdefs to handle the case REVERB_USED == REVERB_DISABLED
**              Combined PV_ServeStereoInstruments and PV_ServeMonoInstruments
**              into one function PV_ServeInstruments
**  1/12/2000   Added GM_IsAudioActive
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  3/7/2000    Fixed a few places where compiler's were complaining about pointers. Removed
**              some dead wood commented out code.
**  4/3/2000    Placed an assertion in PV_ServeThisInstrument to handle period LFOs
**              below 512.
**  4/19/2000   Changed PV_ServeInstruments to handle chorus first then reverb, and chorus
**              is now active all the time if enabled via a midi controller. Also arranged
**              chorus to happen before reverb to match the DLS specs.
**              Added PV_GetLFOAdjustedTimeInMicroseconds to handle the LFO/ADSR time case.
**  4/25/2000   msd: added support for 32kHz and 40kHz output.
**  4/25/2000   sh: Removed dead wood in PV_StartMIDINote surrounding accquiring a new
**              voice.
**  5/2/2000    Removed confusing code in PV_GetLFOAdjustedTimeInMicroseconds
**  5/4/2000    jsc: fixed PV_EndNotes - wasn't allowing notes to fully release
**              in the !kill case
**  5/09/2000   sh  Modified PV_GetLFOAdjustedTimeInMicroseconds to calulate new times
**                  to take in account of the different audio render rates verses
**                  the fixed rates we need for midi and lfo decode rates.
**  5/9/2000    jsc: PV_EndNotes should not set volumeADSRRecord.mode to ADSR_TERMINATE
**  5/18/2000   msd redid calculations in PV_GetWavePitch().
**  5/19/2000   sh  Added PV_GetLogLookupTableEntry to handle the case of non 11 ms slice
**                  times. Used in calculating the decay time of sustain.
**  5/20/2000   sh  In PV_ServeThisInstrument, fixed a bug in which the sustainingDecayTime
**                  was being miscalulated resulting in a odd chunky decay at the tail
**                  of a decaying instrument.
**                  Added some comments of various variables.
**  5/22/2000       Added GM_GetSustainDecayLevelInTime & GM_SetSustainDecayLevelInTime
**  5/23/2000       Added some comments to the logLookupTable table, fixed a bug in
**                  GM_GetSustainDecayLevelInTime. It was returning the fractional multipler
**                  rather than the time. Added a 15 sec entry into logLookupTable[0].
**  5/31/2000       Changed GM_SetSustainDecayLevelInTime & GM_GetSustainDecayLevelInTime
**                  to divide by 16 to represent the amount of time to sustain to 96 dB.
**  6/1/2000        Changed behavior of PV_StopMIDINote to kill only the first note
**                  instead of all notes of the same type.
**  6/2/2000        Put a reasonable entry in expDecayLookup[0]. Changed 
**                  GM_SetSustainDecayLevelInTime & GM_GetSustainDecayLevelInTime to use
**                  a multiply by 8 for its values.
**  6/8/2000        Fixed bug in PV_StopMIDINote in which it turned off the newest note
**                  rather than the oldest.
**  6/15/2000   DS: Fixed stuck note problem #1 by removing check for matching instrument in PV_StopMidiNote()
**                  Also #ifdef'd out assert at bottom of same function since stolen notes will never be found.
**  7/07/2000   DS: Wrote new note stealing routine with modified heuristics.  Fixed bug in modulation ADSR
**                  which was ignoring exponential decaying sustain values.
**  7/12/2000   DS: Fixed bug in 7/07 checkin for exponential modification of mod ADSR envelopes.
**  7/24/2000   msd Fixed problem in GM_IsAudioActive() where it was checking
**                  the right channel of mono data, which was garbage, and returned
**                  TRUE, that audio was still active.
**  7/27/2000   DS: Fixed note-off bug: note off during sustain was not clearing time stamp on voice.
**  7/28/2000   sh  Added in the check that was removed to verify audio active in GM_IsAudioActive
**  8/30/2000   DS: Fixed note start code to properly sum MIDI and fixed inst. pan values.  Now we store
**                  the initial pan offset into stereoPanBend and then add modulation to that.  Only the MIDI
**                  pan values are stored in stereoPosition because SetChannelStereoPosition() overwrites
**                  this value each time controller 10 changes.  As a result of this change, PV_ServeThisInstrument()
**                  now sets stereoPanBend to the fixed instrument pan rather than zeroing it.
**                  Also fixed SquareWave2 output to be a proper full-gain inverted square.
**  8/31/2000   DS: Fixed bug in this fix -- needed to check for NULL instrument.
**  9/05/2000   DS: Fixed bug in loop code under conditions of 1) High pitch 2) Entire sample is looped.
**                  Detail: Fixed two logical checks involving unsigned.
**  9/7/2000    sh  Added GM_KillSongInstrument. Modified PV_EndNotes to allow for a
**                  instrument to passed in as a filter.
**  12/6/2000   sh  Fixed problem with PV_StopMIDINote in which a note off with a songPitchOffset
**                  has changed on a not allowed pitch channel, the note off would get lost and the note
**                  would hang. This is a 'pregnant chad problem'
**                  Put new code in to clear the output mix buffer if the master volume is 0.
**  12/7/2000   sh  Put a fix in PV_EndNotes to remove loop points so that instruments will
**                  stop even if their envelopes don't.
**  2/13/2001   sh  fixed bug in PV_SetPositionFromVoice/PV_GetPositionFromVoice in which
**                  the position was being limited to 1MB even though we're using the
**                  new inner loops.
**  4/12/2001   jsc the change on 12/7 in PV_EndNotes broke instruments with 
**                  sampleAndHold=1 (failed to loop in release stage).
**  4/17/2001   sh  Started partitioning code for MCU/DSP split. Added BAE_BuildMCUSlice &
**                  GM_ProcessSyncUpdateFromDSP.
**  4/19/2001   sh  Added PV_GetVoiceNumberFromVoice. Added new innerloops.
**  4/27/2001   sh  Fixed a bug when starting a note in the BAE_COMPLETE build that didn't
**                  start the note. !
**  5/8/2001    sh  Fixed problem with PV_GetVoiceNumberFromVoice in which the mixer
**                  was not valid.
**  5/23/2001   sh  Changed PV_StartMidiNote to use PV_CalculateMonoVolume
**  5/26/2001   sh  Added PV_GetWavePitchFixed for non floating point support
**                  Modified PV_GetWavePitchU3232 to use the new code for non floating
**                  point support.
**  6/06/2001   DS  Fixed bad bug in PV_FindFreeVoice():  Second heuristic loop was missing line
**                  which set pVoice to each successive voice in list.
**  7/11/2001       Fixed timer wrap problem with CPU load.
**                  Added a new heristic to PV_FindFreeVoice to retrigger voices that are the same.
**  12/7/2001   sh  Modifed PV_StartMIDINote: if the volume of the midi note requested to play or 
**                  the overall song volume is 0, then don't bother starting a note.
*/
/*****************************************************************************/
#include "GenSnd.h"
#include "GenPriv.h"
#include "BAE_API.h"
#include "X_Assert.h"

// the only global. Our current mixer pointer.
GM_Mixer * MusicGlobals = NULL;

// Variables - pitch tables

#define ys  97271
#define PTmake(x)       \
            (539*ys)/(x),   \
            (571*ys)/(x),   \
            (605*ys)/(x),   \
            (641*ys)/(x),   \
            (679*ys)/(x),   \
            (719*ys)/(x),   \
            (762*ys)/(x),   \
            (807*ys)/(x),   \
            (855*ys)/(x),   \
            (906*ys)/(x),   \
            (960*ys)/(x),   \
            (1017*ys)/(x)

static const UINT32 majorPitchTable[] = 
{
    PTmake  (102400),
    PTmake  (51200),
    PTmake  (25600),    // 0..11
    PTmake  (12800),    // 12..23
    PTmake  (6400),     // 24..35
    PTmake  (3200),     // 36..47
    PTmake  (1600),     // 48..59
    PTmake  (800),      // 60-71: first entry of this table should = $1,0000.
    PTmake  (400),      // 72-83
    PTmake  (200),      // 84-95
    PTmake  (100),      // 96-107
    PTmake  (50),       // 108-119
    PTmake  (25),       // 120-127 ($80-up unused)
    PTmake  (25),       // MPW probably won't handle this the same.  This means
    PTmake  (25),       // divide by 25 then multiply by 2.  Same as divide by 12.5
    PTmake  (25)
};

static const UINT32 fractionalPitchTable[] = 
{
    65536,  65566,  65595,  65625,  65654,  65684,  65714,  65743, 
    65773,  65803,  65832,  65862,  65892,  65922,  65951,  65981, 
    66011,  66041,  66071,  66100,  66130,  66160,  66190,  66220, 
    66250,  66280,  66309,  66339,  66369,  66399,  66429,  66459, 
    66489,  66519,  66549,  66579,  66609,  66639,  66670,  66700, 
    66730,  66760,  66790,  66820,  66850,  66880,  66911,  66941, 
    66971,  67001,  67032,  67062,  67092,  67122,  67153,  67183, 
    67213,  67244,  67274,  67304,  67335,  67365,  67395,  67426, 
    67456,  67487,  67517,  67548,  67578,  67609,  67639,  67670, 
    67700,  67731,  67761,  67792,  67823,  67853,  67884,  67915, 
    67945,  67976,  68007,  68037,  68068,  68099,  68129,  68160, 
    68191,  68222,  68252,  68283,  68314,  68345,  68376,  68407, 
    68438,  68468,  68499,  68530,  68561,  68592,  68623,  68654, 
    68685,  68716,  68747,  68778,  68809,  68840,  68871,  68902, 
    68933,  68965,  68996,  69027,  69058,  69089,  69120,  69152, 
    69183,  69214,  69245,  69276,  69308,  69339,  69370,  69402   
};

// Needs to be recomputed if the synthesis time per chunk is ever different than 11 ms.
// entry in table is:
//  15 seconds
//  10 seconds
//  5 seconds
//  4 seconds
//  3 seconds
//  2.5 seconds
//  2 seconds
//  1.5 seconds
//  1 second
//  750 ms
//  500 ms
//  375 ms
//  250 ms
//  175 ms
//  150 ms
//  125 ms
//  80 ms
static const INT32 logLookupTable[] =
{
    15000000, 10000000, 5000000, 4000000, 3000000, 2500000, 2000000, 1500000, 1000000, 700000,
      500000,   400000,  250000,  200000,  150000,  150000,  100000,  100000,  100000, 100000
};

// lookup an entry from the logLookupTable, but compensate for synthesis time per chunk being
// something other than 11 ms.
static INLINE INT32 PV_GetLogLookupTableEntry(INT32 entry)
{
    UINT32  value;

    // this narly peice of code basicly is in floating point:
    // value = (logLookupTable[entry] * FIXED_BUFFER_SLICE_TIME) / BUFFER_SLICE_TIME;
    // we do this fixed point trick instead, ug.
    value = logLookupTable[entry] / 100;
    value = ((value * FIXED_BUFFER_SLICE_TIME) / (MusicGlobals->bufferTime/100));
    return (INT32)value;
}

// 16.16 fixed point table starting at .8 to .9995. range 0 to 319.
// This table is used to determine the amount of time to sustain to 50% of volume level
static const XFIXED expDecayLookup[] =
{
    45000, 55110, 60678, 62370, 63060, 63592, 63876, 64132, 64286, 64438, 64534, 64634, 64716, 64716, 64830, 64830, 64918,64918, 64986, 64986,  // 0, 50-950 ms
    65040, 65040, 65084, 65084, 65120, 65120, 65152, 65152, 65180, 65180, 65204, 65204, 65224, 65224, 65244, 65244, 65260,65260, 65274, 65274,  // 1000-1950 ms
    65288, 65288, 65296, 65296, 65306, 65306, 65316, 65316, 65326, 65326, 65336, 65336, 65342, 65342, 65350, 65350, 65356,65356, 65364, 65364,  // 2000-2950ms
    65372, 65372, 65376, 65376, 65380, 65380, 65384, 65384, 65388, 65388, 65394, 65394, 65396, 65396, 65400, 65400, 65404,65404, 65408, 65408,  // 3000-3950ms
    65412, 65412, 65414, 65414, 65416, 65416, 65418, 65418, 65422, 65422, 65424, 65424, 65426, 65426, 65430, 65430, 65432,65432, 65434, 65434,  // 4000ms
    65438, 65438, 65438, 65438, 65440, 65440, 65442, 65442, 65444, 65444, 65446, 65446, 65446, 65446, 65448, 65448, 65450,65450, 65452, 65452,  // 5000ms
    65454, 65454, 65454, 65454, 65456, 65456, 65456, 65456, 65458, 65458, 65460, 65460, 65460, 65460, 65462, 65462, 65462,65462, 65464, 65464,  // 6000ms
    65466, 65466, 65466, 65466, 65466, 65466, 65468, 65468, 65468, 65468, 65470, 65470, 65470, 65470, 65470, 65470, 65472,65472, 65472, 65472,  // 7000ms
    65474, 65474, 65474, 65474, 65474, 65474, 65476, 65476, 65476, 65476, 65478, 65478, 65478, 65478, 65478, 65478, 65480,65480, 65480, 65480,  // 8000ms
    65482, 65482, 65482, 65482, 65482, 65482, 65482, 65482, 65482, 65482, 65484, 65484, 65484, 65484, 65484, 65484, 65484,65484, 65484, 65484,  // 9000ms
    65486, 65486, 65486, 65486, 65486, 65486, 65486, 65486, 65486, 65486, 65486, 65486, 65488, 65488, 65488, 65488, 65488,65488, 65488, 65488,  // 10 seconds
    65490, 65490, 65490, 65490, 65490, 65490, 65490, 65490, 65490, 65490, 65490, 65490, 65492, 65492, 65492, 65492, 65492,65492, 65492, 65492,  // 11 seconds
    65494, 65494, 65494, 65494, 65494, 65494, 65494, 65494, 65494, 65494, 65496, 65496, 65496, 65496, 65496, 65496, 65496,65496, 65496, 65496,  // 12 seconds
    65498, 65498, 65498, 65498, 65498, 65498, 65498, 65498, 65498, 65498, 65498, 65498, 65500, 65500, 65500, 65500, 65500,65500, 65500, 65500,  // 13 seconds
    65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502, 65502,65502, 65502, 65502,  // 14 seconds
    65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504, 65504,65504, 65504, 65504   // 15 seconds  
 };

#if USE_CREATION_API == TRUE
// return time in microseconds for the decay during sustain system. Values passed in are the stored values
// for ADSRLevel.
//  15 000 000 = 15 seconds
//   1 500 000 = 1.5 seconds
//     150 000 = 15 microseconds
// This is the amount of time to sustain to 96 dB or no volume.
XSDWORD GM_GetSustainDecayLevelInTime(XSDWORD storedValue)
{
    XSDWORD time;

    time = 0;
    if (storedValue < -50)
    {
        // level is greater than 50
        time = -storedValue;
    }
    else
    {
        // level is in the range of 0 to 50
        time = PV_GetLogLookupTableEntry(-storedValue);
    }
    return time * 8;
}

// given a timeInMicroseconds range check between 50000 microseconds and 16 seconds, and return the storable
// value for ADSRLevel.
// This is the amount of time to sustain to 96 dB or no volume.
XSDWORD GM_SetSustainDecayLevelInTime(XDWORD timeInMicroseconds)
{
    timeInMicroseconds /= 8;
    if (timeInMicroseconds > ((sizeof(expDecayLookup) / sizeof(XFIXED)) * 50000L))
    {
        timeInMicroseconds = ((sizeof(expDecayLookup) / sizeof(XFIXED)) * 50000L);
    }
    if (timeInMicroseconds < 50000)
    {
        timeInMicroseconds = 50000;
    }
    return -1 * (long)timeInMicroseconds;
}
#endif  //#if USE_CREATION_API == TRUE

/*
static const UBYTE defaultVolumeScale[] = {
// Subtle curve that ends into zero
127, 125, 123, 121, 119, 117, 115, 113, 111, 109, 108, 106, 104, 102, 101, 99, 97,
96, 94, 93, 91, 90, 88, 87, 85, 84, 82, 81, 80, 78, 77, 76, 75, 73, 72, 71, 70, 69,
67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 53, 52, 51, 50, 49, 48,
48, 47, 46, 45, 44, 44, 43, 42, 42, 41, 40, 40, 39, 38, 38, 37, 36, 36, 35, 35, 34,
33, 33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 27, 26, 26, 25, 25, 24, 24,
24, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17,
17, 16, 16, 16, 16, 0
};

static const UBYTE defaultVlumeScale[] = {
// harsh curve that ends into zero
127, 124, 121, 118, 115, 112, 109, 106, 104, 101, 98, 96, 93, 91, 89, 87, 84, 82, 80,
78, 76, 74, 72, 71, 69, 67, 65, 64, 62, 60, 59, 57, 56, 55, 53, 52, 51, 49, 48, 47, 46,
44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 34, 33, 32, 31, 30, 29, 29, 28, 27, 27, 26,
25, 25, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 16, 15, 15,
14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8,
8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 3, 1, 0
};
*/

/*
static const unsigned short defaultVolumeScale[] = {
// new more agressive curve
296,286,276,267,258,249,241,233,225,217,210,202,196,189,182,176,170,164,159,
153,148,143,138,133,129,124,120,116,112,108,104,101,97,94,90,87,84,81,78,76,
73,70,68,66,63,61,59,57,55,53,51,49,47,46,44,42,41,39,38,36,35,34,32,31,30,
29,28,27,26,25,24,23,22,21,20,19,19,18,17,16,16,15,14,14,13,13,12,12,11,
11,10,10,9,9,8,8,7,7,7,6,6,6,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,2,
1,1,1,1,1,1,0,0,0,0
};
*/

/*
static const unsigned short defaultVolumeScale[] = {
// new more agressive curve
517, 498, 480, 462, 445, 429, 413, 398, 384, 370, 356,
343, 331, 319, 307, 296, 285, 275, 265, 256, 246, 238, 229, 221, 213,
205, 198, 191, 184, 178, 171, 165, 160, 154, 149, 143, 138, 134, 129,
125, 120, 116, 112, 108, 105, 101, 98, 94, 91, 88, 85, 83, 80, 77,
75, 72, 70, 68, 66, 63, 61, 60, 58, 56, 54, 53, 51, 49, 48, 47, 45,
44, 43, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 31, 30, 29, 28, 28,
27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 20, 19, 
19, 19, 18, 18, 18, 17, 17, 17, 17, 16, 16, 16, 16, 15, 15, 15, 15, 
15, 14, 14, 14, 0
};
*/

/*
static const unsigned short defaultVolumeScale[] = {
// new more agressive curve
329, 318, 307, 297, 288, 278, 269, 260, 252, 244, 236,
228, 221, 214, 207, 200, 194, 187, 181, 176, 170, 165, 159, 154, 149, 145,
140, 136, 131, 127, 123, 119, 116, 112, 109, 105, 102, 99, 96, 93, 90, 88,
85, 82, 80, 78, 75, 73, 71, 69, 67, 65, 63, 61, 60, 58, 56, 55, 53, 52, 50,
49, 48, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 32, 31,
30, 30, 29, 28, 28, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 22, 21,
21, 21, 20, 20, 19, 19, 19, 19, 18, 18, 18, 17, 17, 17, 17, 16, 16, 16,
16, 16, 15, 15, 15, 15, 15, 15, 14, 14, 14, 0
};
*/

/*
static const unsigned short defaultVolumeScale[] = {
// new more agressive curve
302, 292, 282, 273, 264, 255, 247, 239, 231, 223, 216, 208, 202, 195, 188, 
182, 176, 170, 165, 159, 154, 149, 144, 139, 135, 130, 126, 122, 118, 114, 
110, 107, 103, 100, 96, 93, 90, 87, 84, 82, 79, 76, 74, 72, 69, 67, 65, 
63, 61, 59, 57, 55, 53, 52, 50, 48, 47, 45, 44, 42, 41, 40, 38, 37, 36, 
35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 25, 24, 23, 22, 22, 21, 20, 
20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 13, 12, 12, 
12, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 
7, 7, 6, 6, 6, 6
};
*/

/*
static const unsigned short defaultVolumeScale[] = {
// 2.5 times linear curve
318, 315, 313, 310, 308, 305, 303, 300, 298, 295, 293, 290, 288, 285,
283, 280, 278, 275, 273, 270, 268, 265, 263, 260, 258, 255, 253, 250,
248, 245, 243, 240, 238, 235, 233, 230, 228, 225, 223, 220, 218, 215,
213, 210, 208, 205, 203, 200, 198, 195, 193, 190, 188, 185, 183, 180,
178, 175, 173, 170, 168, 165, 163, 160, 158, 155, 153, 150, 148, 145,
143, 140, 138, 135, 133, 130, 128, 125, 123, 120, 118, 115, 113, 110,
108, 105, 103, 100, 98, 95, 93, 90, 88, 85, 83, 80, 78, 75, 73, 70, 68,
65, 63, 60, 58, 55, 53, 50, 48, 45, 43, 40, 38, 35, 33, 30, 28, 25, 23,
20, 18, 15, 13, 10, 8, 5, 3, 0
};
*/

/*
static const unsigned short defaultVolumeScale[] = {
// 2.2 times linear curve
279, 277, 275, 273, 271, 268, 266, 264, 262, 260, 257, 255, 253, 251,
249, 246, 244, 242, 240, 238, 235, 233, 231, 229, 227, 224, 222, 220,
218, 216, 213, 211, 209, 207, 205, 202, 200, 198, 196, 194, 191, 189,
187, 185, 183, 180, 178, 176, 174, 172, 169, 167, 165, 163, 161, 158,
156, 154, 152, 150, 147, 145, 143, 141, 139, 136, 134, 132, 130, 128,
125, 123, 121, 119, 117, 114, 112, 110, 108, 106, 103, 101, 99, 97, 95,
92, 90, 88, 86, 84, 81, 79, 77, 75, 73, 70, 68, 66, 64, 62, 59, 57, 55,
53, 51, 48, 46, 44, 42, 40, 37, 35, 33, 31, 29, 26, 24, 22, 20, 18, 15,
13, 11, 9, 7, 4, 2, 0
};
*/


static const UBYTE defaultVolumeScale[] = {
// cool S curve version 2
252, 248, 246, 242, 240, 238, 232, 230, 228, 222,
220, 218, 214, 212, 208, 204, 200, 198, 194, 190,
190, 184, 182, 180, 174, 172, 170, 166, 164, 160,
158, 154, 150, 148, 144, 144, 142, 138, 138, 136,
132, 130, 128, 126, 124, 122, 120, 118, 116, 114,
112, 110, 108, 106, 104, 102, 98, 98, 94, 92,
92, 88, 86, 86, 82, 80, 78, 74, 74, 72,
68, 68, 66, 62, 62, 62, 58, 58, 56, 54,
54, 54, 50, 50, 48, 46, 46, 46, 42, 42,
40, 38, 38, 38, 34, 34, 32, 30, 30, 30,
26, 26, 24, 22, 22, 22, 20, 18, 18, 16,
16, 16, 12, 12, 12, 10, 10, 10, 8, 8,
8, 4, 4, 4, 2, 2, 1, 0
};

static const UBYTE volumeScaleSCurveOriginal[] = {
// cool S Curve version 1
254, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 248, 248, 248, 246, 246,
246, 246, 244, 244, 244, 244, 242, 242, 242, 242, 240, 240, 240, 238, 238, 238,
238, 236, 236, 236, 236, 234, 234, 232, 230, 228, 226, 224, 224, 222, 220, 218,
216, 214, 212, 210, 210, 208, 206, 204, 202, 200, 198, 196, 192, 190, 188, 186,
184, 182, 180, 176, 174, 172, 170, 168, 164, 162, 158, 154, 152, 148, 144, 142,
138, 136, 132, 128, 122, 118, 114, 108, 104, 100, 94, 90, 86, 82, 76, 72, 68, 62,
58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20,
18, 16, 14, 12, 10, 8, 6, 4, 2, 0
};

static const UBYTE volumeScaleTwoTimes[] = {
// two times linear curve
254, 252, 250, 248, 246, 244, 242, 240, 238, 236, 234, 232, 230, 228, 226, 
224, 222, 220, 218, 216, 214, 212, 210, 208, 206, 204, 202, 200, 198, 196, 
194, 192, 190, 188, 186, 184, 182, 180, 178, 176, 174, 172, 170, 168, 166, 
164, 162, 160, 158, 156, 154, 152, 150, 148, 146, 144, 142, 140, 138, 136, 
134, 132, 130, 128, 126, 124, 122, 120, 118, 116, 114, 112, 110, 108, 106, 
104, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 74, 72, 70, 
68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 
30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0
};


static const UBYTE volumeScaleSubtle[] = {
// Subtle curve that is above zero
// This is the default velocity curve for WebTV
254, 248, 243, 238, 232, 227, 222, 217, 213, 208, 203, 199, 194, 190, 186, 182, 178, 174,
170, 166, 163, 159, 156, 152, 149, 146, 142, 139, 136, 133, 130, 127, 125, 122, 119, 117,
114, 112, 109, 107, 104, 102, 100, 98, 96, 93, 91, 89, 87, 85, 84, 82, 80, 78, 76, 75,
73, 72, 70, 68, 67, 65, 64, 63, 61, 60, 59, 57, 56, 55, 54, 52, 51, 50, 49, 48, 47, 46,
45, 44, 43, 42, 41, 40, 39, 38, 38, 37, 36, 35, 34, 34, 33, 32, 31, 31, 30, 29, 29, 28,
28, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 18, 17,
17, 17, 16, 16, 15, 0
};

static const UBYTE volumeScaleTwoTimesExp[] = {
// Rev Exp x2
255, 250, 246, 242, 240, 236, 232, 228, 224, 220,
216, 212, 210, 206, 202, 198, 194, 190, 186, 182,
180, 176, 172, 168, 164, 160, 156, 152, 150, 146,
142, 138, 134, 130, 128, 126, 124, 122, 120, 118,
116, 112, 110, 108, 106, 104, 102, 100, 98, 96,
94, 92, 90, 88, 86, 84, 82, 78, 76, 74,
72, 70, 68, 66, 64, 62, 62, 60, 60, 58,
58, 56, 56, 54, 54, 52, 52, 50, 50, 48,
48, 46, 46, 44, 44, 42, 42, 40, 40, 38,
38, 36, 36, 34, 34, 32, 32, 30, 30, 28,
28, 26, 26, 24, 24, 22, 22, 20, 20, 18,
18, 16, 16, 14, 14, 12, 12, 10, 10, 8,
8, 6, 6, 4, 4, 2, 2, 0
};

#if 0
static unsigned long PV_AdjustTimeFromRate(unsigned long time)
{
    Rate q = MusicGlobals->outputRate;
    unsigned long mixerRate;
    unsigned long realMixerRate;

    mixerRate = GM_ConvertFromOutputRateToPerceivedRate(q);
    realMixerRate = GM_ConvertFromOutputRateToRate(q);
    if (mixerRate != realMixerRate)
    {
        // time = time * mixerRate) / realMixerRate
        time = XFixedMultiply(time, mixerRate);
        time = XFixedDivide(time, realMixerRate);
    }
    return time;
}
#endif

// This value returns is the clock the evelope's and LFO run on. In microseconds.
static unsigned long PV_GetLFOAdjustedTimeInMicroseconds(void)
{
#if 1
    return MusicGlobals->lfoBufferTime;
#else
    unsigned long time = (FIXED_BUFFER_SLICE_TIME * MAX_CHUNK_SIZE / FIXED_MAX_CHUNK_SIZE) - 610;

    return PV_AdjustTimeFromRate(time);
#endif
}

// given a midi volume, translate it via a table to a new value
long PV_ModifyVelocityFromCurve(GM_Song *pSong, long volume)
{
    volume = 127L - (volume & 0x7FL);       // restrict to 0-127 and reverse
    switch (pSong->velocityCurveType)
    {
        default:        // just in case its out of range
        case 0: // default S curve
            volume = defaultVolumeScale[volume];
            break;
        case 1: // more peaky S curve
            volume = volumeScaleSCurveOriginal[volume];
            break;
        case 2: // WebTV curve
            volume = volumeScaleSubtle[volume];
            break;
        case 3: // two time exponential
            volume = volumeScaleTwoTimesExp[volume];
            break;
        case 4: // two times linear
            volume = volumeScaleTwoTimes[volume];
            break;
    }
    return volume;
}



#if USE_CALLBACKS
void PV_DoCallBack(GM_Voice *pVoice)
{
    // this needs to be protected   
    GM_SoundDoneCallbackPtr callback = pVoice->NoteEndCallback;
    pVoice->NoteEndCallback = NULL;
    if (callback)
    {
        (*callback)(pVoice->NoteContext);
    }
}
#endif

void PV_CleanNoteEntry(GM_Voice * the_entry)
{
    VoiceMode   mode;

    mode = the_entry->voiceMode;
    XSetMemory((char *)the_entry, (long)sizeof(GM_Voice), 0);
    the_entry->voiceMode = mode;
}



// Compute scale back amplification factors. Used to amplify and scale the processed audio frame.
//
//  Relies upon:
//  MusicGlobals->MaxNotes
//  MusicGlobals->MaxEffects
//  MusicGlobals->MaxNormNotes
//  MusicGlobals->MasterVolume
//


#if 1
static const short int L2Levels[] = 
{
  1250,    883,    721,    625,    559,    510,    472,    441,
   416,    395,    376,    360,    346,    334,    322,    312,
   303,    294,    286,    279,    272,    266,    260,    255,
   250,    245,    240,    236,    232,    228,    224,    220,
   217,    214,    211,    208,    205,    202,    200,    197,
   195,    192,    190,    188,    186,    184,    182,    180,
   178,    176,    175,    173,    171,    170,    168,    167,
   165,    164,    162,    161,    160,    158,    157,    156
};

static short int PV_L2(short maxVoice)
{
    return L2Levels[maxVoice-1];
}
#else
#define L2_ZERO_LEVEL   1250.0
#include <math.h>

static short int PV_L2(short maxVoice)
{
    double  ratio;

    ratio = 1.0 / sqrt((double)maxVoice);

    return (short int)(ratio * L2_ZERO_LEVEL);
}
#endif

void PV_CalcScaleBack(void)
{
    GM_Mixer        *pMixer = GM_GetCurrentMixer();

#if 1
    pMixer->scaleBackAmount = PV_L2(pMixer->mixLevel); 
#else
    int             noteScale;
    long            scaleSize;

    noteScale = (pMixer->MaxNotes + pMixer->MaxEffects) * UPSCALAR;
    scaleSize = noteScale * MAX_MASTER_VOLUME;
    if (pMixer->mixLevel <= MAX_VOICES)
    {
        scaleSize = scaleSize / (pMixer->mixLevel*UPSCALAR) * pMixer->MasterVolume;
    }
    else
    {
        scaleSize = scaleSize * 100 / (pMixer->mixLevel*UPSCALAR) * pMixer->MasterVolume;
    }

    scaleSize = scaleSize / MAX_MASTER_VOLUME;

    pMixer->scaleBackAmount = (scaleSize * MAX_MASTER_VOLUME) / noteScale;
#endif
#if 0
    BAE_PRINTF("scaleback %ld mixLevel %d maxNotes %d\n", 
                                                pMixer->scaleBackAmount, pMixer->mixLevel, 
                                                pMixer->MaxNotes + pMixer->MaxEffects);
#endif
}


#if USE_CALLBACKS
// used by macro THE_CHECK. This mainly used by double buffered audio clips
long PV_DoubleBufferCallbackAndSwap(GM_DoubleBufferCallbackPtr doubleBufferCallback, 
                                        GM_Voice *pVoice)
{
    INT32   bufferSize;

    bufferSize = (XBYTE *)pVoice->NotePtrEnd - (XBYTE *)pVoice->NotePtr;
    // we hit the end of the loop call double buffer to notify swap
    (*doubleBufferCallback)(pVoice->NoteContext, pVoice->NotePtr, &bufferSize);
    // now we swap pointers
    if (bufferSize)
    {
        if (pVoice->NotePtr == pVoice->doubleBufferPtr1)
        {
            pVoice->NotePtr = (XBYTE *)pVoice->doubleBufferPtr2;
            pVoice->NotePtrEnd = (XBYTE *)pVoice->doubleBufferPtr2 + bufferSize;
        }
        else
        {
            pVoice->NotePtr = (XBYTE *)pVoice->doubleBufferPtr1;
            pVoice->NotePtrEnd = (XBYTE *)pVoice->doubleBufferPtr1 + bufferSize;
        }

        pVoice->NoteLoopPtr = pVoice->NotePtr;
        pVoice->NoteLoopEnd = pVoice->NotePtrEnd;
        pVoice->voiceMode = VOICE_SUSTAINING;           // reset durations otherwise the voice will shut down
        pVoice->NoteDecay = 0x7FFF;
    }
    else
    {
        PV_DoCallBack(pVoice);
        pVoice->voiceMode = VOICE_UNUSED;
#ifdef BAE_MCU
        GM_KillVoiceOnDSP(pVoice);
#endif
    }
    return bufferSize;
}
#endif

unsigned long PV_GetPositionFromVoice(GM_Voice *pVoice)
{
    unsigned long   pos;
    unsigned long   size;

    switch (MusicGlobals->interpolationMode)
    {
#if LOOPS_USED == FLOAT_LOOPS
        case E_LINEAR_INTERPOLATION_FLOAT:
            pos = pVoice->samplePosition_f;
            break;
#endif
#if LOOPS_USED == U3232_LOOPS
        case E_LINEAR_INTERPOLATION_U3232:
            pos = pVoice->samplePosition.i;
            break;
#endif
        default:
            size = (XBYTE *)pVoice->NotePtrEnd - (XBYTE *)pVoice->NotePtr;  // get sample size of active voice
            if (size < MAX_SAMPLE_FRAMES)
            {
                pos = pVoice->NoteWave >> STEP_BIT_RANGE;
            }
            else
            {
                pos = 0;
            }
            break;
    }
    return pos;
}

void PV_SetPositionFromVoice(GM_Voice *pVoice, unsigned long pos)
{
    switch (MusicGlobals->interpolationMode)
    {
#if LOOPS_USED == FLOAT_LOOPS
        case E_LINEAR_INTERPOLATION_FLOAT:
            pVoice->samplePosition_f = pos;
            break;
#endif
#if LOOPS_USED == U3232_LOOPS
        case E_LINEAR_INTERPOLATION_U3232:
            pVoice->samplePosition.i = pos;
            pVoice->samplePosition.f = 0;
            break;
#endif
        default:
            if (pos < MAX_SAMPLE_FRAMES)
            {
                pVoice->NoteWave = pos << STEP_BIT_RANGE;
            }
            else
            {
                pVoice->NoteWave = 0;
            }
            break;
    }
}

#if LOOPS_USED == U3232_LOOPS

//return (22050.0 * notePitch) / mixerRate;
static INLINE XFIXED PV_GetWavePitchFixed(XFIXED notePitch)
{
    XFIXED  coef;
    XFIXED  mixerRate = GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate) << 16L;
    coef = XFixedDivide(22050L << 16L, mixerRate);
    return XFixedMultiply(coef, notePitch);
}

#if USE_FLOAT != FALSE
UFLOAT PV_GetWavePitchFloat(XFIXED notePitch)
{
    UFLOAT  pitch;
    
    pitch = XFIXED_TO_FLOAT(notePitch);
    return (22050.0 * pitch) / (float)GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate);
}
#endif

// Given a note pitch, which is a fixed point multiplier where 1.0 is 22050.0
// in pitch scale, calculate a new rate based upon the current mixer output rate.
//
// The basic calculation is:
//
// newRate = (notePitch * 22050.0) / outputSampleRate;
U3232 PV_GetWavePitchU3232(XFIXED notePitch)
{
#if USE_FLOAT == FALSE
    XFIXED      pitch;
    U3232       increment;

    pitch = PV_GetWavePitchFixed(notePitch);

    increment.i = pitch >> 16L;     // the 16.0
    increment.f = (pitch & 0xFFFFL) << 16L; // the 0.16
    //BAE_PRINTF("i = %ld f = %ld\n", increment.i, increment.f);
    return increment;

#else
    UFLOAT      pitch;
    U3232       increment;

    //MOE: eventually replace all this with pure integer code
    pitch = PV_GetWavePitchFloat(notePitch);

    increment.i = (U32)(INT32)pitch;
    increment.f = (U32)(INT32)((pitch - (UFLOAT)(INT32)increment.i) * (65536.0 * 65536 / 2)) * 2;
    //BAE_PRINTF("i = %ld f = %ld\n", increment.i, increment.f);
    return increment;
#endif
}
#endif

#if LOOPS_USED == LIMITED_LOOPS
// Given a note pitch, which is a fixed point multiplier where 1.0 is 22050.0
// in pitch scale, calculate a new rate based upon the current mixer output rate.
//
// The basic calculation is:
//
// newRate = (notePitch * 22050.0) / outputSampleRate;
//
// Even though this function is marked to return an XFIXED, its actaully a
// fixed value in the range of 20.12, where as 20 bits is the whole number,
// and 12 bits is the fractional amount.
//
// result_20.12 = notePitch_16.16 / ((22050/outputSampleRate)>>4)
//
XFIXED PV_GetWavePitch(XFIXED notePitch)
{
    XFIXED  coef;
    XFIXED  mixerRate = GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate) << 16L;
    coef = XFixedDivide(22050L << 16L, mixerRate);
    return XFixedMultiply(coef, notePitch) >> 4L;
}
#endif

// given a voice structure, calculate what voice this is
XWORD PV_GetVoiceNumberFromVoice(GM_Voice *pVoice)
{
    XWORD       voice;
    GM_Mixer    *pMixer;

    voice = 0;
//  pMixer = pVoice->pMixer;
    pMixer = GM_GetCurrentMixer();
    if (pMixer)
    {
        voice = pVoice - &pMixer->NoteEntry[0];
    }
    return voice;
}

// ------------------------------------------------------------------------------------------------------//



// Generic ADSR Unit
static void PV_ADSRModule(GM_ADSR *a, XBOOL sustaining)
{
    INT32 currentTime = a->currentTime;
    INT32 index =  a->currentPosition;
    INT32 scalar, i;

// Find the release or LAST marker when the note is being turned off.

    if ((!sustaining) && (a->mode != ADSR_RELEASE) && (a->mode != ADSR_TERMINATE))
    {
        for (i = 0; i < ADSR_STAGES; i++)
        {
            scalar = a->ADSRFlags[i];
            if ((scalar == ADSR_RELEASE) || (scalar == ADSR_TERMINATE))
            {
                index = i;
                a->previousTarget = a->currentLevel;
                goto foundRelease;
            }
            if (scalar == ADSR_SUSTAIN)
            {
                index = i+1;
                a->previousTarget = a->currentLevel;
                goto foundRelease;
            }
            
        }
foundRelease:
        currentTime = 0;
        a->mode = ADSR_RELEASE;
    }
            
    switch (a->ADSRFlags[index])
    {
        case ADSR_SUSTAIN:
            a->mode = ADSR_SUSTAIN;
            if (a->ADSRLevel[index] < 0)
            {
                if (a->sustainingDecayLevel)
                {
                    XSDWORD ADSRLevel;
                    XFIXED  levelScale;

                    ADSRLevel = a->ADSRLevel[index];
                    if (ADSRLevel < -50)
                    {
                        // level is greater than 50
                    //  a->sustainingDecayLevel = ( a->sustainingDecayLevel * 
                    //                              (expDecayLookup[-ADSRLevel/50000L] >> 1L)  ) >> 15L;
                        levelScale = expDecayLookup[-ADSRLevel/50000L];

                    }
                    else
                    {
                        // level is in the range of 0 to 50
                    //  a->sustainingDecayLevel = ( a->sustainingDecayLevel * 
                    //                              (expDecayLookup[PV_GetLogLookupTableEntry(-ADSRLevel)/50000L] >> 1L)  ) >> 15L;
                        levelScale = expDecayLookup[PV_GetLogLookupTableEntry(-ADSRLevel)/50000L];
                    }
                    
                    a->sustainingDecayLevel = XFixedMultiply(a->sustainingDecayLevel, levelScale);
                }
            }
            else
            {
                if (currentTime)
                {
                    currentTime += PV_GetLFOAdjustedTimeInMicroseconds();       // microseconds;
                    if (currentTime >= a->ADSRTime[index])
                    {
                        currentTime = a->ADSRTime[index];
                    }
                    if (a->ADSRTime[index] >> 6L)
                    {
                        scalar = (currentTime << 6) / (a->ADSRTime[index] >> 6);    // scalar is 0..4095
                    }
                    else
                    {
                        scalar = 0;
                    }
                    a->currentLevel = a->previousTarget + (((a->ADSRLevel[index] - a->previousTarget) * scalar) >> 12L);
                }
            }
            break;
        default:
            currentTime += PV_GetLFOAdjustedTimeInMicroseconds();       // microseconds;
            if (currentTime >= a->ADSRTime[index])
            {
                a->previousTarget = a->ADSRLevel[index];
                a->currentLevel = a->ADSRLevel[index];
                currentTime -=  a->ADSRTime[index];
                if (a->ADSRFlags[index] != ADSR_TERMINATE)
                {
                    index++;
                }
                else
                {
                    a->mode = ADSR_TERMINATE;
                    currentTime -= PV_GetLFOAdjustedTimeInMicroseconds();       // prevent long note times from overflowing if they stay on for more than 32.767 seconds
                }
            }
            else
            {
                if (currentTime)
                {
                    scalar = (currentTime << 6) / (a->ADSRTime[index] >> 6);    // scalar is 0..4095
                    a->currentLevel = a->previousTarget + (((a->ADSRLevel[index] - a->previousTarget) * scalar) >> 12); // ADSRLevels max out at 64k
                }
            }
            break;
    }

    // the index may have changed, so check for new cases.
#if 0
    switch (a->ADSRFlags[index])
    {
        case ADSR_GOTO:
            index = a->ADSRLevel[index] - 1;
            break;
    }
#endif
    //BAE_PRINTF("currentLevel = %ld\n", (long)a->currentLevel);
    a->currentTime = currentTime;
    a->currentPosition = index & 7; // protect against runaway indexes
}

static INLINE INT32 PV_GetWaveShape (INT32 where, INT32 what_kind)
{
    switch (what_kind)
    {
        case SINE_WAVE:
            if (where > 32768)
            {
                return ((65536- where)* 4) - 65536;
            }
            else
            {
                return (where * 4) - 65536;
            }
        case SAWTOOTH_WAVE:
                return (32768 - where) * 2;
        case SAWTOOTH_WAVE2:
                return (where - 32768) * 2;
        case TRIANGLE_WAVE:
            if (where > 32768)
            {
                return ((65536- where)* 4) - 65536;
            }
            else
            {
                return (where * 4) - 65536;
            }
        case SQUARE_WAVE:
            if (where > 32768)
            {
                return 65536;
            }
            else
            {
                return -65536;
            }
        case SQUARE_WAVE2:
            if (where > 32768)
            {
                return -65536;
            }
            else
            {
                return 65536;
            }
        default:
            if (where > 32768)
            {
                return ((65536- where)* 4) - 65536;
            }
            else
            {
                return (where * 4) - 65536;
            }
    }
}

static const char resonantFilterLookup[] = 
{
    42, 40, 37, 35, 33, 63, 59, 56, 53, 
    50, 47, 45, 42, 40, 37, 35, 33, 63, 59, 56, 53, 
    50, 47, 45, 42, 40, 37, 35, 33, 32, 30, 28, 27, 
    50, 47, 45, 42, 40, 37, 35, 33, 32, 30, 28, 27, 
    50, 47, 45, 42, 40, 37, 35, 33, 32, 30, 28, 27, 
    25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 
    13, 12, 11, 11, 10,  9,  9,  8,  8,  7,  7,  7, 
     6,  6,  6,  6,  6,  5,  5,  5,  5,  5,  5,  5, 
     5,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  3,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static INLINE void PV_ServeInstrumentCurves(GM_Voice *pVoice)
{
    INT32           i, j, count, tieFromValue, scalar;
    GM_Instrument   *pInstrument;
    INT32           curveCount;
    
    if (pVoice->ModWheelValue != pVoice->LastModWheelValue)
    {
        pVoice->LastModWheelValue = pVoice->ModWheelValue;
        pInstrument = pVoice->pInstrument;
        if (pInstrument)
        {
            if (pInstrument->curveRecordCount)
            {
                for (i = 0; i < pInstrument->curveRecordCount; i++)
                {
                    if (pInstrument->curve[i].tieFrom == MOD_WHEEL_CONTROL)
                    {
                        tieFromValue = pVoice->ModWheelValue;
                        curveCount = pInstrument->curve[i].curveCount;      // in case no hits occur
                        scalar = tieFromValue;
                        for (count = 0; count < curveCount; count++)
                        {
                            if (pInstrument->curve[i].from_Value[count] <= tieFromValue)
                            {
                                if (pInstrument->curve[i].from_Value[count+1] >= tieFromValue)
                                {
                                    scalar = pInstrument->curve[i].to_Scalar[count];
                                    if (pInstrument->curve[i].from_Value[count] != pInstrument->curve[i].from_Value[count+1])
                                    {
                                        INT32 from_difference = pInstrument->curve[i].from_Value[count+1] - pInstrument->curve[i].from_Value[count];
                                        INT32 to_difference = pInstrument->curve[i].to_Scalar[count+1] - pInstrument->curve[i].to_Scalar[count];
                                        scalar += ((((tieFromValue - pInstrument->curve[i].from_Value[count]) << 8) / from_difference) * to_difference) >> 8;
                                    }
                                }
                            }
                        }
                        switch (pInstrument->curve[i].tieTo)
                        {
                            case LPF_FREQUENCY:
                                // pull original frequency and scale it based upon current mod wheel settings
                                pVoice->LPF_base_frequency = (pInstrument->LPF_frequency * scalar) >> 8;
                                break;
                            case LOW_PASS_AMOUNT:
                                // pull original lowpassAmount and scale it based upon current mod wheel settings
                                pVoice->LPF_base_lowpassAmount = (pInstrument->LPF_lowpassAmount * scalar) >> 8;
                                break;
                            case PITCH_LFO:
                                for (j = pVoice->LFORecordCount - 1; j >= 0; --j)
                                {
                                    if (pVoice->LFORecords[j].where_to_feed == PITCH_LFO)
                                    {
                                        pVoice->LFORecords[j].level = (pInstrument->LFORecords[j].level * scalar) >> 8;
                                        goto done;
                                    }
                                }
                                break;
                            case VOLUME_LFO:
                                for (j = pVoice->LFORecordCount - 1; j >= 0; --j)
                                {
                                    if (pVoice->LFORecords[j].where_to_feed == VOLUME_LFO)
                                    {
                                        pVoice->LFORecords[j].level = (pInstrument->LFORecords[j].level * scalar) >> 8;
                                        goto done;
                                    }
                                }
                                break;
                            case PITCH_LFO_FREQUENCY:
                                for (j = pVoice->LFORecordCount - 1; j >= 0; --j)
                                {
                                    if (pVoice->LFORecords[j].where_to_feed == PITCH_LFO)
                                    {
                                        pVoice->LFORecords[j].period = (pInstrument->LFORecords[j].period * scalar) >> 8;
                                        goto done;
                                    }
                                }
                                break;
                            case VOLUME_LFO_FREQUENCY:
                                for (j = pVoice->LFORecordCount - 1; j >= 0; --j)
                                {
                                    if (pVoice->LFORecords[j].where_to_feed == VOLUME_LFO)
                                    {
                                        pVoice->LFORecords[j].period = (pInstrument->LFORecords[j].period * scalar) >> 8;
                                        goto done;
                                    }
                                }
                                break;
                        }
                        done:;
                    }
                }
            }
        }
    }
}

static void PV_LockInstrumentAndVoice(GM_Voice *pVoice)
{
    pVoice->processingSlice = TRUE;     // processing
    if (pVoice->pInstrument)
    {
        pVoice->pInstrument->processingSlice = TRUE;
    }
}

static void PV_UnlockInstrumentAndVoice(GM_Voice *pVoice)
{
    pVoice->processingSlice = FALSE;        // done processing
    if (pVoice->pInstrument)
    {
        pVoice->pInstrument->processingSlice = FALSE;
    }
}

// Process this active voice
static void PV_ServeThisInstrument(GM_Voice *pVoice)
{
    register unsigned long  start, end, loopend, size;
    register long           n, i, value;
    GM_LFO                  *rec;
    GM_Mixer                *pMixer;

    pMixer = MusicGlobals;

    if (pVoice->voiceMode == VOICE_ALLOCATED)
    {
        // this is a special thread case in which we have allocate the voice
        // but are in the middle of filling out the voice elements prior to
        // starting.
        return;
    }
    // lock voice and instrument
    PV_LockInstrumentAndVoice(pVoice);

    // We set this each time in order to allow modulations to be summed into
    // stereoPanPlacement each time through.  It is placed here in the code to allow
    // the future possibility of pan modulation inside PV_ServeInstrumentCurves().
    if (pVoice->pInstrument)
        pVoice->stereoPanBend = pVoice->pInstrument->panPlacement;
    else
        pVoice->stereoPanBend = 0;

    // process curves
    PV_ServeInstrumentCurves(pVoice);

    /* Calculate pitch bend before calculating the note's maximum
    ** # of samples that it can play this frame.
    */

// Get the latest pitchbend controller value, which should be munged into an
// 8.8 Fixed value for semitones to bend.
    n = pVoice->NotePitchBend;

//  n += 12*256;    // if we need to bump up sound pitches for testing of advanced filters
// Process LFO's
    pVoice->volumeLFOValue = 4096;  // default value. Will change below if there's a volume LFO unit present.
    pVoice->LPF_resonance = pVoice->LPF_base_resonance;
    pVoice->LPF_lowpassAmount = pVoice->LPF_base_lowpassAmount;
    if (pVoice->LPF_base_frequency <= 0)            // if resonant frequency tied to note pitch, zero out frequency
    {
        pVoice->LPF_frequency = 0;
    }
    if (pVoice->LFORecordCount)
    {
        for (i = 0; i < pVoice->LFORecordCount; i++)
        {
            rec = &(pVoice->LFORecords[i]);
            PV_ADSRModule (&(rec->a),
                            (XBOOL)((pVoice->voiceMode == VOICE_SUSTAINING) ||
                                    (pVoice->sustainMode == SUS_ON_NOTE_ON)));
            if ( (rec->level) || (rec->DC_feed) )
            {
                // scale the current adsr level by the sustainDecayLevel which is fixed point
                XSDWORD adsrLevel;
                if (rec->a.currentLevel < 0)
                {
                    adsrLevel = -((XSDWORD)(((-rec->a.currentLevel >> 1) * (rec->a.sustainingDecayLevel >> 1)) >> 14));
                }
                else
                    adsrLevel = ((rec->a.currentLevel >> 1) * (rec->a.sustainingDecayLevel >> 1)) >> 14;
#if 0
                    BAE_PRINTF("current: %d sust decay: %d adsrLevel: %d\n",
                             rec->a.currentLevel, rec->a.sustainingDecayLevel, adsrLevel);
#endif
                rec->LFOcurrentTime += PV_GetLFOAdjustedTimeInMicroseconds();
                if (rec->period)
                {
                    if (rec->LFOcurrentTime > rec->period)
                        rec->LFOcurrentTime -= rec->period;
                    // Produce a percentage index into the current LFO period, scaled to 0..65536.
                    // this calculation maxes out at about 15 seconds 
                    BAE_ASSERT(rec->period > 512);
                    // don't let it fall below our range
                    if (rec->period > 512)
                    {
                        rec->currentWaveValue = (rec->LFOcurrentTime << 7) / (rec->period >> 9);
                    }
                    value = (adsrLevel * rec->level) >> 16;
//                  value = (value * rec->a.sustainingDecayLevel) >> 16;
                    value = (value * PV_GetWaveShape(rec->currentWaveValue, rec->waveShape)) >> 16;
                }
                else
                {
                    value = 0;
                }
                value += (adsrLevel * rec->DC_feed) >> 16;
                switch (rec->where_to_feed)
                {
                    case PITCH_LFO:
                        n += value;
                        break;
                    case VOLUME_LFO:
                        pVoice->volumeLFOValue += value;
                        break;
                    case STEREO_PAN_LFO:
                    case STEREO_PAN_NAME2:
                        pVoice->stereoPanBend += value;
                        break;
                    case LPF_FREQUENCY:
                        if (pVoice->LPF_base_frequency <= 0)
                            pVoice->LPF_frequency = value;
                        else
                            pVoice->LPF_frequency = pVoice->LPF_base_frequency - value;
                        break;
                    case LOW_PASS_AMOUNT:
                        pVoice->LPF_lowpassAmount += value;
                        break;
                    case LPF_DEPTH:
                        pVoice->LPF_resonance += value;
                        break;
                    default:
                        //DEBUG_STR("\p Invalid LFO Unit Feed-To");
                        break;
                }
            } // if rec->level
        } // for ()
    } // if LFORecordCount

    // to test notes with only pitch bend or LFO applied:  
/*
    if (n == 0) 
    {
        PV_UnlockInstrumentAndVoice(pVoice);    // done processing
        return;
    }
*/
// If pitchbend value has changed, recalculate the pitch data for this instrument
    if (pVoice->LPF_base_frequency <= 0)
    {
        pVoice->LPF_frequency = 
            resonantFilterLookup[(n + ((pVoice->NoteMIDIPitch - pVoice->LPF_base_frequency) << 8) + 0x80) >> 8]
             * 256 + pVoice->LPF_frequency;
    }

    if (n != pVoice->LastPitchBend)
    {
        pVoice->LastPitchBend = (INT16)n;
        n += pVoice->ProcessedPitch << 8;       // ProcessedPitch is based on the sample data and MIDI pitch.
    
    // Clip value to within reasonable MIDI pitch range
        if (n < -0x1800)
            n += 0x0c00;
        if (n < -0x1800)
            n += 0x0c00;
        if (n < -0x1800)
            n = -0x1800;
        if (n > 0x08C00)
            n -= 0x0C00;        // 12 (one octave) in 8.8 Fixed form
        if (n > 0x08C00)
            n -= 0x0C00;        // 12 (one octave) in 8.8 Fixed form
        if (n > 0x08C00)
            n -= 0x0C00;        // 12 (one octave) in 8.8 Fixed form
        if (n > 0x08C00)
            n = 0x0C00;     // 12 (one octave) in 8.8 Fixed form
    // Process whole pitch value in semitones
        pVoice->NotePitch = majorPitchTable[(n >> 8) + 24];
    // Process fractional semitone values
        pVoice->NotePitch = XFixedMultiply(pVoice->NotePitch, fractionalPitchTable[(n & 0xFF) >> 1]);
    // factor in sample rate of sample, if enabled
        pVoice->NotePitch = XFixedMultiply(pVoice->noteSamplePitchAdjust, pVoice->NotePitch);

    // Recalculate number of samples in a slice
        pVoice->NoteNextSize = 0;
    }

    start = 0;
    end = 0;
    size = 0;
    switch (pMixer->interpolationMode)
    {
#if LOOPS_USED == U3232_LOOPS
        case E_LINEAR_INTERPOLATION_U3232:
            if (pVoice->NoteNextSize == 0)
            {
#if USE_FLOAT == FALSE
// use fixed code
                XFIXED  fwave_increment, result;

                fwave_increment = PV_GetWavePitchFixed(pVoice->NotePitch);

                result = (fwave_increment * (pMixer->One_Slice + 1)) / XFIXED_1;
                pVoice->NoteNextSize = (INT16)result;
#else
// use floating point code
                UFLOAT  fwave_increment;

                fwave_increment = PV_GetWavePitchFloat(pVoice->NotePitch);
                pVoice->NoteNextSize = (INT16)(fwave_increment * (pMixer->One_Slice + 1)) + 1;
#endif
            }
            size = pVoice->NoteNextSize;
            start = pVoice->samplePosition.i;
            end = pVoice->NotePtrEnd - pVoice->NotePtr;
            break;
#elif LOOPS_USED == FLOAT_LOOPS
        case E_LINEAR_INTERPOLATION_FLOAT:
            if (pVoice->NoteNextSize == 0)
            {
                UFLOAT  fwave_increment;

                fwave_increment = PV_GetWavePitchFloat(pVoice->NotePitch);
                pVoice->NoteNextSize = (INT16)(fwave_increment * (pMixer->One_Slice + 1)) + 1;
            }
            size = pVoice->NoteNextSize;
            start = pVoice->samplePosition_f;
            end = pVoice->NotePtrEnd - pVoice->NotePtr;
            break;
#elif LOOPS_USED == LIMITED_LOOPS
        default:
            if (pVoice->NoteNextSize == 0)
            {
                register XFIXED         wave_increment;

                wave_increment = PV_GetWavePitch(pVoice->NotePitch);
                pVoice->NoteNextSize = 
                    (INT16)((wave_increment * (pMixer->One_Slice + 1) + STEP_FULL_RANGE) >> STEP_BIT_RANGE);
            }
            size = pVoice->NoteNextSize;
            start = pVoice->NoteWave >> STEP_BIT_RANGE;
            end = (XBYTE *)pVoice->NotePtrEnd - (XBYTE *)pVoice->NotePtr;
            break;
#endif
    }

// This is sure easier than the LFO modules!
    PV_ADSRModule (&(pVoice->volumeADSRRecord),
                    (XBOOL)((pVoice->voiceMode == VOICE_SUSTAINING) ||
                            (pVoice->sustainMode == SUS_ON_NOTE_ON)));

    // now reduce the current volume by the sustainDecayLevel which is fixed point
    pVoice->NoteVolumeEnvelope = (INT16)XFixedMultiply(pVoice->volumeADSRRecord.currentLevel, 
                                                    pVoice->volumeADSRRecord.sustainingDecayLevel);
    //BAE_PRINTF("cl = %ld sdl = %ld\n", (long)pVoice->volumeADSRRecord.currentLevel, 
    //                                  (long)pVoice->volumeADSRRecord.sustainingDecayLevel);
    //BAE_PRINTF("nve = %ld\n", (long)pVoice->NoteVolumeEnvelope);

    pVoice->NoteVolumeEnvelopeBeforeLFO = pVoice->NoteVolumeEnvelope;
    //BAE_PRINTF("2;NoteVolumeEnvelopeBeforeLFO = %ld\n", (long)pVoice->NoteVolumeEnvelopeBeforeLFO);
    if (pVoice->volumeLFOValue >= 0)        // don't handle volume LFO values less than zero.
    {
        pVoice->NoteVolumeEnvelope = (INT16)((pVoice->NoteVolumeEnvelope * pVoice->volumeLFOValue) >> 12L);
    }
    //BAE_PRINTF("3;NoteVolumeEnvelope = %ld\n", (long)pVoice->NoteVolumeEnvelope);

    // now modify the pVoice->NoteVolumeEnvelope to zero, if this voice doesn't match
    // the current audio route
    /*
    if (pVoice->routeBus != pMixer->routeBus)
    {
        pVoice->NoteVolumeEnvelope = 0; // no joy for voice
        pVoice->lastAmplitudeL = 0;
        pVoice->lastAmplitudeR = 0;
        pVoice->NoteVolume = 0;
    }
    */
    if (pVoice->NoteLoopEnd)
    {
        loopend = (unsigned long)pVoice->NoteLoopEnd - (unsigned long)pVoice->NotePtr;
    }
    else
    {
        loopend = 0;
    }

    if (loopend > end)
    {
        loopend = 0;
    }
    // if our current sample position, because of pitch changes, might exceed the end point;
    // if that is the case, and we are not looping, limit it to the end.
    if (loopend == 0 && start > end)
    {
        start = end;
    }
    //BAE_PRINTF("start %ld end %ld size %ld loopend %ld\n", start, end, size, loopend);
    // At end of loop, continue loop?
    if ((loopend) && (start + size > loopend))
    {
        //$$kk: 01.14.98:   changes to allow a sample to start, loop NoteLoopTarget between the loop points, 
        //                  and then play to the end 

        // we can keep looping if the NoteLoopTarget is set to a valid value (>0) and we haven't reached it yet.
        // if it's set to <= 0, we loop continuously unless the NoteLoopProc tells us to quit.
        if ((pVoice->NoteLoopCount < pVoice->NoteLoopTarget) || (pVoice->NoteLoopTarget <= 0))
        {
            pVoice->NoteLoopCount++;
#if USE_CALLBACKS
            if (pVoice->NoteLoopProc)
            {
                // continue loop?
                if ((*pVoice->NoteLoopProc)(pVoice->NoteContext) == FALSE)
                {
                    // nope
                    pVoice->NoteLoopProc = NULL;
                    pVoice->NoteLoopPtr = NULL;
                    pVoice->NoteLoopEnd = NULL;
                    pVoice->voiceMode = VOICE_SUSTAINING;       // let rest of sample play out
                    loopend = 0;

                    pVoice->NoteLoopCount = 0;
                }
                else
                {
                    // yes, so refresh note duration counter, otherwise sample will stop after 8.9 minutes
                    pVoice->voiceMode = VOICE_SUSTAINING;
                }
            } // if (pVoice->NoteLoopProc)
#endif
        } // can keep looping
        else
        // stop looping
        {
            pVoice->voiceMode = VOICE_SUSTAINING;       // let rest of sample play out
            loopend = 0;
        }
    }

    if ((pVoice->volumeADSRRecord.ADSRTime[0] != 0) || (pVoice->volumeADSRRecord.ADSRFlags[0] != ADSR_TERMINATE) || (pVoice->sampleAndHold != 0) )
    {
// New style volume ADSR instruments
        if (pVoice->voiceMode == VOICE_RELEASING)
        {
            if (loopend == 0)
            {
                goto ENDING;        // this case handles sample-and-release or one-shot instruments
            }
            if ((pVoice->volumeADSRRecord.mode == ADSR_TERMINATE) || (pVoice->volumeADSRRecord.mode == ADSR_RELEASE))
            {
                if (pVoice->sampleAndHold == 0)
                {
                    goto ENDING;
                }
            }
            goto LOOPING;       // this case handles sample-and-hold instruments
        }
        else
        {
            if (loopend == 0)
            {
                goto ENDING;
            }
            else
            {
                goto LOOPING;
            }
        }
    }
    else
    {
// Old style instrument decay
        if ((pVoice->voiceMode == VOICE_RELEASING) || (loopend == 0))
        {
            if ((pVoice->voiceMode == VOICE_RELEASING) && (pVoice->NoteDecay == 0))
            {
                // Interpolation of volume ADSR levels will smooth out the transition to note termination.
                pVoice->NoteVolumeEnvelope = 0;
                pVoice->NoteVolumeEnvelopeBeforeLFO = 0;
                goto PARTIAL;
            }
ENDING:
            if ( end <= start + size )
            {
PARTIAL:
                if (((pVoice->LPF_lowpassAmount != 0) || (pVoice->LPF_resonance != 0)) && (pVoice->channels == 1))
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->filterPartialBufferProc16(pVoice, FALSE);
                    }
                    else
                    {
                        pMixer->filterPartialBufferProc(pVoice, FALSE);
                    }
                }
                else
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->partialBufferProc16(pVoice, FALSE);
                    }
                    else
                    {
                        pMixer->partialBufferProc(pVoice, FALSE);
                    }
                }
            }
            else
            {
                if (((pVoice->LPF_lowpassAmount != 0) || (pVoice->LPF_resonance != 0)) && (pVoice->channels == 1))
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->filterFullBufferProc16(pVoice);
                    }
                    else
                    {
                        pMixer->filterFullBufferProc(pVoice);
                    }
                }
                else
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->fullBufferProc16(pVoice);
                    }
                    else
                    {
                        pMixer->fullBufferProc(pVoice);
                    }
                }
            }
        }
        else
        {
LOOPING:
            if (loopend > (start + size) )
            {
                if (((pVoice->LPF_lowpassAmount != 0) || (pVoice->LPF_resonance != 0)) && (pVoice->channels == 1))
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->filterFullBufferProc16(pVoice);
                    }
                    else
                    {
                        pMixer->filterFullBufferProc(pVoice);
                    }
                }
                else
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->fullBufferProc16(pVoice);
                    }
                    else
                    {
                        pMixer->fullBufferProc(pVoice);
                    }
                }
            }
            else
            {
                if (((pVoice->LPF_lowpassAmount != 0) || (pVoice->LPF_resonance != 0)) && (pVoice->channels == 1))
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->filterPartialBufferProc16(pVoice, TRUE);
                    }
                    else
                    {
                        pMixer->filterPartialBufferProc(pVoice, TRUE);
                    }
                }
                else
                {
                    if (pVoice->bitSize == 16)
                    {
                        pMixer->partialBufferProc16(pVoice, TRUE);
                    }
                    else
                    {
                        pMixer->partialBufferProc(pVoice, TRUE);
                    }
                }
            }
        }
    }

//DONE
    if (pVoice->voiceMode == VOICE_RELEASING)
    {
        if ((pVoice->volumeADSRRecord.ADSRTime[0] != 0) || (pVoice->volumeADSRRecord.ADSRFlags[0] != ADSR_TERMINATE))
        {
// Handle new style volume ADSR's
            if (pVoice->volumeADSRRecord.mode == ADSR_TERMINATE)
            {
                if ((pVoice->volumeADSRRecord.currentLevel < 0x100) || (pVoice->volumeADSRRecord.sustainingDecayLevel < 0x100))
                {
#if USE_CALLBACKS
                    PV_DoCallBack(pVoice);
#endif
                    pVoice->voiceMode = VOICE_UNUSED;
#ifdef BAE_MCU
                    GM_KillVoiceOnDSP(pVoice);
#endif
                }
            }
            else
            {
                if (pVoice->volumeADSRRecord.sustainingDecayLevel == 0)
                {
#if USE_CALLBACKS
                    PV_DoCallBack(pVoice);
#endif
                    pVoice->voiceMode = VOICE_UNUSED;
#ifdef BAE_MCU
                    GM_KillVoiceOnDSP(pVoice);
#endif
                }
                // If low in volume, fade it out gracefully next cycle
                if (pVoice->volumeADSRRecord.sustainingDecayLevel < 0x800)
                {
                    pVoice->volumeADSRRecord.sustainingDecayLevel = 0;
                }
            }
        }
        else
        {
// old style scheme with NoteDecay values and so forth
            if (pVoice->NoteDecay == 0)
            {
#if USE_CALLBACKS
                PV_DoCallBack(pVoice);
#endif
                pVoice->voiceMode = VOICE_UNUSED;
#ifdef BAE_MCU
                GM_KillVoiceOnDSP(pVoice);
#endif
            }
            else
            {
                pVoice->NoteDecay--;
            }
        }
    }
    PV_UnlockInstrumentAndVoice(pVoice);    // done processing
}

#ifdef BAE_COMPLETE
static void PV_ClearReverbBuffer()
{
#if REVERB_USED == VARIABLE_REVERB
    if (GM_IsReverbFixed() == FALSE)
    {
        register INT32  *destL = &MusicGlobals->songBufferReverb[0];
        register LOOPCOUNT  count, four_loop = MusicGlobals->Four_Loop;

        for (count = 0; count < four_loop; count++)
        {
            destL[0] = 0;
            destL[1] = 0;
            destL[2] = 0;
            destL[3] = 0;
            destL += 4;
        }
    }
#endif
}
#endif

#ifdef BAE_COMPLETE
static void PV_ClearChorusBuffer()
{
#if USE_NEW_EFFECTS
    register INT32  *destL = &MusicGlobals->songBufferChorus[0];
    register LOOPCOUNT  count, four_loop = MusicGlobals->Four_Loop;

    for (count = 0; count < four_loop; count++)
    {
        destL[0] = 0;
        destL[1] = 0;
        destL[2] = 0;
        destL[3] = 0;
        destL += 4;
    }
#endif
}
#endif

#ifdef BAE_COMPLETE
INLINE static void PV_ClearMixBuffers(XBOOL doStereo)
{
    register INT32      *destL;
    register LOOPCOUNT  count, four_loop;

    destL = &MusicGlobals->songBufferDry[0];
    four_loop = MusicGlobals->Four_Loop;
    if (doStereo)
    {   // stereo
#if USE_STEREO_OUTPUT == TRUE
        for (count = 0; count < four_loop; count++)
        {
            destL[0] = 0;
            destL[1] = 0;
            destL[2] = 0;
            destL[3] = 0;
            
            destL[4] = 0;
            destL[5] = 0;
            destL[6] = 0;
            destL[7] = 0;
            destL += 8;
        }
#endif
    }
    else
    {   // mono
#if USE_MONO_OUTPUT == TRUE
        for (count = 0; count < four_loop; count++)
        {
            destL[0] = 0;
            destL[1] = 0;
            destL[2] = 0;
            destL[3] = 0;
            destL+= 4;
        }
#endif
    }
    
    PV_ClearReverbBuffer();
    PV_ClearChorusBuffer();
}
#endif

#if REVERB_USED == DISABLE_REVERB
// Process active sample voices
INLINE static void PV_ServeInstruments(void)
{
    register GM_Mixer   *pMixer;
    register LOOPCOUNT  count;
    register GM_Voice   *pVoice;
    XBOOL               someSoundActive;

    pMixer = MusicGlobals;
    // Process active voices for the inexpensive reverb cases:
    // Notes with reverb on are processed first, then the reverb unit, then the dry notes.
    someSoundActive = FALSE;
    for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->voiceMode != VOICE_UNUSED)
        {
            PV_ServeThisInstrument(pVoice);
            someSoundActive = TRUE;
        }
    }
}
#else
// Process active sample voices
INLINE static void PV_ServeInstruments(void)
{
    register GM_Mixer   *pMixer;
    register LOOPCOUNT  count;
    register GM_Voice   *pVoice;
    XBOOL               someSoundActive;

    pMixer = MusicGlobals;
#if REVERB_USED == VARIABLE_REVERB
    if (GM_IsReverbFixed() == FALSE)
    {
        // Process all active voices in the full-featured variable reverb case.
        someSoundActive = FALSE;
        for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
        {
            pVoice = &pMixer->NoteEntry[count];
            if (pVoice->voiceMode != VOICE_UNUSED)
            {
                PV_ServeThisInstrument(pVoice);
                someSoundActive = TRUE;
            }
        }
#if USE_NEW_EFFECTS
        RunChorus(pMixer->songBufferChorus, pMixer->songBufferDry, pMixer->One_Loop);
#endif
        GM_ProcessReverb();
    }
    else
#endif
    {
        // Process active voices for the inexpensive reverb cases:
        // Notes with reverb on are processed first, then the reverb unit, then the dry notes.
        someSoundActive = FALSE;
        for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
        {
            pVoice = &pMixer->NoteEntry[count];
            if (pVoice->voiceMode != VOICE_UNUSED)
            {
                if (pVoice->avoidReverb == FALSE)
                {
                    PV_ServeThisInstrument(pVoice);
                    someSoundActive = TRUE;
                }
            }
        }
#if USE_NEW_EFFECTS
        RunChorus(pMixer->songBufferChorus, pMixer->songBufferDry, pMixer->One_Loop);
#endif
        GM_ProcessReverb();

        for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
        {
            pVoice = &pMixer->NoteEntry[count];
            if (pVoice->voiceMode != VOICE_UNUSED)
            {
                if (pVoice->avoidReverb != FALSE)
                {
                    PV_ServeThisInstrument(pVoice);
                    someSoundActive = TRUE;
                }
            }
        }
    }
}
#endif  // REVERB_TYPE

#define GENERATE_TONE       0       // will generate a tone ranther than engine. Used for debugging

static int          gToneOn = GENERATE_TONE;
static int          gToneFreq = 1000;
static int          gTonePhase = 0;


static void PV_FillTone(void *pBuffer, unsigned long toneFrames, int bitSize, int stereo)
{
    char            *pAudioB;
    short           *pAudioW;
    unsigned long   count;
    unsigned long   mixerRate = GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate);
    XFIXED          v;
    long            angle, ta;
    short           value = 0;

    pAudioB = (char *)pBuffer;
    pAudioW = (short *)pBuffer;

    for (count = 0; count < toneFrames; count++)
    {
        // 2 * pi * a1 * t = rads
        // (180 / pi) * 2 * pi * a1 * t = degs
        // 360 * a1 * t = degs
        ta = gTonePhase * 36;
        angle = (gToneFreq * ta) / (mixerRate/10);
        v = XFixedSin(angle);

        value = (short)XFixedFloor(XFixedMultiply(v, 32766<<16L) + 0x8000);

        if (bitSize == 2)   // 16 bit
        {
            *pAudioW++ = value;
            if (stereo == 2)
            {
                *pAudioW++ = value;
            }
        }
        if (bitSize == 1)
        {
            *pAudioB++ = value;
            if (stereo == 2)
            {
                *pAudioB++ = value;
            }
        }
        gTonePhase++;
    }
}

void GM_TestToneFrequency(XFIXED freq)
{
    GM_Mixer        *pMixer;

    pMixer = GM_GetCurrentMixer();
    if (pMixer)
    {
        gToneFreq = freq >> 16L;
        gTonePhase = 0;
    }
}

void GM_TestTone(XBOOL toneStatus)
{
    GM_TestToneFrequency(gToneFreq << 16L);
    gToneOn = toneStatus;
}


#ifdef BAE_MCU
// build next frame. Causes messages to be sent to the DSP. Calls GM_ProcessSyncUpdateFromDSP
void BAE_BuildMCUSlice(void * threadContext, XDWORD dspTime)
{
    GM_Mixer        *pMixer;
    unsigned long   delta, end;
    OPErr           err;

    pMixer = GM_GetCurrentMixer();
    if (pMixer)
    {
        delta = XMicroseconds();        // get current time

        pMixer->insideAudioInterrupt = 1;   // busy

        pMixer->syncCount += BAE_GetSliceTimeInMicroseconds();          // 11 milliseconds
        pMixer->syncBufferCount++;

        err = GM_ProcessSyncUpdateFromDSP(dspTime);

#if USE_CALLBACKS
        if (pMixer->pTaskProc)
        {
            (*pMixer->pTaskProc)(threadContext, pMixer->taskReference);
        }
        if (pMixer->pOutputProc)
        {
            (*pMixer->pOutputProc)(threadContext, pAudioBuffer, 
                                (pMixer->generate16output) ? 2 : 1,
                                (pMixer->generateStereoOutput) ? 2 : 1,
                                sampleFrames);
                                
        }
#endif
//      pMixer->samplesWritten += sampleFrames;

        GM_UpdateSamplesPlayed(BAE_GetDeviceSamplesPlayedPosition());
        pMixer->insideAudioInterrupt = 0;   // free

        end = XMicroseconds();
        if (end < delta)
        {   // we wrapped
            pMixer->timeSliceDifference = delta - end;
        }
        else
        {
            pMixer->timeSliceDifference = end - delta;
        }
    }
}
#endif


// **** Audio Engine feedback functions. These functions are used to direct or get
//      information about the engine.
//
// NOTE:    This is an external function that your audio card code calls to process 
//          a mixer buffer. You call this, don't define it.
//
// Based upon sample rate, channels, and bit size, build 11 milliseconds worth of mixer
// output, and store it in pAudioBuffer.
// bufferByteLength is assumed to be 256, 512, 1024, or 2048 only. The value is ignored
// at the moment. sampleFrames is how many sample frames. These two values should match
// ie. sampleFrames = bufferByteLength / channels / bitsize / 8
#if BAE_COMPLETE
void BAE_BuildMixerSlice(void *threadContext, void *pAudioBuffer, long bufferByteLength,
                                                            long sampleFrames)
{
    GM_Mixer        *pMixer;
    unsigned long   delta, end;

    pMixer = MusicGlobals;
    if (pMixer && pAudioBuffer && bufferByteLength && sampleFrames)
    {
        delta = XMicroseconds();        // get current time

        pMixer->insideAudioInterrupt = 1;   // busy

        pMixer->syncCount += BAE_GetSliceTimeInMicroseconds();          // 11 milliseconds
        pMixer->syncBufferCount++;

        // Generate new audio samples, putting them directly 
        // into the output buffer.
        PV_ProcessSampleFrame(threadContext, pAudioBuffer);

        if (gToneOn)
        {
            PV_FillTone(pAudioBuffer, sampleFrames,
                                (pMixer->generate16output) ? 2 : 1,
                                (pMixer->generateStereoOutput) ? 2 : 1);

        }
#if USE_CALLBACKS
        if (pMixer->pTaskProc)
        {
            (*pMixer->pTaskProc)(threadContext, pMixer->taskReference);
        }
        if (pMixer->pOutputProc)
        {
            (*pMixer->pOutputProc)(threadContext, pAudioBuffer, 
                                (pMixer->generate16output) ? 2 : 1,
                                (pMixer->generateStereoOutput) ? 2 : 1,
                                sampleFrames);
                                
        }
#endif
        pMixer->samplesWritten += sampleFrames;

        GM_UpdateSamplesPlayed(BAE_GetDeviceSamplesPlayedPosition());
        pMixer->insideAudioInterrupt = 0;   // free

        end = XMicroseconds();
        if (end < delta)
        {   // we wrapped
            pMixer->timeSliceDifference = delta - end;
        }
        else
        {
            pMixer->timeSliceDifference = end - delta;
        }
    }
}
#endif

#if BAE_COMPLETE
// Get time in microseconds between calls to BAE_BuildMixerSlice
XDWORD GM_GetMixerUsedTime(void)
{
    XDWORD   time;

    time = 0;
    if (MusicGlobals)
    {
        time = MusicGlobals->timeSliceDifference;
    }
    return time;
}
#endif

#if BAE_COMPLETE
// Get CPU load in percent. This function is realtime and assumes the mixer has been allocated
XDWORD GM_GetMixerUsedTimeInPercent(void)
{
    XDWORD load;

    load = 0;
    if (MusicGlobals)
    {
        load = GM_GetMixerUsedTime() * 100;     // 100 %
        load = (load / BAE_GetSliceTimeInMicroseconds());
    }
    return load;
}
#endif

// Return the maximumn number of samples for 11 milliseconds worth of whatever khz data.
// Typically this is 512. Use this in your calculation of audio buffers. Will return
// 0 if something is wrong.
short int BAE_GetMaxSamplePerSlice(void)
{
    if (MusicGlobals)
    {
        return MusicGlobals->maxChunkSize;
    }
    return 0;
}

// This function will scan for voices that are ready to be started automatically.
// We do it here, because we are guaranteed that no one else is looking at these
// structures, and that the voices will start at exactly the same time.
static void PV_ProcessSyncronizedVoiceStart(void)
{
    GM_Voice        *pArrayToStart[MAX_VOICES];
    GM_Mixer        *pMixer;
    GM_Voice        *pVoice;
    void            *syncReference;
    LOOPCOUNT       count, max;
    unsigned long   time;

    pMixer = GM_GetCurrentMixer();

    max = pMixer->MaxNotes + pMixer->MaxEffects;
    // first, we scan for all voices that are ready to be started, then we 
    // gather all voices that match a particular reference
    syncReference = NULL;
    for (count = 0; count < max; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        pArrayToStart[count] = NULL;    // clear
        if (pVoice->voiceMode == VOICE_ALLOCATED_READY_TO_SYNC_START)
        {
            if (syncReference == NULL)  // got to set the first voice reference
            {
                syncReference = pVoice->syncVoiceReference;
            }
            // does this voice match our reference?
            if (pVoice->syncVoiceReference == syncReference)
            {
                pArrayToStart[count] = pVoice;
            }
        }
    }
    time = XMicroseconds();
    // ok, now we have a list of voices that want to be started
    for (count = 0; count < max; count++)
    {
        pVoice = pArrayToStart[count];
        if (pVoice)
        {   // fire voice
            pVoice->voiceStartTimeStamp = time;
            pVoice->voiceMode = VOICE_SUSTAINING;
            pVoice->syncVoiceReference = NULL;
        }
    }
}

#ifdef BAE_MCU
// setup, runtime, a valid sample process loop. Either filtered, or not, reverb/chorused or not.
// Return TRUE, if everything is valid.
static XBOOL PV_SetupProcessFunctions(GM_Mixer *pMixer)
{
    if (pMixer)
    {
        if (pMixer->generateStereoOutput)
        {
            pMixer->fullBufferProc      = GM_DSPMix8SFullBuffer;    // 8 bit, stereo
            pMixer->partialBufferProc   = GM_DSPMix8SPartialBuffer;
            pMixer->fullBufferProc16    = GM_DSPMix16SFullBuffer;   // 16 bit, stereo
            pMixer->partialBufferProc16 = GM_DSPMix16SPartialBuffer;

#if 1   // filters on
            pMixer->filterFullBufferProc        = GM_DSPMix8SFilterFullBuffer;
            pMixer->filterPartialBufferProc     = GM_DSPMix8SFilterPartialBuffer;
            pMixer->filterFullBufferProc16      = GM_DSPMix16SFilterFullBuffer;
            pMixer->filterPartialBufferProc16   = GM_DSPMix16SFilterPartialBuffer;
#else
            pMixer->filterFullBufferProc        = GM_DSPMix8SFullBuffer;
            pMixer->filterPartialBufferProc     = GM_DSPMix8SPartialBuffer;
            pMixer->filterFullBufferProc16      = GM_DSPMix16SFullBuffer;
            pMixer->filterPartialBufferProc16   = GM_DSPMix16SPartialBuffer;
#endif
        }
        else
        {
            pMixer->fullBufferProc      = GM_DSPMix8MFullBuffer;    // 8 bit, mono
            pMixer->partialBufferProc   = GM_DSPMix8MPartialBuffer;
            pMixer->fullBufferProc16    = GM_DSPMix16MFullBuffer;   // 16 bit, mono
            pMixer->partialBufferProc16 = GM_DSPMix16MPartialBuffer;

#if 1   // filters on
            pMixer->filterFullBufferProc        = GM_DSPMix8MFilterFullBuffer;
            pMixer->filterPartialBufferProc     = GM_DSPMix8MFilterPartialBuffer;
            pMixer->filterFullBufferProc16      = GM_DSPMix16MFilterFullBuffer;
            pMixer->filterPartialBufferProc16   = GM_DSPMix16MFilterPartialBuffer;
#else
            pMixer->filterFullBufferProc        = GM_DSPMix8MFullBuffer;
            pMixer->filterPartialBufferProc     = GM_DSPMix8MPartialBuffer;
            pMixer->filterFullBufferProc16      = GM_DSPMix16MFullBuffer;
            pMixer->filterPartialBufferProc16   = GM_DSPMix16MPartialBuffer;
#endif
        }
    }
    return TRUE;
}
#endif

#ifdef BAE_COMPLETE
// setup, runtime, a valid sample process loop. Either filtered, or not, reverb/chorused or not.
// Return TRUE, if everything is valid.
static XBOOL PV_SetupProcessFunctions(GM_Mixer *pMixer)
{
    XBOOL           okdoky;
    
    okdoky = TRUE;
// Set up the various procs for mixdown:
    switch (pMixer->interpolationMode)
    {
    #if (LOOPS_USED==LIMITED_LOOPS && USE_DROP_SAMPLE==TRUE)
        case E_AMP_SCALED_DROP_SAMPLE:
            if (pMixer->generateStereoOutput)
            {
                pMixer->fullBufferProc      = PV_ServeStereoAmpFullBuffer;
                pMixer->partialBufferProc   = PV_ServeStereoAmpPartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeStereoInterp2FullBuffer16;    // notice:  we use terp 2 cases
                pMixer->partialBufferProc16 = PV_ServeStereoInterp2PartialBuffer16; //          for 16 bit samples
            }
            else
            {
                pMixer->fullBufferProc      = PV_ServeDropSampleFullBuffer;
                pMixer->partialBufferProc   = PV_ServeDropSamplePartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeDropSampleFullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeDropSamplePartialBuffer16;
            }
            break;
    #endif
    #if (LOOPS_USED==LIMITED_LOOPS && USE_TERP1==TRUE)
        case E_2_POINT_INTERPOLATION:
            if (pMixer->generateStereoOutput)
            {
                pMixer->fullBufferProc      = PV_ServeStereoInterp1FullBuffer;
                pMixer->partialBufferProc   = PV_ServeStereoInterp1PartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeStereoInterp2FullBuffer16;    // notice:  we use terp 2 cases
                pMixer->partialBufferProc16 = PV_ServeStereoInterp2PartialBuffer16; //          for 16 bit samples
            }
            else
            {
                pMixer->fullBufferProc      = PV_ServeInterp1FullBuffer;
                pMixer->partialBufferProc   = PV_ServeInterp1PartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeInterp2FullBuffer16;          // notice:  we use terp 2 cases
                pMixer->partialBufferProc16 = PV_ServeInterp2PartialBuffer16;       //          for 16 bit samples
            }
            break;
    #endif
    #if LOOPS_USED == FLOAT_LOOPS
        case E_LINEAR_INTERPOLATION_FLOAT:
            if (pMixer->generateStereoOutput)
            {
                pMixer->fullBufferProc      = PV_ServeStereoFloatFullBuffer;
                pMixer->partialBufferProc   = PV_ServeStereoFloatPartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeStereoFloatFullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeStereoFloatPartialBuffer16;
            }
            else
            {
                pMixer->fullBufferProc      = PV_ServeFloatFullBuffer;
                pMixer->partialBufferProc   = PV_ServeFloatPartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeFloatFullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeFloatPartialBuffer16;
            }
            break;
    #endif
        case E_LINEAR_INTERPOLATION_U3232:
    #if LOOPS_USED == U3232_LOOPS
            if (pMixer->generateStereoOutput)
            {
                pMixer->fullBufferProc      = PV_ServeU3232StereoFullBuffer;
                pMixer->partialBufferProc   = PV_ServeU3232StereoPartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeU3232StereoFullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeU3232StereoPartialBuffer16;
            }
            else
            {
                pMixer->fullBufferProc      = PV_ServeU3232FullBuffer;
                pMixer->partialBufferProc   = PV_ServeU3232PartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeU3232FullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeU3232PartialBuffer16;
            }
            break;
    #endif

    #if LOOPS_USED == LIMITED_LOOPS && USE_TERP2 == TRUE
        case E_LINEAR_INTERPOLATION:
            if (pMixer->generateStereoOutput)
            {
                pMixer->fullBufferProc      = PV_ServeStereoInterp2FullBuffer;
                pMixer->partialBufferProc   = PV_ServeStereoInterp2PartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeStereoInterp2FullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeStereoInterp2PartialBuffer16;
            }
            else
            {
                pMixer->fullBufferProc      = PV_ServeInterp2FullBuffer;
                pMixer->partialBufferProc   = PV_ServeInterp2PartialBuffer;
                pMixer->fullBufferProc16    = PV_ServeInterp2FullBuffer16;
                pMixer->partialBufferProc16 = PV_ServeInterp2PartialBuffer16;
            }
            break;
    #endif
        default:
            okdoky = FALSE;
            break;
    }

// Set up the various procs for filters:
    switch (pMixer->interpolationMode)
    {
    #if LOOPS_USED == FLOAT_LOOPS
        case E_LINEAR_INTERPOLATION_FLOAT:
            if (pMixer->generateStereoOutput)
            {
                pMixer->filterPartialBufferProc     = PV_ServeStereoFloatFilterPartialBuffer;
                pMixer->filterPartialBufferProc16   = PV_ServeStereoFloatFilterPartialBuffer16;
                pMixer->filterFullBufferProc        = PV_ServeStereoFloatFilterFullBuffer;
                pMixer->filterFullBufferProc16      = PV_ServeStereoFloatFilterFullBuffer16;
            }
            else
            {
                pMixer->filterPartialBufferProc     = PV_ServeFloatFilterPartialBuffer;
                pMixer->filterPartialBufferProc16   = PV_ServeFloatFilterPartialBuffer16;
                pMixer->filterFullBufferProc        = PV_ServeFloatFilterFullBuffer;
                pMixer->filterFullBufferProc16      = PV_ServeFloatFilterFullBuffer16;
            }
            break;
    #elif LOOPS_USED == U3232_LOOPS
        case E_LINEAR_INTERPOLATION_U3232:
            if (pMixer->generateStereoOutput)
            {
                pMixer->filterPartialBufferProc     = PV_ServeU3232StereoFilterPartialBuffer;
                pMixer->filterPartialBufferProc16   = PV_ServeU3232StereoFilterPartialBuffer16;
                pMixer->filterFullBufferProc        = PV_ServeU3232StereoFilterFullBuffer;
                pMixer->filterFullBufferProc16      = PV_ServeU3232StereoFilterFullBuffer16;
            }
            else
            {
                pMixer->filterPartialBufferProc     = PV_ServeU3232FilterPartialBuffer;
                pMixer->filterPartialBufferProc16   = PV_ServeU3232FilterPartialBuffer16;
                pMixer->filterFullBufferProc        = PV_ServeU3232FilterFullBuffer;
                pMixer->filterFullBufferProc16      = PV_ServeU3232FilterFullBuffer16;
            }
            break;
    #elif ((LOOPS_USED == LIMITED_LOOPS) && (USE_DROP_SAMPLE==TRUE || USE_TERP1==TRUE || USE_TERP2==TRUE))
        default:
            // notice:  we always use terp2 cases for filters
            if (pMixer->generateStereoOutput)
            {
                pMixer->filterPartialBufferProc     = PV_ServeStereoInterp2FilterPartialBuffer;
                pMixer->filterPartialBufferProc16   = PV_ServeStereoInterp2FilterPartialBuffer16;
                pMixer->filterFullBufferProc        = PV_ServeStereoInterp2FilterFullBuffer;
                pMixer->filterFullBufferProc16      = PV_ServeStereoInterp2FilterFullBuffer16;
            }
            else
            {
                pMixer->filterPartialBufferProc     = PV_ServeInterp2FilterPartialBuffer;
                pMixer->filterPartialBufferProc16   = PV_ServeInterp2FilterPartialBuffer16;
                pMixer->filterFullBufferProc        = PV_ServeInterp2FilterFullBuffer;
                pMixer->filterFullBufferProc16      = PV_ServeInterp2FilterFullBuffer16;
            }
            break;
    #else
        default:
            // this is an error condition. nothing is selected, so just bail
            okdoky = FALSE;
            break;
    #endif
    }
    return okdoky;
}
#endif

#ifdef BAE_MCU
// process next frame
OPErr GM_ProcessSyncUpdateFromDSP(XDWORD dspTime)
{
    GM_Mixer        *pMixer;

    dspTime;
    pMixer = GM_GetCurrentMixer();
    if (pMixer)
    {
        if (PV_SetupProcessFunctions(pMixer) == FALSE)
        {   // something happend. Code is not compiled correctly, etc
            return NOT_SETUP;
        }

        if (pMixer->systemPaused == FALSE)
        {
            // ok, start any voices in sync that need it
            PV_ProcessSyncronizedVoiceStart();

            // process enabled voices, and add verb, and filter
            PV_ServeInstruments();

            PV_ProcessSequencerEvents(NULL);        // process all songs and external events
            // process sound effects fade
            PV_ServeEffectsFades();
    #if USE_CALLBACKS
            // process sound effects callbacks
            PV_ServeEffectCallbacks(threadContext);
    #endif

    #if USE_STREAM_API
            // process stream fades
            PV_ServeStreamFades();
    #endif
        }
    }
    return NO_ERROR;
}
#endif


#if BAE_COMPLETE
// build one frame of audio output
void PV_ProcessSampleFrame(void *threadContext, void *destinationSamples)
{
    GM_Mixer        *pMixer;

    pMixer = MusicGlobals;
    if (PV_SetupProcessFunctions(pMixer) == FALSE)
    {   // something happend. Code is not compiled correctly, etc
        return;
    }

    if (pMixer->systemPaused == FALSE)
    {
        // clear output buffer before starting mix, and verb buffers if enabled
        PV_ClearMixBuffers(pMixer->generateStereoOutput);

#if USE_MOD_API
        // mix MOD output into our output stream before we translate it for final output
        // we do it here first, if enabled, to allow MOD files to use our verb
        if (pMixer->pModPlaying)
        {
            if (pMixer->pModPlaying->enableReverb)
            {
                PV_WriteModOutput(pMixer->outputRate, pMixer->generateStereoOutput);
            }
        }
#endif

#if (X_PLATFORM == X_WEBTV)
// For WebTV, we keep MOD support simple. Verb is always enabled, and we call the Mod sample generation
// code. If at some time in the future you (WebTV) want to support enbled to disabled verb, this code
// will need to be duplicated here below and wrapped around a boolean for enabling or disabling verb.
        PV_WriteModOutput(pMixer->outputRate, pMixer->generateStereoOutput);
#endif

        // ok, start any voices in sync that need it
        PV_ProcessSyncronizedVoiceStart();

        // process enabled voices, and add verb, and filter
        PV_ServeInstruments();
#if USE_MOD_API
        // mix MOD output into our output stream before we translate it for final output
        // and mix again here in case verb is disabled
        if (pMixer->pModPlaying)
        {
            if (pMixer->pModPlaying->enableReverb == FALSE)
            {
                PV_WriteModOutput(pMixer->outputRate, pMixer->generateStereoOutput);
            }
        }
#endif

        PV_ProcessSequencerEvents(threadContext);       // process all songs and external events

        PV_ProcessSampleEvents(threadContext);          // process all sample events

#if USE_STREAM_API
        // process stream fades
        PV_ServeStreamFades();
#endif

        // if master volume has been set to zero, silence reins.
        if ((pMixer->scaleBackAmount == 0) || (pMixer->MasterVolume == 0))
        {
            PV_ClearMixBuffers(pMixer->generateStereoOutput);
        }

        // mix down to final output stage for output to speaker
        if (pMixer->generate16output)
        {
            /* Convert intermediate 16-bit sample format to 16 bit output samples:
            */
            if (pMixer->generateStereoOutput)
            {
            #if (USE_16_BIT_OUTPUT == TRUE) && (USE_STEREO_OUTPUT == TRUE)
                PV_Generate16outputStereo((OUTSAMPLE16 *)destinationSamples);
            #endif
            }
            else
            {
            #if (USE_16_BIT_OUTPUT == TRUE) && (USE_MONO_OUTPUT == TRUE)
                PV_Generate16outputMono((OUTSAMPLE16 *)destinationSamples);
            #endif
            }
        }
        else
        {
            /* Convert intermediate 16-bit sample format to 8 bit output samples:
            */
            if (pMixer->generateStereoOutput)
            {
            #if (USE_8_BIT_OUTPUT == TRUE) && (USE_STEREO_OUTPUT == TRUE)
                PV_Generate8outputStereo((OUTSAMPLE8 *)destinationSamples);
            #endif
            }
            else
            {
            #if (USE_8_BIT_OUTPUT == TRUE) && (USE_MONO_OUTPUT == TRUE)
                PV_Generate8outputMono((OUTSAMPLE8 *)destinationSamples);
            #endif
            }
        }
    }
}
#endif

// This turns on new code which allows a complete search of the voice pool for inactive
// notes prior to attempting to steal an active one.

// If the count of active notes exceeds the normal limits for voices, then replace ACTIVE 
// slots in their decay cycle first (to reduce the voice load on the CPU.) If count's within 
// normal limits, then use EMPTY slots first to improve sound quality of the other notes by allowing
// them to decay more completely before being killed.
static GM_Voice * PV_FindFreeVoice(GM_Mixer *pMixer, 
                                    GM_Song *pSong, 
                                    XSDWORD calculatedNewVolume,
                                    XWORD   newMidiPitch,
                                    XSWORD the_instrument, 
                                    XSWORD the_channel)
{
    GM_Voice        *the_entry = NULL, *pVoice;
    LOOPCOUNT       count;
    XFIXED          bestLevel;
    XSWORD          priority;
    XSDWORD         volume32;
    XDWORD          timeStamp;

    // get synth priority to determine note stealing
    priority = pSong->songPriority;

    // Find a place for the new note
    
    volume32 = calculatedNewVolume;

    // Find an inactive slot (best), or
    // Terminate notes in one-shot decay (less desirable)
    // or notes naturally fading out (preferable)
    // or notes that are a lower level or priority
    bestLevel = XFIXED_1;
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        // completely free?
        if (pVoice->voiceMode == VOICE_UNUSED)
        {
            the_entry = pVoice;
            goto EnterNote;
        }
    }

    // now we know we have no free notes, so begin heuristics
    // to determine which active note to kill
    
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        // lower level of priority than this synth?
        if (pVoice->pSong)  // might be a PCM voice rather than a synth voice
        {
            if (pVoice->pSong->songPriority < priority)
            {
                the_entry = pVoice;
                goto EnterNote;
            }
        }   
        // in release?
        if (pVoice->voiceMode == VOICE_RELEASING)       // only steal notes in release
        {
            if (bestLevel > 0x2000)
            {
                bestLevel = 0x2000;
                the_entry = pVoice;
            }
        }
        else if (pVoice->voiceMode == VOICE_SUSTAINING) // check for notes in decaying sustain
        {
            if (pVoice->NoteProgram == the_instrument)
            {
                if (pVoice->NoteChannel == the_channel)
                {
                    if (pVoice->volumeADSRRecord.sustainingDecayLevel < bestLevel)
                    {
                        bestLevel = pVoice->volumeADSRRecord.sustainingDecayLevel;
                        the_entry = pVoice;
                    }
                }
            }
        }
        if (bestLevel <= 0x2000)
        {
            goto EnterNote;     // break on this note
        }
    }

    the_entry = NULL;   // reset to try again

#if 0
// Now kill notes that are in sustain pedal mode, are in same channel or instrument
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->sustainMode != SUS_NORMAL)
        {
            if (pVoice->NoteProgram == the_instrument)
            {
                if (pVoice->NoteChannel == the_channel)
                {
                    the_entry = pVoice;
                    goto EnterNote;
                }
            }
        }
    }
    the_entry = NULL;   // reset to try again
#endif
    
#if 1
// Now kill the oldest note that is in sustain pedal mode
    timeStamp = 0x7fffffff;
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->sustainMode != SUS_NORMAL)
        {
            if (pVoice->voiceStartTimeStamp < timeStamp)
            {
                timeStamp = pVoice->voiceStartTimeStamp;
                the_entry = pVoice;
            }
        }
    }
    if (the_entry)
        goto EnterNote;

    the_entry = NULL;   // reset to try again
#endif

#if 1
// Now kill notes that are much lower in volume than the current note (less than 25% of the volume)
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->NoteVolume < (XSDWORD)bestLevel)
        {
            bestLevel = pVoice->NoteVolume;
            the_entry = pVoice;
        }
    }
    if (((XSDWORD)bestLevel * 4) <  volume32)       // NOTE:  was * 8 before 6/7/01     DS
    {
        goto EnterNote;
    }

    the_entry = NULL;   // reset to try again
    bestLevel = 0x2000; // reset
    
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if ( (XSDWORD)bestLevel > ((pMixer->NoteEntry[count].NoteVolume * 
            pVoice->NoteVolumeEnvelopeBeforeLFO) >> VOLUME_PRECISION_SCALAR))
        {
            bestLevel = (pVoice->NoteVolume * pVoice->NoteVolumeEnvelopeBeforeLFO) >> VOLUME_PRECISION_SCALAR;
            the_entry = pVoice;
        }
    }

    if (((XSDWORD)bestLevel * 4) <  volume32)
    {
        goto EnterNote;
    }
#endif

// OK now we're really mad.  Kill the oldest note, period.    (Added 6/7/01 for limited voice case).

    the_entry = NULL;   // reset to try again
    timeStamp = 0x7fffffff;
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->voiceStartTimeStamp < timeStamp)
        {
            timeStamp = pVoice->voiceStartTimeStamp;
            the_entry = pVoice;
        }
    }


// gently terminate (euthanize) any notes of same pitch, instrument number, and channel:
#if 0
// don't want to do this all the time. only certain instruments need this feature. We might
// want to be able to flag this and let the instrument designer design this.
// drums rolls don't sound right.
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->voiceMode != VOICE_UNUSED)
        {
            if (pVoice->NoteProgram == the_instrument)
            {
                if (pVoice->NoteMIDIPitch == notePitch)
                {
                    if (pVoice->NoteChannel == the_channel)
                    {
                        if (the_channel != PERCUSSION_CHANNEL)
                        {
                            pVoice->volumeADSRRecord.mode = ADSR_TERMINATE;
                            pVoice->volumeADSRRecord.currentPosition = 0;
                            pVoice->volumeADSRRecord.ADSRLevel[0] = 0;
                            pVoice->volumeADSRRecord.ADSRTime[0] = 1;
                            pVoice->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
                            pVoice->NoteVolumeEnvelopeBeforeLFO = 0;        // so these notes can be reused
                        }
                    }
                }
            }
        }
    }
#endif
EnterNote:
    //printf("audio::midi found free voice %ld\n", the_entry - &pMixer->NoteEntry[0]);
    return the_entry;
}


// Given a valid GM_Song, an instrument, track, channel, midi pitch and volume, allocate a voice
// in the mixer and start a note playing. This assumes that the instrument is loaded and valid.
// This function also will kill notes if needed to activate this new note.
//
// This function is primarly called from the BAE midi sequencer. There's no need to call it directly.
void PV_StartMIDINote(GM_Song *pSong, INT16 the_instrument, 
                        INT16 the_channel, INT16 the_track, INT16 notePitch, INT32 Volume)
{
    register GM_Mixer       *pMixer;
    register UBYTE          *pSample;
    register GM_Voice       *the_entry;
    register GM_Instrument  *theI;
    register GM_Instrument  *pInstrument;
    register GM_KeymapSplit *k;
    register INT32          count;
    INT16                   newPitch, playPitch;
    UINT16                  splitCount;
    UINT32                  loopstart, loopend;
    register INT32          i, j;
    INT32                   volume32;
    INT32                   sampleNumber;
    XSWORD                  priority;

    pMixer = GM_GetCurrentMixer();

    if ((Volume == 0) || (pSong->songVolume == 0))
    {
        // if the volume of the midi note requested to play or the overall song volume
        // is 0, then don't bother starting a note.
        return;
    }
    // get synth priority to determine note stealing
    priority = pSong->songPriority;

// scale with default velocity curve
    Volume = PV_ModifyVelocityFromCurve(pSong, Volume);

    playPitch = notePitch;
    pInstrument = NULL;
    sampleNumber = 0;
    theI = pSong->instrumentData[pSong->remapArray[the_instrument]];
    if (theI)
    {
/*
        // NOTE AGAIN:
        // This code is commented out because it fucks up some content. I'm not sure
        // why its here in the first place. This may be a hack to get some content
        // working with the new root key features for the BeOS. For the time being
        // leave it commented out. -SH
        
        // NOTE!! If useSoundModifierAsRootKey is TRUE, then we are using
        // the Sound Modifier data blocks as a root key replacement for samples in
        // the particular split
        if (theI->useSoundModifierAsRootKey)
        {
            if (theI->masterRootKey)
            {
                playPitch = notePitch - theI->masterRootKey + 60;
            }
            playPitch = playPitch + 60 - theI->miscParameter1;
        }
        else
*/
        {
            // old way
            if (theI->masterRootKey)
            {
                playPitch = notePitch - theI->masterRootKey + 60;
            }
        }
        // keysplit?
        if (theI->doKeymapSplit)
        {   // yes, find an instrument
            splitCount = theI->u.k.KeymapSplitCount;
            k = theI->u.k.keySplits;
            for (count = 0; count < splitCount; count++)
            {
                if ( (playPitch >= k->lowMidi) && (playPitch <= k->highMidi) )
                {
                    theI = k->pSplitInstrument;
                    if (theI)
                    {
                        pInstrument = theI;
                        // NOTE!! If useSoundModifierAsRootKey is TRUE, then we are using
                        // the Sound Modifier data blocks as a root key replacement for samples in
                        // the particular split
                        if (theI->useSoundModifierAsRootKey)
                        {
                            if (theI->masterRootKey)
                            {
                                playPitch = notePitch - theI->masterRootKey + 60;
                            }
                            playPitch = playPitch + 60 - theI->miscParameter1;
                            Volume = (Volume * theI->miscParameter2) / 100;     // scale volume based upon split
                        }
                        else
                        {
                            if (theI->masterRootKey)
                            {
                                playPitch = notePitch - theI->masterRootKey + 60;
                            }
                        }
                        break;
                    }
                    else
                    {
                        BAE_PRINTF("bad sample in instrument %d split %d\n", the_instrument, (int)count);
                    }
                }
                k++;
                sampleNumber++;
            }
        }
        else
        {
            pInstrument = theI;
            if (theI->useSoundModifierAsRootKey)
            {
                playPitch = playPitch + 60 - theI->miscParameter1;
                Volume = (Volume * theI->miscParameter2) / 100;     // scale volume based upon split
            }
        }
    }
    else
    {
        BAE_PRINTF("trying to play unloaded instrument %d\n", the_instrument);
    }
    if (pInstrument == NULL)
    {
        BAE_PRINTF("instrument %d not found on track %d channel %d note %d!\n", 
                        the_instrument, the_track, the_channel, notePitch);
        return;
    }
    loopstart = pInstrument->u.w.startLoop;
    loopend = pInstrument->u.w.endLoop;
    if ( (pInstrument->disableSndLooping) || 
        (loopstart == loopend) || 
        (loopstart > loopend) || 
        (loopend > pInstrument->u.w.waveFrames) ||
        (loopend - loopstart < MIN_LOOP_SIZE) )
    {
        loopstart = 0;
        loopend = 0;
    }

    // NOTE: This the only place we work with the scaleBackAmount. I'm not sure this is the right place for
    // this!!!
    volume32 = (Volume * pMixer->scaleBackAmount) >> 8;

    // get the inital note volume based upon song and channel volume
    // These are used in the note stealing algorythmn
    volume32 = PV_ScaleVolumeFromChannelAndSong(pSong, the_channel, volume32);

    if (theI->useSoundModifierAsRootKey)
    {
        newPitch = playPitch;   // already been modified
    }
    else
    {
        newPitch = playPitch + 60 - pInstrument->u.w.baseMidiPitch;
    }
    while (newPitch < -24) { newPitch += 12; }
    while (newPitch > 144) { newPitch -= 12; }

// If the count of active notes exceeds the normal limits for voices, then replace ACTIVE 
// slots in their decay cycle first (to reduce the voice load on the CPU.) If count's within 
// normal limits, then use EMPTY slots first to improve sound quality of the other notes by allowing
// them to decay more completely before being killed.
    the_entry = PV_FindFreeVoice(pMixer, pSong, volume32, newPitch, the_instrument, the_channel);
    if (the_entry)
    {
        // found or created an empty voice
        the_entry->voiceMode = VOICE_ALLOCATED;
        PV_CleanNoteEntry(the_entry);
        the_entry->pInstrument = pInstrument;
        the_entry->pSong = pSong;
        the_entry->NoteVolumeEnvelopeBeforeLFO = VOLUME_PRECISION_SCALAR;
        pSample = (UBYTE *)pInstrument->u.w.theWaveform;
        the_entry->NotePtr = pSample;
        the_entry->NotePtrEnd = pSample + pInstrument->u.w.waveFrames;
        the_entry->NoteChannel = (SBYTE)the_channel;
        the_entry->NoteTrack = (SBYTE)the_track;

        // copy the volume ADSR record into the GM_Voice
        the_entry->volumeADSRRecord = pInstrument->volumeADSRRecord;
        
        // copy the sample-and-hold flag
        the_entry->sampleAndHold = pInstrument->sampleAndHold;

        // Copy the LFO record count
        the_entry->LFORecordCount = pInstrument->LFORecordCount;

        // If there are any LFO records, copy them into the GM_Voice.
        if (the_entry->LFORecordCount)
        {
            for (i = 0; i < the_entry->LFORecordCount; i++)
            {
                the_entry->LFORecords[i] = pInstrument->LFORecords[i];
            }
        }

#if REVERB_USED != REVERB_DISABLED
        the_entry->avoidReverb = pInstrument->avoidReverb;  // use instrument default. in case instrument designer
        the_entry->reverbLevel = pSong->channelReverb[the_channel]; // set current verb level
        the_entry->chorusLevel = (INT16)PV_ModifyVelocityFromCurve(pSong, pSong->channelChorus[the_channel]);
                                                            // wants no verb enabled
        if (GM_IsReverbFixed())
        {
            // if the instrument defines reverb on or the channel has reverb on, then enable it.
            // if the channel is off, but the instrument defines reverb then enable it
            if (pSong->channelReverb[the_channel] < GM_GetReverbEnableThreshold())
            {
                the_entry->avoidReverb = TRUE;      // force off
            }
        }
        if (the_entry->avoidReverb)
        {
            the_entry->reverbLevel = 0;
            the_entry->chorusLevel = 0;
        }
#endif

        // Setup playback pitch
        if (pInstrument->playAtSampledFreq == FALSE)
        {
            the_entry->ProcessedPitch = newPitch;
            the_entry->NotePitch = majorPitchTable[newPitch+24];

        }
        else
        {
            the_entry->ProcessedPitch = 0;
            the_entry->NotePitch = 0x10000;     // 1.0 step rate
        }

        // factor in sample rate of sample, if enabled
        if (pInstrument->useSampleRate)
        {
            the_entry->noteSamplePitchAdjust = XFixedDivide(pInstrument->u.w.sampledRate >> 2, 22050L << 14);
            the_entry->NotePitch = XFixedMultiply (the_entry->NotePitch, the_entry->noteSamplePitchAdjust);
        }
        else
        {
            the_entry->noteSamplePitchAdjust = 0x10000;
        }
        if (loopend - loopstart)
        {
            the_entry->NoteLoopPtr = (XBYTE *)the_entry->NotePtr + loopstart;
            the_entry->NoteLoopEnd = (XBYTE *)the_entry->NotePtr + loopend;
        }
        else
        {
            the_entry->NoteLoopPtr = NULL;
            the_entry->NoteLoopEnd = NULL;
        }
        the_entry->NoteDecay = 8;       // default note decay
        the_entry->NoteNextSize = 0;    // recalculate next size
        the_entry->NoteWave = 0;        // starting sample position

        the_entry->NoteProgram = the_instrument;
        the_entry->NoteMIDIPitch = (SBYTE)notePitch;    // save note pitch unprocessed
        the_entry->noteOffsetStart = (SBYTE)pSong->songPitchShift;
        the_entry->NoteMIDIVolume = (INT16)Volume;  // save note volume unscaled

        the_entry->NoteVolume = volume32;
// Resonant low-pass filter stuff
        the_entry->LPF_base_frequency = pInstrument->LPF_frequency;
        the_entry->LPF_base_resonance = pInstrument->LPF_resonance;
        the_entry->LPF_base_lowpassAmount = pInstrument->LPF_lowpassAmount;

        the_entry->LPF_frequency = pInstrument->LPF_frequency;
        the_entry->LPF_resonance = pInstrument->LPF_resonance;
        the_entry->LPF_lowpassAmount = pInstrument->LPF_lowpassAmount;

#if USE_CALLBACKS
        the_entry->NoteEndCallback = NULL;
        the_entry->NoteLoopProc = NULL;
        the_entry->NoteContext = NULL;
#endif
        the_entry->NotePitchBend = pSong->channelBend[the_channel];
        the_entry->LastPitchBend = 0;
        the_entry->ModWheelValue = pSong->channelModWheel[the_channel];
        the_entry->LastModWheelValue = 0;
        the_entry->NoteLoopCount = 0;

        // Set the inital pan placement as the combination of the initial pan and the current
        // controller state

        // Grab the unchanged value then scale it for the mixer note setup
        // This and only this pan value is stored in stereoPosition because it is
        // overwritten each time controller 10 changes.
        the_entry->stereoPosition = SetChannelStereoPosition(pSong, the_channel, pSong->channelStereoPosition[the_channel]);

        // We store this in stereoPanBend along with pan modulation (which is summed in).
        // This is actually stored again in PV_ServeThisInstrument, so this line is 
        // superfluous (left in for clarity)
        the_entry->stereoPanBend = pInstrument->panPlacement;

        the_entry->bitSize = pInstrument->u.w.bitSize;
        the_entry->channels = pInstrument->u.w.channels;

        if (pSong->channelSustain[the_channel])
        {
            the_entry->sustainMode = SUS_ON_NOTE_ON;
        }
        else
        {
            the_entry->sustainMode = SUS_NORMAL;
        }

        if (pInstrument->curveRecordCount)
        {
            for (i = 0; i < pInstrument->curveRecordCount; i++)
            {
                INT32 scalar;
                INT32 tieFromValue = 0;
                INT32 curveCount = pInstrument->curve[i].curveCount;
                switch (pInstrument->curve[i].tieFrom)
                {
                    case PITCH_LFO:
                        //DEBUG_STR("\p tying to pitch");
                        tieFromValue = notePitch;
                        break;
                    case VOLUME_LFO:
                        tieFromValue = Volume;
                        break;
                    case SAMPLE_NUMBER:
                        tieFromValue = sampleNumber;
                        break;
                    case MOD_WHEEL_CONTROL:
                        tieFromValue = pSong->channelModWheel[the_channel];
                        break;
                    default:
                        //DEBUG_STR("\p invalid tie-from value");
                        break;
                }
                scalar = tieFromValue;
                for (count = 0; count < curveCount; count++)
                {
                    if (pInstrument->curve[i].from_Value[count] <= tieFromValue)
                    {
                        if (pInstrument->curve[i].from_Value[count+1] >= tieFromValue)
                        {
                            scalar = pInstrument->curve[i].to_Scalar[count];
                            if (pInstrument->curve[i].from_Value[count] != pInstrument->curve[i].from_Value[count+1])
                            {
                                INT32 from_difference = pInstrument->curve[i].from_Value[count+1] - pInstrument->curve[i].from_Value[count];
                                INT32 to_difference = pInstrument->curve[i].to_Scalar[count+1] - pInstrument->curve[i].to_Scalar[count];
                                scalar += ((((tieFromValue - pInstrument->curve[i].from_Value[count]) << 8) / from_difference) * to_difference) >> 8;
                            }
                        }
                    }
                }
                switch (pInstrument->curve[i].tieTo)
                {
                    case NOTE_VOLUME:
                        //DEBUG_STR("\p scaling note velocity");
                        the_entry->NoteVolume = (the_entry->NoteVolume * scalar) >> 8;
                        break;
                    case SUSTAIN_RELEASE_TIME:
                        for (j = 0; j < ADSR_STAGES; j++)
                        {
                            if (the_entry->volumeADSRRecord.ADSRFlags[j] == ADSR_SUSTAIN)
                            {
                                if (the_entry->volumeADSRRecord.ADSRLevel[j] < 0)
                                {
                                    scalar = scalar >> 2;
                                    if (the_entry->volumeADSRRecord.ADSRLevel[j]  < -50)
                                    {
                                        the_entry->volumeADSRRecord.ADSRLevel[j] = 
                                            -(-(the_entry->volumeADSRRecord.ADSRLevel[j] * scalar) >> 6);
                                    }
                                    else
                                    {
                                        the_entry->volumeADSRRecord.ADSRLevel[j] = 
                                            -((PV_GetLogLookupTableEntry(-the_entry->volumeADSRRecord.ADSRLevel[j]) * scalar) >> 6);
                                    }
                                    break;  //j = ADSR_STAGES;
                                }
                            }
                        }
                        break;
                    case SUSTAIN_LEVEL:
                        for (j = 1; j < ADSR_STAGES; j++)
                        {
                            if (the_entry->volumeADSRRecord.ADSRFlags[j] == ADSR_SUSTAIN)
                            {
                                if (the_entry->volumeADSRRecord.ADSRLevel[j] > 0)
                                {
                                        the_entry->volumeADSRRecord.ADSRLevel[j] = 
                                            (the_entry->volumeADSRRecord.ADSRLevel[j] * scalar) >> 8;
                                }
                                else
                                {
                                        the_entry->volumeADSRRecord.ADSRLevel[j-1] = 
                                            (the_entry->volumeADSRRecord.ADSRLevel[j-1] * scalar) >> 8;
                                }
                                break;  //j = ADSR_STAGES;
                            }
                        }
                        break;
                    case RELEASE_TIME:
                        for (j = 1; j < ADSR_STAGES; j++)
                        {
                            if (the_entry->volumeADSRRecord.ADSRFlags[j] == ADSR_TERMINATE)
                            {
                                if (the_entry->volumeADSRRecord.ADSRTime[j] > 0)
                                        the_entry->volumeADSRRecord.ADSRTime[j] = 
                                            (the_entry->volumeADSRRecord.ADSRTime[j] * scalar) >> 8;
                                else
                                        the_entry->volumeADSRRecord.ADSRTime[j-1] = 
                                            (the_entry->volumeADSRRecord.ADSRTime[j-1] * scalar) >> 8;
                                break;  //j = ADSR_STAGES;
                            }
                        }
                        break;
                    case VOLUME_ATTACK_TIME:
                        if (the_entry->volumeADSRRecord.ADSRTime[0] != 0)
                        {
                            the_entry->volumeADSRRecord.ADSRTime[0] = (the_entry->volumeADSRRecord.ADSRTime[0] * scalar) >> 8;
                        }
                        else
                        {
                            the_entry->volumeADSRRecord.ADSRTime[1] = (the_entry->volumeADSRRecord.ADSRTime[1] * scalar) >> 8;
                        }
                        break;
                    case VOLUME_ATTACK_LEVEL:
                        //if (1)    //(*((long *) 0x17a))
                        {
                            if (the_entry->volumeADSRRecord.ADSRLevel[0] >  the_entry->volumeADSRRecord.ADSRLevel[1])
                            {
                                the_entry->volumeADSRRecord.ADSRLevel[0] = (the_entry->volumeADSRRecord.ADSRLevel[0] * scalar) >> 8;
                            }
                            //else
                            //{
                            //  the_entry->volumeADSRRecord.ADSRLevel[1] = (the_entry->volumeADSRRecord.ADSRLevel[1] * scalar) >> 8;
                            //}
                        }
                        break;
                    case WAVEFORM_OFFSET:
                        PV_SetPositionFromVoice(the_entry, scalar);
                        break;
                    case LOW_PASS_AMOUNT:
                        the_entry->LPF_base_lowpassAmount = (the_entry->LPF_base_lowpassAmount * scalar) >> 8;
                        break;
                    case PITCH_LFO:
                        for (j = the_entry->LFORecordCount - 1; j >= 0; --j)
                            if (the_entry->LFORecords[j].where_to_feed == PITCH_LFO)
                            {
                                the_entry->LFORecords[j].level = (the_entry->LFORecords[j].level * scalar) >> 8;
                                goto exit;
                            }
                        break;
                    case VOLUME_LFO:
                        for (j = the_entry->LFORecordCount - 1; j >= 0; --j)
                            if (the_entry->LFORecords[j].where_to_feed == VOLUME_LFO)
                            {
                                the_entry->LFORecords[j].level = (the_entry->LFORecords[j].level * scalar) >> 8;
                                goto exit;
                            }
                        break;
                    case PITCH_LFO_FREQUENCY:
                        for (j = 0; j < the_entry->LFORecordCount; j++)
                            if (the_entry->LFORecords[j].where_to_feed == PITCH_LFO)
                            {
                                the_entry->LFORecords[j].period = (the_entry->LFORecords[j].period * scalar) >> 8;
                                goto exit;
                            }
                        break;
                    case VOLUME_LFO_FREQUENCY:
                        for (j = 0; j < the_entry->LFORecordCount; j++)
                            if (the_entry->LFORecords[j].where_to_feed == VOLUME_LFO)
                            {
                                the_entry->LFORecords[j].period = (the_entry->LFORecords[j].period * scalar) >> 8;
                                goto exit;
                            }
                        break;
                }
            exit:;
            }
        }

        the_entry->routeBus = pSong->routeBus;

// is there an initial volume level in the ADSR record that starts at time=0?  If so, don't interpolate the
// note's volume up from 0 to the first target level.  Otherwise, it's a traditional ramp-up from 0.
        if (the_entry->volumeADSRRecord.ADSRTime[0] == 0)
        {
            the_entry->volumeADSRRecord.currentLevel = the_entry->volumeADSRRecord.ADSRLevel[0];
            the_entry->NoteVolumeEnvelope = (INT16)the_entry->volumeADSRRecord.ADSRLevel[0];
            if (pMixer->generateStereoOutput)
            {
                PV_CalculateStereoVolume(the_entry, &the_entry->lastAmplitudeL, &the_entry->lastAmplitudeR);
            }
            else
            {
                PV_CalculateMonoVolume(the_entry, &the_entry->lastAmplitudeL);
                the_entry->lastAmplitudeR = 0;
            }
        }
        else
        {
                the_entry->lastAmplitudeL = 0;
                the_entry->lastAmplitudeR = 0;
        }

// This step is performed last.
        the_entry->voiceStartTimeStamp = XMicroseconds();
#ifdef BAE_MCU
        if (GM_InitVoiceOnDSP(the_entry) == NO_ERR)
        {
            the_entry->voiceMode = VOICE_SUSTAINING;
        }
        else
        {
            the_entry->voiceMode = VOICE_UNUSED;
        }
#else
        the_entry->voiceMode = VOICE_SUSTAINING;
#endif
    }
}

// Given a valid GM_Song, an instrument, track, channel, midi pitch, stop a voice
// in the mixer
//
// This function is primarly called from the BAE midi sequencer. There's no need to call it directly.
//
// The heristic is kill the longest note of the type passed. For example with two notes at 60, one of time 3000,
// and one at time 6000. The time 3000 note at 60 will be killed.
void PV_StopMIDINote(GM_Song *pSong, XSWORD the_instrument, XSWORD the_channel, XSWORD the_track, XSWORD notePitch)
{
    register LOOPCOUNT      count;
    register GM_Mixer       *pMixer;
    register XSWORD         decay;
    register GM_Voice       *pNote;
    XSWORD                  realNote, compareNote;
    XDWORD                  youngestTime;
    GM_Voice                *pNoteToKill;
    
    pMixer = GM_GetCurrentMixer();
    the_track = the_track;
    the_instrument = the_instrument;
    //BAE_PRINTF("NoteOff i %d c %d p %d\n", the_instrument, the_channel, notePitch); 
    pNoteToKill = NULL;
    youngestTime = 0;   // min time
    for (count = 0; count < pMixer->MaxNotes; count++)
    {
        pNote = &pMixer->NoteEntry[count];
        if (pNote->voiceMode != VOICE_UNUSED)               // still playing
        {
            if (pNote->pSong == pSong)                      // same song
            {
//              if (pNote->NoteProgram == the_instrument)   // same program     // NOTE: can't assume this
                {
                    if (pNote->NoteChannel == the_channel)  // same channel
                    {
                        realNote = pNote->NoteMIDIPitch - pNote->noteOffsetStart;
                        if (GM_DoesChannelAllowPitchOffset(pSong, (unsigned short)pNote->NoteChannel))
                        {
                            compareNote = notePitch - pSong->songPitchShift;
                        }
                        else
                        {
                            compareNote = notePitch - pNote->noteOffsetStart;
                        }
                        if (realNote == compareNote)        // same pitch
                        {
                            // now compare this to the youngest note
                            if (pNote->voiceStartTimeStamp >= youngestTime)
                            {
                                youngestTime = pNote->voiceStartTimeStamp;
                                pNoteToKill = pNote;
                            }
                        }
                    }
                }
            }
        }
    }
    if (pNoteToKill)
    {
        // this makes sure future searches do not find this note again
        pNoteToKill->voiceStartTimeStamp = 0;
        // if pedal is on, put note into SUS_ON_NOTE_OFF, and don't kill note
        if (pSong->channelSustain[the_channel])
        {
            pNoteToKill->sustainMode = SUS_ON_NOTE_OFF;
        }
        else
        {
            pNoteToKill->voiceMode = VOICE_RELEASING;   // put into release mode, decay
            decay = pNoteToKill->NoteDecay;
            if ( (decay > 500) || (decay < 0) )
            {
                BAE_ASSERT(FALSE);  // Decay out of range
                pNoteToKill->NoteDecay = 1;
            }
        }
    }
#if 0
    // it is perfectly legitimate for the note to have been stolen prior to receiving its note off
    // D.S.
    else
    {
        // we didn't find a note to kill, because the note might have been stolen,
        // or there's something wrong.
        BAE_ASSERT(FALSE);
    }
#endif
}

// Stop or kill notes for song passed. If pSong is NULL, then all notes are processed otherwise
// just notes associated with the particular pSong.
// If kill is FALSE then the notes are put into release mode. If kill is TRUE, then the note is terminated
// without release. May cause clicks.
// Set useChannel to -1 if kill all notes, otherwise its a channel filter.
// Set useChannel to -1 to ignore instrument, otherwise its an instrument filter.
static void PV_EndNotes(GM_Song *pSong, XSWORD useChannel, XLongResourceID useInstrument, XBOOL kill)
{
    register short int      count;
    register GM_Mixer       *pMixer;
    register GM_Voice       *pNote;

    pMixer = GM_GetCurrentMixer();
    if (pMixer)
    {
        for (count = 0; count < pMixer->MaxNotes; count++)
        {
            pNote = &pMixer->NoteEntry[count];
            if ((pSong == NULL) || (pNote->pSong == pSong))
            {
                if ((useChannel == -1) || (pNote->NoteChannel == useChannel))
                {
                    if ((useInstrument == -1) || (pNote->NoteProgram == useInstrument))
                    {
                        if (pNote->voiceMode != VOICE_UNUSED)
                        {
                            if (kill)
                            {
#ifdef BAE_MCU
                                GM_KillVoiceOnDSP(pNote);
#endif
                                pNote->NoteDecay = 0;
                                pNote->volumeADSRRecord.currentPosition = 0;
                                pNote->volumeADSRRecord.ADSRLevel[0] = 0;
                                pNote->volumeADSRRecord.ADSRTime[0] = 1;
                                pNote->volumeADSRRecord.ADSRFlags[0] = ADSR_TERMINATE;
                                pNote->NoteVolumeEnvelopeBeforeLFO = 0;     // so these notes can be reused
                                pNote->voiceMode = VOICE_UNUSED;
                            }
                            else
                            {
                                if (!pNote->sampleAndHold) 
                                {
                                    pNote->NoteLoopPtr = NULL;
                                    pNote->NoteLoopEnd = NULL;      // kill loop points, so it will end
                                }
                                pNote->voiceMode = VOICE_RELEASING;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Stop notes for song passed. This will put the note into release mode.
void GM_EndSongNotes(GM_Song *pSong)
{
    PV_EndNotes(pSong, -1, -1, FALSE);
}

// stop notes for a song and channel passed. This will put the note into release mode.
void GM_EndSongChannelNotes(GM_Song *pSong, short int channel)
{
    PV_EndNotes(pSong, channel, -1, FALSE);
}


// Stop just midi notes. Note: This does not kill the notes anymore. It just puts them into release mode.
// Much more professional experience.
void GM_EndAllNotes(void)
{
    GM_EndSongNotes(NULL);
}

// kills notes for song passed. This kill all notes for this GM_Song. Terminates them now.
void GM_KillSongNotes(GM_Song *pSong)
{
    PV_EndNotes(pSong, -1, -1, TRUE);
}

// kills notes that use a particular instrument. The instrument passed is the combined bank/program
// instrument.
void GM_KillSongInstrument(GM_Song *pSong, XLongResourceID instrument)
{
    PV_EndNotes(pSong, -1, (XSWORD)instrument, TRUE);
}

// Stop just midi notes. Note: This kills the notes currently playing. It may result in clicks.
void GM_KillAllNotes(void)
{
    GM_KillSongNotes(NULL);
}


// Used to get the current frame of audio data that has been built. Useful for fun displays. Returns 16 bit information
// only. If generating 8 bit, then data output is converted. If mono data then right channel will be dead.
// This code is deliberately less efficient than the real output scaling code, for space conservation purposes.
INT16 GM_GetAudioSampleFrame(INT16 *pLeft, INT16 *pRight)
{
#ifdef BAE_MCU
    return 0;
#else
    register LOOPCOUNT      size, count;
    register INT32          *sourceL;
    register INT32          i, k8000;
    GM_Mixer                *pMixer;

    pMixer = GM_GetCurrentMixer();
    size = 0;
    if (pMixer)
    {
        k8000 = 0x8000;
        sourceL = &pMixer->songBufferDry[0];
        size = pMixer->One_Loop;

        if (pMixer->generateStereoOutput)
        {
            for (count = 0; count < pMixer->Four_Loop; count++)
            {
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pRight++ = (INT16)(i - k8000);

                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pRight++ = (INT16)(i - k8000);

                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pRight++ = (INT16)(i - k8000);

                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pRight++ = (INT16)(i - k8000);
            }
        }
        else
        {
            for (count = 0; count < pMixer->Four_Loop; count++)
            {
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);

                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);

                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);

                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
                i = (*sourceL++ >> OUTPUT_SCALAR);
                i += k8000;
                if (i & 0xFFFF0000)
                    { if (i > 0) i = 0xFFFE; else i = 0;}
                *pLeft++ = (INT16)(i - k8000);
            }
        }
    }
    return (INT16)size;
#endif
}

void GM_DisplayVoiceData(GM_Mixer *pMixer)
{
    LOOPCOUNT   count;
    GM_Voice    *pVoice;

    if (pMixer == NULL) {
        pMixer = GM_GetCurrentMixer();
    }
    
    for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->voiceMode != VOICE_UNUSED)
        {
            BAE_PRINTF("### Voice %ld\n", count);
            BAE_PRINTF("    voiceMode %d\n", pVoice->voiceMode);
            BAE_PRINTF("    voiceStartTimeStamp %lu\n", (unsigned long)pVoice->voiceStartTimeStamp);
            BAE_PRINTF("    pSong %lx\n", (long)pVoice->pSong);
            BAE_PRINTF("    pInstrument %lx\n", (long)pVoice->pInstrument);
            BAE_PRINTF("    NoteChannel %d\n", pVoice->NoteChannel);
            BAE_PRINTF("    NoteMIDIPitch %d\n", pVoice->NoteMIDIPitch);
            BAE_PRINTF("    sustainMode %d\n", pVoice->sustainMode);
            BAE_PRINTF("    NoteVolumeEnvelope %d\n", pVoice->NoteVolumeEnvelope);
            BAE_PRINTF("    NoteVolume %ld\n", (long)pVoice->NoteVolume);
            BAE_PRINTF("    NoteMIDIVolume %d\n", pVoice->NoteMIDIVolume);
            BAE_PRINTF("    NoteProgram %ld\n", (unsigned long)pVoice->NoteProgram);
            BAE_PRINTF("    volumeADSRRecord.sustainingDecayLevel %g\n", XFIXED_TO_FLOAT(pVoice->volumeADSRRecord.sustainingDecayLevel));
            BAE_PRINTF("###\n");
        }
    }
}

// This will check active voices and look at a sub sample of the audio output to
// determine if there's any audio still playing
static INT16    pcmLeft[2048], pcmRight[2048];
XBOOL GM_IsAudioActive(void)
{
    register GM_Mixer   *pMixer;
    register LOOPCOUNT  count;
    register GM_Voice   *pVoice;
    XBOOL               someSoundActive;

    pMixer = MusicGlobals;
    someSoundActive = FALSE;
    for (count = 0; count < (pMixer->MaxNotes+pMixer->MaxEffects); count++)
    {
        pVoice = &pMixer->NoteEntry[count];
        if (pVoice->voiceMode != VOICE_UNUSED)
        {
            someSoundActive = TRUE;
            break;
        }
    }
    // there's no voices active, but we must check our final mix buss for reverbs, or
    // other effects that can cause audio
    if (someSoundActive == FALSE)
    {
        INT16   pcm, samples;

        samples = GM_GetAudioSampleFrame(pcmLeft, pcmRight);
        pcm = 0;

        if (pMixer->generateStereoOutput)
        {
            for (count = 0; count < samples; count += 8)
            {
                pcm |= ABS(pcmLeft[count]) | ABS(pcmRight[count]);
            }
        }
        else
        {
            for (count = 0; count < samples; count += 8)
            {
                pcm |= ABS(pcmLeft[count]);
            }
        }

        if ( ABS(pcm) > 20 )
        {
            someSoundActive = TRUE;
        }
    }
    return someSoundActive;
}


/* EOF of GenSynth.c
*/

