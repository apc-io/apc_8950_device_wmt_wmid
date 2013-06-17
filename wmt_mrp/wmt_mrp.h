/*
 * Copyright (c) 2008-2012 WonderMedia Technologies, Inc. All Rights Reserved.
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
#ifndef WMT_MRP_H_INCLUDED
#define WMT_MRP_H_INCLUDED

#include <fcntl.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include "com-video.h"

#define MRP_TRUE 0
#define MRP_FALSE -1

typedef struct{
    int crop_left;
    int crop_top;
    int image_width;
    int image_height;
    int fb_width;
    int fb_height;
} mrp_view;

int wmt_mrp_init(void *surface);
void wmt_mrp_uninit();
int mrp_video_config(mrp_view *view);
int mrp_video_flip(vdo_framebuf_t *fb);

#if defined(__cplusplus)
}
#endif
#endif
