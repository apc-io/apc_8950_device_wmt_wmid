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
#ifndef COM_SE_H
/* To assert that only one occurrence is included */
#define COM_SE_H

/*-------------------- MODULE DEPENDENCY -------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


// Include your headers here

/*-------------------- EXPORTED PRIVATE CONSTANTS ----------------------------*/
/* #define  CIPHER_XXXX  1    *//*Example*/
// Global definition
#ifndef u8
#define u8  unsigned char
#endif
#ifndef u16
#define u16 unsigned short
#endif
#ifndef u32
#define u32 unsigned int
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

/*------------------------------------------------------------------------------
    Common members in structure for all kinds of encrypt/descrypt
------------------------------------------------------------------------------*/
#define CYPHER_COMMON_CFG() \
    int fd;  /* file handle from driver */ \
	cypher_algo_t algo_mode; \
	u32 input_addr;   \
	u32 output_addr;	\
	u32 text_length
/* End of CYPHER_COMMON_CFG() */


//#define CIPHER_IOC_MAGIC	'k'
#define CIPHER_IOC_MAGIC	238

#define CIPHER_ENCRYPT		_IOWR(CIPHER_IOC_MAGIC, 0, int)
#define CIPHER_DECRYPT		_IOWR(CIPHER_IOC_MAGIC, 1, int)
#define CIPHER_GET_ADDR_INPUT		_IOWR(CIPHER_IOC_MAGIC, 2, int)
#define CIPHER_GET_ADDR_OUTPUT		_IOWR(CIPHER_IOC_MAGIC, 3, int)
#define CIPHER_LOCK                 _IO(CIPHER_IOC_MAGIC, 4)
#define CIPHER_UNLOCK               _IO(CIPHER_IOC_MAGIC, 5)
#define CIPHER_IOC_MAXNR	6

/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  cipher_xxx_t;  *//*Example*/
/*
enum {
    CIPHER_XXX,
    CIPHER_YYY,

}; */  /*Example*/

// CYPHER Data structure
typedef enum cypher_algo_e
{
	CYPHER_ALGO_AES = 0,		// Advanced Encryption Standard      
	CYPHER_ALGO_RC4,			// Rivest Cipher
	CYPHER_ALGO_DES,			// Data Encryption Standard
	CYPHER_ALGO_TDES,			// Three Data Encryption Standard
	CYPHER_ALGO_SHA1,			// Secure Hash Algorithm 
	//2011/07/07 Brad Zhan
	CYPHER_ALGO_SHA256, 				// Secure Hash Algorithm 256
	CYPHER_ALGO_MAX
} cypher_algo_t;	// Cryptographic algorithm mode

// http://www-128.ibm.com/developerworks/tw/library/s-crypt02.html
typedef enum cypher_op_e
{
	CYPHER_OP_ECB = 0,		// Electronic Codebook mode
	CYPHER_OP_CBC,			// Cipher Block Chaining mode
	CYPHER_OP_CTR,			// Counter mode
	CYPHER_OP_OFB,			// Output Feedback mode
	CYPHER_OP_RC4_HW_KEY, // Key calculate by HW
	CYPHER_OP_RC4_SW_KEY, // Key calculate by SW
	//CYPHER_OP_SHA1_3, 	// PlaintextInSHA1_3
	//CYPHER_OP_SHA1_56, 	// PlaintextInSHA1_56
	//CYPHER_OP_SHA1_64, 	// PlaintextInSHA1_64
	//CYPHER_OP_SHA1_72, 	// PlaintextInSHA1_72
	CYPHER_OP_ECB_HW_KEY,		// Electronic Codebook mode HW key
	CYPHER_OP_CBC_HW_KEY,		// Electronic Codebook mode HW key
	CYPHER_OP_CTR_HW_KEY,		// Electronic Codebook mode HW key
	CYPHER_OP_OFB_HW_KEY,		// Electronic Codebook mode HW key
	CYPHER_OP_MAX
} cypher_op_t;

typedef struct cypher_common_cfg_s
{
    CYPHER_COMMON_CFG();
} cypher_common_cfg_t;

/*------------------------------------------------------------------------------
    cypher_base_cfg_t is used for AES/RC4 encrypt/descrypt
------------------------------------------------------------------------------*/
typedef struct cypher_base_cfg_s
{
    CYPHER_COMMON_CFG();

	cypher_op_t op_mode;
	u32 dec_enc;   /* Used in driver only */
	u32 key_addr;  // address
	u32 IV_addr;   // address: Used in both CBC and CTR modes 
	u32 INC;       // Used in CTR mode only
	u32 key_len;  // Used in RC4 mode only
	u32 sha1_data_length;	// for SHA1 only
	u32 sha256_data_length;	// for SHA256 only
} cypher_base_cfg_t;

/*
 * Add the cipher interface to support the Autotest mode.
 */
struct autolist {
    int total;
    int pass;
    int fail;
};

//int RC4_KEY_BYTE_SIZE;			// from 1Byte to 64Byte	//remove by Brad 2011.08.26

/*-------------------- EXPORTED PRIVATE VARIABLES -----------------------------*/
#ifdef WMT_BASIC_CIPHER_C /* allocate memory for variables only in wmt-basic-cipher.c */
#       define EXTERN
#else
#       define EXTERN   extern
#endif /* ifdef WMT_BASIC_CIPHER_C */

/* EXTERN int      cipher_xxxx; *//*Example*/

#undef EXTERN

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/
// Encrypt/Decrypt mode
#define CYPHER_ENCRYPT 1
#define CYPHER_DECRYPT 0

// Enabled/unabled mode
#define CYPHER_ENABLED 1
#define CYPHER_UNABLED 0

#define CYPHER_NOT_CARE 0
#define CYPHER_ZERO 0x0L

#define CYPHER_ALGO_BASE_START   CYPHER_ALGO_AES
#define CYPHER_ALGO_BASE_END     CYPHER_ALGO_MAX

#define PRD_BYTE_COUNT_BOUNDARY 16

#define BUFFER_MULTIPLE 256
#define DMA_BOUNDARY 16  // 16 bytes

// is 4096 ==> total_byes:4096*sizeof(u32)=16384
#define BUFFER_MAX (BUFFER_MULTIPLE * DMA_BOUNDARY * 64 ) //add size to 1M by Brad 2011.08.26

#define AES_KEY_BYTE_SIZE 16  //  = 128 bits = 16 char = 32 hex int
#define DES_KEY_BYTE_SIZE 8  			//  8B = 64 bits = 8 char = 16 hex int
#define TDES_KEY_BYTE_SIZE 24  		//  24B = 192 bits = 24 char = 48 hex int
#define AES_IV_BYTE_SIZE 16
#define DES_IV_BYTE_SIZE 8

#define AES_RC4_DATA_MAX 307200

#define RC4_SW_KEY_MAX 64
#define RC4_HW_KEY_MAX 32
#define RC4_KEY_BYTE_MAX 256

#define SHA1_TEXT_BOUNDARY 64
#define SHA1_OUTPUT_SIZE 20

//2011/07/07 Brad Zhan
#define SHA256_TEXT_BOUNDARY 4
#define SHA256_OUTPUT_SIZE 32

#define AES_TEXT_BYTE_SIZE		16  //  = 128 bits = 16 char = 32 hex int
#define DES_TEXT_BYTE_SIZE		8  //  = 64 bits = 8 char = 16 hex int
#define TDES_TEXT_BYTE_SIZE		24  //  = 192 bits = 24 char = 48 hex int
#define RC4_TEXT_BYTE_SIZE		1  //  = 8 bits = 1 char = 2 hex int

/* performance test used */
//#define CYPHER_DATA_MAX 16384		// 16KB
#define CYPHER_DATA_MAX 8192		// 8KB
//#define CYPHER_DATA_MAX 4096		// 4KB

//#define Enc_Dec_Time	1
//#define Detail_Time	1


/*--------------------- EXPORTED PRIVATE FUNCTIONS  ---------------------------*/
/* extern void  cipher_xxxx(vdp_Void); *//*Example*/
unsigned int get_timer(void);


#endif /* ifndef COM_SE_H */

/*=== END com-se.h ==========================================================*/
