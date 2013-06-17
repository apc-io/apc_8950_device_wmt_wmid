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
#ifndef WMT_H264_H
/* To assert that only one occurrence is included */
#define WMT_H264_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#include "wmt-api.h"
#include "com-h264.h"

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  WMT_XXXX  1    *//*Example*/


#define CFG_H264_ERR_CHECK    /* Enable error check codes. User may disable this
                                 config for release version */


/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/
/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef H264_COMMON_C
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef H264_COMMON_C */

/* EXTERN int      wmt_h264_xxxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  wmt_h264_xxxx(void); *//*Example*/

/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
    Macros used in user space only
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
    Functions used in user space only
------------------------------------------------------------------------------*/
int wmt_h264_init(vd_handle_t *handle);
int wmt_h264_exit(vd_handle_t *handle);

int wmt_h264_decode_start(vd_handle_t *handle, vd_decode_input_t *input);
int wmt_h264_decode_finish(vd_handle_t *handle, h264_frameinfo_t *frameinfo);

int wmt_h264_get_property(vd_handle_t *handle, h264_property_t *prop);
int wmt_h264_set_attribute(vd_handle_t *handle, h264_attr_t *di);

int wmt_h264_flush(vd_handle_t *handle);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_H264_H */

/*=== END wmt-jdec.h ==========================================================*/
