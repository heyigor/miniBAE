# Microsoft Developer Studio Project File - Name="MBAnsi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MBAnsi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MBAnsi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MBAnsi.mak" CFG="MBAnsi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MBAnsi - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MBAnsi - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MBAnsi"
# PROP Scc_LocalPath "..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MBAnsi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MBAnsi - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /Gm /GX /ZI /Od /I "..\..\..\BAE_Source\Platform" /I "..\..\..\..\MiniBAE\BAE_Source\Common" /I "..\..\..\..\MiniBAE\Tools\TTestMiniBAE\MBAnsi" /D "FLAGSMT" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D X_PLATFORM=X_ANSI /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /D USE_MONO_OUTPUT=TRUE /D USE_16_BIT_OUTPUT=TRUE /D USE_FULL_RMF_SUPPORT=TRUE /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"MBAnsi.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MBAnsi - Win32 Release"
# Name "MBAnsi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "platform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\BAE_Source\Platform\BAE_API.h
# End Source File
# Begin Source File

SOURCE=.\BAE_API_Ansi.c
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

SOURCE=..\..\..\BAE_Source\Common\g72x.h
# End Source File
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

SOURCE=..\..\..\BAE_Source\Common\GenSoundFiles.c
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

SOURCE=..\..\..\BAE_Source\Common\MiniBAE.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MiniBAE.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\MsgHandle.c
# End Source File
# Begin Source File

SOURCE=.\X_API.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_Assert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_Formats.h
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

SOURCE=..\..\..\BAE_Source\Common\MsgSend.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\NewNewLZSS.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\SampleTools.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_Decompress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\BAE_Source\Common\X_IMA.c
# End Source File
# End Group
# Begin Group "test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MBAnsi.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BAE_BuildOptions_Ansi.h
# End Source File
# Begin Source File

SOURCE=.\MBAnsi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
