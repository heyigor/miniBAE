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
** "GenSynthInterp2U3232.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**	Confidential-- Internal use only
**
**	й Copyright 1993-2000 Beatnik, Inc, All Rights Reserved.
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
**	3/11/99		MOE: created file from GenSynthInterp2Float.c
**	10/19/99	MSD: switched to REVERB_USED and LOOPS_USED
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	1/27/2002	sh Fixed warnings.
*/
/*****************************************************************************/

#include "GenSnd.h"
#include "GenPriv.h"
#include "X_Assert.h"

#if LOOPS_USED == U3232_LOOPS

void PV_ServeU3232FullBuffer (GM_Voice *this_voice)
{
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register UBYTE 			*source, *calculated_source;
	register INT32			b, c;
	register U32	 		cur_wave_i, cur_wave_f;
	U3232			 		wave_increment;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232FullBufferNewReverb (this_voice); 
		return;
	}
#endif
	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop;
	dest = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	{
		if (this_voice->channels == 1)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[0] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitude;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[1] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitude;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[2] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitude;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[3] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitude;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				dest += 4;
				amplitude += amplitudeAdjust;
			}
		}
		else
		{	// stereo 8 bit instrument
			for (a = MusicGlobals->Sixteen_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 16; inner++)
				{
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0] + calculated_source[1];	// average left & right channels
					c = calculated_source[2] + calculated_source[3];
					*dest += (((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x100) * amplitude) >> 1;
					dest++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitude += amplitudeAdjust;
			}
		}
	}
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitude;
}

void PV_ServeU3232PartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register UBYTE 			*source, *calculated_source;
	register INT32			b, c;
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232PartialBufferNewReverb (this_voice, looping);
		return;
	}
#endif
	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop;
	dest = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	if (looping)
	{
		wave_adjust = this_voice->NoteLoopEnd - this_voice->NoteLoopPtr;
		end_wave = this_voice->NoteLoopEnd - this_voice->NotePtr;
	}
	else
	{
		end_wave = this_voice->NotePtrEnd - this_voice->NotePtr - 1;
	}

	{
		if (this_voice->channels == 1)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					*dest += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitude;
					dest++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitude += amplitudeAdjust;
			}
		}
		else
		{	// stereo 8 bit instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0] + calculated_source[1];
					c = calculated_source[2] + calculated_source[3];
					*dest += (((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x100) * amplitude) >> 1;
					dest++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitude += amplitudeAdjust;
			}
		}
	}

	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitude;
FINISH:
	return;
}

void PV_ServeU3232StereoFullBuffer(GM_Voice *this_voice)
{
	register INT32 			*destL;
	register LOOPCOUNT		a, inner;
	register UBYTE 			*source, *calculated_source;
	register INT32			b, c;
	register U32	 		cur_wave_i, cur_wave_f;
	U3232			 		wave_increment;
	register INT32			sample;
	INT32					ampValueL, ampValueR;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			amplitudeLincrement;
	register INT32			amplitudeRincrement;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232StereoFullBufferNewReverb (this_voice);
		return;
	}
#endif
	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);
	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = (ampValueL - amplitudeL) / (MusicGlobals->Four_Loop);
	amplitudeRincrement = (ampValueR - amplitudeR) / (MusicGlobals->Four_Loop);

	destL = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	{
		if (this_voice->channels == 1)
		{	// mono instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[0] += sample * amplitudeL;
				destL[1] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[2] += sample * amplitudeL;
				destL[3] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[4] += sample * amplitudeL;
				destL[5] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[6] += sample * amplitudeL;
				destL[7] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				destL += 8;
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{	// stereo 8 bit instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0];
					c = calculated_source[2];
					destL[0] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitudeL;
					b = calculated_source[1];
					c = calculated_source[3];
					destL[1] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitudeR;
					destL += 2;
			
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->lastAmplitudeL = amplitudeL;
	this_voice->lastAmplitudeR = amplitudeR;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
}

void PV_ServeU3232StereoPartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register LOOPCOUNT		a, inner;
	register UBYTE 			*source, *calculated_source;
	register INT32			b, c, sample;
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	INT32					ampValueL, ampValueR;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			amplitudeLincrement, amplitudeRincrement;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232StereoPartialBufferNewReverb (this_voice, looping); 
		return;
	}
#endif
	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);
	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = (ampValueL - amplitudeL) / (MusicGlobals->Four_Loop);
	amplitudeRincrement = (ampValueR - amplitudeR) / (MusicGlobals->Four_Loop);

	destL = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	if (looping)
	{
		wave_adjust = this_voice->NoteLoopEnd - this_voice->NoteLoopPtr;
		end_wave = this_voice->NoteLoopEnd - this_voice->NotePtr;
	}
	else
	{
		end_wave = this_voice->NotePtrEnd - this_voice->NotePtr - 1;
	}

	{
		if (this_voice->channels == 1)
		{	// mono instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
#if 1	//MOE'S OBSESSIVE FOLLY
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
					destL[0] += sample * amplitudeL;
					destL[1] += sample * amplitudeR;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
					destL += 2;
				}
#else
				THE_CHECK_U3232(UBYTE *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[0] += sample * amplitudeL;
				destL[1] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				
				THE_CHECK_U3232(UBYTE *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[2] += sample * amplitudeL;
				destL[3] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(UBYTE *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[4] += sample * amplitudeL;
				destL[5] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(UBYTE *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80;
				destL[6] += sample * amplitudeL;
				destL[7] += sample * amplitudeR;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				destL += 8;
#endif
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{	// Stereo 8 bit instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0];
					c = calculated_source[2];
					destL[0] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitudeL;
					b = calculated_source[1];
					c = calculated_source[3];
					destL[1] += ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) * amplitudeR;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL;
	this_voice->lastAmplitudeR = amplitudeR;
FINISH:
	return;
}

// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее

// 16 bit cases
void PV_ServeU3232FullBuffer16 (GM_Voice *this_voice)
{
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register INT16 			*source, *calculated_source;
	register INT32			b, c, sample;
	register U32	 		cur_wave_i, cur_wave_f;
	U3232			 		wave_increment;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232FullBuffer16NewReverb (this_voice); 
		return;
	}
#endif
	amplitude = this_voice->lastAmplitudeL;
	//BAE_PRINTF("f0, amp = %ld n = %ld nve = %ld\n", (long)amplitude, (long)this_voice->NoteVolume,
	//												(long)this_voice->NoteVolumeEnvelope);
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop >> 4;
	amplitude = amplitude >> 4;
	//BAE_PRINTF("f1, amp = %ld aa = %ld\n", (long)amplitude, (long)amplitudeAdjust);

	dest = &MusicGlobals->songBufferDry[0];
	source = (short *) this_voice->NotePtr;
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	{
		if (this_voice->channels == 1)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[0] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[1] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[2] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[3] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				dest += 4;
				amplitude += amplitudeAdjust;
			}
		}
		else
		{	// stereo 16 bit instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0] + calculated_source[1];
					c = calculated_source[2] + calculated_source[3];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					*dest += (sample  * amplitude) >> 5;	// divide extra for summed stereo channels
					dest++;

					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitude += amplitudeAdjust;
			}
		}
	}
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitude << 4;
}

void PV_ServeU3232PartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register INT16 			*source, *calculated_source;
	register INT32			b, c, sample;
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232PartialBuffer16NewReverb (this_voice, looping); 
		return;
	}
#endif
	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop >> 4;
	amplitude = amplitude >> 4;
	//BAE_PRINTF("p,amp = %ld\n", (long)amplitude);
	dest = &MusicGlobals->songBufferDry[0];
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;
	source = (short *) this_voice->NotePtr;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	if (looping)
	{
		wave_adjust = this_voice->NoteLoopEnd - this_voice->NoteLoopPtr;
		end_wave = this_voice->NoteLoopEnd - this_voice->NotePtr;
	}
	else
	{
		end_wave = this_voice->NotePtrEnd - this_voice->NotePtr - 1;
	}

	{
		if (this_voice->channels == 1)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
#if 1	//MOE'S OBSESSIVE FOLLY
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					dest[0] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
					dest++;
				}
#else
				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[0] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[1] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[2] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				dest[3] += (((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) * amplitude) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				dest += 4;
#endif
				amplitude += amplitudeAdjust;
			}
		}
		else
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0] + calculated_source[1];
					c = calculated_source[2] + calculated_source[3];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					*dest += ((sample >> 1) * amplitude) >> 5;
					dest++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitude += amplitudeAdjust;
			}
		}
	}
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitude << 4;
FINISH:
	return;
}

void PV_ServeU3232StereoFullBuffer16 (GM_Voice *this_voice)
{
	register INT32 			*destL;
	register LOOPCOUNT		a, inner;
	register INT16 			*source, *calculated_source;
	register INT32			b, c;
	register U32	 		cur_wave_i, cur_wave_f;
	U3232			 		wave_increment;
	register INT32			sample;
	INT32					ampValueL, ampValueR;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			amplitudeLincrement;
	register INT32			amplitudeRincrement;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{ 
		PV_ServeU3232StereoFullBuffer16NewReverb (this_voice);
		return;
	}
#endif
	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);
	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = (ampValueL - amplitudeL) / (MusicGlobals->Four_Loop);
	amplitudeRincrement = (ampValueR - amplitudeR) / (MusicGlobals->Four_Loop);

	amplitudeL = amplitudeL >> 4;
	amplitudeR = amplitudeR >> 4;
	amplitudeLincrement = amplitudeLincrement >> 4;
	amplitudeRincrement = amplitudeRincrement >> 4;

	destL = &MusicGlobals->songBufferDry[0];
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	source = (short *) this_voice->NotePtr;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);
	{
		if (this_voice->channels == 1)
		{	// mono instrument

			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[0] += (sample * amplitudeL) >> 4;
				destL[1] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[2] += (sample * amplitudeL) >> 4;
				destL[3] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[4] += (sample * amplitudeL) >> 4;
				destL[5] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[6] += (sample * amplitudeL) >> 4;
				destL[7] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				destL += 8;
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{	// stereo 16 bit instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0];
					c = calculated_source[2];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					destL[0] += (sample * amplitudeL) >> 4;
					b = calculated_source[1];
					c = calculated_source[3];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					destL[1] += (sample * amplitudeR) >> 4;
					destL += 2;
			
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->lastAmplitudeL = amplitudeL << 4;
	this_voice->lastAmplitudeR = amplitudeR << 4;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
}

void PV_ServeU3232StereoPartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register LOOPCOUNT		a, inner;
	register INT16 			*source, *calculated_source;
	register INT32			b, c, sample;
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	INT32					ampValueL, ampValueR;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			amplitudeLincrement, amplitudeRincrement;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeU3232StereoPartialBuffer16NewReverb (this_voice, looping);
		return;
	}
#endif
	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);
	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = (ampValueL - amplitudeL) / (MusicGlobals->Four_Loop);
	amplitudeRincrement = (ampValueR - amplitudeR) / (MusicGlobals->Four_Loop);

	amplitudeL = amplitudeL >> 4;
	amplitudeR = amplitudeR >> 4;
	amplitudeLincrement = amplitudeLincrement >> 4;
	amplitudeRincrement = amplitudeRincrement >> 4;

	destL = &MusicGlobals->songBufferDry[0];
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;
	source = (short *) this_voice->NotePtr;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	if (looping)
	{
		wave_adjust = this_voice->NoteLoopEnd - this_voice->NoteLoopPtr;
		end_wave = this_voice->NoteLoopEnd - this_voice->NotePtr;
	}
	else
	{
		end_wave = this_voice->NotePtrEnd - this_voice->NotePtr - 1;
	}

	{
		if (this_voice->channels == 1)
		{	// mono instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
#if 1	//MOE'S OBSESSIVE FOLLY
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					destL[0] += (sample * amplitudeL) >> 4;
					destL[1] += (sample * amplitudeR) >> 4;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
					destL += 2;
				}
#else
				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[0] += (sample * amplitudeL) >> 4;
				destL[1] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[2] += (sample * amplitudeL) >> 4;
				destL[3] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[4] += (sample * amplitudeL) >> 4;
				destL[5] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				THE_CHECK_U3232(INT16 *);
				b = source[cur_wave_i];
				c = source[cur_wave_i+1];
				sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
				destL[6] += (sample * amplitudeL) >> 4;
				destL[7] += (sample * amplitudeR) >> 4;
				ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);

				destL += 8;
#endif
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{	// Stereo 16 bit instrument
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);
					calculated_source = source + cur_wave_i * 2;
					b = calculated_source[0];
					c = calculated_source[2];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					destL[0] += (sample * amplitudeL) >> 4;
					b = calculated_source[1];
					c = calculated_source[3];
					sample = (((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b;
					destL[1] += (sample * amplitudeR) >> 4;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL << 4;
	this_voice->lastAmplitudeR = amplitudeR << 4;
FINISH:
	return;
}

#endif	// LOOPS_USED == U3232_LOOPS

