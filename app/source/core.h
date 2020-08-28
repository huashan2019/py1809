
#ifndef _CORE_H
#define _CORE_H


#define WAKEUP_TIMEOUT			500

extern	u8		rAreaSt;
#define		_Europe1	1
#define		_USA		2
#define		_Latin		3
#define		_Russia		4
#define		_Asia		5
#define		_Japan		6
#define		_MidEast	7
#define		_Aus		8
#define		_Brazil		9
#define		_Europe2	10

#define AREA_USA					0
#define AREA_LATIN					1
#define AREA_EUROPE					2
#define AREA_OIRT					3
#define AREA_JAPAN					4
#define AREA_MIDEAST				5		//ÖÐ¶«

extern u8 radioinfoState1;
extern u8 radioinfoState2;
extern u8 rdsinfoState3;

enum{
	ACC_IDLE	= 0,
	ACC_FIRST_START = 1,
	ACC_RUN_START = 2,
	ACC_RUN = 3,
	ACC_ACCWAIT = 4,
	ACC_SLEEP_WAIT = 5,
	ACC_SLEEP = 6,
	ACC_WAKEUP_WAIT = 7,
	ACC_ERROR = 8,
	ACC_LOWV_START = 9,
};

enum{
	POWER_OFF	= 0,
	POWER_UP_DELAY = 1,
	POWER_ON = 2,
	POWER_DN_DELAY = 3,
	POWER_ACC_OFF = 4,
};

typedef struct{
	u8 accStatus;
	u8 powerStatus;
	u8 applink;
	u8 gcoreDevice;
	u8 oslink;
	u8 bcanreceive;
	u8 bwakeup;
	u16 acctime;
	u16 powertime;
}CORE_TYPE;

typedef struct{
	u8 flag;
	u8 keybuff[10];
}COREKEYBUFF;


typedef struct workmodestc 
{
	unsigned char Current;
	unsigned char Next;
	unsigned char Last;
	unsigned char Save;
	unsigned char Requst;
} WorkmodeStruct;

typedef struct sysstuct 
{
	unsigned char PowerMode;
	unsigned char AccMode;
	unsigned char PanelMode;
	WorkmodeStruct FWorkMode;
	WorkmodeStruct DispMode;
} SYSStuct;

extern SYSStuct System;

#define WORKMODE_IDLE				0
#define WORKMODE_RADIO			1
#define WORKMODE_DISC				2
#define WORKMODE_USB				3
#define WORKMODE_SD				4
#define WORKMODE_AUX				5
#define WORKMODE_CDC				6
#define WORKMODE_PHONE			7
#define WORKMODE_STANDBY			8
#define WORKMODE_ALIGN			9
#define WORKMODE_NAV				10
#define WORKMODE_BT				11
#define WORKMODE_MUSIC			12
#define WORKMODE_MAINMENU		13
#define WORKMODE_PNDMEDIA		14
#define WORKMODE_DAB			15

//DispMode
#define DISPMODE_IDLE				0	
#define DISPMODE_RADIO				1
#define DISPMODE_CD				2
#define DISPMODE_CDC				3
#define DISPMODE_AUX				4
#define DISPMODE_PHONE				5
#define DISPMODE_STANDBY			6
#define DISPMODE_AUDIO				7
#define DISPMODE_ALIGN				8
#define DISPMODE_RDS				9
#define DISPMODE_NOTE				10
#define DISPMODE_NAV				11
#define DISPMODE_BT				12
#define DISPMODE_MUSIC				13
#define DISPMODE_MAINMENU			14
#define DISPMODE_PNDMEDIA			15
#define DISPMODE_DAB				16


extern u8 bwakeupfor;

extern COREKEYBUFF armkey;
extern TYPE_BYTE fastback;
	#define bfastbackEnterack		fastback.bit.b7
	#define bfastbackExitack		fastback.bit.b6
	#define bfastbackexit			fastback.bit.b5
	#define bcoreupdateack			fastback.bit.b4
	#define bfastpanelbright		fastback.bit.b3
	#define bfastpanelbacklight		fastback.bit.b2
	#define bfastpaneldate			fastback.bit.b1
	#define bBatLowOn				fastback.bit.b0

extern TYPE_BYTE sysflag0;
	#define bPwr2KyPsd		sysflag0.bit.b7
	//#define bPwrOn			sysflag0.bit.b6
	//#define bfastbackexit			sysflag0.bit.b5
	//#define bcoreupdateack			sysflag0.bit.b4
	//#define bfastpanelbright		sysflag0.bit.b3
	//#define bfastpanelbacklight		sysflag0.bit.b2
	//#define bfastpaneldate			sysflag0.bit.b1
	//#define bBatLowOn				sysflag0.bit.b0


	

extern CORE_TYPE core;
extern u8 bfirstbat;
extern u8 btracedata;

extern u16 bfastarm2updatetimeout;


void CoreInit(void);
void ProcessCore(void);
void CoreAccProcess(void);
void CorePowerProcess(void);

void CoreTime(void);
void CoreTranserOpenBT(void);
void MessageARMPoweroff(void);
void CoreFastArm2update(void);
void CoreFastPanel(void);
void CoreFastBack(void);
void ProcessArmkey(void);


#endif
