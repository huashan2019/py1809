/*********************************************************
	Project:		ST Elite Tuner(TDA7786)
	File name:	Tuner_Elite.h
	Description:
	Write/Modify:	Yete HUANG
	Time:		March-2014
*********************************************************/
#define I2C_ADDRESS					0xC2   /* Three options : 0xC0, 0xC2, 0xC8 for tuner Star */
#define AUTOSEEK					0x01

#define FM_NARROWBAND				1

#define LW_FREQ						300
#define AM_FREQ						6450
#define SW_FREQ                   			1840  //1725
#define CHANGE_FM 1
#define CHANGE_AM 2
#define CHANGE_WX 3
#define CHANGE_SW 4

//#define FM_SW_START_FREQ			2300
//#define FM_SW_STOP_FREQ			18135

#define FM_FREQ						6450

#define FM_NORMAL_START_FREQ		8000
#define FM_OIRT_START_FREQ		6450
#define FM_OIRT_STOP_FREQ			7500
#define FM_JAPAN_START_FREQ		7500
#define FM_JAPAN_STOP_FREQ		9100

#define FM_WB_START_FREQ			62300
#define FM_WB_STOP_FREQ			62600

#define FreqInFM(x)				((x)>((AM_FREQ)))
#define FreqInAM(x)				((x)<((FM_FREQ)))
#define FreqInNormalFM(x)			(x>FM_NORMAL_START_FREQ && x<FM_WB_START_FREQ)
#define FreqInOirtFM(x)			((x>FM_OIRT_START_FREQ && x<FM_OIRT_STOP_FREQ)&&Radio_7786.Area==_Russia && cBand == _BandFM3)
#define FreqInJapanFM(x)			(x>FM_JAPAN_START_FREQ && x<FM_JAPAN_STOP_FREQ&&Radio_7786.Area==AREA_JAPAN)
#define FreqInWBFM(x)				(x>FM_WB_START_FREQ && x<FM_WB_STOP_FREQ)
#define FreqInMWAM(x)				((x)>LW_FREQ && (x)<SW_FREQ)
#define FreqInLWAM(x)				((x)<LW_FREQ)
#define FreqInSWAM(x)				((x)<FM_FREQ && (x)>SW_FREQ)

#define FM_BAND_NUM 3
#define AM_BAND_NUM 3
#define BAND_NUM  6

typedef struct FMBand_stuct {
	unsigned int BandFreq;
	#ifndef NOT_SAVE_BAND_AF
	unsigned int BandPI;
	unsigned char BandAFList[8];
	unsigned char BandAFNum;
	#endif
	unsigned int MemFreq[6];
	unsigned char MemSmeter[6];
	unsigned int PI[6];
	unsigned char MemAFList[6][8];
	unsigned char MemAFNum[6];
	unsigned char CurrentMem;
} FMBandStuct;

typedef struct AMBand_stuct {
	unsigned int BandFreq;
	unsigned int MemFreq[6];
	unsigned char MemSmeter[6];
	unsigned char CurrentMem;
} AMBandStuct;


typedef struct Radio_7786_stuct {
	unsigned char SeekMode;
	unsigned char WorkMode;
	unsigned char CurrentBand;
	unsigned int ReqFreq;
	unsigned int Freq;
	unsigned int DisplayFreq;
	unsigned int BackupFreq;
	unsigned int ScanBackupFreq;
	unsigned char TunerBuff[40];
	UCharField ReadBuff;
	//unsigned int TestPointFreq;
	//signed char TestPointVal;
	//signed int Slope;
	//signed int Slope2;
	unsigned char OutTime;
	unsigned char Index;
	FMBandStuct FMBand[FM_BAND_NUM];
	AMBandStuct AMBand[AM_BAND_NUM];

	unsigned int MaxFreq;
	unsigned int MinFreq;
	unsigned char Step;
	//unsigned char SeekStep;
	unsigned char Area;
	signed char Direction;

	unsigned int Smeter;
	unsigned char Multipath;
	unsigned char AdjChannel;
	unsigned char Detuning;
	unsigned char HITAddressBuff[3];
	unsigned char HITDataBuff[40];
	unsigned char IFBW;
	unsigned char IS;

	unsigned F_ForceSetBuff:1;
	unsigned F_GoodStation:1;
	unsigned F_SeekFullCycle:1;
	unsigned F_AutoSeek:1;
	unsigned F_TunerDispReq:1;
	unsigned F_ManualSeek:1;
	unsigned F_LocDx:1;
	unsigned F_Scan:1;
	unsigned F_Stereo:1;//---1: stereo; 0: mono
	unsigned F_AlignDone:1;// 1: aligne done
	unsigned F_TDBusy:1;// 1---Tuner driver busy
	unsigned F_QualDisplay:1;
	unsigned F_SeqChg:1;
	unsigned F_TDCOLL:1;
} Radio7786_stuct;
extern Radio7786_stuct Radio_7786;

typedef struct RDS_stuct {
	unsigned char BlockCnt;
	unsigned char LastBlockCnt;

	unsigned int BlockA;
	unsigned int BlockB;
	unsigned int BlockC;
	unsigned int BlockCp;
	unsigned int BlockD;
	unsigned int BlockLastA;
	unsigned int BlockLastB;
	unsigned int BlockLastC;
	unsigned int BlockLastCp;
	unsigned int BlockLastD;

	unsigned char RDSData[4];

	//unsigned F_GetOneGroup:1;
	unsigned F_GetBlockA:1;
	unsigned F_GetBlockB:1;
	unsigned F_GetBlockC:1;
	unsigned F_GetBlockCp:1;
	unsigned F_GetBlockD:1;
	unsigned F_BlockA:1;
	unsigned F_BlockB:1;
	unsigned F_BlockC:1;
	unsigned F_BlockCp:1;
	unsigned F_BlockD:1;
	unsigned F_RDSStation:1;
	unsigned F_GetNewTP:1;

//	unsigned F_RDSInt:1;
} RDSStuct;
extern RDSStuct RDS;

extern void  HITPushToLast(void);
extern void TunerInit(void);
extern void TunerStatusUpdate(void);
extern void TunerChangeBand( unsigned char  bandreq, unsigned int freq );
extern void Radio7786Turnup(void);
extern void Radio7786Turndown(void);
extern void HIT_ForeceMono();
extern void HIT_ForeceMute(u8 mute)
;

extern void TunerStartupSeq(void);
extern void TunerSetFreq(unsigned int freq);
extern void TunerSeekOn(signed long seekstep);
extern void TunerSeekOff(unsigned char SeekEndOption);
extern void TunerReadSeekStatus(void);
extern int  Elite_DSPStuckCheck(void);
extern void Elite_BootcodeDownload(void);
extern void Elite_GetAverageQuality (unsigned char AverageNum);
extern void Elite_CmdWriteRead(unsigned char cmdcode,  unsigned char para_num, unsigned char rece_num);
extern void Elite_DirectWrite(unsigned char DataNum, unsigned char mode, unsigned char cmd);//bool cmd)
extern void Elite_DirectRead(int DataNum, unsigned char mode);
extern void Elite_CmdStartup(unsigned char AlignMode);
extern void Elite_CmdChangeBand(unsigned char band, unsigned long minfreq, unsigned long maxfreq,unsigned long freq);
extern void Elite_CmdChangeFreq(unsigned long freq);
extern void Elite_CmdSetFEReg(unsigned char band, unsigned char start, unsigned char num);
extern void Elite_CmdReadTunerStatus(unsigned char band);
extern void Elite_CmdReadTDSR(void);
extern void Elite_CmdSetSeekTH(unsigned long MeasCycle, unsigned long fieldstrengthTH, unsigned long AdjTH, unsigned long DetuneTH, unsigned long MultipathTH, unsigned long FSscalingminTH,unsigned long FSscalingmaxTH, unsigned long AvgNum);
extern void Elite_CmdStartManualSeek(signed long  FreqIncrement);
extern void Elite_CmdStartAutoSeek(signed long FreqIncrement, unsigned char AutoSeekEnd, unsigned char AutoSeekMethod);
extern void Elite_CmdSeekEnd(unsigned char SeekEndAction);
extern void Elite_CmdReadSeekStatus(void);
extern unsigned char Elite_CmdReadMem(unsigned long menAddr, unsigned char Numword);
extern unsigned char Elite_CmdWriteMem(unsigned long menAddr, unsigned long menData);
extern void Elite_CmdWriteBeCoeff(unsigned char* p_be_coeff);


extern void Elite_CmdAFStart(unsigned long freq);
extern void Elite_CmdAFEnd(unsigned long freq);
extern void Elite_CmdAFCheck(unsigned long freq);
extern void Elite_CmdReadAFQual(void);



extern void Elite_CmdChangeAFFreq(unsigned long freq);

extern void Tuner10msTimer(void);
extern void Elite_Cmd_IBOC_Blender(void);
extern void Elite_CmdMuSICA(unsigned char val);
extern void Radio7786ChangeBand( unsigned char  bandreq, unsigned int freqreq );

typedef struct TunerStuct{
unsigned char WorkMode;
unsigned char WaitTimer10ms;
}TunerStuct;
extern TunerStuct Tuner;

/*Elite I2C communcation functions*/
extern void I2C3Start(void);
extern void I2C3Stop(void);
extern unsigned char I2C3Out8Bit(unsigned char outbyte);
extern unsigned char I2C3In8Bit(void);
extern void I2C3Ack(void);
extern void I2C3Ack2(void);


#define RADIO_OPERATE_OUTTIME		16


// KC Apr 2012 improve seek performance ++
#define FSscalingthrmin 205	//195 
#define FSscalingthrmax 215	//205  modified on Jan 2015
#define Adjfactor	0.6
// KC Apr 2012 improve seek performance -- 

#define TUNER_STARTUP_IDLE                      0x0
#define TUNER_STARTUP_REQ                       0x1
#define TUNER_STARTUP_WAIT                      0x2
#define TUNER_STARTUP_WAIT1                     0x3
#define TUNER_STARTUP_WAIT_DSPINIT              0x4
#define TUNER_STARTUP_WAIT_VCO_FE_ALIGN         0x5

#define FM                  0
#define AM 					1
#define WB					2
#define SW					3

//#ifdef I2C_C2_BOOT
#define ELITETUNER_I2C_ADDR   0xc2//write: c2; read c3
//#else
//#define ELITETUNER_I2C_ADDR   0xc8//write: c8; read c9
//#endif



#define ModeXX1             0	//No autoinc, no burst, 32bit format
#define ModeXX2             1	//No autoinc, no burst, 24bit format
#define ModeXX3             2	//autoinc, burst, 32bit format
#define ModeXX4             3	//autoinc, burst, 24bit format

#define CmdCode_ReadDMAmem               0x00
#define CmdCode_WriteDMAmem              0x03
#define CmdCode_Writemen                 0x1F
#define CmdCode_Readmen                  0x1E
#define CmdCode_ReadTDS                  0x11

//Tuner Commands
#define CmdCode_Startup                  0x22
#define AlignVCO_EUUSA                   0x00//87.5MHz-108MHz
#define AlignVCO_Japan                   0x01//76MHz-90MHz
#define AlignVCO                         0x02//only align VCO

#define CmdCode_ChangeBand               0x23
#define bandcode_FM                      0x01
#define bandcode_AM_EU_JP                0x02
#define bandcode_AM_US                   0x03
#define bandcode_WB                      0x04

#define CmdCode_ChangeFreq               0x24
#define InjectSide_Auto                  0x00
#define InjectSide_FixedLow              0x01
#define InjectSide_FixedHigh             0x02


#define CmdCode_ReadTunerStatus         0x25
#define bandcode_Read_FM                 0x00
#define bandcode_Read_AM                 0x01
#define bandcode_Read_WB                 0x02

#define CmdCode_SetFEReg                 0x09

#define CmdCode_SetSeekTH                0x14
#define CmdCode_StartManuSeek            0x15

#define CmdCode_AFCheck                  0x07
#define CmdCode_AFSwitch                 0x10
#define CmdCode_AFStart                  0x0d
#define CmdCode_AFMeasure                0x0e
#define CmdCode_AFEnd                    0x0f

#define CmdCode_StartAutoSeek            0x16
#define AutoEnd                           0x01
#define WaitCmdEnd                        0x00
#define OldAutoSeek                       0x00
#define NewAutoSeek                       0x01  //advised to use the new auto seek algorithm implemented inside the DSP


#define CmdCode_SeekEnd                  0x17
#define SeekContinue                     0x01
#define SeekStopMuted                    0x02
#define SeekStopUnmuted                  0x03

#define CmdCode_ReadSeekStatus           0x18
#define SeekStatus_RstQaul               0x01
#define SeekStatus_NoRstQaul             0x00
#define SeekStatus_ManualSeek            0x01
#define SeekStatus_AutoSeek              0x02
#define SeekStatus_ASE                   0x04
#define SeekStatus_Seekok                0x01
#define SeekStatus_FullCircle            0x02

//#define CmdCode_ReadFEReg                0x12
#define CmdCode_ReadRDSQual              0x13

#define CmdCode_ReadSTStatus          	 0x1f

/* DISS command */
#define CmdCode_SetDiss                  0x1a
#define Diss_FM_Auto                     0x00
#define Diss_FM_Manual                   0x01
#define Diss_FM_Special                  0x02
#define Diss_AM_Neutral                  0x0a
#define Diss_WX_Neutral                  0x0b
//BW for manual and special FM mode
#define Diss_FM_Manual_BW0               0x00//(narrowest)
#define Diss_FM_Manual_BW1               0x01
#define Diss_FM_Manual_BW2               0x02
#define Diss_FM_Manual_BW3               0x03
#define Diss_FM_Manual_BW4               0x04
#define Diss_FM_Manual_BW5               0x05
#define Diss_FM_Manual_BW6               0x06
#define Diss_FM_Manual_BW7               0x07//(middle)
#define Diss_FM_Manual_BW8               0x08
#define Diss_FM_Manual_BW9               0x09
#define Diss_FM_Manual_BW10              0x0a//(widest)



#define CmdCode_IBOC_Blender  0x06
#define CmdCode_MuSICA        0x20

#define BootData                         0x00
#define FMPatchData                      0x01
#define WXPatchData                      0x02
#define AMPatchData                      0x03
#define FMWSPData                        0x04
#define AMWSPData                        0x05
#define WXWSPData                        0x06

#ifdef Elite_RADIO_DEBUG
#define DEBUG_Elite_OUTVAL(x)              M_SCI_OUTVAL(x)
#define DEBUG_Elite_SENDSTRING(x)          SCI_SendString(x)
#endif

extern unsigned char HitTimer10ms;

/*********************************************************
				File End
*********************************************************/


