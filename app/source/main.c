


#include "config.h"


void main(void)
{
	VectorRemap(APPLICATION_ADDRESS);
	
	rAreaSt = Radio_7786.Area = _USA;
	BspInit();
	DriveInit();
	ArmUartInit();
	DetectInit();
	KeyInit();
	IrdaInit();
	SwcInit();
	RadioInit();
	CoreInit();
	VMCanInit();
	VM4CanInit();
	#if RTC_CLOCK_ENABLE
	RTC_Proc();
	#endif
	SysDelay(160);
	armcmd_cnt = 0;
	rBackLightLevel = 0x80;
	armsourceState = ARM_SOURCE_STB;
	rRadMainSt = _RadPwrOff;
	bFirstPwrOn = 1;
	CancommandReceiveget = 0;
	Printf("APP Begin -- Software Version : %s \r\n", MCU_VERSION);
	Printf("Code generation time : %s %s \n", __DATE__, __TIME__);

	while(1)
	{
		Testtime_x100ms=0;
		SystemTimeset();
		DetectSignal();
		ProcessPanelKey();
		//ScanSwckey();
		ProcessSwc();
		RemoteProc();
		TASK_Can_Pro();
		ProcessCore();
		ProcessMute();
		ProcessUart2();

		RadioProc();
		RdsProc();
		ProcessDetect();
		ProcessDate();
		
		ProcessVMCan();
		ProcessVM4Can();
		SystemTimereset();
	}

}

