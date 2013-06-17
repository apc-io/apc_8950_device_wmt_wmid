/*
**
** Copyright (C) 2009 0xlab.org - http://0xlab.org/
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef _V4L2CAMERA_H
#define _V4L2CAMERA_H

#define NB_BUFFER 6

#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <linux/videodev.h>

#define DEFAULT_FRAME_RATE 15

#define CAM_CMOS  0
#define CAM_USB 1

#define CAMERA_CMOS_NAME  "/dev/video0"
#define CAMERA_USB_NAME	"/dev/video1"

#define YUV420 1
#define YUV422 0

struct vdIn;        // forward declaration is enough [Vic]

namespace android {

class V4L2Camera {

public:
    V4L2Camera();
    ~V4L2Camera();

    int Open (const char *device, int width, int height, int pixelformat);
    void close();
    bool isOpened() const;

    void term ();

	int v4l2_ioctl_queycap(void);
	int v4l2_ioctl_s_fmt(int width, int height, unsigned int pixelformat);
	int v4l2_ioctl_reqbufs(int count);
	int v4l2_ioctl_querybuf(int count);
	int v4l2_ioctl_qbuf(int count);

    int startStreaming();
    int stopStreaming();
    void stopPreview();
    int stopSnap(void);
	void convert(unsigned char *buf, unsigned char *rgb, int width, int height);    
	void setRecordBuf(char *add);
	void setCameraid(int id);
	int getPreviewSize(void);
	int grabPreviewFrame(char *recordBuffer);
	int getPictureraw(char *src, char *dest, int w, int h);
	void swCropImagetoYUV420(char *src, char *dest, int scr_w, int src_h, int src_f, int dst_w, int dst_h);

private:
    int             mVideoFd;       //v4l2 device file 
    struct vdIn *   mVideoIn;

    int             mQueued;
    int             mDequeued;
	unsigned int buffer_length;
    int v4l2_preview_width;
    int v4l2_preview_height;

	int hal_preview_width;
    int hal_preview_height;

    bool mTakingPicture;
	int mCameraid;
	char *mRecordBuf;

    void YUV422toYUV420(void * cameraIn, char *record_buf);
	void YUY2toYUV420(void * cameraIn, char *record_buf);
	void RecordYUY2toYUV420(void * cameraIn, char *record_buf);
	int EnumerateImageFormats(void);
    int setFrameRate(int fps);
};

}; // namespace android

#endif
