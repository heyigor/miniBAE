/*
    Copyright (c) 2009 Beatnik, Inc All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
    
    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
    
    Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    
    Neither the name of the Beatnik, Inc nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
    TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*****************************************************************************/
/*
**  BAEBuildOptions_WinOS.h
**
**  © Copyright 1999-2000 Beatnik, Inc, All Rights Reserved.
**
**  Beatnik products contain certain trade secrets and confidential and
**  proprietary information of Beatnik.  Use, reproduction, disclosure
**  and distribution by any means are prohibited, except pursuant to
**  a written license from Beatnik. Use of copyright notice is
**  precautionary and does not imply publication or disclosure.
**
**  Restricted Rights Legend:
**  Use, duplication, or disclosure by the Government is subject to
**  restrictions as set forth in subparagraph (c)(1)(ii) of The
**  Rights in Technical Data and Computer Software clause in DFARS
**  252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
**  Computer Software--Restricted Rights at 48 CFR 52.227-19, as
**  applicable.
**
**  Confidential-- Internal use only
**
**  Modification History:
**  10/19/99    MSD:    Created -- extracted from X_API.h
**  11/10/99    Allowed more defined to be overridden.
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  2/17/2000   Changed to support streaming for plugin and editors
*/
/*****************************************************************************/

#ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif


// #includes
// ----------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <windowsx.h>
#include <commdlg.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <mmsystem.h>   // for timeGetTime()


// Non-overwritable Flags
// ----------------------------------------------
#define COMPILER_TYPE                   DEFAULT_COMPILER
#define CPU_TYPE                        k80X86
#define X_WORD_ORDER                    TRUE    // swap words
#define USE_FLOAT                       TRUE
#define USE_8_BIT_OUTPUT                TRUE
#define USE_16_BIT_OUTPUT               TRUE
#define USE_MONO_OUTPUT                 TRUE
#define USE_STEREO_OUTPUT               TRUE
#define USE_TERP2                       TRUE
#define FILE_NAME_LENGTH                _MAX_PATH
#define USE_DEVICE_ENUM_SUPPORT         TRUE


// Overwritable Flags -- default values
// ----------------------------------------------

#ifndef LOOPS_USED      
    #define LOOPS_USED                  U3232_LOOPS
#endif

#if LOOPS_USED == LIMITED_LOOPS
    #ifndef USE_DROP_SAMPLE
        #define USE_DROP_SAMPLE         TRUE
    #endif

    #ifndef USE_TERP1
        #define USE_TERP1               TRUE
    #endif
#endif

#ifndef REVERB_USED     
    #define REVERB_USED                 VARIABLE_REVERB
#endif

#if REVERB_USED == REVERB_DISABLED
    #define USE_NEW_EFFECTS             FALSE
#else
    #define USE_NEW_EFFECTS             TRUE
#endif

#ifndef USE_FULL_RMF_SUPPORT        
    #define USE_FULL_RMF_SUPPORT        TRUE
#endif

#ifndef USE_CREATION_API
    #define USE_CREATION_API            TRUE
#endif

#ifndef USE_HIGHLEVEL_FILE_API
    #define USE_HIGHLEVEL_FILE_API      TRUE
#endif

#ifndef USE_STREAM_API
    #define USE_STREAM_API              TRUE
#endif

#ifndef USE_CAPTURE_API
    #define USE_CAPTURE_API             TRUE
#endif

#ifndef USE_MOD_API
    #define USE_MOD_API                 FALSE
#endif

#ifndef USE_MPEG_ENCODER
    #define USE_MPEG_ENCODER            FALSE
#endif

#ifndef USE_MPEG_DECODER
    #define USE_MPEG_DECODER            FALSE
#endif


// INLINE
// ----------------------------------------------
#define INLINE                          _inline


// DEBUG_STR
// ----------------------------------------------
#ifndef DEBUG_STR
    #if USE_DEBUG == 0
        #define DEBUG_STR(x)
    #endif
    #if USE_DEBUG == 1
        #define DEBUG_STR(x)            fprintf(stderr, x)
    #endif
#endif


// MACROS
// ----------------------------------------------
#if BAE_VXD
    #undef  USE_FLOAT
    #define USE_FLOAT                   FALSE
    #undef  USE_CREATION_API
    #define USE_CREATION_API            FALSE
    #undef  USE_HIGHLEVEL_FILE_API
    #define USE_HIGHLEVEL_FILE_API      FALSE
    #undef  USE_STREAM_API
    #define USE_STREAM_API              FALSE
    #undef  USE_CAPTURE_API
    #define USE_CAPTURE_API             FALSE
    #undef  USE_MOD_API
    #define USE_MOD_API                 FALSE
    #undef  USE_NEW_EFFECTS
    #define USE_NEW_EFFECTS             TRUE
#endif

#if BAE_EDITOR == TRUE
    #undef  USE_MOD_API
    #define USE_MOD_API                 FALSE
    #undef  USE_STREAM_API
    #define USE_STREAM_API              TRUE
    #undef  USE_CAPTURE_API
    #define USE_CAPTURE_API             FALSE
#endif

#if BAE_STANDALONE == TRUE
    #undef  USE_8_BIT_OUTPUT
    #define USE_8_BIT_OUTPUT            FALSE
    #undef  USE_MONO_OUTPUT
    #define USE_MONO_OUTPUT             FALSE
    #undef  USE_CREATION_API
    #define USE_CREATION_API            FALSE
    #undef  USE_MOD_API
    #define USE_MOD_API                 FALSE
    #undef  USE_HIGHLEVEL_FILE_API
    #define USE_HIGHLEVEL_FILE_API      FALSE
    #undef  USE_STREAM_API
    #define USE_STREAM_API              FALSE
    #undef  USE_CAPTURE_API
    #define USE_CAPTURE_API             FALSE
#endif

#if BAE_PLUGIN == TRUE
    #undef  USE_MOD_API
    #define USE_MOD_API                 FALSE
    #undef  USE_8_BIT_OUTPUT
    #define USE_8_BIT_OUTPUT            FALSE
    #undef  USE_MONO_OUTPUT
    #define USE_MONO_OUTPUT             FALSE
    #undef  USE_CREATION_API
    #define USE_CREATION_API            FALSE
    #undef  USE_CAPTURE_API
    #define USE_CAPTURE_API             FALSE
    #undef  USE_NEW_EFFECTS
    #define USE_NEW_EFFECTS             TRUE
    #undef  USE_MPEG_DECODER
    #define USE_MPEG_DECODER            TRUE
    #undef  USE_STREAM_API
    #define USE_STREAM_API              TRUE
#endif

#if JAVA_SOUND == TRUE
    #undef  USE_CREATION_API
    #define USE_CREATION_API            FALSE
    #undef  USE_MOD_API
    #define USE_MOD_API                 FALSE
    #undef  USE_HIGHLEVEL_FILE_API
    #define USE_HIGHLEVEL_FILE_API      FALSE
    #undef  USE_FULL_RMF_SUPPORT
    #define USE_FULL_RMF_SUPPORT        FALSE
#endif
