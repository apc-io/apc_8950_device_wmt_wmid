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

#define LOG_TAG "V4L2Camera"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <linux/fb.h>
#include <linux/videodev.h>

extern "C" {
    #include <jpeglib.h>
  int wmt_getsyspara(const char *varname, char *varval, int * varlen);
}

#include "V4L2Camera.h"

#define WMT_ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

static int cam_rotate;
static int cam_mir;

/*
struct vdIn {
    struct v4l2_capability cap;
    struct v4l2_format format;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
    void *mem[NB_BUFFER];
    bool isStreaming;
    int width;
    int height;
    int formatIn;
    int framesizeIn;
};
*/

struct vdIn {
    void *mem[NB_BUFFER];
    bool isStreaming;
};

namespace android {


V4L2Camera::V4L2Camera ()
    : mQueued(0), 
      mDequeued(0)
{
    mRecordBuf = NULL;
    mTakingPicture = false;
	mCameraid = -1;
    mVideoFd = -1;
    mVideoIn = (struct vdIn *) calloc (1, sizeof (struct vdIn));
    v4l2_preview_width = 0;
    v4l2_preview_height = 0;
}

V4L2Camera::~V4L2Camera()
{
	free(mVideoIn);
}


int V4L2Camera::v4l2_ioctl_queycap(void)
{
	struct v4l2_capability cap;
	int ret;

	memset(&cap, 0, sizeof(struct v4l2_capability));
	ret = ioctl (mVideoFd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		LOGE("Error opening device: unable to query device cap.");
		return -1;
	}

    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
        LOGE("Error opening device: video capture not supported. 0x%X", cap.capabilities);
        return -1;
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        LOGE("Capture device does not support streaming i/o");
        return -1;
    }

	return 0;
}

int V4L2Camera::v4l2_ioctl_s_fmt(int width, int height, unsigned int pixelformat)
{
	struct v4l2_format format;
	int ret;

	memset(&format, 0, sizeof(struct v4l2_format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.width = width;
	format.fmt.pix.height = height;
	format.fmt.pix.pixelformat = pixelformat;
	format.fmt.pix.field = V4L2_FIELD_ANY;
	
	ret = ioctl(mVideoFd, VIDIOC_S_FMT, &format);
	if (ret < 0) {
		LOGE("Open: VIDIOC_S_FMT Failed: %s", strerror(errno));
		return ret;
	}

	v4l2_preview_width = format.fmt.pix.width;
	v4l2_preview_height = format.fmt.pix.height;

	return 0;
}

int V4L2Camera::v4l2_ioctl_reqbufs(int count)
{
	int ret = 0;
	struct v4l2_requestbuffers rb;

	memset(&rb, 0, sizeof(struct v4l2_requestbuffers));
    rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;
    rb.count = count;

	ret = ioctl(mVideoFd, VIDIOC_REQBUFS, &rb);
    if (ret < 0) {
        LOGE("Init: VIDIOC_REQBUFS failed: %s", strerror(errno));
        return ret;
    }

	return 0;
}

int V4L2Camera::v4l2_ioctl_querybuf(int count)
{
	int i, ret;
	struct v4l2_buffer buf;

	for (i = 0; i < count; i++)	{
		memset (&buf, 0, sizeof (struct v4l2_buffer));
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

		ret = ioctl (mVideoFd, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) {
			LOGE("Init: Unable to query buffer (%s)", strerror(errno));
			return ret;
		}

		mVideoIn->mem[i] = mmap (0,
								buf.length,
								PROT_READ | PROT_WRITE,
								MAP_SHARED,
								mVideoFd,
								buf.m.offset);
		if (mVideoIn->mem[i] == MAP_FAILED) {
			LOGE("Init: Unable to map buffer (%s)", strerror(errno));
			return -1;
		}
	}
	return 0;
}

int V4L2Camera::v4l2_ioctl_qbuf(int count)
{
	int i, ret;
	struct v4l2_buffer buf;

	for (i = 0; i < count; i++) {
		memset (&buf, 0, sizeof (struct v4l2_buffer));
		buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        
        ret = ioctl(mVideoFd, VIDIOC_QBUF, &buf);
        if (ret < 0) {
            LOGE("Init: VIDIOC_QBUF Failed");
            return -1;
        }
        mQueued++;
	}
	return 0;
}


int V4L2Camera::Open(const char *device, int width, int height, int pixelformat)
{
	int ret = 0;
	int retry = 0;  
	int i;

  //LOGD("V4L2Camera::Open(%s) pixelformat(%d)" , device, pixelformat);
	if (mCameraid < 0) {
		LOGE("Camera id = %d", mCameraid);
		return -1;
	}

	if (mVideoFd > 0) {
		LOGW("device is already open !!!!");
	} else {
		if (mCameraid&CAM_USB)
			device = CAMERA_USB_NAME;
		else
			device = CAMERA_CMOS_NAME;

		while(retry < 0x10) {
			if ((mVideoFd = open(device, O_RDWR)) == -1) {
				LOGE("ERROR opening V4L interface: %s , retry ...", strerror(errno));
				retry++;
				usleep(2000000);
				continue;
			}
			break;  
		}

		if (mVideoFd == -1) {
			LOGE("ERROR opening V4L interface %s: %s", device, strerror(errno));
			return -1;
		}
	}

	ret = v4l2_ioctl_queycap();
	if (ret)
		return ret;

	hal_preview_width = width;
	hal_preview_height = height;

	if(width <= 320)
    { //Patch for Fring.apk (240 * 160)
       width = 320;
       height = 240;
    }

	if(width > 640)
    { //Patch for QRDroid.apk (800* 480)
       width = 640;
       height = 480;
    }

    if((width > 320) && (width < 640))
    { //Patch for Qik.apk (352 * 288)
       width = 320;
       height = 240;
    }

	ret = v4l2_ioctl_s_fmt(width, height, V4L2_PIX_FMT_YUYV);
	if (ret < 0)
		return ret;

	if (mCameraid&CAM_USB)
		setFrameRate(30);

	ret = v4l2_ioctl_reqbufs(NB_BUFFER);
	if (ret < 0)
		return ret;

	ret = v4l2_ioctl_querybuf(NB_BUFFER);
	if (ret < 0)
		return ret;

	ret = v4l2_ioctl_qbuf(NB_BUFFER);
	if (ret < 0)
		return ret;

	return 0;
}

void V4L2Camera::setCameraid(int id)
{
	mCameraid = id;
	LOGW("\n\nV4L2Camera::setCameraid = %d",mCameraid);
}

int V4L2Camera::setFrameRate(int fps)
{
    int ret;
    int framerate;
    struct v4l2_streamparm parm;
    //LOGD("V4L2Camera::init_parm()");

    //LOGW("setFrameRate = %d",fps);
    framerate = fps;

    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(mVideoFd, VIDIOC_G_PARM, &parm);
    if(ret != 0) {
        LOGE("VIDIOC_G_PARM fail....");
        return ret;
    }

    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = framerate;
    ret = ioctl(mVideoFd, VIDIOC_S_PARM, &parm);
    if(ret != 0) {
        LOGE("VIDIOC_S_PARM  Fail....");
        return -1;
    }

    return 0;
}

bool V4L2Camera::isOpened() const 
{
    return mVideoFd != -1;
}


void V4L2Camera::close()
{
	if( mVideoFd <= 0) {
		LOGE("device is not open !!!\n");
		return;
	}
	::close(mVideoFd);
	mVideoFd = -1;
}

void V4L2Camera::stopPreview()
{
	term();
	stopStreaming();
	close();
}


void V4L2Camera::term()
{
int ret;

	mQueued = 0;
	mDequeued = 0;

    /* Unmap buffers */
    for (int i = 0; i < NB_BUFFER; i++) {
        if (munmap(mVideoIn->mem[i], buffer_length) < 0) {
			LOGE("Uninit: Unmap failed");
		}
	}
}


int V4L2Camera::startStreaming()
{
	enum v4l2_buf_type type;
    int ret;

    if (!mVideoIn->isStreaming) {
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        ret = ioctl (mVideoFd, VIDIOC_STREAMON, &type);
        if (ret < 0) {
            LOGE("StartStreaming: Unable to start capture: %s", strerror(errno));
            return ret;
        }

        mVideoIn->isStreaming = true;
    }

    return 0;
}

int V4L2Camera::stopStreaming ()
{
    enum v4l2_buf_type type;
    int ret;

    if (mVideoIn->isStreaming) {
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        ret = ioctl (mVideoFd, VIDIOC_STREAMOFF, &type);
        if (ret < 0) {
            LOGE("StopStreaming: Unable to stop capture: %s", strerror(errno));
            return ret;
        }

        mVideoIn->isStreaming = false;
    }

    return 0;
}

int V4L2Camera::getPreviewSize(void)
{
  // TODO : check pixel format
  return (v4l2_preview_width*v4l2_preview_height*12)/8;
}

int V4L2Camera::getPictureraw(char *src, char *dest, int w, int h)
{
	const char *device;
	int retry = 0, ret = -1;

// Jiun 02/06/2013, fix alway using cmos when USB camera width is 640
    if (!(mCameraid&CAM_USB)) {
//  if ((w == 320) || (w == 640)) {
		mTakingPicture = true;
		device = CAMERA_CMOS_NAME;

		while(mVideoFd != -1) {
			LOGW("Camera not stoping");
			usleep(10000);
		}

		while(retry < 0x10) {
			if ((mVideoFd = open(device, O_RDWR)) == -1) {
				LOGE("Picture ERROR opening V4L interface: %s , retry ...", strerror(errno));
				retry++;
				usleep(2000000);
				continue;
			}
			break;  
		}

		if (mVideoFd == -1) {
			LOGE("ERROR opening V4L interface %s: %s", device, strerror(errno));
			return -1;
		}

		ret = v4l2_ioctl_s_fmt(w, h, V4L2_PIX_FMT_YUYV);
		if (ret < 0)
			return ret;

		ret = v4l2_ioctl_reqbufs(1);
		if (ret < 0)
			return ret;

		ret = v4l2_ioctl_querybuf(1);
		if (ret < 0)
			return ret;

		ret = v4l2_ioctl_qbuf(1);
		if (ret < 0)
			return ret;

		startStreaming();

		grabPreviewFrame(dest);
	} else {
		swCropImagetoYUV420(src, dest, 
              hal_preview_width, hal_preview_height, YUV420,
              w, h
              );
  }

  return 0;
}

int V4L2Camera::stopSnap(void)
{
	int ret;
	struct v4l2_buffer buf;

	if (mTakingPicture == false)
		return 0;

	mTakingPicture = false;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(mVideoFd, VIDIOC_DQBUF, &buf);
	if (ret < 0)
		LOGE("Uninit: VIDIOC_DQBUF Failed");
 
 
	mQueued = 0;
	mDequeued = 0;

    /* Unmap buffers */
	if (munmap(mVideoIn->mem[0], buf.length) < 0) {
		LOGE("Uninit: Unmap failed");
		return -1;
	}

	stopStreaming();
	close();
	return 0;
}

int V4L2Camera::grabPreviewFrame(char *recordBuffer)
{
    int ret; 
	struct v4l2_buffer buf;

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    /* DQ */
    ret = ioctl(mVideoFd, VIDIOC_DQBUF, &buf);
    if (ret < 0) {
        LOGE("%s : VIDIOC_DQBUF Failed", __FUNCTION__);
        return -1;
    }
    mDequeued++;
	buffer_length = buf.length;

	if(mCameraid&CAM_USB) {
		if (mRecordBuf == NULL)
			YUY2toYUV420(mVideoIn->mem[buf.index], recordBuffer);
		else
			RecordYUY2toYUV420(mVideoIn->mem[buf.index], recordBuffer);
	} else
		YUV422toYUV420(mVideoIn->mem[buf.index], recordBuffer);

	ret = ioctl(mVideoFd, VIDIOC_QBUF, &buf);
	if (ret < 0) {
		LOGE("%s : VIDIOC_QBUF Failed", __FUNCTION__);
		return -1;
	}

	mQueued++;

    return 0;
}

void V4L2Camera::setRecordBuf(char *addr)
{
  mRecordBuf = addr;
}

void V4L2Camera::YUV422toYUV420(void * cameraIn, char *record_buf)
{
  int NumY, j, CbCr_Total;
  int width  = v4l2_preview_width;
  int height = v4l2_preview_height;
  unsigned int * pCbCr;
  unsigned int tempCb, tempCr;

  if (mTakingPicture == false) {
    if ((hal_preview_width != v4l2_preview_width) || (hal_preview_height != v4l2_preview_height)) {
      swCropImagetoYUV420(  (char *)cameraIn, (char *)record_buf, 
                  v4l2_preview_width, v4l2_preview_height, YUV422,
                  hal_preview_width, hal_preview_height
                 );
      return;
    }
  }

  NumY = width*height;
  height = height >> 1;

  memcpy(record_buf, (char *)cameraIn, NumY);

  unsigned char * d = (unsigned char * )record_buf + NumY;
    unsigned char * s = (unsigned char * )cameraIn + NumY;
    for (j = 0; j < height; j++) {
        memcpy(d, s, width);
        d += width;
        s += width * 2;
    }

  CbCr_Total = width * height >> 2;//(4:2:0 + DWORD)
  if (mRecordBuf != NULL) {
    memcpy(mRecordBuf, record_buf, (NumY*12)/8);
  }

  pCbCr = (unsigned int *)((unsigned char * )record_buf + (width * height*2));

  //Swap Cb and Cr
  for(j = 0; j < CbCr_Total; j++)
  {
    tempCb = tempCr = pCbCr [j];
    pCbCr [j] = (((tempCr << 8) & 0xFF00FF00) |  ((tempCb >> 8) & 0x00FF00FF));
  }
}

void V4L2Camera::swCropImagetoYUV420
(
    char *src, char *dest, 
    int scr_w, int src_h, int src_f,
    int dst_w, int dst_h
)
{
	int i, j, spos, dpos;
    int tempCb, tempCr;
    int length, height, xofs = 0, yofs = 0;
    char *s = src;
    char *d = dest;
    char *cs = (char *)(s+(scr_w*src_h));
    char *cd = (char *)(d+(dst_h*dst_w));
	int *CbCrd , *CbCrm;
	int *CbCrs;

	length = scr_w;
	if (dst_w > scr_w) {
		xofs = (dst_w - scr_w) / 2;
		d += xofs;
		cd += xofs;
		length = scr_w;
	} else if (dst_w < scr_w) {
		xofs = (scr_w - dst_w) / 2;
		s += xofs;
		cs += xofs;
		length = dst_w;
		xofs = 0;
	}

	height = src_h;
	if (dst_h > src_h) {
		yofs = (dst_h - src_h)/2;
		d += (yofs*dst_w);
		cd += ((yofs*dst_w) >> 1);
		height = src_h; 
	} else if (src_h > dst_h) {
		yofs = (src_h - dst_h)/2;
		s += (yofs*scr_w);
		if (src_f == YUV420)
			cs += ((yofs*scr_w) >> 1);
		else if (src_f == YUV422)
			cs += (yofs*scr_w);
		height = dst_h;
		yofs = 0;
	}

	//copy Y
	for (j = 0; j < height; j++) {
		memcpy(d, s, length);
        d += dst_w;
        s += scr_w;
	}

	if (xofs || yofs) {
		CbCrd = (int *)(dest+(dst_w*dst_h));
		j = dst_w * dst_h >> 3;
		for(i = 0; i < j; i++)
			CbCrd[i] = 0x80808080;
	}

	if (mRecordBuf != NULL) {
		memcpy(mRecordBuf, dest, (dst_w*dst_h*12)/8);
		CbCrm = (int *)(mRecordBuf+(dst_h*dst_w)+xofs+((yofs*dst_w) >> 1));
	} else
		CbCrm = NULL;

	//copy CbCr
	height = height/2;
	if (src_f == YUV420) {
		for (j = 0; j < height; j++) {
			memcpy(cd, cs, length);
			cd += dst_w;
			cs += scr_w;
		}
	} else if (src_f == YUV422) {
		length = length >> 2;
		CbCrd = (int *)cd;
		CbCrs = (int *)cs;
		spos = 0;
		dpos = 0;
		xofs = (scr_w>>1);
		for (j = 0; j < height; j++) {
			for (i = 0; i < length; i++) {
				tempCb = tempCr = CbCrs[i+spos];
				if (CbCrm)
					CbCrm[i+dpos] = tempCr;
				CbCrd[i+dpos] = (((tempCr << 8) & 0xFF00FF00) |  ((tempCb >> 8) & 0x00FF00FF));
			}
			spos += xofs;
			dpos += (dst_w>>2);
		}
  	}
}

void V4L2Camera::YUY2toYUV420(void * cameraIn, char *record_buf)
{
    unsigned int *p_y;
    unsigned int *p_c;

    unsigned int NumY, cbcr_height;

    int pos, i, j, k;
    unsigned int ytmp[8];
    unsigned int ctmp[8];
    unsigned int w, h, dx, dy, resx;

    unsigned int yuyv, yuyv1;
    unsigned int *raw = (unsigned int *)cameraIn;
    int width  = v4l2_preview_width;
    int height = v4l2_preview_height;

	NumY = width*height;

    p_y = (unsigned int *)record_buf;
    p_c = (unsigned int *)(record_buf + NumY);

    if (cam_rotate == 0) {
        width = width >>1;
        k = 0;
        dx = 0;
        //resx = WMT_ALIGN(v4l2_preview_width, 16);
        //resx = resx>>1;
        resx = v4l2_preview_width>>1;
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i+=2) {

                //if (cam_mir == 1)
                //    pos = i+((height - 1 - j)*resx);
                //else
                    pos = i+(j*resx);

                yuyv = raw[pos];
                yuyv1 = raw[pos+1];
                ytmp[0] = yuyv&0xFF;
                ytmp[1] = (yuyv&0xFF0000)>>8;
                ytmp[2] = (yuyv1&0xFF)<<16;
                ytmp[3] = (yuyv1&0xFF0000)<<8;
                ytmp[0] = (ytmp[0]|ytmp[1]|ytmp[2]|ytmp[3]);
                p_y[k] = ytmp[0];
                if ((j&0x01) == 0) {
                    /*
                    ctmp[0] = (yuyv&0xFF00)>>8;
                    ctmp[1] = (yuyv&0xFF000000)>>16;
                    ctmp[2] = (yuyv1&0xFF00)<<8;
                    ctmp[3] = (yuyv1&0xFF000000);
                    */
          ctmp[0] = (yuyv&0xFF00);
                    ctmp[1] = (yuyv&0xFF000000)>>24;
          ctmp[2] = (yuyv1&0xFF00)<<16;
                    ctmp[3] = (yuyv1&0xFF000000)>>8;
                    ctmp[0] = (ctmp[0]|ctmp[1]|ctmp[2]|ctmp[3]);
                    p_c[dx] = ctmp[0];
                    dx++;
                }
                k++;
            }
        }
    }
}

void V4L2Camera::RecordYUY2toYUV420(void * cameraIn, char *record_buf)
{
    unsigned int *p_y, *p_y2;
    unsigned int *p_c, *p_c2;

    unsigned int NumY, cbcr_height;

    int pos, i, j, k;
    unsigned int ytmp[8];
    unsigned int ctmp[8];
    unsigned int w, h, dx, dy, resx, resx1;

    unsigned int yuyv, yuyv1;
    unsigned int *raw = (unsigned int *)cameraIn;
    int width  = v4l2_preview_width;
    int height = v4l2_preview_height;

  NumY = width*height;

    p_y = (unsigned int *)record_buf;
    p_c = (unsigned int *)(record_buf + NumY);
    p_y2 = (unsigned int *)mRecordBuf;
    p_c2 = (unsigned int *)(mRecordBuf + NumY);

    if (cam_rotate == 0) {
        width = width >>1;
        k = 0;
        dx = 0;
        //resx = WMT_ALIGN(v4l2_preview_width, 16);
        //resx = resx>>1;
        resx = v4l2_preview_width>>1;
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i+=2) {

                //if (cam_mir == 1)
                //    pos = i+((height - 1 - j)*resx);
                //else
                    pos = i+(j*resx);

                yuyv = raw[pos];
                yuyv1 = raw[pos+1];
                ytmp[0] = yuyv&0xFF;
                ytmp[1] = (yuyv&0xFF0000)>>8;
                ytmp[2] = (yuyv1&0xFF)<<16;
                ytmp[3] = (yuyv1&0xFF0000)<<8;
                ytmp[0] = (ytmp[0]|ytmp[1]|ytmp[2]|ytmp[3]);
                p_y[k] = ytmp[0];
                p_y2[k] = ytmp[0];
                if ((j&0x01) == 0) {
                    ctmp[0] = (yuyv&0xFF00)>>8;
                    ctmp[1] = (yuyv&0xFF000000)>>16;
                    ctmp[2] = (yuyv1&0xFF00)<<8;
                    ctmp[3] = (yuyv1&0xFF000000);
                    ctmp[0] = (ctmp[0]|ctmp[1]|ctmp[2]|ctmp[3]);
                    p_c2[dx] = ctmp[0];
					ctmp[0] = (yuyv&0xFF00);
                    ctmp[1] = (yuyv&0xFF000000)>>24;
					ctmp[2] = (yuyv1&0xFF00)<<16;
                    ctmp[3] = (yuyv1&0xFF000000)>>8;
                    ctmp[0] = (ctmp[0]|ctmp[1]|ctmp[2]|ctmp[3]);
                    p_c[dx] = ctmp[0];
                    dx++;
                }
                k++;
            }
        }
    }
}

int V4L2Camera::EnumerateImageFormats(void)
{
  struct v4l2_fmtdesc fmt;
  int ret = -1;

  if (mVideoFd <= 0) {
    LOGE("V4L2 interface not open");
    return -1;
  }

  memset(&fmt, 0, sizeof(fmt));

  fmt.index = 0;

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  while ((ret = ioctl(mVideoFd, VIDIOC_ENUM_FMT, &fmt)) == 0) {

    fmt.index++;

    LOGW("{ pixelformat = %c%c%c%c , description = %s }\n",
      fmt.pixelformat & 0xFF, (fmt.pixelformat >> 8) & 0xFF,
      (fmt.pixelformat >> 16) & 0xFF, (fmt.pixelformat >> 24) & 0xFF,
      fmt.description);
  }
  return 0;
}


}; // namespace android


