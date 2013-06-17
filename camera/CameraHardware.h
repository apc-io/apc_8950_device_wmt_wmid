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

#ifndef ANDROID_HARDWARE_CAMERA_HARDWARE_H
#define ANDROID_HARDWARE_CAMERA_HARDWARE_H

#include <camera/CameraParameters.h>
#include <hardware/camera.h>
#include <utils/threads.h>
#include "V4L2Camera.h"
#include <ui/Rect.h>
#include <ui/GraphicBufferMapper.h>
#include "JpegCompressor.h"
#include <utils/RefBase.h>
#include "swexif.h"

#define CAMERA_EXIF_ENABLE 0x10000
#define CAMERA_THUMB_ENABLE 0x20000
#define CAMERA_ZOOM_ENABLE 0x40000

namespace android  {

class CameraHardware {
public:
    CameraHardware(int id);

    virtual  ~CameraHardware();

	status_t getCameraInfo(struct camera_info* info);

	/****************************************************************************
     * Camera Parameter
	***************************************************************************/
	status_t setParameters(const char* parms);
	status_t setParameters(const CameraParameters& params);
		
	char* getParameters();

	void putParameters(char* params);

	void initDefaultParameters();

	/****************************************************************************
	* Preview
	***************************************************************************/
	status_t setPreviewWindow(struct preview_stream_ops* window);
	status_t startPreview();
	void stopPreview();
	status_t startPreviewHW();
	int getPictureSize(const CameraParameters& params, int *width, int *height);
	void doZoom(int izoomValue);

	/* Checks if preview is enabled. */
    inline bool isPreviewEnabled()
    {
        return mPreviewEnabled;
    }

	/****************************************************************************
	* Video Recording  
	***************************************************************************/

	inline bool isVideoRecordingEnabled()
    {
        return mVideoRecEnabled;
    }

	status_t enableVideoRecording();	
    void disableVideoRecording();    
	status_t storeMetaDataInBuffers(bool enable);
    
	/****************************************************************************
	* takePicture
	***************************************************************************/
	status_t takePicture();
	status_t cancelPicture();

	/****************************************************************************
	* Callback  
	***************************************************************************/
	void setCallbacks(camera_notify_callback notify_cb,
                      camera_data_callback data_cb,
                      camera_data_timestamp_callback data_cb_timestamp,
                      camera_request_memory get_memory,
                      void* user);

	void FrameToWindow(void);
	void FrameCallbackNotifier(nsecs_t timestamp);
	
	/****************************************************************************
	* Message Control 
	***************************************************************************/
    void enableMessage(uint msg_type);
    void disableMessage(uint msg_type);

	inline int isMessageEnabled(uint msg_type)
    {
        return mMessageEnabler & msg_type;
    }

	/****************************************************************************
	* Camera Error
	***************************************************************************/
    void onCameraDeviceError(int err);

	/****************************************************************************
	* autoFocus
	***************************************************************************/
	status_t    autoFocus();
    status_t    cancelAutoFocus();

	/****************************************************************************
	* others
	***************************************************************************/	
	void onreleaseCamera(void);
	status_t sendCommand(unsigned int cmd, unsigned int arg1, unsigned int arg2);

	swExif mswExif;

private:
	/****************************************************************************
	* Preview thread
	***************************************************************************/     
    class PreviewThread : public Thread {
        CameraHardware* mHardware;
    public:
		PreviewThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CameraPreviewThread", PRIORITY_URGENT_DISPLAY);
            //run("CameraPreviewThread", ANDROID_PRIORITY_URGENT_DISPLAY, 0);
        }
        virtual bool threadLoop() {
            mHardware->inPreviewThread();
            // loop until we need to quit
            return false;
        }
    };
	// protected by mLock
	sp<PreviewThread>   mPreviewThread;    
	status_t inPreviewThread(); 

	/****************************************************************************
	* Picture thread
	***************************************************************************/ 

    class PictureThread : public Thread {
        CameraHardware *mHardware;
    public:
        PictureThread(CameraHardware *hw):
        Thread(false),
        mHardware(hw) { }
        virtual bool threadLoop() {
            mHardware->inPictureThread();
            return false;
        }
    };
	sp<PictureThread>   mPictureThread;
	status_t inPictureThread(); 

    static int beginAutoFocusThread(void *cookie);
    int autoFocusThread();

	/****************************************************************************
	* Data members 
	***************************************************************************/

	/* Locks this instance for data changes. */    
	mutable Mutex			mPreviewLock;
	mutable Mutex			mVideoRecLock;
	mutable Mutex			mCaptureLock;

    /* Locks this instance for data change. */
    Mutex 					mObjectLock;
	mutable Mutex			mParamLock;

    /* Camera parameters. */
    CameraParameters		mParameters;

   	char * mCurrentFrame;
   	camera_memory_t* 		mPreviewMemory;
	camera_memory_t* 		mRecordMemory;

	/* Preview window instance. */
    preview_stream_ops*			mPreviewWindow;

	/* V4L2 Camera Hardware */
	V4L2Camera mCamera;


	char *mRecordBuf;
	mutable Condition   mPreviewCondition;
    mutable Condition   mPreviewStoppedCondition;
    mutable Condition 	mCaptureCondition;

    int mCameraid;

	/****************************************************************************
	* Preview parameter 
	***************************************************************************/
	int				mPreviewFrameWidth;
    int				mPreviewFrameHeight;
    int 			mPreviewFrameSize;
	bool			mPreviewEnabled;
	bool 		mThreadWait;
	bool		mExitPreviewThread;
	bool 		mPreviewStop;
	bool 		mPreviewStarting;
	int 		mStartPreviewStatus;
	buffer_handle_t** mPreviewBufferHandle;
	bool 	mZoomSupport;
	int 	mCurrentZoomIndex;
	int 	mTargetZoomIndex;

	/****************************************************************************
	* Recording parameter 
	***************************************************************************/	
	bool		mVideoRecEnabled;

	/****************************************************************************
	* Picture
	***************************************************************************/
	int mJpegQuality;
	bool mTakingPicture;

	/****************************************************************************
	* Callback parameter 
	***************************************************************************/
	camera_notify_callback          mNotifyCB;
    camera_data_callback            mDataCB;
    camera_data_timestamp_callback  mDataCBTimestamp;
    camera_request_memory           mGetMemoryCB;
    void*                           mCBOpaque;

	/****************************************************************************
	* Message Control 
	***************************************************************************/	
    unsigned int                   mMessageEnabler;
    bool mHasExif;
    bool mHasthumbnail;
};

}; // namespace android

#endif
