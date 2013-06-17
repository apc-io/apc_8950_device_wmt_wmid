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
#ifndef FBNATIVEWINDOW_H_INCLUDED
#define FBNATIVEWINDOW_H_INCLUDED


#ifndef EGL_EGLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define	GL_GLEXT_PROTOTYPES
#endif


#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <string.h>     //for memset
#include <unistd.h>
#include "gralloc_priv.h"
#include <utils/threads.h>
#include <ui/GraphicBuffer.h>
#include <ui/egl/android_natives.h>


class FBNativeWindow;

struct WmtEGLWindowArgs
{
//#ifdef __cplusplus
    WmtEGLWindowArgs() {
        memset(this, 0, sizeof(*this));
        fbDev = 1;  //default is from mbdev
    }
//#endif

    int w;
    int h;
    int fmt;
    int bufCount;
    int phyAddr;
    void * virAddr;
    int fbDev;        //from fb0 or mbdev, 0: fb0  , 1:mbdev

    void * userData;
    int (*post)(FBNativeWindow * self, void * userData, int bufIndex);
    int (*close)(FBNativeWindow * self, void * userData);
};

/**
 * Create EGLWindow from frame buffer memory. 
 * the EGLWindow can be used as the opengl target surface. 
 */
EGLNativeWindowType wmt_createEGLWindow(struct WmtEGLWindowArgs * args);


/**
 *	Create FB1 OpenGL window. FB1 can be used as HDMI output.
 */
EGLNativeWindowType wmt_createFb1EGLWindow(WmtEGLWindowArgs * outArgs = NULL);


struct WmtEGLImageArgs 
{
    WmtEGLImageArgs(){
        fbDev = 1;//default is 1 from /dev/mbdev
    }

    //[in]
    EGLDisplay dpy;
    int w;
    int h;
    int fmt;
    int phyAddr;
    int fbDev;  //either 0 for /dev/fb0 or 1 for /dev/mbdev
	
    //[out]
    void * virAddr;		/* based on fb0's start address */
    int    stride;
};

using namespace android;

class FBNativeBuffer 
    : public EGLNativeBase<ANativeWindowBuffer, FBNativeBuffer, LightRefBase<FBNativeBuffer> > {
public:
    FBNativeBuffer(int w, int h, int f, int u, int phyAddr, int dev);
    int getSize() const;
    int getPhyAddr() const { return phyAddr; }
private:
    int     phyAddr;
    friend class LightRefBase<FBNativeBuffer>;    
    ~FBNativeBuffer();
};



//A generic class for fb memory window.
class FBNativeWindow  
        : public EGLNativeBase<
                 ANativeWindow, 
                FBNativeWindow, 
                LightRefBase<FBNativeWindow> > 
{
public:
    FBNativeWindow(WmtEGLWindowArgs * args); 
    bool initCheck();

private:
    friend class LightRefBase<FBNativeWindow>;    
    ~FBNativeWindow(); // this class cannot be overloaded
    static int setSwapInterval(ANativeWindow* window, int interval);
    static int dequeueBuffer(ANativeWindow* window, ANativeWindowBuffer** buffer);
    static int lockBuffer(ANativeWindow* window, ANativeWindowBuffer* buffer);
    static int queueBuffer(ANativeWindow* window, ANativeWindowBuffer* buffer);
    static int query(const ANativeWindow* window, int what, int* value);
    static int perform(ANativeWindow* window, int operation, ...);

    sp<FBNativeBuffer> mBuffers[32];
    sp<FBNativeBuffer> mFront;

    WmtEGLWindowArgs mArgs;

    mutable Mutex mutex;
    Condition mCondition;
    int32_t mNumBuffers;
    int32_t mNumFreeBuffers;
    int32_t mBufferHead;
    int32_t mCurrentBufferIndex;
};

/**
 * Create EGLImage from framebuffer memory. 
 * The EGLImage can be used as the texture (the source image) 
 * @param args 
 * @return EGLImageKHR 
 */
EGLImageKHR wmt_createEGLImage(WmtEGLImageArgs* args);


/**
 * return 0 if OK.
 */
int getStrideAndSize(int w, int h, int fmt, int * outStride, int * outSize);


long getFBStartPhyAddress(int dev);

	
namespace android{
    class String8;
}
void dumpFBNativeWindowObject(android::String8& result, char* buffer, size_t SIZE);
	

#endif // FBNATIVEWINDOW_H_INCLUDED


