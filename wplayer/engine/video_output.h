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
#ifndef WPLAYER_VIDEO_OUTPUT_H_INCLUDED
#define WPLAYER_VIDEO_OUTPUT_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include "pe_vo.h"

int wp_video_init(pe_vo_ctx *ctx, void *priv, char *arg);
void wp_video_uninit(pe_vo_ctx *ctx);
int wp_video_config(pe_vo_ctx *ctx);
int wp_video_control(pe_vo_ctx *ctx, int cmd, void *arg);
int wp_video_flip(pe_vo_ctx *ctx, AVFrame *frame);

#if defined(__cplusplus)
}
#endif
#endif
