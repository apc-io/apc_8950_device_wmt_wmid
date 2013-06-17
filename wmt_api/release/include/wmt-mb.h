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
#ifndef WMT_MB_H
/* To assert that only one occurrence is included */
#define WMT_MB_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/

#define MB_DEVICE     "/dev/mbdev"


/*------------------------------------------------------------------------------
   Memory Block API
------------------------------------------------------------------------------*/
/*	following is the C++ header	*/
#ifdef	__cplusplus
extern "C" {
#endif

// allocate continual memory
void *wmt_mb_alloc(int fd, int size);
// free allocated continual memory
void wmt_mb_free(int fd, void *address);
// map kernal allocated continual memory to user addrsss
// NOTE: the kernal allocated continual memory must come
//       from memblock driver
void *wmt_mb_map(int fd, unsigned long virt);
// unmap wmt_mb_map user memory
void wmt_mb_unmap(int fd, void *user);
// add mb user counter
void wmt_mb_get(int fd, void *user);
// sub mb user counter
void wmt_mb_put(int fd, void *user);
// Initial memblock driver
int wmt_mb_init(void);
// Exit memblock driver
int wmt_mb_exit(int fd);
// Get virt address from specific user address
unsigned long wmt_mb_user2virt(int fd, void *user);
// Get phys address from specific user address
unsigned long wmt_mb_user2phys(int fd, void *user);
// Get current max available size
unsigned long wmt_mb_max_available_size(int fd);
// Get current static mba size.
unsigned long wmt_mb_static_size(int fd);
// pre-fetch memblock for wmt_mb_alloc
unsigned long wmt_mb_prefetch(int fd, int size);
// get mb user count
unsigned long wmt_mb_count(int fd, unsigned long phys);
// allocate continual memory with physical address
unsigned long wmt_mb_phys_alloc(int fd, int size);
// free allocated continual memory with physical address
void wmt_mb_phys_free(int fd, unsigned long phys);
// add mb user counter by physical address
void wmt_mb_phys_get(int fd, unsigned long phys);
// sub mb user counter by physical address
void wmt_mb_phys_put(int fd, unsigned long phys);

#ifdef	__cplusplus
}
#endif	

#endif /* ifndef WMT_MB_H */

/*=== END wmt-mb.h ==========================================================*/

