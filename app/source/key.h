
#ifndef _KEY_H
#define _KEY_H


#define AIN0		0
#define AIN1		1
#define AIN2		2
#define AIN3		3
#define AIN4		4
#define AIN5		5
#define AIN6		6
#define AIN7		7
#define AIN8		8
#define AIN9		9
#define AIN10		10
#define AIN11		11
#define AIN12		12
#define AIN13		13
#define AIN14		14
#define AIN15		15


#define ISPANELKEY_VALUE			242

#define ISPANELKEY_LEVEL0			10			// 0.0v
#define ISPANELKEY_LEVEL1			58			// 0.6v
#define ISPANELKEY_LEVEL2			124			// 1.8v
#define ISPANELKEY_LEVEL3			173			// 2.35v
#define ISPANELKEY_LEVEL4			200			// 2.35v
#define ISPANELKEY_LEVEL5			220			// 2.35v
#define ISPANELKEY_LEVEL6			242			// 2.35v





#define VOL_LEVEL0_MAX		160//175 //> MIN 30k (47 ||82)
#define VOL_LEVEL1_MAX		188//195 //>MID 47K
#define VOL_LEVEL2_MAX		200 //>MAX 82K
#define VOL_LEVEL3_MAX		230

#define TUN_LEVEL0_MAX		116 //> MIN 30k (47 ||82)
#define TUN_LEVEL1_MAX		135 //>MID 47K
#define TUN_LEVEL2_MAX		170 //>MAX 82K
#define TUN_LEVEL3_MAX		194




#define VOLKEY_ARM_INC		0x04
#define VOLKEY_ARM_DEC		0x05
#define TUNKEY_ARM_INC		0x72
#define TUNKEY_ARM_DEC		0x73
#define NO_KEY              0xFF



enum{
	SCANPANELKEY_IDLE			= 0,
	SCANPANELKEY_PRESS1			= 1,
	SCANPANELKEY_PRESS2			= 2,
	SCANPANELKEY_LONG			= 3,
	SCANPANELKEY_RELEASE		= 4,
	SCANPANELKEY_RELEASE_SYSUPDATE = 5
};


enum{
	SCANENCODEKEY_IDLE		= 0,
	SCANENCODEKEY_PRESS		= 1,
	SCANENCODEKEY_RELEASE	= 2
};


enum{
	KEY_SYS_POWER				= 0x01,
	KEY_SYS_MUTE				= 0x02,
	KEY_SYS_EQ					= 0x03,
	KEY_SYS_VOLUP				= 0x04,
	KEY_SYS_VOLDN				= 0x05,
	KEY_SYS_PHONE_ON			= 0x06,
	KEY_SYS_PHONE_OFF			= 0x07,
	KEY_SYS_MODE				= 0x08,
	KEY_SYS_HOME				= 0x09,
	KEY_SYS_MENU				= 0x0A,
	KEY_SYS_VOICE				= 0x0B,
	KEY_SYS_RECENT				= 0x10,
	KEY_SYS_EJECT				= 0x1D,
	IR_NUM0    					 = 0x20,
    IR_NUM1    					 = 0x21,
    IR_NUM2    					 = 0x22,
    IR_NUM3   					 = 0x23,
    IR_NUM4    					 = 0x24,
    IR_NUM5    					 = 0x25,
    IR_NUM6    					 = 0x26,
    IR_NUM7    					 = 0x27,
    IR_NUM8    					 = 0x28,
    IR_NUM9     			     = 0x29,
    
	KEY_SYS_UP					= 0x2A,
	KEY_SYS_DOWN				= 0x2B,
	KEY_SYS_TUNELEFT			= 0x2C,
	KEY_SYS_TUNERIGHT			= 0x2D,
	KEY_SYS_ENTER				= 0x2E,
	KEY_SYS_BACK				= 0x2F,
	KEY_START					= 0x30,
	KEY_NUMBER					= 0x31,

	KEY_MUSIC					= 0x40,
	
	KEY_SYS_NAV 				= 0x43,
	KEY_SYS_RADIO				= 0x44,
	KEY_SYS_DVD 				= 0x45,
	KEY_SYS_BT					= 0x46,
	KEY_SYS_SETTING				= 0x47,
	KEY_SYS_TOUCH_CHECK 		= 0x4F,
	KEY_RADIO_BAND				= 0x50,
	KEY_RADIO_TUNE_UP			= 0x51,
	KEY_RADIO_TUNE_DOWN 		= 0x52,
	KEY_RADIO_SEEK_UP			= 0x53,
	KEY_RADIO_SEEK_DOWN 		= 0x54,
	KEY_RADIO_AS				= 0x55,
	KEY_SYS_STOP				= 0x70,
	KEY_SYS_PLAY				= 0x71,
	KEY_SYS_NEXT				= 0x72,
	KEY_SYS_PREV				= 0x73,
	KEY_SYS_FASTF				= 0x74,
	KEY_SYS_FASTB				= 0x75,
	T_ZOOM						= 0x7A,

	KEY_SYS_BLACKOUT			= 0xA1,
	KEY_SYS_T_SELECT			= 0xA4,
	KEY_SYS_SEL					= 0xA9,
	KEY_SYS_LOUD				= 0xAA,
	
	T_IR_GOTO					= 0xAB,
	T_KEY_DISP					= 0xAC,
	_kcCamera 					= 0xB0,
	_kcRdM1 					= 0xB1,
	_kcRdM2						= 0xB2,
	_kcRdM3						= 0xB3,
	_kcRdM4						= 0xB4,
	_kcRdM5						= 0xB5,		
	_kcRdM6						= 0xB6,
	_kcSeekInc					= 0xB7,
	 _kcSeekDec					= 0xB8,
	 _kcPS						= 0xB9,
	//						= 0xBA,
	//						= 0xBB,
	//						= 0xBC,
	_kcReg						= 0xBD,
	_kcAms						= 0xBE,
	_kcSeekDecManu				= 0xBF,
	_kcBand 				   	= 0xC0,
	_kcWrM1						= 0xC1,
	_kcWrM2						= 0xC2,
	_kcWrM3						= 0xC3,
	_kcWrM4						= 0xC4,
	_kcWrM5						= 0xC5,
	_kcWrM6						= 0xC6,
	_kcAF						= 0xC7,
	_kcTA						= 0xC8,
	_kcPTY						= 0xC9,
	_kcSeekIncManu				= 0xCA,
	_kcFM						= 0xCB,
	_kcAM						= 0xCC,
	_kcLoc						= 0xCD,
	_kcSeekInc1					= 0xCE,
	_kcSeekDec1 				= 0xCF,
	
    _kcPower   					= 0xd0,
    _kcPhone    				= 0xd2,
    IR_STOP    					= 0xd3,

	IR_PLAY_PAUSE 				= 0xd8,
    IR_PREV 	 				= 0xd9,
    IR_NEXT 					= 0xda,
    IR_FR   					= 0xdb,
    IR_FF   					= 0xdc,

	_kcTuneInc					= 0xE0,
	_kcTuneDec					= 0xE1,
	_kcScan 					= 0xE4,

	_kcSelect 					= 0xf5,
	_kcST						= 0xf7,
	_kcAudio1					= 0xf8,
	_kcAudio2					= 0xf9,
	IR_ROOT 	= 0xfA,
	KEY_SYS_LONG_KEY_RELEASE	= 0xFF,
	KEY_SYS_NULL	= 0xFE
	
};

enum{
	KEY_SYS_RECOMBINATION_PHONEOFF				= 0x01,
	KEY_SYS_RECOMBINATION_PHONEON				= 0x02,
};

#define KEYSAVE_MAX		0x08
#define KEYSAVE_SUB		0x07

typedef struct{
	u8 flag;
	u8 key;
	u8 key1;
}KEYBUFF_TYPE;
extern u8 rKeyResult;

extern u8 bpowerkey;
extern u8 bwaitpowerkey;
extern u8 keygetindex;
extern KEYBUFF_TYPE keybuff[KEYSAVE_MAX];

void KeyInit(void);
void ProcessPanelKey(void);
void ScanKey(void);
void ScanVolEncodekey(void);
void ScanTunEncodekey(void);
void ScanPanelkey(void);
void Savekey(u8,u8,u8);
void SaveEncodekey(u8);
void SaveRecombinationkey(u8);
void TransmitKey(void);
void SaveLongkeyRelease(u8 gkey,u8 ch,u8 flag);

#endif
