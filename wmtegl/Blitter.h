/*
 * Copyright 2012 WonderMedia Technologies, Inc. All Rights Reserved. 
 *  
 * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc. 
 * and may contain trade secrets and/or other confidential information of 
 * WonderMedia Technologies, Inc. This file shall not be disclosed to any third party, 
 * in whole or in part, without prior written consent of WonderMedia. 
 *  
 * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS, 
 * WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED, 
 * AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  
 */

#ifndef WMT_BLITTER_H_INCLUDED
#define WMT_BLITTER_H_INCLUDED

//vdo_framebuf_t is an anonymous structures so I can't forward declaration.
#include "com-video.h"  //for vdo_framebuf_t
#include "Common.h"
#include "FBNativeWindow.h"
#include "com-ge.h"

struct framebuffer_device_t;

void vdo_framebuf_t_fb0(const framebuffer_device_t * device, 
                  int bufIndex, 
                  vdo_framebuf_t& out);

void vdo_framebuf_t_fb1(WmtEGLWindowArgs * FB1WindowArgs,
                  int bufIndex, 
                  Rect * OverScan,
                  vdo_framebuf_t& out);

void vdo_framebuf_t_mb(const gralloc_mb_block& mb, vdo_framebuf_t& out);


class SCL444Blitter
{
public:
    SCL444Blitter();

    void init(WmtEGLWindowArgs * FB1WindowArgs) {
        mFB1WindowArgs = FB1WindowArgs;
    }

    void flip(int fb1CurBufIndex);

    int mirrorBlit(vdo_framebuf_t& src_fb, vdo_framebuf_t& out_fb);

    int stretch(vdo_framebuf_t& src_fb, vdo_framebuf_t& dst_fb);
private:
    

    //current center area with avaliable content, outside must fill black boundary
    WmtEGLWindowArgs * mFB1WindowArgs;
};

class GEBlitter
{
public:
    GEBlitter();
    ~GEBlitter();


    //int blitWithSrcColorKey(vdo_framebuf_t& src_fb, vdo_framebuf_t& dst_fb);
    int setAlphaAndSrcColorKey();

private:
    ge_info_t *geinfo;
    //volatile ge_regs_t *regs;
};



#endif  // WMT_BLITTER_H_INCLUDED


