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
#ifndef WMT_MPEG4_H
#define WMT_MPEG4_H
/* To assert that only one occurrence is included */
//------------------------------------------------
//  Include Header
//------------------------------------------------
#include "com-mpeg4.h"

/*----------------- EXPORTED PRIVATE MACROS ----------------------------------*/

/*----------------- EXPORTED PRIVATE TYPES -----------------------------------*/
//Example// typedef void api_xxx_t;
//------------------------------------------------
//  Definitions of enum & structures
//------------------------------------------------

/*----------------- EXPORTED PRIVATE CONSTANTS -------------------------------*/

/*----------------- EXPORTED PRIVATE VARIABLES -------------------------------*/
/* allocate memory for variables only in XXX.c */
#ifdef WMT_MPEG4_C
    #define EXTERN
#else
    #define EXTERN extern
#endif
//Example// EXTERN int api_xxxx;

#undef EXTERN

/*----------------- EXPORTED PRIVATE FUNCTIONS -------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
//Example// extern void api_xxxx(void);
/*------------------------------------------------
     API for AP
------------------------------------------------*/
int wmt_mpeg4_get_property(vd_handle_t *handle, mpeg4_property_t *prop_in);
int wmt_mpeg4_set_attribute(vd_handle_t *handle, mpeg4_attribute_t *att_in);
int wmt_mpeg4_decode_start(vd_handle_t *handle, mpeg4_input_t *input_in);
int wmt_mpeg4_decode_finish(vd_handle_t *handle, mpeg4_frameinfo_t *info_out);
int wmt_mpeg4_flush(vd_handle_t *handle);
int wmt_mpeg4_init(vd_handle_t *handle);
int wmt_mpeg4_exit(vd_handle_t *handle);
#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------*/
/*=== END ====================================================================*/
#endif
