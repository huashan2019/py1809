
#ifndef _DRIVE_H
#define _DRIVE_H


#define		Default_YEAR	2020
#define		Default_MONTH	1
#define		Default_DATA	1





typedef struct{
	u16 year;
	u8 month;
	u8 date;
	u8 week;
	u8 hour;
	u8 minute;
	u8 second;
	u8 secondbak;
}ARMTIME_TYPE;

extern ARMTIME_TYPE armtime;
extern u8 armsettimeStatus;
extern u8 timerInterruptAdd;
extern u8 system10ms;
extern u8 bother10ms;
extern u8 btime10ms;
extern u8 btime100ms;
extern u8 btime200ms;
extern u8 btime1s;
extern u8 bradio10ms;
extern u8 bswc10ms;
extern u8 bkey10ms;
extern u8 bother10ms;

extern u8 bmuteflag;
extern u16 bmutetime;
extern u8 bArmmute;
extern u8 bawuFlag;
extern u8 externAMP;
extern u8 externANT;

void DriveInit(void);
void ProcessMute(void);
void McuPowerDown(void);
void ProcessDate(void);
u8 IsLeapYear(void);
void DEBUG_STR(u8 *p);
void MuteOn(void);
void MuteOff(void);
void MuteTimeset(u16);
void SystemTimeset(void);
void SystemTimereset(void);
void SystemPowerOn(void);
void SystemPowerOff(void);
void SystemSleepWake(void);
void ResetExternDevice(void);
void CloseExternDevice(void);

void MemCopy(u8 *str,u8 *dtr,u16 len);

#endif

