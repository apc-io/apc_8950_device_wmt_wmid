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
#ifndef COM_VE_H
/* To assert that only one occurrence is included */
#define COM_VE_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#ifdef __KERNEL__
  #include <asm/arch/com-video.h>
#else
  #include "com-video.h"
#endif /* #ifdef __KERNEL__ */

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

/*------------------------------------------------------------------------------
    Following macros define hardware encoder type for ve_ioctl_cmd 
------------------------------------------------------------------------------*/
#define VE_BASE    16  /* Resever 16 for video decoders */
#define VE_JPEG    (VE_BASE + 1)
#define VE_H264    (VE_BASE + 2)

/* The format of version is defined below. If version is 1.15.3,
   Please use marco
     MAKE_IDENTITY(VE_JPEG, 11503)
   LOOK OUT: we only use 16 bits for version, so the max value is 
     65536 = 6.55.36
*/
#define MAKE_IDENTITY(ve_type, version)  ( ((ve_type) << 16) | (version & 0xFFFF))

/* Example: 
    jenc_input_t  arg_in;

    IOCTL_CMD_INIT(&arg_in, jenc_input_t, VE_JPEG, 10102)
*/
#define IOCTL_CMD_INIT(cmd, _struct_type_, ve_type, version)  \
             ((ve_ioctl_cmd *)cmd)->size     = sizeof(_struct_type_); \
             ((ve_ioctl_cmd *)cmd)->identity = MAKE_IDENTITY(ve_type, version)


#define CHECK_IDENTITY(identity, expected_type)  \
                     (((identity & 0xFFFF0000) == (expected_type << 16))?0:0x10)

#define CHECK_SIZE(size, _struct_type_)    \
                          (((size) == sizeof(_struct_type_))?0:0x01)

#define IOCTL_CMD_CHECK(ptr, _struct_type_, ve_type) \
             CHECK_IDENTITY( ((ve_ioctl_cmd *)ptr)->identity , ve_type ) | \
             CHECK_SIZE( ((ve_ioctl_cmd *)ptr)->size, _struct_type_ )

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
/* The member of identity is defined as 
     Bit 31~16: used for hardware encoder type 
     Bit 15~0:  used for version 
*/
#define VE_IOCTL_CMD_M \
    unsigned int    identity; /* encoder type */\
    unsigned int    size      /* size of full structure */
/* End of VE_IOCTL_CMD_M */


#define VE_CAPABILITY_M \
    VE_IOCTL_CMD_M
/* End of VE_CAPABILITY_M */


#define VE_ENCODE_ATTR_M \
    VE_IOCTL_CMD_M
/* End of VE_ENCODE_ATTR_M */


#define VE_INPUT_ARG_M  \
    VE_IOCTL_CMD_M; \
    vdo_framebuf_t fb;        /* input fb for HW encoder  */\
    unsigned int   buf_addr;  /* Encoding buffer (dest) address */\
    unsigned int   buf_size;  /* Encoding buffer size in bytes */\    
    unsigned int   quality;   /* 0(worst) - 100(best) */\
    unsigned int   flags;     /* reserved: control flag for HW encoder */
/* End of VE_INPUT_ARG_M */

#define VE_OUTPUT_M  \
    VE_IOCTL_CMD_M; \
    unsigned int   buf_addr;  /* Encoding buffer (dest) address */\
    unsigned int   buf_size;  /* Encoding buffer size in bytes */\    
    unsigned int   enc_size; /* Encoded data size in bytes */    
/* End of VE_OUTPUT_M */

#define VE_STATUS_M \
    VE_IOCTL_CMD_M; \
    unsigned int err_code
/* End of VE_STATUS_M */

/*------------------------------------------------------------------------------
    Definitions of enum
------------------------------------------------------------------------------*/

  
/*------------------------------------------------------------------------------
    Definitions of Struct
------------------------------------------------------------------------------*/
/* Following structure is used for all HW encoder as input arguments */
typedef struct {
    VE_IOCTL_CMD_M;
} ve_ioctl_cmd;

/* Following structure is used for all HW encoder to return HW capability */
typedef struct {
    VE_CAPABILITY_M;
} ve_capability_t;

/* Following structure is used for all HW encoder as input arguments */
typedef struct {
    VE_INPUT_ARG_M;
} ve_input_t;

/* Following structure is used for all HW encoder as output arguments */
typedef struct {
    VE_OUTPUT_M;
} ve_output_t;

/* Following structure is used for all HW encoder to return driver status */
typedef struct {
    VE_STATUS_M;
} ve_status_t;

/* Following structure is used for all HW encoder as output arguments */
typedef struct {
    int   ve_fd;
    int   ve_id;
    void *priv;   /* private data for video encoder used */
} ve_handle_t;

/*------------------------------------------------------------------------------
   Macros below are used for driver in IOCTL
------------------------------------------------------------------------------*/
#define VE_IOC_MAGIC              'e'
#define VE_IOC_MAXNR              4

/* GET_CAPABILITY: application get HW capability through the IOCTL 
                   driver must cast ve_ioctl_cmd to ve_capability_t
*/
#define VEIOGET_CAPABILITY        _IOWR(VE_IOC_MAGIC, 0, ve_ioctl_cmd)

/* SET_ENCODE_PROC: application send encode data to driver through the IOCTL
                   driver must cast ve_ioctl_cmd to ve_input_t
*/                   
#define VEIOSET_ENCODE_PROC       _IOR(VE_IOC_MAGIC, 1, ve_ioctl_cmd)

/* GET_ENCODE_FINISH: application waits decoding end through the IOCTL
                   driver must cast ve_ioctl_cmd to ve_output_t
*/
#define VEIOGET_ENCODE_FINISH     _IOWR(VE_IOC_MAGIC, 2, ve_ioctl_cmd)

/* VEIOGET_ENCODE_STATUS: application get status through the IOCTL
*/
#define VEIOGET_ENCODE_STATUS     _IOWR(VE_IOC_MAGIC, 3, ve_ioctl_cmd)


#endif /* ifndef COM_VE_H */

/*=== END com-ve.h ==========================================================*/
