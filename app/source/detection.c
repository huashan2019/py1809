/*
***************************************************************************************
                                     		 Main

                       (c) Copyright Mesada Technology Co., Ltd.
                                 All Rights Reserved
	File		: detection.c
	By			: wanyuedong
	Version	: v1.00
***************************************************************************************
*/

//=========================== Includes ===========================

#include "config.h"



u8 wakeupdetectHas;
u8 wakeupdetectNo;
u8 wakeupFlag;

u8 accdetectTimeNo;
u8 accdetectTimeHas;
u8 accexistFlag;
u8 rBackLightLevel;


u8 backdetectTimeHas;
u8 backdetectTimeNo;
u8 backexistFlag;

u8 brakedetectTimeHas;
u8 brakedetectTimeNo;
u8 brakeexistFlag;

u8 carilldetectTimeHas;
u8 carilldetectTimeNo;
u8 carillexistFlag;

u8 oldbackexistFlag;
u8 oldcarillexistFlag;
u8 oldbrakeexistFlag;

u8 batok;

//=================================================================
//Function Nam: DetectInit()
//Descript:
//=================================================================
void DetectInit(void)
{
	wakeupdetectHas = reset;
	wakeupdetectNo = reset;
	wakeupFlag = reset;

	accdetectTimeNo = reset;
	accdetectTimeHas = reset;
	accexistFlag = reset;
	
	backdetectTimeHas = reset;
	backdetectTimeNo = reset;
	backexistFlag = reset;

	brakedetectTimeHas = reset;
	brakedetectTimeNo = reset;
	brakeexistFlag = reset;
	
	carilldetectTimeHas = reset;
	carilldetectTimeNo = reset;
	carillexistFlag = reset;
	
	oldbackexistFlag = backexistFlag;
	oldcarillexistFlag = carillexistFlag;
	oldbrakeexistFlag = brakeexistFlag;
}
//=================================================================
//Function Nam: ProcessDetect()
//Descript:
//=================================================================
void ProcessDetect(void)
{
	if (!btime10ms)		return;
	if (!core.applink)		return;
	
	if ((oldbackexistFlag != backexistFlag)|| (oldcarillexistFlag != carillexistFlag)
		|| (oldbrakeexistFlag != brakeexistFlag))
	{
		oldbackexistFlag = backexistFlag;
		oldcarillexistFlag = carillexistFlag;
		oldbrakeexistFlag = brakeexistFlag;
		ReportDeviceStatus(1);
	}
}
//=================================================================
//Function Nam: DetectSignal()
//Descript:
//=================================================================
void DetectSignal(void)
{
	DetecAccSignal();
	CoreWakeupDetect();
	DetecBat();
	if ((HasAcc()) && (READ_ACC_PULLON))
	{
		DetecBackSight();
		DetecBrakeSight();
		DetecCarILL();
	}
}
//=================================================================
//Function Nam: HasAcc()
//Descript:
//=================================================================
u8 HasAcc(void)
{
	if (IO_ACC_EXIST)
		return 1;
	return 0;
}
//=================================================================
//Function Nam: CoreWakeupDetect()
//Descript:
//=================================================================
void CoreWakeupDetect(void)
{
	if (btime10ms == reset)				return;
	
	if (ARM_WAKEUP_REP)
	{
		wakeupdetectHas = reset;
		wakeupdetectNo++;
		if (wakeupdetectNo > 100)
		{
			wakeupFlag = reset;
		}
	}
	else
	{
		wakeupdetectNo = reset;
		wakeupdetectHas++;
		if (wakeupdetectHas > 2)
		{
			wakeupFlag = set;
		}
	}
}
//=================================================================
//Function Nam: DetecAccSignal()
//Descript:	ACC SIGNAL
//=================================================================
void DetecAccSignal(void)
{
	if (btime10ms == reset)				return;
#if 0
	accexistFlag = set;
#else
	if (IO_ACC_EXIST)
	{
		accdetectTimeNo = reset;
		accdetectTimeHas++;
		if (accdetectTimeHas > 2)
		{
			accexistFlag = set;
#if	1//_DebugAcc
	//	Printf("\r\n Acc on:%d",core.accStatus);
#endif
		}
	}
	else
	{
		accdetectTimeHas = reset;
		accdetectTimeNo++;
		if (accdetectTimeNo > 20)
		{
			accexistFlag = reset;
#if	1//_DebugAcc
		//Printf("\r\n Acc off");
#endif
		}
	}
#endif
}

//=================================================================
//Function Nam: DetecBackSight()
//Descript:	daoche
//=================================================================
void DetecBackSight(void)
{
	if (btime10ms == reset)				return;

	if (IO_CAM_EXIST || IO_CAN_R_EXIST)
	{
		backdetectTimeNo = reset;
		backdetectTimeHas++;
		if (backdetectTimeHas > 20)
		{
			if (!backexistFlag)
			{
				backexistFlag = set;
				bfastbackEnterack = reset;
				EXT_CAMERA_PWR_ON;
			}
		}
	}
	else
	{
		backdetectTimeHas = reset;
		backdetectTimeNo++;
		if (backdetectTimeNo > 30)
		{
			if (backexistFlag)
			{
				backexistFlag = reset;
				bfastbackExitack = reset;
				EXT_CAMERA_PWR_OFF;
			}
		}
	}
}

//=================================================================
//Function Nam: DetecBrakeSight()
//Descript:	shache
//=================================================================
void DetecBrakeSight(void)
{
	if (btime10ms == reset)				return;

	if (IO_PARK_EXIST)
	{
		brakedetectTimeNo= reset;
		brakedetectTimeHas++;
		if (brakedetectTimeHas > 20)
		{
			if (!brakeexistFlag)
			{
				brakeexistFlag = set;
			}
		}
	}
	else
	{
		brakedetectTimeHas = reset;
		brakedetectTimeNo++;
		if (brakedetectTimeNo > 30)
		{
			if (brakeexistFlag)
			{
				brakeexistFlag = reset;
			}
		}
	}

}
//=================================================================
//Function Nam: DetecCarILL()
//Descript:	xiaodeng yibiao zhaoming
//=================================================================
void DetecCarILL(void)
{
	if (btime10ms == reset)				return;

	if (IO_ILL_EXIST)
	{
		carilldetectTimeNo= reset;
		carilldetectTimeHas++;
		if (carilldetectTimeHas > 20)
		{
			if (!carillexistFlag)
			{
				carillexistFlag = set;
			}
		}
	}
	else
	{
		carilldetectTimeHas = reset;
		carilldetectTimeNo++;
		if (carilldetectTimeNo > 30)
		{
			if (carillexistFlag)
			{
				carillexistFlag = reset;
			}
		}
	}
}

void DetecBat(void)
{
	static u8 gkeycount;
	static u8 gkeyerrorflag;
	
	u8 gkey;

	if (!bkey10ms)				return;
#if 1
	gkey = adcvalueBuff[3] >> 2;
	if(gkey < BAT_6P5V)
	{
		if (IFArmpowOn)
		{
			MuteOn();
			TFT_OFF;
		}
		bBatLowOn = 0;
		rAccSt = 0x02;
		core.accStatus = ACC_ERROR;
		CoreInit();
		batok = reset;
#if	1//_DebugAcc
	Printf("\r\n Bat Low:%d,%d",core.accStatus,adcvalueBuff[3]);
#endif
	}
	else if (gkey < BAT_9V)/*9v*/
	{
		if (IFArmpowOn)
		{
			MuteOn();
			TFT_OFF;
		}
		
		bBatLowOn = 1;
		//core.accStatus = ACC_ERROR;
		batok = set;
#if	1//_DebugAcc
	Printf("\r\n Bat on:%d,%d",core.accStatus,adcvalueBuff[3]);
#endif
	}
	else if (gkey > BAT_16V)/*16v*/
	{
		bBatLowOn = 0;
		batok = reset;
	}
	else if (gkey > BAT_9P5V)/*9.5v*/
	{
	
		batok = set;
#if	1//_DebugAcc
	if(bBatLowOn)
	{
		//Printf("\r\n Bat ok:%d,%d",core.accStatus,adcvalueBuff[3]);
		TFT_ON;
		MuteTimeset(200);
		bBatLowOn = 0;
	}
#endif
	}
#else
	batok = set;
#endif
}





