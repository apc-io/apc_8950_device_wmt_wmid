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
#ifndef WMT_COMM_H
/* To assert that only one occurrence is included */
#define WMT_COMM_H

#include <stdint.h>      /* for int64_t only */
#include <wmt-log.h>
#include <wmt-mb.h>
#include <unistd.h>

#define MAX_DECODE_RESOLUTION_WIDTH     1920
#define MAX_DECODE_RESOLUTION_HEIGHT    1088

#define VD_INPUT_BUF_SIZE   2*1024*1024 // 2 MiB

#define VD_PTS_NUM          32

//int usleep(useconds_t usec);

/*------------------------------------------------------------------------------
  Definition of enum
------------------------------------------------------------------------------*/
typedef enum  {  /* VDP = Vidoe Decoder Player, VD was used in com-vd.h */
    VDP_STATE_NONE = 0,
    VDP_STATE_INIT,
    VDP_STATE_SEQ_INIT,
    VDP_STATE_DECODING,
    VDP_STATE_DECODED,
    VDP_STATE_FLUSH
} vdp_state; 

/*------------------------------------------------------------------------------
  Definition of Structure
------------------------------------------------------------------------------*/
typedef struct vd_pts_s {
    int64_t      array[VD_PTS_NUM];
    unsigned int num;
    unsigned int insertAt;
} vd_pts_t;

typedef struct wmt_ffmpeg_arg_s {
	int len; // size of this structure
	int major_id; // Product ID
	int minor_id; // release version
	int extrafbs;
	int streaming;
	int disp_width;
	int disp_height;
	int crop_right;	// Based on fb W/H
	int crop_top;	// Based on fb W/H
	int crop_left;	// Based on fb W/H
	int crop_bottom;// Based on fb W/H
	int dec_fmt;	// Based on fb W/H
}wmt_ffmpeg_arg_t;

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  wmt_jdec_xxxx(void); *//*Example*/

/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif

int vd_pts_init(vd_pts_t *pts);
int vd_pts_flush(vd_pts_t *pts);
int vd_pts_in(vd_pts_t *pts, int64_t *pkt_pts);
int vd_pts_out(vd_pts_t *pts, int64_t *pkt_pts);
int vd_pts_discard(vd_pts_t *pts);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_COMM_H */
