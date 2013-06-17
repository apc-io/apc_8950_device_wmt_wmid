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
#ifndef COM_H264_H
/* To assert that only one occurrence is included */
#define COM_H264_H


/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#ifdef __KERNEL__
#include "../com-msvd.h"
#else
#include "com-msvd.h"
#endif

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  VIAAPI_XXXX  1    *//*Example*/

#define H264_VERSION     100   /* version: 0.01.00 */
#define H264_MAX_FB      15    /* the maximum number of framebuffers */

#define IOCTL_H264_INIT(cmd, _struct_type_) \
        memset((cmd), 0, sizeof(_struct_type_)); \
        IOCTL_CMD_INIT((vd_ioctl_cmd *)cmd, _struct_type_, VD_H264, H264_VERSION)


/* YUV To RGB Mode */

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  viaapi_xxx_t;  *//*Example*/


/*------------------------------------------------------------------------------
    Definitions of enum
------------------------------------------------------------------------------*/
typedef enum {
    H264_STA_NONE=0,
    H264_STA_DEC_RUN,
    H264_STA_DEC_DONE,
    H264_STA_DEC_SKIP,
    H264_STA_DEC_ERR,
    H264_STA_DEC_TIMEOUT,
} H264_STATUS;

/*------------------------------------------------------------------------------
    Definitions of structures
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
    Header Information
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
    Frame Information
------------------------------------------------------------------------------*/
typedef struct {
	VD_FRAMEINFO_M;
    unsigned int     y_addr_user;  /* Y address in user space */
    unsigned int     c_addr_user;  /* C address in user space */
    unsigned int     consumedbytes;  /* consumed byte in VLD */
	H264_STATUS      status;
} h264_frameinfo_t;

/*------------------------------------------------------------------------------
    Decoder Information
------------------------------------------------------------------------------*/
typedef struct {
    unsigned int     y_addr_phys;  /* Y address in phys space */
    unsigned int     c_addr_phys;  /* C address in physical space */
    unsigned int     y_addr_user;  /* Y address in user space */
    unsigned int     c_addr_user;  /* C address in user space */
    unsigned int     linesize;     /* Length of one scanline (unit: pixel) for YC and RGB planes */
} h264_framebuf_t;

#define H264_DECODE_INFO_M \
    unsigned int      fbnum;        /* the number of FB */\
    unsigned int      fblvl;        /* base mb count of FB */\
    h264_framebuf_t   fb[H264_MAX_FB];
/* End of H264_DECODE_INFO_M */

typedef struct {
    H264_DECODE_INFO_M;
} h264_decode_info_t;

/*------------------------------------------------------------------------------
    VDIOGET_CAPABILITY
------------------------------------------------------------------------------*/
typedef struct {
    VD_CAPABILITY_M;
    /* Input data for check capability */
	unsigned char *buf;
	unsigned int buf_size;

    /* Output capability */    
    unsigned int chip_id;

	unsigned int width;
	unsigned int height;

} h264_capability_t;

/*------------------------------------------------------------------------------
    VDIOSET_DECODE_INFO
------------------------------------------------------------------------------*/
typedef struct {
    VD_DECODE_ATTR_M;

    /* Information for HW decoder */
    h264_decode_info_t  di;
} h264_attr_t;

/********** Common Definition **********/
typedef struct{
	union{
		msvd_property_t msvd;
	}property;
} h264_property_t;

typedef struct{
	union{
		msvd_attribute_t msvd;
	}attribute;
} h264_attribute_t;

typedef struct{
	union{
		msvd_input_t msvd;
	}input;
} h264_input_t;

typedef struct{
	union{
		msvd_frameinfo_t msvd; 
	}frameinfo;
} h264_frameinfo_base_t;

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef H264_COMMON_C
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef H264_COMMON_C */

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*------------------------------------------------------------------------------
    Macros below are used in user space only
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    API Functions used in user space only
------------------------------------------------------------------------------*/

    
#endif /* ifndef COM_H264_H */

/*=== END com-jdec.h ==========================================================*/
