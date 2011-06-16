/*****************************************************************************/
/*
**	X_PackStructures.h
**
**		Include this to pack structures on a byte boundery.
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

#undef X_PACKBY1
#undef X_BF_1
#undef X_BF_2
#undef X_BF_3
#undef X_BF_4
#undef X_BF_5

#ifndef X_PACK_FAST
// controls to enable structure packing by 1 byte
	#if (X_PLATFORM == X_MACINTOSH) || (X_PLATFORM == X_IOS)
		#define X_PACKBY1	__attribute__ ((packed))
		#define X_BF_1	:1
		#define X_BF_2	:2
		#define X_BF_3	:3
		#define X_BF_4	:4
		#define X_BF_5	:5

		#ifdef __cplusplus
			// gcc++ bug
			#pragma pack(1)
		#endif
	#else
		#if CPU_TYPE == kRISC
			#pragma options align=mac68k
		#endif
		#if ((CPU_TYPE == k80X86) || (CPU_TYPE == kSPARC) || (CPU_TYPE == kARM))
			#pragma pack (1)
		#endif
		#error unknown X_PLATFORM type
		
		// This define is used when declaring the structures. Some compilers, like GCC
		// need to use '__attribute__ ((packed))' at each structure to pack by a byte.

		//#define X_PACKBY1	__attribute__ ((packed))
		#define X_PACKBY1

		#define X_BF_1
		#define X_BF_2
		#define X_BF_3
		#define X_BF_4
		#define X_BF_5
	#endif

#else

// controls to enable structure packing by 4/8 bytes.
	#if X_PLATFORM == X_BE
		#pragma pack (4)
	#endif
	#if (((X_PLATFORM == X_MACINTOSH) || (X_PLATFORM == X_IOS)) && (COMPILER_TYPE == GCC_COMPILER))		
		#pragma pack (4)
	#else
		#if CPU_TYPE == kRISC
			#pragma options align=power
		#endif
		/* Change pack(8) to pack(4) for SPARC.				-Liang */
		#if (CPU_TYPE == kSPARC)
			#pragma pack (4)
		#endif
		/* $$kk: pack(4) for solaris x86 */
		#if (CPU_TYPE == k80X86)
			#if (X_PLATFORM == X_SOLARIS)
				#pragma pack (4)
			#else
				#pragma pack (8)
			#endif
		#endif
	#endif

	#define X_BF_1
	#define X_BF_2
	#define X_BF_3
	#define X_BF_4
	#define X_BF_5

	#define X_PACKBY1

#endif
