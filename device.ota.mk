################################################################################
#                                                                              #
# Copyright c 2009-2011  WonderMedia Technologies, Inc.   All Rights Reserved. #
#                                                                              #
# This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.  #
# and may contain trade secrets and/or other confidential information of       #
# WonderMedia Technologies, Inc. This file shall not be disclosed to any third #
# party, in whole or in part, without prior written consent of WonderMedia.    #
#                                                                              #
# THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,  #
# WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED, #
# AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED          #
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET       #
# ENJOYMENT OR NON-INFRINGEMENT.                                               #
#                                                                              #
################################################################################

# un-tar files for OTA to build boot.img
$(shell rm -rf out/target/product/wmid/root)
$(shell mkdir -p out/target/product/wmid/root)

# host tools
$(shell mkdir -p out/host/linux-x86/framework)

WMT_PACKAGES_BUSYBOX := $(shell find device/wmt/wmid/wmt_packages -name "busybox*.tgz")
$(shell tar zxf $(WMT_PACKAGES_BUSYBOX) -C out/target/product/wmid/root)
WMT_PACKAGES_MALI400 := $(shell find device/wmt/wmid/wmt_packages -name "mali400*.tgz")
$(shell tar zxf $(WMT_PACKAGES_MALI400) -C out/target/product/wmid/root)
WMT_PACKAGES_NTFS3G := $(shell find device/wmt/wmid/wmt_packages -name "ntfs*.tgz")
$(shell tar zxf $(WMT_PACKAGES_NTFS3G) -C out/target/product/wmid/root)
WMT_PACKAGES_KERNEL_MODULES := $(shell find device/wmt/wmid/wmt_packages -name "kernel*modules*.tgz")
$(shell tar zxf $(WMT_PACKAGES_KERNEL_MODULES) -C out/target/product/wmid/root)
WMT_PACKAGES_WIFI_MODULES := $(shell find device/wmt/wmid/wmt_packages -name "wifi*modules*.tgz")
$(shell tar zxf $(WMT_PACKAGES_WIFI_MODULES) -C out/target/product/wmid/root)

# dumpkey.jar
$(shell cp device/wmt/wmid/host/dumpkey.jar out/host/linux-x86/framework)

