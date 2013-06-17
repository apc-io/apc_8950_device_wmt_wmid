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
#ifndef COM_MB_H
/* To assert that only one occurrence is included */
#define COM_MB_H


/*-------------------- MODULE DEPENDENCY -------------------------------------*/

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

/*------------------------------------------------------------------------------
   Macros below are used for driver in IOCTL
------------------------------------------------------------------------------*/
#define MB_IOC_MAGIC			'M'

#define MBIO_MALLOC				_IOWR(MB_IOC_MAGIC, 0, unsigned long) // O: physical address	I: size
#define MBIO_FREE				_IOWR(MB_IOC_MAGIC, 1, unsigned long) // O: ummap size		I: user address if map, physical address if not map
#define MBIO_UNMAP				_IOWR(MB_IOC_MAGIC, 2, unsigned long) // O: ummap size		I: user address
#define MBIO_MBSIZE				_IOWR(MB_IOC_MAGIC, 3, unsigned long) // O: mb size			I: phys address
#define MBIO_MAX_AVAILABLE_SIZE	_IOR (MB_IOC_MAGIC, 4, unsigned long) // O: max free mba size
/* advance use only */
#define MBIO_GET				_IOW (MB_IOC_MAGIC, 5, unsigned long) // I: user address
#define MBIO_PUT				_IOW (MB_IOC_MAGIC, 6, unsigned long) // I: user address
#define MBIO_USER_TO_VIRT		_IOWR(MB_IOC_MAGIC, 7, unsigned long) // O: virt address		I: user address
#define MBIO_USER_TO_PHYS		_IOWR(MB_IOC_MAGIC, 8, unsigned long) // O: phys address		I: user address
#define MBIO_PREFETCH			_IOW (MB_IOC_MAGIC, 9, unsigned long) // I: size
#define MBIO_STATIC_SIZE		_IOR (MB_IOC_MAGIC,10, unsigned long) // O: static mba size
#define MBIO_MB_USER_COUNT		_IOWR(MB_IOC_MAGIC,11, unsigned long) // O: use counter		I: physical address
#define MBIO_FORCE_RESET		_IO  (MB_IOC_MAGIC,12)
#define MBIO_GET_BY_PHYS		_IOW (MB_IOC_MAGIC,13, unsigned long) // I: phys address
#define MBIO_PUT_BY_PHYS		_IOW (MB_IOC_MAGIC,14, unsigned long) // I: phys address

#endif /* ifndef COM_MB_H */

/*=== END com-mb.h ==========================================================*/

