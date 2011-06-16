/*****************************************************************************/
/*
** "NewNewLZSS.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	© Copyright 1993-2000 Beatnik, Inc, All Rights Reserved.
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
**	LZSS compression and decompression functions.
**
****************
 * Haruhiko's original header:
 *	LZSS.C -- A Data Compression Program
 *	4/6/1989 Haruhiko Okumura
 *	Use, distribute, and modify this program freely.
 *	Please send me your improved versions.
 *		PC-VAN		SCIENCE
 *		NIFTY-Serve	PAF01022
 *		CompuServe	74050,1022
****************
**
**	Modification History:
**
**  89/04/06 - H. Okumura, original public domain version (MSDOS)
**	91/04/12 - M. Foley, rewritten to work, new encoder.
**	91/04/14 - J. McCormick, cleaned, optimized.
**	91/07/23 - S. Shumway, fixed bugs in findLongestMatch, doLZSSEncode.
**	91/07/23 - S. Shumway, cleaned up a lot.
**	94/02/14 - Decode routines Converted to 'C'
**	1/18/96		Spruced up for C++ extra error checking
**	2/18/96		Removed Mac stuff, and added X_API.h
**	6/20/96		Added some more wrappers around Mac code
**				Changed DisposHandle to DisposeHandle for MW 2.0
**	7/1/97		Incorporated and tested Moe's changes and improvements. Parts of it
**				failed.
**				All 68k code now works only for CW 2.0
**	12/18/97	Cleaned up some warnings
**	3/23/98		MOE: Sped up doLZSSEncode() a teeny little bit
**				The old code can be restored with a compiler switch at the function
**	4/27/98		MOE: Changed parameters to LZSSUncompress()
**				Renamed LZSSDeltaCompress() --> LZSSCompressDeltaMono8()
**				Renamed LZSSDeltaUncompress() --> LZSSUncompressDeltaMono8()
**				Created LZSSCompressDeltaStereo8(), LZSSUncompressDeltaStereo8()
**				Created LZSSCompressDeltaMono16(), LZSSUncompressDeltaMono16()
**				Created LZSSCompressDeltaStereo16(), LZSSUncompressDeltaStereo16()
**				Changed several functions to accept unsigned long instead of long
**				Fixed cast problems with DecompressHandle and CompressHandle
**	5/14/98		Turned off new encoding code in doLZSSEncode. Its not generating byte for byte
**				versions.
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
**	11/3/98		Removed all MacOS specific stuff, except for 68k assembly. Added
**				new header/copyright block
**	3/16/99		MOE:  Changed parameters of LZSSCompress...(), adding status proc
**				and ability to return "aborted" result
**				Eliminated doLZSSEncode() since its parameters were identical to 
**				those of LZSSCompress()
**	3/25/99		MOE:  Added procData parameter to functions using XCompressStatusProc
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	1/27/2002	sh Fixed warnings.
*/
/*****************************************************************************/
#include "X_API.h"

#if X_PLATFORM == X_MACINTOSH
	// if we're compiling with CW 2.0, then we can enable 68k functions
	#if __MWERKS__
		#define ASM_findLongestMatch		1				// set to 1 to use 68k function
		#define ASM_doLZSSDecode			1
	#endif
#endif


#define TOKENBITS	4L								/* number of bits used for token size	*/
#define OFFSETBITS	12L								/* number of bits used for token offset	*/
#define CODEBITS	(TOKENBITS+OFFSETBITS)			/* total token bits						*/

#define MAXTOKENS	(1L << TOKENBITS)				/* range of token sizes					*/
#define THRESHOLD	((CODEBITS / 8) + 1)			/* minimum match length					*/
#define MAXMATCH	(THRESHOLD + MAXTOKENS - 1)		/* maximum match length					*/
#define LOOKBACK	(1L << OFFSETBITS)				/* size of lookback buffer				*/

// forwards
static void			doLZSSDecode(unsigned char* srcBuffer, unsigned long srcBytes, unsigned char* dstBuffer, long dstSize);
#if USE_CREATION_API == TRUE
static void			DeltaMono8(unsigned char* pData, unsigned long frameCount);
static void			DeltaStereo8(unsigned char* pData, unsigned long frameCount);
static void			DeltaMono16(short* pData, unsigned long frameCount);
static void			DeltaStereo16(short* pData, unsigned long frameCount);
#endif
static void			UnDeltaMono8(unsigned char* pData, unsigned long frameCount);
static void			UnDeltaStereo8(unsigned char* pData, unsigned long frameCount);
static void			UnDeltaMono16(short* pData, unsigned long frameCount);
static void			UnDeltaStereo16(short* pData, unsigned long frameCount);

#if USE_CREATION_API == TRUE
long LZSSCompressDeltaMono8(XBYTE* src, unsigned long srcBytes, XBYTE* dst,
							XCompressStatusProc proc, void* procData)
{
long			dstBytes;

	DeltaMono8(src, srcBytes);
	dstBytes = LZSSCompress(src, srcBytes, dst, proc, procData);
	UnDeltaMono8(src, srcBytes);
	return dstBytes;
}
long LZSSCompressDeltaStereo8(XBYTE* src, unsigned long srcBytes, XBYTE* dst,
								XCompressStatusProc proc, void* procData)
{
unsigned long	const frameCount = srcBytes / 2;
long			dstBytes;

	DeltaStereo8(src, frameCount);
	dstBytes = LZSSCompress(src, srcBytes, dst, proc, procData);
	UnDeltaStereo8(src, frameCount);
	return dstBytes;
}
long LZSSCompressDeltaMono16(short* src, unsigned long srcBytes, XBYTE* dst,
								XCompressStatusProc proc, void* procData)
{
unsigned long	const frameCount = srcBytes / 2;
long			dstBytes;

	DeltaMono16(src, frameCount);
	dstBytes = LZSSCompress((XBYTE*)src, srcBytes, dst, proc, procData);
	UnDeltaMono16(src, frameCount);
	return dstBytes;
}
long LZSSCompressDeltaStereo16(short* src, unsigned long srcBytes, XBYTE* dst,
								XCompressStatusProc proc, void* procData)
{
unsigned long	const frameCount = srcBytes / 4;
long			dstBytes;

	DeltaStereo16(src, frameCount);
	dstBytes = LZSSCompress((XBYTE*)src, srcBytes, dst, proc, procData);
	UnDeltaStereo16(src, frameCount);
	return dstBytes;
}
#endif

void LZSSUncompress(unsigned char* src, unsigned long srcBytes,
					unsigned char* dst, unsigned long dstBytes)
{
	doLZSSDecode(src, srcBytes, dst, dstBytes);
}
void LZSSUncompressDeltaMono8(unsigned char* src, unsigned long srcBytes,
								unsigned char* dst, unsigned long dstBytes)
{
	doLZSSDecode(src, srcBytes, dst, dstBytes);
	UnDeltaMono8(dst, dstBytes);
}
void LZSSUncompressDeltaStereo8(unsigned char* src, unsigned long srcBytes,
								unsigned char* dst, unsigned long dstBytes)
{
	doLZSSDecode(src, srcBytes, dst, dstBytes);
	UnDeltaStereo8(dst, dstBytes / 2);
}
void LZSSUncompressDeltaMono16(unsigned char* src, unsigned long srcBytes,
								short* dst, unsigned long dstBytes)
{
	doLZSSDecode(src, srcBytes, (unsigned char*)dst, dstBytes);
	UnDeltaMono16(dst, dstBytes / 2);
}
void LZSSUncompressDeltaStereo16(unsigned char* src, unsigned long srcBytes,
									short* dst, unsigned long dstBytes)
{
	doLZSSDecode(src, srcBytes, (unsigned char*)dst, dstBytes);
	UnDeltaStereo16(dst, dstBytes / 4);
}




#if USE_CREATION_API == TRUE

#if (X_PLATFORM == X_MACINTOSH) && (CPU_TYPE == k68000) && (ASM_findLongestMatch)

/* -------------------------------------------------------------------------------- *
 * Function: findLongestMatch
 *
 * Description:
 *	This routine finds the longest match of the string starting at patternStart
 *  in the previous LOOKBACK characters.  If it finds a match of from THRESHOLD bytes
 *	to MAXMATCH bytes, it encodes the position as a code word and places the value in 
 *	codeWord and returns the number of characters matched.
 *	If it does not find a match, it returns 0.
 *
 * Parameters:
 *	unsigned char * theData;			-- the buffer of data to search.
 *	long	dataLen;			-- the size of the buffer
 *	long	patternStart;		-- the buffer position of the pattern string.
 *	short*	codeWord;			-- encoded token: AAAA BBBB BBBB BBBB
 *									A = size of match string - THRESHOLD
 *									B = offset of match string from patternStart - LOOKBACK
 * Result:
 *	int 						-- If a match of THRESHOLD or more characters made,
 *									number of characters matched is returned.
 *								-- If no match is found, 0 is returned.
 *
 * -------------------------------------------------------------------------------- */

static asm short int findLongestMatch(unsigned char * theData, unsigned long dataLen, unsigned long patternStart, unsigned short* codeWord)
{
		fralloc +
		MOVEM.L		D2-D6/A2-A3,-(SP)		// Save all registers

		MOVEQ		#0,D4					// Initial match length

// Start scanning up to 4096 characters back in buffer
		MOVE.L		#LOOKBACK,D0
		MOVE.L		patternStart,D1			// d1 = patternStart
		CMP.L		D0,D1					// Is current pos >= 4096?
		BGE.S		@1						// Yep, can go all the way back
		MOVE.L		D1,D2					// else count = patternStart, start at beginning of buffer
		BRA.S		@2
@1:		MOVE.L		D0,D2					// count = 4096, go back 4k in buffer
@2:

// Set how far ahead we can scan without falling off end of buffer
		MOVEQ		#MAXMATCH,D3			//  max match length
		MOVE.L		dataLen,D0				//  D0 = dataLen - patternStart = howFarToEOF
		SUB.L		D1,D0
		CMP.L		D2,D0					//  MIN(count, (dataLen - patternStart))
		BLT.S		@min
		MOVE.L		D2,D0
@min:	CMP.L		D3,D0					//  D3 = MIN(18, count, howFarToEOF)
		BGE.S		@3
		MOVE.L		D0,D3

//  Set the buffer pointers
@3:		
		MOVEA.L		theData,A0
		MOVEA.L		A0,A3
		ADDA.L		D1,A3					// a3 = patternPtr = theData + patternStart

		MOVEA.L		A3,A2
		SUBA.L		D2,A2					// a2 = bufferPtr = theData + patternStart - count

		MOVEQ		#0,D6
		MOVE.B		(A3),D6					// d6 = *patternPtr for quick compares

// Loop until we have checked all the bytes in our scan...
@flmLoop:	
		TST.W		D2						// while (count > 0) ...
		BLE.S		@flmDone				//  count <= 0, we are done!
		CMP.B		(A2)+,D6				// if (*bufferPtr == *patternPtr) ...
		BEQ.S		@flmMatch				// 	see how many bytes match

		SUBQ.W		#1,D2					// count--
		BRA.S		@flmLoop				// do while

@flmMatch:	
		MOVEQ		#1,D0					// i = 1
		LEA			1(A3),A1				// a1 = &patternPtr[1]
@4:		CMP.W		D3,D0					// if (i >= loopMax)
		BGE.S		@5						// 	we have reached max match length
		CMPM.B		(A2)+,(A1)+				// if (patternPtr[i] != bufferPtr[i])
 		BNE.S		@5						// 	we have hit the end of the matching bytes
		ADDQ.W		#1,D0					// else i++, bump number of bytes that matched
		BRA.S		@4						// check next pair of bytes

// If this match length (i) is greater than previous max length, save this as new best length
@5:		CMP.W		D4,D0					// if (i >= bestMatchLen)
 		BLE.S		@6						// 	nope, bestMatchLen was still better
		MOVE.W		D2,D5					// else bestMatch = count
		MOVE.W		D0,D4					//  and bestMatchLen = i

// Now move past the matching bytes in buffer
@6:		SUBQ.W		#1,A2
		SUB.W		D0,D2					// count -= i
		BRA.S		@flmLoop				// do till count <= 0

// If the bestMatchLen is greater than coding threshold length (2) then store the coded bytes
// and return the match length
@flmDone:
		CMP.W		#THRESHOLD,D4			// if (bestMatchLen > THRESHOLD)
 		BLT.S		@flmNoCode				// 	nope, 2 or less bytes matched at patternStart

		MOVE.L		#LOOKBACK,D0			// encode offset
		SUB.W		D5,D0

		MOVE.W		D4,D1					// encode length
		SUBQ.W		#THRESHOLD,D1
		LSL.W		#8,D1
		LSL.W		#4,D1

		OR.W		D1,D0					//  save code word
		MOVEA.L		codeWord,A0
		MOVE.W		D0,(A0)

		MOVE.W		D4,D0			// return (bestMatchLen)
		BRA.S		@flmReturn

@flmNoCode:	
		MOVEQ		#0,D0			// return (0)

@flmReturn:	
		MOVEM.L		(SP)+,D2-D6/A2-A3
		frfree
		rts
}

#else

static short int findLongestMatch(unsigned char * theData, unsigned long dataLen, 
									unsigned long patternStart, unsigned short* codeWord)
{
unsigned char			*pointer;				// scanning source pointer
register unsigned char *lookback;				// scanning compare pointer
register unsigned char *tPointer;				// scanning source pointer
register unsigned char *tLookback;				// scanning compare pointer
register long			counter;				// number of bytes to scan
register unsigned long	maxLen;					// maximum match length
register unsigned long	bestLen;				// best match length
register unsigned long	bestOff;				// best match offset
register unsigned long	length;					// scanning length
unsigned long			forward;				// bytes to scan ahead

	pointer = theData + patternStart;
	
// find how far to look back
	counter = LOOKBACK;
	if (patternStart < LOOKBACK)
	{
		counter = patternStart;
	}

// find maximum match length	
	maxLen = MAXMATCH;
	forward = dataLen - patternStart;
	if (forward < (unsigned long)counter)
	{
		if (forward < maxLen)
		{
			maxLen = forward;
		}
	}
	else
	{
		if ((unsigned long)counter < maxLen)
		{
			maxLen = counter;
		}
	}
	
// initialize
	pointer = theData + patternStart;
	lookback = pointer - counter;
	bestLen = 0;
	bestOff = 0;
	
// scan the lookback buffer
	while (counter > 0)
	{
		length = 0;
		tPointer = pointer;
		tLookback = pointer - counter;
		if (*tPointer != *tLookback)
		{
			counter--;
		}
		else
		{
			while ((*tPointer++ == *tLookback++) && (length < maxLen)) length++;
			if (length > bestLen)
			{
				bestLen = length;
				bestOff = counter;
			}
			counter -= length;
		}
	}

// build code word for a match string

	if (bestLen >= THRESHOLD)
	{
		counter = LOOKBACK - bestOff;
		length = (bestLen - THRESHOLD) << OFFSETBITS;
		
		*codeWord = (unsigned short)(counter | length);
	}
	else
	{
		bestLen = 0;
	}
	return (short)bestLen;
}
#endif
#endif	// USE_CREATION_API == TRUE


#if USE_CREATION_API == TRUE
/* -------------------------------------------------------------------------------- *
 * Compress srcBuffer using LZSS technique.
 * -------------------------------------------------------------------------------- */
long LZSSCompress(XBYTE* srcBuffer, unsigned long srcBytes, XBYTE* dstBuffer,
					XCompressStatusProc proc, void* procData)
//	srcBuffer;					/* pointer to uncompressed data */
//	srcBytes;					/* size of uncompressed data */
//	dstBuffer;					/* pointer to compressed data */
{
#if 0
// This code currently does not generate byte for byte version of the compressed output. It's suspect.

XBYTE*			const srcEnd = srcBuffer + srcBytes;
XBYTE*			const dstEnd = dstBuffer + srcBytes;
XBYTE* 			srcPtr;					/* pointer to uncompressed data */
XBYTE* 			dstPtr;					/* pointer to compressed data */
XBYTE*			callProcPtr;			/* src position at which to call proc */

	srcPtr = srcBuffer;
	dstPtr = dstBuffer;

	if (proc)
	{
		callProcPtr = srcPtr + 1024;	// call proc every 1K
	}
	else
	{
		callProcPtr = srcEnd;			// never call proc
	}

	while (srcPtr < srcEnd)
	{
	unsigned int	codeCount;				/* index for the code group */
	unsigned int	codeNumber;				/* the number 0-7 of the code element */
	XBYTE			codeBuf[17];			/* buffer for flags and the code bytes */

		if (srcPtr >= callProcPtr)
		{
			callProcPtr = srcPtr + 1024;
			if ((*proc)(procData, srcPtr - srcBuffer, srcBytes))
			{
				return 0;
			}
		}

		codeBuf[0] = 0x00;
		codeCount = 1;

		/* build code blocks in groups of 8 */
		for (codeNumber = 0; codeNumber < 8; codeNumber++)
		{
		unsigned short	codeWord;				/* coded token */
		unsigned int	matchLen;				/* the length of the match found */

			/* get the longest match */
			matchLen = findLongestMatch(srcBuffer, srcBytes, srcPtr - srcBuffer, &codeWord);
			if (matchLen)
			{
				/* if we have a match over THRESHOLD characters, encode it */
				codeBuf[codeCount++] = (XBYTE)(codeWord >> 8);
				codeBuf[codeCount++] = (XBYTE)(codeWord & 0x00FF);
				srcPtr += matchLen;
			}
			else
			{
				/* otherwise, pass the character through and mark the appropiate flags bit */
				codeBuf[codeCount++] = *srcPtr++;
				codeBuf[0] |= (1 << codeNumber);
			}

			/* if we run out of data in the middle of a code group, exit */
			if (srcPtr >= srcEnd)
			{
				break;
			}
		}
		
		if (dstPtr + codeCount > dstEnd)
		{
			return -1;	// compression didn't reduce size, return failure
		}

		// write out the flags byte and the 8 code words or characters
		XBlockMove(codeBuf, dstPtr, codeCount);
		dstPtr += codeCount;
	}

	return dstPtr - dstBuffer;
	
#else

register XBYTE *dataPtr;					/* pointer to uncompressed data */
register unsigned long	dataPos;					/* buffer position for uncompressed data */
register XBYTE *cdataPtr;					/* pointer to compressed data */
register unsigned long	cdataPos;					/* buffer position for compressed data */
unsigned long			callProcPos;				/* src position at which to call proc */

register unsigned int	codeCount;					/* index for the code group */
register unsigned int	codeNumber;					/* the number 0-7 of the code element */
register unsigned int	flags;						/* the flags byte of a code group */
unsigned int			matchLen;					/* the length of the match found */
unsigned short			codeWord;					/* coded token */
XBYTE			codeBuf[16];				/* buffer for the code group */

	/* initalize the index variables */
	dataPtr = srcBuffer;
	dataPos = 0;
	
	cdataPtr = dstBuffer;
	cdataPos = 0;

	if (proc)
	{
		callProcPos = 1024;			// call proc every 1K
	}
	else
	{
		callProcPos = srcBytes;		// never call proc
	}

	while (dataPos < srcBytes)
	{
		if (dataPos >= callProcPos)
		{
			callProcPos = dataPos + 1024;
			if ((*proc)(procData, dataPos, srcBytes))
			{
				return 0;
			}
		}

		flags = 0x00;
		codeCount = 0;

		/* build code blocks in groups of 8 */
		for (codeNumber = 0; codeNumber < 8; codeNumber++)
		{
			/* get the longest match */
			matchLen = findLongestMatch(srcBuffer, srcBytes, dataPos, &codeWord);
			if (matchLen)
			{
				/* if we have a match over THRESHOLD characters, encode it */
				codeBuf[codeCount++] = (XBYTE)(codeWord >> 8);
				codeBuf[codeCount++] = (XBYTE)(codeWord & 0x00FF);
				dataPos += matchLen;
			}
			else
			{
				/* otherwise, pass the character through and mark the appropiate flags bit */
				codeBuf[codeCount++] = dataPtr[dataPos];
				flags |= (1 << codeNumber);
				dataPos++;
			}
			/* if we run out of data in the middle of a code group, exit */
			if (dataPos >= srcBytes)
			{
				break;
			}
		}

		if (cdataPos + codeCount >= srcBytes)
		{
			return -1;	// compressed is larger than original, abort
		}

		/* write out the flags byte */
		cdataPtr[cdataPos] = (XBYTE)flags;
		cdataPos++;

		/* write out the 8 (or less) characters/code blocks */
		for (codeNumber = 0; codeNumber < codeCount; codeNumber++)
		{
			cdataPtr[cdataPos] = codeBuf[codeNumber];
			cdataPos++;
		}
	}

	return cdataPos;
	
#endif
}
#endif	// USE_CREATION_API == TRUE



//	srcBuffer;					/* pointer to compressed data */
//	srcBytes;					/* size of compressed data */
//	dstBuffer;					/* pointer to uncompressed data */
//	dstSize;					/* size of uncompressed data */
/* -------------------------------------------------------------------------------- */
#if (X_PLATFORM == X_MACINTOSH) && (CPU_TYPE == k68000) && (ASM_doLZSSDecode)
static asm void	doLZSSDecode(unsigned char * srcBuffer, unsigned long srcBytes,
								unsigned char * dstBuffer, long dstSize)
{
			fralloc +
			MOVEM.L		D2-D7/A2-A3,-(A7)
			MOVEA.L		srcBuffer,A2
			MOVE.L		srcBytes,D6
			MOVEA.L		dstBuffer,A3
			MOVE.L		dstSize,D7

@block:		SUBQ.L		#1,D6						// test source ptr
			BMI.S		@done
			MOVE.B		(A2)+,D3					// get flags
			MOVEQ		#7,D2						// 8 bits
			
@flag:		LSR.B		#1,D3						// get flag bit
			BCC.S		@rept
			
			SUBQ.L		#1,D6						// test source ptr
			BMI.S		@done
			SUBQ.L		#1,D7						// test dest ptr
			BMI.S		@done
			MOVE.B		(A2)+,(A3)+					// copy literal byte
			DBRA		D2,@flag					// next flag bit
			BRA.S		@block
			
@rept:		SUBQ.L		#2,D6						// test source ptr
			BMI.S		@done
			MOVE.B		(A2)+,D0					// get code word
			LSL.W		#8,D0
			MOVE.B		(A2)+,D0
			
			MOVE.W		D0,D5						// make offset
			AND.W		#0x0FFF,D5
			MOVEQ		#0x000F,D4					// make count
			ROL.W		#4,D0
			AND.W		D0,D4
			ADDQ.W		#(THRESHOLD-1),D4
			
			MOVEA.L		A3,A0						// make copy ptr
			SUBA.W		#LOOKBACK,A0
			ADDA.W		D5,A0
			
@copy:		SUBQ.L		#1,D7						// test dest ptr
			BMI.S		@done
			MOVE.B		(A0)+,(A3)+					// copy repeated byte
			DBRA		D4,@copy

			DBRA		D2,@flag					// next flag bit
			BRA.S		@block
			
@done:		MOVEM.L		(A7)+,D2-D7/A2-A3
			frfree
			rts
}
#else
/*

Here is my thrash on the LZSS decompress code.  It makes a fair amount
more sense and has three algorithm changes:

1) A flag-test bit is shifted instead of the flag byte.  This could be
more safe vis-a-vis patent infringement.

2) The flag byte is tested for 0xFF.  In my testing, this case was
coming up nearly half the time.  In this case, I copy with two dword
moves.

3) The "previous buffer" copy loop is split into two parts:  one that
copies dwords, one that copies bytes.  I found the average number of
bytes to be copied was 12.  If this holds true, the savings from the
second loop could be well worth the testing.

I have found the new code to work just fine, but it obviously has to be
tested a lot more.

Chris3
*/


static void	doLZSSDecode(unsigned char* srcBuffer, unsigned long srcBytes,
							unsigned char* dstBuffer, long dstSize)
{
#if 1	//Moe's version
unsigned char*	src;
unsigned char*	dst;
long			srcCountdown;
long			dstCountdown;

	src = srcBuffer;
	dst = dstBuffer;
	srcCountdown = srcBytes;
	dstCountdown = dstSize;

	while (TRUE)
	{
	unsigned char	flagBits;
	
		if (--srcCountdown < 0) 
		{
			return;
		}
		flagBits = *src++;
#if 0
		// ALGORITHM IMPROVEMENT #1
		if ((flagBits == 0xFF) && (srcCountdown >= 8) && (dstCountdown >= 8))
		{
			// In the RMF files I've tested,
			// this case is hit for about 1/2 the time  -Moe
			*(long*)dst = *(long*)src;
			*(long*)(dst + 4) = *(long*)(src + 4);
			src += 8;
			dst += 8;
			srcCountdown -= 8;
			dstCountdown -= 8;
		}
		else
#endif
		{
		unsigned char	testBit;	// using a shifted test bit is better
									// for staying out of the way of patents  -Moe
			testBit = 0x01;
			do
			{
				if (flagBits & testBit)
				{
					if ((--srcCountdown < 0) || (--dstCountdown < 0))
					{
						return;
					}
					*dst++ = *src++;
				}
				else
				{
				unsigned int	bufferBits;
				unsigned char*	previousBuffer;
				unsigned int	byteCount;
//				unsigned int	longCount;

					if ((srcCountdown -= 2) < 0)
					{
						return;
					}
					bufferBits = ((unsigned int)src[0] << 8) | src[1];
					src += 2;
					previousBuffer = dst - (LOOKBACK - (bufferBits & 0x0FFF));

					byteCount = bufferBits >> 12;
					byteCount += THRESHOLD;
					dstCountdown -= byteCount;
					if (dstCountdown < 0) byteCount += dstCountdown;

#if 0
					// ALGORITHM IMPROVEMENT #2
					// In the RMF files I've tested,
					// the average byteCount is 12  -Moe
					longCount = byteCount / 4;
					while ((int)--longCount >= 0)
					{
						*(long*)dst = *(long*)previousBuffer;
						dst += 4;
						previousBuffer += 4;
					}
					byteCount %= 4;
#endif
					while ((int)--byteCount >= 0)
					{
						*dst++ = *previousBuffer++;
					}
				}
			}
			while (testBit <<= 1);
		}
	}
#else
	register short				temp,temp2,regD5,regD4;
	register unsigned char		regD3;
	register short				regD2;
	register unsigned char *	prevBuffer;

	while((long)--srcBytes >= 0)
	{
		regD3 = *(srcBuffer++);
		regD2 = 7;
		while(regD2 >= 0)
		{
			temp = regD3 & 1;
			regD3 >>= 1;
			if(temp == 0)
			{
				srcBytes -= 2;
				if((long)srcBytes < 0) 
				{
					//Debugger();
					return;
				}
				temp = *(srcBuffer++);					// temp = D0
				temp <<= 8;
				temp |= *(srcBuffer++);
				regD5 = temp & 0x0FFF;
#if 1
				regD4 = (temp >> 12) & 0x000F;
#else
				regD4 = 0x000F;
				temp2 = (temp & 0xF000) >> 12;
				temp <<= 4;
				temp |= temp2;
				regD4 &= temp;
#endif
				regD4 += THRESHOLD-1;
				prevBuffer = dstBuffer - LOOKBACK + regD5;
				while(regD4 >= 0)
				{
					if(--dstSize < 0) 
					{
						//Debugger();
						return;
					}
					*(dstBuffer++) = *(prevBuffer)++;
					regD4--;
				}
			}
			else
			{
				if((long)--srcBytes < 0) 
				{
					//Debugger();
					return;
				}
				if(--dstSize < 0) 
				{
					//Debugger();
					return;
				}
				*(dstBuffer++) = *(srcBuffer++);
			}
			regD2--;
		}
	}
#endif
}
#endif // ASM_doLZSSDecode

/* convert buffer by byte differences */
#if USE_CREATION_API == TRUE
static void DeltaMono8(unsigned char * pData, unsigned long frameCount)
{
#if 0
	asm 68000
	{
				MOVEA.L		pData,A0
				MOVE.L		frameCount,D0
				MOVE.B		(A0)+,D1
				BRA.S		@test
	@loop:		MOVE.B 		(A0),D2
				SUB.B		D1,(A0)+
				MOVE.B		D2,D1
	@test		SUBQ.L		#1,D0
				BNE.S		@loop
	}
#else
unsigned char	prev;
	
	prev = *pData++;
	while(--frameCount > 0)								/* taken from 68K, should this be bpl? */
	{
	unsigned char	next;

		next = *pData;
		*pData++ = next - prev;
		prev = next;
	}
#endif
}
static void DeltaStereo8(unsigned char* pData, unsigned long frameCount)
{
unsigned char	prevL;
unsigned char	prevR;

	prevL = *pData++;
	prevR = *pData++;
	while(--frameCount > 0)								/* taken from 68K, should this be bpl? */
	{
	unsigned char	nextL;
	unsigned char	nextR;

		nextL = *pData;
		*pData++ = nextL - prevL;
		prevL = nextL;

		nextR = *pData;
		*pData++ = nextR - prevR;
		prevR = nextR;
	}
}
static void DeltaMono16(short* pData, unsigned long frameCount)
{
short			prev;
	
	prev = *pData++;
	while(--frameCount > 0)								/* taken from 68K, should this be bpl? */
	{
	short			next;

		next = *pData;
		*pData++ = next - prev;
		prev = next;
	}
}
static void DeltaStereo16(short* pData, unsigned long frameCount)
{
short			prevL;
short			prevR;

	prevL = *pData++;
	prevR = *pData++;
	while(--frameCount > 0)								/* taken from 68K, should this be bpl? */
	{
	short			nextL;
	short			nextR;

		nextL = *pData;
		*pData++ = nextL - prevL;
		prevL = nextL;

		nextR = *pData;
		*pData++ = nextR - prevR;
		prevR = nextR;
	}
}
#endif

/* unconvert byte difference buffer */
static void UnDeltaMono8(unsigned char * pData, unsigned long frameCount)
{
#if 0
	asm 68000
	{
				MOVEA.L		pData,A0
				MOVE.L		frameCount,D0
				MOVE.B		(A0)+,D1
				BRA.S		@test
	@loop:		ADD.B	 	(A0),D1
				MOVE.B	 	D1,(A0)+
	@test:		SUBQ.L		#1,D0
				BNE.S		@loop
	}
#else
unsigned char	sample;
	
	sample = *pData++;
	while(--frameCount > 0)
	{
		sample += *pData;
		*pData++ = sample;
	}
#endif
}
static void UnDeltaStereo8(unsigned char* pData, unsigned long frameCount)
{
unsigned char	left;
unsigned char	right;

	left = *pData++;
	right = *pData++;
	while(--frameCount > 0)
	{
		left += *pData;
		*pData++ = left;
		right += *pData;
		*pData++ = right;
	}
}
static void UnDeltaMono16(short* pData, unsigned long frameCount)
{
short		sample;
	
	sample = *(pData++);
	while(--frameCount > 0)
	{
		sample += *pData;
		*pData++ = sample;
	}
}
static void UnDeltaStereo16(short* pData, unsigned long frameCount)
{
short		left;
short		right;

	left = *pData++;
	right = *pData++;
	while(--frameCount > 0)
	{
		left += *pData;
		*pData++ = left;
		right += *pData;
		*pData++ = right;
	}
}



// EOF of NewNewLZSS.c


