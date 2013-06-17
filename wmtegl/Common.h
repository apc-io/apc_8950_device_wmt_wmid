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

#ifndef WMTEGL_COMMON_H_INCLUDED
#define WMTEGL_COMMON_H_INCLUDED

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <ui/GraphicBuffer.h>

#include <utils/threads.h>

using namespace android;

#define MAX_WMTMB_ELGIMAGE_BUFFER 32      //this value must > video player mb block count.

static bool inline isWMTMBGraphicsBuffer(const sp<GraphicBuffer>& graphicBuffer)
{
    return (graphicBuffer != NULL) && (graphicBuffer->getUsage() & GRALLOC_USAGE_WMT_MB);
}

static inline bool isVideoGraphicsBuffer(const sp<GraphicBuffer>& graphicBuffer)
{
    //check a more bit GRALLOC_USAGE_EXTERNAL_DISP
    return (graphicBuffer != NULL) && 
        (graphicBuffer->getUsage() & GRALLOC_USAGE_WMT_MB) &&
        (graphicBuffer->getUsage() & GRALLOC_USAGE_EXTERNAL_DISP);
}


struct AutoGraphicBufferLocker
{
	AutoGraphicBufferLocker(sp<GraphicBuffer>& graphicBuffer) 
		:  gb(graphicBuffer)
	{
		lockStatus = NO_ERROR + 1;
	}

	gralloc_mb_block * lock() {
		void* vaddr;
		lockStatus = gb->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, &vaddr);
		return (lockStatus == NO_ERROR) ? (gralloc_mb_block *)vaddr : NULL;
	}

	void unlock() {
		if( lockStatus == NO_ERROR)
			gb->unlock();
		lockStatus = NO_ERROR + 1;		//prevent unlock again
	}

	~AutoGraphicBufferLocker(){
		if( lockStatus == NO_ERROR)
			gb->unlock();
	}

	sp<GraphicBuffer>& gb;
	status_t lockStatus;
};


static inline bool isLargeVideoGraphicsBuffer(const sp<GraphicBuffer>& graphicBuffer,
                                              int maxFB0VideoWidth, int maxFB0VideoHeight)
{
    //check a more bit GRALLOC_USAGE_EXTERNAL_DISP and video width/height
    if(!isVideoGraphicsBuffer(graphicBuffer))
        return false;

    //check the video resolution
    AutoGraphicBufferLocker lock(const_cast<sp<GraphicBuffer>&>(graphicBuffer));
    gralloc_mb_block * block = lock.lock();
    if(block) {
        return block->video_width * block->video_height > maxFB0VideoWidth * maxFB0VideoHeight;
    }
    return false;
}


struct MBAddrImageMap{
    MBAddrImageMap() {
		reset();
    }

	void reset() {
		phyAddr = 0;
        image = EGL_NO_IMAGE_KHR;
        display = EGL_NO_DISPLAY;
	}

    long phyAddr;
    EGLImageKHR image;
    EGLDisplay  display;
};



class MBImageCacheImpl {
public:
    MBImageCacheImpl();
    ~MBImageCacheImpl();

    void destroyCache();
	//create and cache image.
    bool 	detatchGraphicBuffer(const gralloc_mb_block * block, EGLImageKHR checkImage);

	EGLImageKHR getImage(const gralloc_mb_block * block);
    void updateLastMBBuffer(int updateFlag, const gralloc_mb_block * newblock);
    
private:
	Mutex mMutex;

    //Add for Record queueBuffer/updateTexImage buffer slot id for MB buffer reference
    int mFD;
    int mLastUpdatePhyAddr[3];
    int mLastQueuePhyAddr;
    
    EGLImageKHR findImageLocked(const gralloc_mb_block * block);
    EGLImageKHR createImageLocked(const gralloc_mb_block * block);


    MBAddrImageMap mCache[MAX_WMTMB_ELGIMAGE_BUFFER];
};

extern int getmbfd();

#endif  //WMTEGL_COMMON_H_INCLUDED


