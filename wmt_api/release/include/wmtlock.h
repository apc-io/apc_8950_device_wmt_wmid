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
#ifndef WMTLOCK_H_INCLUDED
#define WMTLOCK_H_INCLUDED

#ifdef	__cplusplus
extern "C" {
#endif

/*Define wmt lock types*/

typedef enum 
{
    lock_jpeg = 0,
    lock_video,   
    lock_max,
}wmt_lock_type;


/*Open wmt_lock driver, return fd ( < 0 if failed )*/
int wmtlock_init();

// close wmt_lock driver
int wmtlock_exit(int lock_fd);

/*Get the lock.                                                                    */
/*timeout:  mseconds wait value, -1: INFINITE, 0:return immediately if can't get it*/
int wmtlock_lock(int lock_fd, wmt_lock_type type, int timeout);

/*Release the lock*/
int wmtlock_unlock(int lock_fd, wmt_lock_type type);


#ifdef	__cplusplus
}
#endif	

#endif //WMTLOCK_H_INCLUDED



