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
#ifndef WMT_WFD_H
/* To assert that only one occurrence is included */
#define WMT_WFD_H

/*--- History ------------------------------------------------------------------- 
*
*  1.0.0 (2012-05-22): First release
*
*------------------------------------------------------------------------------*/
/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#define WMT_WFD_API_VERSION     "1.0.0"


#undef INT64_C
#define INT64_C(v) (int64_t)(v ## ll)
#undef UINT64_C
#define UINT64_C(v) (uint64_t)(v ## ll)


/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/

/*------------------------------------------------------------------------------
    Definitions of enum
------------------------------------------------------------------------------*/
typedef enum { 
    WFD_ERR_NONE = 0,

    WFD_ERR_MEMORY_ALLOC_FAIL       = -2,
    WFD_ERR_OPEN_AUDIO_FAIL         = -3,
    WFD_ERR_OPEN_VIDEO_FAIL         = -4,
    WFD_ERR_OPEN_ENCODER_FAIL       = -5,
    WFD_ERR_ENCODING_FAIL           = -6,
    WFD_ERR_DEVICE_MULTI_ENABLE     = -7,
    WFD_ERR_AUDIO_STREAM_NOT_ON     = -8,
    WFD_ERR_VIDEO_STREAM_NOT_ON     = -9,
    WFD_ERR_UNKNOWN_MEDIA_FORMAT    = -10
} wmt_wfd_error;

typedef enum { 
    WFD_VIDEO,
    WFD_AUDIO,
    WFD_MAX
} wmt_wfd_format;

/*------------------------------------------------------------------------------
    Definitions of structures
------------------------------------------------------------------------------*/
typedef struct  { 
    /*--------------------------------------------------------------------------
       LOOK OUT: All data of following members are returned from library.
                 Application could not modify them by itself 
    --------------------------------------------------------------------------*/
    unsigned short id;          /* ID of this package*/
    wmt_wfd_format format;      /* media format in data buffer */
    unsigned char *data;        /* pointer to point start address of data buffer */
    unsigned int   size;        /* valid data length in data buffer */
    uint64_t       pts;         /* PTS of media in data buffer (unit: us) */
    void          *priv_data;
} wmt_wfd_packet_t;

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef WMT_WFD_C 
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef WMT_WFD_C */

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  w,t_xxxx(void); *//*Example*/

/*  following is the C++ header	*/
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
   All exported WMT WFD API
------------------------------------------------------------------------------*/

/*!*************************************************************************
* wmt_wfd_open
* 
* \retval  
*   non-zero value if success, otherwise return zero
*/ 
int wmt_wfd_open(void);
int wmt_wfd_close(int fd);

/*!*************************************************************************
* wmt_wfd_stream_on / wmt_wfd_stream_off
* 
* \retval  
*   0 if success, otherwise return an error code (negative value)
*/ 
int wmt_wfd_stream_on(int fd, wmt_wfd_format format);
int wmt_wfd_stream_off(int fd, wmt_wfd_format format);

/*!*************************************************************************
* wmt_wfd_get_packet / wmt_wfd_free_packet
* 
* \retval  
*   0 if success, otherwise return an error code (negative value)
*/ 
int wmt_wfd_get_packet(int fd, wmt_wfd_format format, wmt_wfd_packet_t *pkt);
int wmt_wfd_free_packet(int fd, wmt_wfd_format format, wmt_wfd_packet_t *pkt);

/*!*************************************************************************
* wmt_wfd_verison
* 
* \brief   Initialize a timer
*
* \retval  0 if success, else return a negtive value
*/ 
int wmt_wfd_verison(char *version);

#ifdef __cplusplus
}
#endif
        
#endif /* ifndef WMT_WFD_H */

/*=== END wmt-wfd.h ==========================================================*/
