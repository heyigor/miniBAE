/*****************************************************************************/
/*
** "GenSeqTools.h"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	© Copyright 1993-1999 Beatnik, Inc, All Rights Reserved.
**	Written by Jim Nitchals and Steve Hales
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
** Overview
**	The purpose of this layer of code is to remove Macintosh Specific code.
**	No file, or memory access. All functions are passed pointers to data
**	that needs to be passed into the mixer, and MIDI sequencer
**
** Modification History
**
**	12/7/99		Created
**	12/13/99	MSD: changed XGet... functions to accept byteOffset param
**	12/15/99	Added String comparision functions.
*/
/*****************************************************************************/

#ifndef G_SEQ_TOOLS
#define G_SEQ_TOOLS

#ifdef __cplusplus
	extern "C" {
#endif

XWORD		XGetShortWordAligned(XWORD *pData, long byteOffset);
XDWORD		XGetLongWordAligned(XWORD *pData, long byteOffset);
XWORD		XGetCharWordAligned(XWORD *pData, long byteOffset);

short int	XLStrnCmpWordAligned(const char* s1, XWORD *s2, long byteOffset, long n);
long		XStrnToLongWordAligned(XWORD* pData, long byteOffset, long length);

#ifdef __cplusplus
	}
#endif

#endif /* GenSeqTools.h */

