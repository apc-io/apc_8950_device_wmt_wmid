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
#define LOG_TAG "WMT_MRP"

#include <ui/GraphicBufferMapper.h>
#include <cutils/properties.h>
#include <linux/fb.h>
#include <surfaceflinger/ISurface.h>
#include <surfaceflinger/Surface.h>
#include <surfaceflinger/SurfaceComposerClient.h>
#include <gui/ISurfaceTexture.h>
#include "wmt-mb.h"
#include "wmt_mrp.h"

using namespace android;

#if defined(__cplusplus)
extern "C" {
#endif

#define MRP_BUF_CNT 3 // related to wplayer frame buffer count

static sp<ISurfaceTexture> surfaceTexture = NULL;
static sp<ANativeWindow> mrp_anw = NULL;
static sp<SurfaceComposerClient> scc;
static sp<SurfaceControl> sc;


static int get_screen_res(int *w, int *h)
{
    struct fb_var_screeninfo scr_info;
    int fb0_fd;

    if ((fb0_fd = open("/dev/graphics/fb0", O_RDWR)) == -1) {
        LOGE("Can't open fb0: %s", strerror(errno));
        return -1;
    }

    /* Get current FB resolution */
    if( 0 != ioctl(fb0_fd, FBIOGET_VSCREENINFO, &scr_info)) {
        LOGE("FBIOGET_VSCREENINFO error: %s", strerror(errno));
        close(fb0_fd);
        return -1;
    }

    *w = scr_info.xres;
    *h = scr_info.yres;
    close(fb0_fd);
    return MRP_TRUE;
}

void wmt_mrp_uninit()
{
    LOGI("wmt_mrp_uninit");
    if(mrp_anw.get() != NULL) {
        native_window_api_disconnect(mrp_anw.get(), NATIVE_WINDOW_API_MEDIA);
        LOGI("native window disconnect");
    }

    if(scc != NULL) {
        scc->dispose();
        scc = NULL;
        LOGI("scc->dispose");
    }

    if(sc != NULL) {
        sc.clear();
        sc = NULL;
        LOGI("sc.clear()");
    }

    surfaceTexture = NULL;    
}

int wmt_mrp_init(void *surface)
{
    int width, height;

    sp<Surface> mSurface((Surface*)(surface));
    if(mSurface != NULL) {
        surfaceTexture = mSurface->getSurfaceTexture();
    }
    else
        LOGI("mSurface = NULL");

    if(surfaceTexture != NULL) {
        mrp_anw = new SurfaceTextureClient(surfaceTexture);
    }
    else {
        LOGI("Create SurfaceTexture");
        scc = new SurfaceComposerClient;
        if(scc == NULL) {
            LOGE("Surface Composer Client create fail");
            return MRP_FALSE;
        }

        if(get_screen_res(&width, &height) != MRP_TRUE) {
            wmt_mrp_uninit();
            return MRP_FALSE;
        }

        sc = scc->createSurface(String8("WMT_MRP"), 0, width, height, HAL_PIXEL_FORMAT_RGB_565, 0);
        if(sc == NULL) {
            LOGE("CreateSurface fail");
            wmt_mrp_uninit();
            return MRP_FALSE;
        }

        scc->openGlobalTransaction();
        sc->setPosition(0, 0);
        sc->setLayer(INT_MAX - 10);
        scc->closeGlobalTransaction();

        surfaceTexture = sc->getSurface()->getSurfaceTexture();
        mrp_anw = new SurfaceTextureClient(surfaceTexture);
    }

    if(native_window_api_connect(mrp_anw.get(), NATIVE_WINDOW_API_MEDIA) != OK) {
        LOGE("Connect to native window fail");
        wmt_mrp_uninit();
        return MRP_FALSE;
    }

    // initialization
    if(mrp_anw.get() == NULL){
        LOGE("Init fail. (Unknown Native Window)");
        wmt_mrp_uninit();
        return MRP_FALSE;
    }

    native_window_set_usage(
            mrp_anw.get(),
            GRALLOC_USAGE_SW_READ_NEVER | GRALLOC_USAGE_SW_WRITE_OFTEN
            | GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_EXTERNAL_DISP | GRALLOC_USAGE_WMT_MB);

    native_window_set_scaling_mode(
            mrp_anw.get(),
            NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);

    LOGI("WMT_MRP inited");

    return MRP_TRUE;
}

int mrp_video_config(mrp_view *view)
{
    android_native_rect_t crop;

    if(!view) {
        LOGE("Null view !");
        return MRP_FALSE;
    }

    if(mrp_anw.get() == NULL) {
        LOGE("Config fail. (Unknown Native Window)");
        return MRP_FALSE;
    }

    crop.left = view->crop_left;
    crop.top = view->crop_top;
    crop.right = view->image_width;
    crop.bottom = view->image_height;

    native_window_set_buffers_geometry(
            mrp_anw.get(),
            view->fb_width, view->fb_height,
            HAL_PIXEL_FORMAT_YCrCb_420_SP);
    native_window_set_crop(mrp_anw.get(), &crop);
    native_window_set_buffer_count(mrp_anw.get(), MRP_BUF_CNT);

    LOGI("MRP video config : fb_w (%d), fb_h (%d), crop_left (%d), crop_top (%d), crop_right (%d), crop_bottom (%d)",
        view->fb_width, view->fb_height, crop.left, crop.top, crop.right, crop.bottom);
    return MRP_TRUE;
}

int mrp_video_flip(vdo_framebuf_t *fb)
{
    ANativeWindowBuffer *buf = NULL;
    gralloc_mb_block *block = NULL;
    nsecs_t ts = 0;
    int err;

    if(!fb) {
        LOGE("Null fb");
        return MRP_FALSE;
    }

    if(mrp_anw.get() == NULL){
        LOGE("flip fail. (Unknown Native Window)");
        return MRP_FALSE;
    }

    ts = systemTime(SYSTEM_TIME_MONOTONIC)/1000000;
    if((err = mrp_anw->dequeueBuffer(mrp_anw.get(), &buf)) != 0) {
        LOGE("Surface dequeueBuffer returned error %d", err);
        return MRP_FALSE;
    }
    ts = systemTime(SYSTEM_TIME_MONOTONIC)/1000000 - ts;
    if(ts >= 25)
        LOGD("wait native window over %lld ms",ts);

    // update buffer
    mrp_anw->lockBuffer(mrp_anw.get(), buf);
    GraphicBufferMapper &mapper = GraphicBufferMapper::get();
    Rect bounds(fb->img_w, fb->img_h);
    mapper.lock(buf->handle, GRALLOC_USAGE_SW_WRITE_OFTEN, bounds, (void **)&block);
    block->magic = 'WMMB';
    block->fb_width = fb->fb_w;
    block->fb_height = fb->fb_h;
    block->video_width  = fb->img_w;
    block->video_height = fb->img_h;
    block->phy_addr     = (unsigned long)fb->y_addr;
    mapper.unlock(buf->handle);

    if ((err = mrp_anw->queueBuffer(mrp_anw.get(), buf)) != 0) {
        LOGE("Surface::queueBuffer returned error %d", err);
        return MRP_FALSE;
    }

    return MRP_TRUE;
}

#if defined(__cplusplus)
}
#endif
