/*
***************************************************************************************************
**  Copyright   : 
**  Project     : STM32F1
**  File        : app_can.c  ---   CAN
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "config.h"

SCH_U8 AAC_DataRx[13];
SCH_U8 AAC_DataTx[13];
//=================================================================
//Function Nam: ReportCANData()
//Descript:
//=================================================================
void ReportAirHeatData(SCH_U8 type,SCH_U8 numL,SCH_U8 numR)
{
	u8 transmitbuff[20],check,i;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x81; check ^= transmitbuff[1];
	transmitbuff[2] = 0xD1; check ^= transmitbuff[2];
	transmitbuff[3] = 0x09+5; check ^= transmitbuff[3];
	transmitbuff[4] = type; check ^= transmitbuff[4];
	if(type == 1)	/*air heat*/
	{
		transmitbuff[5] = numL; check ^= transmitbuff[5];
		transmitbuff[6] = numR; check ^= transmitbuff[6];
		for(i=0;i<11;i++)
		{
			transmitbuff[7+i] = AAC_DataRx[i];
			check ^= transmitbuff[7+i];
		}
		
		transmitbuff[6+i] = check;
	}
	else
	{
		for(i=0;i<13;i++)
		{
			transmitbuff[5+i] = AAC_DataRx[i];
			check ^= transmitbuff[5+i];
		}
		transmitbuff[5+i] = check;
	}


	ArmUartSenstr(transmitbuff,5+14);
}

void ReportCANData(SCH_U8 num)
{
	u8 transmitbuff[20],check,i;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x81; check ^= transmitbuff[1];
	transmitbuff[2] = 0xD1; check ^= transmitbuff[2];
	transmitbuff[3] = 0x09+5; check ^= transmitbuff[3];
	transmitbuff[4] = num; check ^= transmitbuff[4];
	if(num == 1)	/*air heat*/
	{
		transmitbuff[5] = 3; check ^= transmitbuff[5];
		transmitbuff[6] = 3; check ^= transmitbuff[6];
		for(i=0;i<11;i++)
		{
			transmitbuff[7+i] = AAC_DataRx[i];
			check ^= transmitbuff[7+i];
		}
		
		transmitbuff[6+i] = check;
	}
	else
	{
		for(i=0;i<13;i++)
		{
			transmitbuff[5+i] = AAC_DataRx[i];
			check ^= transmitbuff[5+i];
		}
		transmitbuff[5+i] = check;
	}


	ArmUartSenstr(transmitbuff,5+14);
}


void CanDataRxPro(void)
{
	Can_Data_T Can_Data={0};
	while(CanRxCnt(SCH_Can0))
	{
		CanRxData(SCH_Can0, &Can_Data, 1);
		switch(Can_Data.ExtId)
		{
			case 0x00001314:////测试使用  无意义
				///Printf("CanData ExtId=%x data=%x %x %x %x %x %x %x %x \n",Can_Data.ExtId,Can_Data.Data[0],Can_Data.Data[1],Can_Data.Data[2],Can_Data.Data[3],Can_Data.Data[4],Can_Data.Data[5],Can_Data.Data[6],Can_Data.Data[7]);
				//PostMessage(CAN_MODULE,M2C_TEST,NONE);

				AAC_DataRx[3] = Can_Data.ExtId>>24;
				AAC_DataRx[2] = Can_Data.ExtId>>16;
				AAC_DataRx[1] = Can_Data.ExtId>>8;
				AAC_DataRx[0] = Can_Data.ExtId>>0;
				AAC_DataRx[4] = Can_Data.IDE;
				sch_memcpy(&AAC_DataRx[5],Can_Data.Data,8);
				ReportCANData(0);
				break;

			case 0x1884D3D0:
			case 0x1885D3D0:
				AAC_DataRx[3] = Can_Data.ExtId>>24;
				AAC_DataRx[2] = Can_Data.ExtId>>16;
				AAC_DataRx[1] = Can_Data.ExtId>>8;
				AAC_DataRx[0] = Can_Data.ExtId>>0;
				AAC_DataRx[4] = Can_Data.IDE;
				sch_memcpy(&AAC_DataRx[5],Can_Data.Data,8);
				ReportCANData(0);
				break;

			default:
				break;
		}

	}
}


void M2C_TxService(void)
{
	MESSAGE pMsg;
	CanTxMsg TxMessage={0};
	if(FALSE==GetMessage(CAN_MODULE,&pMsg))
		return;
	switch(pMsg.ID)
	{
		case M2C_TEST:////测试使用  无意义
			TxMessage.StdId = 0x0000;
			TxMessage.ExtId = 0x00001413;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.DLC = 8;
			sch_memset(TxMessage.Data,0x00,8);
			CAN_Transmit(CAN, &TxMessage);
			break;
		case M2C_AAC:///空调设置信息
			TxMessage.StdId = 0x0210;
			TxMessage.ExtId = 0x00000000;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.DLC = 8;
			sch_memcpy(TxMessage.Data,AAC_DataTx,8);
			CAN_Transmit(CAN, &TxMessage);
			break;
		case M2C_DATA:
			if((AAC_DataTx[3]<<24|AAC_DataTx[2]<<16|AAC_DataTx[1]<<8|AAC_DataTx[0]) == 0xffffffff)
			{
				TxMessage.StdId = 0x0210;//AAC_DataTx[3]<<24|AAC_DataTx[2]<<16|AAC_DataTx[1]<<8|AAC_DataTx[0];
				TxMessage.ExtId = 0x0210;//AAC_DataTx[3]<<24|AAC_DataTx[2]<<16|AAC_DataTx[1]<<8|AAC_DataTx[0];
				TxMessage.RTR = CAN_RTR_REMOTE;
				TxMessage.IDE = CAN_ID_STD;
				TxMessage.DLC = 0;
			}
			else if(AAC_DataTx[4] == CAN_ID_STD)
			{
				TxMessage.StdId = AAC_DataTx[3]<<24|AAC_DataTx[2]<<16|AAC_DataTx[1]<<8|AAC_DataTx[0];
				TxMessage.ExtId = 0x00000000;
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.IDE = CAN_ID_STD;
				TxMessage.DLC = 8;
			}
			else
			{
				TxMessage.StdId = 0x0000;
				TxMessage.ExtId = AAC_DataTx[3]<<24|AAC_DataTx[2]<<16|AAC_DataTx[1]<<8|AAC_DataTx[0];
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.IDE = CAN_ID_EXT;
				TxMessage.DLC = 8;
			}
			sch_memcpy(TxMessage.Data,&AAC_DataTx[5],8);
			CAN_Transmit(CAN, &TxMessage);
			break;
			
		case M2C_TOUCH_XY:
		case M2C_YMD_TIME:
			//sch_memcpy(TxRemoteBuff,&AAC_DataTx[0],4);
			//rRmtSt = _Rmtstart;
			//TIM_Cmd(TIM2, ENABLE); 	//使能TIMx外设	
			break;
			
		default:break;
	}
}

void TASK_Can_Pro(void)
{
	M2C_TxService();
}




