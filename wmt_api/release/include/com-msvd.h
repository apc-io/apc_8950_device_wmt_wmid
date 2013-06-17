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

#ifndef COM_MSVD_H
#define COM_MSVD_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#include "com-vd.h"

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

#define MSVD_INPUT_FLAGS_NO_MORE_DATA        BIT(1)
#define MSVD_INPUT_FLAGS_SKIP_B_FRAME          BIT(2)
#define MSVD_INPUT_FLAGS_SKIP_P_B_FRAME      BIT(3)
#define MSVD_INPUT_FLAGS_SKIP_ALL_FRAME       BIT(4)

/*-------------------- EXPORTED PRIVATE TYPES --------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

#define MSVD_FRAMEBUF_M      \
	unsigned int fbY;       \
	unsigned int fbC;       \
	unsigned int fbMvCol;   \
	unsigned int linesize
/* End of MSVD_FRAMEBUF_M */

#define MSVD_IN_PROPERTY_M \
	char *bsbuf;\
	unsigned int bssize;\
	unsigned int reorder;\
	unsigned int filePlay; /* Record (Set Register in VPU_DecGetInitialInfo)*/\
	unsigned int dynamicAlloc; /* Record (Set Register in VPU_DecGetInitialInfo)*/\
	unsigned int Width; /* Record (Set Register in VPU_DecGetInitialInfo)*/\
	unsigned int Height; /* Record (Set Register in VPU_DecGetInitialInfo)*/\
	unsigned int mp4Class;\
	unsigned int mp4Deblk; /* Verify, STD_AVC, STD_VC1, STD_H263, STD_RV, STD_AVS Can't enable mp4DeblkEnable*/\
	unsigned int flag
/* End of MSVD_IN_PROPERTY_M */


/********** CnM Version Definiation **********/
typedef struct {
    MSVD_FRAMEBUF_M;
	unsigned int index;
} msvd_framebuf_t;

typedef struct{
    MSVD_IN_PROPERTY_M;
} msvd_in_property_t;

typedef struct{
	unsigned int picWidth; // {(PicX+15)/16} * 16
	unsigned int picHeight; // {(PicY+15)/16} * 16
	unsigned int frameRateInfo;
	unsigned int minFrameBufferCount;
	unsigned int frameBufDelay;
	unsigned int picCropLeft; 
	unsigned int picCropRight;
	unsigned int picCropTop; 
	unsigned int picCropButtom;
} msvd_out_property_t;

typedef struct{
    VD_CAPABILITY_M;
	union {
		msvd_in_property_t in;
		msvd_out_property_t out;
    }arg;
} msvd_property_t;

#define MSVD_ATTR_FLAG_ISEARCH 0x1
#define MSVD_ATTR_FLAG_YUV     0x2

typedef struct{
    VD_DECODE_ATTR_M;
    VD_DECODE_ATTR_EXT_M;
	unsigned int flags;
    unsigned int fbCount;
    msvd_framebuf_t fb[32];
} msvd_attribute_t;

typedef struct{
    VD_INPUT_ARG_M;
	unsigned int uselessFbCount;
	msvd_framebuf_t uselessFb[32];
} msvd_input_t;

typedef struct{
    VD_FRAMEINFO_M;
	vd_state state;
	unsigned int consumedBytes;
	int dispIdx;
	int decIdx;
	int picType;
	int steCode;
	int interlace;
	int packedPBframe;
	int errMBs;
} msvd_frameinfo_t;

/*-------------------- EXPORTED PRIVATE VARIABLES ----------------------------*/
/*-------------------- EXPORTED PRIVATE MACROS -------------------------------*/
/*-------------------- EXPORTED PRIVATE FUNCTIONS ----------------------------*/
/*=== END com-msvd.h ==========================================================*/
#endif /* ifndef COM_MSVD_H */
