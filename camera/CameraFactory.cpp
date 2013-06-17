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
 * Contains implementation of a class EmulatedCameraFactory that manages cameras
 * available for emulation.
 */

#define LOG_NDEBUG 0
#define LOG_TAG "CameraFactory"
#include <cutils/log.h>
#include "CameraHal.h"
#include <camera/Camera.h>
#include <stdio.h>
#include <string.h>

static int cameraDeviceOpen(const hw_module_t* module, const char* name,
                hw_device_t** device);
static int camera_device_close(hw_device_t* device);
static int camera_get_number_of_cameras(void);
static int camera_get_camera_info(int camera_id, struct camera_info *info);

#define MAX_CAMERAS_SUPPORTED 2
static android::CameraHal* gCameraHals[MAX_CAMERAS_SUPPORTED];

/*
 * Required HAL header.
 */
struct hw_module_methods_t CameraModuleMethods = {
    open: cameraDeviceOpen
};


camera_module_t HAL_MODULE_INFO_SYM = {
    common: {
         tag:           HARDWARE_MODULE_TAG,
         version_major: 1,
         version_minor: 0,
         id:            CAMERA_HARDWARE_MODULE_ID,
         name:          "Emulated Camera Module",
         author:        "The Android Open Source Project",
         methods:       &CameraModuleMethods,
         dso:           NULL,
         reserved:      {0},
    },
    get_number_of_cameras:  camera_get_number_of_cameras,
    get_camera_info:        camera_get_camera_info,
};

typedef struct camera_id_device {
    camera_device_t base;
    int cameraid;
} camera_id_device;

/*******************************************************************
 * implementation of camera_device_ops functions
 *******************************************************************/

int camera_set_preview_window(struct camera_device * device,
        struct preview_stream_ops *window)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->setPreviewWindow(window);
}

void camera_set_callbacks(struct camera_device * device,
        camera_notify_callback notify_cb,
        camera_data_callback data_cb,
        camera_data_timestamp_callback data_cb_timestamp,
        camera_request_memory get_memory,
        void *user)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;

    gCameraHals[dev_id->cameraid]->setCallbacks(notify_cb, data_cb, data_cb_timestamp, get_memory, user);
}

void camera_enable_msg_type(struct camera_device * device, int32_t msg_type)
{

    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;

    gCameraHals[dev_id->cameraid]->enableMsgType(msg_type);
}

void camera_disable_msg_type(struct camera_device * device, int32_t msg_type)
{

    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;

    gCameraHals[dev_id->cameraid]->disableMsgType(msg_type);
}

int camera_msg_type_enabled(struct camera_device * device, int32_t msg_type)
{

    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->isMsgTypeEnabled(msg_type);
}

int camera_start_preview(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->startPreview();
}

void camera_stop_preview(struct camera_device * device)
{

    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;

    gCameraHals[dev_id->cameraid]->stopPreview();
}

int camera_preview_enabled(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->isPreviewEnabled();
}

int camera_store_meta_data_in_buffers(struct camera_device * device, int enable)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->storeMetaDataInBuffers(enable);
}

int camera_start_recording(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->startRecording();
}

void camera_stop_recording(struct camera_device * device)
{

    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;


    gCameraHals[dev_id->cameraid]->stopRecording();
}

int camera_recording_enabled(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->isRecordingEnabled();
}

void camera_release_recording_frame(struct camera_device * device,
                const void *opaque)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;

    gCameraHals[dev_id->cameraid]->releaseRecordingFrame(opaque);
}

int camera_auto_focus(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

   	return gCameraHals[dev_id->cameraid]->setAutoFocus();
}

int camera_cancel_auto_focus(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return  gCameraHals[dev_id->cameraid]->cancelAutoFocus();
}

int camera_take_picture(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->takePicture();
}

int camera_cancel_picture(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->cancelPicture();
}

int camera_set_parameters(struct camera_device * device, const char *params)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->setParameters(params);
}

char* camera_get_parameters(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return NULL;

    dev_id = (camera_id_device*) device;

     return gCameraHals[dev_id->cameraid]->getParameters();

}

static void camera_put_parameters(struct camera_device *device, char *parms)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;
    gCameraHals[dev_id->cameraid]->putParameters(parms);
}

int camera_send_command(struct camera_device * device,
            int32_t cmd, int32_t arg1, int32_t arg2)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->sendCommand(cmd, arg1, arg2);
}

void camera_release(struct camera_device * device)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return;

    dev_id = (camera_id_device*) device;

    gCameraHals[dev_id->cameraid]->releaseCamera();
}

int camera_dump(struct camera_device * device, int fd)
{
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if(!device)
        return  -EINVAL;

    dev_id = (camera_id_device*) device;

    return gCameraHals[dev_id->cameraid]->dumpCamera(fd);
}

int camera_device_close(hw_device_t* device)
{
    int ret = 0;
    camera_id_device* dev_id = NULL;

    //LOGV("%s", __FUNCTION__);

    if (!device) {
        return -EINVAL;
    }

    dev_id = (camera_id_device*) device;

    if (dev_id) {
        if (gCameraHals[dev_id->cameraid]) {
            delete gCameraHals[dev_id->cameraid];
            gCameraHals[dev_id->cameraid] = NULL;
        }

        if (dev_id->base.ops) {
            free(dev_id->base.ops);
        }
        free(dev_id);
    }
    return ret;
}

static android::CameraInfo sCameraInfo[] = {
    {
        CAMERA_FACING_BACK,
        0,  /* orientation */
    }
};

extern "C" {
	int wmt_getsyspara(const char *varname, char *varval, int * varlen);
}

#define ENV_NAME_CAMERA "wmt.camera.param"
#define MAX_BUFFER_LENGTH 24
char *envbuf = NULL;
int gCamType = -1;

int get_uboot_camera_param(const char *name)
{
	int len = MAX_BUFFER_LENGTH;
	int ret = -1;

	if (envbuf == NULL)
		envbuf = (char *)malloc(MAX_BUFFER_LENGTH);
	memset(envbuf, 0, MAX_BUFFER_LENGTH);

	return wmt_getsyspara(name, envbuf, &len);
}


int wmt_camera_id_init(void)
{
	int cap = 0;
	int tmp1, tmp2;

	if (gCamType >= 0)
		return 0;
	
	if (get_uboot_camera_param(ENV_NAME_CAMERA)) {
		LOGE("Get %s form u-boot failed , using default Camera type = CMOS",ENV_NAME_CAMERA);
		gCamType = 0;
		return -1;
	}
	sscanf(envbuf,"%d:%d:%d:%d",&gCamType, &tmp1, &tmp2,&cap);
	gCamType |= (cap <<16);
	return 0;
}


int cameraDeviceOpen(const hw_module_t* module,
			           const char* name,
					   hw_device_t** device)
{
    int rv = 0;
    int cameraid;
    camera_id_device* camera_device = NULL;
    camera_device_ops_t* camera_ops = NULL;
    android::CameraHal* camera = NULL;	

    if (name != NULL) {
        cameraid = atoi(name);
        if(cameraid > MAX_CAMERAS_SUPPORTED)
        {
            LOGE("camera service provided cameraid out of bounds, "
                    "cameraid = %d, num supported = %d",
                    cameraid, MAX_CAMERAS_SUPPORTED);
            rv = -EINVAL;
            goto fail;
        }


        camera_device = (camera_id_device*)malloc(sizeof(*camera_device));
        if(!camera_device)
        {
            LOGE("camera_device allocation fail");
            rv = -ENOMEM;
            goto fail;
        }

        camera_ops = (camera_device_ops_t*)malloc(sizeof(*camera_ops));
        if(!camera_ops)
        {
            LOGE("camera_ops allocation fail");
            rv = -ENOMEM;
            goto fail;
        }

        memset(camera_device, 0, sizeof(*camera_device));
        memset(camera_ops, 0, sizeof(*camera_ops));

		camera_device->base.common.tag = HARDWARE_DEVICE_TAG;
        camera_device->base.common.version = 0;
        camera_device->base.common.module = (hw_module_t *)(module);
        camera_device->base.common.close = camera_device_close;
        camera_device->base.ops = camera_ops;

/*
        camera_device->common.tag = HARDWARE_DEVICE_TAG;
        camera_device->common.version = 0;
        camera_device->common.module = (hw_module_t *)(module);
        camera_device->common.close = camera_device_close;
        camera_device->ops = camera_ops;
*/
        camera_ops->set_preview_window = camera_set_preview_window;
        camera_ops->set_callbacks = camera_set_callbacks;
        camera_ops->enable_msg_type = camera_enable_msg_type;
        camera_ops->disable_msg_type = camera_disable_msg_type;
        camera_ops->msg_type_enabled = camera_msg_type_enabled;
        camera_ops->start_preview = camera_start_preview;
        camera_ops->stop_preview = camera_stop_preview;
        camera_ops->preview_enabled = camera_preview_enabled;
        camera_ops->store_meta_data_in_buffers = camera_store_meta_data_in_buffers;
        camera_ops->start_recording = camera_start_recording;
        camera_ops->stop_recording = camera_stop_recording;
        camera_ops->recording_enabled = camera_recording_enabled;
        camera_ops->release_recording_frame = camera_release_recording_frame;
        camera_ops->auto_focus = camera_auto_focus;
        camera_ops->cancel_auto_focus = camera_cancel_auto_focus;
        camera_ops->take_picture = camera_take_picture;
        camera_ops->cancel_picture = camera_cancel_picture;
        camera_ops->set_parameters = camera_set_parameters;
        camera_ops->get_parameters = camera_get_parameters;
        camera_ops->put_parameters = camera_put_parameters;
        camera_ops->send_command = camera_send_command;
        camera_ops->release = camera_release;
        camera_ops->dump = camera_dump;

        //*device = &camera_device->common;
        *device = &camera_device->base.common;
		camera_device->cameraid = cameraid;

        camera = new android::CameraHal(gCamType);

        gCameraHals[cameraid] = camera;
    }

    return rv;

fail:
    if(camera_device) {
        free(camera_device);
        camera_device = NULL;
    }
    if(camera_ops) {
        free(camera_ops);
        camera_ops = NULL;
    }
    if(camera) {
        delete camera;
        camera = NULL;
    }
    *device = NULL;
    return rv;
}


int camera_get_number_of_cameras(void)
{
	wmt_camera_id_init();
	return 1;
}

int camera_get_camera_info(int camera_id, struct camera_info *info)
{    
    LOGW("%s: id = %d", __FUNCTION__, camera_id);

    //return gCameraHals[camera_id]->getCameraInfo(info);
    //info->facing = CAMERA_FACING_FRONT;
    info->facing = CAMERA_FACING_BACK;
    
    //GigaHsu 2012.4.24 : Force 7 inches Panel(800*480) Camera to "Tablet Mode".
    //info->orientation = 0;
    info->orientation = 90;
    
	return 0;

}



