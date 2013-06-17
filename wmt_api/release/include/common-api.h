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
#ifndef COMMON_API_H
/* To assert that only one occurrence is included */
#define COMMON_API_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/

/*	following is the C++ header	*/
#ifdef	__cplusplus
extern	"C" {
#endif

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  COMMON_XXXX  1    *//*Example*/

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  common_xxx_t;  *//*Example*/

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef COMMON_API_C /* allocate memory for variables only in common-api.c */
#       define EXTERN
#else
#       define EXTERN   extern
#endif /* ifdef COMMON_API_C */

/* EXTERN int      common_xxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/
/* #define COMMON_XXX_YYY   xxxx *//*Example*/
#define CHIP_MASK			0xFFFF0000
#define VERSION_MASK		0x0000FFFF

#define CHIP_VT8430			0x84300000
#define CHIP_VT8500			0x85000000


/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  common_xxx(void); *//*Example*/

void kbd_init(void);
void kbd_close(void);
int kbd_hit(void);
int kbd_readch(void);
int kbd_getch(void);
int kbd_getnum(int min,int max,int retry);
int kdb_get_str(char *str,int max);
int kdb_get_digit(void);
int kbd_get_num(int min, int max, char *msg, int retry);

int sys_get_kernel_version(void);


#ifdef	__cplusplus
}
#endif	

#endif /* ifndef COMMON_API_H */

/*=== END common-api.h ==========================================================*/

