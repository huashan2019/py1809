/**
  ******************************************************************************
  * @file		main.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.02.05
  * @brief   
*/



#include "config.h"

u8 timerInterruptAdd;
u8 system10ms;

u8 btime10ms;
u8 btime100ms;
u8 btime200ms;
u8 btime500ms;
u8 btime1s;


u8 bradio10ms;
u8 bswc10ms;
u8 bkey10ms;
u8 bother10ms;


u8 bmuteflag;
u16 bmutetime;
u8 bArmmute;
u8 bawuFlag;
u8 externAMP;
u8 externANT;

ARMTIME_TYPE armtime;
u8 armsettimeStatus;

const u8 leapYearMaxDateTab[13]=
{ 
	0,31,29,31,30,31,30,31,31,30,31,30,31
};
const u8 comYearMaxDateTab[13]=
{ 
	0,31,28,31,30,31,30,31,31,30,31,30,31
};



void DriveInit(void)
{
	u8 i;
	
	timerInterruptAdd = reset;
	system10ms = reset;
	SystemTimereset();
	for (i = 0; i < ADC_CH_MAX; i++)
		adcvalueBuff[i] = reset;
	//init armtime 2016-01-01 01:01:01
	armtime.year = Default_YEAR;
	armtime.month = Default_MONTH;
	armtime.date = Default_DATA;
	armtime.week = 0;
	armtime.hour = 0;
	armtime.minute = 0;
	armtime.second = 0;
	armsettimeStatus = 0x00;
	bmuteflag = set;
	bmutetime = reset;
	bArmmute = reset;
	bawuFlag = reset;
	externANT = reset;
	externAMP = reset;
	bpoweronFlag = 1;
	//core.powerStatus = POWER_ON;
}
void ProcessMute(void)
{
	if ((bmutetime > 0) && (bmutetime < 3))
	{
		if (!bArmmute)
		{
			//if (rRadMainSt == _RadInit) return;
			if ((armsourceState != ARM_SOURCE_RADIO) || (armsourceState == ARM_SOURCE_RADIO && rRadMainSt == _RadStdby)) 
			MuteOff();
#if Mute_En
		Printf("\r\n mute delay off:src=%d,Acc=%d,rad=%d",armsourceState,core.accStatus,rRadMainSt);
#endif
		}
		bmutetime = reset;
	}
	if (btime10ms)
	{
		if (bmutetime)		bmutetime--;
	}
}
void MuteOn(void)
{
	bmuteflag = set;
	MUTE_CTL_ON;
#if Mute_En
	Printf("\r\n mute on:src=%d,Acc=%d,rad=%d",armsourceState,core.accStatus,rRadMainSt);
#endif
}
void MuteOff(void)
{
	if (!accexistFlag)		return;
	if (!batok)				return;
	if (rRadMainSt == _RadInit) return;
	if (core.accStatus != ACC_RUN)			return;
#if Mute_En
	Printf("\r\n mute off:src=%d,Acc=%d,rad=%d",armsourceState,core.accStatus,rRadMainSt);
#endif
	bmuteflag = reset;
	MUTE_CTL_OFF;
}
void MuteTimeset(u16 time)
{
	if (!core.applink)						return;
	if (!accexistFlag)						return;
	
	if (time > bmutetime)
	{
		MuteOn();
		bmutetime = time;
#if Mute_En
	Printf("\r\n mute delay:src=%d,Acc=%d,rad=%d",armsourceState,core.accStatus,rRadMainSt);
#endif
	}
}
void SystemPowerOn(void)
{
	ACC_PULL_ON;
	ARM_SET_WAKEUP;
	TJA_SET_NORMAL;

	CORE_POWER_ON;
	SysDelay(160);
	SetArmPowOn();
	SetArmRstHi();
	SysDelay(300);
	SetArmRstLow();
	AMP_STANDBY_ON;
	//RADIO_PWR_ON;
	//RADIO_INT_L;
	//RADIO_Pin_RSTN_L;
	SysDelay(200);
}
void SystemPowerOff(void)
{
	Uart4Deinit();
	Uart2Deinit();
	Uart1Deinit();
	EXT_CAMERA_PWR_OFF;
	TJA_SET_STANDBY;
	ARM_SET_SLEEP;
	EX_AMP_CLOSE;
	EX_ANT_CLOSE;
	SetArmRstFloating();
	AMP_STANDBY_OFF;
	#if(!TDA7786_TUNER)
	RADIO_PWR_OFF;
	RADIO_Pin_RSTN_L;
//	bradioPoweron = reset;
	#else
	RADIO_PWR_OFF;
	RADIO_Pin_RSTN_L;
//	bradioPoweron = reset;
	#endif
	SetArmPowOff();
	ACC_PULL_OFF;
	CORE_POWER_OFF;
	SetArmRstLow();
	EX_AMP_CLOSE;
}
void SystemSleepWake(void)
{
	#ifdef _SLEEP
	ARM_SET_SLEEP;
	SysDelay(500);
	ARM_SET_WAKEUP;
	#else
	MessageARMPoweroff();
	#endif
}
void McuPowerDown(void)
{
	u16 i;
	
	DetectInit();			
	usart2Receivesave = reset;
	usart2Receiveget = reset;
	usart2SendSave = reset;
	usart2SendIndex = reset;
	for (i = 0; i < USART2_RECEIVE_MAX; i++)
		usart2Receivebuff[i] = reset;
		
	bmuteflag = reset;
	bmutetime = reset;
	fastback.byte = reset;
	//berrortimeout = reset;

	core.applink = reset;
	core.gcoreDevice = reset;
	core.oslink = reset;
	bawuFlag = reset;
	ARM_SET_SLEEP;
	//MUTE_CTL_OFF;// mute off

	DI;
	CloseExternDevice();
	EI;
}
void CloseExternDevice(void)
{
	AdcDeinit();
	GalaDeInit();
	PWM_DeInit();
	Time14Deinit();
	IrdaDeInit();
	Uart4Deinit();
	Uart2Deinit();
	Uart1Deinit();
	RCC->AHBENR &= 0xFFFFFFFE;				// disable DMA clock
	NVIC->ISER[0] = reset;
	GPIO_I2C_SDA_RESET;
	GPIO_I2C_SCL_RESET;
	MUTE_CTL_OFF;
	SystemClkDown();
	RCC->APB1ENR |= BIT8;
	NVIC->ISER[0] |= BIT19;			// enableTim14 interrupt
	NVIC->IP[4] &= 0x00FFFFFF;			// NVIC_IRQChannel = 19     19/4 = 4      high2  priority
	NVIC->IP[4] |= 0x0C000000;			// interrupt priority 12

	TIM14->ARR = 50000;				// 50ms
	TIM14->PSC = 0;					// 1Mhz/(0+1) = 1Mhz
	TIM14->CNT = reset;
	TIM14->DIER |= 0x0001;
	TIM14->CR1 |= 0x0001;
}

void ResetExternDevice(void)
{
	SystemInit();
	Time14Init();
	IrdaInit();
	AdcInit();
	GalaInit();
	PWM_Init();
	VMCanInit();
	VM4CanInit();
	Bsp_UART_Init();
}
void ProcessDate(void)
{
	if (!btime1s)		return;

	armtime.second++;
	
	if(armtime.second > 59)
	{
		armtime.second = 0;
		armtime.minute++;
	}
	if(armtime.minute > 59)
	{
		armtime.minute = 0;
		armtime.hour++;
	}

	if(armtime.hour > 23)
	{
		armtime.hour = 0;
		armtime.date++;
		if(IsLeapYear())
		{
			if(armtime.date > leapYearMaxDateTab[armtime.month])
			{
				armtime.date = 1;
				armtime.month ++;
				if(armtime.month > 12)
				{
					armtime.month =1;
					armtime.year++;
				}
			}
		}
		else
		{
			if(armtime.date > comYearMaxDateTab[armtime.month])
			{
				armtime.date = 1;
				armtime.month ++;
				if(armtime.month > 12)
				{
					armtime.month =1;
					armtime.year++;
				}
			}
		}
	}
}
u8 IsLeapYear(void)
{
	if (armtime.year % 4==0)
	{
		if ((armtime.year % 100) !=0)
		{ 
			return(1);
		}
		else
		{
			if ((armtime.year % 400) ==0) 
			{
				return(1);
			}
		}
	}
	return(0);
}	
void SystemTimeset(void)
{
	if (system10ms)
	{
		if (system10ms & 0x01)
		{
			btime10ms = set;
			timerInterruptAdd++;
			system10ms &= (u8)(~0x01);
		}
		if (system10ms & 0x02)
		{
			bradio10ms = set;
			system10ms &= (u8)(~0x02);
		}
		if (system10ms & 0x04)
		{
			bswc10ms = set;
			system10ms &= (u8)(~0x04);
		}
		if (system10ms & 0x08)
		{
			bkey10ms = set;
			system10ms &= (u8)(~0x08);
		}
		if (system10ms & 0x10)
		{
			bother10ms = set;
			system10ms &= (u8)(~0x10);
		}
	}
	if (!btime10ms)
		return;
	//UartTxData_Direct(SCH_Uart1,0X55,1);

	Tuner10msTimer();
	//Radio10msTimer();
	//Tuner10msTimer();
	if((timerInterruptAdd % 2) == reset)
	{
		if(RdsMeasure_X16ms)	RdsMeasure_X16ms--;
	}
	if((timerInterruptAdd % 3) == reset)
	{
		if(RdsMeasure_X32ms)	RdsMeasure_X32ms--;
		
		if(IO_CAN_R_EXIST_Timer)
		{
			IO_CAN_R_EXIST_Timer--;
			//if(IO_CAN_R_EXIST_Timer == 0 && IO_CAN_R_EXIST)
			//{
			//	IO_CAN_R_EXIST = 0;
			//}
		}
		
		if(RdsReceive_X32ms)	
		{
			--RdsReceive_X32ms;	
			if(RdsReceive_X32ms == 0)
			{
				RDSDataClear( );
			}
		}
		
		//Printf("\r\n B+ value : %x ", adcvalueBuff[3]);
		

	}
	if((timerInterruptAdd % 6) == reset)
	{
		if(Rad_X64ms)	Rad_X64ms--;
		if(RdsTraffic_X64ms)	RdsTraffic_X64ms--;
		if(Rad_SMeter_X64ms)	Rad_SMeter_X64ms--;
		if(rRdsEon_X64ms)		rRdsEon_X64ms--;
	}
	if ((timerInterruptAdd % 10) == reset)
	{
		btime100ms = set;
		if(RdsPty_X128ms)	RdsPty_X128ms--;
		if(cAfTimer_X128ms)	cAfTimer_X128ms--;
		//Radio100msTimer();
		//RDSDataTimer100ms();
	}
	if ((timerInterruptAdd % 20) == reset)
	{
		btime200ms = set;
		
		if(Rad_SMeter_X256ms)	Rad_SMeter_X256ms--;
	}
	if((timerInterruptAdd % 50) == reset)
	{
		btime500ms = set;
		if(RdsPiSeek_X500ms)RdsPiSeek_X500ms--;
		if(RdsMsAf_X500ms)	RdsMsAf_X500ms--;
		if(RdsTpSeek_X500ms)	RdsTpSeek_X500ms--;
		
	}
	if ((timerInterruptAdd % 100) == reset)
	{
		timerInterruptAdd = reset;
		btime1s = set;
//		F_HalfSecondOut = 1;
		//rRmtSt = _Rmtstart;
	}
}
void SystemTimereset(void)
{
	btime10ms = reset;
	btime100ms = reset;
	btime200ms = reset;
	btime500ms = reset;
	btime1s = reset;

	bradio10ms = reset;
	bswc10ms = reset;
	bkey10ms = reset;
	bother10ms = reset;
}
void MemCopy(u8 *str,u8 *dtr,u16 len)
{
	u16 i;

	for (i = 0; i < len; i++)
		*dtr++ = *str++;
}

void DEBUG_STR(u8 *p)
{
	#ifdef DEBUG
	do
	{
		usart1SendData[usart1SendSave++] = *p++;
	}while(*p != '\0');

	usart1SendData[usart1SendSave++] = '\r';
	usart1SendData[usart1SendSave++] = '\n';
	
	if (!usart1Sendbusy)
	{
		USART1->CR1 |= 0x00000040;
	}
	#endif
}

