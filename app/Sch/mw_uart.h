///=========================
#ifndef __MW_UART_H
#define __MW_UART_H


#define UART0_FUNC    DISABLE
#define UART1_FUNC    ENABLE
#define UART2_FUNC    ENABLE
#define UART3_FUNC    ENABLE
#define UART4_FUNC    ENABLE
#define UART5_FUNC    DISABLE

///=================================================

typedef enum
{
	SCH_Uart0,
	SCH_Uart1,
	SCH_Uart2,
	SCH_Uart3,
	SCH_Uart4,
	SCH_Uart5,
	SCH_Uart_Num
}Uart_T;

typedef enum
{
	Uart_Rx,
	Uart_Tx
}Uart_RT;

extern void UartTxIntEn(Uart_T uart);
extern SCH_U16 UartTxCnt(Uart_T uart);
extern SCH_U16 UartRxCnt(Uart_T uart);
extern SCH_BOOL UartTxData(Uart_T uart, SCH_U8 *const data, SCH_U16 Len);
extern SCH_BOOL UartTxData_Direct(Uart_T uart, SCH_U8 *data, SCH_U16 Len);
extern SCH_BOOL UartRxData(Uart_T uart, SCH_U8 *data, SCH_U16 Len);
extern void SysUartExit(Uart_T uart);
extern SCH_BOOL SysUartInit(Uart_T uart,SCH_U32 Baudrate);
extern void Uart_Rx_DataPro(Uart_T uart,SCH_U8 data);
extern void Uart_Tx_DataPro(Uart_T uart);
extern void UART_IntSerive(Uart_T uart);
#endif
