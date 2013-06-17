/*
 * Interface to libmad for mp3 decoding
 * Copyright (c) 
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file libavcodec/libmad.c
 * Interface to libmad for mp3 decoding.
 */

#include "avcodec.h"
#include <mad.h>

/*
 * when CONFIG_LIBMADBIN is true libmad will be opened at runtime
 */
//#undef CONFIG_LIBMADBIN
//#define CONFIG_LIBMADBIN 0
//#define CONFIG_LIBMADBIN 1

#if CONFIG_LIBMADBIN
#include <dlfcn.h>
static const char* const libmadname = "libmad.so";
#else
#define dlopen(a)
#define dlclose(a)
#endif

#define BUFFER_SIZE 8192

typedef struct mad_decoder_s
{
    struct mad_synth  synth;
    struct mad_stream stream;
    struct mad_frame  frame;

    char  *buffer;      /* buffer of encoded mp3 data */
    int    size;        /* the size of buffer */
    int    read_pos;    /* current read position in buffer */
    int    write_pos;   /* current write position in buffer */
    int    framecount;  /* number of frames decoded */

    void* handle;       /* dlopen handle */

    void(*mad_synth_init)(struct mad_synth *);
    void(*mad_frame_init)(struct mad_frame *);
    void(*mad_stream_init)(struct mad_stream *);

    void(*mad_frame_finish)(struct mad_frame *);
    void(*mad_stream_finish)(struct mad_stream *);

    void(*mad_synth_frame)(struct mad_synth *, struct mad_frame const *);
    int (*mad_frame_decode)(struct mad_frame *, struct mad_stream *);
    void(*mad_stream_buffer)(struct mad_stream *, unsigned char const *, unsigned long);
} mad_decoder_t;

static int decode_init(AVCodecContext *avctx)
{
    mad_decoder_t *this = (mad_decoder_t *) avctx->priv_data;

#if CONFIG_LIBMADBIN
    char* error = 0;
    char* n = 0;

    (this->handle) = dlopen(libmadname, RTLD_LAZY);
    if (!this->handle){
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s could not be opened! \n%s\n", libmadname, dlerror());
        return -1;
    }
    //------------------
    n = "mad_synth_init";
    (this->mad_synth_init) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_frame_init";
    (this->mad_frame_init) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_stream_init";
    (this->mad_stream_init) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_frame_finish";
    (this->mad_frame_finish) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_stream_finish";
    (this->mad_stream_finish) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_synth_frame";
    (this->mad_synth_frame) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_frame_decode";
    (this->mad_frame_decode) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
    //------------------
    n = "mad_stream_buffer";
    (this->mad_stream_buffer) = dlsym(this->handle, n);
    if ((error = dlerror()) != NULL){
        dlclose(this->handle);
        av_log(avctx, AV_LOG_ERROR, "MAD library: %s cannot resolve %s in %s!\n", error, n, libmadname);
        return -1;
    }
#else
    (this->mad_synth_init)    = mad_synth_init;
    (this->mad_frame_init)    = mad_frame_init;
    (this->mad_stream_init)   = mad_stream_init;

    //(this->mad_synth_finish)  = mad_synth_finish;
    (this->mad_frame_finish)  = mad_frame_finish;
    (this->mad_stream_finish) = mad_stream_finish;

    (this->mad_synth_frame)   = mad_synth_frame;
    (this->mad_frame_decode)  = mad_frame_decode;
    (this->mad_stream_buffer) = mad_stream_buffer;
#endif /* CONFIG_LIBMADBIN */

    this->mad_synth_init(&this->synth);
    this->mad_stream_init(&this->stream);
    this->mad_frame_init(&this->frame);

    this->size = BUFFER_SIZE;
    this->buffer = (char *)av_malloc(this->size);
    this->read_pos = this->write_pos = 0;

    avctx->sample_fmt = SAMPLE_FMT_S16;

    return 1;
}

static int uninit(AVCodecContext *avctx)
{
    mad_decoder_t *this = (mad_decoder_t *) avctx->priv_data;

    mad_synth_finish(&this->synth);
    this->mad_frame_finish(&this->frame);
    this->mad_stream_finish(&this->stream);

    av_free(this->buffer);
    this->buffer = NULL;
    this->size = 0;
    return 0;
}

/* utility to scale and round samples to 16 bits */
static inline signed int scale(mad_fixed_t sample)
{
    /* round */
    sample += (1L << (MAD_F_FRACBITS - 16));

    /* clip */
    if (sample >= MAD_F_ONE){
        sample = MAD_F_ONE - 1;
    }
    else if (sample < -MAD_F_ONE){
        sample = -MAD_F_ONE;
    }
    /* quantize */
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/**
 * Add some data to the mad buffer
 */
static inline int data_add(AVCodecContext *avctx, char *bufin, int inlength)
{
    mad_decoder_t *this = (mad_decoder_t *) avctx->priv_data;
    int new_size  = 0;
    int remainder = this->write_pos - this->read_pos;
    //char* old_buffer = this->buffer;

    assert (remainder >= 0);

    while(inlength + remainder > this->size){
        //  buffer size too small
        //  alloc more 
        av_log(avctx, AV_LOG_ERROR,"Buffer size(%d) too small, alloc more(%d) \n", this->size, this->size * 2);
        this->buffer = (char *) av_realloc (this->buffer, this->size * 2);
        this->size *= 2;
    }

    if (this->write_pos + inlength > this->size){
        // move data to the start position
        memmove(this->buffer, this->buffer + this->read_pos, remainder);
        this->read_pos = 0;
        this->write_pos = remainder;
    }

    memcpy(this->buffer + this->write_pos, bufin, inlength);
    this->write_pos += inlength;
    return 0;
}

static int decode_frame(AVCodecContext *avctx, void *data, int *data_size, AVPacket *avpkt)
{
    mad_decoder_t *this = (mad_decoder_t *) avctx->priv_data;
    int len = 0;
    int ret;

    if (avpkt->data){
        data_add (avctx, avpkt->data, avpkt->size);
    }

    this->mad_stream_buffer(&this->stream, this->buffer + this->read_pos, this->write_pos - this->read_pos);

    while (1) {
        ret = this->mad_frame_decode(&this->frame, &this->stream);

        //av_log(avctx, AV_LOG_ERROR, "this->stream.next_frame = 0x%x \n", this->stream.next_frame);
        //av_log(avctx, AV_LOG_ERROR, "mad_frame_decode return %d\n", ret);
        //av_log(avctx, AV_LOG_ERROR, "this->stream.error = 0x%x \n", this->stream.error);

        if (ret == -1){
            if (!MAD_RECOVERABLE(this->stream.error)){
                break;
            }
            continue;
        }

        if (ret == 0){
            unsigned int         nchannels, nsamples;
            mad_fixed_t const   *left_ch, *right_ch;
            struct mad_pcm      *pcm;
            uint16_t            *output = (uint16_t*) data;

            this->mad_synth_frame(&this->synth, &this->frame);

            pcm = &this->synth.pcm;
            nchannels = pcm->channels;
            nsamples  = pcm->length;
            left_ch   = pcm->samples[0];
            right_ch  = pcm->samples[1];

            len  += 2 * nchannels * nsamples;
            data += 2*nchannels*nsamples;

            /* update codec info */
            av_log(avctx, AV_LOG_DEBUG, "update codec  nchannels = %d, nsamples = %d\n", nchannels, nsamples);

            avctx->channels = pcm->channels;
            avctx->sample_rate = pcm->samplerate;
            avctx->frame_size = pcm->length;

            while (nsamples--){
                /* output sample(s) in 16-bit signed little-endian PCM */
                *output++ = scale(*left_ch++);
                if (nchannels == 2){
                    *output++ = scale(*right_ch++);
                }
            }
        }
    }
    this->read_pos += this->stream.next_frame - this->stream.buffer;
    *data_size = len;

    return avpkt->size;
}

AVCodec libmad_mp1_decoder =
{
    "libmad(mp1)",
    CODEC_TYPE_AUDIO,
    CODEC_ID_MP1,
    sizeof(mad_decoder_t),
    decode_init,
    NULL,
    uninit,
    decode_frame,
    CODEC_CAP_PARSE_ONLY,
    .long_name = NULL_IF_CONFIG_SMALL("LIBMAD (MPEG Audio Decoder)"),
};

AVCodec libmad_mp2_decoder =
{
    "libmad(mp2)",
    CODEC_TYPE_AUDIO,
    CODEC_ID_MP2,
    sizeof(mad_decoder_t),
    decode_init,
    NULL,
    uninit,
    decode_frame,
    CODEC_CAP_PARSE_ONLY,
    .long_name = NULL_IF_CONFIG_SMALL("LIBMAD (MPEG Audio Decoder)"),
};

AVCodec libmad_mp3_decoder =
{
    "libmad(mp3)",
    CODEC_TYPE_AUDIO,
    CODEC_ID_MP3,
    sizeof(mad_decoder_t),
    decode_init,
    NULL,
    uninit,
    decode_frame,
    CODEC_CAP_PARSE_ONLY,
    .long_name = NULL_IF_CONFIG_SMALL("LIBMAD (MPEG Audio Decoder)"),
};
