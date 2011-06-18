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
** "GenChorus.c"
**
**  Generalized Music Synthesis package. Part of SoundMusicSys.
**
**  © Copyright 1993-2000 Beatnik, Inc, All Rights Reserved.
**  Written by Chris Rogers
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
**  General purpose Music Synthesis software, C-only implementation 
**  (no assembly language optimizations made)
**
** Modification History:
**
**  15jan98     kcr         created
**  20feb98     kcr         everything now in fixed-point (including param calculation)
**  24feb98     kcr         take care of sample-rate changes...
**
**  6/5/98      Jim Nitchals RIP    1/15/62 - 6/5/98
**              I'm going to miss your irreverent humor. Your coding style and desire
**              to make things as fast as possible. Your collaboration behind this entire
**              codebase. Your absolute belief in creating the best possible relationships 
**              from honesty and integrity. Your ability to enjoy conversation. Your business 
**              savvy in understanding the big picture. Your gentleness. Your willingness 
**              to understand someone else's way of thinking. Your debates on the latest 
**              political issues. Your generosity. Your great mimicking of cartoon voices. 
**              Your friendship. - Steve Hales (igor)
**
**  8/10/98     igor        Moved #if USE_NEW_EFFECTS to after include of X_API.h
**  7/13/99     Removed use of BAE_API.h
**  2/4/2000    Changed copyright. We're Y2K compliant!
*/
/*****************************************************************************/
#include "GenPriv.h"

#if USE_NEW_EFFECTS     // conditionally compile this file


#if X_PLATFORM == X_MACINTOSH_9
#define Assert_(test)   if (!(test)) Debugger();

#else
#define Assert_(test)       ;

#endif



//#undef _PI
//#define _PI       3.14159265359


ChorusParams        gChorusParams;

#define READINDEXSHIFT  8L
#define READINDEXMASK   ((1L << READINDEXSHIFT) - 1)

#define PHISHIFT    16
#define PHIMASK     ((1L << PHISHIFT) - 1)


#define kMinDelayFrames     500
#define kMaxDepthRange      1000 /*200*/

#define kModulationTableLength      200

//++------------------------------------------------------------------------------
//  GetChorusParams()
//
//++------------------------------------------------------------------------------
ChorusParams* GetChorusParams()
{
    return &gChorusParams;
}


//++------------------------------------------------------------------------------
//  InitChorus()
//
//++------------------------------------------------------------------------------
void InitChorus()
{
    ChorusParams* params = GetChorusParams();
    
    // allocate the delay line memory
    long kMaxBytes = 2 * sizeof(INT32) * kChorusBufferFrameSize;
    params->mChorusBufferL = (INT32*)XNewPtr(kMaxBytes );
    params->mChorusBufferR = (INT32*)XNewPtr(kMaxBytes );

    SetupChorusDelay();
    

    params->mRate = 64;     /* 0.5  *  (1L << 7) */
    //params->mDepth = 0.99999;
    params->mPhi = 0;
    
    params->mFeedbackGain = 10 /*10*/;      // value from 0-127

#if 0       // code for generating exp modulation table
    {
        int i;
        FILE *fp = fopen("exp.out", "w");
        
        for(i = 0; i < 200; i++)
        {
            static count = 0;
            double x;
            INT32 value;
            double halfTableLength = kModulationTableLength * 0.5;
            
            if(i < 100)
            {
                x = ((double(i) / halfTableLength) - 0.5) * 2;  // rising exponential
            }
            else
            {
                x = ((double(kModulationTableLength - i) / halfTableLength) - 0.5) * 2; // descending
            }
            
            value = pow(2, x) * (double)(1L << 15);;
            
            fprintf(fp, "%d, ", value);
            
            if(++count == 10)
            {
                count = 0;
                fprintf(fp, "\n");
            }
        }
        
        fclose(fp);
        ExitToShell();
    }
#endif


    params->mIsInitialized = TRUE;
    params->mSampleRate = MusicGlobals->outputRate;
}

//++------------------------------------------------------------------------------
//  ShutdownChorus()
//
//++------------------------------------------------------------------------------
void ShutdownChorus()
{
    ChorusParams* params = GetChorusParams();
    
    if(!params->mIsInitialized) return; // don't shutdown twice!!
    params->mIsInitialized = FALSE;     // do this before deallocating stuff!!

    // deallocate the buffer
    XDisposePtr(params->mChorusBufferL );
    XDisposePtr(params->mChorusBufferR );
}


//++------------------------------------------------------------------------------
//  SetupChorusDelay()
//
//++------------------------------------------------------------------------------
void SetupChorusDelay()
{
    ChorusParams* params = GetChorusParams();
    
    params->mSampleFramesDelay = (GetSR_44100Ratio() * kMinDelayFrames) >> 16;

    params->mWriteIndex = 0;
    
    params->mReadIndexL = (kChorusBufferFrameSize - params->mSampleFramesDelay) << READINDEXSHIFT;
    params->mReadIndexR = (kChorusBufferFrameSize - params->mSampleFramesDelay) << READINDEXSHIFT;
}




// this table doesn't have to be extremely accurate -- just used to modulate chorus delay

static INT32 expTable[] =   /* kModulationTableLength entries fixed point 16.16 */
{
    16384, 16612, 16844, 17079, 17318, 17559, 17805, 18053, 18305, 18561, 
    18820, 19082, 19349, 19619, 19893, 20171, 20452, 20738, 21027, 21321, 
    21618, 21920, 22226, 22536, 22851, 23170, 23493, 23821, 24154, 24491, 
    24833, 25180, 25531, 25888, 26249, 26615, 26987, 27364, 27746, 28133, 
    28526, 28924, 29328, 29737, 30152, 30573, 31000, 31433, 31871, 32316, 
    32768, 33225, 33689, 34159, 34636, 35119, 35610, 36107, 36611, 37122, 
    37640, 38165, 38698, 39238, 39786, 40342, 40905, 41476, 42055, 42642, 
    43237, 43841, 44453, 45073, 45702, 46340, 46987, 47643, 48308, 48983, 
    49667, 50360, 51063, 51776, 52498, 53231, 53974, 54728, 55492, 56266, 
    57052, 57848, 58656, 59475, 60305, 61147, 62000, 62866, 63743, 64633, 
    65536, 64633, 63743, 62866, 62000, 61147, 60305, 59475, 58656, 57848, 
    57052, 56266, 55492, 54728, 53974, 53231, 52498, 51776, 51063, 50360, 
    49667, 48983, 48308, 47643, 46987, 46340, 45702, 45073, 44453, 43841, 
    43237, 42642, 42055, 41476, 40905, 40342, 39786, 39238, 38698, 38165, 
    37640, 37122, 36611, 36107, 35610, 35119, 34636, 34159, 33689, 33225, 
    32768, 32316, 31871, 31433, 31000, 30573, 30152, 29737, 29328, 28924, 
    28526, 28133, 27746, 27364, 26987, 26615, 26249, 25888, 25531, 25180, 
    24833, 24491, 24154, 23821, 23493, 23170, 22851, 22536, 22226, 21920, 
    21618, 21321, 21027, 20738, 20452, 20171, 19893, 19619, 19349, 19082, 
    18820, 18561, 18305, 18053, 17805, 17559, 17318, 17079, 16844, 16612
};


//++------------------------------------------------------------------------------
//  GetChorusReadIncrement()
//
//++------------------------------------------------------------------------------
INT32 GetChorusReadIncrement(INT32 readIndex, long writeIndex, long nSampleFrames, INT32 phase)
{
    ChorusParams* params = GetChorusParams();

    INT32   phi = params->mPhi;
    //float depth = params->mDepth;
    long    sampleFramesDelay = params->mSampleFramesDelay;

    long    currentDelayFrame;
    long    desiredDelayFrame;

    INT32   ratio;
#if 0
    int     sampleFramesShift = 9;
#endif
    INT32   phiIndex = (phi * kModulationTableLength) + (phase << PHISHIFT);    // in 16.16
    INT32   b = expTable[   (phiIndex >> PHISHIFT)      % kModulationTableLength];
    INT32   c = expTable[(  (phiIndex >> PHISHIFT) + 1) % kModulationTableLength];
    INT32   expValue = (((INT32) (phiIndex & PHIMASK) * (c - b))>>PHISHIFT) + b;
    INT32   offsetFrame = (kMaxDepthRange * expValue) >> 16;
    
    // compensate for sampling rate
    offsetFrame = (GetSR_44100Ratio() * offsetFrame) >> 16;


    
    // don't let delay get below minimum required for chorusing
    if(sampleFramesDelay < kMinDelayFrames)
    {
        sampleFramesDelay = kMinDelayFrames;
    }

#if 0   
    switch(nSampleFrames)
    {
        case 96:// degenerate
        case 128:
            sampleFramesShift = 7;
            break;
        case 256:
        case 288:
            sampleFramesShift = 8;
            break;
        case 512:
            sampleFramesShift = 9;
            break;
        case 1024:
            sampleFramesShift = 10;
            break;
        case 2048:
            sampleFramesShift = 11;
            break;
        default:
            Assert_(0);
            break;
    }
#endif  
    
    
    currentDelayFrame = ((long)(kChorusBufferFrameSize + writeIndex - (readIndex >> READINDEXSHIFT) )) % kChorusBufferFrameSize;
    
    
    desiredDelayFrame = sampleFramesDelay + /*depth * */ (offsetFrame );





    ratio = (currentDelayFrame - desiredDelayFrame + nSampleFrames) << READINDEXSHIFT;

#if 0
    ratio >>= sampleFramesShift;
#else
    ratio /= nSampleFrames;
#endif
    
    return ratio;
}

//++------------------------------------------------------------------------------
//  RunChorus()
//
//++------------------------------------------------------------------------------
void RunChorus(INT32 *sourceP, INT32 *destP, int nSampleFrames)
{
#define FEEDBACKSHIFT   7

    ChorusParams* params = GetChorusParams();

    INT32   *bufferL = params->mChorusBufferL;
    INT32   *bufferR = params->mChorusBufferR;
    INT32   phi = params->mPhi;
    INT32   rate = params->mRate;
    INT32   feedbackGain = -params->mFeedbackGain;  // avoid "limit points"
    long    writeIndex = params->mWriteIndex;
    
    INT32   readIndexL = params->mReadIndexL;
    INT32   readIndexR = params->mReadIndexR;
    
    INT32   readIndexIncrL;
    INT32   readIndexIncrR;


    INT32   kReadIndexAdjust = (kChorusBufferFrameSize << READINDEXSHIFT);
    

    if(!params->mIsInitialized) return; // we're not properly initialized for processing...
    
    
    readIndexIncrL =  GetChorusReadIncrement(readIndexL, writeIndex, nSampleFrames, 0);
    readIndexIncrR =  GetChorusReadIncrement(readIndexR, writeIndex, nSampleFrames, kModulationTableLength/2);
    

    phi += (rate * nSampleFrames) >> 7;
    phi %= 65536;
    
    params->mPhi = phi;


    if(!params->mIsInitialized) return; // we're not properly initialized for processing...

    if(params->mSampleRate != MusicGlobals->outputRate)
    {
        // sample rate has changed
        params->mSampleRate = MusicGlobals->outputRate;
        
        SetupChorusDelay();
            
        return;
    }

    
    
    while(nSampleFrames-- > 0)
    {
    // get input
        INT32 inputL = *sourceP++;  // mono input
        INT32 inputR = inputL;


    // calculate sample value
        int intReadIndexL = readIndexL >> READINDEXSHIFT;
        int intReadIndex2L = (intReadIndexL + 1) % kChorusBufferFrameSize;
        INT32 bL = bufferL[intReadIndexL];
        INT32 cL = bufferL[intReadIndex2L];
        
        
        int intReadIndexR = readIndexR >> READINDEXSHIFT;
        int intReadIndex2R = (intReadIndexR + 1) % kChorusBufferFrameSize;
        INT32 bR = bufferR[intReadIndexR];
        INT32 cR = bufferR[intReadIndex2R];
        
        INT32 tapL = (((INT32) (readIndexL & READINDEXMASK) * (cL - bL))>>READINDEXSHIFT) + bL;
        INT32 tapR = (((INT32) (readIndexR & READINDEXMASK) * (cR - bR))>>READINDEXSHIFT) + bR;

    // write input plus feedback back into the delay line
        bufferL[writeIndex] = inputL + ((tapL*feedbackGain) >> FEEDBACKSHIFT) ;
        bufferR[writeIndex] = inputR + ((tapR*feedbackGain) >> FEEDBACKSHIFT) ;
        
    

    // write to output
#if 1
        *destP++ += (tapL ) ;
        *destP++ += (tapR ) ;
#else
        *destP++ = 0;
        *destP++ = 0;
#endif
    
    // increment the read head by fractional amounts    
        readIndexL += readIndexIncrL;
        readIndexR += readIndexIncrR;
        
    // wrap-around read indices 
        if(readIndexL >=  kReadIndexAdjust)
        {
            readIndexL -= kReadIndexAdjust;
        }

        if(readIndexR >=  kReadIndexAdjust)
        {
            readIndexR -= kReadIndexAdjust;
        }

    // wrap-around write pointer
        writeIndex = (writeIndex + 1) % kChorusBufferFrameSize;
    }

    // remember state
    params->mWriteIndex = writeIndex;
    params->mReadIndexL = readIndexL;
    params->mReadIndexR = readIndexR;

}


#if 0       // old mono chorus code   -- don't delete!
    while(nSampleFrames-- > 0)
    {
        INT32 chorusOut;
        
    // get input
        INT32 inputL = sourceP[0];
        INT32 inputR = sourceP[1];
        INT32 input = inputL + inputR;


    // calculate sample value
        int intReadIndex = readIndex >> READINDEXSHIFT;
        int intReadIndex2 = (intReadIndex + 1) % kChorusBufferFrameSize;

        INT32 b = buffer[intReadIndex];
        INT32 c = buffer[intReadIndex2];
        
        INT32 tap = (((INT32) (readIndex & READINDEXMASK) * (c-b))>>READINDEXSHIFT) + b;


    // write input plus feedback back into the delay line
        buffer[writeIndex] = input + ((tap*feedbackGain) >> FEEDBACKSHIFT) ;
        
    
        // divide by two since we added stereo inputs, divide by two again to mix with dry
        chorusOut = tap >> 2;   

    // write to output
        *sourceP++ = chorusOut + (inputL >> 1);
        *sourceP++ = chorusOut + (inputR >> 1);
    
    // increment the read head by fractional amounts    
        readIndex += readIndexIncr;
        
    // wrap-around read index   
        if(readIndex >=  kReadIndexAdjust)
        {
            readIndex -= kReadIndexAdjust;
        }

    // wrap-around write pointer
        writeIndex = (writeIndex + 1) % kChorusBufferFrameSize;
    }
#endif




#endif // USE_NEW_EFFECTS



