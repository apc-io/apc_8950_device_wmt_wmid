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
#ifndef WMT_JDEC_H
/* To assert that only one occurrence is included */
#define WMT_JDEC_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

#include "wmt-api.h"
#include "wmt-log.h"
#include "com-jdec.h"

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  WMT_XXXX  1    *//*Example*/


#define CFG_JDEC_ERR_CHECK    /* Enable error check codes. User may disable this
                                 config for release version */

//#define JPEG_DEBUG        /* Flag to enable/disable JPEG debug message */
//#define JPEG_DEBUG_DETAIL
//#define JPEG_TRACE

#undef DBG
#ifdef JPEG_DEBUG
  #define DBG(fmt, args...)      wmt_log(WMT_LOG_DEBUG,  "[%s] " fmt, __FUNCTION__ , ## args)
#else
  #define DBG(fmt, args...)
#endif
#undef TRACE
#ifdef JPEG_TRACE
  #define TRACE(fmt, args...)    wmt_log(WMT_LOG_VERBOSE,"{%s}: " fmt, __FUNCTION__ , ## args)
#else
  #define TRACE(fmt, args...) 
#endif

#undef DBG_WARN
#define DBG_WARN(fmt, args...)   wmt_log(WMT_LOG_WARNING,"*W* [%s] " fmt, __FUNCTION__ , ## args)
#undef DBG_ERR
#define DBG_ERR(fmt, args...)    wmt_log(WMT_LOG_ERROR,  "*E* [%s] " fmt, __FUNCTION__ , ## args)

#ifdef JPEG_DEBUG_DETAIL
 #define DBG_DETAIL(fmt, args...) wmt_log(WMT_LOG_DEBUG2, "[%s] " fmt, __FUNCTION__ , ## args)
#else
#define DBG_DETAIL(fmt, args...)
#endif


#ifdef CFG_JDEC_ERR_CHECK
  #define CHECK_NULL_POINTER(ptr) \
                            if( ptr == 0 ) { \
                                DBG_ERR("NULL pointer in input argument\n");\
                                return -1;\
                            }
#else
  #define CHECK_NULL_POINTER(ptr)
#endif

/*------------------------------------------------------------------------------
    Following macros are defined and used for "flags" member in vd_decode_input_t 
------------------------------------------------------------------------------*/
/* BIT 0 ~ 15 reserved for each private decoder */

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  wmt_xxx_t;  *//*Example*/

typedef enum { 
    JPEG_DM_BEST_FIT,    
       /* Best Fit on screen: if picture is smaller than display area, it will 
          be located on center of screen. Otherwise, it will scale down to fit 
          display area without torsion. */
    JPEG_DM_Original,
       /* Original size: picture will be put at top-left of screen (0,0). If the
          picture is larger than display area, some part will be invisible.
       */ 
    JPEG_DM_FULL_SCREEN,
       /* FULL_SCREEN: if picture is smaller than display area, it will be scaled 
          up without torsion. If it is larger than display area, it will scale 
          down to fit display area without torsion. */

    JPEG_DM_FULL_SCREEN_TORSION,
       /* FULL_SCREEN_TORSION: if picture is smaller than display area, it will 
          be scaled up WITH torsion. If it is larger than display area, it will 
          scale down to fit display area WITH torsion. */

    JPEG_DM_MAX
} jpeg_disp_mode; /* DM = Display Mode */

typedef enum { 
    JPEG_SA_SMART,
       /* SA_SMART: Use JPEG_SA_QUALITY or JPEG_SA_FAST according to picture size
                    and requested size
       */ 
    JPEG_SA_QUALITY, 
       /* SA_QUALITY: Decode to a larger size than requested size by HW decoder.
          Then use HW SCALER to scaling down to requested size (better quality)
       */ 
    JPEG_SA_FAST,
       /* SA_FAST: Decode to a smaller size than requested size by HW decoder.
          Then use HW SCALER to scaling up to requested size (better performance)
       */ 
    JPEG_SA_HIGH_QUALITY, 
       /* SA_QUALITY: Decode to original size if possible. (If fail, then decode 
         to a larger size than requested size by HW decoder. Same as JPEG_SA_QUALITY)
          Then use HW SCALER to scaling down to requested size (best quality)
       */ 
    JPEG_SA_MAX
} jpeg_scale_alg; /* SA = Scale Algorithm */

typedef enum { 
    JPEG_SM_RANDOM,
       /* JPEG_SM_RANDOM: Use random slide show. It may be any one of all supported
          slide mode
       */ 
    JPEG_SM_L_TO_R,
       /* JPEG_SM_L_TO_R: The picture in upper layer will be moved from left to 
          right on screen
       */ 
    JPEG_SM_R_TO_L,
       /* JPEG_SM_R_TO_L: The picture in upper layer will be moved from right to 
          left on screen
       */ 
    JPEG_SM_T_TO_B,
       /* JPEG_SM_T_TO_B: The picture in upper layer will be moved from top to 
          bottom on screen
       */ 
    JPEG_SM_B_TO_T,
       /* JPEG_SM_B_TO_T: The picture in upper layer will be moved from bottom to 
          top on screen
       */ 
    JPEG_SM_SHUTTER_H,
       /* JPEG_SM_SHUTTER_H: The effect of shutter in horizontal direction */
       
    JPEG_SM_SHUTTER_V,
       /* JPEG_SM_SHUTTER_V: The effect of shutter in vertical direction */ 
       
    JPEG_SM_DOOR_OPEN_H,
       /* JPEG_SM_DOOR_OPEN_H: The effect of opening door in horizontal direction */
       
    JPEG_SM_DOOR_OPEN_V,
       /* JPEG_SM_DOOR_OPEN_V: The effect of opening door in vertical direction */

    JPEG_SM_DOOR_CLOSE_H,
       /* JPEG_SM_DOOR_CLOSE_H: The effect of closing door in horizontal direction */
       
    JPEG_SM_DOOR_CLOSE_V,
       /* JPEG_SM_DOOR_CLOSE_V: The effect of closing door in vertical direction */

    JPEG_SM_FADE,
       /* JPEG_SM_FADE: The bottom frame will fade out and top frame will fade in */

    JPEG_SM_DIAG_LT_TO_RB,
       /* JPEG_SM_DIAG_LT_TO_RB: The top frame will move from left-top to right-botoom  
          diagonally */

    JPEG_SM_DIAG_LB_TO_RT,
       /* JPEG_SM_DIAG_LT_TO_RB: The top frame will move from left-botoom to left-top
          diagonally */

    JPEG_SM_DIAG_RT_TO_LB,
       /* JPEG_SM_DIAG_LT_TO_RB: The top frame will move from right-top to left-botoom
          diagonally */

    JPEG_SM_DIAG_RB_TO_LT,
       /* JPEG_SM_DIAG_LT_TO_RB: The top frame will move from right-botoom to left-top
          diagonally */

    JPEG_SM_MAX
} jpeg_slide_mode; /* SM = Slide Show Mode */

typedef struct wmt_jpeg_mem_s {
    unsigned int phy_addr;   /* Physical Address */
    unsigned int mem_size;   /* Buffer size in bytes */
    unsigned int bpp;        /* bits per pixel (8/16/24/32) */   
    unsigned int usr_addr;   /* User space address */
} wmt_jpeg_mem_t;

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef JPEG_COMMON_C /* allocate memory for variables only in vt8430_viaapi.c */
    #define EXTERN
#else
    #define EXTERN   extern
#endif /* ifdef JPEG_COMMON_C */

/* EXTERN int      wmt_jdec_xxxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  wmt_jdec_xxxx(void); *//*Example*/

/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
    Macros used in user space only
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Functions used in user space only
------------------------------------------------------------------------------*/
int wmt_jdec_init(vd_handle_t *handle);
int wmt_jdec_exit(vd_handle_t *handle);

int wmt_jdec_proc(vd_handle_t *handle, jdec_input_t *arg_in);
int wmt_jdec_finish(vd_handle_t *handle, jdec_frameinfo_t *frameinfo);
int wmt_jdec_cancel(vd_handle_t *handle, jdec_frameinfo_t *frameinfo);

int wmt_jdec_file_header(vd_handle_t *handle, char *filename, jdec_hdr_info_t *hdr);

int wmt_jdec_buf(
    vd_handle_t      *handle,
    unsigned char    *buf_in, 
    unsigned int      bufsize,
    jdec_frameinfo_t *frameinfo);

int wmt_jdec_file(vd_handle_t *handle, char *filename, jdec_frameinfo_t *frameinfo);

int wmt_jdec_show(
    vd_handle_t      *handle, 
    jdec_frameinfo_t *frameinfo, 
    vdo_view_t       *view);
    
int wmt_jdec_show_image(
    vd_handle_t      *handle,
    jdec_frameinfo_t *frameinfo, 
    jpeg_disp_mode    mode);

int wmt_jdec_header (
    vd_handle_t     *handle, 
    unsigned char   *buf_in, 
    unsigned int     buf_size,
    jdec_hdr_info_t *hdr);

int wmt_jdec_set_header(vd_handle_t *handle, jdec_hdr_info_t *hdr);
int wmt_jdec_get_header(vd_handle_t *handle, jdec_hdr_info_t *hdr);

int wmt_jdec_show_err(vd_handle_t *handle, int err_code);
int wmt_jdec_dump_fb_data(jdec_frameinfo_t *fbinfo);

int wmt_jdec_buf_in_size(
    vd_handle_t      *handle,
    unsigned char    *buf_in, 
    unsigned int      bufsize,
    unsigned int      request_w,
    unsigned int      request_h,
    jpeg_scale_alg    scale_alg,
    jdec_frameinfo_t *frameinfo);

int wmt_jdec_file_in_size(
    vd_handle_t      *handle, 
    char             *filename, 
    unsigned int      request_w,
    unsigned int      request_h,
    jpeg_scale_alg    scale_alg,
    jdec_frameinfo_t *frameinfo);

/*!*************************************************************************
* wmt_jdec_set_resolution
*
*   Change system resolution
*
* \retval  0 if success, else return a negtive value
*/ 
int wmt_jdec_set_resolution(
    vd_handle_t  *handle, 
    unsigned int  resx, 
    unsigned int  resy, 
    unsigned int  fps);

/*!*************************************************************************
* wmt_jdec_fb_alloc
*/
/*!
* \brief   Allocate a phyiscally continuously memory for HW decoder
*
* \parameter
*   pic_w         [IN] Decoded picture width  (unit: pixel)
*   pic_h         [IN] Decoded picture height (unit: pixel)
*   src_color     [IN] Picture color format
*   decoded_color [IN] Decoded picture color format
*   arg_in        [OUT] 6 return values are stored in this structure
* \retval  0 if success, else return a negtive value
*/
int wmt_jdec_fb_alloc (
    vd_handle_t    *handle,
    unsigned int    pic_w,  
    unsigned int    pic_h,
    vdo_color_fmt   src_color,
    vdo_color_fmt   decoded_color_in,
    jdec_input_t   *arg_in);

int wmt_jdec_fb_free (vd_handle_t *handle, jdec_frameinfo_t *info);

int wmt_jdec_get_resolution(
    vd_handle_t  *handle, 
    unsigned int *disp_x,
    unsigned int *disp_y);

/*!*************************************************************************
* wmt_jdec_attr_ready
*
*   Inform HW decoder that user will start to decode
*
* \retval  0 if success, else return a negtive value
*/ 
int wmt_jdec_attr_ready(vd_handle_t *handle);

/*------------------------------------------------------------------------------
    Attributions Setting API
------------------------------------------------------------------------------*/
int wmt_jdec_get_attr(vd_handle_t *handle, jdec_attr_t *attr);
int wmt_jdec_set_attr(vd_handle_t *handle, jdec_attr_t *attr);

int wmt_jdec_set_scale(vd_handle_t *handle, vd_scale_ratio ratio);        
    /* default: SCALE_BEST_FIT   */
    
int wmt_jdec_set_dc_format(vd_handle_t *handle, vdo_color_fmt decoded_color); 
    /* default: VDO_COL_FMT_AUTO */

int wmt_jdec_set_pd(vd_handle_t *handle, jdec_pd *pd_in);        
    /* default: Diable PD function  */

int wmt_jdec_set_decode_info(vd_handle_t *handle, jdec_decode_info_t *di_in);

int wmt_jdec_bitblt_ex(
    vd_handle_t      *handle,
    jdec_frameinfo_t *dst_fb, 
    int nXDest, 
    int nYDest,
    jdec_frameinfo_t *src_fb, 
    int nXSrc,
    int nYSrc,
    int nWidth,
    int nHeight);

int wmt_jdec_wait_fb_ready(vd_handle_t *handle, jdec_frameinfo_t *fb_in);

int wmt_jdec_mem_alloc (
    vd_handle_t    *handle,
    unsigned int    buf_w,  
    unsigned int    buf_h,
    unsigned int    bpp_in,
    wmt_jpeg_mem_t *mem_out);

int wmt_jdec_mem_free (vd_handle_t *handle, wmt_jpeg_mem_t *mem_in);

int wmt_jdec_alloc (
    vd_handle_t      *handle,
    unsigned int      buf_w,  
    unsigned int      buf_h,
    vdo_color_fmt     color_fmt,
    jdec_frameinfo_t *jfb);

int wmt_jdec_free(vd_handle_t *handle, jdec_frameinfo_t *jfb);

int wmt_jdec_msd_enable(
    vd_handle_t *handle, 
    unsigned int fileszie,     /* it may be zero if you do not know filesize */
    unsigned int segment_size, /* unit in Bytes */
    unsigned int msd_enable);  /* 0: disalbe MSD;   1: enable MSD */

int wmt_jdec_header_ready(vd_handle_t *handle, int hdr_parsed);

int wmt_jdec_stretch(
    vd_handle_t      *handle,
    unsigned int      request_w,
    unsigned int      request_h,
    jpeg_scale_alg    scale_alg,
    jdec_frameinfo_t *jfb_in);

int wmt_jdec_segment_write(
    vd_handle_t   *handle,
    unsigned char *buf_in, 
    unsigned int   bufsize, 
    unsigned int   filesize, 
    unsigned int   seg_no);

int wmt_jdec_get_status(vd_handle_t *handle, jdec_status_t *drvsts);
            
int wmt_jdec_rotate(
    vd_handle_t      *handle,
    jdec_frameinfo_t *src_fb,
    jdec_frameinfo_t *dst_fb,
    unsigned int      rot_degree);

int wmt_jdec_slide_show(
    vd_handle_t      *handle,
    jdec_frameinfo_t *fb_bottom, 
    jdec_frameinfo_t *fb_top, 
    jpeg_slide_mode   slide_mode,
    unsigned int      step_count,
    unsigned int      sub_step_count);

int wmt_jdec_slide_show_stop(vd_handle_t *handle);

int wmt_jdec_vdma_nonblocking_en(vd_handle_t *handle, unsigned int en);
int wmt_jdec_wait_vdma(vd_handle_t *handle, unsigned int channel, int flag);

int wmt_jdec_auto_orientation(vd_handle_t *handle, int enable);

int wmt_jdec_segment_decode(
    vd_handle_t   *handle,
    unsigned char *buf_in, 
    unsigned int   bufsize, 
    unsigned int   last_segment, 
    unsigned int   seg_no);

/*------------------------------------------------------------------------------
    API for debugging
------------------------------------------------------------------------------*/
/* Memory Block Debug API (Trace memory usage) */
int wmt_jdec_mb_dgb_start(void);
int wmt_jdec_mb_dgb_end(void);

/* Dump information of frame buffer (debug only) */
int wmt_jdec_dump_frame(
    vd_handle_t      *handle,
    jdec_frameinfo_t *pfb, 
    char *name);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_JDEC_H */

/*=== END wmt-jdec.h ==========================================================*/
