#ifndef __SCH_CONFIG_H
#define __SCH_CONFIG_H
#include "config.h"

typedef SCH_U8 *pU8;

typedef void (*pfunction)(void); 
typedef void (*U8pfunction)(SCH_U8); 


///=============config=================
//#include "mw_uart.h"

#define PRINTF_EN
#define PRINTF_UART				SCH_Uart1
#define PRINTF_BAUDRATE         115200
#define SCH_TO_TXBUF(data)		UartTxData_Direct(SCH_Uart1, data, 1)///UartTxData(PRINTF_UART, data, 1)///
//#ifdef PRINTF_EN
//	#define Print_Init()        SysUartInit(PRINTF_UART,PRINTF_BAUDRATE)
//#else
//	#define Print_Init()
//#endif
///==========
#define SCH_INT_DISABLE		__disable_irq()
#define SCH_INT_ENABLE		__enable_irq()

///==============================
#define SCH_NOP        __NOP()

///=================================================
#include "sch_queue.h"
#include "sch_common.h"
#include "sch_message.h"
#include "sch_printf.h"

#endif
