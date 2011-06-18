/*
 * %W% %E%
 *
 * Copyright (c) 1996 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 *
 */

/*
 * g723_24.c
 *
 * Description:
 *
 * g723_24_encoder(), g723_24_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 24 Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which take advantage
 * of workstation attributes, such as hardware 2's complement arithmetic.
 *
 */
#include "g72x.h"

#if USE_HIGHLEVEL_FILE_API != FALSE

/*
 * Maps G.723_24 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static const short  _dqlntab[8] = {-2048, 135, 273, 373, 373, 273, 135, -2048};

/* Maps G.723_24 code word to log of scale factor multiplier. */
static const short  _witab[8] = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};

/*
 * Maps G.723_24 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static const short _fitab[8] = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};

static const short qtab_723_24[3] = {8, 218, 331};

#if 0
/*
 * g723_24_encoder()
 *
 * Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
 * Returns -1 if invalid input coding value.
 */
int
g723_24_encoder(
    int     sl,
    int     in_coding,
    struct g72x_state *state_ptr)
{
    short       sei, sezi, se, sez; /* ACCUM */
    short       d;          /* SUBTA */
    short       y;          /* MIX */
    short       sr;         /* ADDB */
    short       dqsez;          /* ADDC */
    short       dq, i;

    switch (in_coding) {    /* linearize input sample to 14-bit PCM */
    case AUDIO_ENCODING_ALAW:
        sl = alaw2linear(sl) >> 2;
        break;
    case AUDIO_ENCODING_ULAW:
        sl = ulaw2linear(sl) >> 2;
        break;
    case AUDIO_ENCODING_LINEAR:
        sl >>= 2;       /* sl of 14-bit dynamic range */
        break;
    default:
        return (-1);
    }

    sezi = predictor_zero(state_ptr);
    sez = sezi >> 1;
    sei = sezi + predictor_pole(state_ptr);
    se = sei >> 1;          /* se = estimated signal */

    d = sl - se;            /* d = estimation diff. */

    /* quantize prediction difference d */
    y = step_size(state_ptr);   /* quantizer step size */
    i = quantize(d, y, qtab_723_24, 3); /* i = ADPCM code */
    dq = reconstruct(i & 4, _dqlntab[i], y); /* quantized diff. */

    sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq; /* reconstructed signal */

    dqsez = sr + sez - se;      /* pole prediction diff. */

    update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

    return (i);
}
#endif

/*
 * g723_24_decoder()
 *
 * Decodes a 3-bit CCITT G.723_24 ADPCM code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int
g723_24_decoder(
    int     i,
    int     out_coding,
    struct g72x_state *state_ptr)
{
    short       sezi, sei, sez, se; /* ACCUM */
    short       y;          /* MIX */
    short       sr;         /* ADDB */
    short       dq;
    short       dqsez;

    i &= 0x07;          /* mask to get proper bits */
    sezi = predictor_zero(state_ptr);
    sez = sezi >> 1;
    sei = sezi + predictor_pole(state_ptr);
    se = sei >> 1;          /* se = estimated signal */

    y = step_size(state_ptr);   /* adaptive quantizer step size */
    dq = reconstruct(i & 0x04, _dqlntab[i], y); /* unquantize pred diff */

    sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq); /* reconst. signal */

    dqsez = sr - se + sez;          /* pole prediction diff. */

    update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

    switch (out_coding) {
    case AUDIO_ENCODING_ALAW:
        return (tandem_adjust_alaw(sr, se, y, i, 4, (short *)qtab_723_24));
    case AUDIO_ENCODING_ULAW:
        return (tandem_adjust_ulaw(sr, se, y, i, 4, (short *)qtab_723_24));
    case AUDIO_ENCODING_LINEAR:
        return (sr << 2);   /* sr was of 14-bit dynamic range */
    default:
        return (-1);
    }
}
#endif
