/****************************************************************************
*
* simple.c
*
* a command line audiofile player that supports RMF
*
* � Copyright 1999 Beatnik, Inc, All Rights Reserved.
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
*				Added BeatnikPlay(), PlaySound(), playbae()
*	10/26/99	Added PlayMIDI()
*
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MiniBAE.h"
#include "BAE_API.h"

// prototypes

char const usageString[] =
{
   "USAGE:  playbae  -p  {patches.hsb}\n"
   "                 -w  {Play a WAV file}\n"
   "                 -sw {Stream a WAV file}\n"
   "                 -sa {Stream a AIF file}\n"
   "                 -a  {Play a AIF file}\n"
   "                 -r  {Play a RMF file}\n"
   "                 -m  {Play a MID file}\n"
   "                 -o  {write output to file}\n"
   "                 -mr {mixer sample rate ie. 11025}\n"
};


static int gWriteToFile = FALSE;

static void PV_Task(void *reference)
{
   if (reference)
   {
      BAEMixer_ServiceStreams(reference);
   }
}

static void PV_Idle(BAEMixer theMixer, unsigned long time)
{
   unsigned long count;
   unsigned long max;

   if (gWriteToFile)
   {
      BAEMixer_ServiceAudioOutputToFile(theMixer);
   }
   else
   {
      max = time / 12000;
      for (count = 0; count < max; count++)
      {
         BAE_WaitMicroseconds(12000);
      }
   }
}

static void PV_StreamCallback(BAEStream stream, unsigned long reference)
{
   printf("Stream %p reference %lx done\n", stream, reference);
}

static int PV_ParseCommands(int argc, char *argv[], char *command, int getResult, char *result)
{
   int count = 0;

   while (argc--)
   {
      if (strcmp(argv[count], command) == 0)
      {
         if (getResult)
         {
            strcpy(result, argv[count + 1]);
         }
         return(1);
      }
      count++;
   }
   return(0);
}


// PlayPCM()
// ---------------------------------------------------------------------
//
//
static BAEResult PlayPCM(BAEMixer theMixer, char *fileName, BAEFileType type)
{
   BAEResult err;
   BAESound  sound = BAESound_New(theMixer);
   BAE_BOOL  done;

   if (sound)
   {
      err = BAESound_LoadFileSample(sound, (BAEPathName)fileName, type);
      if (err == BAE_NO_ERROR)
      {
         err = BAESound_Start(sound, 0, BAE_FIXED_1, 0);
         if (err == BAE_NO_ERROR)
         {
            printf("BAE memory used for everything %ld\n\n", BAE_GetSizeOfMemoryUsed());
            done = FALSE;
            while (done == FALSE)
            {
               BAESound_IsDone(sound, &done);
               if (done == FALSE)
               {
                  PV_Idle(theMixer, 15000);
               }
            }
            PV_Idle(theMixer, 900000);
         }
         else
         {
            printf("playbae:  Couldn't start sound (BAE Error #%d)\n", err);
         }
      }
      else
      {
         printf("playbae:  Couldn't open sound file '%s' (BAE Error #%d)\n", fileName, err);
      }
   }
   else
   {
      err = BAE_MEMORY_ERR;
   }
   BAESound_Delete(sound);
   return(err);
}

// PlayPCMStreamed()
// ---------------------------------------------------------------------
//
//
static BAEResult PlayPCMStreamed(BAEMixer theMixer, char *fileName, BAEFileType type)
{
   BAEResult err;
   BAEStream stream = BAEStream_New(theMixer);
   BAE_BOOL  done;

   if (stream)
   {
      err = BAEStream_SetupFile(stream, (BAEPathName)fileName,
                                type,
                                BAE_MIN_STREAM_BUFFER_SIZE,
                                FALSE);

      if (err == BAE_NO_ERROR)
      {
         BAEStream_SetCallback(stream, PV_StreamCallback, 0x1234);
         err = BAEStream_Start(stream);
         if (err == BAE_NO_ERROR)
         {
            printf("BAE memory used for everything %ld\n\n", BAE_GetSizeOfMemoryUsed());
            done = FALSE;
            while (done == FALSE)
            {
               BAEStream_IsDone(stream, &done);
               if (done == FALSE)
               {
                  PV_Idle(theMixer, 15000);
               }
            }
            PV_Idle(theMixer, 900000);
         }
         else
         {
            printf("playbae:  Couldn't start sound (BAE Error #%d)\n", err);
         }
      }
      else
      {
         printf("playbae:  Couldn't open sound file '%s' (BAE Error #%d)\n", fileName, err);
      }
   }
   else
   {
      err = BAE_MEMORY_ERR;
   }
   BAEStream_Delete(stream);
   return(err);
}


// PlayMidi()
// ---------------------------------------------------------------------
//
//
static BAEResult PlayMidi(BAEMixer theMixer, char *fileName)
{
   BAEResult err;
   BAESong   theSong = BAESong_New(theMixer);
   BAE_BOOL  done;

   if (theSong)
   {
#if 0
      err = BAESong_ControlChange(theSong, 0, 1, 1, 0);

#else
      err = BAESong_LoadMidiFromFile(theSong, (BAEPathName)fileName, TRUE);
      if (err == BAE_NO_ERROR)
      {
         err = BAESong_Start(theSong, 0);
         BAESong_DisplayInfo(theSong);
         if (err == BAE_NO_ERROR)
         {
            printf("BAE memory used for everything %ld\n\n", BAE_GetSizeOfMemoryUsed());
            done = FALSE;
            while (done == FALSE)
            {
               BAESong_IsDone(theSong, &done);
               if (done == FALSE)
               {
                  PV_Idle(theMixer, 15000);
               }
            }
            PV_Idle(theMixer, 900000);
         }
         else
         {
            printf("playbae:  Couldn't start song (BAE Error #%d)\n", err);
         }
      }
      else
      {
         printf("playbae:  Couldn't open Midi file '%s' (BAE Error #%d)\n", fileName, err);
      }
#endif
   }
   else
   {
      err = BAE_MEMORY_ERR;
   }
   BAESong_Delete(theSong);
   return(err);
}

// PlayRMF()
// ---------------------------------------------------------------------
//
//
static BAEResult PlayRMF(BAEMixer theMixer, char *fileName)
{
   BAEResult err;
   BAESong   theSong = BAESong_New(theMixer);
   BAE_BOOL  done;

   if (theSong)
   {
      err = BAESong_LoadRmfFromFile(theSong, (BAEPathName)fileName, 0, TRUE);
      if (err == BAE_NO_ERROR)
      {
         err = BAESong_Start(theSong, 0);
         BAESong_DisplayInfo(theSong);
         BAESong_SetLoops(theSong, 0);
         if (err == BAE_NO_ERROR)
         {
            printf("BAE memory used for everything %ld\n\n", BAE_GetSizeOfMemoryUsed());
            done = FALSE;
            while (done == FALSE)
            {
               BAESong_IsDone(theSong, &done);
               if (done == FALSE)
               {
                  PV_Idle(theMixer, 15000);
               }
            }
            PV_Idle(theMixer, 900000);
         }
         else
         {
            printf("playbae:  Couldn't start song (BAE Error #%d)\n", err);
         }
      }
      else
      {
         printf("playbae:  Couldn't open RMF file '%s' (BAE Error #%d)\n", fileName, err);
      }
   }
   else
   {
      err = BAE_MEMORY_ERR;
   }
   BAESong_Delete(theSong);
   return(err);
}


// main()
// ---------------------------------------------------------------------
int main(int argc, char *argv[])
{
   BAEResult    err;
   BAEMixer     theMixer;
   short int    rmf, pcm, level;
   BAEBankToken bank;
   int          doneCommand = 0;
   char         parmFile[1024];
   BAERate      rate;

   theMixer = BAEMixer_New();
   if (theMixer)
   {
	   pcm   = 1;
	   rmf   = BAE_MAX_VOICES - pcm;
	   level = rmf / 3;
      rate  = BAE_RATE_44K;
      if (PV_ParseCommands(argc, argv, "-mr", TRUE, parmFile))
      {
         rate = (BAERate)atoi(parmFile);
      }

      printf("Allocating mixer with %d voices for RMF/Midi playback\n"
             "and %d voices for PCM playback at %d sample rate\n",
             rmf, pcm,
             rate);

      err = BAEMixer_Open(theMixer,
                          rate,
                          BAE_LINEAR_INTERPOLATION,
                          BAE_USE_STEREO | BAE_USE_16,
                          rmf,                                          // midi voices
                          pcm,                                          // pcm voices
                          level,
                          TRUE);
      if (err == BAE_NO_ERROR)
      {
         BAEMixer_SetAudioTask(theMixer, PV_Task, (void *)theMixer);

		  // turn on nice verb
		 BAEMixer_SetDefaultReverb(theMixer, BAE_REVERB_TYPE_8);
		  		  
         printf("BAE memory used during idle prior to SetBankToFile: %ld\n\n", BAE_GetSizeOfMemoryUsed());

         if (PV_ParseCommands(argc, argv, "-p", TRUE, parmFile))
         {
            printf("Using bank '%s'\n", parmFile);
            err = BAEMixer_AddBankFromFile(theMixer, (BAEPathName)parmFile, &bank);
            printf("BAE memory used during idle after SetBankToFile: %ld\n\n", BAE_GetSizeOfMemoryUsed());
         }

         if (PV_ParseCommands(argc, argv, "-o", TRUE, parmFile))
         {
            err = BAEMixer_StartOutputToFile(theMixer,
                                             (BAEPathName)parmFile,
                                             BAE_WAVE_TYPE,
                                             BAE_COMPRESSION_NONE);
            if (err)
            {
               printf("BAEMixer_StartOutputToFile failed with error %d\n", err);
            }
            else
            {
               gWriteToFile = TRUE;
               printf("Writing to file %s\n", parmFile);
            }
         }

         if (PV_ParseCommands(argc, argv, "-a", TRUE, parmFile))
         {
            printf("Play AIFF\n");
            err         = PlayPCM(theMixer, parmFile, BAE_AIFF_TYPE);
            doneCommand = 1;
         }
         if (PV_ParseCommands(argc, argv, "-sa", TRUE, parmFile))
         {
            printf("Stream AIFF\n");
            err         = PlayPCMStreamed(theMixer, parmFile, BAE_AIFF_TYPE);
            doneCommand = 1;
         }
         if (PV_ParseCommands(argc, argv, "-w", TRUE, parmFile))
         {
            printf("Play WAVE\n");
            err         = PlayPCM(theMixer, parmFile, BAE_WAVE_TYPE);
            doneCommand = 1;
         }
         if (PV_ParseCommands(argc, argv, "-sw", TRUE, parmFile))
         {
            printf("Stream WAVE\n");
            err         = PlayPCMStreamed(theMixer, parmFile, BAE_WAVE_TYPE);
            doneCommand = 1;
         }
         if (PV_ParseCommands(argc, argv, "-r", TRUE, parmFile))
         {
            printf("PlayRMF\n");
            err         = PlayRMF(theMixer, parmFile);
            doneCommand = 1;
         }
         if (PV_ParseCommands(argc, argv, "-m", TRUE, parmFile))
         {
            printf("PlayMidi\n");
            err         = PlayMidi(theMixer, parmFile);
            doneCommand = 1;
         }

         if (gWriteToFile)
         {
            BAEMixer_StopOutputToFile();
         }
      }
      else
      {
         printf("playbae:  Couldn't open mixer (BAE Error #%d)\n", err);
      }
   }
   else
   {
      printf("playbae:  Memory error.\n");
   }

   if (doneCommand == 0)
   {
      printf(usageString);
   }

   BAE_WaitMicroseconds(160000);
   BAEMixer_Delete(theMixer);
   return(0);
}


// EOF
