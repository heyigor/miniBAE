/*****************************************************************************/
/*
** "BAESongTest.c"
**
**	© Copyright 1999-2000 Beatnik, Inc, All Rights Reserved.
**	Written by Mark Deggeller
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
**	1999.12.16		Created.
**	2000.01.11 MSD	fixed dumb error in testing BAESong_LoadRmfFromMemory()
**	2000.01.13 MSD	fixed dumb error in testing Load*FromMemory()
**	2000.01.14 MSD	added support for BAESong_AreMidiEventsPending()
**	2000.01.18 MSD	x'd out unimplemented tests from menu
**	2000.01.26 MSD	added features to PV_TestKeyboard()
**	2000.03.13 MSD	added scripts
**	2000.03.17 sh	fixed error not being setup in PV_DoScript
**	2000.03.23 msd	changed ...PitchOffset() functions to ...Transpose()
**	2000.03.28 msd	fixed error in test for BAESong_GetVolume()
**	2000.03.28 msd	fixed bug in DoFunction() call to AreMidiEventsPending()
**					Changed copyright and modification history format
**					Renamed BAESong_SetLoopMax to BAESong_SetLoops()
**	2000.05.25 msd	Removed #includes for X_API.h and GenSnd.h, and removed
**						usage of MAX_BANKS and MAX_INSTRUMENTS
**	7/17/2000	sh	modified PV_TestKeyboard to ignore character 10 (crlf)
**	9/28/2000	sh	Added new default script to load an rmf file with and MPEG instrument.
**	10/18/2000	sh	Added more scripts for midi files.
*/
/*****************************************************************************/


#include "BAESongTest.h"
#include "TestTools.h"
#include "TestSuite.h"
#include "MiniBAE.h"
#include "BAE_API.h"
#include <stdio.h>			// for printf()
#include <stdlib.h>			// for malloc, free

#if X_PLATFORM == X_MACINTOSH || X_PLATFORM == X_BE
	#define	_getch getchar
#else
	#include <conio.h>			// for _getch()
#endif

#define DEFAULT_MIDI_FILE		"testMidi1.mid"
#define DEFAULT_MIDI_FILE1		"onenote.mid"
#define DEFAULT_MIDI_FILE2		"2notes.mid"
#define DEFAULT_MIDI_FILE3		"synco.mid"
#define DEFAULT_RMF_FILE		"House.rmf"
#define DEFAULT_RMF_MPEG_FILE	"Tricky_tracks.rmf"
#define DEFAULT_GROOVOID		"Background-Smooth"


enum {
	SCRIPT_ALL = 1,
	SCRIPT_NEW,
	SCRIPT_LOAD_SONG,
	SCRIPT_VOLUME,
	SCRIPT_TRANSPOSE,
	SCRIPT_MUTE_CHANNEL,
	SCRIPT_SOLO_CHANNEL,
	SCRIPT_LOAD_INSTRUMENT,
	SCRIPT_NOTE_ON,
	SCRIPT_KEYBOARD,
	SCRIPT_SHOW_LOADED_INSTS,
	SCRIPT_LOAD_DEFAULT_MIDI,
	SCRIPT_LOAD_DEFAULT_MIDI1,
	SCRIPT_LOAD_DEFAULT_MIDI2,
	SCRIPT_LOAD_DEFAULT_MIDI3,
	SCRIPT_LOAD_DEFAULT_RMF,
	SCRIPT_LOAD_DEFAULT_GROOVOID,
	SCRIPT_LOAD_DEFAULT_MPEG_RMF,
	NUM_SCRIPTS_PLUS_ONE,
	NUM_SCRIPTS = NUM_SCRIPTS_PLUS_ONE - 1
};


static const char *ScriptList[NUM_SCRIPTS] = 
{
	"X Do all tests",
	"X Test New/Delete",
	"X Test Loading Songs",
	"X Test Get/Set Volume",
	"X Test Transpose",
	"X Test Mute Channel",
	"X Test Solo Channel",
	"X Test Load/Unload Instrument",
	"X Test NoteOn / NoteOff",
	"Play keyboard",
	"Print loaded instruments",
	"Load 'testMidi1.mid' into slot 0",
	"Load 'onenote.mid' into slot 0",
	"Load '2note.mid' into slot 0",
	"Load 'synco.mid' into slot 0",
	"Load 'House.rmf' into slot 0",
	"Load Groovoid 'Background-Smooth' from 'patches.hsb' into slot 0.",
	"Load 'tricky_tracks.rmf' into slot 0 (requires MPEG code to be built)"
};


enum {
	FUNC_NEW = 1,
	FUNC_DELETE,
	FUNC_SET_VOLUME,
	FUNC_GET_VOLUME,
	FUNC_DOES_CHANNEL_ALLOW_TRANSPOSE,
	FUNC_ALLOW_CHANNEL_TRANSPOSE,
	FUNC_SET_TRANSPOSE,
	FUNC_GET_TRANSPOSE,
	FUNC_MUTE_CHANNEL,
	FUNC_UNMUTE_CHANNEL,
	FUNC_GET_CHANNEL_MUTE_STATUS,
	FUNC_SOLO_CHANNEL,
	FUNC_UNSOLO_CHANNEL,
	FUNC_GET_CHANNEL_SOLO_STATUS,
	FUNC_LOAD_INSTRUMENT,
	FUNC_UNLOAD_INSTRUMENT,
	FUNC_IS_INSTRUMENT_LOADED,
	FUNC_GET_CONTROL_VALUE,
	FUNC_GET_PROGRAM_BANK,
	FUNC_GET_PITCH_BEND,
	FUNC_PARSE_MIDI_DATA,
	FUNC_NOTE_OFF,
	FUNC_NOTE_ON_WITH_LOAD,
	FUNC_NOTE_ON,
	FUNC_KEY_PRESSURE,
	FUNC_CONTROL_CHANGE,
	FUNC_PROGRAM_BANK_CHANGE,
	FUNC_PROGRAM_CHANGE,
	FUNC_CHANNEL_PRESSURE,
	FUNC_PITCH_BEND,
	FUNC_ALL_NOTES_OFF,
	FUNC_LOAD_GROOVOID,
	FUNC_LOAD_MIDI_FROM_MEMORY,
	FUNC_LOAD_MIDI_FROM_FILE,
	FUNC_LOAD_RMF_FROM_MEMORY,
	FUNC_LOAD_RMF_FROM_FILE,
	FUNC_PREROLL,
	FUNC_START,
	FUNC_STOP,
	FUNC_PAUSE,
	FUNC_RESUME,
	FUNC_IS_PAUSED,
	FUNC_FADE,
	FUNC_IS_DONE,
	FUNC_ARE_MIDI_EVENTS_PENDING,
	FUNC_SET_LOOPS,
	FUNC_GET_LOOPS,
	FUNC_SET_LOOP_FLAG,
	FUNC_GET_LOOP_FLAG,
	FUNC_SET_MICROSECOND_POSITION,
	FUNC_GET_MICROSECOND_POSITION,
	FUNC_GET_MICROSECOND_LENGTH,
	FUNC_SET_MASTER_TEMPO,
	FUNC_GET_MASTER_TEMPO,
	FUNC_MUTE_TRACK,
	FUNC_UNMUTE_TRACK,
	FUNC_GET_TRACK_MUTE_STATUS,
	FUNC_SOLO_TRACK,
	FUNC_UNSOLO_TRACK,
	FUNC_GET_SOLO_TRACK_STATUS,
	NUM_FUNCTIONS_PLUS_ONE,
	NUM_FUNCTIONS = NUM_FUNCTIONS_PLUS_ONE - 1
};


static const char *FunctionList[NUM_FUNCTIONS] = 
{
	"BAESong_New()",
	"BAESong_Delete()",
	"BAESong_SetVolume()",
	"BAESong_GetVolume()",
	"BAESong_DoesChannelAllowTranspose()",
	"BAESong_AllowChannelTranspose()",
	"BAESong_SetTranspose()",
	"BAESong_GetTranspose()",
	"BAESong_MuteChannel()",
	"BAESong_UnmuteChannel()",
	"BAESong_GetChannelMuteStatus()",
	"BAESong_SoloChannel()",
	"BAESong_UnSoloChannel()",
	"BAESong_GetChannelSoloStatus()",
	"BAESong_LoadInstrument()",
	"BAESong_UnloadInstrument()",
	"BAESong_IsInstrumentLoaded()",
	"BAESong_GetControlValue()",
	"BAESong_GetProgramBank()",
	"BAESong_GetPitchBend()",
	"BAESong_ParseMidiData()",
	"BAESong_NoteOff()",
	"BAESong_NoteOnWithLoad()",
	"BAESong_NoteOn()",
	"BAESong_KeyPressure()",
	"BAESong_ControlChange()",
	"BAESong_ProgramBankChange()",
	"BAESong_ProgramChange()",
	"BAESong_ChannelPressure()",
	"BAESong_PitchBend()",
	"BAESong_AllNotesOff()",
	"BAESong_LoadGroovoid()",
	"BAESong_LoadMidiFromMemory()",
	"BAESong_LoadMidiFromFile()",
	"BAESong_LoadRmfFromMemory()",
	"BAESong_LoadRmfFromFile()",
	"BAESong_Preroll()",
	"BAESong_Start()",
	"BAESong_Stop()",
	"BAESong_Pause()",
	"BAESong_Resume()",
	"BAESong_IsPaused()",
	"BAESong_Fade()",
	"BAESong_IsDone()",
	"BAESong_AreMidiEventsPending()",
	"BAESong_SetLoops()",
	"BAESong_GetLoops()",
	"deleted: BAESong_SetLoopFlag()",
	"deleted: BAESong_GetLoopFlag()",
	"BAESong_SetMicrosecondPosition()",
	"BAESong_GetMicrosecondPosition()",
	"BAESong_GetMicrosecondLength()",
	"BAESong_SetMasterTempo()",
	"BAESong_GetMasterTempo()",
	"BAESong_MuteTrack()",
	"BAESong_UnmuteTrack()",
	"BAESong_GetTrackMuteStatus()",
	"BAESong_SoloTrack()",
	"BAESong_UnSoloTrack()",
	"BAESong_GetSoloTrackStatus()"
};


static BAESong	theSong;

//static BAESong	scriptSongs[NUM_SCRIPT_SONGS];
//static BAE_BOOL	scriptSongsNulled = FALSE;
//static BAE_BOOL	scriptSongsInitialized = FALSE;


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
static void PV_DoFunction(int subMenu);
static void PV_DoScript(int subMenu);
static void PV_TestAll(void);
static void PV_TestConstructor(void);
static void PV_TestLoadSong(void);
static void PV_TestKeyboard(void);
static void PV_TestNoteOn(void);


////////////////////////////////////////////////////////////////////////////////
// Exported functions
////////////////////////////////////////////////////////////////////////////////


// SongTest_GetNumMenuItems()
// -----------------------------------------------------
//
//
int SongTest_GetNumMenuItems(void)
{
	return 2;
}



// SongTest_GetNumSubMenuItems()
// -----------------------------------------------------
//
//
int SongTest_GetNumSubMenuItems(int menu)
{
	int result;

	result = 0;
	switch (menu)
	{
		case 1:
			result = NUM_SCRIPTS;
			break;

		case 2:
			result = NUM_FUNCTIONS;
			break;

		default:
			break;
	}
	return result;
}



// SongTest_GetMenuText()
// -----------------------------------------------------
//
//
int SongTest_GetMenuText(int menu, const char **outMenuText)
{
	int result;

	result = 0;
	if (outMenuText)
	{
		switch (menu)
		{
			case 1:
				*outMenuText = "Test scripts";
				result = 1;
				break;
		
			case 2:
				*outMenuText = "Direct function calls";
				result = 1;
				break;

			default:
				break;
		}
	}
	return result;
}



// SongTest_GetSubMenuText()
// -----------------------------------------------------
//
//
int SongTest_GetSubMenuText(int menu, int subMenu, const char **outMenuText)
{
	int result;

	result = 0;
	if (outMenuText)
	{
		switch (menu)
		{
			case 1: // Test Scripts
				if (subMenu > 0 && subMenu <= NUM_SCRIPTS)
				{
					*outMenuText = ScriptList[subMenu-1];
					result = 1;
				}
				break;

			case 2: // Direct function calls
				if (subMenu > 0 && subMenu <= NUM_FUNCTIONS)
				{
					*outMenuText = FunctionList[subMenu-1];
					result = 1;
				}
				break;
	
			default:
				break;
		}
	}
	return result;
}



// SongTest_DoMenuItem()
// -----------------------------------------------------
//
//
void SongTest_DoMenuItem(int menu, int subMenu)
{
	switch (menu)
	{
		case 1:
			PV_DoScript(subMenu);
			break;
		
		case 2:
			PV_DoFunction(subMenu);
			break;
		
		default:
			break;
	}
}



////////////////////////////////////////////////////////////////////////////////
// Static functions
////////////////////////////////////////////////////////////////////////////////


// DoFunction()
// -----------------------------------------------------
//
//
static void PV_DoFunction(int subMenu)
{
	long				slot;
	
	long				tmpLong1, tmpLong2, tmpLong3, tmpLong4, tmpLong5;
	unsigned char		tmpChar1, tmpChar2;
	double				tmpDouble1, tmpDouble2, tmpDouble3;
	BAE_UNSIGNED_FIXED	tmpUFixed;
	BAE_BOOL			tmpBool;
	char				tmpBuffer[256];
	FILE				*filePtr = NULL;
	static void			*bufferPtr = NULL;

	BAEResult			err;
	
	err = BAE_NO_ERROR;

	while (((slot=GetLong("Slot? (0-3) > "))<0) || (slot>=MAX_SLOTS))
	{
		printf("Try again.  Choose a slot between 0 and %d\n", MAX_SLOTS-1);
	}

	switch (subMenu)
	{
		case FUNC_NEW:
		// BAESong_New(BAEMixer mixer);
			{
				if (gSongs[slot])
				{
					printf(": Deleting old song in slot %d...\n", slot);
					BAESong_Delete(gSongs[slot]);
					gSongs[slot] = NULL;
				}
				gSongs[slot] = BAESong_New(gMixer);
				if (gSongs[slot] != NULL)
				{
					printf(": New BAESong in slot %d.\n", slot);
				}
				else
				{
					printf(": BAESong_New() returned NULL\n");
				}
			}
			break;

		case FUNC_DELETE:
		// BAESong_Delete(BAESong song);
			{
				err = BAESong_Delete(gSongs[slot]);
				if (!err) gSongs[slot] = NULL;
			}
			break;
		
		case FUNC_SET_VOLUME:
		// BAESong_SetVolume(BAESong song, BAE_UNSIGNED_FIXED volume);
			{
				tmpDouble1 = GetDouble("Volume (1.0 norm) > ");
				err = BAESong_SetVolume(gSongs[slot], FLOAT_TO_UNSIGNED_FIXED(tmpDouble1));
			}
			break;
		
		case FUNC_GET_VOLUME: 
		// BAESong_GetVolume(BAESong song, BAE_UNSIGNED_FIXED *outVolume);
			{
				err = BAESong_GetVolume(gSongs[slot], &tmpUFixed);
				if (!err) printf(": volume = %g\n", UNSIGNED_FIXED_TO_FLOAT(tmpUFixed));
			}
			break;
		
		case FUNC_DOES_CHANNEL_ALLOW_TRANSPOSE: 
		// BAESong_DoesChannelAllowTranspose(BAESong song, unsigned short int channel, BAE_BOOL *outAllowTranspose);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_DoesChannelAllowTranspose(gSongs[slot], (unsigned short)tmpLong1, &tmpBool);
				if (!err)
				{
					printf(": Channel %ld does ", tmpLong1);
					if (!tmpBool) printf("NOT ");
					printf("allow transpose\n");
				}
			}
			break;
		
		case FUNC_ALLOW_CHANNEL_TRANSPOSE:
		// BAESong_AllowChannelTranspose(BAESong song, unsigned short int channel, BAE_BOOL allowTranspose);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpBool = (BAE_BOOL)GetBool("Allow transpose? (y/n) > ");
				err = BAESong_AllowChannelTranspose(gSongs[slot], (unsigned short)tmpLong1, tmpBool);
			}
			break;
		
		case FUNC_SET_TRANSPOSE:
		// BAESong_SetTranspose(BAESong song, long semitones);
			{
				tmpLong1 = GetLong("Transpose semitones > ");
				err = BAESong_SetTranspose(gSongs[slot], tmpLong1);
			}
			break;
		
		case FUNC_GET_TRANSPOSE:
		// BAESong_GetTranspose(BAESong song, long *outSemitones);
			{
				tmpLong1 = 0;
				err = BAESong_GetTranspose(gSongs[slot], &tmpLong1);
				if (!err) printf(": Transpose = %ld semitones\n", tmpLong1);
			}
			break;
		
		case FUNC_MUTE_CHANNEL:
		// BAESong_MuteChannel(BAESong song, unsigned short int channel);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_MuteChannel(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_UNMUTE_CHANNEL:
		// BAESong_UnmuteChannel(BAESong song, unsigned short int channel);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_UnmuteChannel(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_GET_CHANNEL_MUTE_STATUS:
		// BAESong_GetChannelMuteStatus(BAESong song, BAE_BOOL *outChannels);
			{
				BAE_BOOL result[16];
				int i;

				err = BAESong_GetChannelMuteStatus(gSongs[slot], result);
				if (!err)
				{
					printf("channel:     0123456789ABCDEF\n");
					printf("mute status: ");
					for (i=0; i<16; i++)
					{
						printf("%d", (int)result[i]);
					}
					printf("\n");
				}
			}
			break;

		case FUNC_SOLO_CHANNEL:
		// BAESong_SoloChannel(BAESong song, unsigned short int channel);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_SoloChannel(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_UNSOLO_CHANNEL:
		// BAESong_UnSoloChannel(BAESong song, unsigned short int channel);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_UnSoloChannel(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_GET_CHANNEL_SOLO_STATUS:
		// BAESong_GetChannelSoloStatus(BAESong song, BAE_BOOL *outChannels);
			{
				BAE_BOOL result[16];
				int i;

				err = BAESong_GetChannelSoloStatus(gSongs[slot], result);
				if (!err)
				{
					printf("channel:     0123456789ABCDEF\n");
					printf("solo status: ");
					for (i=0; i<16; i++)
					{
						printf("%d", (int)result[i]);
					}
					printf("\n");
				}
			}
			break;

		case FUNC_LOAD_INSTRUMENT:
		// BAESong_LoadInstrument(BAESong song, BAE_INSTRUMENT instrument);
			{
				tmpLong1 = GetLong("Instrument > ");
				err = BAESong_LoadInstrument(gSongs[slot], tmpLong1);
			}
			break;

		case FUNC_UNLOAD_INSTRUMENT:
		// BAESong_UnloadInstrument(BAESong song, BAE_INSTRUMENT instrument);
			{
				tmpLong1 = GetLong("Instrument > ");
				err = BAESong_UnloadInstrument(gSongs[slot], tmpLong1);
			}
			break;

		case FUNC_IS_INSTRUMENT_LOADED:
		// BAESong_IsInstrumentLoaded(BAESong song, BAE_INSTRUMENT instrument, BAE_BOOL *outIsLoaded);
			{
				tmpLong1 = GetLong("Instrument > ");
				err = BAESong_IsInstrumentLoaded(gSongs[slot], tmpLong1, &tmpBool);
				if (!err) printf(": Is Loaded = %d\n", (int)tmpBool);
			}
			break;

		case FUNC_GET_CONTROL_VALUE:
		// BAESong_GetControlValue(BAESong song, unsigned char channel, unsigned char controller, char *outValue);
			{
				tmpLong1  = GetLong("Channel > ");
				tmpLong2 = GetLong("Controller > ");
				err = BAESong_GetControlValue(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (char *)&tmpChar1);
				if (!err) printf(": Value = %ld\n", tmpChar1);
			}
			break;

		case FUNC_GET_PROGRAM_BANK:
		// BAESong_GetProgramBank(BAESong song, unsigned char channel, unsigned char *outProgram, unsigned char *outBank);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_GetProgramBank(gSongs[slot], (unsigned char)tmpLong1, &tmpChar1, &tmpChar2);
				if (!err) printf(": Program = %ld, bank = %ld\n", tmpChar1, tmpChar2);
			}
			break;

		case FUNC_GET_PITCH_BEND:
		// BAESong_GetPitchBend(BAESong song, unsigned char channel, unsigned char *outLSB, unsigned char *outMSB);
			{
				tmpLong1 = GetLong("Channel > ");
				err = BAESong_GetPitchBend(gSongs[slot], (unsigned char)tmpLong1, &tmpChar1, &tmpChar2);
				if (!err) printf(": LSB = %ld, MSB = %ld\n", tmpChar1, tmpChar2);
			}
			break;

		case FUNC_PARSE_MIDI_DATA:
		// BAESong_ParseMidiData(BAESong song, unsigned char commandByte, unsigned char data1Byte, unsigned char data2Byte, unsigned char data3Byte, unsigned long time);
			{
				tmpLong1 = GetLong("Command byte > ");
				tmpLong2 = GetLong("Data 1 byte > ");
				tmpLong3 = GetLong("Data 2 byte > ");
				tmpLong4 = GetLong("Data 3 byte > ");
				tmpLong5 = GetLong("Time > ");
				err = BAESong_ParseMidiData(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned char)tmpLong4, (unsigned long)tmpLong5);
			}
			break;

		case FUNC_NOTE_OFF:
		// BAESong_NoteOff(BAESong song, unsigned char channel, unsigned char note, unsigned char velocity, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Note > ");
				tmpLong3 = GetLong("Velocity > ");
				tmpLong4 = GetLong("Time > ");
				err = BAESong_NoteOff(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned long)tmpLong4);
			}
			break;

		case FUNC_NOTE_ON_WITH_LOAD:
		// BAESong_NoteOnWithLoad(BAESong song, unsigned char channel, unsigned char note, unsigned char velocity, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Note > ");
				tmpLong3 = GetLong("Velocity > ");
				tmpLong4 = GetLong("Time > ");
				err = BAESong_NoteOnWithLoad(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned long)tmpLong4);
			}
			break;

		case FUNC_NOTE_ON:
		// BAESong_NoteOn(BAESong song, unsigned char channel, unsigned char note, unsigned char velocity, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Note > ");
				tmpLong3 = GetLong("Velocity > ");
				tmpLong4 = GetLong("Time > ");
				err = BAESong_NoteOn(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned long)tmpLong4);
			}
			break;

		case FUNC_KEY_PRESSURE:
		// BAESong_KeyPressure(BAESong song, unsigned char channel, unsigned char note, unsigned char pressure, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Note > ");
				tmpLong3 = GetLong("Pressure > ");
				tmpLong4 = GetLong("Time > ");
				err = BAESong_KeyPressure(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned long)tmpLong4);
			}
			break;

		case FUNC_CONTROL_CHANGE:
		// BAESong_ControlChange(BAESong song, unsigned char channel, unsigned char controlNumber, unsigned char controlValue, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Control number > ");
				tmpLong3 = GetLong("Control value > ");
				tmpLong4 = GetLong("Time > ");
				err = BAESong_ControlChange(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned long)tmpLong4);
			}
			break;

		case FUNC_PROGRAM_BANK_CHANGE:
		// BAESong_ProgramBankChange(BAESong song, unsigned char channel, unsigned char programNumber, unsigned char bankNumber, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Program > ");
				tmpLong3 = GetLong("Bank > ");
				tmpLong4 = GetLong("Time > ");
				err = BAESong_ProgramBankChange(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned char)tmpLong3, (unsigned long)tmpLong4);
			}
			break;

		case FUNC_PROGRAM_CHANGE:
		// BAESong_ProgramChange(BAESong song, unsigned char channel, unsigned char programNumber, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Program > ");
				tmpLong3 = GetLong("Time > ");
				err = BAESong_ProgramChange(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned long)tmpLong3);
			}
			break;

		case FUNC_CHANNEL_PRESSURE:
		// BAESong_ChannelPressure(BAESong song, unsigned char channel, unsigned char pressure, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("Pressure > ");
				tmpLong3 = GetLong("Time > ");
				err = BAESong_ChannelPressure(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned long)tmpLong3);
			}
			break;

		case FUNC_PITCH_BEND:
		// BAESong_PitchBend(BAESong song, unsigned char channel, unsigned char lsb, unsigned char msb, unsigned long time);
			{
				tmpLong1 = GetLong("Channel > ");
				tmpLong2 = GetLong("LSB > ");
				tmpLong3 = GetLong("MSB > ");
				err = BAESong_ProgramChange(gSongs[slot], (unsigned char)tmpLong1, (unsigned char)tmpLong2, (unsigned long)tmpLong3);
			}
			break;

		case FUNC_ALL_NOTES_OFF:
		// BAESong_AllNotesOff(BAESong song, unsigned long time);
			{
				tmpLong1 = GetLong("Time > ");
				err = BAESong_AllNotesOff(gSongs[slot], (unsigned long)tmpLong1);
			}
			break;

		case FUNC_LOAD_GROOVOID:
		// BAESong_LoadGroovoid(BAESong song, char *cName, BAE_BOOL ignoreBadInstruments);
			{
				GetLine("Name > ", tmpBuffer, 256);
				tmpBool = GetBool("Ignore bad instruments? (y/n) > ");
				err = BAESong_LoadGroovoid(gSongs[slot], tmpBuffer, tmpBool);
			}
			break;

		case FUNC_LOAD_MIDI_FROM_MEMORY:
		// BAESong_LoadMidiFromMemory(BAESong song, void const* pMidiData, unsigned long midiSize, BAE_BOOL ignoreBadInstruments);
			{
				GetLine("Filepath > ", tmpBuffer, 256);

				if (bufferPtr)
				{
					free(bufferPtr);
					bufferPtr = NULL;
				}
				filePtr = fopen(tmpBuffer, "rb");
				if (filePtr != NULL)
				{
					// Find the length of the file
					fseek(filePtr, 0, SEEK_END);
					tmpLong1 = ftell(filePtr);
					// Return to the beginning
					fseek(filePtr, 0, SEEK_SET);

					// Now blast everything to a malloc'd buffer
					bufferPtr = malloc((size_t) tmpLong1);
					tmpLong2 = fread(bufferPtr, sizeof(char), tmpLong1, filePtr);
					if (tmpLong2 != tmpLong1)
					{
						/// Yikes, we have an anomaly -- could check ferror...
						printf("ERROR: File read error -- not BAE related...\n");
					}
					else
					{
						err = BAESong_LoadMidiFromMemory(gSongs[slot],
														bufferPtr,
														tmpLong2,
														TRUE);
					}
					fclose(filePtr);
				}
				else
				{
					printf("ERROR: Couldn't find file to open into memory -- not BAE related...\n");
				}
			}
			break;

		case FUNC_LOAD_MIDI_FROM_FILE:
		// BAESong_LoadMidiFromFile(BAESong song, BAEPathName filePath, BAE_BOOL ignoreBadInstruments);
			{
				GetLine("Filepath > ", tmpBuffer, 256);
				tmpBool = GetBool("Ignore bad instruments? (y/n) > ");
				err = BAESong_LoadMidiFromFile(gSongs[slot], tmpBuffer, tmpBool);
			}
			break;

		case FUNC_LOAD_RMF_FROM_MEMORY:
		// BAESong_LoadRmfFromMemory(BAESong song, void *pRMFData, unsigned long rmfSize, short int songIndex, BAE_BOOL ignoreBadInstruments);
			{
				GetLine("Filepath > ", tmpBuffer, 256);
				tmpLong3 = GetLong("Song index > ");

				if (bufferPtr)
				{
					free(bufferPtr);
					bufferPtr = NULL;
				}
				filePtr = fopen(tmpBuffer, "rb");
				if (filePtr != NULL)
				{
					// Find the length of the file
					fseek(filePtr, 0, SEEK_END);
					tmpLong1 = ftell(filePtr);
					// Return to the beginning
					fseek(filePtr, 0, SEEK_SET);

					// Now blast everything to a malloc'd buffer
					bufferPtr = malloc((size_t) tmpLong1);
					tmpLong2 = fread(bufferPtr, sizeof(char), tmpLong1, filePtr);
					if (tmpLong2 != tmpLong1)
					{
						/// Yikes, we have an anomaly -- could check ferror...
						printf("ERROR: File read error -- not BAE related...\n");
					}
					else
					{
						err = BAESong_LoadRmfFromMemory(gSongs[slot],
														bufferPtr,
														(unsigned long)tmpLong2,
														(short)tmpLong3,
														TRUE);
					}
					fclose(filePtr);
				}
				else
				{
					printf("ERROR: Couldn't find file to open into memory -- not BAE related...\n");
				}			
			}
			break;

		case FUNC_LOAD_RMF_FROM_FILE:
		// BAESong_LoadRmfFromFile(BAESong song, BAEPathName filePath, short int songIndex, BAE_BOOL ignoreBadInstruments);
			{
				GetLine("Filepath > ", tmpBuffer, 256);
				tmpLong1 = GetLong("Song index > ");
				tmpBool = GetBool("Ignore bad instruments? (y/n) > ");
				err = BAESong_LoadRmfFromFile(gSongs[slot], (BAEPathName)tmpBuffer, (short)tmpLong1, tmpBool);
			}
			break;

		case FUNC_PREROLL:
		// BAESong_Preroll(BAESong song);
			{
				err = BAESong_Preroll(gSongs[slot]);
			}
			break;

		case FUNC_START:
		// BAESong_Start(BAESong song, short int priority);
			{
				tmpLong1 = GetLong("Priority > ");
				err = BAESong_Start(gSongs[slot], (short)tmpLong1);
			}
			break;

		case FUNC_STOP:
		// BAESong_Stop(BAESong song, BAE_BOOL startFade);
			{
				tmpBool = GetBool("Start fade? (y/n) > ");
				err = BAESong_Stop(gSongs[slot], tmpBool);
			}
			break;

		case FUNC_PAUSE:
		// BAESong_Pause(BAESong song);
			{
				err = BAESong_Pause(gSongs[slot]);
			}
			break;

		case FUNC_RESUME:
		// BAESong_Resume(BAESong song);
			{
				err = BAESong_Resume(gSongs[slot]);
			}
			break;

		case FUNC_IS_PAUSED:
		// BAESong_IsPaused(BAESong song, BAE_BOOL *outIsPaused);
			{
				err = BAESong_IsPaused(gSongs[slot], &tmpBool);
				if (!err) printf(": IsPaused = %d\n", (int)tmpBool);
			}
			break;

		case FUNC_FADE:
		// BAESong_Fade(BAESong song, BAE_FIXED sourceVolume, BAE_FIXED destVolume, BAE_FIXED timeInMiliseconds);
			{
				tmpDouble1  = GetDouble("sourceVolume (1.0 norm) > ");
				tmpDouble2 = GetDouble("destVolume (1.0 norm) > ");
				tmpDouble3 = GetDouble("time (msec) > ");
				err = BAESong_Fade(gSongs[slot], FLOAT_TO_FIXED(tmpDouble1), FLOAT_TO_FIXED(tmpDouble2), FLOAT_TO_FIXED(tmpDouble3));
			}
			break;

		case FUNC_IS_DONE:
		// BAESong_IsDone(BAESong song, BAE_BOOL *outIsDone);
			{
				err = BAESong_IsDone(gSongs[slot], &tmpBool);
				if (!err) printf(": IsDone = %d\n", (int)tmpBool);
			}
			break;

		case FUNC_ARE_MIDI_EVENTS_PENDING:
		// BAESong_AreMidiEventsPending(BAESong song, BAE_BOOL *outPending);
			{
				err = BAESong_AreMidiEventsPending(gSongs[slot], &tmpBool);
				if (!err) printf(": Events Pending = %d\n", (int)tmpBool);
			}
			break;

		case FUNC_SET_LOOPS:
		// BAESong_SetLoops(BAESong song, short numLoops);
			{
				tmpLong1 = GetLong("Number of loops (0 = no repeats)> ");
				err = BAESong_SetLoops(gSongs[slot], (short)tmpLong1);
			}
			break;

		case FUNC_GET_LOOPS:
		// BAESong_GetLoops(BAESong song, short int *outNumLoops);
			{
				err = BAESong_GetLoops(gSongs[slot], (short *)&tmpLong1);
				if (!err) printf(": maxLoopCount = %d\n", (short)tmpLong1);
			}
			break;

		case FUNC_SET_LOOP_FLAG:
		// BAESong_SetLoopFlag(BAESong song, BAE_BOOL loop);
			{
//				tmpBool = GetBool("loop? (y/n) > ");
//				err = BAESong_SetLoopFlag(gSongs[slot], tmpBool);

				// NO LONGER SUPPORTED!
				err = BAE_GENERAL_BAD;
			}
			break;

		case FUNC_GET_LOOP_FLAG:
		// BAESong_GetLoopFlag(BAESong song, BAE_BOOL *outLoop);
			{
//				err = BAESong_GetLoopFlag(gSongs[slot], &tmpBool);
//				if (!err) printf(": Loop flag = %d\n", (int)tmpBool);

				// NO LONGER SUPPORTED!
				err = BAE_GENERAL_BAD;
			}
			break;

		case FUNC_SET_MICROSECOND_POSITION:
		// BAESong_SetMicrosecondPosition(BAESong song, unsigned long ticks);
			{
				tmpLong1 = GetLong("Ticks > ");
				err = BAESong_SetMicrosecondPosition(gSongs[slot], (unsigned long)tmpLong1);
			}
			break;

		case FUNC_GET_MICROSECOND_POSITION:
		// BAESong_GetMicrosecondPosition(BAESong song, unsigned long *outTicks);
			{
				err = BAESong_GetMicrosecondPosition(gSongs[slot], (unsigned long *)&tmpLong1);
				if (!err) printf(": Microsecond position = %ul\n", (unsigned long)tmpLong1);
			}
			break;

		case FUNC_GET_MICROSECOND_LENGTH:
		// BAESong_GetMicrosecondLength(BAESong song, unsigned long *outLength);
			{
				err = BAESong_GetMicrosecondLength(gSongs[slot], (unsigned long *)&tmpLong1);
				if (!err) printf(": Length = %ul\n", tmpLong1);
			}
			break;

		case FUNC_SET_MASTER_TEMPO:
		// BAESong_SetMasterTempo(BAESong song, BAE_UNSIGNED_FIXED tempoFactor);
			{
				tmpDouble1 = GetDouble("Tempo factor (1.0 norm) > ");
				err = BAESong_SetMasterTempo(gSongs[slot], FLOAT_TO_UNSIGNED_FIXED(tmpDouble1));
			}
			break;

		case FUNC_GET_MASTER_TEMPO:
		// BAESong_GetMasterTempo(BAESong song, BAE_UNSIGNED_FIXED *outTempoFactor);
			{
				err = BAESong_GetMasterTempo(gSongs[slot], &tmpUFixed);
				if (!err) printf(": Tempo factor = %g\n", UNSIGNED_FIXED_TO_FLOAT(tmpUFixed));
			}
			break;

		case FUNC_MUTE_TRACK:
		// BAESong_MuteTrack(BAESong song, unsigned short int track);
			{
				tmpLong1 = GetLong("Track > ");
				err = BAESong_MuteTrack(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_UNMUTE_TRACK:
		// BAESong_UnmuteTrack(BAESong song, unsigned short int track);
			{
				tmpLong1 = GetLong("Track > ");
				err = BAESong_UnmuteTrack(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_GET_TRACK_MUTE_STATUS:
		// BAESong_GetTrackMuteStatus(BAESong song, BAE_BOOL *outTracks);
			{				
				BAE_BOOL result[65];
				int i,j;

				err = BAESong_GetTrackMuteStatus(gSongs[slot], result);
				if (!err)
				{
					for (j=0; j<4; j++)
					{
						printf("track:       %d0%d1%d2%d3%d4%d5%d6%d7%d8%d9%dA%dB%dC%dD%dE%dF\n",j,j,j,j,j,j,j,j,j,j,j,j,j,j,j,j);
						printf("mute status: ");
						for (i=0; i<16; i++)
						{
							printf(" %d", (int)result[j*16 + i]);
						}
						printf("\n\n");
					}
				}
			}
			break;

		case FUNC_SOLO_TRACK:
		// BAESong_SoloTrack(BAESong song, unsigned short int track);
			{
				tmpLong1 = GetLong("Track > ");
				err = BAESong_SoloTrack(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_UNSOLO_TRACK:
		// BAESong_UnSoloTrack(BAESong song, unsigned short int track);
			{
				tmpLong1 = GetLong("Track > ");
				err = BAESong_UnSoloTrack(gSongs[slot], (unsigned short)tmpLong1);
			}
			break;

		case FUNC_GET_SOLO_TRACK_STATUS:
		// BAESong_GetSoloTrackStatus(BAESong song, BAE_BOOL *outTracks);
			{
				BAE_BOOL result[65];
				int i,j;

				err = BAESong_GetSoloTrackStatus(gSongs[slot], result);
				if (!err)
				{
					for (j=0; j<4; j++)
					{
						printf("track:       %d0%d1%d2%d3%d4%d5%d6%d7%d8%d9%dA%dB%dC%dD%dE%dF\n",j,j,j,j,j,j,j,j,j,j,j,j,j,j,j,j);
						printf("solo status: ");
						for (i=0; i<16; i++)
						{
							printf(" %d", (int)result[j*16 + i]);
						}
						printf("\n\n");
					}
				}
			}
			break;

		default:
			break;
	}

	if (err)
	{
		printf("\a! Error code: %s (%d)\n", BAEResultToStr(err), (int)err);
	}
	else
	{
		printf("OK.\n");
	}
}



// DoScript()
// -----------------------------------------------------
//
//
static void PV_DoScript(int subMenu)
{
	BAEResult err;

	err = BAE_NO_ERROR;
	switch (subMenu)
	{
		case SCRIPT_ALL: // "Do all tests"
//			PV_TestAll();
			break;

		case SCRIPT_NEW: // "Test New/Delete"
//			PV_TestConstructor();
			break;

		case SCRIPT_LOAD_SONG: // "Test Loading Songs"
//			PV_TestLoadSong();
			break;
		
		case SCRIPT_KEYBOARD:
			PV_TestKeyboard();
			break;

		case SCRIPT_NOTE_ON:
//			PV_TestNoteOn();
			break;

		case SCRIPT_SHOW_LOADED_INSTS:
			{
				long slot;
				BAE_BOOL isLoaded;
				int i;

				slot = GetLong("Slot: ");
				if (gSongs[slot])
				{
					printf("  Loaded instruments: ");

					// These numbers are hardcoded into the engine,
					// and currently not exposed in MiniBAE.h,
					//
					// Max number of instruments = 256
					// Max number of banks = 3
					// Max number of instruments = 256*3 = 768
					for (i=0; i<768; i++)
					{
						err = BAESong_IsInstrumentLoaded(gSongs[slot], i, &isLoaded);
						if (err)
						{
							printf("err = %d\n", (long)err);
						}
						else
						{
							if (isLoaded) printf("%d, ", i);
						}
					}
					printf("\n");
				}
				else
				{
					printf("No song in slot %d\n", slot);
				}
			}
			break;

		case SCRIPT_LOAD_DEFAULT_MIDI:
		case SCRIPT_LOAD_DEFAULT_MIDI1:
		case SCRIPT_LOAD_DEFAULT_MIDI2:
		case SCRIPT_LOAD_DEFAULT_MIDI3:
			{
				if (gSongs[0] == NULL)
				{
					gSongs[0] = BAESong_New(gMixer);
				}
				switch(subMenu)
				{
					case SCRIPT_LOAD_DEFAULT_MIDI:
						err = BAESong_LoadMidiFromFile(gSongs[0], DEFAULT_MIDI_FILE, TRUE);
						break;
					case SCRIPT_LOAD_DEFAULT_MIDI1:
						err = BAESong_LoadMidiFromFile(gSongs[0], DEFAULT_MIDI_FILE1, TRUE);
						break;
					case SCRIPT_LOAD_DEFAULT_MIDI2:
						err = BAESong_LoadMidiFromFile(gSongs[0], DEFAULT_MIDI_FILE2, TRUE);
						break;
					case SCRIPT_LOAD_DEFAULT_MIDI3:
						err = BAESong_LoadMidiFromFile(gSongs[0], DEFAULT_MIDI_FILE3, TRUE);
						break;
				}
			}
			break;

		case SCRIPT_LOAD_DEFAULT_MPEG_RMF:
			{
				if (gSongs[0] == NULL)
				{
					gSongs[0] = BAESong_New(gMixer);
				}
				err = BAESong_LoadRmfFromFile(gSongs[0], DEFAULT_RMF_MPEG_FILE, 0, TRUE);
			}
			break;

		case SCRIPT_LOAD_DEFAULT_RMF:
			{
				if (gSongs[0] == NULL)
				{
					gSongs[0] = BAESong_New(gMixer);
				}
				err = BAESong_LoadRmfFromFile(gSongs[0], DEFAULT_RMF_FILE, 0, TRUE);
			}
			break;

		case SCRIPT_LOAD_DEFAULT_GROOVOID:
			{
				if (gSongs[0] == NULL)
				{
					gSongs[0] = BAESong_New(gMixer);
				}
				err = BAESong_LoadGroovoid(gSongs[0], DEFAULT_GROOVOID, TRUE);
			}
			break;

		default:
			break;
	}

	if (err)
	{
		printf("\a! Error code: %s (%d)\n", BAEResultToStr(err), (int)err);
	}
	else
	{
		printf("OK.\n");
	}
}


static void PV_TestAll(void)
{
}


static void PV_TestConstructor(void)
{
	BAESong		theSong;
	BAEResult	err;
	int			i;

	theSong = NULL;

	err = BAESong_Delete(theSong);	// test deleting a NULL song
	if (err != BAE_NULL_OBJECT)
	{
		FailedTest("BAESong_Delete(NULL) didn't return BAE_NULL_OBJECT");
	}

	for (i=0; i<100; i++)
	{
		theSong = BAESong_New(gMixer);

		if (gMixer)
		{
			if (!theSong)
			{
				FailedTest("BAESong_New() returned NULL with valid BAEMixer");
				break;
			}
		}
		else
		{
			if (theSong)
			{
				FailedTest("BAESong_New() didn't return NULL with NULL BAEMixer");
				break;
			}
		}
		err = BAESong_Delete(theSong);
	}
}


static void PV_TestLoadSong(void)
{
	BAESong		song0, song1, song2, song3;
	BAEResult	err;

	song0 = NULL;
	err = BAESong_LoadMidiFromFile(song0, "", TRUE);
	if (err != BAE_NULL_OBJECT)
	{
		FailedTest("BAESong_LoadMidiFromFile() didn't return BAE_NULL_OBJECT");
	}

	err = BAESong_LoadRmfFromFile(song0, "", 1, TRUE);
	if (err != BAE_NULL_OBJECT)
	{
		FailedTest("BAESong_LoadRmfFromFile() didn't return BAE_NULL_OBJECT");
	}

//	err = BAESong_LoadMidiFromMemory(song0, "", TRUE);
//	if (err != BAE_NULL_OBJECT)
//	{
//		FailedTest("BAESong_LoadMidiFromMemory() didn't return BAE_NULL_OBJECT");
//	}

//	err = BAESong_LoadRmfFromMemory(song0, "", 1, TRUE);
//	if (err != BAE_NULL_OBJECT)
//	{
//		FailedTest("BAESong_LoadRmfFromMemory() didn't return BAE_NULL_OBJECT");
//	}

	err = BAESong_LoadGroovoid(song0, "", TRUE);
	if (err != BAE_NULL_OBJECT)
	{
		FailedTest("BAESong_LoadGroovoid() didn't return BAE_NULL_OBJECT");
	}

	song0 = BAESong_New(gMixer);
	song1 = BAESong_New(gMixer);
	song2 = BAESong_New(gMixer);
	song3 = BAESong_New(gMixer);


	err = BAESong_LoadMidiFromFile(song0, "testMidi1.mid", TRUE);
	if (err == BAE_NO_ERROR)
	{
		err = BAESong_Start(song0, 0);
	}
	else
	{

	}

	err = BAESong_LoadMidiFromFile(song1, "testMidi2.mid", TRUE);
	if (err == BAE_NO_ERROR)
	{
		err = BAESong_Start(song1, 0);
	}
	else
	{

	}

	err = BAESong_LoadMidiFromFile(song2, "fakefile.mid", TRUE);
	if (err == BAE_NO_ERROR)
	{

	}
	else
	{

	}

	err = BAESong_LoadRmfFromFile(song3, "testRmf1.rmf", 1, TRUE);
	if (err == BAE_NO_ERROR)
	{

	}
	else
	{

	}
}


static void PV_TestKeyboard(void)
{
	BAESong song;
	int ch;
	unsigned char channelNum;
	unsigned char progNum;
	unsigned char bankNum;
	BAEResult err;

	song = BAESong_New(gMixer);
	if (song)
	{
		progNum = 0;
		bankNum = 0;
		channelNum = 1;


		err = BAESong_ProgramBankChange(song, channelNum, progNum, bankNum, 0);
		if (err == BAE_NO_ERROR)
		{
			printf("Press '-' for prev program, '+' for next program\n");
			printf("Press '[' for prev bank, ']' for next bank\n");
			printf("Press '0' to exit.\n\n");
			printf("Bank %d, program %d\n", bankNum, progNum);

			while ((ch=_getch())!=0)
			{
				if (ch == '0' || ch == ')') break;
	
				switch (ch)
				{
					case 10:	// ignore
						break;
					case '=':
					case '+':
						// increase program number
						err = BAESong_ProgramChange(song, channelNum, ++progNum, 0);
						if (err == BAE_NO_ERROR)
						{
							printf("Bank %d, program %d\n", bankNum, progNum);
						}
						else
						{
							FailedTest("BAESong_ProgramChange() failed");
						}
						break;
	
					case '_':
					case '-':
						// decrease program number
						err = BAESong_ProgramChange(song, 1, --progNum, 0);
						if (err == BAE_NO_ERROR)
						{
							printf("Bank %d, program %d\n", bankNum, progNum);
						}
						else
						{
							FailedTest("BAESong_ProgramChange() failed");
						}
						break;
	
					case '[':
					case '{':
						// decrease bank number
						if (bankNum > 0) 
						{
							--bankNum;
						}
						err = BAESong_ProgramBankChange(song, channelNum, progNum, bankNum, 0);
						if (err == BAE_NO_ERROR)
						{
							printf("Bank %d, program %d\n", bankNum, progNum);
						}
						else
						{
							FailedTest("BAESong_ProgramBankChange() failed");
						}
						break;

					case ']':
					case '}':
						// increase bank number
						if (bankNum < 2) 
						{
							++bankNum;
						}
						err = BAESong_ProgramBankChange(song, channelNum, progNum, bankNum, 0);
						if (err == BAE_NO_ERROR)
						{
							printf("Bank %d, program %d\n", bankNum, progNum);
						}
						else
						{
							FailedTest("BAESong_ProgramBankChange() failed");
						}
						break;

					default:
						printf("key = %d, '%c'\n", ch, ch);
						err = BAESong_NoteOnWithLoad(song, 1, (unsigned char)ch, 100, 0);
						break;
				}
			}
		}
		else
		{
			FailedTest("BAESong_ProgramBankChange() failed");
		}
		BAESong_Delete(song);
		song = NULL;
	}
	else
	{
		FailedTest("Couldn't allocate BAESong object");
	}
}


static void PV_TestNoteOn(void)
{
	BAESong song;
	BAEResult err;
	unsigned char i;

	song = BAESong_New(gMixer);
	if (song != NULL)
	{
		printf("Playing all notes (no instruments loaded):\n");
		for (i=0; i<127; i++)
		{
			printf("%d,", i);
//			err = BAESong_ProgramBankChange(song, 0, i, 0, 0);
			err = BAESong_NoteOn(song, 0, i, 100, 0);
			if (err)
			{
				printf("Error: %s\n", BAEResultToStr(err));
			}
			BAE_WaitMicroseconds(150000);
		}

		printf("Playing all notes (loading instruments):\n");
		err = BAESong_ProgramBankChange(song, 0, 28, 0, 0);
		for (i=0; i<127; i++)
		{
			printf("%d,", i);
//			err = BAESong_ProgramBankChange(song, 0, i, 0, 0);
//							unsigned char channel,
//							unsigned char programNumber,
//							unsigned char bankNumber,
//							unsigned long time)
			if (err)
			{
				printf("Error: %s\n", BAEResultToStr(err));
			}
			err = BAESong_NoteOnWithLoad(song, 0, i, 100, 0);
//							unsigned char channel, 
//							unsigned char note, 
//							unsigned char velocity,
//							unsigned long time)
			
			if (err)
			{
				printf("Error: %s\n", BAEResultToStr(err));
			}
			BAE_WaitMicroseconds(200000);
		}

		BAESong_Delete(song);
	}
	else
	{
		FailedTest("Couldn't allocate BAESong object");
	}
}
