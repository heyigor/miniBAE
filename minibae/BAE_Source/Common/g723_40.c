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
 * g723_40.c
 *
 * Description:
 *
 * g723_40_encoder(), g723_40_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 40Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which
 * take advantage of workstation attributes, such as hardware 2's
 * complement arithmetic.
 *
 * The deviation from the bit level specification (lookup tables),
 * preserves the bit level performance specifications.
 *
 * As outlined in the G.723 Recommendation, the algorithm is broken
 * down into modules.  Each section of code below is preceded by
 * the name of the module which it is implementing.
 *
 */
#include "g72x.h"

#if USE_HIGHLEVEL_FILE_API != FALSE

/*
 * Maps G.723_40 code word to ructeconstructed scale factor normalized log
 * magnitude values.
 */
static const short  _dqlntab[32] = {-2048, -66, 28, 104, 169, 224, 274, 318,
                                    358, 395, 429, 459, 488, 514, 539, 566,
                                    566, 539, 514, 488, 459, 429, 395, 358,
                                    318, 274, 224, 169, 104, 28, -66, -2048};

/* Maps G.723_40 code word to log of scale factor multiplier. */
static const short _witab[32] = {448, 448, 768, 1248, 1280, 1312, 1856, 3200,
                                    4512, 5728, 7008, 8960, 11456, 14080, 16928, 22272,
                                    22272, 16928, 14080, 11456, 8960, 7008, 5728, 4512,
                                    3200, 1856, 1312, 1280, 1248, 768, 448, 448};

/*
 * Maps G.723_40 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static const short _fitab[32] = {0, 0, 0, 0, 0, 0x200, 0x200, 0x200,
                                0x200, 0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00, 0xC00,
                                0xC00, 0xC00, 0xA00, 0x800, 0x600, 0x400, 0x200, 0x200,
                                0x200, 0x200, 0x200, 0, 0, 0, 0, 0};

static const short qtab_723_40[15] = {-122, -16, 68, 139, 198, 250, 298, 339,
                                        378, 413, 445, 475, 502, 528, 553};
#if 0
/*
 * g723_40_encoder()
 *
 * Encodes a 16-bit linear PCM, A-law or u-law input sample and retuens
 * the resulting 5-bit CCITT G.723 40Kbps code.
 * Returns -1 if the input coding value is invalid.
 */
int
g723_40_encoder(
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

    d = sl - se;            /* d = estimation difference */

    /* quantize prediction difference */
    y = step_size(state_ptr);   /* adaptive quantizer step size */
    i = quantize(d, y, qtab_723_40, 15);    /* i = ADPCM code */

    dq = reconstruct(i & 0x10, _dqlntab[i], y); /* quantized diff */

    sr = (dq < 0) ? se - (dq & 0x7FFF) : se + dq; /* reconstructed signal */

    dqsez = sr + sez - se;      /* dqsez = pole prediction diff. */

    update(5, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

    return (i);
}
#endif

/*
 * g723_40_decoder()
 *
 * Decodes a 5-bit CCITT G.723 40Kbps code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int
g723_40_decoder(
    int     i,
    int     out_coding,
    struct g72x_state *state_ptr)
{
    short       sezi, sei, sez, se; /* ACCUM */
    short       y;          /* MIX */
    short       sr;         /* ADDB */
    short       dq;
    short       dqsez;

    i &= 0x1f;          /* mask to get proper bits */
    sezi = predictor_zero(state_ptr);
    sez = sezi >> 1;
    sei = sezi + predictor_pole(state_ptr);
    se = sei >> 1;          /* se = estimated signal */

    y = step_size(state_ptr);   /* adaptive quantizer step size */
    dq = reconstruct(i & 0x10, _dqlntab[i], y); /* estimation diff. */

    sr = (dq < 0) ? (se - (dq & 0x7FFF)) : (se + dq); /* reconst. signal */

    dqsez = sr - se + sez;      /* pole prediction diff. */

    update(5, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

    switch (out_coding) {
    case AUDIO_ENCODING_ALAW:
        return (tandem_adjust_alaw(sr, se, y, i, 0x10, (short *)qtab_723_40));
    case AUDIO_ENCODING_ULAW:
        return (tandem_adjust_ulaw(sr, se, y, i, 0x10, (short *)qtab_723_40));
    case AUDIO_ENCODING_LINEAR:
        return (sr << 2);   /* sr was of 14-bit dynamic range */
    default:
        return (-1);
    }
}
#endif
