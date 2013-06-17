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
#ifndef WMT_CEC_H
#define WMT_CEC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <unistd.h>

#include "com-cec.h"

// #include "viagfx_api.h"

#ifdef __cplusplus
#define CEC_API extern "C"
#else
#define CEC_API 
#endif

/* define CEC message opcode*/
#define ACTIVE_SOURCE 0x82
#define IMAGE_VIEW_ON 0x04
#define TEXT_VIEW_ON 0x0D
#define REQUEST_ACTIVE_SOURCE 0x85
#define ROUTING_CHANGE 0x80
#define ROUTING_INFO 0x81
#define SET_STREAM_PATH 0x86
#define STANDBY 0x36
#define RECORD_OFF 0x0B
#define RECORD_ON 0x09
#define RECORD_STATUS 0x0A
#define RECORD_TV_SCREEN 0x0F
#define GIVE_PHY_ADDR 0x83
#define GET_MENU_LAN 0x91
#define REPORT_PHY_ADDR 0x84
#define SET_MENU_LAN 0x32
#define DECK_CTRL 0x42
#define DECK_STATUS 0x1B
#define GIVE_DECK_STATUS 0x1A
#define PLAY 0x41
#define GIVE_TUNER_DEVICE_STATUS 0x08
#define SELECT_DIGI_SERVICE 0x93
#define TUNER_DEV_STATUS 0x07
#define TUNER_STEP_DECREMENT 0x06
#define TUNER_STEP_INCREMENT 0x05
#define DEV_VENDOR_ID 0x87
#define GIVE_DEV_VENDOR_ID 0x8c
#define VENDOR_CMD 0x89
#define VENDOR_REMOTE_BTN_DOWN 0x8A
#define VENDOR_REMOTE_BTN_UP 0x8B
#define SET_OSD_STR 0x64
#define GIVE_OSD_NAME 0x46
#define SET_OSD_NAME 0x47
#define MENU_REQUEST 0x8D
#define MENU_STATUS 0x8E
#define USER_CTRL_PRESSED 0x44
#define USER_CTRL_RELEASE 0x45
#define GIVE_DEV_POWER_STATUS 0x8F
#define REPORT_POWER_STATUS 0x90
#define FEATURE_ABORT 0x00
#define MSG_ABORT 0xff
#define INVALID_OPCODE 0xfe

static char BroadcastAddr=15;
typedef enum{
		TV = 0,
		Recording_Device,
		STB,
		DVD,
		Audio_System,
		CEC_SWITCH,
} CECDevType;

//error code
typedef enum{
		CEC_SUCCESS = 0,
		CEC_ERR_NOT_INITED=100,
		CEC_ERR_FAILED=200,
		CEC_ERR_IOCTL_ERROR,
		CEC_ERR_PARA_WRONG,
		CEC_ERR_OPEN_DEVICE_FAILED,
} FUNCTION_RETURN_CODE;

typedef struct {
	struct nlmsghdr nlh;
	wmt_cec_msg_t recv_msg;
} wmt_cec_packet_info_t;

typedef void (*callback)(char msg_type,char srcAddr, char tgtAddr, char opcode, char *opdata, char opdatalen);

//  Exported APIs
CEC_API FUNCTION_RETURN_CODE wmt_cec_init(void);

CEC_API FUNCTION_RETURN_CODE wmt_cec_GetLogicAddr(CECDevType devtype, char *Addr);
CEC_API FUNCTION_RETURN_CODE wmt_cec_SetLogicAddr(int logAddr);
CEC_API FUNCTION_RETURN_CODE wmt_cec_GetPhyAddr(unsigned short *phyaddr);
/*	wmt_cec_tx_msg is to send a message. If you are not clear about what is the message 
	opcode and what information needed to send the message, you can choose to
	use the API for a specific message.
*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_tx_msg(char srcAddr, char tgtAddr, char opcode, char *opdata, char opdata_len);
CEC_API FUNCTION_RETURN_CODE wmt_cec_abort_msg(void);
CEC_API FUNCTION_RETURN_CODE wmt_cec_rx_msg(callback handle_rx);
CEC_API FUNCTION_RETURN_CODE wmt_cec_exit(void);

/* 	Following APIs are interfaces for sending specific messages. 
	According to the parameters of these APIs, one can know what information 
	needed to send a specific message. 
	If you are clear about the message code and what information needed to 
	send the message, you can also use CEC_Tx_Msg directly.
*/

/* broadcast to other devices to inicate that it has started to be the active source */
CEC_API FUNCTION_RETURN_CODE wmt_cec_ActiveSource(char srcAddr, char broadcastAddr, unsigned short phyAddr);
/* send by a source device to the TV when it enters active state */
CEC_API FUNCTION_RETURN_CODE wmt_cec_ImageViewOn(char srcAddr, char tgtAddr);
/* send by a source device to the TV when it enters active state */
CEC_API FUNCTION_RETURN_CODE wmt_cec_TextViewOn(char srcAddr, char tgtAddr);
/* broadcast by a new device to discover the status of the system. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RequestActiveSource(char srcAddr, char broadcastAddr);
/* broadcast by a CEC switch to inform other switches that the active route below the switch has changed. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RoutingChange(char srcAddr, char broadcastAddr, unsigned short oldPhyAddr, unsigned short newPhyAddr);
/* broadcast by a CEC switch to inform other switches the active route. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RoutingInfo(char srcAddr, char broadcastAddr, unsigned short phyAddr);
/* broadcast by TV to request the specified physical address. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_SetStreamPath(char srcAddr, char broadcastAddr, unsigned short phyAddr);
/* switch one or all devices into standby mode. tgtaddress can be a broadcast address, or a specific address. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_StandBy(char srcAddr, char tgtAddr);
/* request a recording device to stop recording. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RecordOff(char srcAddr, char tgtAddr);
/*request the recording device to record the specified source. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RecordOn(char srcAddr, char tgtAddr, char *recordSrc, char recordSrcLen);
/*send by a recording device to inform the initiator of message <record on> about its status. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RecordStatus(char srcAddr, char tgtAddr, char *recordStatusInfo, char recordStatusInfoLen);
/*request by recording device to record the current source. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_RecordTVScreen(char srcAddr, char tgtAddr);
/*request a device to return its physical address. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_GivePhyAddr(char srcAddr, char tgtAddr);
/*send by device that is capable of character generation to TV (or opposite direction) to discover the current menu language. */
CEC_API FUNCTION_RETURN_CODE wmt_cec_GetMenuLan(char srcAddr, char tgtAddr);
/*used by any device for device discovery, through it can find a logic address for a new device */
CEC_API FUNCTION_RETURN_CODE wmt_cec_PollingMsg(char srcAddr, char tgtAddr);
/*inform all other devices the mapping between its physical address and logical address */
CEC_API FUNCTION_RETURN_CODE wmt_cec_ReportPhyAddr(char srcAddr, char broadcastAddr, unsigned short phyAddr, char logicAddr);
/*used by any device for device discovery, through it can find a logic address for a new device */
CEC_API FUNCTION_RETURN_CODE wmt_cec_SetMenuLan(char srcAddr, char broadcastAddr, char *lan, char lanLen);
/*used to control a device's media functions */
CEC_API FUNCTION_RETURN_CODE wmt_cec_DeckControl(char srcAddr, char tgtAddr, char *deckCtrlMode, char deckCtrlModeLen);
/*used to provide a deck's status to the initiator of messge <give deck status>*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_DeckStatus(char srcAddr, char tgtAddr, char *deckInfo, char deckInfoLen);
/*request the status of a device*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_GiveDeckStatus(char srcAddr, char tgtAddr, char *statusRequest, char statusRequestLen);
/*control the playback debhaviour*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_Play(char srcAddr, char tgtAddr, char *playMode, char playModeLen);
/* request the status of a tuner device */
CEC_API FUNCTION_RETURN_CODE wmt_cec_GiveTunerDevStatus(char srcAddr, char tgtAddr, char *statusRequest, char statusRequestLen);
/*directly selects a digital TV, Radio or data broadcast services.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_SelectDigiService(char srcAddr, char tgtAddr, char *digiServiceID, char digiServiceIDLen);
/*used by tuner device to provide its status to the initiator of message <give tuner device status>.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_TunerDevStatus(char srcAddr, char tgtAddr, char *tunerDevInfo, char tunerDevInfoLen);
/*send by TV to tuner device tune to next lowest service in tuner's service list.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_TunerStepDecrement(char srcAddr, char tgtAddr);
/*send by TV to tuner device tune to next highest service in tuner's service list.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_TunerStepIncrement(char srcAddr, char tgtAddr);
/*broadcast to other devices the vendor ID of this device.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_DeviceVendorID(char srcAddr, char broadcastAddr, char *vendorID, char vendorIDLen);
/*request the vendor ID from a device.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_GiveVendorID(char srcAddr, char tgtAddr);
/*request the vendor ID from a device.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_VendorCommand(char srcAddr, char tgtAddr, char *vendorData, char vendorDataLen);
/*indicate that a remote control button has been depressed.*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_VendorRemoteBtnDown(char srcAddr, char tgtAddr, char *vendorRCCode, char vendorRCCodeLen);
/*indicate that a remote control button has been released*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_VendorRemoteBtnUp(char srcAddr, char tgtAddr);
/*used to send a text message to output on a TV*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_SetOSDString(char srcAddr, char tgtAddr, char *displayTiming, char displayTimingLen, char *OSDString, char OSDStringLen);
/*request the preferred OSD name of a device for use in menus*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_GiveOSDName(char srcAddr, char tgtAddr);
/*set the preferred OSD name of a device for use in menus*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_SetOSDName(char srcAddr, char tgtAddr, char *OSDName, char OSDNameLen);
/*request from the TV for a device to show/remove a menu*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_MenuRequest(char srcAddr, char tgtAddr, char MenuRequestType);
/* indicate to TV that the device does show/remove a menu*/
CEC_API FUNCTION_RETURN_CODE wmt_cec_MenuStatus(char srcAddr, char tgtAddr, char MenuStatus);
/* */
CEC_API FUNCTION_RETURN_CODE wmt_cec_UserControlPressed(char srcAddr, char tgtAddr, char UICmd);
/* */
CEC_API FUNCTION_RETURN_CODE wmt_cec_UserControlReleased(char srcAddr, char tgtAddr);
/* */
CEC_API FUNCTION_RETURN_CODE wmt_cec_GiveDevPowerStatus(char srcAddr, char tgtAddr);
/* */
CEC_API FUNCTION_RETURN_CODE wmt_cec_ReportPowerStatus(char srcAddr, char tgtAddr, char powerStatus);
/*********************************
Power Status:
0. on
1. off
*********************************/
CEC_API FUNCTION_RETURN_CODE wmt_cec_FeatureAbort(char srcAddr, char tgtAddr, char featureOpcode, char abortReason);
/*********************************
Abort Reason:
0. unrecognized opcode.
1. not in correct mode to respond.
2. cannot provide source.
3. invalid operand.
4. refused.
*********************************/
#endif
