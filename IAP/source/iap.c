/**
  ******************************************************************************
  * @file		iap.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.04.02
  * @brief   
*/



#include "config.h"




u8 iapSaveindex;
u8 iapGetindex;
u8 iapReadbuff[200];
u8 iapWritebuff[200];
u8 iapWriteindex;


void IapInit(void)
{

}

void IapRun(void)
{
	u8 iapStatus,tempindex;
	u16 tempData;
	u32 flashAddr;

	FLASH_if_Init();
	Uart2Init();
	ReportIapNext(0x01);
	iapStatus = IAP_STATUS_START;
	iapSaveindex = reset;
	iapWriteindex = reset;
	iapGetindex = reset;
	flashAddr = FLASH_APP_START_ADDR;

	while(iapStatus)
	{
		while(IapRead() == FALSE);

		if (iapReadbuff[2] == 0xC6)
		{
			iapStatus = IAP_STATUS_UPDATE;
		}
		else if (iapReadbuff[2] == 0xC4)
		{
			iapStatus = IAP_STATUS_END;
			if (iapReadbuff[4] == 0x01)
				iapStatus = IAP_STATUS_START;
		}
		else if (iapReadbuff[2] == 0xC5)
		{
			iapStatus = IAP_STATUS_IDLE;
		}
		
		switch(iapStatus)
		{
			case IAP_STATUS_START:
				ReportIapNext(0x01);
				break;
			case IAP_STATUS_UPDATE:
				CopyIapdata();
				tempindex = reset;
				if (iapWriteindex)
				{
					do{
						tempData = (u16)(((u16)iapWritebuff[tempindex+1] << 8) + iapWritebuff[tempindex]);
						FLASH_if_Writehalfword(flashAddr,tempData);
						flashAddr += 2;
						tempindex += 2;
					}while(tempindex < iapWriteindex);
					iapWriteindex = reset;
				}
				ReportIapNext(0x01);
				break;
			case IAP_STATUS_END:
				iapStatus = reset;
				ReportIapNext(0x01);
				break;
			default:
				iapSaveindex = reset;
				iapGetindex = reset;
				iapStatus = IAP_STATUS_IDLE;
				break;
		}

		iapSaveindex = reset;
		iapGetindex = reset;
		//usart2Receivesave = reset;
		//usart2Receiveget = reset;
	}

}
bool IapRead(void)
{
	u16 i,receiveindex,len,receivelen;
	u8 temp,check;

	if (usart2Receivesave == usart2Receiveget)		return FALSE;
	if (usart2Receivesave > usart2Receiveget)		len = (u16)(usart2Receivesave - usart2Receiveget);
	else										len = (u16)(USART2_RECEIVE_MAX - usart2Receiveget + usart2Receivesave);
	
	if (len < 4)								return FALSE;

	receiveindex = usart2Receiveget;
	if (usart2Receivebuff[receiveindex] == 0x02)
	{
		receiveindex = (u16)(usart2Receiveget + 1) & USART2_RECEIVE_SUB;
		if ((usart2Receivebuff[receiveindex] == 0x80) || (usart2Receivebuff[receiveindex] == 0x81))
		{
			receiveindex = (u16)(usart2Receiveget + 3) & USART2_RECEIVE_SUB;
			receivelen = (u16)(usart2Receivebuff[receiveindex]+5) & USART2_RECEIVE_SUB;

			if (receivelen > 135)
			{
				usart2Receiveget++;
				usart2Receiveget &= USART2_RECEIVE_SUB;
				return FALSE;
			}
			if (len < receivelen)
			{
				return FALSE;
			}
			else
			{
				check = reset;

				receiveindex = usart2Receiveget;
				for(i = 0; i < receivelen; i++)
				{
					temp = usart2Receivebuff[receiveindex];
					iapReadbuff[i] = temp;
					check ^= temp;
					receiveindex++;
					receiveindex &= USART2_RECEIVE_SUB;
				}
				if (check == 0x02)
				{
					usart2Receiveget += receivelen;
					usart2Receiveget &= USART2_RECEIVE_SUB;
					return TRUE;
				}
				else
				{
					usart2Receiveget++;
					usart2Receiveget &= USART2_RECEIVE_SUB;
					return FALSE;
				}
			}
		}
		else
		{
			usart2Receiveget++;
			usart2Receiveget &= USART2_RECEIVE_SUB;
			return FALSE;
		}
	}
	else
	{
		usart2Receiveget++;
		usart2Receiveget &= USART2_RECEIVE_SUB;
		return FALSE;
	}
}
//=================================================================
//Function Nam: CopyIapdata()
//Descript:
//=================================================================
void CopyIapdata(void)
{
	u8 len,i;

	len = iapReadbuff[3];
	if (len > 150)			return;

	iapWriteindex = reset;
	for (i = 0; i < len; i++)
	{
		iapWritebuff[i] = iapReadbuff[4+i];
		iapWriteindex++;
	}
}

void SysDelay(u32 delay)
{
	u32 i,j;

	for (i = 0; i < delay; i++)
	{
		for (j = 0; j < 2000; j++);
	}
}
//=================================================================
//Function Nam: ReportIapNext()
//Descript:
//=================================================================
void ReportIapNext(u8 flag)
{
	u8 transmitbuff[20],check,index;

	check = reset;
	index = reset;
	transmitbuff[index++] = 0x02; 
	transmitbuff[index] = 0x81; check ^= transmitbuff[index++];
	transmitbuff[index] = 0xC5; check ^= transmitbuff[index++];
	transmitbuff[index] = 0x01; check ^= transmitbuff[index++];
	transmitbuff[index] = flag;		//next
	check ^= transmitbuff[index++];

	transmitbuff[index++] = check;
	
	Uart2SendStr(transmitbuff,index);
}

