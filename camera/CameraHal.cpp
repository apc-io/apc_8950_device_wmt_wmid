/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Contains implementation of a class EmulatedCamera that encapsulates
 * functionality common to all emulated cameras ("fake", "webcam", "video file",
 * etc.). Instances of this class (for each emulated camera) are created during
 * the construction of the EmulatedCameraFactory instance. This class serves as
 * an entry point for all camera API calls that defined by camera_device_ops_t
 * API.
 */

#define LOG_NDEBUG 0
#define LOG_TAG "CameraHal"
#include <cutils/log.h>
#include "CameraHal.h"


namespace android {

CameraHal::CameraHal(int id)
{
	mHardware = new CameraHardware(id);
}

CameraHal::~CameraHal()
{
}

/****************************************************************************
 * Public API
 ***************************************************************************/


void CameraHal::onCameraDeviceError(int err)
{
    /* Errors are reported through the callback notifier */
    mHardware->onCameraDeviceError(err);
}

/****************************************************************************
 * Camera API implementation.
 ***************************************************************************/

status_t CameraHal::getCameraInfo(struct camera_info* info)
{
    //LOGV("%s", __FUNCTION__);

    return mHardware->getCameraInfo(info);
}

status_t CameraHal::setPreviewWindow(struct preview_stream_ops* window)
{
    /* Callback should return a negative errno. */
    return -mHardware->setPreviewWindow(window);
}

void CameraHal::setCallbacks(camera_notify_callback notify_cb,
                                  camera_data_callback data_cb,
                                  camera_data_timestamp_callback data_cb_timestamp,
                                  camera_request_memory get_memory,
                                  void* user)
{
    mHardware->setCallbacks(notify_cb, data_cb, data_cb_timestamp,
                                    get_memory, user);
}

void CameraHal::enableMsgType(int32_t msg_type)
{
    mHardware->enableMessage(msg_type);
}

void CameraHal::disableMsgType(int32_t msg_type)
{
    mHardware->disableMessage(msg_type);
}

int CameraHal::isMsgTypeEnabled(int32_t msg_type)
{
    return mHardware->isMessageEnabled(msg_type);
}

status_t CameraHal::startPreview()
{
    /* Callback should return a negative errno. */
    return -mHardware->startPreview();
}

void CameraHal::stopPreview()
{
    mHardware->stopPreview();
}

int CameraHal::isPreviewEnabled()
{
    return mHardware->isPreviewEnabled();
}

status_t CameraHal::storeMetaDataInBuffers(int enable)
{
    /* Callback should return a negative errno. */
    return -mHardware->storeMetaDataInBuffers(enable);
}

status_t CameraHal::startRecording()
{
    /* Callback should return a negative errno. */
    return -mHardware->enableVideoRecording();
}

void CameraHal::stopRecording()
{
    mHardware->disableVideoRecording();
}

int CameraHal::isRecordingEnabled()
{
    return mHardware->isVideoRecordingEnabled();
}

void CameraHal::releaseRecordingFrame(const void* opaque)
{
    /* TODO: Future enhancements. */
}

status_t CameraHal::setAutoFocus()
{
    //LOGV("%s", __FUNCTION__);

    return mHardware->autoFocus();
}

status_t CameraHal::cancelAutoFocus()
{
    //LOGV("%s", __FUNCTION__);

    return mHardware->cancelAutoFocus();
}

status_t CameraHal::takePicture()
{
	//LOGV("%s", __FUNCTION__);

  	return mHardware->takePicture();
}

status_t CameraHal::cancelPicture()
{
    //LOGV("%s", __FUNCTION__);

    return mHardware->cancelPicture();
}

status_t CameraHal::setParameters(const char* parms)
{
    //LOGV("%s", __FUNCTION__);

	return mHardware->setParameters(parms);
}

char* CameraHal::getParameters()
{
    //LOGV("%s", __FUNCTION__);

    return mHardware->getParameters();
}

void CameraHal::putParameters(char* params)
{
	mHardware->putParameters(params);
}

status_t CameraHal::sendCommand(int32_t cmd, int32_t arg1, int32_t arg2)
{
    //LOGV("%s: cmd = %d, arg1 = %d, arg2 = %d", __FUNCTION__, cmd, arg1, arg2);
    return mHardware->sendCommand(cmd, arg1, arg2);  
}

void CameraHal::releaseCamera()
{
    //LOGV("%s", __FUNCTION__);
	mHardware->onreleaseCamera();
}

status_t CameraHal::dumpCamera(int fd)
{
    //LOGV("%s", __FUNCTION__);

    /* TODO: Future enhancements. */
    return -EINVAL;
}

}; /* namespace android */
