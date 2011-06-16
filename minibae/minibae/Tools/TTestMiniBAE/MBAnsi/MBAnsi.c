/*****************************************************************************
**	MBAnsi.c
**
**  MBAnsi - A version of MiniBAE that takes input files (ff type MID or RMF or WAV) and 
**  processes the data into an output file (of type "WAV") WITHOUT any audio engagement.
**
**  MBAnsi supports the OutputToFile methods from BAE, specially ported to MiniBAE
**  BAEMixer_StartOutputToFile, BAEMixer_StopOutputToFile and BAEMixer_ServiceOutputToFile
**  have been ported to MiniBAE, from (BAE)BAE.c to (MiniBAE)MiniBAE.c
**
**  GM_FinalizeFileHeader and GM_WriteAudioBufferToFile
**  have also been ported to MiniBAE, from (BAE)GenSoundFiles.c to (MiniBAE)GenSoundFiles.c
**
**  MBAnsi supports Windows file i/o and timing methods but also runs without them. 
**  in BAE_API_Ansi.c, #define USE_WIN_IO_AND_TIMING 0 to compile without Windows dependencies
**
**  Use the -i and -o options to specify input and output filenames on the command-line
**  input files must be either midi or rmf or wav (e.g. -i Note1.mid,  -i ASong.rmf, -i AFile.wav)
**  output file names must end with '.wav'  (e.g. output.wav)
**
**  soundbank defaults to npatches.hsb. Use the -bank option to specify a different soundbank
**
**	© Copyright 2000 Beatnik, Inc, All Rights Reserved.
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
**	Confidential-- Internal use only
**
**	Modification History:
**	11/27/00	tom  Created 
**  11/28/00    tom  add WAVE input file support
**
**
*****************************************************************************/

#include "MBAnsi.h"

// forward declarations
void parseArgs(int argc, char *argv[]);
BAEFileType GetFileType(char *fileName);
char * BAEResultToStr(BAEResult r);
BAEResult PlayMidiSong (BAEPathName fileName);
BAEResult PlayRmfSong (BAEPathName fileName);
BAEResult PlaySound (BAEPathName fileName, BAEFileType fileType);

// global variables
char patchBank[128];
char inputFile[128];
char outputFile[128];


int main(int argc, char *argv[])
{
   BAEResult err = BAE_NO_ERROR;
   int i = 0;

   // initialize global variables to default values
   sprintf(inputFile,".\\Note1.mid");
   sprintf(outputFile,".\\output.wav");
   sprintf(patchBank,".\\npatches.hsb");
 
   // get command-line arguments, if any
   parseArgs(argc, argv);   

   // play the song
   if(GetFileType(inputFile) == BAE_MIDI_TYPE)
	   err = PlayMidiSong(inputFile);
   else if(GetFileType(inputFile) == BAE_RMF)
	   err = PlayRmfSong(inputFile);
   else if(GetFileType(inputFile) == BAE_WAVE_TYPE)
	   err = PlaySound(inputFile, BAE_WAVE_TYPE);
   else if(GetFileType(inputFile) == BAE_INVALID_TYPE)
   {
		printf("invalid input file type. must be .mid, .rmf or .wav\n");
		return 0;
   }

   return 0;
}

// get arguments from the command-line
void parseArgs(int argc, char *argv[])
{
	int i;

	for(i = 0; i < argc; i++)
	{
		if( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0 )
		{
			printf("USAGE:\nMBAnsi <options>\n[-i inputFile (*.mid or *.rmf or *.wav) ]\n[-o outputFile (*.wav) ]\n[-bank soundbankFile (*.hsb) ]\n");
			exit(0);
		}
		else if( strcmp(argv[i], "-bank") == 0)  // usage: -bank patches.hsb
		{
			sprintf(patchBank,"%s",argv[++i]);
		}
		else if( strcmp(argv[i], "-i") == 0)     // usage:  -i Note1.mid
		{
			sprintf(inputFile,"%s",argv[++i]);
		}
		else if( strcmp(argv[i], "-o") == 0)     // usage: -o output.wav
		{
			sprintf(outputFile,"%s",argv[++i]);
		}
	}
}


// parse the file name to determine the file type
BAEFileType GetFileType(char *fileName)
{
  int len = strlen((char *)fileName);
  const char *pos = NULL;

  for (pos = (char *)fileName+len; pos!=(char *)fileName; pos--)
  {
	if (*pos == '.')
    {
	   pos++;
	   break;
	}
  } 
  
	if  (strcmp(pos, "mid")==0)	return BAE_MIDI_TYPE;
	else if  (strcmp(pos, "wav")==0)	return BAE_WAVE_TYPE;
	else if  (strcmp(pos, "rmf")==0)	return BAE_RMF;
	else return BAE_INVALID_TYPE;		
	
}

// convert error code to string
char * BAEResultToStr(BAEResult r)
{
	switch (r)
	{
		case BAE_NO_ERROR:					return "BAE_NO_ERROR";
		case BAE_PARAM_ERR:					return "BAE_PARAM_ERR";
		case BAE_MEMORY_ERR:				return "BAE_MEMORY_ERR";
		case BAE_BAD_INSTRUMENT:			return "BAE_BAD_INSTRUMENT";
		case BAE_BAD_MIDI_DATA:				return "BAE_BAD_MIDI_DATA";
		case BAE_ALREADY_PAUSED:			return "BAE_ALREADY_PAUSED";
		case BAE_ALREADY_RESUMED:			return "BAE_ALREADY_RESUMED";
		case BAE_DEVICE_UNAVAILABLE:		return "BAE_DEVICE_UNAVAILABLE";
		case BAE_NO_SONG_PLAYING:			return "BAE_NO_SONG_PLAYING";
		case BAE_STILL_PLAYING:				return "BAE_STILL_PLAYING";
		case BAE_TOO_MANY_SONGS_PLAYING:	return "BAE_TOO_MANY_SONGS_PLAYING";
		case BAE_NO_VOLUME:					return "BAE_NO_VOLUME";
		case BAE_GENERAL_ERR:				return "BAE_GENERAL_ERR";
		case BAE_NOT_SETUP:					return "BAE_NOT_SETUP";
		case BAE_NO_FREE_VOICES:			return "BAE_NO_FREE_VOICES";
		case BAE_STREAM_STOP_PLAY:			return "BAE_STREAM_STOP_PLAY";
		case BAE_BAD_FILE_TYPE:				return "BAE_BAD_FILE_TYPE";
		case BAE_GENERAL_BAD:				return "BAE_GENERAL_BAD";
		case BAE_BAD_FILE:					return "BAE_BAD_FILE";
		case BAE_NOT_REENTERANT:			return "BAE_NOT_REENTERANT";
		case BAE_BAD_SAMPLE:				return "BAE_BAD_SAMPLE";
		case BAE_BUFFER_TOO_SMALL:			return "BAE_BUFFER_TOO_SMALL";
		case BAE_SAMPLE_TOO_LARGE:			return "BAE_SAMPLE_TOO_LARGE";
		case BAE_NULL_OBJECT:				return "BAE_NULL_OBJECT";
		case BAE_ALREADY_EXISTS:			return "BAE_ALREADY_EXISTS";
		case BAE_BAD_BANK:					return "BAE_BAD_BANK";
		case BAE_BAD_SAMPLE_RATE:			return "BAE_BAD_SAMPLE_RATE";
		case BAE_TOO_MANY_SAMPLES:			return "BAE_TOO_MANY_SAMPLES";
		case BAE_UNSUPPORTED_FORMAT:		return "BAE_UNSUPPORTED_FORMAT";
		case BAE_FILE_IO_ERROR:				return "BAE_FILE_IO_ERROR";		
		case BAE_UNSUPPORTED_HARDWARE:		return "BAE_UNSUPPORTED_HARDWARE";
		case BAE_ABORTED:					return "BAE_ABORTED";
		case BAE_FILE_NOT_FOUND:			return "BAE_FILE_NOT_FOUND";
		case BAE_RESOURCE_NOT_FOUND:		return "BAE_RESOURCE_NOT_FOUND";		
		case BAE_ERROR_COUNT:				return "BAE_ERROR_COUNT";		
	}
	return "#Unknown Error Code#";
}


/*****************
*  PlayMidiSong
*
******************/

BAEResult PlayMidiSong (BAEPathName fileName)
{

  BAEMixer		mixer;
  BAESong		song;
  BAEResult		err			= BAE_NO_ERROR;
  BAE_BOOL		outIsDone	= 0;
  BAEBankToken	outToken	= NULL;

  // create a new mixer
  mixer = BAEMixer_New();
  if(!mixer)
  {
	 printf("BAEMixer_NEW returned NULL\n");
	 return -1;
  }

  // open the mixer with default settings
  err = BAEMixer_Open( mixer, BAE_44K, BAE_LINEAR_INTERPOLATION, BAE_USE_16,7,1,8, 1);
  if(err != BAE_NO_ERROR)
  {
	  printf("Open returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

  // add the soundbank to the mixer
  err = BAEMixer_AddBankFromFile(mixer, patchBank, &outToken);
  if(err != BAE_NO_ERROR)
  {
	  printf("AddBankFromFile returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

  // create a new song
  song = BAESong_New(mixer);
  if(!song)
  {
	 printf("BAESong_New returned NULL\n");
	 return -1;
  }


  // add the midi file to the song
  err = BAESong_LoadMidiFromFile(song, fileName, BAE_MIDI_TYPE);
  if(err != BAE_NO_ERROR)
  {
     printf("LoadMidiFromFile returned err %d: %s\n", err, BAEResultToStr(err));
	 return err;
  }
	
  // start playback
  err = BAESong_Start(song,0);
  if(err != BAE_NO_ERROR)
  {
	  printf("Start returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

 // start the output to file (ported from big BAE)
  err = BAEMixer_StartOutputToFile(mixer, outputFile, BAE_WAVE_TYPE, BAE_COMPRESSION_NONE /*BAE_COMPRESSION_MPEG_128*/);
  if(err != BAE_NO_ERROR)
  {
	  printf("StartOutputToFile returned err %d: %s\n", err, BAEResultToStr(err));
	  if(err == 10017)
		  printf("check file permissions on %s. it must not be read-only\n", outputFile);
	  return err;
  }


  // service the output to file while the song is still playing
  while(!outIsDone)
  {
	   // check if done
	  err = BAESong_IsDone(song, &outIsDone);

	  // output to file
	  // BAE_WaitMicroseconds(10000);  // not necessary
	  err = BAEMixer_ServiceAudioOutputToFile(mixer);

  }

  // stop the song
  err = BAESong_Stop(song, 0);

  // stop the output to file
  BAEMixer_StopOutputToFile();

  err = BAEMixer_Delete(mixer);

  return err;
}

/*****************
*  PlayRmfSong
*
******************/

BAEResult PlayRmfSong (BAEPathName fileName)
{

  BAEMixer		mixer;
  BAESong		song;
  BAEResult		err			= BAE_NO_ERROR;
  BAE_BOOL		outIsDone	= 0;
  BAEBankToken	outToken	= NULL;

  // create a new mixer
  mixer = BAEMixer_New();
  if(!mixer)
  {
	 printf("BAEMixer_NEW returned NULL\n");
	 return -1;
  }

  // open the mixer with default settings
  err = BAEMixer_Open( mixer, BAE_44K, BAE_LINEAR_INTERPOLATION, BAE_USE_16,7,1,8, 1);
  if(err != BAE_NO_ERROR)
  {
	  printf("Open returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

  // add the soundbank to the mixer
  err = BAEMixer_AddBankFromFile(mixer, patchBank, &outToken);
  if(err != BAE_NO_ERROR)
  {
	  printf("AddBankFromFile returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

  // create a new song
  song = BAESong_New(mixer);
  if(!song)
  {
	 printf("BAESong_New returned NULL\n");
	 return -1;
  }


  // add the rmf file to the song
  err = BAESong_LoadRmfFromFile(song, fileName, 0, TRUE);
  if(err != BAE_NO_ERROR)
  {
     printf("LoadRmfFromFile returned err %d: %s\n", err, BAEResultToStr(err));
	 return err;
  }
	
  // start playback
  err = BAESong_Start(song,0);
  if(err != BAE_NO_ERROR)
  {
	  printf("Start returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

 // start the output to file (ported from big BAE)
  err = BAEMixer_StartOutputToFile(mixer, outputFile, BAE_WAVE_TYPE, BAE_COMPRESSION_NONE /*BAE_COMPRESSION_MPEG_128*/);
  if(err != BAE_NO_ERROR)
  {
	  printf("StartOutputToFile returned err %d: %s\n", err, BAEResultToStr(err));
	  if(err == 10017)
		  printf("check file permissions on %s. it must not be read-only\n", outputFile);
	  return err;
  }


  // service the output to file while the song is still playing
  while(!outIsDone)
  {
	   // check if done
	  err = BAESong_IsDone(song, &outIsDone);

	  // output to file
	  // BAE_WaitMicroseconds(10000);  // not necessary
	  err = BAEMixer_ServiceAudioOutputToFile(mixer);

  }

  // stop the song
  err = BAESong_Stop(song, 0);

  // stop the output to file
  BAEMixer_StopOutputToFile();

  err = BAEMixer_Delete(mixer);

  return err;
}

/*****************
*  PlaySound
*
******************/

BAEResult PlaySound (BAEPathName fileName, BAEFileType fileType)
{

  BAEMixer			mixer;
  BAESound			sound;
  BAEResult			err				= BAE_NO_ERROR;
  BAE_BOOL			outIsDone		= 0;
  unsigned long		volume			= 1;
  short				priority		= 1;
  unsigned long		sampleStart		= 0;

  // create a new mixer
  mixer = BAEMixer_New();
  if(!mixer)
  {
	 printf("BAEMixer_NEW returned NULL\n");
	 return -1;
  }

  // open the mixer with default settings
  err = BAEMixer_Open( mixer, BAE_44K, BAE_LINEAR_INTERPOLATION, BAE_USE_16,7,1,8, 1);
  if(err != BAE_NO_ERROR)
  {
	  printf("Open returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

  // create a new sound
  sound = BAESound_New(mixer);
  if(!sound)
  {
	 printf("BAESong_New returned NULL\n");
	 return -1;
  }

  // load the sound from the file
  err = BAESound_LoadFileSample(sound, (BAEPathName)fileName, fileType);
  if(err != BAE_NO_ERROR)
  {
	  printf("LoadFileSample returned err: %d\n,  t");
	  return err;
  }
	
  // start playback
  err = BAESound_Start(sound, priority, LONG_TO_UNSIGNED_FIXED(volume), sampleStart);  
  if(err != BAE_NO_ERROR)
  {
	  printf("Start returned err %d: %s\n", err, BAEResultToStr(err));
	  return err;
  }

 // start the output to file (ported from big BAE)
  err = BAEMixer_StartOutputToFile(mixer, outputFile, BAE_WAVE_TYPE, BAE_COMPRESSION_NONE /*BAE_COMPRESSION_MPEG_128*/);
  if(err != BAE_NO_ERROR)
  {
	  printf("StartOutputToFile returned err %d: %s\n", err, BAEResultToStr(err));
	  if(err == 10017)
		  printf("check file permissions on %s. it must not be read-only\n", outputFile);
	  return err;
  }


  // service the output to file while the sound is still playing
  while(!outIsDone)
  {
	   // check if done
	  err = BAESound_IsDone(sound, &outIsDone);

	  // output to file
	  // BAE_WaitMicroseconds(10000);  // not necessary
	  err = BAEMixer_ServiceAudioOutputToFile(mixer);

  }

  // stop the sound
  err = BAESound_Stop(sound, 0);

  // stop the output to file
  BAEMixer_StopOutputToFile();

  err = BAEMixer_Delete(mixer);

  return err;
}

