# Microsoft Developer Studio Project File - Name="TestSuite" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TestSuite - Win32 MCU
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TestSuite.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestSuite.mak" CFG="TestSuite - Win32 MCU"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestSuite - Win32 Complete" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 DSP" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 MCU" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName "TestSuite"
# PROP Scc_LocalPath "..\.."
CPP=xicl5.exe
RSC=rc.exe

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSuit"
# PROP BASE Intermediate_Dir "TestSuit"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Complete"
# PROP Intermediate_Dir "Complete"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D BAE_BOTH=1 /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink5.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSui0"
# PROP BASE Intermediate_Dir "TestSui0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DSP"
# PROP Intermediate_Dir "DSP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MSG_MSG" /D BAE_DSP=1 /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink5.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"DSP/TestSuite_DSP.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSui1"
# PROP BASE Intermediate_Dir "TestSui1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MCU"
# PROP Intermediate_Dir "MCU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MSG_MSG" /D BAE_MCU=1 /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink5.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"MCU/TestSuite_MCU.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ENDIF 

# Begin Target

# Name "TestSuite - Win32 Complete"
# Name "TestSuite - Win32 DSP"
# Name "TestSuite - Win32 MCU"
# Begin Group "MiniBAE_1.00"

# PROP Default_Filter ""
# Begin Group "BAE_Source"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenPatch.c
# ADD BASE CPP /w /W0
# ADD CPP /w /W0
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenPriv.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSample.c
# ADD BASE CPP /w /W0
# ADD CPP /w /W0
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSnd.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MsgCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_API.c
# ADD BASE CPP /w /W0
# ADD CPP /w /W0
# End Source File
# End Group
# Begin Group "Platform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_Msg.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS.c
# ADD BASE CPP /w /W0
# ADD CPP /w /W0
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.CPP
# ADD BASE CPP /w /W0
# ADD CPP /w /W0
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_MsgRcv.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_MsgSnd.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_Thread.c
# ADD BASE CPP /w /W0
# ADD CPP /w /W0
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAEBuildOptions_WinOS.h
# End Source File
# End Group
# Begin Group "mcu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Common\DriverTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g711.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g721.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g723_24.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g723_40.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g72x.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g72x.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSoundFiles.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MiniBAE.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MiniBAE.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MsgSend.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\NewNewLZSS.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\SampleTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_API.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Assert.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Decompress.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Formats.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_IMA.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# End Group
# Begin Group "dsp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenOutputSimple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeq.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeqTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeqTools.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSetup.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSong.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynth.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynthFiltersSimple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynthInterp2Simple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MsgHandle.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# End Group
# Begin Group "TestSuite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BAEMixerTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAEMixerTest.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESongTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESongTest.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESoundTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESoundTest.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DSPMain.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestSuite.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestSuite.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestTools.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
