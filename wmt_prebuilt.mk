# /*
# * Copyright 2011 WonderMedia Technologies, Inc. All Rights Reserved. 
# *  
# * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc. 
# * and may contain trade secrets and/or other confidential information of 
# * WonderMedia Technologies, Inc. This file shall not be disclosed to any third party, 
# * in whole or in part, without prior written consent of WonderMedia. 
# *  
# * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS, 
# * WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED, 
# * AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED WARRANTIES 
# * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  
# */
 
 
## A rule can copy some files (any file type) to target folder
## support "mm", "m" and "make".

### Usage ##########################
# LOCAL_SRC_FILES := src1 src2
# LOCAL_MODULE_PATH := target_dir
# For Example:
#    LOCAL_PATH := $(my-dir)
#    include $(CLEAR_VARS)

#    LOCAL_SRC_FILES := $(call wmt-all-files-under, subdir)
#    LOCAL_SRC_FILES += a.txt b.dat c/d.conf

#    LOCAL_MODULE_PATH  := $(PRODUCT_OUT)/system/etc
#    include $(WMT_PREBUILT)


#   or you can set LOCAL_SRC_FILES format like: src1:dest1 src2:dest2
#   For example: copy dir/foo.conf to $(PRODUCT_OUT)/xxx/bar.conf
# 	LOCAL_SRC_FILES := dir/foo.conf:bar.conf
# 	LOCAL_MODULE_PATH  := $(PRODUCT_OUT)/xxx
#    or
#	LOCAL_SRC_FILES := dir/foo.conf:xxx/bar.conf
# 	LOCAL_MODULE_PATH  := $(PRODUCT_OUT)
#

#  You can even copy one file to differnt place, For example
# LOCAL_PATH:= $(call my-dir)
# include $(CLEAR_VARS)
# LOCAL_SRC_FILES := liba.so:system/lib/liba.so
# LOCAL_SRC_FILES += liba.so:obj/lib/liba.so
# include $(WMT_PREBUILT)




##################################################################################
##             Below is implementation                                          ##
##################################################################################

ifeq ($(LOCAL_PATH),)
$(error WMT_PREBUILT: no LOCAL_PATH defined!)
endif

ifeq ($(LOCAL_MODULE_PATH),)
LOCAL_MODULE_PATH := $(PRODUCT_OUT)
endif


ifeq ($(LOCAL_MODULE_TAGS),)
LOCAL_MODULE_TAGS := $(TARGET_BUILD_VARIANT) 
endif

ifndef _wmt_add_prebuilt_file_defined
_wmt_add_prebuilt_file_defined  := true

# $(1): file list, for example "a.txt b.txt" or "a.txt:b.txt"
# $(2): target folder
# $(3): LOCAL_MODULE_TAGS
#
# Elements in the file list may be bare filenames,
# or of the form "<modulename>:<filename>".
#
# The final install path is : $(LOCAL_MODULE_PATH)/$(LOCAL_MODULE_STEM)
# we set LOCAL_MODULE = $(LOCAL_MODULE_PATH)@$(LOCAL_MODULE_STEM) to get a unique name.

define _wmt_add_prebuilt_file
$(if $(filter %: :%,$(1)), \
  $(error $(LOCAL_PATH): Leading or trailing colons in "$(1)")) \
$(foreach t,$(1), \
  $(eval include $(CLEAR_VARS)) \
  $(eval LOCAL_MODULE_TAGS := $(3)) \
  $(eval LOCAL_MODULE_CLASS := wmt_prebuilt) \
  $(eval LOCAL_MODULE_PATH := $(2)) \
  $(eval tw := $(subst :, ,$(strip $(t)))) \
  $(if $(word 3,$(tw)),$(error $(LOCAL_PATH): Bad wmt_prebuilt filename '$(t)')) \
  $(if $(word 2,$(tw)), \
	$(eval LOCAL_SRC_FILES := $(word 1,$(tw))) \
	$(eval LOCAL_MODULE_STEM := $(word 2,$(tw))) \
   , \
	$(eval LOCAL_SRC_FILES := $(t)) \
	$(eval LOCAL_MODULE_STEM := $(notdir $(t))) \
   ) \
  $(eval LOCAL_MODULE    := $(2)@$(LOCAL_MODULE_STEM)) \
  $(eval include $(BUILD_PREBUILT)) \
 )
endef

endif # _wmt_add_prebuilt_file_defined

$(call _wmt_add_prebuilt_file, $(LOCAL_SRC_FILES), $(LOCAL_MODULE_PATH), $(LOCAL_MODULE_TAGS))

