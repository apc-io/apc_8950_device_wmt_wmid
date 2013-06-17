LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../av.mk

LOCAL_SRC_FILES := $(FFFILES)

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) 			\
	$(LOCAL_PATH)/.. 		\
	$(LOCAL_PATH)/../libavutil 	\
	external/zlib 			\
	device/wmt/wmid/wmt_api/release/include 	\
    
LOCAL_CFLAGS += $(FFCFLAGS)
LOCAL_ASFLAGS += $(FFASFLAGS)
LOCAL_SHARED_LIBRARIES += $(FFLIBS) libwmtapi libavutil \
                          libwmtcodec libwmtchk1 libwmtchk2

LOCAL_MODULE := $(FFNAME)
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false  

LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)
