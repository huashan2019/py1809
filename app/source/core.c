
#include "config.h"

#if _UartDebug
	#define _DebugAcc	1
#endif

#define AD_SW2_MAX_NUM      17

extern 	unsigned char SeekKeyTimer;
extern KEY_AD_DATA AD_SW_Tab[AD_SW_MAX_NUM];
extern KEY_AD_DATA AD_SW_Tab2[AD_SW_MAX_NUM];


SYSStuct System;

CORE_TYPE core;

u8 bwakeupfor;

COREKEYBUFF armkey;
TYPE_BYTE fastback;
TYPE_BYTE sysflag0;

u8		rAreaSt;

u8 armsetpowerorsleepFlag;
u8 armsetsleeptime;

u8 bfirstbat;
u8 btracedata;

u16 bfastarm2updatetimeout;
u8 radioinfoState1;
u8 radioinfoState2;
u8 rdsinfoState3;

void CoreInit(void)
{
	core.applink = reset;
	core.oslink = reset;
	core.gcoreDevice = reset;
	core.acctime = 20;
	core.accStatus = ACC_FIRST_START;
	core.powerStatus = POWER_ON;
	core.bcanreceive = reset;
	bfirstbat = reset;
	bfastarm2updatetimeout = 50;
}
void ProcessCore(void)
{
	CoreAccProcess();
	CorePowerProcess();
	CoreTime();
	ProcessArmkey();
}



void CorePowerProcess(void)
{
	if(/*core.accStatus != ACC_RUN*/!accexistFlag && core.powerStatus != POWER_ACC_OFF)
	{
		core.powerStatus = POWER_ACC_OFF;//power status change to ACC off status	
	}
	switch(core.powerStatus)
	{
		case POWER_OFF:
			if(bPwr2KyPsd)
			{
				bPwr2KyPsd = 0;
				bpoweronFlag = 1;
				core.powertime = 30;
				core.powerStatus++;
#if	_DebugAcc
			Printf("\r\n Pwr on dly:%d ",core.accStatus);
#endif
			}
			break;
		case POWER_UP_DELAY:
			if(!core.powertime)
			{
				if (externAMP)
				{
					EX_AMP_OPEN;
				}
				if ((externANT) && (forRadioAreaMute))
				{
					EX_ANT_OPEN;
				}
				bwaitpowerkey = reset;
				core.powerStatus++;
				TFT_ON;
				MuteTimeset(20);
				ReportMcuPowerinfo();
#if	_DebugAcc
			Printf("\r\n Pwr on:%d ",core.accStatus);
#endif
			}
			break;
		case POWER_ON:
			if(bPwr2KyPsd)
			{
				bPwr2KyPsd = 0;
				bpoweronFlag = 0;
				TFT_OFF;
				MuteOn();
				core.powertime = 30;
				core.powerStatus++;
#if	_DebugAcc
			Printf("\r\n Pwr dn dly:%d ",core.accStatus);
#endif
			}
			break;
		case POWER_DN_DELAY:
			if(!core.powertime)
			{
				EX_AMP_CLOSE;
				EX_ANT_CLOSE;
				bwaitpowerkey = set;
				core.powerStatus = POWER_OFF;
				
				SetRadPwrOff();
				ReportMcuPowerinfo();
#if	_DebugAcc
			Printf("\r\n Pwr off:%d ",core.accStatus);
#endif
			}
			break;
		case POWER_ACC_OFF:
			if(core.accStatus == ACC_RUN)
			{
			
#if	_DebugAcc
			Printf("\r\n Pwr on acc on:%d ",core.powerStatus);
#endif
				if(bpoweronFlag)
					core.powerStatus = POWER_UP_DELAY;
				else
					core.powerStatus = POWER_OFF;
			}
				
			break;
	}

}






void CoreAccProcess(void)
{
	switch(core.accStatus)
	{
		case ACC_IDLE:
			break;
		case ACC_FIRST_START:
			if (core.acctime)	break;
			if (!accexistFlag)
			{
				core.accStatus = ACC_SLEEP_WAIT;
				core.acctime = reset;
				break;
			}
			if (!batok)	break;
			SystemPowerOn();
			SysDelay(200);
			SetArmRstHi();
			
			Bsp_UART_Init();
			Uart2Init();
			//Uart1Init();			
#if	_DebugAcc
			Printf("\r\n Acc start:%d",core.powerStatus);
#endif
			core.acctime = WAKEUP_TIMEOUT *4;
			core.accStatus = ACC_RUN_START;
			core.bcanreceive = reset;
			break;
		case ACC_RUN_START:
			CoreFastBack();
			CoreFastPanel();
			CoreFastArm2update();
			
			if (core.applink)
			{
				if (accexistFlag && bpoweronFlag)
				{
					//TFT_ON;
				}
				core.accStatus = ACC_RUN;
				
				//ChangeFWorkmode(WORKMODE_RADIO);
				break;
			}
			if (core.oslink)
			{
				if  ((!accexistFlag) || (!batok))
				{
					TFT_OFF;
				}
				else
				{
					if (bfastpanelbacklight)
					{
						TFT_ON;
					}
				}
			}
			if ((!core.acctime) && (!accexistFlag && batok) || (!batok))
			{
				if(!batok)
					core.accStatus = ACC_FIRST_START;
				else
					core.accStatus = ACC_SLEEP_WAIT;
			}
			break;
		case ACC_RUN:
			if ((!accexistFlag) || (!batok))
			{
				TFT_OFF;
				bBatLowOn = 0;
				core.accStatus = ACC_ACCWAIT;
				core.acctime = WAKEUP_TIMEOUT;
				if (armsetpowerorsleepFlag)
				{
					core.acctime = armsetsleeptime*10;
				}
				if (!wakeupFlag)
				{
					core.acctime = 500;
				}
				
#if	_DebugAcc
			Printf("\r\n Acc dn:%d",core.powerStatus);
#endif
				MuteOn();
				ReportAccState();
			}
			break;
		case ACC_ACCWAIT:
			if ((accexistFlag) && (batok))
			{
				if (bpoweronFlag || core.powerStatus == POWER_ON)
				{
					//TFT_ON;
				}
				ReportAccState();
				MuteTimeset(200);
				core.accStatus = ACC_RUN;
#if	_DebugAcc
			Printf("\r\n Acc on dly:%d ",core.powerStatus);
#endif
				break;
			}
			if (!core.acctime)
			{
				TFT_OFF;
				if (core.powerStatus == POWER_ON)
				{	
					//CoreTranserOpenBT();
					//SysDelay(2000);
					SystemSleepWake();
				}
				core.acctime = WAKEUP_TIMEOUT;
				core.accStatus = ACC_SLEEP_WAIT;
#if	_DebugAcc
			Printf("\r\n Acc sleep dly:%d ",core.powerStatus);
#endif
			}
			break;
		case ACC_SLEEP_WAIT:
			if ((!wakeupFlag) || (!core.acctime))
			{
				TFT_OFF;
				SystemPowerOff();
				SetRadPwrOff();
				SysDelay(1000);
				MUTE_CTL_OFF;
#if	_DebugAcc
			Printf("\r\n Acc sleep:%d ",core.powerStatus);
#endif
				core.accStatus = ACC_SLEEP;
				break;
			}
			break;
		case ACC_SLEEP:
			McuPowerDown();
			while(1)
			{
				if (btime1s)
				{
					ProcessDate();
					btime1s = reset;
				}
				if (IO_ACC_EXIST)
					break;
			}
			DI;
			bwakeupfor = reset;
			MuteOn();
			SysDelay(16);
			ResetExternDevice();
			EI;
			core.bcanreceive = reset;
			core.acctime = 8;
			core.accStatus = ACC_FIRST_START;
			break;
		case ACC_ERROR:
			TFT_OFF;
			MuteOn();
			if(batok/* && accexistFlag*/)
			{
				core.acctime = 100;			
				core.accStatus = ACC_LOWV_START;
#if	_DebugAcc
			Printf("\r\n Low start");
#endif
			}
			else
			{
#if	_DebugAcc
			Printf("\r\n Reset");
#endif
				bBatLowOn = 0;
				core.acctime = 30;
				rAccSt = 0x02;		
				bfirstbat = reset;
				core.accStatus = ACC_FIRST_START;
			}
			break;

		case ACC_LOWV_START:
			if (core.acctime)	break;
			if(batok)
			{
			
#if	_DebugAcc
			Printf("\r\n Acc start");
#endif
				core.acctime = WAKEUP_TIMEOUT ;
				core.accStatus = ACC_RUN_START;
			}
			else
			{
#if	_DebugAcc
			Printf("\r\n Acc wait");
#endif
				rAccSt = 0x02;
				bfirstbat = reset;
				core.acctime = 20;
				core.accStatus = ACC_FIRST_START;
			}
			break;
			
		default:
			core.acctime = 8;
			core.accStatus = ACC_FIRST_START;
			break;
	}
}
void ProcessArmkey(void)
{
	u8 tempindex;
	u16 tempfreq;
//	KeyCode=NOKEY;
	bNewKey = 0;
	if (armkey.flag == reset)		return;

	armkey.flag = reset;
	

	switch(armkey.keybuff[0])
	{	
		case ARM_CKEY_POWER:
			bPwr2KyPsd = 1;
			break;
			
		case ARM_CKEY_RADIO_SAVE:
			rKeyResult = _kcWrM1+armkey.keybuff[1]-1;
			bNewKey = 1;
			break;
			
		case ARM_CKEY_RADIO_LOAD:
			rKeyResult = _kcRdM1+armkey.keybuff[1]-1;
			bNewKey = 1;
			break;
			
		case ARM_CKEY_RADIO_BAND:
			rKeyResult = _kcBand;
			bNewKey = 1;
			break;
			
		case ARM_CKEY_RADIO_TUNEDOWN:
		case ARM_CKEY_RADIO_TUNEUP:
			if (armkey.keybuff[0] == ARM_CKEY_RADIO_TUNEUP)
				rKeyResult = _kcTuneInc;
			else 
				rKeyResult = _kcTuneDec;
			
			bNewKey = 1;
			break;
		case ARM_CKEY_RADIO_LOCDX:

			#ifdef _RDS
			//if (RDSData.EWS)
			//	break;
			#endif
			if (Radio_7786.F_LocDx == reset)
			{
				bLOCEn = Radio_7786.F_LocDx = set;
				
				radioinfoState1 &= (u8)(~BIT7);
			}
			else
			{
				bLOCEn = Radio_7786.F_LocDx = reset;
				
				radioinfoState1 |= BIT7;
			}
			ReportRadioBaseInfo();
			break;
		case ARM_CKEY_RADIO_AREA:
			if(Radio_7786.Area == armkey.keybuff[1])
			MuteTimeset(20);
			//rAreaSt = Radio_7786.Area = _USA;

			Radio_7786.Area = armkey.keybuff[1];
			if (armkey.keybuff[1] == CKEY_RADIO_AREA_USA1)
			{
				Radio_7786.Area = _USA;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_LATIN)
			{
				Radio_7786.Area = _Latin;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_EUROPE)
			{
				Radio_7786.Area = _Europe1;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_OIRT)
			{
				Radio_7786.Area = AREA_OIRT;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_CHINA)
			{
				Radio_7786.Area = _Asia;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_JAPAN)
			{
				Radio_7786.Area = AREA_JAPAN;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_MIDEAST)
			{
				Radio_7786.Area = _MidEast;
			}
			else if (armkey.keybuff[1] == CKEY_RADIO_AREA_RUSSIAN)
			{
				Radio_7786.Area = _Russia;
			}
			else
				Radio_7786.Area  = AREA_EUROPE;
			if(rAreaSt != Radio_7786.Area)
			{
	            rAreaSt = Radio_7786.Area;
	            MuteTimeset(100);
				RadioInit();
				ReportRadioPreListinfo();
			}
			break;
		case ARM_CKEY_RADIO_SEEKUP:
		case ARM_CKEY_RADIO_SEEKDOWN:
			if (armkey.keybuff[0] == ARM_CKEY_RADIO_SEEKUP)
				rKeyResult = _kcSeekInc1;
			else 
				rKeyResult = _kcSeekDec1;
			bNewKey = 1;			
			break;

		case ARM_CKEY_RADIO_PREV:
		case ARM_CKEY_RADIO_NEXT:
			if (armkey.keybuff[0] == ARM_CKEY_RADIO_NEXT)
				rKeyResult = _kcSeekInc;
			else 
				rKeyResult = _kcSeekDec;
			bNewKey = 1;	
			break;
			
		case ARM_CKEY_RADIO_PS:
			rKeyResult = _kcPS;
			bNewKey = 1;			
			break;
		case ARM_CKEY_RADIO_AS:
			rKeyResult = _kcAms;
			bNewKey = 1;			
			break;
			
		case ARM_CKEY_RADIO_STERO:			
			#if(!TDA7786_TUNER)
//			radioinfoState1 &= (u8)(~BIT4);
//			if (Radio.band < 3)
//				RadioMonoSteroset();
			#else
//				radioinfoState1 &= (u8)(~BIT4);
//				if(FreqInFM(Radio_7786.Freq))
//				HIT_ForeceMono();
			#endif
			rKeyResult = _kcST;
			bNewKey = 1;	
			break;
		case ARM_CKEY_MENU_EXIT:
			break;
		case ARM_CKEY_BLACKOUT:
			rBackLightLevel = armkey.keybuff[2];
			if (armkey.keybuff[1] == reset)
			{
				TFT_OFF;
			}
			else
			{
				if  ((accexistFlag) && (batok) && (core.accStatus == ACC_RUN))
				{
					TFT_ON;
				}
			}
			break;
		case ARM_CKEY_BEEP:
			break;
		case ARM_CKEY_SWC_PRESSINDEX:
			if(GetKeyNum((UICC_KEY_CODE)armkey.keybuff[1],&AD_SW2_Info.StudyNum,AD_SW2_MAX_NUM,AD_SW_Tab2))
			{
				AD_SW2_Info.StudyState = STUDY_ING;
				AD_SW_Info.StudyKeyCode = AD_SW2_Info.StudyKeyCode = armkey.keybuff[1];
			}
			break;
		case ARM_CKEY_SYS_RESET:
			
			if (armkey.keybuff[1] == 0xff)
			ReportAccinfo();
			else if (core.accStatus == ACC_RUN)
			{
				core.gcoreDevice = reset;
				core.applink = reset;
				core.oslink = reset;
				MuteOn();
				TFT_OFF;
				core.acctime = WAKEUP_TIMEOUT*2;
				core.accStatus = ACC_FIRST_START;
				core.powerStatus = POWER_ON;
				rAreaSt = Radio_7786.Area = _USA;
				bFirstPwrOn = 1;
				RadioInit();
			}
			break;
		case ARM_CKEY_SYS_SETSLEEP:
			if (armkey.keybuff[1] == set)	// sleep
			{
				armsetpowerorsleepFlag = set;
				armsetsleeptime = armkey.keybuff[2];
			}
			else							// power off
			{
				armsetpowerorsleepFlag = reset;
			}
			break;
			
		case ARM_CKEY_RDS_AF:
			rKeyResult = _kcAF;
			bNewKey = 1;	
			break;
			
		case ARM_CKEY_RDS_TA:
			rKeyResult = _kcTA;
			bNewKey = 1;			
			break;
			
		case ARM_CKEY_RDS_PTY:
			rKeyResult = _kcPTY;
			rPtyType = armkey.keybuff[1];
			bNewKey = 1;			
			//else if(armcommandbuff[4] == ARM_CKEY_RDS_PTY)
			{
//				if (System.FWorkMode.Current!= WORKMODE_RADIO)
//					break;
				//if (FreqInAM(Radio_7786.Freq)||F_InTASwitch)
//				if (FreqInAM(Radio_7786.Freq)||FreqInJapanFM(Radio_7786.Freq)||FreqInWBFM(Radio_7786.Freq)||F_InTASwitch||F_InEONTASwitch)
//					break;
				
//				if(F_PTYSelect)
				{
//					PTYSelectTimer=1;
//					break;
				}
				
//				PTYSelectType=armkey.keybuff[1];
				
				//PTYSelectType=MaxMinChangeProtect(PTYSelectType, 1, 31, 1, 0);
//				F_PTYSelect=1;
//				F_PTYEnable=1;
//				PTYSelectTimer=100;
//				F_NoPTY=0;
//				Radio_7786.F_TunerDispReq=1;
//				System.DispMode.Requst=DISPMODE_RDS;
//				RDSDispMode=RDSDISP_PTYSELECT;
				//NavigationShow(NAV_ICON_PTY);

			}
			break;
			
		case ARM_CKEY_SAVE_ALL:
			break;
		default:
			break;
	}
}

void CoreTime(void)
{
	if (!btime10ms)			return;
	if (core.acctime)		core.acctime--;
	if (core.powertime)		core.powertime--;
#if(!TDA7786_TUNER)
//	if (Radio.outtime < RADIO_OPERATE_OUTTIME)	Radio.outtime++;
#else
//	if (Radio_7786.OutTime < RADIO_OPERATE_OUTTIME)	Radio_7786.OutTime++;
#endif
}
void CoreTranserOpenBT(void)
{
	keybuff[keygetindex].key = ARM_CKEY_SYS_OPENBT;
	TransmitKey();
	//SysDelay(20);
}
void MessageARMPoweroff(void)
{
	u8 transmitbuff[10],check;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xE9; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = 0x01;	check ^= transmitbuff[4];
	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}
void CoreFastArm2update(void)
{
	u8 transmitbuff[40];
	u8 index,check;

	if (core.applink)			return;
	if (!core.oslink)			return;
	
	if ((btime100ms) && (!bcoreupdateack) && (bfastarm2updatetimeout))
	{
		bfastarm2updatetimeout--;
		index = reset;
		check = reset;

		transmitbuff[index++] = 0x02;
		transmitbuff[index] = 0x90;		check ^= transmitbuff[index++];
		transmitbuff[index] = 0x0F;		check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00;		check ^= transmitbuff[index++];
		transmitbuff[index++] = check;
		ArmUartSenstr(transmitbuff,index);
	}
}

void CoreFastBack(void)
{
	if (core.applink)			return;
	
	if (btime100ms)
	{
		if (backexistFlag)
		{
		
			//ReportBackTrace();
			if (!bfastbackEnterack)
			{
				bfastbackexit = set;
				ReportFastback(ARMOS_CMD_BACK_ENTER);
			}
		}
		if ((!backexistFlag) || (!accexistFlag))
		{
			if ((!bfastbackExitack) && (bfastbackexit))
			{
				ReportFastback(ARMOS_CMD_BACK_EXIT);
			}
		}
	}
}
void CoreFastPanel(void)
{
	if (core.applink)		return;
	if (!core.oslink)			return;
	if (btime100ms)
	{
		if (!bfastpanelbacklight)
		{
			ReportControlBackLight(set);
		}
	}
}

