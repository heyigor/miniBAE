/*****************************************************************************/
/*
** "MiniBAE.h"
**
**	Generalized Audio Synthesis package presented in an oop fashion
**
**	© Copyright 1999-2001 Beatnik, Inc, All Rights Reserved.
**	Written by Mark Deggeller and Steve Hales
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
** Modification History:
**	1999.11.08		Created.
**	1999.11.16		Everything in place... Ready for testing!
**	1999.11.29		Reduced BAE_MAX_VOICES to 8. BAE_MAX_SONGS to 2.
**	1999.12.13		Added copious function comments
**	2000.01.06		Added BAE_NULL_OBJECT and BAE_RESOURCE_NOT_FOUND error codes
**	2000.01.13		Changed BAEPathName from void* to char*.  Result is that
**					Macintosh users must use path (MacHD:Folder:File) rather 
**					than FSSpec
**	2000.01.14		Added BAEMixer_IsAudioActive()
**					Added BAESong_AreMidiEventsPending()
**	2000.03.01		Added multiple bank support
**	2000.03.02		Added BAEMixer_UnloadBanks(), added typedef BAEBankToken
**  2000.03.06 MSD	Added support for 32kHz, 40kHz
**	2000.03.07		Fixed undefined BAEMixer_SendBankToBack.
**	2000.03.16 AER	Added BAE_ALREADY_EXISTS error
**	2000.03.23 msd	changed ...PitchOffset() functions to ...Transpose()
**	2000.03.28 msd	updated description of BAESong_SetLoopMax()
**	2000.03.29 msd	Changed copyright and modification history format
**					Removed BAESong_Set/GetLoopFlag()
**					Renamed BAESong_Set/GetLoopMax() BAESong_Set/GetLoops()
**	2000.04.11 msd	Updated comments for Get/SetLoops(), and BAEUtil_...()
**					Updated comments for BAEMixer_GetMasterVolume()
**					Minor other comments updates.
**	2000.10.17	sh	Added BAEMixer_Idle
**	2000.10.18	sh	Added BAEMixer_GetMemoryUsed, BAESound_GetMemoryUsed, 
**					BAESong_GetMemoryUsed
**  2000.11.29  tom Added BAEMixer_StartOutputToFile, BAEMixer_StopOutputToFile,
**                  BAEMixer_ServiceOutputToFile, and supporting globals - ported from BAE.c
**  2000.12.01  tom moved OutputToFile globals to MiniBAE.c to resolve some possible linker conflicts
**	2001.03.28	sh	Added BAEStream call functions.
**					Added BAEMixer_SetFadeRate & BAEMixer_GetFadeRate
*/
/*****************************************************************************/


#ifndef BAE_AUDIO
#define BAE_AUDIO

#ifdef __cplusplus
	extern "C" {
#endif

// types
typedef enum 
{
	BAE_DROP_SAMPLE = 0,
	BAE_2_POINT_INTERPOLATION,
	BAE_LINEAR_INTERPOLATION
} BAETerpMode;

// Supported sample rates
typedef enum
{
	BAE_RATE_INVALID		=	0,
	BAE_RATE_7K				=	7813L,			// 7.813 kHz
	BAE_RATE_8K				=	8000L,			// 8 kHz
	BAE_RATE_8270			= 	8270L,			// 8.270 kHz
	BAE_RATE_10K			=	10417L,			// 10.417 kHz
	BAE_RATE_11K			=	11025L,			// 11 kHz
	BAE_RATE_11027			=	11027L,			// 11.027 kHz
	BAE_RATE_11K_TERP_22K	=	-11025L,		// 11 kHz interpolated to 22 kHz
	BAE_RATE_15K			=	15625L,			// 15.625 kHz
	BAE_RATE_16K			=	16000L,			// 16 kHz
	BAE_RATE_16540			= 	16540L,			// 16.540 kHz
	BAE_RATE_20K			=	20833L,			// 20.833 kHz
	BAE_RATE_22K			=	22050L,			// 22 kHz
	BAE_RATE_22K_TERP_44K	=	-22050L,		// 22 kHz interpolated to 44 kHz
	BAE_RATE_22053			= 	22053L,			// 22.053 kHz
	BAE_RATE_24K			=	24000L,			// 24 kHz
	BAE_RATE_32K			=	32000L,			// 32 kHz
	BAE_RATE_40K			=	40000L,			// 40 kHz
	BAE_RATE_44K			=	44100L,			// 44 kHz
	BAE_RATE_48K			=	48000			// 48 kHz
} BAERate;

// Modifier types
#define BAE_NONE				0L
#define BAE_USE_16				(1<<0L)		// use 16 bit output
#define BAE_USE_STEREO			(1<<1L)		// use stereo output
#define BAE_DISABLE_REVERB		(1<<2L)		// disable reverb
#define BAE_STEREO_FILTER		(1<<3L)		// if stereo is enabled, use a stereo filter
typedef long BAEAudioModifiers;

typedef enum  
{
	BAE_REVERB_NO_CHANGE = 0,				// don't change the mixer settings
	BAE_REVERB_NONE = 1,
	BAE_REVERB_TYPE_1 = 1,					// None
	BAE_REVERB_TYPE_2,						// Igor's Closet
	BAE_REVERB_TYPE_3,						// Igor's Garage
	BAE_REVERB_TYPE_4,						// Igor's Acoustic Lab
	BAE_REVERB_TYPE_5,						// Igor's Cavern
	BAE_REVERB_TYPE_6,						// Igor's Dungeon
	BAE_REVERB_TYPE_7,						// Small reflections Reverb used for WebTV
	BAE_REVERB_TYPE_8,						// Early reflections (variable verb)
	BAE_REVERB_TYPE_9,						// Basement (variable verb)
	BAE_REVERB_TYPE_10,						// Banquet hall (variable verb)
	BAE_REVERB_TYPE_11,						// Catacombs (variable verb)
	BAE_REVERB_TYPE_COUNT
} BAEReverbType;

// used by the BAEExporter code
typedef enum
{
	BAE_ENCRYPTION_NONE,
	BAE_ENCRYPTION_NORMAL,
	BAE_ENCRYPTION_TYPE_COUNT
} BAEEncryptionType;

typedef enum
{
	BAE_COMPRESSION_NONE,
	BAE_COMPRESSION_LOSSLESS,
	BAE_COMPRESSION_IMA,
	BAE_COMPRESSION_MPEG_32,
	BAE_COMPRESSION_MPEG_40,
	BAE_COMPRESSION_MPEG_48,
	BAE_COMPRESSION_MPEG_56,
	BAE_COMPRESSION_MPEG_64,
	BAE_COMPRESSION_MPEG_80,
	BAE_COMPRESSION_MPEG_96,
	BAE_COMPRESSION_MPEG_112,
	BAE_COMPRESSION_MPEG_128,
	BAE_COMPRESSION_MPEG_160,
	BAE_COMPRESSION_MPEG_192,
	BAE_COMPRESSION_MPEG_224,
	BAE_COMPRESSION_MPEG_256,
	BAE_COMPRESSION_MPEG_320,
	BAE_COMPRESSION_TYPE_COUNT
} BAECompressionType;

typedef char *		BAEPathName;			// this a pointer to a 'C' string
											// ie. "C:\FOLDER\FILE" for WinOS
											// and "MacHD:Folder:File" MacOS

/* Common errors returned from the system */
typedef enum
{
	BAE_NO_ERROR = 0,
	BAE_PARAM_ERR = 10000,
	BAE_MEMORY_ERR,
	BAE_BAD_INSTRUMENT,
	BAE_BAD_MIDI_DATA,
	BAE_ALREADY_PAUSED,
	BAE_ALREADY_RESUMED,
	BAE_DEVICE_UNAVAILABLE,
	BAE_NO_SONG_PLAYING,
	BAE_STILL_PLAYING,
	BAE_TOO_MANY_SONGS_PLAYING,
	BAE_NO_VOLUME,
	BAE_GENERAL_ERR,
	BAE_NOT_SETUP,
	BAE_NO_FREE_VOICES,
	BAE_STREAM_STOP_PLAY,
	BAE_BAD_FILE_TYPE,
	BAE_GENERAL_BAD,
	BAE_BAD_FILE,
	BAE_NOT_REENTERANT,
	BAE_BAD_SAMPLE,
	BAE_BUFFER_TOO_SMALL,
	BAE_BAD_BANK,
	BAE_BAD_SAMPLE_RATE,
	BAE_TOO_MANY_SAMPLES,
	BAE_UNSUPPORTED_FORMAT,
	BAE_FILE_IO_ERROR,
	BAE_SAMPLE_TOO_LARGE,
	BAE_UNSUPPORTED_HARDWARE,
	BAE_ABORTED,
	BAE_FILE_NOT_FOUND,
	BAE_RESOURCE_NOT_FOUND,
	BAE_NULL_OBJECT,
	BAE_ALREADY_EXISTS,
	
	BAE_ERROR_COUNT
} BAEResult;

typedef enum
{
	TITLE_INFO = 0,				// Title
	PERFORMED_BY_INFO,			// Performed by
	COMPOSER_INFO,				// Composer(s)
	COPYRIGHT_INFO,				// Copyright Date
	PUBLISHER_CONTACT_INFO,		// Publisher Contact Info
	USE_OF_LICENSE_INFO,		// Use of License
	LICENSED_TO_URL_INFO,		// Licensed to what URL
	LICENSE_TERM_INFO,			// License term
	EXPIRATION_DATE_INFO,		// Expiration Date
	COMPOSER_NOTES_INFO,		// Composer Notes
	INDEX_NUMBER_INFO,			// Index Number
	GENRE_INFO,					// Genre
	SUB_GENRE_INFO,				// Sub-genre
	TEMPO_DESCRIPTION_INFO,		// Tempo description
	ORIGINAL_SOURCE_INFO,		// Original source

	INFO_TYPE_COUNT				// always count of type InfoType
} BAEInfoType;


typedef enum
{
	BAE_INVALID_TYPE = 0,
	BAE_AIFF_TYPE = 1,
	BAE_WAVE_TYPE,
	BAE_MPEG_TYPE,
	BAE_AU_TYPE,
	BAE_MIDI_TYPE,

	// meta types
	BAE_GROOVOID,
	BAE_RMF,
	BAE_RAW_PCM
} BAEFileType;

// what type of CPU are we running on. 
typedef enum
{
	BAE_CPU_UNKNOWN	= 0,
	BAE_CPU_POWERPC,
	BAE_CPU_SPARC,
	BAE_CPU_JAVA,
	BAE_CPU_MIPS,
	BAE_CPU_INTEL_PENTIUM,
	BAE_CPU_INTEL_PENTIUM3,
	BAE_CPU_CRAY_XMP3,
	
	BAE_CPU_COUNT
} BAECPUType;

// All volume levels are a 16.16 fixed value. 1.0 is 0x10000. Use can use this macro
// to convert a floating point number to a fixed value, and visa versa

typedef long								BAE_FIXED;				// fixed point value can be signed
typedef unsigned long						BAE_UNSIGNED_FIXED;		// fixed point value unsigned 

#define	BAE_FIXED_1							0x10000L
#define FLOAT_TO_FIXED(x)					((BAE_FIXED)((double)(x) * 65536.0))	// the extra long is for signed values
#define FIXED_TO_FLOAT(x)					((double)(x) / 65536.0)
#define LONG_TO_FIXED(x)					((BAE_FIXED)(x) * BAE_FIXED_1)
#define FIXED_TO_LONG(x)					((x) / BAE_FIXED_1)
#define	FIXED_TO_SHORT(x)					((short)((x) / BAE_FIXED_1))

#define FLOAT_TO_UNSIGNED_FIXED(x)			((BAE_UNSIGNED_FIXED)((double)(x) * 65536.0))	// the extra long is for signed values
#define UNSIGNED_FIXED_TO_FLOAT(x)			((double)(x) / 65536.0)
#define LONG_TO_UNSIGNED_FIXED(x)			((BAE_UNSIGNED_FIXED)(x) * BAE_FIXED_1)
#define UNSIGNED_FIXED_TO_LONG(x)			((x) / BAE_FIXED_1)
#define	UNSIGNED_FIXED_TO_SHORT(x)			((unsigned short)((x) / BAE_FIXED_1))

#define RATIO_TO_FIXED(a,b)					(LONG_TO_FIXED(a) / (b))
#define FIXED_TO_LONG_ROUNDED(x)			FIXED_TO_LONG((x) + BAE_FIXED_1 / 2)
#define FIXED_TO_SHORT_ROUNDED(x)			FIXED_TO_SHORT((x) + BAE_FIXED_1 / 2)

#define UNSIGNED_RATIO_TO_FIXED(a,b)		(LONG_TO_UNSIGNED_FIXED(a) / (b))
#define UNSIGNED_FIXED_TO_LONG_ROUNDED(x)	UNSIGNED_FIXED_TO_LONG((x) + BAE_FIXED_1 / 2)
#define UNSIGNED_FIXED_TO_SHORT_ROUNDED(x)	UNSIGNED_FIXED_TO_SHORT((x) + BAE_FIXED_1 / 2)


typedef char			BAE_BOOL;
typedef unsigned long	BAE_INSTRUMENT;			// reference to an instrument
typedef long			BAE_EVENT_REFERENCE;	// reference to an idle time event

#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define	FALSE	0
#endif

#undef NULL
#define	NULL	0L


// General defines
enum 
{
	BAE_MAX_VOICES				=	64,		// total number of voices. This is shared amongst
											// all BAESound's, BAESoundStream's, and BAEMidiSong's
	BAE_MAX_INSTRUMENTS 		=	768,	// 3 banks, pitched and perc
	BAE_MAX_SONGS				=	2,
	BAE_MAX_MIDI_VOLUME			=	127,
	BAE_MAX_MIDI_TRACKS			=	65,		// 64 midi tracks, plus 1 tempo track
	BAE_MAX_MIDI_CHANNELS		=	16,

	BAE_DEFAULT_PROGRAM			=	0,
	BAE_DEFAULT_BANK			=	0,
	
	BAE_PERCUSSION_CHANNEL		=	9,		// MIDI percussion channel, zero based

	BAE_MIN_STREAM_BUFFER_SIZE	=	30000,

	BAE_FULL_LEFT_PAN			=	(-63),
	BAE_CENTER_PAN				=	0,
	BAE_FULL_RIGHT_PAN			=	(63)
};

// BAE mixer version number. If you call BAEOutputMixer::GetMixerVersion, the values returned should match
// these constants. If they don't then your header files and libraries don't match.

enum
{
	BAE_VERSION_MAJOR		=	1,
	BAE_VERSION_MINOR		=	6,
	BAE_VERSION_SUB_MINOR	=	0
};


enum
{
	BAE_RMF_VERSION_MAJOR		=	2,
	BAE_RMF_VERSION_MINOR		=	0,
	BAE_RMF_VERSION_SUB_MINOR	=	0
};


typedef enum {
	BAE_UNKNOWN = 0,			// Voice is an undefined type
	BAE_MIDI_PCM_VOICE,			// Voice is a PCM voice used by MIDI
	BAE_SOUND_PCM_VOICE			// Voice is a PCM sound effect used by BAESound/BAESoundStream
} BAEVoiceType;


struct BAEAudioInfo
{
	short int		voicesActive;						// number of voices active
	short int		voice[BAE_MAX_VOICES];				// voice index
	BAEVoiceType	voiceType[BAE_MAX_VOICES];			// voice type
	long			instrument[BAE_MAX_VOICES];			// current instruments
	short int		midiVolume[BAE_MAX_VOICES];			// current volumes
	short int		scaledVolume[BAE_MAX_VOICES];		// current scaled volumes
	short int		channel[BAE_MAX_VOICES];			// current channel
	short int		midiNote[BAE_MAX_VOICES];			// current midi note
	long			userReference[BAE_MAX_VOICES];		// userReference associated with voice
};
typedef struct BAEAudioInfo BAEAudioInfo;



struct BAESampleInfo
{
	unsigned short		bitSize;			// number of bits per sample
	unsigned short		channels;			// number of channels (1 or 2)
	unsigned short		baseMidiPitch;		// base Midi pitch of recorded sample ie. 60 is middle 'C'
	unsigned long		waveSize;			// total waveform size in bytes
	unsigned long		waveFrames;			// number of frames
	unsigned long		startLoop;			// start loop point offset
	unsigned long		endLoop;			// end loop point offset
	BAE_UNSIGNED_FIXED	sampledRate;		// fixed 16.16 value for recording
};
typedef struct BAESampleInfo BAESampleInfo;



typedef struct sBAESong		*BAESong;
typedef struct sBAEMixer	*BAEMixer;
typedef struct sBAESound	*BAESound;
typedef struct sBAEStream	*BAEStream;

typedef void *				 BAEBankToken;

typedef void		(*BAE_AudioTaskCallbackPtr)(void *reference);
typedef void		(*BAE_SoundCallbackPtr)(void *reference);
typedef void		(*BAE_SongCallbackPtr)(void *reference);

// sequencer callbacks
typedef void		(*BAE_SongControllerCallbackPtr)(BAESong pSong, void* reference, short int channel, short int track, short int controler, short int value);
		
/** Standard Midi constants.
 */
enum
{
	
	/** MIDI status commands most significant bit is 1 */
	NOTE_OFF				=   0x80,
	NOTE_ON					=   0x90,
	POLY_AFTERTOUCH			=   0xA0,
	CONTROL_CHANGE			=   0xB0,
	PROGRAM_CHANGE			=   0xC0,
	CHANNEL_AFTERTOUCH		=   0xD0,
	PITCH_BEND				=   0xE0,
	SYSTEM_EXCLUSIVE		=   0xF0,
	SYSTEM_EXCLUSIVE_CONT	=   0xF7,
	
	/** Controller values. Obtained from:
	 http://www.midi.org/about-midi/table3.shtml
	 */
	BANK_MSB				=	0,
	MODULATION_MSB			=	1,
	DATA_MSB				=	6,
	VOLUME_MSB				=	7,
	BALANCE_MSB				=	8,
	PAN_MSB					=	10,
	EXPRESSION_MSB			=	11,
	
	BANK_LSB				=	32,
	MODULATION_LSB			=	33,
	DATA_LSB				=	38,
	VOLUME_LSB				=	39,
	BALANCE_LSB				=	40,
	PAN_LSB					=	42,
	EXPRESSION_LSB			=	43,
	
	SUSTAIN					=	64,
	SOFT_PEDAL				=	67,
	
	REVERB_TYPE				=	90, // non-standard
	REVERB_SEND				=	91,
	TREMOLO_LEVEL			=	92,
	CHROUS_SEND_LEVEL		=	93,
	DETUNE_DEPTH			=	94,
	PHASER_DEPTH			=	95,
	
	INCREMENT_DATA			=	96,
	DECREMENT_DATA			=	97,
	NRPN_LSB				=	98,
	NRPN_MSB				=	99,
	RPN_LSB					=	100,
	RPN_MSB					=	101,
	
	ALL_NOTES_OFF_CHANNEL	=	120,
	RESET_ALL_CONTROLLERS	=	121,
	ALL_NOTES_OFF			=	123,
	
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
	RPN_PITCH_BEND_SENSITIVITY_LSB		=	0,
	RPN_PITCH_BEND_SENSITIVITY_MSB		=	0,
	
	
	/** Standard MIDI Files meta event definitions */
	META_EVENT				=   0xFF,
};

// These are embedded text events inside of midi files
typedef enum
{
	SEQUENCE_NUMBER			=   0x00,
	
	GENERIC_TEXT_TYPE		=	0x01,	// generic text
	COPYRIGHT_TYPE			=	0x02,	// copyright text
	TRACK_NAME_TYPE			=	0x03,	// track name of sequence text
	INSTRUMENT_NAME			=   0x04,
	LYRIC_TYPE				=	0x05,	// lyric text
	MARKER_TYPE	 			=	0x06,	// marker text (BAE supports LOOPSTART, LOOPEND, LOOPSTART= commands)
	CUE_POINT_TYPE			=	0x07,	// cue point text

	PROGRAM_NAME			=	0x08,
	DEVICE_NAME				=	0x09,
	
	CHANNEL_PREFIX			=   0x20,
	END_OF_TRACK			=   0x2F,
	SET_TEMPO				=   0x51,
	SMPTE_OFFSET			=   0x54,
	TIME_SIGNATURE			=   0x58,
	KEY_SIGNATURE			=   0x59,
	SEQUENCER_SPECIFIC		=   0x74,

} BAEMetaType;

		

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// Mixer class
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------


// BAEMixer_New
// ------------------------------------
// 'Create' a new BAEMixer structure. Actually returns a pointer to the global
// single BAEMixer structure.  Can only be one mixer object per sound card.
//
BAEMixer			BAEMixer_New(void);


// BAEMixer_Delete()
// ------------------------------------
// Deactivates the indicated BAEMixer, effectively deleting it.
//
BAEResult			BAEMixer_Delete(BAEMixer mixer);

// mixer callbacks and tasks
BAEResult			BAEMixer_SetAudioTask(BAEMixer mixer, BAE_AudioTaskCallbackPtr pTaskProc, void *taskReference);
BAEResult			BAEMixer_GetAudioTask(BAEMixer mixer, BAE_AudioTaskCallbackPtr *pResult);


// BAEMixer_GetMemoryUsed()
// --------------------------------------
// Calculates amount of memory used by this mixer in bytes. 
//
BAEResult			BAEMixer_GetMemoryUsed(BAEMixer mixer, unsigned long *pOutResult);

// BAEMixer_GetMixerVersion()
// ------------------------------------
// Upon return, parameters pVersionMajor, pVersionMinor, and pVersionSubMinor will
// point to short ints indicating the BAE version number for the indicated
// BAEMixer.
//
BAEResult			BAEMixer_GetMixerVersion(BAEMixer mixer, 
							short int *pVersionMajor, 
							short int *pVersionMinor, 
							short int *pVersionSubMinor);


// get and set the fade time. Will be used for all song/sound fades
BAEResult			BAEMixer_SetFadeRate(BAEMixer mixer, BAE_UNSIGNED_FIXED rate);
BAEResult			BAEMixer_GetFadeRate(BAEMixer mixer, BAE_UNSIGNED_FIXED *outFadeRate);

// BAEMixer_GetMaxDeviceCount()
// ------------------------------------
// Upon return, parameter outMaxDeviceCount will point to a long indicating the
// maximum number of audio output devices to which the indicated BAEMixer is able
// to send its output.  On platforms not supporting muliple devices, this will be
// 0.
//
BAEResult			BAEMixer_GetMaxDeviceCount(BAEMixer mixer,
							long *outMaxDeviceCount);


// BAEMixer_SetCurrentDevice()
// ------------------------------------
// Causes the indicated BAEMixer to begin sending its audio output to the
// indicated device, with any optional device-specific parameters as pointed to by
// deviceParameter.  On platforms not supporting multiple devices, this call has
// no effect.
//
BAEResult			BAEMixer_SetCurrentDevice(BAEMixer mixer, 
							long deviceID,
							void *deviceParameter);


// BAEMixer_GetCurrentDevice()
// ------------------------------------
// Upon return, parameter outDeviceID will point to a long containing the device
// ID of the audio output device to which the indicated BAEMixer is currently
// sending its audio output; any optional device-specific parameters being used
// for that device will be pointed to by deviceParameter.
//
BAEResult			BAEMixer_GetCurrentDevice(BAEMixer mixer, 
							void *deviceParameter, 
							long *outDeviceID);


// BAEMixer_GetDeviceName()
// ------------------------------------
// Upon return, parameter cName will point to a character string containing the
// name of the audio output device specified by device ID number deviceID for the
// indicated BAEMixer.  Provide the maximum string length in bytes, including the
// terminating NULL, in cNameLength.  On platforms not supporting multiple
// devices, this call has no effect.
//
BAEResult			BAEMixer_GetDeviceName(BAEMixer mixer, 
							long deviceID, 
							char *cName, 
							unsigned long cNameLength);

		
// BAEMixer_SetDefaultReverb()
// BAEMixer_GetDefaultReverb()
// --------------------------------------
// Sets/Gets the master default reverb
//
BAEResult			BAEMixer_SetDefaultReverb(BAEMixer mixer, BAEReverbType verb);
BAEResult			BAEMixer_GetDefaultReverb(BAEMixer mixer, BAEReverbType *pOutResult);


// BAEMixer_IsOpen()
// ------------------------------------
// Upon return, parameter outIsOpen will point to a BAE_BOOL indicating whether
// the indicated BAEMixer is currently open (TRUE) or closed (FALSE).
//
BAEResult			BAEMixer_IsOpen(BAEMixer mixer,
							BAE_BOOL *outIsOpen);


// BAEMixer_Is16BitSupported()
// ------------------------------------
// Upon return, parameter outIsSupported will point to a BAE_BOOL indicating
// whether the indicated BAEMixer supports 16-bit audio output (TRUE) or not
// (FALSE).
//
BAEResult			BAEMixer_Is16BitSupported(BAEMixer mixer,
							BAE_BOOL *outIsSupported);


// BAEMixer_Is8BitSupported()
// ------------------------------------
// Upon return, parameter outIsSupported will point to a BAE_BOOL indicating
// whether the indicated BAEMixer supports 8-bit audio output (TRUE) or not
// (FALSE).
//
BAEResult			BAEMixer_Is8BitSupported(BAEMixer mixer,
							BAE_BOOL *outIsSupported);


// BAEMixer_Open()
// ------------------------------------
// Initializes the indicated BAEMixer in preparation for all sound generation, and
// sets several operating modes. You must call BAEMixer_New and BAEMixer_Open
// before calling any other BAE functions.
// ------------------------------------
// Note: If the platform is not capable of providing the requested level of
// service, BAE will fall back to a lower level during the BAEMixer_Open() call.
// ------------------------------------
// Parameters:
//           mixer          -- The BAEMixer
//           rate           -- Sample rate of mixer. See BAERate for supported rate.
//           t              -- Interpolation mode (see BAETerpMode)
//           am             -- Miscellaneous modes (see BAEAudioModifiers)
//           maxSongVoices  -- Maximum number of rendered notes
//                             playing at once.
//           maxSoundVoices -- Maximum number of Sound objects
//                             playing at once
//           mix level      -- Total number of full-scale
//                             voices before distortion
//                             (Song notes plus Sound objects)
//           engageAudio    -- Whether to send mixer audio
//                             output to the host device
//                         
// ------------------------------------
// BAEResult codes:
//           BAE_PARAM_ERR      -- Bad parameters
//           BAE_NOT_REENTERANT -- Attempt to re-enter BAE
//           BAE_GENERAL_BAD    -- Header file and built code versions 
//                                 don't match
// ------------------------------------
BAEResult			BAEMixer_Open(BAEMixer mixer,
							BAERate r,
							BAETerpMode t,
							BAEAudioModifiers am,
							short int maxMidiVoices,
							short int maxSoundVoices,
							short int mixLevel,
							BAE_BOOL engageAudio);


// BAEMixer_Close()
// ------------------------------------
// Causes the indicated BAEMixer to stop functioning, delete its data, and free
// its memory.
//
BAEResult			BAEMixer_Close(BAEMixer mixer);


// BAEMixer_IsAudioEngaged()
// ------------------------------------
// Upon return, parameter outIsEngaged will point to a BAE_BOOL indicating whether
// the indicated BAEMixer is currently engaged (TRUE) or not (FALSE).
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- Indicated mixer not initialized
// ------------------------------------
BAEResult			BAEMixer_IsAudioEngaged(BAEMixer mixer,
							BAE_BOOL *outIsEngaged);


// BAEMixer_DisengageAudio()
// ------------------------------------
// Causes the indicated BAEMixer to temporarily suspend audio output to the host. 
// This allows for cooperative sharing of the output services with any other sound
// generating entities.  All BAE processing continues to operate in real time
// while disengaged. Use BAEMixer_ReengageAudio() to resume audio output.
//
BAEResult			BAEMixer_DisengageAudio(BAEMixer mixer);


// BAEMixer_ReengageAudio()
// ------------------------------------
// Resumes audio output from the indicated BAEMixer to the host, following a
// BAEMixer_DisengageAudio().
//
BAEResult			BAEMixer_ReengageAudio(BAEMixer mixer);

// mute/unmute all audio playback. These are reference counted
BAEResult			BAEMixer_Mute(BAEMixer mixer);
BAEResult			BAEMixer_Unmute(BAEMixer mixer);
BAE_BOOL			BAEMixer_IsMuted(BAEMixer mixer);

// start/stop a 1khz tone
BAEResult			BAEMixer_TestTone(BAE_BOOL status);
BAEResult			BAEMixer_TestToneFrequency(BAE_UNSIGNED_FIXED freq);


// BAEMixer_Idle()
// ------------------------------------
// Called during idle times to process audio, or other events. Optional
// requiment if threads are available.
//
BAEResult			BAEMixer_Idle(BAEMixer mixer);

// BAEMixer_IsAudioActive()
// ------------------------------------
// Checks active voices and looks at a sub sample of the audio output to
// determine if there's any audio still playing
//
BAEResult			BAEMixer_IsAudioActive(BAEMixer mixer, 
							BAE_BOOL *outIsActive);


// BAEMixer_AddBankFromFile()
// ------------------------------------
// Causes the indicated BAEMixer to load and begin using the instrument bank file
// at path pAudioPathName for note rendering, in addition to any existing
// instrument bank.  This new bank will be searched first for instruments, followed
// by any banks loaded prior. outToken is the reference ID for the bank.
// ------------------------------------
// BAEResult codes:
//           BAE_BAD_FILE  -- Bad file or path spec
// ------------------------------------
BAEResult 			BAEMixer_AddBankFromFile(BAEMixer mixer,
							BAEPathName pAudioPathName,
							BAEBankToken *outToken);


// BAEMixer_AddBankFromMemory()
// ------------------------------------
// Causes the indicated BAEMixer to begin using the instrument bank resource at
// address pAudioFile for note rendering, in addition to any existing instrument
// bank.  Parameter fileSize must indicate the length in bytes of the instrument
// bank resource.  This new bank will be searched first for instruments, followed
// by any banks loaded prior. outToken is the reference ID for the bank.
// ------------------------------------
// BAEResult codes:
//           BAE_BAD_FILE  -- Bad instrument bank resource
// ------------------------------------
BAEResult			BAEMixer_AddBankFromMemory(BAEMixer mixer,
							void * pAudioFile,
							unsigned long fileSize,
							BAEBankToken *outToken);


// BAEMixer_UnloadBank()
// ------------------------------------
// Causes the indicated BAEMixer to close the instrument bank indicated by 'token'.
//
BAEResult 			BAEMixer_UnloadBank(BAEMixer mixer,
							BAEBankToken token);


// BAEMixer_UnloadBanks()
// ------------------------------------
// Causes the indicated BAEMixer to close the all open instrument banks.
//
BAEResult			BAEMixer_UnloadBanks(BAEMixer mixer);


// BAEMixer_BringBankToFront()
// ------------------------------------
// Causes the instrument bank indicated by 'token' to be first in the
// search path
//
BAEResult			BAEMixer_BringBankToFront(BAEMixer mixer,
							BAEBankToken token);


// BAEMixer_SendBankToBack()
// ------------------------------------
// Causes the instrument bank indicated by 'token' to be last in the
// search path
//
BAEResult			BAEMixer_SendBankToBack(BAEMixer mixer,
							BAEBankToken token);


// BAEMixer_GetBankVersion()
// ------------------------------------
// Note: Known as BAEMixer_GetVersionFromAudioFile() in the full versions of BAE
// ------------------------------------
// Upon return, parameters pVersionMajor, pVersionMinor, and pVersionSubMinor will
// point to the version number of the instrument bank indicated by 'token'.
//
BAEResult			BAEMixer_GetBankVersion(BAEMixer mixer,
							BAEBankToken token,
							short int *pVersionMajor,
							short int *pVersionMinor,
							short int *pVersionSubMinor);


// BAEMixer_GetGroovoidNameFromBank()
// ------------------------------------
// Note: Known as GetSongNameFromAudioFile() in the full versions of BAE
// ------------------------------------
// Upon return, parameter cSongName will point to the name of Groovoid number
// (index), by searching through all open instrument banks.  It will return
// the name of the first instance it finds, in the case that multiple open
// banks have duplicated Groovoid numbers.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- The indicated mixer has not not been initialized
// ------------------------------------
BAEResult			BAEMixer_GetGroovoidNameFromBank(BAEMixer mixer,
							long index,
							char *cSongName);


// BAEMixer_ChangeAudioModes()
// ------------------------------------
// Changes the operating modes of the indicated BAEMixer to the indicated
// BAERate, BAETerpMode, and BAEAudioModifiers.
// ------------------------------------
// BAEResult codes:
//           BAE_PARAM_ERR -- Bad parameters
// ------------------------------------
BAEResult			BAEMixer_ChangeAudioModes(BAEMixer mixer,
							BAERate r,
							BAETerpMode t,
							BAEAudioModifiers am);


// BAEMixer_GetRate()
// ------------------------------------
// Upon return, parameter outQuality will point to a BAERate containing the
// indicated BAEMixer's current quality mode (combination of sample rate and
// interpolation mode). See BAERate for interpretation.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- Indicated mixer not initialized
// ------------------------------------
BAEResult			BAEMixer_GetRate(BAEMixer mixer,
							BAERate *outRate);


// BAEMixer_GetTerpMode()
// ------------------------------------
// Upon return, parameter outTerpMode will point to a BAETerpMode containing the
// indicated BAEMixer's current interpolation mode. See BAETerpMode for
// interpretation.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- Indicated mixer not initialized
// ------------------------------------
BAEResult 			BAEMixer_GetTerpMode(BAEMixer mixer,
							BAETerpMode *outTerpMode);


// BAEMixer_GetModifiers()
// ------------------------------------
// Upon return, parameter outMods will point to a BAEAudioModifiers containing the
// indicated BAEMixer's current 'modifiers' flags, which control various
// system-wide BAE operating modes. See BAEAudioModifiers for interpretation.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- Indicated mixer not initialized
// ------------------------------------
BAEResult			BAEMixer_GetModifiers(BAEMixer mixer,
							BAEAudioModifiers *outMods);


// BAEMixer_ChangeSystemVoices()
// ------------------------------------
// Changes the maximum number of note rendering voices (maxSongVoices), maximum
// number of digital audio voices (maxSoundVoices), and maximum number of
// full-scale voices before clipping (mixLevel) for the indicated BAEMixer.
//
BAEResult			BAEMixer_ChangeSystemVoices(BAEMixer mixer,
							short int maxMidiVoices,
							short int maxSoundVoices,
							short int mixLevel);


// BAEMixer_GetMidiVoices()
// ------------------------------------
// Upon return, parameter outNumMidiVoices will point to a short int containing
// the indicated BAEMixer's current maximum number of voices available for
// MIDI/RMF note rendering.
//
BAEResult			BAEMixer_GetMidiVoices(BAEMixer mixer,
							short int *outNumMidiVoices);


// BAEMixer_GetSoundVoices()
// ------------------------------------
// Upon return, parameter outNumSoundVoices will point to a short int containing
// the indicated BAEMixer's current maximum number of voices available for Sound
// objects (samples).
//
BAEResult			BAEMixer_GetSoundVoices(BAEMixer mixer,
							short int *outNumSoundVoices);


// BAEMixer_GetMixLevel()
// ------------------------------------
// Upon return, parameter outMixLevel will point to a short int containing the
// indicated BAEMixer's current maximum number of simultaneous full-scale voices
// before distortion (combined Song and Sound voices).
//
BAEResult			BAEMixer_GetMixLevel(BAEMixer mixer,
							short int *outMixLevel);


// BAEMixer_GetTick()
// ------------------------------------
// Upon return, parameter outTick will point to the indicated BAEMixer's current
// time, expressed in microseconds elapsed since initialization.
//
BAEResult 			BAEMixer_GetTick(BAEMixer mixer,
							unsigned long *outTick);


// BAEMixer_SetAudioLatency()
// ------------------------------------
// Reconfigures the current BAE output device buffers to achieve the requested
// audio output latency, if possible.  Latency is expressed in integer
// milliseconds (1000 = 1 second).
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- Function not available on this platform.
// ------------------------------------
BAEResult			BAEMixer_SetAudioLatency(BAEMixer mixer,
							unsigned long requestedLatency);


// BAEMixer_GetAudioLatency()
// ------------------------------------
// Upon return, parameter outLatency will point to the current BAE audio output
// latency for the indicated BAEMixer, expressed in milliseconds (1000 = 1
// second).
//
BAEResult 			BAEMixer_GetAudioLatency(BAEMixer mixer,
							unsigned long *outLatency);


BAEResult BAEMixer_SetRouteBus(BAEMixer mixer, int routeBus);

// BAEMixer_SetMasterVolume()
// ------------------------------------
// Sets the master volume of the indicated BAEMixer to the indicated volume.
//
BAEResult			BAEMixer_SetMasterVolume(BAEMixer mixer,
							BAE_UNSIGNED_FIXED theVolume);


// BAEMixer_GetMasterVolume()
// ------------------------------------
// Upon return, parameter outVolume will point to the current master volume
// of the indicated BAEMixer.
//
BAEResult			BAEMixer_GetMasterVolume(BAEMixer mixer,
							BAE_UNSIGNED_FIXED *outVolume);


// BAEMixer_SetHardwareVolume()
// ------------------------------------
// Sets the hardware-based final output volume of the audio output device
// currently being used by the indicated BAEMixer to the indicated volume, if
// available on this platform.
//
BAEResult			BAEMixer_SetHardwareVolume(BAEMixer mixer,
							BAE_UNSIGNED_FIXED theVolume);


// BAEMixer_GetHardwareVolume()
// ------------------------------------
// Upon return, parameter outVolume will point to the hardware-based final output
// volume of the audio output device currently being used by the indicated
// BAEMixer, if available on this platform.
//
BAEResult		 	BAEMixer_GetHardwareVolume(BAEMixer mixer,
							BAE_UNSIGNED_FIXED *outVolume);


// BAEMixer_SetMasterSoundEffectsVolume()
// -----------------------------------------
// Sets the shared master volume for all Sound objects played by the indicated
// BAEMixer to the indicated volume.
//
BAEResult			BAEMixer_SetMasterSoundEffectsVolume(BAEMixer mixer,
							BAE_UNSIGNED_FIXED theVolume);



// BAEMixer_GetMasterSoundEffectsVolume()
// ------------------------------------
// Upon return, parameter outVolume will point to the shared master volume for all
// Sound objects played by the indicated BAEMixer.
//
BAEResult			BAEMixer_GetMasterSoundEffectsVolume(BAEMixer mixer,
							BAE_UNSIGNED_FIXED *outVolume);


// BAEMixer_GetAudioSampleFrame()
// ------------------------------------
// Upon return, parameters pLeft and pRight will point to the indicated BAEMixer's
// current left and right master audio output sample buffers, and parameter
// outFrame will point at a short integer containing the BAEMixer's current write
// index (as used in writing to the left and right buffers).
//
BAEResult			BAEMixer_GetAudioSampleFrame(BAEMixer mixer,
							short int *pLeft,
							short int *pRight,
							short int *outFrame);


// BAEMixer_GetRealtimeStatus()
// ------------------------------------
// Upon return, parameter pStatus will point to a BAEAudioStatus struct containing
// the indicated BAEMixer's current status variables (see struct BAEAudioStatus
// for fields).
//
BAEResult			BAEMixer_GetRealtimeStatus(BAEMixer mixer,
							BAEAudioInfo *pStatus);


// BAEMixer_GetCPULoadInMicroseconds()
// ------------------------------------
// Upon return, parameter outLoad will point to an unsigned long containing an
// estimate of the number of microseconds the indicated BAEMixer is taking to
// generate each audio output buffer.
//
BAEResult 			BAEMixer_GetCPULoadInMicroseconds(BAEMixer mixer,
							unsigned long *outLoad);


// BAEMixer_GetCPULoadInPercent()
// ------------------------------------
// Upon return, parameter outLoad will point to an unsigned long containing an
// integer in the range 0-100 reporting what percentage of the available
// procerssor time the indicated BAEMixer is using.
//
BAEResult			BAEMixer_GetCPULoadInPercent(BAEMixer mixer,
							unsigned long *outLoad);



// start saving audio output to a file
BAEResult 			BAEMixer_StartOutputToFile(BAEMixer mixer,
									  BAEPathName pAudioOutputFile, 
									  BAEFileType outputType,
									  BAECompressionType compressionType);

// Stop saving audio output to a file
void				BAEMixer_StopOutputToFile(void);

// once started saving to a file, call this to continue saving to file
BAEResult			BAEMixer_ServiceAudioOutputToFile(BAEMixer mixer);



// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// BAEStream:  Sound effects, linear audio files, streamed
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
typedef void		(*BAE_AudioStreamCallbackPtr)(BAEStream stream, unsigned long reference);

BAEStream			BAEStream_New(BAEMixer mixer);

// BAEStream_Delete()
// ------------------------------------
// Deactivates the indicated BAEStream, unloads its sample media data, and frees
// its memory.  Call this when done with the BAEStream object.
//
BAEResult			BAEStream_Delete(BAEStream stream);

BAEResult			BAEStream_Unload(BAEStream stream);

BAEResult			BAEMixer_ServiceStreams(BAEMixer theMixer);

// BAEStream_GetMemoryUsed()
// --------------------------------------
// Returns total number of bytes used by this object.
//
BAEResult			BAEStream_GetMemoryUsed(BAEStream stream, unsigned long *pOutResult);

// BAEStream_SetMixer()
// BAEResult BAEStream_SetMixer(BAEStream stream, BAEMixer mixer);
// ------------------------------------
// Associates the indicated BAEStream with the indicated BAEMixer, replacing the
// previously associated BAEMixer.
//
BAEResult			BAEStream_SetMixer(BAEStream stream,
							BAEMixer mixer);


// BAEStream_GetMixer()
// ------------------------------------
// Upon return, the BAEMixer pointed at by parameter outMixer will contain the
// address of the BAEMIxer with which the indicated BAEStream is associated.
//
BAEResult			BAEStream_GetMixer(BAEStream stream,
							BAEMixer *outMixer);


// BAEStream_SetVolume()
// --------------------------------------
// Sets the playback volume of the indicated BAEStream object to the indicated
// level.  Normal volume is 1.0.
//
BAEResult			BAEStream_SetVolume(BAEStream stream,
							BAE_UNSIGNED_FIXED newVolume);


// BAEStream_GetVolume()
// --------------------------------------
// Upon return, the BAE_UNSIGNED_FIXED pointed to by parameter outVolume will hold
// a copy of the indicated BAEStream's current playback volume.
//
BAEResult			BAEStream_GetVolume(BAEStream stream,
							BAE_UNSIGNED_FIXED *outVolume);

// pass TRUE to entire loop stream, FALSE to not loop
BAEResult			BAEStream_SetLoopFlag(BAEStream stream, BAE_BOOL loop);
BAEResult			BAEStream_GetLoopFlag(BAEStream stream, BAE_BOOL *outLoop);

// BAEStream_GetInfo()
// --------------------------------------
// Upon return, the BAESampleInfo pointed to by parameter outInfo will contain a
// copy of the current sample playback property set of the indicated BAEStream
// object.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- No stream loaded
// ------------------------------------
BAEResult			BAEStream_GetInfo(BAEStream stream,
							BAESampleInfo *outInfo);

// BAEStream_SetupFile()
// --------------------------------------
// prepare to play a formatted file as a stream.
BAEResult			BAEStream_SetupFile(BAEStream stream, BAEPathName cFileName, 
							BAEFileType fileType,
							unsigned long bufferSize,		// temp buffer to read file
							BAE_BOOL loopFile);				// TRUE will loop file

BAEResult			BAEStream_SetCallback(BAEStream stream, BAE_AudioStreamCallbackPtr callback, unsigned long reference);

// BAEStream_Preroll()
// --------------------------------------
// Prepares the indicated BAEStream for later instant playback by performing any and
// all lengthy resource setup operations.
// ------------------------------------
// BAEResult codes:
//          BAE_NO_FREE_VOICES -- Couldn't allocate a voice at this priority
//			BAE_NOT_SETUP -- must call BAEStream_SetupFile
BAEResult		BAEStream_Preroll(BAEStream stream);

// BAEStream_Start()
// --------------------------------------
// Causes playback of the indicated BAEStream to begin.
// If no voices are available at the indicated priority
// level, this function fails and returns BAE_NO_FREE_VOICES.
// ------------------------------------
// BAEResult codes:
//          BAE_NO_FREE_VOICES -- Couldn't allocate a voice at this priority
//			BAE_NOT_SETUP -- must call BAEStream_SetupFile
// ------------------------------------
BAEResult			BAEStream_Start(BAEStream stream);

// BAEStream_Stop()
// --------------------------------------
// Stops playback of the indicated BAEStream in one of two ways, depending upon the
// value of the startFade parameter: either stop immediately (FALSE), or stop
// after smoothly fading the stream out over a period of about 2.2 seconds (TRUE).
// ------------------------------------
// Note: Returns immediately, not at the end of the fade-out period.
//
BAEResult			BAEStream_Stop(BAEStream stream,
							BAE_BOOL startFade);


// BAEStream_Pause()
// ------------------------------------
// Pauses playback of the indicated BAEStream.  If already paused, this function
// will have no effect. To resume playback, call BAEStream_Resume() or
// BAEStream_Start().
//
BAEResult			BAEStream_Pause(BAEStream stream);


// BAEStream_Resume()
// --------------------------------------
// If the indicated BAEStream is paused at the time of this call, causes playback
// to resume from the point at which it was most recently paused. If not paused,
// this function will have no effect. Another way to resume playback after a
// pause is to call BAEStream_Start().
//
BAEResult			BAEStream_Resume(BAEStream stream);


// BAEStream_IsPaused()
// ------------------------------------
// Upon return, parameter outIsPaused will point to a BAE_BOOL indicating whether
// the indicated BAEStream is currently in a paused state (TRUE) or not (FALSE).
//
BAEResult			BAEStream_IsPaused(BAEStream stream,
							BAE_BOOL *outIsPaused);


// BAEStream_Fade()
// --------------------------------------
// Fades the volume of the indicated BAEStream smoothly from sourceVolume to
// destVolume, over a period of timeInMilliseconds.  Note that this may be either
// a fade up or a fade down.
//
BAEResult			BAEStream_Fade(BAEStream stream,
							BAE_FIXED sourceVolume,
							BAE_FIXED destVolume,
							BAE_FIXED timeInMiliseconds);


// BAEStream_IsDone()
// --------------------------------------
// Upon return, the BAE_BOOL pointed at by parameter outIsDone will indicate
// whether the indicated BAEStream object has (TRUE) or has not (FALSE) played all
// the way to its end and stopped on its own.
//
BAEResult			BAEStream_IsDone(BAEStream stream,
							BAE_BOOL *outIsDone);


// BAEStream_SetRate()
// --------------------------------------
// Sets the playback sample rate of the indicated BAEStream object to the indicated
// rate, in Hertz.
//
BAEResult			BAEStream_SetRate(BAEStream stream,
							BAE_UNSIGNED_FIXED newRate);


// BAEStream_GetRate()
// --------------------------------------
// Upon return, the BAE_UNSIGNED_FIXED pointed to by parameter outRate will hold a
// copy of the indicated BAEStream's current sample rate, in Hertz.
//
BAEResult			BAEStream_GetRate(BAEStream stream,
							BAE_UNSIGNED_FIXED *outRate);


// BAEStream_SetLowPassAmountFilter()
// --------------------------------------
// Sets the depth of the lowpass filter effect for the indicated
// BAEStream object.
//
BAEResult			BAEStream_SetLowPassAmountFilter(BAEStream stream,
							short int lowPassAmount);


// BAEStream_GetLowPassAmountFilter()
// --------------------------------------
// Upon return, the short int pointed to by parameter outLowPassAmount will hold a
// copy of the indicated BAEStream object's current lowpass filter effect's depth
// setting.
//
BAEResult			BAEStream_GetLowPassAmountFilter(BAEStream stream,
							short int *outLowPassAmount);


// BAEStream_SetResonanceAmountFilter()
// --------------------------------------
// Sets the resonance of the lowpass filter effect for the indicated BAEStream
// object.
//
BAEResult			BAEStream_SetResonanceAmountFilter(BAEStream stream,
							short int resonanceAmount);


// BAEStream_GetResonanceAmountFilter()
// ------------------------------------
// Upon return, the short int pointed to by parameter outResonanceAmount will hold
// a copy of the indicated BAEStream object's current lowpass filter effect's
// resonance setting.
//
BAEResult			BAEStream_GetResonanceAmountFilter(BAEStream stream,
							short int *outResonanceAmount);


// BAEStream_SetFrequencyAmountFilter()
// --------------------------------------
// Sets the frequency of the lowpass filter effect for the indicated BAEStream
// object.
//
BAEResult			BAEStream_SetFrequencyAmountFilter(BAEStream stream,
							short int frequencyAmount);


// BAEStream_GetFrequencyAmountFilter()
// --------------------------------------
// Upon return, the short int pointed to by parameter outFrequencyAmount will hold
// a copy of the indicated BAEStream object's current lowpass filter effect's
// frequency setting.
//
BAEResult			BAEStream_GetFrequencyAmountFilter(BAEStream stream,
							short int *outFrequencyAmount);





// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// BAESound:  Sound effects, linear audio files
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------


// BAESound_New()
// ------------------------------------
// Creates a new BAESound structure and associates it with the indicated BAEMixer.
// Note: You must use BAESound_New() and one of the BAESound "Load" functions
// before you can play a sample with a BAESound object.
//
BAESound			BAESound_New(BAEMixer mixer);


// BAESound_Delete()
// ------------------------------------
// Deactivates the indicated BAESound, unloads its sample media data, and frees
// its memory.  Call this when done with the BAESound object.
//
BAEResult			BAESound_Delete(BAESound sound);

// BAESound_GetMemoryUsed()
// --------------------------------------
// Returns total number of bytes used by this object.
//
BAEResult			BAESound_GetMemoryUsed(BAESound sound, unsigned long *pOutResult);

// BAESound_SetMixer()
// BAEResult BAESound_SetMixer(BAESound sound, BAEMixer mixer);
// ------------------------------------
// Associates the indicated BAESound with the indicated BAEMixer, replacing the
// previously associated BAEMixer.
//
BAEResult			BAESound_SetMixer(BAESound sound,
							BAEMixer mixer);


// BAESound_GetMixer()
// ------------------------------------
// Upon return, the BAEMixer pointed at by parameter outMixer will contain the
// address of the BAEMIxer with which the indicated BAESound is associated.
//
BAEResult			BAESound_GetMixer(BAESound sound,
							BAEMixer *outMixer);

BAEResult			BAESound_SetRouteBus(BAESound sounds, int routeBus);

// BAESound_SetVolume()
// --------------------------------------
// Sets the playback volume of the indicated BAESound object to the indicated
// level.  Normal volume is 1.0.
//
BAEResult			BAESound_SetVolume(BAESound sound,
							BAE_UNSIGNED_FIXED newVolume);


// BAESound_GetVolume()
// --------------------------------------
// Upon return, the BAE_UNSIGNED_FIXED pointed to by parameter outVolume will hold
// a copy of the indicated BAESound's current playback volume.
//
BAEResult			BAESound_GetVolume(BAESound sound,
							BAE_UNSIGNED_FIXED *outVolume);

// sample callbacks
BAEResult			BAESound_SetCallback(BAESound sound, BAE_SoundCallbackPtr pCallback, void *callbackReference);
BAEResult			BAESound_GetCallback(BAESound sound, BAE_SoundCallbackPtr *pResult);


BAEResult			BAESound_SetSoundFrame(BAESound sound, unsigned long startFrameOffset,
											void *sourceSamples, unsigned long sourceFrames);


// BAESound_LoadMemorySample()
// --------------------------------------
// Loads the indicated BAESound with the in-memory sample media data at the
// indicated address and in AIFF, WAV, or AU format (as indicated via parameter
// fileType). Also sets sample playback properties according to the file header.
// The sample data is used in place, not copied; however, if any decompression is
// needed to access the data, memory allocation and decompression will occur
// during this call. 
// ------------------------------------
// Note: On some systems, Mini-BAE does not support this feature.  In
//       those cases, this function has no effect and returns BAE_NOT_SETUP.
// --------------------------------------
// Parameters:
//           sound          -- The BAESound
//           pMemoryFile    -- Address of sample file image to load
//           memoryFileSize -- Size in bytes of sample file image at pMemoryFile
//           fileType       -- File format (see BAEFileType)
// ------------------------------------
// BAEResult codes:
//           BAE_BAD_FILE  	   -- Bad or missing sample data
//			 BAE_BAD_FILE_TYPE -- Unknown file type
// ------------------------------------
BAEResult			BAESound_LoadMemorySample(BAESound sound,
							void *pMemoryFile,
							unsigned long memoryFileSize,
							BAEFileType fileType);


// BAESound_LoadFileSample()
// --------------------------------------
// Loads the indicated BAESound with the sample media data found in the
// indicated AIFF, WAV, or AU file (as indicated via parameter filePath and 
// fileType). Also sets sample playback properties according to the file header.
// --------------------------------------
// Parameters:
//           sound          -- The BAESound
//           filePath		-- path of file to load
//           fileType       -- File format (see BAEFileType)
// ------------------------------------
// BAEResult codes:
//           BAE_MEMORY_ERR 	-- Can't allocate memory for sample copy
//           BAE_BAD_FILE  	   	-- Bad or missing sample data
//			 BAE_BAD_FILE_TYPE	-- Unknown file type
// ------------------------------------
BAEResult			BAESound_LoadFileSample(BAESound sound,
							BAEPathName filePath,
							BAEFileType fileType);

// BAESound_LoadEmptySample()
// --------------------------------------
//
//
BAEResult BAESound_LoadEmptySample(BAESound sound,
							unsigned long frames, 			// number of frames of audio
							unsigned short int bitSize, 	// bits per sample 8 or 16
							unsigned short int channels, 	// mono or stereo 1 or 2
							BAE_UNSIGNED_FIXED rate, 		// 16.16 fixed sample rate
							unsigned long loopStart, 		// loop start in frames
							unsigned long loopEnd);			// loop end in frames

// BAESound_LoadCustomSample()
// --------------------------------------
// Loads the indicated BAESound with a copy of the in-memory raw sample media data
// at the indicated address, and sets several sample properties as per the
// parameters.
// --------------------------------------
// Parameters:
//           sound      -- The BAESound
//           sampleData -- Address of sample data to load
//           frames     -- Number of sample frames of data at sampleData
//           bitSize    -- Depth in bits of sample data (always 8 or 16)
//           channels   -- 1 for mono data, 2 for stereo data.
//           rate       -- Sample rate in Hz, in 16.16 fixed-point format
//           loopStart  -- frame number of first sample in loop
//           loopEnd    -- frame number of last sample in loop
// ------------------------------------
// BAEResult codes:
//           BAE_MEMORY_ERR -- Can't allocate memory for sample copy
// ------------------------------------
BAEResult			BAESound_LoadCustomSample(BAESound sound,
							void * sampleData,
							unsigned long frames,
							unsigned short int bitSize,
							unsigned short int channels,
							BAE_UNSIGNED_FIXED rate,
							unsigned long loopStart,
							unsigned long loopEnd);


// BAESound_Unload()
// ------------------------------------
// Unloads any previously loaded sample media data from the indicated BAESound
// object.
//
BAEResult 			BAESound_Unload(BAESound sound);


// BAESound_GetInfo()
// --------------------------------------
// Upon return, the BAESampleInfo pointed to by parameter outInfo will contain a
// copy of the current sample playback property set of the indicated BAESound
// object.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- No sound data loaded
// ------------------------------------
BAEResult			BAESound_GetInfo(BAESound sound,
							BAESampleInfo *outInfo);

// BAESound_GetRawPCMData()
//
// Use BAESound_GetInfo() to get the size in bytes of the currently loaded sound
// Allocate some memory and pass that point into BAESound_GetRawPCMData() to
// get the currently loaded PCM data
BAEResult			BAESound_GetRawPCMData(BAESound sound, char *outDataPointer, 
															unsigned long outDataSize);


BAEResult			BAESound_SetAutoBuzzFlash(BAESound sound, BAE_BOOL buzzOn, BAE_BOOL flashOn);

// BAESound_Start()
// --------------------------------------
// Causes playback of the indicated BAESound to begin, at the indicated priority
// and volume, and optionally beginning at the indicated sample frame number. 
// Normal volume is 1.0.  If no voices are available at the indicated priority
// level, this function fails and returns BAE_NO_FREE_VOICES.
// ------------------------------------
// BAEResult codes:
//           BAE_NO_FREE_VOICES -- Couldn't allocate a voice at this priority
// ------------------------------------
BAEResult			BAESound_Start(BAESound sound,
							short int priority,
							BAE_UNSIGNED_FIXED sampleVolume,	// sample volume
							unsigned long startOffsetFrame);	// starting offset in frames


// BAESound_Stop()
// --------------------------------------
// Stops playback of the indicated BAESound in one of two ways, depending upon the
// value of the startFade parameter: either stop immediately (FALSE), or stop
// after smoothly fading the sound out over a period of about 2.2 seconds (TRUE).
// ------------------------------------
// Note: Returns immediately, not at the end of the fade-out period.
//
BAEResult			BAESound_Stop(BAESound sound,
							BAE_BOOL startFade);


// BAESound_Pause()
// ------------------------------------
// Pauses playback of the indicated BAESound.  If already paused, this function
// will have no effect. To resume playback, call BAESound_Resume() or
// BAESound_Start().
//
BAEResult			BAESound_Pause(BAESound sound);


// BAESound_Resume()
// --------------------------------------
// If the indicated BAESound is paused at the time of this call, causes playback
// to resume from the point at which it was most recently paused. If not paused,
// this function will have no effect. Another way to resume playback after a
// pause is to call BAESound_Start().
//
BAEResult			BAESound_Resume(BAESound sound);


// BAESound_IsPaused()
// ------------------------------------
// Upon return, parameter outIsPaused will point to a BAE_BOOL indicating whether
// the indicated BAESound is currently in a paused state (TRUE) or not (FALSE).
//
BAEResult			BAESound_IsPaused(BAESound sound,
							BAE_BOOL *outIsPaused);


// BAESound_Fade()
// --------------------------------------
// Fades the volume of the indicated BAESound smoothly from sourceVolume to
// destVolume, over a period of timeInMilliseconds.  Note that this may be either
// a fade up or a fade down.
//
BAEResult			BAESound_Fade(BAESound sound,
							BAE_FIXED sourceVolume,
							BAE_FIXED destVolume,
							BAE_FIXED timeInMiliseconds);


// BAESound_IsDone()
// --------------------------------------
// Upon return, the BAE_BOOL pointed at by parameter outIsDone will indicate
// whether the indicated BAESound object has (TRUE) or has not (FALSE) played all
// the way to its end and stopped on its own.
//
BAEResult			BAESound_IsDone(BAESound sound,
							BAE_BOOL *outIsDone);


// BAESound_SetRate()
// --------------------------------------
// Sets the playback sample rate of the indicated BAESound object to the indicated
// rate, in Hertz.
//
BAEResult			BAESound_SetRate(BAESound sound,
							BAE_UNSIGNED_FIXED newRate);


// BAESound_GetRate()
// --------------------------------------
// Upon return, the BAE_UNSIGNED_FIXED pointed to by parameter outRate will hold a
// copy of the indicated BAESound's current sample rate, in Hertz.
//
BAEResult			BAESound_GetRate(BAESound sound,
							BAE_UNSIGNED_FIXED *outRate);


// BAESound_SetLowPassAmountFilter()
// --------------------------------------
// Sets the depth of the lowpass filter effect for the indicated
// BAESound object.
//
BAEResult			BAESound_SetLowPassAmountFilter(BAESound sound,
							short int lowPassAmount);


// BAESound_GetLowPassAmountFilter()
// --------------------------------------
// Upon return, the short int pointed to by parameter outLowPassAmount will hold a
// copy of the indicated BAESound object's current lowpass filter effect's depth
// setting.
//
BAEResult			BAESound_GetLowPassAmountFilter(BAESound sound,
							short int *outLowPassAmount);


// BAESound_SetResonanceAmountFilter()
// --------------------------------------
// Sets the resonance of the lowpass filter effect for the indicated BAESound
// object.
//
BAEResult			BAESound_SetResonanceAmountFilter(BAESound sound,
							short int resonanceAmount);


// BAESound_GetResonanceAmountFilter()
// ------------------------------------
// Upon return, the short int pointed to by parameter outResonanceAmount will hold
// a copy of the indicated BAESound object's current lowpass filter effect's
// resonance setting.
//
BAEResult			BAESound_GetResonanceAmountFilter(BAESound sound,
							short int *outResonanceAmount);


// BAESound_SetFrequencyAmountFilter()
// --------------------------------------
// Sets the frequency of the lowpass filter effect for the indicated BAESound
// object.
//
BAEResult			BAESound_SetFrequencyAmountFilter(BAESound sound,
							short int frequencyAmount);


// BAESound_GetFrequencyAmountFilter()
// --------------------------------------
// Upon return, the short int pointed to by parameter outFrequencyAmount will hold
// a copy of the indicated BAESound object's current lowpass filter effect's
// frequency setting.
//
BAEResult			BAESound_GetFrequencyAmountFilter(BAESound sound,
							short int *outFrequencyAmount);


// BAESound_SetSampleLoopPoints()
// --------------------------------------
// Sets the loop starting and ending points of the indicated BAESound, both
// expressed in terms of sample frame numbers.
// ------------------------------------
// BAEResult codes:
//           BAE_PARAM_ERR        -- Parameters null or out of range
//           BAE_BUFFER_TOO_SMALL -- Loop too short (see MIN_LOOP_SIZE)
//           BAE_NOT_SETUP   	  -- No sound data loaded
// ------------------------------------
BAEResult			BAESound_SetSampleLoopPoints(BAESound sound,
							unsigned long start,
							unsigned long end);


// BAESound_GetSampleLoopPoints()
// --------------------------------------
// Upon return, the unsigned longs pointed at by parameters outStart and outEnd
// will hold copies of the current loop starting and ending points (respectively)
// of the indicated BAESound, both expressed in terms of sample frame numbers.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- No sound data loaded
// ------------------------------------
BAEResult			BAESound_GetSampleLoopPoints(BAESound sound,
							unsigned long *outStart,
							unsigned long *outEnd);


// BAESound_SetSamplePlaybackPosition()
// --------------------------------------
//
//
BAEResult BAESound_SetSamplePlaybackPosition(BAESound sound, unsigned long pos);


// BAESound_GetSamplePlaybackPosition()
// --------------------------------------
//
//
BAEResult BAESound_GetSamplePlaybackPosition(BAESound sound, unsigned long *outPos);


void * BAESound_GetSamplePlaybackPointer(BAESound sound, unsigned long *outLength);


// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// BAESong:  Midi and RMF Songs
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------


// BAESong_New()
// ------------------------------------
// Creates a new BAESong structure and associates it with the indicated mixer. 
// Note: You must use BAESong_New() and one of the BAESong "Load" functions before
// you can play a MIDI or RMF song with a BAESong object.
//
BAESong				BAESong_New(BAEMixer mixer);

// Display debugging information
void				BAESong_DisplayInfo(BAESong song);

// BAESong_Delete()
// ------------------------------------
// Deactivates the indicated BAESong, unloads its MIDI or RMF media data, and
// frees its memory.  Call this when done with the BAESong object.
//
BAEResult			BAESong_Delete(BAESong song);

// BAESong_GetTitle
// ------------------------------------
// If a title has a title, copy it into cName.
//
BAEResult			BAESong_GetTitle(BAESong song, char *cName, int maxSize);

// BAESong_GetMemoryUsed()
// --------------------------------------
// Calculates amount of memory used by this song in bytes. Counts instruments, samples,
// and midi data.
//
BAEResult			BAESong_GetMemoryUsed(BAESong song, unsigned long *pOutResult);

// BAESong_SetMixer()
// ------------------------------------
// Associates the indicated BAESong with the indicated BAEMixer, replacing the
// previously associated BAEMixer.
//
BAEResult			BAESong_SetMixer(BAESong song,
							BAEMixer mixer);


// BAESong_GetMixer()
// --------------------------------------
// Upon return, parameter outMixer will point to a copy of the BAEMixer with which
// the indicated BAESong is associated.
//
BAEResult			BAESong_GetMixer(BAESong song,
							BAEMixer *outMixer);

// song callbacks
BAEResult			BAESong_SetCallback(BAESong sound, BAE_SongCallbackPtr pCallback, void *callbackReference);
BAEResult			BAESong_GetCallback(BAESong sound, BAE_SongCallbackPtr *pResult);

BAEResult			BAESong_GetControllerCallback(BAESong song, BAE_SongControllerCallbackPtr *pResult);
BAEResult			BAESong_SetControllerCallback(BAESong song, BAE_SongControllerCallbackPtr pCallback, void *callbackReference, short int controller);


// BAESong_SetVolume()
// --------------------------------------
// Sets the playback volume of the indicated BAESong object to the indicated
// level.  Normal volume is 1.0.
//
BAEResult			BAESong_SetVolume(BAESong song,
							BAE_UNSIGNED_FIXED volume);

BAEResult			BAESong_SetRouteBus(BAESong song, int routeBus);

// BAESong_GetVolume()
// --------------------------------------
// Upon return, the BAE_UNSIGNED_FIXED pointed to by parameter outVolume will hold
// a copy of the indicated BAESong's current playback volume.
//
BAEResult			BAESong_GetVolume(BAESong song,
							BAE_UNSIGNED_FIXED *outVolume);



// BAESong_DoesChannelAllowTranspose()
// --------------------------------------
// Upon return, the BAE_BOOL pointed to by parameter outAllowTranspose will indicate
// whether the indicated MIDI channel of the indicated BAESong has transposition
// enabled (TRUE) or disabled (FALSE). (See BAESong_SetTranspose)
//
BAEResult			BAESong_DoesChannelAllowTranspose(BAESong song,
							unsigned short int channel,
							BAE_BOOL *outAllowTranspose);


// BAESong_AllowChannelTranspose()
// --------------------------------------
// Enables (TRUE) or disables (FALSE) transposition for the indicated MIDI
// channel of the indicated BAESong.  (See BAESong_SetTranspose)
//
BAEResult			BAESong_AllowChannelTranspose(BAESong song,
							unsigned short int channel,
							BAE_BOOL allowTranspose);


// BAESong_SetTranspose()
// --------------------------------------
// Sets the indicated BAESong's transposition, in terms of a signed number of MIDI
// note numbers (semitones).  Positive offsets produce higher note numbers higher
// pitches; negative offsets produce lower note numbers and pitches. The current
// pitch offset is always added to note numbers played with the BAESong at the
// time each note is rendered (rather than modifying stored MIDI data). However,
// each MIDI channel of the BAESong can independently enable or disable use of the
// pitch offset.
//
BAEResult			BAESong_SetTranspose(BAESong song,
							long semitones);


// BAESong_GetTranspose()
// --------------------------------------
// Upon return, the long pointed to by parameter outPitchOffset will hold a copy
// of the indicated BAESong's current pitch offset. (See BAESong_SetTranspose)
//
BAEResult			BAESong_GetTranspose(BAESong song,
							long *outSemitones);


// BAESong_MuteChannel()
// --------------------------------------
// Mutes the indicated MIDI channel of the indicated BAESong.  In other words,
// turns off the audio output of all notes rendered on the channel.  To restore
// normal output, use BAESong_UnmuteChannel.
//
BAEResult			BAESong_MuteChannel(BAESong song,
							unsigned short int channel);


// BAESong_UnmuteChannel()
// --------------------------------------
// Unmutes the indicated MIDI channel of the indicated BAESong, reversing the
// effect of BAESong_MuteChannel.
//
BAEResult			BAESong_UnmuteChannel(BAESong song,
							unsigned short int channel);


// BAESong_GetChannelMuteStatus()
// --------------------------------------
// Upon return, the array of 16 BAE_BOOLs pointed to by parameter outChannels will
// indicate whether each of the 16 MIDI channels of the indicated BAESong is
// currently muted (TRUE) or not (FALSE).
//
BAEResult			BAESong_GetChannelMuteStatus(BAESong song,
							BAE_BOOL *outChannels);


// BAESong_SoloChannel()
// --------------------------------------
// Solos the indicated MIDI channel of the indicated BAESong.  In other words,
// turns off the audio output of all notes rendered on all other channels.  To
// restore normal output, use BAESong_UnSoloChannel.
//
BAEResult			BAESong_SoloChannel(BAESong song,
							unsigned short int channel);


// BAESong_UnSoloChannel()
// --------------------------------------
// Un-solos the indicated MIDI channel of the indicated BAESong, reversing the
// effect of BAESong_SoloChannel.
//
BAEResult			BAESong_UnSoloChannel(BAESong song,
							unsigned short int channel);


// BAESong_GetChannelSoloStatus()
// --------------------------------------
// Upon return, the array of 16 BAE_BOOLs pointed to by parameter outChannels will
// indicate whether each of the 16 MIDI channels of the indicated BAESong is
// currently soloed (TRUE) or not (FALSE).
//
BAEResult			BAESong_GetChannelSoloStatus(BAESong song,
							BAE_BOOL *outChannels);


// BAESong_LoadInstrument()
// --------------------------------------
// Loads the indicated instrument (and all samples it uses) from the current
// instrument bank into the indicated BAESong, unless already loaded.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- BAESong not initialized
// ------------------------------------
BAEResult			BAESong_LoadInstrument(BAESong song,
							BAE_INSTRUMENT instrument);


// BAESong_UnloadInstrument()
// --------------------------------------
// Deletes the indicated instrument and any sample data not needed by other loaded
// instruments, and frees that memory.
// --------------------------------------
// Note: Unloading an instrument during playback may prevent some or all notes
// from being heard.
// --------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP     -- BAESong not initialized
//           BAE_STILL_PLAYING -- Data is locked, try again later
// --------------------------------------
BAEResult			BAESong_UnloadInstrument(BAESong song,
							BAE_INSTRUMENT instrument);


// BAESong_IsInstrumentLoaded()
// ------------------------------------
// Upon return, the BAE_BOOL pointed to by parameter outIsLoaded will indicate
// whether the requested instrument is currently loaded into the indicated BAESong
// (TRUE) or not (FALSE).
//
BAEResult			BAESong_IsInstrumentLoaded(BAESong song,
							BAE_INSTRUMENT instrument,
							BAE_BOOL *outIsLoaded);


// BAESong_GetControlValue()
// --------------------------------------
// Upon return, the char pointed to by parameter outValue will contain a copy of
// the current value of the requested MIDI continuous controller for the requested
// MIDI channel of the indicated BAESong.  MIDI continuous controller values range
// from 0 through 127.
//
BAEResult			BAESong_GetControlValue(BAESong song, 
							unsigned char channel, 
							unsigned char controller,
							char *outValue);


// BAESong_GetProgramBank()
// --------------------------------------
// Upon return, the unsigned chars pointed to by parameters outProgram and outBank
// will contain copies of the current MIDI program (instrument) number and
// instrument bank number, respectively, for the requested MIDI channel of the
// indicated BAESong. MIDI program number values range from 0 through 127, and
// Beatnik supports three bank numbers: 0 for General MIDI, 1 for Beatnik Special,
// and 2 for User instruments directly contained within RMF files.
//
BAEResult			BAESong_GetProgramBank(BAESong song, 
							unsigned char channel,
							unsigned char *outProgram,
							unsigned char *outBank);


// BAESong_GetPitchBend()
// --------------------------------------
// Upon return, the unsigned chars pointed to by parameters outLSB and outMSB will
// contain a copy of the current MIDI pitchbend value (least significant byte and
// most significant byte, respectively) for the requested MIDI channel of the
// indicated BAESong.
//
BAEResult			BAESong_GetPitchBend(BAESong song, 
							unsigned char channel, 
							unsigned char *outLSB, 
							unsigned char *outMSB);


// BAESong_ParseMidiData()
// --------------------------------------
// Sends the BAESong object an arbitrary short MIDI message, consisting of the
// indicated MIDI commandByte and up to three MIDI data bytes, at the indicated
// time. The Mini-BAE MIDI synthesizer responds to the following commandByte
// values, where 'n' represents the MIDI channel nybble:
//     0x8n   Note off
//     0x9n   Note on
//     0xAn   Key pressure (aftertouch)
//     0xBn   Continuous controller
//     0xCn   Program change
//     0xDn   Channel pressure (aftertouch)
//     0xEn   Pitch bend
// If you supply a value of 0 for the time parameter the event occurs immediately,
// otherwise it occurs when the BAESong's current playback position reaches (or
// passes) time.
// --------------------------------------
// Example: BAESong_ParseMidiData( 0x92, 80, 127, 0 ) Immediately sends a Note On
// for channel 2, note 80, velocity 127.
// --------------------------------------
BAEResult			BAESong_ParseMidiData(BAESong song, 
							unsigned char commandByte,
							unsigned char data1Byte, 
							unsigned char data2Byte,
							unsigned char data3Byte,
							unsigned long time);


// BAESong_NoteOff()
// --------------------------------------
// Causes any and all notes with matching MIDI channel and MIDI note number
// currently rendering on the indicated BAESong to "key off" at the indicated
// time, with the indicated "key off" velocity.  This leads to termination of the
// note's envelope either immediately or at a later time (depending upon the
// design of the instrument being used), and upon envelope termination all
// rendering and maintenance of the note will end.  If you supply a value of 0 for
// the time parameter the "key off" occurs immediately, otherwise it occurs when
// the BAESong's current playback position reaches (or passes) that time.
//
BAEResult			BAESong_NoteOff(BAESong song, 
							unsigned char channel, 
							unsigned char note, 
							unsigned char velocity,
							unsigned long time);


// BAESong_NoteOnWithLoad()
// --------------------------------------
// Renders a new note on the indicated MIDI channel of the indicated BAESong,
// using the indicated MIDI note number and note velocity. If you supply a value
// of 0 for the time parameter the note is started immediately, otherwise the note
// is started when the BAESong's current playback position reaches (or passes)
// that time.  The note will be rendered using the MIDI program (instrument)
// number and bank number in effect for the indicated MIDI channel of the BAESong
// at the time the note is started. If that instrument is not yet loaded, it will
// be loaded in time to start the note.  Once started, the note will be maintained
// (and perhaps audibly sustained) until ended with a corresponding
// BAESong_NoteOff.
// ------------------------------------
// Note: If the required instrument is not loaded at the time the note is started,
// the note may produce unpredictable sound or silence.  If there is any question
// that the instrument you need may not be loaded, use BAESong_NoteOnWithLoad.
// ------------------------------------
// BAEResult codes:
//           BAE_GENERAL_BAD -- Couldn't load instrument
// --------------------------------------
BAEResult			BAESong_NoteOnWithLoad(BAESong song, 
							unsigned char channel, 
							unsigned char note, 
							unsigned char velocity,
							unsigned long time);


// BAESong_NoteOn()
// --------------------------------------
// Renders a note on the indicated MIDI channel of the indicated BAESong, using
// the indicated MIDI note number and note velocity. If you supply a value of 0
// for the time parameter the note is started immediately, otherwise the note is
// started when the BAESong's current playback position reaches (or passes) that
// time.  The note will be rendered using the MIDI program (instrument) number and
// bank number in effect for the indicated MIDI channel of the BAESong at the time
// the note is started. Once started, the note will be maintained (and perhaps
// audibly sustained) until ended with a corresponding BAESong_NoteOff.
// ------------------------------------
// Note: If the required instrument is not loaded at the time the note is started,
// the note may produce unpredictable sound or silence.  If there is any question
// that the instrument you need may not be loaded, use BAESong_NoteOnWithLoad.
//
BAEResult			BAESong_NoteOn(BAESong song, 
							unsigned char channel, 
							unsigned char note, 
							unsigned char velocity,
							unsigned long time);


// BAESong_KeyPressure()
// --------------------------------------
// Sets the MIDI polyphonic key pressure value for the indicated MIDI note number
// on the indicated MIDI channel of the indicated BAESong.  If you supply a value
// of 0 for the time parameter the key pressure event is rendered immediately,
// otherwise the event is rendered when the BAESong's current playback position
// reaches (or passes) time.
//
BAEResult			BAESong_KeyPressure(BAESong song, 
							unsigned char channel, 
							unsigned char note, 
							unsigned char pressure,
							unsigned long time);


// BAESong_ControlChange()
// --------------------------------------
// Sets the indicated MIDI continuous controller for the indicated MIDI
// channel of the indicated BAESong to the indicated value.  If you supply a value
// of 0 for the time parameter the control change event occurs immediately,
// otherwise the event occurs when the BAESong's current playback position reaches
// (or passes) time.
//
BAEResult			BAESong_ControlChange(BAESong song, 
							unsigned char channel, 
							unsigned char controlNumber,
							unsigned char controlValue, 
							unsigned long time);


// BAESong_ProgramBankChange()
// --------------------------------------
// Selects the indicated MIDI instrument bank and sends a MIDI Program Change
// event on the indicated MIDI channel of the indicated BAESong, thus selecting
// the indicated instrument from the indicated instrument bank.  Beatnik supports
// three bank numbers: 0 for General MIDI, 1 for Beatnik Special, and 2 for User
// instruments directly contained within RMF files. If you supply a value of 0 for
// the time parameter the program change event occurs immediately, otherwise the
// event occurs when the BAESong's current playback position reaches (or passes)
// time.
//
BAEResult			BAESong_ProgramBankChange(BAESong song, 
							unsigned char channel,
							unsigned char programNumber,
							unsigned char bankNumber,
							unsigned long time);


// BAESong_ProgramChange()
// --------------------------------------
// Sends a MIDI Program Change event on the indicated MIDI channel of the
// indicated BAESong, selecting the indicated instrument from the channel's
// currently selected instrument bank.  When the BAE_Song is initialized the bank
// number is set to 0, but it can be changed via a MIDI continuous controller 0
// event (which can be either stored in a MIDI or RMF file, or sent via the
// function BAESong_ControlChange) or the BAESong_ProgramBankChange function.  If
// you supply a value of 0 for the time parameter the program change event occurs
// immediately, otherwise the event occurs when the BAESong's current playback
// position reaches (or passes) time.
//
BAEResult			BAESong_ProgramChange(BAESong song, 
							unsigned char channel, 
							unsigned char programNumber,
							unsigned long time);


// BAESong_ChannelPressure()
// --------------------------------------
// Sets the MIDI channel key pressure value for the indicated MIDI channel of the
// indicated BAESong.  If you supply a value of 0 for the time parameter the key
// pressure event is rendered immediately, otherwise the event is rendered when
// the BAESong's current playback position reaches (or passes) time.
//
BAEResult			BAESong_ChannelPressure(BAESong song, 
							unsigned char channel, 	
							unsigned char pressure, 
							unsigned long time);


// BAESong_PitchBend()
// --------------------------------------
// Sets the MIDI pitch bend value for the indicated MIDI channel of the indicated
// BAESong, expressed as a 14 bit (plus sign) Least Significant Byte / Most
// Significant Byte parameter pair.  This pitch bend control detunes all notes
// being rendered on the indicated channel at the time of the pitch bend event, in
// an amount determined by the MIDI channel's current instrument at that time.  To
// produce a continuous pitch sweep effect, you must call BAESong_PitchBend
// repeatedly with a smoothly changing msb + lsb value.  If you supply a value of
// 0 for the time parameter the pitch bend event is rendered immediately,
// otherwise the event is rendered when the BAESong's current playback position
// reaches (or passes) time.
//
BAEResult			BAESong_PitchBend(BAESong song,
							unsigned char channel, 
							unsigned char lsb, 
							unsigned char msb,
							unsigned long time);


// BAESong_AllNotesOff()
// --------------------------------------
// Causes any and all notes rendering on the indicated BAESong to "key off" at the
// indicated time.  This leads to termination of the notes' envelopes either
// immediately or at a later time (depending upon the design of the instrument
// being used), and upon envelope termination all rendering and maintenance of the
// notes will end.  If you supply a value of 0 for the time parameter the "key
// offs" occurs immediately, otherwise they occur when the BAESong's current
// playback position reaches (or passes) time.
//
BAEResult			BAESong_AllNotesOff(BAESong song,
							unsigned long time);


// BAESong_LoadGroovoid()
// ------------------------------------
// Loads the indicated BAESong with the MIDI data contained in the Groovoid with
// the indicated name, if that name is available in the instrument bank currently
// being used by the BAEMixer with which the BAESong is associated.  Parameter
// ignoreBadInstruments controls whether any failures to load instruments required
// to play the indicated Groovoid will (TRUE) or will not (FALSE) be reported in
// the returned BAEResult.
// ------------------------------------
// BAEResult codes:
//           BAE_GENERAL_BAD -- song internally inconsistent
// ------------------------------------
BAEResult			BAESong_LoadGroovoid(BAESong song,
							char *cName,
							BAE_BOOL ignoreBadInstruments);

// BAESong_LoadMidiFromMemory()
// ------------------------------------
// Loads the indicated BAESong with a copy of the in-memory Standard MIDI File
// image media data at the indicated address, with the indicated length in bytes.
// Parameter ignoreBadInstruments controls whether any failures to load
// instruments required to play the indicated MIDI data will (TRUE) or will not
// (FALSE) be reported in the returned BAEResult.
// ------------------------------------
// BAEResult codes:
//           BAE_GENERAL_BAD -- song internally inconsistent
//           BAE_BAD_FILE    -- Bad MIDI data
//           BAE_MEMORY_ERR  -- Couldn't allocate memory
// ------------------------------------
BAEResult			BAESong_LoadMidiFromMemory(BAESong song,
							void const* pMidiData, 
							unsigned long midiSize, 
							BAE_BOOL ignoreBadInstruments);


// BAESong_LoadMidiFromFile()
// ------------------------------------
// Loads the indicated BAESong with a copy of the indicated Standard MIDI File.
// Parameter ignoreBadInstruments controls whether any failures to load
// instruments required to play the indicated MIDI file will (TRUE) or will not
// (FALSE) be reported in the returned BAEResult.
// ------------------------------------
// BAEResult codes:
//           BAE_GENERAL_BAD -- song internally inconsistent
//           BAE_BAD_FILE    -- Bad MIDI data
//           BAE_MEMORY_ERR  -- Couldn't allocate memory
// ------------------------------------
BAEResult			BAESong_LoadMidiFromFile(BAESong song, 
							BAEPathName filePath, 
							BAE_BOOL ignoreBadInstruments);


// BAESong_LoadRmfFromMemory()
// --------------------------------------
// was BAERmfSong::LoadFromMemory()
// ------------------------------------
// Loads the indicated BAESong with a copy of the indicated song number from the
// in-memory RMF File image media data at the indicated address, with the
// indicated length in bytes. Parameter ignoreBadInstruments controls whether any
// failures to load instruments required to play the indicated RMF data will
// (TRUE) or will not (FALSE) be reported in the returned BAEResult.
// ------------------------------------
// BAEResult codes:
//           BAE_GENERAL_BAD -- song internally inconsistent
//           BAE_BAD_FILE    -- Bad MIDI data
// ------------------------------------
BAEResult 			BAESong_LoadRmfFromMemory(BAESong song, 
							void *pRMFData, 
							unsigned long rmfSize, 
							short int songIndex, 
							BAE_BOOL ignoreBadInstruments);


// BAESong_LoadRmfFromFile()
// ------------------------------------
// Loads the indicated BAESong with a copy of the indicated song number from the
// indicated RMF File.  Parameter ignoreBadInstruments controls whether any
// failures to load instruments required to play the indicated RMF data will
// (TRUE) or will not (FALSE) be reported in the returned BAEResult.
// ------------------------------------
// BAEResult codes:
//           BAE_GENERAL_BAD    -- song internally inconsistent
//           BAE_BAD_FILE       -- Bad MIDI data
// ------------------------------------
BAEResult			BAESong_LoadRmfFromFile(BAESong song, 
							BAEPathName filePath, 
							short int songIndex, 
							BAE_BOOL ignoreBadInstruments);


// BAESong_Preroll()
// --------------------------------------
// Prepares the indicated BAESong for later instant playback by performing any and
// all lengthy resource setup operations.
//
BAEResult 			BAESong_Preroll(BAESong song);


// BAESong_Start()
// --------------------------------------
// Causes playback of the indicated BAESong to begin, at the indicated priority.
//
BAEResult			BAESong_Start(BAESong song, 
							short int priority);


// BAESong_Stop()
// --------------------------------------
// Stops playback of the indicated BAESong in one of two ways, depending upon the
// value of the startFade parameter: either stop immediately (FALSE), or stop
// after smoothly fading the sound out over a period of about 2.2 seconds (TRUE).
// ------------------------------------
// Note: Returns immediately, not at the end of the fade-out period.
// ------------------------------------
BAEResult			BAESong_Stop(BAESong song, 
							BAE_BOOL startFade);


// BAESong_Pause()
// ------------------------------------
// Pauses playback of the indicated BAESong.  If already paused, this function
// will have no effect. To resume playback, call BAESong_Resume() or
// BAESong_Start().
//
BAEResult			BAESong_Pause(BAESong song);


// BAESong_Resume()
// --------------------------------------
// If the indicated BAESong is paused at the time of this call, causes playback to
// resume from the point at which it was most recently paused. If not paused, this
// function will have no effect. Another way to resume playback aftyer a pause is
// to call BAESong_Start().
//
BAEResult			BAESong_Resume(BAESong song);


// BAESong_IsPaused()
// ------------------------------------
// Upon return, parameter outIsPaused will point to a BAE_BOOL indicating whether
// the indicated BAESong is currently in a paused state (TRUE) or not (FALSE).
//
BAEResult			BAESong_IsPaused(BAESong song,
							BAE_BOOL *outIsPaused);


// BAESong_Fade()
// --------------------------------------
// Fades the volume of the indicated BAESong smoothly from sourceVolume to
// destVolume, over a period of timeInMilliseconds.  Note that this may be either
// a fade up or a fade down.
// ------------------------------------
// BAEResult codes:
//           BAE_NOT_SETUP -- no song loaded
// ------------------------------------
BAEResult			BAESong_Fade(BAESong song, 
							BAE_FIXED sourceVolume, 
							BAE_FIXED destVolume, 
							BAE_FIXED timeInMiliseconds);


// BAESong_IsDone()
// --------------------------------------
// Upon return, the BAE_BOOL pointed at by parameter outIsDone will indicate
// whether the indicated BAESong object has (TRUE) or has not (FALSE) played all
// the way to its end and stopped on its own.
//
BAEResult			BAESong_IsDone(BAESong song, 
							BAE_BOOL *outIsDone);


// BAESong_AreMidiEventsPending()
// --------------------------------------
// Upon return, the BAE_BOOL pointed at by parameter outPending will indicate
// whether there are any midi events pending
//
BAEResult			BAESong_AreMidiEventsPending(BAESong song,
							BAE_BOOL *outPending);


// BAESong_SetLoops()
// ------------------------------------
// Sets the loop repeat counter for the indicated BAESong to the indicated 
// value. To prevent or cancel looping, call with numLoops equal to 0.
//
// About Looping - Looping behavior for each BAESong - that is, whether or not
// the song will begin again from its start each time playback reaches the end
// of the MIDI or RMF data - is controlled by an internal repeat counter
// variable, which you can override with this function at any time. If the
// value of the loop repeat counter is equal to zero when the end of the song 
// is reached, then the song doesn't repeat any further; otherwise, the counter
// is decremented and the song restarts.
//
// Note that this means your song will play a total of numLoops + 1 times, not
// numLoops. For example, if you call BAESong_SetLoops( yourSong, 1 ); while
// the song is playing, you'll hear the song play twice: a first pass, followed
// by a loop back to the start and a second playback pass.
//
// BAEResult codes:
//			 BAE_NULL_OBJECT	-- Null song object pointer
//			 BAE_PARAM_ERR		-- Bad numLoops - must be non-negative
//
// Note: Calling BAESong_Start() will ordinarily reset the BAESong's repeat
// counter variable to 0, replacing your  numLoops value. To prevent this, call
// BAESong_Preroll() before calling BAESong_SetLoops().
//
// Note: BAESong_SetLoops() controls only RMF whole-song looping, as set in the
// Beatnik Editor's Song Settings window. Looping within individual MIDI File 
// tracks using the Beatnik marker and controller techniques is not affected by
// this function.
//
BAEResult			BAESong_SetLoops(BAESong song,
							short numLoops);

// BAESong_GetLoops()
// ------------------------------------
// Upon return, parameter outNumLoops will point to a short int containing a
// copy of the indicated BAESong's loop repeat setting, as set by
// BAESong_SetLoops(). This is the number of times the song will restart when
// playing back, so the song will be heard that number of times plus one.
//
// BAEREsult codes:
//			BAE_NULL_OBJECT		-- Null song object pointer
//			BAE_PARAM_ERR		-- Null parameter
//
// Note: This function returns the static value of the repeat setting, 
// not the current value of the internal loop counter. Consequently, the 
// returned value will not change during BAESong playback.
//
BAEResult			BAESong_GetLoops(BAESong song,
							short *outNumLoops);


// BAESong_SetMicrosecondPosition()
// ------------------------------------
// Sets the current playback position of the indicated BAESong to the requested
// offset, expressed in microseconds from the beginning of the MIDI or RMF song
// data.
//
BAEResult			BAESong_SetMicrosecondPosition(BAESong song, 
							unsigned long ticks);


// BAESong_GetMicrosecondPosition()
// ------------------------------------
// Upon return, parameter outTicks will point to an unsigned long containing the
// current playback position of the indicated BAESong, expressed in microseconds.
//
BAEResult			BAESong_GetMicrosecondPosition(BAESong song,
							unsigned long *outTicks);


// BAESong_GetMicrosecondLength()
// ------------------------------------
// Upon return, parameter outLength will point to an unsigned long containing the
// length in microseconds of the indicated BAESong's currently loaded MIDI or RMF
// song data.  The result assumes that the song would be played at the tempo
// stored in the song data, so any changes made via BAESong_SetTempo would not be
// reflected.
//
BAEResult			BAESong_GetMicrosecondLength(BAESong song,
							unsigned long *outLength);


// BAESong_SetMasterTempo()
// --------------------------------------
// Sets the tempo of the indicated BAESong, expressed in musical beats per minute.
// --------------------------------------
// Note: This function will appear to have no effect if called while the song is
// stopped, because starting a song resets the tempo to the value stored in the
// MIDI or RMF data.
//
BAEResult			BAESong_SetMasterTempo(BAESong song,
							BAE_UNSIGNED_FIXED tempoFactor);


// BAESong_GetMasterTempo()
// --------------------------------------
// Upon return, parameter outTempoFactor will point at a BAE_UNSIGNED_FIXED
// containing a copy of the indicated BAESong's current tempo, expressed in
// musical beats per minute.
//
BAEResult			BAESong_GetMasterTempo(BAESong song,
							BAE_UNSIGNED_FIXED *outTempoFactor);


// BAESong_MuteTrack()
// --------------------------------------
// Mutes the indicated Standard MIDI File data track or RMF file data track for
// the indicated BAESong.  In other words, turns off the audio output of all notes
// contained on that track.  To restore normal output, use BAESong_UnmuteTrack.
//
BAEResult			BAESong_MuteTrack(BAESong song,
							unsigned short int track);


// BAESong_UnmuteTrack()
// --------------------------------------
// Unmutes the indicated Standard MIDI File data track or RMF file data track for
// the indicated BAESong, reversing the effect of BAESong_MuteTrack.
//
BAEResult			BAESong_UnmuteTrack(BAESong song,
							unsigned short int track);


// BAESong_GetTrackMuteStatus()
// --------------------------------------
// Upon return, the array of 16 BAE_BOOLs pointed to by parameter outTracks will
// indicate whether each of the 16 Standard MIDI File or RMF file data tracks for
// the indicated BAESong is currently muted (TRUE) or not (FALSE).
//
BAEResult			BAESong_GetTrackMuteStatus(BAESong song,
							BAE_BOOL *outTracks);


// BAESong_SoloTrack()
// --------------------------------------
// Solos the indicated Standard MIDI File or RMF file data track for the indicated
// BAESong.  In other words, turns off the audio output of all notes rendered on
// all other tracks.  To restore normal output, use BAESong_UnSoloTrack.
//
BAEResult			BAESong_SoloTrack(BAESong song,
							unsigned short int track);


// BAESong_UnSoloTrack()
// --------------------------------------
// Un-solos the indicated Standard MIDI File or RMF file data track for the
// indicated BAESong, reversing the effect of BAESong_SoloTrack.
//
BAEResult			BAESong_UnSoloTrack(BAESong song,
							unsigned short int track);


// BAESong_GetSoloTrackStatus()
// --------------------------------------
// Upon return, the array of 16 BAE_BOOLs pointed to by parameter outTracks will
// indicate whether each of the 16 Standard MIDI File or RMF file data tracks for
// the indicated BAESong is currently soloed (TRUE) or not (FALSE).
//
BAEResult			BAESong_GetSoloTrackStatus(BAESong song,
							BAE_BOOL *outTracks);





// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Utility functions
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// BAEUtil_TranslateBankProgramToInstrument()
// --------------------------------------
// Returns the BAE_INSTRUMENT ID of the Beatnik instrument being used on 
// the indicated MIDI channel number for the indicated MIDI Program Bank 
// and Program Number; for MIDI channel 10, the MIDI note number is also 
// considered. The note number is ignored for all MIDI channels other 
// than 10.
// 
// Note: Beatnik supports three bank numbers: 0 for General MIDI, 1 
// for Beatnik Special, and 2 for User instruments directly contained 
// within RMF files.
// 
// Note: Mini-BAE conforms to the General MIDI standard,whereby MIDI 
// channel 10 (PERCUSSION_CHANNEL) is considered the 'drum channel', 
// which handles MIDI note numbers differently from the other 15 
// channels. In channel 10 each MIDI note number accesses a separate 
// instrument, rather than transposing a single instrument to different 
// pitches.
//
BAE_INSTRUMENT		TranslateBankProgramToInstrument(unsigned short bank, 
							unsigned short program, 
							unsigned short channel,
							unsigned short note);
							
							
// BAEUtil_GetRmfSongInfoFromFile()
// --------------------------------------
// If the file at filePath contains a song with index 
// songIndex, and that song includes a text info field of type infoType, 
// then upon return the null-terminated character string pointed at by 
// parameter targetBuffer will contain a copy of that text info field. 
// You must supply the size in bytes of your targetBuffer.
// --------------------------------------
// BAEResult codes:
//           BAE_PARAM_ERR -- Bad infoType requested
//           BAE_NOT_SETUP -- RMF info feature not supported
// --------------------------------------
BAEResult BAEUtil_GetRmfSongInfoFromFile(BAEPathName filePath, short songIndex,
								 BAEInfoType infoType, char* targetBuffer, unsigned long bufferBytes);

// BAEUtil_GetInfoSizeFromFile()
// --------------------------------------
// If the file at filePath contains a song with index 
// songIndex, and that song includes a text info field of type infoType, 
// then returns the size in bytes of that text info field. 
// 
// Returns: Text info field size in bytes
//
BAEResult BAEUtil_GetInfoSizeFromFile(BAEPathName filePath, 
									  short songIndex, 
									  BAEInfoType infoType, 
									  unsigned long *pOutResourceSize);
		
		
// BAEUtil_GetRmfSongInfo()
// --------------------------------------
// If the RMF file image at address pRMFData contains a song with index 
// songIndex, and that song includes a text info field of type infoType, 
// then upon return the null-terminated character string pointed at by 
// parameter targetBuffer will contain a copy of that text info field. 
// You must supply the size in bytes of the RMF file image, and the size 
// in bytes of your targetBuffer.
// --------------------------------------
// BAEResult codes:
//           BAE_PARAM_ERR -- Bad infoType requested
//           BAE_NOT_SETUP -- RMF info feature not supported
// --------------------------------------
BAEResult 			BAEUtil_GetRmfSongInfo(void *pRMFData, 
							unsigned long rmfSize, 
							short songIndex, 
							BAEInfoType infoType, 
							char* targetBuffer, 
							unsigned long bufferBytes);


// BAEUtil_GetInfoSize()
// --------------------------------------
// If the RMF file image at address pRMFData contains a song with index 
// songIndex, and that song includes a text info field of type infoType, 
// then returns the size in bytes of that text info field. You must 
// supply the size in bytes of the RMF file image.
// 
// Returns: Text info field size in bytes
//
unsigned long 		BAEUtil_GetInfoSize(void *pRMFData, 
							unsigned long rmfSize, 
							short songIndex, 
							BAEInfoType infoType);


// BAEUtil_IsRmfSongEncrypted()
// --------------------------------------
// If the RMF file image at address pRMFData contains a song with index 
// songIndex, returns a BAE_BOOL indicating whether that song is (TRUE) 
// or is not (FALSE) encrypted. You must supply the size in bytes of the 
// RMF data.
//
BAE_BOOL 			BAEUtil_IsRmfSongEncrypted(void *pRMFData, 
							unsigned long rmfSize, 
							short songIndex);


// BAEUtil_IsRmfSongCompressed()
// --------------------------------------
// If the RMF file image at address pRMFData contains a song with index 
// songIndex, returns a BAE_BOOL indicating whether that song is (TRUE) 
// or is not (FALSE) data-compressed. You must supply the size in bytes 
// of the RMF data.
// 
// Note: While Beatnik RMF generation tools generally data-compress 
// songs, the RMF file format also accomodates uncompressed songs.
//
BAE_BOOL 			BAEUtil_IsRmfSongCompressed(void *pRMFData,
							unsigned long rmfSize, 
							short songIndex);


// BAEUtil_GetRmfVersion()
// --------------------------------------
// If the RMF file image exists at address pRMFData, then upon return 
// the short ints pointed at by parameters pVersionMajor, pVersionMinor, 
// and pVersionSubMinor will contain a copy of the RMF format version in 
// which that RMF file is encoded. You must supply the size in bytes of 
// the RMF data.
// --------------------------------------
// BAEResult codes:
//           BAE_PARAM_ERR -- Null parameters or bad RMF data
// --------------------------------------
BAEResult 			BAEUtil_GetRmfVersion(void *pRMFData, 
							unsigned long rmfSize, 
							short int *pVersionMajor, 
							short int *pVersionMinor, 
							short int *pVersionSubMinor);
							

#ifdef __cplusplus
	}				// extern "C"
#endif

#endif // BAE_AUDIO
