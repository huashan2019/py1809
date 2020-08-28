
#ifndef _UART2_H
#define _UART2_H



#define ArmUartSenstr	Uart2SendStr




#define USART2_SEND_MAX					(u16)0x200
#define USART2_SEND_SUB					(u16)0x1FF
#define USART2_RECEIVE_MAX				(u16)0x200
#define USART2_RECEIVE_SUB					(u16)0x1FF
#define ARMUART_RECEIVEBUF_MAX			USART2_RECEIVE_MAX

#define ARMUART_COMMAND_HEAD0			0x02
#define ARMUART_COMMAND_HEAD1			0x80
#define ARMUART_COMMAND_HEAD2			0x81
#define ARMUART_OSCOMMAND_HEAD		0x91


#define ARM_CMD_STUDY_PANELKEY_STATUS	0xE0
#define ARM_CMD_STUDY_PANELKEY_LIST		0xE1
#define ARM_CMD_1939_JINYE_675			0xD0
#define ARM_CMD_CANBUS					0xD1//0x08
#define ARM_CMD_TRANS_CANDATA			0xD2

#define ARM_CMD_TRANS_CANDATA1			0xD3

#define ARM_CMD_REQ_HANDSHAKE			0xA0
#define ARM_CMD_REQ_SETBAUD				0xA1
#define ARM_CMD_REQ_INITIAL				0xA2
#define ARM_CMD_REQ_STATECONTR			0xA3
#define ARM_CMD_REQ_DEVICESTATE			0xA4
#define ARM_CMD_REP_DEVICESTATE			0xA5
#define MCU_CMD_REP_DEVICESTATE			0xA6

#define ARM_CMD_REQ_CHANGESRC			0xA7
#define MCU_CMD_REQ_CHANGESRC			0xA8
#define ARM_CMD_REQ_VOLUMEINFO			0xA9
#define ARM_CMD_REQ_SETVOLUME			0xAA
#define ARM_CMD_REQ_EQINFO				0xAB
#define ARM_CMD_REQ_SETEQ				0xAC
#define ARM_CMD_KEY						0xB0
#define ARM_KEY_SAVED					0xFD
#define ARM_CMD_TOUCHXY					0xB1
#define MCU_CMD_KEY						0xB2
#define ARM_CMD_REQ_RADIOINFO			0xB3
#define RADIO_CURRENT_FREQ				0x00
#define RADIO_PRESET_LIST				0x01
#define MCU_REP_RADIOINFO				0xB4
#define ARM_CMD_GETINFO					0xB7
#define ARM_CMD_SETINFO					0xB8
#define ARM_SET_MPEG					0x00
#define ARM_SET_BT						0x01
#define ARM_SET_TIME					0x02
#define ARM_SET_CURR					0x03
#define ARM_SET_PANEL					0x04
#define ARM_SET_RADIO					0x05
#define ARM_SET_RDS						0x06
#define ARM_SET_BACKPANEL				0x08
#define ARM_SET_EXTANT_SWITCH			0x0A
#define ARM_SET_EXTAMP_SWITCH			0x0B
#define ARM_SET_VCOM					0x0C
#define ARM_SET_POWER_STATE				0x0D


#define ARM_CMD_GETSWCTABLE				0xBA
#define ARM_CMD_SETSWCTABLE				0xBC
#define ARM_CMD_SWCSTUDY				0xBD
	#define ARM_CMD_SWCSTUDY_START		0x01
	#define ARM_CMD_SWCSTUDY_EXIT		0x00
	#define ARM_CMD_SWCSTUDY_OK			0x02
	#define ARM_CMD_SWCSTUDY_RESET		0x03
#define MCU_REP_SWC_SAMPTABLE			0xBE

#define ARM_CMD_RESPONSESOURCE			0xC0
#define MCU_REP_ACCSTATE					0xC2
#define MCU_VERSIONS						0xC3
#define MCU_UPDATE_START					0xC4
#define ARM_SEND_DVR						0xC8
#define ARM_SEND_DTV						0xC9
#define ARM_SET_CANTYPE					0xCA
#define ARM_CMD_USBUPGRADE				0xD5
#define ARM_CMD_COLOR_LED				0xDB
#define ARM_CMD_FREQ_COMMAND			0xE8

#define ARMOS_CMD_UARTOK				0x00
#define ARMOS_CMD_BACK_ENTER			0x01
#define ARMOS_CMD_BACK_EXIT				0x02
#define ARMOS_CMD_SETDATE				0x03
#define ARMOS_CMD_BACKLIGHTON			0x04
#define ARMOS_CMD_PANELPARA				0x06
#define ARMOS_CMD_UPDATE				0x0F
#define ARMOS_CMD_CANBAUD				0x10
#define ARMOS_CMD_BACK_TRACE			0x0A
#define ARMOS_CMD_ACC_START				0x12


#define REPORT_SUCCESS					0x00
#define REPORT_ERROR						0x01
#define REPORT_UNSUPPORT					0xFF

#define ARM_SOURCE_STB					0xFF
#define ARM_SOURCE_CD					1
#define ARM_SOURCE_SD					2
#define ARM_SOURCE_USB					3
#define ARM_SOURCE_RADIO				4
#define ARM_SOURCE_TV					5
#define ARM_SOURCE_CMMB					6
#define ARM_SOURCE_DVBT					7
#define ARM_SOURCE_AUX					8
#define ARM_SOURCE_AUX2					9
#define ARM_SOURCE_CDC					10
#define ARM_SOURCE_BT					11
#define ARM_SOURCE_CCD					12
#define ARM_SOURCE_GPS					13

#define ARM_SOURCE_MAIN					15
#define ARM_SOURCE_IPOD					16
#define ARM_SOURCE_MCU_TV				17
#define ARM_SOURCE_ARM_UI				18
#define ARM_SOURCE_END					19
#define ARM_POWEROFF					0

#define ARM_CKEY_POWER					0x01
#define ARM_CKEY_EJECT					0x1D
#define ARM_CKEY_RADIO_BAND				0x50
#define ARM_CKEY_RADIO_TUNEUP			0x51
#define ARM_CKEY_RADIO_TUNEDOWN			0x52
#define ARM_CKEY_RADIO_SEEKUP			0x53
#define ARM_CKEY_RADIO_SEEKDOWN			0x54
#define ARM_CKEY_RADIO_AS				0x55
	#define ARM_CKEY_RADIO_AS_AUTO		0x00
#define ARM_CKEY_RADIO_PS				0x56
#define ARM_CKEY_RADIO_LOCDX				0x57
#define ARM_CKEY_RDS_TA					0x58
#define ARM_CKEY_RDS_PTY					0x59
#define ARM_CKEY_RDS_AF					0x5A
#define ARM_CKEY_RDS_REG					0x5B
#define ARM_CKEY_RADIO_SETFMFREQ			0x60
#define ARM_CKEY_RADIO_SETAMFREQ			0x61
#define ARM_CKEY_RADIO_SAVE				0x62
#define ARM_CKEY_RADIO_LOAD				0x63
#define ARM_CKEY_RDS_PTYSEEK				0x64
#define ARM_CKEY_RADIO_STERO				0x65
#define ARM_CKEY_RADIO_PREV				0x73
#define ARM_CKEY_RADIO_NEXT				0x72
#define ARM_CKEY_BLACKOUT				0xA1
#define ARM_CKEY_BEEP					0xA3
	#define ARM_CKEY_BEEP_OK				0x00
	#define ARM_CKEY_BEEP_ERROR			0x01
	#define ARM_CKEY_BEEP_FAIL			0x02
	
#define ARM_CKEY_SWC_PRESSINDEX			0xA6
#define ARM_CKEY_SWC_STUDYOK			0xA7
#define ARM_CKEY_SWC_STUDYRESET			0xA8
#define ARM_CKEY_SYS_RESET				0xB8
#define ARM_CKEY_SYS_OPENBT				0xC0
#define ARM_CKEY_SYS_SETSLEEP				0xC1
#define ARM_CKEY_SAVE_ALL					0xFD

#define ARM_CKEY_RADIO_AREA				0xE5
	#define CKEY_RADIO_AREA_USA1			1
	#define CKEY_RADIO_AREA_USA2			11
	#define CKEY_RADIO_AREA_CHINA			7
	#define CKEY_RADIO_AREA_JAPAN			3
	#define CKEY_RADIO_AREA_OIRT			2
	#define CKEY_RADIO_AREA_EUROPE		0
	#define CKEY_RADIO_AREA_MIDEAST		4
	#define CKEY_RADIO_AREA_LATIN			5
	#define CKEY_RADIO_AREA_RUSSIAN		13

	
#define ARM_CKEY_MENU_EXIT				0xAF
#define ARM_CMD_TEST_MODE				0xFF

extern u8 forRadioAreaMute;
extern u8 armsourceState;
extern u8 bpoweronFlag;


extern u8 usart2Sendbuff[USART2_SEND_MAX];
extern u16 usart2SendSave;
extern u16 usart2SendIndex;
extern u8 usart2Sendbusy;

extern u8 usart2Receivebuff[USART2_RECEIVE_MAX];
extern u16 usart2Receivesave;
extern u16 usart2Receiveget;
extern u8 armcmd_cnt;
extern u8 rAccSt;



void ArmUartInit(void);
void ProcessUart2(void);
void Uart2DmaSendProcess(void);
void ProcessArmCommand(void);
void ReportHandShake(void);
void ReportHeartbeat(void);
void ReportRdsPsinfo(void);
void ReportDeviceStatus(u8);
void ReportDate(u8);
void ReportRadioSignalinfo(u8 info);
void ReportBackPara(void);
void ReportSetinfo(void);
void ReportKeySuccess(void);
void ReportChangeSource(u8);
void ReportRadioBaseInfo(void);
void ReportRadioPreListinfo(void);
void ReportRadioArea(void);
void ReportSetVolume(void);
void ReportAccState(void);
void ReportMcuPowerinfo(void);
void ReportRdsRTinfo(void);

void ReportMcuVisions(void);
void ReportFastback(u8);
void ReportControlBackLight(u8 flag);
void ReportARM2UPDate(void);
void ReportFirstBAT(void);
void ReportAccinfo(void);
void ReportSWCTable(void);

void Uart2SendStr(u8*,u16);
void Uart2SendDECI(u32 p);
void Uart2SendHextoAscii(u8 p);


#endif

