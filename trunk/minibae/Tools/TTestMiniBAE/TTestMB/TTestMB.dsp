# Microsoft Developer Studio Project File - Name="TTestMB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TTestMB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TTestMB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TTestMB.mak" CFG="TTestMB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TTestMB - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TTestMB - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "tom-Win"
# PROP Scc_LocalPath "..\..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TTestMB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /GX /O2 /D "CFLAGSMT" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /D USE_MONO_OUTPUT=TRUE /D USE_16_BIT_OUTPUT=TRUE /D USE_FULL_RMF_SUPPORT=TRUE /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TTestMB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /Gm /GX /ZI /Od /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /I "..\..\..\BAE_Source\Common" /I "..\..\..\BAE_Source\Platform" /I "..\..\..\..\BAE\BAE_Source\Common" /I "..\\" /I "..\..\..\..\BAE\Tools\TTestBase" /D "CFLAGSMT" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /D USE_MONO_OUTPUT=TRUE /D USE_16_BIT_OUTPUT=TRUE /D USE_FULL_RMF_SUPPORT=TRUE /YX /FD /I /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\TTestMB_Win.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TTestMB - Win32 Release"
# Name "TTestMB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "platform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_Msg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_WinOS.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.CPP
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_WinOS_MsgRcv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_WinOS_MsgSnd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API_WinOS_Thread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAEBuildOptions_WinOS.h
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenCache.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenPatch.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenPriv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSnd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSong.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MsgCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_API.c
# End Source File
# End Group
# Begin Group "dsp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenOutputSimple.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSeq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSeqTools.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSeqTools.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSetup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSynth.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSynthFiltersSimple.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSynthInterp2Simple.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MsgHandle.c
# End Source File
# End Group
# Begin Group "mcu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\DriverTools.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\g711.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\g721.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\g723_24.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\g723_40.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\g72x.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\g72x.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\GenSoundFiles.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MiniBAE.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MiniBAE.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MsgSend.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\NewNewLZSS.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\SampleTools.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_API.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_Assert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_Decompress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_Formats.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_IMA.c
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\mothmap.bmp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\small.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTest_Win.ico
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\BuildDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTest.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTest_Settings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTest_Win.h
# End Source File
# Begin Source File

SOURCE=..\TTestMB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTestUtil.h
# End Source File
# End Group
# Begin Group "test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_AudioSampleFrmTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_CustomSampleTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_DisengageAudioTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_DisengageMidiTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_FadeFromToTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_FadeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_FadeToTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_LoadTests.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_LoopFlagTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_LoopMaxTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_MemTests.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiAllNotesOffTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiControlChangeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiLoadFromTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiNoteTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiOneNoteTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiParseMidiDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_MidiPauseResumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_MidiPositionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_MidiPrgBankChangeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiProgramChangeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MidiSetPitchOffsetTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_MixerDeviceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_MuteTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_PitchBendTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RealtimeStatusTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfAllNotesOffTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfControlChangeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_RmfFadeFromToTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfFadeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfFadeToTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfInfoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfLoadFromTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfLoopFlagTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfLoopMaxTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfMuteTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfNoteTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfParseMidiDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_RmfPauseResumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfPitchBendTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_RmfPositionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_RmfPrgBankChangeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfProgramChangeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfSetMasterTempoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfSetPitchOffsetTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfSetTempoInBPMTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfSetTempoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfSetVolumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_RmfSoloTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SampleLoopPointTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetFrequecyTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetHardwareVolumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetLowPassTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetMasterSFXVolumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetMasterTempoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetMasterVolumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetRateTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetResonanceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetReverbAmountTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetTempoInBPMTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetTempoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetVolumeMidiTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SetVolumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_SndFadeFromToTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SndFadeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SndFadeToTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SndLoadSampleTests.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SndLoopTests.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Bae\Tools\TTestBase\Test_SndPauseResumeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_SoloTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\Test_WavPanTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTestBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTestBase_README.txt
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTestUtil.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\AdHoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTest_Win.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\BAE\Tools\TTestBase\TTest_Win.rc
# End Source File
# End Group
# End Target
# End Project
