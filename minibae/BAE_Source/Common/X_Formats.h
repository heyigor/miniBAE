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
**  X_Formats.h
**
**      This is platform independent file and data formats for SoundMusicSys
**
**  © Copyright 1989-2000 Beatnik, Inc, All Rights Reserved.
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
**  History -
**  6/30/96     Created
**  7/3/96      Added packing pragmas
**  7/14/96     Removed PRAGMA_ALIGN_SUPPORTED
**  8/19/96     Added compressionType to SampleDataInfo
**  10/23/96    Changed GetKeySplitFromPtr to XGetKeySplitFromPtr
**  12/5/96     Added locked flags for songs and instruments
**  12/10/96    Added ID_RMF type
**  12/19/96    Added Sparc pragmas
**  12/30/96    Changed copyrights
**  1/2/97      Added ID_MOD type
**  1/3/97      Added XGetSongInformation
**  1/6/97      Added songType to XNewSongPtr
**  1/7/97      Changed structures typedef forms
**  1/12/97     Broke SongResource into two types: SongResource_SMS and SongResource_RMF
**  1/13/97     Added XGetSongResourceInfo & XDisposeSongResourceInfo & 
**              XGetSongResourceObjectID & XGetSongPerformanceSettings &
**              XGetSongResourceObjectType
**  1/18/97     Added XCheckAllInstruments & XCheckValidInstrument
**  1/24/97     Added SongResource_MOD
**  1/29/97     Added XGetSongInstrumentList
**              Added XGetMidiData
**              Added ID_ESND and ID_EMID types
**  1/30/97     Added XGetSoundResourceByName & XGetSoundResourceByID
**              Added XGetSongVoices & XSetSongVoices
**  2/14/97     Added volumeGain in SongResource_RMF
**              Added XSetSongVolumeGain & XGetSongVolumeGain
**  2/15/97     Changed volumeGain to songVolume, changed XSetSongVolumeGain & 
**              XGetSongVolumeGain to XSetSongVolume & XGetSongVolume
**  3/13/97     Added embeddedSong to SongResource_RMF structure
**              Added XGetSongEmbeddedStatus & XSetSongEmbeddedStatus
**  3/14/97     Added XIsSoundUsedInInstrument & XRenumberSampleInsideInstrument
**  3/16/97     Changed KeySplit to support a replacement for sample root key
**              on a per split basis
**  3/27/97     Changed all 4 character constants to use the FOUR_CHAR macro
**  5/13/97     Added ID_VERS & XVersion
**  6/20/97     Added XGetSampleNameFromID
**  7/24/97     Added the structure XExtSndHeader2
**              Added XCreateSoundObjectFromData & XGetSoundEmbeddedStatus & 
**              XSetSoundEmbeddedStatus
**  7/28/97     Changed a pragma for Sparc compilers (jdr)
**  8/7/97      Added XTranslateFromWaveformToSampleData
**  8/25/97     Fixed some Sun compiler warnings
**  9/30/97     Added BankStatus structure, added XCreateBankStatus & XGetBankStatus
**  10/9/97     Added ID_TEXT
**  1/22/98     Modifed XGetMidiData to return type of compression, if any
**              Added XIsSongCompressed
**  3/23/98     MOE: Added definitions of:
**                  AIFF_IMA_HEADER_BYTES
**                  AIFF_IMA_BLOCK_FRAMES
**                  AIFF_IMA_BLOCK_BYTES
**                  WAV_IMA_HEADER_BYTES
**              Added XConvertFromIeeeExtended & XConvertToIeeeExtended. Came
**              from GenSoundFiles.c. Added SndCompressionType
**              Added compression parameter to XCreateSoundObjectFromData
**  4/27/98     Changed XCompressAndEncrypt parameters to be unsigned long
**  4/30/98     Added I_GENRE & I_SUB_GENRE & R_GENRE & R_SUB_GENRE
**              Modified the structure SongResource_Info to handle the new genre and sub_genre fields
**  5/4/98      Removed ZBF_neverInterpolate to ZBF_reserved_0. Moved XBF_disableClickRemoval to reserved
**  5/11/98     Added XGetAllSoundID
**  5/21/98     Added X_IMA3
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
**  7/1/98      Changed various API to use the new XResourceType and XLongResourceID and XShortResourceID
**  7/6/98      Fixed type problems with XGetMidiData
**  7/7/98      Modified XGetSampleNameFromID to use new XLongResourceID type
**  7/15/98     Added XGetMusicObjectFromSong
**
**  JAVASOFT
**  ??          $$kk: added different pragma packs for intel hardware depending on os (solaris or win32)
**  9/21/98     Added MPEG decoder/encoder API's
**  9/26/98     Added C_MPEG enum type SndCompressionType
**  9/29/98     Renamed firstSoundFormat & secondSoundFormat to XFirstSoundFormat & XSecondSoundFormat
**              Added support for new 'snd ' XThirdSoundFormat. Added structure XSoundHeader3 & XSndHeader3
**              Renamed stdSH, extSH, cmpSH to XStandardHeader, XExtendedHeader, XCompressedHeader
**              Changed parameter type of XSetSoundSampleRate
**  9/30/98     Added XTranslateFromSampleDataToWaveform
**  10/2/98     Added XCompressLaw
**  10/10/98    Added to XCreateSoundObjectFromData the ability to call a function during compression
**  10/20/98    Removed SongResource_MOD structure and support. Added SongResource_RMF_Linear.
**              Removed SongType SONG_TYPE_MOD, and added SONG_TYPE_RMF_LINEAR
**  12/17/98    Moved some editor related functions into X_EditorTools.h
**  1/27/99     Added PasswordAccess structure & ID_PASSWORD
**  1/29/99     Added XGetVersionNumber & XCreateVersion
**  2/12/99     Renamed USE_HAE_FOR_MPEG to USE_MPEG_DECODER
**  3/16/99     MOE:  Moved XCompressStatusProc to X_API.h
**  3/17/99     Added C_LZSS into SndCompressionType
**  3/22/99     MOE:  Changed the names of the parameters of XCompressMPEG()
**              to more closely reflect what they mean.
**  3/24/99     Added R_TEMPO & R_ORIGINAL_SOURCE rmf sub resource types
**              Added I_TEMPO & I_ORIGINAL_SOURCE rmf info types
**  3/25/99     MOE:  Added procData parameter to functions using XCompressStatusProc
**  5/15/99     Added XGetRawSoundResourceByID
**  7/13/99     MOE: gave the SndCompressionType enum its own name
**  8/3/99      Added XSwap16BitSamples
**  9/9/99      MOE: Added C_IMA4_WAV
**  9/9/99      MOE: Changed the type of XExtSoundHeader.sampleIsIntelOrder
**              from char to XBYTE
**  9/9/99      MOE: Added XMPEGDecodedData.bitrate
**  9/9/99      MOE: Changed XSwap16BitSamples() calls to XSwapShorts(), which I added for 1.52
**  9/9/99      MOE: eliminated XOpenMPEGStreamFromXFILENAME() since it wasn't
**              being called and it didn't clean up its XFILE at all
**  9/13/99     MOE: added text buffer length parameter to XGetSongInformation(),
**              fixing crash bug that showed up with very long info strings
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  3/1/2000    Added ID_ALIAS, and XAliasLinkResource & XAliasLink structures
**              Added XGetAliasLink & XGetAliasLinkFromFile & XLookupAlias
**  2000.04.14 AER  Promoted XConvert8BitTo16Bit to a non-private function
**  3/22/2000   Commented XGetSamplePtrFromSnd. Added XGetSampleInfoFromSnd
**  5/12/2000   Added XIsValidMPEGSampleRateAndEncodeRate
**  5/15/2000   Added new RMF sub types _MANUFACTURER, _MISC1, _MISC2, _MISC3, _MISC4,
**              _MISC5, _MISC6, _MISC7, _MISC8. Added new elements in SongResource_Info
**              structure
**  5/19/2000   Added XNewSongResourceInfo/XClearSongResourceInfo
**  7/25/2000   sh  Added SndCompressionSubType to handle the case of MPEG 1.0 vs. 2.0
**                  Modifed XCreateSoundObjectFromData to handle the new SndCompressionSubType.
*/
/*****************************************************************************/

#ifndef X_FORMATS
#define X_FORMATS

#ifndef __X_API__
    #include "X_API.h"
#endif

#ifndef G_SOUND
    #include "GenSnd.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#undef X_PACK_FAST
#include "X_PackStructures.h"

/* Instrument and Song structures
*/
typedef struct 
{
    XSWORD       instrumentNumber;
    XSWORD       ResourceINSTID;
} X_PACKBY1 Remap;
    
enum
{
    ID_NULL     =   0L,
    ID_SONG     =   FOUR_CHAR('S','O','N','G'), //  'SONG'      // song
    ID_INST     =   FOUR_CHAR('I','N','S','T'), //  'INST'      // instrument format
    ID_MIDI     =   FOUR_CHAR('M','i','d','i'), //  'Midi'      // standard midi file
    ID_MIDI_OLD =   FOUR_CHAR('M','I','D','I'), //  'MIDI'      // standard midi file
    ID_CMID     =   FOUR_CHAR('c','m','i','d'), //  'cmid'      // compressed midi file
    ID_CMIDI    =   ID_CMID,
    ID_EMID     =   FOUR_CHAR('e','m','i','d'), //  'emid'      // encrypted midi file
    ID_ECMI     =   FOUR_CHAR('e','c','m','i'), //  'ecmi'      // encrypted and compressed midi file
    ID_SND      =   FOUR_CHAR('s','n','d',' '), //  'snd '      // sample
    ID_CSND     =   FOUR_CHAR('c','s','n','d'), //  'csnd'      // compressed sample
    ID_ESND     =   FOUR_CHAR('e','s','n','d'), //  'esnd'      // encrypted sample
    ID_RMF      =   FOUR_CHAR('R','M','F','!'), //  'RMF!'      // rmf object

    ID_BANK     =   FOUR_CHAR('B','A','N','K'), //  'BANK'      // bank ID
 
    ID_PASSWORD =   FOUR_CHAR('E','A','C','S'), //  'EACS'      // encryption access control string (password)
 
    ID_ALIAS    =   FOUR_CHAR('A','L','I','S'), //  'ALIS'      // alias link resource
 
    ID_VERS     =   FOUR_CHAR('V','E','R','S'), //  'VERS'      // version ID
    ID_TEXT     =   FOUR_CHAR('T','E','X','T'), //  'TEXT'      // text
 
    ID_MTHD     =   FOUR_CHAR('M','T','h','d'), //  'MThd'      // midi header ID
    ID_MTRK     =   FOUR_CHAR('M','T','r','k')  //  'MTrk'      // midi track ID
};

// Macro's used to set/clear various flags
#define SET_FLAG_VALUE(oldflag, newflag, value)     (value) ? ((oldflag) | (newflag)) : ((oldflag) & ~(newflag))
#define TEST_FLAG_VALUE(flags, flagbit)             ((flags) & (flagbit)) ? TRUE : FALSE

typedef enum
{
    SONG_TYPE_BAD = -1,
    SONG_TYPE_SMS = 0,
    SONG_TYPE_RMF = 1,          // structured
    SONG_TYPE_RMF_LINEAR = 2    // linear
} SongType;


// bits for SongResource_SMS flags1
#define XBF_locked                      0x80
#define XBF_terminateDecay              0x40
#define XBF_reserved11                  0x20    // not used. use to be XBF_interpolateSong
#define XBF_reserved12                  0x10    // not used. use to be XBF_interpolateLead
#define XBF_fileTrackFlag               0x08
#define XBF_enableMIDIProgram           0x04
#define XBF_reserved13                  0x02    // not used. use to be XBF_disableClickRemoval
#define XBF_useLeadForAllVoices         0x01

// bits for SongResource_SMS flags2
#define XBF_ignoreBadPatches            0x80
#define XBF_reserved4                   0x40
#define XBF_reserved5                   0x20
#define XBF_reserved7                   0x10    // not used. use to be XBF_masterEnablePitchRandomness
#define XBF_reserved8                   0x08    // not used. use to be XBF_ampScaleLead
#define XBF_reserved9                   0x04    // not used. use to be XBF_forceAmpScale
#define XBF_reserved10                  0x02    // not used. use to be XBF_masterEnableAmpScale
#define XBF_reserved6                   0x01

// Song resource (SMS type)
typedef struct
{
    XShortResourceID    midiResourceID;
    XBYTE                reserved_0;
    XBYTE                reverbType;
    XWORD      songTempo;
    XBYTE                songType;                       //  0 - SMS, 1 - RMF, 2 - MOD

    XBYTE                songPitchShift;
    XBYTE                maxEffects;
    XBYTE                maxNotes;
    XSWORD           mixLevel;
    XBYTE       flags1;                         // see XBF for flags1
    XBYTE                noteDecay;
    XBYTE                defaultPercusionProgram;        // yes, I wanted signed!
    XBYTE       flags2;                         // see XBF for flags2
    XSWORD           remapCount;
    XBYTE                remaps;                         // Remap variable
//  XBYTE       copyright;                      // variable pascal string
    XBYTE       author;                         // variable pascal string
    XBYTE       title;                          // variable pascal string
    XBYTE       licensor_contact;
} X_PACKBY1 SongResource_SMS;


typedef enum
{
    R_LAST_RESOURCE     = 0,                            // empty. Not used
    R_TITLE             = FOUR_CHAR('T','I','T','L'),   // TITL Byte zero terminated string
    R_PERFORMED_BY      = FOUR_CHAR('P','E','R','F'),   // PERF Byte zero terminated string
    R_COMPOSER          = FOUR_CHAR('C','O','M','P'),   // COMP Byte zero terminated string
    R_COPYRIGHT_DATE    = FOUR_CHAR('C','O','P','D'),   // COPD Byte zero terminated string

    // obsolete
    R_COPYRIGHT_LINE    = FOUR_CHAR('C','O','P','L'),   // COPL Byte zero terminated string

    R_PUBLISHER_CONTACT = FOUR_CHAR('L','I','C','C'),   // LICC Byte zero terminated string
    R_USE_OF_LICENSE    = FOUR_CHAR('L','U','S','E'),   // LUSE Byte zero terminated string
    R_LICENSED_TO_URL   = FOUR_CHAR('L','D','O','M'),   // LDOM Byte zero terminated string
    R_LICENSE_TERM      = FOUR_CHAR('L','T','R','M'),   // LTRM Byte zero terminated string
    R_EXPIRATION_DATE   = FOUR_CHAR('E','X','P','D'),   // EXPD Byte zero terminated string
    R_COMPOSER_NOTES    = FOUR_CHAR('N','O','T','E'),   // NOTE Byte zero terminated string
    R_INDEX_NUMBER      = FOUR_CHAR('I','N','D','X'),   // INDX Byte zero terminated string
    R_GENRE             = FOUR_CHAR('G','E','N','R'),   // GENR Byte zero terminated string
    R_SUB_GENRE         = FOUR_CHAR('S','U','B','G'),   // SUBG Byte zero terminated string
    R_TEMPO             = FOUR_CHAR('T','M','P','O'),   // TMPO Byte zero terminated string
    R_ORIGINAL_SOURCE   = FOUR_CHAR('O','S','R','C'),   // OSRC Byte zero terminated string
    R_INSTRUMENT_REMAP  = FOUR_CHAR('R','M','A','P'),   // RMAP variable amount
    R_VELOCITY_CURVE    = FOUR_CHAR('V','E','L','C'),   // VELC 128 words
    R_MANUFACTURER      = FOUR_CHAR('M','A','N','U'),   // MANU Byte zero terminated string
    R_MISC1             = FOUR_CHAR('M','S','C','1'),   // MSC1 Byte zero terminated string
    R_MISC2             = FOUR_CHAR('M','S','C','2'),   // MSC2 Byte zero terminated string
    R_MISC3             = FOUR_CHAR('M','S','C','3'),   // MSC3 Byte zero terminated string
    R_MISC4             = FOUR_CHAR('M','S','C','4'),   // MSC4 Byte zero terminated string
    R_MISC5             = FOUR_CHAR('M','S','C','5'),   // MSC5 Byte zero terminated string
    R_MISC6             = FOUR_CHAR('M','S','C','6'),   // MSC6 Byte zero terminated string
    R_MISC7             = FOUR_CHAR('M','S','C','7'),   // MSC7 Byte zero terminated string
    R_MISC8             = FOUR_CHAR('M','S','C','8')    // MSC8 Byte zero terminated string
} SongResourceType;


// Song resource (RMF type - structured)
typedef struct
{
    XShortResourceID    rmfResourceID;
    XBYTE                reserved_0;
    XBYTE                reverbType;
    XWORD      songTempo;
    XBYTE                songType;                       //  0 - SMS, 1 - RMF, 2 - MOD

    XBYTE                locked;                         // are resource encrypted
    XSWORD           songPitchShift;
    XSWORD           maxEffects;
    XSWORD           maxNotes;
    XSWORD           mixLevel;
    XSWORD           songVolume;                     // 127 is 100%, 256 is 200% etc.
    XBYTE                embeddedSong;                   // TRUE if embedded in a bank
    XBYTE                reserved_1;
    XDWORD                unused[7];
    
    XSWORD           resourceCount;
    XSWORD           resourceData;                   // subtract this when calculating empty structure
    //
    // from this point on, the data is based upon types and data blocks
//  XBYTE                title[1];                       // variable C string
//  XBYTE                composer[1];                    // variable C string
//  XBYTE                copyright_date[1];              // variable C string
//  XBYTE                copyright_line[1];              // variable C string
//  XBYTE                contact_info[1];                // variable C string
//  XBYTE                use_license[1];                 // variable C string
//  XBYTE                license_term[1];                // variable C string
//  XBYTE                territory[1];                   // variable C string
//  XBYTE                expire_date[1];                 // variable C string
//  XBYTE                foreign_rights[1];              // variable C string
//  XBYTE                compser_notes[1];               // variable C string
//  XBYTE                index_number[1];                // variable C string
//  XSWORD           velocityCurve[128];
} X_PACKBY1 SongResource_RMF;

// bits for SongResource_RMF_Linear flags
#define XBFL_disableLoops               0x80
#define XBFL_embedded                   0x40
#define XBFL_reserved5                  0x20
#define XBFL_reserved4                  0x10
#define XBFL_reserved3                  0x08
#define XBFL_reserved2                  0x04
#define XBFL_reserved1                  0x02
#define XBFL_reserved0                  0x01

// Song resource (RMF type - linear)
typedef struct
{
    XShortResourceID    audioResourceID;
    XBYTE                reserved_0;
    XBYTE                reverbType;
    XWORD      songTempo;
    XBYTE                songType;                       //  0 - SMS, 1 - RMF, 2 - RMF LINEAR

    XBYTE                locked;                         // are resource encrypted
    XSWORD           maxEffects;
    XSWORD           maxNotes;
    XSWORD           mixLevel;
    XSWORD           songVolume;                     // 100 is 100%, 200 is 200% etc.
    XResourceType       audioFormatType;
    XFIXED              sampleRate;
    XDWORD       lengthInBytes;                  // length in bytes uncompressed
    XDWORD       lengthInFrames;                 // length in frames uncompressed
    XBYTE                channels;
    XBYTE                bitSize;
    XBYTE                flags;                          // see SongResource_RMF_Linear flags
    
    XBYTE                unused1;
    XDWORD                unused2[3];
    
    XSWORD           resourceCount;
    XSWORD           resourceData;                   // subtract this when calculating empty structure
    //
    // from this point on, the data is based upon types and data blocks
//  XBYTE                title[1];                       // variable C string
//  XBYTE                composer[1];                    // variable C string
//  XBYTE                copyright_date[1];              // variable C string
//  XBYTE                copyright_line[1];              // variable C string
//  XBYTE                contact_info[1];                // variable C string
//  XBYTE                use_license[1];                 // variable C string
//  XBYTE                license_term[1];                // variable C string
//  XBYTE                territory[1];                   // variable C string
//  XBYTE                expire_date[1];                 // variable C string
//  XBYTE                foreign_rights[1];              // variable C string
//  XBYTE                compser_notes[1];               // variable C string
//  XBYTE                index_number[1];                // variable C string
} X_PACKBY1 SongResource_RMF_Linear;

#define DEFAULT_RESOURCE_VERS_ID    0       // ID used inside of RMF file, or a bank
// version structure
typedef struct
{
    XSWORD   versionMajor;
    XSWORD   versionMinor;
    XSWORD   versionSubMinor;
} X_PACKBY1 XVersion;

typedef void SongResource;

// SongResource structure expanded. These values are always in native word order
// Use XGetSongResourceInfo, and XNewSongResourceInfo/XDisposeSongResourceInfo
typedef struct
{
    XSWORD           maxMidiNotes;
    XSWORD           maxEffects;
    XSWORD           mixLevel;
    XSWORD           reverbType;
    XShortResourceID    objectResourceID;
    XSWORD           songVolume;
    SongType            songType;
    XDWORD                songTempo;
    XSWORD           songPitchShift;
    XBOOL               songLocked;
    XBOOL               songEmbedded;

    XBYTE                *title;                         // 0
    XBYTE                *performed;                     // 1
    XBYTE                *composer;                      // 2
    XBYTE                *copyright;                     // 3
    XBYTE                *publisher_contact_info;        // 4
    XBYTE                *use_license;                   // 5
    XBYTE                *licensed_to_URL;               // 6
    XBYTE                *license_term;                  // 7
    XBYTE                *expire_date;                   // 8
    XBYTE                *compser_notes;                 // 9
    XBYTE                *index_number;                  // 10
    XBYTE                *genre;                         // 11
    XBYTE                *sub_genre;                     // 12
    XBYTE                *tempo_description;             // 13
    XBYTE                *original_source;               // 14

    XBYTE                *manufacturer;                  // 15
    XBYTE                *misc1;                         // 16
    XBYTE                *misc2;                         // 17
    XBYTE                *misc3;                         // 18
    XBYTE                *misc4;                         // 19
    XBYTE                *misc5;                         // 20
    XBYTE                *misc6;                         // 21
    XBYTE                *misc7;                         // 22
    XBYTE                *misc8;

    XSWORD           remapCount;
    XSWORD           *remaps;

    XSWORD           *velocityCurve;
} X_PACKBY1 SongResource_Info;


typedef struct
{
    XBYTE                lowMidi;
    XBYTE                highMidi;
    XShortResourceID    sndResourceID;
    XSWORD           miscParameter1;     // can be smodParmeter1 if ZBF_enableSoundModifier
                                            // enabled, otherwise its a replacement
                                            // rootKey for sample
    XSWORD           miscParameter2;     // if ZBF_enableSoundModifier is enabled then its
                                            // used as smodParmeter2, otherwise its a volume level
} X_PACKBY1 KeySplit;

// bits for Instrument flags1
#define ZBF_enableInterpolate           0x80    // not used
#define ZBF_enableAmpScale              0x40    // not used
#define ZBF_disableSndLooping           0x20
#define ZBF_reserved_1                  0x10
#define ZBF_useSampleRate               0x08
#define ZBF_sampleAndHold               0x04
#define ZBF_extendedFormat              0x02
#define ZBF_avoidReverb                 0x01    // this is a default enable switch to send to the mix buss. TRUE is off.
// bits for Instrument flags2
#define ZBF_reserved_0                  0x80
#define ZBF_playAtSampledFreq           0x40
#define ZBF_fitKeySplits                0x20    // not used
#define ZBF_enableSoundModifier         0x10
#define ZBF_useSoundModifierAsRootKey   0x08
#define ZBF_notPolyphonic               0x04    // not used
#define ZBF_enablePitchRandomness       0x02    // not used
#define ZBF_playFromSplit               0x01    // not used

// Special Instrument resource. This can only be used when there is no tremolo data, or key splits
typedef struct
{
    XShortResourceID    sndResourceID;
    XSWORD           midiRootKey;
    XBYTE                panPlacement;
    XBYTE       flags1;             // see ZBF bits for values
    XBYTE       flags2;             // see ZBF bits for values
    XBYTE                smodResourceID;     // Really a smaller version of XShortResourceID
    XSWORD           miscParameter1;     // can be smodParmeter1 if ZBF_enableSoundModifier
                                            // enabled, otherwise its a replacement
                                            // rootKey for sample
    XSWORD           miscParameter2;     // if ZBF_enableSoundModifier is enabled then its
                                            // used as smodParmeter2, otherwise its a volume level
    XSWORD           keySplitCount;      // if this is non-zero, then KeySplit structure is inserted
    // to go beyond this point, if keySplitCount is non-zero, you must use function calls.
    XSWORD           tremoloCount;       // if this is non-zero, then a Word is inserted.
    XSWORD           tremoloEnd;         // Always 0x8000
    XSWORD           reserved_3;
    XSWORD           descriptorName;     // Always 0
    XSWORD           descriptorFlags;    // Always 0
} X_PACKBY1 InstrumentResource;

#define DEFAULT_RESOURCE_BANK_ID    0       // ID used inside of RMF file
#define BANK_NAME_MAX_SIZE          4096
// This is a ID_BANK resource
typedef struct
{
    XDWORD   version;
    XBYTE            bankURL[BANK_NAME_MAX_SIZE];
    XBYTE            bankName[BANK_NAME_MAX_SIZE];
} X_PACKBY1 BankStatus;

#define DEFAULT_RESOURCE_PASSWORD_ID 0      // ID used inside of RMF file
// This is a ID_PASSWORD resource
typedef struct
{
    XDWORD   version;
//  XBYTE            eacs[];     // variable length password accessed with XDecryptAndDuplicateStr
} X_PACKBY1 PasswordAccess;

#define DEFAULT_RESOURCE_ALIAS_ID   0       // ID used inside of a patch file
#define ALIAS_ID_RESOURCE_VERSION   0

typedef struct
{
    XLongResourceID aliasFrom;
    XLongResourceID aliasTo;
} X_PACKBY1 XAliasLink;

// This is a ID_ALIAS resource
typedef struct
{
    XDWORD   version;
    XDWORD   numberOfAliases;
    XAliasLink      list[1];    // dynamic list
} X_PACKBY1 XAliasLinkResource;


// audioType for the AudioResource structure
enum
{
    AUDIO_NAME_TYPE =   FOUR_CHAR('A','T','X','T'), //  'ATXT'      // non-terminated string. Length of string is
                                                    //              // length of resource
    AUDIO_SND       =   FOUR_CHAR('s','n','d',' '), //  'snd '      // MacOS 'snd' format
    AUDIO_CSND      =   FOUR_CHAR('c','s','n','d'), //  'csnd'      // MacOS 'snd' format compressed
    AUDIO_ESND      =   FOUR_CHAR('e','s','n','d'), //  'esnd'      // MacOS 'snd' format encrypted
    AUDIO_MPEG      =   FOUR_CHAR('M','P','E','G'), //  'MPEG'      // MPEG
    AUDIO_IMA2_PCM  =   FOUR_CHAR('I','M','A','2'), //  'IMA2'      // IMA PCM 2 to 1
    AUDIO_IMA4_PCM  =   FOUR_CHAR('I','M','A','4'), //  'IMA4'      // IMA PCM 4 to 1
    AUDIO_RAW_PCM   =   FOUR_CHAR('R','P','C','M')  //  'RPCM'      // raw pcm data
};

#define AUDIO_OBJECT_VERSION            0x0001
typedef struct
{
    XDWORD   version;            // structure version 1
    XDWORD   dataLength;         // length of sample data in bytes
    XDWORD   dataOffset;         // offset from begining of structure to data
    XDWORD   audioType;          // audio type of audioType
    XDWORD   usageType;
    XDWORD   sampleRate;         // sample rate in 16.16 fixed point
    XDWORD   sampleFrames;       // number of sample frames
    XDWORD   loopStart;          // first loop start
    XDWORD   loopEnd;            // first loop end
    XSWORD       baseMidiKey;        // base root midi key
    XSWORD       bitSize;            // 8 or 16 bits per sample
    XSWORD       channels;           // 1 or 2 channels
    XResourceType   nameResourceType;   // Resource name type. ie (AUDIO_NAME_TYPE)
                                        // if ID_NULL, then no name
    XLongResourceID nameResourceID;     // Resource name id. ie AUDIO_NAME_TYPE ID 2000
    XBYTE            usedInBank;         // if true, then sample is embedded in a bank
    XBYTE            unusedFlag2;
    XBYTE            unusedFlag3;
    XBYTE            unusedFlag4;
    XDWORD   filler[16];
    XDWORD   firstSampleFiller;
//  data
} X_PACKBY1 AudioResource;

// Sun compiler warns about this as an enum
#define rate48khz           0xBB800000L // 48000.00000 in fixed-point

// These are included here, because we want to be independent of MacOS, but use this standard format
#ifndef __SOUND__

// Sun compiler warns about this as an enum
#define rate44khz           0xAC440000L // 44100.00000 in fixed-point

enum 
{
    notCompressed           = 0,            /*compression ID's*/
    fixedCompression        = -1,           /*compression ID for fixed-sized compression*/
    variableCompression     = -2,           /*compression ID for variable-sized compression*/
    twoToOne                = 1,
    eightToThree            = 2,
    threeToOne              = 3,
    sixToOne                = 4,

    rate22050hz             = 0x56220000L,  /*22050.00000 in fixed-point*/
    rate22khz               = 0x56EE8BA3L,  /*22254.54545 in fixed-point*/
    rate11khz               = 0x2B7745D1L,  /*11127.27273 in fixed-point*/
    rate11025hz             = 0x2B110000,   /*11025.00000 in fixed-point*/

    kMiddleC                = 60,           /*MIDI note value for middle C*/

    soundCmd                = 80,
    bufferCmd               = 81,
};
#endif

enum 
{
// Sound format types for 'snd ' resources
    XFirstSoundFormat       = 0x0001,       // general sound format
    XSecondSoundFormat      = 0x0002,       // special sampled sound format (HyperCard)
    XThirdSoundFormat       = 0x0003,       // new Beatnik 'snd ' type

// Sound type 1 & 2 encode sub types, and the last one for type 3
    XStandardHeader         = 0x00,         // Standard sound header encode value
    XExtendedHeader         = 0xFF,         // Extended sound header encode value
    XCompressedHeader       = 0xFE,         // Compressed sound header encode value
    XType3Header            = 0x80          // New standard type 3 snd resource
};

// This is the third sample format support by Beatnik.
//
typedef struct
{
    XResourceType       subType;            // sub type: C_NONE, C_IMA4, C_ULAW, C_MPEG, etc
    XFIXED              sampleRate;         // sample rate
    XDWORD              decodedBytes;       // the size of the decoded data, not including the frames skipped by startFrame
    XDWORD              frameCount;         // the number of sample-frames, not including the frames skipped by startFrame
                                            // if C_MPEG, the number of blocks
    XDWORD              encodedBytes;       // the size of the encoded data (same as decodedBytes if C_NONE)
    XDWORD              blockBytes;         // once stored the size of an MPEG block, now unused
    XDWORD              startFrame;         // how many frames to skip at the beginning of uncompressed sample data
                                            // if C_MPEG, the number of 16-bit words (samples)
    XDWORD              loopStart[6];       // loop start frame for each channel. max 6 channels
    XDWORD              loopEnd[6];         // loop end frame
    XResourceType       nameResourceType;   // Resource name type. ie (AUDIO_NAME_TYPE)
                                            // if ID_NULL, then no name
    XLongResourceID     nameResourceID;     // Resource name id. ie AUDIO_NAME_TYPE ID 2000

    XBYTE               baseKey;            // base sample key
    XBYTE               channels;           // 1 for mono, 2 for stereo, up to 6
    XBYTE               bitSize;            // sample bit size; 8 or 16
    XBYTE               isEmbedded;         // is sample embedded
    XBYTE               isEncrypted;        // is sample encrypted
    XBYTE               isSampleIntelOrder; // if true, then sampleArea data is intel ordered
    XBYTE               reserved2[2];       // alignment to 8 bytes
    XDWORD              reserved3[8];       // extra

    XBYTE               sampleArea[1];      // space for when samples follow directly
} X_PACKBY1 XSoundHeader3;


typedef struct
{
    XDWORD      samplePtrOffset;    /*if NIL then samples are in sampleArea*/
    XDWORD           length;         /*length of sound in bytes*/
    XDWORD           sampleRate;     /*sample rate for this sound*/
    XDWORD           loopStart;      /*start of looping portion*/
    XDWORD           loopEnd;        /*end of looping portion*/
    XBYTE           encode;         /*header encoding*/
    XBYTE           baseFrequency;  /*baseFrequency value*/
    XBYTE           sampleArea[1];  /*space for when samples follow directly*/
} X_PACKBY1 XSoundHeader;
typedef XSoundHeader *XSoundHeaderPtr;

typedef struct
{
    XDWORD      samplePtrOffset;    /*if NIL then samples are in sampleArea*/
    XDWORD           numChannels;        /*number of channels i.e. mono = 1*/
    XDWORD           sampleRate;         /*sample rate in Apples Fixed point representation*/
    XDWORD           loopStart;          /*loopStart of sound before compression*/
    XDWORD           loopEnd;            /*loopEnd of sound before compression*/
    XBYTE           encode;             /*data structure used , stdSH, extSH, or cmpSH*/
    XBYTE           baseFrequency;      /*same meaning as regular SoundHeader*/
    XDWORD           numFrames;          /*length in frames ( packetFrames or sampleFrames )*/
    XBYTE                    AIFFSampleRate[10]; /*IEEE sample rate*/
    XBYTE                    markerChunk[4];       /*sync track*/
    XDWORD                    format;             /*data format type, was futureUse1*/
    XBYTE                    forceSample8bit;    /*reserved by Apple, Igor will use as IMA encoder to 8 or 16 bit output. Set to 0x80 */
                                                // to encode as 8 bit output
    XBYTE                    soundIsEmbedded;    /*reserved by Apple. Igor uses it as a flag */
    XBYTE                    futureUse2_2;       /*reserved by Apple*/
    XBYTE                    futureUse2_3;       /*reserved by Apple*/
    XBYTE                    stateVars[4];         /*pointer to State Block*/
    XBYTE                    leftOverSamples[4];   /*used to save truncated samples between compression calls*/
    XSWORD                   compressionID;      /*0 means no compression, non zero means compressionID*/
    XWORD          packetSize;         /*number of bits in compressed sample packet*/
    XWORD          snthID;             /*resource ID of Sound Manager snth that contains NRT C/E*/
    XWORD          sampleSize;         /*number of bits in non-compressed sample*/
                                                //MOE: For ALAW, ULAW, and IMA, the number of bytes is stored!
    XBYTE           sampleArea[1];      /*space for when samples follow directly*/
} X_PACKBY1 XCmpSoundHeader;
typedef XCmpSoundHeader * XCmpSoundHeaderPtr;

typedef struct
{
    XDWORD      samplePtrOffset;    /*if NIL then samples are in sampleArea*/
    XDWORD           numChannels;        /*number of channels,  ie mono = 1*/
    XDWORD           sampleRate;         /*sample rate in Apples Fixed point representation*/
    XDWORD           loopStart;          /*same meaning as regular SoundHeader*/
    XDWORD           loopEnd;            /*same meaning as regular SoundHeader*/
    XBYTE           encode;             /*data structure used , stdSH, extSH, or cmpSH*/
    XBYTE           baseFrequency;      /*same meaning as regular SoundHeader*/
    XDWORD           numFrames;          /*length in total number of frames*/
    XBYTE                    AIFFSampleRate[10]; /*IEEE sample rate*/
    //TODO: pointer issues, structure is incorrect for 64 bit
    XBYTE                    *markerChunk;       /*sync track*/
    XBYTE                    *instrumentChunks;  /*AIFF instrument chunks*/
    XBYTE                    *AESRecording;
    XWORD          sampleSize;         /*number of bits in sample*/
    XBYTE                    soundIsEmbedded;    // reserved by Apple. Igor uses it as a flag
    XBYTE                    sampleIsIntelOrder; // reserved by Apple. Igor uses it to determine if samples are Intel ordered
    XDWORD           futureUse2;         /*reserved by Apple*/
    XDWORD           futureUse3;         /*reserved by Apple*/
    XDWORD           futureUse4;         /*reserved by Apple*/
    XBYTE           sampleArea[1];      /*space for when samples follow directly*/
} X_PACKBY1 XExtSoundHeader;
typedef XExtSoundHeader *XExtSoundHeaderPtr;

typedef struct
{
    XSWORD       type;
    XSWORD       numModifiers;
    XWORD  modNumber;
    XDWORD            modInit;
    XSWORD       numCommands;
// first command
    XWORD  cmd;
    XSWORD       param1;
    XDWORD            param2;
} X_PACKBY1 XSoundFormat1;

typedef struct
{
    XSoundFormat1   sndHeader;
    XSoundHeader    sndBuffer;
} X_PACKBY1 XSndHeader1;

typedef struct
{
    XSoundFormat1   sndHeader;
    XExtSoundHeader sndBuffer;
} X_PACKBY1 XExtSndHeader1;

typedef struct
{
    XSoundFormat1   sndHeader;
    XCmpSoundHeader sndBuffer;
} X_PACKBY1 XCmpSndHeader1;

typedef struct
{
    XSWORD       type;
    XSWORD       refCount;
    XSWORD       numCmds;
// first command
    XWORD  cmd;
    XSWORD       param1;
    XDWORD            param2;
} X_PACKBY1 XSoundFormat2;

typedef struct
{
    XSoundFormat2   sndHeader;
    XSoundHeader    sndBuffer;
} X_PACKBY1 XSndHeader2;

typedef struct
{
    XSoundFormat2   sndHeader;
    XExtSoundHeader sndBuffer;
} X_PACKBY1 XExtSndHeader2;

typedef struct
{
    XSWORD       type;
    XSoundHeader3   sndBuffer;
} X_PACKBY1 XSndHeader3;

#include "X_UnpackStructures.h"

// These are the list of compression types BAE supports.
typedef enum SndCompressionType
{
                                                        // for all of these compression types
                                                        // there is no default state. The
                                                        // SndCompressionSubType is not used.
// Compression Types
    C_LZSS              = FOUR_CHAR('L','Z','S','S'),   // 'LZSS'   csnd compressed snd resource. not inside of snd resource

    // these are used in the snd formatted resource
    C_NONE              = FOUR_CHAR('n','o','n','e'),   // 'none'
    C_IMA4              = FOUR_CHAR('i','m','a','4'),   // 'ima4'   CCITT G.721 ADPCM compression (IMA 4 to 1)
    C_IMA4_WAV          = FOUR_CHAR('i','m','a','W'),   // 'imaW'   Microsoft's ADPCM
    C_MACE3             = FOUR_CHAR('m','a','c','3'),   // 'mac3'   Apple MACE type 3 to 1
    C_MACE6             = FOUR_CHAR('m','a','c','6'),   // 'mac6'   Apple MACE type 6 to 1
    C_ULAW              = FOUR_CHAR('u','l','a','w'),   // 'ulaw'   µLaw; 2 to 1
    C_ALAW              = FOUR_CHAR('a','l','a','w'),   // 'ulaw'   aLaw; 2 to 1

                                                        // for all of these compression types
                                                        // the SndCompressionSubType (CS_DEFAULT)
                                                        // is CS_MPEG2.
    C_MPEG_32           = FOUR_CHAR('m','p','g','n'),   // 'mpgn'   Beatnik mpeg implementation 32k bits
    C_MPEG_40           = FOUR_CHAR('m','p','g','a'),   // 'mpga'   Beatnik mpeg implementation 40k bits
    C_MPEG_48           = FOUR_CHAR('m','p','g','b'),   // 'mpgb'   Beatnik mpeg implementation 48k bits
    C_MPEG_56           = FOUR_CHAR('m','p','g','c'),   // 'mpgc'   Beatnik mpeg implementation 56k bits
    C_MPEG_64           = FOUR_CHAR('m','p','g','d'),   // 'mpgd'   Beatnik mpeg implementation 64k bits
    C_MPEG_80           = FOUR_CHAR('m','p','g','e'),   // 'mpge'   Beatnik mpeg implementation 80k bits
    C_MPEG_96           = FOUR_CHAR('m','p','g','f'),   // 'mpgf'   Beatnik mpeg implementation 96k bits
    C_MPEG_112          = FOUR_CHAR('m','p','g','g'),   // 'mpgg'   Beatnik mpeg implementation 112k bits
    C_MPEG_128          = FOUR_CHAR('m','p','g','h'),   // 'mpgh'   Beatnik mpeg implementation 128k bits
    C_MPEG_160          = FOUR_CHAR('m','p','g','i'),   // 'mpgh'   Beatnik mpeg implementation 160k bits
    C_MPEG_192          = FOUR_CHAR('m','p','g','j'),   // 'mpgj'   Beatnik mpeg implementation 192k bits
    C_MPEG_224          = FOUR_CHAR('m','p','g','k'),   // 'mpgk'   Beatnik mpeg implementation 224k bits
    C_MPEG_256          = FOUR_CHAR('m','p','g','l'),   // 'mpgl'   Beatnik mpeg implementation 256k bits
    C_MPEG_320          = FOUR_CHAR('m','p','g','m')    // 'mpgm'   Beatnik mpeg implementation 320k bits

} SndCompressionType;

// This is a modifier sub type for encoding only. 
typedef enum SndCompressionSubType
{
    CS_DEFAULT          = FOUR_CHAR('d','e','f','t'),   // 'deft'   default state
    CS_MPEG1            = FOUR_CHAR('m','1','0','0'),   // MPEG 1.0 compatible streams
    CS_MPEG2            = FOUR_CHAR('m','2','0','0')    // MPEG 2.0 compatible streams
} SndCompressionSubType;

enum
{
    // these are used in AIFF/AIFC formatted files
    X_NONE              = FOUR_CHAR('N','O','N','E'),   //  'NONE'
    X_ACE2              = FOUR_CHAR('A','C','E','2'),   //  'ACE2'
    X_ACE8              = FOUR_CHAR('A','C','E','8'),   //  'ACE8'
    X_MACE3             = FOUR_CHAR('M','A','C','3'),   //  'MAC3'
    X_MACE6             = FOUR_CHAR('M','A','C','6'),   //  'MAC6'
    X_IMA3              = FOUR_CHAR('i','m','a','3'),   //  'ima3'
    X_IMA4              = FOUR_CHAR('i','m','a','4')    //  'ima4'
};

// Apple AIFF SND-recource header length
#define AIFF_IMA_HEADER_BYTES   2
// Apple AIFF SND-recource standard frames-per-block
#define AIFF_IMA_BLOCK_FRAMES   64
// Apple AIFF SND-recource block length
#define AIFF_IMA_BLOCK_BYTES    (AIFF_IMA_BLOCK_FRAMES / 2 + AIFF_IMA_HEADER_BYTES)

// Microsoft WAV file header length
#define WAV_IMA_HEADER_BYTES    4   // bytes, one for each channel


typedef struct
{
    XFIXED              rate;               // sample rate
    XDWORD       frames;             // number of audio frames
    XDWORD       size;               // size in bytes, compressed size if compressed
    XDWORD       loopStart;          // loop start frame
    XDWORD       loopEnd;            // loop end frame
    XSWORD           bitSize;            // sample bit size; 8 or 16
    XSWORD           channels;           // mono or stereo; 1 or 2
    XSWORD           baseKey;            // base sample key
    XShortResourceID    theID;              // sample ID if required
    XResourceType       compressionType;    // compression type
    void                *pMasterPtr;        // master pointer if required
                                            //MOE: This field needs documenting!
} SampleDataInfo;


typedef enum
{
    I_INVALID = 0,              // invalid type
    I_TITLE,                    // Title
    I_PERFORMED_BY,             // Performed by
    I_COMPOSER,                 // Composer(s)
    I_COPYRIGHT,                // Copyright Date
    I_PUBLISHER_CONTACT,        // Publisher Contact Info
    I_USE_OF_LICENSE,           // Use of License
    I_LICENSED_TO_URL,          // License to URL
    I_LICENSE_TERM,             // License term
    I_EXPIRATION_DATE,          // Expiration Date
    I_COMPOSER_NOTES,           // Composer Notes
    I_INDEX_NUMBER,             // Index Number
    I_GENRE,                    // Genre
    I_SUB_GENRE,                // Sub-genre
    I_TEMPO,                    // song tempo (fast, slow, etc)
    I_ORIGINAL_SOURCE,          // Original source (CD, bin box, trash heap)
    I_MANUFACTURER,
    I_MISC1,
    I_MISC2,
    I_MISC3,
    I_MISC4,
    I_MISC5,
    I_MISC6,
    I_MISC7,
    I_MISC8
} SongInfo;

void XGetSongInformation(SongResource *theSong, XDWORD songSize, SongInfo type,
                         XBYTE* targetBuffer, XDWORD bufferBytes);

XDWORD XGetSongInformationSize(SongResource *theSong, XDWORD songSize, SongInfo type);


// Create a new song resource.
SongResource * XNewSongPtr( SongType songType, 
                            XShortResourceID midiID,
                           XSWORD maxSongVoices,
                           XSWORD mixLevel,
                           XSWORD maxEffectVoices,
                            ReverbMode reverbType);

void XDisposeSongPtr(SongResource *theSong);

void XGetKeySplitFromPtr(InstrumentResource *theX, XSWORD entry, KeySplit *keysplit);

XPTR XGetSoundResourceByID(XLongResourceID theID, XDWORD *pReturnedSize);
XPTR XGetSoundResourceByName(void *cName, XDWORD *pReturnedSize);
// Get sound resource and detach from resource manager but don't decompress.
XPTR XGetRawSoundResourceByID(XLongResourceID theID, XResourceType *pReturnedType, XDWORD *pReturnedSize);

XPTR XGetMidiData(XLongResourceID theID, XDWORD *pReturnedSize, XResourceType *pType);

// Given a ID_SND resource, parse through and return in *pInfo the information
// about the sample resource. The pMasterPtr will be set to the resource passed in (pRes).
//
// Returns  a pointer that is either 
//          1) an offset from pRes which is the actaul PCM data
//          2) a new allocated pointer which is the decoded data from its compressed form.
//              Deallocate this pointer with XDisposePtr.
XPTR XGetSamplePtrFromSnd(XPTR pRes, SampleDataInfo *pInfo);

// Given a ID_SND resource, parse through and return in *pOutInfo the information
// about the sample resource. The pMasterPtr will be NULL.
//
// Returns  0   if no problems, and fills *pOutInfo will details about the sample resource.
//          -1  if parameter err
//          -2  badly formatted resource
//          -3  not supported format type
XERR XGetSampleInfoFromSnd(XPTR pResource, SampleDataInfo *pOutInfo);

void XSetSoundLoopPoints(XPTR pRes, XDWORD loopStart, XDWORD loopEnd);
void XSetSoundSampleRate(XPTR pRes, XFIXED sampleRate);
void XSetSoundBaseKey(XPTR pRes, XSWORD baseKey);
XSWORD XGetSoundBaseKey(XPTR pRes);

XBOOL XGetSoundEmbeddedStatus(XPTR pRes);
void XSetSoundEmbeddedStatus(XPTR pRes, XBOOL soundEmbedded);

// Decodes compressed sound data into a newly allocated memory block
OPErr XDecodeSampleData(GM_Waveform const* src, UINT32 startFrame, GM_Waveform* dst);

// Given a data stream in (dst). This can be unformatted PCM data, or a compressed stream,
// a GM_Waveform (src) describing the data 
// Pass in type a compression type use C_NONE for no compression, and CS_NONE for sub type.
// The compression sub type is used as a modifer of the (dstCompression). It is used to
// control somthing specific with the encoder. Most of the time, you'll pass in CS_NONE.
// Pass in a function that will be called proc during compression only
// Pass in a void* parameter to be passed to the proc
OPErr XCreateSoundObjectFromData(XPTR* dst,
                                    GM_Waveform const* src,
                                    SndCompressionType dstCompression,
                                    SndCompressionSubType dstSubCompression,
                                XCompressStatusProc proc, void* procData);

XBOOL XGetSampleNameFromID(XLongResourceID sampleSoundID, char *cName);

SongResource_Info * XGetSongResourceInfo(SongResource *pSong, XDWORD songSize);

// return a new blank SongResource_Info structure
SongResource_Info * XNewSongResourceInfo(void);

// clear a SongResource_Info structure
void XClearSongResourceInfo(SongResource_Info *pSongInfo);

// dispose of strings, and the SongResource_Info structure
void XDisposeSongResourceInfo(SongResource_Info *pSongInfo);

SongResource * XNewSongFromSongResourceInfo(SongResource_Info *pSongInfo);

XShortResourceID XGetSongResourceObjectID(SongResource *pSong);
void XSetSongResourceObjectID(SongResource *pSong, XShortResourceID id);
XBOOL XIsSongLocked(SongResource *pSong);
void XSetSongLocked(SongResource *pSong, XBOOL locked);

// will determine if song is using compression. Requires active resource file
XBOOL XIsSongCompressed(SongResource *pSong);

void XGetSongPerformanceSettings(SongResource * theSong, XSWORD *maxMidiVoices,
                                 XSWORD *maxEffectsVoices, XSWORD *mixLevel);
void XSetSongPerformanceSettings(SongResource *pSong, XSWORD maxMidiVoices, XSWORD maxEffectsVoices,
                                 XSWORD mixLevel);

XSWORD XGetSongReverbType(SongResource *pSong);
void XSetSongReverbType(SongResource *pSong, XSWORD reverbType);

XSWORD XGetSongVolume(SongResource *pSong);
void XSetSongVolume(SongResource *pSong, XSWORD volume);


SongType XGetSongResourceObjectType(SongResource *pSong);

SongResource * XChangeSongResource(SongResource *theSong, XDWORD songSize,
                    SongResourceType resourceType, void *pResource, XDWORD resourceLength);

XBOOL XGetSongEmbeddedStatus(SongResource *pSong);
void XSetSongEmbeddedStatus(SongResource *pSong, XBOOL embedded);

// Return an AliasLinkResource, and deal with endian issues. Always safe for host.
XAliasLinkResource * XGetAliasLinkFromFile(XFILE thisFile);
// Return an AliasLinkResource, and deal with endian issues. Always safe for host.
XAliasLinkResource * XGetAliasLink(void);
// Given a alias structure and a sourceID, this will return in pDest a valid alias. -1 will
// be returned via XERR if there's no alias present
XERR XLookupAlias(XAliasLinkResource *pLink, XLongResourceID sourceID, XLongResourceID *pDestID);

// Translate a GM_Waveform structure into a SampleDataInfo structure
void XTranslateFromWaveformToSampleData(GM_Waveform const* pSource, SampleDataInfo *pDest);
// Translate a SampleDataInfo structure into a GM_Waveform structure
void XTranslateFromSampleDataToWaveform(SampleDataInfo const* pSource, GM_Waveform *pDest);

// Create a bank resource from a BankStatus structure. This data can be written out
// as a BANK_ID resource
XPTR XCreateBankStatus(BankStatus *pStatus);
// Get bank resource from currently open resource file
void XGetBankStatus(BankStatus *pStatus);
// Create version resource that is ready to be stored
XPTR XCreateVersion(XSWORD major, XSWORD minor, XSWORD subMinor);
// Will return a XVersion number in platform order from the currently open resource file
void XGetVersionNumber(XVersion *pVersionNumber);

// convert to and from ieee to XFIXED. Used for AIF files, and SND resources
XFIXED XConvertFromIeeeExtended(XBYTE *bytes);
void XConvertToIeeeExtended(XFIXED ieeeFixedRate, XBYTE *bytes);

// a law / u law compression
void XCompressLaw(SndCompressionType compressionType, XSWORD *pSource, XBYTE *pDest,
                  XDWORD frames, XDWORD channels);

// Simple utility to convert promote 8-bit samples to 16 bit
XWORD * XConvert8BitTo16Bit(XBYTE * p8BitPCMData, XDWORD frames, XDWORD channels);
// given 16 bit data, convert this to 8 bit data
XBYTE * XConvert16BitTo8Bit(XWORD * p16BitPCMData, XDWORD frames, XDWORD channels);


// MPEG decoder
#if USE_MPEG_DECODER != 0
struct XMPEGDecodedData
{
    void            *stream;
    XFIXED          sampleRate;
    XBYTE           bitSize;
    XBYTE           channels;
    XDWORD   bitrate;
    XDWORD   lengthInBytes;      // right now, it's simply frameBufferSize*maxFrameBuffers
                                        // with new MPEG code, it should be the minimum decoded size
    XDWORD   lengthInSamples;
    XDWORD   frameBufferSize;
    XDWORD   maxFrameBuffers;
};
typedef struct XMPEGDecodedData XMPEGDecodedData;

XMPEGDecodedData * XOpenMPEGStreamFromXFILE(XFILE file, OPErr *pErr);
XMPEGDecodedData * XOpenMPEGStreamFromMemory(XPTR pBlock, XDWORD blockSize, OPErr *pErr);
OPErr XCloseMPEGStream(XMPEGDecodedData *stream);
OPErr XFillMPEGStreamBuffer(XMPEGDecodedData *stream, void *pcmAudioBuffer, XBOOL *pDone);
SndCompressionType XGetMPEGBitrateType(XDWORD bitrate);

#endif  // USE_MPEG_DECODER

// MPEG encoder
#if USE_MPEG_DECODER != 0
// Encoder rates
typedef enum
{
    MPG_32 = 32,        // n
    MPG_40 = 40,        // a
    MPG_48 = 48,        // b
    MPG_56 = 56,        // c
    MPG_64 = 64,        // d
    MPG_80 = 80,        // e
    MPG_96 = 96,        // f
    MPG_112 = 112,      // g
    MPG_128 = 128,      // h
    MPG_160 = 160,      // i
    MPG_192 = 192,      // j
    MPG_224 = 224,      // k
    MPG_256 = 256,      // l
    MPG_320 = 320       // m
} XMPEGEncodeRate;
// MOE: If these enum values are ever changed so that they DON'T equal
// the actual bitrates divided by 1024, the bitrate scaling code in
// XCreateSoundObjectFromData() must be changed!


struct XMPEGEncodeData
{
    XDWORD       currentFrameBuffer;     // OUT  current frame buffer processing
    XDWORD       maxFrameBuffers;        // OUT  max number of MPEG frames
    XDWORD       frameBufferSizeInBytes; // OUT  byte size of each frame buffer
    XBYTE                *pFrameBuffer;          // OUT  bytes of completed buffer
    XDWORD       frameBufferSize;        // OUT  size in bytes of completed MPEG buffer

//private:
    void                *pPrivateData;
    XMPEGEncodeRate     encodeRate;
    GM_Waveform         *pAudio;            // NON-ZERO if from memory
    XFILE               file;               // NON-ZERO if from file
};
typedef struct XMPEGEncodeData XMPEGEncodeData;

// This MPEG library only encode MPEG I, layer 3.

// Open a encode stream; either from a file, or from a memory sample. This function will return an interm structure
// that contains the total number of MPEG buffers. If pFrameBuffer is non-zero then write out or move from memory
// the buffer. Then call XProcessMPEGEncoder for XMPEGEncodeData->maxFrameBuffers and write out every pFrameBuffer.
// When done, then call XCloseMPEGEncodeStream to cleanup and free memory.
XMPEGEncodeData *XOpenMPEGEncodeStreamFromMemory(GM_Waveform *pAudio, XMPEGEncodeRate encodeRate, OPErr *pErr);
OPErr   XProcessMPEGEncoder(XMPEGEncodeData *stream);   // call this XMPEGEncodeData->maxFrameBuffers times
OPErr   XCloseMPEGEncodeStream(XMPEGEncodeData *stream, XPTR *pReturnedBuffer, XDWORD *pReturnedSize);

// Given an mpeg bit encode rate, and a sample rate, this will return TRUE if
// this encoder can encode, or FALSE if it will not work.
XBOOL XIsValidMPEGSampleRateAndEncodeRate(XMPEGEncodeRate encodeRate, XFIXED sampleRate, SndCompressionSubType subType, XSWORD numChannel);

// This will encode an MPEG stream from a formatted GM_Waveform
OPErr XCompressMPEG(GM_Waveform const* pWave,
                    SndCompressionType compressionType, 
                    SndCompressionSubType compressionSubType, // CS_DEFAULT is CS_MPEG2
                    XCompressStatusProc proc, void* procData,
                    XPTR* pCompressedData, XDWORD* pCompressedBytes,
                    XDWORD* pFrameBufferCount, XDWORD* pFrameBufferBytes,
                    XDWORD* startFrame);

OPErr XExpandMPEG(GM_Waveform const* src, UINT32 startFrame, GM_Waveform* dst);

#endif  // USE_MPEG_DECODER

#ifdef __cplusplus
    }
#endif

#endif  // X_FORMATS


