/*
***************************************************************************************************
**  Copyright   : Copyright(c) 2015 PROTRULY. All rights reserved.
**  Project     : KEA128_Frame
**  File        : printf.h
**  Description : This file is used to provide print function.
**  Author      : YizhongLiang
**  Created on  : 2015.07.23 - 2015.07.23
**  Note        : NULL
***************************************************************************************************
*/


#ifndef _PRINTF_H_
#define _PRINTF_H_

#ifdef  __cplusplus
extern  "C" {
#endif


typedef SCH_BOOL		PRT_BOOL;    /* Bool */
typedef SCH_S8			PRT_S8;        /* signed char */
typedef SCH_U8			PRT_U8;        /* unsigned char */
typedef SCH_S16			PRT_S16;        /* signed short */
typedef SCH_U16			PRT_U16;        /* unsigned short */
typedef SCH_S32			PRT_S32;        /* signed int */
typedef SCH_U32			PRT_U32;        /* unsigned int */
///typedef SCH_S64			PRT_S64;        /* signed int */
///typedef SCH_U64			PRT_U64;        /* unsigned int */

#define PRINT_TO_TXBUF(data)            SCH_TO_TXBUF(data)

/*
*******************************************************************************
**  Include file
*******************************************************************************
*/
#include <stdarg.h>

/*
*******************************************************************************
**  Macro definition
*******************************************************************************
*/
#define   PRINTF_CONTOHEX                         (16)                                             /*  */
#define   PRINTF_CONTODEC                         (10)                                             /*  */

/*
*******************************************************************************
**
*******************************************************************************
*/
#ifdef PRINTF_EN
void Printf (PRT_U8 *fmt, ...);
#else
#define Printf(fmt,...);
#endif

#ifdef  __cplusplus
}
#endif

#endif



/* ************************************ File end *********************************************** */
