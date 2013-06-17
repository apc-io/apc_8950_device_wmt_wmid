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
#ifndef COM_H263_H
#define COM_H263_H
/* To assert that only one occurrence is included */
//------------------------------------------------
//  Include Header
//------------------------------------------------
#ifndef COM_VD_H
#ifdef __KERNEL__
#include "../com-msvd.h"
#else
#include "com-msvd.h"
#endif
#endif

/*----------------- EXPORTED PRIVATE MACROS ----------------------------------*/
#define H263_VERSION      100   /* version: 0.01.00 */

#define IOCTL_H263_INIT(cmd, _type_)                                        \
        memset((cmd), 0, sizeof(_type_));                                   \
        IOCTL_CMD_INIT((vd_ioctl_cmd *)cmd, _type_, VD_H263, H263_VERSION)

/*----------------- EXPORTED PRIVATE TYPES -----------------------------------*/
//Example// typedef void api_xxx_t;
//------------------------------------------------
//  Definitions of enum & structures
//------------------------------------------------

/********** Common Definition **********/
typedef struct {
    union {
        msvd_property_t msvd;
    } property;
} h263_property_t;

typedef struct {
    union {
        msvd_attribute_t msvd;
    } attribute;
} h263_attribute_t;

typedef struct {
    union {
        msvd_input_t msvd;
    } input;
} h263_input_t;

typedef struct {
    union {
        msvd_frameinfo_t msvd;
    } frameinfo;
} h263_frameinfo_t;

/*----------------- EXPORTED PRIVATE CONSTANTS -------------------------------*/
#if 0
/*----------------- EXPORTED PRIVATE VARIABLES -------------------------------*/
/* allocate memory for variables only in XXX.c */
#ifdef COM_H263_C
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

#ifdef __cplusplus
}
#endif
#endif
/*----------------------------------------------------------------------------*/
/*=== END ====================================================================*/
#endif
