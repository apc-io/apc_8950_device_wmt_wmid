#ifndef WMT_GPIO_DEV_H
/* To assert that only one occurrence is included */
#define WMT_GPIO_DEV_H

/*--- wmt-pwm.h---------------------------------------------------------------
*   Copyright (C) 2009 WonderMedia Tech. Inc.
*
* MODULE       : wmt-pwm.h --
* AUTHOR       : Sam Shen
* DATE         : 2009/8/12
* DESCRIPTION  :
*------------------------------------------------------------------------------*/

/*--- History -------------------------------------------------------------------
*Version 0.01 , Sam Shen, 2009/8/12
*	First version
*
*------------------------------------------------------------------------------*/
/*-------------------- MODULE DEPENDENCY -------------------------------------*/


/*-------------------- EXPORTED PRIVATE TYPES---------------------------------*/
/* typedef  void  pwm_xxx_t;  *//*Example*/
struct wmt_reg_op_t{
	unsigned int addr;
	unsigned int bitmap;
	unsigned int regval;
};

struct gpio_operation_t {
	struct  wmt_reg_op_t ctl;
	struct  wmt_reg_op_t oc;
	struct  wmt_reg_op_t od;
	struct  wmt_reg_op_t id;
};

struct gpio_cfg_t {
	struct  wmt_reg_op_t ctl;
	struct  wmt_reg_op_t oc;
};

struct dedicated_gpio_operation_t {
	unsigned int no;
	unsigned int oc;
	unsigned int val;
};

/*--------------------- EXPORTED PRIVATE MACROS -------------------------------*/
//#define GPIO_IOC_MAGIC	'g'
#define GPIO_IOC_MAGIC	'6'

#define GPIODEDICATED _IOWR(GPIO_IOC_MAGIC, 0, void *)
#define GPIOCFG _IOW(GPIO_IOC_MAGIC, 1, void *)
#define GPIOWREG _IOW(GPIO_IOC_MAGIC, 2, void *)
#define GPIORREG _IOWR(GPIO_IOC_MAGIC, 3, void *)

#define FREESYSCACHES _IO(GPIO_IOC_MAGIC, 4)

#define GPIO_IOC_MAXNR	5

#endif
/*=== END wmt-pwm.h ==========================================================*/
