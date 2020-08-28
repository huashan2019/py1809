/********************************************************************************\
*	Car audio RDS head file												*
*             	Language           : C-Like.				     				*
* Author:  SAMWELL TECHNICAL Co.,LTD                                      		*
\*------------------------------------------------------------------------------*/

#ifdef   RDS_ROOT
#define  RDS_EXT
#else
#define  RDS_EXT     extern
#endif

//#define	_OnlyReduceRAM
#define SetBit(VAR,Place)         ( VAR |= (1<<Place) )
#define ClrBit(VAR,Place)         ( VAR &= ((1<<Place)^255) )
#define ValBit(VAR,Place)         ( VAR & (1<<Place) )

/*
 *   <><> variables group1 define <><> 
 */
 #define		EableRDSSwitch()		 //ArmTxCmd(C_RADIO, 0x0010)
 #define		RDSTxStatName()			 ReportRdsPsinfo()//ArmTxCmd(C_RADIO, 0x000a)
 #define		RDSTxStatRT()			 ReportRdsRTinfo()
 #define		RDSTxRDSFlag()			 //ArmTxCmd(C_RADIO, 0x0006)
 #define		RDSTxStatNum()			 //ArmTxCmd(C_RADIO, 0x0004)

 #define		RDSTxStatMEM()			 //ArmTxCmd(C_RADIO, 0x0002)

 #define		RDSTxPTYType()			 ReportRdsPsinfo()//ArmTxCmd(C_RADIO, 0x0008)
 #define		RDSTxSigLevel()			 //ArmTxCmd(C_RADIO, 0x00fe)
 
RDS_EXT uchar 	cRdsPiChkCnt;
RDS_EXT	uchar 		rRdsMainSt;

/*RDS main status define */
#define	_PtyOff			0	
#define	_PtyOn			1
#define	_PtySeek		2
#define	_TpSeek			3
#define	_PiSeek			4
#define	_Traffic			5
#define	_AMStat			6

RDS_EXT	uchar 		rRdsAfDetSt;
#define	_AfDetLevIdle	    	0
#define	_AfDetLev  	  	1
#define	_AfDetLevEnd  	  2

/*#define	rRdsAfSt	rRdsSubSt*/
/*Af Scan station*/
RDS_EXT	uchar 		rRdsAfSt;
#define	_AfIdle	    	0
#define	_AfWrPll    	1
#define	_AfWrPllDly    	2
#define	_AfWtPllLk		3
#define	_AfWtChkSt		4
/*#define	_AfRestore		4*/
#define	_AfSearchEnd	5

/* RDS sub status define*/
RDS_EXT	uchar 		rRdsSubSt;

#define	rPtySeekSt	rRdsSubSt
/*PI Seek */
#define	_PtySeekWait	0
#define	_PtySeekStart	1
#define	_PtySeeking		2
#define	_PtySeekend		3

#define	rTpSeekSt	rRdsSubSt
/*Af Scan station*/
#define	_TpSeekWait		0
#define	_TpSeeking		1
#define	_TpSeekEnd		2


#define	rPiSeekSt	rRdsSubSt
/*PI Seek */
#define	_PiSeekWait		0
#define	_PiSeeking		1
#define	_PiSeekEnd		2

#define	rTrafficSt	rRdsSubSt
/*PI Seek */
#define	_TrafficInit	0
/*#define	_TrafficMute1	1*/
#define	_TrafficOn		1
#define	_TrafficEnd		2
/*#define	_TrafficMute2	4
#define	_TrafficEnd2	5*/


RDS_EXT	TYPE_BYTE 		rRdsFlag;
#define	bRdsAfDetEn		rRdsFlag.bit.b0
#define 	bRdsTAEn		rRdsFlag.bit.b1
#define	bRdsPtyEn		rRdsFlag.bit.b2
//#define 		rRdsFlag.bit.b3	/* 0:speech,1:music*/
#define bRdsRegOn		rRdsFlag.bit.b4	/* 0:Reg off,1:Reg on*/
//#define 	rRdsFlag.bit.b5	/* 0:EON DX,1:EON LO*/
//#define 	rRdsFlag.bit.b6	/* 0:first,1:second*/
//#define 		rRdsFlag.bit.b7	/* 0:first,1:second*/

RDS_EXT	TYPE_BYTE 		rRdsFlag1;
#define	REAL_F_GetOneGroup	rRdsFlag1.bit.b0
#define	REAL_F_RDSStation	rRdsFlag1.bit.b1
#define	bTrafficOn		rRdsFlag1.bit.b2
#define	bEonTrafficOn	rRdsFlag1.bit.b3
#define	bPtyAlarm		rRdsFlag1.bit.b4
#define	bRdsClkEn		rRdsFlag1.bit.b5
#define	bEonTaTAEn 		rRdsFlag1.bit.b6
#define 	bRdsEONAvail	rRdsFlag1.bit.b7	/* 0:EON absent,1:exist*/

RDS_EXT	TYPE_BYTE 		rRdsFlag2;
#define bMeasureAfSig	rRdsFlag2.bit.b0	
#define	bPtyChged		rRdsFlag2.bit.b1/*bEonTaAfDetEn*/
#define bPiSeeking		rRdsFlag2.bit.b2
#define	bEonTaMute		rRdsFlag2.bit.b3
#define	bEWSAlarm		rRdsFlag2.bit.b4
#define bRdsPiAvail		rRdsFlag2.bit.b5
#define bRdsAfAvail		rRdsFlag2.bit.b6
#define	bInvalidPi		rRdsFlag2.bit.b7


RDS_EXT	TYPE_BYTE 		rRdsFlag3;
#define	bTAAlarmSeek  	rRdsFlag3.bit.b0
#define	bRdsEnBak		rRdsFlag3.bit.b1
#define	bSeekDn			rRdsFlag3.bit.b2
#define	bTpFlag			rRdsFlag3.bit.b3
#define bBand0PSAvail	rRdsFlag3.bit.b4
#define bBand1PSAvail	rRdsFlag3.bit.b5
#define bBand2PSAvail	rRdsFlag3.bit.b6
#define	bRdsCTAvail		rRdsFlag3.bit.b7

RDS_EXT	TYPE_BYTE 		rRdsFlag4;
#define	bPtyType  			rRdsFlag4.bit.b0
#define	bRdsAfDetFlg		rRdsFlag4.bit.b1
#define	bUserTaDisable		rRdsFlag4.bit.b2
#define	bPtyMType			rRdsFlag4.bit.b3
#define	bTaAlarmFlg			rRdsFlag4.bit.b4
#define bRdsAFPiAvail		rRdsFlag4.bit.b5
#define F_RDSInt			rRdsFlag4.bit.b6
//#define	bRadWritePll		rRdsFlag4.bit.b7

RDS_EXT	TYPE_BYTE 		rRdsFlag5;
#define	REAL_F_BlockA  		rRdsFlag5.bit.b0
#define	REAL_F_BlockB		rRdsFlag5.bit.b1
#define	REAL_F_BlockC		rRdsFlag5.bit.b2
#define	REAL_F_BlockCp		rRdsFlag5.bit.b3
#define	REAL_F_BlockD		rRdsFlag5.bit.b4
#define REAL_F_GetNewPTY	rRdsFlag5.bit.b5
#define REAL_F_GetNewTP		rRdsFlag5.bit.b6
#define	REAL_F_GetNewTA		rRdsFlag5.bit.b7

RDS_EXT	TYPE_BYTE 		rRdsFlag6;
#define	REAL_TP  		rRdsFlag6.bit.b0
#define	REAL_TPLast		rRdsFlag6.bit.b1
#define	REAL_TA			rRdsFlag6.bit.b2
#define	REAL_TALast		rRdsFlag6.bit.b3
#define	REAL_F_GetNewPI		rRdsFlag6.bit.b4
#define bRdsRTABSto			rRdsFlag6.bit.b5
//#define REAL_F_GetNewTP		rRdsFlag6.bit.b6
//#define	REAL_F_GetNewTA		rRdsFlag6.bit.b7



RDS_EXT	TYPE_WORD		rRdsCurSR32;	/*shift register (16 bits)*/
#define	rRdsCurSR3	rRdsCurSR32.byte[1]
#define	rRdsCurSR2	rRdsCurSR32.byte[0]

//RDS_EXT	TYPE_BYTE		rRdsCurSR3;	/*shift register (16 bits)*/
//RDS_EXT	TYPE_BYTE		rRdsCurSR2;	

RDS_EXT	uchar		rRdsCrch;
RDS_EXT	uchar		rRdsCrcl;         /* CRC syndrome value.*/
//
RDS_EXT	uchar		cRdsCnt;		/*µ±r0RdsSyncSt =  _BitSyncÊ±£¬cRdsBits = cRdsCnt*/
RDS_EXT	uchar		cRdsBits;	/*RDS Bit counter.*/
#define _BlkLength     26                 /* Number of bits in one RDS block.*/
#define _BlkWindows   208  /* Bit synchro window value (208 bits = 2 groups).*/


/*RDS_EXT	char	cRdsBlks;	RDS Block counter.*/
RDS_EXT	uchar		rRdsBlock;	/*Block code (A, B, C, C' or D) */
#if 0
#define	_BlockA      	0x00                              
#define	_BlockB      	0x01                              
#define	_BlockC      	0x02                              
#define	_BlockCp     	0x03                              
#define	_BlockD      	0x04 
#else
#define	_BlockA      	0x01                              
#define	_BlockB      	0x02                              
#define	_BlockC      	0x03                              
#define	_BlockCp     	0x05                              
#define	_BlockD      	0x04 
#endif
#define	_BlockE      	0x05 	/*USA different services.  */    
#define	_Empty			0x06 	/*Internal use only (BIT_SYNC) */
#define	_Wrong	  		0x07  


RDS_EXT	uchar		rRdsSyncLevel;                    /* Block synchro critera level.*/
#define _FirstSyncLevel  3 /* First value of the Sync Level after bit synchro.*/
#define _SyncLvlInc  	 2 /* SyncLevel Increment value when a new good block.*/
#define _SyncLvlDec  	 1    /* SyncLevel Decrement value when a wrong block.*/

/* R D S   -   P S   D A T A   M A N A G E M E N T --------------------------*/
RDS_EXT	TYPE_BYTE 		rRdsPsStat;             /* => PS1 PS2 PS3 PS4  PS NEW CE1 CE0 */
#define bRdsPSSeg1  	rRdsPsStat.bit.b7   /* PS  : PS Segment1 available.               */
#define bRdsPSSeg2  	rRdsPsStat.bit.b6   /* PS  : PS Segment2 available.               */
#define bRdsPSSeg3  	rRdsPsStat.bit.b5   /* PS  : PS Segment3 available.               */
#define bRdsPSSeg4  	rRdsPsStat.bit.b4   /* PS  : PS Segment4 available.               */
#define bRdsPSAvail  	rRdsPsStat.bit.b3   /* PS  : PS available for appli.               */
#define bRdsPSNew    	rRdsPsStat.bit.b2   /* NEW : New PS stored and available for appli.*/

RDS_EXT	TYPE_BYTE 		rRdsRTStat;
#define bRdsRTABAct    	rRdsRTStat.bit.b6   /*A/B ver*/
#define bRdsRTAvail    	rRdsRTStat.bit.b5   /*A/B ver*/
#define bRdsRTNew    	rRdsRTStat.bit.b4   /*A/B ver*/

RDS_EXT u32  rRdsRTFlag;

/*Disp key toggle display item*/ 
RDS_EXT	TYPE_BYTE		rRdsGbsStat;  /* Software GBS Control/Status register.*/
#define CNB1  	bit.b7                      /*   7    6    5    4   3   2   1   0   */
#define CNB0  	bit.b6                      /*  CNB1 CNB0      VSI ORD BL2 BL1 BL0  */
#define BlkSync	bit.b5
#define ORD   	bit.b4
#define VSI   	bit.b3
#define BL2   	bit.b2
#define BL1   	bit.b1
#define BL0   	bit.b0

RDS_EXT	uchar		rRdsSyncSt;
#define _LeadIn   0x01          /* First RDS clock detection : initial state.*/
#define _BitSync  0x02          /* Bit synchronisation state.*/
#define _BlkSync  0x03          /* Block synchronisation state.*/

//RDS_EXT	uchar		rRdsSR[4];
RDS_EXT	unsigned long		rRdsSR;

//#define rRdsData16h  rRdsSR[3]	/* 16 bit data registers.*/
//#define rRdsData16l  rRdsSR[2]
//#define rRdsChks10h  rRdsSR[1]	/* 10 bit checkword registers.*/
//#define rRdsChks10l  rRdsSR[0]


RDS_EXT	uchar		cRdsBitStk[3];           /* -bit counter.*/

RDS_EXT	uchar		rRdsBlkStk[3];          /* Last 3 blocks reconized: -block type. */
RDS_EXT	TYPE_BYTE 	rRdsAfCtrl1;               /* AF control register 1: see after.*/
RDS_EXT	uchar 	rRdsRevdAfNum;               /* Nb of AFs already received.*/
RDS_EXT	uchar 	rRdsStorAfNb;               /* */
RDS_EXT	uchar 	rRdsAFStatBak;

#define rRdsTotalAfNb   (rRdsAfCtrl1.byte&0x1F)  /* AF nb in current list.*/
#define bRdsMethDet		rRdsAfCtrl1.bit.b6	 	/* A/B Method detected flag.*/
#define bRdsMethB     	rRdsAfCtrl1.bit.b7	    /* A/B Method info flag.*/

RDS_EXT	uchar 	rRdsAfFirst;         /* First AF extracted from the current list.*/
#define	_MaxAfList	25
RDS_EXT	uchar 	rRdsDafFmFreq[_MaxAfList];          /* FM AF dynamic frequency table.*/
RDS_EXT	uchar 	rRdsDafFmFreqCnt[_MaxAfList];          /* FM AF dynamic frequency table.*/

#define _FillerCode		205
RDS_EXT	uchar		rRdsDispSt;
#define		_RdsDpClk	0
#define		_RdsDpPty	1
#define		_RdsDpFreq	2
#define		_RdsDpPs	3
#define		_RdsDpSrc	4


RDS_EXT	uchar		RdsExtr_X4ms;
RDS_EXT	uchar		RdsSync_X8ms;
RDS_EXT	uchar		RdsPty_X32ms;
RDS_EXT	uchar		RdsPty_X128ms;
RDS_EXT	uchar		rRdRDS_X4ms;

RDS_EXT	uchar		RdsTraffic_X64ms;
RDS_EXT	uchar		RdsMeasure_X16ms;
RDS_EXT	uchar		RdsReceive_X32ms;
RDS_EXT	uchar		RdsTpSeek_X500ms;
RDS_EXT	uchar		RdsPiSeek_X500ms;
RDS_EXT	uchar		RdsMsAf_X500ms;
RDS_EXT	uchar		RdsMsAf_X128ms;
RDS_EXT	uchar		RdsMeasure_X32ms;

RDS_EXT	unsigned int REAL_PI;
RDS_EXT	unsigned int REAL_PILast;
RDS_EXT	unsigned int REAL_PILast2;
RDS_EXT	unsigned int REAL_PIBackup;
RDS_EXT	unsigned int REAL_PISeekBackup;
RDS_EXT	unsigned int REAL_PISeekTimer;
RDS_EXT	unsigned char REAL_PTY;
RDS_EXT	unsigned char REAL_PTYLast;

	
RDS_EXT unsigned char REAL_RDSBufferReadTimer;

RDS_EXT	TYPE_BYTE 		rRdsExtStat;
#define	bRdsCTCReady			rRdsExtStat.bit.b2		/* CT block C segment availibility bit.*/
#define	bRdsCTBReady            rRdsExtStat.bit.b3       /* CT block B segment availibility bit */
#define	bRdsRTReady             rRdsExtStat.bit.b4       /* RT segment availibility bit         */
#define	bRdsEONCReady           rRdsExtStat.bit.b5       /* EON block C segment availibility bit*/
#define	bRdsEONBReady           rRdsExtStat.bit.b6       /* EON block B segment availibility bit*/
#define	bRdsPSReady		        rRdsExtStat.bit.b7     	/* PS segment availibility bit.*/


/* R D S   -   T P / T A   D A T A   M A N A G E M E N T --------------------*/
RDS_EXT	TYPE_BYTE 	 	rRdsTaTpStat;         		/* TP & TA info ctrl/status register.*/
#define RdsTAValue    b4   				/*  7   6   5    4      3      2      1      0  */
#define bRdsTACtrl     	rRdsTaTpStat.bit.b5   /*            |<----TA----->|<----TP----->| New */
#define RdsTPValue    b3    				/*  X   X   X  Value Control Value Control      */
#define bRdsTPCtrl     	rRdsTaTpStat.bit.b1
#define bRdsTATPNew    	rRdsTaTpStat.bit.b7
#define bRdsTPPTYVal    	rRdsTaTpStat.bit.b6


#define bRdsTACurVal    rRdsCurSR32.bit.b4	/* Current TA value.*/
#define bRdsTAPreVal    rRdsTaTpStat.bit.b4	/* Previous one.*/
#define bRdsTPCurVal    rRdsCurSR32.bit.b10	/* Current TP value.*/
#define bRdsTPPreVal    rRdsTaTpStat.bit.b3	/* Previous one.*/

RDS_EXT	char 	cRdsTpCnt;

/* R D S   -   M S   D A T A   M A N A G E M E N T --------------------------*/
/*RDS_EXT	char 	rRdsMsStat;              Music/Speech info ctrl/status reg.*/
/*#define RdsMSValue    3          7   6   5   4    3     2      1      0 */
/*#define bRdsMSChange   	rRdsMsStat.2                         |<---------MS------->|   */
/*#define bRdsMSCtrl     	rRdsMsStat.1          X   X   X   X  Value Change Control  X */ 

/*#define bRdsMSCurVal   	rRdsCurSR2.3   Current MS value.*/
/*#define bRdsMSPreVal   	rRdsMsStat.3    Previous one.*/


/* R D S   -   P T Y   D A T A   M A N A G E M E N T ------------------------*/
RDS_EXT	TYPE_BYTE 	 	rRdsPtyStat;           /* Programme type info ctrl/status reg.*/
#define bRdsPtyCtrl    	rRdsPtyStat.bit.b6          /* 7     6      5      4   3   2   1   0  */
#define bRdsPtyChange  	rRdsPtyStat.bit.b5          /* X  Control Change |<------Value----->| */
#define RdsPtyValue    	(rRdsPtyStat.byte & 0x1F)

/* R D S   -   G R O U P   M A N A G E M E N T ------------------------------*/
RDS_EXT	uchar 		rRdsGroup;				/*curent RDS_EXTracted group code.*/
#define	_Group0A    	0x00    		/* 0A/0X   : PI, PTY, TP, PS, TA, DI, M/S, AF.     */
#define	_Group0B    	0x01    		/* 0B      : PI, PTY, TP, PS, TA, DI, M/S.         */
#define	_Group1A    	0x02    		/* 1A/1X   : PI, PTY, TP, PIN.                     */
#define	_Group1B    	0x03    		/* 1B      : PI, PTY, TP, PIN.                     */
#define	_Group2A    	0x04    		/* 2A/2X   : PI, PTY, TP, RT.                      */
#define	_Group2B    	0x05    		/* 2B      : PI, PTY, TP, RT.                      */
#define	_Group3A    	0x06    		/* 3A/3X   : PI, PTY, TP, not define yet.          */
#define	_Group3B    	0x07    		/* 3B      : PI, PTY, TP, not define yet.          */
#define	_Group4A    	0x08    		/* 4A/4X   : PI, PTY, TP, time and date.           */
#define	_Group5A    	0x0a    		/* 5A/5X   : PI, PTY, TP, RDS_EXTernal data use.       */
#define	_Group5B    	0x0b    		/* 5B      : PI, PTY, TP, RDS_EXTernal data use.       */
#define	_Group6A    	0x0c    		/* 6A	   : PI, PTY, TP, internal application.    */
#define	_Group6B    	0x0d    		/* 6B      : PI, PTY, TP, internal application.    */
#define	_Group7A    	0x0e    		/* 7A      : PI, PTY, TP, radio messagery.         */
#define	_Group8A    	0x10    		/* 8A      : PI, PTY, TP, not complete yet.        */
#define	_Group9A    	0x12    		/* 9A      : PI, PTY, TP, emergency warning system.*/
#define	_Group14A   	0x1c   		 	/* 14A	   : PI, PTY, TP, EON.                     */
#define	_Group14B   	0x1d   		 	/* 14B     : PI, PTY, TP, EON.                     */
#define	_Group15B   	0x1f   		 	/* 15B     : PI, PTY, TP (2 times).                */

/* R D S   -   P S   D A T A   M A N A G E M E N T --------------------------*/
RDS_EXT	uchar 		rRdsPsNameAct[8];				/* Current active Prog Station Name.*/
RDS_EXT	uchar 		rRdsPsNameSto[8];				/* current stored PS Name.*/
/*RDS_EXT	char 	rRdsBakPsName[8];				 Backup current PS Name.*/

/* R D S   -   P S   D A T A   M A N A G E M E N T --------------------------*/
RDS_EXT	uchar 		rRdsRTNameAct[64];				/* Current active Prog Station Name.*/
RDS_EXT	uchar 		rRdsRTNameSto[64];				/* current stored PS Name.*/


/* R D S   -   P I   D A T A   M A N A G E M E N T --------------------------*/
RDS_EXT	TYPE_BYTE		rRdsPiStat;			/* PI code control register.*/
#define	bRdsPiCtrl	rRdsPiStat.bit.b0

//#define	bTpStat		rRdsPiStat.bit.b7


RDS_EXT	TYPE_WORD 		rRdsPiCode;	    	/* PI code */

#define	rRdsPiCodeL	rRdsPiCode.byte[0]	    /* PI code (Low  byte).*/
#define	rRdsPiCodeH	rRdsPiCode.byte[1]	    /* PI code (High byte).*/

RDS_EXT	TYPE_WORD 		Rds_SwafFixPi;
#define	Rds_SwafFixPiL	Rds_SwafFixPi.byte[0]		/* Stored PI code (Low  byte).*/
#define	Rds_SwafFixPiH	Rds_SwafFixPi.byte[1]		/* Stored PI code (High byte).*/
RDS_EXT	TYPE_WORD 		Rds_SwafFixPiBak;
#define	_NoPi	0x00
RDS_EXT	uint 		PresetPiCode[6];	    /* PI code compare table. only used in AS process*/

/***********************/
/*RDS_EXT	char 	rSmeterBuffer[10];	     S-meter softerware filter buffer */
RDS_EXT	uchar		rRDSAFLevel;

RDS_EXT	TYPE_WORD 		rRdsSmeterBuff;	    /* S-meter value */
#define	rSmeterBuffL	rRdsSmeterBuff.byte[0]		/* Stored PI code (Low  byte).*/
#define	rSmeterBuffH	rRdsSmeterBuff.byte[1]		/* Stored PI code (High byte).*/
RDS_EXT	uchar 		cSmeterCnt;

RDS_EXT	uchar 		rRDSSmeter1;
RDS_EXT	uchar 		rRDSSmeter2;
RDS_EXT	uchar 		rRDSSmeter3;

/* R D S   -   P I   D A T A   M A N A G E M E N T --------------------------*/
RDS_EXT	uchar 		rRdsUTCHour;
RDS_EXT	uchar 		rRdsUTCMin;
RDS_EXT	uchar 		rRdsLTOffset;
RDS_EXT	uchar		cRdsAfDet;
RDS_EXT	uchar		rAfMeasureTimer;
RDS_EXT	uchar		rCurStaSig;
RDS_EXT	uchar 		cAfTimer_X128ms;

RDS_EXT	uchar 		rPtyType;
//RDS_EXT	uchar 		rPtyMType;
//RDS_EXT	uchar 		rPtySType;

RDS_EXT	uchar 		rPtySTabPnt;
RDS_EXT	uchar 		rOldRdMNumM;
RDS_EXT	uchar 		rOldRdMNumS;

RDS_EXT	uchar 		cBandBackup;
RDS_EXT	uchar	 	rRdsStoPty;           /* Programme type */
RDS_EXT	uchar	 	rRdsStoPtyTemp;

#define	_AfMemLength	25//9
typedef struct	_RDSMemStruct
{
	uint	RDSPi;
	uchar	RDSAf[_AfMemLength];
	uchar	RDSPs[8];
	uchar	RDSRT[64];
}RDSMemStruct;


RDS_EXT	RDSMemStruct 	RDSPreMem[21/*1*/];

#ifndef _OnlyReduceRAM

#define	RDSFM10PreMem	RDSPreMem[0]
#define	RDSFM11PreMem	RDSPreMem[1]
#define	RDSFM12PreMem	RDSPreMem[2]
#define	RDSFM13PreMem	RDSPreMem[3]
#define	RDSFM14PreMem	RDSPreMem[4]
#define	RDSFM15PreMem	RDSPreMem[5]
#define	RDSFM16PreMem	RDSPreMem[6]

#define	RDSFM20PreMem	RDSPreMem[7]
#define	RDSFM21PreMem	RDSPreMem[8]
#define	RDSFM22PreMem	RDSPreMem[9]
#define	RDSFM23PreMem	RDSPreMem[10]
#define	RDSFM24PreMem	RDSPreMem[11]
#define	RDSFM25PreMem	RDSPreMem[12]
#define	RDSFM26PreMem	RDSPreMem[13]

#define	RDSFM30PreMem	RDSPreMem[14]
#define	RDSFM31PreMem	RDSPreMem[15]
#define	RDSFM32PreMem	RDSPreMem[16]
#define	RDSFM33PreMem	RDSPreMem[17]
#define	RDSFM34PreMem	RDSPreMem[18]
#define	RDSFM35PreMem	RDSPreMem[19]
#define	RDSFM36PreMem	RDSPreMem[20]
#endif

//RDS_EXT	char	__near 	RDSPresetPiMem[42];           /* PI code 2*7*3=42 */
//RDS_EXT	char	__near 	RDSPresetAfMem[168];           /* Af list 8*7*3=168 */
//RDS_EXT	char	__near 	RDSPresetPsMem[168];           /* PI code 8*7*3=168 */
  
//#define	FM1PresetPiMem	RDSPresetPiMem[0]
//#define	FM2PresetPiMem	RDSPresetPiMem[14]
//#define	FM3PresetPiMem	RDSPresetPiMem[28]
//
//#define	FM1PresetAfMem	RDSPresetAfMem[0]
//#define	FM2PresetAfMem	RDSPresetAfMem[56]
//#define	FM3PresetAfMem	RDSPresetAfMem[112]
//
//#define	FM1PresetPsMem	RDSPresetPsMem[0]
//#define	FM2PresetPsMem	RDSPresetPsMem[56]
//#define	FM3PresetPsMem	RDSPresetPsMem[112]

/*programme type code*/
#define	_None		0
#define	_News		1
#define	_Affairs		2
#define	_Info		3
#define	_Sport		4
#define	_Educate	5 
#define	_Drama		6
#define	_Culture		7
#define	_Science	8
#define	_Varied 		9
#define	_PopM   		10
#define	_RockM  	11
#define	_EasyM  	12
#define	_LightM 		13
#define	_Classics   	14
#define	_OtherM  	15
#define	_Weather	16
#define	_Finance	17
#define	_Children   	18
#define	_Social 		19
#define	_Religion   	20
#define	_PhoneIn	21
#define	_Travel 		22
#define	_Leisure		23
#define	_Jazz   		24
#define	_Country	25
#define	_NationM	26
#define	_Oldies 		27
#define	_FolkM  		28
#define	_Document   29
#define	_Test   		30	/* Not for user searching */
#define	_Alarm  		31	/* Not for user searching */

#define	_PtyMin		_None
#define	_PtyMax		_Alarm


#define	_TaVolInit	25
#define 	_TaMaxVol	40

//extern	void	RdsStDisp( );
extern void	ReadRdsPreMem(uchar ps);
extern void	WriteRdsPreMem(uchar ps);

		
