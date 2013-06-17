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

#ifndef MBIMAGECACHE_H_INCLUDED
#define MBIMAGECACHE_H_INCLUDED

#include <ui/GraphicBuffer.h>

using namespace android;

class MBImageCacheImpl;

class MBImageCache
{
public:
    MBImageCache();
    ~MBImageCache();

	EGLBoolean 	detatchGraphicBuffer(sp<GraphicBuffer>& graphicBuffer, EGLImageKHR image);
	EGLImageKHR getImage(sp<GraphicBuffer>& graphicBuffer);
    void        updateLastBuffer(int i, int updateFlag, sp<GraphicBuffer>& graphicBuffer);
	void 		destroyCache();
private:
    MBImageCache(const MBImageCache& noimpl);
    MBImageCache& operator=(const MBImageCache& noimpl);
    
    MBImageCacheImpl * mImpl;
};


bool isMBGraphicsBuffer(const sp<GraphicBuffer>& graphicBuffer);


#endif	//MBIMAGECACHE_H_INCLUDED

