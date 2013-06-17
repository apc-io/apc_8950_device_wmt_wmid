LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= wmt_mrp.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    device/wmt/wmid/wmt_api/release/include \
    frameworks/base/include \

LOCAL_SHARED_LIBRARIES:= \
    libcutils \
    libutils \
	libui \
	libsurfaceflinger_client \
    libwmtapi \
    libgui

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libwmtmrp
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)