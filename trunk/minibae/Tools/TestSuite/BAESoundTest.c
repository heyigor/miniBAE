/*****************************************************************************/
/*
** "BAESoundTest.c"
**
**	© Copyright 1999-2000 Beatnik, Inc, All Rights Reserved.
**	Written by Andrew Ezekiel Rostaing
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
**	12/16/99	Created.
**	1/11/2000	Fixed some compiler issues. Included stdlib.h, etc
**	1/13/2000	MSD:	Implemented BAESound_LoadCustomSample() menu item.
**						Output cosmetic changes.
**	3/13/2000	MSD:	added some scripts
**	3/15/2000	sh:		Removed use of Sleep to compile ANSI. Now use BAE_WaitMicroseconds
**						Removed use of X_API, now use strcmpnocase
**	3/15/2000	msd:	made sure to NULL bufferPtr after free()
**	10/18/2000	sh		Changed default file to load with to a smaller file, and
**						added a new script to loop that file.
*/
/*****************************************************************************/


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// MAJOR ISSUE -- SET IN BUILD OPTIONS
// BAEBuildOptions_WinOS.h, line 100
//	#define	USE_HIGHLEVEL_FILE_API		TRUE
// BAEBuildOptions_WinOS.h, line 97
//	#define USE_CREATION_API			TRUE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#include "BAESoundTest.h"
#include "TestSuite.h"
#include "TestTools.h"
#include "BAE_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
static void PV_DoFunction(int subMenu);
static void PV_DoScript(int subMenu);

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
#define	NUM_MENU_ITEMS		2
#define	NUM_SCRIPTS			3
#define NUM_FUNCTIONS		26
#define DEFAULT_WAVE_FILE	"tell-me-about_22.wav"


static const char *ScriptList[NUM_SCRIPTS] = 
{
	"Load 'tell-me-about_22.wav' into slot 0, and play",
	"Load 'tell-me-about_22.wav' into slot 0, and play looped",
	"Sweep slot 0 from 1kHz, to 60kHz"
};

enum
{
	SCRIPT_SND_LOADWAVE		=1,
	SCRIPT_SND_LOADWAVE_LOOP,
	SCRIPT_SND_RATESWEEP
};

static const char *FunctionList[NUM_FUNCTIONS] = 
{
	"BAESound_New()",
	"BAESound_Delete()",
	"BAESound_SetVolume()",
	"BAESound_GetVolume()",
	"BAESound_LoadMemorySample()",
	"BAESound_LoadFileSample()",
	"BAESound_LoadCustomSample()",
	"BAESound_Unload()",
	"BAESound_GetInfo()",
	"BAESound_Start()",
	"BAESound_Stop()",
	"BAESound_Pause()",
	"BAESound_Resume()",
	"BAESound_IsPaused()",
	"BAESound_Fade()",
	"BAESound_IsDone()",
	"BAESound_SetRate()",
	"BAESound_GetRate()",
	"BAESound_SetLowPassAmountFilter()",
	"BAESound_GetLowPassAmountFilter()",
	"BAESound_SetResonanceAmountFilter()",
	"BAESound_GetResonanceAmountFilter()",
	"BAESound_SetFrequencyAmountFilter()",
	"BAESound_GetFrequencyAmountFilter()",
	"BAESound_SetSampleLoopPoints()",
	"BAESound_GetSampleLoopPoints()"
};

enum
{
	FUNC_SND_NEW		= 1,
	FUNC_SND_DELETE,
	FUNC_SND_SETVOLUME,
	FUNC_SND_GETVOLUME,
	FUNC_SND_LOADMEMORYSAMPLE,
	FUNC_SND_LOADFILESAMPLE,
	FUNC_SND_LOADCUSTOMSAMPLE,
	FUNC_SND_UNLOAD,
	FUNC_SND_GETINFO,
	FUNC_SND_START,
	FUNC_SND_STOP,
	FUNC_SND_PAUSE,
	FUNC_SND_RESUME,
	FUNC_SND_ISPAUSED,
	FUNC_SND_FADE,
	FUNC_SND_ISDONE,
	FUNC_SND_SETRATE,
	FUNC_SND_GETRATE,
	FUNC_SND_SETLOWPASSAMOUNTFILTER,
	FUNC_SND_GETLOWPASSAMOUNTFILTER,
	FUNC_SND_SETRESONANCEAMOUNTFILTER,
	FUNC_SND_GETRESONANCEAMOUNTFILTER,
	FUNC_SND_SETFREQUENCYAMOUNTFILTER,
	FUNC_SND_GETFREQUENCYAMOUNTFILTER,
	FUNC_SND_SETSAMPLELOOPPOINTS,
	FUNC_SND_GETSAMPLELOOPPOINTS
};

// SoundTest_GetNumMenuItems()
// -------------------------------------
// Returns the total number of menu selections (not including submenus).
//
int SoundTest_GetNumMenuItems(void)
{
	return NUM_MENU_ITEMS;
}


// SoundTest_GetNumSubMenuItems()
// -------------------------------------
// Returns the total number of submenu selections for a given menu.
//
int SoundTest_GetNumSubMenuItems(int menu)
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


// SoundTest_GetMenuItem()
// -------------------------------------
// upon returning, outMenuText will point to the text description of menu item
// number 'menu'.  The function returns 0 if the menu asked for does not exist,
// 1 if it does.
int		SoundTest_GetMenuItem(int menu, const char **outMenuText)
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


// SoundTest_GetMenuItem()
// -------------------------------------
// upon returning, outMenuText will point to the text description of menu item
// number 'menu'.  The function returns 0 if the menu asked for does not exist,
// 1 if it does.
int		SoundTest_GetSubMenuItem(int menu, int subMenu, const char **outMenuText)
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


// SoundTest_DoMenuItem()
// -------------------------------------
// Executes the fuctionality associtated with the given menu selection.
//
void	SoundTest_DoMenuItem(int menu, int subMenu)
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




///////////////////////////////////////////////////////////////////////////////
// Standard Implementation
///////////////////////////////////////////////////////////////////////////////

// PV_DoFunction()
// -----------------------------------------------------
//
//
static void PV_DoFunction(int subMenu)
{
	long				slot;

	BAEResult			err;

	// Temporary vars
	double				tmpDouble1,
						tmpDouble2;
	long				tmpLong1,
						tmpLong2,
						tmpLong3,
						tmpLong4,
						tmpLong5;
	short				tmpShort1;
	BAE_UNSIGNED_FIXED	tmpUFixed1;
	char				tmpBuffer1[256],
						tmpBuffer2[256];
	BAEFileType			tmpFileType1;
	BAESampleInfo		tmpInfo1;
	BAE_BOOL			tmpBool1;
	FILE*				filePtr;
	static void*		bufferPtr;
	int					i;


	err = BAE_NO_ERROR;


	while (((slot=GetLong("Slot (0-3) > "))<0) || (slot>=MAX_SLOTS))
	{
		printf("Try again.  Choose a slot between 0 and %d\n", MAX_SLOTS-1);
	}


	switch (subMenu)
	{
// BAESound	BAESound_New(BAEMixer mixer);
		case FUNC_SND_NEW:
			{
				if (gSounds[slot])
				{
					printf(": Deleting old sound in slot %d...\n", slot);
					BAESound_Delete(gSounds[slot]);
					gSounds[slot] = NULL;
				}
				gSounds[slot] = BAESound_New(gMixer);
				if (gSounds[slot] != NULL)
				{
					printf(": New BAESound in slot %d.\n", slot);
				}
				else
				{
					printf("Failed to created new BAESound in slot %d.\n", slot);
				}
			}
			break;
// BAEResult BAESound_Delete(BAESound sound);
		case FUNC_SND_DELETE:
			{
				err = BAESound_Delete(gSounds[slot]);
				if (err == BAE_NO_ERROR) gSounds[slot] = NULL;
			}
			break;
// BAEResult BAESound_SetVolume(BAESound sound, BAE_UNSIGNED_FIXED newVolume);
		case FUNC_SND_SETVOLUME:
			{
				tmpDouble1 = GetDouble("Volume (1.0 norm)> ");
				err = BAESound_SetVolume(gSounds[slot],
										 FLOAT_TO_UNSIGNED_FIXED(tmpDouble1));
				printf(": BAESound_SetVolume(slot %d, %g)\n", slot, tmpDouble1);
			}
			break;
// BAEResult BAESound_GetVolume(BAESound sound, BAE_UNSIGNED_FIXED *outVolume);
		case FUNC_SND_GETVOLUME:
			{
				err = BAESound_GetVolume(gSounds[slot], &tmpUFixed1);
				printf(": BAESound_GetVolume(slot=%d) = %f\n",
					   slot,
					   UNSIGNED_FIXED_TO_FLOAT(tmpUFixed1));
			}
			break;
// BAEResult BAESound_LoadMemorySample(BAESound sound,
// 									   void *pMemoryFile,
// 									   unsigned long memoryFileSize,
// 									   BAEFileType fileType);
		case FUNC_SND_LOADMEMORYSAMPLE:
			{
				GetLine("Filepath > ", tmpBuffer1, 256);
				GetLine("File Type (AIFF, WAV, AU)) > ", tmpBuffer2, 256);
				// Convert from text to a real file type
				if (strcmpnocase(tmpBuffer2, "aiff") == 0)
				{
					tmpFileType1 = BAE_AIFF_TYPE;
				}
				else if (strcmpnocase(tmpBuffer2, "wav") == 0 ||
						 strcmpnocase(tmpBuffer2, "wav") == 0)
				{
					tmpFileType1 = BAE_WAVE_TYPE;
				}
				else if (strcmpnocase(tmpBuffer2, "au") == 0)
				{
					tmpFileType1 = BAE_AU_TYPE;
				}
				else
				{
					tmpFileType1 = BAE_INVALID_TYPE;
				}

				filePtr = fopen(tmpBuffer1, "rb");
				if (filePtr != NULL)
				{
					if (bufferPtr)
					{
						free(bufferPtr);
						bufferPtr = NULL;
					}
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
						printf("ERROR: File read error -- not BAE related...");
					}
					else
					{
						err = BAESound_LoadMemorySample(gSounds[slot],
														bufferPtr,
														tmpLong2,
														tmpFileType1);
					}
					fclose(filePtr);
				}
			}
			break;
// BAEResult BAESound_LoadFileSample(BAESound sound,
//									 BAEPathName filePath,
// 									 BAEFileType fileType);
		case FUNC_SND_LOADFILESAMPLE:
			{
				GetLine("Filepath > ", tmpBuffer1, 256);
				GetLine("File Type (AIFF, WAV, AU)) > ", tmpBuffer2, 256);
				// Convert from text to a real file type
				if (strcmpnocase(tmpBuffer2, "aiff") == 0)
				{
					tmpFileType1 = BAE_AIFF_TYPE;
				}
				else if (strcmpnocase(tmpBuffer2, "wav") == 0 ||
						 strcmpnocase(tmpBuffer2, "wav") == 0)
				{
					tmpFileType1 = BAE_WAVE_TYPE;
				}
				else if (strcmpnocase(tmpBuffer2, "au") == 0)
				{
					tmpFileType1 = BAE_AU_TYPE;
				}
				else
				{
					tmpFileType1 = BAE_INVALID_TYPE;
				}
				err = BAESound_LoadFileSample(gSounds[slot],
											  (BAEPathName) tmpBuffer1,
											  tmpFileType1);
			}
			break;
// BAEResult BAESound_LoadCustomSample(BAESound sound,
// 									   void * sampleData,
// 									   unsigned long frames,
// 									   unsigned short int bitSize,
// 									   unsigned short int channels,
// 									   BAE_UNSIGNED_FIXED rate,
// 									   unsigned long loopStart,
// 									   unsigned long loopEnd);
		case FUNC_SND_LOADCUSTOMSAMPLE:
			{
				tmpLong1 = GetLong("Num frames > ");
				tmpLong2 = GetLong("bits per sample > ");
				tmpLong3 = GetLong("Num channels > ");
				tmpLong4 = GetLong("Sample rate (hz) > ");


				if (bufferPtr)
				{
					free(bufferPtr);
					bufferPtr = NULL;
				}
				tmpLong5 = (unsigned long)tmpLong1 * ((unsigned long)tmpLong2 / 8) * tmpLong3; // buffer size
				bufferPtr = malloc(tmpLong5);

				if (bufferPtr)
				{
					// zero out block;
					for (i=0; i< tmpLong5; i++)
					{
						((char *)bufferPtr)[i] = 0;
					}


					tmpDouble1 = 2.0 * 3.1415926 * 500.0 / tmpLong4; // sampled radian freq (left)
					tmpDouble2 = 2.0 * 3.1415926 * 800.0 / tmpLong4; // sampled radian freq (right)

					if (tmpLong3 == 1)
					{
						// mono
						if (tmpLong2 == 8)
						{
							printf("Generating 8-bit mono sine wave at 500hz.\n");
							for (i=0; i<tmpLong1; i++)
							{
								((unsigned char *)bufferPtr)[i] = (unsigned char) (128*(sin(i*tmpDouble1)) + 128);
							}
						}
						else if (tmpLong2 == 16)
						{
							printf("Generating 16-bit mono sine wave at 500hz.\n");
							for (i=0; i<tmpLong1; i++)
							{
								((short *)bufferPtr)[i] = (short) (32768*(sin(i*tmpDouble1)));
							}
						}
					}
					else if (tmpLong3 == 2)
					{
						// stereo
						if (tmpLong2 == 8)
						{
							printf("Generating 8-bit stereo sine wave at 500hz (left) and 800hz (right).\n");
							for (i=0; i<tmpLong1; i++)
							{
								((unsigned char *)bufferPtr)[2*i] = (unsigned char) (128*(sin(i*tmpDouble1)) + 128);
								((unsigned char *)bufferPtr)[(2*i)+1] = (unsigned char) (128*(sin(i*tmpDouble2)) + 128);
							}
						}
						else if (tmpLong2 == 16)
						{
							printf("Generating 16-bit stereo sine wave at 500hz (left) and 800hz (right).\n");
							for (i=0; i<tmpLong1; i++)
							{
								((short *)bufferPtr)[2*i] = (short) (32768*(sin(i*tmpDouble1)));
								((short *)bufferPtr)[(2*i)+1] = (short) (32768*(sin(i*tmpDouble2)));
							}
						}
					}
					err = BAESound_LoadCustomSample(gSounds[slot], 
						bufferPtr, 
						(unsigned long)tmpLong1, 
						(unsigned short)tmpLong2, 
						(unsigned short)tmpLong3, 
						LONG_TO_UNSIGNED_FIXED(tmpLong4),
						0,
						0);
					free(bufferPtr);
					bufferPtr = NULL;
				}
				else
				{
					err = BAE_MEMORY_ERR;
				}
			}
			break;
// BAEResult BAESound_Unload(BAESound sound);
		case FUNC_SND_UNLOAD:
			{
				err = BAESound_Unload(gSounds[slot]);
			}
			break;
// BAEResult BAESound_GetInfo(BAESound sound, BAESampleInfo *outInfo);
		case FUNC_SND_GETINFO:
			{
				err = BAESound_GetInfo(gSounds[slot], &tmpInfo1);
				if (!err)
				{
					printf(": slot %d bitSize = %d\n", slot, tmpInfo1.bitSize);
					printf(": slot %d channels = %d\n", slot, tmpInfo1.channels);
					printf(": slot %d baseMidiPitch = %d\n", slot,
						tmpInfo1.baseMidiPitch);
					printf(": slot %d waveSize = %d\n", slot, tmpInfo1.waveSize);
					printf(": slot %d waveFrames = %d\n", slot,
						tmpInfo1.waveFrames);
					printf(": slot %d startLoop = %d\n", slot, tmpInfo1.startLoop);
					printf(": slot %d endLoop = %d\n", slot, tmpInfo1.endLoop);
				}
			}
			break;
// BAEResult BAESound_Start(BAESound sound,
// 							short int priority,
// 							BAE_UNSIGNED_FIXED sampleVolume,
// 							unsigned long startOffsetFrame);
		case FUNC_SND_START:
			{
				tmpShort1 = (short) GetLong("Priority > ");
				tmpDouble1 = GetDouble("Volume (1.0 norm)> ");
				tmpLong1 = GetLong("Start Offset Frame > ");
				err = BAESound_Start(gSounds[slot],
									 tmpShort1,
									 FLOAT_TO_UNSIGNED_FIXED(tmpDouble1),
									 tmpLong1);
			}
			break;
// BAEResult BAESound_Stop(BAESound sound, BAE_BOOL startFade);
		case FUNC_SND_STOP:
			{
				tmpBool1 = GetBool("Start Fade? (y/n) > ");
				err = BAESound_Stop(gSounds[slot], tmpBool1);
			}
			break;
// BAEResult BAESound_Pause(BAESound sound);
		case FUNC_SND_PAUSE:
			{
				err = BAESound_Pause(gSounds[slot]);
			}
			break;
// BAEResult BAESound_Resume(BAESound sound);
		case FUNC_SND_RESUME:
			{
				err = BAESound_Resume(gSounds[slot]);
			}
			break;
// BAEResult BAESound_IsPaused(BAESound sound, BAE_BOOL *outIsPaused);
		case FUNC_SND_ISPAUSED:
			{
				err = BAESound_IsPaused(gSounds[slot], &tmpBool1);
				printf(": BAESound_IsPaused(slot=%d) = %d\n", slot, tmpBool1);
			}
			break;
// BAEResult BAESound_Fade(BAESound sound,
// 						   BAE_FIXED sourceVolume,
// 						   BAE_FIXED destVolume,
// 						   BAE_FIXED timeInMiliseconds);
		case FUNC_SND_FADE:
			{
				tmpDouble1 = GetDouble("Source Volume (1.0 norm) > ");
				tmpDouble2 = GetDouble("Destination Volume (1.0 norm) > ");
				tmpLong1 = GetLong("Time (msec) > ");
				err = BAESound_Fade(gSounds[slot],
									FLOAT_TO_UNSIGNED_FIXED(tmpDouble1),
									FLOAT_TO_UNSIGNED_FIXED(tmpDouble2),
									LONG_TO_UNSIGNED_FIXED(tmpLong1));
			}
			break;
// BAEResult BAESound_IsDone(BAESound sound, BAE_BOOL *outIsDone);
		case FUNC_SND_ISDONE:
			{
				err = BAESound_IsDone(gSounds[slot], &tmpBool1);
				printf(": BAESound_IsDone(slot=%d) = %d\n", slot, tmpBool1);
			}
			break;
// BAEResult BAESound_SetRate(BAESound sound, BAE_UNSIGNED_FIXED newRate);
		case FUNC_SND_SETRATE:
			{
				tmpLong1 = GetLong("New Rate (hz) > ");
				err = BAESound_SetRate(gSounds[slot],
									   LONG_TO_UNSIGNED_FIXED(tmpLong1));
			}
			break;
// BAEResult BAESound_GetRate(BAESound sound, BAE_UNSIGNED_FIXED *outRate);
		case FUNC_SND_GETRATE:
			{
				err = BAESound_GetRate(gSounds[slot], &tmpUFixed1);
				tmpLong1 = UNSIGNED_FIXED_TO_LONG_ROUNDED(tmpUFixed1);
				printf(": BAESound_GetRate(slot=%d) = %d\n", slot, tmpLong1);
			}
			break;
// BAEResult BAESound_SetLowPassAmountFilter(BAESound sound,
// 											 short int lowPassAmount);
		case FUNC_SND_SETLOWPASSAMOUNTFILTER:
			{
				tmpShort1 = (short) GetLong("Low Pass Amount > ");
				err = BAESound_SetLowPassAmountFilter(gSounds[slot], tmpShort1);
			}
			break;
// BAEResult BAESound_GetLowPassAmountFilter(BAESound sound,
// 											 short int *outLowPassAmount);
		case FUNC_SND_GETLOWPASSAMOUNTFILTER:
			{
				err = BAESound_GetLowPassAmountFilter(gSounds[slot],
													  &tmpShort1);
				printf(": BAESound_GetLowPassAmountFilter(slot=%d) = %d\n",
					slot, tmpShort1);
			}
			break;
// BAEResult BAESound_SetResonanceAmountFilter(BAESound sound,
// 											   short int resonanceAmount);
		case FUNC_SND_SETRESONANCEAMOUNTFILTER:
			{
				tmpShort1 = (short) GetLong("Resonance Amount > ");
				err = BAESound_SetResonanceAmountFilter(gSounds[slot], tmpShort1);
			}
			break;
// BAEResult BAESound_GetResonanceAmountFilter(BAESound sound,
// 											   short int *outResonanceAmount);
		case FUNC_SND_GETRESONANCEAMOUNTFILTER:
			{
				err = BAESound_GetResonanceAmountFilter(gSounds[slot],
														&tmpShort1);
				printf(": BAESound_GetResonanceAmountFilter(slot=%d) = %d\n",
					slot, tmpShort1);
			}
			break;
// BAEResult BAESound_SetFrequencyAmountFilter(BAESound sound,
// 											   short int frequencyAmount);
		case FUNC_SND_SETFREQUENCYAMOUNTFILTER:
			{
				tmpShort1 = (short) GetLong("Frequency Amount > ");
				err = BAESound_SetFrequencyAmountFilter(gSounds[slot], tmpShort1);
			}
			break;
// BAEResult BAESound_GetFrequencyAmountFilter(BAESound sound,
// 											   short int *outFrequencyAmount);
		case FUNC_SND_GETFREQUENCYAMOUNTFILTER:
			{
				err = BAESound_GetFrequencyAmountFilter(gSounds[slot],
														&tmpShort1);
				printf(": BAESound_GetFrequencyAmountFilter(slot=%d) = %d\n",
					slot, tmpShort1);
			}
			break;
// BAEResult BAESound_SetSampleLoopPoints(BAESound sound,
// 										  unsigned long start,
// 										  unsigned long end);
		case FUNC_SND_SETSAMPLELOOPPOINTS:
			{
				tmpLong1 = GetLong("Start Loop Point > ");
				tmpLong2 = GetLong("End Loop Point > ");
				err = BAESound_SetSampleLoopPoints(gSounds[slot],
												   tmpLong1,
												   tmpLong2);
			}
			break;
// BAEResult BAESound_GetSampleLoopPoints(BAESound sound,
// 										  unsigned long *outStart,
// 										  unsigned long *outEnd);
		case FUNC_SND_GETSAMPLELOOPPOINTS:
			{
				err = BAESound_GetSampleLoopPoints(gSounds[slot],
												   (unsigned long *)&tmpLong1,
												   (unsigned long *)&tmpLong2);
				printf(": BAESound_GetSampleLoopPoints(slot=%d) Start loop \
					point = ", slot);
				printf("%d\n", tmpLong1);
				printf(": BAESound_GetSampleLoopPoints(slot=%d) Start loop \
					point = ", slot);
				printf("%d\n", tmpLong2);
			}

	} // case
	if (err)
	{
		printf("\a! Error code: %s (%d)\n", BAEResultToStr(err), (int)err);
	}
	else
	{
		printf("OK.\n");
	}
}


// PV_DoScript()
// -----------------------------------------------------
//
//
static void PV_DoScript(int subMenu)
{
	BAEResult		err;
	BAESampleInfo	info;

	switch (subMenu)
	{
		case SCRIPT_SND_LOADWAVE_LOOP:
		case SCRIPT_SND_LOADWAVE:
			{
				if (gSounds[0])
				{
					BAESound_Delete(gSounds[0]);
				}
				gSounds[0] = BAESound_New(gMixer);
				err = BAESound_LoadFileSample(gSounds[0], (BAEPathName)DEFAULT_WAVE_FILE, BAE_WAVE_TYPE);
				if (err == BAE_NO_ERROR)
				{
					err = BAESound_Start(gSounds[0], 0, BAE_FIXED_1, 0);
				}
				if (subMenu == SCRIPT_SND_LOADWAVE_LOOP)
				{	// loop
					BAESound_GetInfo(gSounds[0], &info);
					BAESound_SetSampleLoopPoints(gSounds[0], 0, info.waveFrames);
				}
			}
			break;

		case SCRIPT_SND_RATESWEEP:
			{
				int i;
				BAE_FIXED f;

				for (i=1000; i<60000; i+=1000)
				{
					f = LONG_TO_UNSIGNED_FIXED(i);
					err = BAESound_SetRate(gSounds[0], f);
					BAE_WaitMicroseconds(20000);
				}

			}
			break;

		default:
			{
				err = BAE_GENERAL_BAD;
			}
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


///////////////////////////////////////////////////////////////////////////////
// Custom Implementation
///////////////////////////////////////////////////////////////////////////////



