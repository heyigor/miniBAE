/*
**  TTestMB.h
**  main header file for the TTestMB project
**  defines wrapper objects as structs 
**
**	© Copyright 2000 Beatnik, Inc, All Rights Reserved.
**	Written by Tom Lichtenberg
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
**
**
** Modification History:
**
**  9/22/00  tl     add AreMidiEventsPending to MidiSong and RmfSong
**  9/18/00  tl     add return values for all non-implemented methods
**  9/07/00  tl     add RMF Info methods
**  8/28/00  tl     add stubs for Set/GetTickPosition (Midi,Rmf)
**  7/20/00  tl     implement IsPlaying for Rmf (!IsDone())
**  7/12/00  tl		Created
**
*/


#ifndef TTEST_MB_H
#define TTEST_MB_H

#include "TTest.h"


#if MINIBAE
// to be defined in platform application file (e.g. TTest_Win.cpp)
  extern struct TTestOutputMixer    tTestOutputMixer;
  extern struct TTestSound			tTestSound;
  extern struct TTestMidiSong		tTestMidiSong;
  extern struct TTestRmfSong		tTestRmfSong;
  extern struct TTestMidiSynth		tTestMidiSynth;
	
struct TTestOutputMixer 
{
	BAEPathName		pAudioPathName;
	BAEResult		err;
	BAEMixer		theOutputMixer;

	unsigned long outLatency; 
	short int outFrame;
	unsigned long outLoad;
	long outDeviceID; 
	long outMaxDeviceCount; 
	short int outNumMidiVoices; 
	short int outMixLevel; 
	BAEQuality outQuality; 
	short int	outNumSoundVoices; 
	BAETerpMode outTerpMode;
	unsigned long outTick;
	BAE_BOOL outIsSupported;
	BAE_BOOL outIsActive;
	BAE_BOOL outIsEngaged;
	BAE_UNSIGNED_FIXED outVolume;
	BAE_BOOL outIsOpen; 
	BAEAudioModifiers outMods;

    void Open() 
		{   err = BAEMixer_Open(theOutputMixer,BAE_22K,BAE_LINEAR_INTERPOLATION,BAE_USE_16,7,1,7, TRUE); }
    void Open( BAEPathName pAudioPathName,  BAEQuality q, BAETerpMode t, BAEReverbType r, BAEAudioModifiers am, short int maxMidiVoices, short int maxSoundVoices, short int mixLevel) 
		{  err = BAEMixer_Open(theOutputMixer,q,t,am,maxMidiVoices,maxSoundVoices,mixLevel, TRUE); }
    void Close() 
		{  err = BAEMixer_Close(theOutputMixer); }
    BAE_BOOL IsOpen() 
		{  err = BAEMixer_IsOpen(theOutputMixer, &outIsOpen); return outIsOpen; }
	void ServiceIdle() 
		{  /* not in MINIBAE API */ }
	void ChangeAudioFile (BAEPathName pAudioPathName) 
		{  /* not in MINIBAE API */ }
	void SetReverbType( BAEReverbType r) 
		{  /* not in MINIBAE API */ }
	BAEResult ChangeAudioModes ( BAEQuality q, BAETerpMode t, BAEAudioModifiers am) 
		{  err = BAEMixer_ChangeAudioModes(theOutputMixer,q,t,am); return err; }
	BAEResult ChangeSystemVoices ( short int maxMidiVoices, short int maxSoundVoices, short int mixLevel) 
		{  err = BAEMixer_ChangeSystemVoices(theOutputMixer,maxMidiVoices,maxSoundVoices,mixLevel); return err; }
    void SetMasterSoundEffectsVolume(BAE_FIXED theVolume) 
		{  err = BAEMixer_SetMasterSoundEffectsVolume(theOutputMixer, (BAE_UNSIGNED_FIXED)theVolume); }
    void SetMasterVolume(BAE_FIXED theVolume) 
		{  err = BAEMixer_SetMasterVolume(theOutputMixer, (BAE_UNSIGNED_FIXED)theVolume); }
    void SetHardwareVolume(BAE_FIXED theVolume) 
		{  err = BAEMixer_SetHardwareVolume(theOutputMixer, (BAE_UNSIGNED_FIXED)theVolume); }
    BAE_FIXED GetMasterSoundEffectsVolume(void) 
		{  err = BAEMixer_GetMasterSoundEffectsVolume(theOutputMixer, &outVolume); return outVolume; }
    BAE_FIXED GetMasterVolume(void) 
		{   err = BAEMixer_GetMasterVolume(theOutputMixer, &outVolume); return outVolume; }
    BAE_FIXED GetHardwareVolume(void) 
		{   err = BAEMixer_GetHardwareVolume(theOutputMixer, &outVolume); return outVolume; }

//    BAE_EVENT_REFERENCE AddEventControlerEvent(BAEMidiSong* object,BAEControlerCallbackPtr callback,void *pReference,short int channel,short int track,short int controler,short int val) {   }
//    BAE_EVENT_REFERENCE AddEventLoadInstrument(BAEMidiSynth *object , BAE_INSTRUMENT instrument) {   }
//    BAE_EVENT_REFERENCE AddEventMetaEvent(BAEMidiSong* object,BAEMetaEventCallbackPtr callback,BAEMetaType type,void *pReference,void *pMetaText,long metaTextLength) {   }
//    BAE_EVENT_REFERENCE AddEventNoteOnWithLoad(BAEMidiSynth *object,unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) {   }
//    BAE_EVENT_REFERENCE AddEventObjectDone( BAENoise *object, BAEDoneCallbackPtr callback, void *pReference) {   }
//	  BAEResult ChangeAudioFileToMemory(void * pAudioFile, unsigned long fileSize) {   }
	void DisengageAudio(void) 
		{  err = BAEMixer_DisengageAudio(theOutputMixer); }
	unsigned long GetAudioLatency(void) 
		{  err = BAEMixer_GetAudioLatency(theOutputMixer, &outLatency); return outLatency; }
	short int GetAudioSampleFrame(short int *pLeft, short int *pRight) 
		{  err = BAEMixer_GetAudioSampleFrame(theOutputMixer,pLeft,pRight,&outFrame); return outFrame; }
	unsigned long GetCPULoadInMicroseconds(void) 
		{  err = BAEMixer_GetCPULoadInMicroseconds(theOutputMixer,&outLoad); return outLoad; }
	unsigned long GetCPULoadInPercent(void) 
		{  err = BAEMixer_GetCPULoadInPercent(theOutputMixer, &outLoad); return outLoad; }
	BAECPUType GetCPUType(void) 
		{  /* not in MINIBAE API */ return (BAECPUType)0; }
	BAE_BOOL GetCacheStatusForAudioFile(void) 
		{  /* not in MINIBAE API */ return false; }
	void GetChannelMuteStatus( BAE_BOOL *pChannels) 
		{  /* not in MINIBAE API ... only for BAESong */  }
	void GetChannelSoloStatus(BAE_BOOL *pChannels) 
		{  /* not in MINIBAE API ... only for BAESong */ }
	long GetCurrentDevice(void *deviceParameter) 
		{  return BAEMixer_GetCurrentDevice(theOutputMixer, deviceParameter, &outDeviceID); }
	void GetDeviceName(long deviceID, char *cName, unsigned long cNameLength) 
		{  err = BAEMixer_GetDeviceName(theOutputMixer, deviceID, cName, cNameLength); }
	BAE_FIXED GetHardwareBalance(void) 
		{  /* not in MINIBAE API */ return LONG_TO_FIXED(0); }
	void GetInstrumentNameFromAudioFile(char *cInstrumentName, long *pID) 
		{  /* not in MINIBAE API */ }
    void GetInstrumentNameFromAudioFileFromID( char *cInstrumentName, long theID) 
		{  /* not in MINIBAE API */ }
	long GetMaxDeviceCount(void) 
		{  err = BAEMixer_GetMaxDeviceCount(theOutputMixer, &outMaxDeviceCount); return outMaxDeviceCount; }
	short int GetMidiVoices(void) 
		{  err = BAEMixer_GetMidiVoices(theOutputMixer, &outNumMidiVoices); return outNumMidiVoices; }
	short int GetMixLevel(void) 
		{  return BAEMixer_GetMixLevel(theOutputMixer, &outMixLevel); }
    void GetMixerVersion(short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor) 
		{  err = BAEMixer_GetMixerVersion(theOutputMixer,pVersionMajor, pVersionMinor, pVersionSubMinor); }
	BAEAudioModifiers GetModifiers(void) 
		{  err = BAEMixer_GetModifiers(theOutputMixer, &outMods); return outMods; }
	BAEResult GetNameFromAudioFile(BAEPathName pAudioPathName, char *cName, unsigned long cLength) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAEResult GetNameFromAudioFile(char *cName,unsigned long cLength) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAEQuality GetQuality(void) 
		{ err = BAEMixer_GetQuality(theOutputMixer, &outQuality); return outQuality;  }
	void GetRealtimeStatus(BAEAudioInfo *pStatus) 
		{  err = BAEMixer_GetRealtimeStatus(theOutputMixer, pStatus); }
	BAEReverbType GetReverbType(void) 
		{  /* not in MINIBAE API */ return (BAEReverbType)0; }
	void GetSampleNameFromAudioFile(char *cSampleName,  long *pID) 
		{  /* not in MINIBAE API */ }
	void GetSongNameFromAudioFile(char *cSongName,  long *pID, BAEFileType *songType) 
		{  /* not in MINIBAE API */ }
    short int GetSoundVoices(void) 
		{ err = BAEMixer_GetSoundVoices(theOutputMixer, &outNumSoundVoices); return outNumSoundVoices; }
//	BAETaskCallbackPtr GetTaskCallback(void) {   }
	void * GetTaskReference(void) 
		{  /* not in MINIBAE API */ return NULL;}
	BAETerpMode GetTerpMode(void) 
		{  err = BAEMixer_GetTerpMode(theOutputMixer,&outTerpMode); return outTerpMode; }
	unsigned long GetTick(void) 
		{  err = BAEMixer_GetTick(theOutputMixer,&outTick); return outTick; }
	BAEResult GetURLFromAudioFile(char *pURL, unsigned long urlLength) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	void GetVersionFromAudioFile( short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor) 
		{  /* not in MINIBAE API */ }
	BAEResult GetVersionFromAudioFile(BAEPathName pAudioPathName,short int *pVersionMajor, short int *pVersionMinor, short int *pVersionSubMinor) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAE_BOOL Is16BitSupported(void) 
		{  err = BAEMixer_Is16BitSupported(theOutputMixer,&outIsSupported); return outIsSupported; } 
	BAE_BOOL Is8BitSupported(void) 
		{  err = BAEMixer_Is8BitSupported(theOutputMixer,&outIsSupported); return outIsSupported; }
	BAE_BOOL IsAudioActive(void) 
		{  err = BAEMixer_IsAudioActive(theOutputMixer,&outIsActive); return outIsActive; }
	BAE_BOOL IsAudioEngaged(void) 
		{  err = BAEMixer_IsAudioEngaged(theOutputMixer,&outIsEngaged); return outIsEngaged; }
	BAE_BOOL IsStereoSupported(void) 
		{  /* not in MINIBAE API */ return false;  } 
	unsigned long MeasureCPUPerformance(void) 
		{  /* not in MINIBAE API */ return 0;  }
	void PerformanceConfigure(void) 
		{  /* not in MINIBAE API */ }
	void ReengageAudio(void) 
		{  err =BAEMixer_ReengageAudio(theOutputMixer); }
	void RemoveEvent(BAE_EVENT_REFERENCE reference) 
		{   /* not in MINIBAE API */ }
	BAEResult SetAudioLatency(unsigned long requestedLatency) 
		{  err = BAEMixer_SetAudioLatency(theOutputMixer, requestedLatency); return err; }
	void SetCacheStatusForAudioFile(BAE_BOOL enableCache) 
		{  /* not in MINIBAE API */  }
    void SetCurrentDevice(long deviceID ,void *deviceParameter) 
		{  err = BAEMixer_SetCurrentDevice(theOutputMixer, deviceID, deviceParameter); }
	void SetHardwareBalance(BAE_FIXED balance) {   }
//	void SetOutputCallback(BAEOutputCallbackPtr pCallback) {   }
//	void SetTaskCallback(BAETaskCallbackPtr pCallback, void *pReference) {   }
//	void StartOutputCallback(void) {   }
	BAEResult StartOutputToFile(BAEPathName pAudioOutputFile) 
		{ /* not in MINIBAE API */ return BAE_GENERAL_ERR;  }
//	void StartTaskCallback(void) {   }
//	void StopOutputCallback(void) {   }
	void StopOutputToFile(void) 
		{  /* not in MINIBAE API */ }
//	void StopTaskCallback(void) {   } 
    BAEResult ValidateAudioFile(BAEPathName pAudioPathName) 
		{ /* not in MINIBAE API */  return BAE_GENERAL_ERR; }

// MiniBAE-only methods
	BAEResult BringBankToFront(BAEMixer mixer, BAEBankToken token)
		{ err = BAEMixer_BringBankToFront( theOutputMixer, token); return err; }
	BAEResult SendBankToBack(BAEMixer mixer, BAEBankToken token)
		{ err =	BAEMixer_SendBankToBack(theOutputMixer, token);  return err; }
	BAEResult GetBankVersion(BAEMixer mixer,	BAEBankToken token,	short int *pVersionMajor,short int *pVersionMinor,short int *pVersionSubMinor)
		{ err =	BAEMixer_GetBankVersion(theOutputMixer,	token,	pVersionMajor,pVersionMinor,pVersionSubMinor);  return err;}
	BAEResult GetGroovoidNameFromBank(BAEMixer mixer,long index, char *cSongName)
		{ err =	BAEMixer_GetGroovoidNameFromBank(theOutputMixer, index, cSongName); return err; }
} ;


// Streaming not yet supported by MiniBAE: stub class only
struct TTestSoundStream
{
    BAEResult	err;
    BAESound	theSoundStream;

	BAE_BOOL	outIsDone;
	BAE_BOOL    outIsPaused;
	short int outResonanceAmount;
	short int outFrequencyAmount;
	short int outLowPassAmount;
	BAE_UNSIGNED_FIXED outVolume;
	BAE_UNSIGNED_FIXED outRate;

	BAEResult LoadFileSample( BAEPathName pAudioPathName, BAEFileType fileType ) 
		{ err = BAESound_LoadFileSample(theSoundStream, pAudioPathName, fileType); return err; }
	BAEResult Start() 
		{ err = BAESound_Start(theSoundStream,0, BAE_FIXED_1 , 0);  return err;} // BAE_FIXED_1 should be this->GetVolume
	BAEResult Start(short int priority, BAE_UNSIGNED_FIXED volume, unsigned long startLoop) 
		{ err = BAESound_Start(theSoundStream, priority, volume , startLoop);  return err;} 
	BAE_BOOL IsPlaying() 
		{ err = BAESound_IsDone(theSoundStream, &outIsDone); return !outIsDone; }
	void SetRate(unsigned long rate) 
		{ err = BAESound_SetRate( theSoundStream, rate);  }
	BAE_UNSIGNED_FIXED GetRate() 
		{  err = BAESound_GetRate(theSoundStream, &outRate); return outRate; }
	void SetStereoPosition(short int position) 
		{  /* not in MINIBAE API */ }
	short int GetStereoPosition(void) 
		{  /* not in MINIBAE API */ return 0; }
	void SetReverbAmount(short int amount) 
		{  /* not in MINIBAE API */ }
	short int GetReverbAmount(void) 
		{  /* not in MINIBAE API */ return 0;}
	void SetFrequencyAmountFilter(short int amount) 
		{  err = BAESound_SetFrequencyAmountFilter(theSoundStream, amount); }
	short int GetFrequencyAmountFilter(void) 
		{ err =  BAESound_GetFrequencyAmountFilter(theSoundStream, &outFrequencyAmount); return outFrequencyAmount; }
	void SetLowPassAmountFilter(short int amount) 
		{  err = BAESound_SetLowPassAmountFilter(theSoundStream, amount); }
	short int GetLowPassAmountFilter(void) 
		{  err = BAESound_GetLowPassAmountFilter(theSoundStream, &outLowPassAmount); return outLowPassAmount; }
	void SetResonanceAmountFilter(short int amount) 
		{  err = BAESound_SetResonanceAmountFilter(theSoundStream,amount); }
	short int GetResonanceAmountFilter(void) 
		{  err = BAESound_GetResonanceAmountFilter(theSoundStream, &outResonanceAmount); return outResonanceAmount; }
	void SetVolume(BAE_FIXED theVolume) 
		{  err = BAESound_SetVolume(theSoundStream, (BAE_UNSIGNED_FIXED)theVolume); }
	BAE_FIXED GetVolume(void) 
		{  err = BAESound_GetVolume(theSoundStream, &outVolume); return outVolume; }
	void Fade(BAE_BOOL doAsync) 
		{  err = BAESound_Fade(theSoundStream, BAESound_GetVolume(theSoundStream, &outVolume),  0, 2200); }
	void FadeTo(BAE_FIXED destVol, BAE_BOOL doAsync) 
		{  err = BAESound_Fade(theSoundStream, BAESound_GetVolume(theSoundStream, &outVolume),  destVol, 2200); }
	void FadeFromToInTime(BAE_FIXED srcVol, BAE_FIXED destVol, BAE_FIXED timeInMillis) 
		{  err = BAESound_Fade(theSoundStream, srcVol, destVol, timeInMillis); }
//	BAEResult AddSampleFrameCallback(unsigned long frame, BAESampleFrameCallbackPtr pCallback, void *pReference) {   }
	void DefaultSampleDoneCallback(void) 
		{  /* not in MINIBAE API */ }
//	BAEDoneCallbackPtr GetDoneCallback(void) {   } 
	void * GetDoneCallbackReference(void) 
		{  /* not in MINIBAE API */ return NULL; } 
	BAEResult GetInfo(BAESampleInfo *info) 
		{  err = BAESound_GetInfo(theSoundStream, info); return err; }
//	BAELoopDoneCallbackPtr GetLoopDoneCallback(void) {   } 
	unsigned long GetPlaybackPosition(void) 
		{  /* not in MINIBAE API */ return 0L; }
	void *GetReference(void) 
		{  /* not in MINIBAE API */ return NULL; }
	BAE_BOOL GetReverb(void) 
		{  /* not in MINIBAE API */ return false; }
	BAEResult GetSampleLoopPoints(unsigned long *pStart, unsigned long *pEnd) 
		{  err = BAESound_GetSampleLoopPoints(theSoundStream, pStart, pEnd); return err; }
	void * GetSamplePointer(unsigned long sampleFrame) 
		{  /* not in MINIBAE API */ return NULL; }
	void * GetSamplePointerFromMixer(void) 
		{  /* not in MINIBAE API */ return NULL; }
	BAE_BOOL IsDone(void) 
		{   err = BAESound_IsDone(theSoundStream, &outIsDone); return outIsDone; }
	BAE_BOOL IsPaused(void) 
		{  err = BAESound_IsPaused(theSoundStream, &outIsPaused); return outIsPaused; }
	BAEResult LoadBankSample(char * cName) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAEResult LoadBankSampleByID(unsigned long id) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAEResult LoadCustomSample( void *sampleData, unsigned long frames, unsigned short int bitSize, unsigned short int channels, BAE_FIXED rate, unsigned long loopStart, unsigned long loopEnd) 
		{  err = BAESound_LoadCustomSample(theSoundStream, sampleData, frames,bitSize,channels, rate, loopStart,loopEnd);  return err;}
	BAEResult LoadMemorySample( void *pMemoryFile, unsigned long memoryFileSize, BAEFileType fileType) 
		{  err = BAESound_LoadMemorySample(theSoundStream, pMemoryFile, memoryFileSize, fileType); return err;}
	BAEResult LoadResourceSample( void *pResource, unsigned long resourceSize) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
    void Pause(void) 
		{  err = BAESound_Pause(theSoundStream); }
//  BAEResult PreStart(BAE_UNSIGNED_FIXED sampleVolume, short int position, void *refData, BAELoopDoneCallbackPtr pLoopContinueProc, BAEDoneCallbackPtr pDoneProc, BAE_BOOL stopIfPlaying) {   }
//	BAEResult RemoveSampleFrameCallback( unsigned long frame, BAESampleFrameCallbackPtr pCallback) {   }
	void Resume(void) 
		{  err = BAESound_Resume(theSoundStream); }
	BAEResult SaveFile(BAEPathName pFile,BAEFileType fileType) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
//	void SetDoneCallback( BAEDoneCallbackPtr pDoneProc, void *pReference) {   }
	void SetReverb(BAE_BOOL useReverb) 
		{  /* not in MINIBAE API */ }
	BAEResult SetSampleLoopPoints( unsigned long start, unsigned long end) 
		{  err = BAESound_SetSampleLoopPoints(theSoundStream,start,end); return err; }
//	BAEResult StartCustomSample(void * sampleData, unsigned long frames, unsigned short int bitSize, unsigned short int channels, BAE_FIXED rate, unsigned long loopStart, unsigned long loopEnd, BAE_FIXED sampleVolume, short int stereoPosition, void * refData, BAELoopDoneCallbackPtr pLoopContinueProc, BAEDoneCallbackPtr pDoneProc, BAE_BOOL stopIfPlaying) {   }
//    BAEResult StartDoubleBuffer(void* buffer1,void* buffer2,unsigned long frames,unsigned short int bitSize,unsigned short int channels,BAE_UNSIGNED_FIXED rate,BAE_UNSIGNED_FIXED sampleVolume,short int stereoPosition,void* refData, BAEDoubleBufferCallbackPtr pDoubleBufferCallback, BAE_BOOL stopIfPlaying) {   }
	void Stop(BAE_BOOL startFade) 
		{  err = BAESound_Stop(theSoundStream, startFade); }  

// MiniBAE-only methods
	BAEResult GetInfo(BAESound sound, BAESampleInfo *outInfo)
		{ err = BAESound_GetInfo(theSoundStream, outInfo); return err; }

};

struct TTestSound
{
    BAEResult	err;
    BAESound	theSound;

	BAE_BOOL	outIsDone;
	BAE_BOOL	outIsPaused;
	short int outResonanceAmount;
	short int outFrequencyAmount;
	short int outLowPassAmount;
	BAE_UNSIGNED_FIXED outVolume;
	BAE_UNSIGNED_FIXED outRate;

	BAEResult LoadFileSample( BAEPathName pAudioPathName, BAEFileType fileType ) 
		{ err = BAESound_LoadFileSample(theSound, pAudioPathName, fileType); return err; }
	BAEResult Start() 
		{ err = BAESound_Start(theSound,0, BAE_FIXED_1 , 0);  return err; }  // should return GetVolume()
	BAEResult Start(short int priority, BAE_UNSIGNED_FIXED volume, unsigned long startLoop) 
		{ err = BAESound_Start(theSound, priority, volume , startLoop);  return err;} 
	BAE_BOOL IsPlaying() 
		{ err = BAESound_IsDone(theSound, &outIsDone);  return !outIsDone; }
	void SetRate(unsigned long rate) 
		{ err = BAESound_SetRate( theSound, rate);  }
	BAE_UNSIGNED_FIXED GetRate() 
		{  err = BAESound_GetRate(theSound, &outRate); return outRate; }
	void SetStereoPosition(short int position) 
		{  /* not in MINIBAE API */ }
	short int GetStereoPosition(void) 
		{  /* not in MINIBAE API */ return 0; }
	void SetReverbAmount(short int amount) 
		{  /* not in MINIBAE API */ }
	short int GetReverbAmount(void) 
		{  /* not in MINIBAE API */ return 0;}
	void SetFrequencyAmountFilter(short int amount) 
		{  err = BAESound_SetFrequencyAmountFilter(theSound, amount); }
	short int GetFrequencyAmountFilter(void) 
		{  err = BAESound_GetFrequencyAmountFilter(theSound,&outFrequencyAmount); return outFrequencyAmount; }
	void SetLowPassAmountFilter(short int amount) 
		{  err = BAESound_SetLowPassAmountFilter(theSound, amount); }
	short int GetLowPassAmountFilter(void) 
		{  err = BAESound_GetLowPassAmountFilter(theSound, &outLowPassAmount); return outLowPassAmount; }
	void SetResonanceAmountFilter(short int amount) 
		{  err = BAESound_SetResonanceAmountFilter(theSound,amount); }
	short int GetResonanceAmountFilter(void) 
		{  err = BAESound_GetResonanceAmountFilter(theSound, &outResonanceAmount); return outResonanceAmount; }
	void SetVolume(BAE_FIXED theVolume) 
		{  err = BAESound_SetVolume(theSound, (BAE_UNSIGNED_FIXED)theVolume); }
	BAE_FIXED GetVolume(void) 
		{  err = BAESound_GetVolume(theSound, &outVolume); return outVolume;}
	void Fade(BAE_BOOL doAsync) 
		{  err = BAESound_Fade(theSound, BAESound_GetVolume(theSound, &outVolume),  0, 2200); }
	void FadeTo(BAE_FIXED destVol, BAE_BOOL doAsync) 
		{  err = BAESound_Fade(theSound, BAESound_GetVolume(theSound, &outVolume),  destVol, 2200);  }
	void FadeFromToInTime(BAE_FIXED srcVol, BAE_FIXED destVol, BAE_FIXED timeInMillis) 
		{  err = BAESound_Fade(theSound, srcVol, destVol, timeInMillis); }
//	BAEResult AddSampleFrameCallback(unsigned long frame, BAESampleFrameCallbackPtr pCallback, void *pReference) {   }
	void DefaultSampleDoneCallback(void) 
		{  /* not in MINIBAE API */ }
//	BAEDoneCallbackPtr GetDoneCallback(void) {   } 
	void * GetDoneCallbackReference(void) 
		{  /* not in MINIBAE API */ return NULL; } 
	BAEResult GetInfo(BAESampleInfo *info) 
		{  return BAESound_GetInfo(theSound, info); }
//	BAELoopDoneCallbackPtr GetLoopDoneCallback(void) {   } 
	unsigned long GetPlaybackPosition(void) 
		{  /* not in MINIBAE API */ return 0L;}
	void *GetReference(void) 
		{  /* not in MINIBAE API */ return NULL; }
	BAE_BOOL GetReverb(void) 
		{  /* not in MINIBAE API */ return false; }
	BAEResult GetSampleLoopPoints(unsigned long *pStart, unsigned long *pEnd) 
		{  err = BAESound_GetSampleLoopPoints(theSound, pStart, pEnd); return err; }
	void * GetSamplePointer(unsigned long sampleFrame) 
		{  /* not in MINIBAE API */ return NULL; }
	void * GetSamplePointerFromMixer(void) 
		{  /* not in MINIBAE API */ return NULL; }
	BAE_BOOL IsDone(void) 
		{   err = BAESound_IsDone(theSound, &outIsDone); return outIsDone; }
	BAE_BOOL IsPaused(void) 
		{  err = BAESound_IsPaused(theSound, &outIsPaused); return outIsPaused; }
	BAEResult LoadBankSample(char * cName) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAEResult LoadBankSampleByID(unsigned long id) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
	BAEResult LoadCustomSample( void *sampleData, unsigned long frames, unsigned short int bitSize, unsigned short int channels, BAE_FIXED rate, unsigned long loopStart, unsigned long loopEnd) 
		{  err = BAESound_LoadCustomSample(theSound, sampleData, frames,bitSize,channels, rate, loopStart,loopEnd); return err; }
	BAEResult LoadMemorySample( void *pMemoryFile, unsigned long memoryFileSize, BAEFileType fileType) 
		{  err = BAESound_LoadMemorySample(theSound, pMemoryFile, memoryFileSize, fileType); return err; }
	BAEResult LoadResourceSample( void *pResource, unsigned long resourceSize) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
    void Pause(void) 
		{  err = BAESound_Pause(theSound); }
// BAEResult PreStart(BAE_UNSIGNED_FIXED sampleVolume, short int position, void *refData, BAELoopDoneCallbackPtr pLoopContinueProc, BAEDoneCallbackPtr pDoneProc, BAE_BOOL stopIfPlaying) {   }
//	BAEResult RemoveSampleFrameCallback( unsigned long frame, BAESampleFrameCallbackPtr pCallback) {   }
	void Resume(void) 
		{  err = BAESound_Resume(theSound); }
	BAEResult SaveFile(BAEPathName pFile,BAEFileType fileType) 
		{  /* not in MINIBAE API */ return BAE_GENERAL_ERR; }
//	void SetDoneCallback( BAEDoneCallbackPtr pDoneProc, void *pReference) {   }
	void SetReverb(BAE_BOOL useReverb) 
		{  /* not in MINIBAE API */ }
	BAEResult SetSampleLoopPoints( unsigned long start, unsigned long end) 
		{  err = BAESound_SetSampleLoopPoints(theSound,start,end); return err; }
//	BAEResult StartCustomSample(void * sampleData, unsigned long frames, unsigned short int bitSize, unsigned short int channels, BAE_FIXED rate, unsigned long loopStart, unsigned long loopEnd, BAE_FIXED sampleVolume, short int stereoPosition, void * refData, BAELoopDoneCallbackPtr pLoopContinueProc, BAEDoneCallbackPtr pDoneProc, BAE_BOOL stopIfPlaying) {   }
//    BAEResult StartDoubleBuffer(void* buffer1,void* buffer2,unsigned long frames,unsigned short int bitSize,unsigned short int channels,BAE_UNSIGNED_FIXED rate,BAE_UNSIGNED_FIXED sampleVolume,short int stereoPosition,void* refData, BAEDoubleBufferCallbackPtr pDoubleBufferCallback, BAE_BOOL stopIfPlaying) {   }
	void Stop(BAE_BOOL startFade) 
		{  err = BAESound_Stop(theSound, startFade); }  

// MiniBAE-only methods
	BAEResult GetInfo(BAESound sound, BAESampleInfo *outInfo)
		{ err = BAESound_GetInfo(theSound, outInfo); return err; }
} ;


struct TTestMidiSynth
{

	 BAEResult		err;
	 BAESong   		theMidiSynth;

	 BAE_BOOL outChannels[65];
	 BAE_BOOL theChannelStatus;
	 BAE_BOOL outPending;
	 BAE_BOOL outAllowTranspose;
	 char *outValue;
	 long outSemitones;
	 BAE_UNSIGNED_FIXED outVolume;
	 BAE_BOOL outIsLoaded;

	 void MuteChannel(short int channel) 
		{  err = BAESong_MuteChannel(theMidiSynth, channel); }
	 void UnmuteChannel(short int channel) 
		{  err = BAESong_UnmuteChannel(theMidiSynth, channel); }
	 BAE_BOOL GetChannelMuteStatus(short int channel) 
		{  err = BAESong_GetChannelMuteStatus(theMidiSynth, outChannels); theChannelStatus = outChannels[channel]; return theChannelStatus; }
	 void GetChannelMuteStatus(BAE_BOOL *pChannels) 
		{  err = BAESong_GetChannelMuteStatus(theMidiSynth, pChannels);  }
	 void SoloChannel(short int channel) 
		{  err = BAESong_SoloChannel(theMidiSynth, channel); }
	 void UnSoloChannel(short int channel) 
		{  err = BAESong_UnSoloChannel(theMidiSynth, channel); }
	 BAE_BOOL GetChannelSoloStatus(short int channel) 
		{  err = BAESong_GetChannelSoloStatus(theMidiSynth, outChannels); theChannelStatus = outChannels[channel]; return theChannelStatus;  }
	 void GetChannelSoloStatus(BAE_BOOL *pChannels) 
		{  err = BAESong_GetChannelSoloStatus(theMidiSynth, pChannels);  }
	void SetQueue(BAE_BOOL queue) 
		{  /* not in MiniBAE API */ }
	BAE_BOOL GetQueue() 
		{  /* not in MiniBAE API */ return false; }
	void AllNotesOff(unsigned long time) 
		{  err = BAESong_AllNotesOff(theMidiSynth, time); }
	void AllowChannelPitchOffset(unsigned short int channel ,BAE_BOOL allowPitch) 
		{  err = BAESong_AllowChannelTranspose(theMidiSynth, channel, allowPitch); }
	BAE_BOOL AreMidiEventsPending(void) 
		{  err = BAESong_AreMidiEventsPending(theMidiSynth, &outPending); return outPending; } 
	void ChannelPressure(unsigned char channel, unsigned char pressure, unsigned long time) 
		{  err = BAESong_ChannelPressure(theMidiSynth, channel, pressure, time); } 
	void Close(void) 
		{ /* not in MiniBAE API */  }
	void ControlChange(unsigned char channel ,unsigned char controlNumber,unsigned char controlValue,unsigned long time) 
		{  err = BAESong_ControlChange(theMidiSynth, channel, controlNumber, controlValue, time); }
	BAEResult CreateInstrumentAsData(BAE_INSTRUMENT instrument, void **pData, unsigned long *pDataSize) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR; }
	BAE_BOOL DoesChannelAllowPitchOffset(unsigned short int channel) 
		{  err = BAESong_DoesChannelAllowTranspose(theMidiSynth, channel, &outAllowTranspose); return outAllowTranspose; }
	char GetControlValue(unsigned char channel, unsigned char controller) 
		{  err = BAESong_GetControlValue(theMidiSynth, channel, controller, outValue); return outValue[0]; } 
	void GetPitchBend(unsigned int channel,unsigned char *pLSB, unsigned char *pMSB) 
		{  err = BAESong_GetPitchBend(theMidiSynth, channel, pLSB, pMSB); } 
	long GetPitchOffset(void) 
		{  err = BAESong_GetTranspose(theMidiSynth, &outSemitones); return outSemitones; }
	short int GetPriority(void) 
		{  /* not in MiniBAE API */ return 0; }
	void GetProgramBank(unsigned char channel,unsigned char *pProgram,unsigned char *pBank) 
		{  err = BAESong_GetProgramBank(theMidiSynth, channel, pProgram, pBank); }
	void * GetReference(void) 
		{  /* not in MiniBAE API */ return NULL; }
	void * GetSongVariables(void) 
		{  /* not in MiniBAE API */ return NULL; } 
	short int GetStereoPosition(void) 
		{  /* not in MiniBAE API */ return 0; } 
	unsigned long GetTick(void) 
		{  /* not in MiniBAE API ... in BAEMixer only */ return 0L; }
	BAE_FIXED GetVolume(void) 
		{  err = BAESong_GetVolume(theMidiSynth, &outVolume); return outVolume; }  
    void InstrumentChange(unsigned char channel, BAE_INSTRUMENT instrument, unsigned long time) 
		{  /* not in MiniBAE API */ }
	BAE_BOOL IsInstrumentLoaded(BAE_INSTRUMENT instrument) 
		{  err = BAESong_IsInstrumentLoaded(theMidiSynth, instrument, &outIsLoaded); return outIsLoaded;  }  
	BAEResult IsInstrumentRemapped(BAE_INSTRUMENT instrument, BAE_BOOL *pOutRemapped, BAE_INSTRUMENT *pOutToInstrument) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR;  } 
	BAE_BOOL IsLoaded(void) 
		{  /* not in MiniBAE API */ return false; } 
	void KeyPressure(unsigned char channel, unsigned char note, unsigned char pressure, unsigned long time) 
		{  err = BAESong_KeyPressure(theMidiSynth, channel, note, pressure, time); }
	BAEResult LoadInstrument(BAE_INSTRUMENT instrument) 
		{  err = BAESong_LoadInstrument(theMidiSynth, instrument); return err;}
	BAEResult LoadInstrumentFromData(BAE_INSTRUMENT instrument, void *data,unsigned long dataSize) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR; }
	void LockQueue(void) 
		{  /* not in MiniBAE API */ }
	void NoteOff(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOff(theMidiSynth, channel, note, velocity, time); }
	void NoteOn(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOn(theMidiSynth, channel, note, velocity, time); }
	void NoteOnWithLoad(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOnWithLoad(theMidiSynth, channel, note, velocity, time); }
	BAEResult Open(BAE_BOOL loadInstruments) 
		{  /* not in MiniBAE API ...in BAEMixer only */ return BAE_GENERAL_ERR; }
	void PanicNotesOff(unsigned long time) 
		{  /* not in MiniBAE API */ }
	void ParseMidiData(unsigned char commandByte,unsigned char data1Byte,unsigned char data2Byte,unsigned char data3Byte, unsigned long time) 
		{  err = BAESong_ParseMidiData(theMidiSynth, commandByte, data1Byte, data2Byte, data3Byte, time); }
	void PitchBend(unsigned char channel ,unsigned char lsb ,unsigned char msb,unsigned long time) 
		{  err = BAESong_PitchBend(theMidiSynth, channel, lsb, msb, time); }
	void ProgramBankChange(unsigned char channel,unsigned char programNumber,unsigned char bankNumber,unsigned long time) 
		{  err = BAESong_ProgramBankChange(theMidiSynth, channel, programNumber, bankNumber, time); }
	void ProgramChange(unsigned char channel ,unsigned char programNumber,unsigned long time) 
		{  err = BAESong_ProgramChange(theMidiSynth, channel, programNumber, time); }
	BAEResult RemapInstrument(BAE_INSTRUMENT from , BAE_INSTRUMENT to) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR;  }
	void SetCacheSample(BAE_BOOL cacheSamples) 
		{  /* not in MiniBAE API */ }
	void SetPitchOffset(long offset) 
		{  BAESong_SetTranspose(theMidiSynth, offset); }
	void SetPriority(short int priority) 
		{  /* not in MiniBAE API */ }
	void SetStereoPosition(short int stereoPosition) 
		{  /* not in MiniBAE API */ } 
	void SetVolume(BAE_FIXED newVolume) 
		{  err = BAESong_SetVolume(theMidiSynth, newVolume); }
	BAE_INSTRUMENT TranslateBankProgramToInstrument(unsigned short bank, unsigned short program, unsigned short channel, unsigned short note) 
		{ BAE_INSTRUMENT n = TranslateBankProgramToInstrument(bank, program, channel, note); return n; }
	BAEResult TranslateInstrumentToBankProgram(BAE_INSTRUMENT instrument,unsigned short *pOutBank,unsigned short *pOutProgram,unsigned short *pOutChannel,unsigned short *pOutNote) 
		{ /* not in MiniBAE API */ return BAE_GENERAL_ERR;  }
	BAEResult UnloadInstrument(BAE_INSTRUMENT instrument) 
		{  err = BAESong_UnloadInstrument(theMidiSynth,instrument); return err; }
	void UnlockQueue(void) 
		{  /* not in MiniBAE API */ }

// MiniBAE-only methods
	BAEResult Delete(BAESong song)
		{ err =  BAESong_Delete(theMidiSynth); return err; }		
	BAEResult LoadGroovoid(BAESong song,	char *cName, BAE_BOOL ignoreBadInstruments)
		{ err =  BAESong_LoadGroovoid(theMidiSynth,	cName,  ignoreBadInstruments); return err; }
	BAEResult Preroll(BAESong song)
		{ err =  BAESong_Preroll(theMidiSynth); return err; }
} ;


struct TTestMidiSong
{
     BAEResult		err;
	 BAESong		theMidiSong;

	 BAE_BOOL outPending;
	 BAE_BOOL outChannels[65];
	 BAE_BOOL theChannelStatus;
	 BAE_BOOL outTracks[65];
	 BAE_BOOL theTrackStatus;
	 short outNumLoops;
	 BAE_UNSIGNED_FIXED outTempoFactor;
	 BAE_UNSIGNED_FIXED outVolume;
	 BAE_BOOL outAllowTranspose;
	 char *outValue;
	 unsigned long outTicks;
	 unsigned long outLength;
	 long outSemitones;
	 BAE_BOOL outIsDone;
	 BAE_BOOL outIsLoaded;
	 BAE_BOOL outIsPaused;

	 void MuteChannel(short int channel) 
		{  err = BAESong_MuteChannel(theMidiSong, channel); }
	 void UnmuteChannel(short int channel) 
		{  err = BAESong_UnmuteChannel(theMidiSong, channel); }
	 BAE_BOOL GetChannelMuteStatus(short int channel) 
		{  err = BAESong_GetChannelMuteStatus(theMidiSong, outChannels); theChannelStatus = outChannels[channel]; return theChannelStatus; }
	 void GetChannelMuteStatus(BAE_BOOL *pChannels) 
		{  err = BAESong_GetChannelMuteStatus(theMidiSong, pChannels);  }
	 void SoloChannel(short int channel) 
		{  err = BAESong_SoloChannel(theMidiSong, channel); }
	 void UnSoloChannel(short int channel) 
		{  err = BAESong_UnSoloChannel(theMidiSong, channel); }
	 BAE_BOOL GetChannelSoloStatus(short int channel) 
		{  err = BAESong_GetChannelSoloStatus(theMidiSong, outChannels); theChannelStatus = outChannels[channel]; return theChannelStatus;}
	 void GetChannelSoloStatus(BAE_BOOL *pChannels) 
		{  err = BAESong_GetChannelSoloStatus(theMidiSong, pChannels);  }
	 void MuteTrack(short int track) 
		{  err = BAESong_MuteTrack(theMidiSong, track); }
	 void UnmuteTrack(short int track) 
		{  err = BAESong_UnmuteTrack(theMidiSong, track); }
	 BAE_BOOL GetTrackMuteStatus(short int track) 
		{  err = BAESong_GetTrackMuteStatus(theMidiSong, outTracks); theTrackStatus = outTracks[track];return theTrackStatus; }
	 void GetTrackMuteStatus(BAE_BOOL *pTracks) 
		{  err = BAESong_GetTrackMuteStatus(theMidiSong, pTracks); }
	 void SoloTrack(short int track) 
		{  err = BAESong_SoloTrack(theMidiSong, track); }
	 void UnSoloTrack(short int track) 
		{  err = BAESong_UnSoloTrack(theMidiSong, track); }
	 BAE_BOOL GetSoloTrackStatus(short int track) 
		{  err = BAESong_GetSoloTrackStatus(theMidiSong, outTracks); theTrackStatus = outTracks[track]; return theTrackStatus;}
	 void GetSoloTrackStatus(BAE_BOOL *pTracks) 
		{  err = BAESong_GetSoloTrackStatus(theMidiSong, pTracks);   }
	 void SetLoopFlag(BAE_BOOL loop) 
		{  /* not in MiniBAE API */ }
	 BAE_BOOL GetLoopFlag(void) 
		{   /* not in MiniBAE API */ return false; }
	 void SetLoopMax(short int max) 
		{  err = BAESong_SetLoops(theMidiSong, max); }
	 short int GetLoopMax() 
		{  err = BAESong_GetLoops(theMidiSong, &outNumLoops); return outNumLoops; }
	 void SetTempo(short int tempo) 
		{  /* not in MiniBAE API */ }
     short int GetTempo(void) 
		{  /* not in MiniBAE API */ return 0; }
	 void SetTempoInBeatsPerMinute(short int tempo) 
		{  /* not in MiniBAE API */ }
	 short int GetTempoInBeatsPerMinute(void) 
		{  /* not in MiniBAE API */ return 0; }
	 void SetMasterTempo(BAE_FIXED masterTempo) 
		{  err = BAESong_SetMasterTempo(theMidiSong, (BAE_UNSIGNED_FIXED)masterTempo); }
	 BAE_FIXED GetMasterTempo(void) 
		{  err = BAESong_GetMasterTempo(theMidiSong, &outTempoFactor); return outTempoFactor;; }
     void SetQueue(BAE_BOOL queue) 
		{  /* not in MiniBAE API */ }
	 BAE_BOOL GetQueue() 
		{  /* not in MiniBAE API */ return false; }
	 void Fade(BAE_BOOL doAsync) 
		{  err = BAESong_Fade(theMidiSong, BAESong_GetVolume(theMidiSong, &outVolume),  0, 2200); }
	 void FadeTo(BAE_FIXED destVol, BAE_BOOL doAsync) 
		{  err = BAESong_Fade(theMidiSong, BAESong_GetVolume(theMidiSong, &outVolume),  destVol, 2200); }
	 void FadeFromToInTime(BAE_FIXED srcVol, BAE_FIXED destVol, BAE_FIXED timeInMillis) 
		{  err = BAESong_Fade(theMidiSong, srcVol, destVol, timeInMillis); }
 
     void AllNotesOff(unsigned long time) 
		{  err = BAESong_AllNotesOff(theMidiSong, time); }
	 void AllowChannelPitchOffset(unsigned short int channel ,BAE_BOOL allowPitch) 
		{  err = BAESong_AllowChannelTranspose(theMidiSong, channel, allowPitch); }
	 BAE_BOOL AreMidiEventsPending(void) 
		{  err = BAESong_AreMidiEventsPending(theMidiSong, &outPending); return outPending; } 
	 void Close(void) 
		{  /* not in MiniBAE API */ }
	 void ControlChange(unsigned char channel ,unsigned char controlNumber,unsigned char controlValue,unsigned long time) 
		{  err = BAESong_ControlChange(theMidiSong, channel, controlNumber, controlValue, time); }
	 BAE_BOOL DoesChannelAllowPitchOffset(unsigned short int channel) 
		{  err = BAESong_DoesChannelAllowTranspose(theMidiSong, channel, &outAllowTranspose); return outAllowTranspose; }
//	 BAEControlerCallbackPtr GetControlCallback(void) {   }
	 void * GetControlCallbackReference(void) 
		{  /* not in MiniBAE API */ return NULL; }
	 char GetControlValue(unsigned char channel, unsigned char controller) 
		{  err = BAESong_GetControlValue(theMidiSong, channel, controller, outValue); return outValue[0]; }  
//	 BAEDoneCallbackPtr GetDoneCallback(void) {   }
	 void * GetDoneCallbackReference(void) 
		{  /* not in MiniBAE API */ return NULL;  }
	 short int GetEmbeddedMidiVoices(void) 
		{  /* not in MiniBAE API */ return 0; }
	 short int GetEmbeddedMixLevel(void) 
		{  /* not in MiniBAE API */ return 0; }
	 BAEReverbType GetEmbeddedReverbType(void) 
		{  /* not in MiniBAE API */ return (BAEReverbType) 0;  }
	 short int GetEmbeddedSoundVoices(void) 
		{  /* not in MiniBAE API */ return 0; }
	 BAE_UNSIGNED_FIXED GetEmbeddedVolume(void) 
		{  /* not in MiniBAE API */ return LONG_TO_UNSIGNED_FIXED(0); } 
	 BAEResult GetInfo(BAEInfoType infoType, char *targetBuffer, unsigned long bufferBytes) 
		{  /* not in MiniBAE API ... BAESound only */ return BAE_GENERAL_ERR; }
	 BAEResult GetInfo(BAEInfoType infoType, char * cInfo) 
		{  /* not in MiniBAE API ... BAESound only */ return BAE_GENERAL_ERR; }
	 unsigned long GetInfoSize(BAEInfoType infoType) 
		{  /* not in MiniBAE API ... BAEUtil only */ return 0L; }
	 BAEResult GetInstruments(BAE_INSTRUMENT *pArray768, short int *pReturnedCount) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR; }
	 BAE_BOOL GetMetaLoopFlag(void) 
		{  /* not in MiniBAE API */ return false; }
	 unsigned long GetMicrosecondLength(void) 
		{  err =  BAESong_GetMicrosecondLength(theMidiSong, &outLength); return outLength; }
	 unsigned long GetMicrosecondPosition(void) 
		{  err =  BAESong_GetMicrosecondPosition(theMidiSong, &outTicks); return outTicks; }
	 const char *GetName(void) 
		{  /* not in MiniBAE API */ return NULL; }
	 long GetPitchOffset(void) 
		{  err = BAESong_GetTranspose(theMidiSong, &outSemitones); return outSemitones; }
	 void GetProgramBank(unsigned char channel,unsigned char *pProgram,unsigned char *pBank) 
		{  err = BAESong_GetProgramBank(theMidiSong, channel, pProgram, pBank); }
	 unsigned long GetTick(void) 
		{  /* not in MiniBAE API ... BAEMixer only */ return 0L; }
	 unsigned long GetTickLength(void) 
		{  /* not in MiniBAE API*/ return 0L; }
	 unsigned long GetTickPosition(void) 
		{  /* not in MiniBAE API*/ return 0L; }
	 BAE_FIXED GetVolume(void) 
		{  err = BAESong_GetVolume(theMidiSong, &outVolume); return outVolume; }  
//	 BAETimeCallbackPtr GetTimeCallback(void) {   }
	 void * GetTimeCallbackReference(void) 
		{  /* not in MiniBAE API*/ return NULL; }
	 BAE_BOOL IsDone(void) 
		{  err = BAESong_IsDone(theMidiSong, &outIsDone); return outIsDone; }
	 BAE_BOOL IsInstrumentLoaded(BAE_INSTRUMENT instrument) 
		{  err = BAESong_IsInstrumentLoaded(theMidiSong, instrument, &outIsLoaded); return outIsLoaded; }  
	 BAE_BOOL IsPaused(void) 
		{  err = BAESong_IsPaused(theMidiSong, &outIsPaused); return outIsPaused; }
	 BAE_BOOL IsPlaying(void) 
		{ err = BAESong_IsDone(theMidiSong, &outIsDone); return !outIsDone;  }
	 BAEResult LoadFromMemory(void* pMidiData ,unsigned long midiSize,BAE_BOOL duplicateObject ,BAE_BOOL ignoreBadInstruments) 
		{  err = BAESong_LoadMidiFromMemory(theMidiSong, pMidiData, midiSize, ignoreBadInstruments); return err; }
	 BAEResult LoadFromBank (char * cName, BAE_BOOL ignoreBadInstruments) 
		{  return BAE_NO_ERROR; /* not in MiniBAE API*/ }
	 BAEResult LoadFromFile (BAEPathName pFilePath, BAE_BOOL ignoreBadInstruments) 
		{  err = BAESong_LoadMidiFromFile(theMidiSong, pFilePath, ignoreBadInstruments); return err; }
	 BAEResult LoadFromID(unsigned long id, BAE_BOOL ignoreBadInstruments) 
		{  return BAE_NO_ERROR; /* not in MiniBAE API*/ }
	 BAEResult LoadInstrument(BAE_INSTRUMENT instrument) 
		{  err = BAESong_LoadInstrument(theMidiSong, instrument); return err; }
	 void NoteOff(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOff(theMidiSong, channel, note, velocity, time); }
	 void NoteOn(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOn(theMidiSong, channel, note, velocity, time); }
	 void NoteOnWithLoad(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOnWithLoad(theMidiSong, channel, note, velocity, time); }
	 BAEResult Open(BAE_BOOL loadInstruments) 
	 {  /* not in MiniBAE API ... BAEMixer only */ return BAE_GENERAL_ERR; }
	 void ParseMidiData(unsigned char commandByte,unsigned char data1Byte,unsigned char data2Byte,unsigned char data3Byte, unsigned long time) 
		{  err = BAESong_ParseMidiData(theMidiSong, commandByte, data1Byte, data2Byte, data3Byte, time); }
	 void Pause(void) 
		{  err = BAESong_Pause(theMidiSong); }
	 void PitchBend(unsigned char channel ,unsigned char lsb ,unsigned char msb,unsigned long time) 
		{  err = BAESong_PitchBend(theMidiSong, channel, lsb, msb, time); }
	 void ProgramBankChange(unsigned char channel,unsigned char programNumber,unsigned char bankNumber,unsigned long time) 
		{  err = BAESong_ProgramBankChange(theMidiSong, channel, programNumber, bankNumber, time); }
	 void ProgramChange(unsigned char channel ,unsigned char programNumber,unsigned long time) 
		{  err = BAESong_ProgramChange(theMidiSong, channel, programNumber, time); }
	 BAEResult RemapInstrument(BAE_INSTRUMENT from , BAE_INSTRUMENT to) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR;  }
	 void Resume(void) 
		{  err = BAESong_Resume(theMidiSong); }
//	 void SetControlCallback(short int controller, BAEControlerCallbackPtr pControllerCallback, void *pReference) {   }
//	 void SetDoneCallback( BAEDoneCallbackPtr pSongCallback, void *pReference) {   }
	 void SetEmbeddedMidiVoices(short int midiVoices) 
		{ /* not in MiniBAE API */  }
	 void SetEmbeddedMixLevel(short int mixLevel) 
		{ /* not in MiniBAE API */  }
	 void SetEmbeddedReverbType(BAEReverbType type) 
		{  /* not in MiniBAE API */ }
	 void SetEmbeddedSoundVoices(short int soundVoices) 
		{  /* not in MiniBAE API */ }
//	 void SetMetaEventCallback( BAEMetaEventCallbackPtr pSongCallback, void *pReference) {   }
	 void SetMetaLoopFlag(BAE_BOOL loop) 
		{ /* not in MiniBAE API */  }
	 BAEResult SetMicrosecondPosition(unsigned long ticks) 
		{  err = BAESong_SetMicrosecondPosition(theMidiSong, ticks); return err; }
	 void SetPitchOffset(long offset) 
		{  err = BAESong_SetTranspose(theMidiSong, offset); }
	 BAEResult SetTickPosition(unsigned long ticks) 
		{  return BAE_NO_ERROR; /* not in MiniBAE API */ }
//	 void SetTimeCallback( BAETimeCallbackPtr pSongCallback, void *pReference) {   }
	 void SetVolume(BAE_FIXED newVolume) 
		{  err = BAESong_SetVolume(theMidiSong, newVolume); }
	 BAEResult Start(BAE_BOOL useEmbeddedMixerSettings) 
		{  err = BAESong_Start(theMidiSong, 0); return err; }
	 void Stop(BAE_BOOL startFade) 
		{  err = BAESong_Stop(theMidiSong, startFade); }
	 BAE_INSTRUMENT TranslateBankProgramToInstrument(unsigned short bank, unsigned short program, unsigned short channel, unsigned short note) 
		{  BAE_INSTRUMENT n = TranslateBankProgramToInstrument(bank, program, channel, note); return n; }
	 void Unload(void) 
		{  /* not in MiniBAE API ... BAESound only */ }
	 BAEResult UnloadInstrument(BAE_INSTRUMENT instrument) 
		{  err = BAESong_UnloadInstrument(theMidiSong, instrument); return err; }
// MiniBAE-only methods
	BAEResult Delete(BAESong song)
		{ err =  BAESong_Delete(theMidiSong); return err; }		
	BAEResult LoadGroovoid(BAESong song,	char *cName, BAE_BOOL ignoreBadInstruments)
		{ err =  BAESong_LoadGroovoid(theMidiSong,	cName,  ignoreBadInstruments); return err; }
	BAEResult Preroll(BAESong song)
		{ err =  BAESong_Preroll(theMidiSong); return err; }
} ;




struct TTestRmfSong
{

    BAEResult		err;
    BAESong			theRmfSong;

     BAE_BOOL outPending;
	 BAE_BOOL outChannels[65];
	 BAE_BOOL theChannelStatus;
	 BAE_BOOL outTracks[65];
	 BAE_BOOL theTrackStatus;
	 short outNumLoops;
	 BAE_UNSIGNED_FIXED outTempoFactor;
	 BAE_UNSIGNED_FIXED outVolume;
	 BAE_BOOL outAllowTranspose;
	 char *outValue;
	 unsigned long outTicks;
	 unsigned long outLength;
	 long outSemitones;
	 BAE_BOOL outIsDone;
	 BAE_BOOL outIsLoaded;
	 BAE_BOOL outIsPaused;

	 void MuteChannel(short int channel) 
		{  err = BAESong_MuteChannel(theRmfSong, channel); }
	 void UnmuteChannel(short int channel) 
		{  err = BAESong_UnmuteChannel(theRmfSong, channel); }
	 BAE_BOOL GetChannelMuteStatus(short int channel) 
		{  err = BAESong_GetChannelMuteStatus(theRmfSong, outChannels); theChannelStatus = outChannels[channel];  return theChannelStatus; }
	 void GetChannelMuteStatus(BAE_BOOL *pChannels) 
		{  err = BAESong_GetChannelMuteStatus(theRmfSong, pChannels);  }
	 void SoloChannel(short int channel) 
		{  err = BAESong_SoloChannel(theRmfSong, channel); }
	 void UnSoloChannel(short int channel) 
		{  err = BAESong_UnSoloChannel(theRmfSong, channel); }
	 BAE_BOOL GetChannelSoloStatus(short int channel) 
		{  err = BAESong_GetChannelSoloStatus(theRmfSong, outChannels); theChannelStatus = outChannels[channel]; return theChannelStatus;}
	 void GetChannelSoloStatus(BAE_BOOL *pChannels) 
		{  err = BAESong_GetChannelSoloStatus(theRmfSong, pChannels);  }
	 void MuteTrack(short int track) 
		{  err = BAESong_MuteTrack(theRmfSong, track); }
	 void UnmuteTrack(short int track) 
		{  err = BAESong_UnmuteTrack(theRmfSong, track); }
	 BAE_BOOL GetTrackMuteStatus(short int track) 
		{  err = BAESong_GetTrackMuteStatus(theRmfSong, outTracks); theTrackStatus = outTracks[track]; return theTrackStatus; }
	 void GetTrackMuteStatus(BAE_BOOL *pTracks) 
		{  err = BAESong_GetTrackMuteStatus(theRmfSong, pTracks); }
	 void SoloTrack(short int track) 
		{  err = BAESong_SoloTrack(theRmfSong, track); }
	 void UnSoloTrack(short int track) 
		{  err = BAESong_UnSoloTrack(theRmfSong, track); }
	 BAE_BOOL GetSoloTrackStatus(short int track) 
		{  err = BAESong_GetSoloTrackStatus(theRmfSong, outTracks); theTrackStatus = outTracks[track]; return theTrackStatus;}
	 void GetSoloTrackStatus(BAE_BOOL *pTracks) 
		{  err = BAESong_GetSoloTrackStatus(theRmfSong, pTracks);   }
	 void SetLoopFlag(BAE_BOOL loop) 
		{  /* not in MiniBAE API */ }
	 BAE_BOOL GetLoopFlag(void) 
		{   /* not in MiniBAE API */ return false;}
	 void SetLoopMax(short int max) 
		{  err = BAESong_SetLoops(theRmfSong, max); }
	 short int GetLoopMax() 
		{  err = BAESong_GetLoops(theRmfSong, &outNumLoops); return 0;}
	 void SetTempo(short int tempo) 
		{  /* not in MiniBAE API */ }
     short int GetTempo(void) 
		{  /* not in MiniBAE API */ return 0; }
	 void SetTempoInBeatsPerMinute(short int tempo) 
		{  /* not in MiniBAE API */ }
	 short int GetTempoInBeatsPerMinute(void) 
		{  /* not in MiniBAE API */ return 0; }
	 void SetMasterTempo(BAE_FIXED masterTempo) 
		{  err = BAESong_SetMasterTempo(theRmfSong, (BAE_UNSIGNED_FIXED)masterTempo); }
	 BAE_FIXED GetMasterTempo(void) 
		{  err = BAESong_GetMasterTempo(theRmfSong, &outTempoFactor); return outTempoFactor; }
     void SetQueue(BAE_BOOL queue) 
		{  /* not in MiniBAE API */ }
	 BAE_BOOL GetQueue() 
		{  /* not in MiniBAE API */ return false; }
	 void Fade(BAE_BOOL doAsync) 
		{  err = BAESong_Fade(theRmfSong, BAESong_GetVolume(theRmfSong, &outVolume),  0, 2200); }
	 void FadeTo(BAE_FIXED destVol, BAE_BOOL doAsync) 
		{  err = BAESong_Fade(theRmfSong, BAESong_GetVolume(theRmfSong, &outVolume),  destVol, 2200); }
	 void FadeFromToInTime(BAE_FIXED srcVol, BAE_FIXED destVol, BAE_FIXED timeInMillis) 
		{  err = BAESong_Fade(theRmfSong, srcVol, destVol, timeInMillis); }
 
     void AllNotesOff(unsigned long time) 
		{  err = BAESong_AllNotesOff(theRmfSong, time); }
	 void AllowChannelPitchOffset(unsigned short int channel ,BAE_BOOL allowPitch) 
		{  err = BAESong_AllowChannelTranspose(theRmfSong, channel, allowPitch); }
	BAE_BOOL AreMidiEventsPending(void) 
		{  err = BAESong_AreMidiEventsPending(theRmfSong, &outPending); return outPending; } 
	 void Close(void) 
		{  /* not in MiniBAE API */ }
	 void ControlChange(unsigned char channel ,unsigned char controlNumber,unsigned char controlValue,unsigned long time) 
		{  err = BAESong_ControlChange(theRmfSong, channel, controlNumber, controlValue, time); }
	 BAE_BOOL DoesChannelAllowPitchOffset(unsigned short int channel) 
		{  err = BAESong_DoesChannelAllowTranspose(theRmfSong, channel, &outAllowTranspose); return outAllowTranspose; }
//	 BAEControlerCallbackPtr GetControlCallback(void) {   }
	 void * GetControlCallbackReference(void) 
		{  /* not in MiniBAE API */ return NULL; }
	 BAEResult GetControlValue(unsigned char channel, unsigned char controller) 
		{  err = BAESong_GetControlValue(theRmfSong, channel, controller, outValue); return err; }  
//	 BAEDoneCallbackPtr GetDoneCallback(void) {   }
	 void * GetDoneCallbackReference(void) 
		{  /* not in MiniBAE API */ return NULL;  }
	 short int GetEmbeddedMidiVoices(void) 
		{  /* not in MiniBAE API */ return 0; }
	 short int GetEmbeddedMixLevel(void) 
		{  /* not in MiniBAE API */ return 0; }
	 BAEReverbType GetEmbeddedReverbType(void) 
		{  /* not in MiniBAE API */ return (BAEReverbType) 0;  }
	 short int GetEmbeddedSoundVoices(void) 
		{  /* not in MiniBAE API */ return 0; }
	 BAE_UNSIGNED_FIXED GetEmbeddedVolume(void) 
		{  /* not in MiniBAE API */ return LONG_TO_UNSIGNED_FIXED(0);  } 
	 BAEResult GetInfo(BAEInfoType infoType, char *targetBuffer, unsigned long bufferBytes) 
		{  /* not in MiniBAE API ... BAESound only */ return BAE_GENERAL_ERR; }
	 BAEResult GetInfo(BAEInfoType infoType, char * cInfo) 
		{  /* not in MiniBAE API ... BAESound only */ return BAE_GENERAL_ERR; }
	 unsigned long GetInfoSize(BAEInfoType infoType) 
	 {  /* not in MiniBAE API ... BAEUtil only */ return 0L; }
	 BAEResult GetInstruments(BAE_INSTRUMENT *pArray768, short int *pReturnedCount) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR;}
	 BAE_BOOL GetMetaLoopFlag(void) 
		{  /* not in MiniBAE API */ return false; }
	 unsigned long GetMicrosecondLength(void) 
		{  err =  BAESong_GetMicrosecondLength(theRmfSong, &outLength); return outLength; }
	 unsigned long GetMicrosecondPosition(void) 
		{  err =  BAESong_GetMicrosecondPosition(theRmfSong, &outTicks); return outTicks; }
	 const char *GetName(void) 
		{  /* not in MiniBAE API */ return NULL; }
	 long GetPitchOffset(void) 
		{  err = BAESong_GetTranspose(theRmfSong, &outSemitones); return outSemitones; }
	 void GetProgramBank(unsigned char channel,unsigned char *pProgram,unsigned char *pBank) 
		{  err = BAESong_GetProgramBank(theRmfSong, channel, pProgram, pBank); }
	 BAEResult GetRmfVersion(void *pRMFData, unsigned long rmfSize, short int *pVersionMajor, short int*pVersionMinor, short int *pVersionSubMinor)
		{	err = BAEUtil_GetRmfVersion( pRMFData, rmfSize, pVersionMajor, pVersionMinor, pVersionSubMinor); return err;	}
	 unsigned long GetTick(void) 
		{  /* not in MiniBAE API ... BAEMixer only */ return 0L; }
	 unsigned long GetTickLength(void) 
		{  /* not in MiniBAE API*/ return 0L; }
	 unsigned long GetTickPosition(void) 
		{  /* not in MiniBAE API*/ return 0L; }
	 BAE_FIXED GetVolume(void) 
		{  err = BAESong_GetVolume(theRmfSong, &outVolume); return outVolume; }  
//	 BAETimeCallbackPtr GetTimeCallback(void) {   }
	 void * GetTimeCallbackReference(void) 
		{  /* not in MiniBAE API*/ return NULL; }
	 BAE_BOOL IsRmfSongCompressed(void *pRMFData, unsigned long rmfSize, short songIndex)
		{  return BAEUtil_IsRmfSongCompressed(pRMFData, rmfSize, songIndex); }
	 BAE_BOOL IsDone(void) 
		{  err = BAESong_IsDone(theRmfSong, &outIsDone); return outIsDone; }
	 BAE_BOOL IsInstrumentLoaded(BAE_INSTRUMENT instrument) 
		{  err = BAESong_IsInstrumentLoaded(theRmfSong, instrument, &outIsLoaded); return outIsLoaded;}  
	 BAE_BOOL IsRmfSongEncrypted(void *pRMFData, unsigned long rmfSize, short songIndex)
		{  return BAEUtil_IsRmfSongEncrypted(pRMFData, rmfSize, songIndex); }
	 BAE_BOOL IsPaused(void) 
		{  err = BAESong_IsPaused(theRmfSong, &outIsPaused); return outIsPaused; }
	 BAE_BOOL IsPlaying(void) 
		{ err = BAESong_IsDone(theRmfSong, &outIsDone); return !outIsDone; }
	 BAEResult LoadFromMemory(void* pRmfData ,unsigned long rmfSize,BAE_BOOL duplicateObject ,BAE_BOOL ignoreBadInstruments) 
		{  err = BAESong_LoadRmfFromMemory(theRmfSong, pRmfData, rmfSize, 0, ignoreBadInstruments); return err; }
	 BAEResult LoadFromBank (char * cName, BAE_BOOL ignoreBadInstruments) 
		{  return BAE_NO_ERROR; /* not in MiniBAE API*/ }
	 BAEResult LoadFromFile (BAEPathName pFilePath, BAE_BOOL ignoreBadInstruments) 
		{  err = BAESong_LoadRmfFromFile(theRmfSong, pFilePath, 0, ignoreBadInstruments); return err; }
	 BAEResult LoadFromID(unsigned long id, BAE_BOOL ignoreBadInstruments) 
		{  return BAE_NO_ERROR; /* not in MiniBAE API*/ }
	 BAEResult LoadInstrument(BAE_INSTRUMENT instrument) 
		{  err = BAESong_LoadInstrument(theRmfSong, instrument); return err; }
	 void NoteOff(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOff(theRmfSong, channel, note, velocity, time); }
	 void NoteOn(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOn(theRmfSong, channel, note, velocity, time); }
	 void NoteOnWithLoad(unsigned char channel,unsigned char note,unsigned char velocity,unsigned long time) 
		{  err = BAESong_NoteOnWithLoad(theRmfSong, channel, note, velocity, time); }
	 BAEResult Open(BAE_BOOL loadInstruments) 
		{  /* not in MiniBAE API ... BAEMixer only */ return BAE_GENERAL_ERR; }
	 void ParseMidiData(unsigned char commandByte,unsigned char data1Byte,unsigned char data2Byte,unsigned char data3Byte, unsigned long time) 
		{  err = BAESong_ParseMidiData(theRmfSong, commandByte, data1Byte, data2Byte, data3Byte, time); }
	 void Pause(void) 
		{  err = BAESong_Pause(theRmfSong); }
	 void PitchBend(unsigned char channel ,unsigned char lsb ,unsigned char msb,unsigned long time) 
		{  err = BAESong_PitchBend(theRmfSong, channel, lsb, msb, time); }
	 void ProgramBankChange(unsigned char channel,unsigned char programNumber,unsigned char bankNumber,unsigned long time) 
		{  err = BAESong_ProgramBankChange(theRmfSong, channel, programNumber, bankNumber, time); }
	 void ProgramChange(unsigned char channel ,unsigned char programNumber,unsigned long time) 
		{  err = BAESong_ProgramChange(theRmfSong, channel, programNumber, time); }
	 BAEResult RemapInstrument(BAE_INSTRUMENT from , BAE_INSTRUMENT to) 
		{  /* not in MiniBAE API */ return BAE_GENERAL_ERR; }
	 void Resume(void) 
		{  err = BAESong_Resume(theRmfSong); }
//	 void SetControlCallback(short int controller, BAEControlerCallbackPtr pControllerCallback, void *pReference) {   }
//	 void SetDoneCallback( BAEDoneCallbackPtr pSongCallback, void *pReference) {   }
	 void SetEmbeddedMidiVoices(short int midiVoices) 
		{ /* not in MiniBAE API */  }
	 void SetEmbeddedMixLevel(short int mixLevel) 
		{ /* not in MiniBAE API */  }
	 void SetEmbeddedReverbType(BAEReverbType type) 
		{  /* not in MiniBAE API */ }
	 void SetEmbeddedSoundVoices(short int soundVoices) 
		{  /* not in MiniBAE API */ }
//	 void SetMetaEventCallback( BAEMetaEventCallbackPtr pSongCallback, void *pReference) {   }
	 void SetMetaLoopFlag(BAE_BOOL loop) 
		{ /* not in MiniBAE API */  }
	 BAEResult SetMicrosecondPosition(unsigned long ticks) 
		{  err = BAESong_SetMicrosecondPosition(theRmfSong, ticks); return err; }
	 void SetPitchOffset(long offset) 
		{  err = BAESong_SetTranspose(theRmfSong, offset); }
	 BAEResult SetTickPosition(unsigned long ticks) 
		{  return BAE_NO_ERROR; /* not in MiniBAE API */ }
//	 void SetTimeCallback( BAETimeCallbackPtr pSongCallback, void *pReference) {   }
	 void SetVolume(BAE_FIXED newVolume) 
		{  err = BAESong_SetVolume(theRmfSong, newVolume); }
	 BAEResult Start(BAE_BOOL useEmbeddedMixerSettings) 
		{  err = BAESong_Start(theRmfSong, 0); return err; }
	 void Stop(BAE_BOOL startFade) 
		{  err = BAESong_Stop(theRmfSong, startFade); }
	 BAE_INSTRUMENT TranslateBankProgramToInstrument(unsigned short bank, unsigned short program, unsigned short channel, unsigned short note) 
		{  BAE_INSTRUMENT n = TranslateBankProgramToInstrument(bank, program, channel, note); return n; }
	 void Unload(void) 
		{  /* not in MiniBAE API ... BAESound only */ }
	 BAEResult UnloadInstrument(BAE_INSTRUMENT instrument) 
		{  err = BAESong_UnloadInstrument(theRmfSong, instrument); return err; }

// MiniBAE-only methods
	BAEResult Delete(BAESong song)
		{ err =  BAESong_Delete(theRmfSong); return err;  }		
	BAEResult LoadGroovoid(BAESong song,	char *cName, BAE_BOOL ignoreBadInstruments)
		{ err =  BAESong_LoadGroovoid(theRmfSong,	cName,  ignoreBadInstruments); return err; }
	BAEResult Preroll(BAESong song)
		{ err =  BAESong_Preroll(theRmfSong); return err; }
} ;


#endif  // MINIBAE

#endif  // TTEST_MB_H