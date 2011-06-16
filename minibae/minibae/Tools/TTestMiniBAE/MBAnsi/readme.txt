/*****************************************************************************
**	MBAnsi readme
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
**
**	11/27/00	tom Created 
**
****************************************************************************/


replace the contents of your platform-implementation file with the contents of:
---------------------------------------------------------------------------------
BAE_API_Ansi.c
(includes the ANSI C file i/o and memory allocation routines)


main test files
----------
MBAnsi.c   (main test code)
MBAnsi.h

npatches.hsb  (default soundbank - store in same directory at executable)
Note1.mid     (default midi file - store in same directory at executable)
wantcha.rmf   (default rmf file  - store in same directory at executable)
newt.wav	  (default wav file  - store in same directory at executable)


MBAnsi executable usage
------------------------
MBAnsi -i inputFile.mid -o outputFile.wav -bank soundbank.hsb


MBAnsi with no arguments will use
Note1.mid as the input file
output.wav as the output file
npatches.hsb as the soundbank

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

the OutputToFile port took 3 functions from BAE.c and 2 from GenSoundFiles.c, and modified them to work with MiniBAE.
Modifications included 
	changing the file names to conform to MiniBAE conventions, 
	adding parameters as needed to conform to MiniBAE conventions
	converting class mermber fields to global variables as needed
	acquiring state data (modifiers, quality) from accessor methods instead of from globals


the purpose is to build a command-line MiniBAE tool which takes input files and generates output 
files without having to engage any actual hardware or audio device. 

the tool can compile and run on any ANSI C compatible platform, and requires no OS-specific file i/o 
or timing or memory allocation services. 

the tool generates wav files from midi, rmf, or wav file input

to build the tool:
----------------------
1)Required: A version of MiniBAE containing the OutputToFile port 
(including MiniBAE.h, MiniBAE.c and GenSoundFiles.c from 11/29/2000 or later)

2)The contents of an existing platform implementation file (e.g. BAE_API_StrongArm.c) should be 
completely replaced by the contents of BAE_API_Ansi.c. this file contains all of the ansi-c compatable 
file i/o and  memory allocation methods, as well as appropriate stubs for the other platform implementation functions.

3)add MBAnsi.c and MBAnsi.h to the project. MBAnsi.c provides the _main entry point and contains  
the necessary sample code showing how to use the OutputToFile methods.
