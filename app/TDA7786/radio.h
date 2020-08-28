/****************************************************************************************
* Car-audio software, define variables for Radio partion				*
* Author:  APICAL ELECTRONICS TECHNICAL Co.,LTD                                      		*
*---------------------------------------------------------------------------------------*/

#ifdef   Rad_ROOT
#define  Rad_EXT
#else
#define  Rad_EXT     extern
#endif
#define C_RADIO		0x01
#define		_rSi475xDataMax	7

//#define		_P6601CkStSmeter	1
#define		_Si475xCkStIFC1		1
#define		_Si475xCkStIFC2		2
//#define		_Si475xCkStIFC3		3
#define		_Si475xStationOk	3

#define	_Si47xxFMRSSILevel		15//32//22      //~16DB
#define	_Si47xxAMRSSILevel		33//28     // ~20DB
#define	_Si47xxFMRSSILevelLoc	_Si47xxFMRSSILevel + 25//about +18dB


#define	_Si47xxFMSNRLevel		9//206//r6601EeromData[1]/*Addr: 0x01*/
#define	_Si47xxAMSNRLevel		10//(12 - 0)//206//r6601EeromData[1]/*Addr: 0x01*/

#define	_Si47xxFMUSNRLevel		25//25
#define	_Si47xxFMWAMRLevel	18//20//26

#define	_Si47xxAMUSNRLevel		20
#define	_Si47xxAMWAMRLevel	26


Rad_EXT	uchar	FM_RDS_TP;
Rad_EXT	uchar	FM_RDS_PTY;
Rad_EXT	uint	FM_RDS_PI;
Rad_EXT	uchar	FM_RDS_RdsFifoUsed;
Rad_EXT	uchar	FM_RDS_BleA;
Rad_EXT	uchar	FM_RDS_BleB;
Rad_EXT	uchar	FM_RDS_BleC;
Rad_EXT	uchar	FM_RDS_BleCp;
Rad_EXT	uchar	FM_RDS_BleD;

Rad_EXT	uint		FM_RDS_BlockA;
Rad_EXT	uint		FM_RDS_BlockB;
Rad_EXT	uint		FM_RDS_BlockC;
Rad_EXT	uint		FM_RDS_BlockCp;
Rad_EXT	uint		FM_RDS_BlockD;
Rad_EXT 	uchar	rSi475xCkStationSt;
Rad_EXT 	uchar	cDTSParmIndex;
Rad_EXT	uchar		rSi475xDataBuff1[_rSi475xDataMax];

typedef struct Band_conststuct {
	unsigned int BandFreq;
	unsigned int MemFreq[6];
	unsigned char CurrentMem;
} BandConstStuct;

typedef struct Area_stuct {
	BandConstStuct Band[6];
	unsigned char FMStep;
	unsigned char FMSeekStep;
	unsigned int FMMaxFreq;
	unsigned int FMMinFreq;
	unsigned char FMStepOirt;
	unsigned int FMMaxFreqOirt;
	unsigned int FMMinFreqOirt;
	unsigned char AMStepMW;
	unsigned int AMMaxFreqMW;
	unsigned int AMMinFreqMW;
	unsigned char AMStepLW;
	unsigned int AMMaxFreqLW;
	unsigned int AMMinFreqLW;
     unsigned int AMStepSW;
     unsigned int AMMaxFreqSW;
     unsigned int AMMinFreqSW;
	unsigned char FMStepWB;
	unsigned int FMMaxFreqWB;
	unsigned int FMMinFreqWB;
} AreaStuct;




// ***Note: this buff must fixed at tiny area
Rad_EXT uchar		Com10Buff[10];
#define	R_L		Com10Buff[0]
#define	R_M		Com10Buff[1]
#define	R_H		Com10Buff[2]


#define		rTempi		R_L
#define		rTempj		R_M
#define		rTempt		R_H


Rad_EXT	uint		FMPresetMem[4][6];

Rad_EXT	uint		AMPresetMem[2][6];

#if 1
Rad_EXT	uint		FMAsMem[2*3*6];
Rad_EXT	uint		AMAsMem[2*2*6];
Rad_EXT	uint		FMAsStat;
Rad_EXT	uint		AMAsStat;
#else
#endif
/* 
 *	radio variables define, section 1 
 */
Rad_EXT  uint	      		BandMsgMem[6];
Rad_EXT	uchar		cBand;
Rad_EXT	uchar		cReqBand;

Rad_EXT	uchar		cBandBak;

#define	_BandFM1		0x0
#define	_BandFM2		0x1
#define	_BandFM3		0x2
#define	_BandAM1		0x3
#define	_BandAM2		0x4

Rad_EXT	TYPE_WORD		cRadStatNum;
#define	cRadStatL	cRadStatNum.byte[0]
#define	cRadStatH	cRadStatNum.byte[1]

Rad_EXT	TYPE_WORD	ArmTxFreData;

#define	_RefAMBand		0x40
#define	_RefFMBand		0x30
#define	_RefLWBand		0x00
#define	_RefOIRTBand	0x30

Rad_EXT TYPE_WORD		OriginStNum;
#define	OrignStNumL	OriginStNum.byte[0]
#define	OrignStNumH	OriginStNum.byte[1]

Rad_EXT TYPE_WORD		cRadStatBak;
#define	cRadStatBakL	cRadStatBak.byte[0]
#define	cRadStatBakH	cRadStatBak.byte[1]


Rad_EXT	uchar		cPSNum;	
Rad_EXT	uchar		cReqPSNum;	

	/*Above messages need to save when battery off*/
Rad_EXT	uchar		cFMASNum;	
Rad_EXT	uchar		cFMOrtNum;	
Rad_EXT	uchar		cAMASNum;	
Rad_EXT	uchar		cSWASNum;	

Rad_EXT	uchar	cPSNumBak;


Rad_EXT	uchar		SMeterBuff[6]; 


//Rad_EXT	uchar		SMeterBuff[6]; 

Rad_EXT	TYPE_WORD	FreData1;

//#define	SMeterBuff	CdDaTrkMem

			
Rad_EXT	uchar		rRadMainSt;
Rad_EXT	uchar		rRadSubSt;
/*
 *    radio procedures status define
 */
/*main procedure*/
#define		_RadIdle		0
#define		_RadPwrOff		1
#define		_RadInit		2
#define		_RadDlToStdby	3	
#define		_RadStdby		4
#define		_ManuTHFDl		5
#define		_ChgBand		6
#define		_RadPS			7
#define		_RadAS			8
#define		_AutoTUp		9
#define		_AutoTDn		10
#define		_RadScan		11



/*#define	Radio_X64ms	CD_X64ms  radio main timer*/
Rad_EXT	uchar		Rad_X64ms;/*radio sub-timer*/		
Rad_EXT	uchar		Rad_X4ms;	
Rad_EXT	uchar		Rad_Seek_X4ms;	
Rad_EXT	uchar		Rad_Err_X4ms;	
Rad_EXT	uint		Rad_ChkStat_X4ms;	
Rad_EXT	uchar		Rad_SMeter_X64ms;	
Rad_EXT	uchar		Rad_SMeter_X256ms;	
#define		ks_X64ms	Rad_X64ms


Rad_EXT	uint		rRadMeterLevel;
Rad_EXT	uchar		rRadSTLevel;

Rad_EXT	uchar		rRAD_WAM;
Rad_EXT	uchar		rRAD_USN;
Rad_EXT	uchar			rRAD_SNR;

Rad_EXT	uchar		rRadFreOffset;

Rad_EXT	uchar	Manu5STmr_X512ms;
Rad_EXT	uchar	rPrgmPllSt;	
Rad_EXT	uchar	ChkStatSt;
//#define	ChkStatSt	rPrgmPllSt

Rad_EXT	uchar		rRadDispSt;	
Rad_EXT	uchar		DevInitSt;

/*#define		rRadDispSt	rSubDpPoint	add for rds*/
#define	_DpRadFreq		0x0
#define	_DpRadRdsPs		0x1
#define	_DpRadPty		0x2
#define	_DpRadPtyT		0x3
#define	_DpRadNoPty		0x4
#define	_DpRadManu		0x5
#define	_DpRadAuto		0x6
#define	_DpRadPiSeek	0x7
#define	_DpRadTaSeek	0x8
#define	_DpRadTraffic	0x9
#define	_DpRadEonTa		0x10
#define	_DpRadAlarm		0x11
#define	_DpRadRegOn		0x12
#define	_DpRadRegOff	0x13
#define	_DpRadNoTpTa	0x14
#define	_DpRadPsPty		0x15

Rad_EXT	uchar		rRadTmpDpSt;	
Rad_EXT uchar	rPtyMusicSum,rPtySpeechSum;


/*boolean variables define*/
Rad_EXT	TYPE_BYTE		rRadFlg0;	
#define	bAM_FM			rRadFlg0.bit.b0/*0: FM band, 1: AM band*/
#define	bMonoOn			rRadFlg0.bit.b1/*0: mono off, 1: on*/
#define	bLOCEn			rRadFlg0.bit.b2/*0: disable, 1: enable*/
#define	bRadFreqChgd	rRadFlg0.bit.b3
#define	bSTCkSt			rRadFlg0.bit.b4/*1: unstable status*/	
#define	bSTOff			rRadFlg0.bit.b5/*0: stereo station, 1: mono station*/
#define	bRadSeeking		rRadFlg0.bit.b6/*radio is seeking station*/
#define	bPrgmPll		rRadFlg0.bit.b7/*program PLL is doing*/		

Rad_EXT	TYPE_BYTE		rRadFlg1;
#define	bPllStable		rRadFlg1.bit.b0/*1: indicator PLL is stable status*/
#define	bChkStatEn		rRadFlg1.bit.b1/*1: check station procedure enable execution*/
#define	bStatPresent	rRadFlg1.bit.b2/*after check station complete, this flag is
                              		1: indicate valid radio station, 0: invalid station*/
#define	bIFCnting		rRadFlg1.bit.b3/*1: IFC doing*/	
#define	bRadSeek2nd		rRadFlg1.bit.b4	
#define	bRadHopFreq		rRadFlg1.bit.b5	// need delay
#define	bManuAuto		rRadFlg1.bit.b6/*0: auto tune, 1: manual*/
#define	bASStatPresent		rRadFlg1.bit.b7
                                
Rad_EXT	TYPE_BYTE		rRadFlg2;
#define	b6601InitFlg	rRadFlg2.bit.b0
#define	bTEF6606		rRadFlg2.bit.b1
#define	iExtSTOff		rRadFlg2.bit.b2
#define	bRadPsNumRead	rRadFlg2.bit.b3	/*1: */	
#define b7786MuteOn		rRadFlg2.bit.b4
#define	bWtChgBand		rRadFlg2.bit.b5
#define	bRadAvailStat		rRadFlg2.bit.b6
#define	bDpSTICON/*bRadSignalFail*/		rRadFlg2.bit.b7



Rad_EXT TYPE_BYTE		rRadFlg3;
#define	bRDSDs			rRadFlg3.bit.b0
#define	bSysMuteSt		rRadFlg3.bit.b1
#define	bRadWrPllEn		rRadFlg3.bit.b2

#define	bTuneEncDec		rRadFlg3.bit.b3
#define	bUserMute		rRadFlg3.bit.b4
#define	bFirstPwrOn		rRadFlg3.bit.b5
#define	bChgSrcToRad	rRadFlg3.bit.b6
#define	bChgSrcToBak	rRadFlg3.bit.b7













#if RadParaAdj
#define	bTunerInit24c01	rRadFlg2.bit.b4
#endif

Rad_EXT	TYPE_BYTE		rRadStatFlg0;	
Rad_EXT	TYPE_BYTE		rRadStatFlg1;	
Rad_EXT	TYPE_BYTE		rRadStatFlg2;
Rad_EXT	TYPE_BYTE		rRadStatFlg3;


//#define	b6601InitFlg	rRadStatFlg0.bit.b0
//#define	bTEF6606	rRadStatFlg0.bit.b1
//#define	iExtSTOff		rRadStatFlg0.bit.b2
//#define	b6601InitFlg	rRadStatFlg0.bit.b3
//#define	bTEF6606	rRadStatFlg0.bit.b4
//#define	iExtSTOff		rRadStatFlg0.bit.b5
//#define	b6601InitFlg	rRadStatFlg0.bit.b6
//#define	bTEF6606	rRadStatFlg0.bit.b7


/*Radio Check Station state Define*/
#define	_ChkStatInit		0
#define 	_CkStStartDl		1
#define 	_CkStStartDl1	2
#define 	_CkStAmEnd		3/*for si4730*/



#define 	_CkStStartD5	5
#define		_CkWaitRds		7		/*add for rds*/
#define		_CkRdsChk		8		/*add for rds*/


/*auto preset memory store procedure status define*/
#define		_asWrPll		1
#define		_asWtPllStable	2
#define		_asWtChkSt		3
#define		_asWtChgToPs	4
#define		_asWrFirstFre	5


#if 0
#define	_emRadStartAddr	_emSysEndAddr + 1
/*S
      radio status(current band message, 18 preset station) save format in EEROM as following:
 area & cBand,		   1 bytes,
 FM Preset Memory(36 bytes)
 AM Preset Memory(24 bytes)	
 Band message Memory 10 bytes
 
    Total occupied units : 72
*/
#define	_emBandAddr		_emRadStartAddr + 00		// 09, band address

#define	_emFM1PresetAddr	_emBandAddr + 01		// 10
#define	_emFM2PresetAddr	_emFM1PresetAddr + 12	// 22
#define	_emFM3PresetAddr	_emFM2PresetAddr + 12	// 34
	/*total 36 units*/
#define	_emAM1PresetAddr	_emFM3PresetAddr + 12	// 46
#define	_emAM2PresetAddr	_emAM1PresetAddr + 12	// 58

#define	_emFM1BandMsgAddr	_emAM2PresetAddr + 12	// 70
#define	_emFM2BandMsgAddr	_emFM1BandMsgAddr + 2	// 72
#define	_emFM3BandMsgAddr	_emFM2BandMsgAddr + 2	// 74
#define	_emAM1BandMsgAddr	_emFM3BandMsgAddr + 2	// 76
#define	_emAM2BandMsgAddr	_emAM1BandMsgAddr + 2	// 78

#define	_emFMLevelAddr		_emAM2BandMsgAddr + 2	// 80
#define	_emAMLevelAddr		_emFMLevelAddr + 1	// 81

#define	_emRadEndAddr		_emAMLevelAddr + 1	// 82
#endif

Rad_EXT	void	SetRadPwrOff(void);
Rad_EXT	uchar	RadListenCk(void);
Rad_EXT	void	SetRadPwrDn(void);
Rad_EXT	void	RadioInit(void);
Rad_EXT	void	UserMuteProc(void);
Rad_EXT	uchar	RadSpKyCk(uchar D);
Rad_EXT	void	RadioProc(void);
//Rad_EXT	void	RadTmpDisp(uchar rTmpDpSt,uchar rTmpTime);
Rad_EXT	uint	StatNumMulN(uchar d);
Rad_EXT	uint GetRadFreOctData(uint McuFreX16);
Rad_EXT	void GetRadPlayFre(uint ArmFreX16);


Rad_EXT	void si47xx_set_property(uint propNumber, uint propValue);

Rad_EXT	const	uchar	*TunParmStartAddr(void);
Rad_EXT	const	uchar	*TunFMParmStartAddr(void);
Rad_EXT	const	uchar	*TunAMParmStartAddr(void);
Rad_EXT	const	uchar	*TunFMORITParmStartAddr(void);

Rad_EXT void	SaveCurBandMsg(void);
Rad_EXT uchar GetIntStatus(void);
Rad_EXT void	WriteSi4730Pll(void);
Rad_EXT void	RadKyAnaly(void );
Rad_EXT void	StartChgBand(uchar b);
Rad_EXT	void RadSetToDl500ms(uchar d);
Rad_EXT void	ReadPresetMem(uchar band,uchar n);		
Rad_EXT void	StartWrPll(void);
Rad_EXT uint	GetMaxStat(void);


/****************************APICAL ELECTRONICS-TECHNOLOGY Co.,LTD*********************************/
/*************************************End of file***************************************/
