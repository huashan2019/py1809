
#ifndef _SWC_H
#define _SWC_H

#include "Uicc_def.h"

#define ISSWCKEY_VALUE		245
#define ISSWCKEY_LEVEL1		0X86/8/*0V*/
#define ISSWCKEY_LEVEL2		(0XBB+0X86)/8/*0.425V*/
#define ISSWCKEY_LEVEL3		(0XBB+0XEF)/8/*0.6V*/
#define ISSWCKEY_LEVEL4		(0XEF+0X14D)/8/*0.762V*/
#define ISSWCKEY_LEVEL5		(0X222+0X14D)/8/*1.055V*/
#define ISSWCKEY_LEVEL6		222/*0V*/
#define ISSWCKEY_LEVEL7		240

#define AD_SW_MAX_NUM           17

#define AD_SW1_CHANNEL     	1//ADC_CH_PB0//AD IN8
#define AD_SW2_CHANNEL     	2//ADC_CH_PC4//AD IN14

#define T80MS_8              8
#define T96MS_8              9
#define T1S_8                100


/*
0:只有短按功能
1:有长按功能，但长按按键没有连续功能
2:有长按功能，并且长按按键有连续功能
*/

typedef struct
{
	
	SCH_U8  KeyStudyFlag;/*0:no study 1: studyed ad0 2:studyed ad1*/
	SCH_U16 KeyADC_Data;
	SCH_U16 KeyADC_DefaultData;
	const UICC_KEY_CODE ShortKeyCode;
	const UICC_KEY_CODE LongKeyCode;
	const SCH_U8 KeyProperty;
}KEY_AD_DATA;



enum{
	SCANSWCKEY_IDLE				= 0,
	SCANSWCKEY_PRESS1			= 1,
	SCANSWCKEY_PRESS2			= 2,
	SCANSWCKEY_LONG				= 3,
	SCANSWCKEY_RELEASE			= 4,
	SCANSWCKEY_RELEASE_SYSUPDATE	= 5
};



typedef enum
{
	Key_Up,
	Key_Down
}KEY_STATE;

typedef enum
{
	NO_STUDY,
	STUDY_ING,
	STUDY_OK
}STUDY_STATE;

typedef struct
{
	UICC_KEY_CODE ShortKeyCode;
	UICC_KEY_CODE LongKeyCode;///
	UICC_KEY_CODE BkKeyCode;
	SCH_U8 KeyProperty;
	KEY_STATE KeyStatus;///1  press down   0 press up	
	SCH_U8 HoldTime;
	SCH_U8 F_HoldPress;
	SCH_U8 StudyNum;
	STUDY_STATE StudyState;
	UICC_KEY_CODE StudyKeyCode;
}KEY_INFO;

extern  KEY_INFO AD_SW2_Info;
extern  KEY_INFO AD_SW_Info;


/*Key scan status define*/
#define	_ksNoKey		0
#define	_ksPsdCfm		1
#define	_LongKyCfm		2
#define	_ksPrsing		3      
#define	_ShortKyCfm		4
#define	_LooseCfm		5

#define ADC_DATA_MIN     0x00
#define ADC_DATA_MAX     0xFF
#define PANEL_STEP   	 0x05//0x15

#define REMOTE					0
#define PANEL 					1
#define TOUCH_SCREEN 			2

void SwcInit(void);
void ProcessSwc(void);
void ScanSwckey(void);
void Saveswckey(u8,u8);
SCH_BOOL GetStudyKeyNum(void);
void ResetStudyKeyNum(void);



#endif


