# Microsoft Developer Studio Project File - Name="TestSuite" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TestSuite - Win32 DSP MPEG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TestSuite_VC6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestSuite_VC6.mak" CFG="TestSuite - Win32 DSP MPEG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestSuite - Win32 Complete" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 MCU" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 DSP" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 Complete MPEG" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 MCU MPEG" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 DSP MPEG" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "TestSuite"
# PROP Scc_LocalPath "..\.."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Complete"
# PROP Intermediate_Dir "Complete"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSuite___Win32_MCU"
# PROP BASE Intermediate_Dir "TestSuite___Win32_MCU"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MCU"
# PROP Intermediate_Dir "MCU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"MCU/TestSuite_MCU.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSuite___Win32_DSP"
# PROP BASE Intermediate_Dir "TestSuite___Win32_DSP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DSP"
# PROP Intermediate_Dir "DSP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"DSP/TestSuite_DSP.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSuite___Win32_Complete_MPEG"
# PROP BASE Intermediate_Dir "TestSuite___Win32_Complete_MPEG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Complete_MPEG"
# PROP Intermediate_Dir "Complete_MPEG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_MPEG_DECODER=1 /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSuite___Win32_MCU_MPEG"
# PROP BASE Intermediate_Dir "TestSuite___Win32_MCU_MPEG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MCU_MPEG"
# PROP Intermediate_Dir "MCU_MPEG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_MPEG_DECODER=1 /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"MCU/TestSuite_MCU.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"MCU_MPEG/TestSuite_MCU.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestSuite___Win32_DSP_MPEG"
# PROP BASE Intermediate_Dir "TestSuite___Win32_DSP_MPEG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DSP_MPEG"
# PROP Intermediate_Dir "DSP_MPEG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_MPEG_DECODER=1 /D "BAE_MSG_MSG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"DSP/TestSuite_DSP.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"
# ADD LINK32 dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"DSP_MPEG/TestSuite_DSP.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\"

!ENDIF 

# Begin Target

# Name "TestSuite - Win32 Complete"
# Name "TestSuite - Win32 MCU"
# Name "TestSuite - Win32 DSP"
# Name "TestSuite - Win32 Complete MPEG"
# Name "TestSuite - Win32 MCU MPEG"
# Name "TestSuite - Win32 DSP MPEG"
# Begin Group "MiniBAE_1.00"

# PROP Default_Filter ""
# Begin Group "BAE_Source"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenCache.c
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenPatch.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenPriv.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSample.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSnd.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSong.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MsgCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_API.c
# ADD CPP /W3
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
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.CPP
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.h
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_MsgRcv.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_MsgSnd.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_Thread.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Platform\BAEBuildOptions_WinOS.h
# End Source File
# End Group
# Begin Group "mcu"

# PROP Default_Filter ""
# Begin Group "MPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_c_copybytes.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_conv.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_bits.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_crc.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l1.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l12.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l2.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l3.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l3.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l3hf.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_l3u.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_mc.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_poly.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_proto.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_tab.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_tab.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_dec_vec.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\mpa_version.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\XMPEG_BAE_API.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\XMPEG_BAE_API.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_MPEG_Source_II\XMPEGFilesSun.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\BAE_Source\Common\DriverTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g711.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g721.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g723_24.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g723_40.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g72x.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\g72x.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSoundFiles.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MiniBAE.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MiniBAE.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MsgSend.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\NewNewLZSS.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\SampleTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_API.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Assert.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Decompress.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_Formats.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\X_IMA.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# End Group
# Begin Group "dsp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenOutputSimple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeq.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeqTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSeqTools.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSetup.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynth.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynthFiltersSimple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\GenSynthInterp2Simple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD BASE CPP /w /W0
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\BAE_Source\Common\MsgHandle.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

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

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAEMixerTest.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESongTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESongTest.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESoundTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BAESoundTest.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DSPMain.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestSuite.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestSuite.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE CPP /W3
# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestTools.h

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\BuildNotes.txt
# End Source File
# Begin Source File

SOURCE="..\..\MiniBAE Build notes.txt"
# End Source File
# Begin Source File

SOURCE=..\..\ReleaseNotes.txt
# End Source File
# Begin Source File

SOURCE=.\TestSuite_VC6.dep

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestSuite_VC6.mak

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 Complete MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP MPEG"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
