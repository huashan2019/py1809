///=========================
#ifndef __MESSAGE_H
#define __MESSAGE_H



typedef struct
{
	SCH_U8 ID;
	SCH_U16 prm;
}MESSAGE;

typedef enum
{
	POWER_MODULE,  ///0 
	ARM_MODULE,
	AUTO_MODULE,
	CAN_MODULE,
	LIN_MODULE,
	RADAR_MODULE,
	MAIN_MODULE,         
	TUNER_MODULE,    
	MMI_MODULE,
	NUMOFMODE
}MSG_MODULE;











extern SCH_BOOL GetMessage(MSG_MODULE Module,MESSAGE *Msg);
extern SCH_BOOL InsertMessage(MSG_MODULE Module,SCH_U8 ID,SCH_U16 param);
extern SCH_BOOL PostMessage(MSG_MODULE Module, SCH_U8 ID, SCH_U16 param);
extern void ClearMessage(MSG_MODULE Module);
extern void ClearAllModeMessage(void);
#endif
