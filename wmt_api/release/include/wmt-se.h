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
#ifndef WMT_SE_H
/* To assert that only one occurrence is included */
#define WMT_SE_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "com-se.h"

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  CRYUT_XXXX  1    *//*Example*/

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  cryut_xxx_t;  *//*Example*/

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
/* EXTERN int      cryut_xxx; *//*Example*/


/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/
/* #define CRYUT_XXX_YYY   xxxx *//*Example*/
#define SIZEOF(x) sizeof(x)
#define NO_OF( x )  ( SIZEOF( (x) ) / SIZEOF( (x)[0] ) )

/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  cryut_xxx(void); *//*Example*/
#ifdef __cplusplus
extern "C" {
#endif

int wmt_cipher_init( void );
void wmt_cipher_exit(int fd_in);
u32* wmt_cipher_in_buf_addr(int fd_in);
u32* wmt_cipher_out_buf_addr(int fd_in);
u32 wmt_cipher_get_buf_size(void);
int wmt_cipher_enc_dec( IN OUT cypher_base_cfg_t *cipher_obj );
void wmt_cipher_clear( IN cypher_base_cfg_t *cipher_obj );

int wmt_cipher_set_algorithm(
	IN OUT cypher_base_cfg_t *cipher_obj, 	
	IN cypher_algo_t algorithm );

int wmt_cipher_set_operation(
	IN OUT cypher_base_cfg_t *cipher_obj, 	
	IN cypher_op_t operation );

int wmt_cipher_set_key( 
	IN OUT cypher_base_cfg_t *cipher_obj, 
	IN u32 key_buf, 
	IN u32 key_len );

int wmt_cipher_set_iv( 
	IN OUT cypher_base_cfg_t *cipher_obj, 
	IN u32 iv_buf, 
	IN u32 iv_len );

int wmt_cipher_set_inc( 
	IN OUT cypher_base_cfg_t *cipher_obj,
	IN u32 inc );

int wmt_cipher_set_input( 
	IN OUT cypher_base_cfg_t *cipher_obj, 
	IN u32 input_buf, 
	IN u32 input_len );

int wmt_cipher_set_output( 
	IN OUT cypher_base_cfg_t *cipher_obj, 
	IN u32 output_buf, 
	IN u32 output_len );

int wmt_cipher_check_base( IN cypher_base_cfg_t *cipher_obj );

#ifdef __cplusplus
}
#endif

#endif /* ifndef WMT_SE_H */

/*=== END wmt-se.h ==========================================================*/
