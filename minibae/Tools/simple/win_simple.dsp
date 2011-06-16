# Microsoft Developer Studio Project File - Name="simple" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=simple - Win32 no recording
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win_simple.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win_simple.mak" CFG="simple - Win32 no recording"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "simple - Win32 normal loops" (based on "Win32 (x86) Console Application")
!MESSAGE "simple - Win32 filter loops" (based on "Win32 (x86) Console Application")
!MESSAGE "simple - Win32 no recording" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MiniBAE_1.00/Tools/simple", HLOAAAAA"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "simple - Win32 normal loops"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W4 /Gm /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "\dxsdk\sdk\inc" /D WRITE_LOOPS=TRUE /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D USE_MPEG_DECODER=0 /D USE_MPEG_ENCODER=0 /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D BAE_COMPLETE=1 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib ole32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/normalloop.exe" /pdbtype:sept /libpath:"\dxsdk\sdk\lib"

!ELSEIF  "$(CFG)" == "simple - Win32 filter loops"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /Zi /Od /Gf /Gy /I "..\..\BAE_MPEG_source" /I "..\..\BAE_MPEG_source\(MPEG_decoder)" /I "..\..\BAE_MPEG_source\(MPEG_encoder)\8hz-mp3" /I "..\..\BAE_Source\Platform" /I "C:\Moe\X" /I "..\..\BAEExporter" /I "..\..\BAE_Source\Common" /I "\dxsdk\sdk\inc" /I "\projects\Xtra\Source\WINFILES\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D USE_MPEG_DECODER=0 /D USE_MPEG_ENCODER=0 /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D WRITE_LOOPS=TRUE /FR /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W4 /Gm /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "\dxsdk\sdk\inc" /D WRITE_LOOPS=TRUE /D FILTER_TEST=TRUE /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D USE_MPEG_DECODER=0 /D USE_MPEG_ENCODER=0 /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D BAE_COMPLETE=1 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib ole32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/simple.exe" /pdbtype:sept /libpath:"\dxsdk\sdk\lib"
# ADD LINK32 dxguid.lib ole32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/filterloop.exe" /pdbtype:sept /libpath:"\dxsdk\sdk\lib"

!ELSEIF  "$(CFG)" == "simple - Win32 no recording"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "simple__"
# PROP BASE Intermediate_Dir "simple__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /w /W0 /Gm /Zi /Od /Gf /Gy /I "..\..\BAE_MPEG_source" /I "..\..\BAE_MPEG_source\(MPEG_decoder)" /I "..\..\BAE_MPEG_source\(MPEG_encoder)\8hz-mp3" /I "..\..\BAE_Source\Platform" /I "C:\Moe\X" /I "..\..\BAEExporter" /I "..\..\BAE_Source\Common" /I "\dxsdk\sdk\inc" /I "\projects\Xtra\Source\WINFILES\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D USE_MPEG_DECODER=0 /D USE_MPEG_ENCODER=0 /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D WRITE_LOOPS=TRUE /FR /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W4 /Gm /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "\dxsdk\sdk\inc" /D WRITE_LOOPS=FALSE /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D USE_MPEG_DECODER=0 /D USE_MPEG_ENCODER=0 /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D BAE_COMPLETE=1 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib ole32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/normalloop.exe" /pdbtype:sept /libpath:"\dxsdk\sdk\lib"
# ADD LINK32 dxguid.lib ole32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/simple.exe" /pdbtype:sept /libpath:"\dxsdk\sdk\lib"

!ENDIF 

# Begin Target

# Name "simple - Win32 normal loops"
# Name "simple - Win32 filter loops"
# Name "simple - Win32 no recording"
# Begin Group "MiniBAE 1.0"

# PROP Default_Filter ""
# Begin Group "BAE_Source"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Common\DriverTools.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g711.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g721.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g723_24.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g723_40.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g72x.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g72x.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenCache.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenOutputSimple.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenPatch.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenPriv.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSample.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeq.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeqTools.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeqTools.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSetup.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSnd.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSong.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSoundFiles.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynth.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynthFiltersSimple.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynthInterp2Simple.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MiniBAE.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MiniBAE.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\NewNewLZSS.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\SampleTools.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_API.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_API.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Assert.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Decompress.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Formats.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_IMA.c
# End Source File
# End Group
# Begin Group "Platform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.CPP
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_Thread.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAEBuildOptions_WinOS.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "source"

# PROP Default_Filter "*.*"
# Begin Source File

SOURCE=.\simple.c
# ADD CPP /W1
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
