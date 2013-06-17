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
#ifndef COM_H264ENC_H
	/* To assert that only one occurrence is included */
#define COM_H264ENC_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#ifdef __KERNEL__
#include "../com-ve.h"
#else
#include "com-ve.h"
#endif

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  VIAAPI_XXXX  1    *//*Example*/

#define H264ENC_VERSION     001   /* version: 0.00.01 */


#define IOCTL_H264ENC_INIT(cmd, _struct_type_) \
        memset((cmd), 0, sizeof(_struct_type_)); \
        IOCTL_CMD_INIT((ve_ioctl_cmd *)cmd, _struct_type_, VE_H264, H264ENC_VERSION)


/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef	void  viaapi_xxx_t;  *//*Example*/
		
/*------------------------------------------------------------------------------
    Definitions of enum
------------------------------------------------------------------------------*/
typedef enum
{
  P_SLICE = 0,
  B_SLICE = 1,
  I_SLICE = 2,
  SP_SLICE = 3,
  SI_SLICE = 4,
  NUM_SLICE_TYPES = 5
} SliceType;

typedef enum {
  H264ENC_NONE,
  H264ENC_OPENED,
  H264ENC_INITED,
  H264ENC_READY,
  H264ENC_ENCODING,
  H264ENC_SUCC,
  H264ENC_FAIL
} h264_encoder_status;

/*------------------------------------------------------------------------------
	Definitions of structures
------------------------------------------------------------------------------*/

typedef struct {
	VE_CAPABILITY_M;
} h264enc_capability_t;

typedef struct {
	VE_STATUS_M;
} h264enc_status_t;

typedef struct {
	VE_INPUT_ARG_M;
	int frame_num;
	int idr_flag;
	int QP;
	int profile_idc;
	SliceType slice_type;
	vdo_color_fmt format;
	int levelIDC;
} h264enc_input_t;

typedef struct {
	VE_OUTPUT_M;
} h264enc_output_t;

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef H264ENC_COMMON_C /* allocate memory for variables only in vt8430_viaapi.c */
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef H264ENC_COMMON_C */

/* EXTERN int      viaapi_xxxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  viaapi_xxxx(vdp_Void); *//*Example*/

/*------------------------------------------------------------------------------
    Macros below are used in user space only
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    API Functions used in user space only
------------------------------------------------------------------------------*/

    
#endif /* ifndef COM_H264ENC_H */

/*=== END com-h264enc.h ==========================================================*/













