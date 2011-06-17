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
** "GenOutput.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**	Confidential-- Internal use only
**
**	© Copyright 1993-2001 Beatnik, Inc, All Rights Reserved.
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
** Modification History:
**
**	4/21/96		Removed register usage in parameters
**	11/21/96	Added PHASE_OFFSET macro to allow for zero based 8 bit DACs
**	12/30/96	Changed copyright
**	1/22/97		Added support for MOD code in all output loops
**	6/10/97		Moved MOD support code to GenSynth.c prior to output stage
**	7/28/97		Merged Sun timing test code
**	11/10/97	Flipped phase of 8 bit output for Solaris
**	12/16/97	Moe: removed compiler warnings
**	2/3/98		Renamed songBufferLeftMono to songBufferDry
**	2/11/98		Added support for Q_24K & Q_48K & Q_8K
**	2/13/98		Removed sun's timing code
**	2/15/98		Added more obvious support for 11 terped to 22 and 22 terped to 44
**
**	6/5/98		Jim Nitchals RIP	1/15/62 - 6/5/98
**				I'm going to miss your irreverent humor. Your coding style and desire
**				to make things as fast as possible. Your collaboration behind this entire
**				codebase. Your absolute belief in creating the best possible relationships 
**				from honesty and integrity. Your ability to enjoy conversation. Your business 
**				savvy in understanding the big picture. Your gentleness. Your willingness 
**				to understand someone else's way of thinking. Your debates on the latest 
**				political issues. Your generosity. Your great mimicking of cartoon voices. 
**				Your friendship. - Steve Hales
**
**	11/23/98	Added support for Intel Katmai CPU
**	1/4/99		Re ordered Katmai code and duplicated inner loops
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	4/19/2001	sh	Put wrapper for conditional compiles.
*/
/*****************************************************************************/

#include "GenSnd.h"
#include "GenPriv.h"
#include "X_Assert.h"

#ifdef BAE_COMPLETE

// This is 8 bit phase. Some hardware wants silence to be 0, some wants it
// to be 128.
#if ( (X_PLATFORM == X_BE) 				||	\
	  (X_PLATFORM == X_SOLARIS) )
	#define PHASE_OFFSET	0		// silence is 0
#else
	#define PHASE_OFFSET	0x80	// silence is 128
#endif

#if (USE_8_BIT_OUTPUT == TRUE) && (USE_STEREO_OUTPUT == TRUE)
void PV_Generate8outputStereo(OUTSAMPLE8 * dest8)
{
	register INT32			b, c;
	register INT32			*sourceLR;
	register LOOPCOUNT		count;

	/* Convert intermediate 16-bit sample format to 16 bit output samples:
	*/
	sourceLR = &MusicGlobals->songBufferDry[0];

	if ( (MusicGlobals->outputRate != Q_RATE_11K_TERP_22K) && (MusicGlobals->outputRate != Q_RATE_22K_TERP_44K) )
	{
		// native sample rates
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			dest8[0] = (sourceLR[0] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[1] = (sourceLR[1] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[2] = (sourceLR[2] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[3] = (sourceLR[3] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[4] = (sourceLR[4] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[5] = (sourceLR[5] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[6] = (sourceLR[6] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[7] = (sourceLR[7] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8 += 8;
			sourceLR += 8;
		}
	}
	else
	{                   
		// 11k terped to 22k, and 22k terped to 44k
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			b =  (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			c =  (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[0] = (OUTSAMPLE8)b;
			dest8[1] = (OUTSAMPLE8)c;
			dest8[2] = (OUTSAMPLE8)b;
			dest8[3] = (OUTSAMPLE8)c;

			b = (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			c = (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[4] = (OUTSAMPLE8)b;
			dest8[5] = (OUTSAMPLE8)c;
			dest8[6] = (OUTSAMPLE8)b;
			dest8[7] = (OUTSAMPLE8)c;

			b = (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			c = (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[8] = (OUTSAMPLE8)b;
			dest8[9] = (OUTSAMPLE8)c;
			dest8[10] = (OUTSAMPLE8)b;
			dest8[11] = (OUTSAMPLE8)c;

			b = (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			c = (*sourceLR++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[12] = (OUTSAMPLE8)b;
			dest8[13] = (OUTSAMPLE8)c;
			dest8[14] = (OUTSAMPLE8)b;
			dest8[15] = (OUTSAMPLE8)c;
			dest8 += 16;
		}
	}
}
#endif	// (USE_8_BIT_OUTPUT == TRUE) && (USE_STEREO_OUTPUT == TRUE)

#if (USE_8_BIT_OUTPUT == TRUE) && (USE_MONO_OUTPUT == TRUE)
void PV_Generate8outputMono(OUTSAMPLE8 * dest8)
{
	register LOOPCOUNT		count;
	register INT32			b;
	register INT32			*source;

	/* Convert intermediate 16-bit sample format to 16 bit output samples:
	*/
	source = &MusicGlobals->songBufferDry[0];

	/* Here's how to add a buzz if we want to make sure the Sound Manager's alive.
	*/
//		source[0] += 0x60;
	if ( (MusicGlobals->outputRate != Q_RATE_11K_TERP_22K) && (MusicGlobals->outputRate != Q_RATE_22K_TERP_44K) )
	{
		// native sample rates
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			dest8[0] = (source[0] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[1] = (source[1] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[2] = (source[2] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[3] = (source[3] >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			source += 4;
			dest8 += 4;
		}
	}
	else
	{                   
		// 11k terped to 22k, and 22k terped to 44k
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			b = (*source++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[0] = (OUTSAMPLE8)b;
			dest8[1] = (OUTSAMPLE8)b;

			b = (*source++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[2] = (OUTSAMPLE8)b;
			dest8[3] = (OUTSAMPLE8)b;

			b = (*source++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[4] = (OUTSAMPLE8)b;
			dest8[5] = (OUTSAMPLE8)b;

			b = (*source++ >> (OUTPUT_SCALAR + 8)) + PHASE_OFFSET;
			dest8[6] = (OUTSAMPLE8)b;
			dest8[7] = (OUTSAMPLE8)b;
			dest8 += 8;
		}
	}
}
#endif	// (USE_8_BIT_OUTPUT == TRUE) && (USE_MONO_OUTPUT == TRUE)

#if (USE_16_BIT_OUTPUT == TRUE) && (USE_STEREO_OUTPUT == TRUE)
void PV_Generate16outputStereo(OUTSAMPLE16 * dest16)
{
	register LOOPCOUNT	count;
#if X_PLATFORM != X_WEBTV
	register INT32		b, c;
#endif
	register INT32		*sourceLR;
	register INT32		i, overflow_test, k8000;

	/* Convert intermediate 16-bit sample format to 16 bit output samples:
	*/
	sourceLR = &MusicGlobals->songBufferDry[0];

	k8000 = 0x8000;
	if ( (MusicGlobals->outputRate != Q_RATE_11K_TERP_22K) && (MusicGlobals->outputRate != Q_RATE_22K_TERP_44K) )
	{
		{
			// native sample rates
			for (count = MusicGlobals->Four_Loop; count > 0; --count)
			{
				i = sourceLR[0] >> OUTPUT_SCALAR;
				overflow_test = i + k8000;
				dest16[0] = (OUTSAMPLE16)i;
				i = sourceLR[1] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[1] = (OUTSAMPLE16)i;

				i = sourceLR[2] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[2] = (OUTSAMPLE16)i;
				i = sourceLR[3] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[3] = (OUTSAMPLE16)i;

				i = sourceLR[4] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[4] = (OUTSAMPLE16)i;
				i = sourceLR[5] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[5] = (OUTSAMPLE16)i;

				i = sourceLR[6] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[6] = (OUTSAMPLE16)i;
				i = sourceLR[7] >> OUTPUT_SCALAR;
				overflow_test |= i + k8000;
				dest16[7] = (OUTSAMPLE16)i;

				if (overflow_test & 0xFFFF0000)
				{
					i = sourceLR[0] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[0] = (OUTSAMPLE16)(i - k8000);
					i = sourceLR[1] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[1] = (OUTSAMPLE16)(i - k8000);
		
					i = sourceLR[2] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[2] = (OUTSAMPLE16)(i - k8000);
					i = sourceLR[3] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[3] = (OUTSAMPLE16)(i - k8000);
		
					i = sourceLR[4] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[4] = (OUTSAMPLE16)(i - k8000);
					i = sourceLR[5] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[5] = (OUTSAMPLE16)(i - k8000);
		
					i = sourceLR[6] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[6] = (OUTSAMPLE16)(i - k8000);
					i = sourceLR[7] >> OUTPUT_SCALAR;
					i += k8000;
					if (i & 0xFFFF0000)
						{ if (i > 0) i = 0xFFFF; else i = 0;}
					dest16[7] = (OUTSAMPLE16)(i - k8000);
				}
				sourceLR += 8; dest16 += 8;
			}
		}
	}
#if X_PLATFORM != X_WEBTV
	else
	{
		// 11k terped to 22k, and 22k terped to 44k
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			b = *sourceLR++ >> OUTPUT_SCALAR;
			b += k8000;
			if (b & 0xFFFF0000)
				{ if (b > 0) b = 0xFFFF; else b = 0;}
			b -= k8000;
			c = *sourceLR++ >> OUTPUT_SCALAR;
			c += k8000;
			if (c & 0xFFFF0000)
				{ if (c > 0) c = 0xFFFF; else c = 0;}
			c -= k8000;
			dest16[0] = (OUTSAMPLE16)b;
			dest16[1] = (OUTSAMPLE16)c; 
			dest16[2] = (OUTSAMPLE16)b;
			dest16[3] = (OUTSAMPLE16)c; 

			b = *sourceLR++ >> OUTPUT_SCALAR;
			b += k8000;
			if (b & 0xFFFF0000)
				{ if (b > 0) b = 0xFFFF; else b = 0;}
			b -= k8000;
			c = *sourceLR++ >> OUTPUT_SCALAR;
			c += k8000;
			if (c & 0xFFFF0000)
				{ if (c > 0) c = 0xFFFF; else c = 0;}
			c -= k8000;
			dest16[4] = (OUTSAMPLE16)b;
			dest16[5] = (OUTSAMPLE16)c; 
			dest16[6] = (OUTSAMPLE16)b;
			dest16[7] = (OUTSAMPLE16)c; 

			b = *sourceLR++ >> OUTPUT_SCALAR;
			b += k8000;
			if (b & 0xFFFF0000)
				{ if (b > 0) b = 0xFFFF; else b = 0;}
			b -= k8000;
			c = *sourceLR++ >> OUTPUT_SCALAR;
			c += k8000;
			if (c & 0xFFFF0000)
				{ if (c > 0) c = 0xFFFF; else c = 0;}
			c -= k8000;
			dest16[8] = (OUTSAMPLE16)b;
			dest16[9] = (OUTSAMPLE16)c; 
			dest16[10] = (OUTSAMPLE16)b;
			dest16[11] = (OUTSAMPLE16)c; 

			b = *sourceLR++ >> OUTPUT_SCALAR;
			b += k8000;
			if (b & 0xFFFF0000)
				{ if (b > 0) b = 0xFFFF; else b = 0;}
			b -= k8000;

			c = *sourceLR++ >> OUTPUT_SCALAR;
			c += k8000;
			if (c & 0xFFFF0000)
				{ if (c > 0) c = 0xFFFF; else c = 0;}
			c -= k8000;
			dest16[12] = (OUTSAMPLE16)b;
			dest16[13] = (OUTSAMPLE16)c; 
			dest16[14] = (OUTSAMPLE16)b;
			dest16[15] = (OUTSAMPLE16)c; 

			dest16 += 16;
		}
	}
#endif	// X_PLATFORM != X_WEBTV
}
#endif	// (USE_16_BIT_OUTPUT == TRUE) && (USE_STEREO_OUTPUT == TRUE)

#if (USE_16_BIT_OUTPUT == TRUE) && (USE_MONO_OUTPUT == TRUE)
void PV_Generate16outputMono(OUTSAMPLE16 * dest16)
{
	register LOOPCOUNT	count;
	register INT32		i;
	register INT32		*source;
	register INT32		overflow_test;
	register INT32		k8000 = 0x8000;

	/* Convert intermediate 16-bit sample format to 16 bit output samples:
	*/
	source = &MusicGlobals->songBufferDry[0];

	/* Here's how to add a buzz if we want to make sure the Sound Manager's alive.
	*/
//		source[0] += 0x60;
	if ( (MusicGlobals->outputRate != Q_RATE_11K_TERP_22K) && (MusicGlobals->outputRate != Q_RATE_22K_TERP_44K) )
	{
		// native sample rates
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			i = source[0] >> OUTPUT_SCALAR;
			overflow_test = i + k8000;
			dest16[0] = (OUTSAMPLE16)i;

			i = source[1] >> OUTPUT_SCALAR;
			overflow_test |= i + k8000;
			dest16[1] = (OUTSAMPLE16)i;

			i = source[2] >> OUTPUT_SCALAR;
			overflow_test |= i + k8000;
			dest16[2] = (OUTSAMPLE16)i;

			i = source[3] >> OUTPUT_SCALAR;
			overflow_test |= i + k8000;
			dest16[3] = (OUTSAMPLE16)i;

			if (overflow_test & 0xFFFF0000)
			{
				i = source[0] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[0] = (OUTSAMPLE16)(i - k8000);
	
				i = source[1] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[1] = (OUTSAMPLE16)(i - k8000);
	
				i = source[2] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[2] = (OUTSAMPLE16)(i - k8000);
	
				i = source[3] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[3] = (OUTSAMPLE16)(i - k8000);
			}
			source += 4; dest16 += 4;
		}
	}
	else
	{                   
		// 11k terped to 22k, and 22k terped to 44k
		for (count = MusicGlobals->Four_Loop; count > 0; --count)
		{
			i = source[0] >> OUTPUT_SCALAR;
			overflow_test = i + k8000;
			dest16[0] = (OUTSAMPLE16)i;
			dest16[1] = (OUTSAMPLE16)i;

			i = source[1] >> OUTPUT_SCALAR;
			overflow_test |= i + k8000;
			dest16[2] = (OUTSAMPLE16)i;
			dest16[3] = (OUTSAMPLE16)i;

			i = source[2] >> OUTPUT_SCALAR;
			overflow_test |= i + k8000;
			dest16[4] = (OUTSAMPLE16)i;
			dest16[5] = (OUTSAMPLE16)i;

			i = source[3] >> OUTPUT_SCALAR;
			overflow_test |= i + k8000;
			dest16[6] = (OUTSAMPLE16)i;
			dest16[7] = (OUTSAMPLE16)i;

			if (overflow_test & 0xFFFF0000)
			{
				i = source[0] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[0] = (OUTSAMPLE16)(i - k8000);
				dest16[1] = (OUTSAMPLE16)(i - k8000);
	
				i = source[1] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[2] = (OUTSAMPLE16)(i - k8000);
				dest16[3] = (OUTSAMPLE16)(i - k8000);
	
				i = source[2] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[4] = (OUTSAMPLE16)(i - k8000);
				dest16[5] = (OUTSAMPLE16)(i - k8000);
	
				i = source[3] >> OUTPUT_SCALAR;
				i += k8000;
				if (i & 0xFFFF0000)
					{ if (i > 0) i = 0xFFFF; else i = 0;}
				dest16[6] = (OUTSAMPLE16)(i - k8000);
				dest16[7] = (OUTSAMPLE16)(i - k8000);
			}
			source += 4; dest16 += 8;
		}
	}
}
#endif	// (USE_16_BIT_OUTPUT == TRUE) && (USE_MONO_OUTPUT == TRUE)

#endif	// #ifdef BAE_COMPLETE

// EOF of GenOutput.c

