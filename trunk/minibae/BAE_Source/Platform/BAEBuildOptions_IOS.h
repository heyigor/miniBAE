/*****************************************************************************/
/*
**	BAEBuildOptions_IOSX.h
**
**	© Copyright 1999 Beatnik, Inc, All Rights Reserved.
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
**	10/19/99	MSD:	Created -- extracted from X_API.h
**	2/17/2000	Changed settings for plugin to allow for streaming.
**	10/1/2000	Placed define for USE_MAC_OS_X if building under Carbon or MacOS X.
*/
/*****************************************************************************/


// #includes
// ----------------------------------------------
//#include <MacTypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Non-overwritable Flags
// ----------------------------------------------
#define COMPILER_TYPE                                           GCC_COMPILER

#if TARGET_IPHONE_SIMULATOR
	#define CPU_TYPE                               	 			k80X86
#else
	#if TARGET_OS_IPHONE
		#define CPU_TYPE                               	 		kARM
	#else
		#define CPU_TYPE                               	 		k80X86
	#endif
#endif

#define X_WORD_ORDER                                            TRUE
#define USE_FLOAT                                               FALSE
#define USE_8_BIT_OUTPUT                                        FALSE
#define USE_16_BIT_OUTPUT                                       TRUE
#define USE_MONO_OUTPUT                                         TRUE
#define USE_STEREO_OUTPUT                                       TRUE
#define LOOPS_USED                                              U3232_LOOPS
#define USE_DROP_SAMPLE                                         FALSE
#define USE_TERP1                                               FALSE
#define USE_TERP2                                               TRUE
#define USE_NEW_EFFECTS                                         TRUE
#define FILE_NAME_LENGTH                                        1024
#define USE_DEVICE_ENUM_SUPPORT                                 TRUE
#define USE_CALLBACKS                                           TRUE
#define USE_CREATION_API                                        TRUE
#define BAE_COMPLETE                                            1
#define USE_STREAM_API                                          TRUE
// Overwritable Flags -- default values
// ----------------------------------------------
#ifndef REVERB_USED
	//#define REVERB_USED       REVERB_DISABLED
	#define REVERB_USED			VARIABLE_REVERB

#endif
#ifndef USE_FULL_RMF_SUPPORT
        #define USE_FULL_RMF_SUPPORT            TRUE
#endif

#ifndef USE_CREATION_API
        #define USE_CREATION_API                        TRUE
#endif

#ifndef USE_HIGHLEVEL_FILE_API
        #define USE_HIGHLEVEL_FILE_API          TRUE
#endif

#ifndef USE_STREAM_API
        #define USE_STREAM_API                          TRUE
#endif

#ifndef USE_CAPTURE_API
        #define USE_CAPTURE_API                         FALSE
#endif

#ifndef USE_MOD_API
        #define USE_MOD_API                                     FALSE
#endif

#ifndef USE_MPEG_ENCODER
        #define USE_MPEG_ENCODER                        FALSE
#endif

#ifndef USE_MPEG_DECODER
        #define USE_MPEG_DECODER                        FALSE
#endif


// INLINE
// ------------------------
#ifndef __MOTO__
        #define INLINE                                          inline
#else
        #define INLINE
#endif


// DEBUG_STR
// ------------------------
#ifndef DEBUG_STR
        #if USE_DEBUG == 0
                #define DEBUG_STR(x)
        #else
                #include <stdio.h>
        #endif
                #if USE_DEBUG == 1
                #define DEBUG_STR(x)    DebugStr((unsigned char *)(x))
        #endif
                #if USE_DEBUG == 2
extern short int drawDebug;
                #ifdef __cplusplus
extern "C" {
                #endif
extern void DPrint(short int d, ...);

                #ifdef __cplusplus
}
                #endif
                #define DEBUG_STR(x)    DPrint(drawDebug, "%s\r", x)
        #endif
                #if USE_DEBUG == 3
                #define DEBUG_STR(x)
extern short int drawDebug;
                #ifdef __cplusplus
extern "C" {
                #endif
extern void DPrint(short int d, ...);

                #ifdef __cplusplus
}
                #endif
                #define DEBUG_STR2(x)    DPrint(drawDebug, "%s\r", x)
        #else
                #define DEBUG_STR2(x)
        #endif
#endif


// MACROS
// ----------------------------------------------
#if 0           // this is for my testing. Please keep set to 0!!!
        #undef  USE_DROP_SAMPLE
        #define USE_DROP_SAMPLE                                 FALSE
        #undef  USE_TERP1
        #define USE_TERP1                                       FALSE
        #undef  USE_8_BIT_OUTPUT
        #define USE_8_BIT_OUTPUT                                FALSE
        #undef  USE_MONO_OUTPUT
        #define USE_MONO_OUTPUT                                 FALSE
        #undef  USE_CREATION_API
        #define USE_CREATION_API                                FALSE
        #undef  USE_MOD_API
        #define USE_MOD_API                                     FALSE
        #undef  USE_HIGHLEVEL_FILE_API
        #define USE_HIGHLEVEL_FILE_API                          FALSE
        #undef  USE_STREAM_API
        #define USE_STREAM_API                                  FALSE
        #undef  USE_FULL_RMF_SUPPORT
        #define USE_FULL_RMF_SUPPORT                            FALSE
        #undef  USE_CAPTURE_API
        #define USE_CAPTURE_API                                 FALSE
#endif

#if BAE_EDITOR == TRUE
        #undef  USE_MOD_API
        #define USE_MOD_API                                     FALSE
        #undef  USE_STREAM_API
        #define USE_STREAM_API                                  FALSE
        #undef  USE_CAPTURE_API
        #define USE_CAPTURE_API                                 FALSE
#endif

#if BAE_STANDALONE == TRUE
        #undef  USE_8_BIT_OUTPUT
        #define USE_8_BIT_OUTPUT                                FALSE
        #undef  USE_MONO_OUTPUT
        #define USE_MONO_OUTPUT                                 FALSE
        #undef  USE_CREATION_API
        #define USE_CREATION_API                                FALSE
        #undef  USE_MOD_API
        #define USE_MOD_API                                     FALSE
        #undef  USE_HIGHLEVEL_FILE_API
        #define USE_HIGHLEVEL_FILE_API                          FALSE
        #undef  USE_STREAM_API
        #define USE_STREAM_API                                  FALSE
        #undef  USE_CAPTURE_API
        #define USE_CAPTURE_API                                 FALSE
#endif

#if BAE_PLUGIN == TRUE
        #undef  USE_MOD_API
        #define USE_MOD_API                                     FALSE
        #undef  USE_8_BIT_OUTPUT
        #define USE_8_BIT_OUTPUT                                FALSE
        #undef  USE_MONO_OUTPUT
        #define USE_MONO_OUTPUT                                 FALSE
        #undef  USE_CREATION_API
        #define USE_CREATION_API                                FALSE
        #undef  USE_CAPTURE_API
        #define USE_CAPTURE_API                                 FALSE
        #undef  USE_NEW_EFFECTS
        #define USE_NEW_EFFECTS                                 TRUE
        #undef  USE_MPEG_DECODER
        #define USE_MPEG_DECODER                                TRUE
#endif

#if JAVA_SOUND == TRUE
        #undef  USE_CAPTURE_API
        #define USE_CAPTURE_API                                 FALSE
        #undef  USE_CREATION_API
        #define USE_CREATION_API                                FALSE
        #undef  USE_MOD_API
        #define USE_MOD_API                                     FALSE
        #undef  USE_HIGHLEVEL_FILE_API
        #define USE_HIGHLEVEL_FILE_API                          FALSE
        #undef  USE_FULL_RMF_SUPPORT
        #define USE_FULL_RMF_SUPPORT                            FALSE
#endif

#if CPU_TYPE == k68000
        #undef  REVERB_USED
        #define REVERB_USED                                     SMALL_MEMORY_REVERB
        #undef  USE_FLOAT
        #define USE_FLOAT                                       FALSE
#endif
