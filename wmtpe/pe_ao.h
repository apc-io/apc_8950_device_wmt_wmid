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
#ifndef WMTPE_AO_H
#define WMTPE_AO_H

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef CONFIG_ANDROID
#undef INT64_C
#define INT64_C(v) (int64_t)(v ## ll) // for AV_NOPTS_VALUE
#undef UINT64_C
#define UINT64_C(v) (uint64_t)(v ## ll) // for AV_NOPTS_VALUE
#endif

#include "pe_log.h"
#include "libavcodec/avcodec.h"

/* defination of return vale */
#define AO_TRUE		 1
#define AO_FALSE	 0
#define AO_ERROR	-1
#define AO_NOTAVAIL	-2
#define AO_NOTIMPL	-3

#define PE_AUDIO_FLAGS_HDMI 0x1

/* cmd defination */
#define AOCTRL_QUERY_FORMAT 1 /* test for availabilty of a format */
#define AOCTRL_GET_BUFSIZE  2
#define AOCTRL_SET_BUFSIZE  3
#define AOCTRL_GET_VOLUME   4
#define AOCTRL_SET_VOLUME   5
#define AOCTRL_SET_MUTE     6
#define AOCTRL_SET_OUTPUT   7
#define AOCTRL_FLUSH_BUFFER 8
#define AOCTRL_QUERY_STATUS 255

typedef enum{
	PE_AUDIOOUT_SOUNDCARD = 0,
	PE_AUDIOOUT_HDMI,
	PE_AUDIOOUT_MAX,
}PEAudioOutPath;

typedef struct{
	PEAudioOutPath path;
	enum SampleFormat format;
	int channels;
	int samplerate;
	int bps; // byte per sample
	int buffersize;
	int flags;
	void *opaque;
} pe_ao_ctx;

/* interface of audio out */
typedef struct audio_output_device_s{
	const char *name;
	int (*control)(pe_ao_ctx *ctx, int cmd, void *data);
	int (*init)(pe_ao_ctx *ctx, void *priv, char *arg);
	void (*uninit)(pe_ao_ctx *ctx, int immed);
	void (*reset)(pe_ao_ctx *ctx);
	void (*pause)(pe_ao_ctx *ctx, int enable);
	int (*play)(pe_ao_ctx *ctx, void* data, int len);
	int (*get_space)(pe_ao_ctx *ctx); // how many space are empty for input
	float (*get_delay)(pe_ao_ctx *ctx); // how long can current buffer play
	struct audio_output_device_s *next;
	void *priv;
} audio_output_device;

/*------------------ arg specific types and declaration -------------------*/
int register_audio_output_device(audio_output_device **plist, audio_output_device *dev);
audio_output_device* find_audio_output_device(audio_output_device *list, char* arg);
const char *audio_outfmt_name(int sample_fmt);
const char *audio_ctrlcmd_name(int ctrl);
const char *audio_outpath_name(PEAudioOutPath path);

#if defined(__cplusplus)
}
#endif

#endif /* WMTPE_AO_H */
