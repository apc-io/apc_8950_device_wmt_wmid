#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/wmt/wmid/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES := \
        $(LOCAL_KERNEL):kernel \
        device/wmt/wmid/init.wmt.rc:root/init.wmt.rc \
        device/wmt/wmid/init.rc:root/init.rc 

# support external storage
PRODUCT_COPY_FILES += \
	device/wmt/wmid/vold.fstab:system/etc/vold.fstab

# Camera
PRODUCT_COPY_FILES += \
	device/wmt/wmid/camera/media_profiles.xml:system/etc/media_profiles.xml

# Custom ueventd to config correct permission for /dev/video*
PRODUCT_COPY_FILES += \
       device/wmt/wmid/ueventd.wmt.rc:root/ueventd.wmt.rc

PRODUCT_CHARACTERISTICS := tablet

DEVICE_PACKAGE_OVERLAYS := \
    device/wmt/wmid/overlay

$(call inherit-product, frameworks/base/build/tablet-dalvik-heap.mk)

# OTA solution
$(call inherit-product-if-exists, device/wmt/wmid/device.ota.mk)

