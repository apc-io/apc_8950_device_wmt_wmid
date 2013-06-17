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
#ifndef COM_RV_H
/* To assert that only one occurrence is included */
#define COM_RV_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#ifndef COM_VD_H
#ifdef __KERNEL__
#include "../com-msvd.h"
#else
#include "com-msvd.h"
#endif
#endif

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

#define RV_VERSION     001   /* version: 0.00.01 */

#define IOCTL_RV_INIT(cmd,types) \
        memset((cmd), 0, sizeof(types)); \
        IOCTL_CMD_INIT((vd_ioctl_cmd *)cmd, types, VD_RV, RV_VERSION)


/*-------------------- EXPORTED PRIVATE TYPES --------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

/********** Common Definition **********/
typedef struct{
	union{
		msvd_property_t msvd;
	}property;
} rv_property_t;

typedef struct{
	union{
		msvd_attribute_t msvd;
	}attribute;
} rv_attribute_t;

typedef struct{
	union{
		msvd_input_t msvd;
	}input;
} rv_input_t;

typedef struct{
	union{
		msvd_frameinfo_t msvd; 
	}frameinfo;
} rv_frameinfo_t;
/*-------------------- EXPORTED PRIVATE VARIABLES ----------------------------*/
/*-------------------- EXPORTED PRIVATE MACROS -------------------------------*/
/*-------------------- EXPORTED PRIVATE FUNCTIONS ----------------------------*/
/*=== END com-rv.h ==========================================================*/
#endif /* ifndef COM_RV_H */
