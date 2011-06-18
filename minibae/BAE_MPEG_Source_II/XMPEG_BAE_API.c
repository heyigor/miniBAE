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
**  XMPEG_BAE_API.c
**
**  Layer between BAE and MPEG decoder/encoder
**
**  © Copyright 2000-2001 Beatnik, Inc, All Rights Reserved.
**  Written by Steve Hales
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
**  History -
**  4/15/2000   Created
**  5/1/2000    Fixed compile problem with READ_STYLE
**              Moved XGetMPEGBitrateType into the decoder area
**  5/4/2000    Fixed bug in MPEGDecoder_ReadBuffer to stay in bounds while reading.
**  5/6/2000    Close file when MPEGDecoder opens it.
**  5/7/2000    Start of an file encoder
**  5/8/2000    Fixed a return value in MPEGDecoder_Open_XFILENAME
**  5/18/2000   Increased MIN_READ_BUFFER_SIZE to 8192 to get extra data to scan for ID3
**              headers.
**              Started working on in memory encoding
**  6.21.2000 msd   MPEGEncoder now works for a streaming input, through the
**                  use of a callback to refill the buffer and continue encoding.
**  7/17/2000   sh  Reordered sample rates in XGetClosestMPEGSampleRate.
**  7/25/2000   sh  Added new SndCompressionSubType support to XGetClosestMPEGSampleRate.
**                  Added new XGetClosestMPEGSampleRateAndEncodeRate to support
**                  the new SndCompressionSubType
**  7/26/2000       Changed MPEGEncoder_New to always pad.
**                  Changed MPEGDecoder_Open_XFILE & MPEGDecoder_Open_Memory
**                  to fail correctly with bad streams.
**  7/28/2000   sh  Changed MPEGEncoder_New to only allocate 1 extra frame when
**                  calculating m_sampleSizeInBytesQuantized
**  8/2/2000    sh  Changed MPEGEncoder_New to add another frame after the round up when
**                  calculating m_sampleSizeInBytesQuantized. This is to compensate for 
**                  the old decoder in which it needs to have an extra buffer to pass 
**                  the inital size checks.
**  8/30/2000   sh  Added XGetMPEGCompressionType.
**  1/3/2001    sh  Added MPG_GetEncodedFrameBufferSizeInBytes.
**  1/16/2001   sh  Renamed MPG_GetEncodedFrameBufferSizeInBytes to 
**                  MPG_GetFrameBufferSizeInBytes
**  1/23/2001   sh  put some boundry checks into the seek code.
*/
/*****************************************************************************/

// Doug says. stuff 481 samples prior to encoding before sending in you're real data.
// When decoding ignore the first 481 samples.

#include "XMPEG_BAE_API.h"
#include "X_Assert.h"

#include "mpa.h"
#include "mpa_dec_proto.h"


// typical min buffer size
#define MIN_READ_BUFFER_SIZE        8192

typedef enum
{
    READ_NOP        = 0,
    READ_MEMORY,
    READ_MEMORY_FILLED,
    READ_XFILE
} READ_STYLE;

typedef struct
{
    unsigned long       m_sampleRate;
    unsigned long       m_channels;
    unsigned long       m_bitRate;

    // read data stream style
    READ_STYLE          m_readMode;

    // used when using a file
    XFILE               m_streamFile;
    XBOOL               m_closeFileWhenDone;

    // decoder variables
    struct
    {
        // used when reading
        mpa_u8              m_bitBuffer[MIN_READ_BUFFER_SIZE];
        unsigned long       m_bufferSize;
        unsigned long       m_bufferSizeEncoded;            // size of bytes of 1 encoded mpeg frame
        unsigned long       m_maxFrames;

        // used when reading from memory
        mpa_u8              *m_streamBlock;

        // set in all cases
        mpa_int             m_streamLength;
        mpa_int             m_streamOffset;

        void                *m_decoder;
        mpa_int             m_bcount;
        mpa_int             m_length;
        mpa_f32             m_pcmDecodeBuffer[MPA_MAX_CHANNELS][MPA_SAMPLES_IN_CHANNEL];
        MPA_FRAME_INFO      m_frame_info;
        MPA_HEADER          m_header;
    } decode;
    // encoder variables
    struct
    {
        mpa_s32             m_framecount;                   // MPEG frame counter
        XPTR                m_pBuffer;                      // sample data buffer
        unsigned long       m_sampleSizeInBytesQuantized;
        unsigned long       m_bufferSize;                   // total bytes per buffer
        unsigned long       m_bufferPos;                    // num bytes into m_pBuffer
        MPEGFillBufferFn    m_Refill;
        void                *m_refillRef;

        mpa_s16             m_pcmEncodeBuffer[MPA_MAX_CHANNELS][MPA_SAMPLES_IN_CHANNEL];
        mpa_s32             m_pcmSamplesUsed;

        mpa_u8              m_encodeBuffer[MPA_BUFFER_SIZE];
        mpa_s32             m_encodedBufferSize;

        mpa_params          m_par;
        mpa_frame_params    m_fpar;
        void                *m_encoder;
        mpa_s32             m_sampleFramesPerMPEGFrame;  // sample frames per MPEG frame (was m_nread)
    } encode;
} MPEGStreamData;

// MPEG reader class. Private class functions
static MPEGStreamData * MPEGDecoder_New(void);
static void             MPEGDecoder_Delete(MPEGStreamData *data);
static int              MPEGDecoder_ReadBuffer(MPEGStreamData *data);
static int              MPEGDecoder_SeekBufferRelative(MPEGStreamData *data, int amount);
static int              MPEGDecoder_SeekBuffer(MPEGStreamData *data, unsigned long newPos);
static int              MPEGDecoder_Open_XFILE(MPEGStreamData *data, XFILE file_reference);
static int              MPEGDecoder_Open_Memory(MPEGStreamData *data, XPTR m_streamBlock, unsigned long m_streamLength);
static int              MPEGDecoder_Open_XFILENAME(MPEGStreamData *data, XFILENAME*fileName_in);    // not used
static int              MPEGDecoder_Open_String(MPEGStreamData *data, char*fileName_in);    // not used
static int              MPEGDecoder_FillBuffer(MPEGStreamData *data, void *pBuffer);

static unsigned long    MPEGDecoder_GetMaxFrames(MPEGStreamData *data);
static unsigned long    MPEGDecoder_GetStreamSize(MPEGStreamData *data);

static unsigned long    MPEGDecoder_GetBitRate(MPEGStreamData *data);
static unsigned long    MPEGDecoder_GetBufferSize(MPEGStreamData *data);
static unsigned long    MPEGDecoder_GetChannels(MPEGStreamData *data);
static unsigned long    MPEGDecoder_GetSampleRate(MPEGStreamData *data);
static unsigned long    MPEGDecoder_GetBitSize(MPEGStreamData *data);

// MPEG Encoder class
static MPEGStreamData * MPEGEncoder_New(unsigned long encodeRate, unsigned long sampleRate, unsigned long channels,
                                        XPTR pSampleData16Bits, unsigned long frames);
static void             MPEGEncoder_Delete(MPEGStreamData *data);
static OPErr            MPEGEncoder_Process(MPEGStreamData *data);
static unsigned long    MPEGEncoder_GetMaxFrames(MPEGStreamData *data);
static unsigned long    MPEGEncoder_GetMaxFrameSize(MPEGStreamData *data);
static void             MPEGEncoder_GetBits(MPEGStreamData *data, XPTR *pReturnedBuffer, unsigned long *pReturnedSize);
static void             MPEGEncoder_SetRefillCallback(MPEGStreamData *data, MPEGFillBufferFn callback, void *userRef);

#if 0
    #pragma mark #### tool functions
#endif

// returns the number of bytes used per frame in a file.
static unsigned long PV_GetBytesPerFrame(MPA_HEADER *h, MPA_FRAME_INFO *i)
{
    unsigned long   size;

    size = 0;
    if (h && i)
    {
        size = i->framelen + 4 - h->padding_bit;
    }
    return size;
}

#if 0
    #pragma mark #### decoder class
#endif
// MPEGStreamData class
MPEGStreamData * MPEGDecoder_New(void)
{
    MPEGStreamData  *data;

    data = (MPEGStreamData *)XNewPtr(sizeof(MPEGStreamData));
    if (data)
    {
        data->m_sampleRate = 0;
        data->m_channels = 0;
        data->m_bitRate = 0;
        data->m_readMode = READ_NOP;
        data->m_closeFileWhenDone = FALSE;
        data->decode.m_decoder = NULL;
        data->decode.m_streamBlock = NULL;
        data->m_streamFile = (XFILE)0;
        data->decode.m_bufferSize = 0;
        data->decode.m_bufferSizeEncoded = 0;
        data->decode.m_maxFrames = 0;
    }
    return data;
}

void MPEGDecoder_Delete(MPEGStreamData *data)
{
    if (data)
    {
        switch (data->m_readMode)
        {
            case READ_XFILE:
                if (data->m_closeFileWhenDone)
                {
                    XFileClose(data->m_streamFile);
                }
                break;
        }
        mpa_free(data->decode.m_decoder);
        XDisposePtr((XPTR)data);
    }
}

int MPEGDecoder_ReadBuffer(MPEGStreamData *data)
{
    int size;

    size = 0;
    if (data)
    {
        switch (data->m_readMode)
        {
            case READ_XFILE:
                if (data->m_streamFile)
                {
                    if ((data->decode.m_streamOffset + MIN_READ_BUFFER_SIZE) > data->decode.m_streamLength)
                    {
                        size = data->decode.m_streamLength - data->decode.m_streamOffset;
                    }
                    else
                    {
                        size = MIN_READ_BUFFER_SIZE;
                    }
                    if (XFileRead((XFILE)data->m_streamFile, data->decode.m_bitBuffer, (long)size) == 0)
                    {
                        data->decode.m_streamOffset += size;
                    }
                }
                break;
            case READ_MEMORY:
                if ((data->decode.m_streamOffset + MIN_READ_BUFFER_SIZE) > data->decode.m_streamLength)
                {
                    size = data->decode.m_streamLength - data->decode.m_streamOffset;
                }
                else
                {
                    size = MIN_READ_BUFFER_SIZE;
                }
                XBlockMove(data->decode.m_streamBlock + data->decode.m_streamOffset, data->decode.m_bitBuffer, size);
                data->decode.m_streamOffset += size;
                break;
        }
    }
    return size;
}

int MPEGDecoder_SeekBuffer(MPEGStreamData *data, unsigned long newPos)
{
    if (data)
    {
        if (newPos > (unsigned long)data->decode.m_streamLength)
        {
            newPos = (unsigned long)data->decode.m_streamLength;
        }
        if (newPos < 0)
        {
            newPos = 0;
        }
        switch (data->m_readMode)
        {
            case READ_XFILE:
                if (data->m_streamFile)
                {
                    XFileSetPosition((XFILE)data->m_streamFile, newPos);
                    data->decode.m_streamOffset = XFileGetPosition((XFILE)data->m_streamFile);
                }
                break;
            case READ_MEMORY:
                data->decode.m_streamOffset = newPos;
                break;
        }
    }
    return data->decode.m_streamOffset;
}

// relative seek. Amount is signed.
int MPEGDecoder_SeekBufferRelative(MPEGStreamData *data, int amount)
{
    if (data)
    {
        switch (data->m_readMode)
        {
            case READ_XFILE:
                if (data->m_streamFile)
                {
                    XFileSetPositionRelative((XFILE)data->m_streamFile, amount);
                    data->decode.m_streamOffset = XFileGetPosition((XFILE)data->m_streamFile);
                }
                break;
            case READ_MEMORY:
                data->decode.m_streamOffset += amount;

                if (data->decode.m_streamOffset > data->decode.m_streamLength)
                {
                    data->decode.m_streamOffset = data->decode.m_streamLength;
                }
                if (data->decode.m_streamOffset < 0)
                {
                    data->decode.m_streamOffset = 0;
                }
                break;
        }
    }
    return data->decode.m_streamOffset;
}

int MPEGDecoder_Open_XFILE(MPEGStreamData *data, XFILE file_reference)
{
    int             ok_flag;
    mpa_int         result;
    unsigned long   frameSizeInBytes;

    ok_flag = -1;
    if (data && file_reference)
    {
        data->m_readMode = READ_XFILE;
        result = mpa_init(&data->decode.m_decoder);
        if (result == MPA_OK)
        {
            mpa_reset(data->decode.m_decoder);
            mpa_set_quality(data->decode.m_decoder, 0); // best quality
            mpa_set_crc(data->decode.m_decoder, 1);           /* crc on, it is default too   */
            data->m_streamFile = file_reference;
            data->decode.m_streamLength = (unsigned long)XFileGetLength((XFILE)file_reference);

            data->decode.m_streamOffset = 0;
            
            data->decode.m_length = MPEGDecoder_ReadBuffer(data);

            result = mpa_get_header(&data->decode.m_header, data->decode.m_bitBuffer, data->decode.m_length);
            data->decode.m_bcount = 0;
            if (result == MPA_OK)
            {
                // find first m_header in bitstream and get m_frame info
                result = mpa_get_next_frame_info(&data->decode.m_frame_info, data->decode.m_bitBuffer, data->decode.m_length);
                if (result == MPA_OK)
                {
                    data->m_channels = data->decode.m_frame_info.nchannels;
                    data->m_sampleRate = data->decode.m_frame_info.sampling_rate;
                    data->m_channels = data->decode.m_frame_info.nchannels;
                    data->m_bitRate = data->decode.m_frame_info.bit_rate;
                    data->decode.m_bufferSize = data->decode.m_frame_info.nsamples * sizeof(short) * data->decode.m_frame_info.nchannels;

                    frameSizeInBytes = PV_GetBytesPerFrame(&data->decode.m_header,
                                                                &data->decode.m_frame_info);
                    data->decode.m_bufferSizeEncoded = frameSizeInBytes;
                    data->decode.m_maxFrames = data->decode.m_streamLength / frameSizeInBytes;
                    ok_flag = 0;
                }
            }
        }
    }
    return ok_flag;
}

int MPEGDecoder_Open_XFILENAME(MPEGStreamData *data, XFILENAME *fileName_in)
{
    int result = MPEGDecoder_Open_XFILE(data, XFileOpenForRead(fileName_in));
    if (data)
    {
        data->m_closeFileWhenDone = TRUE;   // close file when done
    }
    return result;
}

int MPEGDecoder_Open_String(MPEGStreamData *data, char *fileName_in)
{
    XFILENAME   file;

    XConvertNativeFileToXFILENAME(fileName_in, &file);
    return MPEGDecoder_Open_XFILENAME(data, &file);
}

int MPEGDecoder_Open_Memory(MPEGStreamData *data, XPTR dataBlock, unsigned long dataLength)
{
    int             ok_flag;
    mpa_int         result;
    unsigned long   frameSizeInBytes;

    ok_flag = -1;
    if (data)
    {
        data->m_readMode = READ_MEMORY;
        result = mpa_init(&data->decode.m_decoder);
        if (result == MPA_OK)
        {
            mpa_reset(data->decode.m_decoder);
            mpa_set_quality(data->decode.m_decoder, 0); // best quality
            mpa_set_crc(data->decode.m_decoder, 1);           /* crc on, it is default too   */
            data->decode.m_streamBlock = (mpa_u8 *)dataBlock;
            data->decode.m_streamLength = dataLength;
            data->decode.m_streamOffset = 0;
            
            data->decode.m_length = MPEGDecoder_ReadBuffer(data);

            result = mpa_get_header(&data->decode.m_header, data->decode.m_bitBuffer, data->decode.m_length);
            data->decode.m_bcount = 0;
            if (result == MPA_OK)
            {
                // find first m_header in bitstream and get m_frame info
                result = mpa_get_next_frame_info(&data->decode.m_frame_info, data->decode.m_bitBuffer, data->decode.m_length);
                if (result == MPA_OK)
                {
                    data->m_channels = data->decode.m_frame_info.nchannels;
                    data->m_sampleRate = data->decode.m_frame_info.sampling_rate;
                    data->m_bitRate = data->decode.m_frame_info.bit_rate;
                    data->decode.m_bufferSize = data->decode.m_frame_info.nsamples * sizeof(short) * data->decode.m_frame_info.nchannels;
                    frameSizeInBytes = PV_GetBytesPerFrame(&data->decode.m_header,
                                                                &data->decode.m_frame_info);
                    data->decode.m_bufferSizeEncoded = frameSizeInBytes;
                    data->decode.m_maxFrames = data->decode.m_streamLength / frameSizeInBytes;
                    ok_flag = 0;
                }
            }
        }
    }
    return ok_flag;
}

unsigned long MPEGDecoder_GetBitRate(MPEGStreamData *data)
{
    if (data)
    {
        return (unsigned long)data->m_bitRate;
    }
    return 0;
}

unsigned long MPEGDecoder_GetBufferSize(MPEGStreamData *data)
{
    if (data)
    {
        return (unsigned long)data->decode.m_bufferSize;
    }
    return 0;
}

unsigned long MPEGDecoder_GetStreamSize(MPEGStreamData *data)
{
    if (data)
    {
        return (unsigned long)data->decode.m_streamLength;
    }
    return 0;
}

unsigned long MPEGDecoder_GetChannels(MPEGStreamData *data)
{
    if (data)
    {
        return (unsigned long)data->m_channels;
    }
    return 0;
}

unsigned long MPEGDecoder_GetSampleRate(MPEGStreamData *data)
{
    if (data)
    {
        return (unsigned long)data->m_sampleRate;
    }
    return 0;
}

unsigned long MPEGDecoder_GetBitSize(MPEGStreamData *data)
{
    data;
    return 16;
}

unsigned long MPEGDecoder_GetMaxFrames(MPEGStreamData *data)
{
    if (data)
    {
        // hack, adding an extra 2 frames. Not sure why.
        return data->decode.m_maxFrames + 2;
    }
    return 0;
}

int MPEGDecoder_FillBuffer(MPEGStreamData *data, void *pBuffer)
{
    int             size;
    void            *psamp[1];
    int             lastBuffer;
    mpa_int         result;

    lastBuffer = 0; // no
    if (data)
    {
        size = 0;
        if(data->decode.m_length - data->decode.m_bcount > MPA_MIN_BYTES_IN_FRAME) /* it can be enough */
        {
decodeagain:
            result = mpa_decode_frame(data->decode.m_pcmDecodeBuffer, data->decode.m_decoder, data->decode.m_bitBuffer+data->decode.m_bcount, 
                                            data->decode.m_length - data->decode.m_bcount, 
                                            data->decode.m_bcount);
            /* get m_frame info from m_decoder, not from bitstream */
            mpa_get_curr_frame_info(&data->decode.m_frame_info, data->decode.m_decoder);

            if (result == MPA_ERR_NOPREV ) 
            { 
                /* requires previous m_frame, BTW, if data from this m_frame will be used
                 in next, m_decoder have got it  */
                /* skip m_frame, because there is no output */
                data->decode.m_bcount += data->decode.m_frame_info.header_offset;
                data->decode.m_bcount += data->decode.m_frame_info.framelen;
                goto decodeagain;
            }
            if(result == MPA_ERR_LOWBUFFER || result == MPA_ERR_NEXTFRAME)
            {
                /* force to decode by fraud, it can helps to decode last m_frame */ 
                result = mpa_decode_last_frame(data->decode.m_pcmDecodeBuffer, data->decode.m_decoder, data->decode.m_bitBuffer+data->decode.m_bcount, 
                                           data->decode.m_length-data->decode.m_bcount, data->decode.m_bcount);
            }
            if(result == MPA_ERR_CRCFAIL) 
            {
            //  printf("\nCRC failed in m_frame %d, CRC switched off ", m_frame);
                mpa_set_crc(data->decode.m_decoder, 0);           /* crc off now   */
                data->decode.m_bcount += data->decode.m_frame_info.header_offset;
                data->decode.m_bcount += data->decode.m_frame_info.framelen;
                goto decodeagain;
            }
            else if(result != MPA_OK) 
            {
            //  printf("\nmpa_decode_frame failed: %d m_frame %d\n", m_result, m_frame);
            }

            psamp[0] = (void *)pBuffer;
            psamp[1] = NULL;
            if (data->m_channels == 1)
            {
                result = mpa_convert(psamp,
                            data->decode.m_pcmDecodeBuffer,
                            data->decode.m_frame_info.nsamples,
                            MPA_OUT_16BIT |
                            0
                            );
            }
            else
            {
                result = mpa_convert(psamp,
                            data->decode.m_pcmDecodeBuffer,
                            data->decode.m_frame_info.nsamples,
                            MPA_OUT_16BIT |
                            MPA_OUT_STEREO | 
                            MPA_OUT_INTERLEAVE |
                            0
                            );
            }

            /*
            // If for some reason we can decode the mpeg stream in the host endian format,
            // here's where we'd swap the resulting samples
            if(result == MPA_OK) 
            {
            #if X_WORD_ORDER != FALSE   // intel?
                XSwapShorts(pBuffer, data->m_channels * data->decode.m_frame_info.nsamples);
            #endif
            }
            */

            data->decode.m_bufferSize = data->decode.m_frame_info.nsamples * sizeof(short) * data->decode.m_frame_info.nchannels;

            /* advance buffer by m_frame m_lengthgth */
            data->decode.m_bcount += data->decode.m_frame_info.header_offset;
            data->decode.m_bcount += data->decode.m_frame_info.framelen;
            if(data->decode.m_bcount + MPA_MAX_BYTES_IN_FRAME > MIN_READ_BUFFER_SIZE) 
            {
                /* we will read undecoded data again */
                MPEGDecoder_SeekBufferRelative(data, data->decode.m_bcount - data->decode.m_length);
                data->decode.m_bcount = 0;
                
                data->decode.m_length = MPEGDecoder_ReadBuffer(data);
            }
        }
        else
        {
            lastBuffer = -1;    // yes
        }
    }
    return lastBuffer;
}

#if 0
    #pragma mark #### encoder class
#endif

#if USE_MPEG_ENCODER != FALSE



// MPEGEncoder_New()
// ----------------------------------------------------------------------------
// 
//
static MPEGStreamData * MPEGEncoder_New(unsigned long encodeRate, 
                                        unsigned long sampleRate,
                                        unsigned long channels,
                                        XPTR pSampleData16Bits,
                                        unsigned long frames)
{
    MPEGStreamData  *data;

    data = (MPEGStreamData *)XNewPtr(sizeof(MPEGStreamData));
    if (data)
    {
        data->m_sampleRate = sampleRate;
        data->m_channels = channels;
        data->m_bitRate = encodeRate;
        data->m_streamFile = (XFILE)0;
        data->encode.m_framecount = 0;
        data->encode.m_encodedBufferSize = 0;
        data->encode.m_Refill = NULL;
        data->encode.m_refillRef = NULL;

        data->encode.m_par.layer                = (mpa_layer)MPA_LAYER3;
        data->encode.m_par.crc_on               = MPA_TRUE;
        data->encode.m_par.sampling_freq        = sampleRate;
        data->encode.m_par.bit_rate         = encodeRate;
        data->encode.m_par.emphasis         = MPA_EMPH_NONE;
        data->encode.m_par.psy_model1           = MPA_FALSE;

        if (channels != 1)
        {
            data->encode.m_par.stereo_mode  = MPA_STEREO;
            data->encode.m_par.mode_extension   = MPA_NORMAL_STEREO; 
        }
        else
        {
            data->encode.m_par.stereo_mode  = MPA_SINGLE_CHANNEL;    
            data->encode.m_par.mode_extension   = MPA_NORMAL_STEREO; 
        }

        if (mpa_encode_init(&data->encode.m_encoder, &data->encode.m_par) == MPA_SUCCESS)
        {
            data->encode.m_sampleFramesPerMPEGFrame = data->encode.m_par.nsamples;
            data->encode.m_pBuffer = pSampleData16Bits;
            data->encode.m_bufferSize = frames * channels * sizeof(short);
            data->encode.m_bufferPos = 0;

            // quantize to mpeg frame size
            data->encode.m_sampleSizeInBytesQuantized = (frames / data->encode.m_par.nsamples) + 1; // round up
            data->encode.m_sampleSizeInBytesQuantized += 1; // this is to compensate for the old decoder in which
                                                                // it needs to have an extra buffer to pass the inital size
                                                                // checks.
            data->encode.m_sampleSizeInBytesQuantized *= data->encode.m_par.nsamples * channels * sizeof(short);

            data->encode.m_fpar.samples[0] = data->encode.m_pcmEncodeBuffer[0];  
            data->encode.m_fpar.samples[1] = data->encode.m_pcmEncodeBuffer[1];  
            data->encode.m_fpar.bit_rate   = data->encode.m_par.bit_rate;       
            data->encode.m_fpar.padding    = MPA_PAD_YES;
        }
        else
        {
            MPEGEncoder_Delete(data);
            data = NULL;
        }
    }
    return data;
}



// MPEGEncoder_Delete()
// ----------------------------------------------------------------------------
// Frees memory associated with the Encoder.
//
static void MPEGEncoder_Delete(MPEGStreamData *data)
{
    if (data)
    {
        if (data->encode.m_encoder)
        {
            mpa_encode_free(data->encode.m_encoder);
        }

        XDisposePtr((XPTR)data);
    }
}



// PV_ReadNextSamplesForEncode()
// ----------------------------------------------------------------------------
// get the next buffer in uncompressed pcm stream. Might be called recursively.
// Returns number of bytes we filled in the destination (not including any 
// padded zeros).
//
static unsigned long PV_ReadNextSamplesForEncode(MPEGStreamData *stream,
                                                 void *pDestData,
                                                 unsigned long destLength)
{
    unsigned long bufferSize, remainingBufferSize, bytesCopied;

    bytesCopied = 0;
    bufferSize = stream->encode.m_bufferSize;
    remainingBufferSize = bufferSize - stream->encode.m_bufferPos;

    if (remainingBufferSize == 0)
    {
        // We're at the end of our buffer.  Can we refill it?
        if (stream->encode.m_Refill && 
            stream->encode.m_Refill(stream->encode.m_pBuffer, stream->encode.m_refillRef))
        {
            stream->encode.m_bufferPos = 0;
            remainingBufferSize = stream->encode.m_bufferSize;
        }
        else
        {
            // we have no more data, so fill dest with zeros.
            XSetMemory(pDestData, destLength, 0);
            return 0;
        }
    }

    if (remainingBufferSize >= destLength)
    {   
        // There's enough data, so just copy it straight over

        XBlockMove((char *)stream->encode.m_pBuffer + stream->encode.m_bufferPos, pDestData, destLength);
        stream->encode.m_bufferPos += destLength;
        bytesCopied = destLength;
    }
    else
    {
        // hmmm. There's not enough data to fill the destination buffer,
        // so let's copy what's left, and see if we can refill the
        // input buffer to get some more data.

        XBlockMove((char*)stream->encode.m_pBuffer + stream->encode.m_bufferPos, pDestData, remainingBufferSize);
        pDestData = (char*)pDestData + remainingBufferSize;  // advance write pointer
        stream->encode.m_bufferPos += remainingBufferSize;
        bytesCopied = remainingBufferSize +
            PV_ReadNextSamplesForEncode(stream, pDestData, destLength - remainingBufferSize);
    }

    return bytesCopied;
}



// MPEGEncoder_Process()
// ----------------------------------------------------------------------------
//
//
static OPErr MPEGEncoder_Process(MPEGStreamData *data)
{
//  mpa_s32         count;
    OPErr           err;
    mpa_s32         i = 0;
    mpa_s16         insamples[MPA_MAX_CHANNELS * MPA_SAMPLES_IN_CHANNEL];
    unsigned long   dataSize;
    
    XDWORD          numChannels, numSamples, numBytes, numFrames;
    XDWORD          channel, frame, maxFrames;
    XDWORD          numBytesCompressed;

    err = NO_ERR;
    if (data)
    {   
        numChannels = data->m_channels;
        maxFrames = data->encode.m_sampleFramesPerMPEGFrame;
        dataSize = maxFrames * numChannels * sizeof(short);

        numBytes = PV_ReadNextSamplesForEncode(data, insamples, dataSize);
        numSamples = numBytes / sizeof(short);
        numFrames = numSamples / numChannels;

        if (numBytes)
        {
            // We have some data to encode!  

            // First, split the data into channels
            for (channel = 0; channel < numChannels; channel++)
            {
                for (frame = 0; frame < maxFrames; frame++)
                {
                    data->encode.m_pcmEncodeBuffer[channel][frame] = insamples[numChannels * frame + channel];
                }
            }

            // Then encode the frame
            numBytesCompressed = mpa_encode_frame(data->encode.m_encodeBuffer,
                                                  data->encode.m_encoder,
                                                  &data->encode.m_fpar);
            if (numBytesCompressed != MPA_FAILURE)
            {
                data->encode.m_framecount++;
                data->encode.m_encodedBufferSize = numBytesCompressed;
                data->encode.m_pcmSamplesUsed = numSamples;
            }
            else
            {
                err = GENERAL_BAD;
            }
        }
        else
        {
            // No data, so flush and we're done!
            numBytesCompressed = mpa_encode_flush(data->encode.m_encodeBuffer,
                                                  data->encode.m_encoder);
            if(numBytesCompressed != MPA_FAILURE)
            {
                data->encode.m_framecount++;
                data->encode.m_encodedBufferSize = numBytesCompressed;
                data->encode.m_pcmSamplesUsed = numSamples;
            }
            err = STREAM_STOP_PLAY; // last frame
        }
    }
    return err;
}



// MPEGEncoder_GetBits()
// ----------------------------------------------------------------------------
// Gets pointers to the actual compressed data, and its size.
//
static void MPEGEncoder_GetBits(MPEGStreamData *data, XPTR *pReturnedBuffer, unsigned long *pReturnedSize)
{
    if (data && pReturnedBuffer && pReturnedSize)
    {
        *pReturnedBuffer = (XPTR)data->encode.m_encodeBuffer;
        *pReturnedSize = (unsigned long)data->encode.m_encodedBufferSize;
    }
}



// MPEGEncoder_GetMaxFrameSize()
// ----------------------------------------------------------------------------
// return size of MPEG frame in bytes
//
static unsigned long MPEGEncoder_GetMaxFrameSize(MPEGStreamData *data)
{
    unsigned long   frames = 0;

    if (data)
    {
        // number of samples per frame
        frames = data->encode.m_par.nsamples * data->m_channels * sizeof(short);
    }
    return frames;
}



// MPEGEncoder_GetMaxFrames()
// ----------------------------------------------------------------------------
// return number of MPEG frames
//
static unsigned long MPEGEncoder_GetMaxFrames(MPEGStreamData *data)
{
    unsigned long   frames = 0;

    if (data)
    {
        // number of samples per frame times
        frames = MPEGEncoder_GetMaxFrameSize(data);
        if (frames)
        {
            frames = data->encode.m_sampleSizeInBytesQuantized / frames;
        }
    }
    return frames;
}


// MPEGEncoder_SetRefillCallback()
// ----------------------------------------------------------------------------
// Sets the refill callback.  When MPEGEncoder_Process() reaches the end of its
// buffer, it will call this callback to refill the buffer and continue
// encoding.  If the callback returns TRUE, then it has refilled the buffer.
//
static void MPEGEncoder_SetRefillCallback(MPEGStreamData *data, MPEGFillBufferFn callback, void *userRef)
{
    if (data)
    {
        data->encode.m_Refill = callback;
        data->encode.m_refillRef = userRef;
    }
}



#endif  // USE_MPEG_ENCODER





#if 0
    #pragma mark #### decoder API
#endif

// fileName_in is either a 'C' string for normal platforms, or a FSSpec for MacOS
void * MPG_NewStream(void *fileName_in)
{
    MPEGStreamData  *stream;

    stream = MPEGDecoder_New();
    if (stream)
    {
        if (MPEGDecoder_Open_String(stream, (char *)fileName_in))
        {
            MPEGDecoder_Delete(stream);
            stream = NULL;
        }
    }
    return stream;
}

void * MPG_NewStreamXFILENAME(XFILENAME *fileName_in)
{
    MPEGStreamData  *stream;

    stream = MPEGDecoder_New();
    if (stream)
    {
        if (MPEGDecoder_Open_XFILENAME(stream, fileName_in))
        {
            MPEGDecoder_Delete(stream);
            stream = NULL;
        }
    }
    return stream;
}

void * MPG_NewStreamXFILE(XFILE file)
{
    MPEGStreamData  *stream;

    stream = MPEGDecoder_New();
    if (stream)
    {
        if (MPEGDecoder_Open_XFILE(stream, file))
        {
            MPEGDecoder_Delete(stream);
            stream = NULL;
        }
    }
    return stream;
}

void * MPG_NewStreamFromMemory(void *mpeg_stream, unsigned long mpeg_stream_m_lengthgth)
{
    MPEGStreamData  *stream;

    stream = MPEGDecoder_New();
    if (stream)
    {
        if (MPEGDecoder_Open_Memory(stream, mpeg_stream, mpeg_stream_m_lengthgth))
        {
            MPEGDecoder_Delete(stream);
            stream = NULL;
        }
    }
    return stream;
}

void MPG_FreeStream(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        MPEGDecoder_Delete(stream);
    }
}

int MPG_FillBuffer(void *reference, void *buffer)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        return MPEGDecoder_FillBuffer(stream, buffer);
    }
    return -1;  // we're done
}

// return the size in bytes of an encoded frame block
int MPG_GetFrameBufferSizeInBytes(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;
    int             size;

    size = 0;
    if (stream)
    {
        size = stream->decode.m_bufferSizeEncoded;
    }
    return size;
}

// reposition stream, start reading from newPos.
int MPG_SeekStream(void *reference, unsigned long newPos)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        MPEGDecoder_SeekBuffer(stream, newPos);
        stream->decode.m_bcount = 0;
        stream->decode.m_length = MPEGDecoder_ReadBuffer(stream);
    }
    return 0;
}

// return the number of bytes filled when calling MPG_FillBuffer
int MPG_GetBufferSize(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        return MPEGDecoder_GetBufferSize(stream);
    }
    return 0;
}

// return the max number of frames
unsigned long MPG_GetMaxBuffers(void *reference)
{
    MPEGStreamData      *stream = (MPEGStreamData *)reference;
    unsigned long   frames;

    frames = 0;
    if (reference)
    {
        frames = MPEGDecoder_GetMaxFrames(stream);
    }
    return frames;
}

int MPG_GetChannels(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        return MPEGDecoder_GetChannels(stream);
    }
    return 1;
}

int MPG_GetBitSize(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        return MPEGDecoder_GetBitSize(stream);
    }
    return 16;
}

int MPG_GetBitrate(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        return MPEGDecoder_GetBitRate(stream) * 1000;
    }
    return 0;
}

int MPG_GetSampleRate(void *reference)
{
    MPEGStreamData *stream = (MPEGStreamData *)reference;

    if (stream)
    {
        return MPEGDecoder_GetSampleRate(stream);
    }
    return 0;
}

unsigned long MPG_GetNumberOfSamples(void *reference)
{
    unsigned long   samples;

    samples = 0;
    if (reference)
    {
        samples = MPG_GetSizeInBytes(reference) / MPG_GetChannels(reference) / (MPG_GetBitSize(reference) / 8);
    }
    return samples;
}

//MOE:  Steve, right now this calculation almost always m_results in a few
//      frames too many.  When you install the new codec, please try to
//      make this more accurate.
// return the size in bytes of the complete uncompressed MPEG file
unsigned long MPG_GetSizeInBytes(void *reference)
{
    //MPEGStreamData        *stream = (MPEGStreamData *)reference;
    unsigned long       bytes;

    bytes = 0;
    if (reference)
    {
        bytes = MPG_GetMaxBuffers(reference) * MPG_GetBufferSize(reference);
    }
    return bytes;
}

SndCompressionType XGetMPEGBitrateType(unsigned long bitrate)
{
    BAE_ASSERT(bitrate >= 16000);   // otherwise it doesn't make sense
    
    if (bitrate < 36000)
    {
        return C_MPEG_32;
    }
    if (bitrate < 44000)
    {
        return C_MPEG_40;
    }
    if (bitrate < 52000)
    {
        return C_MPEG_48;
    }
    if (bitrate < 60000)
    {
        return C_MPEG_56;
    }
    if (bitrate < 72000)
    {
        return C_MPEG_64;
    }
    if (bitrate < 88000)
    {
        return C_MPEG_80;
    }
    if (bitrate < 104000)
    {
        return C_MPEG_96;
    }
    if (bitrate < 120000)
    {
        return C_MPEG_112;
    }
    if (bitrate < 144000)
    {
        return C_MPEG_128;
    }
    if (bitrate < 176000)
    {
        return C_MPEG_160;
    }
    if (bitrate < 208000)
    {
        return C_MPEG_192;
    }
    if (bitrate < 240000)
    {
        return C_MPEG_224;
    }
    if (bitrate < 288000)
    {
        return C_MPEG_256;
    }
    return C_MPEG_320;
}

#if 0
    #pragma mark #### encoder API
#endif
#if USE_MPEG_ENCODER
// create new mpeg stream to prepare for encoding. Returns NULL if failed.
void * MPG_EncodeNewStream(unsigned long encodeRate,
                            unsigned long sampleRate, unsigned long channels,
                            XPTR pSampleData16Bits, unsigned long frames)
{
    return MPEGEncoder_New(encodeRate, sampleRate, channels, pSampleData16Bits, frames);
}



// get max mpeg frames we're going to encode
unsigned long MPG_EncodeMaxFrames(void *stream)
{
    return MPEGEncoder_GetMaxFrames((MPEGStreamData *)stream);
}

// get max size of an mpeg frame in bytes
unsigned long MPG_EncodeMaxFrameSize(void *stream)
{
    return MPEGEncoder_GetMaxFrameSize((MPEGStreamData *)stream);
}


void MPG_EncodeSetRefillCallback(void *stream, MPEGFillBufferFn callback, void *userRef)
{
    MPEGEncoder_SetRefillCallback((MPEGStreamData *)stream, callback, userRef);
}


// MPG_EncodeProcess()
// ----------------------------------------------------------------------------
// compress a frame.  Returns number of samples compressed
//
int MPG_EncodeProcess(void *stream, XPTR *pReturnedBuffer, unsigned long *pReturnedSize, XBOOL *pLastFrame)
{
    int     total;

    if (pLastFrame)
    {
        *pLastFrame = FALSE;
    }
    total = 0;
    if (stream)
    {
        if (MPEGEncoder_Process((MPEGStreamData *)stream) == STREAM_STOP_PLAY)
        {
            if (pLastFrame)
            {
                *pLastFrame = TRUE;
            }
        }
        total = ((MPEGStreamData *)stream)->->->encode.m_pcmSamplesUsed;
        MPEGEncoder_GetBits((MPEGStreamData *)stream, pReturnedBuffer, pReturnedSize);
    }
    return total;
}


// free vaild stream
void MPG_EncodeFreeStream(void *stream)
{
    MPEGEncoder_Delete((MPEGStreamData *)stream);
}


typedef struct MPEGEncodeRateElement
{
    SndCompressionType  type;
    XMPEGEncodeRate     encodeRate;
} MPEGEncodeRateElement;

static MPEGEncodeRateElement    mpegEncodeRateTable[] =
{
    {C_MPEG_32, MPG_32},
    {C_MPEG_40, MPG_40},
    {C_MPEG_48, MPG_48},
    {C_MPEG_56, MPG_56},
    {C_MPEG_64, MPG_64},
    {C_MPEG_80, MPG_80},
    {C_MPEG_96, MPG_96},
    {C_MPEG_112, MPG_112},
    {C_MPEG_128, MPG_128},
    {C_MPEG_160, MPG_160},
    {C_MPEG_192, MPG_192},
    {C_MPEG_224, MPG_224},
    {C_MPEG_256, MPG_256},
    {C_MPEG_320, MPG_320},
};

// This code depends on the fact that XMPEGEncodeRate{}s are defined to
// equal their bitrates divided by 1024 (defined in X_Formats.h).
XMPEGEncodeRate XGetMPEGEncodeRate(SndCompressionType type)
{
int             i;

    i = sizeof(mpegEncodeRateTable) / sizeof(MPEGEncodeRateElement);
    while (--i >= 0)
    {
        if (mpegEncodeRateTable[i].type == type)
        {
            return mpegEncodeRateTable[i].encodeRate;
        }
    }
    return (XMPEGEncodeRate)0;
}

SndCompressionType XGetMPEGCompressionType(XMPEGEncodeRate rate)
{
int             i;

    i = sizeof(mpegEncodeRateTable) / sizeof(MPEGEncodeRateElement);
    while (--i >= 0)
    {
        if (mpegEncodeRateTable[i].encodeRate == rate)
        {
            return mpegEncodeRateTable[i].type;
        }
    }
    return (SndCompressionType)0;

}

// This code depends on the fact that XMPEGEncodeRate{}s are defined to
// equal their bitrates divided by 1024 (defined in X_Formats.h).
XMPEGEncodeRate XGetClosestMPEGEncodeRate(unsigned int bitrate)
{
XMPEGEncodeRate closestRate;
long            smallestDifference;
int             i;

    closestRate = MPG_128;  // just in case
    smallestDifference = 0x7FFFFFFF;
    i = sizeof(mpegEncodeRateTable) / sizeof(MPEGEncodeRateElement);
    while (--i >= 0)
    {
    long            difference;
    
        difference = (long)mpegEncodeRateTable[i].encodeRate - bitrate;
        if (difference < 0) difference = -difference;
        if (difference < smallestDifference)
        {
            closestRate = mpegEncodeRateTable[i].encodeRate;
            smallestDifference = difference;
        }
    }
    return closestRate;
}

// Given a 16.16 fixed point sample rate, return the closest one that matches
// what the encoder can encode
XFIXED XGetClosestMPEGSampleRate(XFIXED sourceRate, SndCompressionSubType subType)
{
    unsigned long   const rate = XFIXED_TO_UNSIGNED_LONG(sourceRate);
    unsigned long   closestRate;
    long            smallestDifference;
    int             i;
    unsigned long   *table;
    // source sample rates that this encoder can work encode
    static unsigned long    mpegSample1onlyRateTable[] =
    {
        32000,
        44100,
        48000,
    };

    // source sample rates that this encoder can work encode
    static unsigned long    mpegSample1and2RateTable[] =
    {
//      16000,
        22050,
        24000,
        32000,
        44100,
        48000,
    };

    closestRate = 44100;    // just in case
    smallestDifference = 0x7FFFFFFF;

    if (subType == CS_MPEG1)
    {
        i = sizeof(mpegSample1onlyRateTable) / sizeof(unsigned long);
        table = mpegSample1onlyRateTable;
    }
    else
    {
        i = sizeof(mpegSample1and2RateTable) / sizeof(unsigned long);
        table = mpegSample1and2RateTable;
    }
    while (--i >= 0)
    {
#if TRUE
    long            difference;
    
        difference = table[i] - rate;
        if (difference < 0) difference = -difference;
        if (difference < smallestDifference)
        {
            closestRate = table[i];
            smallestDifference = difference;
        }
#else
        if (rate >= table[i])
        {
            closestRate = table[i];
            break;
        }
#endif
    }

    return UNSIGNED_LONG_TO_XFIXED(closestRate);
}

// given a sample rate, an bit encode rate and a SndCompressionSubType subType
// calulate a new sample rate, new encode rate and return them in
// *outSampleRate & *outEncodeRate
// This code depends on the fact that XMPEGEncodeRate{}s are defined to
// equal their bitrates divided by 1024 (defined in X_Formats.h).
void XGetClosestMPEGSampleRateAndEncodeRate(XFIXED inSampleRate, 
                                            XMPEGEncodeRate inEncodeRate,
                                            XFIXED *outSampleRate,
                                            XMPEGEncodeRate *outEncodeRate,
                                            SndCompressionSubType subType)
{
    int     newEncodedRate;

    if (outSampleRate && outEncodeRate)
    {
        *outSampleRate = XGetClosestMPEGSampleRate(inSampleRate, subType);

// This code has been turned off because it lies to the user about the bit rate that
// they select.
//      if ((subType == CS_MPEG2) || (subType == CS_DEFAULT))
//      {
//          newEncodedRate = (int)(((double)inEncodeRate * 
//                                  (double)*outSampleRate) / 
//                                  (double)inSampleRate);
//      }
//      else
        {
            newEncodedRate = (int)inEncodeRate;
        }
        *outEncodeRate = XGetClosestMPEGEncodeRate(newEncodedRate);
    }
}

#endif  //USE_MPEG_ENCODER


// EOF of XMPEG_BAE_API.c
