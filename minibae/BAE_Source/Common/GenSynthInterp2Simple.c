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
** "GenSynthInterp2Simple.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**	Confidential-- Internal use only
**
**	й Copyright 1993-1999 Beatnik, Inc, All Rights Reserved.
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
**	1/18/96		Spruced up for C++ extra error checking
**				Changed the macro 'THE_CHECK' to accept a type for typecasting the source pointer
**	3/1/96		Removed extra PV_DoCallBack, and PV_GetWavePitch
**	4/25/96		Fixed bug with PV_ServeInterp2PartialBuffer16 that preserved the amp differently 
**				than PV_ServeInterpFulllBuffer16. Caused a click when playing mono with mono output
**	5/2/96		Changed 'int's to INT32 and to BOOL_FLAG
**	7/8/96		Improved enveloping and wave shaping code
**	7/10/96		Fixed stereo filter bug
**	12/30/96	Changed copyright
**	6/4/97		Added USE_SMALL_MEMORY_REVERB tests around code to disable when this
**				flag is used
**	2/3/98		Renamed songBufferLeftMono to songBufferDry
**	2/8/98		Changed BOOL_FLAG to XBOOL
**	2/20/98		now support variable send chorus as well as reverb
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
**	11/10/98	Removed CLIP macro
**	11/23/98	Added support for Intel Katmai CPU
**	1/4/99		Removed FAR macro. Re ordered Katmai code and duplicated inner loops
**	10/19/99	MSD: switched to REVERB_USED and LOOPS_USED
**	11/10/99	Added WRITE_LOOPS code to write out inner loops into file. Used
**				for debugging
**				Split from GenSynthInterp2.c
**				Removed Katmai code
**				Added some comments
**				Removed stereo support
**	12/11/2000	Removed 8-bit source sample inner loops
*/
/*****************************************************************************/

#include "GenSnd.h"
#include "GenPriv.h"

#if LOOPS_USED == LIMITED_LOOPS

#if WRITE_LOOPS == TRUE
	#include <stdio.h>
#endif

#if USE_TERP2 == TRUE
// handle 8 bit voices that are mixed down mono in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeInterp2FullBuffer (GM_Voice *this_voice)
{
#if 1
	this_voice;
#else
// Not required because 8-bit samples are converted to 16-bit upon load.
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register UBYTE 			*source, *calculated_source;
	register INT32			b, c;
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeInterp2FullBufferNewReverb (this_voice); 
		return;
	}
#endif
	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop;
	dest = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave = this_voice->NoteWave;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (this_voice->channels == 1)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
			b = calculated_source[0];
			c = calculated_source[1];
			*dest += ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) * amplitude;
			cur_wave += wave_increment;

			calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
			b = calculated_source[0];
			c = calculated_source[1];
			dest[1] += ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) * amplitude;
			cur_wave += wave_increment;

			calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
			b = calculated_source[0];
			c = calculated_source[1];
			dest[2] += ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) * amplitude;
			cur_wave += wave_increment;

			calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
			b = calculated_source[0];
			c = calculated_source[1];
			dest[3] += ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) * amplitude;
			cur_wave += wave_increment;
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
				calculated_source = source + ((cur_wave>> STEP_BIT_RANGE) * 2);
				b = calculated_source[0] + calculated_source[1];	// average left & right channels
				c = calculated_source[2] + calculated_source[3];
				*dest += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x100) * amplitude) >> 1;
				dest++;
				cur_wave += wave_increment;
			}
			amplitude += amplitudeAdjust;
		}
	}
	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitude;
#endif
}
#endif	// USE_TERP2

#if (USE_TERP2 == TRUE) || (USE_DROP_SAMPLE == TRUE)
// handle 8 bit voices that are mixed down mono in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeInterp2PartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
#if 1
	this_voice;
	looping;
#else
// Not required because 8-bit samples are converted to 16-bit upon load.
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register UBYTE 			*source, *calculated_source;
	register INT32			b, c;
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register XFIXED 		end_wave, wave_adjust;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeInterp2PartialBufferNewReverb (this_voice, looping);
		return;
	}
#endif
	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop;
	dest = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave = this_voice->NoteWave;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (looping)
	{
		end_wave = (XFIXED) (this_voice->NoteLoopEnd - this_voice->NotePtr) << STEP_BIT_RANGE;
		wave_adjust = (XFIXED) (this_voice->NoteLoopEnd - this_voice->NoteLoopPtr) << STEP_BIT_RANGE;
	}
	else
	{
		end_wave = (XFIXED) (this_voice->NotePtrEnd - this_voice->NotePtr - 1) << STEP_BIT_RANGE;
	}

	if (this_voice->channels == 1)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			for (inner = 0; inner < 4; inner++)
			{
				THE_CHECK(UBYTE *);
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				*dest += ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) * amplitude;
				dest++;
				cur_wave += wave_increment;
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
				THE_CHECK(UBYTE *);
				calculated_source = source + ( (cur_wave>> STEP_BIT_RANGE) * 2);
				b = calculated_source[0] + calculated_source[1];
				c = calculated_source[2] + calculated_source[3];
				*dest += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x100) * amplitude) >> 1;
				dest++;
				cur_wave += wave_increment;
			}
			amplitude += amplitudeAdjust;
		}
	}

	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitude;
FINISH:
	return;
#endif
}
#endif	// USE_TERP2

#if USE_TERP2 == TRUE
// handle 8 bit voices that are mixed down stereo in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeStereoInterp2FullBuffer(GM_Voice *this_voice)
{
	// dead function placed here as a placeholder. Will be removed later.
	this_voice;
}
#endif	// USE_TERP2

#if (USE_TERP2 == TRUE) || (USE_DROP_SAMPLE == TRUE)
// handle 8 bit voices that are mixed down stereo in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeStereoInterp2PartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
	// dead function placed here as a placeholder. Will be removed later.
	this_voice;
	looping;
}
#endif	// USE_TERP2

// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее

// 16 bit cases
#if USE_TERP2 == TRUE
// handle 16 bit voices that are mixed down mono in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeInterp2FullBuffer16 (GM_Voice *this_voice)
{
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register INT16 			*source, *calculated_source;
	register INT32			b, c, sample;
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeInterp2FullBuffer16NewReverb (this_voice); 
		return;
	}
#endif

#if WRITE_LOOPS == TRUE
	FILE *file = fopen("PV_ServeInterp2FullBuffer16.da1", "a+b");	// append to file; binary write
	fprintf(file, "BEGIN-");
	fprintf(file, "Time: %ld", GM_GetSyncTimeStamp());
	fwrite(&this_voice->lastAmplitudeL, sizeof(this_voice->lastAmplitudeL), 1, file);
	fwrite(&this_voice->NoteVolume, sizeof(this_voice->NoteVolume), 1, file);
	fwrite(&this_voice->NoteVolumeEnvelope, sizeof(this_voice->NoteVolumeEnvelope), 1, file);
	fwrite(&this_voice->NotePitch, sizeof(this_voice->NotePitch), 1, file);
	fwrite(&this_voice->NoteWave, sizeof(this_voice->NoteWave), 1, file);
	fwrite(&MusicGlobals->Four_Loop, sizeof(MusicGlobals->Four_Loop), 1, file);
	fwrite(&this_voice->voiceMode, sizeof(this_voice->voiceMode), 1, file);
	fwrite(&this_voice->NotePtr, sizeof(this_voice->NotePtr), 1, file);
	fwrite(&this_voice->NotePtrEnd, sizeof(this_voice->NotePtrEnd), 1, file);
	fwrite(&this_voice->NoteLoopPtr, sizeof(this_voice->NoteLoopPtr), 1, file);
	fwrite(&this_voice->NoteLoopEnd, sizeof(this_voice->NoteLoopEnd), 1, file);
	fwrite(this_voice->NotePtr, this_voice->NotePtrEnd-this_voice->NotePtr, 1, file);
	fwrite(MusicGlobals->songBufferDry, MusicGlobals->One_Slice, 1, file);
#endif

	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop >> 4;
	amplitude = amplitude >> 4;

	dest = &MusicGlobals->songBufferDry[0];
	source = (short *) this_voice->NotePtr;
	cur_wave = this_voice->NoteWave;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (this_voice->channels == 1)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			b = source[cur_wave>>STEP_BIT_RANGE];
			c = source[(cur_wave>>STEP_BIT_RANGE)+1];
			*dest += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;

			cur_wave += wave_increment;
			b = source[cur_wave>>STEP_BIT_RANGE];
			c = source[(cur_wave>>STEP_BIT_RANGE)+1];
			dest[1] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;

			cur_wave += wave_increment;
			b = source[cur_wave>>STEP_BIT_RANGE];
			c = source[(cur_wave>>STEP_BIT_RANGE)+1];
			dest[2] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;

			cur_wave += wave_increment;
			b = source[cur_wave>>STEP_BIT_RANGE];
			c = source[(cur_wave>>STEP_BIT_RANGE)+1];
			dest[3] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
			dest += 4;
			cur_wave += wave_increment;
			amplitude += amplitudeAdjust;
		}
	}
	else
	{	// stereo 16 bit instrument
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			for (inner = 0; inner < 4; inner++)
			{
				calculated_source = source + ((cur_wave>> STEP_BIT_RANGE) * 2);
				b = calculated_source[0] + calculated_source[1];
				c = calculated_source[2] + calculated_source[3];
				sample = (((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b;
				*dest += (sample  * amplitude) >> 5;	// divide extra for summed stereo channels
				dest++;

				cur_wave += wave_increment;
			}
			amplitude += amplitudeAdjust;
		}
	}

	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitude << 4;

#if WRITE_LOOPS == TRUE
	fwrite(&this_voice->NoteWave, sizeof(this_voice->NoteWave), 1, file);
	fwrite(&this_voice->lastAmplitudeL, sizeof(this_voice->lastAmplitudeL), 1, file);
	fwrite(MusicGlobals->songBufferDry, MusicGlobals->One_Slice, 1, file);
	fprintf(file, "-END");
	fclose(file);
#endif
}
#endif	// USE_TERP2

#if (USE_TERP2 == TRUE) || (USE_DROP_SAMPLE == TRUE)
// handle 16 bit voices that are mixed down mono in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeInterp2PartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*dest;
	register LOOPCOUNT		a, inner;
	register INT16 			*source, *calculated_source;
	register INT32			b, c, sample;
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register XFIXED 		end_wave, wave_adjust;
	register INT32			amplitude, amplitudeAdjust;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel || this_voice->chorusLevel)
	{
		PV_ServeInterp2PartialBuffer16NewReverb (this_voice, looping); 
		return;
	}
#endif
#if WRITE_LOOPS == TRUE
	FILE *file = fopen("PV_ServeInterp2PartialBuffer16.da1", "a+b");	// append to file; binary write
	fprintf(file, "BEGIN-");
	fprintf(file, "Time: %ld", GM_GetSyncTimeStamp());
	fwrite(&this_voice->lastAmplitudeL, sizeof(this_voice->lastAmplitudeL), 1, file);
	fwrite(&this_voice->NoteVolume, sizeof(this_voice->NoteVolume), 1, file);
	fwrite(&this_voice->NoteVolumeEnvelope, sizeof(this_voice->NoteVolumeEnvelope), 1, file);
	fwrite(&this_voice->NotePitch, sizeof(this_voice->NotePitch), 1, file);
	fwrite(&this_voice->NoteWave, sizeof(this_voice->NoteWave), 1, file);
	fwrite(&MusicGlobals->Four_Loop, sizeof(MusicGlobals->Four_Loop), 1, file);
	fwrite(&this_voice->voiceMode, sizeof(this_voice->voiceMode), 1, file);
	fwrite(&this_voice->NotePtr, sizeof(this_voice->NotePtr), 1, file);
	fwrite(&this_voice->NotePtrEnd, sizeof(this_voice->NotePtrEnd), 1, file);
	fwrite(&this_voice->NoteLoopPtr, sizeof(this_voice->NoteLoopPtr), 1, file);
	fwrite(&this_voice->NoteLoopEnd, sizeof(this_voice->NoteLoopEnd), 1, file);
	fwrite(this_voice->NotePtr, this_voice->NotePtr-this_voice->NotePtrEnd, 1, file);
	fwrite(MusicGlobals->songBufferDry, MusicGlobals->One_Slice, 1, file);
	fprintf(file, "-END");
	fclose(file);
#endif

	amplitude = this_voice->lastAmplitudeL;
	amplitudeAdjust = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeAdjust = (amplitudeAdjust - amplitude) / MusicGlobals->Four_Loop >> 4;
	amplitude = amplitude >> 4;

	dest = &MusicGlobals->songBufferDry[0];
	cur_wave = this_voice->NoteWave;
	source = (short *) this_voice->NotePtr;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (looping)
	{
		end_wave = (XFIXED) (this_voice->NoteLoopEnd - this_voice->NotePtr) << STEP_BIT_RANGE;
		wave_adjust = (XFIXED) (this_voice->NoteLoopEnd - this_voice->NoteLoopPtr) << STEP_BIT_RANGE;
	}
	else
	{
		end_wave = (XFIXED) (this_voice->NotePtrEnd - this_voice->NotePtr - 1) << STEP_BIT_RANGE;
	}

	if (this_voice->channels == 1)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			if (cur_wave + (wave_increment << 2) >= end_wave)
			{
				THE_CHECK(INT16 *);
				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				*dest += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
				cur_wave += wave_increment;
				THE_CHECK(INT16 *);
				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				dest[1] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
				cur_wave += wave_increment;
				THE_CHECK(INT16 *);
				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				dest[2] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
				cur_wave += wave_increment;
				THE_CHECK(INT16 *);
				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				dest[3] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
			}
			else
			{
				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				*dest += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
				cur_wave += wave_increment;

				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				dest[1] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
				cur_wave += wave_increment;

				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				dest[2] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
				cur_wave += wave_increment;

				calculated_source = source + (cur_wave>> STEP_BIT_RANGE);
				b = *calculated_source;
				c = calculated_source[1];
				dest[3] += (((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) * amplitude) >> 4;
			}
			dest += 4;
			cur_wave += wave_increment;
			amplitude += amplitudeAdjust;
		}
	}
	else
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			for (inner = 0; inner < 4; inner++)
			{
				THE_CHECK(INT16 *);
				calculated_source = source + ((cur_wave>> STEP_BIT_RANGE) * 2);
				b = calculated_source[0] + calculated_source[1];
				c = calculated_source[2] + calculated_source[3];
				sample = (((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b;
				*dest += ((sample >> 1) * amplitude) >> 5;
				dest++;
				cur_wave += wave_increment;
			}
			amplitude += amplitudeAdjust;
		}
	}

	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitude << 4;
FINISH:
#if WRITE_LOOPS == TRUE
	fwrite(&this_voice->NoteWave, sizeof(this_voice->NoteWave), 1, file);
	fwrite(&this_voice->lastAmplitudeL, sizeof(this_voice->lastAmplitudeL), 1, file);
	fwrite(MusicGlobals->songBufferDry, MusicGlobals->One_Slice, 1, file);
	fprintf(file, "-END");
	fclose(file);
#endif
	return;
}
#endif	// USE_TERP2

#if (USE_TERP2 == TRUE) || (USE_DROP_SAMPLE == TRUE)
// handle 16 bit voices that are mixed down mono in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeStereoInterp2FullBuffer16 (GM_Voice *this_voice)
{
	// dead function placed here as a placeholder. Will be removed later.
	this_voice;
}

#endif	// USE_TERP2

#if (USE_TERP2 == TRUE) || (USE_DROP_SAMPLE == TRUE)
// handle 8 bit voices that are mixed down stereo in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeStereoInterp2PartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	// dead function placed here as a placeholder. Will be removed later.
	this_voice;
	looping;
}
#endif	// USE_TERP2

#endif	// LOOPS_USED

// EOF of GenSynthInterp2.c

