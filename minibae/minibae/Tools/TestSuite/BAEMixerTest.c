/*****************************************************************************/
/*
** "BAEMixerTest.c"
**
**	© Copyright 1999-2000 Beatnik, Inc, All Rights Reserved.
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
**	2000.01.13 MSD	removed call to GetPath in AddBank script
**	2000.01.14		Added support for BAEMixer_IsAudioActive()
**					Added Function Tests
**	2000.01.17		Added more scripts and tests
**	2000.03.01		Changed SetBank() to AddBank()
**  2000.03.06 MSD	Added support for 32kHz, 40kHz
**	2000.03.07 msd	Placed a test mixer script to load the small bank with npatches.hsb
**					Also changed the bank add scripts to UnloadBanks prior to adding.
**	2000.03.15 msd	changed mix level in Open scripts to 7 from 4.
**	2000.03.28 sh	Added a new mixer script to open it with 8 voices.
**	2000.03.29 msd	now correctly using bank tokens with BAEMixer_GetBankVersion()
**					Changed copyright and modification history format
**	2000.04.19 msd	added scripts to open mixer at 16kHz.
**	8/23/2000	sh	Added new scripts for a mixer control
*/
/*****************************************************************************/


#include "TestSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "TestTools.h"
#include "BAEMixerTest.h"
#include "BAE_API.h"

// These two variables are used when loading a bank into memory.
static void				*gBankBlock = NULL;
static unsigned long	gBankBlockLength = 0;

#define NUM_SCRIPTS			15
#define NUM_FUNCTIONS		44

static const char *ScriptList[NUM_SCRIPTS] = 
{
	"Open Mixer (8 kHz, 16 bit, mono) 7 midi, 1 pcm, 7 mix level)",
	"Open Mixer (22 kHz, 16 bit, mono) 7 midi, 1 pcm, 7 mix level)",
	"Close and delete Mixer",
	"Set default bank to 'patches.hsb'",
	"Display loop of CPU load in percent",
	"Display loop of sample frame output",
	"Display loop of real time status",
	"Display device info",
	"Display mixer info",
	"Display Bank info",
	"Set default bank to small test case bank 'npatches.hsb'",
	"Open Mixer (8 kHz, 16 bit, mono) 8 midi, 0 pcm, 7 mix level)",
	"Open Mixer (16 kHz,16 bit, mono) 7 midi, 1 pcm, 7 mix level)",
	"Open Mixer (16 kHz,16 bit, mono) 8 midi, 0 pcm, 7 mix level)",
	"Open Mixer (22 kHz, 16 bit, mono) 8 midi, 0 pcm, 7 mix level)"
};

enum
{
	SCRIPT_MIXER_OPEN_8_16_MONO = 1,
	SCRIPT_MIXER_OPEN_22_16_MONO,
	SCRIPT_MIXER_CLOSE,
	SCRIPT_MIXER_SET_DEFAULT_BIG_BANK,
	SCRIPT_MIXER_DISPLAY_CPU_LOAD,
	SCRIPT_MIXER_DISPLAY_SAMPLE_FRAMES,
	SCRIPT_MIXER_DISPLAY_REALTIME_STATUS,
	SCRIPT_MIXER_GET_DEVICE_INFO,
	SCRIPT_MIXER_GET_MIXER_INFO,
	SCRIPT_MIXER_GET_BANK_INFO,
	SCRIPT_MIXER_SET_DEFAULT_SMALL_BANK,
	SCRIPT_MIXER_OPEN_8_8_16_MONO,
	SCRIPT_MIXER_OPEN_16_16_7_1_MONO,
	SCRIPT_MIXER_OPEN_16_16_8_8_MONO,
	SCRIPT_MIXER_OPEN_22_16_8_MONO
};

static const char *FunctionList[NUM_FUNCTIONS] = 
{
	"BAEMixer_New()",							//
	"BAEMixer_Delete()",						//
	"BAEMixer_IsOpen()",						//
	"BAEMixer_Open()",							//
	"BAEMixer_Close()",							//
	"BAEMixer_GetMixerVersion()",				//
	"BAEMixer_GetMaxDeviceCount()",				//
	"BAEMixer_SetCurrentDevice()",					//
	"BAEMixer_GetCurrentDevice()",				//
	"BAEMixer_GetDeviceName()",					//
	"BAEMixer_Is16BitSupported()",				//
	"BAEMixer_Is8BitSupported()",				//
	"BAEMixer_IsAudioEngaged()",				//
	"BAEMixer_DisengageAudio()",				
	"BAEMixer_ReengageAudio()",					
	"BAEMixer_IsAudioActive()",					//
	"BAEMixer_AddBankFromFile()",					
	"BAEMixer_AddBankFromMemory()",				
	"BAEMixer_UnloadBank()",					
	"BAEMixer_GetBankVersion()",				
	"BAEMixer_GetGroovoidNameFromBank()",		
	"BAEMixer_ChangeAudioModes()",				
	"BAEMixer_GetQuality()",					//
	"BAEMixer_GetTerpMode()",					//
	"BAEMixer_GetModifiers()",					//
	"BAEMixer_ChangeSystemVoices()",			
	"BAEMixer_GetMidiVoices()",					//
	"BAEMixer_GetSoundVoices()",				//
	"BAEMixer_GetMixLevel()",					//
	"BAEMixer_GetTick()",						
	"BAEMixer_SetAudioLatency()",				
	"BAEMixer_GetAudioLatency()",				//
	"BAEMixer_SetMasterVolume()",				
	"BAEMixer_GetMasterVolume()",				//
	"BAEMixer_SetHardwareVolume()",				
	"BAEMixer_GetHardwareVolume()",				//
	"BAEMixer_SetMasterSoundEffectsVolume()",	
	"BAEMixer_GetMasterSoundEffectsVolume()",	//
	"BAEMixer_GetAudioSampleFrame()",			
	"BAEMixer_GetRealtimeStatus()",				
	"BAEMixer_GetCPULoadInMicroseconds()",		
	"BAEMixer_GetCPULoadInPercent()",
	"BAEMixer_BringBankToFront()",
	"BAEMixer_SendBankToBack()"
};

enum
{
	FUNC_MIXER_NEW = 1,						
	FUNC_MIXER_DELETE,						
	FUNC_MIXER_ISOPEN,						
	FUNC_MIXER_OPEN,						
	FUNC_MIXER_CLOSE,						
	FUNC_MIXER_GETMIXERVERSION,				
	FUNC_MIXER_GETMAXDEVICECOUNT,			
	FUNC_MIXER_SETCURRENTDEVICE,				
	FUNC_MIXER_GETCURRENTDEVICE,				
	FUNC_MIXER_GETDEVICENAME,					
	FUNC_MIXER_IS16BITSUPPORTED,				
	FUNC_MIXER_IS8BITSUPPORTED,				
	FUNC_MIXER_ISAUDIOENGAGED,				
	FUNC_MIXER_DISENGAGEAUDIO,				
	FUNC_MIXER_REENGAGEAUDIO,
	FUNC_MIXER_ISAUDIOACTIVE,			
	FUNC_MIXER_ADDBANKFROMFILE,				
	FUNC_MIXER_ADDBANKFROMMEMORY,				
	FUNC_MIXER_UNLOADBANK,					
	FUNC_MIXER_GETBANKVERSION,				
	FUNC_MIXER_GETGROOVOIDNAMEFROMBANK,		
	FUNC_MIXER_CHANGEAUDIOMODES,			
	FUNC_MIXER_GETQUALITY,					
	FUNC_MIXER_GETTERPMODE,					
	FUNC_MIXER_GETMODIFIERS,				
	FUNC_MIXER_CHANGESYSTEMVOICES,			
	FUNC_MIXER_GETMIDIVOICES,				
	FUNC_MIXER_GETSOUNDVOICES,				
	FUNC_MIXER_GETMIXLEVEL,					
	FUNC_MIXER_GETTICK,						
	FUNC_MIXER_SETAUDIOLANTENCY,			
	FUNC_MIXER_GETAUDIOLANTENCY,			
	FUNC_MIXER_SETMASTERVOLUME,				
	FUNC_MIXER_GETMASTERVOLUME,				
	FUNC_MIXER_SETHARDWAREVOLUME,			
	FUNC_MIXER_GETHARDWAREVOLUME,			
	FUNC_MIXER_SETMASTERSOUNDEFFECTSVOLUME,	
	FUNC_MIXER_GETMASTERSOUNDEFFECTSVOLUME,	
	FUNC_MIXER_GETAUDIOSAMPLEFRAME,			
	FUNC_MIXER_GETREALTIMESTATUS,			
	FUNC_MIXER_GETCPULOADINMICROSECONDS,	
	FUNC_MIXER_GETCPULOADINPERCENT,
	FUNC_MIXER_BRINGBANKTOFRONT,
	FUNC_MIXER_SENDBANKTOBACK
};


enum
{
	TEST_MIXER_CONSTRUCTOR = 1,
	TEST_MIXER_OPEN_CLOSE,
	TEST_MIXER_GET_SET_MASTER_VOLUME,
	TEST_MIXER_GET_SET_HARDWARE_VOLUME,
	NUM_TESTS_PLUS_ONE,
	NUM_TESTS = NUM_TESTS_PLUS_ONE - 1
};

static const char *TestList[NUM_TESTS] = 
{
	"Test Constructor",
	"Test Open/Close",
	"Test Get/Set Master Volume",
	"Test Get/Set Hardware Volume"
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
static BAEResult DoScript(int subMenu);
static BAEResult DoFunction(int subMenu);
static BAEResult DoTest(int subMenu);

static BAEResult TestOpenClose(void);
static BAEResult TestGetSetMasterVolume(void);
static BAEResult TestGetSetHardwareVolume(void);

////////////////////////////////////////////////////////////////////////////////
// Exported functions
////////////////////////////////////////////////////////////////////////////////


// MixerTest_GetNumMenuItems()
// -----------------------------------------------------
//
//
int MixerTest_GetNumMenuItems(void)
{
	return 3;
}



// MixerTest_GetNumSubMenuItems()
// -----------------------------------------------------
//
//
int MixerTest_GetNumSubMenuItems(int menu)
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

		case 3:
			result = NUM_TESTS;
			break;

		default:
			break;
	}
	return result;
}



// MixerTest_GetMenuItem()
// -----------------------------------------------------
//
//
int MixerTest_GetMenuItem(int menu, const char **outMenuItem)
{
	int result;

	result = 0;
	if (outMenuItem)
	{
		switch (menu)
		{
			case 1:
				*outMenuItem = "Test scripts";
				result = 1;
				break;
		
			case 2:
				*outMenuItem = "Direct function calls";
				result = 1;
				break;

			case 3:
				*outMenuItem = "Function tests";
				result = 1;
				break;

			default:
				break;
		}
	}
	return result;
}



// MixerTest_GetSubMenuItem()
// -----------------------------------------------------
//
//
int MixerTest_GetSubMenuItem(int menu, int subMenu, const char **outMenuItem)
{
	int result;

	result = 0;
	if (outMenuItem)
	{
		switch (menu)
		{
			case 1: // Test Scripts
				if (subMenu > 0 && subMenu <= NUM_SCRIPTS)
				{
					*outMenuItem = ScriptList[subMenu-1];
					result = 1;
				}
				break;

			case 2: // Direct function calls
				if (subMenu > 0 && subMenu <= NUM_FUNCTIONS)
				{
					*outMenuItem = FunctionList[subMenu-1];
					result = 1;
				}
				break;

			case 3:
				if (subMenu > 0 && subMenu <= NUM_TESTS)
				{
					*outMenuItem = TestList[subMenu-1];
					result = 1;
				}
				break;
	
			default:
				break;
		}
	}
	return result;
}



// MixerTest_DoMenuItem()
// -----------------------------------------------------
//
//
void MixerTest_DoMenuItem(int menu, int subMenu)
{
	BAEResult err;

	err = BAE_NO_ERROR;
	switch (menu)
	{
		case 1:
			err = DoScript(subMenu);
			break;
		
		case 2:
			err = DoFunction(subMenu);
			break;

		case 3:
			err = DoTest(subMenu);
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



////////////////////////////////////////////////////////////////////////////////
// Static functions
////////////////////////////////////////////////////////////////////////////////


// DoScript()
// -----------------------------------------------------
//
//
static BAEResult DoScript(int subMenu)
{
	BAEResult			err;
	BAE_BOOL			validerr;
	short				tmpShort1,
						tmpShort2,
						tmpShort3;
	long				tmpLong1,
						tmpLong2;
	char				buffer[256];
	BAE_BOOL			tmpBool1;
	BAE_UNSIGNED_FIXED	tmpUFixed1;
	short int			midiVoices, pcmVoices;
	BAEQuality			q;
	BAEAudioModifiers	s;

	validerr = TRUE;
	err = BAE_NO_ERROR;
	s = BAE_NONE;
	switch (subMenu)
	{
		case SCRIPT_MIXER_OPEN_8_16_MONO:
		case SCRIPT_MIXER_OPEN_22_16_MONO:
		case SCRIPT_MIXER_OPEN_8_8_16_MONO:
		case SCRIPT_MIXER_OPEN_16_16_7_1_MONO:
		case SCRIPT_MIXER_OPEN_16_16_8_8_MONO:
		case SCRIPT_MIXER_OPEN_22_16_8_MONO:
			{
				switch (subMenu)
				{
					case SCRIPT_MIXER_OPEN_8_8_16_MONO:
					case SCRIPT_MIXER_OPEN_8_16_MONO:
						q = BAE_8K;
						break;
					case SCRIPT_MIXER_OPEN_22_16_8_MONO:
						q = BAE_22K;
						break;
					case SCRIPT_MIXER_OPEN_22_16_MONO:
						q = BAE_22K;
						break;
					case SCRIPT_MIXER_OPEN_16_16_7_1_MONO:
					case SCRIPT_MIXER_OPEN_16_16_8_8_MONO:
						q = BAE_16K;
						break;
					default:
						return BAE_PARAM_ERR;
				}	
	
				switch (subMenu)
				{
					case SCRIPT_MIXER_OPEN_22_16_8_MONO:
						midiVoices = 8;
						pcmVoices = 0;
						break;
					
					case SCRIPT_MIXER_OPEN_8_16_MONO:
					case SCRIPT_MIXER_OPEN_22_16_MONO:
					case SCRIPT_MIXER_OPEN_16_16_7_1_MONO:
						midiVoices = 7;
						pcmVoices = 1;
						break;
						
					case SCRIPT_MIXER_OPEN_8_8_16_MONO:
					case SCRIPT_MIXER_OPEN_16_16_8_8_MONO:
						midiVoices = 8;
						pcmVoices = 0;
						break;
					default:
						return BAE_PARAM_ERR;
				}

				if (gMixer == NULL)
				{
					gMixer = BAEMixer_New();
				}
				else
				{
					printf("Mixer already allocated\n");
					validerr = FALSE;
				}
				err = BAEMixer_Open(gMixer, q, BAE_LINEAR_INTERPOLATION,BAE_USE_16 | s,
												midiVoices,
												pcmVoices,
												7,
												TRUE);
				printf("\n");
			}
			break;
		case SCRIPT_MIXER_CLOSE:
			DoFunction(FUNC_MIXER_DELETE);
			validerr = FALSE;
			break;

		case SCRIPT_MIXER_SET_DEFAULT_SMALL_BANK:
			{
				BAEBankToken token;
			
				err = BAEMixer_UnloadBanks(gMixer);	// free all banks
				if (err)
				{
					printf("Error unloading banks (%d)\n", err);
				}
				err = BAEMixer_AddBankFromFile(gMixer, "npatches.hsb", &token);
				if (err == 0)
				{
					printf("token = %d\n", token);
				}
			}
			break;
		case SCRIPT_MIXER_SET_DEFAULT_BIG_BANK:
			{
				BAEBankToken token;
			
				err = BAEMixer_UnloadBanks(gMixer);	// free all banks
				if (err)
				{
					printf("Error unloading banks (%d)\n", err);
				}
				err = BAEMixer_AddBankFromFile(gMixer, "patches.hsb", &token);
				if (err == 0)
				{
					printf("token = %d\n", token);
				}
			}
			break;
		case SCRIPT_MIXER_DISPLAY_CPU_LOAD:
			{
				int	count;

				for (count = 0; count < 30; count++)
				{
					DoFunction(FUNC_MIXER_GETCPULOADINPERCENT);
				}
			}
			break;

		case SCRIPT_MIXER_DISPLAY_SAMPLE_FRAMES:
			{
				int	count;

				for (count = 0; count < 30; count++)
				{
					DoFunction(FUNC_MIXER_GETAUDIOSAMPLEFRAME);
				}
			}
			break;
		
		case SCRIPT_MIXER_DISPLAY_REALTIME_STATUS:
			{
				int	count;

				for (count = 0; count < 30; count++)
				{
					DoFunction(FUNC_MIXER_GETREALTIMESTATUS);
				}
			}
			break;
		
		case SCRIPT_MIXER_GET_DEVICE_INFO:
			{
				err = BAEMixer_GetMaxDeviceCount(gMixer, &tmpLong1);
				if (err) break;
				printf("Num devices = %d\n", tmpLong1);

				for (tmpLong2=0; tmpLong2<tmpLong1; tmpLong2++)
				{
					err = BAEMixer_GetDeviceName(gMixer, tmpLong2, buffer, 256);
					if (err) break;
					printf("  device %d: %s\n", tmpLong2, buffer);
				}

				err = BAEMixer_GetCurrentDevice(gMixer, NULL, &tmpLong1);
				if (err) break;
				printf("Current device:       %d\n", tmpLong1);

				err = BAEMixer_Is16BitSupported(gMixer, &tmpBool1);
				if (err) break;
				printf("  16-bit support?     %s\n", (tmpBool1) ? "Yes" : "No");

				err = BAEMixer_Is8BitSupported(gMixer, &tmpBool1);
				if (err) break;
				printf("  8-bit support?      %s\n", (tmpBool1) ? "Yes" : "No");

				err = BAEMixer_GetAudioLatency(gMixer, (unsigned long *)&tmpLong1);
				if (err) break;
				printf("  Latency:            %d milliseconds\n", tmpLong1);

				err = BAEMixer_GetMasterVolume(gMixer, &tmpUFixed1);
				if (err) break;
				printf("  Master volume:      %g\n", FIXED_TO_FLOAT(tmpUFixed1));

				err = BAEMixer_GetHardwareVolume(gMixer, &tmpUFixed1);
				if (err) break;
				printf("  Hardware volume:    %g\n", FIXED_TO_FLOAT(tmpUFixed1));

				err = BAEMixer_GetMasterSoundEffectsVolume(gMixer, &tmpUFixed1);
				if (err) break;
				printf("  Master Sfx volume:  %g\n", FIXED_TO_FLOAT(tmpUFixed1));

			}
			break;

		case SCRIPT_MIXER_GET_MIXER_INFO:
			{
				BAEQuality q;
				BAETerpMode t;

				err = BAEMixer_GetMixerVersion(gMixer, &tmpShort1, &tmpShort2, &tmpShort3);
				if (err) break;
				printf("\nBAEMixer v%d.%d.%d\n", tmpShort1, tmpShort2, tmpShort3);

				err = BAEMixer_IsOpen(gMixer, &tmpBool1);
				if (err) break;
				printf("  Is Open?            %s\n", (tmpBool1) ? "Yes" : "No");

				err = BAEMixer_GetQuality(gMixer, &q);
				if (err) break;
				printf("  Quality:            %s\n", BAEQualityToStr(q));
				
				err = BAEMixer_GetTerpMode(gMixer, &t);
				if (err) break;
				printf("  Terp mode:          %s\n", BAETerpModeToStr(t));
				
				err = BAEMixer_GetModifiers(gMixer, &tmpLong1);
				if (err) break;
				printf("  Modifiers:          %x\n", tmpLong1);
				
				err = BAEMixer_GetMidiVoices(gMixer, &tmpShort1);
				if (err) break;
				printf("  Midi voices:        %d\n", tmpShort1);
				
				err = BAEMixer_GetSoundVoices(gMixer, &tmpShort1);
				if (err) break;
				printf("  Sound voices:       %d\n", tmpShort1);

				err = BAEMixer_GetMixLevel(gMixer, &tmpShort1);
				if (err) break;
				printf("  Mix level:          %d\n", tmpShort1);

				err = BAEMixer_IsAudioEngaged(gMixer, &tmpBool1);
				if (err) break;
				printf("  Audio engaged?      %s\n", (tmpBool1) ? "Yes" : "No");

				err = BAEMixer_IsAudioActive(gMixer, &tmpBool1);
				if (err) break;
				printf("  Audio active?       %s\n", (tmpBool1) ? "Yes" : "No");
			}
			break;

		case SCRIPT_MIXER_GET_BANK_INFO:
			{
				tmpLong1 = GetLong(">>Bank token: ");
				err = BAEMixer_GetBankVersion(gMixer, (BAEBankToken)tmpLong1, &tmpShort1, &tmpShort2, &tmpShort3);
				if (err) break;
				printf("Bank version %d.%d.%d\n", tmpShort1, tmpShort2, tmpShort3);


			}
			break;

		default:
			break;
	}
	return err;
}


// DoFunction()
// -----------------------------------------------------
//
//
static BAEResult DoFunction(int subMenu)
{
	BAEResult			err;
	BAE_BOOL			validerr;

	validerr = TRUE;
	err = BAE_NO_ERROR;

	switch (subMenu)
	{
// BAEMixer BAEMixer_New(void);
		case FUNC_MIXER_NEW:
			if (gMixer == NULL)
			{
				gMixer = BAEMixer_New();
			}
			else
			{
				printf("Mixer already allocated\n");
				validerr = FALSE;
			}
			break;
// BAEResult BAEMixer_Delete(BAEMixer mixer);
		case FUNC_MIXER_DELETE:
			err = BAEMixer_Delete(gMixer);
			gMixer = NULL;
			if (gBankBlock)
			{
				free(gBankBlock);
				gBankBlock = NULL;
			}
			break;
// BAEResult BAEMixer_IsOpen(BAEMixer mixer,BAE_BOOL *outIsOpen);
		case FUNC_MIXER_ISOPEN:
			{
				BAE_BOOL	open;

				err = BAEMixer_IsOpen(gMixer, &open);
				printf("= %s\n", (open) ? "TRUE" : "FALSE");
			}				
			break;
// BAEResult BAEMixer_Open(BAEMixer mixer,BAEQuality q,BAETerpMode t,BAEAudioModifiers am,short int maxMidiVoices,short int maxSoundVoices,short int mixLevel,BAE_BOOL engageAudio);
		case FUNC_MIXER_OPEN:
			{
				BAEQuality			q;
				BAETerpMode			t;
				BAEAudioModifiers	am;
				long				maxSongVoices, maxSoundVoices, level;
				BAE_BOOL			engageAudio;
				BAE_BOOL			ok;

				ok = FALSE;

				printf("\n");
				while (ok == FALSE)
				{
					printf( ">>Output quality is: \n"
							"  0 for 8 kHz\n"
							"  1 for 11 kHz interpolated to 22 kHz\n"
							"  2 for 11 kHz\n"
							"  3 for 16 kHz\n"
							"  4 for 22 kHz\n"
							"  5 for 22 kHz interpolated to 44 kHz\n"
							"  6 for 24 kHz\n"
							"  7 for 32 kHz\n"
							"  8 for 40 kHz\n"
							"  9 for 44 kHz\n"
							" 10 for 48 kHz\n");

					q = (BAEQuality)GetLong(">>Quality? ");
					t = BAE_LINEAR_INTERPOLATION;

					am = BAE_NONE;
					if (GetBool(">>Use stereo? (y/n) "))
					{
						am |= BAE_USE_STEREO;
					}
					if (GetBool(">>16 bit output? (y/n) "))
					{
						am |= BAE_USE_16;
					}
					maxSongVoices = GetLong(">>Maximum number of rendered notes playing at once? ");

					maxSoundVoices = GetLong(">>Maximum number of Sound objects playing at once? ");

					level = GetLong(">>Total number of full-scale voices before distortion (Song notes plus Sound objects)? ");
					engageAudio = GetBool(">>Send mixer audio output to the host device? (y/n) ");

					printf( ">>Open mixer with: \n"
							"  BAEQuality = %ld\n"
							"  maxSongVoices %ld\n"
							"  maxSoundVoices = %ld\n"
							"  mixLevel %ld\n",
							q, 
							maxSongVoices, 
							maxSoundVoices, 
							level);
					if (GetBool(">>(y/n)? "))
					{
						ok = TRUE;
					}
				}
				err = BAEMixer_Open(gMixer,q,t,am,
											(short int)maxSongVoices,
											(short int)maxSoundVoices,
											(short int)level,
											engageAudio);
			}
			break;
// BAEResult BAEMixer_Close(BAEMixer mixer);
		case FUNC_MIXER_CLOSE:
			err = BAEMixer_Close(gMixer);
			if (gBankBlock)
			{
				free(gBankBlock);
				gBankBlock = NULL;
			}
			break;
// BAEResult BAEMixer_GetMixerVersion(BAEMixer mixer, short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor);
		case FUNC_MIXER_GETMIXERVERSION:
			{
				short int major, minor, sub;

				err = BAEMixer_GetMixerVersion(gMixer, &major, &minor, &sub);
				if (err == 0)
				{
					printf("= %d.%d.%d\n", major, minor, sub);
				}
			}
			break;
// BAEResult BAEMixer_GetMaxDeviceCount(BAEMixer mixer,long *outMaxDeviceCount);
		case FUNC_MIXER_GETMAXDEVICECOUNT:
			{
				long	output;

				err = BAEMixer_GetMaxDeviceCount(gMixer, &output);
				if (err == 0)
				{
					printf("= %d\n", output);
				}
			}
			break;
// BAEResult BAEMixer_SetCurrentDevice(BAEMixer mixer, long deviceID,void *deviceParameter);
		case FUNC_MIXER_SETCURRENTDEVICE:
			{
				long	device;

				device = GetLong("deviceID > ");

				err = BAEMixer_SetCurrentDevice(gMixer, device, NULL);
			}
			break;
// BAEResult BAEMixer_GetCurrentDevice(BAEMixer mixer, void *deviceParameter, long *outDeviceID);
		case FUNC_MIXER_GETCURRENTDEVICE:
			{
				long	device;

				err = BAEMixer_GetCurrentDevice(gMixer, NULL, &device);
				if (err == 0)
				{
					printf("= %ld\n", device);
				}
			}
			break;
// BAEResult BAEMixer_GetDeviceName(BAEMixer mixer, long deviceID, char *cName, unsigned long cNameLength);
		case FUNC_MIXER_GETDEVICENAME:
			{
				long	device;
				char	name[512];

				err = BAEMixer_GetMaxDeviceCount(gMixer, &device);
				if (err == 0)
				{
					printf("\n>>deviceID? (0-%ld", device);
					device = GetLong(") ");
					err = BAEMixer_GetDeviceName(gMixer, device, name, 512);
					if (err == 0)
					{
						printf("= %s\n", name);
					}
				}
			}
			break;
// BAEResult BAEMixer_Is16BitSupported(BAEMixer mixer,BAE_BOOL *outIsSupported);
		case FUNC_MIXER_IS16BITSUPPORTED:
			{
				BAE_BOOL	output;

				err = BAEMixer_Is16BitSupported(gMixer, &output);
				if (err == 0)
				{
					printf("= %s\n", (output) ? "TRUE" : "FALSE");
				}
			}
			break;
// BAEResult BAEMixer_Is8BitSupported(BAEMixer mixer,BAE_BOOL *outIsSupported);
		case FUNC_MIXER_IS8BITSUPPORTED:
			{
				BAE_BOOL	output;

				err = BAEMixer_Is8BitSupported(gMixer, &output);
				if (err == 0)
				{
					printf("= %s\n", (output) ? "TRUE" : "FALSE");
				}
			}
			break;
// BAEResult BAEMixer_IsAudioEngaged(BAEMixer mixer,BAE_BOOL *outIsEngaged);
		case FUNC_MIXER_ISAUDIOENGAGED:
			{
				BAE_BOOL	output;

				err = BAEMixer_IsAudioEngaged(gMixer, &output);
				if (err == 0)
				{
					printf("= %s\n", (output) ? "TRUE" : "FALSE");
				}
			}
			break;
// BAEResult BAEMixer_DisengageAudio(BAEMixer mixer);
		case FUNC_MIXER_DISENGAGEAUDIO:
			err = BAEMixer_DisengageAudio(gMixer);
			break;
// BAEResult BAEMixer_ReengageAudio(BAEMixer mixer);
		case FUNC_MIXER_REENGAGEAUDIO:
			err = BAEMixer_ReengageAudio(gMixer);
			break;
// BAEResult BAEMixer_IsAudioActive(BAEMixer mixer, BAE_BOOL *outIsActive);
		case FUNC_MIXER_ISAUDIOACTIVE:
			{
				BAE_BOOL isActive;
				
				err = BAEMixer_IsAudioActive(gMixer, &isActive);
				if (err == BAE_NO_ERROR)
				{
					printf("= %s\n", (isActive) ? "TRUE" : "FALSE");
				}
			}
			break;
// BAEResult  BAEMixer_AddBankFromFile(BAEMixer mixer,BAEPathName pAudioPathName);
		case FUNC_MIXER_ADDBANKFROMFILE:
			{
				char			filename[3072];
				BAEBankToken	token;

				if (GetLine("\n>>File path for new bank: ", filename, 3072))
				{
					err = BAEMixer_AddBankFromFile(gMixer, (BAEPathName)filename, &token);
					if (err == 0)
					{
						printf("token = %d\n", (long)token);
					}
				}
			}

			break;
// BAEResult BAEMixer_AddBankFromMemory(BAEMixer mixer,void * pAudioFile,unsigned long fileSize);
		case FUNC_MIXER_ADDBANKFROMMEMORY:
			{
				FILE			*file;
				int				err;
				char			filename[3072];
				BAEBankToken	token;

				if (gBankBlock)
				{
					free(gBankBlock);
					gBankBlock = NULL;
				}
				gBankBlockLength = 0;
				if (GetLine("\n>>File path for new bank to be loaded into memory: ", filename, 3072))
				{
					// get size of file, allocate a block of memory, then read entire file into memory
					file = fopen(filename, "rb");
					if (file)
					{
						err = fseek(file, 0, SEEK_END);
						if (err == 0)
						{
							gBankBlockLength = ftell(file);
							fseek(file, 0, SEEK_SET);
						}
						gBankBlock = malloc(gBankBlockLength);
						if (gBankBlock)
						{
							gBankBlockLength = fread(gBankBlock, gBankBlockLength, 1, file);
						}
						fclose(file);
					}
				}
				if (gBankBlockLength)
				{
					err = BAEMixer_AddBankFromMemory(gMixer, gBankBlock, gBankBlockLength, &token);
					if (err == 0)
					{
						printf("token = %d\n", (unsigned long)token);
					}
				}
			}
			break;
// BAEResult  BAEMixer_UnloadBank(BAEMixer mixer);
		case FUNC_MIXER_UNLOADBANK:
			{
				BAEBankToken token = (BAEBankToken)GetLong(">>Bank token: ");
				err = BAEMixer_UnloadBank(gMixer, token);
			}
			break;
// BAEResult BAEMixer_GetBankVersion(BAEMixer mixer,short int *pVersionMajor,short int *pVersionMinor,short int *pVersionSubMinor);
		case FUNC_MIXER_GETBANKVERSION:
			{
				short int major, minor, sub;

				BAEBankToken token = (BAEBankToken)GetLong(">>Bank token: ");
				err = BAEMixer_GetBankVersion(gMixer, token, &major, &minor, &sub);
				if (err == 0)
				{
					printf("= %d.%d.%d\n", major, minor, sub);
				}
			}
			break;
// BAEResult BAEMixer_GetGroovoidNameFromBank(BAEMixer mixer,long index,char *cSongName);
		case FUNC_MIXER_GETGROOVOIDNAMEFROMBANK:
			{
				long	which;
				char	name[2048];

				which = GetLong("\n>>which name (index)? ");
				err = BAEMixer_GetGroovoidNameFromBank(gMixer, which, name);
				printf("%s\n", name);
			}
			break;
// BAEResult BAEMixer_ChangeAudioModes(BAEMixer mixer,BAEQuality q,BAETerpMode t,BAEAudioModifiers am);
		case FUNC_MIXER_CHANGEAUDIOMODES:
			{
				BAEQuality			q;
				BAETerpMode			t;
				BAEAudioModifiers	am;

				printf("\n");
				printf( ">>Output quality is: \n"
						"  0 for 8 kHz\n"
						"  1 for 11 kHz interpolated to 22 kHz\n"
						"  2 for 11 kHz\n"
						"  3 for 16 kHz\n"
						"  4 for 22 kHz\n"
						"  5 for 22 kHz interpolated to 44 kHz\n"
						"  6 for 24 kHz\n"
						"  7 for 32 kHz\n"
						"  8 for 40 kHz\n"
						"  9 for 44 kHz\n"
						" 10 for 48 kHz\n");

				q = (BAEQuality)GetLong(">>Quality? ");
				t = BAE_LINEAR_INTERPOLATION;

				am = BAE_NONE;
				if (GetBool(">>Use stereo? (y/n) "))
				{
					am |= BAE_USE_STEREO;
				}
				if (GetBool(">>16 bit output? (y/n) "))
				{
					am |= BAE_USE_16;
				}
				err = BAEMixer_ChangeAudioModes(gMixer, q, t, am);
			}
			break;
// BAEResult BAEMixer_GetQuality(BAEMixer mixer,BAEQuality *outQuality);
		case FUNC_MIXER_GETQUALITY:
			{
				BAEQuality	output;

				err = BAEMixer_GetQuality(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult  BAEMixer_GetTerpMode(BAEMixer mixer,BAETerpMode *outTerpMode);
		case FUNC_MIXER_GETTERPMODE:
			{
				BAETerpMode	output;

				err = BAEMixer_GetTerpMode(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_GetModifiers(BAEMixer mixer,BAEAudioModifiers *outMods);
		case FUNC_MIXER_GETMODIFIERS:
			{
				BAEAudioModifiers	output;

				err = BAEMixer_GetModifiers(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_ChangeSystemVoices(BAEMixer mixer,short int maxMidiVoices,short int maxSoundVoices,short int mixLevel);
		case FUNC_MIXER_CHANGESYSTEMVOICES:
			{
				long maxSongVoices, maxSoundVoices, mixLevel;

				printf("\n");
				maxSongVoices = GetLong(">>Maximum number of rendered notes playing at once? ");

				maxSoundVoices = GetLong(">>Maximum number of Sound objects playing at once? ");

				mixLevel = GetLong(">>Total number of full-scale voices before distortion (Song notes plus Sound objects)? ");
				err = BAEMixer_ChangeSystemVoices(gMixer, (short)maxSongVoices, 
															(short)maxSoundVoices, 
															(short)mixLevel);
			}
			break;
// BAEResult BAEMixer_GetMidiVoices(BAEMixer mixer,short int *outNumMidiVoices);
		case FUNC_MIXER_GETMIDIVOICES:
			{
				short int output;

				err = BAEMixer_GetMidiVoices(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_GetSoundVoices(BAEMixer mixer,short int *outNumSoundVoices);
		case FUNC_MIXER_GETSOUNDVOICES:
			{
				short int output;

				err = BAEMixer_GetSoundVoices(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_GetMixLevel(BAEMixer mixer,short int *outMixLevel);
		case FUNC_MIXER_GETMIXLEVEL:
			{
				short int output;

				err = BAEMixer_GetMixLevel(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult  BAEMixer_GetTick(BAEMixer mixer,unsigned long *outTick);
		case FUNC_MIXER_GETTICK:
			{
				unsigned long output;

				err = BAEMixer_GetTick(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_SetAudioLatency(BAEMixer mixer,unsigned long requestedLatency);
		case FUNC_MIXER_SETAUDIOLANTENCY:
			{
				unsigned long input;

				input = GetLong("\n>>New Latency? ");
				err = BAEMixer_SetAudioLatency(gMixer, input);
			}
			break;
// BAEResult  BAEMixer_GetAudioLatency(BAEMixer mixer,unsigned long *outLatency);
		case FUNC_MIXER_GETAUDIOLANTENCY:
			{
				unsigned long output;

				err = BAEMixer_GetAudioLatency(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_SetMasterVolume(BAEMixer mixer,BAE_UNSIGNED_FIXED theVolume);
		case FUNC_MIXER_SETMASTERVOLUME:
			{
				double	finput;

				finput = GetDouble("Master volume (1.0 norm) > ");
				err = BAEMixer_SetMasterVolume(gMixer, FLOAT_TO_UNSIGNED_FIXED(finput));
			}
			break;
// BAEResult BAEMixer_GetMasterVolume(BAEMixer mixer,BAE_UNSIGNED_FIXED *outVolume);
		case FUNC_MIXER_GETMASTERVOLUME:
			{
				BAE_UNSIGNED_FIXED	output;
				double				foutput;

				err = BAEMixer_GetMasterVolume(gMixer, &output);
				foutput = UNSIGNED_FIXED_TO_FLOAT(output);
				printf("= %lf\n", foutput);
			}
			break;
// BAEResult BAEMixer_SetHardwareVolume(BAEMixer mixer,BAE_UNSIGNED_FIXED theVolume);
		case FUNC_MIXER_SETHARDWAREVOLUME:
			{
				double	finput;

				finput = GetDouble("Hardware volume (1.0 norm) > ");
				err = BAEMixer_SetHardwareVolume(gMixer, FLOAT_TO_UNSIGNED_FIXED(finput));
			}
			break;
// BAEResult BAEMixer_GetHardwareVolume(BAEMixer mixer,BAE_UNSIGNED_FIXED *outVolume);
		case FUNC_MIXER_GETHARDWAREVOLUME:
			{
				BAE_UNSIGNED_FIXED	output;
				double				foutput;

				err = BAEMixer_GetHardwareVolume(gMixer, &output);
				foutput = UNSIGNED_FIXED_TO_FLOAT(output);
				printf("= %lf\n", foutput);
			}
			break;
// BAEResult BAEMixer_SetMasterSoundEffectsVolume(BAEMixer mixer,BAE_UNSIGNED_FIXED theVolume);
		case FUNC_MIXER_SETMASTERSOUNDEFFECTSVOLUME:
			{
				double	finput;

				finput = GetDouble("Sound effects volume (1.0 norm) > ");
				err = BAEMixer_SetMasterSoundEffectsVolume(gMixer, FLOAT_TO_UNSIGNED_FIXED(finput));
			}
			break;
// BAEResult BAEMixer_GetMasterSoundEffectsVolume(BAEMixer mixer,BAE_UNSIGNED_FIXED *outVolume);
		case FUNC_MIXER_GETMASTERSOUNDEFFECTSVOLUME:
			{
				BAE_UNSIGNED_FIXED	output;
				double				foutput;

				err = BAEMixer_GetMasterSoundEffectsVolume(gMixer, &output);
				foutput = UNSIGNED_FIXED_TO_FLOAT(output);
				printf("= %lf\n", foutput);
			}
			break;
// BAEResult BAEMixer_GetAudioSampleFrame(BAEMixer mixer,short int *pLeft,short int *pRight,short int *outFrame);
		case FUNC_MIXER_GETAUDIOSAMPLEFRAME:
			{
				short int left[1024], right[1024];
				short int outputSize, leftcrc, rightcrc, count;

				err = BAEMixer_GetAudioSampleFrame(gMixer, left, right, &outputSize);
				leftcrc = 0;
				rightcrc = 0;
				for (count = 0; count < outputSize; count++)
				{
					leftcrc += left[count];
					rightcrc += right[count];
				}
				printf("= frames %d, left crc %d, right crc %d\n", outputSize, leftcrc, rightcrc);
			}
			break;
// BAEResult BAEMixer_GetRealtimeStatus(BAEMixer mixer,BAEAudioInfo *pStatus);
		case FUNC_MIXER_GETREALTIMESTATUS:
			{
				BAEAudioInfo	status;
				short int		count, voice;

				err = BAEMixer_GetRealtimeStatus(gMixer, &status);
				if (err == BAE_NO_ERROR)
				{
					printf("\nThere are %d active voices\n", status.voicesActive);

					printf("Type:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %d ", voice, status.voiceType[voice]);
					}	
					printf("\nInstrument:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %d ", voice, status.instrument[voice]);
					}	
					printf("\nMidi Volume:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %d ", voice, status.midiVolume[voice]);
					}	
					printf("\nScaled Volume:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %d ", voice, status.scaledVolume[voice]);
					}	
					printf("\nChannel:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %d ", voice, status.channel[voice]);
					}	
					printf("\nMidi Note:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %d ", voice, status.midiNote[voice]);
					}	
					printf("\nUser Reference:\n");
					for (count = 0; count < status.voicesActive; count++)
					{
						voice = status.voice[count];
						printf("[%d] %ld ", voice, status.userReference[voice]);
					}	
					printf("\n");
				}
			}
			break;
// BAEResult  BAEMixer_GetCPULoadInMicroseconds(BAEMixer mixer,unsigned long *outLoad);
		case FUNC_MIXER_GETCPULOADINMICROSECONDS:
			{
				unsigned long output;

				err = BAEMixer_GetCPULoadInMicroseconds(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;
// BAEResult BAEMixer_GetCPULoadInPercent(BAEMixer mixer,unsigned long *outLoad);
		case FUNC_MIXER_GETCPULOADINPERCENT:
			{
				unsigned long output;

				err = BAEMixer_GetCPULoadInPercent(gMixer, &output);
				if (err == 0)
				{
					printf("= %ld\n", (long)output);
				}
			}
			break;

		case FUNC_MIXER_BRINGBANKTOFRONT:
			{
				BAEBankToken token;

				token = (BAEBankToken)GetLong("Bank token: ");
				err = BAEMixer_BringBankToFront(gMixer, token);
			}
			break;
		
		case FUNC_MIXER_SENDBANKTOBACK:
			{
				BAEBankToken token;

				token = (BAEBankToken)GetLong("Bank token: ");
				err = BAEMixer_SendBankToBack(gMixer, token);
			}
			break;

		default:
			err = BAE_GENERAL_BAD;
			break;
	}
	return err;
}


// DoTest()
// -----------------------------------------------------
//
//
static BAEResult DoTest(int subMenu)
{
	BAEResult err;
	int i;

	err = BAE_NO_ERROR;
	switch (subMenu)
	{
		case TEST_MIXER_CONSTRUCTOR:
			{
				if (gMixer)
				{
					BAEMixer_Delete(gMixer);
					gMixer = NULL;
				}
				
				for (i=0; i<100; i++)
				{
					gMixer = BAEMixer_New();
					if (!gMixer)
					{
						FailedTest("Couldn't allocate BAEMixer object.  Exiting Test!");
						return BAE_MEMORY_ERR;
					}
					BAEMixer_Delete(gMixer);
					gMixer = NULL;
				}

				gMixer = BAEMixer_New();
			}
			break;

		case TEST_MIXER_OPEN_CLOSE:
			{
				err = TestOpenClose();
			}
			break;
	
		case TEST_MIXER_GET_SET_MASTER_VOLUME:
			{
				err = TestGetSetMasterVolume();
			}
			break;

		case TEST_MIXER_GET_SET_HARDWARE_VOLUME:
			{
				err = TestGetSetHardwareVolume();
			}
			break;

		default:
			break;
	}
	return err;
}


// TestOpenClose()
// ---------------------------------------------
//
//
static BAEResult TestOpenClose(void)
{
	BAEQuality q;
	BAETerpMode t;
	BAEAudioModifiers am;
	short int maxMidiVoices;
	short int maxSoundVoices;
	short int mixLevel;
	BAE_BOOL engageAudio;

	BAEResult err;
	BAE_BOOL isOpen;
	BAE_BOOL isEngaged;

	if (!gMixer) gMixer = BAEMixer_New();

	q = BAE_8K;
	t = BAE_LINEAR_INTERPOLATION;
	am = BAE_USE_16;
	maxMidiVoices = 7;
	maxSoundVoices = 1;
	mixLevel = 4;
	engageAudio = TRUE;


	err = BAEMixer_Close(gMixer);
	if (err) return err;

	err = BAEMixer_IsOpen(gMixer, &isOpen);
	if (err) return err;

	if (isOpen)
	{
		FailedTest("IsOpen() returned true after Close()");
	}

//	for (i=0; i<100; i++) // keep an eye out for memory leaks!
	{
		err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, engageAudio);
		if (err) return err;
		err = BAEMixer_Close(gMixer);
		if (err) return err;
	}

	err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, engageAudio);
	if (err) return err;

	err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, engageAudio);
	if (err != BAE_NOT_REENTERANT)
	{
		FailedTest("Opening already open mixer didn't return BAE_NOT_REENTERANT");
	}


	// Check argument boundaries for Open()
	BAEMixer_Close(gMixer);


	// maxMidiVoices
	printf("\tmaxMidiVoices\n");
	for (maxMidiVoices = -10; maxMidiVoices< 16; maxMidiVoices++)
	{
		short tmpMidiVoices;

		err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, engageAudio);
		BAEMixer_GetMidiVoices(gMixer, &tmpMidiVoices);
		BAEMixer_IsOpen(gMixer, &isOpen);
		printf("maxMidiVoices = %d -> %d : %s, IsOpen = %s\n", maxMidiVoices, tmpMidiVoices, BAEResultToStr(err), (isOpen) ? "TRUE" : "FALSE");
/*
		if (maxMidiVoices<4)
		{
			if (err != BAE_PARAM_ERR || mMixer->GetMidiVoices()!=4)
				FailedTest("Lower boundary problem for maxMidiVoices");
		}
		else if (maxSoundVoices+maxMidiVoices>64)
		{
			if (mErr != BAE_PARAM_ERR || mMixer->GetMidiVoices()!=4)
				FailedTest("Upper boundary problem for maxMidiVoices");
		}
		else if (maxMidiVoices>=4 && maxSoundVoices+maxMidiVoices<=64)
		{
			if (mErr != BAE_NO_ERROR || mMixer->GetMidiVoices()!=maxMidiVoices)
				FailedTest("Problem setting maxMidiVoices for valid argument.");
		}
		else FailedTest("Unknown maxMidiVoices error");
*/
		err = BAEMixer_Close(gMixer);
	}
	maxMidiVoices = 7; // reset
	printf("<press return>");  getchar();


	// maxSoundVoices
	printf("\tmaxSoundVoices\n");
	for (maxSoundVoices = -4; maxSoundVoices< 4; maxSoundVoices++)
	{
		short tmpSoundVoices;

		err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, engageAudio);
		BAEMixer_GetSoundVoices(gMixer, &tmpSoundVoices);
		BAEMixer_IsOpen(gMixer, &isOpen);
		printf("maxSoundVoices = %d -> %d : %s, IsOpen = %s\n", maxSoundVoices, tmpSoundVoices, BAEResultToStr(err), (isOpen) ? "TRUE" : "FALSE");
/*
		if (maxSoundVoices<0)
		{
			if (mErr != BAE_PARAM_ERR || mMixer->GetSoundVoices()!=0)
				FailedTest("Lower boundary problem for maxSoundVoices");
		}
		else if (maxSoundVoices+maxMidiVoices>64)
		{
			if (mErr != BAE_PARAM_ERR || mMixer->GetSoundVoices()!=0)
				FailedTest("Upper boundary problem for maxSoundVoices");
		}
		else if (maxSoundVoices>=0 && maxSoundVoices+maxMidiVoices<=64)
		{
			if (mErr != BAE_NO_ERROR || mMixer->GetSoundVoices()!=maxSoundVoices)
				FailedTest("Problem setting maxSoundVoices for valid argument.");
		}
		else FailedTest("Unknown maxSoundVoices error");
*/
		err = BAEMixer_Close(gMixer);
	}
	maxSoundVoices = 1; // reset
	printf("<press return>");  getchar();


	// mixLevel
	printf("\tmixLevel\n");
	for (mixLevel = -4; mixLevel<16; mixLevel++)
	{
		short tmpMixLevel;

		err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, engageAudio);
		BAEMixer_GetMixLevel(gMixer, &tmpMixLevel);
		BAEMixer_IsOpen(gMixer, &isOpen);
		printf("mixLevel = %d -> %d : %s, IsOpen = %s\n", mixLevel, tmpMixLevel, BAEResultToStr(err), (isOpen) ? "TRUE" : "FALSE");
/*
		if (mixLevel<0)
		{
			if (mErr != BAE_PARAM_ERR || mMixer->GetMixLevel() != 1) 
				FailedTest("Lower boundary problem for mixLevel");
		}
		else if (mixLevel>64)
		{
			if (mErr != BAE_PARAM_ERR || mMixer->GetMixLevel() != 1)
				FailedTest("Upper boundary problem for mixLevel");
		}
		else // in range
		{
			if (mErr != BAE_NO_ERROR || mixLevel != mMixer->GetMixLevel())
				FailedTest("Problem setting mixLevel for valid argument.");
		}
*/
		err = BAEMixer_Close(gMixer);
	}
	mixLevel = 4; // reset
	printf("<press return>");  getchar();


	// engageAudio
	printf("\tengageAudio\n");
	err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, TRUE);
	err = BAEMixer_IsAudioEngaged(gMixer, &isEngaged);
	if (!isEngaged)
		FailedTest("Open with audio engaged didn't work");

	err = BAEMixer_Close(gMixer);
	
	err = BAEMixer_Open(gMixer, q, t, am, maxMidiVoices, maxSoundVoices, mixLevel, FALSE);
	err = BAEMixer_IsAudioEngaged(gMixer, &isEngaged);
	if (isEngaged)
		FailedTest("Open with audio not engaged didn't work");

	err = BAEMixer_Close(gMixer);
	printf("<press return>");  getchar();

	return err;
}


// TestGetSetMasterVolume()
// ---------------------------------------------
//
//
static BAEResult TestGetSetMasterVolume(void)
{
	BAEResult err;
	BAE_UNSIGNED_FIXED origVal, newVal;
	double f_in, f_out;

	err = BAEMixer_GetMasterVolume(gMixer, &origVal);
	if (err) return err;

	for (f_in=-1.0; f_in<10.0; f_in+=.1)
	{
		err = BAEMixer_SetMasterVolume(gMixer, FLOAT_TO_UNSIGNED_FIXED(f_in));
		if (err) return err;

		err = BAEMixer_GetMasterVolume(gMixer, &newVal);
		if (err) return err;

		f_out = UNSIGNED_FIXED_TO_FLOAT(newVal);
		
		printf("%g -> %g\n", f_in, f_out);
		if (f_in < 0)
		{
			if (f_out != 0)
			{
//				FailedTest("Setting master volume to negative values should set instead to zero.");
//				break;
			}
		}
		else
		{
			if ((f_in - f_out > .01) || (f_in - f_out < -.01))
			{
				FailedTest("Difference between Set and Get values was not within tolerance.");		
//				break;
			}
		}
		BAE_WaitMicroseconds(100000);
	}
	err = BAEMixer_SetMasterVolume(gMixer, origVal); // reset volume
	return err;
}



// TestGetSetHardwareVolume()
// ---------------------------------------------
//
//
static BAEResult TestGetSetHardwareVolume(void)
{
	BAEResult err;
	BAE_UNSIGNED_FIXED origVal, newVal;
	double f_in, f_out;

	err = BAEMixer_GetHardwareVolume(gMixer, &origVal);
	if (err) return err;

	for (f_in=-1.0; f_in<5.0; f_in+=.1)
	{
		err = BAEMixer_SetHardwareVolume(gMixer, FLOAT_TO_UNSIGNED_FIXED(f_in));
		if (err) return err;

		err = BAEMixer_GetHardwareVolume(gMixer, &newVal);
		if (err) return err;

		f_out = UNSIGNED_FIXED_TO_FLOAT(newVal);
		
		printf("%g -> %g\n", f_in, f_out);
		if (f_in < 0)
		{
			if (f_out != 0)
			{
				FailedTest("Setting hardware volume to negative values should set to zero instead.");
				break;
			}
		}
		else if (f_in > 1)
		{
			if (f_out != 1)
			{
				FailedTest("Setting hardware volume to values >1.0 should set to 1.0 instead.");
				break;		
			}
		}
		else
		{
			if ((f_in - f_out > .01) || (f_in - f_out < -.01))
			{
				FailedTest("Difference between Set and Get values was not within tolerance.");		
				break;
			}
		}
		BAE_WaitMicroseconds(200000);
	}
	err = BAEMixer_SetHardwareVolume(gMixer, origVal); // reset volume
	return err;
}
