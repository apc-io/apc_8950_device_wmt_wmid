#!/bin/sh
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

HAS_VT1609=$(exec wmtenv get wmt.io.ts| grep -c 'vt1609')
VT160x_ts_param=$(exec wmtenv get vt160x.ts.param)

if [ "$HAS_VT1609" = "1" ]; then
    if [ -z "$VT160x_ts_param" ];then
        insmod /lib/modules/vt1609_spi_dual_touch_support.ko vxy=6 scale=1 stay=9 dual_f=5
    else
        insmod /lib/modules/vt1609_spi_dual_touch_support.ko $VT160x_ts_param
    fi
fi
