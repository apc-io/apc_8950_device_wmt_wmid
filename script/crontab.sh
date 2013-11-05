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
mount -o remount,rw -t yaffs2 `grep /system /proc/mounts | cut -d' ' -f1` /system
echo "root:x:0:0::/root:/system/bin/sh" > /etc/passwd
mkdir -p /etc/cron
echo "* * * * * /usr/sbin/ntpd -d -N -q -p 64.250.177.145" > /etc/cron/root
mount -o remount,ro -t yaffs2 `grep /system /proc/mounts | cut -d' ' -f1` /system
#mount -o remount,rw rootfs /
#ln -s /system/bin/ /bin
#mount -o remount,ro rootfs /
crond -c /etc/cron
