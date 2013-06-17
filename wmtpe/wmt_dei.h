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
#ifndef WMTPE_DEI_H
#define WMTPE_DEI_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    void *user[4];
    unsigned long phys[4];
	int pict_type;
    int img_w;
    int img_h;
    int fb_w;
    int fb_h;
}DeInterlaceFrameBuffer;

void wmt_sw_deinterlace(DeInterlaceFrameBuffer *sDifb, DeInterlaceFrameBuffer *dDifb);
void wmt_sw_lb_deinterlace(DeInterlaceFrameBuffer *sDifb, DeInterlaceFrameBuffer *dDifb);

#if defined(__cplusplus)
}
#endif

#endif /* WMTPE_DEI_H */
