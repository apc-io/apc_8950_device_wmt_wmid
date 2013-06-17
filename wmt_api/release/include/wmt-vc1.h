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
#ifndef WMT_VC1_H
/* To assert that only one occurrence is included */
#define WMT_VC1_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#include "com-vc1.h"

/*-------------------- EXPORTED PRIVATE TYPES --------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  WMT_XXXX  1    *//*Example*/
//#define TRACE(fmt, args...)    printf("{%s}:  " fmt, __FUNCTION__ , ## args)

/*-------------------- EXPORTED PRIVATE VARIABLES ----------------------------*/

/*-------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*-------------------- EXPORTED PRIVATE FUNCTIONS ----------------------------*/
/* extern void  wmt_vc1_xxxx(void); *//*Example*/

/*------------------------------------------------------------------------------
    Macros used in user space only
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
     API for AP
------------------------------------------------------------------------------*/
int wmt_vc1_init(vd_handle_t *handle);
int wmt_vc1_exit(vd_handle_t *handle);
int wmt_vc1_get_property(vd_handle_t *handle, vc1_property_t *prop);
int wmt_vc1_set_attribute(vd_handle_t *handle, vc1_attribute_t *attr);
int wmt_vc1_decode_start(vd_handle_t *handle, vc1_input_t *input_in);
int wmt_vc1_decode_finish(vd_handle_t *handle, vc1_frameinfo_t *info_out);
int wmt_vc1_flush(vd_handle_t *handle);
#endif /* ifndef WMT_VC1_H */

/*=== END wmt-vc1.h ==========================================================*/
