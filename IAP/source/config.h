
#ifndef _CONFIG_H
#define _CONFIG_H


#define BIT0				0x0001
#define BIT1				0x0002
#define BIT2				0x0004
#define BIT3				0x0008
#define BIT4				0x0010
#define BIT5				0x0020
#define BIT6				0x0040
#define BIT7				0x0080
#define BIT8				0x0100
#define BIT9				0x0200
#define BIT10				0x0400
#define BIT11				0x0800
#define BIT12				0x1000
#define BIT13				0x2000
#define BIT14				0x4000
#define BIT15				0x8000
#define BIT16				0x10000
#define BIT17				0x20000
#define BIT18				0x40000
#define BIT19				0x80000
#define BIT20				0x100000
#define BIT21				0x200000
#define BIT22				0x400000
#define BIT23				0x800000
#define BIT24				0x1000000
#define BIT25				0x2000000
#define BIT26				0x4000000
#define BIT27				0x8000000
#define BIT28				0x10000000
#define BIT29				0x20000000
#define BIT30				0x40000000
#define BIT31				0x80000000



typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef enum{
	TRUE = 0,
	FALSE
}bool;
enum{reset = 0,set = 1};


#define assert_param(expr) ((void)0)



#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_rcc.h"
#include "uart2.h"
#include "flash_if.h"
#include "iap.h"
#include "bsp.h"
















#endif
