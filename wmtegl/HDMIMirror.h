/*
 * Copyright 2011 WonderMedia Technologies, Inc. All Rights Reserved. 
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

 
#ifndef WMTEGL_HIDE_IMPL_HDMIMIRROR_H_INCLUDED
#define WMTEGL_HIDE_IMPL_HDMIMIRROR_H_INCLUDED  
 
#include <utils/threads.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <ui/FramebufferNativeWindow.h>
#include <gui/SurfaceTexture.h>
#include <utils/Vector.h>

#include "wmtegl.h"
#include "Common.h"
#include "FBNativeWindow.h"
#include "Blitter.h"

using namespace android;

//vdo_framebuf_t is an anonymous structures so I can't forward declaration.
#include "com-video.h"  //for vdo_framebuf_t



void extDisplayPlug(bool enable, int resx, int resy);

enum Stage{
    NOINIT,
    INITDONE,
    STOP,
    EXITED,
};

class HDMIMirror : public android::Thread
{
public:
    HDMIMirror();
    ~HDMIMirror();

	int init();
    void deinit();
	void newFrameReady();
    void newFramePosted();
    void newVideoFrameReady(SurfaceTexture * st, sp<GraphicBuffer>& graphicBuffer);
    void newVideoUIReady();

    void setVideoMode(bool videoMode);
    bool isVideoMode() const {
        return mVideoMode;
    }

    void showBlankVideo(const Region& clip, const GLfloat mVertices[4][2]);
    void    endMirrorWithSCL444(int bufIdx);    
protected:
	virtual android::status_t readyToRun();
    virtual bool     threadLoop();
    
    void    freeGLResources();

    void    drawUIAboveVideo();

    void    startMirrorWithSCL444();

    void    startMirrorWithMali();
    void    endMirrorWithMali();
private:
    Mutex mHDMIMutex;
    Condition    mHDMICond;
    int          mMBFD;

  
    Stage   mStage;

    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;

	//for UI mirror
    struct FBImage  {
        FBImage() {
            eglImage = 0;
            textureId = 0;
        }

        EGLImageKHR  eglImage;
        unsigned int textureId;
    };

    status_t createFBImages(WmtEGLImageArgs& imageArgs, FBImage* fbImage, int count);

    FBImage   mFB0Image[NUM_FRAME_BUFFERS];
    int       mFB0NewBuffer;        //fb0's new buffer index
    int       mFB0CopyingBuffer;    //coping buffer index of fb0
    int       mFB0LastCopiedBuffer ;
    int       mUITextureType;  

    //For Video mirror
    GLuint              mVideoTexName;
    MBImageCacheImpl    mImageCacheImpl;
    gralloc_mb_block 	mNewVideoBlock;
    gralloc_mb_block 	mLastVideoBlock;
    WmtEGLWindowArgs    mFB1WindowArgs;

    int                 mFB1CurBufIndex;


    //current center area with avaliable content, outside must fill black boundary
    Rect  mOverScan[3];       //3 for triple-buffer
    

    int     mVideoTextureType;
    bool    mVideoMode;

    GLuint  mTVIconTexture;

    SCL444Blitter mSCL444;
    GEBlitter     mGE;
};

#endif /* WMTEGL_HIDE_IMPL_HDMIMIRROR_H_INCLUDED */

