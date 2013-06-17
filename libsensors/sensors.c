/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <hardware/sensors.h>
#include <cutils/log.h>

#include "nusensors.h"

/*****************************************************************************/

/*
 * The SENSORS Module
 */

extern int wmt_getsyspara(char *varname, unsigned char *varval, int * varlen);

// AddVendor{
static const struct sensor_t dmt08SensorList[] = {
    {
        "WMT 3-axis Accelerometer",
        "WMT Developer",
        1,
        SENSORS_HANDLE_BASE+ID_A,
        SENSOR_TYPE_ACCELEROMETER,
        DMT08_ACCEL_MAX_RANGE,
        DMT08_ACCEL_RESOLUTION,
        DMT08_ACCEL_POWER,
        DMT08_ACCEL_MIN_DELAY,
        {0}
    },
};

static const struct sensor_t kxti9SensorList[] = {
    {
        "WMT 3-axis Accelerometer",
        "WMT Developer",
        1,
        SENSORS_HANDLE_BASE+ID_A,
        SENSOR_TYPE_ACCELEROMETER,
        KXTI9_ACCEL_MAX_RANGE,
        KXTI9_ACCEL_RESOLUTION,
        KXTI9_ACCEL_POWER,
        KXTI9_ACCEL_MIN_DELAY,
        {0}
    },
};
//}

static int open_sensors(const struct hw_module_t* module, const char* name,
        struct hw_device_t** device);

static int sensors__get_sensors_list(struct sensors_module_t* module,
        struct sensor_t const** list)
{
    char buf[64];
    int len = sizeof(buf);
    int sensor_dev = 0;
    int arr_size;

    memset((void*)buf, 0, len);

    if (wmt_getsyspara("wmt.io.gsensor", (unsigned char *)buf, &len))
        LOGE("Can not get wmt.io.gsensor  %s",strerror(errno));
    else
        sscanf(buf,"%d",&sensor_dev);

    // AddVendor{
    if (sensor_dev == 2) {
        *list = kxti9SensorList;
        arr_size = ARRAY_SIZE(kxti9SensorList);
    } else {
        *list = dmt08SensorList;
        arr_size = ARRAY_SIZE(dmt08SensorList);
    }
    //}

    return arr_size;
}

static struct hw_module_methods_t sensors_module_methods = {
    .open = open_sensors
};

const struct sensors_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .version_major = 1,
        .version_minor = 0,
        .id = SENSORS_HARDWARE_MODULE_ID,
        .name = "WMT SENSORS Module",
        .author = "WMT Developer",
        .methods = &sensors_module_methods,
    },
    .get_sensors_list = sensors__get_sensors_list
};

/*****************************************************************************/

static int open_sensors(const struct hw_module_t* module, const char* name,
        struct hw_device_t** device)
{
    return init_nusensors(module, device);
}
