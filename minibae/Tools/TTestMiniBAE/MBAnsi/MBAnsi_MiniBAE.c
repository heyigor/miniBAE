/*****************************************************************************/
/*
** "MiniBAE.c"
**
**	Generalized Audio Synthesis package presented in an oop fashion
**
**	© Copyright 1999-2000 Beatnik, Inc, All Rights Reserved.
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
**	1999.01.05	sh	Fixed bug in BAEMixer_GetGroovoidNameFromBank().
**	2000.01.06 MSD	Added NULL_OBJECT and RESOURCE_NOT_FOUND error codes.
**	2000.01.10 MSD	Made sure that BAESong can't get stuck in uninitialized mode
**					caused during BAESong_Load*().
**					Added PV_BAESong_InitLiveSong()
**	2000.01.11		Removed the Mac hook support.
**					Fixed some random error enum translation problems.
**	2000.01.13		Changed XConvertNativeFileToXFILENAME() calls to 
**						XConvertPathToXFILENAME()
**					Removed some MacOS deadwood I left in...
**	2000.01.14		Added BAEMixer_IsAudioActive()
**					Added BAESong_AreMidiEventsPending()
**	2000.01.17		Added more parameter checking in BAEMixer_Open()
**	2000.01.18		Added check to see if mixer is allocated in GetMidiVoices,
**						GetSoundVoices, and GetMixLevel.
**					Added a linked list to the BAEMixer object to track song and
**					sound objects associated with it
**	2000.01.19		Fixed problem where BAEMixer->pMixer could could change
**					behind its back, on BAEMixer_Open() and BAEMixer_Close()
**	2000.01.20		Fixed some error code transistions. OPErr from BAEResult. Caused warnings
**					Fixed defined structure BAEObjectListElem misspelling.
**	2000.01.31		Removed all direct access to GM_Mixer, GM_Song, and GM_Waveform
**						structures they all go through a GenAPI function instead.
**					Removed all references to MusicGlobals
**					Reworked mechanism to know if the mixer has been allocated.
**	2000.02.01		Added BAE_TranslateQuality(), BAE_TranslateBAEQuality()
**					Fixed lack of casting result of XNewPtr() in LoadCustomSample()
**	2000.02.25		Changed PV_BAESong_InitLiveSong & BAESong_LoadGroovoid & 
**						BAESong_LoadMidiFromMemory
**					BAESong_LoadMidiFromFile & BAESong_LoadRmfFromMemory & 
**						BAESong_LoadRmfFromFile to call GM_SetSongMixer, or 
**						pass a GM_Mixer to GM_LoadSong
**	2000.03.01		Added multiple bank support
**	2000.03.02		Added BAEMixer_UnloadBanks(), added typedef BAEBankToken
**	2000.03.06 MSD	Added use of GM_GetProgramBank() in BAE_GetProgramBank()
**					Added support for 32kHz, 40kHz
**	2000.03.07		Fixed bug in BAEMixer_UnloadBanks in which if there's no bank
**						open it returns BAE_NO_ERROR rather than an unset variable.
**	2000.03.08		Fixed BAEMixer_GetBankVersion() to do the right thing.
**					Added PV_BAEMixer_SubmitBankOrder()
**	2000.03.13 msd	cleaned up BAESound loading
**	2000.03.15 msd	fixed bug in PV_BAEMixer_RemoveObject()
**	2000.03.20 msd	fixed bugs in BAEMixer_BringBankToFront(), and BAEMixer_SendBankToBack().
**	2000.03.21 AER	Moved to new caching model where the mixer always caches
**						and uses unique bank token to differentiate entries
**					Removed references to GM cache functions
**					Revised reference to GM_LoadSong to send bank tokens
**	2000.03.23 msd	fixed memory leaks in loading BAESong's; I wasn't freeing pXSong ptrs.
**	2000.03.23 msd	changed ...PitchOffset() functions to ...Transpose()
**	2000.03.28 msd	fixed off-by-one error in BAESong_GetLoopMax()
**					created mVolume member of struct sBAESong, which shadows
**						song volume.  this fixes the problem of BAESong_Stop()
**						with fade=true resulting in the songs volume set to 0.
**						Now, subsequent calls to BAESong_Start() will playback
**						at the pre-fade volume.
**	2000.03.29 msd	Changed copyright and modification history format
**					Fixed bug BAESong_GetBankVersion() when no banks loaded.
**					Removed BAESong_Set/GetLoopFlag()
**					Renamed BAESong_Set/GetLoopMax() BAESong_Set/GetLoops()
**					Added parameter checking to BAEMixer_SetMasterSoundEffectsVolume()
**					Eliminated MCU-side access to DSP-based GM_Song ptr in
**						BAEMixer_GetRealtimeStatus().
**	2000.03.29 AER	Added GM_ClearSampleCache to free potential reource leaks
**						prior to releasing the mixer pointer
**	2000.03.30 AER	Relegated use of GM_ClearSampleCache to release builds
**	2000.03.31 MSD	BAEMixer_SetAudioLatency() is no longer supported in the
**						dual-cpu build.
**	2000.04.03 sh	Removed some warnings in BAESound_LoadCustomSample
**	2000.10.17	sh	Added BAEMixer_Idle
**	2000.10.18	sh	Added BAEMixer_GetMemoryUsed, BAESound_GetMemoryUsed, 
**					BAESong_GetMemoryUsed
**	2000.11.8	sh	Added copyright.
*/
/*****************************************************************************/

#include "MiniBAE.h"
#include "X_API.h"
#include "GenSnd.h"
#include "GenPriv.h"
#include "X_Formats.h"
#include "BAE_API.h"
#include "X_Assert.h"
#include <limits.h>

#include <stdio.h>  // for DUMP_OUTPUTFILE
// forward declarations. tom
void BAEMixer_StopOutputToFile(void);

// Private types/structs
// ----------------------------------------------------------------------------

typedef enum {
	BAE_MIXER_OBJECT = 1,
	BAE_SONG_OBJECT,
	BAE_SOUND_OBJECT
} BAE_OBJECT_TYPE;


typedef struct BAEObjectListElem {
	void						*object;
	BAE_OBJECT_TYPE				 type;
	struct BAEObjectListElem	*next;
} BAEObjectListElem;


struct sBAEMixer {
	GM_Mixer			*pMixer;		// Don't dereference pMixer, since if you are running
										// the dual-CPU version of MiniBAE, this will be only a reference.
	BAE_BOOL			 audioEngaged;
	XFILE				*pPatchFiles;
	short				 numPatchFiles;
	BAEObjectListElem	*pObjects;
};


struct sBAESong {
	GM_Song 			*pSong;			// Don't dereference pMixer, since if you are running
										// the dual-CPU version of MiniBAE, this will be only a reference.
	BAEMixer			 mixer;
	XSWORD				 mVolume;
};


struct sBAESound {
	GM_Waveform			*pWave;			// Don't dereference pMixer, since if you are running
										// the dual-CPU version of MiniBAE, this will be only a reference.
	BAEMixer			 mixer;
	VOICE_REFERENCE		 voiceRef;
	BAE_UNSIGNED_FIXED	 mPauseVariable;
};


// MiniBAE.c globals
// ----------------------------------------------------------------------------
static XShortResourceID	midiSongCount = 0;			// everytime a new song is loaded, this is increments
													// this is used as an ID for song callbacks and such


// Prototypes
// ----------------------------------------------------------------------------
BAEResult			BAE_TranslateOPErr(OPErr theErr);
OPErr				BAE_TranslateBAErr(BAEResult theErr);
BAEQuality			BAE_TranslateQuality(Quality q);
Quality				BAE_TranslateBAEQuality(BAEQuality q);

#if USE_HIGHLEVEL_FILE_API != FALSE
AudioFileType		BAE_TranslateBAEFileType(BAEFileType fileType);
#endif

#if REVERB_USED != REVERB_DISABLED
ReverbMode			BAE_TranslateFromBAEReverb(BAEReverbType igorVerb);
BAEReverbType		BAE_TranslateToBAEReverb(ReverbMode r);
#endif


// Private function prototypes
// ----------------------------------------------------------------------------
static	BAEResult	PV_BAEMixer_AddObject(BAEMixer mixer, void *theObject, BAE_OBJECT_TYPE type);
static	BAEResult	PV_BAEMixer_RemoveObject(BAEMixer mixer, void *theObject, BAE_OBJECT_TYPE type);

static	BAEResult	PV_BAEMixer_AddBank(BAEMixer mixer, XFILE newPatchFile);
static	void		PV_BAEMixer_SubmitBankOrder(BAEMixer mixer);
//static	BAEResult	PV_BAEMixer_KillBank(BAEMixer mixer, XFILE patchFile);

static	BAEResult	PV_BAESound_PreStart(
						BAESound sound,
						BAE_UNSIGNED_FIXED sampleVolume,
						BAE_BOOL stopIfPlaying);

static	BAEResult	PV_BAESong_InitLiveSong(BAESong song);

static	void		PV_BAESong_Unload(BAESong song);

static	BAETerpMode	PV_TranslateTerpModeToBAETerpMode(TerpMode tm_in);

static	double		PV_CalculateTimeDeltaForFade(
						BAE_FIXED sourceVolume,
						BAE_FIXED destVolume,
						BAE_FIXED timeInMiliseconds);



#if 0
	#pragma mark -
	#pragma mark ##### Support functions #####
	#pragma mark -
#endif

#if X_PLATFORM == X_MACINTOSH
#if 0
// Menu hook will allow code to run while a menu is being held down
static MenuHookUPP saveHook;
static MenuHookUPP newHook;

static pascal void PV_MacDoMenuHook(void)
{
}

static void PV_MacSetupMenuHook(void)
{
	saveHook = (MenuHookUPP)LMGetMenuHook();
	newHook = NewMenuHookProc(PV_MacDoMenuHook);
	LMSetMenuHook((UniversalProcPtr)newHook);
}

static void PV_MacCleanupMenuHook(void)
{
	LMSetMenuHook((UniversalProcPtr)saveHook);
	DisposeRoutineDescriptor(newHook);
}

// GetNextEvent hook will allow code to run every time GetNextEvent is called outside of
// the clients codebase
static GNEFilterUPP	saveGNEHook;
static GNEFilterUPP	newGNEHook;

static void PV_MacDoGNEHook(EventRecord *theEvent, Boolean *result)
{
	theEvent;
	result;
}

static void PV_MacSetupGNEFilter(void)
{
	saveGNEHook = LMGetGNEFilter();
	newGNEHook = NewGetNextEventFilterProc(PV_MacDoGNEHook);
	LMSetGNEFilter(newGNEHook);

}

static void PV_MacCleanupGNEFilter(void)
{
	LMSetGNEFilter(saveGNEHook);	
}
#endif	// 0
#endif	// X_PLATFORM == X_MACINTOSH


// Read a file into memory and return an allocated pointer
static XPTR PV_GetFileAsData(XFILENAME *pFile, long *pSize)
{
	XPTR	data;

	if (XGetFileAsData(pFile, &data, pSize))
	{
		data = NULL;
	}
	return data;
}

/*
static INLINE XFILE PV_GetIndexedFile(XFILE *fileList, unsigned long fileIndex)
{
	if (fileList)
	{
		return fileList[fileIndex];
	}
	return (XFILE)NULL;
}

static INLINE void PV_SetIndexedFile(XFILE *fileList, unsigned long fileIndex, XFILE file)
{
	if (fileList)
	{
		fileList[fileIndex] = file;
	}
}

// given an list of xfiles, a count of xfiles, and a file path; open the file, expand the list
// and store the file index. Returns NULL if file fails to open, or memory allocation failure
static XFILE * PV_OpenToFileList(XFILE * files, unsigned long fileCount, BAEPathName pAudioPathName)
{
	XFILENAME		theFile;
	XFILE			*newFileList;
	unsigned long	size;
	XFILE			file;

	newFileList = NULL;
	// everythings ok, so open the file
	XConvertPathToXFILENAME(pAudioPathName, &theFile);
	file = XFileOpenResource(&theFile, TRUE);
	if (file)
	{
		XFileUseThisResourceFile(file);
		size = sizeof(XFILE) * (fileCount + 1);
		newFileList = (XFILE *)XNewPtr(size);
		if (newFileList)
		{
			if (files)
			{
				XBlockMove(files, newFileList, size - sizeof(XFILE));
				XDisposePtr(files);
			}
	
			PV_SetIndexedFile(newFileList, fileCount, file);
		}
	}
	return newFileList;
}

// given a list of xfiles, a count of xfiles, and a xfile index to delete. shrink the list, close the file and remove
// from list
static XFILE * PV_CloseFromFileList(XFILE * files, unsigned long fileCount, unsigned long thisFileIndex)
{
	unsigned long	size, count, count2;
	XFILE			*newFileList;
	XFILE			xfile, file;

	newFileList = files;	
	if (thisFileIndex < fileCount)
	{
		xfile = PV_GetIndexedFile(files, thisFileIndex);
		if (xfile)
		{
			XFileClose(xfile);
		}
		if (fileCount > 0)
		{	// something in the list
			fileCount--;
			size = sizeof(XFILE) * fileCount;
			newFileList = (XFILE *)XNewPtr(size);
			if (newFileList)
			{
				// copy all except file to remove
				for (count = 0, count2 = 0; count < fileCount; count++)
				{
					file = PV_GetIndexedFile(files, count);
					if (xfile != file)
					{
						PV_SetIndexedFile(newFileList, count2++, file);
					}
				}
			}
		}
		else
		{	// empty, so return empty list
			newFileList = NULL;
		}
	}
	return newFileList;
}

// close all files in file list, and delete memory
static void PV_CloseAllFromFileList(XFILE * files, unsigned long fileCount)
{
	unsigned long	count;
	XFILE			file;

	if (fileCount > 0)
	{	// something in the list
		// copy all except file to remove
		for (count = 0; count < fileCount; count++)
		{
			file = PV_GetIndexedFile(files, count);
			if (file)
			{
				XFileClose(file);
			}
		}
		XDisposePtr((XPTR)files);
	}
}
*/

#if REVERB_USED != REVERB_DISABLED
static const ReverbMode translateInternal[] = {
								REVERB_NO_CHANGE,
								REVERB_TYPE_1,
								REVERB_TYPE_2,
								REVERB_TYPE_3,
								REVERB_TYPE_4,
								REVERB_TYPE_5,
								REVERB_TYPE_6,
								REVERB_TYPE_7,
								REVERB_TYPE_8,
								REVERB_TYPE_9,
								REVERB_TYPE_10,
								REVERB_TYPE_11
											};
static const BAEReverbType translateExternal[] = {
								BAE_REVERB_NO_CHANGE,
								BAE_REVERB_TYPE_1,
								BAE_REVERB_TYPE_2,
								BAE_REVERB_TYPE_3,
								BAE_REVERB_TYPE_4,
								BAE_REVERB_TYPE_5,
								BAE_REVERB_TYPE_6,
								BAE_REVERB_TYPE_7,
								BAE_REVERB_TYPE_8,
								BAE_REVERB_TYPE_9,
								BAE_REVERB_TYPE_10,
								BAE_REVERB_TYPE_11
											};
// translate reverb types from BAEReverbType to ReverbMode
ReverbMode BAE_TranslateFromBAEReverb(BAEReverbType igorVerb)
{
	ReverbMode				r;
	short int				count;

	r = REVERB_TYPE_1;
	for (count = 0; count < MAX_REVERB_TYPES; count++)
	{
		if (igorVerb == translateExternal[count])
		{
			r = translateInternal[count];
			break;
		}
	}
	return r;
}

// translate reverb types to BAEReverbType from ReverbMode
BAEReverbType BAE_TranslateToBAEReverb(ReverbMode r)
{
	BAEReverbType			igorVerb;
	short int				count;

	igorVerb = BAE_REVERB_TYPE_1;
	for (count = 0; count < MAX_REVERB_TYPES; count++)
	{
		if (r == translateInternal[count])
		{
			igorVerb = translateExternal[count];
			break;
		}
	}
	return igorVerb;
}
#endif


static const BAEQuality translateExternalQuality[] = {
	BAE_8K,					// 8 kHz
	BAE_11K_TERP_22K,		// 11 kHz interpolated to 22 kHz
	BAE_11K,				// 11 kHz
	BAE_16K,				// 16 kHz
	BAE_22K,				// 22 kHz
	BAE_22K_TERP_44K,		// 22 kHz interpolated to 44 kHz
	BAE_24K,				// 24 kHz
	BAE_32K,				// 32 kHz
	BAE_40K,				// 40 kHz
	BAE_44K,				// 44 kHz
	BAE_48K					// 48 kHz
};


static const Quality translateInternalQuality[] = {
	Q_8K,					// 8 kHz
	Q_11K_TERP_22K,			// 11 kHz terped to 22 kHz
	Q_11K,					// 11 kHz
	Q_16K,					// 16 kHz
	Q_22K,					// 22 kHz
	Q_22K_TERP_44K,			// 22 kHz terped to 44 kHz
	Q_24K,					// 24 kHz
	Q_32K,					// 32 kHz
	Q_40K,					// 40 kHz
	Q_44K,					// 44 kHz
	Q_48K					// 48 kHz
};
										
// Translate from Quality to BAEQuality
BAEQuality BAE_TranslateQuality(Quality q)
{
	BAEQuality	result;
	short int	count,  max;

	result = BAE_8K;
	max = sizeof(translateExternalQuality) / sizeof(BAEQuality);
	for (count = 0; count < max; count++)
	{
		if (translateInternalQuality[count] == q)
		{
			result = translateExternalQuality[count];
			break;
		}
	}
	return result;
}


// Translate from BAEQuality to Quality
Quality BAE_TranslateBAEQuality(BAEQuality q)
{
	Quality 	result;
	short int	count,  max;

	result = Q_44K;
	max = sizeof(translateExternalQuality) / sizeof(BAEQuality);
	for (count = 0; count < max; count++)
	{
		if (translateExternalQuality[count] == q)
		{
			result = translateInternalQuality[count];
			break;
		}
	}
	return result;
}

static const BAEResult translateExternalError[] = {
										BAE_NO_ERROR,
										BAE_BUFFER_TOO_SMALL,
										BAE_NOT_SETUP,
										BAE_PARAM_ERR,
										BAE_MEMORY_ERR,
										BAE_BAD_INSTRUMENT,
										BAE_BAD_MIDI_DATA,
										BAE_ALREADY_PAUSED,
										BAE_ALREADY_RESUMED,
										BAE_DEVICE_UNAVAILABLE,
										BAE_STILL_PLAYING,
										BAE_NO_SONG_PLAYING,
										BAE_TOO_MANY_SONGS_PLAYING,
										BAE_NO_VOLUME,
										BAE_NO_FREE_VOICES,
										BAE_STREAM_STOP_PLAY,
										BAE_BAD_FILE_TYPE,
										BAE_GENERAL_BAD,
										BAE_BAD_SAMPLE,
										BAE_BAD_FILE,
										BAE_FILE_NOT_FOUND,
										BAE_NOT_REENTERANT,
										BAE_SAMPLE_TOO_LARGE,
										BAE_UNSUPPORTED_HARDWARE,
										BAE_ABORTED,
										BAE_RESOURCE_NOT_FOUND,
										BAE_NULL_OBJECT
									};


static const OPErr translateInternalError[] = {
										NO_ERR,
										BUFFER_TO_SMALL,
										NOT_SETUP,
										PARAM_ERR,
										MEMORY_ERR,
										BAD_INSTRUMENT,
										BAD_MIDI_DATA,
										ALREADY_PAUSED,
										ALREADY_RESUMED,
										DEVICE_UNAVAILABLE,
										STILL_PLAYING,
										NO_SONG_PLAYING,
										TOO_MANY_SONGS_PLAYING,
										NO_VOLUME,
										NO_FREE_VOICES,
										STREAM_STOP_PLAY,
										BAD_FILE_TYPE,
										GENERAL_BAD,
										BAD_SAMPLE,
										BAD_FILE,
										FILE_NOT_FOUND,
										NOT_REENTERANT,
										SAMPLE_TO_LARGE,
										UNSUPPORTED_HARDWARE,
										ABORTED_PROCESS,
										RESOURCE_NOT_FOUND,
										NULL_OBJECT
									};
										
// Translate from OPErr to BAEResult
BAEResult BAE_TranslateOPErr(OPErr theErr)
{
	BAEResult		igorErr;
	short int	count,  max;

	igorErr = BAE_GENERAL_ERR;
	max = sizeof(translateExternalError) / sizeof(BAEResult);
	for (count = 0; count < max; count++)
	{
		if (translateInternalError[count] == theErr)
		{
			igorErr = translateExternalError[count];
			break;
		}
	}
	return igorErr;
}


// Translate from BAEResult to OPErr
OPErr BAE_TranslateBAErr(BAEResult theErr)
{
	OPErr		igorErr;
	short int	count,  max;

	igorErr = GENERAL_BAD;
	max = sizeof(translateExternalError) / sizeof(BAEResult);
	for (count = 0; count < max; count++)
	{
		if (translateExternalError[count] == theErr)
		{
			igorErr = translateInternalError[count];
			break;
		}
	}
	return igorErr;
}




#if USE_HIGHLEVEL_FILE_API != FALSE
AudioFileType BAE_TranslateBAEFileType(BAEFileType fileType)
{
	AudioFileType	haeFileType;

	haeFileType = FILE_INVALID_TYPE;
	switch (fileType)
	{
		case BAE_AIFF_TYPE:
			haeFileType = FILE_AIFF_TYPE;
			break;
		case BAE_WAVE_TYPE:
			haeFileType = FILE_WAVE_TYPE;
			break;
#if USE_MPEG_DECODER != FALSE
		case BAE_MPEG_TYPE:
			haeFileType = FILE_MPEG_TYPE;
			break;
#endif
		case BAE_AU_TYPE:
			haeFileType = FILE_AU_TYPE;
			break;
	}
	return haeFileType;
}
#endif

// ------------------------------------------------------------------
// BAEMixer Functions
// ------------------------------------------------------------------
#if 0
	#pragma mark -
	#pragma mark ##### BAEMixer #####
	#pragma mark -
#endif


// BAEMixer_New
// ------------------------------------
//
//
BAEMixer BAEMixer_New(void)
{
	BAEMixer mixer;

	mixer = (BAEMixer)XNewPtr(sizeof(struct sBAEMixer));
	if (mixer)
	{
		mixer->pMixer = NULL;
		mixer->audioEngaged = FALSE;
		mixer->pPatchFiles = NULL;
		mixer->pObjects = NULL;
	}
	return mixer;
}


// BAEMixer_Delete()
// ------------------------------------
//
//
BAEResult BAEMixer_Delete(BAEMixer mixer)
{
	BAEResult err;
	BAEObjectListElem *elem, *next;
	
	//	2000.03.29 AER	In case of resource leaking, we need to wipe this...
#ifndef _DEBUG	//	Only to this nasty bit in release builds
	GM_ClearSampleCache(mixer->pMixer);
#endif
	err = BAEMixer_Close(mixer);
	if (err == BAE_NO_ERROR)
	{
		elem = mixer->pObjects;
		while (elem)
		{
			next = elem->next;
			switch (elem->type)
			{
				case BAE_SONG_OBJECT:
					((BAESong)elem->object)->mixer = NULL;
					break;
	
				case BAE_SOUND_OBJECT:
					((BAESound)elem->object)->mixer = NULL;
					break;
				
				case BAE_MIXER_OBJECT:
					BAE_ASSERT(FALSE);
					break;
			}
			XDisposePtr(elem);
			elem = next;
		}

		BAEMixer_UnloadBanks(mixer);

		XDisposePtr(mixer);
	}
	return err;
}

// BAEMixer_GetMemoryUsed()
// --------------------------------------
//
//
BAEResult BAEMixer_GetMemoryUsed(BAEMixer mixer, unsigned long *pOutResult)
{
	unsigned long	size;

	size = 0;
	if (mixer)
	{
		// mixer size
		size += XGetPtrSize((XPTR)mixer);
		size += sizeof(GM_Mixer);
		if (pOutResult)
		{
			*pOutResult = size;
		}
	}
	return BAE_NO_ERROR;
}


// PV_BAEMixer_AddObject()
// ------------------------------------
//
//
static BAEResult PV_BAEMixer_AddObject(BAEMixer mixer, void *theObject, BAE_OBJECT_TYPE type)
{
	OPErr err;
	BAEObjectListElem *elem;

	err = NO_ERR;

	if (mixer)
	{
		if (theObject)
		{
			elem = (BAEObjectListElem *)XNewPtr(sizeof(BAEObjectListElem));
			if (elem)
			{
				// insert object at start of list
				elem->object = theObject;
				elem->type = type;
				elem->next = mixer->pObjects;
				mixer->pObjects = elem;
			}
			else
			{
				err = MEMORY_ERR;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_BAEMixer_RemoveObject()
// ------------------------------------
//
//
static BAEResult PV_BAEMixer_RemoveObject(BAEMixer mixer, void *theObject, BAE_OBJECT_TYPE type)
{
	OPErr err;
	BAEObjectListElem *elem, **prev;

	err = NO_ERR;

	if (mixer)
	{
		if (theObject)
		{
			elem = mixer->pObjects;
			prev = &(mixer->pObjects);
			while (elem)
			{
				if (elem->object == theObject && elem->type == type)
				{
					*prev = elem->next;
					XDisposePtr(elem);
					break;
				}
				else
				{
					prev = &(elem->next);
					elem = elem->next;
				}
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetMaxDeviceCount()
// ------------------------------------
//
//
BAEResult BAEMixer_GetMaxDeviceCount(BAEMixer mixer, long *outMaxDeviceCount)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outMaxDeviceCount)
		{
			#if USE_DEVICE_ENUM_SUPPORT == TRUE
				*outMaxDeviceCount = GM_MaxDevices();
			#else
				*outMaxDeviceCount = 0;
			#endif
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_SetCurrentDevice()
// ------------------------------------
//
//
BAEResult BAEMixer_SetCurrentDevice(BAEMixer mixer, long deviceID, void *deviceParameter)
{	
	OPErr 		err;
	BAE_BOOL 	isOpen;
	long		deviceCount;
	
	err = NO_ERR;
	if (mixer)
	{
		#if USE_DEVICE_ENUM_SUPPORT == TRUE
			BAEMixer_GetMaxDeviceCount(mixer, &deviceCount);
			if (deviceID < deviceCount)
			{
				BAEMixer_IsOpen(mixer, &isOpen);
				if (isOpen)
				{
					BAEMixer_DisengageAudio(mixer);		// shutdown from hardware
				}
				GM_SetDeviceID(deviceID, deviceParameter);	// change to new device
				BAEMixer_IsOpen(mixer, &isOpen);
				if (isOpen)
				{
					BAEMixer_ReengageAudio(mixer);		// connect back to audio with new device
				}
			}
		#else
			deviceID = deviceID;
			deviceParameter = deviceParameter;
		#endif
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetCurrentDevice()
// ------------------------------------
//
//
BAEResult BAEMixer_GetCurrentDevice(BAEMixer mixer, void *deviceParameter, long *outDeviceID)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outDeviceID)
		{
			*outDeviceID = GM_GetDeviceID(deviceParameter);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetDeviceName()
// ------------------------------------
//
//
BAEResult BAEMixer_GetDeviceName(BAEMixer mixer, long deviceID, char *cName, unsigned long cNameLength)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (cName && cNameLength)
		{
			#if USE_DEVICE_ENUM_SUPPORT == TRUE
				GM_GetDeviceName(deviceID, cName, cNameLength);
			#else
				deviceID = deviceID;
				cName = cName;
				cNameLength = cNameLength;
			#endif
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_IsOpen()
// ------------------------------------
//
//
BAEResult BAEMixer_IsOpen(BAEMixer mixer, BAE_BOOL *outIsOpen)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outIsOpen)
		{
			*outIsOpen = mixer->audioEngaged;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_Is16BitSupported()
// ------------------------------------
//
//
BAEResult BAEMixer_Is16BitSupported(BAEMixer mixer, BAE_BOOL *outIsSupported)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outIsSupported)
		{
			*outIsSupported = (BAE_BOOL)XIs16BitSupported();
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_Is8BitSupported()
// ------------------------------------
//
//
BAEResult BAEMixer_Is8BitSupported(BAEMixer mixer, BAE_BOOL *outIsSupported)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outIsSupported)
		{
			*outIsSupported = (BAE_BOOL)XIs8BitSupported();
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_GetDefaultTerp()
// ------------------------------------
//
//
static TerpMode PV_GetDefaultTerp(BAETerpMode t)
{
	TerpMode	theTerp;

	switch (t)
	{
		#if USE_DROP_SAMPLE == TRUE
		case BAE_DROP_SAMPLE:
			theTerp = E_AMP_SCALED_DROP_SAMPLE;
			break;
		#endif
		#if USE_TERP1 == TRUE
		case BAE_2_POINT_INTERPOLATION:
			theTerp = E_2_POINT_INTERPOLATION;
			break;
		#endif
		default:
		case BAE_LINEAR_INTERPOLATION:
		#if USE_TERP2 == TRUE
			theTerp = E_LINEAR_INTERPOLATION;
		#endif
		#if LOOPS_USED == U3232_LOOPS
			theTerp = E_LINEAR_INTERPOLATION_U3232;
		#elif LOOPS_USED == FLOAT_LOOPS
			theTerp = E_LINEAR_INTERPOLATION_FLOAT;
		#endif
			break;
	}
	return theTerp;
}


// BAEMixer_Open()
// ------------------------------------
//
//
BAEResult BAEMixer_Open(BAEMixer mixer,
						BAEQuality q, 
						BAETerpMode t, 
						BAEAudioModifiers am,
						short int maxSongVoices,
						short int maxSoundVoices,
						short int mixLevel,
						BAE_BOOL engageAudio)
{
	OPErr			theErr;
	Quality			theQuality;
	TerpMode		theTerp;
	AudioModifiers	theMods;

	theErr = NO_ERR;
	if (mixer)
	{
		// if we've never setup the audio engine, do that now
		if (mixer->pMixer == NULL)
		{
#if (X_PLATFORM == X_MACINTOSH) && (CPU_TYPE == k68000)
			// we're running on a MacOS 68k, so we've got to restrict the features in order to get decent playback
			q = BAE_11K;
			am &= ~BAE_USE_STEREO;			// mono only
			am &= ~BAE_STEREO_FILTER;		// don't allow
			am |= BAE_DISABLE_REVERB;		// don't allow
//			am &= ~BAE_USE_16;
			switch (q)
			{
				case BAE_44K:		// no way
				case BAE_48K:
				case BAE_24K:
				case BAE_22K_TERP_44K:
					q = BAE_22K;
					break;
			}
			t = BAE_DROP_SAMPLE;
	
			switch (t)
			{
				case BAE_LINEAR_INTERPOLATION:
					t = BAE_2_POINT_INTERPOLATION;
					break;
			}
#endif
			switch (q)
			{
				case BAE_8K:
					theQuality = Q_8K;
					break;
				case BAE_16K:
					theQuality = Q_16K;
					break;
				case BAE_11K:
					theQuality = Q_11K;
					break;
				case BAE_11K_TERP_22K:
					theQuality = Q_11K_TERP_22K;
					break;
				case BAE_22K_TERP_44K:
					theQuality = Q_22K_TERP_44K;
					break;
				case BAE_22K:
					theQuality = Q_22K;
					break;
				case BAE_24K:
					theQuality = Q_24K;
					break;
				case BAE_32K:
					theQuality = Q_32K;
					break;
				case BAE_40K:
					theQuality = Q_40K;
					break;
				case BAE_44K:
					theQuality = Q_44K;
					break;
				case BAE_48K:
					theQuality = Q_48K;
					break;
				default:
					theErr = PARAM_ERR;
					break;
			}
		
			switch (t)
			{
				case BAE_DROP_SAMPLE:
				case BAE_2_POINT_INTERPOLATION:
				case BAE_LINEAR_INTERPOLATION:
					theTerp = PV_GetDefaultTerp(t);
					break;
				default:
					theErr = PARAM_ERR;
					break;
			}
	
			theMods = M_NONE;
			if ((am & BAE_USE_16) && XIs16BitSupported())
			{
				theMods |= M_USE_16;
			}
			else
			{
				am &= BAE_USE_16;			// 8 bit
			}
	
			if ( (am & BAE_USE_STEREO) && XIsStereoSupported())
			{
				theMods |= M_USE_STEREO;
				if (am & BAE_STEREO_FILTER)
				{
					theMods |= M_STEREO_FILTER;
				}
			}
			else
			{
				am &= ~BAE_USE_STEREO;			// mono
			}
			if (am & BAE_DISABLE_REVERB)
			{
				theMods |= M_DISABLE_REVERB;
			}

			if (maxSongVoices < 0)
			{
				theErr = PARAM_ERR;
			}

			if (maxSoundVoices < 0)
			{
				theErr = PARAM_ERR;
			}

			if (mixLevel <= 0)
			{
				theErr = PARAM_ERR;
			}

#if X_PLATFORM == X_MACINTOSH
// make sure we have at least 1MB of free memory
			if (FreeMem() < (1024L * 1024L))
			{
				theErr = MEMORY_ERR;
			}
#endif

			// check to see if the version numbers match the header files and
			// the built codebase
			{
				short int major, minor, subminor;
	
				BAEMixer_GetMixerVersion(mixer, &major, &minor, &subminor);
				if ((major != BAE_VERSION_MAJOR) || (minor != BAE_VERSION_MINOR) || 
					(subminor != BAE_VERSION_SUB_MINOR))
				{
					theErr = GENERAL_BAD;
				}
			}

			if (theErr == NO_ERR)
			{
				theErr = GM_InitGeneralSound(NULL, theQuality, theTerp, theMods,
												maxSongVoices,
												mixLevel,
												maxSoundVoices,
												&mixer->pMixer);
				if (theErr == NO_ERR)
				{
					if (engageAudio)
					{
						theErr = GM_ResumeGeneralSound(NULL);
						if (theErr == NO_ERR)
						{
							mixer->audioEngaged = TRUE;
						}
					}
				}
			}
		}
		else
		{
			theErr = NOT_REENTERANT;		// can't be reentrant
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
}



// BAEMixer_Close()
// ------------------------------------
//
//
BAEResult BAEMixer_Close(BAEMixer mixer)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		// Shut down mixer
		if (mixer->pMixer)
		{
			GM_SetAudioTask(NULL, NULL);

			if (mixer->audioEngaged)
			{
				// Close up sound manager BEFORE releasing memory!
				GM_StopHardwareSoundManager(NULL);
				mixer->audioEngaged = FALSE;
			}
			GM_FinisGeneralSound(NULL, mixer->pMixer);
			mixer->pMixer = NULL;
		}
		else
		{
			if (mixer->audioEngaged)
			{
				// Mixer is NULL, but active? This should not occur!
				BAE_ASSERT(FALSE);
				mixer->audioEngaged = FALSE;
			}
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetMixerVersion()
// ------------------------------------
//
//
BAEResult BAEMixer_GetMixerVersion(BAEMixer mixer, short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (pVersionMajor && pVersionMinor && pVersionSubMinor)
		{
			*pVersionMajor = BAE_VERSION_MAJOR;
			*pVersionMinor = BAE_VERSION_MINOR;
			*pVersionSubMinor = BAE_VERSION_SUB_MINOR;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_BAEMixer_AddBank()
// ------------------------------------
//
//
static BAEResult PV_BAEMixer_AddBank(BAEMixer mixer, XFILE newPatchFile)
{
	OPErr	err;
	XFILE	*newList;

	err = NO_ERR;

	if (mixer)
	{
		newList = (XFILE*)XNewPtr(sizeof(XFILE) * (mixer->numPatchFiles+1));
		if (newList)
		{
			// copy old list, and append new file to end 
			XBlockMove(mixer->pPatchFiles, newList, sizeof(XFILE)*mixer->numPatchFiles);
			newList[mixer->numPatchFiles] = newPatchFile;

			// dispose of old list, and attach new list
			XDisposePtr(mixer->pPatchFiles);
			mixer->pPatchFiles = newList;
			mixer->numPatchFiles++;

			XFileUseThisResourceFile(newPatchFile);			
		}
		else
		{
			err = MEMORY_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_AddBankFromMemory()
// ------------------------------------
//
//
BAEResult BAEMixer_AddBankFromMemory(BAEMixer mixer, void * pAudioFile, unsigned long fileSize, BAEBankToken *outToken)
{
	BAEResult		theErr;
	XFILE			newPatchFile;

	theErr = BAE_NO_ERROR;
	if (mixer)
	{
		newPatchFile = XFileOpenResourceFromMemory(pAudioFile, fileSize, FALSE);
		if (newPatchFile)
		{
			theErr = PV_BAEMixer_AddBank(mixer, newPatchFile);
			if (outToken)
			{
				*outToken = (BAEBankToken)newPatchFile;
			}
		}
		else
		{
			theErr = BAE_BAD_FILE;
		}
	}
	else
	{
		theErr = BAE_NULL_OBJECT;
	}
	return theErr;
}


// BAEMixer_AddBankFromFile()
// ------------------------------------
//
//
BAEResult BAEMixer_AddBankFromFile(BAEMixer mixer, BAEPathName pAudioPathName, BAEBankToken *outToken)
{
	BAEResult		theErr;
	XFILE			newPatchFile;
	XFILENAME		theFile;

	theErr = BAE_NO_ERROR;
	if (mixer)
	{
		XConvertPathToXFILENAME(pAudioPathName, &theFile);
		newPatchFile = XFileOpenResource(&theFile, TRUE);
		if (newPatchFile)
		{
			theErr = PV_BAEMixer_AddBank(mixer, newPatchFile);
			if (outToken)
			{
				*outToken = (BAEBankToken)newPatchFile;
			}
		}
		else
		{
			theErr = BAE_BAD_FILE;
		}
	}
	else
	{
		theErr = BAE_NULL_OBJECT;
	}
	return theErr;
}


// BAEMixer_UnloadBank()
// ------------------------------------
//
//
BAEResult BAEMixer_UnloadBank(BAEMixer mixer, BAEBankToken token)
{
	XFILE *pPatchFiles;
	XFILE patchFile;
	OPErr err;
	BAE_BOOL ok;
	int i,j;

	err = NO_ERR;

	if (mixer)
	{
		pPatchFiles = mixer->pPatchFiles;
		patchFile = (XFILE) token;

		for (i=0; i<mixer->numPatchFiles; i++)
		{
			if (patchFile == pPatchFiles[i])
			{
				ok = TRUE; // found it!
				XFileClose(patchFile);

				// compact the array.  
				// This will leave a unused slot on the end, but that's ok.
				for (j=i+1; j<mixer->numPatchFiles; j++)
				{
					pPatchFiles[j-1] = pPatchFiles[j];
				}
				mixer->numPatchFiles--;

				// was that the last one? kill the array.  
				// Don't really need to do this, but logically cleaner.
				if (mixer->numPatchFiles == 0)
				{
					XDisposePtr(mixer->pPatchFiles);
					mixer->pPatchFiles = NULL;
				}
			}
		}

		if (!ok)
		{
			err = RESOURCE_NOT_FOUND;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}



BAEResult BAEMixer_UnloadBanks(BAEMixer mixer)
{
	BAEResult err;

	if (mixer)
	{
		err = BAE_NO_ERROR;
		// Close patch files
		while (mixer->numPatchFiles)
		{
			err = BAEMixer_UnloadBank(mixer, (BAEBankToken)mixer->pPatchFiles[mixer->numPatchFiles-1]);
			if (err) break;
		}
	}
	else
	{
		err = BAE_NULL_OBJECT;
	}
	return err;
}


// BAEMixer_BringBankToFront()
// ------------------------------------
//
//
BAEResult BAEMixer_BringBankToFront(BAEMixer mixer, BAEBankToken token)
{
	int i, j;
	XFILE *pPatchFiles;
	short numPatchFiles;
	XFILE file;
	BAE_BOOL ok;
	OPErr err;

	err = NO_ERR;
	ok = FALSE;

	if (mixer)
	{
		pPatchFiles = mixer->pPatchFiles;
		numPatchFiles = mixer->numPatchFiles;
		file = (XFILE)token;

		for (i=0; i<numPatchFiles; i++)
		{
			if (file == pPatchFiles[i])
			{
				ok = TRUE; // found it!
				
				// move higher layers down one, and move the token layer to the end.
				for (j=i+1; j<numPatchFiles; j++)
				{
					pPatchFiles[j-1] = pPatchFiles[j];
				}
				pPatchFiles[numPatchFiles - 1] = file;
				XFileUseThisResourceFile(file);
				break;
			}
		}

		if (!ok)
		{
			err = RESOURCE_NOT_FOUND;
		}

	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_SendBankToBack()
// ------------------------------------
//
//
BAEResult BAEMixer_SendBankToBack(BAEMixer mixer, BAEBankToken token)
{
	int i, j;
	XFILE *pPatchFiles;
	short numPatchFiles;
	XFILE file;
	BAE_BOOL ok;
	OPErr err;

	err = NO_ERR;
	ok = FALSE;
	
	if (mixer)
	{
		pPatchFiles = mixer->pPatchFiles;
		numPatchFiles = mixer->numPatchFiles;
		file = (XFILE)token;

		// find the patch file in the array, and reorder.
		for (i=0; i<numPatchFiles; i++)
		{
			if (file == pPatchFiles[i])
			{
				ok = TRUE; // found it!

				// move lower layers up one, and move the token layer to the start.
				for (j=i; j>0; j--)
				{
					pPatchFiles[j] = pPatchFiles[j-1];
				}
				pPatchFiles[0] = file;
				break;
			}
		}

		if (ok)
		{	
			PV_BAEMixer_SubmitBankOrder(mixer);
		}
		else
		{
			err = RESOURCE_NOT_FOUND;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_BAEMixer_SubmitBankOrder()
// ------------------------------------
//
//
static void PV_BAEMixer_SubmitBankOrder(BAEMixer mixer)
{
	int i;

	for (i=0; i<mixer->numPatchFiles; i++)
	{
		XFileUseThisResourceFile(mixer->pPatchFiles[i]);
	}
}


// BAEMixer_GetBankVersion()
// ------------------------------------
// was BAEMixer_GetVersionFromAudioFile()
//
BAEResult BAEMixer_GetBankVersion(BAEMixer mixer, BAEBankToken token, short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor)
{
	OPErr 		err;
	XVersion	vers;
	int			i;
	XFILE		file;
	BAE_BOOL	foundBank;

	err = NO_ERR;
	
	if (mixer)
	{
		file = (XFILE)token;
		foundBank = FALSE;

		for (i=0; i<mixer->numPatchFiles; i++)
		{
			if (mixer->pPatchFiles[i] == file)
			{
				foundBank = TRUE;
				XFileUseThisResourceFile(file);

				if (pVersionMajor && pVersionMinor && pVersionSubMinor)
				{
					*pVersionMajor = 0;
					*pVersionMinor = 0;
					*pVersionSubMinor = 0;
					if (mixer->pMixer)
					{
						XGetVersionNumber(&vers);
						*pVersionMajor = vers.versionMajor;
						*pVersionMinor = vers.versionMinor;
						*pVersionSubMinor = vers.versionSubMinor;
					}
					else
					{
						err = NOT_SETUP;
					}
				}
				else
				{
					err = PARAM_ERR;
				}
				PV_BAEMixer_SubmitBankOrder(mixer); // restore the bank order;
				break;
			}
		}

		if (!foundBank)
		{
			err = RESOURCE_NOT_FOUND;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetGroovoidNameFromBank()
// -------------------------------------------
// was GetSongNameFromAudioFile()
//
BAEResult BAEMixer_GetGroovoidNameFromBank(BAEMixer mixer, long index, char *cSongName)
{
	XPTR		pData;
	OPErr 		err;
	XLongResourceID id;
	
	err = NO_ERR;
	if (mixer)
	{
		if (cSongName)
		{
			if (mixer->pMixer)
			{
				cSongName[0] = 0;
				pData = NULL;
				pData = XGetIndexedResource(ID_SONG, &id, index, cSongName, NULL);
				if (pData)
				{
					XPtoCstr(cSongName);
					XDisposePtr(pData);
				}
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_ChangeAudioModes()
// ------------------------------------
//
//
BAEResult BAEMixer_ChangeAudioModes(BAEMixer mixer, BAEQuality q, BAETerpMode t, BAEAudioModifiers am)
{
	OPErr			err;
	Quality			theQuality;
	TerpMode		theTerp;
	AudioModifiers	theMods;

	err = NO_ERR;
	if (mixer)
	{
		switch (q)
		{
			case BAE_8K:
				theQuality = Q_8K;
				break;
			case BAE_11K_TERP_22K:
				theQuality = Q_11K_TERP_22K;
				break;
			case BAE_22K_TERP_44K:
				theQuality = Q_22K_TERP_44K;
				break;
			case BAE_11K:
				theQuality = Q_11K;
				break;
			case BAE_16K:
				theQuality = Q_16K;
				break;
			case BAE_22K:
				theQuality = Q_22K;
				break;
			case BAE_24K:
				theQuality = Q_24K;
				break;
			case BAE_32K:
				theQuality = Q_32K;
				break;
			case BAE_40K:
				theQuality = Q_40K;
				break;
			case BAE_44K:
				theQuality = Q_44K;
				break;
			case BAE_48K:
				theQuality = Q_48K;
				break;
			default:
				err = PARAM_ERR;
				break;
		}
		
		switch (t)
		{
			case BAE_DROP_SAMPLE:
			case BAE_2_POINT_INTERPOLATION:
			case BAE_LINEAR_INTERPOLATION:
				theTerp = PV_GetDefaultTerp(t);
				break;
			default:
				err = PARAM_ERR;
				break;
		}

		theMods = M_NONE;
		if ((am & BAE_USE_16) && XIs16BitSupported())
		{
			theMods |= M_USE_16;
		}
		else
		{
			am &= ~BAE_USE_16;	// 8 bit
		}
		if ( (am & BAE_USE_STEREO) && XIsStereoSupported())
		{
			theMods |= M_USE_STEREO;
			if (am & BAE_STEREO_FILTER)
			{
				theMods |= M_STEREO_FILTER;
			}
		}
		else
		{
			am &= ~BAE_USE_STEREO;	// mono
		}
		if (am & BAE_DISABLE_REVERB)
		{
			theMods |= M_DISABLE_REVERB;
		}
		if (err == NO_ERR)
		{
			err = GM_ChangeAudioModes(NULL, theQuality, theTerp, theMods);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_ChangeSystemVoices()
// ------------------------------------
//
//
BAEResult BAEMixer_ChangeSystemVoices(BAEMixer mixer, short int maxSongVoices, short int maxSoundVoices, short int mixLevel)
{
	OPErr	err;

	err = NO_ERR;
	if (mixer)
	{
		err = GM_ChangeSystemVoices(maxSongVoices, mixLevel, maxSoundVoices);
	}
	else
	{
		err = NULL_OBJECT;	
	}	
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetTick()
// ------------------------------------
//
//
BAEResult BAEMixer_GetTick(BAEMixer mixer, unsigned long *outTick)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		*outTick = GM_GetSyncTimeStamp();
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_SetAudioLatency()
// ------------------------------------
//
//
BAEResult BAEMixer_SetAudioLatency(BAEMixer mixer, unsigned long requestedLatency)
{
	BAEResult error;
	
	error = BAE_NO_ERROR;
	if (mixer)
	{
		#if (X_PLATFORM == X_WIN95) && BAE_COMPLETE	
		{
			BAEWinOSParameters	parms;
			long				device;
		
			error = BAEMixer_GetCurrentDevice(mixer, (void *)&parms, &device);
			if (error == BAE_NO_ERROR)	// get current
			{
				parms.synthFramesPerBlock = (requestedLatency / BAE_GetSliceTimeInMicroseconds()) + 1;
				error = BAEMixer_SetCurrentDevice(mixer, device, (void *)&parms);	// set modified
			}
		}
		#else
		{
			mixer = mixer;
			requestedLatency = requestedLatency;
			error = BAE_NOT_SETUP;
		}
		#endif
	}
	else
	{
		error = BAE_NULL_OBJECT;
	}
	return error;
}


// BAEMixer_GetAudioLatency()
// ------------------------------------
//
//
BAEResult BAEMixer_GetAudioLatency(BAEMixer mixer, unsigned long *outLatency)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outLatency)
		{
			*outLatency = GM_GetSyncTimeStampQuantizedAhead() - GM_GetSyncTimeStamp();
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_SetMasterVolume()
// ------------------------------------
//
//
BAEResult BAEMixer_SetMasterVolume(BAEMixer mixer, BAE_UNSIGNED_FIXED theVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		GM_SetMasterVolume((INT32)(UNSIGNED_FIXED_TO_LONG_ROUNDED(theVolume * MAX_MASTER_VOLUME)));
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetMasterVolume()
// ------------------------------------
//
//
BAEResult BAEMixer_GetMasterVolume(BAEMixer mixer, BAE_UNSIGNED_FIXED *outVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outVolume)
		{
			*outVolume = UNSIGNED_RATIO_TO_FIXED(GM_GetMasterVolume(), MAX_MASTER_VOLUME);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_SetHardwareVolume()
// ------------------------------------
//
//
BAEResult BAEMixer_SetHardwareVolume(BAEMixer mixer, BAE_UNSIGNED_FIXED theVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		XSetHardwareVolume(FIXED_TO_SHORT_ROUNDED(theVolume * X_FULL_VOLUME));
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetHardwareVolume()
// ------------------------------------
//
//
BAEResult BAEMixer_GetHardwareVolume(BAEMixer mixer, BAE_UNSIGNED_FIXED *outVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outVolume)
		{
			*outVolume = UNSIGNED_RATIO_TO_FIXED(XGetHardwareVolume(), X_FULL_VOLUME);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_SetMasterSoundEffectsVolume()
// -----------------------------------------
//
//
BAEResult BAEMixer_SetMasterSoundEffectsVolume(BAEMixer mixer, BAE_UNSIGNED_FIXED theVolume)
{
	OPErr err;
	short newVolume;

	err = NO_ERR;
	if (mixer)
	{
		newVolume = FIXED_TO_SHORT_ROUNDED(theVolume * MAX_MASTER_VOLUME);
		if ((newVolume < 0) || (newVolume > MAX_MASTER_VOLUME * 5))
		{
			err = PARAM_ERR;
		}
		else
		{
			GM_SetEffectsVolume(newVolume);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetMasterSoundEffectsVolume()
// -----------------------------------------
//
//
BAEResult BAEMixer_GetMasterSoundEffectsVolume(BAEMixer mixer, BAE_UNSIGNED_FIXED *outVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outVolume)
		{
			*outVolume = UNSIGNED_RATIO_TO_FIXED(GM_GetEffectsVolume(), MAX_MASTER_VOLUME);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetAudioSampleFrame()
// ------------------------------------
//
//
BAEResult BAEMixer_GetAudioSampleFrame(BAEMixer mixer, short int *pLeft, short int *pRight, short int *outFrame)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outFrame)
		{
			*outFrame = GM_GetAudioSampleFrame(pLeft, pRight);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetRealtimeStatus()
// ------------------------------------
//
//
BAEResult BAEMixer_GetRealtimeStatus(BAEMixer mixer, BAEAudioInfo *pStatus)
{
	GM_AudioInfo	status;
	short int		count;
	BAEVoiceType	voiceType;
	OPErr 			err;

	err = NO_ERR;
	if (mixer)
	{
		if (pStatus)
		{
			GM_GetRealtimeAudioInformation(&status);
			XSetMemory(pStatus, (long)sizeof(BAEAudioInfo), 0);
			pStatus->voicesActive = status.voicesActive;
			for (count = 0; count < status.voicesActive; count++)
			{
				pStatus->voice[count] = status.voice[count];
	
				voiceType = BAE_UNKNOWN;
				switch (status.voiceType[count])
				{
					case MIDI_PCM_VOICE:
						voiceType = BAE_MIDI_PCM_VOICE;
						break;
					case SOUND_PCM_VOICE:
						voiceType = BAE_SOUND_PCM_VOICE;
						break;
				}
				pStatus->voiceType[count] = voiceType;
				pStatus->instrument[count] = status.patch[count];
				pStatus->scaledVolume[count] = status.scaledVolume[count];
				pStatus->midiVolume[count] = status.volume[count];
				pStatus->channel[count] = status.channel[count];
				pStatus->midiNote[count] = status.midiNote[count];
				if (status.pSong[count])
				{
#if BAE_MCU
					pStatus->userReference[count] = NULL;	
					// can't access song structure directly.
					// should probably fix this in future for completeness.
#endif
#if BAE_COMPLETE
					pStatus->userReference[count] = status.pSong[count]->userReference;
#endif
				}
			}
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_IsAudioEngaged()
// ------------------------------------
//
//
BAEResult BAEMixer_IsAudioEngaged(BAEMixer mixer, BAE_BOOL *outIsEngaged)
{
	OPErr		err;
	BAE_BOOL	isPaused;

	err = NO_ERR;
	if (mixer)
	{
		if (outIsEngaged)
		{
			if (mixer->pMixer)
			{
				err = GM_IsGeneralSoundPaused(&isPaused);
				*outIsEngaged = !isPaused;
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_DisengageAudio()
// ------------------------------------
//
//
BAEResult BAEMixer_DisengageAudio(BAEMixer mixer)
{
	OPErr	err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (mixer->pMixer)
		{
			err = GM_PauseGeneralSound(NULL);
			if (err == NO_ERR)
			{
				mixer->audioEngaged = FALSE;
			}
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_ReengageAudio()
// ------------------------------------
//
//
BAEResult BAEMixer_ReengageAudio(BAEMixer mixer)
{
	OPErr	err;
	
	err = NO_ERR;	
	if (mixer)
	{
		if (mixer->pMixer)
		{
			err = GM_ResumeGeneralSound(NULL);
			if (err == NO_ERR)
			{
				mixer->audioEngaged = TRUE;
			}
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}

// BAEMixer_Idle()
// ------------------------------------
// Called during idle times to process audio, or other events. Optional
// requiment if threads are available.
//
BAEResult BAEMixer_Idle(BAEMixer mixer)
{
	mixer;
	// BAE_Idle((void *)mixer);
	return BAE_NO_ERROR;
}


// BAEMixer_IsAudioActive()
// ------------------------------------
// This will check active voices and look at a sub sample of the audio output to
// determine if there's any audio still playing
BAEResult BAEMixer_IsAudioActive(BAEMixer mixer, BAE_BOOL *outIsActive)
{
	OPErr	err;

	err = NO_ERR;
	if (mixer)
	{
		if (outIsActive)
		{
			BAEMixer_IsAudioEngaged(mixer, outIsActive);
			if (*outIsActive == TRUE)
			{
				*outIsActive = (BAE_BOOL)GM_IsAudioActive();
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetCPULoadInMicroseconds()
// --------------------------------------
//
//
BAEResult BAEMixer_GetCPULoadInMicroseconds(BAEMixer mixer, unsigned long *outLoad)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outLoad)
		{
			*outLoad = GM_GetMixerUsedTime();
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetCPULoadInPercent()
// --------------------------------------
//
//
BAEResult BAEMixer_GetCPULoadInPercent(BAEMixer mixer, unsigned long *outLoad)
{
	OPErr err;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outLoad)
		{
			*outLoad = GM_GetMixerUsedTimeInPercent();
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetModifiers()
// --------------------------------------
//
//
BAEResult BAEMixer_GetModifiers(BAEMixer mixer, BAEAudioModifiers *outMods)
{
	BAEAudioModifiers theMods;
	OPErr err;
	BAE_BOOL generate16output;
	BAE_BOOL generateStereoOutput;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outMods)
		{
			if (mixer->pMixer)
			{
				theMods = 0;
				err = GM_Generate16bitOutP(&generate16output);
				err = GM_GenerateStereoOutP(&generateStereoOutput);
				if (generate16output) theMods |= BAE_USE_16;
				if (generateStereoOutput) theMods |= BAE_USE_STEREO;
				*outMods = theMods;
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetTerpMode()
// --------------------------------------
//
//
BAEResult BAEMixer_GetTerpMode(BAEMixer mixer, BAETerpMode *outTerpMode)
{
	OPErr err;
	TerpMode t;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outTerpMode)
		{
			if (mixer->pMixer)
			{
				err = GM_GetInterpolationMode(&t);
				if (err == NO_ERR)
				{
					*outTerpMode = PV_TranslateTerpModeToBAETerpMode(t);
				}
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}	


// BAEMixer_GetQuality()
// --------------------------------------
//
//
BAEResult BAEMixer_GetQuality(BAEMixer mixer, BAEQuality *outQuality)
{
	OPErr	err;
	Quality	q;

	err = NO_ERR;
	if (mixer)
	{
		if (outQuality)
		{
			if (mixer->pMixer)
			{
				err = GM_GetQuality(&q);
				if (!err)
				{
					*outQuality = BAE_TranslateQuality(q);
				}
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetMidiVoices()
// --------------------------------------
//
//
BAEResult BAEMixer_GetMidiVoices(BAEMixer mixer, short int *outNumMidiVoices)
{
	OPErr err;
	INT16 song, mix, sound;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outNumMidiVoices)
		{
			if (mixer->pMixer)
			{
				GM_GetSystemVoices(&song, &mix, &sound);
				*outNumMidiVoices = song;
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetSoundVoices()
// --------------------------------------
//
//
BAEResult BAEMixer_GetSoundVoices(BAEMixer mixer, short int *outNumSoundVoices)
{
	OPErr err;
	INT16 song, mix, sound;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outNumSoundVoices)
		{
			if (mixer->pMixer)
			{
				GM_GetSystemVoices(&song, &mix, &sound);
				*outNumSoundVoices = sound;
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAEMixer_GetMixLevel()
// --------------------------------------
//
//
BAEResult BAEMixer_GetMixLevel(BAEMixer mixer, short int *outMixLevel)
{
	OPErr err;
	INT16 song, mix, sound;
	
	err = NO_ERR;
	if (mixer)
	{
		if (outMixLevel)
		{
			if (mixer->pMixer)
			{
				GM_GetSystemVoices(&song, &mix, &sound);
				*outMixLevel = mix;
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// ------------------------------------------------------------------
// BAESound Functions
// ------------------------------------------------------------------
#if 0
	#pragma mark -
	#pragma mark ##### BAESound #####
	#pragma mark -
#endif


// BAESound_New()
// --------------------------------------
//
//
BAESound BAESound_New(BAEMixer mixer)
{
	BAESound sound;
	sound = NULL;
	
	if (mixer)
	{
		sound = (BAESound)XNewPtr(sizeof(struct sBAESound));
		if (sound)
		{
			if (BAESound_SetMixer(sound, mixer) != BAE_NO_ERROR)
			{
				XDisposePtr(sound);
				sound = NULL;
			}
			PV_BAEMixer_AddObject(mixer, sound, BAE_SOUND_OBJECT);
		}
	}
	return sound;
}


// BAESound_Delete()
// --------------------------------------
//
//
BAEResult BAESound_Delete(BAESound sound)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		PV_BAEMixer_RemoveObject(sound->mixer, sound, BAE_SOUND_OBJECT);
		BAESound_Unload(sound);
		XDisposePtr(sound);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}

// BAESound_GetMemoryUsed()
// --------------------------------------
//
//
BAEResult BAESound_GetMemoryUsed(BAESound sound, unsigned long *pOutResult)
{
	unsigned long	size;

	size = 0;
	if (sound)
	{ 
		// song size
		size = XGetPtrSize((XPTR)sound);
		size += sound->pWave->waveSize;;
	}
	if (pOutResult)
	{
		*pOutResult = size;
	}
	return BAE_NO_ERROR;
}


// BAESound_SetMixer()
// --------------------------------------
//
//
BAEResult BAESound_SetMixer(BAESound sound, BAEMixer mixer)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound && mixer)
	{
		sound->mixer = mixer;
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetMixer()
// --------------------------------------
//
//
BAEResult BAESound_GetMixer(BAESound sound, BAEMixer *outMixer)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outMixer)
		{
			*outMixer = sound->mixer;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_Unload()
// --------------------------------------
//
//
BAEResult BAESound_Unload(BAESound sound)
{
	OPErr err;

	err = NO_ERR;
	if (sound)
	{
		if (sound->pWave)
		{
			BAESound_Stop(sound, FALSE);
			GM_FreeWaveform(sound->pWave);
			sound->pWave = NULL;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_LoadCustomSample()
// --------------------------------------
//
//
BAEResult BAESound_LoadCustomSample(BAESound sound,
							void * sampleData,				// pointer to audio data
							unsigned long frames, 			// number of frames of audio
							unsigned short int bitSize, 	// bits per sample 8 or 16
							unsigned short int channels, 	// mono or stereo 1 or 2
							BAE_UNSIGNED_FIXED rate, 		// 16.16 fixed sample rate
							unsigned long loopStart, 		// loop start in frames
							unsigned long loopEnd)			// loop end in frames
{
#if USE_HIGHLEVEL_FILE_API
	OPErr			theErr;

	theErr = NO_ERR;
	if (sound)
	{
		// if sound already loaded, then free it...
		BAESound_Unload(sound);

		// load new sound
		sound->pWave = GM_ReadRawAudioIntoMemoryFromMemory(sampleData, frames, 
															bitSize, channels, 
															 rate, loopStart, 
															 loopEnd, &theErr);
		if ( (sound->pWave == NULL) && (theErr == NO_ERR) )
		{
			theErr = BAD_FILE;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
#else
	return BAE_NOT_SETUP;
#endif
}


// BAESound_LoadMemorySample()
// --------------------------------------
//
//
BAEResult BAESound_LoadMemorySample(BAESound sound, void *pMemoryFile, unsigned long memoryFileSize, BAEFileType fileType)
{
#if USE_HIGHLEVEL_FILE_API
	OPErr			theErr;
	AudioFileType	type;

	theErr = NO_ERR;
	if (sound)
	{
		type = BAE_TranslateBAEFileType(fileType);
		if (type != FILE_INVALID_TYPE)
		{
			// if sound already loaded, then free it...
			BAESound_Unload(sound);

			// load new sound
			sound->pWave = GM_ReadFileIntoMemoryFromMemory(pMemoryFile, memoryFileSize,
																type, TRUE, &theErr);
			if ( (sound->pWave == NULL) && (theErr == NO_ERR) )
			{
				theErr = BAD_FILE;
			}
		}
		else
		{
			theErr = BAD_FILE_TYPE;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
#else
	return BAE_NOT_SETUP;
#endif
}


// BAESound_LoadMemorySample()
// --------------------------------------
//
//
BAEResult BAESound_LoadFileSample(BAESound sound, BAEPathName filePath, BAEFileType fileType)
{
#if USE_HIGHLEVEL_FILE_API
	XFILENAME		theFile;
	OPErr			theErr;
	AudioFileType	type;

	theErr = NO_ERR;
	if (sound)
	{
		type = BAE_TranslateBAEFileType(fileType);
		if (type != FILE_INVALID_TYPE)
		{
			// if sound already loaded, then free it...
			BAESound_Unload(sound);

			// load new sound...
			XConvertPathToXFILENAME(filePath, &theFile);
			sound->pWave = GM_ReadFileIntoMemory(&theFile, type, TRUE, &theErr);

			if ( (sound->pWave == NULL) && (theErr == NO_ERR) )
			{
				theErr = BAD_FILE;
			}
		}
		else
		{
			theErr = BAD_FILE_TYPE;
		}

		if ((sound->pWave == NULL) && (theErr == NO_ERR))
		{
			theErr = BAD_FILE;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
#else
	fileType;
	filePath;
	sound;
	return BAE_NOT_SETUP;
#endif
}


// BAESound_IsPaused()
// --------------------------------------
//
//
BAEResult BAESound_IsPaused(BAESound sound, BAE_BOOL *outIsPaused)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outIsPaused)
		{
			*outIsPaused = (sound->mPauseVariable) ? (BAE_BOOL)TRUE : (BAE_BOOL)FALSE;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_Pause()
// --------------------------------------
//
//
BAEResult BAESound_Pause(BAESound sound)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->mPauseVariable == 0)
		{
			BAESound_GetRate(sound, &sound->mPauseVariable);
			BAESound_SetRate(sound, 0L);	// pause samples in their tracks
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_Resume()
// --------------------------------------
//
//
BAEResult BAESound_Resume(BAESound sound)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->mPauseVariable)
		{
			BAESound_SetRate(sound, sound->mPauseVariable);
			sound->mPauseVariable = 0;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_Fade()
// --------------------------------------
//
//
BAEResult BAESound_Fade(BAESound sound, BAE_FIXED sourceVolume, BAE_FIXED destVolume, BAE_FIXED timeInMiliseconds)
{
	short int	source, dest;
	double		delta;
	INT16		minVolume;
	INT16		maxVolume;
	OPErr 		err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->voiceRef != DEAD_VOICE)
		{
			delta = PV_CalculateTimeDeltaForFade(sourceVolume, destVolume, timeInMiliseconds);
			delta = delta * -MAX_NOTE_VOLUME;
	
			source = FIXED_TO_SHORT_ROUNDED(sourceVolume * MAX_NOTE_VOLUME);
			dest = FIXED_TO_SHORT_ROUNDED(destVolume * MAX_NOTE_VOLUME);
			minVolume = XMIN(source, dest);
			maxVolume = XMAX(source, dest);
			GM_SetSampleFadeRate(sound->voiceRef, FLOAT_TO_FIXED(delta), minVolume, maxVolume, FALSE);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_BAESound_PreStart()
// --------------------------------------
//
//
BAEResult PV_BAESound_PreStart(BAESound sound,
						BAE_UNSIGNED_FIXED sampleVolume, 			// sample volume
						BAE_BOOL stopIfPlaying)
{
	OPErr		err;
	long		volume;
	BAE_BOOL	isDone;
	XDWORD		numFrames;

	err = NO_ERR;
	if (sound)
	{
		if (sound->pWave)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				BAESound_IsDone(sound, &isDone);
				if (!isDone)		// sample playing?
				{
					if (stopIfPlaying)	// can we stop it
					{
						BAESound_Stop(sound, FALSE);
					}
					else
					{
						err = STILL_PLAYING;
					}
				}
			}
			if (err == NO_ERR)
			{
				sound->voiceRef = DEAD_VOICE;
				volume = UNSIGNED_FIXED_TO_LONG_ROUNDED(sampleVolume * MAX_NOTE_VOLUME);
				if (volume)
				{
#if (LOOPS_USED == LIMITED_LOOPS)
					err = GM_GetWaveformNumFrames(sound->pWave, &numFrames);
					if (!err && (numFrames > MAX_SAMPLE_FRAMES))
					{
						err = SAMPLE_TO_LARGE;
					}
#endif
				}
				else
				{
					err = NO_VOLUME;
				}
			}
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_Start()
// --------------------------------------
//
//
BAEResult BAESound_Start(BAESound sound,
							short int priority,
							BAE_UNSIGNED_FIXED sampleVolume,		// sample volume	(1.0)
							unsigned long startOffsetFrame)			// starting offset in frames
{
	OPErr	theErr;
	long	volume;


	priority = priority; // NEED TO IMPLEMENT PRIORITY FOR SOUNDS IN ENGINE.

	if (sound)
	{
		theErr = BAE_TranslateBAErr(
										PV_BAESound_PreStart(sound, sampleVolume, TRUE)
									);
		if (theErr == NO_ERR)
		{
			sound->voiceRef = DEAD_VOICE;
			volume = UNSIGNED_FIXED_TO_LONG_ROUNDED(sampleVolume * MAX_NOTE_VOLUME);
			if (volume)
			{
				sound->voiceRef = GM_SetupSampleFromInfo(sound->pWave, (void *)sound, 
													volume,
													0,
													NULL, 
													NULL,
													startOffsetFrame);
				if (sound->voiceRef == DEAD_VOICE)
				{
					theErr = NO_FREE_VOICES;
				}
				else
				{
					GM_StartSample(sound->voiceRef);
				}
			}
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
}


// BAESound_Stop()
// --------------------------------------
//
//
BAEResult BAESound_Stop(BAESound sound, BAE_BOOL startFade)
{
	OPErr 		err;
	short int	sampleVolume;
	BAE_BOOL	isPaused;

	err = NO_ERR;
	if (sound)
	{
		BAESound_IsPaused(sound, &isPaused);
		if (isPaused)
		{
			BAESound_Resume(sound);
		}
		if (sound->voiceRef != DEAD_VOICE)
		{
			if (startFade)
			{
				sampleVolume = GM_GetSampleVolume(sound->voiceRef);
				GM_SetSampleFadeRate(sound->voiceRef, FLOAT_TO_FIXED(2.2),
														0, sampleVolume, TRUE);
			}
			else
			{
				GM_EndSample(sound->voiceRef);
#if BAE_NOT_USED
				GM_SetSampleOffsetCallbackLinks(sound->voiceRef, NULL);
#endif
			}
		}
		sound->voiceRef = DEAD_VOICE;	// done
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetInfo()
// --------------------------------------
//
//
BAEResult BAESound_GetInfo(BAESound sound, BAESampleInfo *outInfo)
{
	GM_Waveform 	*pWave;
	OPErr			err;

	err = NO_ERR;
	if (sound)
	{
		if (outInfo)
		{
			pWave = sound->pWave;
			if (pWave)
			{
				if (
					(err = GM_GetWaveformByteSize(pWave, &outInfo->waveSize)) != NO_ERR ||
					(err = GM_GetWaveformNumFrames(pWave, &outInfo->waveFrames)) != NO_ERR ||
					(err = GM_GetWaveformBitDepth(pWave, &outInfo->bitSize)) != NO_ERR ||
					(err = GM_GetWaveformNumChannels(pWave, &outInfo->channels)) != NO_ERR ||
					(err = GM_GetWaveformSampleRate(pWave, &outInfo->sampledRate)) != NO_ERR ||
					(err = GM_GetWaveformLoopPoints(pWave, &outInfo->startLoop, &outInfo->startLoop)) != NO_ERR ||
					(err = GM_GetWaveformBaseMidiPitch(pWave, &outInfo->baseMidiPitch)) != NO_ERR
				)
				{
					// if one of the conditions fails (non-zero error code), it will stop 
					// evaluating the rest and 'err' will store the error code.
					// otherwise err = NO_ERR.
				}
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_IsDone()
// --------------------------------------
//
//
BAEResult BAESound_IsDone(BAESound sound, BAE_BOOL *outIsDone)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outIsDone)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				*outIsDone = (BAE_BOOL)GM_IsSoundDone(sound->voiceRef);
				if (*outIsDone)
				{
					sound->voiceRef = DEAD_VOICE;
				}
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_SetVolume()
// --------------------------------------
//
//
BAEResult BAESound_SetVolume(BAESound sound, BAE_UNSIGNED_FIXED newVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->voiceRef != DEAD_VOICE)
		{
			GM_ChangeSampleVolume(sound->voiceRef, FIXED_TO_SHORT_ROUNDED(newVolume * MAX_NOTE_VOLUME));
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetVolume()
// --------------------------------------
//
//
BAEResult BAESound_GetVolume(BAESound sound, BAE_UNSIGNED_FIXED *outVolume)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outVolume)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				*outVolume = UNSIGNED_RATIO_TO_FIXED(GM_GetSampleVolume(sound->voiceRef), MAX_NOTE_VOLUME);
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_SetRate()
// --------------------------------------
//
//
BAEResult BAESound_SetRate(BAESound sound, BAE_UNSIGNED_FIXED newRate)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->voiceRef != DEAD_VOICE)
		{
			GM_ChangeSamplePitch(sound->voiceRef, newRate);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetRate()
// --------------------------------------
//
//
BAEResult BAESound_GetRate(BAESound sound, BAE_UNSIGNED_FIXED *outRate)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outRate)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				*outRate = GM_GetSamplePitch(sound->voiceRef);
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_SetLowPassAmountFilter()
// --------------------------------------
//
//
BAEResult BAESound_SetLowPassAmountFilter(BAESound sound, short int lowPassAmount)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->voiceRef != DEAD_VOICE)
		{
			GM_SetSampleLowPassAmountFilter(sound->voiceRef, lowPassAmount);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetLowPassAmountFilter()
// --------------------------------------
//
//
BAEResult BAESound_GetLowPassAmountFilter(BAESound sound, short int *outLowPassAmount)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outLowPassAmount)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				*outLowPassAmount = GM_GetSampleLowPassAmountFilter(sound->voiceRef);
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_SetResonanceAmountFilter()
// --------------------------------------
//
//
BAEResult BAESound_SetResonanceAmountFilter(BAESound sound, short int resonanceAmount)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->voiceRef != DEAD_VOICE)
		{
			GM_SetSampleResonanceFilter(sound->voiceRef, resonanceAmount);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetResonanceAmountFilter()
// --------------------------------------
//
//
BAEResult BAESound_GetResonanceAmountFilter(BAESound sound, short int *outResonanceAmount)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outResonanceAmount)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				*outResonanceAmount = GM_GetSampleResonanceFilter(sound->voiceRef);
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_SetFrequencyAmountFilter()
// --------------------------------------
//
//
BAEResult BAESound_SetFrequencyAmountFilter(BAESound sound, short int frequencyAmount)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (sound->voiceRef != DEAD_VOICE)
		{
			GM_SetSampleFrequencyFilter(sound->voiceRef, frequencyAmount);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetFrequencyAmountFilter()
// --------------------------------------
//
//
BAEResult BAESound_GetFrequencyAmountFilter(BAESound sound, short int *outFrequencyAmount)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outFrequencyAmount)
		{
			if (sound->voiceRef != DEAD_VOICE)
			{
				*outFrequencyAmount = GM_GetSampleFrequencyFilter(sound->voiceRef);
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_SetSampleLoopPoints()
// --------------------------------------
//
//
BAEResult BAESound_SetSampleLoopPoints(BAESound sound, unsigned long start, unsigned long end)
{
	OPErr	err;

	err = NO_ERR;
	if (sound)
	{
		if (sound->pWave)
		{
			err = GM_SetWaveformLoopPoints(sound->pWave, start, end);
			if (err == NO_ERR)
			{
				if (sound->voiceRef != DEAD_VOICE)
				{
					GM_SetSampleLoopPoints(sound->voiceRef, start, end);
				}
			}
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESound_GetSampleLoopPoints()
// --------------------------------------
//
//
BAEResult BAESound_GetSampleLoopPoints(BAESound sound, unsigned long *outStart, unsigned long *outEnd)
{
	OPErr err;
	
	err = NO_ERR;
	if (sound)
	{
		if (outStart && outEnd)
		{
			if (sound->pWave)
			{
				err = GM_GetWaveformLoopPoints(sound->pWave, outStart, outEnd);
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}







// ------------------------------------------------------------------
// BAESong Functions
// ------------------------------------------------------------------
#if 0
	#pragma mark -
	#pragma mark ##### BAESong #####
	#pragma mark -
#endif


// BAESong_New()
// --------------------------------------
//
//
BAESong BAESong_New(BAEMixer mixer)
{
	BAESong song;
	
	song = NULL;
	if (mixer)
	{
		song = (BAESong)XNewPtr(sizeof(struct sBAESong));
		if (song)
		{
			song->mVolume = MAX_SONG_VOLUME;
			BAESong_SetMixer(song, mixer);
			if (PV_BAESong_InitLiveSong(song) != BAE_NO_ERROR)
			{
				BAESong_Delete(song);
				song = NULL;
			}
			PV_BAEMixer_AddObject(mixer, song, BAE_SONG_OBJECT);
		}
	}
	return song;
}

// BAESong_GetMemoryUsed()
// --------------------------------------
//
//
BAEResult BAESong_GetMemoryUsed(BAESong song, unsigned long *pOutResult)
{
	unsigned long	size;
	short int		count, splitCount;
	GM_Instrument	*pI, *pSI;

	size = 0;
	if (song)
	{
		// song size
		size = XGetPtrSize((XPTR)song);
		size += song->pSong->midiSize;
		size += sizeof(GM_Song);

		// instruments size
		for (count = 0; count < MAX_INSTRUMENTS * MAX_BANKS; count++)
		{
			pI = song->pSong->instrumentData[count];
			if (pI)
			{
				size += XGetPtrSize((XPTR)pI);
				if (pI->doKeymapSplit)
				{
					for (splitCount = 0; splitCount < pI->u.k.KeymapSplitCount; ++splitCount)
					{
						pSI = pI->u.k.keySplits[splitCount].pSplitInstrument;
						size += XGetPtrSize((XPTR)pSI);
					}
				}
				else
				{
					size += pI->u.w.waveSize;
				}
			}
		}
	}
	if (pOutResult)
	{
		*pOutResult = size;
	}
	return BAE_NO_ERROR;
}

// PV_BAESong_InitLiveSong()
// --------------------------------------
//
//
static BAEResult PV_BAESong_InitLiveSong(BAESong song)
{
	OPErr err;
	short int maxSongVoices, maxEffectVoices, mixLevel;

	err = NO_ERR;
	if (song)
	{
		song->pSong = GM_CreateLiveSong(NULL, midiSongCount++);
		if (song->pSong)
		{
			GM_SetSongMixer(song->pSong, song->mixer->pMixer);	// associate mixer to song
																// other we can't load instruments

			BAEMixer_GetMidiVoices(song->mixer, &maxSongVoices);
			BAEMixer_GetSoundVoices(song->mixer, &maxEffectVoices);
			BAEMixer_GetMixLevel(song->mixer, &mixLevel);
			GM_ChangeSongVoices(song->pSong, maxSongVoices, mixLevel, maxEffectVoices);

			//	2000.03.21 AER	Obsolete with the advent of bank tokens
			//GM_SetCacheSamples(song->pSong, TRUE);
			err = GM_StartLiveSong(song->pSong, FALSE);
			if (err)
			{
				while (GM_FreeSong(NULL, song->pSong) == STILL_PLAYING)
				{
					XWaitMicroseocnds(BAE_GetSliceTimeInMicroseconds());
				}
				song->pSong = NULL;
			}
		}
		else
		{
			err = MEMORY_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}

	return BAE_TranslateOPErr(err);
}


// BAESong_Delete()
// --------------------------------------
//
//
BAEResult BAESong_Delete(BAESong song)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		PV_BAEMixer_RemoveObject(song->mixer, song, BAE_SONG_OBJECT);
		PV_BAESong_Unload(song);
		XDisposePtr(song);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetMixer()
// --------------------------------------
//
//
BAEResult BAESong_GetMixer(BAESong song, BAEMixer *outMixer)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (outMixer)
		{
			*outMixer = song->mixer;
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SetMixer()
// --------------------------------------
//
//
BAEResult BAESong_SetMixer(BAESong song, BAEMixer mixer)
{
	OPErr err;

	err = NO_ERR;
	if (song && mixer)
	{
		song->mixer = mixer;
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_BAESong_Unload()
// --------------------------------------
//
//
void PV_BAESong_Unload(BAESong song)
{
	BAE_ASSERT(song);

	BAESong_Stop(song, FALSE);
	//	2000.03.21 AER	Obsolete with advent of bank tokens
	//GM_SetCacheSamples(song->pSong, FALSE);
	GM_KillSongNotes(song->pSong);
	while (GM_FreeSong(NULL, song->pSong) == STILL_PLAYING)
	{
		XWaitMicroseocnds(BAE_GetSliceTimeInMicroseconds());
	}
	song->pSong = NULL;
}


// BAESong_LoadGroovoid()
// --------------------------------------
//
//
BAEResult BAESong_LoadGroovoid(BAESong song, char *cName, BAE_BOOL ignoreBadInstruments) // was LoadFromBank
{
	SongResource		*pXSong;
	long				size;
	OPErr				theErr;
	XShortResourceID	theID;
	GM_Song				*pSong;

	theErr = NO_ERR;
	if (song)
	{
		
#if X_PLATFORM != X_MACINTOSH
		// on all platforms except MacOS we need a valid open resource file. BAE's resource manager is designed
		// to fall back into the MacOS resource manager if no valid BAE file is open. So this test is removed
		// MacOS.
		if (song->mixer && song->mixer->pPatchFiles)
#endif
		{
			pXSong = (SongResource *)XGetNamedResource(ID_SONG, cName, &size);		// look for song
			if (pXSong)
			{
				if (song->pSong)
				{
					PV_BAESong_Unload(song);
					theID = midiSongCount++;	// runtime midi ID
					pSong = GM_LoadSong(song->mixer->pMixer,
										NULL,
										song,
										theID,
										(void *) pXSong,
										NULL,
										0L,
										NULL,		// no callback
										TRUE,		// load instruments
										ignoreBadInstruments,
										(XTOKEN) XFileGetCurrentResourceFile(),
										&theErr);
					if (pSong)
					{
						// things are cool
						GM_SetDisposeSongDataWhenDoneFlag(pSong, TRUE); // dispose of midi data
						GM_SetSongLoopFlag(pSong, FALSE);		// don't loop song
						song->pSong = pSong;				// preserve for use later
						theErr = NO_ERR;
					}
					else
					{
						// need to re-initialize
						PV_BAESong_InitLiveSong(song); 
						theErr = BAD_FILE;
					}
				}
				else
				{
					return BAE_GENERAL_BAD; // a BAESong must always have a pSong...
				}
				XDisposePtr(pXSong);
			}
			else
			{
				theErr = RESOURCE_NOT_FOUND;
			}
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
}


// BAESong_LoadMidiFromMemory()
// --------------------------------------
//
//
BAEResult BAESong_LoadMidiFromMemory(BAESong song, void const* pMidiData, unsigned long midiSize, BAE_BOOL ignoreBadInstruments)
{
	SongResource		*pXSong;
	OPErr				theErr;
	XShortResourceID	theID;
	GM_Song				*pSong;
	short				soundVoices, midiVoices, mixLevel;

	theErr = NO_ERR;
	if (song)
	{
		if (pMidiData && midiSize)
		{
			theID = midiSongCount++;	// runtime midi ID
			BAEMixer_GetMidiVoices(song->mixer, &midiVoices);
			BAEMixer_GetMixLevel(song->mixer, &mixLevel);
			BAEMixer_GetSoundVoices(song->mixer, &soundVoices);
			pXSong = XNewSongPtr(SONG_TYPE_SMS,
								theID,
								midiVoices,
								mixLevel,
								soundVoices,
								REVERB_TYPE_1
								);
			if (pXSong)
			{
				if (song->pSong)
				{
					PV_BAESong_Unload(song);
					pSong = GM_LoadSong(song->mixer->pMixer,
										NULL,
										song,
										theID,
										(void *) pXSong,
										(void *) pMidiData,
										(long) midiSize,
										NULL,		// no callback
										TRUE,		// load instruments
										ignoreBadInstruments,
										(XTOKEN) XFileGetCurrentResourceFile(),
										&theErr);
					if (pSong)
					{
						// things are cool
						GM_SetDisposeSongDataWhenDoneFlag(pSong, FALSE); // don't dispose of midi data
						GM_SetSongLoopFlag(pSong, FALSE);			// don't loop song
						song->pSong = pSong;						// preserve for use later
					}
					else
					{
						// need to re-initialize
						PV_BAESong_InitLiveSong(song); 
						theErr = BAD_FILE;
					}
				}
				else
				{
					theErr = GENERAL_BAD;  // a BAESong must always have a pSong...
				}
				XDisposePtr(pXSong);

			}
			else
			{
				theErr = MEMORY_ERR;
			}
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
}


// BAESong_LoadMidiFromFile()
// --------------------------------------
//
//
BAEResult BAESong_LoadMidiFromFile(BAESong song, BAEPathName filePath, BAE_BOOL ignoreBadInstruments)
{
	XFILENAME			name;
	XPTR				pMidiData;
	SongResource		*pXSong;
	long				midiSize;
	OPErr				theErr;
	XShortResourceID	theID;
	GM_Song				*pSong;
	short				soundVoices, midiVoices, mixLevel;

	theErr = NO_ERR;
	if (song)
	{
		XConvertPathToXFILENAME(filePath, &name);
		pMidiData = PV_GetFileAsData(&name, &midiSize);
		if (pMidiData)
		{
			theID = midiSongCount++;	// runtime midi ID
			BAEMixer_GetMidiVoices(song->mixer, &midiVoices);
			BAEMixer_GetMixLevel(song->mixer, &mixLevel);
			BAEMixer_GetSoundVoices(song->mixer, &soundVoices);
			pXSong = XNewSongPtr(SONG_TYPE_SMS,
								theID,
								midiVoices,
								mixLevel,
								soundVoices,
								BAE_REVERB_TYPE_1
								);
	
			if (pXSong)
			{
				if (song->pSong)
				{
					PV_BAESong_Unload(song);
					pSong = GM_LoadSong(song->mixer->pMixer,
										NULL,
										song,
										theID,
										(void *) pXSong,
										pMidiData,
										midiSize,
										NULL,		// no callback
										TRUE,		// load instruments
										ignoreBadInstruments,
										(XTOKEN) XFileGetCurrentResourceFile(),
										&theErr);
					if (pSong)
					{
						// things are cool
						GM_SetDisposeSongDataWhenDoneFlag(pSong, TRUE); // dispose of midi data
						GM_SetSongLoopFlag(pSong, FALSE);		// don't loop song
						song->pSong = pSong;					// preserve for use later
					}
					else
					{
						// need to re-initialize
						PV_BAESong_InitLiveSong(song); 
						theErr = BAD_FILE;
						XDisposePtr(pMidiData);
					}
				}
				else
				{
					return BAE_GENERAL_BAD;  // a BAESong must always have a pSong...
				}
				XDisposePtr(pXSong);
			}
			else
			{
				XDisposePtr(pMidiData);
				theErr = MEMORY_ERR;
			}
		}
		else
		{
			theErr = BAD_FILE;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
}


// BAESong_LoadRmfFromMemory()
// --------------------------------------
// was BAERmfSong::LoadFromMemory()
//
BAEResult BAESong_LoadRmfFromMemory(BAESong song, void *pRMFData, unsigned long rmfSize, short int songIndex, BAE_BOOL ignoreBadInstruments)
{
#if USE_FULL_RMF_SUPPORT == TRUE
	XFILE				fileRef;
	SongResource		*pXSong;
	GM_Song				*pSong;
	OPErr				theErr;
	XLongResourceID		theID;
	long				size;

	theErr = NO_ERR;
	if (song)
	{
		if (pRMFData && rmfSize)
		{
			fileRef = XFileOpenResourceFromMemory((XPTR)pRMFData, rmfSize, TRUE);
			if (fileRef)
			{
				pXSong = (SongResource *)XGetIndexedFileResource(fileRef, ID_SONG, &theID, songIndex, NULL, &size);
				if (pXSong)
				{
					if (song->pSong)
					{
						PV_BAESong_Unload(song);
						pSong = GM_LoadSong(song->mixer->pMixer,
											NULL,
											song,
											(XShortResourceID) theID,
											(void *) pXSong,
											NULL,
											0L,
											NULL,		// no callback
											TRUE,		// load instruments
											ignoreBadInstruments,
											(XTOKEN) XFileGetCurrentResourceFile(),
											&theErr);
						if (pSong)
						{
							// things are cool
							GM_SetDisposeSongDataWhenDoneFlag(pSong, TRUE); // dispose of midi data
							GM_SetSongLoopFlag(pSong, FALSE);		// don't loop song
							song->pSong = pSong;					// preserve for use later
						}
						else
						{
							// need to re-initialize
							PV_BAESong_InitLiveSong(song); 
							theErr = BAD_FILE;
						}
					}
					else
					{
						theErr = GENERAL_BAD; // a BAESong must always have a pSong...
					}
					XDisposePtr(pXSong);
				}
				else
				{
					theErr = RESOURCE_NOT_FOUND;
				}
				XFileClose(fileRef);
			}
			else
			{
				theErr = BAD_FILE;
			}
		}
		else
		{
			theErr = PARAM_ERR;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);
#else
	return BAE_NOT_SETUP;
#endif	//#if USE_FULL_RMF_SUPPORT == TRUE
}


// BAESong_LoadRmfFromFile()
// --------------------------------------
//
//
BAEResult BAESong_LoadRmfFromFile(BAESong song, BAEPathName filePath, short int songIndex, BAE_BOOL ignoreBadInstruments)
{
#if USE_FULL_RMF_SUPPORT == TRUE
	XFILE				fileRef;
	XFILENAME			name;
	SongResource		*pXSong;
	GM_Song				*pSong;
	OPErr				theErr;
	XLongResourceID		theID;
	long				size;

	theErr = NO_ERR;
	if (song)
	{
		XConvertPathToXFILENAME(filePath, &name);
		fileRef = XFileOpenResource(&name, TRUE);
		if (fileRef)
		{
			pXSong = (SongResource *)XGetIndexedFileResource(fileRef, ID_SONG, &theID, songIndex, NULL, &size);
			if (pXSong)
			{
				if (song->pSong)
				{
					PV_BAESong_Unload(song);
					pSong = GM_LoadSong(song->mixer->pMixer,
										NULL,
										song,
										(XShortResourceID) theID,
										(void *) pXSong,
										NULL,
										0L,
										NULL,		// no callback
										TRUE,		// load instruments
										ignoreBadInstruments,
										(XTOKEN) XFileGetCurrentResourceFile(),
										&theErr);
					if (pSong)
					{
						// things are cool
						GM_SetDisposeSongDataWhenDoneFlag(pSong, TRUE); // dispose of midi data
						GM_SetSongLoopFlag(pSong, FALSE);		// don't loop song
						song->pSong = pSong;					// preserve for use later
					}
					else
					{
						// need to re-initialize
						PV_BAESong_InitLiveSong(song); 
						theErr = BAD_FILE;
					}
				}
				else
				{
					theErr = GENERAL_BAD; // a BAESong should always have a pSong...
				}
				XDisposePtr(pXSong);
			}
			else
			{
				theErr = RESOURCE_NOT_FOUND;
			}
			XFileClose(fileRef);
		}
		else
		{
			theErr = BAD_FILE;
		}
	}
	else
	{
		theErr = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(theErr);	
#else
	return BAE_NOT_SETUP;
#endif	//#if USE_FULL_RMF_SUPPORT == TRUE
}


// BAESong_SetVolume()
// --------------------------------------
//
//
BAEResult BAESong_SetVolume(BAESong song, BAE_UNSIGNED_FIXED volume)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		song->mVolume = FIXED_TO_SHORT_ROUNDED(volume * MAX_SONG_VOLUME);
		GM_SetSongVolume(song->pSong, song->mVolume);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetVolume()
// --------------------------------------
//
//
BAEResult BAESong_GetVolume(BAESong song, BAE_UNSIGNED_FIXED *outVolume)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (outVolume)
		{
			song->mVolume = GM_GetSongVolume(song->pSong);
			*outVolume = UNSIGNED_RATIO_TO_FIXED(song->mVolume, MAX_SONG_VOLUME);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SetTranspose()
// --------------------------------------
//
//
BAEResult BAESong_SetTranspose(BAESong song, long semitones)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		semitones *= -1;
		if ( (semitones > -128) && (semitones < 128) )
		{
			GM_SetSongPitchOffset(song->pSong, semitones);
		}
	}
	else
	{
		err = PARAM_ERR;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetTranspose()
// --------------------------------------
//
//
BAEResult BAESong_GetTranspose(BAESong song, long *outSemitones)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (outSemitones)
		{
			*outSemitones = -GM_GetSongPitchOffset(song->pSong);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_AllowChannelTranspose()
// --------------------------------------
//
//
BAEResult BAESong_AllowChannelTranspose(BAESong song, unsigned short int channel, BAE_BOOL allowTranspose)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_AllowChannelPitchOffset(song->pSong, channel, allowTranspose);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_DoesChannelAllowTranspose()
// --------------------------------------
//
//
BAEResult BAESong_DoesChannelAllowTranspose(BAESong song, unsigned short int channel, BAE_BOOL *outAllowTranspose)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outAllowTranspose)
		{
			*outAllowTranspose = GM_DoesChannelAllowPitchOffset(song->pSong, channel);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_MuteChannel()
// --------------------------------------
//
//
BAEResult BAESong_MuteChannel(BAESong song, unsigned short int channel)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_MuteChannel(song->pSong, channel);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_UnmuteChannel()
// --------------------------------------
//
//
BAEResult BAESong_UnmuteChannel(BAESong song, unsigned short int channel)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_UnmuteChannel(song->pSong, channel);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetChannelMuteStatus()
// --------------------------------------
//
//
BAEResult BAESong_GetChannelMuteStatus(BAESong song, BAE_BOOL *outChannels)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outChannels)
		{
			GM_GetChannelMuteStatus(song->pSong, outChannels);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SoloChannel()
// --------------------------------------
//
//
BAEResult BAESong_SoloChannel(BAESong song, unsigned short int channel)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_SoloChannel(song->pSong, channel);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_UnSoloChannel()
// --------------------------------------
//
//
BAEResult BAESong_UnSoloChannel(BAESong song, unsigned short int channel)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_UnsoloChannel(song->pSong, channel);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetChannelSoloStatus()
// --------------------------------------
//
//
BAEResult BAESong_GetChannelSoloStatus(BAESong song, BAE_BOOL *outChannels)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outChannels)
		{
			GM_GetChannelSoloStatus(song->pSong, outChannels);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_LoadInstrument()
// --------------------------------------
//
//
BAEResult BAESong_LoadInstrument(BAESong song, BAE_INSTRUMENT instrument)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (song->pSong)	// MOVE THIS CHECK INTO ENGINE
		{
			err = GM_LoadSongInstrument(song->pSong,
										(XLongResourceID) instrument,
										(XTOKEN) XFileGetCurrentResourceFile());
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_UnloadInstrument()
// --------------------------------------
//
//
BAEResult BAESong_UnloadInstrument(BAESong song, BAE_INSTRUMENT instrument)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (song->pSong)	// MOVE THIS CHECK INTO ENGINE
		{
			err = GM_UnloadSongInstrument(song->pSong, (XLongResourceID)instrument);
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_IsInstrumentLoaded()
// --------------------------------------
//
//
BAEResult BAESong_IsInstrumentLoaded(BAESong song, BAE_INSTRUMENT instrument, BAE_BOOL *outIsLoaded)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outIsLoaded)
		{
			*outIsLoaded = (BAE_BOOL)GM_IsSongInstrumentLoaded(song->pSong, (XLongResourceID)instrument);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetControlValue()
// --------------------------------------
//
//
BAEResult BAESong_GetControlValue(BAESong song, unsigned char channel, unsigned char controller, char *outValue)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outValue)
		{
			if (song->pSong) // MOVE THIS CHECK INTO THE ENGINE
			{
				*outValue = GM_GetControllerValue(song->pSong, channel, controller);
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetProgramBank()
// --------------------------------------
//
//
BAEResult BAESong_GetProgramBank(BAESong song,
							unsigned char channel,
							unsigned char *outProgram,
							unsigned char *outBank)
{
	OPErr err;
	XSWORD bank, program;

	err = NO_ERR;
	if (song)
	{
		if (outBank && outProgram)
		{
			if (song->pSong)
			{
				GM_GetProgramBank(song->pSong, channel, &program, &bank);
				*outProgram = (unsigned char) program;
				*outBank = (unsigned char) bank;

//				*outProgram = (unsigned char)(song->pSong)->channelProgram[channel];
//				*outBank = (unsigned char)(song->pSong)->channelBank[channel];
			}
			else
			{
				err = NOT_SETUP;
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetPitchBend()
// --------------------------------------
//
//
BAEResult BAESong_GetPitchBend(BAESong song,
							unsigned char channel, 
							unsigned char *outLSB, 
							unsigned char *outMSB)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outLSB && outMSB)
		{
			GM_GetPitchBend(song->pSong, channel, outLSB, outMSB);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_NoteOff()
// --------------------------------------
//
//
BAEResult BAESong_NoteOff(BAESong song, 
							unsigned char channel, 
							unsigned char note, 
							unsigned char velocity,
							unsigned long time)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
		
		QGM_NoteOff(song->pSong, time, channel, note, velocity);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_NoteOnWithLoad()
// --------------------------------------
//
//
BAEResult BAESong_NoteOnWithLoad(BAESong song, 
							unsigned char channel, 
							unsigned char note, 
							unsigned char velocity,
							unsigned long time)
{
	BAE_INSTRUMENT	inst;
	unsigned char	program, bank;
	BAEMixer		mixer;
	OPErr			err;
	BAE_BOOL		isLoaded;
	unsigned long	latency;
	
	err = NO_ERR;
	if (song)
	{
		BAESong_GetMixer(song, &mixer);
		// wait around for at least one slice to let events catch up
		BAEMixer_GetAudioLatency(mixer, &latency);
		XWaitMicroseocnds(latency / 1000);
		XWaitMicroseocnds(latency / 1000);

		// pull the current program, bank from the current state. Should be valid by this time.
		BAESong_GetProgramBank(song, channel, &program, &bank);
		inst = TranslateBankProgramToInstrument(bank, program, channel, note);
		if (BAESong_IsInstrumentLoaded(song, inst, &isLoaded) == BAE_NO_ERROR)
		{
			if (isLoaded == FALSE)
			{
				BAESong_LoadInstrument(song, inst);
			}
		}
		else
		{
			return BAE_GENERAL_BAD;
		}
	
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}

		QGM_NoteOn(song->pSong, time, channel, note, velocity);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_NoteOn()
// --------------------------------------
//
//
BAEResult BAESong_NoteOn(BAESong song, 
					unsigned char channel, 
					unsigned char note, 
					unsigned char velocity,
					unsigned long time)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
	
		QGM_NoteOn(song->pSong, time, channel, note, velocity);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_KeyPressure()
// --------------------------------------
//
//
BAEResult BAESong_KeyPressure(BAESong song, 
						unsigned char channel, 
						unsigned char note, 
						unsigned char pressure,
						unsigned long time)
{
	song = song;
	time = time;
	pressure = pressure;
	channel = channel;
	note = note;
	return BAE_NO_ERROR;
}


// BAESong_ControlChange()
// --------------------------------------
//
//
BAEResult BAESong_ControlChange(BAESong song, 
							unsigned char channel, 
							unsigned char controlNumber,
							unsigned char controlValue, 
							unsigned long time)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
	
		QGM_Controller(song->pSong, time, channel, controlNumber, controlValue);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_ProgramBankChange()
// --------------------------------------
//
//
BAEResult BAESong_ProgramBankChange(BAESong song, 
								unsigned char channel,
								unsigned char programNumber,
								unsigned char bankNumber,
								unsigned long time)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
	
		QGM_Controller(song->pSong, time, channel, 0, bankNumber);
		QGM_ProgramChange(song->pSong, time, channel, programNumber);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_ProgramChange()
// --------------------------------------
//
//
BAEResult BAESong_ProgramChange(BAESong song, 
							unsigned char channel, 
							unsigned char programNumber,
							unsigned long time)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
	
		QGM_ProgramChange(song->pSong, time, channel, programNumber);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_ChannelPressure()
// --------------------------------------
//
//
BAEResult BAESong_ChannelPressure(BAESong song, 
								unsigned char channel, 	
								unsigned char pressure, 
								unsigned long time)
{
	song = song;
	time = time;
	channel = channel;
	pressure = pressure;
	return BAE_NO_ERROR;
}


// BAESong_PitchBend()
// --------------------------------------
//
//
BAEResult BAESong_PitchBend(BAESong song, 
						unsigned char channel, 
						unsigned char lsb, 
						unsigned char msb,
						unsigned long time)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
	
		QGM_PitchBend(song->pSong, time, channel, msb, lsb);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_AllNotesOff()
// --------------------------------------
//
//
BAEResult BAESong_AllNotesOff(BAESong song, unsigned long time)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (time == 0)
		{
			time = GM_GetSyncTimeStamp();
		}
	
		QGM_AllNotesOff(song->pSong, time);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_ParseMidiData()
// --------------------------------------
//
//
BAEResult BAESong_ParseMidiData(BAESong song, unsigned char commandByte, unsigned char data1Byte, 
					unsigned char data2Byte, unsigned char data3Byte,
					unsigned long time)
{
	BAEResult theErr;
	unsigned char	channel;

	theErr = BAE_NO_ERROR;
	if (song)
	{
		channel = commandByte & 0x0F;
		data3Byte = data3Byte;
		switch(commandByte & 0xF0)
		{
			case 0x80:	// Note off
				theErr = BAESong_NoteOff(song, channel, data1Byte, data2Byte, time);
				break;
			case 0x90:	// Note on
				theErr = BAESong_NoteOn(song, channel, data1Byte, data2Byte, time);
				break;
			case 0xA0:	// key pressure (aftertouch)
				theErr = BAESong_KeyPressure(song, channel, data1Byte, data2Byte, time);
				break;
			case 0xB0:	// controllers
				theErr = BAESong_ControlChange(song, channel, data1Byte, data2Byte, time);
				break;
			case 0xC0:	// Program change
				theErr = BAESong_ProgramChange(song, channel, data1Byte, time);
				break;
			case 0xD0:	// channel pressure (aftertouch)
				theErr = BAESong_ChannelPressure(song, channel, data1Byte, time);
				break;
			case 0xE0:	// SetPitchBend
				theErr = BAESong_PitchBend(song, channel, data1Byte, data2Byte, time);
				break;
		}
	}
	else
	{
		theErr = BAE_NULL_OBJECT;
	}
	return theErr;
}


// BAESong_Preroll()
// --------------------------------------
//
//
BAEResult BAESong_Preroll(BAESong song)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		err = GM_PrerollSong(song->pSong, NULL, FALSE, FALSE);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_Start()
// --------------------------------------
//
//
BAEResult BAESong_Start(BAESong song, short int priority)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (song->mixer)
		{
			GM_SetSongPriority(song->pSong, priority);
			GM_SetSongVolume(song->pSong, song->mVolume);
			err = GM_BeginSong(song->pSong, NULL, FALSE, FALSE);
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_Stop()
// --------------------------------------
//
//
BAEResult BAESong_Stop(BAESong song, BAE_BOOL startFade)
{
	OPErr		err;
	BAE_BOOL	isPaused;

	err = NO_ERR;
	if (song)
	{
		BAESong_IsPaused(song, &isPaused);
		if (isPaused)
		{
			BAESong_Resume(song);
		}
	
		if (startFade)
		{
			song->mVolume = GM_GetSongVolume(song->pSong);
			GM_SetSongFadeRate(song->pSong, FLOAT_TO_FIXED(2.2), 0, song->mVolume, TRUE);
		}
		else
		{
			GM_KillSongNotes(song->pSong);
			// End the song, but keep it active in the mixer. This is done so that we can still send
			// midi events to this object.
			GM_EndSongButKeepActive(NULL, song->pSong);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// PV_CalculateTimeDeltaForFade()
// --------------------------------------
//
//
static double PV_CalculateTimeDeltaForFade(BAE_FIXED sourceVolume, BAE_FIXED destVolume, BAE_FIXED timeInMiliseconds)
{
	double	delta;
	double	source, dest;
	double	time;

	source = FIXED_TO_FLOAT(sourceVolume);
	dest = FIXED_TO_FLOAT(destVolume);
	time = FIXED_TO_FLOAT(timeInMiliseconds) * 1000;

	delta = (dest - source) / (time / BAE_GetSliceTimeInMicroseconds());
	return delta;
}


// BAESong_Fade()
// --------------------------------------
//
//
BAEResult BAESong_Fade(BAESong song, BAE_FIXED sourceVolume, BAE_FIXED destVolume, BAE_FIXED timeInMiliseconds)
{
	short int	source, dest;
	double		delta;
	INT16		minVolume;
	INT16		maxVolume;
	OPErr		err;
	
	err = NO_ERR;
	if (song)
	{
		if (song->pSong)
		{
			delta = PV_CalculateTimeDeltaForFade(sourceVolume, destVolume, timeInMiliseconds);
			delta = delta * -MAX_SONG_VOLUME;
	
			source = FIXED_TO_SHORT_ROUNDED(sourceVolume * MAX_SONG_VOLUME);
			dest = FIXED_TO_SHORT_ROUNDED(destVolume * MAX_SONG_VOLUME);
			minVolume = XMIN(source, dest);
			maxVolume = XMAX(source, dest);
			GM_SetSongFadeRate(song->pSong, FLOAT_TO_FIXED(delta), minVolume, maxVolume, FALSE);
		}
		else
		{
			err = NOT_SETUP;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}

// BAESong_Pause()
// --------------------------------------
//
//
BAEResult BAESong_Pause(BAESong song)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_PauseSong(song->pSong);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_Resume()
// --------------------------------------
//
//
BAEResult BAESong_Resume(BAESong song)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_ResumeSong(song->pSong);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_IsPaused()
// --------------------------------------
//
//
BAEResult BAESong_IsPaused(BAESong song, BAE_BOOL *outIsPaused)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outIsPaused)
		{
			*outIsPaused = GM_IsSongPaused(song->pSong);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SetLoops()
// --------------------------------------
//
//
BAEResult BAESong_SetLoops(BAESong song, short numLoops)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (numLoops >= 0)
		{
			GM_SetSongLoopMax(song->pSong, numLoops);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetLoops()
// --------------------------------------
//
//
BAEResult BAESong_GetLoops(BAESong song, short *outNumLoops)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (outNumLoops)
		{
			*outNumLoops = GM_GetSongLoopMax(song->pSong);
		}
		else
		{
			err = PARAM_ERR;
		}
}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetMicrosecondLength()
// --------------------------------------
//
//
BAEResult BAESong_GetMicrosecondLength(BAESong song, unsigned long *outLength)
{
	OPErr err;
	
	err = NO_ERR;	
	if (song)
	{
		if (outLength)
		{
			*outLength = GM_GetSongMicrosecondLength(song->pSong, &err);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SetMicrosecondPosition()
// --------------------------------------
//
//
BAEResult BAESong_SetMicrosecondPosition(BAESong song, unsigned long ticks)
{
	OPErr	err;

	err = NO_ERR;
	if (song)
	{
		if (song->pSong) 	// MOVE THIS CHECK INTO THE ENGINE
		{
			err = GM_SetSongMicrosecondPosition(song->pSong, ticks);
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetMicrosecondPosition()
// --------------------------------------
//
//
BAEResult BAESong_GetMicrosecondPosition(BAESong song, unsigned long *outTicks)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outTicks)
		{
			*outTicks = GM_SongMicroseconds(song->pSong);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_IsDone()
// --------------------------------------
//
//
BAEResult BAESong_IsDone(BAESong song, BAE_BOOL *outIsDone)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outIsDone)
		{
			*outIsDone = (BAE_BOOL)GM_IsSongDone(song->pSong);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_AreMidiEventsPending()
// --------------------------------------
// returns TRUE if there are midi events pending
//
BAEResult BAESong_AreMidiEventsPending(BAESong song, BAE_BOOL *outPending)
{
	OPErr err;

	err = NO_ERR;
	if (song)
	{
		if (outPending)
		{
			*outPending = FALSE;
			if (song->pSong)
			{
				// first check to see if there are any events posted into the queue
				*outPending = (BAE_BOOL)GM_AreEventsPending(song->pSong);
				if (*outPending == FALSE)
				{
					// none there, so see if this is a midi file playing via the sequencer
					*outPending = ((BAE_BOOL)GM_IsSongDone(song->pSong) ? FALSE : TRUE);
				}
			}
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SetMasterTempo()
// --------------------------------------
//
//
BAEResult BAESong_SetMasterTempo(BAESong song, BAE_UNSIGNED_FIXED tempoFactor)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_SetMasterSongTempo(song->pSong, tempoFactor);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetMasterTempo()
// --------------------------------------
//
//
BAEResult BAESong_GetMasterTempo(BAESong song, BAE_UNSIGNED_FIXED *outTempoFactor)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outTempoFactor)
		{
			*outTempoFactor = GM_GetMasterSongTempo(song->pSong);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_MuteTrack()
// --------------------------------------
//
//
BAEResult BAESong_MuteTrack(BAESong song, unsigned short int track)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_MuteTrack(song->pSong, track);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_UnmuteTrack()
// --------------------------------------
//
//
BAEResult BAESong_UnmuteTrack(BAESong song, unsigned short int track)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_UnmuteTrack(song->pSong, track);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_GetTrackMuteStatus()
// --------------------------------------
//
//
BAEResult BAESong_GetTrackMuteStatus(BAESong song, BAE_BOOL *outTracks)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outTracks)
		{
			GM_GetTrackMuteStatus(song->pSong, outTracks);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_SoloTrack()
// --------------------------------------
//
//
BAEResult BAESong_SoloTrack(BAESong song, unsigned short int track)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_SoloTrack(song->pSong, track);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


// BAESong_UnSoloTrack()
// --------------------------------------
//
//
BAEResult BAESong_UnSoloTrack(BAESong song, unsigned short int track)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		GM_UnsoloTrack(song->pSong, track);
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}


//  BAESong_GetSoloTrackStatus()
// --------------------------------------
//
//
BAEResult BAESong_GetSoloTrackStatus(BAESong song, BAE_BOOL *outTracks)
{
	OPErr err;
	
	err = NO_ERR;
	if (song)
	{
		if (outTracks)
		{
			GM_GetTrackSoloStatus(song->pSong, outTracks);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = NULL_OBJECT;
	}
	return BAE_TranslateOPErr(err);
}





// ------------------------------------------------------------------
// Utility Functions
// ------------------------------------------------------------------
#if 0
	#pragma mark -
	#pragma mark >>>>> Utility Functions <<<<<
#endif


#if USE_FULL_RMF_SUPPORT == TRUE
// PV_TranslateInfoType()
// --------------------------------------
//
//
static SongInfo PV_TranslateInfoType(BAEInfoType infoType)
{
	SongInfo	info;

	switch (infoType)
	{
		default:
			info = I_INVALID;
			break;
		case TITLE_INFO:
			info = I_TITLE;
			break;
		case PERFORMED_BY_INFO:
			info = I_PERFORMED_BY;
			break;
		case COMPOSER_INFO:
			info = I_COMPOSER;
			break;
		case COPYRIGHT_INFO:
			info = I_COPYRIGHT;
			break;
		case PUBLISHER_CONTACT_INFO:
			info = I_PUBLISHER_CONTACT;
			break;
		case USE_OF_LICENSE_INFO:
			info = I_USE_OF_LICENSE;
			break;
		case LICENSE_TERM_INFO:
			info = I_LICENSE_TERM;
			break;
		case LICENSED_TO_URL_INFO:
			info = I_LICENSED_TO_URL;
			break;
		case EXPIRATION_DATE_INFO:
			info = I_EXPIRATION_DATE;
			break;
		case COMPOSER_NOTES_INFO:
			info = I_COMPOSER_NOTES;
			break;
		case INDEX_NUMBER_INFO:
			info = I_INDEX_NUMBER;
			break;
		case GENRE_INFO:
			info = I_GENRE;
			break;
		case SUB_GENRE_INFO:
			info = I_SUB_GENRE;
			break;
		case TEMPO_DESCRIPTION_INFO:
			info = I_TEMPO;
			break;
		case ORIGINAL_SOURCE_INFO:
			info = I_ORIGINAL_SOURCE;
			break;
	}
	return info;
}
#endif	//#if USE_FULL_RMF_SUPPORT == TRUE


// TranslateBankProgramToInstrument()
// --------------------------------------
//
//
BAE_INSTRUMENT TranslateBankProgramToInstrument(unsigned short bank, 
									unsigned short program, 
									unsigned short channel,
									unsigned short note)
{
	BAE_INSTRUMENT	instrument;

	instrument = program;
	if (channel == PERCUSSION_CHANNEL)
	{
		bank = (bank * 2) + 1;		// odd banks are percussion
	}
	else
	{
		bank = bank * 2 + 0;		// even banks are for instruments
		note = 0;
	}

	if (bank < MAX_BANKS)
	{
		instrument = (bank * 128) + program + note;
	}

	return instrument;
}


// PV_GetRmfSongResource()
// --------------------------------------
//
//
#if USE_FULL_RMF_SUPPORT == TRUE
static OPErr PV_GetRmfSongResource(void *pRMFData, unsigned long rmfSize, short index, 
						SongResource **ppOutResource, long *pOutResourceSize)
{
	XFILE				fileRef;
	XLongResourceID		theID;
	OPErr				err;

	err = NO_ERR;
	if (pRMFData && rmfSize && ppOutResource && pOutResourceSize)
	{
		fileRef = XFileOpenResourceFromMemory((XPTR)pRMFData, rmfSize, FALSE);
		if (fileRef)
		{
			*ppOutResource = (SongResource *)XGetIndexedResource(ID_SONG, &theID, index, NULL, pOutResourceSize);
			if (*ppOutResource)
			{
				err = NO_ERR;
			}
			else
			{
				err = PARAM_ERR;
			}
			XFileClose(fileRef);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = PARAM_ERR;
	}
	return err;
}
#endif	//#if USE_FULL_RMF_SUPPORT == TRUE


// BAEUtil_GetRmfSongInfo()
// --------------------------------------
//
//
BAEResult BAEUtil_GetRmfSongInfo(void *pRMFData, unsigned long rmfSize, short songIndex,
							 BAEInfoType infoType, char* targetBuffer, unsigned long bufferBytes)
{
#if USE_FULL_RMF_SUPPORT == TRUE
	SongInfo		info;
	BAEResult		theErr;
	SongResource 	*pSongRes;
	long			songResSize;

	theErr = BAE_NO_ERROR;
	targetBuffer[0] = 0;
	info = PV_TranslateInfoType(infoType);

	if (info != I_INVALID)
	{
		if (PV_GetRmfSongResource(pRMFData, rmfSize, songIndex, &pSongRes, &songResSize) == NO_ERR)
		{
			XGetSongInformation(pSongRes, songResSize, info, targetBuffer, bufferBytes);

#if (X_PLATFORM != X_MACINTOSH)
			// data stored in the copyright fields is Mac ASCII, any other platform should translate
			while (*targetBuffer)
			{
				*targetBuffer = XTranslateMacToWin(*targetBuffer);
				targetBuffer++;
			}
#endif
		}
	}
	else
	{
		theErr = BAE_PARAM_ERR;
	}
	return theErr;
#else
	infoType;
	targetBuffer[0] = 0;
	return BAE_NOT_SETUP;
#endif
}


// BAEUtil_GetInfoSize()
// --------------------------------------
//
//
unsigned long BAEUtil_GetInfoSize(void *pRMFData, unsigned long rmfSize, short songIndex, BAEInfoType infoType)
{
#if USE_FULL_RMF_SUPPORT == TRUE
	SongInfo		info;
	unsigned long	size;
	SongResource 	*pSongRes;
	long			songResSize;

	size = 0;
	info = PV_TranslateInfoType(infoType);
	if (info != I_INVALID)
	{
		if (PV_GetRmfSongResource(pRMFData, rmfSize, songIndex, &pSongRes, &songResSize) == NO_ERR)
		{
			size = XGetSongInformationSize(pSongRes, songResSize, info);
		}
	}
	return size;
#else
	infoType;
	return 0;
#endif
}


#if USE_FULL_RMF_SUPPORT == TRUE
// BAEUtil_IsRmfSongEncrypted()
// --------------------------------------
//
//
BAE_BOOL BAEUtil_IsRmfSongEncrypted(void *pRMFData, unsigned long rmfSize, short songIndex)
{
	SongResource 	*pSongRes;
	long			songResSize;
	BAE_BOOL		locked;
	
	pSongRes = NULL;
	songResSize = 0;
	locked = FALSE;
	
	if (PV_GetRmfSongResource(pRMFData, rmfSize, songIndex, &pSongRes, &songResSize) == NO_ERR)
	{
		locked = (BAE_BOOL)XIsSongLocked(pSongRes);
		XDisposePtr((XPTR)pSongRes);
	}
	return locked;
}


// BAEUtil_IsRmfSongCompressed()
// --------------------------------------
//
//
BAE_BOOL BAEUtil_IsRmfSongCompressed(void *pRMFData, unsigned long rmfSize, short songIndex)
{
	SongResource 	*pSongRes;
	long			songResSize;
	BAE_BOOL 		compressed;
	
	pSongRes = NULL;
	songResSize = 0;
	compressed = FALSE;
	
	if (PV_GetRmfSongResource(pRMFData, rmfSize, songIndex, &pSongRes, &songResSize) == NO_ERR)
	{
		compressed = (BAE_BOOL)XIsSongCompressed(pSongRes);
		XDisposePtr((XPTR)pSongRes);
	}
	return compressed;
}
#endif	//#if USE_FULL_RMF_SUPPORT == TRUE

#if USE_FULL_RMF_SUPPORT == TRUE
// BAEUtil_GetRmfVersion()
// --------------------------------------
//
//
BAEResult BAEUtil_GetRmfVersion(void *pRMFData, unsigned long rmfSize, 
								short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor)
{
	XFILE				fileRef;
	OPErr				err;
	XVersion			vers;

	err = NO_ERR;
	vers.versionMajor = 0;
	vers.versionMinor = 0;
	vers.versionSubMinor = 0;
	
	if (pRMFData && rmfSize && pVersionMajor && pVersionMinor && pVersionSubMinor)
	{
		fileRef = XFileOpenResourceFromMemory((XPTR)pRMFData, rmfSize, FALSE);
		if (fileRef)
		{
			XGetVersionNumber(&vers);
		}
		else
		{
			err = PARAM_ERR;
		}
	}
	else
	{
		err = PARAM_ERR;
	}
	*pVersionMajor = vers.versionMajor;
	*pVersionMinor = vers.versionMinor;
	*pVersionSubMinor = vers.versionMinor;
	return BAE_TranslateOPErr(err);
}
#endif	// #if USE_FULL_RMF_SUPPORT == TRUE

// PV_TranslateTerpModeToBAETerpMode()
// -------------------------------------------
//
//
static BAETerpMode PV_TranslateTerpModeToBAETerpMode(TerpMode mode_in)
{
	BAETerpMode mode_out;

	switch (mode_in)
	{
		case E_AMP_SCALED_DROP_SAMPLE:
			mode_out = BAE_DROP_SAMPLE;
			break;
		case E_2_POINT_INTERPOLATION:
			mode_out = BAE_2_POINT_INTERPOLATION;
			break;
		case E_LINEAR_INTERPOLATION:
		case E_LINEAR_INTERPOLATION_FLOAT:
		case E_LINEAR_INTERPOLATION_U3232:		
			mode_out = BAE_LINEAR_INTERPOLATION;
			break;
		default:
			BAE_ASSERT(FALSE);
	}
	return mode_out;
}

static char mCopyright[] =
{
"\
(c) Copyright 1996-2000 Beatnik, Inc, All Rights Reserved\r\
Beatnik products contain certain trade secrets and confidential and \
proprietary information of Beatnik.  Use, reproduction, disclosure \
and distribution by any means are prohibited, except pursuant to \
a written license from Beatnik. Use of copyright notice is \
precautionary and does not imply publication or disclosure.\
\r\
Restricted Rights Legend:\r\
Use, duplication, or disclosure by the Government is subject to \
restrictions as set forth in subparagraph (c)(1)(ii) of The \
Rights in Technical Data and Computer Software clause in DFARS \
252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial \
Computer Software--Restricted Rights at 48 CFR 52.227-19, as \
applicable."
};

static char mAboutNames[] =
{
"\
Audio Engine Programming: \
Steve Hales, \
Mark Deggeller, \
Doug Scott, \
Andrew Rostaing, \
John Cooper, \
Jim Nitchals, \
Chris Rogers, \
Chris Schardt \
QA: \
Elizabeth Smolgovsky, \
Kari Reynolds, \
Chris Muir, \
Chris van Rensburg, \
Chris Ticknor, \
Sean Echevarria, \
Tim Maroney, \
Sal Orlando \
Music: \
Brian Salter \
Documentation: \
Chris Grigg \
In memory of Jim Nitchals, 1962-1998.  A subtle genius and original thinker."
};

// ************************************************************************
// ********************** *OutputToFile methods ***************************
// ********************** ported from big BAE   ***************************
// ********************** 11/28/2000 tom        ***************************

#define DUMP_OUTPUTFILE 0

// global fields for OutputToFile methods - BAEMixer class members in BAE

#if DUMP_OUTPUTFILE
	FILE *fp;
#endif

		BAE_BOOL			mWritingToFile;
		BAEFileType			mWriteToFileType;
		void				*mWritingToFileReference;
		void				*mWritingEncoder;
		void				*mWritingDataBlock;
		unsigned long		mWritingDataBlockSize;

// ********************** BAEMixer_StartOutputToFile ************************
// ********************** added from BAE 11/28/00 tom ***********************
// ********************** parameter 1 added for MiniBAE *********************
// ********************** method name changed for MiniBAE conformance *******
// 'iModifiers' and 'iQuality' replaced by calls to accessor methods ********

// start saving audio output to a file
BAEResult BAEMixer_StartOutputToFile(BAEMixer theMixer,
									 BAEPathName pAudioOutputFile,
									 BAEFileType outputType,
									 BAECompressionType compressionType)
{
#if USE_CREATION_API == TRUE
	OPErr			theErr;
	XFILENAME		theFile;

// begin block added for MiniBAE 11/28/00 tom
	BAEAudioModifiers theModifiers;
	BAEQuality theQuality;
	BAEResult err;
// end block added for MiniBAE

#if DUMP_OUTPUTFILE
#if DUMP_C_PLUS_PLUS
	std::string fullpath("C:\\temp\\test.txt");
	file = new std::ofstream(fullpath.c_str(), std::ios::out);

	*file << "BAEOutputMixer::StartOutputToFile dump\n";
#else
	fp = fopen("C:\\temp\\test.txt", "w");
	if(fp)
		fprintf(fp, "BAEOutputMixer::StartOutputToFile dump\n");
#endif
#endif

// begin block added for MiniBAE  11/28/00  tom
	err = BAEMixer_GetModifiers(theMixer, &theModifiers);
	err = BAEMixer_GetQuality(theMixer, &theQuality);
// end block added for MiniBAE

	theErr = NO_ERR;

	// close old one first
	if (mWritingToFile)
	{
		// StopOutputToFile();
		BAEMixer_StopOutputToFile();
	}

	mWriteToFileType = outputType;
	XConvertNativeFileToXFILENAME(pAudioOutputFile, &theFile);

	// mWritingDataBlock is where we will store the results of BAE_BuildMixerSlice() 
	if (mWritingDataBlock)
	{
		XDisposePtr(mWritingDataBlock);
	}
	mWritingDataBlockSize = GM_GetAudioBufferOutputSize();
	
	mWritingDataBlock = XNewPtr(mWritingDataBlockSize);

#if DUMP_OUTPUTFILE
#if DUMP_C_PLUS_PLUS
	*file << "\nmWritingDataBlockSize = ";
	*file << mWritingDataBlockSize;

	*file << "\noutput rate = ";
	*file << GM_ConvertFromOutputQualityToRate(iQuality);

	*file << "\nmaxChunkSize = " << MusicGlobals->maxChunkSize;
	*file << "\noutputQuality = " << MusicGlobals->outputQuality;
#else
	if(fp)
	{
		fprintf(fp,"\nmWritingDataBlockSize = %d",mWritingDataBlockSize);

		fprintf(fp,"\noutput rate = %d",GM_ConvertFromOutputQualityToRate(theQuality));

		fprintf(fp,"\nmaxChunkSize = %d",MusicGlobals->maxChunkSize);
		fprintf(fp,"\noutputQuality = %d",MusicGlobals->outputQuality);
	}
#endif
#endif

	switch (outputType)
	{
#if USE_MPEG_ENCODER != FALSE
		case BAE_MPEG_TYPE:
		{
			if (iModifiers & BAE_USE_16)
			{
				mWritingToFileReference = (void *)XFileOpenForWrite(&theFile, TRUE);
				if (mWritingToFileReference)
				{
					XDWORD channels = (theModifiers /*iModifiers*/ & BAE_USE_STEREO) ? 2 : 1;
					mWritingEncoder = MPG_EncodeNewStream(BAE_TranslateMPEGTypeToBitrate(compressionType),
														  GM_ConvertFromOutputQualityToRate(theQuality /*iQuality*/),
													      channels,
												   	      mWritingDataBlock,
													      mWritingDataBlockSize / (sizeof(short) * channels));

					MPG_EncodeSetRefillCallback(mWritingEncoder, PV_RefillMPEGEncodeBuffer, (void*)this);

					GM_StopHardwareSoundManager(NULL);		// disengage from hardware
					mWritingToFile = TRUE;
				}
				else
				{
					theErr = BAD_FILE;
				}
			}
			else
			{
				// Can only encode 16bit data.
				theErr = PARAM_ERR;
			}
		} break;
#else
		compressionType;
#endif
		case BAE_WAVE_TYPE:
		case BAE_AIFF_TYPE:
		case BAE_AU_TYPE:
		{
			GM_Waveform *w = GM_NewWaveform();
			char buf[4] = {0,0,0,0};

			// initialize GM_Waveform with one frame of data, so that GM_WriteFileFromMemory()
			// doesn't complain.



			w->bitSize = ( theModifiers/*iModifiers*/ & BAE_USE_16) ? 16 : 8;
			w->channels = ( theModifiers /*iModifiers*/ & BAE_USE_STEREO) ? 2 : 1;
			w->sampledRate = LONG_TO_UNSIGNED_FIXED(GM_ConvertFromOutputQualityToRate(theQuality/*iQuality*/));
			w->compressionType = C_NONE;
			w->theWaveform = &buf;
			w->waveFrames = 1;
			w->waveSize = (w->bitSize/8) * (w->channels);

			// Write out the header now, we'll add data to it in ServiceAudioOutputToFile()
			theErr = GM_WriteFileFromMemory(&theFile, w, BAE_TranslateBAEFileType(outputType));

			GM_FreeWaveform(w);
			w = NULL;

			// Reopen the file and jump to the end, so we can add data to it later...
			mWritingToFileReference = (void *)XFileOpenForWrite(&theFile, FALSE);
			if (mWritingToFileReference)
			{
				XFileSetPosition((XFILE)mWritingToFileReference, XFileGetLength((XFILE)mWritingToFileReference));
				
				GM_StopHardwareSoundManager(NULL);		// disengage from hardware
				mWritingToFile = TRUE;
			}
			else
			{
				theErr = BAD_FILE;
			}
		} break;

		case BAE_RAW_PCM:
			mWritingToFileReference = (void *)XFileOpenForWrite(&theFile, TRUE);
			if (mWritingToFileReference)
			{
				GM_StopHardwareSoundManager(NULL);		// disengage from hardware
				mWritingToFile = TRUE;
			}
			else
			{
				theErr = BAD_FILE;
			}
			break;

		default:
			theErr = BAD_FILE_TYPE;
			break;
	}
	
	if (theErr != NO_ERR)
	{
		XDisposePtr(mWritingDataBlock);
		mWritingDataBlock = NULL;
	}
	return BAE_TranslateOPErr(theErr);
#else
	pAudioOutputFile = pAudioOutputFile;
	return BAE_NOT_SETUP;
#endif
}

// *********************** BAEMixer_StopOutputToFile ***********************
// ********************** added from BAE 11/28/00 tom **********************
// ********************** method name changed for MiniBAE conformance ******

// Stop saving audio output to a file
void BAEMixer_StopOutputToFile(void)
{
	if (mWritingToFile && mWritingToFileReference)
	{
		switch (mWriteToFileType)
		{
#if USE_MPEG_ENCODER != FALSE
			case BAE_MPEG_TYPE:
				MPG_EncodeFreeStream(mWritingEncoder);
				mWritingEncoder = NULL;
				break;
#endif
			case BAE_WAVE_TYPE:
			case BAE_AIFF_TYPE:
			case BAE_AU_TYPE:
				GM_FinalizeFileHeader((XFILE)mWritingToFileReference, BAE_TranslateBAEFileType(mWriteToFileType));
				break;

			default:
				break;
		}
		XFileClose((XFILE)mWritingToFileReference);
		mWritingToFileReference = NULL;

		XDisposePtr(mWritingDataBlock);
		mWritingDataBlock = NULL;

		GM_StartHardwareSoundManager(NULL);			// reconnect to hardware
	}
	mWritingToFile = FALSE;
#if DUMP_OUTPUTFILE
#if DUMP_C_PLUS_PLUS
	delete file;
#else
	if(fp)
		fclose(fp);
#endif
#endif
}

// ************************ BAEMixer_ServiceAudioOutputToFile ************
// ********************** added from BAE 11/28/00 tom ********************
// ********************** method name changed for MiniBAE conformance ****
// ********************** parameter 1 added for MiniBAE ******************
// 'iModifiers'  replaced by call to accessor method  ********************

BAEResult BAEMixer_ServiceAudioOutputToFile(BAEMixer theMixer)
{
	long						sampleSize, channels;
	OPErr						theErr;

// begin block added for MiniBAE
	BAEAudioModifiers theModifiers;
	BAEResult err = BAEMixer_GetModifiers(theMixer, &theModifiers);
// end block added for MiniBAE

	theErr = NO_ERR;



	if (mWritingToFile && mWritingToFileReference)
	{
		channels = ( theModifiers /*iModifiers*/ & ~BAE_USE_STEREO) ? 2 : 1;
		sampleSize = (theModifiers /*iModifiers*/ & BAE_USE_16) ? 2 : 1;
		if (mWritingDataBlockSize)
		{
			if (mWritingDataBlockSize < 8192 && mWritingDataBlock)
			{
#if DUMP_OUTPUTFILE
#if DUMP_C_PLUS_PLUS
				*file << "\nwite block size = ";
				*file << mWritingDataBlockSize;
				*file << ", ";
				*file << mWritingDataBlockSize / sampleSize / channels;
				*file << "\nsampleSize = " << sampleSize << "channels = " << channels;
#else
		if(fp)
		{
			fprintf(fp,"\nwite block size = %d",mWritingDataBlockSize);
			fprintf(fp,", %d",mWritingDataBlockSize / sampleSize / channels);
			fprintf(fp, "\nsampleSize = %d, ""channels = %d", sampleSize, channels);
		}
#endif
#endif
				switch (mWriteToFileType)
				{
#if USE_MPEG_ENCODER != FALSE
					case BAE_MPEG_TYPE:
					{
						XPTR compressedData = NULL;
						XDWORD compressedLength = NULL;
						XBOOL isDone;

						MPG_EncodeProcess(mWritingEncoder, &compressedData, &compressedLength, &isDone);
						if (compressedLength > 0)
						{
							if (XFileWrite((XFILE)mWritingToFileReference, compressedData, compressedLength) == -1)
							{
								theErr = BAD_FILE;
							}
						}
					} 
					break;
#endif

					case BAE_RAW_PCM:
					{
						BAE_BuildMixerSlice(NULL, mWritingDataBlock, mWritingDataBlockSize, 
											(unsigned long)(mWritingDataBlockSize / sampleSize / channels));
						if (XFileWrite((XFILE)mWritingToFileReference, mWritingDataBlock, mWritingDataBlockSize) == -1)
						{
							theErr = BAD_FILE;
						}
					} break;

					case BAE_WAVE_TYPE:
					case BAE_AIFF_TYPE:
					case BAE_AU_TYPE:
					{
						BAE_BuildMixerSlice(NULL, mWritingDataBlock, mWritingDataBlockSize, 
												(unsigned long)(mWritingDataBlockSize / sampleSize / channels));
						theErr = GM_WriteAudioBufferToFile((XFILE)mWritingToFileReference, 
															BAE_TranslateBAEFileType(mWriteToFileType),
															mWritingDataBlock,
															mWritingDataBlockSize,
															channels,
															sampleSize);
					}
					break;

					default:
					{
						theErr = BAD_FILE_TYPE;
					}
					break;
				}
			}
			else
			{
				theErr = BUFFER_TO_SMALL;
			}
		}
		else
		{
			theErr = BUFFER_TO_SMALL;
		}
	}
	else
	{
		theErr = NOT_SETUP;
	}
	return BAE_TranslateOPErr(theErr);
}



// EOF MiniBAE.c


