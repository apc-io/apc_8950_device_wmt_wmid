# Copyright (C) 2008 The Android Open Source Project
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

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PRELINK_MODULE := false

LOCAL_C_INCLUDES += device/wmt/wmid/ \

LOCAL_SRC_FILES := \
	canon/exif-mnote-data-canon.c \
	canon/mnote-canon-entry.c \
	canon/mnote-canon-tag.c \
	fuji/exif-mnote-data-fuji.c \
	fuji/mnote-fuji-entry.c \
	fuji/mnote-fuji-tag.c \
	olympus/exif-mnote-data-olympus.c \
	olympus/mnote-olympus-entry.c \
	olympus/mnote-olympus-tag.c \
	pentax/exif-mnote-data-pentax.c \
	pentax/mnote-pentax-entry.c \
	pentax/mnote-pentax-tag.c \
	exif-byte-order.c \
	exif-content.c \
	exif-data.c \
	exif-entry.c \
	exif-format.c \
	exif-ifd.c \
	exif-loader.c \
	exif-log.c \
	exif-mem.c \
	exif-mnote-data.c \
	exif-tag.c \
	exif-utils.c

LOCAL_MODULE_TAGS := eng
LOCAL_MODULE := libswexif
include $(BUILD_SHARED_LIBRARY)

