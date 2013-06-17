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
#ifndef WMTPE_VO_H
#define WMTPE_VO_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "pe_log.h"
#include "libavcodec/avcodec.h"

/* defination of return vale */
#define VO_TRUE		1
#define VO_FALSE	0
#define VO_ERROR	-1
#define VO_NOTAVAIL	-2
#define VO_NOTIMPL	-3

/* defination of control request vale */
#define VOCTRL_QUERY_FORMAT 1
#define VOCTRL_RESET 2
#define VOCTRL_FLUSH 3
#define VOCTRL_WMT_SET_MBFD 4
#define VOCTRL_WMT_GET_RESOLUTION 5
#define VOCTRL_WMT_SET_RESOLUTION 6
#define VOCTRL_WMT_GET_OUTPUT_MODE 7
#define VOCTRL_WMT_SET_OUTPUT_MODE 8
#define VOCTRL_WMT_GET_OUTPUT_DEV 9
#define VOCTRL_WMT_SET_OUTPUT_DEV 10
#define VOCTRL_WMT_CONFIG_FB 11
#define VOCTRL_WMT_CONFIG_VIEW 12
#define VOCTRL_WMT_CONFIG_FPS 13
#define VOCTRL_WMT_STATUS 14
#define VOCTRL_WMT_HURRYUP 15 // argument: if queued frames over the number, flush it

typedef enum{
	PE_VIEW_BESTFIT = 0,
	PE_VIEW_FULLSIZE,
	PE_VIEW_REALSIZE,
	PE_VIEW_USERDEF,
    PE_VIEW_MAX,
}PEViewMode;

typedef enum{
	PE_VIDEOOUT_GOVW = 0,
	PE_VIDEOOUT_GOVR,
	PE_VIDEOOUT_GE,
	PE_VIDEOOUT_VPU,
	PE_VIDEOOUT_SCL,
	PE_VIDEOOUT_MAX,
}PEVideoOutPath;

typedef enum{
	PE_HDMI_NONE = 0,
	PE_HDMI_ONCE,
	PE_HDMI_REPEAT,
	PE_HDMI_MAX,
}PEHDMIMode;

typedef struct{
	enum PixelFormat fmt;
	int flags;
    int fb_w;
    int fb_h;
    int img_w;
    int img_h;
    int crop_left;
    int crop_top;
    int crop_right;
    int crop_bottom;
}pe_fb_config;

typedef struct{
	PEViewMode mode;
    int width;
    int height;
    int posx;
    int posy;
}pe_view_config;

typedef struct{
	PEVideoOutPath path;
	PEHDMIMode hdmi;
	pe_fb_config fb;
	pe_view_config view;
	int fps;
	int flags;
	void *opaque;
}pe_vo_ctx;

typedef struct{
	int vpp_width;
	int vpp_height;
	int ge_width;
	int ge_height;
}pe_vo_capa;

typedef struct video_output_device_s{
	const char *name;
    int (*init)(pe_vo_ctx *ctx, void *priv, char *arg);
    int (*config)(pe_vo_ctx *ctx);
	int (*control)(pe_vo_ctx *ctx, int cmd, void *arg);
    int (*flip)(pe_vo_ctx *ctx, AVFrame *frame);
    void (*uninit)(pe_vo_ctx *ctx);
	struct video_output_device_s *next;
	void *priv;
} video_output_device;

int register_video_output_device(video_output_device **plist, video_output_device *dev);
video_output_device* find_video_output_device(video_output_device *list, char* arg);
const char *video_outpath_name(PEVideoOutPath path);
const char *video_hdmimode_name(PEHDMIMode mode);
const char *video_viewmode_name(PEViewMode mode);

#if defined(__cplusplus)
}
#endif

#endif /* WMTPE_VO_H */
