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

PRODUCT_COPY_FILES += \
	device/wmt/wmid/wmt_api/libwmtapi.so:system/lib/libwmtapi.so \
	device/wmt/wmid/wmt_api/libwmtapi.so:obj/lib/libwmtapi.so \
	\
	device/wmt/wmid/wmt_codec/libwmtcodec.so:system/lib/libwmtcodec.so \
	device/wmt/wmid/wmt_codec/libwmtcodec.so:obj/lib/libwmtcodec.so \
	\
	device/wmt/wmid/wmt_thumb/libwmtthumb.so:system/lib/libwmtthumb.so \
	device/wmt/wmid/wmt_thumb/libwmtthumb.so:obj/lib/libwmtthumb.so \
	\
	device/wmt/wmid/wmtegl/libwmtegl.so:system/lib/libwmtegl.so \
	device/wmt/wmid/wmtegl/libwmtegl.so:obj/lib/libwmtegl.so \
	\
	device/wmt/wmid/wmtpe/libwmtpe.so:system/lib/libwmtpe.so \
	device/wmt/wmid/wmtpe/libwmtpe.so:obj/lib/libwmtpe.so \
	\
	device/wmt/wmid/wplayer/engine/libwplayer.so:system/lib/libwplayer.so \
	device/wmt/wmid/wplayer/engine/libwplayer.so:obj/lib/libwplayer.so \
	\
	device/wmt/wmid/wplayer/executable/wplayer:system/bin/wplayer \
	\
	hardware/libhardware/modules/ARM_gralloc/gralloc.default.so:system/lib/hw/gralloc.default.so \
	hardware/libhardware/modules/ARM_gralloc/gralloc.default.so:obj/lib/gralloc.default.so \
	\
	hardware/libhardware/modules/gralloc/gralloc.android.so:system/lib/hw/gralloc.android.so \
	hardware/libhardware/modules/gralloc/gralloc.android.so:obj/lib/gralloc.android.so \
	\
	hardware/wmt/libstagefrighthw/libstagefrighthw.so:system/lib/libstagefrighthw.so \
	hardware/wmt/libstagefrighthw/libstagefrighthw.so:obj/lib/libstagefrighthw.so \
	\
	hardware/wmt/vdec/libOMX.WMT.Video.Decoder.so:system/lib/libOMX.WMT.Video.Decoder.so \
	hardware/wmt/vdec/libOMX.WMT.Video.Decoder.so:obj/lib/libOMX.WMT.Video.Decoder.so \
	\
	hardware/wmt/wmt-enc/libOMX.WMT.Video.Encoder.so:system/lib/libOMX.WMT.Video.Encoder.so \
	hardware/wmt/wmt-enc/libOMX.WMT.Video.Encoder.so:obj/lib/libOMX.WMT.Video.Encoder.so \
	\
	device/wmt/wmid/avcap/libwmtavcap.so:system/lib/libwmtavcap.so \
	device/wmt/wmid/avcap/libwmtavcap.so:obj/lib/libwmtavcap.so \
	\
	device/wmt/wmid/env/lib/libwmtenv.so:system/lib/libwmtenv.so \
	device/wmt/wmid/env/lib/libwmtenv.so:obj/lib/libwmtenv.so \
	\
	device/wmt/wmid/env/env/wmtenv:symbols/system/bin/wmtenv \
	\
	device/wmt/wmid/getubootenv/native/libgetubootenv.so:system/lib/libgetubootenv.so \
	device/wmt/wmid/getubootenv/native/libgetubootenv.so:obj/lib/libgetubootenv.so \
	\
	device/wmt/wmid/ss_h264/engine/libwmth264adp.so:system/lib/libwmth264adp.so \
	device/wmt/wmid/ss_h264/engine/libwmth264adp.so:obj/lib/libwmth264adp.so \
	\
	device/wmt/wmid/wmt_check/libwmtchk1.so:system/lib/libwmtchk1.so \
	device/wmt/wmid/wmt_check/libwmtchk1.so:obj/lib/libwmtchk1.so \
	\
	device/wmt/wmid/wmt_check/libwmtchk2.so:system/lib/libwmtchk2.so \
	device/wmt/wmid/wmt_check/libwmtchk2.so:obj/lib/libwmtchk2.so \
	\
#	device/wmt/wmid/wmt_tscal/native/nativecal/libwmtnativecal.so:system/lib/libwmtnativecal.so \
#	device/wmt/wmid/wmt_tscal/native/nativecal/libwmtnativecal.so:obj/lib/libwmtnativecal.so \
#	\

