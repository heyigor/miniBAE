# Microsoft Developer Studio Generated NMAKE File, Based on TestSuite_VC6.dsp
!IF "$(CFG)" == ""
CFG=TestSuite - Win32 DSP
!MESSAGE No configuration specified. Defaulting to TestSuite - Win32 DSP.
!ENDIF 

!IF "$(CFG)" != "TestSuite - Win32 Complete" && "$(CFG)" != "TestSuite - Win32 MCU" && "$(CFG)" != "TestSuite - Win32 DSP"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestSuite_VC6.mak" CFG="TestSuite - Win32 DSP"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestSuite - Win32 Complete" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 MCU" (based on "Win32 (x86) Console Application")
!MESSAGE "TestSuite - Win32 DSP" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

OUTDIR=.\Complete
INTDIR=.\Complete
# Begin Custom Macros
OutDir=.\Complete
# End Custom Macros

ALL : "$(OUTDIR)\TestSuite_VC6.exe" "$(OUTDIR)\TestSuite_VC6.bsc"


CLEAN :
	-@erase "$(INTDIR)\BAE_API_WinOS.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_GetDXVer.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_Thread.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_Thread.sbr"
	-@erase "$(INTDIR)\BAEMixerTest.obj"
	-@erase "$(INTDIR)\BAEMixerTest.sbr"
	-@erase "$(INTDIR)\BAESongTest.obj"
	-@erase "$(INTDIR)\BAESongTest.sbr"
	-@erase "$(INTDIR)\BAESoundTest.obj"
	-@erase "$(INTDIR)\BAESoundTest.sbr"
	-@erase "$(INTDIR)\DriverTools.obj"
	-@erase "$(INTDIR)\DriverTools.sbr"
	-@erase "$(INTDIR)\g711.obj"
	-@erase "$(INTDIR)\g711.sbr"
	-@erase "$(INTDIR)\g721.obj"
	-@erase "$(INTDIR)\g721.sbr"
	-@erase "$(INTDIR)\g723_24.obj"
	-@erase "$(INTDIR)\g723_24.sbr"
	-@erase "$(INTDIR)\g723_40.obj"
	-@erase "$(INTDIR)\g723_40.sbr"
	-@erase "$(INTDIR)\g72x.obj"
	-@erase "$(INTDIR)\g72x.sbr"
	-@erase "$(INTDIR)\GenCache.obj"
	-@erase "$(INTDIR)\GenCache.sbr"
	-@erase "$(INTDIR)\GenOutputSimple.obj"
	-@erase "$(INTDIR)\GenOutputSimple.sbr"
	-@erase "$(INTDIR)\GenPatch.obj"
	-@erase "$(INTDIR)\GenPatch.sbr"
	-@erase "$(INTDIR)\GenSample.obj"
	-@erase "$(INTDIR)\GenSample.sbr"
	-@erase "$(INTDIR)\GenSeq.obj"
	-@erase "$(INTDIR)\GenSeq.sbr"
	-@erase "$(INTDIR)\GenSeqTools.obj"
	-@erase "$(INTDIR)\GenSeqTools.sbr"
	-@erase "$(INTDIR)\GenSetup.obj"
	-@erase "$(INTDIR)\GenSetup.sbr"
	-@erase "$(INTDIR)\GenSong.obj"
	-@erase "$(INTDIR)\GenSong.sbr"
	-@erase "$(INTDIR)\GenSoundFiles.obj"
	-@erase "$(INTDIR)\GenSoundFiles.sbr"
	-@erase "$(INTDIR)\GenSynth.obj"
	-@erase "$(INTDIR)\GenSynth.sbr"
	-@erase "$(INTDIR)\GenSynthFiltersSimple.obj"
	-@erase "$(INTDIR)\GenSynthFiltersSimple.sbr"
	-@erase "$(INTDIR)\GenSynthInterp2Simple.obj"
	-@erase "$(INTDIR)\GenSynthInterp2Simple.sbr"
	-@erase "$(INTDIR)\MiniBAE.obj"
	-@erase "$(INTDIR)\MiniBAE.sbr"
	-@erase "$(INTDIR)\NewNewLZSS.obj"
	-@erase "$(INTDIR)\NewNewLZSS.sbr"
	-@erase "$(INTDIR)\SampleTools.obj"
	-@erase "$(INTDIR)\SampleTools.sbr"
	-@erase "$(INTDIR)\TestSuite.obj"
	-@erase "$(INTDIR)\TestSuite.sbr"
	-@erase "$(INTDIR)\TestTools.obj"
	-@erase "$(INTDIR)\TestTools.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\X_API.obj"
	-@erase "$(INTDIR)\X_API.sbr"
	-@erase "$(INTDIR)\X_Decompress.obj"
	-@erase "$(INTDIR)\X_Decompress.sbr"
	-@erase "$(INTDIR)\X_IMA.obj"
	-@erase "$(INTDIR)\X_IMA.sbr"
	-@erase "$(OUTDIR)\TestSuite_VC6.bsc"
	-@erase "$(OUTDIR)\TestSuite_VC6.exe"
	-@erase "$(OUTDIR)\TestSuite_VC6.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=xicl6.exe
CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TestSuite_VC6.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GenCache.sbr" \
	"$(INTDIR)\GenPatch.sbr" \
	"$(INTDIR)\GenSample.sbr" \
	"$(INTDIR)\GenSong.sbr" \
	"$(INTDIR)\X_API.sbr" \
	"$(INTDIR)\BAE_API_WinOS.sbr" \
	"$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr" \
	"$(INTDIR)\BAE_API_WinOS_Thread.sbr" \
	"$(INTDIR)\DriverTools.sbr" \
	"$(INTDIR)\g711.sbr" \
	"$(INTDIR)\g721.sbr" \
	"$(INTDIR)\g723_24.sbr" \
	"$(INTDIR)\g723_40.sbr" \
	"$(INTDIR)\g72x.sbr" \
	"$(INTDIR)\GenSoundFiles.sbr" \
	"$(INTDIR)\MiniBAE.sbr" \
	"$(INTDIR)\NewNewLZSS.sbr" \
	"$(INTDIR)\SampleTools.sbr" \
	"$(INTDIR)\X_Decompress.sbr" \
	"$(INTDIR)\X_IMA.sbr" \
	"$(INTDIR)\GenOutputSimple.sbr" \
	"$(INTDIR)\GenSeq.sbr" \
	"$(INTDIR)\GenSeqTools.sbr" \
	"$(INTDIR)\GenSetup.sbr" \
	"$(INTDIR)\GenSynth.sbr" \
	"$(INTDIR)\GenSynthFiltersSimple.sbr" \
	"$(INTDIR)\GenSynthInterp2Simple.sbr" \
	"$(INTDIR)\BAEMixerTest.sbr" \
	"$(INTDIR)\BAESongTest.sbr" \
	"$(INTDIR)\BAESoundTest.sbr" \
	"$(INTDIR)\TestSuite.sbr" \
	"$(INTDIR)\TestTools.sbr"

"$(OUTDIR)\TestSuite_VC6.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=xilink6.exe
LINK32_FLAGS=dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TestSuite_VC6.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TestSuite_VC6.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\" 
LINK32_OBJS= \
	"$(INTDIR)\GenCache.obj" \
	"$(INTDIR)\GenPatch.obj" \
	"$(INTDIR)\GenSample.obj" \
	"$(INTDIR)\GenSong.obj" \
	"$(INTDIR)\X_API.obj" \
	"$(INTDIR)\BAE_API_WinOS.obj" \
	"$(INTDIR)\BAE_API_WinOS_GetDXVer.obj" \
	"$(INTDIR)\BAE_API_WinOS_Thread.obj" \
	"$(INTDIR)\DriverTools.obj" \
	"$(INTDIR)\g711.obj" \
	"$(INTDIR)\g721.obj" \
	"$(INTDIR)\g723_24.obj" \
	"$(INTDIR)\g723_40.obj" \
	"$(INTDIR)\g72x.obj" \
	"$(INTDIR)\GenSoundFiles.obj" \
	"$(INTDIR)\MiniBAE.obj" \
	"$(INTDIR)\NewNewLZSS.obj" \
	"$(INTDIR)\SampleTools.obj" \
	"$(INTDIR)\X_Decompress.obj" \
	"$(INTDIR)\X_IMA.obj" \
	"$(INTDIR)\GenOutputSimple.obj" \
	"$(INTDIR)\GenSeq.obj" \
	"$(INTDIR)\GenSeqTools.obj" \
	"$(INTDIR)\GenSetup.obj" \
	"$(INTDIR)\GenSynth.obj" \
	"$(INTDIR)\GenSynthFiltersSimple.obj" \
	"$(INTDIR)\GenSynthInterp2Simple.obj" \
	"$(INTDIR)\BAEMixerTest.obj" \
	"$(INTDIR)\BAESongTest.obj" \
	"$(INTDIR)\BAESoundTest.obj" \
	"$(INTDIR)\TestSuite.obj" \
	"$(INTDIR)\TestTools.obj"

"$(OUTDIR)\TestSuite_VC6.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

OUTDIR=.\MCU
INTDIR=.\MCU
# Begin Custom Macros
OutDir=.\MCU
# End Custom Macros

ALL : "$(OUTDIR)\TestSuite_MCU.exe" "$(OUTDIR)\TestSuite_VC6.bsc"


CLEAN :
	-@erase "$(INTDIR)\BAE_API_WinOS.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_GetDXVer.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_MsgSnd.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_MsgSnd.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_Thread.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_Thread.sbr"
	-@erase "$(INTDIR)\BAEMixerTest.obj"
	-@erase "$(INTDIR)\BAEMixerTest.sbr"
	-@erase "$(INTDIR)\BAESongTest.obj"
	-@erase "$(INTDIR)\BAESongTest.sbr"
	-@erase "$(INTDIR)\BAESoundTest.obj"
	-@erase "$(INTDIR)\BAESoundTest.sbr"
	-@erase "$(INTDIR)\DriverTools.obj"
	-@erase "$(INTDIR)\DriverTools.sbr"
	-@erase "$(INTDIR)\g711.obj"
	-@erase "$(INTDIR)\g711.sbr"
	-@erase "$(INTDIR)\g721.obj"
	-@erase "$(INTDIR)\g721.sbr"
	-@erase "$(INTDIR)\g723_24.obj"
	-@erase "$(INTDIR)\g723_24.sbr"
	-@erase "$(INTDIR)\g723_40.obj"
	-@erase "$(INTDIR)\g723_40.sbr"
	-@erase "$(INTDIR)\g72x.obj"
	-@erase "$(INTDIR)\g72x.sbr"
	-@erase "$(INTDIR)\GenCache.obj"
	-@erase "$(INTDIR)\GenCache.sbr"
	-@erase "$(INTDIR)\GenPatch.obj"
	-@erase "$(INTDIR)\GenPatch.sbr"
	-@erase "$(INTDIR)\GenSample.obj"
	-@erase "$(INTDIR)\GenSample.sbr"
	-@erase "$(INTDIR)\GenSong.obj"
	-@erase "$(INTDIR)\GenSong.sbr"
	-@erase "$(INTDIR)\GenSoundFiles.obj"
	-@erase "$(INTDIR)\GenSoundFiles.sbr"
	-@erase "$(INTDIR)\MiniBAE.obj"
	-@erase "$(INTDIR)\MiniBAE.sbr"
	-@erase "$(INTDIR)\MsgSend.obj"
	-@erase "$(INTDIR)\MsgSend.sbr"
	-@erase "$(INTDIR)\NewNewLZSS.obj"
	-@erase "$(INTDIR)\NewNewLZSS.sbr"
	-@erase "$(INTDIR)\SampleTools.obj"
	-@erase "$(INTDIR)\SampleTools.sbr"
	-@erase "$(INTDIR)\TestSuite.obj"
	-@erase "$(INTDIR)\TestSuite.sbr"
	-@erase "$(INTDIR)\TestTools.obj"
	-@erase "$(INTDIR)\TestTools.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\X_API.obj"
	-@erase "$(INTDIR)\X_API.sbr"
	-@erase "$(INTDIR)\X_Decompress.obj"
	-@erase "$(INTDIR)\X_Decompress.sbr"
	-@erase "$(INTDIR)\X_IMA.obj"
	-@erase "$(INTDIR)\X_IMA.sbr"
	-@erase "$(OUTDIR)\TestSuite_MCU.exe"
	-@erase "$(OUTDIR)\TestSuite_MCU.pdb"
	-@erase "$(OUTDIR)\TestSuite_VC6.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=xicl6.exe
CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TestSuite_VC6.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GenCache.sbr" \
	"$(INTDIR)\GenPatch.sbr" \
	"$(INTDIR)\GenSample.sbr" \
	"$(INTDIR)\GenSong.sbr" \
	"$(INTDIR)\X_API.sbr" \
	"$(INTDIR)\BAE_API_WinOS.sbr" \
	"$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr" \
	"$(INTDIR)\BAE_API_WinOS_MsgSnd.sbr" \
	"$(INTDIR)\BAE_API_WinOS_Thread.sbr" \
	"$(INTDIR)\DriverTools.sbr" \
	"$(INTDIR)\g711.sbr" \
	"$(INTDIR)\g721.sbr" \
	"$(INTDIR)\g723_24.sbr" \
	"$(INTDIR)\g723_40.sbr" \
	"$(INTDIR)\g72x.sbr" \
	"$(INTDIR)\GenSoundFiles.sbr" \
	"$(INTDIR)\MiniBAE.sbr" \
	"$(INTDIR)\MsgSend.sbr" \
	"$(INTDIR)\NewNewLZSS.sbr" \
	"$(INTDIR)\SampleTools.sbr" \
	"$(INTDIR)\X_Decompress.sbr" \
	"$(INTDIR)\X_IMA.sbr" \
	"$(INTDIR)\BAEMixerTest.sbr" \
	"$(INTDIR)\BAESongTest.sbr" \
	"$(INTDIR)\BAESoundTest.sbr" \
	"$(INTDIR)\TestSuite.sbr" \
	"$(INTDIR)\TestTools.sbr"

"$(OUTDIR)\TestSuite_VC6.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=xilink6.exe
LINK32_FLAGS=dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TestSuite_MCU.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TestSuite_MCU.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\" 
LINK32_OBJS= \
	"$(INTDIR)\GenCache.obj" \
	"$(INTDIR)\GenPatch.obj" \
	"$(INTDIR)\GenSample.obj" \
	"$(INTDIR)\GenSong.obj" \
	"$(INTDIR)\X_API.obj" \
	"$(INTDIR)\BAE_API_WinOS.obj" \
	"$(INTDIR)\BAE_API_WinOS_GetDXVer.obj" \
	"$(INTDIR)\BAE_API_WinOS_MsgSnd.obj" \
	"$(INTDIR)\BAE_API_WinOS_Thread.obj" \
	"$(INTDIR)\DriverTools.obj" \
	"$(INTDIR)\g711.obj" \
	"$(INTDIR)\g721.obj" \
	"$(INTDIR)\g723_24.obj" \
	"$(INTDIR)\g723_40.obj" \
	"$(INTDIR)\g72x.obj" \
	"$(INTDIR)\GenSoundFiles.obj" \
	"$(INTDIR)\MiniBAE.obj" \
	"$(INTDIR)\MsgSend.obj" \
	"$(INTDIR)\NewNewLZSS.obj" \
	"$(INTDIR)\SampleTools.obj" \
	"$(INTDIR)\X_Decompress.obj" \
	"$(INTDIR)\X_IMA.obj" \
	"$(INTDIR)\BAEMixerTest.obj" \
	"$(INTDIR)\BAESongTest.obj" \
	"$(INTDIR)\BAESoundTest.obj" \
	"$(INTDIR)\TestSuite.obj" \
	"$(INTDIR)\TestTools.obj"

"$(OUTDIR)\TestSuite_MCU.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

OUTDIR=.\DSP
INTDIR=.\DSP
# Begin Custom Macros
OutDir=.\DSP
# End Custom Macros

ALL : "$(OUTDIR)\TestSuite_DSP.exe" "$(OUTDIR)\TestSuite_VC6.bsc"


CLEAN :
	-@erase "$(INTDIR)\BAE_API_WinOS.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_GetDXVer.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_MsgRcv.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_MsgRcv.sbr"
	-@erase "$(INTDIR)\BAE_API_WinOS_Thread.obj"
	-@erase "$(INTDIR)\BAE_API_WinOS_Thread.sbr"
	-@erase "$(INTDIR)\DSPMain.obj"
	-@erase "$(INTDIR)\DSPMain.sbr"
	-@erase "$(INTDIR)\GenCache.obj"
	-@erase "$(INTDIR)\GenCache.sbr"
	-@erase "$(INTDIR)\GenOutputSimple.obj"
	-@erase "$(INTDIR)\GenOutputSimple.sbr"
	-@erase "$(INTDIR)\GenPatch.obj"
	-@erase "$(INTDIR)\GenPatch.sbr"
	-@erase "$(INTDIR)\GenSample.obj"
	-@erase "$(INTDIR)\GenSample.sbr"
	-@erase "$(INTDIR)\GenSeq.obj"
	-@erase "$(INTDIR)\GenSeq.sbr"
	-@erase "$(INTDIR)\GenSeqTools.obj"
	-@erase "$(INTDIR)\GenSeqTools.sbr"
	-@erase "$(INTDIR)\GenSetup.obj"
	-@erase "$(INTDIR)\GenSetup.sbr"
	-@erase "$(INTDIR)\GenSong.obj"
	-@erase "$(INTDIR)\GenSong.sbr"
	-@erase "$(INTDIR)\GenSynth.obj"
	-@erase "$(INTDIR)\GenSynth.sbr"
	-@erase "$(INTDIR)\GenSynthFiltersSimple.obj"
	-@erase "$(INTDIR)\GenSynthFiltersSimple.sbr"
	-@erase "$(INTDIR)\GenSynthInterp2Simple.obj"
	-@erase "$(INTDIR)\GenSynthInterp2Simple.sbr"
	-@erase "$(INTDIR)\MsgHandle.obj"
	-@erase "$(INTDIR)\MsgHandle.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\X_API.obj"
	-@erase "$(INTDIR)\X_API.sbr"
	-@erase "$(OUTDIR)\TestSuite_DSP.exe"
	-@erase "$(OUTDIR)\TestSuite_DSP.pdb"
	-@erase "$(OUTDIR)\TestSuite_VC6.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=xicl6.exe
CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TestSuite_VC6.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GenCache.sbr" \
	"$(INTDIR)\GenPatch.sbr" \
	"$(INTDIR)\GenSample.sbr" \
	"$(INTDIR)\GenSong.sbr" \
	"$(INTDIR)\X_API.sbr" \
	"$(INTDIR)\BAE_API_WinOS.sbr" \
	"$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr" \
	"$(INTDIR)\BAE_API_WinOS_MsgRcv.sbr" \
	"$(INTDIR)\BAE_API_WinOS_Thread.sbr" \
	"$(INTDIR)\GenOutputSimple.sbr" \
	"$(INTDIR)\GenSeq.sbr" \
	"$(INTDIR)\GenSeqTools.sbr" \
	"$(INTDIR)\GenSetup.sbr" \
	"$(INTDIR)\GenSynth.sbr" \
	"$(INTDIR)\GenSynthFiltersSimple.sbr" \
	"$(INTDIR)\GenSynthInterp2Simple.sbr" \
	"$(INTDIR)\MsgHandle.sbr" \
	"$(INTDIR)\DSPMain.sbr"

"$(OUTDIR)\TestSuite_VC6.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=xilink6.exe
LINK32_FLAGS=dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TestSuite_DSP.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TestSuite_DSP.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\lib\\" 
LINK32_OBJS= \
	"$(INTDIR)\GenCache.obj" \
	"$(INTDIR)\GenPatch.obj" \
	"$(INTDIR)\GenSample.obj" \
	"$(INTDIR)\GenSong.obj" \
	"$(INTDIR)\X_API.obj" \
	"$(INTDIR)\BAE_API_WinOS.obj" \
	"$(INTDIR)\BAE_API_WinOS_GetDXVer.obj" \
	"$(INTDIR)\BAE_API_WinOS_MsgRcv.obj" \
	"$(INTDIR)\BAE_API_WinOS_Thread.obj" \
	"$(INTDIR)\GenOutputSimple.obj" \
	"$(INTDIR)\GenSeq.obj" \
	"$(INTDIR)\GenSeqTools.obj" \
	"$(INTDIR)\GenSetup.obj" \
	"$(INTDIR)\GenSynth.obj" \
	"$(INTDIR)\GenSynthFiltersSimple.obj" \
	"$(INTDIR)\GenSynthInterp2Simple.obj" \
	"$(INTDIR)\MsgHandle.obj" \
	"$(INTDIR)\DSPMain.obj"

"$(OUTDIR)\TestSuite_DSP.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TestSuite_VC6.dep")
!INCLUDE "TestSuite_VC6.dep"
!ELSE 
!MESSAGE Warning: cannot find "TestSuite_VC6.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TestSuite - Win32 Complete" || "$(CFG)" == "TestSuite - Win32 MCU" || "$(CFG)" == "TestSuite - Win32 DSP"
SOURCE=..\..\BAE_Source\Common\GenCache.c

"$(INTDIR)\GenCache.obj"	"$(INTDIR)\GenCache.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\BAE_Source\Common\GenPatch.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenPatch.obj"	"$(INTDIR)\GenPatch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenPatch.obj"	"$(INTDIR)\GenPatch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenPatch.obj"	"$(INTDIR)\GenPatch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSample.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSample.obj"	"$(INTDIR)\GenSample.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSample.obj"	"$(INTDIR)\GenSample.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSample.obj"	"$(INTDIR)\GenSample.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSong.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSong.obj"	"$(INTDIR)\GenSong.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSong.obj"	"$(INTDIR)\GenSong.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSong.obj"	"$(INTDIR)\GenSong.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\X_API.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_API.obj"	"$(INTDIR)\X_API.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_API.obj"	"$(INTDIR)\X_API.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_API.obj"	"$(INTDIR)\X_API.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS.obj"	"$(INTDIR)\BAE_API_WinOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS.obj"	"$(INTDIR)\BAE_API_WinOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS.obj"	"$(INTDIR)\BAE_API_WinOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_GetDXVer.CPP

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_GetDXVer.obj"	"$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_GetDXVer.obj"	"$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_GetDXVer.obj"	"$(INTDIR)\BAE_API_WinOS_GetDXVer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_MsgRcv.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_MsgRcv.obj"	"$(INTDIR)\BAE_API_WinOS_MsgRcv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_MsgSnd.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_MsgSnd.obj"	"$(INTDIR)\BAE_API_WinOS_MsgSnd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Platform\BAE_API_WinOS_Thread.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_Thread.obj"	"$(INTDIR)\BAE_API_WinOS_Thread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_Thread.obj"	"$(INTDIR)\BAE_API_WinOS_Thread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAE_API_WinOS_Thread.obj"	"$(INTDIR)\BAE_API_WinOS_Thread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\DriverTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DriverTools.obj"	"$(INTDIR)\DriverTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DriverTools.obj"	"$(INTDIR)\DriverTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\g711.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g711.obj"	"$(INTDIR)\g711.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g711.obj"	"$(INTDIR)\g711.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\g721.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g721.obj"	"$(INTDIR)\g721.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g721.obj"	"$(INTDIR)\g721.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\g723_24.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g723_24.obj"	"$(INTDIR)\g723_24.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g723_24.obj"	"$(INTDIR)\g723_24.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\g723_40.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g723_40.obj"	"$(INTDIR)\g723_40.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g723_40.obj"	"$(INTDIR)\g723_40.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\g72x.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g72x.obj"	"$(INTDIR)\g72x.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\g72x.obj"	"$(INTDIR)\g72x.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSoundFiles.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSoundFiles.obj"	"$(INTDIR)\GenSoundFiles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSoundFiles.obj"	"$(INTDIR)\GenSoundFiles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\MiniBAE.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MiniBAE.obj"	"$(INTDIR)\MiniBAE.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MiniBAE.obj"	"$(INTDIR)\MiniBAE.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\MsgSend.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MsgSend.obj"	"$(INTDIR)\MsgSend.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\NewNewLZSS.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NewNewLZSS.obj"	"$(INTDIR)\NewNewLZSS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NewNewLZSS.obj"	"$(INTDIR)\NewNewLZSS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\SampleTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SampleTools.obj"	"$(INTDIR)\SampleTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SampleTools.obj"	"$(INTDIR)\SampleTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\X_Decompress.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_Decompress.obj"	"$(INTDIR)\X_Decompress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_Decompress.obj"	"$(INTDIR)\X_Decompress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\X_IMA.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_IMA.obj"	"$(INTDIR)\X_IMA.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\X_IMA.obj"	"$(INTDIR)\X_IMA.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenOutputSimple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenOutputSimple.obj"	"$(INTDIR)\GenOutputSimple.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenOutputSimple.obj"	"$(INTDIR)\GenOutputSimple.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSeq.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSeq.obj"	"$(INTDIR)\GenSeq.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSeq.obj"	"$(INTDIR)\GenSeq.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSeqTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSeqTools.obj"	"$(INTDIR)\GenSeqTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSeqTools.obj"	"$(INTDIR)\GenSeqTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSetup.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSetup.obj"	"$(INTDIR)\GenSetup.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSetup.obj"	"$(INTDIR)\GenSetup.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSynth.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSynth.obj"	"$(INTDIR)\GenSynth.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSynth.obj"	"$(INTDIR)\GenSynth.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSynthFiltersSimple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSynthFiltersSimple.obj"	"$(INTDIR)\GenSynthFiltersSimple.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSynthFiltersSimple.obj"	"$(INTDIR)\GenSynthFiltersSimple.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\GenSynthInterp2Simple.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSynthInterp2Simple.obj"	"$(INTDIR)\GenSynthInterp2Simple.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GenSynthInterp2Simple.obj"	"$(INTDIR)\GenSynthInterp2Simple.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\BAE_Source\Common\MsgHandle.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MsgHandle.obj"	"$(INTDIR)\MsgHandle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\BAEMixerTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAEMixerTest.obj"	"$(INTDIR)\BAEMixerTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAEMixerTest.obj"	"$(INTDIR)\BAEMixerTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=.\BAESongTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAESongTest.obj"	"$(INTDIR)\BAESongTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAESongTest.obj"	"$(INTDIR)\BAESongTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=.\BAESoundTest.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAESoundTest.obj"	"$(INTDIR)\BAESoundTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\BAESoundTest.obj"	"$(INTDIR)\BAESoundTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=.\DSPMain.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_DSP" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DSPMain.obj"	"$(INTDIR)\DSPMain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TestSuite.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TestSuite.obj"	"$(INTDIR)\TestSuite.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TestSuite.obj"	"$(INTDIR)\TestSuite.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 

SOURCE=.\TestTools.c

!IF  "$(CFG)" == "TestSuite - Win32 Complete"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_COMPLETE" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TestTools.obj"	"$(INTDIR)\TestTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 MCU"

CPP_SWITCHES=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\BAE_Source\Platform" /I "..\..\BAE_Source\Common" /I "..\..\..\..\..\Dependencies\Win\DirectX 5 SDK\sdk\inc" /D "BAE_MCU" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D X_PLATFORM=X_WIN95 /D LOOPS_USED=LIMITED_LOOPS /D REVERB_USED=REVERB_DISABLED /D USE_DROP_SAMPLE=FALSE /D USE_TERP1=FALSE /D USE_HIGHLEVEL_FILE_API=TRUE /D USE_CREATION_API=TRUE /D "BAE_MSG_MSG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TestSuite_VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TestTools.obj"	"$(INTDIR)\TestTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestSuite - Win32 DSP"

!ENDIF 


!ENDIF 

