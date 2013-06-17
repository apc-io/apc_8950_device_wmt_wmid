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
#ifndef WMT_MSVD_H
/* To assert that only one occurrence is included */
#define WMT_MSVD_H

/*--- wmt-mb.h---------------------------------------------------------------
*   Copyright (C) 2010 WonderMedia Tech. Inc.
*
* MODULE       : wmt_msvd.h
* AUTHOR       : Willy Chuang
* DATE         : 2010/10/26
* DESCRIPTION  : Claim common defination
*------------------------------------------------------------------------------*/

/*--- History ------------------------------------------------------------------- 
*Version 0.01 , Willy Chuang, 2010/10/26
*   First version
*
*------------------------------------------------------------------------------*/

#include <com-msvd.h>

#include "libavcodec/avcodec.h"

// #define CFG_MSVD_DGB
#ifdef CFG_MSVD_DGB
  #define TRACELVL          AV_LOG_ERROR 
  #define TRACELVL1         AV_LOG_ERROR
#else
  #define TRACELVL          AV_LOG_DEBUG
  #define TRACELVL1         AV_LOG_DEBUG
#endif

#define IS_ASYNC(s) (s->flags2 & CODEC_FLAG2_ASYNC)
#define MSVD_ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

/*------------------------------------------------------------------------------
  Definition of Structure
------------------------------------------------------------------------------*/
typedef struct {
    unsigned int defMBCnt;
    unsigned int count;
    AVFrame *fbs;
} wmt_vd_fb_pool;

typedef struct wmt_vd_pic {
   struct wmt_vd_pic *prev,*next;
   AVFrame *pic;
   unsigned int available;
} wmt_vd_pic;

typedef struct wmt_vd_pic_pool {
    unsigned int count;
    wmt_vd_pic *pics; // decoded picture
} wmt_vd_pic_pool;

typedef struct wmt_vd_input_pool {
    unsigned int bufToggle;
    unsigned int bufPhys[2];
    void *bufVirs[2];
} wmt_vd_input_pool;

#define MSVD_CLASS_INIT(ctx, vdid) \
    ctx->prepare_decode   = ff_wmt_##vdid##_prepare_decode;   \
    ctx->ff_decode_finish = ff_wmt_##vdid##_finish_decode;    \
    ctx->init             = wmt_##vdid##_init;                \
    ctx->get_property     = wmt_##vdid##_get_property;        \
    ctx->set_attribute    = wmt_##vdid##_set_attribute;       \
    ctx->decode_start     = wmt_##vdid##_decode_start;        \
    ctx->decode_finish    = wmt_##vdid##_decode_finish;       \
    ctx->flush            = wmt_##vdid##_flush;               \
    ctx->exit             = wmt_##vdid##_exit
/* End of MSVD_CLASS_INIT */


#define WMT_VD_CTX_M \
    int (*prepare_decode)(AVCodecContext *avctx, AVPacket *avpkt, void *input);\
    int (*ff_decode_finish)(AVCodecContext *avctx, AVFrame *pict, int *data_size);\
    int (*init)(vd_handle_t *handle);\
    int (*get_property)(vd_handle_t *handle, void *prop);\
    int (*set_attribute)(vd_handle_t *handle, void *attr);\
    int (*decode_start)(vd_handle_t *handle, void *input);\
    int (*decode_finish)(vd_handle_t *handle, void *input);\
    int (*flush)(vd_handle_t *handle);\
    int (*exit)(vd_handle_t *handle);\
    AVCodecContext    *avctx;\
    vd_handle_t vhd;    \
    /* resource */\
	int minfbs;\
	int extrafbs;\
	int ipRefCnt;\
	int flushing;\
	int flushIdx;\
	int iSearch;\
    wmt_vd_input_pool  inPool;\
    wmt_vd_pic_pool    picPool;\
    wmt_vd_fb_pool     fbPool;\
    /* state */\
    int seqInited;\
    int startDecoded;\
    vdp_state state;\
    /* for interlace control */\
	int likelyInterlaceDecIdx; \
    /* pts control */\
	int reusedChunk;\
    AVPacket *pkt;\
    vd_pts_t pts;\
    int64_t num_faulty_pts; /* Number of incorrect PTS values so far */\
    int64_t num_faulty_dts; /* Number of incorrect DTS values so far */\
    int64_t last_pts; /* PTS of the last frame */\
    int64_t last_dts; /* DTS of the last frame */\
	int64_t likelyInterlacePts; \
    /* directpath control */\
	unsigned int is_direct;\
	unsigned int fbshiftsize;\
    unsigned int fbpatchsize;\
    /* for H264 only  */\
	unsigned int is_avc; \
	unsigned int got_avcC
/* End of WMT_VD_CTX_M */

typedef struct {
    WMT_VD_CTX_M;
} WmtVDCtx;
    
/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef WMT_COMM_C 
    #define EXTERN
#else
    #define EXTERN   extern
#endif 

/* EXTERN int      wmt_jdec_xxxx; *//*Example*/

#undef EXTERN


/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  wmt_jdec_xxxx(void); *//*Example*/

/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif


int wmt_msvd_finish_decode(
    AVCodecContext *avctx, 
    AVFrame *pict, 
    void    *fi,
    msvd_frameinfo_t *out, 
    int *data_size);

int wmt_msvd_seq_init(
    AVCodecContext *avctx, 
    AVPacket *avpkt,
	msvd_attribute_t    *attr,
	msvd_in_property_t  *inProp,
	msvd_out_property_t *outProp,
    void     *drv_prop,
    void     *drv_attr);

int wmt_msvd_get_buffer(AVCodecContext *avctx, msvd_input_t *in);

int wmt_msvd_decode_init(AVCodecContext *avctx, int vd_id);
int wmt_msvd_decode_end(AVCodecContext *avctx);

int wmt_msvd_decode_frame(
    AVCodecContext *avctx, 
    void *data, 
    int *data_size, 
    AVPacket *avpkt,
    void  *input);

int wmt_msvd_decode_flush(AVCodecContext *avctx);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_MSVD_H */
