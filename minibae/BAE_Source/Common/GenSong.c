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
** "GenSong.c"
**
**  Generalized Music Synthesis package. Part of SoundMusicSys.
**
**  © Copyright 1993-2001 Beatnik, Inc, All Rights Reserved.
**  Written by Steve Hales
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
**  This contains code to load an maintain songs and midi data. This code library is specific
**  to the platform, but the API is independent in nature.
**
**  NOTE: relies on structures from SoundMusicSys API
**
**
** Modification History:
**
**  1/24/96     Created
**  1/25/96     Moved GM_GetUsedPatchlist from GenSeq.c
**  1/28/96     Moved GM_FreeSong from GenSeq.c
**  2/3/96      Removed extra includes
**  2/5/96      Removed unused variables. Working towards multiple songs
**              Added GM_GetSongTickLength
**  2/12/96     Added GM_SetSongTickPosition
**  3/1/96      Fixed bug with GM_SetSongTickPosition that would blow the songLoop flag away
**  3/5/96      Eliminated the global songVolume
**  4/15/96     Added support to interpret SONG resource via GM_MergeExternalSong
**  4/21/96     Removed CPU edian issues by use XGetShort & XGetLong
**              Removed register usage in parameters
**  5/18/96     More error correction in GM_GetSongTickLength & GM_SetSongTickPosition
**  5/30/96     Fixed possible bad code in PV_CreateSongFromMidi
**              Added ignore Bad patches code
**  6/7/96      Added some error correction changes to GM_LoadSong
**  7/4/96      Changed font and re tabbed
**  9/17/96     Added GM_LoadSongInstrument & GM_UnloadSongInstrument
**  9/23/96     Added channel muting feature
**  9/25/96     Changed GM_SetSongTickPosition to end just the song notes its working on
**  11/9/96     Fixed a bug with GM_FreeSong that would free instruments before
**              instruments were done playing
**  12/30/96    Changed copyright
**  1/12/97     Added support for more song types
**              Changed maxNormalizedVoices to mixLevel
**  1/28/97     Eliminated terminateDecay flag. Not used anymore
**  1/29/97     Reworked GM_LoadSong to support encrypted midi files
**  2/1/97      Added support for pitch offset control on a per channel basis
**  3/20/97     Added GM_SetSongMicrosecondPosition & GM_GetSongMicrosecondLength
**  4/20/97     Changed PV_MusicIRQ to PV_ProcessMidiSequencerSlice
**  5/3/97      Fixed a few potential problems that the MOT compiler found. Specificly
**              changed a theSong->songMidiTickLength = -1 to theSong->songMidiTickLength = 0
**              to signal calculate length.
**  7/29/97     (ddz) Added call to PV_FreePgmEntries in GM_FreeSong, and GM_GetSongProgramChanges
**  8/7/97      Changed GM_SongTicks & GM_SetSongTickPosition & GM_GetSongTickLength
**              to support UFLOAT
**  8/13/97     Renamed GM_GetSongProgramChanges to GM_GetSongInstrumentChanges and changed
**              Byte reference to XBYTE
**  8/15/97     Fixed a bug in which a data block was being freed in GM_FreeSong after
**              the main pointer was trashed.
**              Added disposal in GM_FreeSong of the new controller callbacks
**  9/5/97      (ddz) Made all pgm change stuff in one structure PatchInfo, changed PV_FreePgmEntries
**              to PV_FreePatchInfo
**  9/19/97     Fixed bug with GM_GetSongTickLength that allowed callbacks to be
**              called and freed during a length calculation
**  9/25/97     Added ddz changes, and wrapped a couple of function around USE_CREATION_API == TRUE
**  10/15/97    Modified GM_UnloadSongInstrument to handle the case in which a instrument
**              is still busy
**              Modified GM_FreeSong to handle the case in which instruments, samples, or
**              midi data is still busy.
**  10/16/97    Changed GM_LoadSong parmeters to include an option to ignore bad instruments
**              when loading.
**              Renamed ignoreBadPatches to ignoreBadInstruments
**  10/18/97    Fixed some compiler warnings and modified GM_FreeSong to kill midi data
**              pointer now rather than let the decoder thread get it later.
**  10/27/97    Removed reference to MusicGlobals->theSongPlaying
**  2/2/98      Added GM_SetVelocityCurveType
**  2/8/98      Changed BOOL_FLAG to XBOOL
** JAVASOFT
**  02.10.98:   $$kk: GM_SetSongMicrosecondPosition: added check for whether
**              song was paused so that we don't resume if this method was called on a paused song.
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
**  6/30/98     Removed INT16 casting in GM_MergeExternalSong
**              Changed GM_LoadSong/GM_CreateLiveSong to accept a long rather than a short for 
**              the songID
**  7/6/98      Added GM_IsSongInstrumentLoaded
**              Fixed type problems with GM_LoadSong & PV_CreateSongFromMidi
** 2/18/99      Changed GM_LoadSong & GM_CreateLiveSong to pass in a context
**              Added GM_GetSongContext & GM_SetSongContext
**  3/5/99      Added threadContext to GM_LoadSong & GM_FreeSong
**  5/12/99     Fixed a problem with RMF files and GM_LoadSong in which it would not
**              decode and use the passed in Midi data.
**  5/16/99     Moved GM_BeginSong & GM_PrerollSong from GenSeq.c
**  6/15/99     Moved GM_EndSong from GenSeq.c. Split the control of GM_EndSong into
**              two functions.
**              Added GM_EndSongButKeepActive
**  6/17/99     Removed access to global MusicGlobals from PV_EndSongWithControl. Changed
**              GM_EndSongButKeepActive to pause the song prior to ending.
**  8/4/99      MOE: Changed PV_CreateSongFromMidi() to return error code
**              so that proper error was returned when no MIDI resource found
**  10/30/99    Removed GM_GetSongInstrumentChanges, which is not used.
**  11/3/99     MSD: Fixed logic error in finding empty slot in GM_PrerollSong
**  11/4/99     Added GM_KillSongEventsFromQueue
**              Fixed bug in GM_FreeSong by calling GM_KillSongEventsFromQueue
**              that clear events in the future when a song is deleted.
**  11/9/99     Added GM_SetSongPriority & GM_GetSongPriority
**  11/10/99    Added ifdefs to handle the case REVERB_USED == REVERB_DISABLED
**  1/31/00     Added GM_SetDisposeSongDataWhenDoneFlag(), GM_GetDisposeSongDataWhenDoneFlag()
**              Added GM_GetSongVoices(), GM_ChangeSongVoices()
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  2/5/2000    Removed unused variable in GM_ChangeSongVoices
**  2/26/2000   Added GM_SetSongMixer & GM_GetSongMixer
**              Added GM_Mixer parameter to GM_LoadSong
**  3/3/2000    Added GM_SetupSongRemaps. Moved GM_RemapInstrument from GenPatch.c
**              Modifed PV_CreateSongFromMidi & GM_CreateLiveSong & GM_LoadSongInstrument
**              to support the new function GM_SetupSongRemaps.
**              Changed GM_IsSongInstrumentLoaded & GM_UnloadSongInstrument & GM_LoadSongInstrument
**              to deal with aliases of instruments when working with passed in instruments.
**  3/7/2000    Added GM_IsSongInstrumentRemapped
**  3/10/2000   Fixed memory leak in GM_FreeSong. Thanks Andrew. In the rare case of an instrument
**              failing to unload, the midi data pointer is lost.
**  3/14/2000   Fixed GM_RemapInstrument to actaully map the correct direction! Fixed
**              GM_LoadSongInstrument to map the correct direction!
**  4/3/2000    Changed GM_GetSongInstrumentReamp to GM_GetSongInstrumentRemap
**  4/19/2000   Changed GM_LoadSongInstrument to always force a recheck of the
**              alias link for the current patch bank.
**  4/25/2000   msd: added GM_GetProgramBank() to support MiniBAE API lego.
**  2000.05.16 AER  Completed modifications for new sample cache
**  2000.05.25 sh   Use GM_SetSongLoopFlag/GM_GetSongLoopFlag instead of touching 
**                  the structure GM_Song directly
**  6/19/2000   jsc: in GM_SetSongTickPosition, if song was paused, then don't resume song  
**  2000.07.14 AER  Modified access to XBankTokens for new system
**  8/27/2000   sh  Changed GM_SetSongTickPosition & GM_SetSongMicrosecondPosition to return 
**                  a error code correctly.
**  12/7/2000   sh  Changed GM_GetSongTickLength & GM_GetSongMicrosecondLength to return
**                  an overflow error.
**  1/25/2001   sh  Added GM_IsSongPrerolled.
**  2/14/2001   se  Found pause/resume bug in GM_SetSongTickPosition/GM_SetSongMicrosecondPosition
**  2/16/2001   se  Modified GM_SetSongMicrosecondPosition to kill current notes rather than 
**                  allow them to release.
**  3/8/2001    se  Modified GM_GetSongTickLength to stop calculation
**                  when the song being operated on has a duration of over one hour.
**  3/29/2001   sh  Removed extra line in GM_GetSongTickLength that copied over the
**                  temporary position of the song which resulted in a bogus position
**                  being returned for the song in question.
**  7/24/2001       Added PV_FindEmptySongSlot, and now only place preloaded song
**                  into mixer when GM_BeginSong is called.
*/
/*****************************************************************************/

#include "X_API.h"
#include "X_Formats.h"
#include "GenSnd.h"
#include "GenPriv.h"
#include "X_Assert.h"

// Functions


static GM_Song * PV_CreateSongFromMidi(XLongResourceID theID,
                                        XPTR useThisMidiData, long midiSize,
                                        OPErr *pErr)
{
    XPTR        theMidiData;
    GM_Song     *theSong;
    OPErr       err;

    err = NO_ERR;
    theSong = NULL;
    if (useThisMidiData)
    {
        theMidiData = useThisMidiData;
    }
    else
    {
        midiSize = 0;
        theMidiData = XGetMidiData(theID, &midiSize, NULL);
        if (theMidiData == NULL)
        {
            err = BAD_FILE;
        }
    }
    if (theMidiData)
    {
        theSong = (GM_Song *)XNewPtr((long)sizeof(GM_Song));
        if (theSong)
        {
            theSong->sequenceData = theMidiData;
            theSong->sequenceDataSize = midiSize;
            theSong->seqType = SEQ_MIDI;
            theSong->disposeSongDataWhenDone = (useThisMidiData == NULL) ? TRUE : FALSE;
            // Fill in remap first
            GM_SetupSongRemaps(theSong, FALSE);
        }
        else
        {
            err = MEMORY_ERR;
        }
    }
    if (pErr)
    {
        *pErr = err;
    }
    return theSong;
}

static void PV_SetTempo(GM_Song *pSong, long masterTempo)
{
    if (pSong)
    {
        if (masterTempo == 0L)
        {
            masterTempo = 16667;
        }
        masterTempo = (100L * masterTempo) / 16667;
        if (masterTempo < 25) masterTempo = 25;
        if (masterTempo > 300) masterTempo = 300;
        GM_SetMasterSongTempo(pSong, (masterTempo << 16L) / 100L);
    }
}

void GM_MergeExternalSong(void *theExternalSong, XShortResourceID theSongID, GM_Song *theSong)
{
    short int           maps;
    short int           count;
    short int           number;
    SongResource_SMS    *songSMS;
    SongResource_RMF    *songRMF;
    Remap               *pMap;

    if (theExternalSong && theSong)
    {
        switch (((SongResource_SMS *)theExternalSong)->songType)
        {
            case SONG_TYPE_SMS:
                songSMS = (SongResource_SMS *)theExternalSong;
                theSong->songID = theSongID;
                theSong->songPitchShift = songSMS->songPitchShift;
                theSong->allowProgramChanges = (songSMS->flags1 & XBF_enableMIDIProgram) ? TRUE : FALSE;
                theSong->defaultPercusionProgram = songSMS->defaultPercusionProgram;
#if REVERB_USED != REVERB_DISABLED
                theSong->defaultReverbType = songSMS->reverbType;
#endif
                theSong->maxSongVoices = songSMS->maxNotes;
                theSong->mixLevel = XGetShort(&songSMS->mixLevel);
                theSong->maxEffectVoices = songSMS->maxEffects;
                theSong->ignoreBadInstruments = (songSMS->flags2 & XBF_ignoreBadPatches) ? TRUE : FALSE;
                maps = XGetShort(&songSMS->remapCount);
                PV_SetTempo(theSong, XGetShort(&songSMS->songTempo));
                theSong->songVolume = XGetSongVolume((SongResource *)theExternalSong);

        // Load instruments
                if (maps)
                {
                    pMap = (Remap *)&songSMS->remaps;
                    for (count = 0; count < maps; count++)
                    {
                        number = XGetShort(&pMap[count].instrumentNumber) & ((MAX_INSTRUMENTS*MAX_BANKS)-1);
                        theSong->remapArray[number] = XGetShort(&pMap[count].ResourceINSTID);
                    }
                }
                break;
        
            case SONG_TYPE_RMF:
                songRMF = (SongResource_RMF *)theExternalSong;
                theSong->songID = theSongID;
                theSong->songPitchShift = songRMF->songPitchShift;
                theSong->allowProgramChanges = TRUE;            // aloways allow program changes
                theSong->defaultPercusionProgram = -1;          // GM percussion only
#if REVERB_USED != REVERB_DISABLED
                theSong->defaultReverbType = songRMF->reverbType;
#endif
                theSong->maxSongVoices = XGetShort(&songRMF->maxNotes);
                theSong->mixLevel = XGetShort(&songRMF->mixLevel);
                theSong->maxEffectVoices = XGetShort(&songRMF->maxEffects);
                theSong->ignoreBadInstruments = TRUE;
                PV_SetTempo(theSong, XGetShort(&songRMF->songTempo));
                theSong->songVolume = XGetSongVolume((SongResource *)theExternalSong);
                break;
        }
    }
}


static void PV_ClearSongInstruments(GM_Song *pSong)
{
    long    count;

    if (pSong)
    {
        for (count = 0; count < (MAX_INSTRUMENTS*MAX_BANKS); count++)
        {
            pSong->instrumentData[count] = NULL;
        }
    }
}

// return valid context for song that was passed in when calling GM_LoadSong or GM_CreateLiveSong
void * GM_GetSongContext(GM_Song *pSong)
{
    void    *context;

    context = NULL;
    if (pSong)
    {
        context = pSong->context;
    }
    return context;
}

// set valid context for song
void GM_SetSongContext(GM_Song *pSong, void *context)
{
    if (pSong)
    {
        pSong->context = context;
    }
}

// return associated Mixer from Song
GM_Mixer * GM_GetSongMixer(GM_Song *pSong)
{
    if (pSong)
    {
        return pSong->pMixer;
    }
    return NULL;
}

// Associate a Mixer to a Song
OPErr GM_SetSongMixer(GM_Song *pSong, GM_Mixer *pMixer)
{
    OPErr   err;

    err = NO_ERR;
    if (pSong && pMixer)
    {
        pSong->pMixer = pMixer;
    }
    else
    {
        err = NOT_SETUP;
    }
    return err;
}

#if 0
// This will walk through all active songs and return a max number of voices used for midi
static short int PV_GetMaxVoicesPlayingFromAllSongs(void)
{
    register GM_Mixer       *pMixer;
    register GM_Song        *pSong;
    register short int      count, max;

    max = 0;
    pMixer = GM_GetCurrentMixer();
    if (pMixer)
    {
        for (count = 0; count < MAX_SONGS; count++)
        {
            pSong = pMixer->pSongsToPlay[count];
            if (pSong)
            {
                max += pSong->maxSongVoices;
            }
        }
    }
    return max;
}
#endif

#if 1
// This will walk through all active songs and return a max mix level used for midi
static short int PV_GetMixLevelPlayingFromAllSongs(void)
{
    register GM_Mixer       *pMixer;
    register GM_Song        *pSong;
    register short int      count, mix;

    mix = 0;
    pMixer = MusicGlobals;
    if (pMixer)
    {
        for (count = 0; count < MAX_SONGS; count++)
        {
            pSong = pMixer->pSongsToPlay[count];
            if (pSong)
            {
                if (pSong->mixLevel > mix)
                {
                    mix = pSong->mixLevel;
                }
            }
        }
    }
    return mix;
}
#endif

// scan through mixer and find an empty song slot.
// Returns -1, if failure.
static short int PV_FindEmptySongSlot(GM_Mixer *pMixer, GM_Song *pSong)
{
    short int   songSlot = -1;
    short int   count;

    if (pSong)
    {
// first find a slot in the song queue
        for (count = 0; count < MAX_SONGS; count++)
        {
            // Reuse slot, if song already playing
            if (pMixer->pSongsToPlay[count] == pSong)
            {
                pMixer->pSongsToPlay[count] = NULL;
                GM_KillSongNotes(pSong);
                songSlot = count;
                break;
            }
        }
        if (songSlot == -1)
        {
            // Song wasn't already playing, so try to find empty slot.
            for (count = 0; count < MAX_SONGS; count++)
            {
                if (pMixer->pSongsToPlay[count] == NULL)
                {
                    songSlot = count;
                    break;
                }
            }
        }
    }
    return songSlot;
}

// preroll song but don't start
OPErr GM_PrerollSong(GM_Song *pSong, GM_SongCallbackProcPtr theCallbackProc, 
                    XBOOL useEmbeddedMixerSettings, XBOOL autoLevel)
{
    OPErr       theErr;
    short int   count;
    INT16       sMaxSong, sMixLevel, sMaxEffect;

    theErr = NO_ERR;
    if (pSong)
    {
        // preroll song prior to start
        pSong->AnalyzeMode = SCAN_NORMAL;
        pSong->songEndCallbackPtr = theCallbackProc;

        if (pSong->seqType == SEQ_MIDI)
        {
            theErr = PV_ConfigureMusic(pSong);
        }
        else
        {
            theErr = BAD_FILE;
        }
        if (theErr == NO_ERR)
        {   
            pSong->SomeTrackIsAlive = TRUE;
            pSong->songFinished = FALSE;

            // first time looping, and set mute tracks to off
            pSong->songLoopCount = 0;
            pSong->songMaxLoopCount = 0;
            for (count = 0; count < MAX_TRACKS; count++)
            {
                XClearBit(&pSong->trackMuted, count);
                XClearBit(&pSong->soloTrackMuted, count);
                pSong->pTrackPositionSave[count] = NULL;
                pSong->trackTicksSave[count] = 0;
                pSong->trackStatusSave[count] = TRACK_OFF;
            }
            pSong->loopbackSaved = FALSE;
            pSong->loopbackCount = -1;
            for (count = 0; count < MAX_CHANNELS; count++)
            {
                XClearBit(&pSong->channelMuted, count);
                XClearBit(&pSong->soloChannelMuted, count);
                XSetBit(&pSong->allowPitchShift, count);
            }
            XClearBit(&pSong->allowPitchShift, PERCUSSION_CHANNEL);     // don't allow pitch changes on percussion

            pSong->velocityCurveType = DEFAULT_VELOCITY_CURVE;

#if REVERB_USED != REVERB_DISABLED
            // reconfigure reverb settings if desired   
            // Set reverb type now.
            GM_SetReverbType(pSong->defaultReverbType); 
#endif
#if 0
            autoLevel = TRUE;
#endif
            // rebalance them mixer based upon load
            if (autoLevel)
            {

                pSong->mixLevel = (pSong->averageVoiceUsage * 90) / 100;    // reduce by 10%
                if (pSong->mixLevel < 1)
                {
                    pSong->mixLevel = 1;
                }
                pSong->maxSongVoices = MAX_VOICES - pSong->maxEffectVoices;

                //BAE_PRINTF("audio::max voices from all songs %d\n", PV_GetMaxVoicesPlayingFromAllSongs());
                //BAE_PRINTF("audio::mix level from all songs %d\n", PV_GetMixLevelPlayingFromAllSongs());

                GM_GetSystemVoices(&sMaxSong, &sMixLevel, &sMaxEffect);
                // if the mix level for all currently playing songs is not 0, then reconfigure mix level only up
                // other go up and down
                count = PV_GetMixLevelPlayingFromAllSongs();
                if (count)
                {
                    // if there's a song playing don't lower the mix level
                    if (pSong->mixLevel < count)
                    {
                        pSong->mixLevel = count;
                    }
                }

                //BAE_PRINTF("audio::Song mix %d midi %d pcm %d\n", pSong->mixLevel, pSong->maxSongVoices, pSong->maxEffectVoices);
                //BAE_PRINTF("audio::Mixer mix %d midi %d pcm %d\n", sMixLevel, sMaxSong, sMaxEffect);
                useEmbeddedMixerSettings = TRUE;    // force a reconfigure
            }
            // reconfigure global mixer settings if desired 
            if (useEmbeddedMixerSettings)
            {
                theErr = GM_ChangeSystemVoices(pSong->maxSongVoices,
                                            pSong->mixLevel,
                                            pSong->maxEffectVoices);
                GM_GetSystemVoices(&sMaxSong, &sMixLevel, &sMaxEffect);
                //BAE_PRINTF("audio::mixer settings: mix %d midi %d pcm %d\n", sMixLevel, sMaxSong, sMaxEffect);
            }
            // allocate song song, but don't play sequencer
            pSong->songPaused = TRUE;
            pSong->songPrerolled = TRUE;
        }
    }
    return theErr;
}

// return preroll status
XBOOL GM_IsSongPrerolled(GM_Song *pSong)
{
    XBOOL   roll;

    roll = FALSE;
    if (pSong)
    {
        roll = pSong->songPrerolled;
    }
    return roll;
}

// Set up the system to start playing a song
OPErr GM_BeginSong(GM_Song *pSong, GM_SongCallbackProcPtr theCallbackProc, 
                    XBOOL useEmbeddedMixerSettings, XBOOL autoLevel)
{
    OPErr       theErr;
    GM_Mixer    *pMixer = GM_GetCurrentMixer();
    short int   songSlot;

    theErr = NO_ERR;
    if (pSong)
    {
        if (GM_IsSongPrerolled(pSong) == FALSE)
        {
            theErr = GM_PrerollSong(pSong, theCallbackProc, useEmbeddedMixerSettings, autoLevel);
        }
        if (theErr == NO_ERR)
        {
            // we clear the preroll flag here, because if GM_BeginSong is called again while the song
            // is playing, we don't want to song in a unsafe state, this forces a reset.

            // also find an empty slot in the mixer for playback.
            songSlot = PV_FindEmptySongSlot(pMixer, pSong);
            if (songSlot != -1)
            {
                pMixer->pSongsToPlay[songSlot] = pSong;
                pSong->songPrerolled = FALSE;
                pSong->songPaused = FALSE;      // start sequencer
            }
            else
            {
                theErr = TOO_MANY_SONGS_PLAYING;
            }
        }
    }
    else
    {
        theErr = PARAM_ERR;
    }
    return theErr;
}

XSWORD GM_GetSongRouteBus(GM_Song *pSong)
{
    XSWORD  routeBus;

    routeBus = 0;
    if (pSong)
    {
        routeBus = pSong->routeBus;
    }
    return routeBus;
}

OPErr GM_SetSongRouteBus(GM_Song *pSong, XSWORD routeBus)
{
    OPErr   theErr = NO_ERR;

    if (pSong)
    {
        pSong->routeBus = routeBus;
    }
    else
    {
        theErr = PARAM_ERR;
    }
    return theErr;
}

// Get song priority level. Higher values will allow for better note stealing.
XSWORD GM_GetSongPriority(GM_Song *pSong)
{
    XSWORD  priority;

    priority = 0;
    if (pSong)
    {
        priority = pSong->songPriority;
    }
    return priority;
}

// Set song priority level. Higher values will allow for better note stealing.
OPErr GM_SetSongPriority(GM_Song *pSong, XSWORD songPriority)
{
    OPErr   theErr = NO_ERR;

    if (pSong)
    {
        pSong->songPriority = songPriority;
    }
    else
    {
        theErr = PARAM_ERR;
    }
    return theErr;
}

// REMAP

OPErr GM_SetupSongRemaps(GM_Song *pSong, XBOOL checkForAliases)
{
    short int           count;
    XAliasLinkResource  *pAlias;
    XLongResourceID     newInstrumentID;
    OPErr               err;

    err = NO_ERR;
    if (pSong)
    {
        pAlias = NULL;
        pSong->checkedForAliases = checkForAliases;
        // Fill in remap first
        if (checkForAliases)
        {
            pAlias = XGetAliasLink();   // get current aliases
        }
        for (count = 0; count < MAX_INSTRUMENTS*MAX_BANKS; count++)
        {
            pSong->remapArray[count] = (XLongResourceID)count;      // no remap
            if (checkForAliases)
            {
                if (XLookupAlias(pAlias, (XLongResourceID)count, &newInstrumentID) == 0)
                {
                    pSong->remapArray[count] = newInstrumentID;
                }
            }
        }
        if (checkForAliases)
        {
            XDisposePtr((XPTR)pAlias);
        }
    }
    else
    {
        err = NOT_SETUP;
    }
    return err;
}

OPErr GM_GetSongInstrumentRemap(GM_Song *pSong, XLongResourceID fromInstrument, XLongResourceID *pToInstrument)
{
    OPErr               err;

    err = NO_ERR;
    if (pSong && pToInstrument)
    {
        *pToInstrument = pSong->remapArray[fromInstrument];
    }
    else
    {
        err = NOT_SETUP;
    }
    return err;
}

// This will remap the 'from' instrument into the 'to' instrument.
OPErr GM_RemapInstrument(GM_Song *pSong, XLongResourceID from, XLongResourceID to)
{
    OPErr           theErr;

    theErr = BAD_INSTRUMENT;
    if (pSong && (from >= 0) && (from < MAX_INSTRUMENTS*MAX_BANKS) )
    {
        if ( (to >= 0) && (to < MAX_INSTRUMENTS*MAX_BANKS) )
        {
            if (to != from)
            {
                if (pSong->instrumentData[from])
                {
                    pSong->remapArray[from] = to;
                    theErr = NO_ERR;
                }
            }
            else
            {
                theErr = NO_ERR;
            }
        }
    }
    return theErr;
}

// GM_CreateLiveSong is used to create an active midi object that can be
// controled directly. ie via midi commands without loading midi data
//
//  context             context of song creation. C++ 'this' pointer, thread, etc.
//                      Its just stored in the GM_Song->context variable
//  songID              unique ID for song structure
GM_Song * GM_CreateLiveSong(void *context, XShortResourceID songID)
{
    GM_Song             *pSong;
    short int           count;

    pSong = NULL;

    pSong = (GM_Song *)XNewPtr((long)sizeof(GM_Song));
    if (pSong)
    {
        pSong->context = context;
        // Fill in remap first
        GM_SetupSongRemaps(pSong, FALSE);

        for (count = 0; count < MAX_CHANNELS; count++)
        {
            pSong->firstChannelBank[count] = 0;
            pSong->firstChannelProgram[count] = -1;
        }
        PV_ConfigureInstruments(pSong);

#if REVERB_USED != REVERB_DISABLED
        pSong->defaultReverbType = GM_GetReverbType();
#endif
        pSong->songID = songID;
        pSong->songPitchShift = 0;
        pSong->allowProgramChanges = TRUE;
        pSong->defaultPercusionProgram = -1;

        pSong->maxSongVoices = MusicGlobals->MaxNotes;
        pSong->mixLevel = MusicGlobals->mixLevel;
        pSong->maxEffectVoices = MusicGlobals->MaxEffects;

        PV_SetTempo(pSong, 0L);
        pSong->songVolume = MAX_SONG_VOLUME;
    }
    return pSong;
}

OPErr GM_StartLiveSong(GM_Song *pSong, XBOOL loadPatches, XBankToken bankToken)
{
    OPErr       theErr;
    short int   songSlot, count;

    theErr = NO_ERR;
    if (pSong)
    {
// first find a slot in the song queue
        songSlot = -1;
        for (count = 0; count < MAX_SONGS; count++)
        {
            if (MusicGlobals->pSongsToPlay[count] == NULL)
            {
                songSlot = count;
                break;
            }
        }
        if (songSlot != -1)
        {
            if (loadPatches)
            {
                for (count = 0; count < (MAX_INSTRUMENTS*MAX_BANKS); count++)
                {
                    GM_LoadSongInstrument(pSong, (XLongResourceID)count, bankToken);
                }
            }

            pSong->SomeTrackIsAlive = FALSE;
            pSong->songFinished = FALSE;
            pSong->AnalyzeMode = SCAN_NORMAL;

            theErr = GM_ChangeSystemVoices(pSong->maxSongVoices,
                                        pSong->mixLevel,
                                        pSong->maxEffectVoices);

#if REVERB_USED != REVERB_DISABLED
            // Set reverb type now.
            GM_SetReverbType(pSong->defaultReverbType);
#endif

            // first time looping, and set mute tracks to off
            pSong->songLoopCount = 0;
            pSong->songMaxLoopCount = 0;
            for (count = 0; count < MAX_TRACKS; count++)
            {
                XClearBit(&pSong->trackMuted, count);
                XSetBit(&pSong->soloTrackMuted, count);
                pSong->pTrackPositionSave[count] = NULL;
                pSong->trackTicksSave[count] = 0;
            }
            pSong->loopbackSaved = FALSE;
            pSong->loopbackCount = -1;
            for (count = 0; count < MAX_CHANNELS; count++)
            {
                XClearBit(&pSong->channelMuted, count);
                XClearBit(&pSong->soloChannelMuted, count);
                XSetBit(&pSong->allowPitchShift, count);
            }
            XClearBit(&pSong->allowPitchShift, PERCUSSION_CHANNEL);     // don't allow pitch changes on percussion

            pSong->velocityCurveType = DEFAULT_VELOCITY_CURVE;

            // Start song playing now.
            MusicGlobals->pSongsToPlay[songSlot] = pSong;
        }
    }
    return theErr;
}

// This follows reamped or aliased instruments.
OPErr GM_LoadSongInstrument(GM_Song *pSong,
                            XLongResourceID instrument,
                            XBankToken bankToken)
{
    XLongResourceID realInstrument;
    OPErr           theErr;

    theErr = BAD_INSTRUMENT;
    if ( pSong && (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)) )
    {
        GM_SetupSongRemaps(pSong, TRUE);    // always reset alias links
        
        if (GM_GetSongInstrumentRemap(pSong, instrument, &realInstrument) != NO_ERR)
        {
            realInstrument = instrument;    // failed, no alias, so try to load instrument passed
        }
        theErr = GM_LoadInstrument(pSong, realInstrument, bankToken);
        GM_RemapInstrument(pSong, instrument, realInstrument);
    }
    return theErr;
}

// This follows reamped or aliased instruments.
OPErr GM_UnloadSongInstrument(GM_Song *pSong, XLongResourceID instrument)
{
    XLongResourceID realInstrument;
    OPErr           theErr;

    theErr = BAD_INSTRUMENT;
    if ( pSong && (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)) )
    {
        if (GM_GetSongInstrumentRemap(pSong, instrument, &realInstrument) != NO_ERR)
        {
            realInstrument = instrument;    // failed, no alias, so try to load instrument passed
        }
        theErr = GM_UnloadInstrument(pSong, realInstrument);
    }
    return theErr;
}

// Will check to see if an instrument has got a remap.
// Returns TRUE if instrument is remapped, otherwise FALSE
XBOOL GM_IsSongInstrumentRemapped(GM_Song *pSong, XLongResourceID instrument)
{
    XLongResourceID realInstrument;
    XBOOL           aliased;

    aliased = FALSE;
    if (pSong && (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)))
    {
        if (GM_GetSongInstrumentRemap(pSong, instrument, &realInstrument) != NO_ERR)
        {
            realInstrument = instrument;    // failed, no alias, so try to load instrument passed
        }
        if (realInstrument != instrument)
        {
            aliased = TRUE;
        }
    }
    return aliased;
}

// Returns TRUE if instrument is loaded, otherwise FALSE
// This follows reamped or aliased instruments.
XBOOL GM_IsSongInstrumentLoaded(GM_Song *pSong, XLongResourceID instrument)
{
    XLongResourceID realInstrument;
    XBOOL           loaded;

    loaded = FALSE;
    if (pSong && (instrument >= 0) && (instrument < (MAX_INSTRUMENTS*MAX_BANKS)))
    {
        if (GM_GetSongInstrumentRemap(pSong, instrument, &realInstrument) != NO_ERR)
        {
            realInstrument = instrument;    // failed, no alias, so try to load instrument passed
        }
        if (pSong->instrumentData[realInstrument])
        {
            loaded = TRUE;
        }
    }
    return loaded;
}


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
                      void *theExternalMidiData,
                      long midiSize,
                      XShortResourceID *pInstrumentArray,
                      XBOOL loadInstruments,
                      XBOOL ignoreBadInstruments,
                      XBankToken bankToken,
                      OPErr *pErr)
{
    GM_Song             *pSong;
    XLongResourceID     songObjectID;
    OPErr               err;

    err = NO_ERR;
    pSong = NULL;
    if (theExternalSong)
    {
        songObjectID = (XLongResourceID)XGetSongResourceObjectID(theExternalSong);
        switch (XGetSongResourceObjectType(theExternalSong))
        {
            case SONG_TYPE_SMS:
                pSong = PV_CreateSongFromMidi(songObjectID, theExternalMidiData, midiSize, &err);
                break;
            case SONG_TYPE_RMF:
                if (theExternalMidiData == NULL)
                {
                    pSong = PV_CreateSongFromMidi(songObjectID, NULL, 0, &err);
                }
                else
                {
                    pSong = PV_CreateSongFromMidi(songObjectID, theExternalMidiData, midiSize, &err);
                }
                break;
            default:
                err = PARAM_ERR;
                break;
        }
    }
    else
    {
        err = PARAM_ERR;
    }

// load instruments
    if (pSong)
    {
        pSong->context = context;
        GM_SetSongMixer(pSong, pMixer);

        GM_MergeExternalSong(theExternalSong, songID, pSong);
        pSong->ignoreBadInstruments = ignoreBadInstruments;
        err = GM_LoadSongInstruments(pSong,
                                     pInstrumentArray,
                                     bankToken,
                                     loadInstruments);
        if (err)
        {
            GM_FreeSong(threadContext, pSong);  // we ignore the error codes, because it should be ok to dispose
                                                // since this song was never engaged
            pSong = NULL;
        }
        else
        {
            // song length not calculated
            pSong->songMidiTickLength = (UFLOAT)0;
            pSong->songMicrosecondLength = (UFLOAT)0;
            err = NO_ERR;
        }
    }
    if (pErr)
    {
        *pErr = err;
    }
    return pSong;
}


// Stop this song playing, or pass NULL with pSong to stop all songs. "removeFromMixer" determines
// if the song is removed from the mixer or not. If TRUE then you can still send midi events to the song
// because the song is still in the mixer for event processing. FALSE removes the song from the mixer.
static void PV_EndSongWithControl(void *threadContext, GM_Song *pSong, XBOOL removeFromMixer)
{
    LOOPCOUNT   count;
    GM_Mixer    *pMixer;

    pMixer = MusicGlobals;
    if (pMixer)
    {
        if (pSong)
        {
            GM_EndSongNotes(pSong);     // end just notes associated with this song
            if (removeFromMixer)
            {
                for (count = 0; count < MAX_SONGS; count++)
                {
                    if (pMixer->pSongsToPlay[count] == pSong)
                    {
                        pMixer->pSongsToPlay[count] = NULL;
                        break;
                    }
                }
            }
            for (count = 0; count < MAX_TRACKS; count++)
            {
                pSong->ptrack[count] = NULL;
                pSong->trackon[count] = TRACK_OFF;
            }
            PV_CallSongCallback(threadContext, pSong, TRUE);
        }
        else
        {
            for (count = 0; count < MAX_SONGS; count++)
            {
                if (pMixer->pSongsToPlay[count])
                {
                    PV_EndSongWithControl(threadContext, pMixer->pSongsToPlay[count], removeFromMixer);
                }
            }
        }
    }
}

// Stop this song playing, or if NULL stop all songs playing.
// This removes the Song from the mixer, so you can no longer send
// midi events to the song for processing.
void GM_EndSong(void *threadContext, GM_Song *pSong)
{
    PV_EndSongWithControl(threadContext, pSong, TRUE);
}

// Stop this song playing, or if NULL stop all songs playing.
// This does NOT remove the Song from the mixer, so you can send
// midi events to the song for processing.
void GM_EndSongButKeepActive(void *threadContext, GM_Song *pSong)
{
    GM_PauseSong(pSong, TRUE);
    PV_EndSongWithControl(threadContext, pSong, FALSE);
}

// Walk through the current queue and invalidate all events that are associated to a specific
// GM_Song.
void GM_KillSongEventsFromQueue(GM_Song *pSong)
{
    short int   count;
    Q_MIDIEvent *pEvent;

    for (count = 0; count < MAX_QUEUE_EVENTS; count++)
    {
        pEvent = &MusicGlobals->theExternalMidiQueue[count];
        if (pEvent)
        {
            if (pEvent->pSong == pSong)
            {
                pEvent->pSong = NULL;
            }
        }
    }
}

// Given a song pointer, this will attempt to free all memory related to the song: midi
// data, instruments, samples, etc. It can fail and will return STILL_PLAYING if
// midi data is still being accessed, or samples, or instruments.
//
// If you pass NULL, then this function will be called recursively will all songs
// currently playing.
OPErr GM_FreeSong(void *threadContext, GM_Song *pSong)
{
    OPErr   err;
    XPTR    midiData;

    err = NO_ERR;
    GM_EndSong(threadContext, pSong);
    if (pSong)
    {
        GM_KillSongNotes(pSong);        // we must kill the notes because we are about to free
                                        // instrument memory
        if (pSong->processingSlice == FALSE)
        {
            GM_PauseSong(pSong, TRUE);
            // remove any events associated with this song
            GM_KillSongEventsFromQueue(pSong);

            midiData = (XPTR)pSong->sequenceData;       // save midi pointer now
            pSong->sequenceData = NULL;                 // and disable midi decoder now, just
            pSong->sequenceDataSize = 0;
                                                        // in case the decoder thread comes to life
            // GM_SetCacheSamples(pSong, FALSE);
            err = GM_UnloadSongInstruments(pSong);
            if (err == NO_ERR)
            {
                if (pSong->disposeSongDataWhenDone)
                {
                    XDisposePtr(midiData);
                }
                XDisposePtr((XPTR)pSong->controllerCallback);

#if 0 && USE_CREATION_API == TRUE
                if (pSong->pPatchInfo)
                {
                    PV_FreePatchInfo(pSong);    // must free before killing pSong pointer
                }
#endif
                XDisposePtr((XPTR)pSong);
            }
            else
            {
                //DebugStr("\pGM_FreeSong::GM_UnloadSongInstruments::STILL_PLAYING");

                // we've failed to unload all the instruments. So we need to restore
                // our midi pointer, so this function can be called again without a leak.
                pSong->sequenceData = midiData;
            }
        }
        else
        {
            //DebugStr("\pGM_FreeSong::STILL_PLAYING");
            err = STILL_PLAYING;
        }
    }
    return err;
}

// Return the length in MIDI ticks of the song passed

//  pSong   GM_Song structure. Data will be cloned for this function.
//  pErr    OPErr error type
UINT32 GM_GetSongTickLength(GM_Song *pSong, OPErr *pErr)
{
    GM_Song     *theSong;
    UINT32      tickLength;
    OPErr       err;

    err = NO_ERR;
    tickLength = 0;
    if (pSong->seqType != SEQ_MIDI)
    {
        if (pErr)
        {
            *pErr = NOT_SETUP;
        }
        return 0;
    }
    if (pSong->songMidiTickLength == (UFLOAT)0)
    {
        theSong = (GM_Song *)XNewPtr(sizeof(GM_Song));
        if (theSong)
        {
            *theSong = *pSong;
            theSong->controllerCallback = NULL;     // ignore callbacks
            theSong->songEndCallbackPtr = NULL;
            theSong->songTimeCallbackPtr = NULL;
            theSong->metaEventCallbackPtr = NULL;
            theSong->disposeSongDataWhenDone = FALSE;
            PV_ClearSongInstruments(theSong);       // don't free the instruments

            err = PV_ConfigureMusic(theSong);
            if (err == NO_ERR)
            {
                theSong->AnalyzeMode = SCAN_DETERMINE_LENGTH;
                theSong->SomeTrackIsAlive = TRUE;
    
                GM_SetSongLoopFlag(theSong, FALSE);
                GM_SetSongMetaLoopFlag(theSong, FALSE);
                theSong->songLoopCount = 0;
                theSong->songMaxLoopCount = 0;
                while (theSong->SomeTrackIsAlive)
                {
                    // don't need a thread context here because we don't callback
                    err = PV_ProcessMidiSequencerSlice(NULL, theSong);
                    if (err)
                    {
                        break;
                    }

                    // if theSong is longer than an hour, bail
                    if (theSong->songMicroseconds >= (UFLOAT)3600000000u)
                    {
                        err = OUT_OF_RANGE;
                        break;
                    }
                }
                theSong->AnalyzeMode = SCAN_NORMAL;
                pSong->songMidiTickLength = theSong->CurrentMidiClock;
                pSong->songMicrosecondLength = theSong->songMicroseconds;
                theSong->sequenceData = NULL;
                theSong->songEndCallbackPtr = NULL;
                theSong->disposeSongDataWhenDone = FALSE;

                tickLength = (UINT32)theSong->CurrentMidiClock;
                if (tickLength == 0xFFFFFFFFL)
                {
                    err = OUT_OF_RANGE;
                }

                if (err)
                {
                    tickLength = 0;
                }
            }
            // don't need a thread context here because we don't callback
            GM_FreeSong(NULL, theSong); // we ignore the error codes, because it should be ok to dispose
                                        // since this song was never engaged
        }
        else
        {
            err = MEMORY_ERR;
        }
    }
    else
    {
        tickLength = (UINT32)pSong->songMidiTickLength;
        if (tickLength == 0xFFFFFFFFL)
        {
            tickLength = 0;
            err = OUT_OF_RANGE;
        }
    }
    if (pErr)
    {
        *pErr = err;
    }
    return tickLength;
}

// meta event callback that collects track names
#if USE_CREATION_API == TRUE
static void PV_TrackNameCallback(void *threadContext, GM_Song *pSong, char markerType, void *pMetaText, long metaTextLength, short currentTrack)
{
    XBYTE **tnArray,*str;

    threadContext;
    if (markerType == 0x03) 
    {   // track name
        if (currentTrack != -1)
        {
            str = (XBYTE *)XNewPtr(metaTextLength+1);
            if (str)
            {
                XBlockMove(pMetaText,str+1,metaTextLength);
                str[0] = (XBYTE)metaTextLength;
                tnArray = (XBYTE **)pSong->metaEventCallbackReference;
                tnArray[currentTrack] = str;
            }
        }
    }
}
#endif

#if 0
// unsued
// Go through the song and find all the program changes on each track
// theSongResource -> Standard Song Resource
//  pErr        OPErr error type
//  outSong      modified, loaded song
//  trackNames   array of track names extracted
#if USE_CREATION_API == TRUE
OPErr GM_GetSongInstrumentChanges(void *theSongResource, GM_Song **outSong, XBYTE **outTrackNames)
{
    OPErr       err;
    ScanMode    saveScan;
    XBOOL       saveLoop,saveAlive;
    GM_Song     *theSong;
    void        *newMidiData;
    
    err = NO_ERR;
    // don't need a thread context here because we don't callback
    theSong = GM_LoadSong(NULL, NULL, NULL, 0, theSongResource, NULL, 0L, NULL, FALSE, TRUE, &err);
    if (!theSong)
    {
        return err;
    }   
    err = PV_ConfigureMusic(theSong);
    // first pass: count the program changes, so we know how big to make our
    if (err == NO_ERR)
    {
        theSong->pPatchInfo = (PatchInfo *)XNewPtr(sizeof(PatchInfo));
        if (theSong->pPatchInfo)
        {
            saveScan = theSong->AnalyzeMode;
            theSong->AnalyzeMode = SCAN_COUNT_PATCHES;
            saveLoop = GM_GetSongLoopFlag(theSong);
            GM_SetSongLoopFlag(theSong, FALSE);
            GM_SetSongMetaLoopFlag(theSong, FALSE);
            saveAlive = theSong->SomeTrackIsAlive;
            theSong->SomeTrackIsAlive = TRUE;
            while (theSong->SomeTrackIsAlive)
            {
            // don't need a thread context here because we don't callback
                err = PV_ProcessMidiSequencerSlice(NULL, theSong);
                if (err)
                {
                    break;
                }
            }
        }
        else
        {
            err = MEMORY_ERR;
        }
    }
    // now theSong->pPatchInfo->pgmCount points to the number of program
    // changes without immediately preceding bank changes, and rsCount
    // is the number of program changes that were discovered to have been
    // written into the file in running status mode (implement this later)
    // done in two passes because we can't keep changing the midi data
    // pointer, so we have figure out how big it needs to be first and
    // move it in place

    if (err == NO_ERR)
    {
        // expand the ptr enough to accomodate bank messages (4 bytes each)
        newMidiData = XNewPtr(XGetPtrSize(theSong->midiData) + theSong->pPatchInfo->instrCount * 4);
        XBlockMove(theSong->midiData,newMidiData,XGetPtrSize(theSong->midiData));
        XDisposePtr(theSong->midiData);
        theSong->midiData = newMidiData;

        // start over
        err = PV_ConfigureMusic(theSong);

        // second pass: get the program changes, add bank events before each one
        if (err == NO_ERR)
        {
            GM_SetSongMetaEventCallback(theSong, PV_TrackNameCallback, (long)outTrackNames);
            saveScan = theSong->AnalyzeMode;
            theSong->AnalyzeMode = SCAN_FIND_PATCHES;
            saveLoop = GM_GetSongLoopFlag(theSong);
            GM_SetSongLoopFlag(theSong, FALSE);
            GM_SetSongMetaLoopFlag(theSong, FALSE);
            saveAlive = theSong->SomeTrackIsAlive;
            theSong->SomeTrackIsAlive = TRUE;
            while (theSong->SomeTrackIsAlive)
            {
                // don't need a thread context here because we don't callback
                err = PV_ProcessMidiSequencerSlice(NULL, theSong);
                if (err)
                {
                    break;
                }
            }
        }
    }
    // since there was an error, caller won't be using the song
    if (err)
    {
        if (theSong->pPatchInfo)
        {
            XDisposePtr(theSong->pPatchInfo);
        }
        // don't need a thread context here because we don't callback
        GM_FreeSong(NULL, theSong); // we ignore the error codes, because it should be ok to dispose
                                    // since this song was never engaged
    }
    else
    {
        *outSong = theSong;
    }
    return err;
}
#endif  // USE_CREATION_API == TRUE
#endif

// Set the song position in midi ticks
OPErr GM_SetSongTickPosition(GM_Song *pSong, UINT32 songTickPosition)
{
    GM_Song     *theSong;
    OPErr       theErr;
    XBOOL       foundPosition;
    long        count;
    XBOOL       songPaused = FALSE;

    if (pSong->seqType != SEQ_MIDI)
    {
        return NOT_SETUP;
    }
    theErr = NO_ERR;
    theSong = (GM_Song *)XNewPtr(sizeof(GM_Song));
    if (theSong)
    {
        *theSong = *pSong;
        PV_ClearSongInstruments(theSong);       // don't free the instruments

        theErr = PV_ConfigureMusic(theSong);
        if (theErr == NO_ERR)
        {
            theSong->AnalyzeMode = SCAN_DETERMINE_LENGTH;
            theSong->SomeTrackIsAlive = TRUE;

            GM_SetSongLoopFlag(theSong, FALSE);
            GM_SetSongMetaLoopFlag(theSong, FALSE);
            foundPosition = FALSE;
            if (pSong->songPaused)
            {
                songPaused = TRUE;
            }
            GM_PauseSong(pSong, TRUE);
            while (theSong->SomeTrackIsAlive)
            {
                // don't need a thread context here because we don't callback
                theErr = PV_ProcessMidiSequencerSlice(NULL, theSong);
                if (theErr)
                {
                    break;
                }
                if (theSong->CurrentMidiClock > (UFLOAT)songTickPosition)
                {
                    foundPosition = TRUE;
                    break;
                }
            }
            theSong->AnalyzeMode = SCAN_NORMAL;
            GM_SetSongLoopFlag(theSong, GM_GetSongLoopFlag(pSong));
            GM_SetSongMetaLoopFlag(theSong, GM_GetSongMetaLoopFlag(pSong));
            if (foundPosition)
            {
                for (count = 0; count < (MAX_INSTRUMENTS*MAX_BANKS); count++)
                {
                    theSong->instrumentData[count] = pSong->instrumentData[count];
                }

                GM_EndSongNotes(pSong);
                *pSong = *theSong;      // copy over all song information at the new position
                PV_ClearSongInstruments(theSong);       // don't free the instruments
            }

            if (!songPaused)
            {
                GM_ResumeSong(pSong);
            }
            // free duplicate song
            theSong->sequenceData = NULL;
            theSong->disposeSongDataWhenDone = FALSE;
            theSong->songEndCallbackPtr = NULL;
            theSong->songTimeCallbackPtr = NULL;
            theSong->metaEventCallbackPtr = NULL;
            theSong->controllerCallback = NULL;
        }
        // don't need a thread context here because we don't callback
        GM_FreeSong(NULL, theSong); // we ignore the error codes, because it should be ok to dispose
                                    // since this song was never engaged
    }
    return theErr;
}

UINT32 GM_SongTicks(GM_Song *pSong)
{
    if (pSong)
    {
        if (GM_IsSongDone(pSong) == FALSE)
        {
            return (UINT32)pSong->CurrentMidiClock;
        }
    }
    return 0L;
}

UINT32 GM_SongMicroseconds(GM_Song *pSong)
{
    if (pSong)
    {
        if (GM_IsSongDone(pSong) == FALSE)
        {
            // XXX - callers should check for overflow
            return (UINT32)pSong->songMicroseconds;
        }
    }
    return 0L;
}

UINT32 GM_GetSongMicrosecondLength(GM_Song *pSong, OPErr *pErr)
{
    UINT32  ms;

    ms = 0;
    if (pErr && pSong)
    {
        GM_GetSongTickLength(pSong, pErr);
        if (*pErr == NO_ERR)
        {
            ms = (UINT32)pSong->songMicrosecondLength;
            if (ms == 0xFFFFFFFFL)
            {
                // we've overflowed,
                ms = 0;
                *pErr = OUT_OF_RANGE;
            }
        }
    }
    return ms;
}

// Set the song position in microseconds
// $$kk: 08.12.98 merge: changed this method
OPErr GM_SetSongMicrosecondPosition(GM_Song *pSong, UINT32 songMicrosecondPosition)
{
    GM_Song     *theSong;
    OPErr       theErr;
    XBOOL       foundPosition;
    long        count;
    XBOOL       songPaused = FALSE;

    // $$kk: 02.10.98
    // the way this was, it paused the song, changed the position, and resumed.
    // if this is applied to a paused song, it suddenly starts playing again!
    // i am adding a mechanism to record whether the song was paused and only
    // resume it in that case.

    if (pSong->seqType != SEQ_MIDI)
    {
        return NOT_SETUP;
    }
    theErr = NO_ERR;
    theSong = (GM_Song *)XNewPtr(sizeof(GM_Song));
    if (theSong)
    {
        *theSong = *pSong;
        PV_ClearSongInstruments(theSong);       // don't free the instruments

        theErr = PV_ConfigureMusic(theSong);
        if (theErr == NO_ERR)
        {
            theSong->AnalyzeMode = SCAN_DETERMINE_LENGTH;
            theSong->SomeTrackIsAlive = TRUE;

            GM_SetSongLoopFlag(theSong, FALSE);
            GM_SetSongMetaLoopFlag(theSong, FALSE);
            foundPosition = FALSE;

            // $$kk: 02.10.98: keep track of whether song is paused
            if (pSong->songPaused)
            {
                songPaused = TRUE;
            }
            GM_PauseSong(pSong, TRUE);
            while (theSong->SomeTrackIsAlive)
            {
                // don't need a thread context here because we don't callback
                theErr = PV_ProcessMidiSequencerSlice(NULL, theSong);
                if (theErr)
                {
                    break;
                }
                if (theSong->songMicroseconds > (UFLOAT)songMicrosecondPosition)
                {
                    foundPosition = TRUE;
                    break;
                }
            }
            theSong->AnalyzeMode = SCAN_NORMAL;
            GM_SetSongLoopFlag(theSong, GM_GetSongLoopFlag(pSong));
            GM_SetSongMetaLoopFlag(theSong, GM_GetSongMetaLoopFlag(pSong));
            if (foundPosition)
            {
                for (count = 0; count < (MAX_INSTRUMENTS*MAX_BANKS); count++)
                {
                    theSong->instrumentData[count] = pSong->instrumentData[count];
                }

                GM_KillSongNotes(pSong);
                *pSong = *theSong;      // copy over all song information at the new position
                PV_ClearSongInstruments(theSong);       // don't free the instruments
            }

            // $$kk: 02.10.98: do not resume if song was paused before
            if (!songPaused)
            {
                GM_ResumeSong(pSong);
            }
            // free duplicate song
            theSong->sequenceData = NULL;
            theSong->songEndCallbackPtr = NULL;
            theSong->disposeSongDataWhenDone = FALSE;
        }
        // don't need a thread context here because we don't callback
        GM_FreeSong(NULL, theSong); // we ignore the error codes, because it should be ok to dispose
                                // since this song was never engaged
    }
    return theErr;
}

// Return the used patch array of instruments used in the song passed.

//  theExternalSong     standard SONG resource structure
//  theExternalMidiData if not NULL, then will use this midi data rather than what is found in external SONG resource
//  midiSize            size of midi data if theExternalMidiData is not NULL
//  pInstrumentArray    array, if not NULL will be filled with the instruments that need to be loaded.
//  pErr                pointer to an OPErr
#if USE_CREATION_API == TRUE
INT32 GM_GetUsedPatchlist(void *theExternalSong,
                          void *theExternalMidiData,
                          long midiSize,
                          XShortResourceID *pInstrumentArray,
                          OPErr *pErr)
{
    GM_Song             *theSong;
    long                count;
    XBankToken          bankToken;

    *pErr = NO_ERR;

    bankToken = CreateBankToken();
    // don't need a thread context here because we don't callback
    theSong = GM_LoadSong(NULL,
                          NULL,
                          NULL,
                          0,
                          theExternalSong,
                          theExternalMidiData,
                          midiSize,
                          pInstrumentArray,
                          FALSE,
                          TRUE,
                          bankToken,
                          pErr);
    if (theSong)
    {
        GM_FreeSong(NULL, theSong); // we ignore the error codes, because it should be ok to dispose
                                    // since this song was never engaged
    }

    count = 0;
    if (*pErr == NO_ERR)
    {
        for (; count < MAX_INSTRUMENTS*MAX_BANKS; count++)
        {
            if (pInstrumentArray[count] == (XShortResourceID)-1)
            {
                break;
            }
        }
    }
    return count;
}
#endif  // USE_CREATION_API

void GM_SetVelocityCurveType(GM_Song *pSong, VelocityCurveType velocityCurveType)
{
    if (pSong)
    {
        pSong->velocityCurveType = velocityCurveType;
    }
}


OPErr GM_SetDisposeSongDataWhenDoneFlag(GM_Song *pSong, XBOOL disposeData)
{
    OPErr   theErr;

    theErr = NO_ERR;
    if (pSong)
    {
        pSong->disposeSongDataWhenDone = disposeData;
    }
    else
    {
        theErr = NOT_SETUP;
    }
    return theErr;
}


OPErr GM_GetDisposeSongDataWhenDoneFlag(GM_Song *pSong, XBOOL *outDisposeData)
{
    OPErr   theErr;

    theErr = NO_ERR;
    if (pSong)
    {
        if (outDisposeData)
        {
            *outDisposeData = pSong->disposeSongDataWhenDone;
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


OPErr GM_GetSongVoices(GM_Song *pSong, INT16 *pMaxSongVoices, INT16 *pMixLevel, INT16 *pMaxEffectVoices)
{
    OPErr   theErr;

    theErr = NO_ERR;
    if (pSong)
    {
        if (pMaxSongVoices && pMixLevel && pMaxEffectVoices)
        {
            *pMaxSongVoices = MusicGlobals->MaxNotes;
            *pMixLevel = MusicGlobals->mixLevel;
            *pMaxEffectVoices = MusicGlobals->MaxEffects;
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


OPErr GM_ChangeSongVoices(GM_Song *pSong, INT16 maxSongVoices, INT16 mixLevel, INT16 maxEffectVoices)
{
    OPErr   theErr;

    theErr = NO_ERR;
    if (pSong)
    {
        if ((maxSongVoices >= 0) &&
            (mixLevel > 0) &&
            (maxEffectVoices >= 0) &&
            ((maxEffectVoices+maxSongVoices) > 0) &&
            ((maxEffectVoices+maxSongVoices) <= MAX_VOICES) )
        {
            pSong->maxSongVoices = maxSongVoices;
            pSong->mixLevel = mixLevel;
            pSong->maxEffectVoices = maxEffectVoices;
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


OPErr GM_GetProgramBank(GM_Song *pSong, XSWORD channel, XSWORD *outProgram, XSWORD *outBank)
{
    OPErr err;

    err = NO_ERR;
    if (pSong)
    {
        if (outProgram && outBank)
        {
            *outProgram = pSong->channelProgram[channel];
            *outBank = pSong->channelBank[channel];
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


// EOF of GenSong.c
