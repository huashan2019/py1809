/**
  ******************************************************************************
  * @file		main.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.02.05
  * @brief   
*/



#include "config.h"

#if _UartDebug
	#define Tx_Enable	1
	#define Rx_Enable	1

#endif
u8 usart2Sendbuff[USART2_SEND_MAX];
u16 usart2SendSave;
u16 usart2SendIndex;
u8 usart2Sendbusy;

u8 usart2Receivebuff[USART2_RECEIVE_MAX];
u16 usart2Receivesave;
u16 usart2Receiveget;

u8 armcommandbuff[ARMUART_RECEIVEBUF_MAX];
u8 armcmd_temp[255];
u8 armcmd_cnt;
u8 rAccSt;

u8 armsourceState;
u8 forRadioAreaMute;

u8 bpoweronFlag;

SCH_U8 AAC_AirHeatL;
SCH_U8 AAC_AirHeatR;

extern unsigned char FmIdx;
extern unsigned char AmIdx;
extern const AreaStuct Area[];
//extern unsigned char REAL_PS[8];
//extern unsigned char REAL_PTY;
extern KEY_AD_DATA AD_SW_Tab[AD_SW_MAX_NUM];
extern KEY_AD_DATA AD_SW_Tab2[AD_SW_MAX_NUM];

extern u8 Cancommandbuff[20];

void ArmUartInit(void)
{
	u16 i;
	
	usart2SendSave = reset;
	usart2SendIndex = reset;
	usart2Sendbusy = reset;

	usart2Receivesave = reset;
	usart2Receiveget = reset;

	for (i = 0; i < USART2_RECEIVE_MAX; i++)
		usart2Receivebuff[i] = reset;
}
void ProcessUart2(void)
{
	u16 i,receiveindex, len,receivelen;
	u8 temp,check;
	
	if (usart2Receivesave == usart2Receiveget)		return;
	if (usart2Receivesave > usart2Receiveget)		len = (u16)(usart2Receivesave - usart2Receiveget);
	else										len = (u16)((USART2_RECEIVE_MAX - usart2Receiveget) + usart2Receivesave);
	
	if (len < 4)								return;

	receiveindex = usart2Receiveget;
	if (usart2Receivebuff[receiveindex] == ARMUART_COMMAND_HEAD0)
	{
		receiveindex = (u16)(usart2Receiveget + 1) & USART2_RECEIVE_SUB;
		if ((usart2Receivebuff[receiveindex] == ARMUART_COMMAND_HEAD1) 
			|| (usart2Receivebuff[receiveindex] == ARMUART_COMMAND_HEAD2)
			|| (usart2Receivebuff[receiveindex] == ARMUART_OSCOMMAND_HEAD))
		{
			receiveindex = (u16)(usart2Receiveget + 3) & USART2_RECEIVE_SUB;
			receivelen = (u16)(usart2Receivebuff[receiveindex]+5) & USART2_RECEIVE_SUB;

			if (len < receivelen)
			{
				return;
			}
			else
			{
				check = reset;
				#if Rx_Enable
					Printf("\r\n UART RX:src=%d,f=%d,fre=%d",armsourceState,cRadStatNum.word,Radio_7786.Freq);
				#endif
				for(i = 0; i < receivelen; i++)
				{
					temp = usart2Receivebuff[usart2Receiveget];
					armcommandbuff[i] = temp;
				#if Rx_Enable
					Printf(" %x",temp);
				#endif
					check ^= temp;
					usart2Receiveget++;
					usart2Receiveget &= USART2_RECEIVE_SUB;
				}
				if (check == ARMUART_COMMAND_HEAD0)
				{
					ProcessArmCommand();
				}
			}
		}
		else
		{
			usart2Receiveget++;
			usart2Receiveget &= USART2_RECEIVE_SUB;
		}
	}
	else
	{
		usart2Receiveget++;
		usart2Receiveget &= USART2_RECEIVE_SUB;
	}
}
//=================================================================
//Function Nam: ProcessArmCommand()
//Descript:
//=================================================================
void ProcessArmCommand(void)
{
	u32 canid;
	u8 i,j,index,len,*p;
	
#if	_UartDebug
		//armcmd_temp[armcmd_cnt]=armcommandbuff[2];
		//Printf("\r\n armcmd  :%x",armcmd_temp[armcmd_cnt++]);
#endif
	switch(armcommandbuff[2])
	{
		case ARM_CMD_REQ_HANDSHAKE:
			core.applink = set;
			bArmmute = reset;
			ReportHandShake();

			if(armsourceState == ARM_SOURCE_RADIO)
			Radio7786Poweron();			
			bFirstPwrOn = 0;
			ReportRadioArea();
			break;
		case ARM_CMD_TRANS_CANDATA:
			if(armcommandbuff[4] == 1)/*touch x,y*/
			{
				sch_memcpy(AAC_DataTx,&armcommandbuff[5],13);
				PostMessage(CAN_MODULE,M2C_TOUCH_XY,NONE);
			}
			else if(armcommandbuff[4] == 2)/*year*/
			{
				sch_memcpy(AAC_DataTx,&armcommandbuff[5],13);
				PostMessage(CAN_MODULE,M2C_YMD_TIME,NONE);
			}
			else if(armcommandbuff[4] == 3)/*month*/
			{
				sch_memcpy(AAC_DataTx,&armcommandbuff[5],13);
				PostMessage(CAN_MODULE,M2C_YMD_TIME,NONE);
			}
			else if(armcommandbuff[4] == 4)/*day*/
			{
				sch_memcpy(AAC_DataTx,&armcommandbuff[5],13);
				PostMessage(CAN_MODULE,M2C_YMD_TIME,NONE);
			}
			else if(armcommandbuff[4] == 5)/*CAN Respond frame*/
			{
				sch_memcpy(Cancommandbuff,&armcommandbuff[5],armcommandbuff[2]-1);
				Uart4SendStr(Cancommandbuff,armcommandbuff[3]-1);
			}
			else
			Uart1SendStr(&armcommandbuff[4],armcommandbuff[3]);
			break;
		case ARM_CMD_TRANS_CANDATA1:
			if(armcommandbuff[4] == 1)/*1:air heat;0:can data send*/
			{
				//ReportAirHeatData(armcommandbuff[4],AAC_AirHeatL,AAC_AirHeatR);
			}
			else
			{
				sch_memcpy(AAC_DataTx,&armcommandbuff[5],13);
				PostMessage(CAN_MODULE,M2C_DATA,NONE);
			}
			break;
		case ARM_CMD_REQ_SETBAUD:
			break;
		case ARM_CMD_REQ_INITIAL:
			break;
		case ARM_CMD_REQ_DEVICESTATE:
			core.gcoreDevice = set;
			ReportDeviceStatus(0);
			break;
		case ARM_CMD_REP_DEVICESTATE:
			
#if 0
			if (armcommandbuff[4] & 0x01)		//BT ����
			{
				#if(!TDA7786_TUNER)
				if (Radio.workmode != RADIO_WORK_IDLE)
					RadioBreak();
				#else
				if (Radio_7786.WorkMode != RADIO_WORK_IDLE)
					Radio7786Break();
				#endif
			}
#endif
			break;
		case ARM_CMD_REQ_CHANGESRC:
			ReportChangeSource(REPORT_SUCCESS);
			#if 1
			
			MuteTimeset(50);
			armsourceState = armcommandbuff[4];
			if (armsourceState == ARM_SOURCE_RADIO)
			{
			
				//Radio7786Poweron();			
				forRadioAreaMute = set;
				if (externANT)
				{
					EX_ANT_OPEN;
				}
			}
			else if (armsourceState == ARM_SOURCE_MAIN)
			{
			}
			else if (armsourceState == ARM_POWEROFF)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_STB)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_CD)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_SD)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_USB)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_TV)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_CMMB)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_DVBT)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_AUX)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_AUX2)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_CDC)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_BT)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_CCD)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_IPOD)
			{
				//forRadioAreaMute = reset;
			}
			else if (armsourceState == ARM_SOURCE_MCU_TV)
			{
				//forRadioAreaMute = reset;
			}
			
			if ((armcommandbuff[4] != ARM_SOURCE_RADIO) && (!forRadioAreaMute))
			{
				EX_ANT_CLOSE;				
			}
			#endif
			break;
		case ARM_CMD_REQ_VOLUMEINFO:
			break;
		case ARM_CMD_REQ_SETVOLUME:
			bwakeupfor = set;
			ReportSetVolume();
			if (armcommandbuff[8])
			{
				bArmmute = set;
				MuteOn();
			}
			else
			{
				bArmmute = reset;				
				MuteTimeset(50);
			}
			
			break;
		case ARM_CMD_KEY:
			ReportKeySuccess();
			armkey.keybuff[0] = armcommandbuff[4];
			armkey.keybuff[1] = armcommandbuff[5];
			armkey.keybuff[2] = armcommandbuff[6];
			armkey.keybuff[3] = armcommandbuff[7];
			armkey.keybuff[4] = armcommandbuff[8];
#if	_DebugTestDly
				Testtime_x10ms = 0;
				Printf("\r\n test0 = %d,Rad_X64ms= %d",Testtime_x10ms,Rad_X64ms);
#endif
			armkey.flag = set;
			break;
		case ARM_CMD_REQ_RADIOINFO:
			if (armcommandbuff[4] == RADIO_CURRENT_FREQ)
			{
			
				Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
				ReportRadioSignalinfo(0);
				
			}
			else if (armcommandbuff[4] == RADIO_PRESET_LIST)
			{
				ReportRadioPreListinfo();
			}
			break;
		case ARM_CMD_GETINFO:
			if (armcommandbuff[4] == ARM_SET_TIME)
			{
			#if RTC_CLOCK_ENABLE
				RTC_TimeTypeDef RTC_TimeStructure;
				RTC_DateTypeDef RTC_DateStructure;
				  /* Get the current Time */
				RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
				  /* Get the current Date */
				RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
				if(RTC_DateStructure.RTC_Year == 0 && armtime.year%100 == 99)
				{
					armtime.year += 1;
				}
				armtime.year = armtime.year/100*100+RTC_DateStructure.RTC_Year;
				armtime.month = RTC_DateStructure.RTC_Month;//wMonth;/*month*/
				armtime.date = RTC_DateStructure.RTC_Date;//wDay;

				armtime.hour = RTC_TimeStructure.RTC_Hours;//wHour;
				armtime.minute = RTC_TimeStructure.RTC_Minutes;//wMinute;
				armtime.second = RTC_TimeStructure.RTC_Seconds;//wSec;
			#endif
				ReportDate(0);
			}
			else if (armcommandbuff[4] == ARM_SET_RADIO)
			{
				//ReportRadioInfo();
			}
			else if (armcommandbuff[4] == ARM_SET_PANEL)
			{
				
			}
			else if (armcommandbuff[4] == ARM_SET_BACKPANEL)
			{
				
			}
			else if (armcommandbuff[4] == ARM_SET_POWER_STATE)
			{
				ReportMcuPowerinfo();	
			}
			
			break;

		
		case ARM_CMD_SETINFO:
			ReportSetinfo();
			if (armcommandbuff[4] == ARM_SET_TIME)
			{
				RTC_TimeTypeDef RTC_TimeStructure;
				RTC_DateTypeDef RTC_DateStructure;
				
				armtime.year = ((armcommandbuff[5] >> 4) & 0x0F) * 1000;
				armtime.year += (armcommandbuff[5] & 0x0F) * 100;
				armtime.year += ((armcommandbuff[6] >> 4) & 0x0F) * 10;
				armtime.year += (armcommandbuff[6] & 0x0F);
				
				armtime.month = ((armcommandbuff[7] >> 4) & 0x0F) * 10;
				armtime.month += (armcommandbuff[7] & 0x0F);
				armtime.date = ((armcommandbuff[8] >> 4) & 0x0F) * 10;
				armtime.date += (armcommandbuff[8] & 0x0F);

				armtime.hour = ((armcommandbuff[9] >> 4) & 0x0F) * 10;
				armtime.hour += (armcommandbuff[9] & 0x0F);

				armtime.minute = ((armcommandbuff[10] >> 4) & 0x0F) * 10;
				armtime.minute += (armcommandbuff[10] & 0x0F);

				armtime.second = ((armcommandbuff[11] >> 4) & 0x0F) * 10;
				armtime.second += (armcommandbuff[11] & 0x0F);

				armsettimeStatus = armcommandbuff[12];/* 0:24;bit 1:12 */
			#if RTC_CLOCK_ENABLE	
				RTC_DateStructure.RTC_Year = armtime.year%100;
				RTC_DateStructure.RTC_Month = armtime.month;
				RTC_DateStructure.RTC_Date = armtime.date;
				RTC_TimeStructure.RTC_Hours = armtime.hour;
				RTC_TimeStructure.RTC_Minutes = armtime.minute;
				RTC_TimeStructure.RTC_Seconds = armtime.second;
				
				/* Configure the RTC time register */
				if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
				{
#if   _UartDebug
				  Printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
#endif
				}
				  /* Configure the RTC time register */
				if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
				{
#if   _UartDebug
				  Printf("\n\r>> !! RTC Set Data failed. !! <<\n\r");
#endif
				} 
			#endif
				ReportDate(1);
			}
			else if (armcommandbuff[4] == ARM_SET_CURR)
			{
				
			}
			else if (armcommandbuff[4] == ARM_SET_RADIO)
			{
				armkey.keybuff[0] = ARM_CKEY_RADIO_AREA;
				armkey.keybuff[1] = armcommandbuff[5];
				armkey.flag = set;
			}
			else if (armcommandbuff[4] == ARM_SET_RDS)
			{
				if((armcommandbuff[5]&0x03) != 0x00)
				{
					bRDSDs = 0;
				}
				else 
				{
					bRDSDs = 1;
					RDSInit();
				}
				
				if((armcommandbuff[5]&0x04) == 0x04)
					bRdsClkEn = 1;
				else bRdsClkEn = 0;

				if((armcommandbuff[5]&0x08) == 0x08)
					bRdsAfDetEn = 1;
				else bRdsAfDetEn = 0;

				if((armcommandbuff[5]&0x10) == 0x10)
					bRdsTAEn = 1;
				else bRdsTAEn = 0;

				if((armcommandbuff[5]&0x20) == 0x20)
					bEonTaTAEn = 1;
				else bEonTaTAEn = 0;

				if((armcommandbuff[5]&0x40) == 0x40)
					bRdsRegOn = 1;
				else bRdsRegOn = 0;
			}
			else if(armcommandbuff[4] == ARM_SET_POWER_STATE)
			{ 
				ReportMcuPowerinfo();	
			}
			else if (armcommandbuff[4] == ARM_SET_EXTANT_SWITCH)
			{
				externANT = armcommandbuff[5];
				
				if ((externANT) && (forRadioAreaMute))
				{
					EX_ANT_OPEN;
				}
				else
				{
					EX_ANT_CLOSE;
				}
			}
			else if (armcommandbuff[4] == ARM_SET_EXTAMP_SWITCH)
			{
				externAMP = armcommandbuff[5];
				if (externAMP)
				{
					EX_AMP_OPEN;
				}
				else
				{
					EX_AMP_CLOSE;
				}
			}
			break;
		case ARM_CMD_RESPONSESOURCE:
			//berrortimeout = reset;
			break;
		case ARM_CMD_COLOR_LED:
			//ledconfig.mode = armcommandbuff[4];
			//ledconfig.ledR = armcommandbuff[5];
			//ledconfig.ledG = armcommandbuff[6];
			//ledconfig.ledB = armcommandbuff[7];
			//ledconfig.refresh = set;
			break;
		case ARM_CMD_FREQ_COMMAND:
			if(rRadMainSt==_RadIdle || ((armsourceState != ARM_SOURCE_RADIO) && (bRDSDs || bAM_FM)))	return;
	      	if(bRadSeeking)
			{/* in Pty on state, start pty searching */
			   	if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);	
				RestoreStatNum( );
			}
			if(rRadMainSt == _RadPS)
			radioinfoState2 &= (u8)(~BIT0);
			if(rRadMainSt == _RadAS)
			radioinfoState1 &= (u8)(~BIT3);

			bRadSeeking = 0;

			GetRadPlayFre((u16)(armcommandbuff[5] << 8)|armcommandbuff[6]);
			StartWrPll();
			
			if(cBand != armcommandbuff[4]) 
			{
				StartChgBand(armcommandbuff[4]);
				Radio_7786.CurrentBand = cBand;
				Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
			}
			cPSNumBak = cPSNum = 0;
			
			rRadMainSt = _RadDlToStdby;
			StartWrPll( );
			Rad_X64ms = 600/64;
			ReportRadioBaseInfo();
			ReportRadioSignalinfo(0);
			ReportRadioPreListinfo();
			break;
		case MCU_VERSIONS:
			ReportMcuVisions();
			break;
		case MCU_UPDATE_START:
			MuteOn();
			McuIapStart();
			break;
		case ARM_CMD_USBUPGRADE:
			MuteOn();
			break;
		/*****OS COMMAND REPORT***************/
		case ARMOS_CMD_UARTOK:
			//MuteOn();
			fastback.byte = reset;
			core.oslink = set;
			core.applink = reset;
			core.gcoreDevice = reset;
			ReportFirstBAT();
			break;
		case ARMOS_CMD_CANBAUD:
			if (armcommandbuff[3] == 4)
			{
				canbaudRate = (u32)armcommandbuff[4] << 24;
				canbaudRate = canbaudRate + ((u32)armcommandbuff[5] << 16);
				canbaudRate = canbaudRate + ((u32)armcommandbuff[6] << 8);
				canbaudRate = canbaudRate + (u32)armcommandbuff[7];
				if ((canbaudRate < 9600) || (canbaudRate > 115200))
					canbaudRate = 38400;
				SetLinUartBaud(canbaudRate);
			}
			break;
		case ARMOS_CMD_BACK_ENTER:
			bfastbackEnterack = set;
			bfastbackexit = set;
			TFT_ON;
			break;
		case ARMOS_CMD_BACK_EXIT:
			bfastbackExitack = set;
			break;
		case ARMOS_CMD_SETDATE:
			bfastpaneldate = set;
			break;
		case ARMOS_CMD_PANELPARA:
			bfastpanelbright = set;
			break;
		case ARMOS_CMD_BACKLIGHTON:
			bfastpanelbacklight = set;
			//sDelay(50);
			if (accexistFlag)
			{
				TFT_ON;
			}
			break;
		case ARMOS_CMD_UPDATE:
			bfastarm2updatetimeout = reset;
			bcoreupdateack = set;
			break;


		case ARM_CMD_GETSWCTABLE:
			ReportSWCTable();
			break;

		case ARM_CMD_SWCSTUDY:
			if (armcommandbuff[4] == ARM_CMD_SWCSTUDY_START)
			{
				AD_SW2_Info.StudyState = STUDY_ING;
			}
			else if(armcommandbuff[4] == ARM_CMD_SWCSTUDY_OK)
			{
				AD_SW2_Info.StudyState = STUDY_OK;
			}
			else if(armcommandbuff[4] == ARM_CMD_SWCSTUDY_RESET)
			{
				AD_SW2_Info.StudyState = NO_STUDY;
				AD_SW_Info.StudyState = NO_STUDY;
				ResetStudyKeyNum();
				ReportSWCTable();
			}
			else if(armcommandbuff[4] == ARM_CMD_SWCSTUDY_EXIT)
			{
				AD_SW_Info.StudyState = NO_STUDY;
				AD_SW2_Info.StudyState = NO_STUDY;
				if(GetSWStudyKeyNum())AD_SW_Info.StudyState = STUDY_OK;
				if(GetSW2StudyKeyNum())AD_SW2_Info.StudyState = STUDY_OK;
			}
				
			break;
			
		default:
			break;
	}
}
//=================================================================
//Function Nam: ReportHandShake()
//Descript:
//=================================================================
void ReportHandShake(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xA0; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;		//acc status
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}
//=================================================================
//Function Nam: ReportHeartbeat()
//Descript:
//=================================================================
void ReportHeartbeat(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xA1; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;		//acc status
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}

//=================================================================
//Function Nam: ReportDeviceChangeStatus()
//Descript:
//=================================================================
void ReportDeviceStatus(u8 change)
{
	u8 transmitbuff[20],state,check,index,i;

	index = reset;
	state = reset;
	check = reset;
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = (u8)(0x80 | change);		check ^= transmitbuff[index++];
	transmitbuff[index] = 0xA4; 
	if (change)
		transmitbuff[index] = 0xA6;
	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(0x0A - change);		check ^= transmitbuff[index++];

	if (!change)
	{
		transmitbuff[index] = REPORT_SUCCESS;
		check ^= transmitbuff[index++];
	}
	
	transmitbuff[index] = (u8)(0x00);
	check ^= transmitbuff[index++];

	state |= backexistFlag;
	state |= (u8)(carillexistFlag << 1);
	state |= (u8)(brakeexistFlag << 2);
	transmitbuff[index] = state;
	check ^= transmitbuff[index++];

	transmitbuff[index] = 0x00;
	check ^= transmitbuff[index++];
	for (i = 0; i < 6; i++)
	{
		transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
	}
	transmitbuff[index++] = check;

	ArmUartSenstr(transmitbuff,index);	
}
//=================================================================
//Function Nam: ReportDate(u8)
//Descript:
//=================================================================
void ReportDate(u8 update)
{
	u8 transmitbuff[20],check,index;
	u8 time;
	
	index = reset;
	check = reset;
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = (u8)(0x80 | update); 
	check ^= transmitbuff[index++];
	transmitbuff[index] = 0xB7;
	if (update)
		transmitbuff[index] = 0xB9;
	check ^= transmitbuff[index++];
	
	transmitbuff[index] = (u8)(0x0A - update);
	check ^= transmitbuff[index++];
	if (!update)
	{
		transmitbuff[index] = REPORT_SUCCESS;		//acc status
		check ^= transmitbuff[index++];
	}
	transmitbuff[index] = ARM_SET_TIME;
	check ^= transmitbuff[index++];
	//arm time 2012-01-01 01:01:01
	time = (u8)((armtime.year / 1000) << 4) & 0xF0;
	time |= (u8)((armtime.year % 1000) / 100) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];

	time = (u8)(((armtime.year % 100) / 10) << 4) & 0xF0;
	time |= (u8)(armtime.year % 10) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];

	time = (u8)((armtime.month / 10) << 4) & 0xF0;
	time |= (u8)(armtime.month % 10) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];
	
	time = (u8)((armtime.date / 10) << 4) & 0xF0;
	time |= (u8)(armtime.date % 10) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];

	time = (u8)((armtime.hour / 10) << 4) & 0xF0;
	time |= (u8)(armtime.hour % 10) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];

	time = (u8)((armtime.minute / 10) << 4) & 0xF0;
	time |= (u8)(armtime.minute % 10) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];

	time = (u8)((armtime.second / 10) << 4) & 0xF0;
	time |= (u8)(armtime.second % 10) & 0x0F;
	transmitbuff[index] = time;
	check ^= transmitbuff[index++];

	//rear source
	transmitbuff[index] = armsettimeStatus; 
	check ^= transmitbuff[index++];

	transmitbuff[index++] = check;

	ArmUartSenstr(transmitbuff,index);
}
//=================================================================
//Function Nam: ReportSetinfo()
//Descript:
//=================================================================
void ReportMcuPowerinfo(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xB7; check ^= transmitbuff[2];
	transmitbuff[3] = 0x03; check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;check ^= transmitbuff[4];
	transmitbuff[5] = 0x0D; 			check ^= transmitbuff[5];
	transmitbuff[6] = core.powerStatus; check ^= transmitbuff[6];
	transmitbuff[7] = check;

	ArmUartSenstr(transmitbuff,8);

}

//=================================================================
//Function Nam: ReportSetinfo()
//Descript:
//=================================================================
void ReportRdsPsinfo(void)
{
	u8 transmitbuff[20],check,i;
	//u8 Temp[8]={'A','B','C','D','E','F','G','H'};

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xB3; check ^= transmitbuff[2];
	transmitbuff[3] = 11; 	check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;	check ^= transmitbuff[4];
	transmitbuff[5] = 0x02; check ^= transmitbuff[5];
	for (i = 0; i < 8; i++)
	{
		transmitbuff[6+i] = rRdsPsNameSto[i];
		check ^= transmitbuff[6+i];
	}
	transmitbuff[14] = rRdsStoPtyTemp; check ^= transmitbuff[14];
	transmitbuff[15] = check;
	ArmUartSenstr(transmitbuff,16);

}

//=================================================================
//Function Nam: ReportSetinfo()
//Descript:
//=================================================================
void ReportRdsRTinfo(void)
{
	u8 transmitbuff[80],check,i;
	//u8 Temp[8]={'A','B','C','D','E','F','G','H'};

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xB3; check ^= transmitbuff[2];
	transmitbuff[3] = 67; 	check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;	check ^= transmitbuff[4];
	transmitbuff[5] = 0x03; check ^= transmitbuff[5];
	transmitbuff[6] = 64; check ^= transmitbuff[6];/*0:RT vail;1:RT invail;*/
	for (i = 0; i < 64; i++)
	{
		transmitbuff[7+i] = rRdsRTNameSto[i];
		check ^= transmitbuff[7+i];
	}
	
	transmitbuff[7+i] = check;
	ArmUartSenstr(transmitbuff,72);

}

//=================================================================
//Function Nam: ReportSetinfo()
//Descript:
//=================================================================
void ReportSetinfo(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xB8; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;		//acc status
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);

}
//=================================================================
//Function Nam: ReportChangeSource()
//Descript:
//=================================================================
void ReportChangeSource(u8 report)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02;
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xA7; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = report;
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}
//=================================================================
//Function Nam: ReportKeySuccess()
//Descript:
//=================================================================
void ReportKeySuccess(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xB0; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;		//acc status
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}

//=================================================================
//Function Nam: ReportRadioBaseInfo()
//Descript:
//=================================================================
void ReportRadioBaseInfo(void)
{
	u8 transmitbuff[40],check,index;
	TYPE_WORD	FreDataMin,FreDataMax;
	uchar const *p;

	check = reset;
	index = reset;
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x81;		check ^= transmitbuff[index++];
	transmitbuff[index] = 0xB4;		check ^= transmitbuff[index++];
	transmitbuff[index] = 20;		check ^= transmitbuff[index++];

	transmitbuff[index] = 0x04;	check ^= transmitbuff[index++]; 	

	p = TunFMParmStartAddr();/*FM1*/
	FreDataMin.ByteH = *(p+3);
	FreDataMin.ByteL = *(p+4);
	FreDataMax.ByteH = *(p+7);
	FreDataMax.ByteL = *(p+8);
	///rTempi = *(p + 2)/10;

	transmitbuff[index] = (u8)(FreDataMin.byte[1]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(FreDataMin.byte[0]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(FreDataMax.byte[1]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(FreDataMax.byte[0]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(*(p + 2)/10);	check ^= transmitbuff[index++];
	
	if (Radio_7786.Area == _Russia && cBand == _BandFM3)/*FM2*/
	{
		p = TunFMORITParmStartAddr();
		FreDataMin.ByteH = *(p+3);
		FreDataMin.ByteL = *(p+4);
		FreDataMax.ByteH = *(p+7);
		FreDataMax.ByteL = *(p+8);
		//rTempi = *(p + 2)/10;
		transmitbuff[index] = (u8)(FreDataMin.byte[1]); check ^= transmitbuff[index++];
		transmitbuff[index] = (u8)(FreDataMin.byte[0]); 	check ^= transmitbuff[index++];
		transmitbuff[index] = (u8)(FreDataMax.byte[1]);check ^= transmitbuff[index++];
		transmitbuff[index] = (u8)(FreDataMax.byte[0]); 	check ^= transmitbuff[index++];
		transmitbuff[index] = (u8)(*(p + 2)/10);					check ^= transmitbuff[index++];
	}
	else
	{
		transmitbuff[index] = reset;	check ^= transmitbuff[index++];
		transmitbuff[index] = reset;	check ^= transmitbuff[index++];
		transmitbuff[index] = reset;	check ^= transmitbuff[index++];
		transmitbuff[index] = reset;	check ^= transmitbuff[index++];
		transmitbuff[index] = reset;	check ^= transmitbuff[index++];
	}
	
	p = TunAMParmStartAddr();/*am*/
	FreDataMin.ByteH = *(p+3);
	FreDataMin.ByteL = *(p+4);
	FreDataMax.ByteH = *(p+7);
	FreDataMax.ByteL = *(p+8);
	//rTempi = *(p + 2);
	transmitbuff[index] = (u8)(FreDataMin.byte[1]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(FreDataMin.byte[0]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(FreDataMax.byte[1]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(FreDataMax.byte[0]);	check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(*(p + 2));	check ^= transmitbuff[index++];

	transmitbuff[index] = rRadMeterLevel; 	check ^= transmitbuff[index++]; 	//Signal
	transmitbuff[index] = radioinfoState1;	check ^= transmitbuff[index++]; 	//Radio status0
	transmitbuff[index] = radioinfoState2;	check ^= transmitbuff[index++]; 	//Radio status1
	transmitbuff[index] = rdsinfoState3;	check ^= transmitbuff[index++]; 	//rds

	transmitbuff[index++] = check;

	ArmUartSenstr(transmitbuff,index);
}

void ReportRadioPreListinfo(void)
{
	u8 transmitbuff[30],check,index,a,len,i;
	TYPE_WORD cRadStatNum_Temp;
	
	check = reset;
	index = reset;
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x81; check ^= transmitbuff[index++];
	transmitbuff[index] = 0xB4; check ^= transmitbuff[index++];
	transmitbuff[index] = 17; 	check ^= transmitbuff[index++];
	transmitbuff[index] = 0x01; check ^= transmitbuff[index++];
	transmitbuff[index] = Radio_7786.CurrentBand; check ^= transmitbuff[index++];/*5th:0~fm1,1~fm2,....4~am2*/

	if(rRadMainSt == _RadPS || bRadPsNumRead)
	{
		bRadPsNumRead = 0;
		transmitbuff[index] = cPSNum; check ^= transmitbuff[index++];/*6th:1~6*/
	}
	else
	{
		if(!cPSNum)
		{
			if(PSStationChk() == 0xff)	transmitbuff[index] = 0;
			else
				transmitbuff[index] = PSStationChk();
			check ^= transmitbuff[index++];/*6th:1~6*/
		}
		else 
		{
			transmitbuff[index] = PSStationChk();
			check ^= transmitbuff[index++];/*6th:1~6*/
		}
	}

	cRadStatNum_Temp.word = Radio_7786.Freq;
	transmitbuff[index] = cRadStatNum_Temp.byte[1]; check ^= transmitbuff[index++];
	transmitbuff[index] = cRadStatNum_Temp.byte[0]; check ^= transmitbuff[index++];
	for(i = 0;i<6;i++)
	{
		if(bAM_FM)
		{
			cRadStatNum_Temp.word = AMPresetMem[Radio_7786.CurrentBand - _BandAM1][i];
		}
		else
		{
			cRadStatNum_Temp.word = FMPresetMem[Radio_7786.CurrentBand][i];
		}
		
		cRadStatNum_Temp.word = GetRadFreOctData(cRadStatNum_Temp.word);
		transmitbuff[index] = cRadStatNum_Temp.byte[1];check ^= transmitbuff[index++];
		transmitbuff[index] = cRadStatNum_Temp.byte[0];check ^= transmitbuff[index++];
	}
	transmitbuff[index++] = check;
	ArmUartSenstr(transmitbuff,index);
}

void ReportRadioSignalinfo(u8 info)
{
	u8 transmitbuff[30],check,index,a,len;

	check = reset;
	index = reset;
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x80; check ^= transmitbuff[index++];
	transmitbuff[index] = 0xE8; check ^= transmitbuff[index++];
	transmitbuff[index] = 0X04; check ^= transmitbuff[index++];
	transmitbuff[index] = Radio_7786.CurrentBand; check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)(Radio_7786.Freq>>8); check ^= transmitbuff[index++];
	transmitbuff[index] = (u8)Radio_7786.Freq; check ^= transmitbuff[index++];
	transmitbuff[index] = info; check ^= transmitbuff[index++];
	transmitbuff[index++] = check;
	ArmUartSenstr(transmitbuff,index);
}
//=================================================================
//Function Nam: ReportRadioArea()
//Descript:
//=================================================================
void ReportRadioArea(void)
{
	u8 transmitbuff[20],check,i;

	check = reset;
	transmitbuff[0] = 0x02;
	transmitbuff[1] = 0x81; 		check ^= transmitbuff[1];
	transmitbuff[2] = 0xB9; 		check ^= transmitbuff[2];
	transmitbuff[3] = 0x07; 		check ^= transmitbuff[3];
	transmitbuff[4] = 0x05; 		check ^= transmitbuff[4];
	transmitbuff[5] = Radio_7786.Index;	check ^= transmitbuff[5];
	for (i = 6; i < 11; i++)
	{
		transmitbuff[i] = reset;
		check ^= transmitbuff[i];
	}

	transmitbuff[11] = check;

	ArmUartSenstr(transmitbuff,12);
}
//=================================================================
//Function Nam: ReportSetVolume()
//Descript:
//=================================================================
void ReportSetVolume(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02;
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xAA; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = REPORT_SUCCESS;		//acc status
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}
//=================================================================
//Function Nam: ReportAccState()
//Descript:
//=================================================================
void ReportAccState(void)
{
	u8 transmitbuff[20],check;

	check = reset;
	transmitbuff[0] = 0x02;
	transmitbuff[1] = 0x81; check ^= transmitbuff[1];
	transmitbuff[2] = 0xC2; check ^= transmitbuff[2];
	transmitbuff[3] = 0x01; check ^= transmitbuff[3];
	transmitbuff[4] = accexistFlag; 	//acc status
	check ^= transmitbuff[4];

	transmitbuff[5] = check;

	ArmUartSenstr(transmitbuff,6);
}
//=================================================================
//Function Nam: ReportMcuVisions()
//Descript:
//=================================================================
void ReportMcuVisions(void)
{
	u8 transmitbuff[30],check,index,a,len;
	u8 *versionstr = MCU_VERSION;//"MCU-20190613.2.00.B";

	check = reset;
	index = reset;
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x80; check ^= transmitbuff[index++];
	transmitbuff[index] = 0xC3; check ^= transmitbuff[index++];

	for (a = 0;a < 20; a++)
		if (versionstr[a] == '\0')		break;
		
	transmitbuff[index] = a+1; check ^= transmitbuff[index++];
	transmitbuff[index] = REPORT_SUCCESS; check ^= transmitbuff[index++];
	len = a;

	for (a = 0; a < len; a++)
	{
		transmitbuff[index] = versionstr[a]; 
		check ^= transmitbuff[index++];
	}

	transmitbuff[index++] = check;

	ArmUartSenstr(transmitbuff,index);
	//Uart4SendStr(transmitbuff,index);
}
//=================================================================
//Function Nam: ReportFastback()
//Descript:
//=================================================================
void ReportFastback(u8 nback)
{
	u8 transmitbuff[20],index,check,i;

	index = reset;
	check = reset;
	
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x90; 	check ^= transmitbuff[index++];
	transmitbuff[index] = nback;	check ^= transmitbuff[index++];
	if (nback == 0x01)
	{
		transmitbuff[index] = 0x02; check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
	}
	else
	{
		transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
	}
	transmitbuff[index++] = check;

	ArmUartSenstr(transmitbuff,index);

}
//=================================================================
//Function Nam: ReportControlBackLight()
//Descript:
//=================================================================
void ReportControlBackLight(u8 flag)
{
	u8 transmitbuff[20];
	u8 index,check;

	index = reset;
	check = reset;
	
	if (flag)
	{
		transmitbuff[index++] = 0x02;
		transmitbuff[index] = 0x90; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x04; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x01; 	check ^= transmitbuff[index++];
		transmitbuff[index] = rBackLightLevel;	check ^= transmitbuff[index++];
		transmitbuff[index++] = check;
	}
	else
	{
		transmitbuff[index++] = 0x02;
		transmitbuff[index] = 0x90; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x05; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x01; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00; 		check ^= transmitbuff[index++];
		transmitbuff[index++] = check;
	}
	ArmUartSenstr(transmitbuff,index);
}
void ReportBackTrace(void)
{
	u8 transmitbuff[20],index,check,i;

	index = reset;
	check = reset;
	btracedata++;
	if(btracedata >= 180) btracedata = 0;
	
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x90;		check ^= transmitbuff[index++];
	transmitbuff[index] = ARMOS_CMD_BACK_TRACE;	check ^= transmitbuff[index++];
	transmitbuff[index] = 0x1;		check ^= transmitbuff[index++];
	transmitbuff[index] = btracedata;	check ^= transmitbuff[index++];
	transmitbuff[index++] = check;

	bfirstbat = set;
	ArmUartSenstr(transmitbuff,index);	
}

void ReportFirstBAT(void)
{
	u8 transmitbuff[20],index,check,i;

	index = reset;
	check = reset;
	
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x90;		check ^= transmitbuff[index++];
	transmitbuff[index] = ARMOS_CMD_ACC_START;	check ^= transmitbuff[index++];
	transmitbuff[index] = 0x1;		check ^= transmitbuff[index++];
	transmitbuff[index] = bfirstbat;	check ^= transmitbuff[index++];
	transmitbuff[index++] = check;
	bfirstbat = set;
	ArmUartSenstr(transmitbuff,index);	
}

//=================================================================
//Function Nam: ReportSetinfo()
//Descript:
//=================================================================
void ReportAccinfo(void)
{
	u8 transmitbuff[20],index,check,i;

	index = reset;
	check = reset;
	
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x81;		check ^= transmitbuff[index++];
	transmitbuff[index] = MCU_REP_ACCSTATE;	check ^= transmitbuff[index++];
	transmitbuff[index] = 0x1;		check ^= transmitbuff[index++];
	transmitbuff[index] = rAccSt;	check ^= transmitbuff[index++];
	if(rAccSt == 0x02 && batok) rAccSt = 0x00;
	
	transmitbuff[index++] = check;
	ArmUartSenstr(transmitbuff,index);	

}


void ReportARM2UPDate(void)
{
	u8 transmitbuff[40];
	u8 index,check;

	index = reset;
	check = reset;
	if (!core.applink)
	{
		transmitbuff[index++] = 0x02;
		transmitbuff[index] = 0x90; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x0F; 	check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00; 	check ^= transmitbuff[index++];
		transmitbuff[index++] = check;
		ArmUartSenstr(transmitbuff,index);
	}
}


//=================================================================
//Function Nam: ReportHandShake()
//Descript:
//=================================================================
void ReportSWCTable(void)
{
	u8 transmitbuff[50],check,i;

	check = reset;
	transmitbuff[0] = 0x02; 
	transmitbuff[1] = 0x80; check ^= transmitbuff[1];
	transmitbuff[2] = 0xBA; check ^= transmitbuff[2];
	transmitbuff[3] = 34; check ^= transmitbuff[3];
	
	for(i=0;i<17;i++)
	{
		if(AD_SW2_Info.StudyState == NO_STUDY && AD_SW_Info.StudyState == NO_STUDY) 
		{
			transmitbuff[4+2*i] = 0;				check ^= transmitbuff[4+2*i];
		}
		else
		{
			if(AD_SW_Tab[i].KeyStudyFlag != 0x00 || AD_SW_Tab2[i].KeyStudyFlag != 0x00)
			{
				transmitbuff[4+2*i] = 1;				check ^= transmitbuff[4+2*i];
			}
			else
			{
				transmitbuff[4+2*i] = 0;				check ^= transmitbuff[4+2*i];
			}

		}
		
		if(AD_SW2_Info.StudyState == NO_STUDY && AD_SW_Info.StudyState == NO_STUDY) 
		{
			transmitbuff[4+2*i+1] = 0;check ^= transmitbuff[4+2*i+1];
		}
		else if(AD_SW_Tab[i].KeyStudyFlag != 0x00)
		{
			transmitbuff[4+2*i+1] = AD_SW_Tab[i].ShortKeyCode;	check ^= transmitbuff[4+2*i+1];
		}
		else //if(AD_SW_Tab2[i].KeyStudyFlag != 0x00)
		{
			transmitbuff[4+2*i+1] = AD_SW_Tab2[i].ShortKeyCode;	check ^= transmitbuff[4+2*i+1];
		}
			
	}
	
	transmitbuff[4+2*i] = check;
	ArmUartSenstr(transmitbuff,34+5);
}

void Uart2DmaSendProcess(void)
{
	if (usart2SendSave == usart2SendIndex)		return;
	if ((u16)DMA1_Channel4->CNDTR)				return;
	if ((DMA1_Channel5->CCR & 1) == reset)		return;
	
	DMA1_Channel4->CCR &= 0xFFFFFFFE;
	DMA1_Channel4->CMAR = (u32)usart2Sendbuff + usart2SendIndex;
	if (usart2SendSave > usart2SendIndex)
	{
		DMA1_Channel4->CNDTR = usart2SendSave - usart2SendIndex;
		usart2SendIndex = usart2SendSave;
	}
	else
	{
		DMA1_Channel4->CNDTR = USART2_SEND_MAX - usart2SendIndex;
		usart2SendIndex = reset;
	}
	DMA1_Channel4->CCR |= (u32)0x1;
}
void Uart2SendStr(u8 *p, u16 len)
{
	u8 i;
	
#if Tx_Enable
	Printf("\r\n UART TX:radmain=%d,radsub=%d,acc=%d,pwr=%d",rRadMainSt,rRadSubSt,core.accStatus,core.powerStatus);
#endif
	for (i = 0; i < len; i++)
	{
	
	#if Tx_Enable
		Printf(" %x",*p);
	#endif
		usart2Sendbuff[usart2SendSave++] = *p++;
		usart2SendSave &= USART2_SEND_SUB;
	}
	if (!usart2Sendbusy)
	{
		USART2->CR1 |= 0x00000080;
	}
}


