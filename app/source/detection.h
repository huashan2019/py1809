
#ifndef _DETECTION_H
#define _DETECTION_H

typedef enum{
	WAKEUPSCAN_IDLE		= 0,
	WAKEUPSCAN_EXIST	= 1,
	WAKEUPSCAN_NON		= 2
}WAKEUPSCAN_TYPE;

typedef enum{
	ACCSCAN_IDLE		= 0,
	ACCSCAN_EXIST		= 1,
	ACCSCAN_NON			= 2
}ACCSCAN_TYPE;


typedef enum{
	BACKSCAN_IDLE		= 0,
	BACKSCAN_EXIST		= 1,
	BACKSCAN_NON		= 2
}BACKSCAN_TYPE;

typedef enum{
	BRAKESCAN_IDLE		= 0,
	BRAKESCAN_EXIST		= 1,
	BRAKESCAN_NON		= 2
}BRAKESCAN_TYPE;

typedef enum{
	CARILLSCAN_IDLE		= 0,
	CARILLSCAN_EXIST	= 1,
	CARILLSCAN_NON		= 2
}CARILLSCAN_TYPE;

typedef enum{
	DISCSCAN_IDLE		= 0,
	DISCSCAN_EXIST		= 1
}DISCSCAN_TYPE;

#define BAT_6P5V		0xc4/4
#define BAT_8V			0xd0/4
#define BAT_9V			0xe0/4//80/2
#define BAT_9P5V		0xea/4//95/2
#define BAT_15V			0x193/4
#define BAT_16V			0x1ae/4
#define BAT_17V			0x1b4/4


extern u8 wakeupFlag;

extern u8 accexistFlag;
extern u8 backexistFlag;
extern u8 brakeexistFlag;
extern u8 carillexistFlag;
extern u8 rightcameraexistFlag;

extern u8 oldbackexistFlag;
extern u8 oldcarillexistFlag;
extern u8 oldbrakeexistFlag;
extern u8 oldrightcameraexistFlag;
extern u8 batok;
extern u8 rBackLightLevel;

void DetectInit(void);
void ProcessDetect(void);
void DetectSignal(void);
u8 HasAcc(void);
void DetecAccSignal(void);
void DetecBackSight(void);
void DetecBrakeSight(void);
void DetecCarILL(void);
void DetecDisc(void);
void CoreWakeupDetect(void);
void DetecRightCamera(void);
void DetecBat(void);

#endif


