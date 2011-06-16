/*****************************************************************************/
/*
** "GenReverbNew.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	© Copyright 1993-2000 Beatnik, Inc, All Rights Reserved.
**	Written by Chris Rogers
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
**	2/8/98		kcr			created
**	2/20/98		kcr			everything now in fixed point
**	2/24/98		kcr			deal with sample-rate changes...
**	2/24/98		igor		moved pre-init variable ScaleDelayTimes
**	2/24/98		kcr			share reverb buffers with old reverb
**	2/24/98		kcr			now clear reverb buffers every time change reverb mode
**	3/12/98		igor		Modified GetSamplingRate to use the rate function
**							GM_ConvertFromOutputQualityToRate
**	3/16/98		igor		Changed InitNewReverb to return a XBOOL for success or failure
**
**	6/5/98		Jim Nitchals RIP	1/15/62 - 6/5/98
**				I'm going to miss your irreverent humor. Your coding style and desire
**				to make things as fast as possible. Your collaboration behind this entire
**				codebase. Your absolute belief in creating the best possible relationships 
**				from honesty and integrity. Your ability to enjoy conversation. Your business 
**				savvy in understanding the big picture. Your gentleness. Your willingness 
**				to understand someone else's way of thinking. Your debates on the latest 
**				political issues. Your generosity. Your great mimicking of cartoon voices. 
**				Your friendship. - Steve Hales (igor)
**
**	7/6/98		igor		Fixed a missing type in the data object earlyReflectionFrames
**							Fixed a sign problem warning with Get44100_SRRatio
**	8/10/98		igor		Moved #if USE_NEW_EFFECTS down below X_API.h
**	1/13/99					Removed DebugStr
**	7/13/99		Renamed HAE to BAE
**	10/30/99	Added support for Q_16K
**	2/4/2000	Changed copyright. We're Y2K compliant!
*/
/*****************************************************************************/

#include "GenSnd.h"
#include "GenPriv.h"
#include "BAE_API.h"
#include "X_API.h"

#if USE_NEW_EFFECTS		// conditionally compile this file

#define INPUTSHIFT	10 /*9*/

// if COEFF_SHIFT is changed, then a number of constant parameters must be recalculated!
#define COEFF_SHIFT			16
#define	COEFF_MULTIPLY		(1L << 	COEFF_SHIFT)

// if COMB_FILTER_SHIFT is changed, must recreate inverseFeedbackTable
#define COMB_FILTER_SHIFT		16
#define	COMB_FILTER_MULTIPLY	(1L << 	COMB_FILTER_SHIFT)
#define COMB_FILTER_ROUND		(COMB_FILTER_MULTIPLY >> 1)





// based on 0.7 feedback
#define kDiffusionFeedback	 	45875L
#define kDiffusionDelayGain		33423L

// based on 0.4 feedback
#define kStereoizerFeedbackL	26214L
#define kStereoizerDelayGainL	55050L
#define kStereoizerFeedbackR	(-26214L)
#define kStereoizerDelayGainR	55050L




#if X_PLATFORM == X_MACINTOSH_9
#if 0
	#define Assert_(test)	if (!(test)) Debugger();
#else
	#define Assert_(test);
#endif
#else
	#define Assert_(test);
#endif


NewReverbParams		gNewReverbParams;

//++------------------------------------------------------------------------------
//	GetNewReverbParams()
//
//++------------------------------------------------------------------------------
NewReverbParams* GetNewReverbParams()
{
	return &gNewReverbParams;
}



//static INT32 lopassKList[6];

//++------------------------------------------------------------------------------
//	InitNewReverb()
//
//++------------------------------------------------------------------------------
XBOOL InitNewReverb()
{
	int i;
	NewReverbParams* params = GetNewReverbParams();
	
#if 0	// code to generate lopass filter coefficent for different sampling rates...

	FILE *fp = fopen("lopassK.out", "w");
	
	for(i = 0; i < 6; i++)
	{
#define _PI 	3.14159265359
		int srList[] = {8000, 11000, 22000, 24000, 44000, 48000};
		
		float filterValue = 0.2;
		float freq = filterValue  *  22000.0;
		float srate = srList[i];
		
		INT32 lopassK = (1.0 - exp(-2*_PI * freq / srate) ) * COEFF_MULTIPLY;
		
		fprintf(fp, "%d\n", lopassK);
	}
	
	fclose(fp);
	ExitToShell();
#endif

	// memory allocation
	params->mIsInitialized = FALSE;

	// OK, this is a little bit gross, but we're sharing memory from the old reverb buffer
	// in order to reduce our RAM footprint (especially for WebTV).  Therefore, we won't
	// actually allocate our own buffers for the 6 comb filters and the early reflections
	// buffer which happen to perfectly fit into the old buffer ;-)
#if 0
	long kMaxBytes = sizeof(INT32) * kCombBufferFrameSize;
	
	// allocate the comb filter delay line memory
	for(i = 0; i < kNumberOfCombFilters; i++)
	{
		params->mReverbBuffer[i] = (INT32*)XNewPtr(kMaxBytes );
		if (params->mReverbBuffer[i] == NULL)
		{
			ShutdownNewReverb();
			return FALSE;
		}
	}

	params->mEarlyReflectionBuffer = (INT32*)XNewPtr(sizeof(INT32) * kEarlyReflectionBufferFrameSize);
	if (params->mEarlyReflectionBuffer == NULL)
	{
		ShutdownNewReverb();
		return FALSE;
	}
#else	// share the old buffer

	Assert_(REVERB_BUFFER_SIZE*2 == (kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) );
	
	if(!MusicGlobals->reverbBuffer
			|| (REVERB_BUFFER_SIZE*2 != (kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize))  )
	{
		// can't share the memory because somebody changed the buffer sizes!! New reverb cannot
		// be initialized...
		params->mIsInitialized = FALSE;
		return FALSE;
	}

	for(i = 0; i < kNumberOfCombFilters; i++)
	{
		params->mReverbBuffer[i] = MusicGlobals->reverbBuffer + i*kCombBufferFrameSize;
	}

	params->mEarlyReflectionBuffer = MusicGlobals->reverbBuffer + REVERB_BUFFER_SIZE*2 - kEarlyReflectionBufferFrameSize;
#endif

	// allocate the diffusion delay line memory
	for(i = 0; i < kNumberOfDiffusionStages; i++)
	{
		params->mDiffusionBuffer[i] = (INT32*)XNewPtr(sizeof(INT32) * kDiffusionBufferFrameSize);		
		if (params->mDiffusionBuffer[i] == NULL)
		{
			ShutdownNewReverb();
			return FALSE;
		}
	}

	params->mStereoizerBufferL = (INT32*)XNewPtr(sizeof(INT32) * kStereoizerBufferFrameSize);
	if (params->mStereoizerBufferL == NULL)
	{
		ShutdownNewReverb();
		return FALSE;
	}
	params->mStereoizerBufferR = (INT32*)XNewPtr(sizeof(INT32) * kStereoizerBufferFrameSize);
	if (params->mStereoizerBufferR == NULL)
	{
		ShutdownNewReverb();
		return FALSE;
	}


	params->mRoomChoice = 1;
	params->mRoomSize = 70 /*51*/;				// 0.4 ~ 51/128;
	params->mMaxRegen = 120 /*77*/;				// 0.6 ~ 77/128
	params->mDiffusedBalance = 0;				// 0-127	0 - 2x


	params->mReverbType = -1; // force parameter recalc


	CheckReverbType();

	GenerateDelayTimes();
	GenerateFeedbackValues();
	SetupDiffusion();
	SetupStereoizer();
	SetupEarlyReflections();

	params->mIsInitialized = TRUE;
	params->mSampleRate = MusicGlobals->outputRate;
	MusicGlobals->reverbBufferSize = 	(kNumberOfDiffusionStages * kDiffusionBufferFrameSize * sizeof(INT32)) +
										(sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
										((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * sizeof(INT32));
	return TRUE;
}

//++------------------------------------------------------------------------------
//	ShutdownNewReverb()
//
//++------------------------------------------------------------------------------
void ShutdownNewReverb()
{
	int i;
	
	NewReverbParams* params = GetNewReverbParams();
	
	params->mIsInitialized = FALSE;		// do this before deallocating stuff!!

#if 0	// we're sharing the comb filter and early reflection buffer with the old reverb
	// deallocate the comb filter buffers
	for(i = 0; i < kNumberOfCombFilters; i++)
	{
		XDisposePtr(params->mReverbBuffer[i] );
		params->mReverbBuffer[i] = NULL;
	}

	// deallocate early reflection buffer
	XDisposePtr(params->mEarlyReflectionBuffer);
	params->mEarlyReflectionBuffer = NULL;
#endif




	// deallocate the diffusion buffers
	for(i = 0; i < kNumberOfDiffusionStages; i++)
	{
		XDisposePtr(params->mDiffusionBuffer[i]);
		params->mDiffusionBuffer[i] = NULL;
	}

	// deallocate stereoizer buffers
	XDisposePtr(params->mStereoizerBufferL);
	params->mStereoizerBufferL = NULL;
	XDisposePtr(params->mStereoizerBufferR);
	params->mStereoizerBufferR = NULL;
	
}


//++------------------------------------------------------------------------------
//	CheckReverbType()
//
//		Should be called 
//++------------------------------------------------------------------------------
XBOOL CheckReverbType()
{
	XBOOL changed = FALSE;
	NewReverbParams* params = GetNewReverbParams();
	int i;
	
	if (params->mIsInitialized)
	{
		if(params->mReverbType != MusicGlobals->reverbUnitType)
		{
			params->mIsInitialized = FALSE;	// set to false to stop playback
			changed = TRUE;
			params->mReverbType = MusicGlobals->reverbUnitType;

			params->mDiffusedBalance = 64;	// default to reasonable diffused sound

			switch(MusicGlobals->reverbUnitType)
			{
				case REVERB_TYPE_1:		// no reverb
					break;
				case REVERB_TYPE_8:		// Early reflections
					params->mDiffusedBalance = 0;	// just early reflections
					break;
				case REVERB_TYPE_9:		// Basement
					params->mMaxRegen = 120;
					params->mRoomSize = 20;
					break;
				case REVERB_TYPE_10:	// Banquet hall
					params->mMaxRegen = 77;
					params->mRoomSize = 51;
					break;
				case REVERB_TYPE_11:	// Catacombs
					params->mMaxRegen = 120;
					params->mRoomSize = 80;
					break;
			}
			
			// clear out shared buffers, when switching reverb modes...
			for(i = 0; i < kNumberOfCombFilters; i++)
			{
				XSetMemory(params->mReverbBuffer[i], sizeof(INT32)*kCombBufferFrameSize, 0);
			}
			
			XSetMemory(params->mEarlyReflectionBuffer, sizeof(INT32)*kEarlyReflectionBufferFrameSize, 0);
			params->mIsInitialized = TRUE;	// enable runtime again
		}
	}	
	return changed;
}


//++------------------------------------------------------------------------------
//	GetSR_44100Ratio()
//
//++------------------------------------------------------------------------------
UINT32 GetSR_44100Ratio()
{
	UINT32 currentSR = GetSamplingRate();
	
	return currentSR / 44100UL;
}

//++------------------------------------------------------------------------------
//	Get44100_SRRatio()
//
//++------------------------------------------------------------------------------
UINT32 Get44100_SRRatio()
{
	UINT32 currentSR = GetSamplingRate();
	
	return (44100UL << 16L) / (currentSR >> 16L);
}


//++------------------------------------------------------------------------------
//	GetSamplingRate()
//
//		fixed point 16.16
//++------------------------------------------------------------------------------
UINT32 GetSamplingRate()
{
	return GM_ConvertFromOutputRateToRate(MusicGlobals->outputRate) * XFIXED_1;
}



/*
 	The below table represents 16 discrete value from 0.0 <= maxRegen <= 15/16
 	(maxRegen = i/16, where i is the index in the list )
 	
 	The value is COEFF_MULTIPLY * log10(0.7 + 0.3 * (float(i) / 16.0) ) / (-3.0) 	
 */
 
static INT32 regenList[] =
{
	3383,
	3133,
	2888,
	2650,
	2418,
	2191,
	1969,
	1753,
	1541,
	1334,
	1132,
	933,
	739,
	549,
	362,
	179
};

//++------------------------------------------------------------------------------
//	ScaleDelayTimes()
//
//++------------------------------------------------------------------------------
void ScaleDelayTimes()
{
	NewReverbParams* params = GetNewReverbParams();
	int i;
	INT32		T60;
	INT32		desiredMinFrames;
	INT32		kMinDelayFrames;
	INT32		currentMinFrames;
	INT32		delayScale;
	INT32		delayFrames;
	INT32		log10maxG;
	UINT32		srate = GetSamplingRate() >> 16;
	INT32		maxDelayFrame;

	// mRoomSize 0-127
	T60 = params->mRoomSize * 4;	// range of 4 seconds
	
	//maxG = 0.7 + 0.3 * params->mMaxRegen;
	//if(maxG > 0.97) maxG = 0.97;
	
	// mMaxRegen should be between 0-127, shift over three to leave 4 bit index into table
	log10maxG = regenList[ (params->mMaxRegen >> 3 ) & 0xf ];		
	desiredMinFrames = srate * ((log10maxG * T60) >> 7);    /*log10(maxG) * T60 / -3.0 */
	
	// ok, at this point, desiredMinFrames if FIXED 16.16
	kMinDelayFrames = 485L * GetSR_44100Ratio();  /*485L << 16*/	/* 11ms */
	if(desiredMinFrames < kMinDelayFrames) desiredMinFrames = kMinDelayFrames;
	
	
	currentMinFrames = params->mUnscaledDelayFrames[0];
	
	delayScale = desiredMinFrames / currentMinFrames;

	// clip scale factor to max delay time
	maxDelayFrame = params->mUnscaledDelayFrames[kNumberOfCombFilters-1];
	
	if( ((delayScale * maxDelayFrame) >> 16) >= kCombBufferFrameSize )
	{
		delayScale = (kCombBufferFrameSize << 16) / maxDelayFrame;
		delayScale = (delayScale * 99) / 100;	// times 0.99
	}
	
	
	for(i = 0; i < kNumberOfCombFilters; i++)
	{
		// do the scaling
		params->mDelayFrames[i] = (delayScale * params->mUnscaledDelayFrames[i]) >> 16;
		delayFrames = params->mDelayFrames[i];
		
		// setup read and write indices
		params->mReadIndex[i] = kCombBufferFrameSize - delayFrames;
		params->mWriteIndex[i] = 0;

		Assert_(delayFrames < kCombBufferFrameSize);
		Assert_(delayFrames > 1);
	}	
}

/* the following table corresponds to feedback values (for the comb filter) from 0.5 -> 1.0
   in increments of 0.05. The actual value in the table is a ratio of delay frame size
   divided by the room size (0-127) multiplied by (1L << 16)
 */
 
static INT32 inverseFeedbackTable[] =	/* 100 entries */
{
	9062688, 8932591, 8803775, 8676216, 8549889, 8424772, 8300840, 8178072, 8056446, 7935941, 
	7816537, 7698213, 7580950, 7464730, 7349533, 7235343, 7122142, 7009912, 6898637, 6788302, 
	6678889, 6570385, 6462774, 6356041, 6250172, 6145154, 6040973, 5937615, 5835068, 5733319, 
	5632355, 5532165, 5432738, 5334060, 5236122, 5138912, 5042419, 4946633, 4851544, 4757142, 
	4663416, 4570357, 4477956, 4386204, 4295090, 4204608, 4114747, 4025499, 3936857, 3848812, 
	3761355, 3674480, 3588178, 3502442, 3417264, 3332638, 3248556, 3165012, 3081997, 2999507, 
	2917534, 2836071, 2755113, 2674653, 2594685, 2515204, 2436202, 2357675, 2279617, 2202022, 
	2124885, 2048200, 1971963, 1896167, 1820809, 1745882, 1671382, 1597304, 1523644, 1450396, 
	1377556, 1305120, 1233083, 1161441, 1090189, 1019323, 948839, 878733, 809002, 739640, 
	670644, 602010, 533735, 465814, 398244, 331022, 264144, 197606, 131405, 65537
};



//++------------------------------------------------------------------------------
//	GenerateFeedbackValues()
//
//++------------------------------------------------------------------------------
void GenerateFeedbackValues()
{
	NewReverbParams* params = GetNewReverbParams();
	int i;


#if 0	
	float T60 = (float(params->mRoomSize) / 128.0) * 4;	// range of 4 seconds
	
	for(i = 0; i < kNumberOfCombFilters; i++)
	{
		long frames = params->mDelayFrames[i];
		
		
		float D = (float)(params->mDelayFrames[i]) / 44100.0;
		float g = pow(10, -3*D / T60);
		
		if(g > 0.97) g = 0.97;

		g = -g;	// g is always negative to avoid "limit cycles" in fixed-point comb filter regen

		params->mFeedbackList[i] = g;
	}
	
#else
	if(params->mRoomSize == 0) params->mRoomSize = 1;	// avoid divide by zero

	for(i = 0; i < kNumberOfCombFilters; i++)
	{
		long frames = (params->mDelayFrames[i] * Get44100_SRRatio() ) >> 16;
		INT32 fakeRatio = (frames << COMB_FILTER_SHIFT) / params->mRoomSize;
		int k;
		INT32 g, kOneHalf;
		
		for(k = 0; k < 100; k++)
		{
			if(inverseFeedbackTable[k] <= fakeRatio)
				break;
		
		}
		
		if(k == 100) k = 99;
		
		kOneHalf = (1L << (COMB_FILTER_SHIFT-1) );
		
		g = kOneHalf + ((kOneHalf * k) / 100);
		
		// g is always negative to avoid "limit cycles" in fixed-point comb filter regen
		g = -g;
		
		params->mFeedbackList[i] = g;
	}

#endif

#if 0
	// code for generating the inverse table...
	FILE *fp = fopen("regenParams.out", "w");

	for(i = 0; i < 100; i++)
	{
		static count = 0;
		
		//INT32 fakeRatio = (frames << 16) / params->mRoomSize;
		double g = 0.5 + (0.01 * i) * 0.5;
		
		const double k = -3 * 32 / 44100.0;
		
		double fakeRatio = log10(g) / k;
		INT32 intFakeRatio = fakeRatio * (1L << COMB_FILTER_SHIFT);
		
		
		fprintf(fp, "%d, ", intFakeRatio);

		if(++count == 10)
		{
			count = 0;
			fprintf(fp, "\n");
		}
		
	}

	fclose(fp);
	
	ExitToShell();
	
#endif
}

static long delay6tapList[6][6] =
{
	{
		1259,
		1459,
		1621,
		1831,
		1993,
		2269
	},
	{
		1279,
		1433,
		1669,
		1867,
		2113,
		2311
	},
	{
		1381,
		1543,
		1669,
		1907,
		2017,
		2203
	},
	{
		1249,
		1451,
		1747,
		1901,
		2017,
		2161
	},
	{
		1283,
		1471,
		1777,
		1907,
		1993,
		2203
	},
	{
		1381,
		1531,
		1753,
		1789,
		1979,
		2309
	}
};

//++------------------------------------------------------------------------------
//	GenerateDelayTimes()
//
//++------------------------------------------------------------------------------
void GenerateDelayTimes()
{
	int i;
	long	*delayFrameList;
	NewReverbParams* params = GetNewReverbParams();
	int index = params->mRoomChoice;
	
	if(index > 5) index = 5;

	
	delayFrameList = delay6tapList[index];


	for(i = 0; i < kNumberOfCombFilters; i++)
	{		
		params->mUnscaledDelayFrames[i] = (delayFrameList[i] * GetSR_44100Ratio() ) >> 16;
	}
	
	ScaleDelayTimes();
}

static INT32 diffusionFrameList[] =
{
	105,
	176,
	246
};


//++------------------------------------------------------------------------------
//	SetupDiffusion()
//
//++------------------------------------------------------------------------------
void SetupDiffusion()
{
	NewReverbParams* params = GetNewReverbParams();
	int i;

	Assert_(kNumberOfDiffusionStages == 3);	// diffusionFrameList has three elements currently
	
	for(i = 0; i < kNumberOfDiffusionStages; i++)
	{
		long delayFrames = (diffusionFrameList[i] * GetSR_44100Ratio() ) >> 16;
		
		params->mDiffReadIndex[i] = kDiffusionBufferFrameSize - delayFrames;
		params->mDiffWriteIndex[i] = 0;
	}

}

//++------------------------------------------------------------------------------
//	SetupStereoizer()
//
//++------------------------------------------------------------------------------
void SetupStereoizer()
{
	NewReverbParams* params = GetNewReverbParams();
	long delayFrames;
	
	// generated lopass filter coeffs for different sample rates
	static INT32 lopassKList[] = {63467, 60227, 46883, 44824, 30573, 28693};
	
	int i;


	// output filter
	switch(MusicGlobals->outputRate)
	{
		case Q_RATE_8K:
			i = 0;
			break;
		case Q_RATE_11K_TERP_22K:
		case Q_RATE_11K:
		case Q_RATE_16K:
			i = 1;
			break;
		case Q_RATE_22K_TERP_44K:
		case Q_RATE_22K:
			i = 2;
			break;
		case Q_RATE_24K:
			i = 3;
			break;
		case Q_RATE_44K:
			i = 4;
			break;
		case Q_RATE_48K:
			i = 5;
			break;
		default:
			i = 4;
			break;
	}
	
	Assert_(i >= 0 && i < 6);
	params->mLopassK = lopassKList[i]  /*30523*/;


	params->mFilterMemory = 0;

	

	delayFrames = (307 * GetSR_44100Ratio() ) >> 16;
	params->mStereoReadIndex = kStereoizerBufferFrameSize - delayFrames;
	params->mStereoWriteIndex = 0;
	
}

// early reflection stuff

static INT32 earlyReflectionFrames[] =
 {0x00000f7b, 0x00000861, 0x00000de2, 0x00000ac2, 0x00000b74, 0x00001420, /*0x0000089d*/ 0x00000550};
 
static INT32 earlyReflectionsGains[] =
{
	50918,		// 	0.77695906162261963
	127900,		// 	1.9516065120697021
	59954,		//	0.91482698917388916
	87903,		// 	1.3413060903549194
	80038,		// 	1.2212871313095093
	34353,		// 	0.52418613433837891
	49152		//	0.75	 sent to diffused reverb
};

//++------------------------------------------------------------------------------
//	SetupEarlyReflections()
//
//++------------------------------------------------------------------------------
void SetupEarlyReflections()
{
	NewReverbParams* params = GetNewReverbParams();
	int i;
	
	
	params->mReflectionWriteIndex = 0;
		
	for(i = 0; i < kNumberOfEarlyReflections; i++)
	{
		INT32 frames = (earlyReflectionFrames[i] * GetSR_44100Ratio() ) >> 16;
		params->mEarlyReflectionGain[i] = earlyReflectionsGains[i];
		params->mReflectionReadIndex[i] = kEarlyReflectionBufferFrameSize - frames;
	}
}


#define CIRCULAR_INCREMENT(index_, mask_)	\
	index_ = (index_ + 1) & mask_;

//++------------------------------------------------------------------------------
//	RunNewReverb()
//
//++------------------------------------------------------------------------------
void RunNewReverb(INT32 *sourceP, INT32 *destP, int nSampleFrames)
{
	NewReverbParams* params = GetNewReverbParams();

	// get local state variables from global struct (for efficiency)
	INT32 *delayBuffer1 = params->mReverbBuffer[0];
	INT32 *delayBuffer2 = params->mReverbBuffer[1];
	INT32 *delayBuffer3 = params->mReverbBuffer[2];
	INT32 *delayBuffer4 = params->mReverbBuffer[3];
	INT32 *delayBuffer5 = params->mReverbBuffer[4];
	INT32 *delayBuffer6 = params->mReverbBuffer[5];

	INT32	intFeedback1 = params->mFeedbackList[0];
	INT32	intFeedback2 = params->mFeedbackList[1];
	INT32	intFeedback3 = params->mFeedbackList[2];
	INT32	intFeedback4 = params->mFeedbackList[3];
	INT32	intFeedback5 = params->mFeedbackList[4];
	INT32	intFeedback6 = params->mFeedbackList[5];
	
	int		readIndex1 = params->mReadIndex[0];
	int		readIndex2 = params->mReadIndex[1];
	int		readIndex3 = params->mReadIndex[2];
	int		readIndex4 = params->mReadIndex[3];
	int		readIndex5 = params->mReadIndex[4];
	int		readIndex6 = params->mReadIndex[5];
	
	int		writeIndex1 = params->mWriteIndex[0];
	int		writeIndex2 = params->mWriteIndex[1];
	int		writeIndex3 = params->mWriteIndex[2];
	int		writeIndex4 = params->mWriteIndex[3];
	int		writeIndex5 = params->mWriteIndex[4];
	int		writeIndex6 = params->mWriteIndex[5];
	
	// diffusion parameters	
	INT32	*diffusionBuffer1 = params->mDiffusionBuffer[0];
	INT32	*diffusionBuffer2 = params->mDiffusionBuffer[1];
	INT32	*diffusionBuffer3 = params->mDiffusionBuffer[2];
	
	int		diffReadIndex1 = params->mDiffReadIndex[0];
	int		diffReadIndex2 = params->mDiffReadIndex[1];
	int		diffReadIndex3 = params->mDiffReadIndex[2];

	int		diffWriteIndex1 = params->mDiffWriteIndex[0];
	int		diffWriteIndex2 = params->mDiffWriteIndex[1];
	int		diffWriteIndex3 = params->mDiffWriteIndex[2];
	
	

	INT32	*stereoizerBufferL = params->mStereoizerBufferL;
	INT32	*stereoizerBufferR = params->mStereoizerBufferR;

	int 	stereoReadIndex = params->mStereoReadIndex;
	int 	stereoWriteIndex = params->mStereoWriteIndex;



	// early reflection stuff
	INT32 earlyReflectionGain1 = params->mEarlyReflectionGain[0];
	INT32 earlyReflectionGain2 = params->mEarlyReflectionGain[1];
	INT32 earlyReflectionGain3 = params->mEarlyReflectionGain[2];
	INT32 earlyReflectionGain4 = params->mEarlyReflectionGain[3];
	INT32 earlyReflectionGain5 = params->mEarlyReflectionGain[4];
	INT32 earlyReflectionGain6 = params->mEarlyReflectionGain[5];
	
	// 0-127 corresponds to 0x to 2x the mEarlyReflectionGain[6] value
	INT32 preDelayGain = (params->mEarlyReflectionGain[6] * params->mDiffusedBalance) >> 6;
	
	int		reflectionReadIndex1 = params->mReflectionReadIndex[0];
	int		reflectionReadIndex2 = params->mReflectionReadIndex[1];
	int		reflectionReadIndex3 = params->mReflectionReadIndex[2];
	int		reflectionReadIndex4 = params->mReflectionReadIndex[3];
	int		reflectionReadIndex5 = params->mReflectionReadIndex[4];
	int		reflectionReadIndex6 = params->mReflectionReadIndex[5];
	int		reflectionReadIndex7 = params->mReflectionReadIndex[6];
	
	int		reflectionWriteIndex = params->mReflectionWriteIndex;

	INT32	*earlyReflectionBuffer = params->mEarlyReflectionBuffer;


	// filter stuff
	INT32 lopassK = params->mLopassK;
	INT32 filterMemory = params->mFilterMemory;

	int framesToProcess = nSampleFrames;


	if(!params->mIsInitialized) return;	// we're not properly initialized for processing...

	if(CheckReverbType() || params->mSampleRate != MusicGlobals->outputRate )
	{
		// sample rate has changed
		params->mSampleRate = MusicGlobals->outputRate;
		
		GenerateDelayTimes();
		GenerateFeedbackValues();
		SetupDiffusion();
		SetupStereoizer();
		SetupEarlyReflections();
		
		return;
	}
	
	while(framesToProcess-- > 0)
	{
		INT32 reverbOutL, reverbOutR;
		INT32 diffOut1, diffOut2, diffOut3;
		INT32 wet, combOutput, temp, combInput, diffInput, filterOutput, stereoInput;

		INT32 input = *sourceP++ >> (INPUTSHIFT + 1);


		// comb filter bank
		INT32 tap1 = (delayBuffer1[readIndex1] * intFeedback1) >> COMB_FILTER_SHIFT;
		INT32 tap2 = (delayBuffer2[readIndex2] * intFeedback2) >> COMB_FILTER_SHIFT;
		INT32 tap3 = (delayBuffer3[readIndex3] * intFeedback3) >> COMB_FILTER_SHIFT;
		INT32 tap4 = (delayBuffer4[readIndex4] * intFeedback4) >> COMB_FILTER_SHIFT;
		INT32 tap5 = (delayBuffer5[readIndex5] * intFeedback5) >> COMB_FILTER_SHIFT;
		INT32 tap6 = (delayBuffer6[readIndex6] * intFeedback6) >> COMB_FILTER_SHIFT;


		// early reflections
		INT32 refl1 = (earlyReflectionBuffer[reflectionReadIndex1] * earlyReflectionGain1) >> COEFF_SHIFT;
		INT32 refl2 = (earlyReflectionBuffer[reflectionReadIndex2] * earlyReflectionGain2) >> COEFF_SHIFT;
		INT32 refl3 = (earlyReflectionBuffer[reflectionReadIndex3] * earlyReflectionGain3) >> COEFF_SHIFT;
		INT32 refl4 = (earlyReflectionBuffer[reflectionReadIndex4] * earlyReflectionGain4) >> COEFF_SHIFT;
		INT32 refl5 = (earlyReflectionBuffer[reflectionReadIndex5] * earlyReflectionGain5) >> COEFF_SHIFT;
		INT32 refl6 = (earlyReflectionBuffer[reflectionReadIndex6] * earlyReflectionGain6) >> COEFF_SHIFT;
		INT32 preDelay = (earlyReflectionBuffer[reflectionReadIndex7] * preDelayGain) >> COEFF_SHIFT;

		INT32 reflSum = refl1 + refl2 + refl3 + refl4 + refl5 + refl6;

		earlyReflectionBuffer[reflectionWriteIndex] = input;

		// increment reflection indices
		CIRCULAR_INCREMENT( reflectionReadIndex1, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionReadIndex2, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionReadIndex3, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionReadIndex4, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionReadIndex5, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionReadIndex6, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionReadIndex7, kEarlyReflectionBufferMask);
		CIRCULAR_INCREMENT( reflectionWriteIndex, kEarlyReflectionBufferMask);

		combInput = preDelay;

		
		// feed delay outputs back into delay inputs
		delayBuffer1[writeIndex1] = combInput + tap1;
		delayBuffer2[writeIndex2] = combInput + tap2;
		delayBuffer3[writeIndex3] = combInput + tap3;
		delayBuffer4[writeIndex4] = combInput + tap4;
		delayBuffer5[writeIndex5] = combInput + tap5;
		delayBuffer6[writeIndex6] = combInput + tap6;
		
		// increment read indices
		CIRCULAR_INCREMENT( readIndex1, kCombBufferMask);
		CIRCULAR_INCREMENT( readIndex2, kCombBufferMask);
		CIRCULAR_INCREMENT( readIndex3, kCombBufferMask);
		CIRCULAR_INCREMENT( readIndex4, kCombBufferMask);
		CIRCULAR_INCREMENT( readIndex5, kCombBufferMask);
		CIRCULAR_INCREMENT( readIndex6, kCombBufferMask);
		
		// increment write indices
		CIRCULAR_INCREMENT( writeIndex1, kCombBufferMask);
		CIRCULAR_INCREMENT( writeIndex2, kCombBufferMask);
		CIRCULAR_INCREMENT( writeIndex3, kCombBufferMask);
		CIRCULAR_INCREMENT( writeIndex4, kCombBufferMask);
		CIRCULAR_INCREMENT( writeIndex5, kCombBufferMask);
		CIRCULAR_INCREMENT( writeIndex6, kCombBufferMask);

		combOutput = (tap1 + tap2 + tap3 + tap4 + tap5 + tap6) >> 1;

		diffInput = combOutput;

		
		// diffusion 1
		temp =  diffusionBuffer1[diffReadIndex1];
		diffusionBuffer1[diffWriteIndex1] = diffInput + ((temp * kDiffusionFeedback) >> COEFF_SHIFT);
		diffOut1 = (-diffInput * kDiffusionFeedback + temp * kDiffusionDelayGain) >> COEFF_SHIFT;
		
		// diffusion 2
		temp =  diffusionBuffer2[diffReadIndex2];
		diffusionBuffer2[diffWriteIndex2] = diffOut1 + ((temp * kDiffusionFeedback) >> COEFF_SHIFT);
		diffOut2 = (-diffOut1 * kDiffusionFeedback + temp * kDiffusionDelayGain) >> COEFF_SHIFT;
		
		// diffusion 3
		temp =  diffusionBuffer3[diffReadIndex3];
		diffusionBuffer3[diffWriteIndex3] = diffOut2 + ((temp * kDiffusionFeedback) >> COEFF_SHIFT);
		diffOut3 = (-diffOut2 * kDiffusionFeedback + temp * kDiffusionDelayGain) >> COEFF_SHIFT;
		
		
		// update diffusion indices
		CIRCULAR_INCREMENT( diffReadIndex1, kDiffusionBufferMask);
		CIRCULAR_INCREMENT( diffReadIndex2, kDiffusionBufferMask);
		CIRCULAR_INCREMENT( diffReadIndex3, kDiffusionBufferMask);
		
		CIRCULAR_INCREMENT( diffWriteIndex1, kDiffusionBufferMask);
		CIRCULAR_INCREMENT( diffWriteIndex2, kDiffusionBufferMask);
		CIRCULAR_INCREMENT( diffWriteIndex3, kDiffusionBufferMask);

		wet =  reflSum + diffOut3;	// reverb level




		// now run through its filter
		filterOutput = (lopassK * filterMemory) >> COEFF_SHIFT;
		filterMemory = wet  + filterMemory - filterOutput;



		stereoInput = filterOutput;


		// stereoizer stage
		temp =  stereoizerBufferL[stereoReadIndex];
		stereoizerBufferL[stereoWriteIndex] = stereoInput + ((temp * kStereoizerFeedbackL) >> COEFF_SHIFT);
		reverbOutL = (-stereoInput * kStereoizerFeedbackL + temp * kStereoizerDelayGainL) >> COEFF_SHIFT;
		
		temp =  stereoizerBufferR[stereoReadIndex];
		stereoizerBufferR[stereoWriteIndex] = stereoInput + ((temp * kStereoizerFeedbackR) >> COEFF_SHIFT);
		reverbOutR = (-stereoInput * kStereoizerFeedbackR + temp * kStereoizerDelayGainR) >> COEFF_SHIFT;

		CIRCULAR_INCREMENT( stereoReadIndex, kStereoizerBufferMask);
		CIRCULAR_INCREMENT( stereoWriteIndex, kStereoizerBufferMask);

		// ADD in reverb to output buffer
		*destP++ += reverbOutL << (INPUTSHIFT);
		*destP++ += reverbOutR << (INPUTSHIFT);
	}


	// put local copies of indices back into global struct
	params->mReadIndex[0] = readIndex1;
	params->mReadIndex[1] = readIndex2;
	params->mReadIndex[2] = readIndex3;
	params->mReadIndex[3] = readIndex4;
	params->mReadIndex[4] = readIndex5;
	params->mReadIndex[5] = readIndex6;
	
	params->mWriteIndex[0] = writeIndex1;
	params->mWriteIndex[1] = writeIndex2;
	params->mWriteIndex[2] = writeIndex3;
	params->mWriteIndex[3] = writeIndex4;
	params->mWriteIndex[4] = writeIndex5;
	params->mWriteIndex[5] = writeIndex6;

	params->mDiffReadIndex[0] = diffReadIndex1;
	params->mDiffReadIndex[1] = diffReadIndex2;
	params->mDiffReadIndex[2] = diffReadIndex3;

	params->mDiffWriteIndex[0] = diffWriteIndex1;
	params->mDiffWriteIndex[1] = diffWriteIndex2;
	params->mDiffWriteIndex[2] = diffWriteIndex3;

	params->mStereoReadIndex = stereoReadIndex;
	params->mStereoWriteIndex = stereoWriteIndex;
	
	
	
	
	params->mReflectionReadIndex[0] = reflectionReadIndex1;
	params->mReflectionReadIndex[1] = reflectionReadIndex2;
	params->mReflectionReadIndex[2] = reflectionReadIndex3;
	params->mReflectionReadIndex[3] = reflectionReadIndex4;
	params->mReflectionReadIndex[4] = reflectionReadIndex5;
	params->mReflectionReadIndex[5] = reflectionReadIndex6;
	params->mReflectionReadIndex[6] = reflectionReadIndex7;
	
	params->mReflectionWriteIndex = reflectionWriteIndex;
	
	
	params->mFilterMemory = filterMemory;
}

#endif // USE_NEW_EFFECTS
