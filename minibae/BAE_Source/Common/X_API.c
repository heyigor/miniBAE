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
**  X_API.c
**
**      This provides platform specfic functions
**
**  © Copyright 1995-2001 Beatnik, Inc, All Rights Reserved.
**  Written by Steve Hales
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
**  History -
**  9/25/95     Created
**  12/14/95    Added XGetAndDetachResource
**  12/19/95    Added XSetBit & XTestBit
**  1/18/96     Spruced up for C++ extra error checking
**  1/28/96     Changed XGetAndDetachResource
**  2/3/96      Removed extra includes
**  2/11/96     Added XIsStereoSupported & XIs16BitSupported
**              Added XFixedDivide & XFixedMultiply
**              Started putting in platform defines around code functions
**  2/21/96     Changed XGetAndDetachResource to return a size
**  3/25/96     Modifed XExpandMace1to6 & XExpandMace1to3 to create silence if 
**              not supported
**  3/29/96     Added XPutLong & XPutShort
**  4/20/96     Moved myFixedMultiply from GenSynth.c and moved all references 
**              to XFixedMultiply
**              Moved myFixedDivide from GenSynth.c and moved all references 
**              to XFixedDivide
**  4/21/96     Removed register usage in parameters
**  5/14/96     Fixed Odd address error in XIsOurMemoryPtr
**              Replaced XFixedMultiply and XFixedDivide with integer versions rather 
**              than floating point
**  6/1/96      Added some resource management code
**  6/5/96      Added some Win95 stuff.
**  6/28/96     Added BeBox stuff
**  6/30/96     Changed font and re tabbed
**              Moved DecompressSampleFormatPtr from MacSpecificSMS
**              Moved XSetVolume & XGetVolume from MacSpecificSMS
**              Moved DecompressPtr from MacSpecificSMS
**  7/1/96      Added XCreateAccessCache
**              Changed XFILERESOURCEID to XFILERESOURCE_ID
**  7/2/96      Changed XFileOpenResource to support read only access
**  7/3/96      Added XStrCmp
**  7/4/96      Added error checking for opening read only resources
**  7/7/96      Added XStrnCmp
**  7/18/96     Modifed XFileOpenResource to read Cache resource
**              Fixed a NULL reference in PV_XFileValid
**              Fixed problem with cached resources and byte ordering
**  7/21/96     Fixed dumb bug in XGetFileResource that caused extra file scanning
**  7/23/96     Added XMicroseconds
**  8/6/96      Added XGetIndexedType & XCountTypes
**  8/12/96     Moved XExpandMace1to3 & XExpandMace1to6 to X_Compress.c
**  9/22/96     Added XRandom & XSeedRandom & XRandomRange
**  10/8/96     Added XStrCpy
**  10/11/96    Added XConvertNativeFileToXFILENAME
**              Added XFileSetPositionRelative
**              Added XDetermineByteOrder
**              Added XSwapLong & XSwapShort
**  10/15/96    Added XFileOpenResourceFromMemory
**  10/23/96    Changed XGetAndDetachResource to support our resource manager
**              if a file has been opened
**              Changed XFileClose to NULL out currentResourceFile when closing
**              file
**  11/7/96     Fixed a bug in XMicroseconds
**  11/8/96     Fixed bug in XStrCpy. Forgot to copy zero terminator
**  11/14/96    Added XGetResourceName & XGetNamedResource
**              Removed dependancy on MacSpecificSMS.h
**  11/26/96    Changed file read status native to MacOS from read/write
**              to read only
**  12/3/96     Removed C++ warnings on void * to char * conversion
**  12/4/96     Added XStrLen
**  12/15/96    Added X_SOLARIS
**  12/17/96    Fixed a bug in XBlockMove for WinOS.
**  12/18/96    Added all Solaris APIs
**  12/19/96    Added create flag in XFileOpenForWrite
**  12/30/96    Changed copyright
**  1/2/97      Added XCtoPstr & XPtoCstr
**  1/12/97     Renamed DecompressPtr to XDecompressPtr
**  1/13/97     Added XDuplicateStr
**  1/16/97     Fixed bug with XStrLen that added extra byte
**  1/20/97     Added XLStrCmp
**              Put in more support for MacOS native resource management
**  1/21/97     Added XLStrnCmp
**  1/23/97     Special cased XFixedMultiply to handle zero faster
**  1/24/97     Fixed XWaitMicroseocnds on WinOS to handle threads better
**  1/28/97     Changed XGetFileResource to not duplicate a resource if its
**              being loaded from memory
**              Added some Liberate Code
**              Fixed a bug with XIsOurMemoryPtr (Thanks Jeff!). Forgot to
**              get the data blocks in a platform way.
**              Added XGetIndexedResource
**  1/29/97     Fixed some platform bugs with XGetIndexedFileResource
**              Added XCompressPtr
**  2/4/97      Fixed XBlockMove for Liberate case. It was backwards
**  2/5/97      Added XFileOpenForReadFromMemory
**  2/6/97      Fixed a bug with XFileOpenResourceFromMemory that creates a cache,
**              its not needed for memory based resources
**  2/11/97     Modified XGetIndexedFileResource to support MacOS native resource manager
**  2/18/97     Fixed XFileOpenResource & XFileOpenResourceFromMemory to fail if file
**              is not a XFile resource
**  2/19/97     Added XStrStr
**  4/22/97     Fixed a bug with XGetPtrSize that referenced a NULL pointer
**
**  6/5/97      (Beatnik Editor Windows)
**              bvk - Changed the _creat call (in XFileOpenResource) to use the right 
**              flags based on this goofy Microsoft C++ 5.0 (_S_IREAD | _S_IWRITE 
**              instead of _O_RDWR).  Added an include (sys/stat.h).
**
**  6/12/97     bvk - Added XGetResourceNameOnly().  Copied/modified from other code.
**              Doesn't load the resource data, just checks resource headers.
**  6/13/97     bvk - Added XCountFileResourcesOfType(XFILE fileRef, long int theType);
**
**  6/19/97     bvk - Fixed bug in XAddFileResource.  Wasn't flipping the 'next' value,
**                  Fixed bug getting name in XGetResourceNameOnly().
**
**  6/22/97     bvk - Changed XGetPtrSize(XPTR data).  It used to innocently assume that
**                  it could get the handle for any data pointer to get the size.  That
**                  actually leaves a lot of room for error...
**
**  6/27/97     bvk - Added XDeleteFileResource.  This functions in two ways, either to delete
**                  the resource immediately, or to just mark it as a 'TRSH' resource.
**                  Then, trash collection occurs when XCleanResourceFile() is called.
**                  For either case, resource deletion fails if the file has been opened as
**                  read only.
**
**  7/02/97     bvk - Fixed bug in XGetResourceNameOnly().
**
**  7/03/97     bvk - Fixed bug in XCleanResourceFile() that was writing the # of resources wrong.
**
**  7/05/97     bvk - Modified the resource info caching system.  Read carefully:
**                  Originally, the resource cache was only created if the res
**                  file was opened ReadOnly.  Ostensibly, this was because the
**                  resource map cache would be out of date should a resource add
**                  or delete take place.
**
**                  Now, the cache is used for all types of files.  It is updated
**                  correctly should the res file have added or deleted resources.
**
**                  I also rewrote some frequently called routines to use the cache.
**
**                  I also disabled loading the pre-generated cache from the resource file.
**                  Too many of my files didn't have 'em (and none of these routines write it out!)
**
**  5/3/97      Fixed a few potential problems that the MOT compiler found
**  5/13/97     Fixed extra byte swap with XGetIndexedFileResource. Thanks Moe!
**  6/20/97     Changed XGetHardwareVolume & XSetHardwareVolume for MacOS
**              to support news names for CW 2.0
**  6/30/97     Fixed a bug in XCompressPtr that forgot the extra four bytes! Thanks Moe!
**  7/14/97     Put in special case to XFixedDivide to handle zero values better and faster.
**              Put in support for new X_PLATFORM type, called X_WIN_BAE
**  7/17/97     Removed XIsVirtualMemoryAvailable & XLockMemory & XUnlockMemory. Because
**              its assumed that all memory is locked.
**              Removed XSetHardwareSampleRate & XGetHardwareSampleRate
**  7/18/97     Added XFileSetLength
**  7/21/97     Changed XGetResourceName to call XGetResourceNameOnly.
**  7/30/97     Removed references to creat because its not ANSI, now using the ANSI way which
**              is to call open with O_CREAT flag.
**  8/13/97     Fixed bug in XDuplicateAndStripStr that didn't increment counter the right way
**  8/18/97     Changed X_WIN_BAE to USE_BAE_EXTERNAL_API
**  9/3/97      Wired XGetHardwareVolume & XSetHardwareVolume to BAE_EXTERNAL_API
**              Wrapped a few more functions around USE_CREATION_API
**  9/29/97     Changed XSetBit & XClearBit & XTestBit to be unsigned long rather than long
**  10/2/97     Fixed a bug with XGetFileResource when trying to load a memory based resource
**              it still continued searching after it found what it was looking for.
**              Fixed XGetIndexedFileResource to support memory based resource files without
**              duplicating memory
**  10/14/97    Fixed bug with XFileOpenForWrite that didn't allow creation of files.
**  10/18/97    Modified XDisposePtr to mark blocks deallocated
**  11/19/97    Modified XNewPtr to allocate memory for performance
**  12/16/97    Moe: removed compiler warnings
**  12/17/97    bvk - Modified PV_XGetNamedCacheEntry().  Here's the problem:
**              Memory resource files don't have caches.  So how does a function 
**              like XGetNamedResource(), that relies on PV_XGetNamedCacheEntry() 
**              work?  IT DOESN'T.
**              PV_XGetNamedCacheEntry() now will search the whole file if for the
**              named resource if it is a memory file!
**  12/18/97    bvk - Modified XGetAndDetachResource so that in the case of memory files,
**              it truly creates a new buffer (so that calls to dispose of the resource
**              don't fail/nuke the res file).
**  1/8/98      MOE: added XSwapLongsInAccessCache()
**  1/9/98      Added XFileDelete
**  1/21/98     Changed the functions XGetShort & XGetLong & XPutShort & XPutLong
**              into macros that fall out for Motorola order hardware
**  1/26/98     Added XERR to various functions
**  1/31/98     Moved XPI_Memblock structures to X_API.h, and moved the function
**              XIsOurMemoryPtr to X_API.h
**  2/7/98      Changed XFIXED back to an unsigned long to fix broken content
**  2/9/98      Fixed bug with memory files and XFileClose in which the memory
**              block allocated outside of the resource file system was deallocated!
**  3/18/98     Added XIs8BitSupported
**  3/23/98     MOE: Changed _XGetShort() and _XGetLong() to accept const* pointers
**  3/23/98     MOE: Added _XGetShortIntel() and _XGetLongIntel()
**  3/23/98     MOE: Fixed big memory leak in XCompressPtr()
**  4/15/98     Removed the conditional compile for XGet/XPutXXX functions
**  4/20/98     Fixed a bug with XGetIndexedFileResource that return the wrong resource length
**              Fixed a alignment bug with Solaris only that caused memory to not be freed in
**              XIsOurMemoryPtr and XNewPtr. Did this by changing the size of the XPI_Memblock
**              structure to 16 bytes rather than 12.
**  4/27/98     Changed XCompressPtr to handle XCOMPRESSION_TYPE
**  4/27/98     MOE:  Changed XDecompressPtr() and XCompressPtr(),
**              eliminated XDecompressSampleFormatPtr()
**  5/7/98      Added XGetTempXFILENAME
**  5/12/98     Added some missing MacOS header files
**
**  6/5/98      Jim Nitchals RIP    1/15/62 - 6/5/98
**              I'm going to miss your irreverent humor. Your coding style and desire
**              to make things as fast as possible. Your collaboration behind this entire
**              codebase. Your absolute belief in creating the best possible relationships 
**              from honesty and integrity. Your ability to enjoy conversation. Your business 
**              savvy in understanding the big picture. Your gentleness. Your willingness 
**              to understand someone else's way of thinking. Your debates on the latest 
**              political issues. Your generosity. Your great mimicking of cartoon voices. 
**              Your friendship. - Steve Hales
**
**  6/18/98     Added XFileFreeResourceCache
**  7/1/98      Changed various API to use the new XResourceType and XLongResourceID
**  7/6/98      Fixed some type problems with XCompressPtr
**              Changed _XPutShort to pass a unsigned short rather than an unsigned long
**  7/10/98     Added XGetUniqueFileResourceID & XGetUniqueResourceID & XAddResource
**              Added XDeleteResource
**              Added XCountResourcesOfType
**              Added XCleanResource
**              Added XFileGetCurrentResourceFile
**  7/13/98     Fixed bug with XGetUniqueFileResourceID & XRandom
**  7/15/98     Fixed bug with XGetResourceName inwhich the name being returned was a
**              pascal string, rather than a C string
**  7/17/98     Fixed bug with XCleanResourceFile inwhich a errant buffer might be
**              deallocated twice
**  7/20/98     Fixed a problem with XConvertNativeFileToXFILENAME that didn't clear
**              the XFILENAME if there was a problem
**  9/10/98     Put XGetFileAsData into the common build code because BAE.cpp uses it,
**              and the new exporter API uses it.
**              Fixed XGetIndexedType & XCountTypes to handle a NULL file reference being passed
**              in meaning use from most recently opened file.
**  9/30/98     Added test to XNewPtr to fail if size passed in is zero.
**  11/11/98    Cleaned up some left over bark in XGetAndDetachResource. Renamed pName to pResourceName
**              because of a MacOS macro conflict.
**              Added XReadPartialFileResource
**              Removed unused macros XGet/Put and placed them back into
**              real functions
**  11/18/98    Fixed a bug with XFileClose in which a file that was closed
**              twice would crash rather than get ignored.
**  12/21/98    Fixed a memory leak in XCleanResourceFile
**  12/22/98    Added USE_FILE_CACHE to control cache usage
**              Modified XCleanResourceFile to delete the cache resource if there is one, because
**              it can become invalid after a clean or delete. Also modified XAddFileResource to
**              delete the file cache
**  1/5/98      Fixed a warning in XFileClose and changed copyrights
**  2/8/99      Added XLStrStr & XStrCat
**  2/21/99     Added XStripStr & XFileCreateResourceCache
**  3/1/99      Removed some warnings for BeOS compiles
**  3/16/99     MOE:  Changed XCompressPtr() to use new LZSS... parameters
**  3/25/99     MOE:  Added procData parameter to functions using XCompressStatusProc
**  5/13/99     Fixed bug with XDuplicateAndStripStr in which it stripped characters
**              outside the 0-32 range.
**  5/20/99     Changed XGetTempXFILENAME on Mac to create temp files in the trash.
**  5/26/99     MOE:  Added XResizePtr()
**  5/26/99     MOE:  Made XStrLen() more efficient and straightforward
**  5/28/99     MOE:  Changed XStrLen() & XEncryptedStrLen() to return long,
**              and accept const pointers
**  5/28/99     MOE:  Changed the string/memory manipulation functions
**              to accept const pointers
**  6/8/99      Removed references to USE_BAE_EXTERNAL_API. We now require BAE_API
**  7/13/99     Renamed HAE to BAE. Renamed BAEAudioMixer to BAEAudioOutput. Renamed BAEMidiDirect
**              to BAEMidiSynth. Renamed BAEMidiFile to BAEMidiSong. Renamed BAERMFFile to BAERmfSong.
**              Renamed BAErr to BAEResult. Renamed BAEMidiExternal to BAEMidiInput. Renamed BAEMod
**              to BAEModSong. Renamed BAEGroup to BAENoiseGroup. Renamed BAEReverbMode to BAEReverbType.
**              Renamed BAEAudioNoise to BAENoise.
**  7/27/99     Fixed misspelling in XWaitMicroseocnds
**  8/10/99     Changed XFileSetPositionRelative & XFileSetLength to return an XERR
**  8/11/99     MOE: Changed XAddFileResource() to fix two problems:
**                  writing name was failing but XFileWrite()'s return value was ignored
**                  cacheItem.fileOffsetData was not being set right
**                      (this problem was masked by the fact that the built cache was never used)
**  8/11/99     MOE: Added XCtoPascalString()
**  8/11/99     MOE: Made XCtoPstr() safely handle strings longer than 255 characters
**  8/11/99     MOE:  Added XLongToStr()
**  8/17/99     Fixed bug in XDeleteFileResource in which if a cache is used and the resource is
**              not in the cache, it now will search the file and delete it.
**  8/19/99     Fixed XCleanResourceFile to not ignore an error returned from changing the length of
**              a file. Also fixed XDeleteFileResource to not ignore the error returned from calling
**              XCleanResourceFile.
**  8/28/99     Renamed resourceFileCount & openResourceFiles to g_resourceFileCount & g_openResourceFiles
**              to emphisize their global nature. This is not thread safe. If you need thread safe
**              resource access, only access the resource API's that you pass in an XFILE.
**              This is going to change.
**              Increased MAX_OPEN_XFILES to 10 open files at once. Removed obsolete macro USE_WIN32_FILE_IO.
**  9/1/99      Changed PV_CopyWithinFile to return an XERR rather than a int.
**  9/8/99      MOE: Changed XResizePtr() to check for same-size reallocation
**  9/8/99      MOE: Changed XResizePtr() to not always attempt to reallocate large blocks
**              This is a hack to be taken out when BAE_ResizePointer() exists
**  9/8/99      MOE: Added XGetFileResourceName(),
**              XExistsResource(), XExistsFileResource(),
**              XMakeUniqueResourceID(), XMakeUniqueFileResourceID()
**              Changed XGetResourceName() to return whether it found a resource,
**              and changed it so that searching would stop if it finds a
**              resource with a NULL name.  (It had been falling through to
**              match a resource in a lower-level resource file if the higher
**              level resource had a NULL name.)
**  10/7/99     Fixed bugs in XGetResourceName in which pascal strings were not being
**              converted to C strings prior to returning. Also failed to handle
**              the Mac native resource case
**  10/9/99     Modified PV_AddResourceFileToOpenFiles & PV_RemoveResourceFileFromOpenFiles
**              to fail if called from different threads
**              Fixed bug in XFileOpenResource in which an attempt to open a resource left it
**              open and in the search change even though it failed the validation test
**              Fixed XGetResourceNameOnly to search through Mac resources if non found in XFILE
**              resources
**  10/20/99    MSD: moved X_PLATFORM dependent #includes to BAEBuildOptions_*.h
**  12/13/99    Modified XFileClose to call XFileFreeResourceCache to clear the RAM based cached.
**              Fixed bug in XAddFileResource in which the RAM based cache that is maintained when
**              adding new resources with PV_AddToAccessCache was not storing into the structure
**              in the native endian mode, so cache entries would become invalid unless you closed
**              the file and reopened it.
**  1/21/00     Fixed a few cast issues with XFILE and MacOS.
**              MSD: added XConvertPathToXFILENAME()
**  1/25/00     Modified XConvertPathToXFILENAME to not convert a path if its larger than 255
**              bytes. Pascal string limit for MacOS
**  2/4/2000    Changed copyright. We're Y2K compliant!
**  3/14/2000   Fixed XGetNamedResource to search through the file for a named resource
**              if the file cache system is not enabled.
**  3/15/2000   Fixed XGetNamedResource to work correctly under MacOS.
**  3/29/2000   jsc: XFileOpenResource was leaving file open on failure.
**  4/14/2000   jsc: moved MAX_OPEN_XFILES to X_API.h and increased from 10 to 100
**  5/2/2000    sh: Added XFileIsValidResourceFromName & XFileIsValidResource
**  7/07/2000   DS: Created Mutex "object" for future use -- currently ifdef'd out.
**  2000.07.14 AER  Due to collisions between XBankTokens discovered in the
**                  field, I've made XBankToken a struct with some added data.
**                  It should collide less now (although without a better
**                  GUID-style system, there's still the potential)
**                  Added functions to more easily create and read them
**  7/25/2000   sh  Removed PV_IsVirtualMemoryAvailable. It was MacOS specific
**                  and not used here anymore.
**  10/1/2000   sh  Changed XConvertPathToXFILENAME to call the internal c to
**                  pascal string function. Conflicts with MacOS X Carbon.
**  12/21/2000  sh  Forced a setting in XFileOpenResourceFromMemory. There's a bug
**                  when passing FALSE to allowMemCopy into this function. NOT FIXED.
**                  Needs to be looked into.
**  1/24/2002   sh  Removed XGetHardwareVolume/XSetHardwareVolume, they were
**                  duplicated by BAE_GetHardwareVolume/BAE_SetHardwareVolume
**                  Fixed declaration of XDuplicateMemory
**  5/2/2001    sh  Added cool Mac trick to XConvertPathToXFILENAME to convert unix/dos
**                  paths to Mac FSSpec. Also handles relative paths.
**  5/29/2001   sh  Added new debugging system with BAE_PRINTF
**  7-16-2001   dcm Added unsigned long XSwapShortInLong(unsigned long value) to swap shorts in long
**  8/28/2001       Fixed massive memory leak in XGetAndDetachResource when using memory files.
**                  Was duplicating resource when it was not required.
**  1/24/2002   sh  Removed XGetHardwareVolume/XSetHardwareVolume, they were
**                  duplicated by BAE_GetHardwareVolume/BAE_SetHardwareVolume
**                  Fixed declaration of XDuplicateMemory
*/
/*****************************************************************************/

#include "X_API.h"
#include "X_Formats.h"
#include "X_Assert.h"
#include "BAE_API.h"


#define DEBUG_PRINT_RESOURCE        0
#define USE_FILE_CACHE              0   // if 1, then file cache is enabled

// Structures

// Variables
static short int    g_resourceFileCount = 0;                // number of open resource files
static XFILE        g_openResourceFiles[MAX_OPEN_XFILES];

// Private functions

// Check for a valid file reference
static XBOOL PV_XFileValid(XFILE fileRef)
{
    XFILENAME   *pReference;
    XBOOL       valid;

    valid = FALSE;
    pReference = (XFILENAME *)fileRef;
    if (pReference)
    {
        int         code;
        code = BAE_IsBadReadPointer(&pReference->fileValidID, sizeof(long));
        if (code == 0 || code == 2)
        {
            if (pReference->fileValidID == XPI_BLOCK_3_ID)
            {
                valid = TRUE;
            }
        }
    }
    return valid;
}

// Given a valid XFILE, this will return the open index. Will return -1 if not valid
static short int PV_FindResourceFileReferenceIndex(XFILE fileRef)
{
    short int   count;

    for (count = 0; count < g_resourceFileCount; count++)
    {
        if (g_openResourceFiles[count] == fileRef)
        {
            return count;
        }
    }
    return -1;
}

// add an newly open resource file to the resource search path.
// NOTE:    This is not thread safe. There's a hack cause this function to fail if another
//          thread is trying to add at the same time.
static XBOOL PV_AddResourceFileToOpenFiles(XFILE fileRef)
{
    XBOOL           full;
    short int       count;
    static XBOOL    inUse = FALSE;

    full = TRUE;
    if (inUse == FALSE)
    {
        inUse = TRUE;   // this is a temporary fix for thread issues

        if (g_resourceFileCount < MAX_OPEN_XFILES)
        {
            for (count = MAX_OPEN_XFILES-2; count >= 0; count--)
            {
                g_openResourceFiles[count+1] = g_openResourceFiles[count];
            }
            g_openResourceFiles[0] = fileRef;
            g_resourceFileCount++;
            full = FALSE;
        }
        inUse = FALSE;
    }
    return full;
}

// remove an open resource file to the resource search path.
// NOTE:    This is not thread safe. There's a hack cause this function to fail if another
//          thread is trying to remove at the same time.
static void PV_RemoveResourceFileFromOpenFiles(XFILE fileRef)
{
    short int       count;
    short int       found;
    static XBOOL    inUse = FALSE;

    found = -1;
    if (inUse == FALSE)
    {
        inUse = TRUE;   // this is a temporary fix for thread issues

        for (count = 0; count < g_resourceFileCount; count++)
        {
            if (g_openResourceFiles[count] == fileRef)
            {
                found = count;
                break;
            }
        }
        if (found != -1)
        {
            for (count = found; count < g_resourceFileCount-1; count++)
            {
                g_openResourceFiles[count] = g_openResourceFiles[count+1];
            }
            g_openResourceFiles[count] = 0;
            g_resourceFileCount--;
        }
        inUse = FALSE;
    }
}

static INLINE XBOOL PV_IsAnyOpenResourceFiles(void)
{
    return (g_resourceFileCount) ? TRUE : FALSE;
}

// Functions

// convert a c string to a pascal string
void XCtoPascalString(char const* cString, char pascalString[256])
{
    long        length;

    if (cString && pascalString)
    {
        length = XStrLen(cString);
        if (length > 255) length = 255;
        XBlockMove(cString, pascalString + 1, length);
        pascalString[0] = (char)length;
    }
}

// convert a c string to a pascal string
void* XCtoPstr(void* cstr)
{
    XCtoPascalString((char const *)cstr, (char *)cstr);
    return cstr;
}

// convert a pascal string to a c string
void * XPtoCstr(register void *pstr)
{
    register char * source;
    register char * dest;
    register long   length;
    char            data[256];

    if (pstr)
    {
        length = *((unsigned char *)pstr);
        source = (char *)pstr;
        source++;
        dest = data;

        while (length--)
        {
            *dest++ = *source++;
        }
        *dest = 0;
        length = *((unsigned char *)pstr) + 1;
        XBlockMove(data, pstr, length);
    }
    return pstr;
}


// Determine if a data block was allocated with our memory allocation API
XPI_Memblock * XIsOurMemoryPtr(XPTR data)
{
    char            *pData;
    XPI_Memblock    *pBlock, *pBlockReturn;
    short int       code;

    pBlockReturn = NULL;
    if (data)
    {
        pData = (char *)data;
        pData -= sizeof(XPI_Memblock);
        pBlock = (XPI_Memblock *)pData;
        code = BAE_IsBadReadPointer(pBlock, sizeof(XPI_Memblock));
        if (code == 0 || code == 2)
        {
            if ( (XGetLong(&pBlock->blockID_one) == XPI_BLOCK_1_ID) &&
                (XGetLong(&pBlock->blockID_two) == XPI_BLOCK_2_ID) )
            {
                pBlockReturn = pBlock;
            }
        }
    }
    return pBlockReturn;
}

// This function re-allocates a memory block
// ptr may be NULL, in which case the functionality is the same as XNewPtr()
// If allocation fails, ptr is unaffected (It's still allocated.)
// Like with XNewPtr(), any newly allocated memory is zeroed.
XPTR XResizePtr(XPTR ptr, long size)
{
long        const currentSize = ptr ? XGetPtrSize(ptr) : 0;

    //MOE:  If we are allocating to a certain quantum (typically 16 bytes),
    //      We should apply that quantum before this test.
    if (size != currentSize)
    {
    XPTR        newPtr;

#if TRUE    // disable when these is an BAE_ResizePointer()
        if ((size < currentSize) &&
            (size >= 0x100000) &&               // if over 1Mb would have to be allocated
            (currentSize - size <= 0x10000) &&  // if less than 64Kb would be saved
            XIsOurMemoryPtr(ptr))
        {
            newPtr = NULL;  // force hacky but quick pseudo resizing
        }
        else
#endif
        {
            newPtr = XNewPtr(size);
        }

        if (newPtr && ptr)
        {
        long        copyLength;
        
            copyLength = currentSize;
            if (copyLength > size)
            {
                copyLength = size;
            }
            XBlockMove(ptr, newPtr, copyLength);
            XDisposePtr(ptr);
        }
        else if (ptr && (size < currentSize))
        {
        XPI_Memblock*       odata;

            odata = (XPI_Memblock*)XIsOurMemoryPtr(ptr);
            if (odata)
            {
                odata->blockSize = size;
                return ptr;
            }
        }
        return newPtr;
    }
    return ptr;
}

// Allocates a block of ZEROED!!!! memory and locks it down
XPTR XNewPtr(long size)
{
    char            *data;
    XPI_Memblock    *pBlock;

    if (size == 0)
    {
        return NULL;
    }
    size += sizeof(XPI_Memblock);
    data = (char *)BAE_Allocate(size);
    if (data)
    {
        pBlock = (XPI_Memblock *)data;
        XPutLong(&pBlock->blockID_one, XPI_BLOCK_1_ID);         // set our ID for this block
        XPutLong(&pBlock->blockID_two, XPI_BLOCK_2_ID);
        data += sizeof(XPI_Memblock);
        pBlock->blockSize = size - sizeof(XPI_Memblock);
    }
    return (XPTR)data;
}

void XDisposePtr(XPTR data)
{
    long            size;
    XPTR            osAllocatedData;
    XPI_Memblock    *pBlock;

    osAllocatedData = (XPTR)XIsOurMemoryPtr(data);
    if (osAllocatedData)
    {
        size = XGetPtrSize(data);   // need to get the size before we translate the pointer

        pBlock = (XPI_Memblock *)osAllocatedData;
        XPutLong(&pBlock->blockID_one, (unsigned long)XPI_DEAD_ID);         // set our ID for this block
        XPutLong(&pBlock->blockID_two, (unsigned long)XPI_DEAD_ID);         // to be dead. Used for tracking
        BAE_Deallocate(osAllocatedData);
    }
}

long XGetPtrSize(XPTR data)
{
    long        size;
    XPTR        odata;

    size = 0;
    if (data)
    {
        odata = (XPTR)XIsOurMemoryPtr(data);
        if (odata)
        {
            size = ((XPI_Memblock *)odata)->blockSize;
        }
        else
        {
            // then this block is not ours, so use the system to determine its real size
            printf("bae memory mismatch\n");
            BAE_ASSERT(TRUE);
        }
    }
    return size;
}

// Typical block move, expect it must be able to handle overlapping source and
// destination pointers
void XBlockMove(XPTRC source, XPTR dest, long size)
{
    if (source && dest && size)
    {
        BAE_BlockMove((void *)source, dest, size);
    }
}

// set memory range with value
void XSetMemory(void *pAdr, long len, char value)
{
    if (pAdr && (len > 0)) {
        memset(pAdr, value, len);
    }
}

// Given a pointer, and a bit number; this will set that bit to 1
void XSetBit(void *pBitArray, unsigned long whichbit)
{
    unsigned long   byteindex, bitindex;
    unsigned char *byte;

    if (pBitArray)
    {
        byteindex = whichbit / 8;
        bitindex = whichbit % 8;
        byte = &((unsigned char *)pBitArray)[byteindex];
        *byte |= (1L << (7-bitindex));
    }
}

// Given a pointer, and a bit number; this will set that bit to 0
void XClearBit(void *pBitArray, unsigned long whichbit)
{
    unsigned long   byteindex, bitindex;
    unsigned char *byte;

    if (pBitArray)
    {
        byteindex = whichbit / 8;
        bitindex = whichbit % 8;
        byte = &((unsigned char *)pBitArray)[byteindex];
        *byte &= ~(1L << (7-bitindex));
    }
}

// Given a pointer, and a bit number; this return the value of that bit
XBOOL XTestBit(void *pBitArray, unsigned long whichbit)
{
    register unsigned long  byteindex, byte, bitindex;
        
    if (pBitArray)
    {
        byteindex = whichbit / 8;
        bitindex = whichbit % 8;
        byte = ((unsigned char *)pBitArray)[byteindex];
        return (byte & (1L << (7-bitindex))) ? TRUE : FALSE;
    }
    else
    {
        return FALSE;
    }
}

/* Sort an integer array from the lowest to the highest
*/
#if USE_CREATION_API == TRUE
void XBubbleSortArray(short int theArray[], short int theCount)
{
    register short int i, j, swapValue;
    
    for (i = 0; i < (theCount - 1); i++)
    {
        for (j = i + 1; j < theCount; j++)
        {
            if (theArray[i] > theArray[j])
            {
                swapValue = theArray[i];
                theArray[i] = theArray[j];
                theArray[j] = swapValue;
            }
        }
    }
}
#endif  // USE_CREATION_API


// Does sound hardware support stereo output
XBOOL XIsStereoSupported(void)
{
    return BAE_IsStereoSupported();
}

// wait for a waitAmount of microseconds to pass
// CLS??: If this function is called from within the frame thread and
// JAVA_THREAD is non-zero, we'll probably crash.
void XWaitMicroseocnds(unsigned long waitAmount)
{
    BAE_WaitMicroseconds(waitAmount);
}


// Returns microseconds since boot
// 1 second = 1000 milliseconds = 100000 microseconds
unsigned long XMicroseconds(void)
{
    return BAE_Microseconds();
}

// Does sound hardware support 16 bit output
XBOOL XIs16BitSupported(void)
{
    return BAE_Is16BitSupported();
}

// Does sound hardware support 8 bit output
XBOOL XIs8BitSupported(void)
{
    return BAE_Is8BitSupported();
}

// do a 16.16 fixed point multiply
XFIXED XFixedMultiply(XFIXED factor1, XFIXED factor2)
{
    XFIXED  result;
    
    if (factor1 && factor2)
    {
        result = (((factor1 >> 16L) & 0xFFFFL) * ((factor2 >> 16L) & 0xFFFFL)) << 16L;
        result += ((factor1 >> 16L) & 0xFFFFL) * ( factor2 & 0xFFFFL);
        result += (factor1 & 0xFFFFL) * ((factor2 >> 16L) & 0xFFFFL);
        result += (((factor1 & 0xFFFFL) * (factor2 & 0xFFFFL)) >> 16L) & 0xFFFFL;
    }
    else
    {
        result = 0;
    }
    return result;
}

// do a 16.16 fixed point divide
XFIXED XFixedDivide(XFIXED dividend, XFIXED divisor)
{
    XFIXED  result;
    
    if (divisor && dividend)
    {
        XFIXED      temp, rfactor, f2;
        int         i;
        
        f2 = divisor;
        temp = dividend;
        result = 0;
        rfactor = 0x10000L;
        for (i = 0; i < 16; i++)
        {
            while ((temp >= f2) && (rfactor != 0) && (temp != 0))
            {
                temp -= f2;
                result += rfactor;
            }
            f2 = f2 >> 1L;  
            rfactor = rfactor >> 1L;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

// 1/4 of cos/sin tables in 16.16 fixed point math
// 65536 = 1.0, 1144 = .01745605469
static const XFIXED cosSinTable90[] = 
{
    // 0-44
    65536, 65526, 65496, 65446, 65376, 65287, 65177, 65048, 64898, 64729, 64540, 64332, 
    64104, 63856, 63589, 63303, 62997, 62672, 62328, 61966, 61584, 61183, 60764, 60326, 
    59870, 59396, 58903, 58393, 57865, 57319, 56756, 56175, 55578, 54963, 54332, 53684, 
    53020, 52339, 51643, 50931, 50203, 49461, 48703, 47930, 47143, 
    // 45-89
    46341, 45525, 44695, 43852, 42995, 42126, 41243, 40348, 39441, 38521, 37590, 36647, 
    35693, 34729, 33754, 32768, 31772, 30767, 29753, 28729, 27697, 26656, 25607, 24550, 
    23486, 22415, 21336, 20252, 19161, 18064, 16962, 15855, 14742, 13626, 12505, 11380, 
    10252, 9121, 7987, 6850, 5712, 4572, 3430, 2287, 1144, 0
};


// angle = 0-359. returns 16.16 value
XFIXED XFixedSin(int angle)
{
    XFIXED  sin = 0;
    
    angle = angle % 360;
    
    if ( (angle >= 0) && (angle < 90) )
    {
        sin = cosSinTable90[90 - angle];    // 0-89
    }
    else if ( (angle >= 90) && (angle < 180) )
    {
        sin = cosSinTable90[angle - 90];    // 90-179
    }
    else if ( (angle >= 180) && (angle < 270) )
    {
        sin = -cosSinTable90[270 - angle];  // 180-269
    }
    else if ( (angle >= 270) && (angle <= 359) )
    {
        sin = -cosSinTable90[angle - 270];  // 270-359
    }
    return sin;
}

// angle = 0-359. returns 16.16 value
XFIXED XFixedCos(int angle)
{
    XFIXED  cos = 0;
    
    angle = angle % 360;
    
    if ( (angle >= 0) && (angle < 90) )
    {
        cos = cosSinTable90[angle]; // 0-89
    }
    else if ( (angle >= 90) && (angle < 180) )
    {
        cos = -cosSinTable90[180 - angle];  // 90-179
    }
    else if ( (angle >= 180) && (angle < 270) )
    {
        cos = -cosSinTable90[angle - 180];  // 180-269
    }
    else if ( (angle >= 270) && (angle <= 359) )
    {
        cos = cosSinTable90[90 - (angle - 270)];    // 270-359
    }
    
    return cos;
}

// given a fixed point value, do a floor and return the closest integer
unsigned long XFixedFloor(XFIXED value)
{
    return ((value + XFIXED_ONEHALF) >> 16L);
}

// given a pointer, get a short int ordered in an Intel way
unsigned short int XGetShortIntel(void const* pData)
{
    register unsigned char  *pByte;
    register unsigned short data;

    pByte = (unsigned char *)pData;
    data = ((unsigned short)pByte[1] << 8) |
           (unsigned short)pByte[0];
    return data;
}
// given a pointer, get a long ordered in an Intel way
unsigned long XGetLongIntel(void const* pData)
{
    register unsigned char  *pByte;
    register unsigned long  data;

    pByte = (unsigned char *)pData;
    data = ((unsigned long)pByte[3] << 24L) |
           ((unsigned long)pByte[2] << 16L) | 
           ((unsigned long)pByte[1] << 8L) |
           (unsigned long)pByte[0];
    return data;
}

// given a pointer, get a short int ordered in a Motorola way
unsigned short int XGetShort(void const* pData)
{
    register unsigned char  *pByte;
    register unsigned short data;

    pByte = (unsigned char *)pData;
    data = ((unsigned short int)pByte[0] << 8) |
           (unsigned short int)pByte[1];
    return data;
}
// given a pointer, get a long ordered in a Motorola way
unsigned long XGetLong(void const* pData)
{
    register unsigned char  *pByte;
    register unsigned long  data;

    pByte = (unsigned char *)pData;
    data = ((unsigned long)pByte[0] << 24L) |
           ((unsigned long)pByte[1] << 16L) | 
           ((unsigned long)pByte[2] << 8L) |
           (unsigned long)pByte[3];
    return data;
}
// given a pointer and a value, this with put a short in a ordered 68k way
void XPutShort(void *pData, unsigned short data)
{
    register unsigned char  *pByte;

    pByte = (unsigned char *)pData;
    pByte[0] = (UBYTE)((data >> 8) & 0xFF);
    pByte[1] = (UBYTE)(data & 0xFF);
}

// given a pointer and a value, this with put a long in a ordered 68k way
void XPutLong(void *pData, unsigned long data)
{
    register unsigned char  *pByte;

    pByte = (unsigned char *)pData;
    pByte[0] = (UBYTE)((data >> 24) & 0xFF);
    pByte[1] = (UBYTE)((data >> 16) & 0xFF);
    pByte[2] = (UBYTE)((data >> 8) & 0xFF);
    pByte[3] = (UBYTE)(data & 0xFF);
}

unsigned long XSwapLong(unsigned long value)
{
    unsigned long   newValue;
    unsigned char   *pOld;
    unsigned char   *pNew;
    unsigned char   temp;

    pOld = (unsigned char *)&value;
    pNew = (unsigned char *)&newValue;

    temp = pOld[0];
    pNew[3] = temp;
    temp = pOld[1];
    pNew[2] = temp;
    temp = pOld[2];
    pNew[1] = temp;
    temp = pOld[3];
    pNew[0] = temp;

    return newValue;
}

unsigned short XSwapShort(unsigned short value)
{
    unsigned short  newValue;
    unsigned char   *pOld;
    unsigned char   *pNew;
    unsigned char   temp;

    pOld = (unsigned char *)&value;
    pNew = (unsigned char *)&newValue;

    temp = pOld[0];
    pNew[1] = temp;
    temp = pOld[1];
    pNew[0] = temp;

    return newValue;
}
// Swap shorts within a long, but not the bytes in each each short.

unsigned long XSwapShortInLong(unsigned long value)
{
    unsigned long   newValue;
    unsigned short  *pOld;
    unsigned short  *pNew;
    unsigned short  temp;

    pOld = (unsigned short *)&value;
    pNew = (unsigned short *)&newValue;

    temp = pOld[0];
    pNew[1] = temp;
    temp = pOld[1];
    pNew[0] = temp;

    return newValue;
}
// if TRUE, then motorola; if FALSE then intel
XBOOL XDetermineByteOrder(void)
{
    static long value = 0x12345678;
    XBOOL       order;

    order = FALSE;
    if (XGetLong(&value) == 0x12345678)
    {
        order = TRUE;
    }
    return order;
}

// given a path (MacHD:Folder:Path for MacOS, c:\folder\path for WinOS), fill in a XFILENAME
// If Mac, this will convert '/', '\' to ':'
// Also if Mac, this will convert relative paths, ie. ..\..\folder\file to mac equals.
void XConvertPathToXFILENAME(void *path, XFILENAME *xfile)
{
    void *file;

#if X_PLATFORM == X_MACINTOSH_9
    FSSpec  spec;
    char    tmp[256];
    char    *src, *dst;
    OSErr   err;
    XBOOL   firstChar;

    firstChar = TRUE;
    tmp[0] = 0;
    src = (char *)path;
    dst = tmp;
    while (*src)
    {
        if ((*src == '/') || (*src == '\\'))
        {
            *dst++ = ':';
            src++;
            if (firstChar)
            {
                *dst++ = ':';   // first char, need an extra :
                firstChar = FALSE;
            }           
        }
        else
        {
            if ((src[0] == '.') && (src[1] == '.') && ((src[2] == '/') || (src[2] == '\\')))
            {
                if ( firstChar || (dst[-1] != ':') && (dst[0] != ':') ) // already going up directories?
                {   // no
                    *dst++ = ':';
                }
                *dst++ = ':';   // yes, only need to add single colon
                src += 3;
                firstChar = FALSE;
            }
            else
            {
                *dst++ = *src++;
                firstChar = FALSE;
            }
        }
    }
    *dst = 0;
    err = FSMakeFSSpec(0, 0, (unsigned char *)XCtoPstr(tmp), &spec);
    file = (void *)&spec;
#else
    file = path;
#endif

    XConvertNativeFileToXFILENAME(file, xfile); 
}

// given a native file spec (FSSpec for MacOS, and 'C' string for WinOS, fill in a XFILENAME
void XConvertNativeFileToXFILENAME(void *file, XFILENAME *xfile)
{
    if (xfile)
    {
        XSetMemory(xfile, (long)sizeof(XFILENAME), 0);
    }
    if (file)
    {
        void    *dest;

        dest = &xfile->theFile;
        BAE_CopyFileNameNative(file, dest);
    }
}

// Read a file into memory and return an allocated pointer.
// 0 is ok, -1 failed to open, -2 failed to read, -3 failed memory
// if 0, then *pData is valid
XERR XGetFileAsData(XFILENAME *pResourceName, XPTR *ppData, long *pSize)
{
    XFILE   ref;
    long    size;
    XERR    error;
    XPTR    pData;

    error = -3; // failed memory
    pData = NULL;
    if (pResourceName && pSize && ppData)
    {
        *pSize = 0;
        *ppData = NULL;
        ref = XFileOpenForRead(pResourceName);
        if (ref)
        {
            size = XFileGetLength(ref);
            pData = XNewPtr(size);
            if (pData)
            {
                if (XFileRead(ref, pData, size))
                {
                    error = -2; // failed to read
                    XDisposePtr(pData);
                    pData = NULL;
                }
                else
                {
                    error = 0;  // ok
                }
            }
            else
            {
                error = -3; // failed memory
            }
            XFileClose(ref);
            *pSize = size;
        }
        else
        {
            error = -1; // failed open
        }
        *ppData = pData;
    }
    return error;
}

#if USE_CREATION_API == TRUE
// Create a temporary file name and fill an XFILENAME structure. Return -1 for failure, or 0 for sucess.
XERR XGetTempXFILENAME(XFILENAME* xfilename)
{
#if X_PLATFORM == X_IOS
    return -1;
#endif
#if X_PLATFORM == X_ANDROID
	return -1;
#endif
#if X_PLATFORM == X_MACINTOSH
    char sfn[30] = "/tmp/bae_tmp.XXXXXX";
    char* result = NULL;

    result = mktemp(sfn);
    XConvertNativeFileToXFILENAME(result, xfilename);
#endif

#if X_PLATFORM == X_MACINTOSH_9
    short           vRefNum;
    long            theDirID;
    OSErr           err;
    FSSpec          fileSpec;
    char            name[64], name2[32];
    int             length;

    // point to the prefs folder
    err = FindFolder(kOnSystemDisk, kTrashFolderType, kCreateFolder, &vRefNum, &theDirID);
    if (err == noErr) 
    {
        XStrCpy(name, "BAE_TEMP_");
        length = XStrLen(name);

        NumToString(XMicroseconds() & 0x3FFF, (unsigned char *)name2);  // create a text plot based upon time
        XStrCpy(&name[length], (char *)XPtoCstr(name2));                // concatinate it to name

        // Create FSSpec structure from file name
        XSetMemory(&fileSpec, (long)sizeof(FSSpec), 0);
        err = FSMakeFSSpec(vRefNum, theDirID, (unsigned char *)XCtoPstr(name), &fileSpec);
        if ((err == noErr) || (err == fnfErr))
        {
            XConvertNativeFileToXFILENAME(&fileSpec, xfilename);
            return 0;   // success
        }
    }
#endif
#if (X_PLATFORM == X_WIN95) || (X_PLATFORM == X_WIN_HARDWARE)
    TCHAR           directory[MAX_PATH];
    TCHAR           path[MAX_PATH];
    unsigned int    length;

    length = GetTempPath(MAX_PATH, directory);
    if ((length != 0) && (length < MAX_PATH))
    {
        if (GetTempFileName(directory, "BAE", 0, path) != 0)
        {
            XConvertNativeFileToXFILENAME(path, xfilename);
            return 0;   // success
        }
    }
#endif
    return -1;  // failure
}
#endif

// Given an open file, return TRUE if this is a valid resource file.
XBOOL XFileIsValidResource(XFILE file)
{
    XBOOL               valid;
    XFILERESOURCEMAP    map;

    valid = FALSE;
    if (file)
    {
        // validate resource file
        XFileSetPosition(file, 0L);     // at start
        if (XFileRead(file, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
        {
            if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
            {
                if (XGetLong(&map.version) == 0)
                {
                    valid = TRUE;
                }
            }
        }
    }
    return valid;
}

// Given a filename, return TRUE if this is a valid resource file.
XBOOL XFileIsValidResourceFromName(XFILENAME *file)
{
    XBOOL               valid;
    XFILE               reference;

    valid = FALSE;
    if (file)
    {
        reference = XFileOpenForRead(file);
        valid = XFileIsValidResource(reference);
        XFileClose(reference);
    }
    return valid;
}


XFILE XFileOpenResourceFromMemory(XPTR pResource, unsigned long resourceLength, XBOOL allowCopy)
{
    XFILENAME           *pReference;
    XFILERESOURCEMAP    map;
    short int           err;

    allowCopy;
    err = 0;
    pReference = (XFILENAME *)XNewPtr((long)sizeof(XFILENAME));
    if (pReference)
    {
        pReference->pResourceData = pResource;
        pReference->resMemLength = resourceLength;
        pReference->resMemOffset = 0;
        pReference->resourceFile = TRUE;

        pReference->allowMemCopy = TRUE;

        // NOTE: There's a bug when allowMemCopy is set to FALSE. Needs to be looked into.
        //pReference->allowMemCopy = allowCopy;

        pReference->fileValidID = XPI_BLOCK_3_ID;
        if (pReference)
        {
            if (PV_AddResourceFileToOpenFiles((XFILE)pReference))
            {   // can't open any more files
                err = 1;
            }
            else
            {
                pReference->pCache = NULL;
                // since we are pointer based, we don't care about caching.

                // validate resource file
                XFileSetPosition((XFILE)pReference, 0L);        // at start
                if (XFileRead((XFILE)pReference, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
                {
                    if (XGetLong(&map.mapID) != XFILERESOURCE_ID)
                    {
                        err = 2;
                    }
                }
                else
                {
                    err = 3;
                }
            }
        }
        if (err)
        {
            PV_RemoveResourceFileFromOpenFiles((XFILE)pReference);  // make sure we remove it from the list
            XDisposePtr(pReference);
            pReference = NULL;
        }
    }
    return (XFILE)pReference;
}

XFILE XFileOpenResource(XFILENAME *file, XBOOL readOnly)
{
    XFILENAME           *pReference;
    XFILERESOURCEMAP    map;
    XERR                err;

    err = 0;
    pReference = (XFILENAME *)XNewPtr((long)sizeof(XFILENAME));
    if (pReference)
    {
        *pReference = *file;
        pReference->resourceFile = TRUE;
        pReference->fileValidID = XPI_BLOCK_3_ID;
        pReference->pResourceData = NULL;
        pReference->allowMemCopy = TRUE;
        pReference->readOnly = readOnly;

        if (readOnly)
        {
            pReference->fileReference = BAE_FileOpenForRead((void *)&pReference->theFile);
            if (pReference->fileReference == -1)
            {
                XDisposePtr(pReference);
                pReference = NULL;
            }
        }
        else
        {
            pReference->fileReference = BAE_FileOpenForReadWrite((void *)&pReference->theFile);
            if (pReference->fileReference == -1)
            {
                // must not be there, so create it and prepare it as a resource file
                BAE_FileCreate((void *)&pReference->theFile);
                pReference->fileReference = BAE_FileOpenForReadWrite((void *)&pReference->theFile);
                if (pReference->fileReference == -1)
                {
                    XDisposePtr(pReference);
                    pReference = NULL;
                }
                else
                {
                    // prepare it as a resource file
                    XFileSetPosition((XFILE)pReference, 0L);        // at start
                    XPutLong(&map.mapID, XFILERESOURCE_ID);
                    XPutLong(&map.version, 1);
                    XPutLong(&map.totalResources, 0);
                    XFileWrite((XFILE)pReference, &map, (long)sizeof(XFILERESOURCEMAP));
                }
            }
        }

        if (pReference)
        {
            // success
            if (PV_AddResourceFileToOpenFiles((XFILE)pReference))
            {   // can't open any more files
                BAE_FileClose(pReference->fileReference);       // jsc 3/29/00
                XDisposePtr(pReference);
                pReference = NULL;
            }
            else
            {
                pReference->pCache = NULL;
#if USE_FILE_CACHE != 0
//              if (readOnly)
                {
                    // Try to open XFILECACHE_ID cache block.
                    pReference->pCache = (XFILERESOURCECACHE *)XGetFileResource((XFILE)pReference,
                                                                XFILECACHE_ID, 0, NULL, NULL);
                    if (pReference->pCache != NULL)
                    {
                        XSwapLongsInAccessCache(pReference->pCache, TRUE);
                    }
                    else
                    {
                        pReference->pCache = XCreateAccessCache((XFILE)pReference);
                    }
                }
#endif
                // validate resource file
                XFileSetPosition((XFILE)pReference, 0L);        // at start
                if (XFileRead((XFILE)pReference, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
                {
                    if (XGetLong(&map.mapID) != XFILERESOURCE_ID)
                    {
                        err = -1;
                    }
                }
                else
                {
                    err = -2;
                }
            }
            if (err)
            {
                XDisposePtr(pReference->pCache);
                PV_RemoveResourceFileFromOpenFiles((XFILE)pReference);  // make sure we remove it from the list
                BAE_FileClose(pReference->fileReference);               // jsc 3/29/00
                XDisposePtr(pReference);
                pReference = NULL;
            }
        }
    }
    return (XFILE)pReference;
}

XFILE XFileOpenForReadFromMemory(XPTR pMemoryBlock, unsigned long memoryBlockSize)
{
    XFILENAME   *pReference;

    pReference = (XFILENAME *)XNewPtr((long)sizeof(XFILENAME));
    if (pReference)
    {
        pReference->pResourceData = pMemoryBlock;
        pReference->resMemLength = memoryBlockSize;
        pReference->resMemOffset = 0;
        pReference->resourceFile = FALSE;
        pReference->allowMemCopy = TRUE;
        pReference->fileValidID = XPI_BLOCK_3_ID;
        pReference->pCache = NULL;
        pReference->fileReference = 0;
    }
    return (XFILE)pReference;
}

XFILE XFileOpenForRead(XFILENAME *file)
{
    XFILENAME   *pReference;

    pReference = (XFILENAME *)XNewPtr((long)sizeof(XFILENAME));
    if (pReference)
    {
        *pReference = *file;
        pReference->resourceFile = FALSE;
        pReference->fileValidID = XPI_BLOCK_3_ID;
        pReference->pResourceData = NULL;
        pReference->allowMemCopy = TRUE;
        pReference->pCache = NULL;

        pReference->fileReference = BAE_FileOpenForRead((void *)&pReference->theFile);
        if (pReference->fileReference == -1)
        {
            XDisposePtr(pReference);
            pReference = NULL;
        }
    }
    return (XFILE)pReference;
}

#if USE_CREATION_API == TRUE
XFILE XFileOpenForWrite(XFILENAME *file, XBOOL create)
{
    XFILENAME   *pReference;

    pReference = (XFILENAME *)XNewPtr((long)sizeof(XFILENAME));
    if (pReference)
    {
        *pReference = *file;
        pReference->resourceFile = FALSE;
        pReference->fileValidID = XPI_BLOCK_3_ID;
        pReference->pResourceData = NULL;
        pReference->allowMemCopy = TRUE;
        pReference->pCache = NULL;

        if (create)
        {
            pReference->fileReference = BAE_FileCreate((void *)&pReference->theFile);
        }
        pReference->fileReference = BAE_FileOpenForReadWrite((void *)&pReference->theFile);
        if (pReference->fileReference == -1)
        {
            XDisposePtr(pReference);
            pReference = NULL;
        }
    }
    return (XFILE)pReference;
}
#endif

// delete file. 0 is ok, -1 for failure
XERR XFileDelete(XFILENAME *file)
{
    void    *dest;

    dest = &file->theFile;
    return BAE_FileDelete(dest);
}

void XFileClose(XFILE fileRef)
{
    XFILENAME   *pReference;

    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        XFileFreeResourceCache(fileRef);
        pReference->fileValidID = (long)XPI_DEAD_ID;
        if (pReference->pResourceData)
        {
            pReference->pResourceData = NULL;   // clear memory file access
        }
        else
        {
            BAE_FileClose(pReference->fileReference);
        }
        PV_RemoveResourceFileFromOpenFiles(fileRef);
        XDisposePtr(pReference);
    }
}

XERR XFileRead(XFILE fileRef, XPTR buffer, long bufferLength)
{
    XFILENAME   *pReference;
    long        newLength;
    long        err;

    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData)
        {
            err = 0;
            newLength = pReference->resMemOffset + bufferLength;
            if (newLength > pReference->resMemLength)
            {
                bufferLength -= (newLength - pReference->resMemLength);
                err = -1;   // eof
            }
            XBlockMove((char *)pReference->pResourceData + pReference->resMemOffset, 
                        buffer, bufferLength);
            pReference->resMemOffset += bufferLength;
            return err;
        }
        else
        {
            return (BAE_ReadFile(pReference->fileReference, buffer, bufferLength) == bufferLength) ? 0 : -1;
        }
    }
    return -1;
}

XERR XFileWrite(XFILE fileRef, XPTRC buffer, long bufferLength)
{
    XFILENAME   *pReference;

    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData)
        {
            return -1;  // can't write a memory resource
        }
        else
        {
            return (BAE_WriteFile(pReference->fileReference, (XPTR)buffer, bufferLength) == bufferLength) ? 0 : -1;
        }
    }
    return -1;
}

XERR XFileSetPosition(XFILE fileRef, long filePosition)
{
    XFILENAME   *pReference;
    XERR        err;

    err = -1;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData)
        {
            if ( (filePosition >= 0) && (filePosition < pReference->resMemLength) )
            {
                pReference->resMemOffset = filePosition;
                err = 0;
            }
        }
        else
        {
            err = BAE_SetFilePosition(pReference->fileReference, filePosition);
        }
    }
    return err;
}

XERR XFileSetPositionRelative(XFILE fileRef, long relativeOffset)
{
    long        pos;
    XERR        err;

    err = -1;
    pos = XFileGetPosition(fileRef);
    if (pos != -1)
    {
        err = XFileSetPosition(fileRef, pos + relativeOffset);
    }
    return err;
}

static XBYTE * PV_GetFilePositionFromMemoryResource(XFILE fileRef)
{
    XFILENAME   *pReference;
    XBYTE       *pos;

    pos = NULL;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData)
        {
            pos = ((XBYTE *)pReference->pResourceData) + pReference->resMemOffset;
        }
    }
    return pos;
}

long XFileGetPosition(XFILE fileRef)
{
    XFILENAME   *pReference;
    long        pos;

    pos = -1;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData)
        {
            pos = pReference->resMemOffset;
        }
        else
        {
            pos = BAE_GetFilePosition(pReference->fileReference);
        }
    }
    return pos;
}

#if USE_CREATION_API == TRUE
XERR XFileSetLength(XFILE fileRef, unsigned long newSize)
{
    XFILENAME   *pReference;
    long        error;

    error = 0;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData == NULL)
        {   // not a memory file
            error = BAE_SetFileLength(pReference->fileReference, newSize);
        }
    }
    return (error) ? -1 : 0;
}
#endif

long XFileGetLength(XFILE fileRef)
{
    XFILENAME   *pReference;
    long        pos;

    pos = -1;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData)
        {
            pos = pReference->resMemLength;
        }
        else
        {
            pos = BAE_GetFileLength(pReference->fileReference);
        }
    }
    return pos;
}

// search the cache for a particular item
static XFILE_CACHED_ITEM * PV_XGetCacheEntry(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID)
{
    XFILENAME           *pReference;
    long                count, total;
    XFILERESOURCECACHE  *pCache;
    XFILE_CACHED_ITEM   *pItem;

    pItem = NULL;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        pCache = pReference->pCache;
        if (pCache)
        {
            total = pCache->totalResources;
            for (count = 0; count < total; count++)
            {
                if (pCache->cached[count].resourceType == resourceType)
                {
                    if (pCache->cached[count].resourceID == resourceID)
                    {
                        pItem = &pCache->cached[count];
                        break;
                    }
                }
            }
        }
    }
    return pItem;
}

// search the cache for a particular item with a name
static XFILE_CACHED_ITEM * PV_XGetNamedCacheEntry(XFILE fileRef, XResourceType resourceType, void *cName)
{
    XFILENAME           *pReference;
    long                count, total;
    XFILERESOURCECACHE  *pCache;
    XFILE_CACHED_ITEM   *pItem;
    long                err=0;
    char                tempPascalName[256];
    long                savePos;
    XFILERESOURCEMAP    map;
    long                data, next;

    pItem = NULL;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
        {
            //Yes sir!  We have a memory file!
            //Gotta search the whole bleedin' thing!

            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type

                                if ((XResourceType)XGetLong(&data) == resourceType)
                                {
                                    pReference->memoryCacheEntry.resourceType = (XResourceType)XGetLong(&data);
                                        
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID

                                    pReference->memoryCacheEntry.resourceID = (XLongResourceID)XGetLong(&data);

                                    err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                                    if (tempPascalName[0])
                                    {
                                        err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);

                                        XPtoCstr(tempPascalName);
                                        if (!XStrCmp((char *)tempPascalName, (char *)cName))
                                        {
                                            pItem = &pReference->memoryCacheEntry;
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        
        }
        else
        {
            savePos = XFileGetPosition(fileRef);
            pCache = pReference->pCache;
            if (pCache)
            {
                total = pCache->totalResources;
                for (count = 0; count < total; count++)
                {
                    if (pCache->cached[count].resourceType == resourceType)
                    {
                        XFileSetPosition(fileRef, pCache->cached[count].fileOffsetName);
                        err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                        if (tempPascalName[0])
                        {
                            err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                            if (XStrCmp((char *)cName, (char *)XPtoCstr(tempPascalName)) == 0)
                            {
                                pItem = &pCache->cached[count];
                                break;
                            }
                        }
                    }
                }
            }
            XFileSetPosition(fileRef, savePos);
        }
    }
    return pItem;
}


// bvk 6/12/97
//  This scans the resource file for the header data and returns the name associated
//      with a type and id.  Taken pretty much verbatim from XGetFileResource (just
//      without the data loading).
// pResourceName is a pascal string
char *  XGetResourceNameOnly(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID, char *pResourceName)
{
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    char                tempPascalName[256];
    XFILE_CACHED_ITEM   *pCacheItem;

    BAE_ASSERT(pResourceName);  //MOE: why would anyone call this with NULL?
    if (pResourceName)
    {
        *pResourceName = '\0';
    }
    
    err = 0;
    tempPascalName[0] = 0;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        // first do we have a cache?
        if (pReference->pCache)
        {
            // second check to see if its in the cache
            pCacheItem = PV_XGetCacheEntry(fileRef, resourceType, resourceID);
            if (pCacheItem)
            {
                // get name
                if (pResourceName)
                {
                    XFileSetPosition(fileRef, pCacheItem->fileOffsetName);
                    err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                    if (tempPascalName[0])
                    {
                        err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                        XBlockMove(tempPascalName, pResourceName, (long)tempPascalName[0] + 1);
                    }
                }
            }
            else
            {
                err = -1;   // can't find it
            }
        }
        else
        {
            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                if ((XResourceType)XGetLong(&data) == resourceType)
                                {
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                    if ((XLongResourceID)XGetLong(&data) == resourceID)
                                    {
                                        err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                                        if (tempPascalName[0])
                                        {
                                            err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                                            if (pResourceName)
                                            {
                                                XBlockMove(tempPascalName, pResourceName, (long)tempPascalName[0] + 1 );
                                                break;
                                            }
                                        }
                                        err = XFileRead(fileRef, &data, (long)sizeof(long));        // get length
                                        data = XGetLong(&data);     // get resource size
                                    }
                                }
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        }
    }
#if X_PLATFORM == X_MACINTOSH_9
    else
    {   // use native resource manager
        short int       theID;
        unsigned long   theType;
        Handle          theData;
        short int       currentResourceFile;

        currentResourceFile = CurResFile();
        UseResFile((short int)fileRef);

        SetResLoad(FALSE);
        theData = Get1Resource(resourceType, (short int)resourceID);
        SetResLoad(TRUE);
        err = -1;
        if (theData)
        {
            GetResInfo(theData, &theID, &theType, (unsigned char *)pResourceName);
            DetachResource(theData);
            DisposeHandle(theData);
            err = 0;
        }
        UseResFile(currentResourceFile);
    }
#endif
    return (err ? NULL : pResourceName);
}

XERR XReadPartialFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID,
                                char *pResourceName,
                                XPTR *pReturnedBuffer, long bytesToReadAndAllocate)
{
    XFILENAME           *pReference;
    XERR                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    XPTR                pData;
    char                tempPascalName[256];
    XFILE_CACHED_ITEM   *pCacheItem;

    err = 0;
    pData = NULL;
    tempPascalName[0] = 0;
    pReference = (XFILENAME *)fileRef;

    if (PV_XFileValid(fileRef) && pReturnedBuffer && bytesToReadAndAllocate)
    {
        // first do we have a cache?
        if (pReference->pCache)
        {
            // second check to see if its in the cache
            pCacheItem = PV_XGetCacheEntry(fileRef, resourceType, resourceID);
            if (pCacheItem)
            {
                // get name
                if (pResourceName)
                {
                    XFileSetPosition(fileRef, pCacheItem->fileOffsetName);
                    err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                    if (tempPascalName[0])
                    {
                        err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                        if (pResourceName)
                        {
                            XBlockMove(tempPascalName, pResourceName, (long)tempPascalName[0] + 1);
                        }
                    }
                }
                // get data
                XFileSetPosition(fileRef, pCacheItem->fileOffsetData);

                // is data memory based?
                if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
                {   // don't bother coping data again, since its already in memory
                    // just return the pointer. 
                    pData = PV_GetFilePositionFromMemoryResource(fileRef);
                    if (pData == NULL)
                    {
                        err = -2;
                        BAE_PRINTF("Out of memory; can't allocate resource\n");
                    }
                }
                else
                {
                    pData = XNewPtr(bytesToReadAndAllocate);
                    if (pData)
                    {
                        err = XFileRead(fileRef, pData, bytesToReadAndAllocate);
                    }
                    else
                    {
                        err = -2;
                        BAE_PRINTF("Out of memory; can't allocate resource\n");
                    }
                }
            }
            else
            {
                err = -1;   // can't find it
            }
        }
        else
        {
            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                if ((XResourceType)XGetLong(&data) == resourceType)
                                {
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                    if ((XLongResourceID)XGetLong(&data) == resourceID)
                                    {
                                        err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                                        if (tempPascalName[0])
                                        {
                                            err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                                            if (pResourceName)
                                            {
                                                XBlockMove(tempPascalName, pResourceName, (long)tempPascalName[0] + 1);
                                            }
                                        }
                                        err = XFileRead(fileRef, &data, (long)sizeof(long));        // get length
                                        data = XGetLong(&data);     // get resource size

                                        // get data
                                        // is data memory based?
                                        if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
                                        {   // don't bother coping data again, since its already in memory
                                            pData = PV_GetFilePositionFromMemoryResource(fileRef);
                                            if (pData)
                                            {
                                                err = 0;
                                                break;
                                            }
                                            else
                                            {
                                                err = -2;
                                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                            }
                                        }
                                        else
                                        {
                                            pData = XNewPtr(bytesToReadAndAllocate);
                                            if (pData)
                                            {
                                                err = XFileRead(fileRef, pData, bytesToReadAndAllocate);
                                                break;
                                            }
                                            else
                                            {
                                                err = -2;
                                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        err = -1;
    }
    return err;
}


// Read from a resource a particular type and ID. Return the size and name and data block.
// fileRef is an open resource file
// resourceType is a vaild resource type
// resourceID is an ID
// pResourceName is a pascal string. pResourceName can be NULL.
// pReturnedResourceSize be filled with the size of the resource. pReturnedResourceSize can be NULL.
XPTR XGetFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID, void *pResourceName, long *pReturnedResourceSize)
{
    XFILENAME           *pReference;
    XERR                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    XPTR                pData;
    char                tempPascalName[256];
    XFILE_CACHED_ITEM   *pCacheItem;

    err = 0;
    pData = NULL;
    if (pReturnedResourceSize)
    {
        *pReturnedResourceSize = 0;
    }
#if DEBUG_PRINT_RESOURCE
    XPutLong(tempPascalName, resourceType);
    tempPascalName[4] = 0;
    BAE_PRINTF("GetResource %s %ld is ", tempPascalName, resourceID);
#endif
    tempPascalName[0] = 0;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        // first do we have a cache?
        if (pReference->pCache)
        {
            // second check to see if its in the cache
            pCacheItem = PV_XGetCacheEntry(fileRef, resourceType, resourceID);
            if (pCacheItem)
            {
                // get name
                if (pResourceName)
                {
                    XFileSetPosition(fileRef, pCacheItem->fileOffsetName);
                    err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                    if (tempPascalName[0])
                    {
                        err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                        if (pResourceName)
                        {
                            XBlockMove(tempPascalName, pResourceName, (long)tempPascalName[0] + 1);
                        }
                    }
                }
                // get data
                XFileSetPosition(fileRef, pCacheItem->fileOffsetData);

                // is data memory based?
                if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
                {   // don't bother coping data again, since its already in memory
                    pData = PV_GetFilePositionFromMemoryResource(fileRef);
                    if (pData)
                    {
                        if (pReturnedResourceSize)
                        {
                            *pReturnedResourceSize = pCacheItem->resourceLength;
                        }
                    }
                    else
                    {
                        err = -2;
                        BAE_PRINTF("Out of memory; can't allocate resource\n");
                    }
                }
                else
                {
                    pData = XNewPtr(pCacheItem->resourceLength);
                    if (pData)
                    {
                        err = XFileRead(fileRef, pData, pCacheItem->resourceLength);
                        if (pReturnedResourceSize)
                        {
                            *pReturnedResourceSize = pCacheItem->resourceLength;
                        }
                    }
                    else
                    {
                        err = -2;
                        BAE_PRINTF("Out of memory; can't allocate resource\n");
                    }
                }
            }
            else
            {
                err = -1;   // can't find it
                BAE_PRINTF("Can't find it\n");
            }
        }
        else
        {
            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                if ((XResourceType)XGetLong(&data) == resourceType)
                                {
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                    if ((XLongResourceID)XGetLong(&data) == resourceID)
                                    {
                                        err = XFileRead(fileRef, &tempPascalName[0], 1L);       // get name
                                        if (tempPascalName[0])
                                        {
                                            err = XFileRead(fileRef, &tempPascalName[1], (long)tempPascalName[0]);
                                            if (pResourceName)
                                            {
                                                XBlockMove(tempPascalName, pResourceName, (long)tempPascalName[0] + 1);
                                            }
                                        }
                                        err = XFileRead(fileRef, &data, (long)sizeof(long));        // get length
                                        data = XGetLong(&data);     // get resource size

                                        // get data
                                        // is data memory based?
                                        if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
                                        {   // don't bother coping data again, since its already in memory
                                            pData = PV_GetFilePositionFromMemoryResource(fileRef);
                                            if (pData)
                                            {
                                                if (pReturnedResourceSize)
                                                {
                                                    *pReturnedResourceSize = data;
                                                }
                                                err = 0;
                                                break;
                                            }
                                            else
                                            {
                                                err = -2;
                                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                            }
                                        }
                                        else
                                        {
                                            pData = XNewPtr(data);
                                            if (pData)
                                            {
                                                err = XFileRead(fileRef, pData, data);
                                                if (pReturnedResourceSize)
                                                {
                                                    *pReturnedResourceSize = data;
                                                }
                                                break;
                                            }
                                            else
                                            {
                                                err = -2;
                                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        }
    }
#if DEBUG_PRINT_RESOURCE
    BAE_PRINTF((pData) ? "OK\n" : "BAD\n");
#endif
    return pData;
}



/******************************************************************************
*******************************************************************************
*******************************************************************************
**
**  Functions to manage bank tokens
**
*******************************************************************************
*******************************************************************************
******************************************************************************/


XBOOL AreBankTokensIdentical(XBankToken tok1, XBankToken tok2)
{
    if (tok1.fileLen == tok2.fileLen && tok1.xFile == tok2.xFile)
    {
        return TRUE;
    }
    return FALSE;
}

XBankToken CreateBankToken(void)
{
    XBankToken          retVal;

    retVal.xFile = (XTOKEN) XFileGetCurrentResourceFile();
    retVal.fileLen = (XTOKEN) XFileGetLength(retVal.xFile);

    return retVal;
}

XBankToken CreateBankTokenFromInputs(XTOKEN tok1, XTOKEN tok2)
{
    XBankToken          retVal;

    retVal.fileLen = tok1;
    retVal.xFile = tok2;

    return retVal;
}



//bvk New!
//  Adds another cache entry to end of cache.

#if USE_CREATION_API == TRUE
static XBOOL PV_AddToAccessCache(XFILE fileRef, XFILE_CACHED_ITEM *cacheItemPtr )
{
    XFILENAME           *pReference;
    XFILERESOURCECACHE  *pCache,*newCache;
    long int            resCount;
    XFILE_CACHED_ITEM   *pItem;

    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        pCache = pReference->pCache;
        if (pCache)
        {
            resCount = pCache->totalResources + 1;
            newCache = (XFILERESOURCECACHE *)XNewPtr((long)sizeof(XFILERESOURCECACHE) + 
                                                    ((long)sizeof(XFILE_CACHED_ITEM) * resCount));
            if (newCache)
            {
                XBlockMove(pCache, newCache, (long)sizeof(XFILERESOURCECACHE) + 
                                                ((long)sizeof(XFILE_CACHED_ITEM) * (resCount - 1)));

                XDisposePtr(pCache);
                pReference->pCache = newCache;
                newCache->totalResources = resCount;
                pItem = &newCache->cached[resCount - 1];
                // copy cache item
                *pItem = *cacheItemPtr;
                return TRUE;
            }
        }
    }
    return FALSE;
}
#endif  // USE_CREATION_API == TRUE

// This will scan through the resource file and return an pointer to an array of
XFILERESOURCECACHE * XCreateAccessCache(XFILE fileRef)
{
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    XFILERESOURCECACHE  *pCache;
    char                pPName[256];

    err = 0;
    pCache = NULL;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        XFileSetPosition(fileRef, 0L);      // at start
        if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
        {
            if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
            {
                next = sizeof(XFILERESOURCEMAP);
                total = XGetLong(&map.totalResources);
                pCache = (XFILERESOURCECACHE *)XNewPtr((long)sizeof(XFILERESOURCECACHE) + 
                                                        ((long)sizeof(XFILE_CACHED_ITEM) * total));
                if (pCache)
                {
                    pCache->totalResources = total;
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));                // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                err = XFileRead(fileRef, &data, (long)sizeof(long));            // get type
                                pCache->cached[count].resourceType = (XResourceType)XGetLong(&data);

                                err = XFileRead(fileRef, &data, (long)sizeof(long));            // get ID
                                pCache->cached[count].resourceID = (XLongResourceID)XGetLong(&data);

                                pCache->cached[count].fileOffsetName = XFileGetPosition(fileRef);   // get name
                                err = XFileRead(fileRef, &pPName[0], 1L);               
                                if (pPName[0])
                                {
                                    err = XFileRead(fileRef, &pPName[1], (long)pPName[0]);
                                }

                                err = XFileRead(fileRef, &data, (long)sizeof(long));            // get length
                                pCache->cached[count].resourceLength = XGetLong(&data);
                                pCache->cached[count].fileOffsetData = XFileGetPosition(fileRef);   // save data offset
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        }
    }
    if (err)
    {
        XDisposePtr((XPTR)pCache);
        pCache = NULL;
    }
    return pCache;
}

void XSwapLongsInAccessCache(XFILERESOURCECACHE *pCache, XBOOL inFileOrder)
{
#if X_WORD_ORDER != FALSE
long                count;
XFILE_CACHED_ITEM   *item;

    count = pCache->totalResources;
    pCache->totalResources = XGetLong(&pCache->totalResources);
    if (inFileOrder)
    {
        count = pCache->totalResources;
    }

    item = pCache->cached;
    while (--count >= 0)
    {
        item->resourceType = (XResourceType)XGetLong(&item->resourceType);
        item->resourceID = (XLongResourceID)XGetLong(&item->resourceID);
        item->resourceLength = XGetLong(&item->resourceLength);
        item->fileOffsetName = XGetLong(&item->fileOffsetName);
        item->fileOffsetData = XGetLong(&item->fileOffsetData);
        item++;
    }
#else
pCache;
inFileOrder;
#endif
}

// Create a resource cache for a file
XERR XFileCreateResourceCache(XFILE fileRef)
{
    XERR        err;
    XFILENAME   *pReference;

    err = 0;
    pReference = NULL;
#if USE_FILE_CACHE != 0
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pCache)
        {
            XFileFreeResourceCache(fileRef);
        }
        // Try to open XFILECACHE_ID cache block.
        pReference->pCache = (XFILERESOURCECACHE *)XGetFileResource((XFILE)pReference,
                                                    XFILECACHE_ID, 0, NULL, NULL);
        if (pReference->pCache != NULL)
        {
            XSwapLongsInAccessCache(pReference->pCache, TRUE);
        }
        else
        {
            pReference->pCache = XCreateAccessCache((XFILE)pReference);
        }
    }
#endif
    return err;
}

// Free cache of a resource file
void XFileFreeResourceCache(XFILE fileRef)
{
    XFILENAME   *pReference;

    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pCache)
        {
            XDisposePtr((XPTR)pReference->pCache);
            pReference->pCache = NULL;
        }
    }
}

static XBOOL PV_CheckForTypes(XResourceType *pTypes, long total, XResourceType typeCheck)
{
    long    count;
    XBOOL   found;

    found = FALSE;
    for (count = 0; count < total; count++)
    {
        if (pTypes[count] == typeCheck)
        {
            found = TRUE;
            break;
        }
    }
    return found;
}

#define MAX_XFILE_SCAN_TYPES        5120L

// Return the number of resource types included in file

//  bvk:  why doesn't this use the cache?

long XCountTypes(XFILE fileRef)
{
    long                typeCount;
    XResourceType       lastResourceType;
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    long                *pTypes;

    err = 0;
    typeCount = 0;
    lastResourceType = 0;
#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        // use native resource manager
        return Count1Types() - 1;
    }
#endif
    if (PV_IsAnyOpenResourceFiles())
    {
        if (fileRef == (XFILE)NULL)
        {   // then use first open file
            fileRef = g_openResourceFiles[0];
        }
        pTypes = (long *)XNewPtr((sizeof(long) * MAX_XFILE_SCAN_TYPES));
        if (pTypes)
        {
            pReference = (XFILENAME *)fileRef;
            if (PV_XFileValid(fileRef))
            {
                XFileSetPosition(fileRef, 0L);      // at start
                if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
                {
                    if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                    {
                        next = sizeof(XFILERESOURCEMAP);
                        total = XGetLong(&map.totalResources);
                        for (count = 0; (count < total) && (err == 0); count++)
                        {
                            err = XFileSetPosition(fileRef, next);      // at start
                            if (err == 0)
                            {
                                err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                                next = XGetLong(&next);
                                if (next != -1L)
                                {   
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                    lastResourceType = (XResourceType)XGetLong(&data);
                                    if (typeCount < MAX_XFILE_SCAN_TYPES)
                                    {
                                        if (PV_CheckForTypes(pTypes, typeCount, lastResourceType) == FALSE)
                                        {
                                            pTypes[typeCount] = lastResourceType;
                                            typeCount++;
                                        }
                                    }
                                    else
                                    {
                                        err = -5;
                                        break;
                                    }
                                }
                                else
                                {
                                    err = -4;
                                    BAE_PRINTF("Next offset is bad\n");
                                    break;
                                }
                            }
                            else
                            {
                                err = -3;
                                BAE_PRINTF("Can't set next position\n");
                                break;
                            }
                        }
                    }
                }
            }
            XDisposePtr((XPTR)pTypes);
        }
    }
    return typeCount;
}

// Return the type from the file based upon an index of 0 to XCountTypes
//  bvk:  why doesn't this use the cache?
XResourceType XGetIndexedType(XFILE fileRef, long resourceIndex)
{
    long                typeCount;
    XResourceType       lastResourceType;
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    long                *pTypes;

#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        // use native resource manager
        ResType type;

        Get1IndType(&type, resourceIndex + 1);
        return (XResourceType)type;
    }
#endif
    err = 0;
    typeCount = 0;
    lastResourceType = 0;
    if (PV_IsAnyOpenResourceFiles())
    {
        if (fileRef == (XFILE)NULL)
        {   // then use first open file
            fileRef = g_openResourceFiles[0];
        }
        pTypes = (long *)XNewPtr((sizeof(long) * MAX_XFILE_SCAN_TYPES));
        if (pTypes)
        {
            pReference = (XFILENAME *)fileRef;
            if (PV_XFileValid(fileRef))
            {
                XFileSetPosition(fileRef, 0L);      // at start
                if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
                {
                    if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                    {
                        next = sizeof(XFILERESOURCEMAP);
                        total = XGetLong(&map.totalResources);
                        for (count = 0; (count < total) && (err == 0); count++)
                        {
                            err = XFileSetPosition(fileRef, next);      // at start
                            if (err == 0)
                            {
                                err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                                next = XGetLong(&next);
                                if (next != -1L)
                                {   
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                    lastResourceType = XGetLong(&data);
                                    if (typeCount < MAX_XFILE_SCAN_TYPES)
                                    {
                                        if (PV_CheckForTypes(pTypes, typeCount, lastResourceType) == FALSE)
                                        {
                                            pTypes[typeCount] = lastResourceType;
                                            if (typeCount == resourceIndex)
                                            {
                                                break;
                                            }
                                            else
                                            {
                                                typeCount++;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        err = -5;
                                        break;
                                    }
                                }
                                else
                                {
                                    err = -4;
                                    BAE_PRINTF("Next offset is bad\n");
                                    break;
                                }
                            }
                            else
                            {
                                err = -3;
                                BAE_PRINTF("Can't set next position\n");
                                break;
                            }
                        }
                    }
                }
            }
            XDisposePtr((XPTR)pTypes);
        }
    }
    return lastResourceType;
}

// Returns TRUE if the file is read only

#if USE_CREATION_API == TRUE
static XBOOL PV_IsXFileLocked(XFILE fileRef)
{
    XFILENAME           *pReference;

    if (PV_XFileValid(fileRef))
    {
        pReference = (XFILENAME *)fileRef;
        return pReference->readOnly;
    }
    return FALSE;
}
#endif

#if USE_CREATION_API == TRUE
static XERR PV_CopyWithinFile( XFILE fileRef, long offsetIn, long offsetOut, long size, 
                            void *buffer, long bufferSize)
{
    long    xferSize, bytesTransferred;
    long    offset;
    XERR    err;

    err = 0;
    offset = 0;
    bytesTransferred = 0;
    if (buffer)
    {
        while (bytesTransferred < size)
        {
            xferSize = XMIN(bufferSize, (size - bytesTransferred));

            //Read data
            err = XFileSetPosition(fileRef, offsetIn + offset);
            if (err == 0)
            {
                err = XFileRead(fileRef, buffer, xferSize );
                if (err == 0)
                {
                    //Write data
                    err = XFileSetPosition(fileRef, offsetOut + offset);
                    if (err == 0)
                    {
                        err = XFileWrite(fileRef, buffer, xferSize );
                        if (err != 0)
                        {
                            err = -5;
                            break;
                        }
                    }
                    else
                    {
                        err = -4;
                        break;
                    }
                }
                else
                {
                    err = -3;
                    break;
                }

                bytesTransferred += xferSize; 
                offset += xferSize;
            }
            else
            {
                err = -2;
                break;
            }
        }
    }
    else
    {
        //GACK!
        err = -1;
    }
    return err;
}
#endif  // USE_CREATION_API == TRUE

#if USE_CREATION_API == TRUE
// Force a clean/update of the most recently opened resource file
XBOOL XCleanResource(void)
{
    XBOOL   err;

#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        // use native resource manager
        UpdateResFile(CurResFile());
        return TRUE;
    }
#endif
    err = FALSE;
    if (PV_IsAnyOpenResourceFiles())
    {   // clean from the most recent open file
        err = XCleanResourceFile(g_openResourceFiles[0]);
    }
    return err;
}

// bvk - NEW for Beatnik Windows
//  
//  Scans the file for all resources of 'TRSH' type, and rewrites the file.
//  It does this by scanning through the file, maintaining two sets of offsets.
//  When a 'TRSH' resource is found, the function skips the resource and starts
//  copying over the trashed resource in place.  At the end, the file size is 
//  reset and the appropriate header data changed.
//

#define XFER_BUFFER_SIZE    1024

XBOOL XCleanResourceFile(XFILE fileRef)
{
    XFILENAME           *pReference;
    XERR                err;
    XFILERESOURCEMAP    map;
    long                data, nextIn, nextOut, resType, resID, totalResSize;
    long                count, inResTotal, outResTotal;
    long                resDataSize, fileSize, resInStart, resDataIn, resDataOut;
    char                pResourceName[256];
    XBOOL               isCompacting;
    void                *fileBuffer;

    isCompacting = FALSE;
    err = 0;
    fileBuffer = NULL;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        // first, do we have memory data?
        if (pReference->pResourceData)
        {
            return FALSE;
        }
        // second, are we a read only file?
        else if (PV_IsXFileLocked(fileRef))
        {
            return FALSE;
        }
        else
        {
            // if we have a cache resource, delete it
            XDeleteFileResource(fileRef, XFILECACHE_ID, 0, FALSE);

            fileSize = XFileGetLength(fileRef);
            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    nextOut = nextIn = sizeof(XFILERESOURCEMAP);
                    inResTotal = XGetLong(&map.totalResources);
                    outResTotal = inResTotal;
                    for (count = 0; (count < inResTotal) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, nextIn);        // at start of input resource
                        if (err != 0)
                        {
                            err = -4;
                            break;
                        }
                        resInStart = nextIn;
                        err = XFileRead(fileRef, &nextIn, (long)sizeof(long));      // get next input pointer
                        if (err != 0)
                        {   
                            err = -5;
                            break;
                        }
                        nextIn = XGetLong(&nextIn);
                        totalResSize = nextIn - resInStart;
                        if (nextIn != -1L)
                        {   
                            err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                            if (err != 0)
                            {
                                err = -6;
                                break;
                            }
                            resType = XGetLong(&data);
                            if (resType == XFILETRASH_ID)
                            {
                                //Since we have skipped a trash resource, 
                                //now we must copy/compact every resource that follows!
                                if (isCompacting == FALSE)
                                {
                                    isCompacting = TRUE;
                                    // do we have a cache?
                                    if (pReference->pCache)
                                    {
                                        //if we do, REMOVE IT.  It will be invalid
                                        XFileFreeResourceCache(fileRef);
                                    }

                                    fileBuffer = XNewPtr(XFER_BUFFER_SIZE);
                                    if (fileBuffer == NULL)
                                    {
                                        err = -7;
                                        break;
                                    }
                                }
                                //Note that the outputBuffer pointer (nextOut) is NOT being updated!
                                fileSize -= totalResSize;
                                outResTotal--;
                            }
                            else
                            {
                                //This wasn't a trash resource, so move the output pointer
                                //and/or copy the resource!
                                if (isCompacting)
                                {
                                    //Get remainder of input info
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                    if (err != 0)
                                    {
                                        err = -8;
                                        break;
                                    }
                                    resID = XGetLong(&data);
                                    err = XFileRead(fileRef, &pResourceName[0], 1L);        // get name
                                    if (pResourceName[0])
                                    {
                                        err = XFileRead(fileRef, &pResourceName[1], (long)pResourceName[0]);
                                    }
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get length
                                    if (err != 0)
                                    {
                                        err = -9;
                                        break;
                                    }
                                    resDataSize = XGetLong(&data);      // get resource size
                                    resDataIn = XFileGetPosition(fileRef);  // get current pos

                                    //reposition to start of output block
                                    err = XFileSetPosition(fileRef, nextOut);       // at start
                                    if (err != 0)
                                    {
                                        err = -10;
                                        break;
                                    }

                                    nextOut += totalResSize;    //move output pointer
                                    
                                    //write out information data
                                    XPutLong(&data, nextOut);
                                    err = XFileWrite(fileRef, &data, (long)sizeof(long));
                                    if (err != 0)
                                    {
                                        err = -16;
                                        break;
                                    }
                                    XPutLong(&data, resType);
                                    err = XFileWrite(fileRef, &data, (long)sizeof(long));       // put type
                                    if (err != 0)
                                    {
                                        err = -11;
                                        break;
                                    }
                                    XPutLong(&data, resID);
                                    err = XFileWrite(fileRef, &data, (long)sizeof(long));       // put ID
                                    if (err != 0)
                                    {
                                        err = -12;
                                        break;
                                    }
                                    err = XFileWrite(fileRef, pResourceName, (long)(((char *)pResourceName)[0])+1L);        // put name
                                    if (err != 0)
                                    {
                                        err = -13;
                                        break;
                                    }
                                    XPutLong(&data, resDataSize);
                                    err = XFileWrite(fileRef, &data, (long)sizeof(long));               // put length
                                    if (err != 0)
                                    {
                                        err = -14;
                                        break;
                                    }
                                    resDataOut = XFileGetPosition(fileRef); // get current pos

                                    //Since we are filling for deleted resources, copy this
                                    //res data back into this file over old data!
                                    err = PV_CopyWithinFile( fileRef, 
                                                        resDataIn, resDataOut, resDataSize, 
                                                        fileBuffer, XFER_BUFFER_SIZE );
                                    if (err != 0)
                                    {
                                        err = -15;
                                        break;
                                    }
                                }
                                else
                                {
                                    nextOut += totalResSize;    //move output pointer
                                }
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
                else
                {
                    err = -3;
                }
            }
            else
            {
                err = -2;
            }
        }
    }
    else
    {
        err = -1;
    }

    if (isCompacting && err == 0)
    {
        XFileFreeResourceCache(fileRef);

        err = XFileSetLength(fileRef, fileSize);
        if (err == 0)
        {
            //Update the resource map
            XFileSetPosition(fileRef, 0L);      // at start
            XPutLong(&map.totalResources, outResTotal);
            err = XFileWrite(fileRef, &map, (long)sizeof(XFILERESOURCEMAP));

#if USE_FILE_CACHE != 0
            if (err == 0)
            {
                pReference->pCache = XCreateAccessCache(fileRef);
            }
#endif
        }
    }

    if (fileBuffer)
    {
        XDisposePtr(fileBuffer);
        fileBuffer = NULL;
    }
    return (err == 0) ? TRUE : FALSE;
}
#endif  // USE_CREATION_API == TRUE


//  Marks the selected resource as a 'TRSH' type w/ID of 0.  If collectTrash is TRUE, 
//  writes out the file IN PLACE to remove dead space.  Does not work on read only or
//  memory mapped files.
//
//  Not the fastest thing in the world, but none of the resource functions are!
//
#if USE_CREATION_API == TRUE
XBOOL XDeleteFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID resourceID, XBOOL collectTrash )
{
    XFILENAME           *pReference;
    long                err=0;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    long                whereType, whereID;
    XFILE_CACHED_ITEM   *pCachedItem;

    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        // first, do we have memory data?
        if (pReference->pResourceData)
        {
            return FALSE;
        }
        // second, are we a read only file?
        else if (PV_IsXFileLocked(fileRef))
        {
            return FALSE;
        }

        // do we have a cache?  
        if (pReference->pCache)
        {
            pCachedItem = PV_XGetCacheEntry(fileRef, resourceType, resourceID);
            if (pCachedItem)
            {
                pCachedItem->resourceType = XFILETRASH_ID;
                pCachedItem->resourceID = 0;
                whereType = pCachedItem->fileOffsetName;
                whereType -= ( sizeof(resourceType) + sizeof(resourceID) );
                err = XFileSetPosition(fileRef, whereType );
                if (err != -1)
                {
                    XPutLong(&data, XFILETRASH_ID);
                    err = XFileWrite(fileRef, &data, (long)sizeof(long));                       // put type

                    if (err == 0)
                    {
                        XPutLong(&data, 0);
                        err = XFileWrite(fileRef, &data, (long)sizeof(long));                   // put ID
                    }
                }
                else
                {
                    err = -7;
                }
            }
            else
            {
                goto deleteanyways;
            }
        }
        else
        {
deleteanyways:
            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                whereType = XFileGetPosition(fileRef);  // get current pos

                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                if ((XResourceType)XGetLong(&data) == resourceType)
                                {
                                    whereID = XFileGetPosition(fileRef);    // get current pos

                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                    if ((XLongResourceID)XGetLong(&data) == resourceID)
                                    {
                                        //We found it!
                                        //Now 'TRASH' everything
                                        //td - added to properly overwrite existing id
                                        err = XFileSetPosition(fileRef, whereType);

                                        XPutLong(&data, XFILETRASH_ID);
                                        err = XFileWrite(fileRef, &data, (long)sizeof(long));                       // put type
    
                                        if (err == 0)
                                        {
                                            XPutLong(&data, 0);
                                            err = XFileWrite(fileRef, &data, (long)sizeof(long));                   // put ID
                                            break;
                                        }
                                        else
                                        {
                                            err = -5;
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        }
    }

    if (collectTrash)
    {
        if (XCleanResourceFile(fileRef) == FALSE)
        {
            err = -8;
        }
    }
    return (err == 0);
}
#endif  // USE_CREATION_API == TRUE


// return the number of resources of a particular type.
long XCountResourcesOfType(XResourceType resourceType)
{
    XERR    err;

    err = -1;

#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        return Count1Resources((ResType)resourceType);
    }
#endif
    if (PV_IsAnyOpenResourceFiles())
    {   // delete from the most recent open file
        err = XCountFileResourcesOfType(g_openResourceFiles[0], resourceType);
    }
    return err;
}


// bvk - NEW for Beatnik Windows
//  For a given resource type, counts the number of resources of a type.
//  Does not check for duplicate IDs.
long XCountFileResourcesOfType(XFILE fileRef, XResourceType theType)
{
    long                resCount;
    XResourceType       resourceType;
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total;
    XFILERESOURCECACHE  *pCache;
    XFILE_CACHED_ITEM   *pCacheItem;

    err = 0;
    resCount = 0;
    resourceType = 0;
    pReference = (XFILENAME *)fileRef;
    if (PV_IsAnyOpenResourceFiles())
    {
        if (PV_XFileValid(fileRef))
        {
            if (pReference->pCache)
            {
                pCache = pReference->pCache;
                
                for (count = 0; count < pCache->totalResources; count++)
                {
                    pCacheItem = &pCache->cached[count];

                    if (theType == pCacheItem->resourceType)
                    {
                        resCount++;
                    }
                }
            }
            else
            {
                XFileSetPosition(fileRef, 0L);      // at start
                if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
                {
                    if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                    {
                        next = sizeof(XFILERESOURCEMAP);
                        total = XGetLong(&map.totalResources);
                        for (count = 0; (count < total) && (err == 0); count++)
                        {
                            err = XFileSetPosition(fileRef, next);      // at start
                            if (err == 0)
                            {
                                err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                                next = XGetLong(&next);
                                if (next != -1L)
                                {   
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                    resourceType = (XResourceType)XGetLong(&data);
                                    if (resourceType == theType )
                                    {
                                        resCount++;
                                    }
                                }
                                else
                                {
                                    err = -4;
                                    BAE_PRINTF("Next offset is bad\n");
                                    break;
                                }
                            }
                            else
                            {
                                err = -3;
                                BAE_PRINTF("Can't set next position\n");
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return resCount;
}


// search through open resource files until the resourceType is found based upon the resource index
// pResourceName is a pascal string
XPTR XGetIndexedResource(XResourceType resourceType, XLongResourceID *pReturnedID, long resourceIndex, 
                                void *pResourceName, long *pReturnedResourceSize)
{
    long    count;
    XPTR    pData;

    pData = NULL;
    if (PV_IsAnyOpenResourceFiles())
    {
        for (count = 0; count < g_resourceFileCount; count++)
        {
            pData = XGetIndexedFileResource(g_openResourceFiles[count], resourceType, 
                                    pReturnedID, resourceIndex, pResourceName, pReturnedResourceSize);
            if (pData)
            {
                break;
            }
        }
    }
#if X_PLATFORM == X_MACINTOSH_9
    if ((pData == NULL) && (PV_IsAnyOpenResourceFiles() == FALSE))
    {
        // use native resource manager
        ResType     type;
        Handle      data;
        short       shortID;
        char        pPName[256];
        long        total;

        data = Get1IndResource(resourceType, resourceIndex + 1);
        if (data)
        {
            total = GetHandleSize(data);
            if (pReturnedResourceSize)
            {
                *pReturnedResourceSize = total;
            }
            pData = XNewPtr(total);
            if (pData)
            {
                HLock(data);
                XBlockMove(*data, pData, total);
                HUnlock(data);

                GetResInfo(data, &shortID, &type, (unsigned char *)pPName);
                if (pReturnedID)
                {
                    *pReturnedID = shortID;
                }
                if (pResourceName)
                {
                    XBlockMove(pPName, pResourceName, pPName[0]+1);
                }
                ReleaseResource(data);
            }
        }
    }
#endif
    return pData;
}

// Get a resource based upon its entry count into the resource file. Return NULL if resourceIndex 
// is out of range
// pResourceName is a pascal string
XPTR XGetIndexedFileResource(XFILE fileRef, XResourceType resourceType, XLongResourceID *pReturnedID, 
                                long resourceIndex, void *pResourceName, long *pReturnedResourceSize)
{
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data, next;
    long                count, total, typeCount;
    XPTR                pData;
    char                pPName[256];
    XFILERESOURCECACHE  *pCache;
    XFILE_CACHED_ITEM   *pCacheItem;

    pData = NULL;
    err = 0;
    if (pReturnedResourceSize)
    {
        *pReturnedResourceSize = 0;
    }
    pPName[0] = 0;
    pReference = (XFILENAME *)fileRef;
    typeCount = 0;
    if (PV_XFileValid(fileRef))
    {
        if (pReference->pCache)
        {
            pCache = pReference->pCache;
            
            for (count = 0; count < pCache->totalResources; count++)
            {
                pCacheItem = &pCache->cached[count];

                if (pCacheItem->resourceType == resourceType)
                {
                    if (resourceIndex == typeCount)
                    {
                        *pReturnedID = pCacheItem->resourceID;

                        XFileSetPosition(fileRef, pCacheItem->fileOffsetName);
                        err = XFileRead(fileRef, &pPName[0], 1L);       // get name length
                        if (pPName[0])
                        {
                            err = XFileRead(fileRef, &pPName[1], (long)pPName[0]);
                        }
                        // get data
                        XFileSetPosition(fileRef, pCacheItem->fileOffsetData);

                        // is data memory based?
                        if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
                        {   // don't bother coping data again, since its already in memory
                            pData = PV_GetFilePositionFromMemoryResource(fileRef);
                            if (pData)
                            {
                                if (pReturnedResourceSize)
                                {
                                    *pReturnedResourceSize = pCacheItem->resourceLength;
                                }
                            }
                            else
                            {
                                err = -2;
                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                            }
                        }
                        else
                        {
                            pData = XNewPtr(pCacheItem->resourceLength);
                            if (pData)
                            {
                                err = XFileRead(fileRef, pData, pCacheItem->resourceLength);
                                if (pReturnedResourceSize)
                                {
                                    *pReturnedResourceSize = pCacheItem->resourceLength;
                                }
                                break;
                            }
                            else
                            {
                                err = -2;
                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                break;
                            }
                        }
                    }
                    typeCount++;
                }
            }
        }
        else
        {
            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    for (count = 0; (count < total) && (err == 0); count++)
                    {
                        err = XFileSetPosition(fileRef, next);      // at start
                        if (err == 0)
                        {
                            err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                            next = XGetLong(&next);
                            if (next != -1L)
                            {   
                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                if ((XResourceType)XGetLong(&data) == resourceType)
                                {
                                    if (resourceIndex == typeCount)
                                    {
                                        err = XFileRead(fileRef, pReturnedID, (long)sizeof(long));      // get ID
                                        *pReturnedID = (XLongResourceID)XGetLong(pReturnedID);
                                        err = XFileRead(fileRef, &pPName[0], 1L);       // get name length
                                        if (pPName[0])
                                        {
                                            err = XFileRead(fileRef, &pPName[1], (long)pPName[0]);
                                        }
                                        err = XFileRead(fileRef, &data, (long)sizeof(long));        // get length
                                        data = XGetLong(&data);     // get resource size
                                        // get data
                                        // is data memory based?
                                        if (pReference->pResourceData && (pReference->allowMemCopy == FALSE) )
                                        {   // don't bother coping data again, since its already in memory
                                            pData = PV_GetFilePositionFromMemoryResource(fileRef);
                                            if (pData)
                                            {
                                                if (pReturnedResourceSize)
                                                {
                                                    *pReturnedResourceSize = data;
                                                }
                                                err = 0;
                                                break;
                                            }
                                            else
                                            {
                                                err = -2;
                                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                            }
                                        }
                                        else
                                        {
                                            pData = XNewPtr(data);
                                            if (pData)
                                            {
                                                err = XFileRead(fileRef, pData, data);
                                                if (pReturnedResourceSize)
                                                {
                                                    *pReturnedResourceSize = data;
                                                }
                                                break;
                                            }
                                            else
                                            {
                                                err = -2;
                                                BAE_PRINTF("Out of memory; can't allocate resource\n");
                                                break;
                                            }
                                        }
                                    }
                                    typeCount++;
                                }
                            }
                            else
                            {
                                err = -4;
                                BAE_PRINTF("Next offset is bad\n");
                                break;
                            }
                        }
                        else
                        {
                            err = -3;
                            BAE_PRINTF("Can't set next position\n");
                            break;
                        }
                    }
                }
            }
        }
    }
    if (pResourceName)
    {
        XBlockMove(pPName, pResourceName, (long)pPName[0] + 1);
    }
    return pData;
}

#if USE_CREATION_API == TRUE
// get unique ID from most recent open resource file
XERR XGetUniqueResourceID(XResourceType resourceType, XLongResourceID *pReturnedID)
{
    XERR    err;

    err = -1;
#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        *pReturnedID = UniqueID(resourceType);
        return 0;
    }
#endif
    if (PV_IsAnyOpenResourceFiles())
    {   // pull from the most recent open file
        err = XGetUniqueFileResourceID(g_openResourceFiles[0], resourceType, pReturnedID);
    }
    return err;
}

// Given a resource file, and a type, scan through the file and return a unique and unused XLongResourceID. Will
// return 0 if ok, -1 if failure
XERR XGetUniqueFileResourceID(XFILE fileRef, XResourceType resourceType, XLongResourceID *pReturnedID)
{
    XFILENAME           *pReference;
    XERR                err;
    XFILERESOURCECACHE  *pCache;
    long                count, total, idCount, next, data;
    XLongResourceID     *pIDs;
    XFILERESOURCEMAP    map;

    err = -1;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef) && pReturnedID)
    {
        // theory is we are going to collect all the IDs from either the cache or the file, then walk
        // through them and figure out the best new ID
        *pReturnedID = 0;
        idCount = 0;
        pIDs = NULL;

        // scan from cache and collect them pesky IDs
        pCache = pReference->pCache;
        if (pCache)
        {
            total = pCache->totalResources;
            pIDs = (XLongResourceID *)XNewPtr(sizeof(XLongResourceID) * total);
            if (pIDs)
            {
                err = 0;
                for (count = 0; count < total; count++)
                {
                    // same type
                    if (pCache->cached[count].resourceType == resourceType)
                    {
                        pIDs[idCount] = pCache->cached[count].resourceID;
                        idCount++;
                    }
                }
            }
        }
        else
        {
            err = XFileSetPosition(fileRef, 0L);        // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    next = sizeof(XFILERESOURCEMAP);
                    total = XGetLong(&map.totalResources);
                    pIDs = (XLongResourceID *)XNewPtr(sizeof(XLongResourceID) * total);
                    if (pIDs)
                    {
                        for (count = 0; (count < total) && (err == 0); count++)
                        {
                            err = XFileSetPosition(fileRef, next);      // at start
                            if (err == 0)
                            {
                                err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                                next = XGetLong(&next);
                                if (next != -1L)
                                {   
                                    err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                    if ((XResourceType)XGetLong(&data) == resourceType)
                                    {
                                        err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                        pIDs[idCount] = (XLongResourceID)XGetLong(&data);
                                        idCount++;
                                    }
                                }
                                else
                                {
                                    err = -4;
                                    BAE_PRINTF("Next offset is bad\n");
                                    break;
                                }
                            }
                            else
                            {
                                err = -3;
                                BAE_PRINTF("Can't set next position\n");
                                break;
                            }
                        }
                    }
                }
                else
                {
                    err = -2;
                }
            }
            else
            {
                err = -1;
            }
        }
        // ok, we've now got a list of ID's. Pick a random number and search for a match, if there's
        // a match, try again.
        if (pIDs && (err == 0))
        {
            XBOOL               good, failureCount;
            XLongResourceID     newID;

            good = FALSE;
            failureCount = 0;
            while (good == FALSE)
            {
                newID = (XLongResourceID)XRandom(); // we only pick numbers within 0-32767. This is on purpose.
                                                    // numbers outside of this range are reserved
                good = TRUE;
                for (count = 0; count < idCount; count++)
                {
                    if (pIDs[count] == newID)
                    {
                        good = FALSE;
                        break;
                    }
                }
                failureCount++;
                if (good == FALSE)
                {
                    if (failureCount > idCount) // we've looked through all possible IDs, time to bail
                    {
                        err = -1;
                        good = TRUE;
                    }
                }
                else
                {
                    *pReturnedID = newID;
                }
            }
        }
        else
        {
            err = -1;   // I guess not
        }
        XDisposePtr(pIDs);
    }
#if X_PLATFORM == X_MACINTOSH_9
    else
    {
        short int   currentResourceFile;

        currentResourceFile = CurResFile();
        UseResFile((short int)fileRef);
        *pReturnedID = UniqueID(resourceType);
        UseResFile(currentResourceFile);
        err = 0;
    }
#endif
    return err;
}

XERR XMakeUniqueResourceID(XResourceType resourceType,
                                XLongResourceID* id)
{
    if (XExistsResource(resourceType, *id))
    {
    XLongResourceID     newId;
    
        if (XGetUniqueResourceID(resourceType, &newId) == 0)
        {
            *id = newId;
            return 1;
        }
        return -1;
    }
    return 0;
}
XERR XMakeUniqueFileResourceID(XFILE fileRef, XResourceType resourceType,
                                XLongResourceID* id)
{
    if (XExistsResource(resourceType, *id))
    {
    XLongResourceID     newId;
    
        if (XGetUniqueFileResourceID(fileRef, resourceType, &newId) == 0)
        {
            *id = newId;
            return 1;
        }
        return -1;
    }
    return 0;
}

// Add a resource to the most recently open resource file.
//      resourceType is a type
//      resourceID is an ID
//      pResourceName is a pascal string
//      pData is the data block to add
//      length is the length of the data block
XERR    XAddResource(XResourceType resourceType, XLongResourceID resourceID, void *pResourceName, void *pData, long length)
{
    XERR    err;

    err = -1;
#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        Handle  dataBlock;
        
        dataBlock = NewHandle(length);
        if (dataBlock)
        {
            HLock(dataBlock);
            BlockMove(pData, *dataBlock, length);
            HUnlock(dataBlock);
            AddResource(dataBlock, (ResType)resourceType, resourceID, (unsigned char *)pResourceName);
        }
        return (ResError() == noErr) ? 0 : -1;
    }
#endif
    if (PV_IsAnyOpenResourceFiles())
    {   // add to the most recent open file
        err = XAddFileResource(g_openResourceFiles[0], resourceType, resourceID, pResourceName, pData, length);
    }
    return err;
}


// Delete a resource from the most recently open resource file.
//      resourceType is a type
//      resourceID is an ID
//      collectTrash if TRUE will force an update, otherwise it will happen when the file is closed
XBOOL XDeleteResource(XResourceType resourceType, XLongResourceID resourceID, XBOOL collectTrash)
{
#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        Handle  dataBlock;
        
        dataBlock = Get1Resource(resourceType, resourceID);
        if (dataBlock)
        {
            RemoveResource(dataBlock);
            if (collectTrash)
            {
                UpdateResFile(CurResFile());
            }
        }
        return (ResError() == noErr) ? TRUE : FALSE;
    }
#endif
    if (PV_IsAnyOpenResourceFiles())
    {   // delete from the most recent open file
        return XDeleteFileResource(g_openResourceFiles[0], resourceType, resourceID, collectTrash);
    }
    return FALSE;
}


// Add a resource to a particular file
//      fileRef is the open file
//      resourceType is a type
//      resourceID is an ID
//      pResourceName is a pascal string
//      pData is the data block to add
//      length is the length of the data block
XERR XAddFileResource(XFILE fileRef, XResourceType resourceType,
                        XLongResourceID resourceID, void const* pResourceName,
                        void *pData, long length)
{
    XFILENAME           *pReference;
    long                err;
    XFILERESOURCEMAP    map;
    long                data;
    char                fakeName[2];
    long                next, nextsave;
    XFILE_CACHED_ITEM   cacheItem;

#if X_PLATFORM == X_MACINTOSH_9
    if (PV_IsAnyOpenResourceFiles() == FALSE)
    {
        // use native resource manager
        Handle  theData;

        theData = NewHandle(length);
        if (theData)
        {
            HLock(theData);
            XBlockMove(pData, *theData, length);
            HUnlock(theData);
            AddResource(theData, resourceType, (short)resourceID, (unsigned char *)pResourceName);
        }
        return 0;
    }
#endif
    err = -1;
    pReference = (XFILENAME *)fileRef;
    if (PV_XFileValid(fileRef))
    {
        // the cache will updated, and the file based cache will be deleted
        if (pData && length)
        {
            // if we have a cache resource, delete it
            XDeleteFileResource(fileRef, XFILECACHE_ID, 0, FALSE);

            XFileSetPosition(fileRef, 0L);      // at start
            if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
            {
                if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                {
                    XFileSetPosition(fileRef, XFileGetLength(fileRef));                     // at end
    
                    nextsave = XFileGetPosition(fileRef);   // rewrite this later
                    next = -1;                              // store all Fs for now
                    err = XFileWrite(fileRef, &next, (long)sizeof(long));

                    XPutLong(&data, (unsigned long)resourceType);
                    err = XFileWrite(fileRef, &data, (long)sizeof(long));                       // put type
                    cacheItem.resourceType = (unsigned long)resourceType;
    
                    if (err == 0)
                    {
                        XPutLong(&data, (unsigned long)resourceID);
                        err = XFileWrite(fileRef, &data, (long)sizeof(long));                   // put ID
                        cacheItem.resourceID = (unsigned long)resourceID;

                        if (err == 0)
                        {
                            data = XFileGetPosition(fileRef);   // get name pos
                            cacheItem.fileOffsetName = data;

                            if (pResourceName)
                            {
                                err = XFileWrite(fileRef, pResourceName, (long)(((char *)pResourceName)[0])+1L);        // put name
                            }
                            else
                            {
                                fakeName[0] = 0;
                                err = XFileWrite(fileRef, fakeName, 1L);        // put name
                            }
                            
                            if (err == 0)
                            {
                                XPutLong(&data, length);
                                err = XFileWrite(fileRef, &data, (long)sizeof(long));               // put length
                                cacheItem.resourceLength = length;
        
                                if (err == 0)
                                {
                                    data = XFileGetPosition(fileRef);   // get data pos
                                    cacheItem.fileOffsetData = data;
                                    err = XFileWrite(fileRef, pData, length);                   // put data block
        
                                    if (err == 0)
                                    {
                                        next = XFileGetPosition(fileRef);   // get pos of next resource

                                        XFileSetPosition(fileRef, 0L);      // back to start
                                        data = XGetLong(&map.totalResources) + 1;
                                        XPutLong(&map.totalResources, data);
                                        err = XFileWrite(fileRef, &map, (long)sizeof(XFILERESOURCEMAP));

                                        if (err == 0)
                                        {
                                            // write real offset-of-next-resource value
                                            XFileSetPosition(fileRef, nextsave);
                                            XPutLong(&data, next);
                                            err = XFileWrite(fileRef, &data, (long)sizeof(long));

                                            if (err == 0)
                                            {
                                                // Now we add this to the native RAM cache
                                                if (pReference->pCache)
                                                {
                                                    PV_AddToAccessCache(fileRef, &cacheItem );
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return err;
}
#endif  // USE_CREATION_API == TRUE


// get a resource type from the name. cName is a C string
XPTR XGetNamedResource(XResourceType resourceType, void *cName, long *pReturnedResourceSize)
{
    XPTR                pData;
    XFILE_CACHED_ITEM   *pCacheItem;
    char                pResourceName[256];
    long                count, total;
    short int           fileCount;
    XERR                err;
    XFILE               fileRef;
    XFILERESOURCEMAP    map;
    long                next, data;
    XLongResourceID     resourceID;

    pData = NULL;
    if (pReturnedResourceSize)
    {
        *pReturnedResourceSize = 0;
    }
#if X_PLATFORM == X_MACINTOSH_9
    {
        Handle  theData;
        long    size;

        // first look inside any open resource files
        if (PV_IsAnyOpenResourceFiles())
        {
            for (count = 0; count < g_resourceFileCount; count++)
            {
                pCacheItem = PV_XGetNamedCacheEntry(g_openResourceFiles[count], resourceType, cName);
                if (pCacheItem)
                {
                    pData = XGetFileResource(g_openResourceFiles[count], pCacheItem->resourceType, 
                                                                    pCacheItem->resourceID, 
                                                                    pResourceName, pReturnedResourceSize);
                    // we found our resource
                    break;
                }
            }
        }
        if (pData == NULL)
//      if ((pData == NULL) && (PV_IsAnyOpenResourceFiles() == FALSE))
        {
            // use native resource manager
            XStrCpy(pResourceName, (char *)cName);
            XCtoPstr(pResourceName);
            theData = Get1NamedResource(resourceType, (unsigned char *)pResourceName);
            if (theData)
            {
                size = GetHandleSize(theData);
                pData = XNewPtr(size);
                if (pData)
                {
                    HLock(theData);
                    XBlockMove(*theData, pData, size);
                    HUnlock(theData);
                    ReleaseResource(theData);
                    if (pReturnedResourceSize)
                    {
                        *pReturnedResourceSize = size;
                    }
                }
            }
        }
    }
#endif
    if (pData == NULL)
    {
        if (PV_IsAnyOpenResourceFiles())
        {
            for (fileCount = 0; fileCount < g_resourceFileCount; fileCount++)
            {
                pCacheItem = PV_XGetNamedCacheEntry(g_openResourceFiles[fileCount], resourceType, cName);
                if (pCacheItem)
                {
                    pData = XGetFileResource(g_openResourceFiles[fileCount], pCacheItem->resourceType, 
                                                                    pCacheItem->resourceID, 
                                                                    pResourceName, pReturnedResourceSize);
                    // we found our resource
                    break;
                }
                else
                {
                    err = 0;
                    // search through without cache.
                    fileRef = g_openResourceFiles[fileCount];
                    XFileSetPosition(fileRef, 0L);      // at start
                    if (XFileRead(fileRef, &map, (long)sizeof(XFILERESOURCEMAP)) == 0)
                    {
                        if (XGetLong(&map.mapID) == XFILERESOURCE_ID)
                        {
                            next = sizeof(XFILERESOURCEMAP);
                            total = XGetLong(&map.totalResources);
                            for (count = 0; (count < total) && (err == 0); count++)
                            {
                                err = XFileSetPosition(fileRef, next);      // at start
                                if (err == 0)
                                {
                                    err = XFileRead(fileRef, &next, (long)sizeof(long));        // get next pointer
                                    next = XGetLong(&next);
                                    if (next != -1L)
                                    {   
                                        err = XFileRead(fileRef, &data, (long)sizeof(long));        // get type
                                        if ((XResourceType)XGetLong(&data) == resourceType)
                                        {
                                            err = XFileRead(fileRef, &data, (long)sizeof(long));        // get ID
                                            resourceID = (XLongResourceID)XGetLong(&data);
                                            err = XFileRead(fileRef, &pResourceName[0], 1L);        // get name
                                            if (pResourceName[0])
                                            {
                                                err = XFileRead(fileRef, &pResourceName[1], (long)pResourceName[0]);
                                                XPtoCstr(pResourceName);
                                                if (XStrCmp(pResourceName, (char *)cName) == 0)
                                                {   // match?
                                                    return XGetFileResource(fileRef, 
                                                                    resourceType, resourceID, 
                                                                    pResourceName, pReturnedResourceSize);
                                                }
                                                err = XFileRead(fileRef, &data, (long)sizeof(long));        // get length
                                                data = XGetLong(&data);     // get resource size
                                                if (XFileSetPositionRelative(fileRef, data))
                                                {
                                                    err = -2;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    err = -4;
                                    BAE_PRINTF("Next offset is bad\n");
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return pData;
}

XBOOL XExistsResource(XResourceType resourceType, XLongResourceID resourceID)
{
char        name[256];

    return XGetResourceName(resourceType, resourceID, name);
}
XBOOL XExistsFileResource(XFILE fileRef,
                            XResourceType resourceType, XLongResourceID resourceID)
{
char        name[256];

    return XGetFileResourceName(fileRef, resourceType, resourceID, name);
}

// Get just the resource name from resourceType and resourceID.
// The cName is a 'C' string which is returned
XBOOL XGetResourceName(XResourceType resourceType, XLongResourceID resourceID,
                        char *cName)
{
    int         count;

    if (cName)
    {
        cName[0] = 0;
#if X_PLATFORM == X_MACINTOSH_9
        // first look in any open resource files
        if (PV_IsAnyOpenResourceFiles())
        {
            for (count = 0; count < g_resourceFileCount; count++)
            {
                if (XGetFileResourceName(g_openResourceFiles[count],
                                            resourceType, resourceID, cName))
                {
                    // we found data
                    break;
                }
            }
        }
        if (cName[0] == 0)
        {   // use native resource manager
            short int       theID;
            unsigned long   theType;
            Handle          theData;

            SetResLoad(FALSE);
            theData = Get1Resource(resourceType, (short int)resourceID);
            SetResLoad(TRUE);
            if (theData)
            {
                GetResInfo(theData, &theID, &theType, (unsigned char *)cName);
                XPtoCstr(cName);
                DetachResource(theData);
                DisposeHandle(theData);
                return TRUE;
            }
        }
        else
        {
            return TRUE;
        }
#else
        for (count = 0; count < g_resourceFileCount; count++)
        {
            if (XGetFileResourceName(g_openResourceFiles[count],
                                        resourceType, resourceID, cName))
            {
                return TRUE;
            }
        }
#endif
    }
    return FALSE;
}
XBOOL XGetFileResourceName(XFILE fileRef, XResourceType resourceType,
                            XLongResourceID resourceID, char cName[256])
{
    if (cName)
    {
        cName[0] = 0;
    }
    if (XGetResourceNameOnly(fileRef, resourceType, resourceID, cName))
    {
        XPtoCstr(cName);
        return TRUE;
    }
    return FALSE;
}

// Get a resource and detach it from the resource manager. Which means that you'll need to call XDisposePtr
// to free the memory
XPTR XGetAndDetachResource(XResourceType resourceType, XLongResourceID resourceID, long *pReturnedResourceSize)
{
#if X_PLATFORM == X_MACINTOSH_9
    Handle      theData;
    XPTR        pData;
    XPTR        pNewData;
    long        size;
    char        szPName[256];
    short int   count;
    XFILE       fileRef;
    XFILENAME   *pReference;

    pData = NULL;
    if (pReturnedResourceSize)
    {
        *pReturnedResourceSize = 0;
    }
    // first look in any open resource files
    if (PV_IsAnyOpenResourceFiles())
    {
        for (count = 0; count < g_resourceFileCount; count++)
        {
            pData = XGetFileResource(g_openResourceFiles[count], resourceType, resourceID, szPName, &size);
            if (pData)
            {
                fileRef = g_openResourceFiles[count];
                pReference = (XFILENAME *)fileRef;
                if (pReference->pResourceData && (pReference->allowMemCopy) )
                {
                    //In the case of a memory file, we have to create a new block to return.
                    pNewData = XNewPtr(size);
                    if (pNewData)
                    {
                        XBlockMove(pData, pNewData, size);
                        pData = pNewData;
                    }
                    else
                    {
                        pData = NULL;   //not detaching is NO SUCCESS!
                    }
                }
                if (pReturnedResourceSize)
                {
                    *pReturnedResourceSize = size;
                }
                // we found data
                break;
            }
        }
    }
    if (pData == NULL)
//  if ((pData == NULL) && (PV_IsAnyOpenResourceFiles() == FALSE))
    {   // use native resource manager
        theData = Get1Resource(resourceType, (short int)resourceID);
        if (theData)
        {
            size = GetHandleSize(theData);
            pData = XNewPtr(size);
            if (pData)
            {
                HLock(theData);
                XBlockMove(*theData, pData, size);
                HUnlock(theData);
                ReleaseResource(theData);
                if (pReturnedResourceSize)
                {
                    *pReturnedResourceSize = size;
                }
            }
        }
    }
    return pData;
#else   // X_PLATFORM == X_MACINTOSH_9
    char        szPName[256];
    long        lSize = 0;
    XPTR        pData = NULL;
    XPTR        pNewData;
    short int   count;
    XFILE       fileRef;
    XFILENAME   *pReference;

    for (count = 0; count < g_resourceFileCount; count++)
    {
        pData = XGetFileResource(g_openResourceFiles[count], resourceType, resourceID, szPName, &lSize);

        if (pData)
        {
            fileRef = g_openResourceFiles[count];
            pReference = (XFILENAME *)fileRef;
            if (pReference->pResourceData && (pReference->allowMemCopy) )
            {
                //In the case of a memory file, we have to create a new block to return.
                pNewData = XNewPtr( lSize );
                if (pNewData)
                {
                    XBlockMove(pData, pNewData, lSize );
                    pData = pNewData;
                }
                else
                {
                    pData = NULL;   //not detaching is NO SUCCESS!
                }
            }

            if (pReturnedResourceSize)
            {
                *pReturnedResourceSize = lSize;
            }
            // we found data
            break;
        }
    }
    return pData;
#endif  //  X_PLATFORM == X_MACINTOSH_9
}

// get current most recently opened resource file, or NULL if nothing is open
XFILE XFileGetCurrentResourceFile(void)
{
    if (PV_IsAnyOpenResourceFiles())
    {
        return g_openResourceFiles[0];
    }
    return (XFILE)NULL;
}

// make sure this resource file is first in the scan list
void XFileUseThisResourceFile(XFILE fileRef)
{
    short int   fileCount;
    XFILE       currentFirst;

    if (PV_XFileValid(fileRef))
    {
        fileCount = PV_FindResourceFileReferenceIndex(fileRef);
        if (fileCount != -1)
        {
            currentFirst = g_openResourceFiles[0];
            g_openResourceFiles[0] = fileRef;
            g_openResourceFiles[fileCount] = currentFirst;
        }
    }
}


// Decompress a 'csnd' format sound.
// First byte is a type.
// Next three bytes are a length.
// Type 0 is Delta LZSS compression
void * XDecompressPtr(void* pData, unsigned long dataSize, XBOOL ignoreType)
{
    unsigned long       theTotalSize;
    XCOMPRESSION_TYPE   theType;
    XPTR                theNewData;

    theNewData = NULL;
    if (pData && dataSize)
    {
        theTotalSize = XGetLong(pData);
        theType = ignoreType ? X_RAW
                             : (XCOMPRESSION_TYPE)(theTotalSize >> 24L);
        theTotalSize &= 0x00FFFFFFL;
        theNewData = XNewPtr(theTotalSize);
        if (theNewData)
        {
            switch (theType)
            {
                case X_RAW:
                    LZSSUncompress((unsigned char*)pData + sizeof(long),
                                    dataSize - sizeof(long), 
                                    (unsigned char*)theNewData, 
                                    theTotalSize);
                    break;
                case X_MONO_8:
                    LZSSUncompressDeltaMono8((unsigned char*)pData + sizeof(long),
                                                dataSize - sizeof(long), 
                                                (unsigned char*)theNewData, 
                                                theTotalSize);
                    break;
                case X_STEREO_8:
                    LZSSUncompressDeltaStereo8((unsigned char*)pData + sizeof(long),
                                                dataSize - sizeof(long), 
                                                (unsigned char*)theNewData, 
                                                theTotalSize);
                    break;
                case X_MONO_16:
                    LZSSUncompressDeltaMono16((unsigned char*)pData + sizeof(long),
                                                dataSize - sizeof(long), 
                                                (short*)theNewData, 
                                                theTotalSize);
                    break;
                case X_STEREO_16:
                    LZSSUncompressDeltaStereo16((unsigned char*)pData + sizeof(long),
                                                dataSize - sizeof(long), 
                                                (short*)theNewData, 
                                                theTotalSize);
                    break;
                default:
                    XDisposePtr(theNewData);
                    theNewData = NULL;
                    break;
            }
        }
    }
    return theNewData;
}

#if USE_CREATION_API == TRUE

// Given a block of data and a size, this will compress it into a newly-allocated.
// block of data.  The original pointer is not deallocated.  The new pointer must
// be deallocated when no longer needed.  The first byte of the compressed data
// is the XCOMPRESSION_TYPE used and the following 3 bytes are the length of the 
// uncompressed data.  (Original data cannot be larger than than 16 MB.)
// A pointer to the compressed block is stored at compressedDataTarget.
// The length of the compressed data is returned if compression succeeds
//  If -1 is returned, the compression failed
//  If 0 is returned, compression was aborted by proc.
long XCompressPtr(XPTR* compressedDataTarget,
                    XPTR pData, unsigned long dataSize,
                    XCOMPRESSION_TYPE type,
                    XCompressStatusProc proc, void* procData)
{
XPTR            compressedData;
long            compressedSize;
XBYTE           *realData;

    if (!compressedDataTarget)
    {
        return -1;  // bad parameters
    }
    *compressedDataTarget = NULL;
    if (!pData || (dataSize == 0))
    {
        return -1;  // bad parameters
    }
    if (dataSize > 0x00FFFFFF)
    {
        return -1;  // too big
    }

    compressedData = XNewPtr(dataSize);
    if (!compressedData)
    {
        return -1;  // out of memory
    }

    switch (type)
    {
    case X_RAW:
        compressedSize = LZSSCompress((XBYTE*)pData, dataSize,
                                        (XBYTE*)compressedData,
                                        proc, procData);
        break;
    case X_MONO_8:
        compressedSize = LZSSCompressDeltaMono8((XBYTE*)pData, dataSize,
                                                (XBYTE*)compressedData,
                                                proc, procData);
        break;
    case X_STEREO_8:
        compressedSize = LZSSCompressDeltaStereo8((XBYTE*)pData, dataSize,
                                                    (XBYTE*)compressedData,
                                                    proc, procData);
        break;
    case X_MONO_16:
        compressedSize = LZSSCompressDeltaMono16((short*)pData, dataSize,
                                                    (XBYTE*)compressedData,
                                                    proc, procData);
        break;
    case X_STEREO_16:
        compressedSize = LZSSCompressDeltaStereo16((short*)pData, dataSize,
                                                    (XBYTE*)compressedData,
                                                    proc, procData);
        break;
    }
    
    if (compressedSize > 0)
    {
        compressedSize += sizeof(long);
        realData = (XBYTE*)XNewPtr(compressedSize);
        if (realData)
        {
            XPutLong(realData, dataSize);
            realData[0] = (XBYTE)type;
            XBlockMove(compressedData, realData + sizeof(long),
                        compressedSize - sizeof(long));
        }
        *compressedDataTarget = realData;
    }
    XDisposePtr(compressedData);

    return compressedSize;
}

#endif  // USE_CREATION_API == TRUE

XPTR XDuplicateMemory(XPTRC src, XDWORD len)
{
    XPTR dup;

    dup = NULL;
    if (src)
    {
        dup = XNewPtr((long)len);
        if (dup)
        {
            XBlockMove(src, dup, (long)len);
        }
    }
    return dup;
}

char * XDuplicateStr(char const* src)
{
    char *dup;

    dup = NULL;
    if (src)
    {
        dup = (char *)XNewPtr(XStrLen(src)+1);
        if (dup)
        {
            XStrCpy(dup, src);
        }
    }
    return dup;
}

// Strip characters between 0-32 in place
void XStripStr(char *pString)
{
    char *pNew;
    
    pNew = XDuplicateAndStripStr(pString);
    if (pNew)
    {
        XStrCpy(pString, pNew);
    }
    XDisposePtr(pNew);
}
    

// Duplicate and string characters between 0-32
char * XDuplicateAndStripStr(char const* src)
{
    long            length;         // must be signed
    char*           cStrippedString;
    char*           cDest;
    char const*     cSource;

    cStrippedString = NULL;
    // strip out undesirable characters, give them some walking money
    length = XStrLen(src);
    if (length)
    {
        cStrippedString = (char *)XNewPtr(length+1);
        if (cStrippedString)
        {
            cDest = cStrippedString;
            cSource = src;
            while (*cSource)
            {
                // we only want to strip 0-32 nothing else
                if ((*cSource >= 32) || (*cSource < 0))
                {
                    *cDest++ = *cSource;
                }
                cSource++;
            }
            *cDest = 0;
        }
    }
    return cStrippedString;
}

// standard strcpy
// Copies 'C' string src into dest
char * XStrCpy(char *dest, char const* src)
{
    char *sav;

    sav = dest;
    if (src == NULL)
    {
        src = "";
    }
    if (dest)
    {
        while (*src)
        {
            *dest++ = *src++;
        }
        *dest = 0;
    }
    return sav;
}

XBOOL XIsDigit(short int c)
{
    if (c >= '0' && c <= '9')
    {
        return TRUE;
    }   
    return FALSE;
}

short int XLowerCase(short int c)
{
    return( ( ((c >= 'A') && (c <= 'Z')) ? c | 0x20 : c) );
}

// string search.
char * XStrStr(char *source, char const* pattern)
{
    unsigned char*          s1;
    unsigned char const*    p1;
    unsigned char           firstc, c1, c2;

    if (source == NULL)
    {
        source = "";
    }
    if (pattern == NULL)
    {
        pattern = "";
    }
    s1 = (unsigned char *)source;
    p1 = (unsigned char const*)pattern;

    if (!(firstc = *p1++))
    {
        return((char *) s1);
    }
    while((c1 = *s1) != 0)
    {
        s1++;
        if (c1 == firstc)
        {
            const unsigned char * s2 = s1;
            const unsigned char * p2 = p1;
            
            while ((c1 = *s2++) == (c2 = *p2++) && c1) {};
            
            if (!c2)
            {
                return((char *) s1 - 1);
            }
        }
    }
    return(NULL);
}

// string search, but ignore case
char * XLStrStr(char *source, char const* pattern)
{
    unsigned char* s1;
    unsigned char const* p1;
    unsigned char firstc, c1, c2;

    if (source == NULL)
    {
        source = "";
    }
    if (pattern == NULL)
    {
        pattern = "";
    }
    s1 = (unsigned char*)source;
    p1 = (unsigned char const*)pattern;

    if (!(firstc = *p1++))
    {
        return((char *) s1);
    }
    while((c1 = *s1) != 0)
    {
        s1++;
        if (XLowerCase(c1) == XLowerCase(firstc))
        {
            const unsigned char * s2 = s1;
            const unsigned char * p2 = p1;
            
            while (XLowerCase(c1 = *s2++) == XLowerCase(c2 = *p2++) && XLowerCase(c1)) {};
            
            if (!c2)
            {
                return((char *) s1 - 1);
            }
        }
    }
    return(NULL);
}

char* XStrCatChar(char *dest, char c)
{
    char    cc[2];

    cc[0] = c;
    cc[1] = 0;
    return XStrCat(dest, (char const*)cc);
}

char * XStrCat(char * dest, char const* source)
{
    char const* p = source;
    char*       q = dest;
    if (dest)
    {
        if (source == NULL)
        {
            source = "";
        }
        while ((*q++) != 0) {};
        
        q--;
        
        while ((*q++ = *p++) != 0) {};
    }   
    return dest;
}

long XStrLen(char const* src)
{
#if 1
    short int len;

    len = -1;
    if (src == NULL)
    {
        src = "";
    }
    do
    {
        len++;
    } while  (*src++);

    return len;
#else
char    const* s;

    s = src;
    if (s)
    {
        while (*s) s++;
    }
    return s - src;
#endif
}

// standard strcmp
short int XStrCmp(char const* s1, char const* s2)
{
    if (s1 == NULL)
    {
        s1 = "";
    }
    if (s2 == NULL)
    {
        s2 = "";
    }
    
    while (1) 
    {
        if (*s1 == *s2) 
        {
            if (*s1 == 0) 
            {
                return 0;
            } 
            else 
            {
                s1++;
                s2++;
            }
        } 
        else if (*s1 > *s2) 
        {
            return 1;
        } 
        else 
        {
            return -1;
        }
    }
}

// standard strcmp, but ignore case
short int XLStrCmp(char const* s1, char const* s2)
{
    if (s1 == NULL)
    {
        s1 = "";
    }
    if (s2 == NULL)
    {
        s2 = "";
    }
    
    while (1) 
    {
        if (XLowerCase(*s1) == XLowerCase(*s2)) 
        {
            if (*s1 == 0) 
            {
                return 0;
            } 
            else 
            {
                s1++;
                s2++;
            }
        } 
        else if (XLowerCase(*s1) > XLowerCase(*s2)) 
        {
            return 1;
        } 
        else 
        {
            return -1;
        }
    }
}

// Standard strncmp, but ignore case. Compares zero terminated s1 with non zero terminated s2 with s2 having
// a length of n
short int XLStrnCmp(char const* s1, char const* s2, long n)
{
    if (s1 == NULL)
    {
        s1 = "";
    }
    if (s2 == NULL)
    {
        s2 = "";
    }

    if (n)
    {
        do 
        {
            if (XLowerCase(*s1) != XLowerCase(*s2++))
            {
                return (*(unsigned char *)s1 - *(unsigned char *)--s2);
            }
            if (*s1++ == 0)
            {
                break;
            }
        } while (--n != 0);
    }
    return 0;
}
// Standard strncmp. Compares zero terminated s1 with non zero terminated s2 with s2 having
// a length of n
short int XStrnCmp(char const* s1, char const* s2, register long n)
{
    if (s1 == NULL)
    {
        s1 = "";
    }
    if (s2 == NULL)
    {
        s2 = "";
    }

    if (n)
    {
        do 
        {
            if (*s1 != *s2++)
            {
                return (*(unsigned char *)s1 - *(unsigned char *)--s2);
            }
            if (*s1++ == 0)
            {
                break;
            }
        } while (--n != 0);
    }
    return 0;
}

// Converts a long value to a base 10 string,
// Returns pointer to the '\0' at the end of the string
char* XLongToStr(char* dest, long value)
{
char*           t;

    t = dest;
    if (t)
    {
        if (value == 0)
        {
            *t++ = '0';
        }
        else
        {
        unsigned long   v;
        unsigned long   power;
        XBOOL           nonzeroFound;

            v = value;
            if (value < 0)
            {
                *t++ = '-';
                v = -value;
            }
            
            nonzeroFound = FALSE;
            power = 1000000000;
            while (power > 0)
            {
            unsigned int    digit;
            
                digit = v / power;
                if ((digit > 0) || nonzeroFound)
                {
                    BAE_ASSERT(digit <= 9);
                    *t++ = (char)(digit + '0');
                    v -= digit * power;
                    nonzeroFound = TRUE;
                }
                power /= 10;
            }
        }
        *t = '\0';
    }
    return t;
}

// This will convert a string to a base 10 long value
long XStrnToLong(char const* pData, long length)
{
    long    result, num, count;
    char    data[12];
    
    result = 0;
    num = 0;
    for (count = 0; count < length; count++)
    {
        if (*pData != 0x20)
        {
            if ( ((*pData) >= '0') && ((*pData) <= '9') )
            {
                data[num++] = *pData;
                if (num > 11)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        pData++;
    }
    if (num)
    {
        for (count = 0; count < num; count++)
        {
            result *= 10;
            result += data[count] - '0';
        }
    }
    return result;
}


#if 1
short int XMemCmp(void const* src1, void const * src2, long n)
{
    unsigned const char * p1;
    unsigned const char * p2;

    
    for (p1 = (unsigned const char *) src1, p2 = (unsigned const char *) src2, n++; --n;)
    {
        if (*p1++ != *p2++)
        {
            return((*--p1 < *--p2) ? -1 : +1);
        }
    }
    return 0;
}
#else
short int XMemCmp(void const* src1, void const* src2, long n)
{
unsigned char const*    p1;
unsigned char const*    p2;

    p1 = (unsigned char const*)src1;
    p2 = (unsigned char const*)src2;
    while (--n >= 0)
    {
        if (*p1++ != *p2++)
        {
            return((*--p1 < *--p2) ? -1 : +1);
        }
    }
    return 0;
}
#endif


#if USE_CREATION_API == TRUE
/*
 *  pseudo-random number generator
 *
 */
static unsigned long seed = 1;

// return a pseudo-random number in the range of 0-32767
short int XRandom(void)
{
    seed = seed * 1103515245 + 12345;

    return (short int)((seed >> 16L) & 0x7FFFL);        // high word of long, remove high bit
}


/*
 *  seed pseudo-random number generator
 *
 */

void XSeedRandom(unsigned long n)
{
    seed = n;
}

short int XRandomRange(short int max)
{
    static char setup = 0;

    if (setup == 0)
    {
        XSeedRandom(XMicroseconds());
        setup = 1;
    }
    return XRandom() % max;
}
#endif  // USE_CREATION_API

#if USE_FULL_RMF_SUPPORT == TRUE
/****************************************************************************
**
** XIsWinInMac() determines whether a Macintosh character has an
** equivalent in the Windows character set.
**
** XIsMacInWin() determines whether a Windows character has an
** equivalent in the Macintosh character set.
**
** XTranslateWinToMac() provides the Macintosh-equivalent of a Windows
** character code.
** 
** XTranslateMacToWin() provides the Windows-equivalent of a Macintosh
** character code.
**
*****************************************************************************/
////////////////////////////////////////////////// DATA:

static const unsigned char macToWinTable[128] =
{
    0xC4,   // A-umlaut
    0xC5,   // A-circle
    0xC7,   // C-cedire
    0xC9,   // E-acute
    0xD1,   // N-tilde
    0xD6,   // O-umlaut
    0xDC,   // U-umlaut
    0xE1,   // a-acute
    0xE0,   // a-grave
    0xE2,   // a-circumflex
    0xE4,   // a-umlaut
    0xE3,   // a-tilde
    0xE5,   // a-circle
    0xE7,   // c-cedire
    0xE9,   // e-acute
    0xE8,   // e-grave

    0xEA,   // e-circumflex
    0xEB,   // e-umlaut
    0xED,   // i-acute
    0xEC,   // i-grave
    0xEE,   // i-circumflex
    0xEF,   // i-umlaut
    0xF1,   // n-tilde
    0xF3,   // o-acute
    0xF2,   // o-grave
    0xF4,   // o-circumflex
    0xF6,   // o-umlaut
    0xF5,   // o-tilde
    0xFA,   // u-acute
    0xF9,   // u-grave
    0xFB,   // u-circumflex
    0xFC,   // u-umlaut

    0x86,   // little up arrow (dagger substituted)
    0xB0,   // degrees
    0xA2,   // cents
    0xA3,   // pounds
    0xA7,   // section
    0x95,   // bullet
    0xB6,   // paragraph
    0xDF,   // german double-s
    0xAE,   // registered
    0xA9,   // copyright (same in both sets)
    0x99,   // tm
    0xB4,   // acute
    0xA8,   // umlaut
    0xB1,   // not equals (plus-minus substituted)
    0xC6,   // AE
    0xD8,   // O-slash

    0xBF,   // infinity (upsidedown-? substituted)
    0xB1,   // plus-minus
    '<',    // less-than or equal-to
    '>',    // greater-than or equal-to
    0xA5,   // yen
    0xB5,   // mu
    'd',    // delta
    'S',    // SIGMA
    'P',    // PI
    'p',    // pi
    0x83,   // f without bar (fi substituted)
    0xAA,   // small a with bar
    0xBA,   // small o with bar
    'O',    // omega
    0xE6,   // ae
    0xF8,   // o-slash

    0xBF,   // upsidedown-?
    0xA1,   // upsidedown-!
    0xAC,   // rho(?)
    '/',    // integral
    0x83,   // fi(?)
    '~',    // approx=
    'D',    // DELTA
    0xAB,   // euro double-open-quote
    0xBB,   // euro double-close-quote
    0x85,   // ellipsis
    0xA0,   // non-breaking space
    0xC0,   // A-grave
    0xC3,   // A-tilde
    0xD5,   // O-tilde
    0x8C,   // OE
    0x9C,   // oe

    0x96,   // dash
    0x97,   // long dash
    0x93,   // open "
    0x94,   // close "
    0x91,   // open '
    0x92,   // close '
    0xF7,   // divide by
    '$',    // wordstar diamond
    0xFF,   // y-umlaut
    0x9F,   // Y-umlaut
    '/',    // some other kinda slash?
    0xA4,   // can't remember what this is called
    0x8B,   // euro single-open-quote
    0x9B,   // euro single-close-quote
    'f',    // fi
    'f',    // fl

    0x87,   // little up/down arrow (double dagger substituted)
    0xB7,   // center dot
    0x82,   // baseline single-quote
    0x84,   // baseline double-quote
    0x89,   // per-thousand
    0xC2,   // A-circumflex 
    0xCA,   // E-circumflex
    0xC1,   // A-acute
    0xCB,   // E-umlaut
    0xC8,   // E-grave
    0xCD,   // I-acute
    0xCE,   // I-circumflex
    0xCF,   // I-umlaut
    0xCC,   // I-grave
    0xD3,   // O-acute
    0xD4,   // O-circumflex

    '@',    // apple
    0xD2,   // O-grave
    0xDA,   // U-acute
    0xDB,   // U-circumflex
    0xD9,   // U-grave
    'i',    // dotless i
    0x88,   // circumflex
    0x98,   // tilde
    0x8F,   // bar
    '\'',   // scoop accent
    0xB0,   // single dot accent (circle accent substituted)
    0xB0,   // circle accent
    0xB8,   // cedire
    0x98,   // another tilde(?)
    '.',    // backwards cedire
    '\'',   // another scoop accent
};


////////////////////////////////////////////////// FUNCTIONS:

XBOOL XIsWinInMac(char ansiChar)
{
    return (XTranslateWinToMac(ansiChar) == (char)0xF0) ? (XBOOL)FALSE
                                                 : (XBOOL)TRUE;
}

XBOOL XIsMacInWin(char macChar)
{
char        ansiChar = XTranslateMacToWin(macChar);

    return (XTranslateWinToMac(ansiChar) != macChar) ? (XBOOL)FALSE
                                                    : (XBOOL)TRUE;
}

char XTranslateWinToMac(char ansiChar)
{
    int iAnsiChar = (int)ansiChar;

    if (iAnsiChar < 0x80)
    {
        return ansiChar;
    }
    else
    {
    int         macChar;

        macChar = 0x80;
        while (--macChar >= 0)
        {
            if ((char)macToWinTable[macChar] == ansiChar)
            {
                return (char)(macChar + 0x80);
            }
        }
        return (char)0xF0;  // apple character
    }
}

char XTranslateMacToWin(char macChar)
{
    int iMacChar = (int)macChar;

    if (iMacChar < 0x80)
    {
        return macChar;
    }
    else
    {
        return (char)macToWinTable[macChar - 0x80];
    }
}
#endif  // USE_FULL_RMF_SUPPORT

// EOF of X_API.c
