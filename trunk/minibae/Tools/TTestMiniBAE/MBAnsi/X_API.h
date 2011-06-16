/*****************************************************************************/
/*
**	X_API.h
**
**	This provides for platform specfic functions that need to be rewitten,
**	but will provide a API that is stable across all platforms
**
**	© Copyright 1995-2000 Beatnik, Inc, All Rights Reserved.
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
** Overview
**	Machine dependent code and equates
**
**	History	-
**	9/25/95		Created
**	10/15/95	Added XFiles
**	10/20/95	Added XMemory support
**	2/11/96		Added X_WEBTV system
**				Added XSound Manager stuff
**	2/17/96		Added more platform compile arounds
**	2/21/96		Changed XGetAndDetachResource to return a size
**	3/29/96		Added XPutLong & XPutShort
**	6/28/96		Added BeBox stuff
**	6/30/96		Changed font and re tabbed
**				Added XSetHardwareSampleRate and XGetHardwareSampleRate
**				Moved some types here ABS, DEBUG_STR
**				Added BeBox stuff and we are now dependent upon X_API.h to set
**				the machine type only
**	7/1/96		Added XCreateAccessCache
**				Added caching system to XFILERESOURCE
**	7/2/96		Merged Machine.h into X_API.h
**	7/3/96		Added XStrCmp
**	7/7/96		Added XStrnCmp
**	7/23/96		Added XMicroseconds
**	8/6/96		Added XGetIndexedType & XCountTypes
**	8/19/96		Added XGetCompressionName, XExpandMacADPCMto16BitLinear, 
**				XExpandULawto16BitLinear
**	9/22/96		Added XRandom & XSeedRandom & XRandomRange
**				Changed XExpandMacADPCMto16BitLinear to XExpandMacADPCMtoXBitLinear
**				to support 8 bit expansion
**	10/8/96		Added XStrCpy
**	10/9/96		Removed PASCAL usage
**	10/11/96	Added XConvertNativeFileToXFILENAME
**				Added XFileSetPositionRelative
**				Added XDetermineByteOrder
**				Added XSwapLong & XSwapShort
**	10/15/96	Added XFileOpenResourceFromMemory
**	11/14/96	Added XGetResourceName & XGetNamedResource
**				Added XWaitMicroseocnds
**	12/4/96		Added XStrLen
**	12/15/96	Added X_SOLARIS
**	12/18/96	Added more Solaris stuff
**	12/19/96	Added create flag in XFileOpenForWrite
**	12/19/96	Added Sparc pragmas
**	12/30/96	Changed copyright
**	1/2/97		Moved USE_MOD_API and USE_STREAM_API into this file
**				Added XCtoPstr & XPtoCstr
**	1/12/97		Added XDecryptData and XEncryptData
**				Renamed DecompressPtr to XDecompressPtr
**	1/13/97		Added XDuplicateStr
**	1/20/97		Added XEncryptedStrCpy && XEncryptedStrLen && XLStrCmp
**	1/21/97		Added XLStrnCmp
**	1/24/97		Added LONG_TO_FIXED & FIXED_TO_LONG
**	1/28/97		Added some Navio changes
**				Added XGetIndexedResource
**	1/29/97		Changed XFileOpenResourceFromMemory to support resource duplication
**				Added XCompressPtr
**	1/30/97		Added XDecryptAndDuplicateStr
**	2/2/97		Added XStringToLong
**	2/5/97		Added XFileOpenForReadFromMemory
**	2/19/97		Added XStrStr
**	6/3/97		Added flag USE_HIGHLEVEL_FILE_API to enable/disable sound file decoding
**	6/4/97		Added JAVA_SOUND for moe
**	6/10/97		Removed XIMAState structure
**				Added some more JAVA_SOUND wrappers
**	6/12/97		bvk Added XGetResourceNameOnly( openResourceFiles[currentResFile], theType, theID, pName );
**				Doesn't load resource data, just headers.
**	6/13/97		bvk Added XCountResources(XFILE fileRef, long int theType);
**	6/18/97		Removed some extra windows header files
**	6/27/97		bvk Added XDeleteFileResource()
**					Added TRSH resource type
**					Added XBOOL XCleanResourceFile( XFILE fileRef )
**	7/14/97		Added X_WIN_HAE to X_PLATFORM types
**	7/17/97		Removed XIsVirtualMemoryAvailable & XLockMemory & XUnlockMemory. Because
**				its assumed that all memory is locked.
**				Removed XSetHardwareSampleRate & XGetHardwareSampleRate
**	7/28/97		Changed DEBUG_STR and compile files for X_SOLARIS
**	8/6/97		Moved USE_FLOAT from GenSnd.h
**	8/7/97		Added XDuplicateAndStripStr
**	8/18/97		Changed X_WIN_HAE to USE_HAE_EXTERNAL_API
**	9/2/97		Added HAE_VXD & HAE_STANDALONE as modifiers for the X_WIN95 platform
**	9/3/97		Added USE_FULL_RMF_SUPPORT to eliminate excess RMF support
**	9/29/97		Changed XSetBit & XClearBit & XTestBit to be unsigned long rather than long
**	11/10/97	Changed some preprocessor tests and flags to explicity test for flags rather
**				than assume
**	11/11/97	Added USE_DEVICE_ENUM_SUPPORT to support multiple devices
**	12/16/97	Moe: removed compiler warnings
**	12/17/97	Added memoryCacheEntry element to XFILENAME structure. Used for memory cache
**				searches. See PV_XGetNamedCacheEntry in X_API.c for details
**	12/18/97	Cleaned up some warnings and added some rounding devices. Changed XFIXED
**	1/8/98		MOE: added XSwapLongsInAccessCache()
**	1/9/98		Added XFileDelete
**	1/21/98		Changed the functions XGetShort & XGetLong & XPutShort & XPutLong
**				into macros that fall out for Motorola order hardware
**	1/26/98		Added XERR to various functions
**	1/27/98		Added H_ prefix to CODE_TYPE's
**	1/31/98		Moved XPI_Memblock structures to X_API.h, and moved the function
**				XIsOurMemoryPtr to X_API.h
**	2/1/98		Changed XFILENAME to allow for different sized names. Created constant
**				FILE_NAME_LENGTH that is defined per platform
**	2/7/98		Changed XFIXED back to an unsigned long to fix broken content
**	3/12/98		MOE: Subtly changed XExpandMacADPCMtoXBitLinear()'s parameter types
**	3/18/98		Added XIs8BitSupported
**	3/20/98		MOE: Renamed XExpandIMAtoXBitLinearStreamed()-->XExpandAiffImaStream()
**	3/20/98		MOE: Renamed XExpandMacADPCMtoXBitLinear()-->XExpandAiffIma()
**	3/20/98		MOE: Renamed XDecodeIMA4Bit()-->XExpandWavIma()
**	3/23/98		MOE: Changed _XGetShort() and _XGetLong() to accept const* pointers
**	3/23/98		MOE: Added _XGetShortIntel() and _XGetLongIntel()
**				Added XRESOURCE
**				Added new parameter to XCompressAiffIma & XAllocateCompressedAiffIma
**	3/24/98		Added HAE_STANDALONE for use when making standalone players
**	4/15/98		Fixed a bug with XGetXXXX that failed on Solaris because the
**				data was being access as a macro and it expected it to be long
**				word aligned and it wasn't!
**	4/20/98		Changed the size of the XPI_Memblock structure to accomodate
**				the SolarisOS 8 byte alignment performance gain.
**	4/27/98		Changed XCompressPtr to handle XCOMPRESSION_TYPE
**	4/27/98		MOE:  Changed XDecompressPtr(),
**				eliminated XDecompressSampleFormatPtr()
**	5/5/98		Added XGetTempXFILENAME
**	5/12/98		MOE: Changed all the "ExpandAiffIma" functions to accept a
**				predictorCache[] parameter, rather than an indexCache[]
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
**	6/18/98		Added XFileFreeResourceCache
**				Changed macro FILE_NAME_LENGTH to use _MAX_PATH for Windows and
**				added a missing include file
**	6/19/98		Added USE_CAPTURE_API
**	7/1/98		Changed various API to use the new XResourceType and XLongResourceID
**	7/6/98		Changed _XPutShort to pass a unsigned short rather than an unsigned long
**	7/10/98		Added XGetUniqueFileResourceID & XGetUniqueResourceID & XAddResource
**				Added XDeleteResource
**				Added XCountResourcesOfType
**				Added XCleanResource
**				Added XFileGetCurrentResourceFile
**	8/10/98		Added macros XFIXED_TO_UNSIGNED_LONG & UNSIGNED_LONG_TO_XFIXED
**	10/2/98		Added XExpandALawto16BitLinear
**	11/6/98		Added HAE_EDITOR for editor support
**	11/11/98	Renamed pName to pResourceName because of a MacOS macro conflict.
**				Added XReadPartialFileResource
**				Removed unused macros XGet/Put and placed them back into
**				real functions
**	12/22/98	Changed status of USE_VARIABLE_REVERB is various builds
**	2/8/99		Added XLStrStr & XStrCat
**	2/11/99		Removed MOD support when HAE_PLUGIN is defined
**	2/12/99		Added types USE_MPEG_ENCODER & USE_MPEG_DECODER
**	2/21/99		Added XStripStr & XFileCreateResourceCache
**	2/24/99		Added XGetShortCompressionName
**	2/28/99		Changed some default BeOS settings
**	3/16/99		MOE:  Changed parameters of XCompressPtr() and LZSSCompress..()
**				Moved XCompressStatusProc from X_Formats.h
**	3/25/99		MOE:  Added void*data parameter to XCompressStatusProc
**				Added procData parameter to functions using XCompressStatusProc
**	5/22/99		Set USE_MOD_API to undefined for all cases.
**	5/26/99		MOE:  Added XResizePtr()
**	5/28/99		MOE:  Changed XStrLen() & XEncryptedStrLen() to return long,
**				and accept const pointers
**	5/28/99		MOE:  Changed the string functions and XBlockMove() to
**				accept const pointers
**	5/28/99		MOE:  Added XPTRC type (void const*)
**	7/14/99		Removed type G_PTR
**				Created type XSWORD, XSDWORD. 
**				Changed types UBYTE, SBYTE, INT16, UINT16, INT32, UINT32 to use macros
**				because of conflicts with CW5
**	8/10/99		Changed XFileSetPositionRelative & XFileSetLength to return an XERR
**	8/11/99		MOE: Added XCtoPascalString()
**	8/11/99		MOE:  Added XLongToStr()
**	 9/8/99		MOE: Added XSwapShorts()
**	 9/8/99		MOE: Added XLongToStr()
**	 9/8/99		MOE:  Changed XGetResourceNameOnly() last parameter to make
**				it clear what it does
**	 9/8/99		MOE: Added XGetFileResourceName(),
**				XExistsResource(), XExistsFileResource(),
**				XMakeUniqueResourceID(), XMakeUniqueFileResourceID()
**				Changed XGetResourceName() to return whether it found a resource
**	10/19/99	MSD: major restructure, breaking out build options into separate files.
**	10/25/99	MSD: moved TRUE, FALSE, NULL to top of file (before #includes)
**	11/21/99	Split from main source into MiniBAE
**	11/26/99	Added XBOOL16 that is a "short int"
**	1/13/00		MSD: added XConvertPathToXFILENAME()
**	2/4/2000	Changed copyright. We're Y2K compliant!
**  4/14/2000	jsc: moved MAX_OPEN_XFILES to X_API.h.
**	2000.03.21 AER	Added XTOKEN type
**	5/2/2000	sh: Added XFileIsValidResourceFromName & XFileIsValidResource
**  10/26/2000  tom: Added X_PLATFORM == X_STRONGARM definition
**  12/01/2000  tom: Added X_PLATFORM == X_ANSI definition
*/
/*****************************************************************************/

#ifndef __X_API__
#define __X_API__

// some common types
#undef TRUE
#ifndef TRUE
	#define TRUE	1
#endif

#undef FALSE
#ifndef FALSE
	#define FALSE	0
#endif

#undef NULL
#ifndef NULL
	#define NULL	0L
#endif



#if 0
	#pragma mark ## X_PLATFORM defines ##
#endif

// types for X_PLATFORM
// *******************************************************************************
// Make sure you set the X_PLATFORM define correctly. Everything depends upon this 
// flag being setup correctly.
// *******************************************************************************
#define X_UNDEFINED			0
#define X_MACINTOSH			1		// MacOS
#define X_WIN95				2		// Windows 95/NT OS
#define X_WEBTV				3		// WebTV OS
#define X_BE				4		// BeOS
#define X_MAGIC				5		// MagicCap OS. Note: This probably won't work yet
#define X_SOLARIS			6		// SunOS
#define X_NAVIO				7		// NaviOS
#define X_WIN_HARDWARE		8		// Windows 95 only direct to Sound Blaster hardware
#define X_WEBTV_CE			9		// WebTV with CE


// types for CPU_TYPE
#define k68000				0
#define kRISC				1
#define k80X86				2
#define kSPARC				3


// types for COMPILER_TYPE
#define DEFAULT_COMPILER	0


// types for CPU_TYPE
#define k68000				0
#define kRISC				1
#define k80X86				2
#define kSPARC				3


// types for LOOPS_USED
#define LIMITED_LOOPS		0
#define FLOAT_LOOPS			1
#define U3232_LOOPS			2


// types for REVERB_USED
#define REVERB_DISABLED		0
#define SMALL_MEMORY_REVERB	1
#define VARIABLE_REVERB		2


// backward compatibility
#ifdef HAE_VXD
	#ifndef BAE_VXD
		#define BAE_VXD		HAE_VXD
	#endif
#endif

#ifdef HAE_EDITOR
	#ifndef BAE_EDITOR
		#define BAE_EDITOR	HAE_EDITOR
	#endif
#endif

#ifdef HAE_STANDALONE
	#ifndef BAE_STANDALONE
		#define BAE_STANDALONE	HAE_STANDALONE
	#endif
#endif

#ifdef HAE_PLUGIN
	#ifndef BAE_PLUGIN
		#define BAE_PLUGIN	HAE_PLUGIN
	#endif
#endif



#ifndef X_PLATFORM
	#error "You need to define X_PLATFORM outside of the source. Use the types above."
#endif

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
#if 0
	#pragma mark ## X_PLATFORM includes ##
#endif


#if X_PLATFORM == X_MACINTOSH
	#include "BAEBuildOptions_MacOS.h"
#elif X_PLATFORM == X_NAVIO
	#include "BAEBuildOptions_Navio.h"
#elif X_PLATFORM == X_WEBTV
	#include "BAEBuildOptions_WebTV.h"
#elif X_PLATFORM == X_WIN95
	#include "BAEBuildOptions_WinOS.h"
#elif X_PLATFORM == X_WIN_HARDWARE
	#include "BAEBuildOptions_WinXvD.h"
#elif X_PLATFORM == X_BE
	#include "BAEBuildOptions_BeOS.h"
#elif X_PLATFORM == X_SOLARIS
	#include "BAEBuildOptions_SolarisOS.h"
#elif X_PLATFORM == X_WEBTV_CE
	#include "BAEBuildOptions_WebTV_CE.h"
#elif X_PLATFORM == X_ANSI
	#include "BAE_BuildOptions_Ansi.h"
#elif X_PLATFORM == X_STRONGARM
	#include "BAEBuildOptions_StrongARM.h"
#endif

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

// Make sure everything is defined
#ifndef COMPILER_TYPE
	#error COMPILER_TYPE not defined!
#endif
#ifndef CPU_TYPE
	#error CPU_TYPE not defined!
#endif
#ifndef X_WORD_ORDER
	#error X_WORD_ORDER not defined!
#endif
#ifndef USE_FLOAT
	#error USE_FLOAT not defined!
#endif
#ifndef USE_8_BIT_OUTPUT
	#error USE_8_BIT_OUTPUT not defined!
#endif
#ifndef USE_16_BIT_OUTPUT
	#error USE_16_BIT_OUTPUT not defined!
#endif
#ifndef USE_MONO_OUTPUT
	#error USE_MONO_OUTPUT not defined!
#endif
#ifndef USE_STEREO_OUTPUT
	#error USE_STEREO_OUTPUT not defined!
#endif
#ifndef LOOPS_USED
	#error LOOPS_USED not defined!
#endif

#if LOOPS_USED == LIMITED_LOOPS
	#ifndef USE_DROP_SAMPLE
		#error USE_DROP_SAMPLE not defined!
	#endif
	#ifndef USE_TERP1
		#error USE_TERP1 not defined!
	#endif
	#ifndef USE_TERP2
		#error USE_TERP2 not defined!
	#endif
#endif

#ifndef USE_FULL_RMF_SUPPORT
	#error USE_FULL_RMF_SUPPORT not defined!
#endif
#ifndef USE_CREATION_API
	#error USE_CREATION_API not defined!
#endif
#ifndef USE_MPEG_ENCODER
	#error USE_MPEG_ENCODER not defined!
#endif
#ifndef USE_MPEG_DECODER
	#error USE_MPEG_DECODER not defined!
#endif
#ifndef USE_HIGHLEVEL_FILE_API
	#error USE_HIGHLEVEL_FILE_API not defined!
#endif
#ifndef USE_STREAM_API
	#error USE_STREAM_API not defined!
#endif
#ifndef REVERB_USED
	#error REVERB_USED not defined!
#endif
#ifndef USE_NEW_EFFECTS
	#error USE_NEW_EFFECTS not defined!
#endif
#ifndef USE_DEVICE_ENUM_SUPPORT
	#error USE_DEVICE_ENUM_SUPPORT not defined!
#endif
#ifndef USE_CAPTURE_API
	#error USE_CAPTURE_API not defined!
#endif
#ifndef USE_MOD_API
	#error USE_MOD_API not defined!
#endif
#ifndef FILE_NAME_LENGTH
	#error FILE_NAME_LENGTH not defined!
#endif
#ifndef INLINE
	#error INLINE not defined!
#endif
#ifndef DEBUG_STR
	#error DEBUG_STR(x) not defined!
#endif

// ------------------------------------------------------------------------------------
// Type definitions
typedef void *			XPTR;
typedef void const*		XPTRC;
typedef void *			XRESOURCE;
typedef long			XERR;
typedef char			XBOOL;			// 8 bit boolean
typedef unsigned short	XBOOL16;		// 16 bit boolean
typedef long			LOOPCOUNT;

typedef char			XSBYTE;			// 8 bit signed
typedef unsigned char	XBYTE;			// 8 bit unsigned
typedef short			XSWORD;			// 16 bit signed
typedef unsigned short	XWORD;			// 16 bit unsigned
// NOTE: on 64-bit platforms, this needs to be redefined because a long is 64 bits!
typedef long			XSDWORD;		// 32 bit signed
typedef unsigned long	XDWORD;			// 32 bit unsigned
typedef XDWORD			XTOKEN;

// these macros need to be removed soon. Conflicts are coming
#define UBYTE			XBYTE
#define SBYTE			XSBYTE
#if X_PLATFORM != X_NAVIO
#define INT16			XSWORD
#define UINT16			XWORD
#define INT32			XSDWORD
#define UINT32			XDWORD
#endif


// This is used to solve the 4 character constant problem on some compilers
#define FOUR_CHAR(ch1,ch2,ch3,ch4) \
			((((unsigned long)(ch1)&0x0FFL)<<24L) + (((ch2)&0x0FFL)<<16L) + (((ch3)&0x0FFL)<<8L) + ((ch4)&0x0FFL))


#undef ABS
#define ABS(x)				(((x) < 0) ? -(x) : (x))

#define XMAKELONG(a, b)		((long)(((XWORD)(a)) | ((XDWORD)((XWORD)(b))) << 16L))
#define XLOWORD(l)			((XWORD)(l))
#define XHIWORD(l)			((XWORD)(((XDWORD)(l) >> 16L) & 0xFFFFL))

#define XMAX(a,b)			(((a) > (b)) ? (a) : (b))
#define XMIN(a,b)			(((a) < (b)) ? (a) : (b))




typedef unsigned long				XFIXED;
// The type XFIXED is an unsigned value, but the the calculations allow for negative numbers. If you changes this
// from long to unsigned, then all the fade API's will fail. If you need the extra bit for an unsigned value
// use the unsigned macros
#define XFIXED_1					65536L
#define LONG_TO_XFIXED(x)			(XFIXED)((((long)(x)) * XFIXED_1))
#define UNSIGNED_LONG_TO_XFIXED(x)	(XFIXED)((((unsigned long)(x)) * XFIXED_1))
#define RATIO_TO_XFIXED(a,b)		(LONG_TO_XFIXED(a) / (b))
#define XFIXED_TO_LONG(x)			(long)(((long)(x)) / XFIXED_1)
#define XFIXED_TO_UNSIGNED_LONG(x)	(unsigned long)(((unsigned long)(x)) / XFIXED_1)
#define XFIXED_TO_SHORT(x)			((short)((x) / XFIXED_1))
#define XFIXED_TO_LONG_ROUNDED(x)	XFIXED_TO_LONG((x) + XFIXED_1 / 2)
#define XFIXED_TO_SHORT_ROUNDED(x)	XFIXED_TO_SHORT((x) + XFIXED_1 / 2)

//#if USE_FLOAT == TRUE
	#define FLOAT_TO_XFIXED(x)		((XFIXED)((double)(x) * XFIXED_1))
	#define XFIXED_TO_FLOAT(x)		((double)(x) / XFIXED_1)
//#endif

// This fixes a newly discovered bug in which there is a loss of precision when calculating
// and playing back midi files with the internal sequencer. By changing the appropiate
// variables to floats the problem goes away. This needs to be recoded for fixed point math
// to preserve the engine's non use of floating point
#if USE_FLOAT == FALSE
	typedef UINT32		UFLOAT;
	typedef INT32		IFLOAT;
#else
	typedef double		UFLOAT;
	typedef double		IFLOAT;
#endif



#ifdef __cplusplus
	extern "C" {
#endif


// Memory Manager

// Every block of data allocated with XNewPtr will contain this structure before the pointer that is
// passed to the user.
struct XPI_Memblock
{
	long	blockID_one;		// ID that this is our block. part 1
	long	blockSize;			// block size
	long	blockID_two;		// ID that this is our block. part 2
#if (X_PLATFORM == X_SOLARIS)
	long	alignment8;			// used for alignment to 8 byte boundries
#endif
};
typedef struct XPI_Memblock XPI_Memblock;

#define XPI_BLOCK_1_ID		FOUR_CHAR('I','G','O','R')		//	'IGOR'
#define XPI_BLOCK_2_ID		FOUR_CHAR('G','S','N','D')		//	'GSND'
#define XPI_BLOCK_3_ID		FOUR_CHAR('F','L','A','T')		//	'FLAT'
#define XPI_DEAD_ID			FOUR_CHAR(0xDE,0xAD,0xFF,0xFF)	//	Dead block

// This function is used to see of a particular memory block has been allocated with
// XNewPtr. If not this function will return NULL, otherwise the new pointer
// reference, which will be the real memory allocated with the host allocate memory function.
XPI_Memblock * XIsOurMemoryPtr(XPTR data);

XPTR	XNewPtr(long size);
void	XDisposePtr(XPTR data);
long	XGetPtrSize(XPTR data);
// This function re-allocates a memory block
// ptr may be NULL, in which case the functionality is the same as XNewPtr()
// If allocation fails, ptr is unaffected (It's still allocated.)
// Like with XNewPtr(), any newly allocated memory is zeroed.
XPTR	XResizePtr(XPTR ptr, long size);

void	XBlockMove(XPTRC source, XPTR dest, long size);
void	XSetMemory(void *pAdr, long len, char value);
void	XBubbleSortArray(short int *theArray, short int theCount);
void	XSetBit(void *pBitArray, unsigned long whichbit);
void	XClearBit(void *pBitArray, unsigned long whichbit);
XBOOL	XTestBit(void *pBitArray, unsigned long whichbit);

unsigned long XMicroseconds(void);
void XWaitMicroseocnds(unsigned long waitAmount);


// Resource Manager

typedef long			XResourceType;
typedef long			XLongResourceID;
typedef short			XShortResourceID;

struct XFILE_CACHED_ITEM
{
	XResourceType	resourceType;		// resource type
	XLongResourceID	resourceID;			// resource ID
	long			resourceLength;		// resource ID
	long			fileOffsetName;		// file offset from 0 to resource name
	long			fileOffsetData;		// file offset from 0 to resource data
};
typedef struct XFILE_CACHED_ITEM		XFILE_CACHED_ITEM;

struct XFILERESOURCECACHE
{
	long				totalResources;
	XFILE_CACHED_ITEM	cached[1];
};
typedef struct XFILERESOURCECACHE		XFILERESOURCECACHE;


struct XFILENAME
{
// public platform specific
	long				fileReference;
#if X_PLATFORM == X_MACINTOSH
	FSSpec				theFile;
#else
	char				theFile[FILE_NAME_LENGTH];	// "C" string name for path
#endif

// private variables. Zero out before calling functions
	long				fileValidID;
	XBOOL				resourceFile;

	XPTR				pResourceData;	// if file is memory based
	long				resMemLength;	// length of memory resource file
	long				resMemOffset;	// current offset of memory resource file
	XBOOL				readOnly;		// TRUE then file is read only
	XBOOL				allowMemCopy;	// if TRUE, when a memory based resource is
										// read, a copy will be created otherwise
										// its just a pointer into the larger memory resource
										// file
	XFILE_CACHED_ITEM	memoryCacheEntry;
	XFILERESOURCECACHE	*pCache;		// if file has been cached this will point to it
};
typedef struct XFILENAME	XFILENAME;
typedef long				XFILE;

#define XFILERESOURCE_ID	FOUR_CHAR('I','R','E','Z')	// IREZ
#define XFILECACHE_ID		FOUR_CHAR('C','A','C','H')	// CACH
#define XFILETRASH_ID		FOUR_CHAR('T','R','S','H')	// TRSH

struct XFILERESOURCEMAP
{
	long		mapID;
	long		version;
	long		totalResources;
};
typedef struct XFILERESOURCEMAP		XFILERESOURCEMAP;

// Resource Entry
//	long	nextentry
//	long	resourceType
//	long	resourceID
//	pascal	string resourceName
//	long	resourceLength
//	data block

// Resource file works as follows:
//	MAP
//	ENTRIES
//
// You can assume that at the end of the file length you can tack new resources. Just update the map


// Create a temporary file name and fill an XFILENAME structure. Return -1 for failure, or 0 for sucess.
XERR XGetTempXFILENAME(XFILENAME* xfilename);

// Given an open file, return TRUE if this is a valid resource file.
XBOOL	XFileIsValidResource(XFILE file);
// Given a filename, return TRUE if this is a valid resource file.
XBOOL	XFileIsValidResourceFromName(XFILENAME *file);

#define MAX_OPEN_XFILES				10		// max number of open resource files

// Open file as a resource file. Pass TRUE to 'readOnly' for read only.
XFILE	XFileOpenResource(XFILENAME *file, XBOOL readOnly);

// Open file as a read only resource file from a memory pointer. Assumes memory block
// is an exact copy of the resource file format. Don't dispose of pResource until you
// have closed the file. If allowCopy is TRUE, then when resources are read new copies
// will be created, otherwise just a pointer into the mapped resource file
XFILE	XFileOpenResourceFromMemory(XPTR pResource, unsigned long resourceLength, XBOOL allowCopy);

// Open file as a read only file from a memory pointer. Don't dispose of pMemoryBlock until you
// have closed the file.
XFILE XFileOpenForReadFromMemory(XPTR pMemoryBlock, unsigned long memoryBlockSize);

// open file for reading and writing. Direct access.
XFILE	XFileOpenForRead(XFILENAME *file);
XFILE	XFileOpenForWrite(XFILENAME *file, XBOOL create);

// close file. Direct or resource
void	XFileClose(XFILE fileRef);
void	XFileUseThisResourceFile(XFILE fileRef);

// get current most recently opened resource file, or NULL if nothing is open
XFILE	XFileGetCurrentResourceFile(void);

// delete file. 0 is ok, -1 for failure
XERR XFileDelete(XFILENAME *file);

// Read a file into memory and return an allocated pointer.
// 0 is ok, -1 failed to open, -2 failed to read, -3 failed memory
// if 0, then *pData is valid
XERR XGetFileAsData(XFILENAME *pResourceName, XPTR *pData, long *pSize);

void XConvertNativeFileToXFILENAME(void *file, XFILENAME *xfile);
void XConvertPathToXFILENAME(void *path, XFILENAME *xfile);

XFILERESOURCECACHE* XCreateAccessCache(XFILE fileRef);
void XSwapLongsInAccessCache(XFILERESOURCECACHE	*pCache, XBOOL inFileOrder);

// Create a resource cache for a file
XERR XFileCreateResourceCache(XFILE fileRef);

// Free cache of a resource file
void XFileFreeResourceCache(XFILE fileRef);

// search through open resource files
XBOOL	XExistsResource(XResourceType resourceType, XLongResourceID resourceID);
XBOOL	XGetResourceName(XResourceType resourceType, XLongResourceID resourceID, char *cName);
XPTR	XGetNamedResource(XResourceType resourceType, void *cName, long *pReturnedResourceSize);
XPTR	XGetAndDetachResource(XResourceType resourceType, XLongResourceID resourceID, long *pReturnedResourceSize);
XPTR	XGetIndexedResource(XResourceType resourceType, XLongResourceID *pReturnedID, long resourceIndex, 
								void *pResourceName, long *pReturnedResourceSize);

// get a unique ID for a particular file to be used as a resource ID
XERR	XGetUniqueFileResourceID(XFILE fileRef, XResourceType resourceType, XLongResourceID *pReturnedID);
XERR	XGetUniqueResourceID(XResourceType resourceType, XLongResourceID *pReturnedID);

// make sure ID is unique
// 0 - no change, 1 - id changed, -1 - failure
XERR	XMakeUniqueResourceID(XResourceType resourceType,
								XLongResourceID* id);
XERR	XMakeUniqueFileResourceID(XFILE fileRef, XResourceType resourceType,
									XLongResourceID* id);

// Add a resource to the most recently open resource file.
//		resourceType is a type
//		resourceID is an ID
//		pResourceName is a pascal string
//		pData is the data block to add
//		length is the length of the data block
XERR	XAddResource(XResourceType resourceType, XLongResourceID resourceID, void *pResourceName, void *pData, long length);

// Delete a resource from the most recently open resource file.
//		resourceType is a type
//		resourceID is an ID
//		collectTrash if TRUE will force an update, otherwise it will happen when the file is closed
XBOOL	XDeleteResource(XResourceType theType, XLongResourceID resourceID, XBOOL collectTrash );

// return the number of resources of a particular type.
long	XCountResourcesOfType(XResourceType resourceType);

// Force a clean/update of the most recently opened resource file
XBOOL	XCleanResource(void);

// Add a resource to a particular file
//		fileRef is the open file
//		resourceType is a type
//		resourceID is an ID
//		pResourceName is a pascal string
//		pData is the data block to add
//		length is the length of the data block
XERR	XAddFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID, void const*pResourceName, void *pData, long length);

XBOOL	XExistsFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID);

// Get just the name of a resource only
//		fileRef is the open file
//		resourceType is a type
//		resourceID is an ID
//		cName is a "C" string
//		Returns TRUE if found, otherwise FALSE
XBOOL	XGetFileResourceName(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID, char *cName);

XPTR	XGetFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID, void *pResourceName, long *pReturnedResourceSize);
XPTR	XGetIndexedFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID *pReturnedID, long resourceIndex, 
									void *pResourceName, long *pReturnedResourceSize);

XERR XReadPartialFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID,
								char *pReturnedResourceName,
								XPTR *pReturnedBuffer, long bytesToReadAndAllocate);

XResourceType XGetIndexedType(XFILE fileRef, long resourceIndex);
long	XCountTypes(XFILE fileRef);
long	XCountFileResourcesOfType(XFILE fileRef, XResourceType theType);

// Get just the name of a resource only
//		fileRef is the open file
//		theType is a type
//		theID is an ID
//		pResourceName is a pascal string
//		Returns pResourceName
char	*XGetResourceNameOnly(XFILE fileRef,
								XResourceType theType, XLongResourceID theID,
								char *pResourceName);

// returns TRUE if ok.
XBOOL	XDeleteFileResource(XFILE fileRef, XResourceType theType, XLongResourceID resourceID, XBOOL collectTrash );

// returns TRUE if ok.
XBOOL	XCleanResourceFile( XFILE fileRef );

// File Manager
XERR	XFileRead(XFILE fileRef, XPTR buffer, long bufferLength);
XERR	XFileWrite(XFILE fileRef, XPTRC buffer, long bufferLength);
XERR	XFileSetPosition(XFILE fileRef, long filePosition);
long	XFileGetPosition(XFILE fileRef);
XERR	XFileSetPositionRelative(XFILE fileRef, long relativeOffset);
long	XFileGetLength(XFILE fileRef);
XERR	XFileSetLength(XFILE refRef, unsigned long newSize);

// standard string functions
short int	XStrCmp(char const* string1, char const* string2);
short int	XStrnCmp(char const* string1, char const* string2, long n);
char*		XStrCpy(char* dest, char const* src);
char*		XStrStr(char* source, char const* pattern);
long		XStrLen(char const* src);
char*		XStrCat(char* dest, char const* source);

// standard string functions, but ignore case
short int	XLStrCmp(const char* string1, const char* string2);
short int	XLStrnCmp(const char* string1, const char* string2, long n);
char*		XLStrStr(char* source, char const* pattern);

short int	XMemCmp(void const* src1, void const* src2, long n);
char*		XDuplicateStr(char const* src);
// Duplicate and string characters below 32
char*		XDuplicateAndStripStr(char const* src);
void		XStripStr(char* pString);
// Converts a long value to a base 10 string, returns pointer to the end
char*		XLongToStr(char* dest, long value);
// This will convert a string to a base 10 long value
long		XStrnToLong(char const* pData, long length);

enum
{
	X_SOURCE_ENCRYPTED = 0,				// source encrypted, destination is not encrypted
	X_SOURCE_DEST_ENCRYPTED				// source and destination encrypted	
};
// standard strcpy, but with crypto controls
char		*XEncryptedStrCpy(char* dest, char const* src, short int copy);
long		XEncryptedStrLen(char const* src);
char		*XDecryptAndDuplicateStr(char const* src);



// convert a c string to a pascal string
void		XCtoPascalString(char const* cString, char pascalString[256]);
void*		XCtoPstr(void *cstr);
// convert a pascal string to a c string
void*		XPtoCstr(void *pstr);

XFIXED	XFixedDivide(XFIXED divisor, XFIXED dividend);
XFIXED	XFixedMultiply(XFIXED prodA, XFIXED prodB);

// if TRUE, then motorola; if FALSE then intel
XBOOL				XDetermineByteOrder(void);

// NOTE:
//	!!!!	These can't be turned into macros because the code accesses bytes
//			that are not byte aligned. We need to get them byte by byte to prevent
//			CPU's from failing.
unsigned short 		XGetShortIntel(void const* address);
unsigned long		XGetLongIntel(void const* address);
unsigned short 		XGetShort(void const* address);
unsigned long		XGetLong(void const* address);
void				XPutShort(void *address, unsigned short value);
void				XPutLong(void *address, unsigned long value);

// These will swap bytes no matter the byte order
unsigned short		XSwapShort(unsigned short value);
unsigned long		XSwapLong(unsigned long value);

// Type 0 - Delta encoded LZSS

typedef enum
{	X_RAW			= 0xFF,
	X_MONO_8		= 0,
	X_STEREO_8		= 1,
	X_MONO_16		= 2,
	X_STEREO_16		= 3
} XCOMPRESSION_TYPE;

// First byte is a compression type.
// Next 3 bytes is uncompressed length.
void*	XDecompressPtr(void * pData, unsigned long dataSize, XBOOL ignoreType);

void	LZSSUncompress(unsigned char* src, unsigned long srcBytes,
						unsigned char* dst, unsigned long dstBytes);
void	LZSSUncompressDeltaMono8(unsigned char* src, unsigned long srcBytes,
									unsigned char* dst, unsigned long dstBytes);
void	LZSSUncompressDeltaStereo8(unsigned char* src, unsigned long srcBytes,
									unsigned char* dst, unsigned long dstBytes);
void	LZSSUncompressDeltaMono16(unsigned char* src, unsigned long srcBytes,
									short* dst, unsigned long dstBytes);
void	LZSSUncompressDeltaStereo16(unsigned char* src, unsigned long srcBytes,
									short* dst, unsigned long dstBytes);

// return TRUE to stop
typedef XBOOL	(*XCompressStatusProc)(void* data,
										unsigned long currentBuffer,
										unsigned long maxBuffer);

#if USE_CREATION_API != FALSE
// Given a block of data and a size, this will compress it into a newly-allocated.
// block of data.  The original pointer is not deallocated.  The new pointer must
// be deallocated when no longer needed.  The first byte of the compressed data
// is the XCOMPRESSION_TYPE used and the following 3 bytes are the length of the 
// uncompressed data.  (Original data cannot be larger than than 256 MB.)
// A pointer to the compressed block is stored at compressedDataTarget.
// The length of the compressed data is returned if compression succeeds
//	If -1 is returned, the compression failed
//	If 0 is returned, compression was aborted by proc.
long	XCompressPtr(XPTR* compressedDataTarget,
						XPTR pData, unsigned long dataSize,
						XCOMPRESSION_TYPE type,
						XCompressStatusProc proc, void* procData);

long	LZSSCompress(XBYTE* src, unsigned long srcBytes, XBYTE* dst,
						XCompressStatusProc proc, void* procData);
long	LZSSCompressDeltaMono8(XBYTE* src, unsigned long srcBytes, XBYTE* dst,
								XCompressStatusProc proc, void* procData);
long	LZSSCompressDeltaStereo8(XBYTE* src, unsigned long srcBytes, XBYTE* dst,
									XCompressStatusProc proc, void* procData);
long	LZSSCompressDeltaMono16(short* src, unsigned long srcBytes, XBYTE* dst,
								XCompressStatusProc proc, void* procData);
long	LZSSCompressDeltaStereo16(short* src, unsigned long srcBytes, XBYTE* dst,
									XCompressStatusProc proc, void* procData);
#endif

void	XSwapShorts(short* shortArray, long count);
void	XPhase8BitWaveform(unsigned char * pByte, long size);

// Sound Support
XBOOL	XIs8BitSupported(void);
XBOOL	XIs16BitSupported(void);
XBOOL	XIsStereoSupported(void);

#define X_FULL_VOLUME	256		// full volume (1.0)

short int		XGetHardwareVolume(void);
void			XSetHardwareVolume(short theVolume);

void		XGetCompressionName(long compressionType, void *cName);
void		XGetShortCompressionName(long compressionType, void *cName);

// Mac ADPCM compression (IMA 4 to 1)
XPTR		XAllocateCompressedAiffIma(void const* src, XDWORD srcBitsPerSample,
										XDWORD frameCount, XDWORD channelCount);
void		XCompressAiffIma(void const* src, XDWORD srcBitsPerSample, XBYTE* dst,
							XDWORD frameCount, XDWORD channelCount);
// Mac ADPCM decompression (IMA 4 to 1)
void		XExpandAiffIma(XBYTE const* src, XDWORD srcBytesPerBlock,
							void* dst, XDWORD dstBitsPerSample,
							XDWORD frameCount, XDWORD channelCount);

XDWORD		XExpandAiffImaStream(XBYTE const* src, XDWORD srcBytesPerBlock,
									void *dst, XDWORD dstBitsPerSample,
									XDWORD srcBytes, XDWORD channelCount,
									short predictorCache[2]);

// This is used for WAVE files
XDWORD		XExpandWavIma(XBYTE const* src, XDWORD srcBytesPerBlock,
							void* pbDst, XDWORD dstBitsPerSample,
							XDWORD srcBytes, XDWORD channelCount);

// u law decompression
void		XExpandULawto16BitLinear(unsigned char *pSource, short int *pDest, long frames, long channels);

// a law decompression
void		XExpandALawto16BitLinear(unsigned char *pSource, short int *pDest, long frames, long channels);

// MACE decompression
void		XExpandMace1to6(void *inBuffer, void *outBuffer, unsigned long cnt, 
					void * inState, void * outState, 
					unsigned long numChannels, unsigned long whichChannel);
void		XExpandMace1to3(void *inBuffer, void *outBuffer, unsigned long cnt, 
					void * inState, void * outState, 
					unsigned long numChannels, unsigned long whichChannel);

// Decrypt a block of data. This should be U.S. munitions safe. ie below 40 bit
void		XDecryptData(void *pData, unsigned long size);
// Encrypt a block of data. This should be U.S. munitions safe. ie below 40 bit
void		XEncryptData(void *pData, unsigned long size);

// Random numbers
short int	XRandom(void);					// return pseudo-random from 0 to 32767
void		XSeedRandom(unsigned long n);	// set pseudo-random generator
short int	XRandomRange(short int max);	// return pseudo-random from 0 to max - 1

// Character translation functions

// XIsWinInMac() determines whether a Macintosh character has an equivalent in the Windows character set.
XBOOL	XIsWinInMac(char ansiChar);

// XIsMacInWin() determines whether a Windows character has an equivalent in the Macintosh character set.
XBOOL	XIsMacInWin(char macChar);

// XTranslateWinToMac() provides the Macintosh-equivalent of a Windows character code.
char	XTranslateWinToMac(char ansiChar);

// XTranslateMacToWin() provides the Windows-equivalent of a Macintosh character code.
char	XTranslateMacToWin(char macChar);



#ifdef __cplusplus
	}
#endif

#endif	// __X_API__


