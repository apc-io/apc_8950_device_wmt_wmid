/*
 * Copyright (c) 2008-2011 WonderMedia Technologies, Inc. All Rights Reserved.
 *
 * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
 * and may contain trade secrets and/or other confidential information of 
 * WonderMedia Technologies, Inc. This file shall not be disclosed to any
 * third party, in whole or in part, without prior written consent of
 * WonderMedia.
 *
 * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED
 * AS IS, WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS
 * OR IMPLIED, AND WONDERMEDIA TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS
 * OR IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * QUIET ENJOYMENT OR NON-INFRINGEMENT.
 */
#ifndef WMTPE_OPTS_H
#define WMTPE_OPTS_H

#include "libavformat/avformat.h"
#include "libavcodec/opt.h"
#include "libavutil/avstring.h"

#define WMTMAX(a,b) ((a) > (b) ? (a) : (b))
#define WMTMIN(a,b) ((a) > (b) ? (b) : (a))

#define VO_QUEUE_SIZE 4 // the same with vo_queue_size
#define VD_QUEUE_SIZE 6*1024*1024
#define AO_QUEUE_SIZE 375*1024 // 2s * 2 chs * 2 bytes per sample * 48 KHz
#define AD_QUEUE_SIZE 768*1024
#define DMX_QUEUE_SIZE 5

#define DEINTERLACE_NONE 0x0
#define DEINTERLACE_SW 0x10000000
#define DEINTERLACE_SW_LI DEINTERLACE_SW // Line Interpolate
#define DEINTERLACE_SW_LB (DEINTERLACE_SW + 0x1) // Liner Blend
#define DEINTERLACE_MALI 0x40000000

typedef struct {
    const char *name;
    int flags;
#define HAS_ARG    0x0001
#define OPT_BOOL   0x0002
#define OPT_EXPERT 0x0004
#define OPT_STRING 0x0008
#define OPT_VIDEO  0x0010
#define OPT_AUDIO  0x0020
#define OPT_GRAB   0x0040
#define OPT_INT    0x0080
#define OPT_FLOAT  0x0100
#define OPT_SUBTITLE 0x0200
#define OPT_FUNC2  0x0400
#define OPT_INT64  0x0800
#define OPT_EXIT   0x1000
     union {
        void (*func_arg)(const char *); //FIXME passing error code as int return would be nicer then exit() in the func
        int *int_arg;
        char **str_arg;
        float *float_arg;
        int (*func2_arg)(const char *, const char *);
        int64_t *int64_arg;
    } u;
    const char *help;
    const char *argname;
} OptionDef;

typedef struct{
    int channels;      /* number of channels */
    int samplerate;    /* rate in samples per second */
    int bitpersecond;  /* sample size in bytes */
    int bitrate;       /* bitrate for rawaudio files */
    int format;
} RawAudioConfig;

typedef struct{
	int disable;
	int qsize;
	int priority;
}PeModuleOptions;

typedef struct{
	int width;
	int height;
	int posx;
	int posy;
}PeViewOptions;

#define PE_DBGOPT_MODULE	0x0001
#define PE_DBGOPT_HTTP		0x0002
#define PE_DBGOPT_INIT		0x0004
#define PE_DBGOPT_INFO		0x0008
#define PE_DBGOPT_DEI		0x0010

typedef struct{
	char *ao_arg;
	char *vo_arg;
	char *stream_name;
	int64_t start_time;
	int freerun;
	int norange;
	int loop;
	int fullscreen;
	int volume;
	int quiet;
	int signal_handle;
	int open_timeout;
	int read_timeout;
	int cache_size;
	int seek_by_bytes;
	int wanted_audio_stream;
	int wanted_video_stream;
	int surround;
	int passthrough;
	int ff_debug; // ffmpeg debug level
	int genpts;
	int error_recognition;
	int decoder_reorder_pts;
	int sysinfo;
	int immediate_first_frame;
	int buffered;
	int buf_guarantee; // Improvement-(buffer): try to integrate with buffered
	int display_smooth_check;
	int drop_frame;
	int debug;
	int http_no_section_cross;
	int realtime;
	int autoexit;
	int video_out_path;
	int tolerate_buffer_miss_rate;
	int dynamic_resolution_change;
	int probe_size;
	int analyze_duration;
	int deinterlace;
	int frames_per_second;
	PeModuleOptions dmx;
	PeModuleOptions vd;
	PeModuleOptions vo;
	PeModuleOptions ad;
	PeModuleOptions ao;
	PeViewOptions view;
	RawAudioConfig rawaudio;
	AVInputFormat *file_iformat;
	AVCodec *video_codec;
	AVCodec *audio_codec;
}PeOptions;

/**
 * Parses the command line arguments.
 * @param options Array with the definitions required to interpret every
 * @param parse_arg_function Name of the function called to process every
 * argument without a leading option name flag. NULL if such arguments do
 * not have to be processed.
 */
int parse_options(int argc, char **argv, PeOptions *opts, void (* parse_arg_function)(const char*));
void print_error(const char *filename, int err);
void config_codecs(void);
/**
 * Prints the program banner to stderr. The banner contents depend on the
 * current version of the repository and of the libav* libraries used by
 * the program.
 */
void show_banner(void);
#endif /* WMTPE_OPTS_H */
