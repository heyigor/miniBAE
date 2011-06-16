/****************************************************************************
 *
 * simple.c
 *
 * a command line audiofile player that supports RMF
 *
 * © Copyright 1999 Beatnik, Inc, All Rights Reserved.
 * Written by Mark Deggeller (mark@beatnik.com)
 *
 * Legal Notice:
 *	Beatnik products contain certain trade secrets and confidential and
 *	proprietary information of Beatnik.  Use, reproduction, disclosure
 *	and distribution by any means are prohibited, except pursuant to
 *	a written license from Beatnik. Use of copyright notice is
 *	precautionary and does not imply publication or disclosure.
 *
 * Restricted Rights Legend:
 *	Use, duplication, or disclosure by the Government is subject to
 *	restrictions as set forth in subparagraph (c)(1)(ii) of The
 *	Rights in Technical Data and Computer Software clause in DFARS
 *	252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
 *	Computer Software--Restricted Rights at 48 CFR 52.227-19, as
 *	applicable.
 *
 * Confidential - Internal use only
 *
 * History:
 *	7/30/99		Created
 *  8/11/99		Added PV_PrintRMFFields
 *  9/21/99		Added support for wav, au, aiff, mpeg files.
 *				Added BeatnikPlay(), PlaySound(), PlayRMF()
 *	10/26/99	Added PlayMIDI()
 *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MiniBAE.h"
#include "BAE_API.h"

#include "GenPriv.h"

// prototypes
static BAEFileType PV_GetFileType(char *fileName);
static void PV_PrintRMFFields(BAESong pRmfSong, unsigned long rmfSongSize);
BAEResult BeatnikPlay(char *filePath, char *patchesPath);
BAEResult PlayRMF(BAEMixer theMixer, char *fileName);


char const usageString[] =
	"USAGE:  playrmf <audiofile path> <Patches file path = Patches.hsb>\n";

static void PV_PrintSizeofs(void)
{
	GM_Voice	*this_voice;
	int			size;

	printf("sizeof this_voice->lastAmplitudeL=%d\n", sizeof(this_voice->lastAmplitudeL));
	printf("sizeof this_voice->NoteVolume=%d\n", sizeof(this_voice->NoteVolume));
	printf("sizeof this_voice->NoteVolumeEnvelope=%d\n", sizeof(this_voice->NoteVolumeEnvelope));
	printf("sizeof this_voice->NotePitch=%d\n", sizeof(this_voice->NotePitch));
	printf("sizeof this_voice->NoteWave=%d\n", sizeof(this_voice->NoteWave));
	printf("sizeof MusicGlobals->Four_Loop=%d\n", sizeof(MusicGlobals->Four_Loop));
	printf("sizeof this_voice->voiceMode=%d\n", sizeof(this_voice->voiceMode));

	printf("sizeof this_voice->z=%d\n", sizeof(this_voice->z));
	printf("sizeof this_voice->Z1value=%d\n", sizeof(this_voice->Z1value));
	printf("sizeof this_voice->zIndex=%d\n", sizeof(this_voice->zIndex));
	printf("sizeof this_voice->LPF_resonance=%d\n", sizeof(this_voice->LPF_resonance));
	printf("sizeof this_voice->LPF_frequency=%d\n", sizeof(this_voice->LPF_frequency));
	printf("sizeof this_voice->previous_zFrequency=%d\n", sizeof(this_voice->previous_zFrequency));

	printf("\nStructure sizes\n");
	printf("sizeof GM_Voice=%d\n", sizeof(GM_Voice));
	printf("sizeof GM_Mixer=%d\n", sizeof(GM_Mixer));
	printf("sizeof GM_Song=%d\n", sizeof(GM_Song));
	printf("sizeof GM_Instrument=%d\n", sizeof(GM_Instrument));
	printf("sizeof GM_Waveform=%d\n", sizeof(GM_Waveform));

	size = sizeof(GM_Voice) + sizeof(GM_Mixer) + sizeof(GM_Song) + 
			sizeof(GM_Instrument) + sizeof(GM_Waveform);
	printf("total=%d\n", size);
	printf("\npress Enter to continue\n");
	getchar();
}

// main()
// ---------------------------------------------------------------------
int main(int argc, char* argv[])
{
	char *filePath;
	char *patchesPath;

	if (argc == 1)
	{
		argc++;
#if WRITE_LOOPS==FALSE
	#if 0
		#if 01
			argv[1] = "normalwithsamples.rmf";
		#else
			argv[1] = "normalwithoutsamples.rmf";
		#endif
	#else
		argv[1] = "UncleJamesFunk.rmf";
	#endif
#else
	#if FILTER_TEST==TRUE
		argv[1] = "filteredsquarewave.rmf";
	#else
		argv[1] = "squarewave.rmf";
	#endif
#endif
	}
	if (argc != 2 && argc != 3)
	{
		printf(usageString);
		return -1;
	}
	
	filePath = argv[1];
	patchesPath = NULL;

	if (argc==3) {
		patchesPath = argv[2];
	} else {
		// no patch file given, so use default name.
		patchesPath = "NPatches.hsb";
	}

	PV_PrintSizeofs();
	return (BeatnikPlay(filePath, patchesPath) == BAE_NO_ERROR) ? 0 : -1;
}

// BeatnikPlay()
// ---------------------------------------------------------------------
//
//
BAEResult BeatnikPlay(char *filePath, char *patchesPath)
{
	BAEResult		err;
	BAEMixer		theMixer;
	short int		rmf, pcm;
	BAEBankToken	bank;

	theMixer = BAEMixer_New();
	if (theMixer)
	{
		rmf = BAE_MAX_VOICES;
		pcm = 0;
		printf("Allocating mixer with %d voices for RMF/Midi playback\n"
				"and %d voices for PCM playback\n",
				rmf, pcm);

		err = BAEMixer_Open(theMixer, 
								BAE_8K,
								BAE_LINEAR_INTERPOLATION,
								BAE_USE_16 | BAE_DISABLE_REVERB,
								rmf,	// midi voices
								pcm,	// pcm voices
								(short)((rmf+pcm)/2),
								TRUE);
		if (err == BAE_NO_ERROR)
		{
			printf("BAE memory used during idle prior to SetBankToFile: %ld\n\n", BAE_GetSizeOfMemoryUsed());
			err = BAEMixer_AddBankFromFile(theMixer, (BAEPathName)patchesPath, &bank);
			printf("BAE memory used during idle after SetBankToFile: %ld\n\n", BAE_GetSizeOfMemoryUsed());
			if (err == BAE_NO_ERROR)
			{
				err = PlayRMF(theMixer, filePath);
			}
			else
			{
				printf("playrmf:  Couldn't set mixer bank file (BAE Error #%d)\n", err);
			}
		}
		else printf("playrmf:  Couldn't open mixer (BAE Error #%d)\n", err);
	}
	else printf("playrmf:  Memory error.\n");

	BAEMixer_Delete(theMixer);

	return err;
}


// PlayRMF()
// ---------------------------------------------------------------------
//
//
BAEResult PlayRMF(BAEMixer theMixer, char *fileName)
{
	BAEResult err;
	BAESong	theSong = BAESong_New(theMixer);
	BAE_BOOL	done;

	if (theSong)
	{
		err = BAESong_LoadRmfFromFile(theSong, (BAEPathName)fileName, 0, TRUE);
		if (err == BAE_NO_ERROR)
		{
			err = BAESong_Start(theSong, 0);
			if (err == BAE_NO_ERROR)
			{
				printf("BAE memory used for everything %ld\n\n", BAE_GetSizeOfMemoryUsed());
				done = FALSE;
				while (done == FALSE)
				{
					BAESong_IsDone(theSong, &done);
					if (done == FALSE)
					{
						BAE_WaitMicroseconds(15000);
					}
				}
				BAE_WaitMicroseconds(900000);
			}
			else printf("playrmf:  Couldn't start song (BAE Error #%d)\n", err);
		}
		else printf("playrmf:  Couldn't open RMF file '%s' (BAE Error #%d)\n", fileName, err);				
	}
	else
	{
		err = BAE_MEMORY_ERR;
	}
	BAESong_Delete(theSong);
	return err;
}



// PV_PrintRMFFields()
// ---------------------------------------------------------------------
// prints info fields for an RMF song.
//
static void PV_PrintRMFFields(BAESong pRmfSong, unsigned long rmfSongSize)
{
	int				count;
	char			name[8192];
	unsigned long	size;
	static char *fields_str[] = {
		"TITLE",
		"PERFORMED BY",
		"COMPOSER",
		"COPYRIGHT",
		"PUBLISHER CONTACT",
		"USE OF LICENSE",
		"LICENSED TO URL",
		"LICENSE TERM",
		"EXPIRATION DATE",
		"COMPOSER NOTES",
		"INDEX NUMBER",
		"GENRE"
					};
	static BAEInfoType fields_idx[] = {
		TITLE_INFO,
		PERFORMED_BY_INFO,
		COMPOSER_INFO,
		COPYRIGHT_INFO,
		PUBLISHER_CONTACT_INFO,
		USE_OF_LICENSE_INFO,
		LICENSED_TO_URL_INFO,
		LICENSE_TERM_INFO,
		EXPIRATION_DATE_INFO,
		COMPOSER_NOTES_INFO,
		INDEX_NUMBER_INFO,
		GENRE_INFO
					};
	printf("\nRMF Embbeded fields:\n");
	for (count = 0; count < sizeof(fields_idx) / sizeof(BAEInfoType); count++)
	{
		size = BAEUtil_GetInfoSize(pRmfSong, rmfSongSize, 0, fields_idx[count]);
		if ( (size > 0) && (size < 8192) )
		{
			BAEUtil_GetRmfSongInfo(pRmfSong, rmfSongSize, 0, fields_idx[count], name, 8191);
			printf("%20.20s : %s\n", fields_str[count], name);
		}
	}
}
