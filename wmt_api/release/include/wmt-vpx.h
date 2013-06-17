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

/*----------------- MODULE DEPENDENCY ----------------------------------------*/
#ifndef WMT_VPX_H
#define WMT_VPX_H
/* To assert that only one occurrence is included */
//------------------------------------------------
//  Include Header
//------------------------------------------------
#ifndef COM_VPX_H
    #include "com-vpx.h"
#endif

/*----------------- EXPORTED PRIVATE MACROS ----------------------------------*/

/*----------------- EXPORTED PRIVATE TYPES -----------------------------------*/
//Example// typedef void api_xxx_t;
//------------------------------------------------
//  Definitions of enum & structures
//------------------------------------------------

/*----------------- EXPORTED PRIVATE CONSTANTS -------------------------------*/

/*----------------- EXPORTED PRIVATE VARIABLES -------------------------------*/
/* allocate memory for variables only in XXX.c */
#undef EXTERN
#ifdef WMT_VPX_C
    #define EXTERN
#else
    #define EXTERN extern
#endif
//Example// EXTERN int api_xxxx;

/*----------------- EXPORTED PRIVATE FUNCTIONS -------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
//Example// extern void api_xxxx(void);
/*------------------------------------------------
     API for AP
------------------------------------------------*/
int wmt_vpx_get_property(vd_handle_t *handle, vpx_property_t *prop);
int wmt_vpx_set_attribute(vd_handle_t *handle, vpx_attribute_t *attr);
int wmt_vpx_decode_start(vd_handle_t *handle, vpx_input_t *input);
int wmt_vpx_decode_finish(vd_handle_t *handle, vpx_frameinfo_t *info);
int wmt_vpx_flush(vd_handle_t *handle);
int wmt_vpx_init(vd_handle_t *handle);
int wmt_vpx_exit(vd_handle_t *handle);
#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------*/
/*=== END ====================================================================*/
#endif
