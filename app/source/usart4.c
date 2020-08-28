
#include "config.h"




u8 usart4Sendbuff[USART4_SEND_MAX];
u16 usart4SendSave;
u16 usart4SendIndex;
u8 usart4Sendbusy;

u8 usart4Receivebuff[USART4_RECEIVE_MAX];
u16 usart4Receivesave;
u16 usart4Receiveget;

u8 Cancommandbuff[100];

u8 Cancommandbuff_bak[255];
u16 CancommandReceiveget;

u8 IO_CAN_R_EXIST;
u8 IO_CAN_R_EXIST_Timer;



void VM4CanInit(void)
{
	usart4SendSave = reset;
	usart4SendIndex = reset;
	usart4Sendbusy = reset;
	usart4Receivesave = reset;
	usart4Receiveget = reset;
}

void ProcessVM4Can(void)
{
	u8 i,receiveindex;
	u8 flag,receivelen,check;
	u16 len=0;
	u8 index;
	
	if (usart4Receivesave == usart4Receiveget)	
		{
			bCan4Timeour = reset;
			return;
		}
	if (usart4Receivesave > usart4Receiveget)	len = (u16)(usart4Receivesave - usart4Receiveget);
	else	len = (u16)(USART4_RECEIVE_MAX - usart4Receiveget + usart4Receivesave);
	if (len > 50 || ((bCan4Timeour >= 4) && (len)))
	{
#if _UartDebug
		Printf("\r\n rx:len= %d,%d,%d,tim= %d \r\n",len,usart4Receivesave,usart4Receiveget,bCan4Timeour);
#endif

		bCan4Timeour = reset;
		if (len > 50)	len = 50;
		ReadVM4ArmCanStr(len);
	}
}

void AnalysisCanStr(u8 *p, u16 len)
{
	u8 i,temp[11];
	for(i = 0;i<len;i++)
		temp[i] = *(p++);
	//if(temp[0] != 0x03 || temp[1] != 0x01 && temp[1] != 0x03 || temp[4] != 0x30)/*juge head,tail*/
	//{
	//	return;
	//}
	if(temp[0] == 0x03 && temp[1] == 0x03 || temp[0] == 0x03 && temp[1] == 0x01)
	{
		if((temp[2] & 0x30)  == 0x10)	
		{
			IO_CAN_R_EXIST = 1;
			//IO_CAN_R_EXIST_Timer = 1200/30;
		}
		else //if((temp[2] & 0x30)	== 0x00 || (temp[2] & 0x30)  == 0x30) 
		IO_CAN_R_EXIST = 0;

	}

}

void ReadVM4ArmCanStr(u8 len)
{
	u8 i,check,index;
	
	check = reset;
	index = reset;
	Cancommandbuff[index++] = 0x02;
	Cancommandbuff[index] = 0x81;				check ^= Cancommandbuff[index++];
	Cancommandbuff[index] = ARM_CMD_CANBUS; 	check ^= Cancommandbuff[index++];
	Cancommandbuff[index] = len;				check ^= Cancommandbuff[index++];
	
#if _UartDebug
	Printf(" tx:");
#endif
	for(i = 0; i < len; i++)
	{
		Cancommandbuff[index] = usart4Receivebuff[(u8)(usart4Receiveget++)];
		
#if _UartDebug
#if 0
		Cancommandbuff_bak[CancommandReceiveget] = Cancommandbuff[index];
		Printf(" %d ",Cancommandbuff_bak[CancommandReceiveget]);
		if(CancommandReceiveget && Cancommandbuff_bak[CancommandReceiveget] - Cancommandbuff_bak[CancommandReceiveget-1] != 1)
		{
			if(Cancommandbuff_bak[CancommandReceiveget-1] - Cancommandbuff_bak[CancommandReceiveget] != 0xff)
			Printf("err........................................... \r\n");
		}
		else if(CancommandReceiveget == 0 && (Cancommandbuff_bak[255]-Cancommandbuff_bak[0] != 255))
			Printf("err........................................... \r\n");
		if(CancommandReceiveget++ >= 255 ) CancommandReceiveget = 0;
		if(CancommandReceiveget%10 == 0) Printf(" \r\n");
#endif
#endif
		usart4Receiveget &= USART4_RECEIVE_SUB;
		check ^= Cancommandbuff[(u8)(index++)];
	}
	Cancommandbuff[(u8)(index++)] = check;
	
	
	AnalysisCanStr(&Cancommandbuff[4],5);
	
	Uart2SendStr(Cancommandbuff,index);
}

void Uart4SendStr(u8 *p, u16 len)
{
	u8 i;

	for (i = 0; i < len; i++)
	{
		usart4Sendbuff[usart4SendSave++] = *p++;
		usart4SendSave &= USART4_SEND_SUB;
	}

	if (!usart4Sendbusy)
	{
		USART4->CR1 |= 0x00000080;
	}
}


