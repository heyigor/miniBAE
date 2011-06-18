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
** "GenPriv.h"
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
**
** Overview
**  Private structures
**
** Modification History
**
**  11/7/95     Major changes, revised just about everything.
**  11/11/95    Added microSyncCount for live link
**  11/16/95    Removed microSyncCount
**              Moved static variables into MusicVar structure
**              Created an external function 'PV_GetExternalTimeSync()' for the external midi source
**   12/95      upgraded mixing bus to 32 bit; improved scaleback resolution; added reverb unit; first pass at volume ADSR
**  12/6/95     removed reference to USE_AMP_LOOKUP
**              moved REVERB_TYPE to GENSND.H
**  12/7/95     Added channelReverb to GM_Mixer structure
**              Added REVERB_CONTROLER_THRESHOLD
**  1/18/96     Spruced up for C++ extra error checking
**              Changed InstUsedList to pUsedList and allocate it when needed
**  2/5/96      Removed unused variables. Working towards multiple songs
**              Moved lots of variables from the GM_Mixer structure into
**              Moved the MAX_TRACKS define to GenSnd.h
**  2/12/96     Added PV_CleanExternalQueue
**              Moved SongMicroseconds to GenSnd.h
**  2/13/96     Added multi song support
**  3/5/96      Eliminated the global songVolume
**  3/28/96     Added PV_SetSampleIntoCache & PV_GetInstrument
**  4/10/96     Reworked the sample cache system to not clone the sample data
**  5/2/96      Changed int to BOOL_FLAG
**  5/18/96     Added error condition to PV_MusicIRQ
**  6/30/96     Changed font and re tabbed
**  7/3/96      Added packing pragmas
**              Removed usage of Machine.h. Now merged into X_API.h
**  7/14/96     Fixed structure alignment issue for PowerPC
**  7/23/96     Changed PV_GetExternalTimeSync to unsigned long
**  7/24/96     Changed Midi Queue system to use a head/tail
**  7/25/96     Moved Mac audio variables to GenMacTools.c
**              Changed PV_GetExternalTimeSync to GM_GetSyncTimeStampQuantizedAhead
**  8/12/96     Changed PV_ResetControlers to support semi-complete reset
**  9/25/96     Added GM_Song pointer in NoteRecord structure
**  9/27/96     Added more parameters to ServeMIDINote & PV_StopMIDINote
**  10/18/96    Made CacheSampleInfo smaller
**  10/23/96    Removed reference to BYTE and changed them all to XBYTE or XSBYTE
**  12/19/96    Added Sparc pragmas
**  12/30/96    Changed copyrights
**  1/23/97     Added support for stereoFilter
**              In NoteRecord changed PitchBend to NotePitchBend
**              Added in NoteRecord NoteFadeRate
**  1/30/97     Changed SYMPHONY_SIZE to MAX_VOICES
**  3/17/97     Changed the API to PV_GetInstrument. Enlarged CacheSampleInfo member
**              theID to a long
**  4/9/97      Added sampleExpansion factor
**  4/20/97     Changed PV_MusicIRQ to PV_ProcessMidiSequencerSlice
**  6/4/97      Renamed InitSoundManager to GM_StartHardwareSoundManager, and
**              renamed FinsSoundManager to GM_StopHardwareSoundManager, and
**              now pass in a thread context
**  712/97      Added a drift fixer flag to GM_Mixer that tries
**              to compensate for real time midi time stamping
**  7/16/97     Moved GM_Mixer *MusicGlobals to be protected againsts C++
**              name mangling
**  7/17/97     Aligned GM_Mixer structure to 8 bytes
**  7/18/97     Moved GM_AudioTaskCallbackPtr pTaskProc &
**              GM_AudioOutputCallbackPtr pOutputProc into here from GenXXXTools.c
**  7/22/97     Changed SYNC_BUFFER_TIME to BUFFER_SLICE_TIME
**  7/28/97     Changed pack structure alignment for SPARC from 8 to 4. Compiler bug.
**  8/8/97      Added PV_FreePgmEntries
**  8/27/97     Moved GM_StartHardwareSoundManager & GM_StopHardwareSoundManager to
**              GenSnd.h
**  9/2/97      Fixed bug with THE_CHECK that forgot to look at zero length buffers
**  9/19/97     Changed name of PV_FreePatchInfo.
**              Added PV_InsertBankSelect
**  10/15/97    Added processingSlice to NoteRecord to handle threading issues
**  10/27/97    Removed reference to MusicGlobals->theSongPlaying
**  10/28/97    Eliminated reference to FAR
**  10/29/97    Promoted PV_AnyStereoInstrumentsLoaded to GM_AnyStereoInstrumentsLoaded and
**              moved it to GenSnd.h
**  12/4/97     Renamed GM_Mixer to GM_Mixer. Renamed NoteRecord to GM_Voice
**  1/14/98     kk: added NoteLoopTarget to GM_Voice (number of loops between loop points desired
**              before sample continues to end.
**              changed NoteLoopCount from XBYTE to XDWORD because we are actually counting loops now and  
**              may want quite a few.
**  1/27/98     Renamed MACINTOSH to H_MACINTOSH
**  2/3/98      Renamed songBufferLeftMono to songBufferDry
**  2/5/98      Added a GM_Song pointer to PV_SetSampleIntoCache
**  2/8/98      Changed BOOL_FLAG to XBOOL
**  2/10/98     added a bunch of structures for storing new effect parameters
**  2/20/98     kcr converted floating-point to fixed for new effects -- added chorus buffer
**  2/23/98     Removed last of old variable reverb code
**  2/24/98     kcr deal with sample-rate changes for chorus and reverb...
**  3/16/98     Removed PV_ProcessReverbMono & PV_ProcessReverbStereo & PV_PostFilterStereo
**              from public view
**              Changed InitNewReverb to return a XBOOL for success or failure
**  4/1/98      MOE: took out references to FilterEnvelope{} so that all compiles
**  4/14/98     Added some comments and removed extra structures that are not being used
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
**  7/1/98      Changed various API to use the new XResourceType and XLongResourceID
**  7/7/98      Removed reverbIsVariable from GM_Mixer structure. Using function 
**              GM_IsReverbFixed instead.
**  7/28/98     Renamed inst_struct to pInstrument
**              Changed meaning of processExternalMidiQueue in GM_Song. Now its a counter
**              instead of just a boolean.
**  7/30/98     Added constant value to MAX_CHUNK_SIZE for 48k output in GM_Mixer structure
**  8/12/98     Added PV_ModifyVelocityFromCurve
**  10/27/98    Moved MIN_LOOP_SIZE to GenSnd.h
**  11/9/98     Renamed NoteDur to voiceMode
**  12/22/98    Removed old USE_SEQUENCER flag
**  1/12/99     Added a useKatmaiCPU flag that is dynamic if the USE_KAT flag
**              is set to build.
**  3/1/99      Changed NoteRefNum to NoteContext
**  3/3/99      Added PV_GetVoiceFromSoundReference
**              Added voiceStartTimeStamp to GM_Voice
**              Removed USE_DIRECT_MIXDOWN
**  3/5/99      Added VOICE_ALLOCATED_READY_TO_SYNC_START
**              Added threadContext to PV_ServeEffectCallbacks & PV_ProcessSampleFrame &
**              PV_ProcessSequencerEvents & PV_ProcessMidiSequencerSlice
**  3/11/99     Renamed ADSRRecord to GM_ADSR. Renamed LFORecord to GM_LFO. Renamed CurveRecord to GM_TieTo.
**  3/31/99     Added pMixer pointer to the GM_Voice structure.
**              Renamed ServeMIDINote to PV_StartMIDINote, renamed StopMIDINote to PV_StopMIDINote
**  5/10/99     Removed CODE_TYPE
**  5/28/99     MOE:  Moved DEFAULT_ constants to GenSnd.h
**  6/8/99      Modified Q_MIDIEvent and added an status byte for type of event to work with the new
**              wire event model.
**  6/15/99     Changed PV_CleanExternalQueue parmeters
**  7/9/99      Added taskReference for the Taskcallback
**  7/19/99     Renamed UBYTE to XBYTE. Renamed INT16 to XSWORD. Renamed INT32 to XSDWORD.
**              Renamed UINT32 to XDWORD. Renamed SBYTE to XSBYTE. Renamed UINT16 to XWORD
**  8/3/99      Changed pragma settings for X_BE
**  10/19/99    MSD: switched to REVERB_USED and LOOPS_USED
**  10/30/99    Removed cacheBlockID field from CacheSampleInfo
**              Removed extra LFORecords in GM_Voice structure
**              Added the ability to change the heartbeat from 11610 ms to 10000 ms with
**              a compile time swtich. See BUFFER_SLICE_TIME
**  11/10/99    Set default to 10 ms slice time
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  3/21/2000   Set default to 11 ms slice time. See BUFFER_SLICE_TIME.
**  5/9/2000    sh  Added FIXED_BUFFER_SLICE_TIME & FIXED_MAX_CHUNK_SIZE. Need
**                  this is allow for different audio render rates, because our
**                  midi decoder and lfo's run at a fixed 11.61 ms rate.
**  2000.05.15 AER  Renamed CacheSampleInfo to GM_SampleCacheEntry
**  2000.05.16 AER  Completed modifications for new sample cache
**  2000.05.28 sh   Added PV_UnloadInstrumentData, and documented PV_GetInstrument
**  2000.06.01 sh   Changed element voiceStartTimeStamp in GM_Voice structure
**                  to a XDWORD.
**  7/07/2000  DS:  Increased MIDI queue size to 1024 for Windows platform.  Added
**                  mutex struct to GM_Mixer as future placeholder, but #ifdef'd out.
**  7/11/2000  DS:  Added NoteStartFrame member to GM_Voice.
**  9/7/2000    sh  Comments added to NoteProgram for GM_Voice. Increased size to a
**                  XLongResourceID to better reflect what how the value is defined.
**  2/18/2001   sh  Added GM_Mixer::outputReference for callbacks.
**              sh  Enabled bitfields for GM_Voice & GM_Mixer
**  3/28/2001   sh  grrr. Corrected (!) bitfield size of GM_Voice::sustainMode.
**                  Suppose to be 2 bits, not 1.
**  4/18/2001   sh  Added PV_GetVoiceNumberFromVoice
**  4/19/2001   sh  Removed songBufferDry from GM_Mixer for split build
**                  Add U3232_TO_XFIXED & ADD_32_16
**  4/23/2001   sh  Added PV_CalculateMonoVolume
**  5/23/2001   sh  Removed bitfields. Failed on gcc.
**  7/5/2001        Modified parmeters of PV_GetInstrument
*/
/*****************************************************************************/

#ifndef G_PRIVATE
#define G_PRIVATE

#include "BAE_API.h"
#include "X_API.h"
#include "GenSnd.h"

#define VOLUME_PRECISION_SCALAR     6L      // used to be 8, so we must scale down output by 2
#define OUTPUT_SCALAR               9L      // 9 for volume minus 4 for increased volume_range resolution, plus 2 for increased volume precision scalar
#define VOLUME_RANGE                4096    // original range was 256, therefore:
#define UPSCALAR                    16L     // multiplier (NOT a shift count!) for increasing amplitude resolution
#define MAXRESONANCE                127     // mask and buffer size for resonant filter.  Higher means wider frequency range.

// BUFFER_SLICE_TIME is calculated by the formula:
//
// 1 second / sample rate * samples
// 1 000 000 / 22050 * 256
//
// the amount of time in microseconds that
// passes when calling ProcessSampleFrame
#if 1
    #define BUFFER_SLICE_TIME           11610
#else
    #define BUFFER_SLICE_TIME           10000
#endif

// These times are fixed because our LFO's, and midi decode code
// relies on the constant 11.6 ms decode rate. This allows for content to
// sound the same. 
#define FIXED_BUFFER_SLICE_TIME         11610
#define FIXED_MAX_CHUNK_SIZE            512

#if BUFFER_SLICE_TIME == 5000
    #define MAX_CHUNK_SIZE          224     // max samples to build per slice at 44k
#endif

#if BUFFER_SLICE_TIME == 10000
    #define MAX_CHUNK_SIZE          448     // max samples to build per slice at 44k
#endif

#if BUFFER_SLICE_TIME == 11610
    #define MAX_CHUNK_SIZE          512     // max samples to build per slice at 44k
#endif

#ifndef MAX_CHUNK_SIZE
    #error "MAX_CHUNK_SIZE not defined!" 
#endif

#if (MAX_CHUNK_SIZE%16) != 0
    #error "Bad MAX_CHUNK_SIZE, Divisible by 16 only!" 
#endif


#define SOUND_EFFECT_CHANNEL        16      // channel used for sound effects. One beyond the normal

// 20.12 (whole.fractional)
#define STEP_BIT_RANGE              12L
#define STEP_OVERFLOW_FLAG          (1<<(STEP_BIT_RANGE-1))     
#define STEP_FULL_RANGE             ((1<<STEP_BIT_RANGE)-1)

#define ALLOW_16_BIT            1           // 1 - allow 16 bit if available, 0 - force 8 bit
#define ALLOW_STEREO            1           // 1 - allow stereo if available, 0 - force mono
#define ALLOW_DEBUG_STEREO      0           // 1 - allow keyboard debugging of stereo code
#define USE_DLS                 0           // 1 - allow DLS changes, 0 - IGOR

#if USE_CALLBACKS
// a macro to handle broken loops and partial buffers in the inner loop code
#define THE_CHECK(TYPE) \
    if (cur_wave >= end_wave)\
    {\
        if (looping)\
        {\
            cur_wave -= wave_adjust;    /* back off pointer for previous sample*/ \
            if (this_voice->doubleBufferProc)\
            {\
                /* we hit the end of the loop call double buffer to notify swap*/ \
                if (PV_DoubleBufferCallbackAndSwap(this_voice->doubleBufferProc, this_voice)) \
                {\
                    /* recalculate our internal pointers */\
                    end_wave = (XFIXED)(this_voice->NoteLoopEnd - this_voice->NotePtr) << STEP_BIT_RANGE;\
                    wave_adjust =  (XFIXED)(this_voice->NoteLoopEnd - this_voice->NoteLoopPtr) << STEP_BIT_RANGE;\
                    source = (TYPE) this_voice->NotePtr;\
                }\
                else\
                {\
                    goto FINISH;\
                }\
            }\
        }\
        else\
        {\
            this_voice->voiceMode = VOICE_UNUSED;\
            PV_DoCallBack(this_voice);\
            goto FINISH;\
        }\
    }
#else
// a macro to handle broken loops and partial buffers in the inner loop code
#define THE_CHECK(TYPE) \
    if (cur_wave >= end_wave)\
    {\
        if (looping)\
        {\
            cur_wave -= wave_adjust;    /* back off pointer for previous sample*/ \
        }\
        else\
        {\
            this_voice->voiceMode = VOICE_UNUSED;\
            goto FINISH;\
        }\
    }
#endif


#if LOOPS_USED == U3232_LOOPS

typedef unsigned long   U32;
typedef struct U3232
{
    U32     i;
    U32     f;
} U3232;


#define ADD_32_32(target_int, target_frac, addend_int, addend_frac)\
    {\
        target_frac += addend_frac;\
        if ((U32)target_frac < (U32)addend_frac) target_int++;\
        target_int += addend_int;\
    }
#define ADD_U3232(target_int, target_frac, u3232)\
    ADD_32_32(target_int, target_frac, u3232.i, u3232.f)

#define U3232_TO_XFIXED(u3232)  \
    (((u3232.i&0xFFFFL)<<16L) | (u3232.f>>16L))

#define ADD_32_16(target_int, target_frac, u1616) \
    ADD_32_32(target_int, target_frac, ((u1616&0xFFFF0000L)>>16L), ((u1616&0xFFFFL) << 16L))


#if USE_CALLBACKS
#define THE_CHECK_U3232(TYPE) \
    if (cur_wave_i >= end_wave)\
    {\
        if (looping)\
        {\
            cur_wave_i -= wave_adjust;  /* back off pointer for previous sample*/\
/*          cur_wave_f = 0; TRY PUTTING THIS IN SOME DAY, MIGHT SOUND BETTER */\
            if (this_voice->doubleBufferProc)\
            {\
                /* we hit the end of the loop call double buffer to notify swap*/ \
                if (PV_DoubleBufferCallbackAndSwap(this_voice->doubleBufferProc, this_voice)) \
                {\
                    /* recalculate our internal pointers */\
                    end_wave = this_voice->NoteLoopEnd - this_voice->NotePtr;\
                    wave_adjust = this_voice->NoteLoopEnd - this_voice->NoteLoopPtr;\
                    source = (TYPE)this_voice->NotePtr;\
                }\
                else\
                {\
                    goto FINISH;\
                }\
            }\
        }\
        else\
        {\
            this_voice->voiceMode = VOICE_UNUSED;\
            PV_DoCallBack(this_voice);\
            goto FINISH;\
        }\
    }
#else
#define THE_CHECK_U3232(TYPE) \
    if (cur_wave_i >= end_wave)\
    {\
        if (looping)\
        {\
            cur_wave_i -= wave_adjust;  /* back off pointer for previous sample*/\
/*          cur_wave_f = 0; TRY PUTTING THIS IN SOME DAY, MIGHT SOUND BETTER */\
        }\
        else\
        {\
            this_voice->voiceMode = VOICE_UNUSED;\
            goto FINISH;\
        }\
    }
#endif
#endif


#if LOOPS_USED == FLOAT_LOOPS
#if USE_CALLBACKS
#define THE_CHECK_FLOAT(TYPE) \
    if (cur_wave >= end_wave)\
    {\
        if (looping)\
        {\
            cur_wave -= wave_adjust;    /* back off pointer for previous sample*/ \
            if (this_voice->doubleBufferProc)\
            {\
                /* we hit the end of the loop call double buffer to notify swap*/ \
                if (PV_DoubleBufferCallbackAndSwap(this_voice->doubleBufferProc, this_voice)) \
                {\
                    /* recalculate our internal pointers */\
                    end_wave = (this_voice->NoteLoopEnd - this_voice->NotePtr);\
                    wave_adjust = (this_voice->NoteLoopEnd - this_voice->NoteLoopPtr);\
                    source = (TYPE) this_voice->NotePtr;\
                }\
                else\
                {\
                    goto FINISH;\
                }\
            }\
        }\
        else\
        {\
            this_voice->voiceMode = VOICE_UNUSED;\
            PV_DoCallBack(this_voice);\
            goto FINISH;\
        }\
    }
#else
#define THE_CHECK_FLOAT(TYPE) \
    if (cur_wave >= end_wave)\
    {\
        if (looping)\
        {\
            cur_wave -= wave_adjust;    /* back off pointer for previous sample*/ \
        }\
        else\
        {\
            this_voice->voiceMode = VOICE_UNUSED;\
            goto FINISH;\
        }\
    }
#endif
#endif

typedef unsigned char           OUTSAMPLE8;
typedef short int               OUTSAMPLE16;        // 16 bit output sample

enum
{
    SUS_NORMAL          =   0,      // normal release at note off
    SUS_ON_NOTE_ON      =   1,      // note on, with pedal
    SUS_ON_NOTE_OFF     =   2       // note off, with pedal
};


#define X_PACK_FAST
#include "X_PackStructures.h"

// Mode in which a GM_Voice is currently being used
typedef enum 
{
    // These are left as reference. They refer to the old code base of what the numbers
    // ment.
//  VOICE_UNUSED        =   -1,             // voice is free
//  VOICE_RELEASING     =   0,              // voice is releasing
//  VOICE_SUSTAINING    =   32767,          // voice is sustaining
//  VOICE_ALLOCATED     =   1               // voice is allocated, but not active

    VOICE_UNUSED        =   0,              // voice is free
    VOICE_ALLOCATED,                        // voice is allocated, but not active
    VOICE_ALLOCATED_READY_TO_SYNC_START,    // voice is allocated, ready to start and on the next slice it will
                                            // set them to VOICE_SUSTAINING. This will look at the syncVoiceReference
                                            // variable and start all voices with the same reference
    VOICE_RELEASING,                        // voice is releasing
    VOICE_SUSTAINING                        // voice is sustaining
} VoiceMode;

// Need a forward reference to the GM_Mixer struct to keep
// our compiler from complaining.
struct GM_Mixer;

// This structure is created and maintained for each sample that is to mixed into the final output
struct GM_Voice
{
    VoiceMode               voiceMode;              // duration of note to play. VOICE_UNUSED is dead
                                                    // This field must be first!
    void                    *syncVoiceReference;    // this field is used when voiceMode has been set to VOICE_ALLOCATED_READY_TO_SYNC_START
                                                    // A single pass search will happen and it will look for matching syncVoiceReference
                                                    // values. Once the voice is started it will be set to NULL.
    XSWORD                  NoteDecay;              // after voiceMode == VOICE_RELEASING then this is ticks of decay
    XDWORD                  voiceStartTimeStamp;    // this is a time stamp of when this voice is started, used to
                                                    // track unique voices
    GM_Instrument           *pInstrument;           // read-only pointer to instrument information
    GM_Song                 *pSong;                 // read-only pointer to song information
    struct GM_Mixer         *pMixer;                // read-only pointer to mixer information
                                                    // used to backtrace where note came from
    XBYTE                   *NotePtr;               // pointer to start of sample
    XBYTE                   *NotePtrEnd;            // pointer to end of sample
    XDWORD                  NoteStartFrame;         // offset to start of sample in frames
#if LOOPS_USED == U3232_LOOPS
    U3232                   samplePosition;         // new index from NotePtr
#endif
#if LOOPS_USED == FLOAT_LOOPS
    UFLOAT                  samplePosition_f;       // new index from NotePtr
#endif
    XFIXED                  NoteWave;               // current fractional position within sample (NotePtr:NotePtrEnd)
    XFIXED                  NotePitch;              // playback pitch in 16.16 fixed. 1.0 will play recorded speed
    XFIXED                  noteSamplePitchAdjust;  // adjustment to pitch based on difference from 22KHz in recorded rate
    XBYTE                   *NoteLoopPtr;           // pointer to start of loop point within NotePtr & NotePtrEnd
    XBYTE                   *NoteLoopEnd;           // pointer to end of loop point within NotePtr & NotePtrEnd
    
    // $$kk: 01.14.98: added NoteLoopTarget
    XDWORD                  NoteLoopTarget;         // target number of loops before continuing to end of sample

#if USE_CALLBACKS
    void                    *NoteContext;           // user context for callbacks
// Double buffer variables. If using double buffering, then doubleBufferPtr1 will be non-zero. These variables
// will be swapped with NotePtr, NotePtrEnd, NoteLoopPtr, NoteLoopPtrEnd
    XBYTE                       *doubleBufferPtr1;
    XBYTE                       *doubleBufferPtr2;
    GM_DoubleBufferCallbackPtr  doubleBufferProc;

// Call back procs
    GM_LoopDoneCallbackPtr      NoteLoopProc;       // normal loop continue proc
    GM_SoundDoneCallbackPtr     NoteEndCallback;    // sample done callback proc
#endif

    XSWORD                  NoteNextSize;           // number of samples per slice. Use 0 to recalculate
    XSBYTE                  NoteMIDIPitch;          // midi note pitch to start note
    XSBYTE                  noteOffsetStart;        // at the start of the midi note, what was the offset
    XSWORD                  ProcessedPitch;         // actual pitch to play (proccessed)
    XLongResourceID         NoteProgram;            // note program number. This is a combined value
                                                    // of program and bank.
    XSBYTE                  NoteChannel;            // channel note is playing on
    XSBYTE                  NoteTrack;              // track note is playing on
    XSDWORD                 NoteVolume;             // note volume (scaled)
    XSWORD                  NoteVolumeEnvelope;     // scalar from volume ADSR and LFO's.  0 min, VOLUME_RANGE max.
    XSWORD                  NoteVolumeEnvelopeBeforeLFO;    // as described.
    XSWORD                  NoteMIDIVolume;         // note volume (unscaled)
    XSWORD                  NotePitchBend;          // 8.8 Fixed amount of bend
    XSWORD                  ModWheelValue;          // 0-127
    XSWORD                  LastModWheelValue;      // has it changed?  This is how we know.
    XSWORD                  LastPitchBend;          // last bend
    XSWORD                  stereoPosition;         // -63 (left) 0 (Middle) 63 (Right)
    XSWORD                  routeBus;

    // $$kk: 01.14.98: changed NoteLoopCount from XBYTE to XSDWORD because we are actually counting loops now and may want quite a few
    XDWORD                  NoteLoopCount;

    XBYTE                   bitSize;                // 8 or 16 bit data
    XBYTE                   channels;               // mono or stereo data
    XBYTE                   sustainMode;            // sustain mode, for pedal controls
    XBYTE                   sampleAndHold;          // flag whether to sample & hold, or sample & release
    XBYTE                   processingSlice;        // if TRUE, then thread is processing slice of this instrument
    XBYTE                   avoidReverb;            // don't mix into reverb unit
    XDWORD                  largestPeak;
#if REVERB_USED != REVERB_DISABLED
    XBYTE                   reverbLevel;            // 0-127 when reverb is enabled
#endif

// sound effects variables. Not used for normal envelope or instruments
    XBYTE                   soundEndAtFade;
    XFIXED                  soundFadeRate;          // when non-zero fading is enabled
    XFIXED                  soundFixedVolume;       // inital volume level that will be changed by soundFadeRate
    XSWORD                  soundFadeMaxVolume;     // max volume
    XSWORD                  soundFadeMinVolume;     // min volume
#if USE_CALLBACKS
    GM_SampleCallbackEntry  *pSampleMarkList;       // linked list of callbacks on a per sample frame basis
#endif

    XSDWORD                 stereoPanBend;

    GM_ADSR                 volumeADSRRecord;
    XSDWORD                 volumeLFOValue;
    XSWORD                  LFORecordCount;
    GM_LFO                  LFORecords[MAX_LFOS];   // allocate for maximum allowed
    XSDWORD                 lastAmplitudeL;
    XSDWORD                 lastAmplitudeR;         // used to interpolate between points in volume ADSR
#if REVERB_USED != REVERB_DISABLED
    XSWORD                  chorusLevel;            // 0-127 when chorus is enabled
#endif
    XSWORD                  z[MAXRESONANCE+1];
    XSDWORD                 zIndex, Z1value, previous_zFrequency;
    XSDWORD                 LPF_lowpassAmount, LPF_frequency, LPF_resonance;
    XSDWORD                 LPF_base_lowpassAmount, LPF_base_frequency, LPF_base_resonance;
//  XSDWORD                 s1Left, s2Left, s3Left, s4Left, s5Left, s6Left; // for INTERP3 mode only
};
typedef struct GM_Voice GM_Voice;

// support for historical reasons
#define NoteRecord  GM_Voice

// Structure used for caching samples for instruments
struct GM_SampleCacheEntry
{
    XSampleID       theID;          // sample ID
    XBankToken      bankToken;      // The unique bank token to supplement theID
    XFIXED          rate;           // sample rate
    unsigned long   waveSize;       // size in bytes
    unsigned long   waveFrames;     // number of frames
    unsigned long   loopStart;      // loop start frame
    unsigned long   loopEnd;        // loop end frame
    char            bitSize;        // sample bit size; 8 or 16
    char            channels;       // mono or stereo; 1 or 2
    short int       baseKey;        // base sample key
    long            referenceCount; // how many references to this sample block
    void            *pSampleData;   // pointer to sample data. This may be an offset into the pMasterPtr
    void            *pMasterPtr;    // master pointer that contains the snd format information
};
typedef struct GM_SampleCacheEntry GM_SampleCacheEntry;

#define MAX_QUEUE_EVENTS                1024

#define REVERB_BUFFER_SIZE_SMALL        4096        // * sizeof(long)
#define REVERB_BUFFER_MASK_SMALL        4095

#if REVERB_USED == SMALL_MEMORY_REVERB
    #define REVERB_BUFFER_SIZE          REVERB_BUFFER_SIZE_SMALL
    #define REVERB_BUFFER_MASK          REVERB_BUFFER_MASK_SMALL
#elif REVERB_USED == VARIABLE_REVERB
    #define REVERB_BUFFER_SIZE          16384
    #define REVERB_BUFFER_MASK_SHORT    16383
    #define REVERB_BUFFER_MASK          32767
#endif


enum
{
    Q_MIDI_DEAD = 0,
    Q_MIDI_ALLOCATING,
    Q_MIDI_READY
};

// This structure is to allow for queuing midi events into the playback other than those that are
// pulled from the midi file stream
struct Q_MIDIEvent
{
    GM_Song         *pSong;         // pSong the event was placed from
    XDWORD          timeStamp;      // timestamp of event
    XBYTE           status;         // status of event: 0 - dead, 1 - allocating, 2 - ready
    XBYTE           midiChannel;    // which channel
    XBYTE           command;        // which command
    XBYTE           byte1;          // note, controller
    XBYTE           byte2;          // velocity, lsb/msb
};
typedef struct Q_MIDIEvent Q_MIDIEvent;

typedef void            (*InnerLoop)(GM_Voice *pVoice);
typedef void            (*InnerLoop2)(GM_Voice *pVoice, XBOOL looping);

// tried to 8 byte align structure (7/17/97)
struct GM_Mixer
{
    TerpMode            interpolationMode;              // output interpolation mode
    Rate                outputRate;                 // output sample rate

    ReverbMode          reverbUnitType;                 // verb mode
    ReverbMode          reverbTypeAllocated;            // verb mode allocated

    XBYTE               sampleFrameSize;                // size in bytes of each sample frame
    XBYTE               sampleExpansion;                // output expansion factor 1, 2, or 4
    XSWORD              MasterVolume;

    XSWORD              effectsVolume;                  // volume multiplier of all effects
    XSDWORD             scaleBackAmount;
    XSWORD              routeBus;

    XSWORD              MaxNotes;
    XSWORD              mixLevel;
    XSWORD              MaxEffects;
    XSWORD              maxChunkSize;
    XDWORD              bufferTime;
    XDWORD              lfoBufferTime;

    XWORD               One_Slice, One_Loop, Two_Loop, Four_Loop;
    XWORD               Sixteen_Loop;

    XBOOL       /*0*/   generate16output;               // if TRUE, then build 16 bit output
    XBOOL       /*1*/   generateStereoOutput;           // if TRUE, then output stereo data
    XBOOL       /*2*/   insideAudioInterrupt;
    XBOOL       /*3*/   systemPaused;                   // all sound paused and disengaged from hardware

    XBOOL       /*4*/   enableDriftFixer;               // if enabled, this will fix the drift of real time with our synth time.
    XBOOL       /*5*/   sequencerPaused;                // MIDI sequencer paused
    XBOOL       /*6*/   cacheInstruments;               // current not used

    XBOOL       /*7*/   stereoFilter;                   // if TRUE, then filter stereo output
    XBYTE       /*0*/   processExternalMidiQueue;       // counter flag to lock processing of queue. 0 means process
    GM_SampleCacheEntry *sampleCaches[MAX_SAMPLES];     // cache of samples loaded

    // voice allocation, and dry and wet mix buffers
    GM_Voice            NoteEntry[MAX_VOICES];
#ifdef BAE_COMPLETE
    XSDWORD             songBufferDry[(MAX_CHUNK_SIZE+64)*2];   // interleaved samples: left-right
#if REVERB_USED != REVERB_DISABLED
    XSDWORD             songBufferReverb[MAX_CHUNK_SIZE+64];    // the +64 is for 48k output
    XSDWORD             songBufferChorus[MAX_CHUNK_SIZE+64];
#endif
#endif

// MIDI Interpreter variables
    GM_Song             *pSongsToPlay[MAX_SONGS];       // number of songs to play at once

// normal inner loop procs
    InnerLoop2          partialBufferProc;
    InnerLoop           fullBufferProc;
    InnerLoop2          partialBufferProc16;
    InnerLoop           fullBufferProc16;

// procs for resonant low-pass filtering
    InnerLoop2          filterPartialBufferProc;
    InnerLoop           filterFullBufferProc;
    InnerLoop2          filterPartialBufferProc16;
    InnerLoop           filterFullBufferProc16;

// external midi control variables
    Q_MIDIEvent         theExternalMidiQueue[MAX_QUEUE_EVENTS];

// lock for queue access
    BAE_Mutex           queueLock;

// pointers for circular event buffer
    Q_MIDIEvent         *pHead;                         // pointer to events to read from queue
    Q_MIDIEvent         *pTail;                         // pointer to events to write to queue
                                                        // always points to the next one to use
    XDWORD              syncCount;                      // in microseconds. Current tick of audio output
    XSDWORD             syncBufferCount;

    XDWORD              samplesPlayed;                  // number of samples played by device
    XDWORD              samplesWritten;                 // number of samples written to device
    XDWORD              lastSamplePosition;             // last time GM_UpdateSamplesPlayed was called

    XDWORD              timeSliceDifference;            // value in microseconds between calls to
                                                        // HAE_BuildMixerSlice
#if USE_CALLBACKS
    GM_AudioTaskCallbackPtr     pTaskProc;              // callback for audio tasks
    void                        *taskReference;
    GM_AudioOutputCallbackPtr   pOutputProc;            // callback for audio output
#endif
#if REVERB_USED != REVERB_DISABLED
// variables used for "classic" fixed verb
    XSDWORD             *reverbBuffer;          // this is the master pointer used
                                                // for verb. It is shared between
                                                // different types of verbs, although
                                                // the data maybe different
    XDWORD              reverbBufferSize;       // Set the size of memory allocated here.
                                                // Make sure you set this because it is
                                                // compared and tested against
    XSDWORD             reverbPtr;              // delay line index into verb buffer
    XSDWORD             LPfilterL, LPfilterR;   // used for fixed verb
    XSDWORD             LPfilterLz, LPfilterRz;
#endif
};
typedef struct GM_Mixer GM_Mixer;

// support for historical reasons
#define MusicVars   GM_Mixer

#include "X_UnpackStructures.h"

#ifdef __cplusplus
    extern "C" {
#endif

extern GM_Mixer *MusicGlobals;

#if USE_NEW_EFFECTS
/******************************* new reverb stuff *****************************/

#define kCombBufferFrameSize            4096    /* 5000 */
#define kDiffusionBufferFrameSize       4096    /* 4410 */
#define kStereoizerBufferFrameSize      1024    /* 1000 */
#define kEarlyReflectionBufferFrameSize 0x2000  /* 0x1500 */

#define kCombBufferMask                 (kCombBufferFrameSize - 1)
#define kDiffusionBufferMask            (kDiffusionBufferFrameSize - 1)
#define kStereoizerBufferMask           (kStereoizerBufferFrameSize - 1)
#define kEarlyReflectionBufferMask      (kEarlyReflectionBufferFrameSize - 1)


#define kNumberOfCombFilters        6
#define kNumberOfEarlyReflections   7


#define kNumberOfDiffusionStages    3


struct NewReverbParams
{
    XBOOL               mIsInitialized;
    Rate                mSampleRate;
    XSDWORD             mReverbType;    
    
    /* early reflection params */
    XSDWORD             *mEarlyReflectionBuffer;
    XSDWORD             mEarlyReflectionGain[kNumberOfEarlyReflections];
    int                 mReflectionWriteIndex;
    int                 mReflectionReadIndex[kNumberOfEarlyReflections];
    
    
    /* comb filter params */    
    XSDWORD             *mReverbBuffer[kNumberOfCombFilters];
    
    int                 mReadIndex[kNumberOfCombFilters];
    int                 mWriteIndex[kNumberOfCombFilters];
    
    long                mUnscaledDelayFrames[kNumberOfCombFilters];
    long                mDelayFrames[kNumberOfCombFilters];
    
    XSDWORD                 mFeedbackList[kNumberOfCombFilters];
    
    XSDWORD             mRoomSize;
    XSDWORD             mRoomChoice;
    XSDWORD             mMaxRegen;      // 0-127
    XSDWORD             mDiffusedBalance;
    
    /* diffusion params */
    XSDWORD             *mDiffusionBuffer[kNumberOfDiffusionStages];
    int                 mDiffReadIndex[kNumberOfDiffusionStages];
    int                 mDiffWriteIndex[kNumberOfDiffusionStages];
    
    /* output filter */
    XSDWORD             mLopassK;
    XSDWORD             mFilterMemory;
    
    /* stereoizer params */
    XSDWORD             *mStereoizerBufferL;
    XSDWORD             *mStereoizerBufferR;
    int                 mStereoReadIndex;
    int                 mStereoWriteIndex;
};

typedef struct NewReverbParams NewReverbParams;

extern NewReverbParams      gNewReverbParams;

/* prototypes */
NewReverbParams*    GetNewReverbParams();
XBOOL InitNewReverb();  // returns TRUE if success
void ShutdownNewReverb();
XBOOL CheckReverbType();
void ScaleDelayTimes();
void GenerateDelayTimes();
void GenerateFeedbackValues();
void SetupDiffusion();
void SetupStereoizer();
void SetupEarlyReflections();
void RunNewReverb(XSDWORD *sourceP, XSDWORD *destP, int nSampleFrames);
XDWORD GetSamplingRate();
XDWORD GetSR_44100Ratio();
XDWORD Get44100_SRRatio();

/******************************* new chorus stuff *****************************/
#define kChorusBufferFrameSize      4410L

struct ChorusParams
{
    XBOOL               mIsInitialized;
    Rate                mSampleRate;
    
    XSDWORD*                mChorusBufferL;
    XSDWORD*                mChorusBufferR;

    int                 mWriteIndex;
    XSDWORD             mReadIndexL;
    XSDWORD             mReadIndexR;
    
    int                 mSampleFramesDelay;

    XSDWORD             mRate;
    //float             mDepth;
    XSDWORD             mPhi;
    
    XSDWORD             mFeedbackGain;  // between 0-127
};

typedef struct ChorusParams ChorusParams;

/* prototypes */
ChorusParams* GetChorusParams();
void InitChorus();
void ShutdownChorus();
XSDWORD GetChorusReadIncrement(XSDWORD readIndex, long writeIndex, long nSampleFrames, XSDWORD phase);
void SetupChorusDelay();
void RunChorus(XSDWORD *sourceP, XSDWORD *destP, int nSampleFrames);


#if 0   // only reverb and chorus are currently activated...

/******************************* delay stuff *****************************/
#define kDelayBufferFrameSize       44100

struct DelayEffect
{
    XSDWORD*                mDelayBuffer;

    int                 mWriteIndex;
    int                 mReadIndex;
    
    float               mSecondsDelay;
    
    float               mFeedbackValue;
    float               mFeedbackGain;
    
    XSDWORD             mFilterMemoryL;
    XSDWORD             mFilterMemoryR;
    XSDWORD             mLopassK;
};

typedef struct DelayEffect DelayEffect;

/* prototypes */
void Delay_Initialize(DelayEffect *This);
void Delay_Shutdown(DelayEffect *This);
void Delay_Run(DelayEffect *This, XSDWORD *sourceP);

extern DelayEffect      gDelay;


/******************************* graphic eq stuff *****************************/
#define kNumberOfBands      7

struct GraphicEqParams
{
    /* right and left filter memory */
    XSDWORD     mHistory1L[kNumberOfBands];
    XSDWORD     mHistory2L[kNumberOfBands];
    XSDWORD     mHistory1R[kNumberOfBands];
    XSDWORD     mHistory2R[kNumberOfBands];
    
    float       mControlList[kNumberOfBands];       /* values between 0.0 and 1.0 */
    float       mGain[kNumberOfBands];
};

typedef struct GraphicEqParams GraphicEqParams;

/* prototypes */
GraphicEqParams* GetGraphicEqParams();
void InitGraphicEq();
void CalculateGraphicEqGains();
void RunGraphicEq(XSDWORD *sourceP, int nSampleFrames);


/******************************* parametric eq stuff *****************************/

struct ParametricEq
{
    float   mFreqValue;
    float   mQValue;
    float   mGainValue;

    float   mControlList[3];
    
    double  pi;

    float   sweep;
    
    /* filter memory */
    XSDWORD x1;
    XSDWORD x2;
    XSDWORD y1;
    XSDWORD y2;
    
    /* filter coefficients */
    float   b0;
    float   b1;
    float   b2;
    float   a1;
    float   a2;
};

typedef struct ParametricEq ParametricEq;

/* prototypes */
void    ParametricEq_Initialize(ParametricEq *This);
void    ParametricEq_CalculateParams(ParametricEq *This);
void    ParametricEq_Run(ParametricEq *This, XSDWORD *buffer);

extern ParametricEq     gParametricEq;

/******************************* resonant filter stuff *****************************/

struct ResonantFilterParams
{
    float   mFrequency;
    float   mResonance;

    float   mControlList[2];
    
    double  pi;

    float   sweep;
    
    /* filter memory */
    XSDWORD y1;
    XSDWORD y2;
    
    /* filter coefficients */
    float   c0;
    float   c1;
    float   c2;
};

typedef struct ResonantFilterParams ResonantFilterParams;

/* prototypes */
ResonantFilterParams* GetResonantFilterParams();
void InitResonantFilter();
void CalculateResonantParams(float inFrequency, float inResonance);
void RunResonantFilter(XSDWORD *buffer, int nSampleFrames);

#endif // 0

#endif // USE_NEW_EFFECTS
/******************************************************************************/





// internal function declarations

void PV_Generate8outputStereo(OUTSAMPLE8 * dest8);
void PV_Generate8outputMono(OUTSAMPLE8 * dest8);
void PV_Generate16outputStereo(OUTSAMPLE16 * dest16);
void PV_Generate16outputMono(OUTSAMPLE16 * dest16);

long PV_DoubleBufferCallbackAndSwap(GM_DoubleBufferCallbackPtr doubleBufferCallback, 
                                        GM_Voice *this_voice);
void PV_CalculateStereoVolume(GM_Voice *this_voice, XSDWORD *pLeft, XSDWORD *pRight);
void PV_CalculateMonoVolume(GM_Voice *pVoice, XSDWORD *pVolume);

void PV_ProcessSampleEvents(void *threadContext);           // process all sample events

#if LOOPS_USED == U3232_LOOPS
void PV_ServeU3232FilterFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeU3232StereoFilterFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeU3232FilterFullBufferNewReverb16 (GM_Voice *this_voice);
void PV_ServeU3232StereoFilterFullBufferNewReverb16 (GM_Voice *this_voice);

void PV_ServeU3232FilterFullBuffer (GM_Voice *this_voice);
void PV_ServeU3232StereoFilterFullBuffer (GM_Voice *this_voice);
void PV_ServeU3232FilterFullBuffer16 (GM_Voice *this_voice);
void PV_ServeU3232StereoFilterFullBuffer16 (GM_Voice *this_voice);

void PV_ServeU3232FilterPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoFilterPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232FilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoFilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeU3232FilterPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232FilterPartialBufferNewReverb16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoFilterPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoFilterPartialBufferNewReverb16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeU3232FullBuffer (GM_Voice *this_voice);
void PV_ServeU3232StereoFullBuffer (GM_Voice *this_voice);
void PV_ServeU3232FullBuffer16 (GM_Voice *this_voice);
void PV_ServeU3232StereoFullBuffer16 (GM_Voice *this_voice);

void PV_ServeU3232PartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232PartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeU3232FullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeU3232StereoFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeU3232FullBuffer16NewReverb (GM_Voice *this_voice);
void PV_ServeU3232StereoFullBuffer16NewReverb (GM_Voice *this_voice);

void PV_ServeU3232PartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232PartialBuffer16NewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeU3232StereoPartialBuffer16NewReverb (GM_Voice *this_voice, XBOOL looping);
#endif

#if LOOPS_USED == FLOAT_LOOPS
void PV_ServeFloatFilterFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeStereoFloatFilterFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeFloatFilterFullBufferNewReverb16 (GM_Voice *this_voice);
void PV_ServeStereoFloatFilterFullBufferNewReverb16 (GM_Voice *this_voice);

void PV_ServeFloatFilterFullBuffer (GM_Voice *this_voice);
void PV_ServeStereoFloatFilterFullBuffer (GM_Voice *this_voice);
void PV_ServeFloatFilterFullBuffer16 (GM_Voice *this_voice);
void PV_ServeStereoFloatFilterFullBuffer16 (GM_Voice *this_voice);

void PV_ServeFloatFilterPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatFilterPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeFloatFilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatFilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeFloatFilterPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeFloatFilterPartialBufferNewReverb16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatFilterPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatFilterPartialBufferNewReverb16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeFloatFullBuffer (GM_Voice *this_voice);
void PV_ServeStereoFloatFullBuffer (GM_Voice *this_voice);
void PV_ServeFloatFullBuffer16 (GM_Voice *this_voice);
void PV_ServeStereoFloatFullBuffer16 (GM_Voice *this_voice);

void PV_ServeFloatPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeFloatPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeFloatFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeStereoFloatFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeFloatFullBuffer16NewReverb (GM_Voice *this_voice);
void PV_ServeStereoFloatFullBuffer16NewReverb (GM_Voice *this_voice);

void PV_ServeFloatPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeFloatPartialBuffer16NewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoFloatPartialBuffer16NewReverb (GM_Voice *this_voice, XBOOL looping);
#endif

void PV_ServeInterp2FilterFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeStereoInterp2FilterFullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeInterp2FilterFullBufferNewReverb16 (GM_Voice *this_voice);
void PV_ServeStereoInterp2FilterFullBufferNewReverb16 (GM_Voice *this_voice);

void PV_ServeInterp2FilterFullBuffer (GM_Voice *this_voice);
void PV_ServeStereoInterp2FilterFullBuffer (GM_Voice *this_voice);
void PV_ServeInterp2FilterFullBuffer16 (GM_Voice *this_voice);
void PV_ServeStereoInterp2FilterFullBuffer16 (GM_Voice *this_voice);

void PV_ServeInterp2FilterPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2FilterPartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeInterp2FilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2FilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeInterp2FilterPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeInterp2FilterPartialBufferNewReverb16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2FilterPartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2FilterPartialBufferNewReverb16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeInterp2FullBuffer (GM_Voice *this_voice);
void PV_ServeStereoInterp2FullBuffer (GM_Voice *this_voice);
void PV_ServeInterp2FullBuffer16 (GM_Voice *this_voice);
void PV_ServeStereoInterp2FullBuffer16 (GM_Voice *this_voice);

void PV_ServeInterp2PartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2PartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeInterp2PartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2PartialBuffer16 (GM_Voice *this_voice, XBOOL looping);

void PV_ServeInterp2FullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeStereoInterp2FullBufferNewReverb (GM_Voice *this_voice);
void PV_ServeInterp2FullBuffer16NewReverb (GM_Voice *this_voice);
void PV_ServeStereoInterp2FullBuffer16NewReverb (GM_Voice *this_voice);

void PV_ServeInterp2PartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2PartialBufferNewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeInterp2PartialBuffer16NewReverb (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp2PartialBuffer16NewReverb (GM_Voice *this_voice, XBOOL looping);

void PV_ServeInterp1FullBuffer (GM_Voice *this_voice);
void PV_ServeInterp1PartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoInterp1FullBuffer (GM_Voice *this_voice);
void PV_ServeStereoInterp1PartialBuffer (GM_Voice *this_voice, XBOOL looping);

void PV_ServeDropSampleFullBuffer (GM_Voice *this_voice);
void PV_ServeDropSamplePartialBuffer (GM_Voice *this_voice, XBOOL looping);
void PV_ServeDropSampleFullBuffer16 (GM_Voice *this_voice);
void PV_ServeDropSamplePartialBuffer16 (GM_Voice *this_voice, XBOOL looping);
void PV_ServeStereoAmpFullBuffer (GM_Voice *this_voice);
void PV_ServeStereoAmpPartialBuffer (GM_Voice *this_voice, XBOOL looping);


void PV_StartMIDINote(GM_Song *pSong, XSWORD the_instrument, 
                        XSWORD the_channel, XSWORD the_track, XSWORD notePitch, XSDWORD Volume);
void PV_StopMIDINote(GM_Song *pSong, XSWORD the_instrument, 
                        XSWORD the_channel, XSWORD the_track, XSWORD notePitch);

// voices modifiers
XSWORD SetChannelPitchBend(GM_Song *pSong, XSWORD the_channel, XBYTE bendRange, XBYTE bendMSB, XBYTE bendLSB);
void SetChannelVolume(GM_Song *pSong, XSWORD the_channel, XSWORD newVolume);
XSWORD SetChannelStereoPosition(GM_Song *pSong, XSWORD the_channel, XWORD newPosition);
void SetChannelModWheel(GM_Song *pSong, XSWORD the_channel, XWORD value);
void PV_ChangeSustainedNotes(GM_Song *pSong, XSWORD the_channel, XSWORD data);

void PV_CleanExternalQueue(GM_Mixer *pMixer);

// process 11 ms worth of sample data
void PV_ProcessSampleFrame(void *threadContext, void *destSampleData);
void PV_ProcessSequencerEvents(void *threadContext);

OPErr PV_ProcessMidiSequencerSlice(void *threadContext, GM_Song *pSong);

// MIDI
void PV_ConfigureInstruments(GM_Song *theSong);
OPErr PV_ConfigureMusic(GM_Song *theSong);
void PV_ResetControlers(GM_Song *pSong, XSWORD channel2Reset, XBOOL completeReset);

// GenPatch.c

// given an instrument ID and a bank token or a block of data, load an instrument
// and create a GM_Instrument structure. This will also load any samples required to play
GM_Instrument * PV_GetInstrument(GM_Mixer *pMixer, GM_Song *pSong, 
                                    XLongResourceID theID,
                                     XBankToken bankToken,
                                     void *theExternalX,
                                     long patchSize,
                                     OPErr *pErr);

// unload an instrument and remove all of its memory and optionally the samples
OPErr PV_UnloadInstrumentData(GM_Instrument *theI, GM_Mixer *pMixer, XBOOL freeSamples);

XDWORD PV_ScaleVolumeFromChannelAndSong(GM_Song *pSong, XSWORD channel, XDWORD volume);
#if USE_CALLBACKS
void PV_DoCallBack(GM_Voice *this_one);
#endif
void PV_CleanNoteEntry(GM_Voice * the_entry);
void PV_CalcScaleBack(void);


// given a voice structure, calculate what voice this is
XWORD PV_GetVoiceNumberFromVoice(GM_Voice *pVoice);

XFIXED PV_GetWavePitch(XFIXED notePitch);
#if LOOPS_USED == FLOAT_LOOPS
UFLOAT PV_GetWavePitchFloat(XFIXED notePitch);
#define FRAC(num, num_whole)        ((num) - (num_whole))
#endif
#if LOOPS_USED == U3232_LOOPS
UFLOAT PV_GetWavePitchFloat(XFIXED notePitch);
U3232 PV_GetWavePitchU3232(XFIXED notePitch);
#endif

// get voice sample position from active voice
unsigned long PV_GetPositionFromVoice(GM_Voice *pVoice);
void PV_SetPositionFromVoice(GM_Voice *pVoice, unsigned long pos);

// GenModFiles.c
void PV_WriteModOutput(Rate q, XBOOL stereo);

// GenAudioStreams.c
void PV_ServeStreamFades(void);

// GenSeq.c
void PV_FreePatchInfo(GM_Song *pSong);
void PV_InsertBankSelect(GM_Song *pSong, short channel, short currentTrack);
// process end song callback
void PV_CallSongCallback(void *threadContext, GM_Song *theSong, XBOOL clearCallback);

// GenSynth.c
long PV_ModifyVelocityFromCurve(GM_Song *pSong, long volume);

// GenSample.c
GM_Voice * PV_GetVoiceFromSoundReference(VOICE_REFERENCE reference);

// GenSetup.c
#if (X_PLATFORM == X_WIN95) && (USE_KAT)
XBOOL PV_IntelKatActive(void);
#endif

#ifdef __cplusplus
    }
#endif

#endif  /* G_PRIVATE    */ 
