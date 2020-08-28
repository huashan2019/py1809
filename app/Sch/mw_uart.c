/*
***************************************************************************************************
**  Copyright   : 
**  Project     : STM32F103
**  File        : mw_uart.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.05.05
**  Note        : NULL
***************************************************************************************************
*/
#include "config.h"

#if UART0_FUNC == ENABLE
#define MAX_UART0_TX_BUF       128
SCH_U8 Uart0_TX_buf[MAX_UART0_TX_BUF];
QUEUE_T UART0_TX_QUEUE = {0,0,0,MAX_UART0_TX_BUF,1,Uart0_TX_buf};
#define MAX_UART0_RX_BUF       128
SCH_U8 Uart0_RX_buf[MAX_UART0_RX_BUF];
QUEUE_T UART0_RX_QUEUE = {0,0,0,MAX_UART0_RX_BUF,1,Uart0_RX_buf};
#endif
#if UART1_FUNC == ENABLE
#define MAX_UART1_TX_BUF       128
SCH_U8 Uart1_TX_buf[MAX_UART1_TX_BUF];
QUEUE_T UART1_TX_QUEUE = {0,0,0,MAX_UART1_TX_BUF,1,Uart1_TX_buf};
#define MAX_UART1_RX_BUF       128
SCH_U8 Uart1_RX_buf[MAX_UART1_RX_BUF];
QUEUE_T UART1_RX_QUEUE = {0,0,0,MAX_UART1_RX_BUF,1,Uart1_RX_buf};
#endif
#if UART2_FUNC == ENABLE
#define MAX_UART2_TX_BUF       128
SCH_U8 Uart2_TX_buf[MAX_UART2_TX_BUF];
QUEUE_T UART2_TX_QUEUE = {0,0,0,MAX_UART2_TX_BUF,1,Uart2_TX_buf};
#define MAX_UART2_RX_BUF       128
SCH_U8 Uart2_RX_buf[MAX_UART2_RX_BUF];
QUEUE_T UART2_RX_QUEUE = {0,0,0,MAX_UART2_RX_BUF,1,Uart2_RX_buf};
#endif
#if UART3_FUNC == ENABLE
#define MAX_UART3_TX_BUF       128
SCH_U8 Uart3_TX_buf[MAX_UART3_TX_BUF];
QUEUE_T UART3_TX_QUEUE = {0,0,0,MAX_UART3_TX_BUF,1,Uart3_TX_buf};
#define MAX_UART3_RX_BUF       128
SCH_U8 Uart3_RX_buf[MAX_UART3_RX_BUF];
QUEUE_T UART3_RX_QUEUE = {0,0,0,MAX_UART3_RX_BUF,1,Uart3_RX_buf};
#endif
#if UART4_FUNC == ENABLE
#define MAX_UART4_TX_BUF       128
SCH_U8 Uart4_TX_buf[MAX_UART4_TX_BUF];
QUEUE_T UART4_TX_QUEUE = {0,0,0,MAX_UART4_TX_BUF,1,Uart4_TX_buf};
#define MAX_UART4_RX_BUF       128
SCH_U8 Uart4_RX_buf[MAX_UART4_RX_BUF];
QUEUE_T UART4_RX_QUEUE = {0,0,0,MAX_UART4_RX_BUF,1,Uart4_RX_buf};
#endif
#if UART5_FUNC == ENABLE
#define MAX_UART5_TX_BUF       128
SCH_U8 Uart5_TX_buf[MAX_UART5_TX_BUF];
QUEUE_T UART5_TX_QUEUE = {0,0,0,MAX_UART5_TX_BUF,1,Uart5_TX_buf};
#define MAX_UART5_RX_BUF       128
SCH_U8 Uart5_RX_buf[MAX_UART5_RX_BUF];
QUEUE_T UART5_RX_QUEUE = {0,0,0,MAX_UART5_RX_BUF,1,Uart5_RX_buf};
#endif

QUEUE_T *const UartBufAddr[][2] = 
{
#if UART0_FUNC == ENABLE
	&UART0_RX_QUEUE,&UART0_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART1_FUNC == ENABLE
	&UART1_RX_QUEUE,&UART1_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART2_FUNC == ENABLE
	&UART2_RX_QUEUE,&UART2_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART3_FUNC == ENABLE
	&UART3_RX_QUEUE,&UART3_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART4_FUNC == ENABLE
	&UART4_RX_QUEUE,&UART4_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART5_FUNC == ENABLE
	&UART5_RX_QUEUE,&UART5_TX_QUEUE,
#else
	NULL,NULL,
#endif
};

USART_TypeDef *Uart_Arry[]={USART1,USART1,USART2,USART3,USART4};
///========================================================
void UartBufInit(Uart_T uart,Uart_RT TxRx)
{
	if(UartBufAddr[uart][TxRx])
		Queue_Init(UartBufAddr[uart][TxRx]);
}
SCH_U16 UartBufCnt(Uart_T uart,Uart_RT TxRx)
{
	QUEUE_T *pUartBuf;
	pUartBuf = UartBufAddr[uart][TxRx];
	if(pUartBuf)
		return Queue_Cnt(pUartBuf);
	return 0;
}
SCH_BOOL UartGetFromBuf(Uart_T uart, Uart_RT TxRx, SCH_U8 *data, SCH_U16 Len)
{
	QUEUE_T *pUartBuf;
	pUartBuf = UartBufAddr[uart][TxRx];
	if(pUartBuf)
		return Queue_Out(pUartBuf, data, Len);
	return FALSE;
}
SCH_BOOL UartPutToBuf(Uart_T uart, Uart_RT TxRx, SCH_U8 *const data, SCH_U16 Len)
{
	QUEUE_T *pUartBuf;
	pUartBuf = UartBufAddr[uart][TxRx];
	if(pUartBuf)
		return Queue_In(pUartBuf, data,Len);
	return FALSE;
}
/********************************************************************************
**  Function	: UartSendData8
**  Author		: 
**  Created on	: 20160621
**  Description	: 发送一个8位的数据
**  Return		: void
********************************************************************************/
void UartSendData8(Uart_T uart,SCH_U8 u8data)
{
	USART_SendData(Uart_Arry[uart], u8data); 
}
/********************************************************************************
**  Function	: UartTxInt En/Dis
**  Author		: 
**  Created on	: 20160621
**  Description	:
**  Return		: void
********************************************************************************/
void UartTxIntEn(Uart_T uart)
{
	USART_ITConfig(Uart_Arry[uart],USART_IT_TXE,ENABLE);
}
void UartTxIntDis(Uart_T uart)
{
	USART_ITConfig(Uart_Arry[uart],USART_IT_TXE,DISABLE);
}
/********************************************************************************
**  Function	: UartRx/TxCnt
**  Author		: 
**  Created on	: 20160621
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 UartRxCnt(Uart_T uart)
{
	return UartBufCnt(uart,Uart_Rx);
}
SCH_U16 UartTxCnt(Uart_T uart)
{
	return UartBufCnt(uart,Uart_Tx);
}
/********************************************************************************
**  Function	: UartRxData
**  Author		: 
**  Created on	: 20160621
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL UartRxData(Uart_T uart, SCH_U8 *data, SCH_U16 Len)
{
	return UartGetFromBuf(uart,Uart_Rx,data,Len);
}
/********************************************************************************
**  Function         : UartTxData
**  Author           : 
**  Created on       : 20160621
**  Description      :
**  Return           : BOOL
********************************************************************************/
SCH_BOOL UartTxData(Uart_T uart, SCH_U8 *const data, SCH_U16 Len)
{
	if(UartTxCnt(uart) != 0)
	{
		if(UartPutToBuf(uart,Uart_Tx,data,Len))
			return TRUE;
		return FALSE;
	}
	if(UartPutToBuf(uart,Uart_Tx,data,Len))
	{
		Uart_Tx_DataPro(uart);
		return TRUE;
	}
	return FALSE;
}
SCH_BOOL UartTxData_Direct(Uart_T uart, SCH_U8 *data, SCH_U16 Len)
{
	while(Len--)
	{
		UartSendData8(uart,*data);
		while(USART_GetFlagStatus(Uart_Arry[uart],USART_FLAG_TXE)!=SET);
		data++;
	}
	return TRUE;
}
/********************************************************************************
**  Function    : Uart_Rx_DataPro
**  Author      : 
**  Created on  : 20160621
**  Description	:  ---   int函数中执行
**  Return		: NULL
********************************************************************************/
void Uart_Rx_DataPro(Uart_T uart,SCH_U8 data)
{
	UartPutToBuf(uart,Uart_Rx,&data,1);
}
/********************************************************************************
**  Function    : Uart_Tx_DataPro
**  Author      : 
**  Created on  : 20160621
**  Description :   ---   int函数中执行
**  Return      : NULL
********************************************************************************/
void Uart_Tx_DataPro(Uart_T uart)
{
	SCH_U8 u8data;
	if(UartTxCnt(uart) == 0)
	{
		UartTxIntDis(uart);
		return;
	}
	if(UartGetFromBuf(uart,Uart_Tx,&u8data,1))
	{
		UartSendData8(uart, u8data);
		UartTxIntEn(uart);
	}
}
/********************************************************************************
**  Function    : UART_IntSerive
**  Author      : 
**  Created on  : 20170407
**  Description :  ---   int函数中执行
**  Return      : void
********************************************************************************/
void UART_IntSerive(Uart_T uart)
{
	SCH_U8 data;
	if(USART_GetITStatus(Uart_Arry[uart], USART_IT_LBD) != RESET)
	{
		USART_ClearITPendingBit(Uart_Arry[uart], USART_IT_LBD);
		USART_ITConfig(Uart_Arry[uart], USART_IT_TXE, DISABLE);
	}
	if(USART_GetITStatus(Uart_Arry[uart], USART_IT_RXNE))/* Rx interrupt */
	{
		data = USART_ReceiveData(Uart_Arry[uart]);
		Uart_Rx_DataPro(uart, data);
	}
	if(USART_GetFlagStatus(Uart_Arry[uart], USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(Uart_Arry[uart], USART_FLAG_ORE);
		USART_ReceiveData(Uart_Arry[uart]);
	}
	if(USART_GetITStatus(Uart_Arry[uart], USART_IT_TXE))/* Tx interrupt */
	{
		Uart_Tx_DataPro(uart);
	}
}

/********************************************************************************
**  Function    : SysUartExit
**  Author      : 
**  Created on  : 20170830
**  Description :
**  Return      : BOOL
********************************************************************************/
void SysUartExit(Uart_T uart)
{
	switch(uart)
	{
		case SCH_Uart0:
			break;
		case SCH_Uart1:
			break;
		case SCH_Uart2:
			break;
		case SCH_Uart3:
			break;
		default:break;
	}
}
/********************************************************************************
**  Function    : SysUartInit
**  Author      : 
**  Created on  : 20170406
**  Description :
**  Return      : BOOL
********************************************************************************/
SCH_BOOL SysUartInit(Uart_T uart,SCH_U32 Baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	USART_ClockInitTypeDef	USART_ClockInitStructure;
	switch(uart)
	{
		case SCH_Uart0:
			break;
		case SCH_Uart1:			
#if UART1_FUNC == ENABLE			
			RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			//RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			
		  /* USARTx Pins configuration **************************************************/	
		  /* Connect pin to Periph */
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);	  
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); 

			/*USART1_TX ->PA9  USART1_RX ->PA10*/		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; 	   //	   
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
			GPIO_Init(GPIOA, &GPIO_InitStructure);		 //
		  
			USART_InitStructure.USART_BaudRate = 115200;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No ;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
			USART_Init(USART1, &USART_InitStructure);
			//rTxBuffRdPnt = rTxBuffWrPnt=0;
			//cUARTRxByte =0;
			NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
		//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			USART_Init(USART1, &USART_InitStructure);
			USART_ClearFlag(USART1,USART_FLAG_TC);
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);		 
			USART_Cmd(USART1, ENABLE);


#endif
			break;
		case SCH_Uart2:			
#if UART2_FUNC == ENABLE

#endif
			break;
		case SCH_Uart3:
#if UART3_FUNC == ENABLE

#endif
			break;
		case SCH_Uart4:
#if UART4_FUNC == ENABLE			
			//RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);

			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4, ENABLE);
			
		  /* USARTx Pins configuration **************************************************/	
		  /* Connect pin to Periph */
			GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_0);	  
			GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_0); 

			/*USART1_TX ->PA9  USART1_RX ->PA10*/		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;	   //	   
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
			GPIO_Init(GPIOC, &GPIO_InitStructure);		 //
		  
			USART_InitStructure.USART_BaudRate = 115200;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No ;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
			USART_Init(USART4, &USART_InitStructure);
			//rTxBuffRdPnt = rTxBuffWrPnt=0;
			//cUARTRxByte =0;
			NVIC_InitStructure.NVIC_IRQChannel=USART3_4_IRQn;
		//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			USART_Init(USART4, &USART_InitStructure);
			USART_ClearFlag(USART4,USART_FLAG_TC);
			USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);
			USART_ITConfig(USART4, USART_IT_TXE, ENABLE);		 
			USART_Cmd(USART4, ENABLE);


#endif
			break;
		case SCH_Uart5:
			break;
		default:
			return FALSE;
	}		 
	UartBufInit(uart, Uart_Tx);
	UartBufInit(uart, Uart_Rx);
	return TRUE;
}
///=============================================================
