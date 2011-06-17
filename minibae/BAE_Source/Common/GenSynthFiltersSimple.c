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
**
** "GenSynthFiltersSimple.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	й Copyright 1995-1999 Beatnik, Inc, All Rights Reserved.
**	Written by Jim Nitchals
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
**				Changed the macro 'THE_CHECK' to accept a type for typecasting 
**				the source pointer
**	3/1/96		Removed extra PV_DoCallBack, and PV_GetWavePitch
**	5/2/96		Changed int to BOOL_FLAG
**	6/30/96		Changed false to FALSE
**				Changed font and re tabbed
**	7/6/96		Fixed compiler warnings
**				Reduced Beatnik for 16 bit samples for low pass filters
**	7/8/96		Improved enveloping and wave shaping code
**	12/30/96	Changed copyright
**	6/4/97		Added USE_SMALL_MEMORY_REVERB tests around code to disable when this
**				flag is used
**	12/16/97	Moe: removed compiler warnings
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
**	11/23/98	Added support for Intel Katmai CPU
**	1/4/99		Removed FAR macro. Re ordered Katmai code and duplicated inner loops
**	3/12/99		Added macros to control compile time.
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

#if WRITE_LOOPS == TRUE
	#include <stdio.h>
#endif

#if ((LOOPS_USED==LIMITED_LOOPS) && (USE_DROP_SAMPLE==TRUE || USE_TERP1==TRUE || USE_TERP2==TRUE))

#define CLIP(LIMIT_VAR, LIMIT_LOWER, LIMIT_UPPER) if (LIMIT_VAR < LIMIT_LOWER) LIMIT_VAR = LIMIT_LOWER; if (LIMIT_VAR > LIMIT_UPPER) LIMIT_VAR = LIMIT_UPPER;
#define GET_FILTER_PARAMS \
	CLIP (this_voice->LPF_frequency, 0x200, MAXRESONANCE*256);	\
	if (this_voice->previous_zFrequency == 0)\
		this_voice->previous_zFrequency = this_voice->LPF_frequency;\
	CLIP (this_voice->LPF_resonance, 0, 0x100);\
	CLIP (this_voice->LPF_lowpassAmount, -0xFF, 0xFF);\
	Z1 = this_voice->LPF_lowpassAmount << 8;\
	if (Z1 < 0)\
		Xn = 65536 + Z1;\
	else\
		Xn = 65536 - Z1;\
	if (Z1 >= 0)\
	{\
		Zn = ((0x10000 - Z1) * this_voice->LPF_resonance) >> 8;\
		Zn = -Zn;\
	}\
	else\
		Zn = 0;



// handle 8 bit voices that are mixed down mono in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeInterp2FilterPartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
#if 1
	this_voice;
	looping;
#else
// Not required because 8-bit samples are converted to 16-bit upon load.
	register INT32 			*destL;
	register UBYTE 			*source;
#if 1	// MOE'S OBSESSIVE FOLLY
	register INT32			b, c;
#else
	register UBYTE			b, c;
#endif
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register XFIXED 		end_wave, wave_adjust;
	register INT32			amplitudeL;
	register INT32			inner;

	INT32					amplitudeLincrement;
	INT32					ampValueL;
	INT32					a;

	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeInterp2FilterPartialBufferNewReverb (this_voice, looping); 
		return;
	}
#endif
	z = this_voice->z;
	Z1value = this_voice->Z1value;
	zIndex2 = this_voice->zIndex;

	GET_FILTER_PARAMS

	amplitudeL = this_voice->lastAmplitudeL;
	ampValueL = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeLincrement = (ampValueL - amplitudeL) / MusicGlobals->Four_Loop;
	
	amplitudeL = amplitudeL >> 2;
	amplitudeLincrement = amplitudeLincrement >> 2;

	destL = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave = this_voice->NoteWave;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (looping)
	{
		end_wave = (XFIXED) (this_voice->NoteLoopEnd - this_voice->NotePtr) << STEP_BIT_RANGE;
		wave_adjust = (this_voice->NoteLoopEnd - this_voice->NoteLoopPtr) << STEP_BIT_RANGE;
	}
	else
	{
		end_wave = (XFIXED) (this_voice->NotePtrEnd - this_voice->NotePtr - 1) << STEP_BIT_RANGE;
	}

	if (this_voice->LPF_resonance == 0)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			for (inner = 0; inner < 4; inner++)
			{
				THE_CHECK(UBYTE *);
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				sample = ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) << 2;
				sample = (sample * Xn + Z1value * Z1) >> 16;
				Z1value = sample - (sample >> 9);	// remove DC bias
				*destL += sample * amplitudeL;
				destL++;
				cur_wave += wave_increment;
			}
			amplitudeL += amplitudeLincrement;
		}
	}
	else
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			this_voice->previous_zFrequency += (this_voice->LPF_frequency - this_voice->previous_zFrequency) >> 5;
			zIndex1 = zIndex2 - (this_voice->previous_zFrequency >> 8);

			for (inner = 0; inner < 4; inner++)
			{
				THE_CHECK(UBYTE *);
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				sample = ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) << 2;
				sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
				zIndex1++;
				z[zIndex2 & MAXRESONANCE] = (INT16)sample;
				zIndex2++;
				Z1value = sample - (sample >> 9);
				*destL += sample * amplitudeL;
				destL++;
				cur_wave += wave_increment;
			}
			amplitudeL += amplitudeLincrement;
		}
	}

	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitudeL << 2;
FINISH:
	return;
#endif
}

// handle 8 bit voices that are mixed down stereo in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeStereoInterp2FilterPartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
	this_voice;
	looping;
}

// handle 8 bit voices that are mixed down mono in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeInterp2FilterFullBuffer(GM_Voice *this_voice)
{
#if 1
	this_voice;
#else
// Not required because 8-bit samples are converted to 16-bit upon load.
	register INT32 			*destL;
	register UBYTE 			*source;
#if 1	// MOE'S OBSESSIVE FOLLY
	register INT32			b, c;
#else
	register UBYTE			b, c;
#endif
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register INT32			amplitudeL;
	register INT32			inner;

	INT32					amplitudeLincrement;
	INT32					ampValueL;
	INT32					a;

	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

	// We can't filter stereo samples, so bail on this.
	if (this_voice->channels > 1) 
	{
		PV_ServeInterp2PartialBuffer (this_voice, FALSE); 
		return; 
	}
#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeInterp2FilterFullBufferNewReverb (this_voice); 
		return;
	}
#endif
	z = this_voice->z;
	Z1value = this_voice->Z1value;
	zIndex2 = this_voice->zIndex;

	GET_FILTER_PARAMS

	amplitudeL = this_voice->lastAmplitudeL;
	ampValueL = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeLincrement = (ampValueL - amplitudeL) / MusicGlobals->Four_Loop;
	
	amplitudeL = amplitudeL >> 2;
	amplitudeLincrement = amplitudeLincrement >> 2;

	destL = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave = this_voice->NoteWave;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (this_voice->LPF_resonance == 0)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			for (inner = 0; inner < 4; inner++)
			{
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				sample = ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) << 2;
				sample = (sample * Xn + Z1value * Z1) >> 16;
				Z1value = sample - (sample >> 9);	// remove DC bias
				*destL += sample * amplitudeL;
				destL++;
				cur_wave += wave_increment;
			}
			amplitudeL += amplitudeLincrement;
		}
	}
	else
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			this_voice->previous_zFrequency += (this_voice->LPF_frequency - this_voice->previous_zFrequency) >> 5;
			zIndex1 = zIndex2 - (this_voice->previous_zFrequency >> 8);

			for (inner = 0; inner < 4; inner++)
			{
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				sample = ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b - 0x80) << 2;
				sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
				zIndex1++;
				z[zIndex2 & MAXRESONANCE] = (INT16)sample;
				zIndex2++;
				Z1value = sample - (sample >> 9);
				*destL += sample * amplitudeL;
				destL++;
				cur_wave += wave_increment;
			}
			amplitudeL += amplitudeLincrement;
		}
	}

	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitudeL << 2;
#endif
}


// handle 8 bit voices that are mixed down stereo in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeStereoInterp2FilterFullBuffer (GM_Voice *this_voice)
{
	this_voice;
}

// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// 16 bit cases

// handle 16 bit voices that are mixed down mono in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeInterp2FilterFullBuffer16 (GM_Voice *this_voice)
{
	PV_ServeInterp2FilterPartialBuffer16 (this_voice, FALSE);
}

// handle 16 bit voices that are mixed down mono in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeInterp2FilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register INT16 			*source;
	register INT16			b, c;
	register XFIXED 		cur_wave;
	register XFIXED 		wave_increment;
	register XFIXED 		end_wave, wave_adjust;
	register INT32			amplitudeL;
	register INT32			inner;

	INT32					amplitudeLincrement;
	INT32					ampValueL;
	INT32					a;

	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

#if WRITE_LOOPS == TRUE
	FILE *file = NULL;
#endif

	if (this_voice->channels > 1) 
	{ 
		PV_ServeInterp2PartialBuffer16 (this_voice, looping); 
		return; 
	}
#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeInterp2FilterPartialBufferNewReverb16 (this_voice, looping); 
		return;
	}
#endif
#if WRITE_LOOPS == TRUE
	file = fopen("PV_ServeInterp2FilterPartialBuffer16.da2", "a+b");	// append to file; binary write
	fprintf(file, "BEGIN-");
	fprintf(file, "Time: %ld", GM_GetSyncTimeStamp());
	fwrite(this_voice->z, sizeof(this_voice->z), 1, file);
	fwrite(&this_voice->Z1value, sizeof(this_voice->Z1value), 1, file);
	fwrite(&this_voice->zIndex, sizeof(this_voice->zIndex), 1, file);
	fwrite(&this_voice->LPF_resonance, sizeof(this_voice->LPF_resonance), 1, file);
	fwrite(&this_voice->LPF_frequency, sizeof(this_voice->LPF_frequency), 1, file);
	fwrite(&this_voice->previous_zFrequency, sizeof(this_voice->previous_zFrequency), 1, file);

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

	z = this_voice->z;
	Z1value = this_voice->Z1value;
	zIndex2 = this_voice->zIndex;

	GET_FILTER_PARAMS

	amplitudeL = this_voice->lastAmplitudeL;
	ampValueL = (this_voice->NoteVolume * this_voice->NoteVolumeEnvelope) >> VOLUME_PRECISION_SCALAR;
	amplitudeLincrement = (ampValueL - amplitudeL) / MusicGlobals->Four_Loop;

	destL = &MusicGlobals->songBufferDry[0];
	source = (short *) this_voice->NotePtr;
	cur_wave = this_voice->NoteWave;

	wave_increment = PV_GetWavePitch(this_voice->NotePitch);

	if (looping)
	{
		end_wave = (XFIXED) (this_voice->NoteLoopEnd - this_voice->NotePtr) << STEP_BIT_RANGE;
		wave_adjust = (this_voice->NoteLoopEnd - this_voice->NoteLoopPtr) << STEP_BIT_RANGE;
	}
	else
	{
		end_wave = (XFIXED) (this_voice->NotePtrEnd - this_voice->NotePtr - 1) << STEP_BIT_RANGE;
	}

	if (this_voice->LPF_resonance == 0)
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			for (inner = 0; inner < 4; inner++)
			{
				THE_CHECK(INT16 *);		// is in the mail
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				sample = ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) >> 6;
				sample = (sample * Xn + Z1value * Z1) >> 16;
				Z1value = sample - (sample >> 9);	// remove DC bias
				*destL += (sample * amplitudeL) >> 2;
				destL++;
				cur_wave += wave_increment;
			}
			amplitudeL += amplitudeLincrement;
		}
	}
	else
	{
		for (a = MusicGlobals->Four_Loop; a > 0; --a)
		{
			this_voice->previous_zFrequency += (this_voice->LPF_frequency - this_voice->previous_zFrequency) >> 5;
			zIndex1 = zIndex2 - (this_voice->previous_zFrequency >> 8);

			for (inner = 0; inner < 4; inner++)
			{
				THE_CHECK(INT16 *);		// is in the mail
				b = source[cur_wave>>STEP_BIT_RANGE];
				c = source[(cur_wave>>STEP_BIT_RANGE)+1];
				sample = ((((INT32) (cur_wave & STEP_FULL_RANGE) * (c-b))>>STEP_BIT_RANGE) + b) >> 6;
				sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
				zIndex1++;
				z[zIndex2 & MAXRESONANCE] = (INT16)sample;
				zIndex2++;
				Z1value = sample - (sample >> 9);
				*destL += (sample * amplitudeL) >> 2;
				destL++;
				cur_wave += wave_increment;
			}
			amplitudeL += amplitudeLincrement;
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->NoteWave = cur_wave;
	this_voice->lastAmplitudeL = amplitudeL;
FINISH:
#if WRITE_LOOPS == TRUE
	fwrite(&this_voice->NoteWave, sizeof(this_voice->NoteWave), 1, file);
	fwrite(&this_voice->lastAmplitudeL, sizeof(this_voice->lastAmplitudeL), 1, file);
	fwrite(&this_voice->Z1value, sizeof(this_voice->Z1value), 1, file);
	fwrite(&this_voice->zIndex, sizeof(this_voice->zIndex), 1, file);
	fwrite(MusicGlobals->songBufferDry, MusicGlobals->One_Slice, 1, file);
	fprintf(file, "-END");
	fclose(file);
#endif
	return;
}

// handle 8 bit voices that are mixed down stereo in the full case in which we can 
// process a complete slice of data without checking for loop points
void PV_ServeStereoInterp2FilterFullBuffer16 (GM_Voice *this_voice)
{
	PV_ServeStereoInterp2FilterPartialBuffer16 (this_voice, FALSE);
}


// handle 16 bit voices that are mixed down stereo in the partial case in which we can 
// process a complete slice of data but we check for loop points every 4 samples 
// with the macro THE_CHECK
void PV_ServeStereoInterp2FilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	this_voice;
	looping;
}
#endif	// (USE_TERP2 == TRUE) || (USE_DROP_SAMPLE == TRUE) || (USE_TERP1 == TRUE)

// EOF GenSynthFilters.c
