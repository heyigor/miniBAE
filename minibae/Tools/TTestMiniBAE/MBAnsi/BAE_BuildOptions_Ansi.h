/*****************************************************************************/
/*
**	BAE_BuildOptions_Ansi.h
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
*/
/*****************************************************************************/


// #includes
// ----------------------------------------------
#include <stdlib.h>

// Non-overwritable Flags
// ----------------------------------------------
#define COMPILER_TYPE					DEFAULT_COMPILER
#define CPU_TYPE						k80X86
#define X_WORD_ORDER					TRUE	// swap words
#define USE_FLOAT						TRUE
#define USE_8_BIT_OUTPUT				TRUE
#define USE_16_BIT_OUTPUT				TRUE
#define USE_MONO_OUTPUT					TRUE
#define USE_STEREO_OUTPUT				FALSE
#define USE_TERP2						TRUE
#define FILE_NAME_LENGTH				_MAX_PATH
#define USE_DEVICE_ENUM_SUPPORT			TRUE


// Overwritable Flags -- default values
// ----------------------------------------------

#ifndef LOOPS_USED		
	#define LOOPS_USED					U3232_LOOPS
#endif

#if LOOPS_USED == LIMITED_LOOPS
	#ifndef USE_DROP_SAMPLE
		#define USE_DROP_SAMPLE			TRUE
	#endif

	#ifndef USE_TERP1
		#define USE_TERP1				TRUE
	#endif
#endif

#ifndef REVERB_USED		
	#define REVERB_USED					VARIABLE_REVERB
#endif

#if REVERB_USED == REVERB_DISABLED
	#define USE_NEW_EFFECTS				FALSE
#else
	#define USE_NEW_EFFECTS				TRUE
#endif

#ifndef USE_FULL_RMF_SUPPORT		
	#define USE_FULL_RMF_SUPPORT		TRUE
#endif

#ifndef USE_CREATION_API
	#define USE_CREATION_API			FALSE
#endif

#ifndef USE_HIGHLEVEL_FILE_API
	#define USE_HIGHLEVEL_FILE_API		FALSE
#endif

#ifndef USE_STREAM_API
	#define USE_STREAM_API				FALSE
#endif

#ifndef USE_CAPTURE_API
	#define USE_CAPTURE_API				FALSE
#endif

#ifndef USE_MOD_API
	#define USE_MOD_API					FALSE
#endif

#ifndef USE_MPEG_ENCODER
	#define USE_MPEG_ENCODER			FALSE
#endif

#ifndef USE_MPEG_DECODER
	#define USE_MPEG_DECODER			FALSE
#endif


// INLINE
// ----------------------------------------------
#define INLINE							_inline


// DEBUG_STR
// ----------------------------------------------
#ifndef DEBUG_STR
	#if USE_DEBUG == 0
		#define DEBUG_STR(x)
	#endif
	#if USE_DEBUG == 1
		#define DEBUG_STR(x)			fprintf(stderr, x)
	#endif
#endif


// MACROS
// ----------------------------------------------
#if BAE_VXD
	#undef 	USE_FLOAT
	#define	USE_FLOAT					FALSE
	#undef	USE_CREATION_API
	#define	USE_CREATION_API			FALSE
	#undef	USE_HIGHLEVEL_FILE_API
	#define	USE_HIGHLEVEL_FILE_API		FALSE
	#undef	USE_STREAM_API
	#define	USE_STREAM_API				FALSE
	#undef	USE_CAPTURE_API
	#define	USE_CAPTURE_API				FALSE
	#undef	USE_MOD_API
	#define	USE_MOD_API					FALSE
	#undef	USE_NEW_EFFECTS
	#define USE_NEW_EFFECTS				TRUE
#endif

#if BAE_EDITOR == TRUE
	#undef	USE_MOD_API
	#define	USE_MOD_API					FALSE
	#undef	USE_STREAM_API
	#define	USE_STREAM_API				FALSE
	#undef	USE_CAPTURE_API
	#define	USE_CAPTURE_API				FALSE
#endif

#if BAE_STANDALONE == TRUE
	#undef	USE_8_BIT_OUTPUT
	#define	USE_8_BIT_OUTPUT			FALSE
	#undef	USE_MONO_OUTPUT
	#define	USE_MONO_OUTPUT				FALSE
	#undef	USE_CREATION_API
	#define	USE_CREATION_API			FALSE
	#undef	USE_MOD_API
	#define	USE_MOD_API					FALSE
	#undef	USE_HIGHLEVEL_FILE_API
	#define	USE_HIGHLEVEL_FILE_API		FALSE
	#undef	USE_STREAM_API
	#define	USE_STREAM_API				FALSE
	#undef	USE_CAPTURE_API
	#define	USE_CAPTURE_API				FALSE
#endif

#if BAE_PLUGIN == TRUE
	#undef	USE_STREAM_API
	#define	USE_STREAM_API				FALSE
	#undef	USE_MOD_API
	#define	USE_MOD_API					FALSE
	#undef	USE_8_BIT_OUTPUT
	#define	USE_8_BIT_OUTPUT			FALSE
	#undef	USE_MONO_OUTPUT
	#define	USE_MONO_OUTPUT				FALSE
	#undef	USE_CREATION_API
	#define	USE_CREATION_API			FALSE
	#undef	USE_CAPTURE_API
	#define	USE_CAPTURE_API				FALSE
	#undef	USE_NEW_EFFECTS
	#define USE_NEW_EFFECTS				TRUE
	#undef	USE_MPEG_DECODER
	#define USE_MPEG_DECODER			TRUE
#endif

#if JAVA_SOUND == TRUE
	#undef	USE_CREATION_API
	#define	USE_CREATION_API			FALSE
	#undef	USE_MOD_API
	#define	USE_MOD_API					FALSE
	#undef	USE_HIGHLEVEL_FILE_API
	#define	USE_HIGHLEVEL_FILE_API		FALSE
	#undef	USE_FULL_RMF_SUPPORT
	#define	USE_FULL_RMF_SUPPORT		FALSE
#endif
