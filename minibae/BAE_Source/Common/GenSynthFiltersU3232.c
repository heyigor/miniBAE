/*****************************************************************************/
/*
**
** "GenSynthFiltersU3232.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**	й Copyright 1995-2000 Beatnik, Inc, All Rights Reserved.
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
**	3/11/99		MOE: created file from GenSynthFiltersFloat.c
**	10/19/99	MSD: switched to REVERB_USED and LOOPS_USED
**	2/4/2000	Changed copyright. We're Y2K compliant!
**	1/27/2002	sh Fixed warnings.
*/
/*****************************************************************************/

#include "GenSnd.h"
#include "GenPriv.h"


#if LOOPS_USED == U3232_LOOPS

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



void PV_ServeU3232FilterPartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register UBYTE 			*source;
#if 1	// MOE'S OBSESSIVE FOLLY
	register INT32			b, c;
#else
	register UBYTE			b, c;
#endif
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
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
		PV_ServeU3232FilterPartialBufferNewReverb (this_voice, looping); 
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
		if (this_voice->LPF_resonance == 0)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1) >> 16;
					Z1value = sample - (sample >> 9);	// remove DC bias
					*destL += sample * amplitudeL;
					destL++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
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
					THE_CHECK_U3232(UBYTE *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
					zIndex1++;
					z[zIndex2 & MAXRESONANCE] = (INT16)sample;
					zIndex2++;
					Z1value = sample - (sample >> 9);
					*destL += sample * amplitudeL;
					destL++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
			}
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL << 2;
FINISH:
	return;
}

void PV_ServeU3232StereoFilterPartialBuffer (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register UBYTE 			*source;
#if 1	// MOE'S OBSESSIVE FOLLY
	register INT32			b, c;
#else
	register UBYTE			b, c;
#endif
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			inner;
	INT32					amplitudeLincrement, amplitudeRincrement;
	INT32					ampValueL, ampValueR;
	INT32					a;
	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

	if (this_voice->channels > 1) 
	{
		PV_ServeU3232StereoPartialBuffer (this_voice, looping); 
		return; 
	}

#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{ 
		PV_ServeU3232StereoFilterPartialBufferNewReverb (this_voice, looping); 
		return; 
	}
#endif
	z = this_voice->z;
	Z1value = this_voice->Z1value;
	zIndex2 = this_voice->zIndex;

	GET_FILTER_PARAMS

	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);

	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = ((ampValueL - amplitudeL) / MusicGlobals->Four_Loop) >> 2;
	amplitudeRincrement = ((ampValueR - amplitudeR) / MusicGlobals->Four_Loop) >> 2;

	amplitudeL = amplitudeL >> 2;
	amplitudeR = amplitudeR >> 2;

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
		if (this_voice->LPF_resonance == 0)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1) >> 16;
					Z1value = sample - (sample >> 9);
					destL[0] += sample * amplitudeL;
					destL[1] += sample * amplitudeR;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				zIndex1 = zIndex2 - (this_voice->previous_zFrequency >> 8);
				this_voice->previous_zFrequency += (this_voice->LPF_frequency - this_voice->previous_zFrequency) >> 3;

				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(UBYTE *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
					zIndex1++;
					z[zIndex2 & MAXRESONANCE] = (INT16)sample;
					zIndex2++;
					Z1value = sample - (sample >> 9);
					destL[0] += sample * amplitudeL;
					destL[1] += sample * amplitudeR;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL << 2;
	this_voice->lastAmplitudeR = amplitudeR << 2;
FINISH:
	return;
}


void PV_ServeU3232FilterFullBuffer(GM_Voice *this_voice)
{
	register INT32 			*destL;
	register UBYTE 			*source;
#if 1	// MOE'S OBSESSIVE FOLLY
	register INT32			b, c;
#else
	register UBYTE			b, c;
#endif
	register U32	 		cur_wave_i, cur_wave_f;
	U3232			 		wave_increment;
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
		PV_ServeU3232PartialBuffer (this_voice, FALSE); 
		return; 
	}
#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeU3232FilterFullBufferNewReverb (this_voice); 
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
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	{
		if (this_voice->LPF_resonance == 0)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1) >> 16;
					Z1value = sample - (sample >> 9);	// remove DC bias
					*destL += sample * amplitudeL;
					destL++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
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
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
					zIndex1++;
					z[zIndex2 & MAXRESONANCE] = (INT16)sample;
					zIndex2++;
					Z1value = sample - (sample >> 9);
					*destL += sample * amplitudeL;
					destL++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
			}
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL << 2;
}



void PV_ServeU3232StereoFilterFullBuffer (GM_Voice *this_voice)
{
	register INT32 			*destL;
	register UBYTE 			*source;
#if 1	// MOE'S OBSESSIVE FOLLY
	register INT32			b, c;
#else
	register UBYTE			b, c;
#endif
	register U32	 		cur_wave_i, cur_wave_f;
	U3232			 		wave_increment;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			inner;
	INT32					amplitudeLincrement, amplitudeRincrement;
	INT32					ampValueL, ampValueR;
	INT32					a;

	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

	if (this_voice->channels > 1) 
	{
		PV_ServeU3232StereoPartialBuffer (this_voice, FALSE); 
		return; 
	}
#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeU3232StereoFilterFullBufferNewReverb (this_voice); 
		return;
	}
#endif
	z = this_voice->z;
	Z1value = this_voice->Z1value;
	zIndex2 = this_voice->zIndex;

	GET_FILTER_PARAMS

	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);

	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = ((ampValueL - amplitudeL) / MusicGlobals->Four_Loop) >> 2;
	amplitudeRincrement = ((ampValueR - amplitudeR) / MusicGlobals->Four_Loop) >> 2;

	amplitudeL = amplitudeL >> 2;
	amplitudeR = amplitudeR >> 2;

	destL = &MusicGlobals->songBufferDry[0];
	source = this_voice->NotePtr;
	cur_wave_i = this_voice->samplePosition.i;
	cur_wave_f = this_voice->samplePosition.f;

	wave_increment = PV_GetWavePitchU3232(this_voice->NotePitch);

	{
		if (this_voice->LPF_resonance == 0)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1) >> 16;
					Z1value = sample - (sample >> 9);
					destL[0] += sample * amplitudeL;
					destL[1] += sample * amplitudeR;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				zIndex1 = zIndex2 - (this_voice->previous_zFrequency >> 8);
				this_voice->previous_zFrequency += (this_voice->LPF_frequency - this_voice->previous_zFrequency) >> 3;

				for (inner = 0; inner < 4; inner++)
				{
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 16) * (INT32)(c-b)) >> 16) + b - 0x80) << 2;
					sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
					zIndex1++;
					z[zIndex2 & MAXRESONANCE] = (INT16)sample;
					zIndex2++;
					Z1value = sample - (sample >> 9);
					destL[0] += sample * amplitudeL;
					destL[1] += sample * amplitudeR;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL << 2;
	this_voice->lastAmplitudeR = amplitudeR << 2;
}

// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее еееееееееееее ееееееееее
// 16 bit cases

void PV_ServeU3232FilterFullBuffer16 (GM_Voice *this_voice)
{
	PV_ServeU3232FilterPartialBuffer16 (this_voice, FALSE);
}

void PV_ServeU3232FilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register INT16 			*source;
	register INT16			b, c;
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	register INT32			amplitudeL;
	register INT32			inner;

	INT32					amplitudeLincrement;
	INT32					ampValueL;
	INT32					a;

	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

	if (this_voice->channels > 1) 
	{ 
		PV_ServeU3232PartialBuffer16 (this_voice, looping); 
		return; 
	}
#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeU3232FilterPartialBufferNewReverb16 (this_voice, looping); 
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

	destL = &MusicGlobals->songBufferDry[0];
	source = (short *) this_voice->NotePtr;
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
		if (this_voice->LPF_resonance == 0)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);		// is in the mail
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) >> 6;
					sample = (sample * Xn + Z1value * Z1) >> 16;
					Z1value = sample - (sample >> 9);	// remove DC bias
					*destL += (sample * amplitudeL) >> 2;
					destL++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
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
					THE_CHECK_U3232(INT16 *);		// is in the mail
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) >> 6;
					sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
					zIndex1++;
					z[zIndex2 & MAXRESONANCE] = (INT16)sample;
					zIndex2++;
					Z1value = sample - (sample >> 9);
					*destL += (sample * amplitudeL) >> 2;
					destL++;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
			}
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL;
FINISH:
	return;
}


void PV_ServeU3232StereoFilterFullBuffer16 (GM_Voice *this_voice)
{
	PV_ServeU3232StereoFilterPartialBuffer16 (this_voice, FALSE);
}

void PV_ServeU3232StereoFilterPartialBuffer16 (GM_Voice *this_voice, XBOOL looping)
{
	register INT32 			*destL;
	register INT16 			*source;
	register INT16			b, c;
	register U32	 		cur_wave_i, cur_wave_f;
	register U32	 		end_wave, wave_adjust = 0;
	U3232	 				wave_increment;
	register INT32			amplitudeL;
	register INT32			amplitudeR;
	register INT32			inner;

	INT32					amplitudeLincrement, amplitudeRincrement;
	INT32					ampValueL, ampValueR;
	INT32					a;

	register INT16			*z;
	register INT32			Z1value, zIndex1, zIndex2, Xn, Z1, Zn, sample;

	if (this_voice->channels > 1) 
	{ 
		PV_ServeU3232StereoPartialBuffer16 (this_voice, looping); 
		return; 
	}
#if REVERB_USED == VARIABLE_REVERB
	if (this_voice->reverbLevel > 1 || this_voice->chorusLevel > 1)
	{
		PV_ServeU3232StereoFilterPartialBufferNewReverb16 (this_voice, looping); 
		return;
	}
#endif
	z = this_voice->z;
	Z1value = this_voice->Z1value;
	zIndex2 = this_voice->zIndex;

	GET_FILTER_PARAMS

	PV_CalculateStereoVolume(this_voice, &ampValueL, &ampValueR);

	amplitudeL = this_voice->lastAmplitudeL;
	amplitudeR = this_voice->lastAmplitudeR;
	amplitudeLincrement = (ampValueL - amplitudeL) / MusicGlobals->Four_Loop;
	amplitudeRincrement = (ampValueR - amplitudeR) / MusicGlobals->Four_Loop;

	destL = &MusicGlobals->songBufferDry[0];
	source = (short *) this_voice->NotePtr;
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
		if (this_voice->LPF_resonance == 0)
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);		// is in the mail
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) >> 6;
					sample = (sample * Xn + Z1value * Z1) >> 16;
					Z1value = sample - (sample >> 9);
					destL[0] += (sample * amplitudeL) >> 2;
					destL[1] += (sample * amplitudeR) >> 2;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
		else
		{
			for (a = MusicGlobals->Four_Loop; a > 0; --a)
			{
				zIndex1 = zIndex2 - (this_voice->previous_zFrequency >> 8);
				this_voice->previous_zFrequency += (this_voice->LPF_frequency - this_voice->previous_zFrequency) >> 3;

				for (inner = 0; inner < 4; inner++)
				{
					THE_CHECK_U3232(INT16 *);
					b = source[cur_wave_i];
					c = source[cur_wave_i+1];
					sample = ((((INT32)(cur_wave_f >> 17) * (INT32)(c-b)) >> 15) + b) >> 6;
					sample = (sample * Xn + Z1value * Z1 + z[zIndex1 & MAXRESONANCE] * Zn) >> 16;
					zIndex1++;
					z[zIndex2 & MAXRESONANCE] = (INT16)sample;
					zIndex2++;
					Z1value = sample - (sample >> 9);
					destL[0] += (sample * amplitudeL) >> 2;
					destL[1] += (sample * amplitudeR) >> 2;
					destL += 2;
					ADD_U3232(cur_wave_i, cur_wave_f, wave_increment);
				}
				amplitudeL += amplitudeLincrement;
				amplitudeR += amplitudeRincrement;
			}
		}
	}
	this_voice->Z1value = Z1value;
	this_voice->zIndex = zIndex2;
	this_voice->samplePosition.i = cur_wave_i;
	this_voice->samplePosition.f = cur_wave_f;
	this_voice->lastAmplitudeL = amplitudeL;
	this_voice->lastAmplitudeR = amplitudeR;
FINISH:
	return;
}

#endif	// LOOPS_USED == U3232_LOOPS

// EOF GenSynthFilters.c
