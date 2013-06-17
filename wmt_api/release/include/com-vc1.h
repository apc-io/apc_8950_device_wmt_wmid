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

#ifndef COM_VC1_H
#define COM_VC1_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#ifndef COM_VD_H
#ifdef __KERNEL__
#include "../com-msvd.h"
#else
#include "com-msvd.h"
#endif
#endif

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

#define VC1_VERSION     001   /* version: 0.00.01 */

#define IOCTL_VC1_INIT(cmd,types) \
        memset((cmd), 0, sizeof(types)); \
        IOCTL_CMD_INIT((vd_ioctl_cmd *)cmd, types, VD_VC1, VC1_VERSION)

/*-------------------- EXPORTED PRIVATE TYPES --------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

/********** Common Definition **********/
typedef struct{
	union{
		msvd_property_t msvd;
	}property;
} vc1_property_t;

typedef struct{
	union{
		msvd_attribute_t msvd;
	}attribute;
} vc1_attribute_t;

typedef struct{
	union{
		msvd_input_t msvd;
	}input;
} vc1_input_t;

typedef struct{
	union{
		msvd_frameinfo_t msvd; 
	}frameinfo;
} vc1_frameinfo_t;
/*-------------------- EXPORTED PRIVATE VARIABLES ----------------------------*/
/*-------------------- EXPORTED PRIVATE MACROS -------------------------------*/
/*-------------------- EXPORTED PRIVATE FUNCTIONS ----------------------------*/
/*=== END com-vc1.h ==========================================================*/
#endif /* ifndef COM_VC1_H */
