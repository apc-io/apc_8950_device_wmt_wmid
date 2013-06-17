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
#ifndef WMT_H264ENC_H
/* To assert that only one occurrence is included */
#define WMT_H264ENC_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#include "wmt-api.h"
#include "wmt-log.h"
#include "com-h264enc.h"

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  WMT_XXXX  1    *//*Example*/


#define CFG_H264ENC_ERR_CHECK    /* Enable error check codes. User may disable this
                                 config for release version */

//#define H264ENC_DEBUG        /* Flag to enable/disable H264ENC debug message */
//#define H264ENC_DEBUG_DETAIL
//#define H264ENC_TRACE

#undef DBG
#ifdef H264ENC_DEBUG
  #define DBG(fmt, args...)      wmt_log(WMT_LOG_DEBUG,  "[%s] " fmt, __FUNCTION__ , ## args)
#else
  #define DBG(fmt, args...)
#endif
#undef TRACE
#ifdef H264ENC_TRACE
  #define TRACE(fmt, args...)    wmt_log(WMT_LOG_VERBOSE,"{%s}: " fmt, __FUNCTION__ , ## args)
#else
  #define TRACE(fmt, args...) 
#endif

#undef DBG_WARN
#define DBG_WARN(fmt, args...)   wmt_log(WMT_LOG_WARNING,"*W* [%s] " fmt, __FUNCTION__ , ## args)
#undef DBG_ERR
#define DBG_ERR(fmt, args...)    wmt_log(WMT_LOG_ERROR,  "*E* [%s] " fmt, __FUNCTION__ , ## args)

#ifdef H264ENC_DEBUG_DETAIL
 #define DBG_DETAIL(fmt, args...) wmt_log(WMT_LOG_DEBUG2, "[%s] " fmt, __FUNCTION__ , ## args)
#else
#define DBG_DETAIL(fmt, args...)
#endif


#ifdef CFG_H264ENC_ERR_CHECK
  #define CHECK_NULL_POINTER(ptr) \
                            if( ptr == 0 ) { \
                                DBG_ERR("NULL pointer in input argument\n");\
                                return -1;\
                            }
#else
  #define CHECK_NULL_POINTER(ptr)
#endif

/*------------------------------------------------------------------------------
    Following macros are defined and used for "flags" member in ve_input_t 
------------------------------------------------------------------------------*/
/* BIT 0 ~ 15 reserved for each private decoder */

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef H264ENC_COMMON_C /* allocate memory for variables only in vt8430_viaapi.c */
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef H264ENC_COMMON_C */

/* EXTERN int      wmt_h264enc_xxxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  wmt_h264enc_xxxx(void); *//*Example*/

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
int wmt_h264enc_init(ve_handle_t *handle);
int wmt_h264enc_exit(ve_handle_t *handle);

int wmt_h264enc_proc(ve_handle_t *handle, h264enc_input_t *arg_in);
int wmt_h264enc_finish(ve_handle_t *handle, h264enc_output_t *info_out);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_H264ENC_H */

/*=== END wmt-h264enc.h ==========================================================*/
