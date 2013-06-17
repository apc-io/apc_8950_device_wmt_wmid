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
#ifndef WMT_VD_H
/* To assert that only one occurrence is included */
#define WMT_VD_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#include "com-vd.h"

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  WMT_XXXX  1    *//*Example*/

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef WMT_VD_C /* allocate memory for variables only in vt8430_viaapi.c */
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef WMT_VD_C */

/* EXTERN int      wmt_vd_xxxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  wmt_vd_xxxx(void); *//*Example*/
/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif

int wmt_vd_init(vd_handle_t *handle, int id);
int wmt_vd_exit(vd_handle_t *handle);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_VD_H */

/*=== END wmt-vd.h ==========================================================*/
