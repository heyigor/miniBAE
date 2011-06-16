/*****************************************************************************/
/*
**	X_UnpackStructures.h
**
**		Include this to reset the pack structures to default
**
**	© Copyright 2001 Beatnik, Inc, All Rights Reserved.
**	Written by Steve Hales
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
**	History	-
**	5/23/2001	Created
*/
/*****************************************************************************/

#ifndef __X_API__
	#include "X_API.h"
#endif

#if (((X_PLATFORM == X_MACINTOSH) || (X_PLATFORM == X_IOS)) && (COMPILER_TYPE == GCC_COMPILER))
	#ifdef __cplusplus
		#pragma pack()
	#endif
#else
	#if CPU_TYPE == kRISC
		#pragma options align=reset
	#endif

	// $$jdr: This should be pack(), but for a
	// bug in the compilers we're using.
	#if (CPU_TYPE == kSPARC)
		#pragma pack(4)
	#endif

	// $$kk: 08.12.98 merge: changed this 
	/* $$kk: pack(4) for solaris x86 */
	#if ((CPU_TYPE == k80X86) || (CPU_TYPE == kARM))
		#if (X_PLATFORM == X_SOLARIS)
			#pragma pack (4)
		#else
			#pragma pack ()
		#endif
	#endif
#endif

#undef X_PACKBY1
#define X_PACKBY1

#undef X_PACK_FAST

