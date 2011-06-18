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
** "GenReverb.c"
**
**  Generalized Music Synthesis package. Part of SoundMusicSys.
**
**  © Copyright 1993-2000 Beatnik, Inc, All Rights Reserved.
**  Written by Jim Nitchals
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
**
** Modification History:
**
**  1/18/96     Spruced up for C++ extra error checking
**              Changed the macro 'THE_CHECK' to accept a type for typecasting the source pointer
**  3/1/96      Removed extra PV_DoCallBack, and PV_GetWavePitch
**  4/21/96     Put test around ReverbBuffer being NULL
**  12/30/96    Changed copyright
**  1/23/97     Added WebTV small footprint reverb unit, and support for PV_PostFilterStereo
**              code
**              Fixed REVERB_TYPE_6 to match be the same amount of spring for all output
**              rates
**  6/4/97      Added USE_SMALL_MEMORY_REVERB tests around code to disable when this
**              flag is used
**  2/3/98      Renamed songBufferLeftMono to songBufferDry
**  2/11/98     Added support for Q_48K & Q_24K
**  2/20/98     kcr     interrcept most fixed reverbs to the new variable reverb
**  2/22/98     Disabled the use of the PV_PostFilterStereo filter. It sounds bad
**  2/23/98     Removed last of old variable reverb code and filter (PV_PostFilterStereo)
**              Removed GM_InitReverbTaps & GM_GetReverbTaps & GM_SetReverbTaps
**  2/24/98     Removed redundant parameter setting in new reverb -- also now call
**              CheckReverbType() to clear out reverb buffers when switching type
**  3/2/98      Added some wrappers with USE_NEW_EFFECTS around some code
**  3/16/98     Rearranged how the verbs are setup/cleaned up and run. Added the missing
**              REVERB_TYPE_2 is the fixed case. Moved in from GenSetup.c GM_SetupReverb &
**              GM_CleanupReverb.
**              Added GM_ProcessReverb & GM_GetReverbEnableThreshold.
**              Moved GM_SetReverbType & GM_GetReverbType from GenSetup.c
**  3/24/98     Added a version of PV_RunMonoFixedReverb that does nothing for
**              no mono code support
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
**  6/26/98     Added GM_IsReverbFixed
**  7/7/98      Removed reverbIsVariable
**  7/30/98     Modified the verb (Lab and cavern) to be smaller when running at 48k to
**              avoid clicks
**  8/12/98     Modified GM_SetReverbType to walk through active voices and reset
**              various verb settings
**  11/9/98     Renamed NoteDur to voiceMode
**  10/30/99    Added support for Q_16K
**  12/3/99     Added controls REVERB_USED != REVERB_DISABLED for conditional compiles
**  2/4/2000    Changed copyright. We're Y2K compliant!
*/
/*****************************************************************************/
#include "GenSnd.h"
#include "GenPriv.h"

#if REVERB_USED != REVERB_DISABLED

#if USE_MONO_OUTPUT == TRUE
static void PV_RunMonoFixedReverb(ReverbMode which)
{
    register INT32      b, c, bz, cz;
    register INT32      *sourceLR, *sourceReverb;
    register INT32      *reverbBuf;
    register LOOPCOUNT  a;
    register long       reverbPtr1, reverbPtr2, reverbPtr3, reverbPtr4;

    reverbBuf = &MusicGlobals->reverbBuffer[0];
    if (reverbBuf)
    {
        sourceLR = &MusicGlobals->songBufferDry[0];
        sourceReverb = &MusicGlobals->songBufferReverb[0];
    
        b = MusicGlobals->LPfilterL;
        c = MusicGlobals->LPfilterR;
        bz = MusicGlobals->LPfilterLz;
        cz = MusicGlobals->LPfilterRz;
        reverbPtr1 = MusicGlobals->reverbPtr;

    // NOTE: do not exceed a tap distance of 2730.  2730*6 is almost the full buffer size.
        switch (which)
        {
    // ----------
            case REVERB_TYPE_2:     // Igor's Closet
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 1;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 2;
                        break;
                    case Q_RATE_44K:
                        a = 4;
                        break;
                    case Q_RATE_48K:
                        a = 5;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 450*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;

                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= b >> 2;
                    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
                    *sourceLR += b >> 2;
                    sourceLR++;
                                
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_3:     // Igor's Garage
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 1;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 2;
                        break;
                    case Q_RATE_44K:
                        a = 4;
                        break;
                    case Q_RATE_48K:
                        a = 5;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 430*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= b >> 2;
                    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
                    *sourceLR += b >> 1;
                    sourceLR++;
                    
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_4:     // Igor's Acoustic Lab
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 1;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 2;
                        break;
                    case Q_RATE_44K:
                        a = 4;
                        break;
                    case Q_RATE_48K:
                        a = 5;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= (bz + b) >> 2;
                    bz = b;
                    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
                    *sourceLR = *sourceLR +  (b >> 1);
                    sourceLR++;
                    
                    
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_5:     // Igor's Dungeon
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 1;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 2;
                        break;
                    case Q_RATE_44K:
                        a = 4;
                        break;
                    case Q_RATE_48K:
                        a = 5;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 500*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 674*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 1174*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= b >> 1;
                    b = (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 2;
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
                    *sourceLR = (*sourceLR + (b << 2)) >> 1;
                    sourceLR++;
    
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_6:     // Igor's Cavern
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 2;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 4;
                        break;
                    case Q_RATE_44K:
                        a = 8;
                        break;
                    case Q_RATE_48K:
                        a = 9;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 2700/2*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 3250/2*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 4095/2*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b += ((reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 4) - (b >> 2);
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 8);
                    *sourceLR = *sourceLR + b;
                    sourceLR++;
    
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_7:     // webtv
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 1;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 2;
                        break;
                    case Q_RATE_44K:
                        a = 4;
                        break;
                    case Q_RATE_48K:
                        a = 5;
                        break;
                }
                reverbPtr1 = MusicGlobals->reverbPtr & REVERB_BUFFER_MASK_SMALL;
                reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK_SMALL; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK_SMALL; 
                reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK_SMALL;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= (c + b) >> 2;
                    c = b;
                    b += reverbBuf[reverbPtr2] >> 3;
                    b += reverbBuf[reverbPtr3] >> 3;
                    b += reverbBuf[reverbPtr4] >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
                    *sourceLR = *sourceLR +  (b >> 1) - (b >> 3);
                    sourceLR++;
                            
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK_SMALL;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK_SMALL;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK_SMALL;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK_SMALL;
                }
                break;
        }
        MusicGlobals->LPfilterL = b;
        MusicGlobals->LPfilterLz = bz;
        MusicGlobals->LPfilterR = c;
        MusicGlobals->LPfilterRz = cz;
        MusicGlobals->reverbPtr = reverbPtr1;
    }
}
#else
static void PV_RunMonoFixedReverb(ReverbMode which)
{
    which = which;
}
#endif  // USE_MONO_OUTPUT

static void PV_RunStereoFixedReverb(ReverbMode which)
{
    register INT32      b, c, bz, cz;
    register INT32      *sourceLR;
    register INT32      *reverbBuf;
    register LOOPCOUNT  a;
    register long       reverbPtr1, reverbPtr2, reverbPtr3, reverbPtr4;

    reverbBuf = &MusicGlobals->reverbBuffer[0];
    if (reverbBuf)
    {
        sourceLR = &MusicGlobals->songBufferDry[0];
    
        b = MusicGlobals->LPfilterL;
        c = MusicGlobals->LPfilterR;
        bz = MusicGlobals->LPfilterLz;
        cz = MusicGlobals->LPfilterRz;
        reverbPtr1 = MusicGlobals->reverbPtr;


    // NOTE: do not exceed a tap distance of 2047.  2047*8 is almost the full buffer size.
        switch (which)
        {
    // ----------
            case REVERB_TYPE_2: // was called closet -- really just early reflections
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 2;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 4;
                        break;
                    case Q_RATE_44K:
                        a = 8;
                        break;
                    case Q_RATE_48K:
                        a = 9;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 450*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;

                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= b >> 2;
                    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
                    *sourceLR += (b + c) >> 2;
                    sourceLR++;
                    
                    c -= c >> 2;
                    c += (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 3;
                    reverbBuf[reverbPtr1+1] = *sourceLR + (c >> 1);
                    *sourceLR += (c + b) >> 2;
                    sourceLR++;
                    
                    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
                }
                break;
                                
    // ----------
            case REVERB_TYPE_3:     // Igor's Garage
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 2;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 4;
                        break;
                    case Q_RATE_44K:
                        a = 8;
                        break;
                    case Q_RATE_48K:
                        a = 9;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 430*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;

                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= b >> 2;
                    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
                    *sourceLR += (b + c) >> 1;
                    sourceLR++;
                    
                    c -= c >> 2;
                    c += (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 3;
                    reverbBuf[reverbPtr1+1] = *sourceLR + (c >> 1);
                    *sourceLR += (c + b) >> 1;
                    sourceLR++;
                    
                    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_4:     // Igor's Acoustic Lab
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 2;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 4;
                        break;
                    case Q_RATE_44K:
                    case Q_RATE_48K:
                        a = 8;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= (bz + b) >> 2;
                    bz = b;
                    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
                    *sourceLR = *sourceLR +  ((c + b) >> 2);
                    sourceLR++;
                    
                    c -= (cz + c) >> 2;
                    cz = c;
                    c += (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 3;
                    reverbBuf[reverbPtr1+1] = *sourceLR + c - (c >> 2);
                    *sourceLR = *sourceLR + ((c + b) >> 2);
                    sourceLR++;
                    
                    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_5:     // Igor's Dungeon
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 2;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 4;
                        break;
                    case Q_RATE_44K:
                        a = 8;
                        break;
                    case Q_RATE_48K:
                        a = 9;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 500*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 674*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 1174*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= b >> 1;
                    b = (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 2;
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
                    *sourceLR = *sourceLR + b;
                    sourceLR++;
                    
                    c -= c >> 1;
                    c = (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 2;
                    reverbBuf[reverbPtr1+1] = *sourceLR + c - (c >> 2);
                    *sourceLR = *sourceLR + c;
                    sourceLR++;
                    
                    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
                }
                break;
    // ---------- 
            case REVERB_TYPE_6:     // Igor's Cavern
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 2;
                        break;
                    case Q_RATE_24K:
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                        a = 4;
                        break;
                    case Q_RATE_44K:
                    case Q_RATE_48K:
                        a = 8;
                        break;
                }
                reverbPtr2 = (MusicGlobals->reverbPtr - 2700*a) & REVERB_BUFFER_MASK; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 3250*a) & REVERB_BUFFER_MASK;
                reverbPtr4 = (MusicGlobals->reverbPtr - 4095*a) & REVERB_BUFFER_MASK;
                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b += ((reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 4) - (b >> 2);
                    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 8);
                    *sourceLR = *sourceLR + b + c;
                    sourceLR++;
                    
                    c += ((reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 4) - (c >> 2);
                    reverbBuf[reverbPtr1+1] = *sourceLR + c - (c >> 8);
                    *sourceLR = *sourceLR + c + b;
                    sourceLR++;
                    
                    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
                    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
                }
                break;
    // ----------
            case REVERB_TYPE_7:     // webtv
                switch (MusicGlobals->outputRate)
                {
                    case Q_RATE_8K:
                    case Q_RATE_11K_TERP_22K:
                    case Q_RATE_11K:
                    case Q_RATE_16K:
                        a = 1;
                        break;
                    case Q_RATE_22K_TERP_44K:
                    case Q_RATE_22K:
                    case Q_RATE_24K:
                        a = 2;
                        break;
                    case Q_RATE_44K:
                        a = 4;
                        break;
                    case Q_RATE_48K:
                        a = 5;
                        break;                  
                }
                reverbPtr1 = MusicGlobals->reverbPtr & REVERB_BUFFER_MASK_SMALL;
                reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK_SMALL; 
                reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK_SMALL; 
                reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK_SMALL;

                for (a = MusicGlobals->One_Loop; a > 0; --a)
                {
                    b -= (c + b) >> 2;
                    c = b;
                    b += reverbBuf[reverbPtr2] >> 3;
                    b += reverbBuf[reverbPtr3] >> 3;
                    b += reverbBuf[reverbPtr4] >> 3;
                    reverbBuf[reverbPtr1] = *sourceLR + sourceLR[1] + b - (b >> 2);
                    *sourceLR = *sourceLR +  (b >> 1) - (b >> 3);
                    sourceLR++;
                    *sourceLR = *sourceLR +  (b >> 1) - (b >> 3);
                    sourceLR++;
                            
                    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK_SMALL;
                    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK_SMALL;
                    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK_SMALL;
                    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK_SMALL;
                }
                break;
        }
        MusicGlobals->LPfilterL = b;
        MusicGlobals->LPfilterLz = bz;
        MusicGlobals->LPfilterR = c;
        MusicGlobals->LPfilterRz = cz;
        MusicGlobals->reverbPtr = reverbPtr1;
    }   
}


#if USE_NEW_EFFECTS == TRUE
static void PV_RunStereoNewReverb(ReverbMode which)
{
    which = which;
    CheckReverbType();
    RunNewReverb(MusicGlobals->songBufferReverb, MusicGlobals->songBufferDry, MusicGlobals->One_Loop);
}
#endif



// This table must not be in ROM, because the function pointers are set at runtime.
// This table matches the ReverbMode index but the first two ReverbMode's
// are empty. If you add more verb types, add an entry into this table
static GM_ReverbConfigure verbTypes[MAX_REVERB_TYPES] = 
{
    {   // dead
        REVERB_NO_CHANGE,
        127,
        TRUE,                           // fixed
        0,
        NULL,
        NULL
    },
    {   // none - dead
        REVERB_TYPE_1,
        127,
        TRUE,                           // fixed
        0,
        NULL,
        NULL
    },
    {   // Igor's Closet
        REVERB_TYPE_2,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Garage
        REVERB_TYPE_3,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Acoustic Lab
        REVERB_TYPE_4,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Dungeon
        REVERB_TYPE_5,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Cavern
        REVERB_TYPE_6,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Small reflections Reverb used for WebTV
        REVERB_TYPE_7,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE_SMALL * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
#if USE_NEW_EFFECTS == TRUE
// if we're using the cool new verbs, set them up
    {   // Early reflections (variable verb)
        REVERB_TYPE_8,
        0,
        FALSE,                          // fixed
        (kNumberOfDiffusionStages * kDiffusionBufferFrameSize * sizeof(INT32)) +
            (sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
            ((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * sizeof(INT32)),
        NULL,
        PV_RunStereoNewReverb
    },
    {   // Basement (variable verb)
        REVERB_TYPE_9,
        0,
        FALSE,                          // fixed
        (kNumberOfDiffusionStages * kDiffusionBufferFrameSize * sizeof(INT32)) +
            (sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
            ((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * sizeof(INT32)),
        NULL,
        PV_RunStereoNewReverb
    },
    {   // Banquet hall (variable verb)
        REVERB_TYPE_10,
        0,
        FALSE,                          // fixed
        (kNumberOfDiffusionStages * kDiffusionBufferFrameSize * sizeof(INT32)) +
            (sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
            ((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * sizeof(INT32)),
        NULL,
        PV_RunStereoNewReverb
    },
    {   // Catacombs (variable verb)
        REVERB_TYPE_11,
        0,
        FALSE,                          // fixed
        (kNumberOfDiffusionStages * kDiffusionBufferFrameSize * sizeof(INT32)) +
            (sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
            ((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * sizeof(INT32)),
        NULL,
        PV_RunStereoNewReverb
    }
#else
// else we match them to our exisiting verbs
    {   // Igor's Closet
        REVERB_TYPE_2,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Garage
        REVERB_TYPE_3,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Acoustic Lab
        REVERB_TYPE_4,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
    {   // Igor's Dungeon
        REVERB_TYPE_5,
        REVERB_CONTROLER_THRESHOLD,
        TRUE,                           // fixed
        REVERB_BUFFER_SIZE * 2L * sizeof(long),
        PV_RunMonoFixedReverb,
        PV_RunStereoFixedReverb
    },
#endif
};

#define MAX_VERB_CONFIG_ENTRIES     (long)(sizeof (verbTypes) / sizeof(GM_ReverbConfigure))

// private function to allocate and setup the fixed verb types
static XBOOL PV_SetupFixedReverb(void)
{
    GM_Mixer    *pMixer;
    long        size;

    pMixer = MusicGlobals;

    // clean fixed verb filter status
    pMixer->LPfilterL = 0;
    pMixer->LPfilterR = 0;
    pMixer->LPfilterLz = 0;
    pMixer->LPfilterRz = 0;

    size = REVERB_BUFFER_SIZE * 2L * sizeof(long);
    pMixer->reverbBuffer = (INT32 *)XNewPtr(size);
    if (pMixer->reverbBuffer == NULL)
    {
        // if this failed, try to allocate the smaller verb entry
        size = REVERB_BUFFER_SIZE_SMALL * 2L * sizeof(long);
        pMixer->reverbBuffer = (INT32 *)XNewPtr(size);
        if (pMixer->reverbBuffer == NULL)
        {
            size = 0;   // no verb
        }
    }
    pMixer->reverbBufferSize = size;
    return (size) ? TRUE : FALSE;
}

// private function to deallocate and cleanup the fixed verb types
static void PV_CleanupFixedReverb(void)
{
    XPTR    verb1;

    if (MusicGlobals)
    {
        // capture all pointers and set them to zero so that the interrupt services
        // will stop then deallocate the memory
        verb1 = MusicGlobals->reverbBuffer;
        if (verb1)
        {
            MusicGlobals->reverbPtr = 0;
            GM_SetReverbType(REVERB_TYPE_1);        // no reverb
            MusicGlobals->reverbBuffer = NULL;
            XDisposePtr(verb1);
        }
    }
}

void GM_ProcessReverb(void)
{
    GM_ReverbProc   pVerbProc;
    ReverbMode      type;

    if (MusicGlobals->reverbBuffer)
    {
        pVerbProc = NULL;
        type = MusicGlobals->reverbUnitType;
        switch (type)
        {
            default:
                type = REVERB_TYPE_1;   // none;
                break;
            
            // valid table entries
            case REVERB_TYPE_2:         // Igor's Closet
            case REVERB_TYPE_3:         // Igor's Garage
            case REVERB_TYPE_4:         // Igor's Acoustic Lab
            case REVERB_TYPE_5:         // Igor's Cavern
            case REVERB_TYPE_6:         // Igor's Dungeon
            case REVERB_TYPE_7:         // Small reflections Reverb used for WebTV
            case REVERB_TYPE_8:         // Early reflections (variable verb)
            case REVERB_TYPE_9:         // Basement (variable verb)
            case REVERB_TYPE_10:        // Banquet hall (variable verb)
            case REVERB_TYPE_11:        // Catacombs (variable verb)
                break;
        }
        if (type != REVERB_TYPE_1)
        {
            if (verbTypes[type].globalReverbUsageSize <= MusicGlobals->reverbBufferSize)
            {
                if (MusicGlobals->generateStereoOutput)
                {
                    pVerbProc = verbTypes[type].pStereoRuntimeProc;
                }
                else
                {
                    pVerbProc = verbTypes[type].pMonoRuntimeProc;
                }
                if (pVerbProc)
                {
                    (*pVerbProc)(verbTypes[type].type);
                }
            }
        }
    }
}

// Reverb setup and configure. This will cleanup the exisiting verb if enabled
void GM_SetupReverb(void)
{
    GM_Mixer    *pMixer;

    pMixer = MusicGlobals;
    if (pMixer)
    {
        GM_CleanupReverb();

        pMixer->reverbUnitType = REVERB_NO_CHANGE;
        pMixer->reverbTypeAllocated = REVERB_NO_CHANGE;
        pMixer->reverbBufferSize = 0;

        // since we can only allocate memory in this function because its not being called during 
        // an interrupt, we need to walk through all the verb types 

        if (PV_SetupFixedReverb())  // try to allocate the fixed verb first)
        {
#if USE_NEW_EFFECTS == TRUE
            if (InitNewReverb())
            {
                InitChorus();       
            }
#endif
        }
    }
}

void GM_CleanupReverb(void)
{
    if (MusicGlobals)
    {
        PV_CleanupFixedReverb();

#if USE_NEW_EFFECTS
        ShutdownNewReverb();
        ShutdownChorus();

        // these effects will be fully integrated later...
        //ShutdownDelay();
        //ShutdownGraphicEq();
        //ShutdownParametricEq();
        //ShutdownResonantFilter();
#endif
    }
}

// get highest MIDI verb amount required to activate verb
UBYTE GM_GetReverbEnableThreshold(void)
{
    UBYTE   thres;

    thres = MAX_NOTE_VOLUME;
    if (MusicGlobals)
    {
        if (MusicGlobals->reverbBuffer)
        {
            thres = verbTypes[MusicGlobals->reverbUnitType].thresholdEnableValue;
        }
    }
    return thres;
}

// Is current reverb fixed (old style)?
XBOOL GM_IsReverbFixed(void)
{
    UBYTE   fixed;

    fixed = TRUE;
    if (MusicGlobals)
    {
        if (MusicGlobals->reverbBuffer)
        {
            fixed = verbTypes[MusicGlobals->reverbUnitType].isFixed;
        }
    }
    return fixed;
}

// Set the global reverb type. This can happen at interrupt time, so don't allocate any memory
void GM_SetReverbType(ReverbMode reverbMode)
{
    XBOOL   changed;

    changed = FALSE;
    if (MusicGlobals)
    {
        if (MusicGlobals->reverbBuffer)
        {
            switch (reverbMode)
            {
                case REVERB_NO_CHANGE:  // no change
                default:
                    break;
                // valid types
                case REVERB_TYPE_1:
                case REVERB_TYPE_2:
                case REVERB_TYPE_3:
                case REVERB_TYPE_4:
                case REVERB_TYPE_5:
                case REVERB_TYPE_6:
                case REVERB_TYPE_7:
                case REVERB_TYPE_8:
                case REVERB_TYPE_9:
                case REVERB_TYPE_10:
                case REVERB_TYPE_11:
                    MusicGlobals->reverbUnitType = reverbMode;
#if USE_NEW_EFFECTS == TRUE
                    CheckReverbType();
#endif
                    changed = TRUE;
                    break;
            }
        }

        // now walk through all active voices and reset various reverb controls
        if (changed)
        {
            register LOOPCOUNT count;
            register GM_Voice *pVoice;

            for (count = 0; count < MusicGlobals->MaxNotes + MusicGlobals->MaxEffects; count++)
            {
                pVoice = &MusicGlobals->NoteEntry[count];
                if (pVoice->voiceMode != VOICE_UNUSED)  // active?
                {
                    if (pVoice->pSong)      // active song voice
                    {
                        if (pVoice->pInstrument)
                        {
                            pVoice->avoidReverb = pVoice->pInstrument->avoidReverb; // use instrument default. in case instrument designer
                        }
                        else
                        {
                            pVoice->avoidReverb = FALSE;
                        }
                        pVoice->reverbLevel = pVoice->pSong->channelReverb[pVoice->NoteChannel];    // set current verb level
                        pVoice->chorusLevel = (INT16)PV_ModifyVelocityFromCurve(pVoice->pSong, pVoice->pSong->channelChorus[pVoice->NoteChannel]);
                                                                            // wants no verb enabled
                        if (GM_IsReverbFixed())
                        {
                            // if the instrument defines reverb on or the channel has reverb on, then enable it.
                            // if the channel is off, but the instrument defines reverb then enable it
                            if (pVoice->pSong->channelReverb[pVoice->NoteChannel] < GM_GetReverbEnableThreshold())
                            {
                                pVoice->avoidReverb = TRUE;     // force off
                            }
                            if (pVoice->avoidReverb)
                            {
                                pVoice->reverbLevel = 0;
                                pVoice->chorusLevel = 0;
                            }
                        }
                    }
                }
            }           
        }
    }
}

// Return the current verb type
ReverbMode GM_GetReverbType(void)
{
    ReverbMode reverbMode;

    reverbMode = REVERB_TYPE_1;
    if (MusicGlobals)
    {
        reverbMode = (ReverbMode)MusicGlobals->reverbUnitType;
    }
    return reverbMode;
}
#endif  // REVERB_USED != REVERB_DISABLED

// EOF of GenReverb.c
