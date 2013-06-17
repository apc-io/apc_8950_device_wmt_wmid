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
#ifndef WMT_API_H
/* To assert that only one occurrence is included */
#define WMT_API_H

/*--- History ------------------------------------------------------------------- 
* 
*  6.12.0 (2012-04-14):
*       - Support SCL as bitblt engine
*
*  6.11.0 (2012-03-08):
*       - Fix a bug in JPEG module for new MB architecture
* 
*  6.10.0 (2012-02-20):
*       - Add a new API wmt_jdec_segment_decode for stream JPEG
*
*  6.9.0 (2012-01-12):
*       - Support both YC420 and YUV (3 planes) decoded frame buffer
*       - Add a VPP API for application to set CEC logical address
*
*  6.8.0 (2011-12-13):
*       - Fix a bug in H.264 encoder about memory allcate issue in PRD
*
*  6.7.0 (2011-10-21):
*       - Add levelIDC for H.264 encoder
*       - Add new API wmt_vpp_set_csc(), wmt_vpp_stream_get_fb() and wmt_vpp_stream_put_fb()
*       - Add error handling for MAP_FAILED in MB
*       - Add AUTO path for WMT VPP VO Path
*
*  6.6.0 (2011-09-15): 
*       - Add new ApI wmt_vpp_vout_get_cp_info()
*
*  6.5.0 (2011-09-02): 
*       - Fix a bug in WMT SE about AES counter mode
*
*  6.4.0 (2011-08-23): 
*       - Add new API wmt_jdec_auto_orientation() to support auto rotate 
*         according to orientation in Exif
*
*  6.3.0 (2011-08-11): 
*       - Add H.264 hardware encoder API
*
*  6.2.0 (2011-07-28): 
*       - Fix a bug for progressvie DCT JPEG
*       - Merge wmt-lock into WMT API
*       - Add HDMI CEC API
*
*  6.1.0 (2011-07-04): 
*       - Fix a bug for H.264 reused chunk
*       - Fix bugs in seek function
*       - Add GE into WMT API
*
*  6.0.0 (2011-05-11): 
*       - Support H.263
*       - Add new API wmt_jdec_get_header( ) and wmt_vpp_get_edid()
*
*  5.7.0 (2011-03-04): 
*       - Add new API wmt_vpp_get_fbdisp_info()
* 
*  5.6.0 (2010-12-31): 
*       - Support WM8710
*       - Add VE (Video Encoder) modules
*
*  5.5.0 (2010-12-16): 
*       - Add new API: wmt_vpp_get_resolution()
*       - Add new APIs: wmt_vpp_cursor_set_color_key() and wmt_vpp_cursor_set_hotspot()
*         for HW cursor
*
*  5.4.0 (2010-11-22): 
*       - Refine VPP to suppor WM8650 new features
*       - Add new two APIs: wmt_vpp_scale() and wmt_vpp_scale_finish()
*
*  5.3.0 (2010-11-2): 
*       - Refactoring video decoders
*
*  5.2.0 (2010-10-14): 
*       - Support video decoder direct path 
*
*  5.1.0 (2010-09-23): 
*       - Modify VPP to support bilinear mode up to 1/2048 precision
*       - Refine wmt_jdec_bitblt_ex for VDMA non-blocking mode
*       - Refine and robust wmt_jdec_segment_write( )for MSD
*
*  5.0.0 (2010-08-20): 
*       - Refine to support WM8440
*       - Add VDO_COL_FMT_RGB_888, VDO_COL_FMT_RGB_666, VDO_COL_FMT_RGB_565,  
*         VDO_COL_FMT_RGB_1555, VDO_COL_FMT_RGB_5551 in vdo_color_fmt
*
*  4.8.0 (2010-07-13): Support VDMA driver non-blocking mode
*                      Refine MB API for robustness
*                      Add two APIs wmt_jdec_vdma_nonblocking_en and wmt_jdec_wait_vdma
*
*  4.7.0 (2010-06-23): Add new API wmt_get_chip_info
*                      Refine wmt_jdec_file() to improve performance inside this API
*
*  4.6.0 (2010-06-11): Support for both MVL and Android version
*                      Fix a bug and remove useless code in wmt_jdec_stretch()
*
*  4.5.0 (2010-04-09): Add new API wmt_mb_phys_alloc() and wmt_mb_phys_free()
*
*  4.4.0 (2010-03-29): Add a new API wmt_jdec_slide_show() to support slide show
*                      Bugfxing: Keep original "scaled" when do frame buffer copy
*
*  4.3.0 (2010-03-15): Modify WMT JDEC to auto-detect EOI
*                      Add a new API wmt_jdec_bitblt_ex() to support muti-thread
*                      Refine WMT VPP API to support multiple instances
*                      Add a new API wmt_vpp_get_support_vmode() to get supported
*                        resolution from output device
*
*  4.2.5 (2010-02-23): Fix a bug in WMT JDEC because allocated memory size is too small in some special case
*
*  4.2.4 (2010-02-03): Fix a bug in JPEG_DM_FULL_SCREEN mode inside wmt_jdec_show_image()  
*
*  4.2.3 (2010-01-28): Add a new error code for illegal H/V sampling factor
*
*  4.2.2 (2010-01-03): Support audio echo canceling
*
*  4.2.1 (2009-12-17): Fix a bug in automatically downgrade for JPEG_SA_HIGH_QUALITY
*                      Add a new API wmt_jdec_set_scale_alg()
*
*  4.2.0 (2009-12-08): Add multi-open protection mechanism for all video decoders
*                      Add a flag to judge JPEG or MJPEG in com-jdec.h
*                      Fix a bug in wmt_jdec_bitblt() when user calls this API 
*                      but VDMA was not initialized
*
*  4.1.2 (2009-12-04): Fix a bug in wmt_jdec_file_in_size() about wrong return value
*                      Add new API wmt_jdec_cancel()
*
*  4.1.1 (2009-11-25): Fix a bug in SE driver if driver returns error
*                      Add new member "JPEG_SA_HIGH_QUALITY" in jpeg_scale_alg
*                      Fix a bug in wmt_jdec_file() when an error happened
*
*  4.1.0 (2009-11-06): Add new API wmt_jdec_rotate()
*
*  4.0.5 (2009-11-05): Add scaling up 32X limitation in wmt_jdec_show_image()
*
*  4.0.4 (2009-10-20): Add two new APIs wmt_jdec_alloc() and wmt_jdec_free()
*
*  4.0.3 (2009-10-13): Fix a bug in JPEG algorithm for best fit
*
*  4.0.2 (2009-10-01): Fix a bug in wmt_jdec_header_ready() under some special case
*
*  4.0.1 (2009-09-29): Fix bugs in wmt_jdec_file_in_size()
*                      Fix bugs in wmt_jdec_file_header() if filesize is very large
*
*  4.0.0 (2009-09-21): Add "aud" in WMT API
*                      Refine cipher data structure
*                      Add LCDC module in VPP moduel
*                      Add new API to dump VDU frame buffer
*                      Support new JPEG display mode: JPEG_DM_FULL_SCREEN_TORSION
*                      Add new API: wmt_api_ver_require()
*                      Add new JPEG APIs: 
*                          wmt_jdec_mem_alloc()/wmt_jdec_mem_free()                     
*                          wmt_jdec_msd_enable()   (MSD: Multi-Segment Decoding)
*                          wmt_jdec_segment_write()
*                          wmt_jdec_header_ready()
*                          wmt_jdec_stretch()
*
*  3.3.2 (2009-09-04): Add "PTS" in VPP module
*
*  3.3.1 (2009-08-27): Disable multi-segment in wmt_jdec_file()
*
*  3.3.0 (2009-08-21): Add multi-segment enable flag for all video decoders
*
*  3.2.0 (2009-08-18): Add DES,TDES,SHA1 in security engine
*                      Support multi-segment in JPEG decoder
*
*  3.1.0 (2009-07-23): Add Flush IOCTL for all vidoe decoders
*
*  3.0.1 (2009-07-16): Support RGB32 Bitblt
*
*  3.0.0 (2009-07-01): Support MPEG 1/2/4, Xvid and H.264 HW decoders
*
*  2.0.0 (2009-06-01): Support multi-thread in JPEG decoder (refine all JDEC API)
*
*  1.1.0 (2009-04-23): Modify com-xxx.h for BSP 0.5.0
*
*  1.0.6 (2009-03-27): Modify JPEG API to fix some bugs in 422V format
*                      Modify VD API to fix a bug if returned file handle is 0
*
*  1.0.5 (2009-03-19): Integrate for BSP 0.04.01 (Add [extern "C"] for C++)
*
*  1.0.4 (2009-03-05): Add one new timer API "wmt_timer_set_threshold()"
*                      Add one new API "wmt_vpp_vpu_clr_fbdisp()"
*
*  1.0.3 (2009-02-26): Add 4 timer APIs for performance analysis
*                      VDMA was integrated from this version
*
*  1.0.2 (2009-02-17): Add "ERR_JDEC_NOT_SUPPORT_DC" in wmt_err_code
*
*  1.0.1 (2009-02-03): Add enum "wmt_err_code" for all modules
*                      Update JPEG API version to 0.01.04
*
*  1.0.0 (2009-01-22): First release
*
*------------------------------------------------------------------------------*/

#define WMT_API_VERSION     "6.12.0"


#define CFG_WMT_VPP
#define CFG_WMT_JDEC
//#define CFG_WMT_GE    /* Graphic Engine */
//#define CFG_WMT_SE    /* Security Engine */


/*-------------------- MODULE DEPENDENCY -------------------------------------*/


/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
typedef enum { 
    /* Reserved for common modules: 0 ~ -99 */
    ERR_NONE    = 0,
    ERR_GENERAL = -1,

    ERR_OLD_API_VERSION           = -2,
    ERR_DRV_INVALID               = -3,
    ERR_DRIVER_NOT_OPENED         = -4,
    ERR_DRIVER_OPEN_FAIL          = -5,
    ERR_SYS_MEM_EMPTY             = -10,

    ERR_VD_UNKNOWN                = -70,
    ERR_VE_UNKNOWN                = -71,
    ERR_INIT_MB                   = -80,
    ERR_INIT_JDEC                 = -81,

    /* Reserved for JPEG decoder: -100 ~ -199 */
    ERR_JDEC_BEGIN                = -100,
    ERR_JDEC_API_VERSION_CONFLICT = ERR_JDEC_BEGIN,
    ERR_JDEC_NO_DEVICE_NODE       = -101,
    ERR_JDEC_DRIVER_NOT_OPENED    = -102,
    ERR_JDEC_DRIVER_MULTI_OPEN    = -103,
    ERR_JDEC_MB_NOT_OPENED        = -104,
    ERR_JDEC_VDMA_FAIL            = -105,
    ERR_JDEC_NOT_SUPPORT_PROFILE  = -120,
    ERR_JDEC_NOT_SUPPORT_IMAGE    = -121,
    ERR_JDEC_NOT_SUPPORT_NF       = -122,
    ERR_JDEC_NOT_SUPPORT_DC       = -123,  /* DC: Decode Color Format */
    ERR_JDEC_PARSE_READ_EOI       = -124,
    ERR_JDEC_NOT_READ_SOI         = -125,
    ERR_JDEC_MEMORY_NOT_ENOUGH    = -126,
    ERR_JDEC_IO_FAIL              = -127,
    ERR_JDEC_ILLEGAL_INPUT        = -128,
    ERR_JDEC_NOT_SUPPORT_HV       = -129,
    ERR_JDEC_END                  = -199,

    /* Reserved for MB: -200 ~ -299 */
    ERR_MB_BEGIN                = -200,
    ERR_MB_END                  = -299,

    /* Reserved for VPP: -300 ~ -399 */
    ERR_VPP_BEGIN                = -300,
    ERR_VPP_END                  = -399,

    /* Reserved for GE: -400 ~ -499 */
    ERR_GE_BEGIN                = -400,
    ERR_GE_END                  = -499,

    /* Reserved for SE: -500 ~ -599 */
    ERR_SE_BEGIN                = -500,
    ERR_SE_END                  = -599,

} wmt_err_code;


/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
    API for performance analysis
------------------------------------------------------------------------------*/
int wmt_timer_init(int id, char *name);
int wmt_timer_start(int id);
int wmt_timer_stop(int id, int show);
int wmt_timer_show(int id);
int wmt_timer_set_threshold(int id, unsigned int usec);
int wmt_timer_get_total_time(int id, unsigned int *usec);

int wmt_api_ver_require(char *version);
int wmt_get_chip_info(unsigned int *chip_id, unsigned int *chip_ver);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_API_H */

/*=== END wmt-api.h ==========================================================*/
