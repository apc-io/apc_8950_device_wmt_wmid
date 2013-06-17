/*
 * Copyright (c) 2008-2012 WonderMedia Technologies, Inc. All Rights Reserved.
 *
 * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
 * and may contain trade secrets and/or other confidential information of 
 * WonderMedia Technologies, Inc. This file shall not be disclosed to any
 * third party, in whole or in part, without prior written consent of
 * WonderMedia.
 *
 * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED
 * AS IS, WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS
 * OR IMPLIED, AND WONDERMEDIA TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS
 * OR IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * QUIET ENJOYMENT OR NON-INFRINGEMENT.
 */

#ifndef COM_CEC_H
#define COM_CEC_H

#if defined(__cplusplus)
extern "C" {
#endif

/* define for notify AP */
#define USER_PID 1
#define WP_PID	 2
#define USER_PIX_MAX 2
#define NETLINK_CEC_TEST (MAX_LINKS-2)

#define DEVICE_RX_DATA	0
#define DEVICE_PLUG_IN 	2
#define DEVICE_PLUG_OUT 3

#define MAX_MSG_BYTE 16
#define MSG_ABORT 0xff

typedef struct wmt_cec_msg{
	char msglen;
	char msgdata[MAX_MSG_BYTE];
} wmt_cec_msg_t;

typedef struct wmt_phy_addr {
	unsigned int phy_addr;
} wmt_phy_addr_t;

#define WMT_CEC_IOC_MAGIC           'c'
#define WMT_CEC_IOC_MAXNR           3
#define CECIO_TX_DATA				_IOW(WMT_CEC_IOC_MAGIC,0,wmt_cec_msg_t)
#define CECIO_TX_LOGADDR			_IO(WMT_CEC_IOC_MAGIC,1)
#define CECIO_RX_PHYADDR			_IOR(WMT_CEC_IOC_MAGIC,2,wmt_phy_addr_t)

#if defined(__cplusplus)
}
#endif

#endif

