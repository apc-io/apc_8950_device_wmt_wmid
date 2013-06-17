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

#/system/bin/ifconfig eth0 10.5.86.56
#route add default gw 10.5.87.254 dev eth0
IP_ADDRESS=$(exec getprop static.eth0.ipaddress)
GATEWAYIP=$(exec getprop static.eth0.gateway)
NETMASK=$(exec getprop static.eth0.mask)
DNS1=$(exec getprop static.eth0.dns1)
DNS2=$(exec getprop static.eth0.dns2)
eth_mode=$(exec getprop eth.mode)
boot_completed=$(exec getprop sys.boot_completed)
eth_enable=$(exec getprop eth.enable)
current_ipaddr=$(exec ifconfig eth0 | grep "inet addr" |  tr -s ' ' ' ' | cut -d ' ' -f3 | cut -d ':' -f2) 

if [ "$eth_enable" = "1" ];then
    if [ "$eth_mode" = "static" ];then
        if [ "$boot_completed" = "1" ];then
            /system/bin/ifconfig eth0 down
        fi
        /system/bin/ifconfig eth0 $IP_ADDRESS 
        route add default gw $GATEWAYIP dev eth0
    else
        if [ "$boot_completed" = "1" ];then
            /system/bin/ifconfig eth0 down
            /system/bin/ip addr del $current_ipaddr dev eth0
            /system/bin/ifconfig eth0 up
         else
            /system/bin/dhcpcd eth0
        fi
    fi
else
    /system/bin/ifconfig eth0 down
fi
