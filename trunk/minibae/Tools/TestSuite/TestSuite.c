/*****************************************************************************/
/*
** "TestSuite.c"
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
**	1999.12.16 msd	Created.
**	2000.01.11		Added global variables here rather than in header file. Caused
**						duplication problems.
**	2000.01.14 msd	Added Pause support.
**	2000.01.17 msd	Added Pause to the main menu
**	2000.01.19 msd	Added GetMenuChoice() - menu commands 'n', 'p', and 'h', 
**	2000.01.21 msd	Implemented test logging, using LogText() and global logFile
**	2000.02.27 msd	Added call to BAE_SetupMsgSender to initialize message handling
**						on dual cpu configuration
**	2000.02.28 msd	Added Memory Usage option
**	2000.03.20 msd	Version 1.00 r2
**	2000.03.24 msd	Version 1.00 r3
**	2000.03.27		Renamed BAE_BOTH to BAE_COMPLETE
**	2000.03.30 msd	Version 1.00 r4
**					Changed copyright, modification history format
**	2000.03.31 msd	Version 1.00 r5
**	2000.05.18 sh	Version 1.00 r8
**	2000.05.25 sh	Removed use of X_Assert.h and XLStrCmp in GetMenuChoice
**	2000.05.25 msd	Removed reliance on calling messaging API's directly.
**						Now use BAE_GetSizeOfMemoryUsedDSP() and 
**						BAE_GetMaxSizeOfMemoryUsedDSP() instead.
**	10/17/2000	sh	Added in the main loop the call to BAEMixer_Idle.
**					Added macro for MAIN
**	10/18/2000	sh	Added a loop count for idle.
*/
/*****************************************************************************/

#include <stdio.h>
#include "TestSuite.h"
#include "MiniBAE.h"
#include "TestTools.h"
#include "BAEMixerTest.h"
#include "BAESoundTest.h"
#include "BAESongTest.h"
#include "BAE_API.h"

#if BAE_MCU
	#include "BAE_API_Msg.h"
#endif

// Prototypes
//-------------------------------
void	Initialize(void);
void	PrintWelcome(void);
void	PrintHelp(void);
void	PrintMainMenu(void);
int		DoUserChoice(void);
void	DoMenu(void);
int		DoSubMenu(int menu);
void	PrintMenuPage(int pageNum);
void	PrintSubMenuPage(int menu, int pageNum);
long	GetMenuChoice(char *prompt);
void	PrintMemoryUsage(void);

// #defines
//-------------------------------
#define ITEMS_PER_PAGE			23
#define LOG_FILE_PATH			"test.log"

// Globals
//-------------------------------
static	GetNumMenuItemsFn		gGetNumMenuItems;
static	GetNumSubMenuItemsFn	gGetNumSubMenuItems;
static	GetMenuTextFn			gGetMenuText;
static	GetSubMenuTextFn		gGetSubMenuText;
static	DoMenuItemFn			gDoMenuItem;

BAEMixer						gMixer;
BAESong							gSongs[MAX_SLOTS];
BAESound						gSounds[MAX_SLOTS];
FILE							*logFile;
BAE_BOOL						verboseMode;

enum {
	MENU_PREV_PAGE	= -1,
	MENU_NEXT_PAGE	= -2,
	MENU_HELP		= -3
};


// main()
// -----------------------------------------
//
//
// in case you need to redefine and call from another main()
#if 1	
	#define MAIN	main
#else
	#define MAIN	Test_main
#endif

int MAIN()
{
	int	count;

#if BAE_MCU
	if (BAE_SetupMsgSender() == BAE_MSG_NO_ERROR)
	{
#endif

		Initialize();
		PrintWelcome();
		while (DoUserChoice() != 0)
		{
			// do a few idle prior to locking in keyboard wait mode
			for (count = 0; count < 50; count++)
			{
				BAEMixer_Idle(gMixer);
			}
		}

#if BAE_MCU
		BAE_TearDownMsgSender();
	}
	else
	{
		printf("\a\n###\n### You must start the DSP application first.\n###\n\n\n");
	}
#endif

	// if mixer is still there, delete it
	if (gMixer)
	{
		BAEMixer_Delete(gMixer);
		gMixer = NULL;
	}
	return 0;
}


// DoUserChoice()
// -----------------------------------------
//
//
int DoUserChoice(void)
{
	long choice;
	int		pauseTime;

	PrintMainMenu();

	while ((choice=GetLong("> ")) < 0 || choice > 6)
	{
		printf("Invalid option.  Try again: \n");
	}

	if (choice != 0)
	{
		switch (choice)
		{
			case 1:
				gGetNumMenuItems = MixerTest_GetNumMenuItems;
				gGetNumSubMenuItems = MixerTest_GetNumSubMenuItems;
				gGetMenuText = MixerTest_GetMenuItem;
				gGetSubMenuText = MixerTest_GetSubMenuItem;
				gDoMenuItem = MixerTest_DoMenuItem;
				break;
			case 2:
				gGetNumMenuItems = SoundTest_GetNumMenuItems;
				gGetNumSubMenuItems = SoundTest_GetNumSubMenuItems;
				gGetMenuText = SoundTest_GetMenuItem;
				gGetSubMenuText = SoundTest_GetSubMenuItem;
				gDoMenuItem = SoundTest_DoMenuItem;
				break;
			case 3:
				gGetNumMenuItems = SongTest_GetNumMenuItems;
				gGetNumSubMenuItems = SongTest_GetNumSubMenuItems;
				gGetMenuText = SongTest_GetMenuText;
				gGetSubMenuText = SongTest_GetSubMenuText;
				gDoMenuItem = SongTest_DoMenuItem;
				break;
			case 4:
				pauseTime = GetLong("Sleep time (sec) : ");
				BAE_WaitMicroseconds(pauseTime*1000000);
				return 4;
				break;
			case 5:
				verboseMode = !verboseMode;
				if (verboseMode)
				{
					LogText("\n----------------------------\n- start log\n----------------------------\n");
					printf("Logging started.\n");
				}
				else
				{
					printf("Logging stopped.\n");
				}
				return 5;
			case 6:
				PrintMemoryUsage();

				return 6;

			default:
				break;
		}
		DoMenu();
	}
	return choice;
}



// PrintMenuPage()
// --------------------------------------------
//
//
void PrintMenuPage(int pageNum)
{
	int		i;
	int		itemNum;
	const char	*menuText;
	int		maxPageNum;

	maxPageNum = (gGetNumMenuItems() / ITEMS_PER_PAGE) + 1;
	printf("Page %d of %d:\n", pageNum, maxPageNum);

	for (i=1; i<=ITEMS_PER_PAGE; i++)
	{
		itemNum = (pageNum-1)*ITEMS_PER_PAGE + i;
		if (gGetMenuText(itemNum, &menuText))
		{
			printf("      %d) ", itemNum);
			if (itemNum < 10) printf(" ");
			printf("%s\n", menuText);
		}
		else
		{
			break;
		}
	}
}



// PrintSubMenuPage()
// --------------------------------------------
//
//
void PrintSubMenuPage(int menu, int pageNum)
{
	int		i;
	int		itemNum;
	const char	*menuText;
	int		maxPageNum;

	maxPageNum = (gGetNumSubMenuItems(menu) / ITEMS_PER_PAGE) + 1;
	printf("Page %d of %d:\n", pageNum, maxPageNum);

	for (i=1; i<=ITEMS_PER_PAGE; i++)
	{
		itemNum = (pageNum-1)*ITEMS_PER_PAGE + i;
		if (gGetSubMenuText(menu, itemNum, &menuText))
		{
			printf("         %d) ", itemNum);
			if (itemNum < 10) printf(" ");
			printf("%s\n", menuText);
		}
		else
		{
			break;
		}
	}
}



// DoMenu()
// --------------------------------------------
//
//
void DoMenu(void)
{
	long	menuChoice, maxMenuChoice;
	long	subMenuChoice;
	long	pageNum, maxPageNum;
	const char	*menuText;

	maxMenuChoice = gGetNumMenuItems();
	pageNum = 1;
	maxPageNum = (maxMenuChoice / ITEMS_PER_PAGE) + 1;

	do {
		while (1)
		{
			PrintMenuPage(pageNum);
			menuChoice = GetMenuChoice("#/n/p>");
			if (menuChoice == 0)		// cancel
			{
				return;
			}
			else if (menuChoice == MENU_PREV_PAGE)	// previous page
			{
				if (pageNum > 1) pageNum--;
			}
			else if (menuChoice == MENU_NEXT_PAGE)	// next page
			{
				if (pageNum < maxPageNum) pageNum++;
			}
			else if (menuChoice == MENU_HELP)
			{
				PrintHelp();
			}
			else if (menuChoice < 0 || menuChoice > maxMenuChoice)
			{
				printf(": Invalid Option.  Try again.\n");
			}
			else
			{
				break;
			}
		}
	} while ((subMenuChoice=DoSubMenu(menuChoice))==0);

	if (gGetSubMenuText(menuChoice, subMenuChoice, &menuText))
	{
		printf("%s:\n", menuText);
	}


	if (verboseMode)
	{
		logFile = fopen(LOG_FILE_PATH, "a+");
		fprintf(logFile, "// %s\n", menuText);
		fclose(logFile);
	}

	gDoMenuItem(menuChoice, subMenuChoice);
	LogText("\n\n");
}



// DoSubMenu()
// --------------------------------------------
//
//
int DoSubMenu(int menu)
{
	long	subMenuChoice, maxSubMenuChoice;
	long	pageNum, maxPageNum;

	maxSubMenuChoice = gGetNumSubMenuItems(menu);
	pageNum = 1;
	maxPageNum = (maxSubMenuChoice / ITEMS_PER_PAGE) + 1;

	while (1)
	{
		PrintSubMenuPage(menu, pageNum);
		subMenuChoice = GetMenuChoice("#/n/p> ");

		if (subMenuChoice == 0)		// cancel
		{
			break;
		}
		else if (subMenuChoice == MENU_PREV_PAGE)	// previous page
		{
			if (pageNum > 1) pageNum--;
		}
		else if (subMenuChoice == MENU_NEXT_PAGE)	// next page
		{
			if (pageNum < maxPageNum) pageNum++;
		}
		else if (subMenuChoice == MENU_HELP)
		{
			PrintHelp();
		}
		else if (subMenuChoice < 0 || subMenuChoice > maxSubMenuChoice)
		{
			printf(": Invalid Option.  Try again.\n");
		}
		else
		{
			break;
		}
	}
	return subMenuChoice;
}



// Initialize()
// -----------------------------------------
//
//
void Initialize(void)
{
	int i;

	gMixer = NULL;

	for (i=0; i<MAX_SLOTS; i++)
	{
		gSounds[i] = NULL;
		gSongs[i] = NULL;
	}

	logFile = NULL;
	verboseMode = FALSE;
}



// PrintWelcome()
// -----------------------------------------
//
//
void PrintWelcome(void)
{
	printf("#####################################################\n");
	printf("#####          MiniBAE 1.00 Test Suite          #####\n");
	printf("#####                release 8                  #####\n");
	printf("#####              May 18, 2000               #####\n");
	printf("#####################################################\n");
	printf("\n");
}


// PrintHelp()
// -----------------------------------------
//
//
void PrintHelp(void)
{
	printf("TestSuite help:\n");
	printf("\ttype '0' (zero) to cancel and go up a menu level\n");
	printf("\ttype 'n' for next menu page\n");
	printf("\ttype 'p' for previous menu page\n");
	printf("\n");
}


// PrintMainMenu()
// -----------------------------------------
//
//
void PrintMainMenu(void)
{
	printf("\n");
	printf("Choose one of the following:\n");
	printf("   1) BAEMixer\n");
	printf("   2) BAESound\n");
	printf("   3) BAESong\n");
	printf("   4) Pause\n");
	printf("   5) Turn logging %s\n", (verboseMode) ? "off" : "on");
	printf("   6) Memory Usage\n");
	printf("   0) Quit\n");
}


void FailedTest(const char *text)
{
	printf("\a! %s\n", text);
}


long GetMenuChoice(char *prompt)
{
	long	result;
	char	termChar;
	char	buffer[256];

	while (1)
	{
		GetLine(prompt, buffer, 256);

		// check for special characters
		if (strcmpnocase(buffer, "n")==0)
		{
			return MENU_NEXT_PAGE;
		}
		else if (strcmpnocase(buffer, "p")==0)
		{
			return MENU_PREV_PAGE;
		}
		else if (strcmpnocase(buffer, "h")==0)
		{
			return MENU_HELP;
		}


		switch (sscanf(buffer, "%ld%c", &result, &termChar))
		{
			case 1:
				return result;
			case 2:
				printf("Unexpected character: '%c'\n", termChar);
				break;
			default:
				break;
		}
		printf("Try again:\n");
	}
}


void LogText(char *text)
{
	if (verboseMode)
	{
		logFile = fopen(LOG_FILE_PATH, "a+");
		fprintf(logFile, "%s\n", text);
		fclose(logFile);
	}
}


void PrintMemoryUsage(void)
{
	unsigned long	size, count, total;
	BAEResult		err;

	#if BAE_MCU

		// MCU memory
		printf("Current MCU memory usage = %d\n", BAE_GetSizeOfMemoryUsed());
		printf("Max MCU memory usage = %d\n", BAE_GetMaxSizeOfMemoryUsed());

		// DSP memory
		printf("Current DSP memory usage = %d\n", BAE_GetSizeOfMemoryUsedDSP());
		printf("Max DSP memory usage = %d\n", BAE_GetMaxSizeOfMemoryUsedDSP());

	#elif BAE_COMPLETE

		printf("Current memory usage = %d\n", BAE_GetSizeOfMemoryUsed());
		printf("Max memory usage = %d\n", BAE_GetMaxSizeOfMemoryUsed());

		printf("---\nStructure sizes, internal structures\n");
		total = 0;
		err = BAEMixer_GetMemoryUsed(gMixer, &size);
		total += size;
		printf("Mixer = %ld\n", size);

		for (count = 0; count < MAX_SLOTS; count++)
		{
			err = BAESong_GetMemoryUsed(gSongs[count], &size);
			total += size;
			printf("Song[%ld] = %ld\n", count, size);
		}

		for (count = 0; count < MAX_SLOTS; count++)
		{
			err = BAESound_GetMemoryUsed(gSounds[count], &size);
			total += size;
			printf("Sound[%ld] = %ld\n", count, size);
		}
		printf("Total %ld\n", total);
	#endif
}