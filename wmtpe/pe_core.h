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
#ifndef WMTPE_CORE_H
#define WMTPE_CORE_H

#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include "pe_opts.h"
#include "pe_log.h"
#include "pe_ao.h"
#include "pe_vo.h"
#include "pe_version.h"
#include "wmt_dei.h"

#include "libavcodec/audioconvert.h"

// #define TICK_TRACE_ENABLE
#ifdef CONFIG_ANDROID
#define SYS_gettid __NR_gettid
#endif

struct PlayEngineContext;
typedef void (*PeCallback)(void *priv, int msg, int ext1, int ext2);
typedef void (*PeSentevent)(struct PlayEngineContext *pec, int msg, int ext1, int ext2);
typedef void (*PeBroadcast)(struct PlayEngineContext *pec, int status); // improvement: rename by PEBroadCast and add ret value
typedef int64_t (*GetPts)(struct PlayEngineContext *, int ModuleType);

enum ModuleType { // Improvement-(subtitle): add subtitle
    MODULE_TYPE_NONE = 0,
    MODULE_TYPE_DS   = (1<<0), // 0x1
    MODULE_TYPE_AD   = (1<<1), // 0x2
    MODULE_TYPE_AO   = (1<<2), // 0x4
    MODULE_TYPE_VD   = (1<<3), // 0x8
    MODULE_TYPE_VO   = (1<<4), // 0x10
    MODULE_TYPE_ALL  = MODULE_TYPE_DS |
                       MODULE_TYPE_AD | MODULE_TYPE_AO |
                       MODULE_TYPE_VD | MODULE_TYPE_VO,
};

enum ModuleState {
    MODULE_STE_NONE = 0,
    MODULE_STE_INIT = 1,
    MODULE_STE_END = 2,
    MODULE_STE_STOP = 3,
    MODULE_STE_CONFIG = 4,
    MODULE_STE_WAIT = 5,
    MODULE_STE_IDLE = 6,
    MODULE_STE_PROC = 7,
    MODULE_STE_RESET = 8,
    MODULE_STE_INPUT = 9,
    MODULE_STE_OUTPUT = 10,
    MODULE_STE_RECONFIG = 11,
    MODULE_STE_SYNC = 12,
    MODULE_STE_BUSY = 13,
    MODULE_STE_DISCARD = 14,
    MODULE_STE_EOS = 15,
};

#ifdef TICK_TRACE_ENABLE
#define TICK_TAG_MAX 20
typedef struct TickSummary{
	int tag;
	int64_t ticks;
	int64_t summary;
}TickSummary;
#endif

typedef struct JiffyContext {
	unsigned long long usr, nic, sys, idle;
	unsigned long long iowait, irq, softirq, steal;
	unsigned long long total;
	unsigned long long busy;
} JiffyContext;

typedef struct{
    double total_time;
    double start_time;
    int bit_rate;
    int nb_programs;
    AVProgram **programs;
    int nb_streams;
    AVStream **streams;
    char video_lang[4];
    char video_codec[16];
    int video_bitrate;
    int video_width;
    int video_height;
    double video_fps;
    char audio_lang[4];
    char audio_codec[16];
    int audio_bitrate;
    int audio_channel;
    int audio_sample;
	// Metadata
    char title[16];
    char artist[16];
    char album[16];
    char year[16];
    char comment[16];
    char track[16];
    char genre[16];
    char copyright[16];
} StreamInfo;

typedef struct{
	AVPacketList *first_pkt,*last_pkt;
	int nb_packets;
	int size; // unit depends on module, ex: ao - bytes, vo - frames, ...
	int capacity_type; // 0 - by packet 1 - by size
	volatile int capacity; // the capacity of the queue
	volatile int abort_request;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}PacketQueue;

typedef struct ModuleContext{
 	PacketQueue queue;
    char name[16];
	volatile int eos; // end of stream
	volatile enum ModuleState state;
	enum ModuleType type;
    pthread_t pid;
	int priority;
	void *player;
	struct ModuleContext *next;
	void* (*handler)(void *arg);
	int64_t (*get_pts)(struct ModuleContext *);
	/* Time paramater */
	volatile double pts;
	/* static use only */
	int nb_packets;
	int throughput;
	/* CPU USAGE TICKS */
	pid_t tid;
	int64_t start_ticks;
	int64_t ticks;
	int stat_fd;
	volatile int usage;
	volatile int nb_ticks;
}ModuleContext;

typedef struct{
	ModuleContext mc;
	const video_output_device *device;
	enum PixelFormat format;
	pe_vo_ctx ctx;
	DeInterlaceFrameBuffer *difb;
	int nb_difb;
	int nb_dei_frame;
	int default_mbcnt;
	int wmt_mb_fd;
	int seeking;
	int first_flip;
	int64_t start_srt;
	int64_t idle_srt;
	int64_t last_pts;
}VoModule;

typedef struct{
	ModuleContext mc;
	AVStream *stream;
	pe_fb_config config;
	AVRational frame_rate;
	int fps;
	int start_work;
	int64_t seek_time;
	int64_t last_pts;
}VdModule;

typedef struct{
	ModuleContext mc;
	audio_output_device *device;
	pe_ao_ctx ctx;
	int new_byte_rate;
	int byte_rate;
	int dev_buf_size; // device buffer size
	int is_pause;
	int cache;
	int seeking;
	// for error handling
	int latency_count;
}AoModule;

typedef struct{
	ModuleContext mc;
	AVStream *stream;
	enum SampleFormat out_fmt;
	AVPacket out_pkt;
	AVAudioConvert *reformat;
	int channels;
	int sample_rate;
	int out_bype_rate; // bytes per second
	int out_pkt_size; // 0.5 s
	int out_adjust;
}AdModule;

typedef struct{
	ModuleContext mc;
    AVInputFormat *iformat;
    AVFormatContext *avfctx;
	int64_t read_frame_time;
    int audio_stream;
    int video_stream;
	int error_code; // improvement: move to PEContext and integrate with PEBroadCast
}DmxModule;

#define PE_PKT_FLAG_CTRL         0x80000000 // improvement: discard this by check PE_PKT_FLAG_SEEK/PE_PKT_FLAG_STOP/PE_PKT_FLAG_EOS
#define PE_PKT_FLAG_CONFIG       0x40000000
#define PE_PKT_FLAG_IMMEDIATE    0x20000000
#define PE_PKT_FLAG_SEEK         0x40000000
#define PE_PKT_FLAG_STOP         0x20000000
#define PE_PKT_FLAG_EOS          0x10000000

enum VideoConfigType{
	VIDEO_CONFIG_INIT = 0, // include output device change, such as LCD to HDMI,
	VIDEO_CONFIG_ALL, // include output device change, such as LCD to HDMI,
	VIDEO_CONFIG_FB,
	VIDEO_CONFIG_VIEW,
	VIDEO_CONFIG_FPS,
};

typedef struct VideoContent {
	int type;
	int fps;
	pe_fb_config fb;
	pe_view_config view;
} VideoContent;

typedef struct AudioContent {
    enum SampleFormat fmt;
	int channels;
	int sample_rate;
	int bype_rate;
	int pkt_size;
	int adjustment;
	int bps;
} AudioContent;

enum PePktMsg{ // improvement: add subtitle
	PE_PKT_MSG_VIDEO = 0,
	PE_PKT_MSG_AUDIO,
};

typedef struct PePktContent{
	int msg;
	union{ // improvement: add subtitle
		VideoContent v;
		AudioContent a;
	}ext;
}PePktContent;

enum PEEvent {
	PE_EVENT_STREAM_INFO = 0,
	PE_EVENT_PREPARED,
	PE_EVENT_PLAY_COMPLETE,
	PE_EVENT_SEEK_COMPLETE,
	PE_EVENT_STOP_COMPLETE,
	PE_EVENT_PROGRESS,
	PE_EVENT_BUFFER_LEVEL,
	PE_EVENT_SET_VIDEO_SIZE,
	PE_EVENT_ERROR,
};

enum PEState { // Improvement-(state+buffer): define with Bit
    PE_STE_NONE		= 0,
    PE_STE_ABORT	= 1, // After abort, return to INIT state
    PE_STE_INIT     = 2,
    PE_STE_START	= 3, // Start to cache
    PE_STE_WAIT		= 4, // Cache finished
    PE_STE_PLAY     = 5,
    PE_STE_PAUSE	= 6,
};

enum PEBUFState {
    PE_BUFSTA_FILL = 0,
	PE_BUFSTA_FULL = 1,
};

enum PEIFStatus{
	PEIF_READY = 0,
	PEIF_WAIT_EVENT = 1,
	PEIF_OPEN_SUCCESS = 2,
	PEIF_STREAM_BUFFERED = 3,
	PEIF_SEEK_SUCCESS = 4,
	PEIF_VOREQ_SUCCESS = 5,
	PEIF_ERR_ABORT = -1,
	PEIF_ERR_STATE = -2,
	PEIF_ERR_OPERATION = -3,
	PEIF_ERR_UNKNOWN_ARG = -4,
	PEIF_ERR_THREAD_FAIL = -5,
	PEIF_ERR_MODULE_FAULT = -6,
	PEIF_ERR_UNKNOWN_STREAM = -7,
	PEIF_ERR_UNSUPPORT_FMT = -8,
	PEIF_ERR_SEEK_FAIL = -9,
	PEIF_ERR_URIOPEN_FAIL = -10,
	PEIF_ERR_UNKNOWN = -11,
};

typedef struct PlayEngineContext{
	/**** status ****/
	volatile enum PEState state; // Improvement-(state+buffer): use bit define instead of number define
	volatile int module_fault;

	/**** modules ****/
	VdModule vd; // video decode
	VoModule vo; // video output
	AdModule ad; // audio decode
	AoModule ao; // audio output
	DmxModule ds; // stream demuxer
	ModuleContext sys;

	/**** control packets ****/
	AVPacket seek_pkt; // Improvement-(control packet): dynamic allocate with flag PE_PKT_FLAG_SEEK instead of this
	AVPacket stop_pkt; // Improvement-(control packet): dynamic allocate with flag PE_PKT_FLAG_STOP instead of this
	AVPacket eos_pkt; // Improvement-(control packet): dynamic allocate with flag PE_PKT_FLAG_EOS instead of this

	/**** stream information ****/
	char filename[1024];
	StreamInfo si;

	/**** default configuration ****/
	pe_vo_capa vo_capa;  // Improvement-(misc): remove it, VOCTRL_WMT_GET_RESOLUTION, and PE_VIDEOOUT_GOVR path
	PeOptions opts;

	/**** output device ****/
	audio_output_device *ao_list;
	video_output_device *vo_list;
	audio_output_device def_aodev;
	video_output_device def_vodev;

	/**** user operations ****/
	volatile int video_config; // config type, refence VideoConfigType, // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int audio_config; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int volume_req; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int volume; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int mute_req; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int mute; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int step; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int stop_req; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	volatile int seek_req; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	int seek_flags; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	int64_t seek_pos; // Improvement-(control packet): input config packet to dmx module instead of set this directly
	int64_t new_pts; // new pts after seek

	/**** time context ****/
	GetPts get_pts;

	/**** other contexts ****/
	int uid; // each instance have its unique ID
	int is_streamed;  // Improvement-(misc): iscard it
	int is_ts; // Improvemen-(misc)t: discard it
	int is_buffering; // Improvement-(State+Buffer): integrate with steam_buffered
	int stream_buffered;
	int wlock;
	int wlock_fd;
	PeCallback callback;
	PeSentevent sendevent;
	PeBroadcast broadcast;
	volatile enum PEIFStatus status;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	void *priv; // for callback
	void *cache;
	int cache_total_ticks;

	/**** statistic contexts ****/
	int64_t start_time;
	pid_t pid;
	int stat_fd;
	int64_t start_ticks;
	int64_t ticks;
	int jiffy_fd;
	volatile int usage;
	JiffyContext start_jiffy;
	JiffyContext jiffy;
	int jiffy_diff;
#ifdef TICK_TRACE_ENABLE
	TickSummary ts[TICK_TAG_MAX];
#endif
}PlayEngineContext;

// Improvement-(state+buffer): use Bit
#define IS_UNINIT(pec)		(pec->state == PE_STE_NONE)
#define IS_ABORT(pec)		(pec->state == PE_STE_ABORT)
#define IS_INITED(pec)		(pec->state == PE_STE_INIT)
#define IS_STARTED(pec)		(pec->state == PE_STE_START)
#define IS_WAITING(pec)		(pec->state == PE_STE_WAIT)
#define IS_PLAYING(pec)		(pec->state == PE_STE_PLAY)
#define IS_PAUSED(pec)		(pec->state == PE_STE_PAUSE)
#define CAN_OPERATE(pec)	(IS_WAITING(pec) || IS_PLAYING(pec) || IS_PAUSED(pec))
#define HAS_STARTED(pec)	(pec->state >= PE_STE_START)
#define IS_IMMED_PKT(p)     ((p)->flags & PE_PKT_FLAG_IMMEDIATE)
#define IS_CTRL_PKT(p)      ((p)->flags & PE_PKT_FLAG_CTRL)
#define IS_CONF_PKT(p)      ((p)->flags & PE_PKT_FLAG_CONFIG)
#define IS_PE_PKT(p)        (IS_CTRL_PKT(p) || IS_CONF_PKT(p))

// #define PE_CFG_DIRECT_PATH  (1<<31)
// #define PE_CFG_MUTE_INHERIT (1<<30)
// #define PE_CFG_PAUSE_SEEK   (1<<29)
// #define PE_CFG_FULL_SCREEN  (1<<28)
// #define PE_CFG_QUIET        (1<<27)
#define us2s(x)             ((x)/1000000.0)
#define us2ms(x)            ((x)/1000.0)

int system_module_init(PlayEngineContext *pec, ModuleContext *mc);
int video_output_module_init(PlayEngineContext *pec, VoModule *vo);
int video_decode_module_init(PlayEngineContext *pec, VdModule *vd);
int audio_output_module_init(PlayEngineContext *pec, AoModule *ao);
int audio_decode_module_init(PlayEngineContext *pec, AdModule *ad);
int demux_module_init(PlayEngineContext *pec, DmxModule *ds);

#if defined(__cplusplus)
}
#endif

#endif /* WMTPE_CORE_H */
