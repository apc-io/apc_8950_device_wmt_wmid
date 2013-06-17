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
/*------------------------------------------------------------------------------
* Description : 
*-----------------------------------------------------------------------------*/
/*----------------- MODULE DEPENDENCY ----------------------------------------*/
#ifndef COM_VPX_H
#define COM_VPX_H
/* To assert that only one occurrence is included */
//------------------------------------------------
//  Include Header
//------------------------------------------------
#ifndef COM_VD_H
    #ifdef __KERNEL__
        #include "../com-vd.h"
    #else
        #include "com-vd.h"
    #endif
#endif

/*----------------- EXPORTED PRIVATE MACROS ----------------------------------*/
#define VPX_VERSION      100   /* version: 0.01.00 */

#define IOCTL_VPX_INIT(cmd, _type_)                                     \
        memset((cmd), 0, sizeof(_type_));                               \
        IOCTL_CMD_INIT((vd_ioctl_cmd *)cmd, _type_, VD_VPX, VPX_VERSION)

/*----------------- EXPORTED PRIVATE TYPES -----------------------------------*/
//Example// typedef void api_xxx_t;
//------------------------------------------------
//  Definitions of enum & structures
//------------------------------------------------
/********** Common Definition **********/
typedef struct {
	unsigned int fbY;
	unsigned int fbC;
} vpx_framebuf_t;

typedef struct{
    VD_CAPABILITY_M;
} on2_property_t;

typedef struct{
    VD_DECODE_ATTR_M;
    unsigned int fbnum;
    unsigned int fbw;
    unsigned int fbh;
    vpx_framebuf_t fb_pool[16];
} on2_attribute_t;

typedef struct{
    VD_INPUT_ARG_M;
    unsigned int nextI;
} on2_input_t;

typedef struct{
    VD_FRAMEINFO_M;
    //{vdo_framebuf_t  fb}
    //unsigned int    y_addr;   /* Addr of Y plane in YUV domain or RGB plane in ARGB domain */
    //unsigned int    c_addr;   /* C plane address */
    //unsigned int    y_size;   /* Buffer size in bytes */
    //unsigned int    c_size;   /* Buffer size in bytes */
    //unsigned int    img_w;    /* width of valid image (unit: pixel) */
    //unsigned int    img_h;    /* height of valid image (unit: line) */
    //unsigned int    fb_w;     /* width of frame buffer (scanline offset) (unit: pixel)*/
    //unsigned int    fb_h;     /* height of frame buffer (unit: line) */
    //unsigned int    bpp;      /* bits per pixel (8/16/24/32) */
    //vdo_color_fmt   col_fmt;  /* Color format on frame buffer */
    //unsigned int    h_crop;   /* Horental Crop (unit: pixel) */
    //unsigned int    v_crop;   /* Vertical Crop (unit: pixel) */
	//unsigned int 	  flag;     /* frame flags */
    //--------------------------
    unsigned int dec_status;
    unsigned int show_frame;    /* if frame need to show */
    unsigned int key_frame;     /* if frame need to show */
    unsigned int nextI;         /* next decoded index */
    unsigned int outputI;       /* output index */
    unsigned int alternateI;    /* alternate index */
    unsigned int goldenI;       /* golden index */
    unsigned int referenceI;    /* reference index */
    unsigned int out_frame_addY;/* virtual address of the output frame buffer (luma) */
    unsigned int out_frame_addC;/* virtual address of the output frame buffer (chroma) */
    unsigned int out_frame_busY;/* DMA bus address of the output frame buffer (luma) */
    unsigned int out_frame_busC;/* DMA bus address of the output frame buffer (chroma) */
    unsigned int coded_width;   /* coded width of the picture */
    unsigned int coded_height;  /* coded height of the picture */
    unsigned int frame_width;   /* pixels width of the frame as stored in memory */
    unsigned int frame_height;  /* pixel height of the frame as stored in memory */
} on2_frameinfo_t;

//----------------------------

typedef struct{
    union{
        on2_property_t hantro;
    } property;
} vpx_property_t;

typedef struct{
    union{
        on2_attribute_t hantro;
    } attribute;
} vpx_attribute_t;

typedef struct{
    union{
        on2_input_t hantro;
    } input;
} vpx_input_t;

typedef struct{
    union{
        on2_frameinfo_t hantro;
    } frameinfo;
} vpx_frameinfo_t;

/*----------------- EXPORTED PRIVATE CONSTANTS -------------------------------*/

/*----------------- EXPORTED PRIVATE VARIABLES -------------------------------*/
#if 0
/* allocate memory for variables only in XXX.c */
#undef EXTERN
#ifdef XXX_C
    #define EXTERN
#else
    #define EXTERN extern
#endif
//Example// EXTERN int api_xxxx;
#endif
/*----------------- EXPORTED PRIVATE FUNCTIONS -------------------------------*/
#if 0
#ifdef __cplusplus
extern "C" {
#endif
//Example// extern void api_xxxx(void);
#ifdef __cplusplus
}
#endif
#endif
/*----------------------------------------------------------------------------*/
/*=== END ====================================================================*/
#endif
