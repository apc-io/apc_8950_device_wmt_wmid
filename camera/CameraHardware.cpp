/*
**
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

#define LOG_TAG "CameraHardware"
#include <utils/Log.h>

#include "CameraHardware.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define CAMHAL_GRALLOC_USAGE GRALLOC_USAGE_HW_TEXTURE | \
                             GRALLOC_USAGE_HW_RENDER | \
                             GRALLOC_USAGE_SW_READ_RARELY | \
                             GRALLOC_USAGE_SW_WRITE_NEVER

extern "C" {
	int wmt_getsyspara(const char *varname, char *varval, int * varlen);
}

namespace android {



#define ENV_NAME_CAMERA "wmt.camera.fps"
#define MAX_BUFFER_LENGTH 24

#define CAMERA_WINDOW_SET 0x02
#define CAMERA_HARDWARE_EANBLE 0x01
#define CAMERA_WINDOW_CHANGE 0x04
#define CAMERA_PREVIEW_OK (CAMERA_WINDOW_SET|CAMERA_HARDWARE_EANBLE)


#define MAX_ZOOM_INDEX 6
int g_ZOOM_RATIOS[MAX_ZOOM_INDEX+1] = {100,150,200,250,300,350,400};
CameraHardware::CameraHardware(int id)
			: mParameters(),			  
			  mCurrentFrame(NULL),			  
			  mCamera(),
			  mPreviewEnabled(false),
			  mVideoRecEnabled(false),
			  mMessageEnabler(0)
{

	//LOGW("%s", __FUNCTION__);
	mTakingPicture = false;
	mRecordBuf = NULL;
	mPreviewWindow = NULL;
	mExitPreviewThread = false;
	mPreviewThread = new PreviewThread(this);
	mPictureThread = new PictureThread(this);
	mCameraid = id;
	mThreadWait = true;
	mStartPreviewStatus = 0;
	mPreviewStop = false;
	mPreviewMemory = NULL;
	mRecordMemory = NULL;
	mPreviewStarting = false;
	mCurrentZoomIndex = 0;
	mTargetZoomIndex = 0;
	mHasExif = false;
	mHasthumbnail = false;
	if (mCameraid&CAMERA_EXIF_ENABLE) {
		mHasExif = true;
		if (mCameraid&CAMERA_THUMB_ENABLE)
			mHasthumbnail = true;
	}
	mPreviewBufferHandle = new buffer_handle_t*[NB_BUFFER];
	mZoomSupport = false;
	if (mCameraid&CAMERA_ZOOM_ENABLE)
		mZoomSupport = true;
	initDefaultParameters();
}

CameraHardware::~CameraHardware()
{
	//LOGW("%s", __FUNCTION__);
}

void CameraHardware::initDefaultParameters()
{
	//LOGW("%s", __FUNCTION__);

	CameraParameters p;
	String8 parameterString;
	int i = 0;
	char str[16];

	// preview
    p.setPreviewSize(640, 480);
	p.setPreviewFormat("yuv420sp");// CameraTest.java say that default format should be yuv420sp
	p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES, "176x144,320x240,640x480");
	p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS, "yuv420sp,yuv420p");

	//frame-rate
	p.setPreviewFrameRate(15);
	p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATES, "5,15,30");
	p.set(CameraParameters::KEY_PREVIEW_FPS_RANGE, "8000,30000");
	p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FPS_RANGE, "(8000,30000)");


	//video
	//p.set(CameraParameters::KEY_VIDEO_SIZE, "640x480");
	//p.set(CameraParameters::KEY_SUPPORTED_VIDEO_SIZES, "176x144,320x240,640x480");
	//p.set(CameraParameters::KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO, "640x480");	
	p.set(CameraParameters::KEY_VIDEO_FRAME_FORMAT, CameraParameters::PIXEL_FORMAT_YUV420SP);
	
	//jpeg
    p.setPictureFormat("jpeg");
    p.setPictureSize(640, 480);// picture resolution should be the same with preview
    p.set(CameraParameters::KEY_JPEG_QUALITY, "100");
    p.set(CameraParameters::KEY_SUPPORTED_PICTURE_SIZES, "320x240,640x480");
	p.set(CameraParameters::KEY_SUPPORTED_PICTURE_FORMATS, "jpeg");

	//thumbnail
	p.set(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH, "176");
	p.set(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, "144");
	p.set(CameraParameters::KEY_JPEG_THUMBNAIL_QUALITY, "90");
	p.set(CameraParameters::KEY_SUPPORTED_JPEG_THUMBNAIL_SIZES, "176x144,0x0");

	/*
	p.set(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH, "320");
	p.set(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, "240");
	p.set(CameraParameters::KEY_JPEG_THUMBNAIL_QUALITY, "90");
	p.set(CameraParameters::KEY_SUPPORTED_JPEG_THUMBNAIL_SIZES, "320x240,0x0");
	*/
	//focus
	p.set(CameraParameters::KEY_FOCAL_LENGTH, "2.78");
	p.set(CameraParameters::KEY_FOCUS_MODE, "fixed");
	p.set(CameraParameters::KEY_SUPPORTED_FOCUS_MODES, "off,fixed");
	p.set(CameraParameters::KEY_FOCUS_DISTANCES,"0.10,1.20,Infinity");

	//exposure
	p.set(CameraParameters::KEY_MAX_EXPOSURE_COMPENSATION,"6");
	p.set(CameraParameters::KEY_MIN_EXPOSURE_COMPENSATION,"0");
	p.set(CameraParameters::KEY_EXPOSURE_COMPENSATION_STEP,"0.3333333333");
	/*
	p.set(CameraParameters::KEY_AUTO_EXPOSURE_LOCK,"true");
	p.set(CameraParameters::KEY_AUTO_EXPOSURE_LOCK_SUPPORTED,"true");
	p.set(CameraParameters::KEY_MAX_EXPOSURE_COMPENSATION,"6");
	p.set(CameraParameters::KEY_MIN_EXPOSURE_COMPENSATION,"0");
	p.set(CameraParameters::KEY_EXPOSURE_COMPENSATION_STEP,"0.3333333333");
	*/
	// zoom
	if (mZoomSupport == false) {
		p.set(CameraParameters::KEY_ZOOM_SUPPORTED, "false");
	} else {
		if ((sizeof(g_ZOOM_RATIOS)>>2) != (MAX_ZOOM_INDEX+1)) {
			LOGE("KEY_ZOOM_RATIOS error : %d != %d",(sizeof(g_ZOOM_RATIOS)>>2),(MAX_ZOOM_INDEX+1));
			p.set(CameraParameters::KEY_ZOOM_SUPPORTED, "false");
		} else {
			p.set(CameraParameters::KEY_ZOOM, "0");
			sprintf(str, "%d", MAX_ZOOM_INDEX);
			p.set(CameraParameters::KEY_MAX_ZOOM, str);
			sprintf(str, "%d", g_ZOOM_RATIOS[0]);
			parameterString = str;
			for (i = 1; i <= MAX_ZOOM_INDEX; i++) {
				parameterString.append(",");
				sprintf(str, "%d", g_ZOOM_RATIOS[i]);
				parameterString.append(str);
			}
			p.set(CameraParameters::KEY_ZOOM_RATIOS, parameterString.string());
			p.set(CameraParameters::KEY_ZOOM_SUPPORTED, "true");
			p.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");
		}
	}

	//EFFECT
/*
	p.set(CameraParameters::KEY_EFFECT, CameraParameters::EFFECT_NONE);
	parameterString = CameraParameters::EFFECT_NEGATIVE;
    parameterString.append(",");
    parameterString.append(CameraParameters::EFFECT_MONO);
    parameterString.append(",");
    parameterString.append(CameraParameters::EFFECT_NEGATIVE);
    parameterString.append(",");
    parameterString.append(CameraParameters::EFFECT_SEPIA);
    p.set(CameraParameters::KEY_SUPPORTED_EFFECTS, parameterString.string());
*/
	//WHITE_BALANCE	
	p.set(CameraParameters::KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED, "false");
/*
	p.set(CameraParameters::KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED, "true");
	p.set(CameraParameters::KEY_WHITE_BALANCE, CameraParameters::WHITE_BALANCE_AUTO);
    parameterString = CameraParameters::WHITE_BALANCE_AUTO;
    parameterString.append(",");
    parameterString.append(CameraParameters::WHITE_BALANCE_INCANDESCENT);
    parameterString.append(",");
    parameterString.append(CameraParameters::WHITE_BALANCE_FLUORESCENT);
    parameterString.append(",");
    parameterString.append(CameraParameters::WHITE_BALANCE_DAYLIGHT);
    parameterString.append(",");
    parameterString.append(CameraParameters::WHITE_BALANCE_CLOUDY_DAYLIGHT);
    p.set(CameraParameters::KEY_SUPPORTED_WHITE_BALANCE,
          parameterString.string());
*/
	//SCENE_MODE
/*
	parameterString = CameraParameters::SCENE_MODE_AUTO;
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_PORTRAIT);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_LANDSCAPE);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_NIGHT);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_BEACH);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_SNOW);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_SUNSET);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_FIREWORKS);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_SPORTS);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_PARTY);
	parameterString.append(",");
	parameterString.append(CameraParameters::SCENE_MODE_CANDLELIGHT);
	p.set(CameraParameters::KEY_SUPPORTED_SCENE_MODES,
              parameterString.string());
	p.set(CameraParameters::KEY_SCENE_MODE, CameraParameters::SCENE_MODE_AUTO);
*/
	//others
	p.set(CameraParameters::KEY_HORIZONTAL_VIEW_ANGLE, "90");
	p.set(CameraParameters::KEY_VERTICAL_VIEW_ANGLE, "90");

   	p.set(CameraParameters::KEY_MAX_NUM_DETECTED_FACES_HW, "0");
	p.set(CameraParameters::KEY_MAX_NUM_DETECTED_FACES_SW, "0");
	//
    if (setParameters(p) != NO_ERROR) {
        LOGE("Failed to set default parameters?!");
    }
}


status_t CameraHardware::setParameters(const CameraParameters& params)
{
	const char *string;
	int val[2];
	int width = 0,height = 0;
{
    Mutex::Autolock lock(mParamLock);

    //LOGW("Camera parameters: %s", params.flatten().string());

	if ((strcmp(params.getPreviewFormat(), "yuv420sp") != 0) && (strcmp(params.getPreviewFormat(), "yuv420p") != 0)) {
        LOGE("preview not supported %s", params.getPreviewFormat());
        return BAD_VALUE;
    }

	//need to modify 
	string = params.get(CameraParameters::KEY_FOCUS_MODE);
	if (string != NULL) {
		if ((strcmp(string, "fixed") != 0) && (strcmp(string, "off") != 0)) {
			LOGE("set KEY_FOCUS_MODE error");
			return BAD_VALUE;
		}
	}
    
    if (strcmp(params.getPictureFormat(), "jpeg") != 0) {
        LOGE("Only jpeg still pictures are supported");
        return BAD_VALUE;
    }

	string = params.get(CameraParameters::KEY_PREVIEW_FPS_RANGE);
	if (string != NULL) {
		params.getPreviewFpsRange(&val[0], &val[1]);
		if ((val[0] < 0) || (val[1] < 0) || (val[0] > val[1]))
			return BAD_VALUE;
	}

	params.getPreviewSize(&width, &height);
	if ((width <= 0) || (height <= 0))
		return BAD_VALUE;


	if (mZoomSupport == true)
		mTargetZoomIndex = atoi(params.get(CameraParameters::KEY_ZOOM));

    mParameters = params;
	return NO_ERROR;
}
}


status_t CameraHardware::setParameters(const char* parms)
{
    //LOGW("%s , string = %s", __FUNCTION__,parms);

    CameraParameters new_param;
    String8 str8_param(parms);
    new_param.unflatten(str8_param);
    //mParameters = new_param;

	return setParameters(new_param);
    //return NO_ERROR;
}

/* A dumb variable indicating "no params" / error on the exit from
 * CameraHal::getParameters(). */
static char lNoParam = '\0';
char* CameraHardware::getParameters()
{
    //LOGW("%s", __FUNCTION__);
    String8 params(mParameters.flatten());
    char* ret_str =
        reinterpret_cast<char*>(malloc(sizeof(char) * (params.length()+1)));
    memset(ret_str, 0, params.length()+1);
    if (ret_str != NULL) {
        strncpy(ret_str, params.string(), params.length()+1);
        return ret_str;
    } else {
        LOGE("%s: Unable to allocate string for %s", __FUNCTION__, params.string());
        /* Apparently, we can't return NULL fron this routine. */
        return &lNoParam;
    }
}

void CameraHardware::putParameters(char* params)
{
    //LOGW("%s", __FUNCTION__);
    /* This method simply frees parameters allocated in getParameters(). */
    if (params != NULL && params != &lNoParam) {
        free(params);
    }
}

status_t CameraHardware::getCameraInfo(struct camera_info* info)
{
	//LOGW("%s", __FUNCTION__);
	const char* valstr = NULL;	

    valstr = mParameters.get("prop-facing");
    if (valstr != NULL) {
        if (strcmp(valstr, "front") == 0) {
            info->facing = CAMERA_FACING_FRONT;
        }
        else if (strcmp(valstr, "back") == 0) {
            info->facing = CAMERA_FACING_BACK;
        }
    } else {
        info->facing = CAMERA_FACING_BACK;
    }

    valstr = mParameters.get("prop-orientation");
    if (valstr != NULL) {
        info->orientation = atoi(valstr);
    } else {
        info->orientation = 0;
    }

	return NO_ERROR;
}

status_t CameraHardware::startPreview()
{
	//LOGW("%s", __FUNCTION__);
	status_t ret;

   	mPreviewLock.lock();
    if (mPreviewEnabled == true) {
        //already running
        LOGW("startPreview:  but already running");
        mPreviewLock.unlock();
        return INVALID_OPERATION;
    }

	mPreviewEnabled = true;
	ret = NO_ERROR;
	if ((mStartPreviewStatus&CAMERA_WINDOW_SET) == 0) {
		ret = startPreviewHW();
		if (ret == NO_ERROR)
			mThreadWait = true;
		else
			LOGE("\nstartPreviewHW failed");
	} else {
		mPreviewCondition.signal();
        mPreviewStarting = true;
	}
	mPreviewLock.unlock();
    return ret;
}

status_t CameraHardware::startPreviewHW()
{
	int video_w = 0, video_h = 0;

	mStartPreviewStatus &= ~CAMERA_HARDWARE_EANBLE;

	mCamera.setCameraid(mCameraid);

  	mParameters.getPreviewSize(&mPreviewFrameWidth, &mPreviewFrameHeight);

	if (mCamera.Open(NULL, mPreviewFrameWidth, mPreviewFrameHeight, V4L2_PIX_FMT_YVU420) < 0) {
        LOGE("Camera Open failed: cannot open device %dx%d.", mPreviewFrameWidth, mPreviewFrameHeight);
        return UNKNOWN_ERROR;
    }


	mPreviewFrameSize = (mPreviewFrameHeight * mPreviewFrameWidth *12) / 8; //yuv
	//mPreviewFrameSize = (mPreviewFrameHeight * mPreviewFrameWidth * 2); //rgb565
	//mPreviewFrameSize = (mPreviewFrameHeight * mPreviewFrameWidth * 4); //rgb8888

	//LOGW("alloc mPreviewMemory size = %d",mPreviewFrameSize);
	if (mPreviewMemory) {
		mPreviewMemory->release(mPreviewMemory);
        mPreviewMemory = NULL;
	}
	mPreviewMemory = mGetMemoryCB(-1, mPreviewFrameSize, 1, NULL);
	if (NULL == mPreviewMemory || NULL == mPreviewMemory->data) {
		LOGE("%s: Memory failure when alloc mPreviewMemory", __FUNCTION__);
		return UNKNOWN_ERROR;
	}

	if (mRecordMemory) {
		mRecordMemory->release(mRecordMemory);
        mRecordMemory = NULL;
	}

	mRecordMemory = mGetMemoryCB(-1, mPreviewFrameSize, 1, NULL);
	if (NULL == mRecordMemory || NULL == mRecordMemory->data) {
		LOGE("%s: Memory failure when alloc mRecordMemory", __FUNCTION__);
		return UNKNOWN_ERROR;
	}


	if (mCamera.startStreaming()) {
		LOGE("startPreview failed: device StartStreaming failed");
		return UNKNOWN_ERROR;
	}

	mStartPreviewStatus |= CAMERA_HARDWARE_EANBLE;
	return NO_ERROR;
}


int CameraHardware::inPreviewThread()
{
	//LOGW("%s", __FUNCTION__);
	nsecs_t timeout = 8000000000LL;
	while (1) {
	mPreviewLock.lock();
	while (mThreadWait == true) {
		if (mPreviewStop == true) {
        	LOGW("%s: calling stopPreview() and waiting", __func__);
        	if(mCamera.isOpened()) {
	 			mCamera.stopPreview();
	 		}
			/* signal that we're stopping */
			mPreviewStoppedCondition.signal();
	 	}
		//mPreviewCondition.wait(mPreviewLock);
		LOGW("\nstart waiting ..........");
		if (mPreviewStarting == false) {
		if (NO_ERROR != mPreviewCondition.waitRelative(mPreviewLock, timeout)) {
			LOGW("Timeout for wait mPreviewLock , but still runing");
			}
		}
		mPreviewStarting = false;
	    mThreadWait = false;
		LOGW("%s: return from wait", __func__);
	}

	mPreviewLock.unlock();

	if (mExitPreviewThread) {
		LOGW("%s: exiting", __func__);
			if(mCamera.isOpened())
				mCamera.stopPreview();
		return NO_ERROR;
	}

	if ((mPreviewWindow != NULL) && ((mStartPreviewStatus&CAMERA_PREVIEW_OK) == CAMERA_PREVIEW_OK)) {

		nsecs_t timeStamp = systemTime(SYSTEM_TIME_MONOTONIC);

		FrameToWindow();
		FrameCallbackNotifier(timeStamp);
	}
	}
	return NO_ERROR;
}

status_t CameraHardware::setPreviewWindow(struct preview_stream_ops* window)                                         
{
	LOGW("%s: current: %p -> new: %p", __FUNCTION__, mPreviewWindow, window);

	mPreviewWindow = window;

	if (window == NULL) {
		mStartPreviewStatus &= ~CAMERA_WINDOW_SET;
		return NO_ERROR;
 	}

   	mPreviewLock.lock();
    status_t res = NO_ERROR;

	mParameters.getPreviewSize(&mPreviewFrameWidth, &mPreviewFrameHeight);

	res = mPreviewWindow->set_buffer_count(mPreviewWindow, NB_BUFFER);
	if (res != NO_ERROR) {
		LOGE("%s: Error in set_buffer_count %d -> %s",
		__FUNCTION__, -res, strerror(-res));
		return INVALID_OPERATION;
	}

	res = mPreviewWindow->set_usage(mPreviewWindow, CAMHAL_GRALLOC_USAGE);
	if (res != NO_ERROR) {
		window = NULL;
		res = -res; // set_usage returns a negative errno.
		LOGE("%s: Error setting preview window usage %d -> %s",
			__FUNCTION__, res, strerror(res));
	}

	res = mPreviewWindow->set_buffers_geometry(	mPreviewWindow,
										mPreviewFrameWidth,
										mPreviewFrameHeight,
										//HAL_PIXEL_FORMAT_RGB_565);
										//HAL_PIXEL_FORMAT_RGBA_8888);
										HAL_PIXEL_FORMAT_YCrCb_420_SP);
										//HAL_PIXEL_FORMAT_YV12);
	if (res != NO_ERROR) {
			LOGE("%s: Error in set_buffers_geometry %d -> %s",
			__FUNCTION__, -res, strerror(-res));
		return INVALID_OPERATION;
	}


	int tmp = 0;
	mPreviewWindow->get_min_undequeued_buffer_count(mPreviewWindow, &tmp);
	if (tmp >= NB_BUFFER) {
		LOGE("undequeued_buffer = %d , NB_BUFFER = %d",tmp,NB_BUFFER);
	}

    int stride = 0, i = 0;
    buffer_handle_t* buffer = NULL;
    for (i = 0; i < NB_BUFFER; i++) {
    	res = mPreviewWindow->dequeue_buffer(mPreviewWindow, &buffer, &stride);
    	mPreviewBufferHandle[i] = buffer;
    	if (res != NO_ERROR || mPreviewBufferHandle[i] == NULL) {
        	LOGE("%s: Unable to dequeue preview window buffer: %d -> %s",
            	__FUNCTION__, -res, strerror(-res));
        	return res;
    	}
	}

	void* img = NULL;
    const Rect rect(mPreviewFrameWidth, mPreviewFrameHeight);
    GraphicBufferMapper& grbuffer_mapper(GraphicBufferMapper::get());
    int *vaddr = NULL;
    int j = 0, ofs = 0;
	for (i = 0; i < (NB_BUFFER-tmp); i++) {
    	res = mPreviewWindow->lock_buffer(mPreviewWindow, mPreviewBufferHandle[i]);
    	if (res != NO_ERROR) {
        	LOGE("%s: Unable to lock preview window buffer: %d -> %s",
            	 __FUNCTION__, -res, strerror(-res));
        	return res;
    	}

    	res = grbuffer_mapper.lock(*mPreviewBufferHandle[i], CAMHAL_GRALLOC_USAGE, rect, &img);
    	if (res != NO_ERROR) {
        	LOGE("%s: grbuffer_mapper.lock failure: %d -> %s",
            	 __FUNCTION__, res, strerror(res));
        	return res;
    	}
    	vaddr = (int *)((char *)img+(mPreviewFrameWidth*mPreviewFrameHeight));
    	ofs = (mPreviewFrameWidth*mPreviewFrameHeight)>>3;
    	for (j = 0; j < ofs; j++)
    		vaddr[j] = 0x80808080;
	}

	for (i = 0; i < (NB_BUFFER-tmp); i++) {
		mPreviewWindow->enqueue_buffer(mPreviewWindow, mPreviewBufferHandle[i]);
	}

	for (i = (NB_BUFFER-tmp); i < NB_BUFFER; i++) {
		mPreviewWindow->cancel_buffer(mPreviewWindow, mPreviewBufferHandle[i]);
	}

	if ((mStartPreviewStatus&CAMERA_WINDOW_SET) == 0) {
		mStartPreviewStatus |= CAMERA_WINDOW_SET;
		if ((mStartPreviewStatus&CAMERA_HARDWARE_EANBLE) == 0) {
			res = startPreviewHW();
			mThreadWait = true;
		} else {
			mPreviewCondition.signal();
			mPreviewStarting = true;
		}
	}

	mPreviewLock.unlock();
    return res;
}

void CameraHardware::doZoom(int izoomValue)
{
	int zoomValue=0;
	float temp, left, top, right, bottom;

	mCurrentZoomIndex = mTargetZoomIndex;
	zoomValue = g_ZOOM_RATIOS[mTargetZoomIndex];
	temp=(float)zoomValue/100.0;
	left=mPreviewFrameWidth*(temp-1)/temp/2.0;
	top=mPreviewFrameHeight*(temp-1)/temp/2.0;
	right=mPreviewFrameWidth-left;
	bottom=mPreviewFrameHeight-top;
	//LOGE("zoomvalue %d, letf %f, top %f, right %f, bottom %f", zoomValue,
	//	left, top, right, bottom);
	
	mPreviewWindow->set_crop(mPreviewWindow, (int)left, (int)top, (int)right, 
		(int)bottom);
}
void CameraHardware::FrameToWindow(void)
{
 	////LOGW("%s", __FUNCTION__);   
    int res;

    /*
     * Push new frame to the preview window.
     */
    /* Dequeue preview window buffer for the frame. */    
    buffer_handle_t* buffer = NULL;
    int stride = 0;
    res = mPreviewWindow->dequeue_buffer(mPreviewWindow, &buffer, &stride);
    if (res != NO_ERROR || buffer == NULL) {
        LOGE("%s: Unable to dequeue preview window buffer: %d -> %s",
            __FUNCTION__, -res, strerror(-res));
        return;
    }

    /* Let the preview window to lock the buffer. */
    res = mPreviewWindow->lock_buffer(mPreviewWindow, buffer);
    if (res != NO_ERROR) {
        LOGE("%s: Unable to lock preview window buffer: %d -> %s",
             __FUNCTION__, -res, strerror(-res));
        mPreviewWindow->cancel_buffer(mPreviewWindow, buffer);
        return;
    }

    /* Now let the graphics framework to lock the buffer, and provide
     * us with the framebuffer data address. */
    void* img = NULL;
    const Rect rect(mPreviewFrameWidth, mPreviewFrameHeight);
    GraphicBufferMapper& grbuffer_mapper(GraphicBufferMapper::get());
    res = grbuffer_mapper.lock(*buffer, CAMHAL_GRALLOC_USAGE, rect, &img);
    if (res != NO_ERROR) {
        LOGE("%s: grbuffer_mapper.lock failure: %d -> %s",
             __FUNCTION__, res, strerror(res));
        mPreviewWindow->cancel_buffer(mPreviewWindow, buffer);
        return;
    }
    /* Frames come in in YV12/NV12/NV21 format. Since preview window doesn't
     * supports those formats, we need to obtain the frame in RGB565. */

	mCamera.grabPreviewFrame((char *)mPreviewMemory->data);
	mCurrentFrame = (char *)mPreviewMemory->data;
	memcpy((char *)img, mCurrentFrame, mPreviewFrameSize);

	if ((mCurrentZoomIndex != mTargetZoomIndex) && (mZoomSupport == true)) {
		LOGW("Zoom Current index = %d",mTargetZoomIndex);
		doZoom(0);	
	}
	/* Show it. */
	mPreviewWindow->enqueue_buffer(mPreviewWindow, buffer);
    grbuffer_mapper.unlock(*buffer);
}


void CameraHardware::FrameCallbackNotifier(nsecs_t timestamp)
{

	if (isMessageEnabled(CAMERA_MSG_VIDEO_FRAME) && isVideoRecordingEnabled()) {
		mDataCBTimestamp(timestamp, CAMERA_MSG_VIDEO_FRAME,
						mRecordMemory, 0, mCBOpaque);
    }

    if (isMessageEnabled(CAMERA_MSG_PREVIEW_FRAME)) {
		mDataCB(CAMERA_MSG_PREVIEW_FRAME, mPreviewMemory, 0, NULL, mCBOpaque);
    }   
}

int CameraHardware::getPictureSize(const CameraParameters& params, int *width, int *height)
{
	int pictruew = 0, pictrueh = 0;

	//TODO : check pixel format
	params.getPictureSize(&pictruew, &pictrueh);
	
	*width = pictruew;
	*height = pictrueh;

	return (pictruew*pictrueh*4);

}


status_t CameraHardware::inPictureThread()
{
	status_t res = -1;
	NV21JpegCompressor compressor;
	NV21JpegCompressor thumbcomp;
	camera_memory_t* jpeg_buff = NULL;
	int picsize = 0;
	char patch[2];
	int exif_len, thumbsize = 0;
	char *jpegbuffer = NULL;
	int pictruew = 0, pictrueh = 0;
	int previewsize = 0 , picturesize = 0;
	camera_memory_t* picraw = NULL;
	camera_memory_t* thumbraw = NULL;
	CameraParameters params;

	if (mTakingPicture) {
        /* The sequence of callbacks during picture taking is:
         *  - CAMERA_MSG_SHUTTER
         *  - CAMERA_MSG_RAW_IMAGE_NOTIFY
         *  - CAMERA_MSG_COMPRESSED_IMAGE
         */
        if (isMessageEnabled(CAMERA_MSG_SHUTTER)) {
            mNotifyCB(CAMERA_MSG_SHUTTER, 0, 0, mCBOpaque);
        }
        if (isMessageEnabled(CAMERA_MSG_RAW_IMAGE_NOTIFY)) {
            mNotifyCB(CAMERA_MSG_RAW_IMAGE_NOTIFY, 0, 0, mCBOpaque);
        }
        if (isMessageEnabled(CAMERA_MSG_COMPRESSED_IMAGE)) {
{
			Mutex::Autolock lock(mParamLock);
        	params = mParameters;
}
						
			picturesize = getPictureSize(params, &pictruew, &pictrueh);
			previewsize = mCamera.getPreviewSize();
			if (!picturesize || !previewsize) {
				LOGE("%s : Preview size = %d , Picture = %d",__FUNCTION__,previewsize,picturesize);
				goto out;
			}
	
			picraw = mGetMemoryCB(-1, picturesize, 1, NULL);
			mCamera.getPictureraw(mCurrentFrame, (char *)picraw->data, pictruew, pictrueh);
			mCamera.stopSnap();
            /* Compress the frame to JPEG. Note that when taking pictures, we
             * have requested camera device to provide us with NV21 frames. */
			res = compressor.compressRawImage((char *)picraw->data, pictruew,
            	                              pictrueh,
                	                          mJpegQuality);

			picsize = compressor.getCompressedSize();
			if ((res != NO_ERROR) || (picsize == 0)) {
				 LOGE("%s: Compression failure in CAMERA_MSG_COMPRESSED_IMAGE", __FUNCTION__);
				goto out;
			}
			if (mHasExif == false)
				goto no_exif;
			// exif
			pictruew = params.getInt(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH);
			pictrueh = params.getInt(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT);
			if ((pictruew <= 0) || (pictrueh <= 0) || (mHasthumbnail == false)) {
				mHasthumbnail = false;
				mswExif.setThumbnail(false);
				res = NO_ERROR;
			}
			if (mHasthumbnail == true) {
				thumbraw = mGetMemoryCB(-1, (pictruew*pictrueh*12)/8, 1, NULL);
				mCamera.getPictureraw(mCurrentFrame, (char *)thumbraw->data, pictruew, pictrueh);
				res = thumbcomp.compressRawImage((char *)thumbraw->data, 
											params.getInt(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH),
                                            params.getInt(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT),
                                            mJpegQuality);

				if (res == NO_ERROR) {
					thumbsize = thumbcomp.getCompressedSize();
					if (thumbsize) {
						mswExif.setThumbnailsize(thumbsize);
					} else {
						res = -1;
						LOGE("Thumbnail failed : size = 0");
						goto out;
					}
				} else {
					LOGE("Create Thumbnail failed");
					goto out;
				}
			}

			if (res == NO_ERROR) {
				res = -1;
				mswExif.setParameters(params);
				if (!mswExif.createswexif()) {
					if (!mswExif.getExiflength(&exif_len)) {
						//LOGW("exif_len = 0x%x !!!!!!!!!!!!!",exif_len);
						jpeg_buff =
                   			mGetMemoryCB(-1, (picsize+thumbsize+exif_len-2), 1, NULL);
						if (NULL != jpeg_buff && NULL != jpeg_buff->data) {
							//LOGW("JPEG buffer = %p",jpeg_buff->data);
							jpegbuffer = (char *)jpeg_buff->data;
							mswExif.copyExif(jpegbuffer);
							if (thumbsize != 0) {
								thumbcomp.getCompressedImage((char *)(jpegbuffer+exif_len));
								//LOGW("put thumbnail  to = %p !!!!!!!!!!!!!",(char *)(jpegbuffer+exif_len));
								exif_len += thumbsize;
							}
							res = NO_ERROR;
						} else
							LOGE("%s: Memory failure in CAMERA_MSG_COMPRESSED_IMAGE", __FUNCTION__);
					} else
						LOGE("Exif Length == 0");
				} else 
					LOGE("Create Exif failed");
				if (res != NO_ERROR)
					goto out;
			}
			//
no_exif:
			if (mHasExif == false) {
				//LOGW("no exif .....");
				jpeg_buff =
                    mGetMemoryCB(-1, compressor.getCompressedSize(), 1, NULL);
                if (NULL != jpeg_buff && NULL != jpeg_buff->data) {
                    compressor.getCompressedImage(jpeg_buff->data);
                    mDataCB(CAMERA_MSG_COMPRESSED_IMAGE, jpeg_buff, 0, NULL, mCBOpaque);
                } else {
                    LOGE("%s: Memory failure in CAMERA_MSG_COMPRESSED_IMAGE", __FUNCTION__);
                    goto out;
                }
			} else {
					patch[0] = *(char *)(jpegbuffer+exif_len-1);
					patch[1] = *(char *)(jpegbuffer+exif_len-2);
					compressor.getCompressedImage((char *)jpegbuffer+exif_len-2);
					*(char *)(jpegbuffer+exif_len-1) = patch[0];
					*(char *)(jpegbuffer+exif_len-2) = patch[1];
					//LOGW("put JPEG  to = %p !!!!!!!!!!!!!",(char *)(jpegbuffer+exif_len));
                    mDataCB(CAMERA_MSG_COMPRESSED_IMAGE, jpeg_buff, 0, NULL, mCBOpaque);
			}				
			//LOGW("Taking Picture finish");
        }
    }
    res = NO_ERROR;
out:
	if (picraw != NULL) {
		picraw->release(picraw);
    	picraw = NULL;
	}

    if (thumbraw != NULL) {
    	thumbraw->release(thumbraw);
    	thumbraw = NULL;
 	}
	if (jpeg_buff != NULL) {
		jpeg_buff->release(jpeg_buff);
		jpeg_buff = NULL;
	}
    /* This happens just once. */
	mCaptureLock.lock();
    mTakingPicture = false;
    mCaptureCondition.signal();
	mCaptureLock.unlock();

	return res;
}

status_t CameraHardware::cancelPicture()
{
    if (mPictureThread.get()) {
        mPictureThread->requestExitAndWait();
    }

    return NO_ERROR;
}

status_t CameraHardware::takePicture()
{
    status_t res;
    int width, height;

	//LOGW("starting takePicture");
	
    stopPreview();

    /* Get JPEG quality. */
    int jpeg_quality = mParameters.getInt(CameraParameters::KEY_JPEG_QUALITY);
    if (jpeg_quality <= 0) {
        jpeg_quality = 90;  /* Fall back to default. */
    }

	if (mTakingPicture == true) {
    	nsecs_t timeout = 3000000000LL;
		//LOGW("re-entry takePicture , waiting for picture thread to complete.");
        if (NO_ERROR != mCaptureCondition.waitRelative(mCaptureLock, timeout)) {
        	LOGE("Wait taking picture timeout !!!!!!!!!!!!!!!!!!");
        	return TIMED_OUT;
        }
	}

    /* Deliver one frame only. */
   	mJpegQuality = jpeg_quality;

    if (mPictureThread->run("CameraPictureThread", PRIORITY_DEFAULT) != NO_ERROR) {
        LOGE("%s : couldn't run picture thread", __func__);
        return INVALID_OPERATION;
    }

	mCaptureLock.lock();
	mTakingPicture = true;
	mCaptureLock.unlock();

  	return NO_ERROR;
}

void CameraHardware::stopPreview()
{
   	nsecs_t timeout = 3000000000LL;

	if ((mStartPreviewStatus&CAMERA_PREVIEW_OK) != CAMERA_PREVIEW_OK) {
		//LOGW("stopPreview already runing , mStartPreviewStatus = %x",mStartPreviewStatus);
		return;
	}

    mPreviewLock.lock();
	if (mPreviewEnabled == true) {
		mPreviewStop = true;
		mStartPreviewStatus = 0;
		mThreadWait = true;
		mPreviewCondition.signal();
		/* wait until preview thread is stopped */
		//mPreviewStoppedCondition.wait(mPreviewLock);
        if (NO_ERROR != mPreviewStoppedCondition.waitRelative(mPreviewLock, timeout)) {
        	LOGE("\n Wait stopPreview Timeout!!!!!!!!!!!!!!");
        }
   		mPreviewEnabled = false;
        mPreviewStop = false;
	}
	mPreviewLock.unlock();
}

status_t CameraHardware::enableVideoRecording()
{
    //LOGW("%s", __FUNCTION__);
    Mutex::Autolock lock(mVideoRecLock);
    mVideoRecEnabled = true;
    if (mRecordMemory) {
    	mCamera.setRecordBuf((char *)mRecordMemory->data);
    }
    return NO_ERROR;
}

void CameraHardware::disableVideoRecording()
{
    //LOGW("%s", __FUNCTION__);
    Mutex::Autolock lock(mVideoRecLock);
    mVideoRecEnabled = false;
    mCamera.setRecordBuf(NULL);
}

/****************************************************************************
* Callback  
***************************************************************************/


status_t CameraHardware::storeMetaDataInBuffers(bool enable)
{
    //LOGW("%s : %d", __FUNCTION__,enable);
    /* Return INVALID_OPERATION means HAL does not support metadata. So HAL will
     * return actual frame data with CAMERA_MSG_VIDEO_FRRAME. Return
     * INVALID_OPERATION to mean metadata is not supported. */
    return INVALID_OPERATION;
}

void CameraHardware::enableMessage(uint msg_type)
{
    //LOGW("%s: msg_type = 0x%x", __FUNCTION__, msg_type);

    Mutex::Autolock locker(&mObjectLock);
    mMessageEnabler |= msg_type;
    LOGV("**** Currently enabled messages:");
}

void CameraHardware::disableMessage(uint msg_type)
{
    //LOGW("%s: msg_type = 0x%x", __FUNCTION__, msg_type);

    Mutex::Autolock locker(&mObjectLock);
    mMessageEnabler &= ~msg_type;
}

void CameraHardware::onCameraDeviceError(int err)
{
    //LOGW("%s", __FUNCTION__);
    if (isMessageEnabled(CAMERA_MSG_ERROR) && mNotifyCB != NULL) {
        mNotifyCB(CAMERA_MSG_ERROR, err, 0, mCBOpaque);
    }
}

void CameraHardware::setCallbacks(camera_notify_callback notify_cb,
                                    camera_data_callback data_cb,
                                    camera_data_timestamp_callback data_cb_timestamp,
                                    camera_request_memory get_memory,
                                    void* user)
{
    //LOGW("%s: %p, %p, %p, %p (%p)",
    //     __FUNCTION__, notify_cb, data_cb, data_cb_timestamp, get_memory, user);

    Mutex::Autolock locker(&mObjectLock);
    mNotifyCB = notify_cb;
    mDataCB = data_cb;
    mDataCBTimestamp = data_cb_timestamp;
    mGetMemoryCB = get_memory;
    mCBOpaque = user;
}


// ---------------------------------------------------------------------------

int CameraHardware::beginAutoFocusThread(void *cookie)
{
    CameraHardware *c = (CameraHardware *)cookie;
    return c->autoFocusThread();
}

int CameraHardware::autoFocusThread()
{
    if (isMessageEnabled(CAMERA_MSG_FOCUS))
        mNotifyCB(CAMERA_MSG_FOCUS, true, 0, mCBOpaque);
    return NO_ERROR;
}


status_t CameraHardware::autoFocus()
{
   Mutex::Autolock locker(&mObjectLock);
    if (createThread(beginAutoFocusThread, this) == false)
        return UNKNOWN_ERROR;
	return NO_ERROR;
}

status_t CameraHardware::cancelAutoFocus()
{
    //[Vic] Nothing to do as we just create an autoFocus thread and report done.
    return NO_ERROR;
}

// ---------------------------------------------------------------------------

status_t CameraHardware::sendCommand(unsigned int cmd, unsigned int arg1, unsigned int arg2)
{
	return BAD_VALUE;	
}

void CameraHardware::onreleaseCamera()
{
	if (mPreviewThread != NULL) {
        /* this thread is normally already in it's threadLoop but blocked
         * on the condition variable or running.  signal it so it wakes
         * up and can exit.
         */
        //mMessageEnabler = 0;
        //mPreviewThread->requestExit();
        mExitPreviewThread = true;
        mThreadWait = true;
        mPreviewCondition.signal();
        mPreviewThread->requestExitAndWait();
        mPreviewThread.clear();
    }

    if (mPictureThread != NULL) {
        mPictureThread->requestExitAndWait();
        mPictureThread.clear();
    }

	mCamera.close();

    if (mPreviewMemory != NULL) {
    	mPreviewMemory->release(mPreviewMemory);
    	mPreviewMemory = NULL;
    }

	if (mRecordMemory != NULL) {
    	mRecordMemory->release(mRecordMemory);
    	mRecordMemory = NULL;
    }

    if( mPreviewBufferHandle != NULL)
    {
        delete [] mPreviewBufferHandle;
        mPreviewBufferHandle = NULL;
    }

    mCurrentFrame = NULL;
	mRecordBuf = NULL;
}

}; // namespace android