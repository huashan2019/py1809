
#include "config.h"




u8 usart1SendData[USART1_SEND_MAX];
u16 usart1SendSave;
u16 usart1SendIndex;
u8 usart1Sendbusy;

u8 usart1Receivebuff[USART1_RECEIVE_MAX];
u16 usart1Receivesave;
u16 usart1Receiveget;

u8 lincommandbuff[200];

volatile u8 bCanTimeour;
volatile u8 bCan4Timeour;

u32 canbaudRate;


void VMCanInit(void)
{
	usart1SendSave = reset;
	usart1SendIndex = reset;
	usart1Sendbusy = reset;
	usart1Receivesave = reset;
	usart1Receiveget = reset;
}

void ProcessVMCan(void)
{
	u8 i,receiveindex;
	u8 flag,receivelen,check;
	u16 len;
	u8 index;
	
	if (usart1Receivesave == usart1Receiveget)	
	{
		bCanTimeour = 0;
		return;
	}
	if (usart1Receivesave > usart1Receiveget)		len = (u16)(usart1Receivesave - usart1Receiveget);
	else										len = (u16)(USART1_RECEIVE_MAX - usart1Receiveget + usart1Receivesave);

	if ((len > 100) || ((bCanTimeour > 10) && (len)))
	{
		bCanTimeour = reset;
		if (len > 100)
			len = 100;
		ReadArmCanStr(len);
	}
}
void ReadArmCanStr(u8 len)
{
	u8 i,check,index;
	
	check = reset;
	index = reset;
	lincommandbuff[index++] = 0x02;
	lincommandbuff[index] = 0x81;				check ^= lincommandbuff[index++];
	lincommandbuff[index] = ARM_CMD_CANBUS; 	check ^= lincommandbuff[index++];
	lincommandbuff[index] = len;				check ^= lincommandbuff[index++];
	
	for(i = 0; i < len; i++)
	{
		lincommandbuff[index] = usart1Receivebuff[(u8)(usart1Receiveget++)];
		usart1Receiveget &= USART1_RECEIVE_SUB;
		check ^= lincommandbuff[(u8)(index++)];
	}
	lincommandbuff[(u8)(index++)] = check;
	Uart2SendStr(lincommandbuff,index);
}

void Uart1SendStr(u8 *p, u16 len)
{
	u8 i;

	for (i = 0; i < len; i++)
	{
		usart1SendData[usart1SendSave++] = *p++;
		usart1SendSave &= USART1_SEND_SUB;
	}

	if (!usart1Sendbusy)
	{
		USART1->CR1 |= 0x00000080;
	}
}
void SetLinUartBaud(u32 baudRate)
{
	u16 baud;
	u8 dummy;
	u32 tempdiv;
	
	DI;
	tempdiv = (u32)(160000000/baudRate);
	dummy = (u8)(tempdiv % 10);
	if (dummy > 5)		dummy = 1;
	else				dummy = 0;
	
	USART1->CR1 = 0x0000002C;
	USART1->BRR = (u16)(16000000/baudRate) + dummy;	// 16MHZ/57600 = 0x116;
	USART1->CR1 |= 0x00000001;
	usart1SendSave = reset;
	usart1SendIndex = reset;
	usart1Sendbusy = reset;
	EI;
}


