/******************************************************************************
**
**	"GenCache.h"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	© Copyright 2000 Beatnik, Inc, All Rights Reserved.
**	Written by Steve Hales, Mark Deggeler, Andrew Rostaing
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
**
**	Overview
**	This contains the header for code to  manage the mixer's sample cache.
**
**
**	Modification History:
**
**	2000.04.21 AER	File Created
**
******************************************************************************/


#ifndef G_CACHE
#define G_CACHE

#ifndef __X_API__
	#include "X_API.h"
#endif

#include "GenPriv.h"

#ifdef __cplusplus
	extern "C" {
#endif

// Prototypes (Documentation in .c file)
GM_SampleCacheEntry * GMCache_BuildSampleCacheEntry(GM_Mixer * pMixer,
													const XSampleID theID,
													const XBankToken bankToken,
													const XPTR useThisSnd,
													OPErr * pErr);
OPErr GMCache_IncrCacheEntryRef(const GM_Mixer * pMixer,
								GM_SampleCacheEntry * pCache);
OPErr GMCache_DecrCacheEntryRef(GM_Mixer * pMixer,
								GM_SampleCacheEntry * pCache);
OPErr GMCache_ClearSampleCache(GM_Mixer * pMixer);



XBOOL GMCache_IsIDInCache(const GM_Mixer * pMixer,
						  const XSampleID theID,
						  const XBankToken bankToken);
GM_SampleCacheEntry * GMCache_GetCachePtrFromID(const GM_Mixer * pMixer,
												const XSampleID theID,
												const XBankToken bankToken,
												OPErr * pErr);
GM_SampleCacheEntry * GMCache_GetCachePtrFromPtr(const GM_Mixer * pMixer,
												 const XPTR pSample,
												 OPErr * pErr);
XPTR GMCache_GetSamplePtr(const GM_SampleCacheEntry * pCache,
						  OPErr * pErr);


#ifdef __cplusplus
	}
#endif

#endif	//	G_CACHE





