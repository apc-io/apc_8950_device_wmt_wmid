#!/bin/sh
############################################################
# \copyright @ VIA 2013
# \author Nguyen Thanh Trung <nguyenthanh.trung@nomovok.vn>
############################################################

DEFAULT_DNS1="208.67.222.222"
DEFAULT_DNS2="208.67.220.220"

NEW_DNS1=$(exec getprop dhcp.eth0.dns1)
NEW_DNS2=$(exec getprop dhcp.eth0.dns2)

check_and_set_dns() {
	_propName=$1
	_newDns=$2
	_defaultDns=$3

	currentDns=$(exec getprop $_propName)

	dnsToSet=$_newDns

	if [ "$dnsToSet" = "" ] ; then
		echo "Empty value, use default one ($_defaultDns) for $_propName"
		dnsToSet=$_defaultDns
	fi

	if [ "$dnsToSet" = "$currentDns" ] ; then
		echo "value $dnsToSet is already set for $_propName"
		return
	fi

	echo "Set $_propName to $dnsToSet"
	setprop $_propName $dnsToSet
}

check_and_set_dns net.dns1 $NEW_DNS1 $DEFAULT_DNS1
check_and_set_dns net.dns2 $NEW_DNS2 $DEFAULT_DNS2


