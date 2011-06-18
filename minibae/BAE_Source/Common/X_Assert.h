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
**  X_Assert.h
**
**  This provides for platform specfic functions that need to be rewitten,
**  but will provide a API that is stable across all platforms
**
**  © Copyright 1995-2000 Beatnik, Inc, All Rights Reserved.
**  Written by Christopher Schardt
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
** Overview
**  platform-dependent BAE_ASSERT() and BAE_VERIFY() macros
**
**  History -
**  5/7/99      MOE: created
**  7/13/99     Renamed HAE to BAE
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  5/29/2001   sh  Added new debugging system with BAE_PRINTF
**  6/4/2001    sh  Eliminated use of ... in macro for windows
*/
/*****************************************************************************/

#ifndef X_Assert_H
#define X_Assert_H


#include "X_API.h"

// new BAE_PRINTF system
//  BAE_PRINTF("This is a test of me %d %s\n", 34, "hello");
//      actaully does work because the BAE_PRINTF macro is defined as printf.
//      To disable you define BAE_PRINTF(...) as blank and it eats everything
//      in between the () and does nothing.


#ifndef _DEBUG
    #if (X_PLATFORM == X_WIN95) || (X_PLATFORM == X_WIN_HARDWARE) || (X_PLATFORM == X_MACINTOSH) || (X_PLATFORM == X_IOS)
        #define BAE_PRINTF
    #else
        #define BAE_PRINTF(...)
    #endif
    #define BAE_ASSERT(exp)         ((void)0)
    #define BAE_VERIFY(exp)         (exp)
#else
    #if (X_PLATFORM == X_WIN95) || (X_PLATFORM == X_WIN_HARDWARE) || (X_PLATFORM == X_MACINTOSH) || (X_PLATFORM == X_IOS)
        #define BAE_PRINTF          printf      
        #ifdef ASSERT
            #define BAE_ASSERT(exp)     ASSERT(exp)
            #define BAE_VERIFY(exp)     ASSERT(exp)
        #else
            #include <assert.h>
            #define BAE_ASSERT(exp)     assert(exp)
            #define BAE_VERIFY(exp)     assert(exp)
        #endif
    #else
        #include <assert.h>
        #define BAE_ASSERT(exp)     assert(exp)
        #define BAE_VERIFY(exp)     assert(exp)
    #endif

#endif
    
#define HAE_ASSERT                      BAE_ASSERT
#define HAE_VERIFY                      BAE_VERIFY

#endif  // X_Assert_H


