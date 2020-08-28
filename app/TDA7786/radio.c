/***************************************************************************************\
* TP82A4 software, radio program file													*
* Language : toshiba 86 ANSI enhanced C				     								*
*      PLL : external IC, TC9257														*
* Author:  APICAL ELECTRONICS TECHNICAL Co.,LTD                                      		*
\*-------------------------------------------------------------------------------------*/

#define	Rad_ROOT
#include	"config.h"

#if	_UartDebug
//#define		_Debug4730			0
#define		_DebugRad			1
#define		_DebugWrPll			0
#define		_DebugTestDly		0

#endif
//#define		_manual_seek		0

//extern	uchar	WrEeromBuff[];
extern void TunerSeekOn(signed long seekstep);
extern void ReportRadioPreListinfo();

/*-----------------------------------------------------------------------------*\
*	           各波段电台及预置电台号存储区定义								*
\*-----------------------------------------------------------------------------*/
/*
    15,14,13,  12,11,10,9,8,7,6,5,4,3,2,1,0
    FM1 Preset number,  Station Number
    FM2 Preset number,  Station Number
    FM3 Preset number,  Station Number
    AM1 Preset number,  Station Number 
    AM2 Preset number,  Station Number 
*/

/*-----------------------------------------------------------------------------*\
*	           AS时，电台信号强度存储区										*
\*-----------------------------------------------------------------------------*/

/*preset memory station procedure status define*/
#define		_psWrPll		1
#define		_psWtPllStable	2
#define		_psPgmPllDl		3
#define		_psWtChkSt		4
#define		_psListen5S		5

/*radio scan procedure status define*/
#define		_rsWrPll		1
#define		_rsWtPllStab	2
#define		_rsWtChkSt		3
#define		_rsListen5S		4

/*Auto tunning procedure status define*/
#define		_atWrPll		1
#define		_atWtPllLk		2
#define		_atWtChkSt		3



#define	_FMRef			10
#define	_FMStep			100		// 100KHz
#define	_FMStartFreq	87500
#define	_FMEndFreq		108000

#define	_FMORTRef			10
#define	_FMORTStep			30		// 30KHz
#define	_FMORTStartFreq	65000
#define	_FMORTEndFreq		74000

#define	_LWRef			1
#define	_LWStep			1		
#define	_LWStartFreq	144
#define	_LWEndFreq		290


uchar	const	FMPrmTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMRef,				// 100K/50
/*2*/	_FMStep,						// 50K/10
/*3*/	(_FMStartFreq/_FMRef) >> 8,			// start 87.50MHz
/*4*/	(_FMStartFreq/_FMRef) & 0xff,
/*5*/	((_FMEndFreq - _FMStartFreq)/_FMStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMEndFreq - _FMStartFreq)/_FMStep) & 0xff,
/*7*/	(_FMEndFreq/_FMRef) >> 8,		// end 87.50MHz
/*8*/	(_FMEndFreq/_FMRef) & 0xff,	
/*9*/	(_FMRef/10) & 0xff,	

};



#define	_FMEurRef			10
#define	_FMEurStep			50		// 100KHz
#define	_FMEurStartFreq		87500
#define	_FMEurEndFreq		108000

uchar	const	FMPrmEurTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMEurRef,				// 100K/50
/*2*/	_FMEurStep,						// 50K/10
/*3*/	(_FMEurStartFreq/_FMEurRef) >> 8,			// start 87.50MHz
/*4*/	(_FMEurStartFreq/_FMEurRef) & 0xff,
/*5*/	((_FMEurEndFreq - _FMEurStartFreq)/_FMEurStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMEurEndFreq - _FMEurStartFreq)/_FMEurStep) & 0xff,
/*7*/	(_FMEurEndFreq/_FMEurRef) >> 8,		// end 87.50MHz
/*8*/	(_FMEurEndFreq/_FMEurRef) & 0xff,	
/*9*/	(_FMEurRef/10) & 0xff,	
};

#define	_FMUSARef			10
#define	_FMUSAStep			200		// 100KHz
#define	_FMUSAStartFreq		87500
#define	_FMUSAEndFreq		107900

uchar	const	FMPrmUSATab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMUSARef,				// 100K/50
/*2*/	_FMUSAStep,						// 200K/10
/*3*/	(_FMUSAStartFreq/_FMUSARef) >> 8,			// start 87.50MHz
/*4*/	(_FMUSAStartFreq/_FMUSARef) & 0xff,
/*5*/	((_FMUSAEndFreq - _FMUSAStartFreq)/_FMUSAStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMUSAEndFreq - _FMUSAStartFreq)/_FMUSAStep) & 0xff,
/*7*/	(_FMUSAEndFreq/_FMUSARef) >> 8,		// end 87.50MHz
/*8*/	(_FMUSAEndFreq/_FMUSARef) & 0xff,	
/*9*/	(_FMUSARef/10) & 0xff,	
};

#define	_FMLATINRef			10
#define	_FMLATINStep		100		// 100KHz
#define	_FMLATINStartFreq	87500
#define	_FMLATINEndFreq		108000

uchar	const	FMPrmLATINTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMLATINRef,				// 100K/50
/*2*/	_FMLATINStep,						// 50K/10
/*3*/	(_FMLATINStartFreq/_FMLATINRef) >> 8,			// start 87.50MHz
/*4*/	(_FMLATINStartFreq/_FMLATINRef) & 0xff,
/*5*/	((_FMLATINEndFreq - _FMLATINStartFreq)/_FMLATINStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMLATINEndFreq - _FMLATINStartFreq)/_FMLATINStep) & 0xff,
/*7*/	(_FMLATINEndFreq/_FMLATINRef) >> 8,		// end 87.50MHz
/*8*/	(_FMLATINEndFreq/_FMLATINRef) & 0xff,	
/*9*/	(_FMLATINRef/10) & 0xff,	
};


#define	_FMAusRef			10
#define	_FMAusStep			100		// 100KHz
#define	_FMAusStartFreq		87500
#define	_FMAusEndFreq		108000

uchar	const	FMPrmAusTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMAusRef,				// 100K/50
/*2*/	_FMAusStep,						// 50K/10
/*3*/	(_FMAusStartFreq/_FMAusRef) >> 8,			// start 87.50MHz
/*4*/	(_FMAusStartFreq/_FMAusRef) & 0xff,
/*5*/	((_FMAusEndFreq - _FMAusStartFreq)/_FMAusStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMAusEndFreq - _FMAusStartFreq)/_FMAusStep) & 0xff,
/*7*/	(_FMAusEndFreq/_FMAusRef) >> 8,		// end 87.50MHz
/*8*/	(_FMAusEndFreq/_FMAusRef) & 0xff,	
/*9*/	(_FMAusRef/10) & 0xff,	
};

#define	_FMRussiaRef			10
#define	_FMRussiaStep			100		// 100KHz
#define	_FMRussiaStartFreq		87500
#define	_FMRussiaEndFreq		108000

uchar	const	FMPrmRussiaTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMRussiaRef,				// 100K/50
/*2*/	_FMRussiaStep,						// 50K/10
/*3*/	(_FMRussiaStartFreq/_FMRussiaRef) >> 8,			// start 87.50MHz
/*4*/	(_FMRussiaStartFreq/_FMRussiaRef) & 0xff,
/*5*/	((_FMRussiaEndFreq - _FMRussiaStartFreq)/_FMRussiaStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMRussiaEndFreq - _FMRussiaStartFreq)/_FMRussiaStep) & 0xff,
/*7*/	(_FMRussiaEndFreq/_FMRussiaRef) >> 8,		// end 87.50MHz
/*8*/	(_FMRussiaEndFreq/_FMRussiaRef) & 0xff,
/*9*/	(_FMRussiaRef/10) & 0xff,	
};

uchar	const	FMORTPrmTab[ ]=
{
/*0*/	_RefOIRTBand,
/*1*/	_FMORTRef,				// 30K/10
/*2*/	_FMORTStep,						// 30K/10
/*3*/	(_FMORTStartFreq/_FMORTRef) >> 8,					// 87.50MHz
/*4*/	(_FMORTStartFreq/_FMORTRef) & 0xff,
/*5*/	((_FMORTEndFreq - _FMORTStartFreq)/_FMORTStep) >> 8,	// 75MHz - 65MHz
/*6*/	((_FMORTEndFreq - _FMORTStartFreq)/_FMORTStep) & 0xff,
/*7*/	(_FMORTEndFreq/_FMORTRef) >> 8,		// start divisor data high 8 bits
/*8*/	(_FMORTEndFreq/_FMORTRef) & 0xff,	// start divisor data low 8 bits
/*9*/	(_FMORTRef/10) & 0xff,	

};


#define	_FMJapenRef			10
#define	_FMJapenStep			100		// 100KHz
#define	_FMJapenStartFreq		76000
#define	_FMJapenEndFreq		90000

uchar	const	FMPrmJapenTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMJapenRef,				// 100K/50
/*2*/	_FMJapenStep,						// 50K/10
/*3*/	(_FMJapenStartFreq/_FMJapenRef) >> 8,			// start 87.50MHz
/*4*/	(_FMJapenStartFreq/_FMJapenRef) & 0xff,
/*5*/	((_FMJapenEndFreq - _FMJapenStartFreq)/_FMJapenStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMJapenEndFreq - _FMJapenStartFreq)/_FMJapenStep) & 0xff,
/*7*/	(_FMJapenEndFreq/_FMJapenRef) >> 8,		// end 87.50MHz
/*8*/	(_FMJapenEndFreq/_FMJapenRef) & 0xff,	
/*9*/	(_FMJapenRef/10) & 0xff,	

};

#define	_FMBrazilRef				10
#define	_FMBrazilStep			200		// 100KHz
#define	_FMBrazilStartFreq		87500
#define	_FMBrazilEndFreq		108000

uchar	const	FMPrmBrazilTab[ ]=
{
/*0*/	_RefFMBand,
/*1*/	_FMBrazilRef,				// 100K/50
/*2*/	_FMBrazilStep,						// 50K/10
/*3*/	(_FMBrazilStartFreq/_FMBrazilRef) >> 8,			// start 87.50MHz
/*4*/	(_FMBrazilStartFreq/_FMBrazilRef) & 0xff,
/*5*/	((_FMBrazilEndFreq - _FMBrazilStartFreq)/_FMBrazilStep) >> 8,	// 108.00MHz - 87.50MHz
/*6*/	((_FMBrazilEndFreq - _FMBrazilStartFreq)/_FMBrazilStep) & 0xff,
/*7*/	(_FMBrazilEndFreq/_FMBrazilRef) >> 8,		// end 87.50MHz
/*8*/	(_FMBrazilEndFreq/_FMBrazilRef) & 0xff,	
/*9*/	(_FMBrazilRef/10) & 0xff,	

};

#define	_AMRef			1
#define	_AMStep		9	
#define	_AMStartFreq	531
#define	_AMEndFreq		1629

uchar	const	AMPrmTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMRef,							// 9K
/*2*/	_AMStep,							// 9
/*3*/	_AMStartFreq >> 8,			// AM start frequency
/*4*/	_AMStartFreq & 0xff,			    	
/*5*/	((_AMEndFreq - _AMStartFreq)/_AMStep) >> 8,	// AM total station number
/*6*/	((_AMEndFreq - _AMStartFreq)/_AMStep) & 0xff,	
/*7*/	(_AMEndFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndFreq) & 0xff, 	
/*9*/	(_AMRef) & 0xff,	
};

#define	_AMEurRef			1
#define	_AMEurStep			9
#define	_AMStartEurFreq	522
#define	_AMEndEurFreq		1620

uchar	const	AMPrmEurTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMEurRef,							// 9K
/*2*/	_AMEurStep,							// 9
/*3*/	_AMStartEurFreq >> 8,			// AM start frequency
/*4*/	_AMStartEurFreq & 0xff,			    	
/*5*/	((_AMEndEurFreq - _AMStartEurFreq)/_AMEurStep) >> 8,	// AM total station number
/*6*/	((_AMEndEurFreq - _AMStartEurFreq)/_AMEurStep) & 0xff,	
/*7*/	(_AMEndEurFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndEurFreq) & 0xff, 
/*9*/	(_AMRef) & 0xff,	

};

#define	_AMUSARef			1
#define	_AMUSAStep			10
#define	_AMStartUSAFreq	530
#define	_AMEndUSAFreq		1710

uchar	const	AMPrmUSATab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMUSARef,							// 9K
/*2*/	_AMUSAStep,							// 9
/*3*/	_AMStartUSAFreq >> 8,			// AM start frequency
/*4*/	_AMStartUSAFreq & 0xff,			    	
/*5*/	((_AMEndUSAFreq - _AMStartUSAFreq)/_AMUSAStep) >> 8,	// AM total station number
/*6*/	((_AMEndUSAFreq - _AMStartUSAFreq)/_AMUSAStep) & 0xff,	
/*7*/	(_AMEndUSAFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndUSAFreq) & 0xff, 	
/*9*/	(_AMRef) & 0xff,	

};

#define	_AMLATINRef			1
#define	_AMLATINStep			10
#define	_AMStartLATINFreq		520
#define	_AMEndLATINFreq		1620

uchar	const	AMPrmLATINTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMLATINRef,							// 9K
/*2*/	_AMLATINStep,							// 9
/*3*/	_AMStartLATINFreq >> 8,			// AM start frequency
/*4*/	_AMStartLATINFreq & 0xff,			    	
/*5*/	((_AMEndLATINFreq - _AMStartLATINFreq)/_AMLATINStep) >> 8,	// AM total station number
/*6*/	((_AMEndLATINFreq - _AMStartLATINFreq)/_AMLATINStep) & 0xff,	
/*7*/	(_AMEndLATINFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndLATINFreq) & 0xff, 	
/*9*/	(_AMRef) & 0xff,	

};

#define	_AMAusRef			1
#define	_AMAusStep			9
#define	_AMStartAusFreq	522
#define	_AMEndAusFreq		1710

uchar	const	AMPrmAusTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMAusRef,							// 9K
/*2*/	_AMAusStep,							// 9
/*3*/	_AMStartAusFreq >> 8,			// AM start frequency
/*4*/	_AMStartAusFreq & 0xff,			    	
/*5*/	((_AMEndAusFreq - _AMStartAusFreq)/_AMAusStep) >> 8,	// AM total station number
/*6*/	((_AMEndAusFreq - _AMStartAusFreq)/_AMAusStep) & 0xff,	
/*7*/	(_AMEndAusFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndAusFreq) & 0xff, 	
/*9*/	(_AMRef) & 0xff,	

};

#define	_AMRussiaRef			1
#define	_AMRussiaStep			9
#define	_AMStartRussiaFreq		522
#define	_AMEndRussiaFreq		1620

uchar	const	AMPrmRussiaTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMRussiaRef,							// 9K
/*2*/	_AMRussiaStep,							// 9
/*3*/	_AMStartRussiaFreq >> 8,			// AM start frequency
/*4*/	_AMStartRussiaFreq & 0xff,			    	
/*5*/	((_AMEndRussiaFreq - _AMStartRussiaFreq)/_AMRussiaStep) >> 8,	// AM total station number
/*6*/	((_AMEndRussiaFreq - _AMStartRussiaFreq)/_AMRussiaStep) & 0xff,	
/*7*/	(_AMEndRussiaFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndRussiaFreq) & 0xff, 
/*9*/	(_AMRef) & 0xff,	

};

#define	_AMJapenRef			1
#define	_AMJapenStep			9
#define	_AMStartJapenFreq		522
#define	_AMEndJapenFreq		1629

uchar	const	AMPrmJapanTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMJapenRef,							// 9K
/*2*/	_AMJapenStep,							// 9
/*3*/	_AMStartJapenFreq >> 8,			// AM start frequency
/*4*/	_AMStartJapenFreq & 0xff,			    	
/*5*/	((_AMEndJapenFreq - _AMStartJapenFreq)/_AMJapenStep) >> 8,	// AM total station number
/*6*/	((_AMEndJapenFreq - _AMStartJapenFreq)/_AMJapenStep) & 0xff,	
/*7*/	(_AMEndJapenFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndJapenFreq) & 0xff, 
/*9*/	(_AMRef) & 0xff,	
};

#define	_LWJapenRef			1
#define	_LWJapenStep			1
#define	_LWStartJapenFreq		144
#define	_LWEndJapenFreq		288

uchar	const	LWPrmJapanTab[ ]=
{
/*0*/	_RefLWBand,
/*1*/	_LWJapenRef,							// 9K
/*2*/	_LWJapenStep,							// 9
/*3*/	_LWStartJapenFreq >> 8,			// AM start frequency
/*4*/	_LWStartJapenFreq & 0xff,			    	
/*5*/	((_LWEndJapenFreq - _LWStartJapenFreq)/_LWJapenStep) >> 8,	// AM total station number
/*6*/	((_LWEndJapenFreq - _LWStartJapenFreq)/_LWJapenStep) & 0xff,	
/*7*/	(_LWEndJapenFreq) >> 8,	// AM start station division data
/*8*/	(_LWEndJapenFreq) & 0xff, 
/*9*/	(_AMRef) & 0xff,	

};

#define	_AMBrazilRef			1
#define	_AMBrazilStep			10
#define	_AMStartBrazilFreq	530
#define	_AMEndBrazilFreq		1710

uchar	const	AMPrmBrazilTab[ ]=
{
/*0*/	_RefAMBand,
/*1*/	_AMBrazilRef,							// 9K
/*2*/	_AMBrazilStep,							// 9
/*3*/	_AMStartBrazilFreq >> 8,			// AM start frequency
/*4*/	_AMStartBrazilFreq & 0xff,			    	
/*5*/	((_AMEndBrazilFreq - _AMStartBrazilFreq)/_AMBrazilStep) >> 8,	// AM total station number
/*6*/	((_AMEndBrazilFreq - _AMStartBrazilFreq)/_AMBrazilStep) & 0xff,	
/*7*/	(_AMEndBrazilFreq) >> 8,	// AM start station division data
/*8*/	(_AMEndBrazilFreq) & 0xff, 	
/*9*/	(_AMRef) & 0xff,	

};

/***************************************************************************************\
*         各区域预置电台初始化表														*
\*-------------------------------------------------------------------------------------*/
uint	const	FMStatInitTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMStep,
};

uint	const	FMStatInitEurTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMEurStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMEurStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMEurStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMEurStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMEurStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMEurStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMEurStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMEurStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMEurStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMEurStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMEurStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMEurStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMEurStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMEurStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMEurStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMEurStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMEurStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMEurStep,
};

uint	const	FMStatInitUSATab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMUSAStep,
	/*M2:  90.10MHz*/	( 90100 - _FMStartFreq)/_FMUSAStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMUSAStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMUSAStep,
	/*M5: 107.90MHz*/	( 107900 - _FMStartFreq)/_FMUSAStep,
	/*M6: 87.50MHz*/	(_FMStartFreq - _FMStartFreq)/_FMUSAStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMUSAStep,
	/*M2:  90.10MHz*/	( 90100 - _FMStartFreq)/_FMUSAStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMUSAStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMUSAStep,
	/*M5: 107.90MHz*/	( 107900 - _FMStartFreq)/_FMUSAStep,
	/*M6: 87.50MHz*/	(_FMStartFreq - _FMStartFreq)/_FMUSAStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMUSAStep,
	/*M2:  90.10MHz*/	( 90100 - _FMStartFreq)/_FMUSAStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMUSAStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMUSAStep,
	/*M5: 107.90MHz*/	( 107900 - _FMStartFreq)/_FMUSAStep,
	/*M6: 87.50MHz*/	(_FMStartFreq - _FMStartFreq)/_FMUSAStep,
};

uint	const	FMStatInitLatinTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMLATINStep,
	/*M2:  90.00MHz*/	( 90100 - _FMStartFreq)/_FMLATINStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMLATINStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMLATINStep,
	/*M5: 108.00MHz*/	( 107900 - _FMStartFreq)/_FMLATINStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMLATINStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMLATINStep,
	/*M2:  90.00MHz*/	( 90100 - _FMStartFreq)/_FMLATINStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMLATINStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMLATINStep,
	/*M5: 108.00MHz*/	( 107900 - _FMStartFreq)/_FMLATINStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMLATINStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMLATINStep,
	/*M2:  90.00MHz*/	( 90100 - _FMStartFreq)/_FMLATINStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMLATINStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMLATINStep,
	/*M5: 108.00MHz*/	( 107900 - _FMStartFreq)/_FMLATINStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMLATINStep,
};

uint	const	FMStatInitAusTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMAusStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMAusStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMAusStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMAusStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMAusStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMAusStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMAusStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMAusStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMAusStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMAusStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMAusStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMAusStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMAusStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMAusStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMAusStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMAusStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMAusStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMAusStep,
};

uint	const	FMStatInitRussiaTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMRussiaStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMRussiaStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMRussiaStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMRussiaStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMRussiaStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMRussiaStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMRussiaStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMRussiaStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMRussiaStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMRussiaStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMRussiaStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMRussiaStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMRussiaStep,
	/*M2:  90.00MHz*/	( 90000 - _FMStartFreq)/_FMRussiaStep,
	/*M3:  98.10MHz*/	( 98000 - _FMStartFreq)/_FMRussiaStep,
	/*M4: 106.10MHz*/	( 106000 - _FMStartFreq)/_FMRussiaStep,
	/*M5: 108.00MHz*/	( 108000 - _FMStartFreq)/_FMRussiaStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMRussiaStep,
};

uint	const	FMStatInitJapenTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  76.00MHz*/	( _FMJapenStartFreq - _FMJapenStartFreq)/_FMJapenStep,
	/*M2:  79.50MHz*/	( 79500 - _FMJapenStartFreq)/_FMJapenStep,
	/*M3:  83.00MHz*/	( 83000 - _FMJapenStartFreq)/_FMJapenStep,
	/*M4:  86.50MHz*/	( 86500 - _FMJapenStartFreq)/_FMJapenStep,
	/*M5:  90.00MHz*/	( _FMJapenEndFreq - _FMJapenStartFreq)/_FMJapenStep,
	/*M6: 76.00MHz*/		(_FMJapenStartFreq - _FMJapenStartFreq)/_FMJapenStep,

	/*M1:  87.50MHz*/	( _FMJapenStartFreq - _FMJapenStartFreq)/_FMJapenStep,
	/*M2:  90.00MHz*/	( 79500 - _FMJapenStartFreq)/_FMJapenStep,
	/*M3:  98.10MHz*/	( 83000 - _FMJapenStartFreq)/_FMJapenStep,
	/*M4: 106.10MHz*/	( 86500 - _FMJapenStartFreq)/_FMJapenStep,
	/*M5: 108.00MHz*/	( _FMJapenEndFreq - _FMJapenStartFreq)/_FMJapenStep,
	/*M6: 108.00MHz*/	(_FMJapenStartFreq - _FMJapenStartFreq)/_FMJapenStep,

	/*M1:  87.50MHz*/	( _FMJapenStartFreq - _FMJapenStartFreq)/_FMJapenStep,
	/*M2:  90.00MHz*/	( 79500 - _FMJapenStartFreq)/_FMJapenStep,
	/*M3:  98.10MHz*/	( 83000 - _FMJapenStartFreq)/_FMJapenStep,
	/*M4: 106.10MHz*/	( 86500 - _FMJapenStartFreq)/_FMJapenStep,
	/*M5: 108.00MHz*/	( _FMJapenEndFreq - _FMJapenStartFreq)/_FMJapenStep,
	/*M6: 108.00MHz*/	(_FMJapenStartFreq - _FMJapenStartFreq)/_FMJapenStep,
};

uint	const	FMStatInitBrazilTab[ ]=/* Austrial FM initialization table*/
{//FM:	Freq: 87.5~108MHz,  Step size: 100KHz*/
	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMBrazilStep,
	/*M2:  90.00MHz*/	( 90100 - _FMStartFreq)/_FMBrazilStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMBrazilStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMBrazilStep,
	/*M5: 108.00MHz*/	( 107900 - _FMStartFreq)/_FMBrazilStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMBrazilStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMBrazilStep,
	/*M2:  90.00MHz*/	( 90100 - _FMStartFreq)/_FMBrazilStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMBrazilStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMBrazilStep,
	/*M5: 108.00MHz*/	( 107900 - _FMStartFreq)/_FMBrazilStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMBrazilStep,

	/*M1:  87.50MHz*/	( _FMStartFreq - _FMStartFreq)/_FMBrazilStep,
	/*M2:  90.00MHz*/	( 90100 - _FMStartFreq)/_FMBrazilStep,
	/*M3:  98.10MHz*/	( 98100 - _FMStartFreq)/_FMBrazilStep,
	/*M4: 106.10MHz*/	( 106100 - _FMStartFreq)/_FMBrazilStep,
	/*M5: 108.00MHz*/	( 107900 - _FMStartFreq)/_FMBrazilStep,
	/*M6: 108.00MHz*/	(_FMStartFreq - _FMStartFreq)/_FMBrazilStep,
};

uint	const	FMORTStatInitTab[ ]=/* ORT FM initialization table*/
{//FM:	Freq: 65~75MHz,  Step size: 30KHz*/
	/*M2:  65.10MHz*/	( 65000 - _FMORTStartFreq)/_FMORTStep,
	/*M2:  65.10MHz*/	( 67100 - _FMORTStartFreq)/_FMORTStep,
	/*M3:  65.10MHz*/	( 70400 - _FMORTStartFreq)/_FMORTStep,
	/*M4: 65.10MHz*/		( 72500 - _FMORTStartFreq)/_FMORTStep,
	/*M5: 65.00MHz*/		( 74000 - _FMORTStartFreq)/_FMORTStep,
	/*M6: 65.00MHz*/		( 65000 - _FMORTStartFreq)/_FMORTStep,
};

uint	const	AMStatInitTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartFreq - _AMStartFreq)/_AMStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartFreq)/_AMStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartFreq)/_AMStep,
	/*M4: 1404KHz*/		(1404 - _AMStartFreq)/_AMStep,
	/*M5: 1629KHz*/		(1629 - _AMStartFreq)/_AMStep,
	/*M6: 531KHz*/		(_AMStartFreq - _AMStartFreq)/_AMStep,

	/*M1:  531KHz*/	 	( _AMStartFreq - _AMStartFreq)/_AMStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartFreq)/_AMStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartFreq)/_AMStep,
	/*M4: 1404KHz*/		(1404 - _AMStartFreq)/_AMStep,
	/*M5: 1629KHz*/		(1629 - _AMStartFreq)/_AMStep,
	/*M6: 531KHz*/		(_AMStartFreq - _AMStartFreq)/_AMStep,
};

uint	const	AMStatInitEurTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartEurFreq - _AMStartEurFreq)/_AMEurStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartEurFreq)/_AMEurStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartEurFreq)/_AMEurStep,
	/*M4: 1404KHz*/		(1404 - _AMStartEurFreq)/_AMEurStep,
	/*M5: 1629KHz*/		(1620 - _AMStartEurFreq)/_AMEurStep,
	/*M6: 531KHz*/		(_AMStartEurFreq - _AMStartEurFreq)/_AMEurStep,

	/*M1:  531KHz*/	 	( _AMStartEurFreq - _AMStartEurFreq)/_AMEurStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartEurFreq)/_AMEurStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartEurFreq)/_AMEurStep,
	/*M4: 1404KHz*/		(1404 - _AMStartEurFreq)/_AMEurStep,
	/*M5: 1629KHz*/		(1620 - _AMStartEurFreq)/_AMEurStep,
	/*M6: 531KHz*/		(_AMStartEurFreq - _AMStartEurFreq)/_AMEurStep,
};

uint	const	AMStatInitUSATab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  530KHz*/	 	( _AMStartUSAFreq - _AMStartUSAFreq)/_AMUSAStep,
	/*M2:  600KHz*/	 	( 600 - _AMStartUSAFreq)/_AMUSAStep,
	/*M3:  1000KHz*/	 	( 1000 - _AMStartUSAFreq)/_AMUSAStep,
	/*M4: 1400KHz*/		(1400 - _AMStartUSAFreq)/_AMUSAStep,
	/*M5: 1710KHz*/		(1710 - _AMStartUSAFreq)/_AMUSAStep,
	/*M6: 530KHz*/		(_AMStartUSAFreq - _AMStartUSAFreq)/_AMUSAStep,

	/*M1:  530KHz*/	 	( _AMStartUSAFreq - _AMStartUSAFreq)/_AMUSAStep,
	/*M2:  600KHz*/	 	( 600 - _AMStartUSAFreq)/_AMUSAStep,
	/*M3:  1000KHz*/	 	( 1000 - _AMStartUSAFreq)/_AMUSAStep,
	/*M4: 1400KHz*/		(1400 - _AMStartUSAFreq)/_AMUSAStep,
	/*M5: 1710KHz*/		(1710 - _AMStartUSAFreq)/_AMUSAStep,
	/*M6: 530KHz*/		(_AMStartUSAFreq - _AMStartUSAFreq)/_AMUSAStep,
};

uint	const	AMStatInitLATINTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartLATINFreq - _AMStartLATINFreq)/_AMLATINStep,
	/*M2:  603KHz*/	 	( 600 - _AMStartLATINFreq)/_AMLATINStep,
	/*M3:  999KHz*/	 	( 1000 - _AMStartLATINFreq)/_AMLATINStep,
	/*M4: 1404KHz*/		(1400 - _AMStartLATINFreq)/_AMLATINStep,
	/*M5: 1629KHz*/		(1620 - _AMStartLATINFreq)/_AMLATINStep,
	/*M6: 531KHz*/		(_AMStartLATINFreq - _AMStartLATINFreq)/_AMLATINStep,

	/*M1:  531KHz*/	 	( _AMStartLATINFreq - _AMStartLATINFreq)/_AMLATINStep,
	/*M2:  603KHz*/	 	( 600 - _AMStartLATINFreq)/_AMLATINStep,
	/*M3:  999KHz*/	 	( 1000 - _AMStartLATINFreq)/_AMLATINStep,
	/*M4: 1404KHz*/		(1400 - _AMStartLATINFreq)/_AMLATINStep,
	/*M5: 1629KHz*/		(1620 - _AMStartLATINFreq)/_AMLATINStep,
	/*M6: 531KHz*/		(_AMStartLATINFreq - _AMStartLATINFreq)/_AMLATINStep,
};

uint	const	AMStatInitAusTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartAusFreq - _AMStartAusFreq)/_AMAusStep,
	/*M2:  603KHz*/	 	( 594 - _AMStartAusFreq)/_AMAusStep,
	/*M3:  999KHz*/	 	( 990 - _AMStartAusFreq)/_AMAusStep,
	/*M4: 1404KHz*/		(1395 - _AMStartAusFreq)/_AMAusStep,
	/*M5: 1629KHz*/		(1593 - _AMStartAusFreq)/_AMAusStep,
	/*M6: 531KHz*/		(_AMStartAusFreq - _AMStartAusFreq)/_AMAusStep,

	/*M1:  531KHz*/	 	( _AMStartAusFreq - _AMStartAusFreq)/_AMAusStep,
	/*M2:  603KHz*/	 	( 594 - _AMStartAusFreq)/_AMAusStep,
	/*M3:  999KHz*/	 	( 990 - _AMStartAusFreq)/_AMAusStep,
	/*M4: 1404KHz*/		(1395 - _AMStartAusFreq)/_AMAusStep,
	/*M5: 1629KHz*/		(1593 - _AMStartAusFreq)/_AMAusStep,
	/*M6: 531KHz*/		(_AMStartAusFreq - _AMStartAusFreq)/_AMAusStep,
};

uint	const	AMStatInitRussiaTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartRussiaFreq - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M4: 1404KHz*/		(1404 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M5: 1629KHz*/		(1620 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M6: 531KHz*/		(_AMStartRussiaFreq - _AMStartRussiaFreq)/_AMRussiaStep,

	/*M1:  531KHz*/	 	( _AMStartRussiaFreq - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M4: 1404KHz*/		(1404 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M5: 1629KHz*/		(1620 - _AMStartRussiaFreq)/_AMRussiaStep,
	/*M6: 531KHz*/		(_AMStartRussiaFreq - _AMStartRussiaFreq)/_AMRussiaStep,
};

uint	const	AMStatInitJapenTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartJapenFreq - _AMStartJapenFreq)/_AMJapenStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartJapenFreq)/_AMJapenStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartJapenFreq)/_AMJapenStep,
	/*M4: 1404KHz*/		(1404 - _AMStartJapenFreq)/_AMJapenStep,
	/*M5: 1629KHz*/		(1629 - _AMStartJapenFreq)/_AMJapenStep,
	/*M6: 531KHz*/		(_AMStartJapenFreq - _AMStartJapenFreq)/_AMJapenStep,

	/*M1:  531KHz*/	 	( _AMStartJapenFreq - _AMStartJapenFreq)/_AMJapenStep,
	/*M2:  603KHz*/	 	( 603 - _AMStartJapenFreq)/_AMJapenStep,
	/*M3:  999KHz*/	 	( 999 - _AMStartJapenFreq)/_AMJapenStep,
	/*M4: 1404KHz*/		(1404 - _AMStartJapenFreq)/_AMJapenStep,
	/*M5: 1629KHz*/		(1629 - _AMStartJapenFreq)/_AMJapenStep,
	/*M6: 531KHz*/		(_AMStartJapenFreq - _AMStartJapenFreq)/_AMJapenStep,
};

uint	const	LWStatInitJapenTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 144~288, Step size: 9KHz*/
	/*M1:  144KHz*/	 	( _LWStartJapenFreq - _LWStartJapenFreq)/_LWJapenStep,
	/*M2:  160KHz*/	 	( 160 - _LWStartJapenFreq)/_LWJapenStep,
	/*M3:  200KHz*/	 	( 200 - _LWStartJapenFreq)/_LWJapenStep,
	/*M4: 260KHz*/		(260 - _LWStartJapenFreq)/_LWJapenStep,
	/*M5: 288KHz*/		(288 - _LWStartJapenFreq)/_LWJapenStep,
	/*M6: 160KHz*/		(_LWStartJapenFreq - _LWStartJapenFreq)/_LWJapenStep,
};

uint	const	AMStatInitBrazilTab[ ]=/* Austrial AM initialization table*/
{/*AM: 	Freq: 522~1620KHz, Step size: 9KHz*/
	/*M1:  603KHz*/	 	( _AMStartBrazilFreq - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M2:  603KHz*/	 	( 600 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M3:  999KHz*/	 	( 1000 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M4: 1404KHz*/		(1400 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M5: 1629KHz*/		(1710 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M6: 531KHz*/		(_AMStartBrazilFreq - _AMStartBrazilFreq)/_AMBrazilStep,

	/*M1:  531KHz*/	 	( _AMStartBrazilFreq - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M2:  603KHz*/	 	( 600 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M3:  999KHz*/	 	( 1000 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M4: 1404KHz*/		(1400 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M5: 1629KHz*/		(1710 - _AMStartBrazilFreq)/_AMBrazilStep,
	/*M6: 531KHz*/		(_AMStartBrazilFreq - _AMStartBrazilFreq)/_AMBrazilStep,
};
const AreaStuct Area[];

const AreaStuct Area[]=
{
	// CHINA
	8750,8750,9010,9810,10610,10800,8750,0,
	8750,8750,8750,8750,8750,8750,8750,0,
	////162400, 162425, 162450, 162475, 162500, 162525
	//2400, 2400, 2425, 2450, 2475, 2500, 2525, 0,//Weather band
	8750,8750,9010,9810,10610,10800,8750,0,
	520,520,600,1000,1400,1620,520,0,
	520,520,520,520,520,520,520,0,
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	10,10,10800,8750,
	0,0,0,
	10,1620,520,
	0,0,0,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,
	//25,2550,2400,

	// EUROPE
	8750,8750,9510,9810,10610,10800,9120,0,
	8780,8780,9180,9390,9710,10300,10430,0,
	8750,8750,8750,8750,8750,8750,8750,0,
	//522,522,603,999,1404,1620,522,0,
	522,522,846,999,1404,1620,522,0,
	//522,522,603,999,1404,1620,522,0,
	144,144,171,216,270,288,144,0,//---tempory remove
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	5,10,10800,8750,
	//5,5,10800,8750,
	0,0,0,
	9,1620,522,
	9,288,144,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,

	// USA
	8750,8750,9010,9810,10610,10790,8750,0,
	8750,8750,9670,10490,10690,10790,8750,0,
	//162400, 162425, 162450, 162475, 162500, 162525,
	62400, 62400, 62425, 62450, 62475, 62500, 62525, 0,//Weather band
	//8750,8750,9670,10490,10690,10790,8750,0,
	530,530,600,1000,1600,1720,530,0,
	530,530,600,1000,1600,1720,530,0,	
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	20,20,10790,8750,
	0,0,0,
	10,1720,530,
	0,0,0,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	25,62550,62400,

	// LATIN
	8750,8750,9010,9810,10610,10800,8750,0,
	8750,8750,8750,8750,8750,8750,8750,0,
	////162400, 162425, 162450, 162475, 162500, 162525
	//2400, 2400, 2425, 2450, 2475, 2500, 2525, 0,//Weather band
	8750,8750,9010,9810,10610,10800,8750,0,
	520,520,600,1000,1400,1620,520,0,
	520,520,520,520,520,520,520,0,
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	10,10,10800,8750,
	0,0,0,
	10,1620,520,
	0,0,0,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,
	//25,2550,2400,


	// OIRT
	6500,6500,8750,9000,9800,10600,10800,0,
	6500,6500,8750,9000,9800,10600,10800,0,
	6500,6500,8750,9000,9800,10600,10800,0,
	531,531,603,999,1404,1620,531,0,
	531,531,603,999,1404,1620,531,0,
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	//144,144,171,216,270,288,144,0,
	5,10,10800,8750,
	3,7400,6500,
	9,1620,522,
	9,288,144,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,


	// CHINA
	8750,8750,9010,9810,10610,10800,8750,0,
	8750,8750,8750,8750,8750,8750,8750,0,
	////162400, 162425, 162450, 162475, 162500, 162525
	//2400, 2400, 2425, 2450, 2475, 2500, 2525, 0,//Weather band
	8750,8750,9010,9810,10610,10800,8750,0,
	520,520,600,1000,1400,1620,520,0,
	520,520,520,520,520,520,520,0,
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	10,10,10800,8750,
	0,0,0,
	10,1620,520,
	0,0,0,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,
	//25,2550,2400,


	// JAPAN
	7600,7600,7820,7900,8000,8500,9000,0,
	7600,7600,7900,8200,8500,8800,9000,0,
	7600,7600,7950,8250,8550,8850,9000,0,
	531,531,603,999,1404,1620,531,0,
	531,531,603,999,1404,1620,531,0,
	//144,144,171,216,270,288,144,0,
	 //2300KHz,(xxx-1384)*5KHz,..., 25080KHz
        1844,1844,2500,3000,4000,5000,6400,0, /* sw frequency */ 
	5,10,9000,7600,
	0,0,0,
	9,1620,522,
	//9,288,144,
	0,0,0,
	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */ 	
	0,0,0,
};
/*******************************************************************************\
* Function: digital filter.														*
* Input   : 	                                       							*  
* Output  : middle value                                                   		*  
\*-----------------------------------------------------------------------------*/ 
uchar	AutoAlignDigitalFilter(uchar *pBuffer,uchar cLength)
{
     /*rWay = 0:Middel data
                   =1:Avarage*/
	uchar t,i,j;
	uchar	*p;
	for(i=0; i<cLength-1; i++)
	{
		p = pBuffer;
		for(j=0; j<cLength-i-1; j++)
		{
			if(*p > *(p+1))
			{
				t = *p;
				*p = *(p+1);
				*(p+1) = t;
			}
			++p;
		}
	}
	t = cLength >> 1;
	
	return(*(pBuffer+t));
}

//=================================================================
//Function Nam: RadioBreak()
//Descript:
//=================================================================
void RadioBreak(void)
{
	u8 info;

	//if (Radio.workmode != RADIO_WORK_IDLE)
	{
		//MuteTimeset(100);
		//Radio.workmode = RADIO_WORK_IDLE;
		info = reset;
		info |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT6);
		radioinfoState1 &= (u8)(~info);

		info = reset;
		info |= BIT0;
		radioinfoState2 &= (u8)(~info);

		//info = reset;
		//info |= (BIT4 | BIT5 | BIT6);
		//rdsinfoState3 &= (u8)(~info);
		//SysDelay(50);
	}
	//Radio.workmode = RADIO_WORK_IDLE;
	//ReportRadioBaseInfo();
	//RadioSetFreq(Radio.freq,0);
}

//=================================================================
//Function Nam: RadioPoweron()
//Descript:
//=================================================================
void Radio7786Poweron(void)
{
	//if (bradioPoweron == set)		return;
	//Radio_7786.initerrorcount = reset;
	//Radio_7786.WorkMode = RADIO_WORK_STARTUP;
	//rRadMainSt = _RadInit;
	rRadSubSt = TUNER_STARTUP_REQ;
	//rd_startup_fsm = RD_STARTUP_REQ;
	//Tuner.WorkMode=TUNER_STARTUP_REQ;
	//radioworktimer10ms = reset;
	//bradioPoweron = set;
}

 /*********************************************
	Function:	      RadioChangeBand
	Description:   Radio change band
*********************************************/
void Radio7786ChangeBand( unsigned char  bandreq, unsigned int freqreq )
{
	unsigned char temp;
	TunerChangeBand(bandreq,freqreq);
	//if(bandreq == CHANGE_FM)
	//{
	//	if(F_AFEnable)
	//	{
	//		Enable_RDS_Int();
	//	}
	//}
	//else if(F_AFEnable)//disable RDS int not in FM band
	//{
	//	Disable_RDS_Int();
	//}
	Radio7786SetFreq(freqreq);

#if 0
    if(HD_Enable_flag == 0)
    {
         //HD_Enable
        //tda7706 send cmd 0x06
        HIT_Cmd_IBOC_Blender();
        //tda7706 send cmd 0x08
        HIT_CmdSetFreq(9810,InjectSide_Auto);
        //sta680 send 0x01
        HDCmd_IBOC_Acquire();

        //same to hit2 gui
        Wait1ms(20);
        //sta680 tune select
        HDCmd_Sys_Tune_Tune_Select(9810,0 ,0);
        HD_Enable_flag = 1;
        Wait1ms(20);
        HDCmd_IBOC_Acquire();
        Wait1ms(20);
        //sta680 tune select
        HDCmd_Sys_Tune_Tune_Select(9810,0 ,0);
        //sta680 get status
        //HDCmd_Sys_Tune_Tune_Get_Status();
    }
    else
    {

        //tda7706 send cmd 0x06
        HIT_Cmd_IBOC_Blender();
        //sta680 send 0x01
        HDCmd_IBOC_Acquire();

        //same to hit2 gui
        Wait1ms(20);
        HD_Enable_flag = 0;
    }
#endif
}


/***************************************************************************************\
*	   Function     : Initial radio preset station										*
*	   Input        :																	*	
*	   Output	: 																		*
*	   Description	: 																	*	
\*-------------------------------------------------------------------------------------*/			
void	InitFMPresetStat( )
{
	const	uint 	*pRomInt = 0;
	uint	*pRamInt;
	
	/*Initialize  FM  M2~M5*/
	if(rAreaSt == _Asia || rAreaSt == _MidEast)
		pRomInt = FMStatInitTab;
	else if(rAreaSt == _Europe1 ||rAreaSt == _Europe2)
		pRomInt = FMStatInitEurTab;
	else if(rAreaSt == _USA)
		pRomInt = FMStatInitUSATab;
	else if(rAreaSt == _Latin)
		pRomInt = FMStatInitLatinTab;
	else if(rAreaSt == _Aus)
		pRomInt = FMStatInitAusTab;
	else if(rAreaSt == _Russia)
	{
		if(cBand == _BandFM3)
			pRomInt = FMORTStatInitTab;
		else
			pRomInt = FMStatInitRussiaTab;
	}
	else if(rAreaSt == _Japan)
		pRomInt = FMStatInitJapenTab;
	else if(rAreaSt == _Brazil)
		pRomInt = FMStatInitBrazilTab;

	pRamInt = &FMPresetMem[0][0];
	for( ; ; )
	{
		//*(pRamInt + 12)	= 				/*FM 3*/
		//*(pRamInt +  6) = 					/*FM 2*/
		*pRamInt 		= *pRomInt;		/*FM 1*/
		if(pRamInt == &FMPresetMem[2][5])	break;
		++pRomInt;
		++pRamInt;
	}
	
	if(rAreaSt == _Russia)
	{
		pRomInt = FMORTStatInitTab;/*FM ORT*/
		pRamInt = &FMPresetMem[2][0];
		for( ; ; )
		{
			*pRamInt 		= *pRomInt;		
			if(pRamInt == &FMPresetMem[2][5])	break;
			++pRomInt;
			++pRamInt;
		}
	}

}
/***************************************************************************************\
*	   Function     : Initial radio preset station										*
*	   Input        :																	*	
*	   Output	: 																		*
*	   Description	: 																	*	
\*-------------------------------------------------------------------------------------*/			
void	InitAMPresetStat( )
{
	const	uint 	*pRomInt = 0;
	uint	*pRamInt;

	/*Initialize  AM  M2~M5*/
	if(rAreaSt == _Asia || rAreaSt == _MidEast)
		pRomInt = AMStatInitTab;
	else if(rAreaSt == _Europe1)
		pRomInt = AMStatInitEurTab;
	else if(rAreaSt == _USA || rAreaSt == _Latin )
		pRomInt = AMStatInitUSATab;
	else if(rAreaSt == _Latin)
		pRomInt = AMStatInitLATINTab;
	else if(rAreaSt == _Aus)
		pRomInt = AMStatInitAusTab;
	else if(rAreaSt == _Russia)
		pRomInt = AMStatInitRussiaTab;
	else if(rAreaSt == _Japan)
		pRomInt = AMStatInitJapenTab;
	else if(rAreaSt == _Brazil)
		pRomInt = AMStatInitBrazilTab;

	pRamInt = &AMPresetMem[0][0];
	for( ; ; )
	{
		//*(pRamInt + 6) 	= 				/*AM 2*/
		*pRamInt			= *pRomInt;		/*AM 1*/
		if(pRamInt == &AMPresetMem[1][5])	break;
		++pRomInt;
		++pRamInt;
	}

	if(rAreaSt == _Japan || rAreaSt == _Europe2)
	{
		pRamInt = &AMPresetMem[1][0];
		pRomInt = &LWStatInitJapenTab[0];
		for( ; ; )
		{
			//*(pRamInt + 6) 	= 				/*AM 2*/
			*pRamInt			= *pRomInt;		/*AM 2*/
			if(pRamInt == &AMPresetMem[1][5])	break;
			++pRomInt;
			++pRamInt;
		}
	}
}

/***************************************************************************************\
*	   Function     : Initial radio preset station										*
*	   Input        :																	*	
*	   Output	: 																		*
*	   Description	: 																	*	
\*-------------------------------------------------------------------------------------*/			
void	InitPresetStat( )
{
	InitFMPresetStat();
	InitAMPresetStat();
}

/*******************************************************************************\
*   Function: Radio Parameter Table Start Address			 					*
*   Input   : cBand,bAM_FM														*
*   Output  : target parameters table start Address								*
\*-----------------------------------------------------------------------------*/			
const	uchar	*TunParmStartAddr( )
{
	if(!bAM_FM)
	{
		{
			if(rAreaSt == _Asia || rAreaSt == _MidEast)
				return(FMPrmTab);
			else if(rAreaSt == _Europe1 || rAreaSt == _Europe2)
				return(FMPrmEurTab);
			else if(rAreaSt == _USA)
				return(FMPrmUSATab);
			else if(rAreaSt == _Latin)
				return(FMPrmLATINTab);
			else if(rAreaSt == _Aus)
				return(FMPrmAusTab);
			else if(rAreaSt == _Russia)
			{
				if(cBand == _BandFM3)	
					return(FMORTPrmTab);
				else
					return(FMPrmRussiaTab);
			}
			else if(rAreaSt == _Japan)
				return(FMPrmJapenTab);
			else if(rAreaSt == _Brazil)
				return(FMPrmBrazilTab);
		}
	}
	else
	{
		if(rAreaSt == _Asia || rAreaSt == _MidEast)
		return(AMPrmTab);
		else if(rAreaSt == _Europe1 || rAreaSt == _Europe2)
		{
			if(cBand == _BandAM2 &&  rAreaSt == _Europe2)	
				return(LWPrmJapanTab);
			else
				return(AMPrmEurTab);
		}
		else if(rAreaSt == _USA || rAreaSt == _Latin )
		return(AMPrmUSATab);
		else if(rAreaSt == _Aus)
		return(AMPrmAusTab);
		else if(rAreaSt == _Russia)
		return(AMPrmRussiaTab);
		else if(rAreaSt == _Japan)
		{
			if(cBand == _BandAM2)	
				return(LWPrmJapanTab);
			else
				return(AMPrmJapanTab);
		}
		else if(rAreaSt == _Brazil)
		return(AMPrmBrazilTab);

	}
	return(0);
}
/*******************************************************************************\
*   Function: Radio Parameter Table Start Address			 					*
*   Input   : cBand,bAM_FM														*
*   Output  : target parameters table start Address								*
\*-----------------------------------------------------------------------------*/			
const	uchar	*TunFMParmStartAddr( )
{
	if(rAreaSt == _Asia || rAreaSt == _MidEast)
		return(FMPrmTab);
	else if(rAreaSt == _Europe1 || rAreaSt == _Europe2)
		return(FMPrmEurTab);
	else if(rAreaSt == _USA)
		return(FMPrmUSATab);
	else if(rAreaSt == _Latin)
		return(FMPrmLATINTab);
	else if(rAreaSt == _Aus)
		return(FMPrmAusTab);
	else if(rAreaSt == _Russia)
	{
		if(cBand == _BandFM3)	
			return(FMORTPrmTab);
		else
			return(FMPrmRussiaTab);
	}
	else if(rAreaSt == _Japan)
		return(FMPrmJapenTab);
	else if(rAreaSt == _Brazil)
		return(FMPrmBrazilTab);
	return(0);
}
/*******************************************************************************\
*   Function: Radio Parameter Table Start Address			 					*
*   Input   : cBand,bAM_FM														*
*   Output  : target parameters table start Address								*
\*-----------------------------------------------------------------------------*/			
const	uchar	*TunFMORITParmStartAddr(void)
{
	return(FMORTPrmTab);
	return(0);
}
/*******************************************************************************\
*   Function: Radio Parameter Table Start Address			 					*
*   Input   : cBand,bAM_FM														*
*   Output  : target parameters table start Address								*
\*-----------------------------------------------------------------------------*/			
const	uchar	*TunAMParmStartAddr(void)
{
	if(rAreaSt == _Asia || rAreaSt == _MidEast)
	return(AMPrmTab);
	else if(rAreaSt == _Europe1 || rAreaSt == _Europe2)
	{
		if(cBand == _BandAM2 &&  rAreaSt == _Europe2)	
			return(LWPrmJapanTab);
		else
			return(AMPrmEurTab);
	}
	else if(rAreaSt == _USA || rAreaSt == _Latin )
	return(AMPrmUSATab);
	else if(rAreaSt == _Aus)
	return(AMPrmAusTab);
	else if(rAreaSt == _Russia)
	return(AMPrmRussiaTab);
	else if(rAreaSt == _Japan)
	{
		if(cBand == _BandAM2)	
			return(LWPrmJapanTab);
		else
			return(AMPrmJapanTab);
	}
	else if(rAreaSt == _Brazil)
	return(AMPrmBrazilTab);
}


/*******************************************************************************\
* Function: restore station number for abort seeking station	 				*
* Input   : 																	*
* Output  : 																	*
\*-----------------------------------------------------------------------------*/			
void	RestoreStatNum( )
{
	cRadStatNum.word = cRadStatBak.word;
	cPSNum = cPSNumBak;	
	
	if(bRadSeeking)
	TunerSeekOff(SeekStopUnmuted);
	StartWrPll( );
}

void	RestoreStatNum1( )
{
	cRadStatNum.word = cRadStatBak.word;
	cPSNum = cPSNumBak;	
	
	if(bRadSeeking)
	TunerSeekOff(SeekStopUnmuted);
	Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
}

/*******************************************************************************\
*Function 	: Save Current band station message									*
\*-----------------------------------------------------------------------------*/			
void	SaveCurBandMsg(void)
{
	TYPE_WORD	dw;
//	uchar	d;
	
	if(bRadSeeking)
	{/*radio seeking status,restore backup station number */
		RestoreStatNum( );
		TunerSeekOff(SeekStopUnmuted);	
		bRadSeeking = False;
	}
	dw.word = cRadStatNum.word;
	
	dw.ByteH |= (cPSNum << 5);
	BandMsgMem[cBand] = dw.word;
	// start save to EEROM
	//d = (cBand << 1) + _emFM1BandMsgAddr;
	//StartWrEeromByte(d++,dw.ByteL);
	//StartWrEeromByte(d,dw.ByteH);
	bRadFreqChgd = 0;
}

/*******************************************************************************\
* Function 	: read Current band station message									*
*  Input	: 																	*
\*-----------------------------------------------------------------------------*/			
void	ReadBandMsg( )
{
	TYPE_WORD	dw;

	/*Read band station number*/
	bRadSeeking = False;
	
	dw.word = BandMsgMem[cBand];
	cPSNum = dw.ByteH >> 5;
	dw.ByteH &= 0x1f;
	cRadStatNum.word = dw.word;
	Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
	//if(!bAM_FM)	ReadRdsPreMem(cPSNum);
}

/*******************************************************************************\
*Function: set radio main procedure to delay for enter standby subroutine 		*
*Input   : rRadMainSt															*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	RadSetToDl500ms(uchar d)
{
	if(rRadMainSt < _RadStdby)	return;
	RadioBreak();
	Rad_X64ms = 512/64;
	rRadMainSt = _RadDlToStdby;	
	bRadSeeking = False;
	Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
	ReportRadioBaseInfo();
	ReportRadioPreListinfo();
#if	_UartDebug
	Printf("\r\n RadSetToDl500ms  :%d,%x",Radio_7786.Freq,d);
#endif

}

/*******************************************************************************\
*Function: read station number from preset memory								*
*Input   : 																		*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	ReadPresetMem(uchar band,uchar n)		
{
	if(bAM_FM)
	{
		cRadStatNum.word = AMPresetMem[band - _BandAM1][n - 1];
	}
	else
	{
		cRadStatNum.word = FMPresetMem[band][n - 1];
		//ReadRdsPreMem(cPSNum);
	}
	//rRadInfoID = _RadPreList;
	//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
	Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
	Radio_7786.CurrentBand = band;
	if(rRadMainSt == _RadPS)	ReportRadioPreListinfo();

}		
/*******************************************************************************\
* Function: write current station number to preset memory		 				*
* Input   : preset station number												*
*  Output : none																*
\*-----------------------------------------------------------------------------*/			
void	WritePresetMem(uchar band,uchar n)		
{
	uchar	i;
	
	i = n - 1;
	if(bAM_FM)	AMPresetMem[band - _BandAM1][i] = cRadStatNum.word;
	else
	{
		FMPresetMem[band][i] = cRadStatNum.word;
	}
	
	Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
	Radio_7786.CurrentBand = band;
	ReportRadioPreListinfo();

	// start save to EEROM
	//i = cBand * 12 + (i << 1) + _emFM1PresetAddr;
	//StartWrEeromByte(i,cRadStatNum.ByteL);
	//StartWrEeromByte(i,cRadStatNum.ByteH);
	//rRadInfoID = _RadPreList;
	//ArmTxCmd(0x81, 0xb4);/*return Rad flag */

}		

/*******************************************************************************\
* Function: Start to Program PLL					 							*
* Input   : 																	*
* Output  : 																	*
*Description: if system is already mute status , write PLL immediately			*
*		     else	mute first													*
\*-----------------------------------------------------------------------------*/			
void	StartWrPll(void)
{
	rPrgmPllSt = 0;/*set to delay for write PLL status */
	bPrgmPll = True;
	bPllStable = False;
	//bRadSignalFail = 0;
	bChkStatEn = False;
	bSTOff = True;
	bSTCkSt = 0;/*stable status*/
	Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);

}
/*******************************************************************************\
*Function: backup station number for end or abort seeking station	 			*
*Input   : 																		*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	BkupStatNum( )
{
	cRadStatBak.word = cRadStatNum.word;
	cPSNumBak = cPSNum;
}

/*******************************************************************************\
*Function: save original station number for end or abort SCAN&AS	 			*
*Input   : 																		*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	SaveOriginalStat( )
{
	OriginStNum.word = cRadStatNum.word;
}

/*******************************************************************************\
*Function : Get maximun radio station number of current band					*
*Input    :																		*	
*Output	: max station number													*
*Description	: 																*	
\*-----------------------------------------------------------------------------*/			
uint	GetMaxStat(void)
{
	TYPE_WORD	dw;
	uchar const *p;
	
	p = TunParmStartAddr( );/*start address in TabPoint(HL)*/
	
	dw.ByteH = *(p + 5);
	dw.ByteL = *(p + 6);
	
	return(dw.word);
}

uint	GetMaxOctStat(void)
{
	TYPE_WORD	dw;
	uchar const *p;
	
	p = TunParmStartAddr( );/*start address in TabPoint(HL)*/
	dw.ByteH = *(p + 7);
	dw.ByteL = *(p + 8);
	
	return(dw.word);
}
/*******************************************************************************\
*Function: Preset station check					 								*
*Input   : 																		*
*Output  : 																		*
*Description: 检查当前波段中是否有当前频率的预置电台存在.						*
\*-----------------------------------------------------------------------------*/			
uchar PSStationChk( )
{
	uchar	i;
	i = 0;
	for( ; ; )
	{
		if(!bAM_FM)
		{
			if(FMPresetMem[cBand][i++] == cRadStatNum.word)	break;
		}
		else
		{
			if(AMPresetMem[cBand - _BandAM1][i++] == cRadStatL)	break;
		}
		if(i > 5)	return 0xff;
	}
	return i;
}
/*******************************************************************************\
* Function: 当前电台数减一 and check maximun station							*
* Input   : mode, 0: manual, 1: auto tunning									*
* Output  : 1: current band minimun station, 0: not minimun station				*
\*-----------------------------------------------------------------------------*/			
#define	_SeekModeManu		0
#define	_SeekModeAuto		1
uchar	StatNumDec(uchar m)
{
	uchar	d;
	ROMuchar *p;

	p = TunParmStartAddr( );/*start address in TabPoint(HL)*/
	//rSi47xxTxBuff[0] = *p; 	//Band  0:AM,LW  1:FM  2: SW  3: OIRT
	rTempi = *(p + 2);		//Step

	bRadFreqChgd = 1;
	#if 1
	if(rAreaSt == _Russia && cBand == _BandFM3)	
	{
		if(cRadStatNum.word)
		{
			--cRadStatNum.word;/*station number decrement*/
			d = False;
		}
		else
		{/*current station minimun number*/
			cRadStatNum.word = GetMaxStat( );	/*get maximun station */
			d = True;	/*set hop frequency flag*/
		}
	}
	else
	{
		if(cRadStatNum.word)
		{
			--cRadStatNum.word;/*station number decrement*/
			if(m == _SeekModeAuto && !bAM_FM)
			{
				if(!bAM_FM)
				{// FM, manual: 50KHz, auto: 100KHz
					if(rAreaSt == _USA && (cRadStatNum.word%2 != 0))
					{
					//	if(cRadStatNum.bit.b0)	--cRadStatNum.word;
					}
					if(rAreaSt != _Russia && rTempi < 100 || rAreaSt == _Russia && cBand <= _BandFM2)
					{
						if(cRadStatNum.bit.b0)	cRadStatNum.bit.b0 = 0;
					}
				}
			}
			d = False;
		}
		else
		{/*current station minimun number*/
			cRadStatNum.word = GetMaxStat( );	/*get maximun station */
			d = True;	/*set hop frequency flag*/
		}
	}
		//rRadInfoID = _RadPreList;
		//ArmTxCmd(0x81, 0xb4);/*radio info*/	
	#else
	if(cRadStatNum.word)
	{
		--cRadStatNum.word;/*station number decrement*/
		d = False;
	}
	else
	{/*current station minimun number*/
		cRadStatNum.word = GetMaxStat( );	/*get maximun station */
		d = True;	/*set hop frequency flag*/
	}
	#endif
	//PSStationChk( );
	return(d);
}

/*******************************************************************************\
*Function: 当前电台数加一 and check maximun station								*
* Input   : mode, 0: manual, 1: auto tunning									*
*Output  : 1: current band maximun station, 0: not maximun station				*
\*-----------------------------------------------------------------------------*/			
uchar	StatNumInc(uchar m)
{
	uchar	d;
	ROMuchar *p;

	p = TunParmStartAddr( );/*start address in TabPoint(HL)*/
	//rSi47xxTxBuff[0] = *p; 	//Band  0:AM,LW  1:FM  2: SW  3: OIRT
	rTempi = *(p + 2);		//Step
	
	bRadFreqChgd = 1;
	#if 1
	if(rAreaSt == _Russia && cBand == _BandFM3)	
	{
		if(cRadStatNum.word < GetMaxStat( ))
		{
			++cRadStatNum.word;/*increment station number*/
			d = False;/*not maximun station flag*/
		}
		else
		{/*current station is maximun station*/
			cRadStatNum.word = 0;/*restart from No. 1*/
			d = True;/*maximun station flag*/
		}
	}
	else
	{
		if(cRadStatNum.word < GetMaxStat( ))
		{
			++cRadStatNum.word;/*increment station number*/
			if(m == _SeekModeAuto && !bAM_FM)
			{
				if(!bAM_FM)
				{// FM manual: 50KHz, auto: 100KHz
					if(rAreaSt == _USA && (cRadStatNum.word%2 != 0))
					{
							//if(cRadStatNum.bit.b0)	++cRadStatNum.word;
					}
					else if(rAreaSt != _Russia && rTempi < 100 || rAreaSt == _Russia && cBand <= _BandFM2)
					{
						if(cRadStatNum.bit.b0)	++cRadStatNum.word;
					}
				}
			}
			d = False;/*not maximun station flag*/
		}
		else
		{/*current station is maximun station*/
			cRadStatNum.word = 0;/*restart from No. 1*/
			d = True;/*maximun station flag*/
		}	
	}
	//rRadInfoID = _RadPreList;
	//ArmTxCmd(0x81, 0xb4);/*radio info*/	
	#else
	if(cRadStatNum.word < GetMaxStat( ))
	{
		++cRadStatNum.word;/*increment station number*/
		d = False;/*not maximun station flag*/
	}
	else
	{/*current station is maximun station*/
		cRadStatNum.word = 0;/*restart from No. 1*/
		d = True;/*maximun station flag*/
	}
	#endif
	//if(rRadMainSt != _RadAS && rRadMainSt != _RadPS)	PSStationChk( );
	return(d);
}

uint GetRadFreOctData(uint McuFreX16)
{
	ROMuchar *p;
	TYPE_WORD FreData,FreMin,FreMax;
	if(McuFreX16 > GetMaxStat( ))	McuFreX16=GetMaxStat( );
	p = TunParmStartAddr( );/*start address in TabPoint(HL)*/

	FreMin.ByteH = *(p + 3);
	FreMin.ByteL = *(p + 4);
	FreMax.ByteH = *(p + 7);
	FreMax.ByteL = *(p + 8);
	Radio_7786.MinFreq = FreMin.word;
	Radio_7786.MaxFreq = FreMax.word;
	Radio_7786.Step = *(p + 2);
	
	if(bAM_FM)
	{
		rTempi = *(p + 2);
	}
	else
	{

		rTempi = *(p + 2)/10;
	}
	
	/* 计算当前电台分频系数,偏移量+起始值 */ /*cRadStat * step length*/
	FreData.word =FreMin.word + McuFreX16 * rTempi;//StatNumMulN(rTempi);
	//Radio_7786.Freq = FreData.word;

	return(FreData.word);
}

uint GetRdsAFFreq(uint McuFreX16)
{
	ROMuchar *p;
	TYPE_WORD FreData,FreMin,FreMax;
	//if(McuFreX16 > GetMaxStat( ))	McuFreX16=GetMaxStat( );

	p = TunParmStartAddr( );

	FreMin.ByteH = *(p + 3);
	FreMin.ByteL = *(p + 4);
	FreMax.ByteH = *(p + 7);
	FreMax.ByteL = *(p + 8);
	Radio_7786.MinFreq = FreMin.word;
	Radio_7786.MaxFreq = FreMax.word;
	rTempi = 100/10;/*100Khz*/
	
	/* 计算当前电台分频系数,偏移量+起始值 */ /*cRadStat * step length*/
	FreData.word = FreMin.word + McuFreX16 * rTempi;//StatNumMulN(rTempi);
	if(FreData.word >= Radio_7786.MaxFreq) FreData.word = Radio_7786.MaxFreq;
	return(FreData.word);
}

void GetRadPlayFre(uint ArmFreX16)
{
	ROMuchar *p;
	TYPE_WORD	FreDataOtc1,FreDataMin,FreDataMax;
	uint FreDataOtc2;

	
	Radio_7786.Freq = FreDataOtc2 = ArmFreX16;
	p = TunParmStartAddr( );
	FreDataMin.ByteH = *(p+3);
	FreDataMin.ByteL = *(p+4);
	FreDataMax.ByteH = *(p+7);
	FreDataMax.ByteL = *(p+8);
	
	Radio_7786.MinFreq = FreDataMin.word;
	Radio_7786.MaxFreq = FreDataMax.word;
	Radio_7786.Step = *(p + 2);
	
	if(bAM_FM)
	{
		rTempi = *(p + 2);
	}
	else
	{
		rTempi = *(p + 2)/10;
	}

	
	if(FreDataOtc2 >= FreDataMax.word)
	{
		FreDataOtc2 = (FreDataMax.word - FreDataMin.word);
		Radio_7786.Freq = FreDataMax.word;
	}
	else if(FreDataOtc2 >= FreDataMin.word)
		FreDataOtc2 = (FreDataOtc2 - FreDataMin.word);
	else	
		FreDataOtc2 =  0;
	
	cRadStatNum.word = FreDataOtc2/rTempi;
	//StartWrPll( );
	//RadSetToDl500ms(2);	
#if	_UartDebug
	Printf("\r\n select fre = :%d,%x,",ArmFreX16,cRadStatNum.word);
#endif
}


/*******************************************************************************\
* Function	: radio special key check					 						*
* Input  	: key number														*
*Output  	: 0: normal key, 1: special key, 2: disable the key					*
\*-----------------------------------------------------------------------------*/			
uchar	RadSpKyCk(uchar k)
{	
	switch( k )
	{
	   	case  _kcTuneInc:
	   	case  _kcTuneDec:
		case  _kcScan:
		case  _kcST:
		//case  _kcAf:
			if(rRdsMainSt == _Traffic)	return(_DsTheKy);
			break;
			
	    case  _kcSeekInc:
	    case  _kcSeekDec:
	    	if(rRdsMainSt == _Traffic)	return(_DsTheKy);
	    	if(bManuAuto/* || rRadMainSt == _AutoTUp || rRadMainSt == _AutoTDn*/)	break;
		    /*long key enable for change TUNE +/- to manual mode 5s*/
    		ks_X64ms = 1000/64;/*long key time 1000ms*/
    		return(_NormalKy);
    	
    		case  _kcPS:	
	    //case  _kcPty:
	    //case  _kcScan:
	    	if(rRdsMainSt == _Traffic)	return(_DsTheKy);
	    	   		
	 	default:
			return(_NormalKy);
	}
	return(_OnlyShortKy);
}

void	RadDpUpdate(uchar rDpSt)             
{
	
}

/*******************************************************************************\
*Function: 启动电台检查程序						 								*
*Input   : 置位bChkStatEn,复位ChkStatSt											*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	StartChkStat( )
{
	bChkStatEn = True;
	
	ChkStatSt = _ChkStatInit;
}

/*******************************************************************************\
*Function	: Radio atuo scan and save to preset memory procedure	 			*
*Input   	: rRadSubSt															*
* Output  	: 																	*
*Description: scan all freq-point of current band for one loop					*
\*-----------------------------------------------------------------------------*/			
/*auto preset memory store procedure status define*/

void	ASProc( )
{
	uchar	r,i,s,j;
	uint 	*pPrest,*p,AsTotalStat;
	TYPE_WORD dw;
	
	switch(rRadSubSt)
	{
		case _asWrFirstFre:
			if(Rad_Seek_X4ms)	return;
			bRadSeeking = True;
			rRadSubSt = _asWrPll;
			SaveOriginalStat( );/*backup original station number */ 
			break
				;
		case  _asWrPll:
		 #if 1
				if(Rad_X4ms)	return;
				r = StatNumInc(_SeekModeAuto);/* seeking up */
				if(r == 1)
				{
					bRadHopFreq = 1;// set hopping frequency lfag
				}
				Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);	
				TunerSeekOn(Radio_7786.Step);
				StartWrPll( );	/*start write PLL procedure*/
				Rad_X4ms = 10/4;
				rRadSubSt++;
 		#else
			#ifdef _EnableRadSi4730
				if( StatNumInc(_SeekModeAuto ) )	bRadHopFreq = 1;// set hopping frequency lfag
				/*set display frequency*/
				StartWrPll( );	/*start write PLL procedure*/
			#else
				/*station increment and check maximun station*/
				if( StatNumInc(_SeekModeAuto ) )	bRadHopFreq = 1;// set hopping frequency lfag
				/*set display frequency*/
				StartWrPll( );	/*start write PLL procedure*/
			#endif
			++rRadSubSt;
		#endif
			return;
	   			
		case  _asWtPllStable:
	   		if(bPllStable)
	   		{/*now, PLL is stable*/
	   			StartChkStat( );
	   			++rRadSubSt;/*convert to wait check station end*/
	   		}
	   		return;
	   			
		case  _asWtChkSt:	/*wait check station end*/	
	 		if( bChkStatEn )	return;
	 		/*check station end process*/
	 		if(bStatPresent)
	 		{
	 			if(bAM_FM)
 				{
 					AMAsStat++;
					if(rAreaSt == _Japan || rAreaSt == _Europe2)
					AsTotalStat = 6;
					else
					AsTotalStat = 12;
					if(AMAsStat <= AsTotalStat)
		   			{
						AMAsMem[2*(AMAsStat-1)] = rRadMeterLevel;
						AMAsMem[2*(AMAsStat-1)+1] = cRadStatNum.word;
						
						if(rAreaSt == _Japan || rAreaSt == _Europe2)
						{
							if(cBand == _BandAM1)
							{
								for(i = 0;i < AMAsStat;i++)
								{
									AMPresetMem[0][i%6-1] = AMAsMem[2*i+1];
								}
							}
							else
							{
								for(i = 0;i < AMAsStat;i++)
								{
									AMPresetMem[1][i%6-1] = AMAsMem[2*i+1];
								}

							}
						}
						else
						{
							for(i = 0;i < AMAsStat;i++)
							{
								AMPresetMem[i/6][i%6-1] = AMAsMem[2*i+1];
							}
						}
						ReportRadioPreListinfo();
					}
					else
					{
						AMAsStat = AsTotalStat;
						p = &AMAsMem[2*AsTotalStat-2];
						for(i = j = AsTotalStat,s = 0xff;; )
						{
							if(s > *p)
							{
								s = *p;
								j = i;
							}
							if(i <= 1)	break;	
							i--;
							p = p-2;
						}
						
						if(rRadMeterLevel > s)
						{ 
							for(;;)
							{
								if(j > AsTotalStat) break;
								AMAsMem[2*j] = AMAsMem[2*(j+1)];
								AMAsMem[2*j+1] = AMAsMem[2*(j+1)+1];
								j++;
							}   
							
							AMAsMem[2*(AsTotalStat-1)] = rRadMeterLevel;
							AMAsMem[2*(AsTotalStat-1)+1] = cRadStatNum.word;

							if(rAreaSt == _Japan || rAreaSt == _Europe2)/*6个频点*/
							{
								if(cBand == _BandAM1)
								{
									for(i = 0;i < AsTotalStat;i++)
									{
										AMPresetMem[0][i%6] = AMAsMem[2*i+1];
									}
								}
								else
								{
									for(i = 0;i < AsTotalStat;i++)
									{
										AMPresetMem[1][i%6] = AMAsMem[2*i+1];
									}

								}
							}
							else
							{
								for(i = 0;i < AsTotalStat;i++)
								{
									AMPresetMem[i/6][i%6] = AMAsMem[2*i+1];
								}
							}
							
							ReportRadioPreListinfo();
						}
					}
					
				}
				else
				{
	 				if(rAreaSt == _Russia)	
	 				{
		 				if(cReqBand == _BandFM3)
							AsTotalStat = 6;
						else
							AsTotalStat = 12;
	 				}
					else 	AsTotalStat = 18;
					
					FMAsStat++;
					if(FMAsStat <= AsTotalStat)
		   			{
						FMAsMem[2*(FMAsStat-1)] = rRadMeterLevel;
						FMAsMem[2*(FMAsStat-1)+1] = cRadStatNum.word;
						if(rAreaSt == _Russia && cReqBand == _BandFM3)	
						{
							for(i = 0;i < FMAsStat;i++)
							{
								FMPresetMem[2][i%6] = FMAsMem[2*i+1];
							}
						}
						else
						{
							for(i = 0;i < FMAsStat;i++)
							{
								FMPresetMem[i/6][i%6] = FMAsMem[2*i+1];
							}
						}
						
						ReportRadioPreListinfo();
					}
					else
					{
						FMAsStat = AsTotalStat;
						p = &FMAsMem[2*AsTotalStat-2];
						for(i = j = AsTotalStat,s = 0xff; ;)
						{
							if(s > *p)
							{
								s = *p;
								j = i;
							}
							if(i <= 1)	break;	
							i--;
							p = p-2;
						}
						
						if(rRadMeterLevel > s)
						{ 
							for(;;)
							{
								if(j > AsTotalStat) break;
								FMAsMem[2*(j-1)] = FMAsMem[2*j];
								FMAsMem[2*(j-1)+1] = FMAsMem[2*j+1];
								j++;
							}   
							
							FMAsMem[2*(AsTotalStat-1)] = rRadMeterLevel;
							FMAsMem[2*(AsTotalStat-1)+1] = cRadStatNum.word;
							if(rAreaSt == _Russia && cReqBand == _BandFM3)	
							{
								for(i = 0;i < FMAsStat;i++)
								{
									FMPresetMem[2][i%6] = FMAsMem[2*i+1];
								}
							}
							else
							{
								for(i = 0;i < FMAsStat;i++)
								{
									FMPresetMem[i/6][i%6] = FMAsMem[2*i+1];
								}
							}

							//rRadInfoID = _RadPreList;
							//ArmTxCmd(0x81, 0xb4);
							//RDSTxStatNum();
							//RDSTxStatMEM();
							ReportRadioPreListinfo();
						}
					}
				
				}
					
			}
			if(OriginStNum.word != cRadStatNum.word)
			//if(GetMaxStat( ) != cRadStatNum.word)
			{/* No , Scan next station */
				rRadSubSt = _asWrPll;
			}
			else
			{/* Yes, AS 结束处理 */
				#if 1
				if(!bAM_FM)
				{
					for(i = 0;i<6;i++)
					{
					//	FMPresetMem[0][i] = FMAsMem[2*i+1];
					//	FMPresetMem[1][i] = FMAsMem[12+2*i+1];
					//	FMPresetMem[2][i] = FMAsMem[24+2*i+1];
					}
					if(rAreaSt == _Russia && cReqBand == _BandFM3)	
	 				{
	 					if(cFMOrtNum)	cFMOrtNum = FMAsStat;
					}
					else if(FMAsStat)		cFMASNum = FMAsStat;
				}
				else
				{
					for(i = 0;i<6;i++)
					{
					//	AMPresetMem[0][i] = AMAsMem[2*i+1];
					//	AMPresetMem[1][i] = AMAsMem[12+2*i+1];
					}
					if(rAreaSt == _Japan || rAreaSt == _Europe2)/*6个频点*/
					{
						if(cSWASNum)	cSWASNum = AMAsStat;	
					}
					else if(AMAsStat)		cAMASNum = AMAsStat;	
				}
				bRadSeek2nd = 0;
				
				if(bRadSeeking)
				TunerSeekOff(SeekStopUnmuted);
				bRadSeeking = 0;
				//rRadSubSt = _asWtChgToPs;
				Rad_X64ms = 64/64;
				rRadSubSt = _asWtChgToPs;
				if(rRadMainSt == _RadAS)
				radioinfoState1 &= (u8)(~BIT3);
				radioinfoState2 |= (u8)(BIT0);
				ReportRadioBaseInfo();
				
				#else
				if(bRadSeek2nd)
				{
					bRadSeek2nd = 0;
					rRadSubSt = _asWtChgToPs;
					Rad_X64ms = 0;
				}
				else
				{
					//bLOCEn = 0;
					bRadSeek2nd = 1;
					rRadSubSt = _asWrPll;
				}
				#endif
			}
	 		return;

		case  _asWtChgToPs:	/*AS end to PS process*/
			if(Rad_X64ms)	return;
			#if 1
				if((rAreaSt == _Japan || rAreaSt == _Europe2) && (cAMASNum || cSWASNum))
				{
					if((rAreaSt == _Japan || rAreaSt == _Europe2) && cSWASNum)
					{
						cReqBand = cBand = cBandBak;
					}
					else
					{
						cReqBand = cBand = _BandAM1;
					}
					
					cPSNum = 1;
					ReadPresetMem(cReqBand,1);
				}
				else if(bAM_FM && cAMASNum)
				{
					{
						cPSNum = 1;
						cReqBand = _BandAM2;
						ReadPresetMem(cReqBand,1);
						dw.word = cRadStatNum.word;
						dw.ByteH |= (cPSNum << 5);
						BandMsgMem[cReqBand] = dw.word;
					}

					if(rAreaSt == _Japan || rAreaSt == _Europe2)
						cReqBand = cBand = cBandBak;
					else
						cReqBand = cBand = _BandAM1;
					cPSNum = 1;
					ReadPresetMem(cReqBand,1);
				}
				else if(!bAM_FM && rAreaSt == _Russia && (cFMOrtNum ||cFMASNum) )	
 				{
					if(rAreaSt == _Russia && cReqBand ==_BandFM3 && cFMOrtNum)	
					{
						cPSNum = 1;
						cReqBand = _BandFM3;
						ReadPresetMem(cReqBand,1);		
						dw.word = cRadStatNum.word;
						dw.ByteH |= (cPSNum << 5);
						BandMsgMem[cReqBand] = dw.word;

					}
					else if(rAreaSt == _Russia && cReqBand < _BandFM3 && cFMASNum)	
					{		
						cPSNum = 1;
						cReqBand = _BandFM2;
						ReadPresetMem(cReqBand,1);					
						dw.word = cRadStatNum.word;
						dw.ByteH |= (cPSNum << 5);
						BandMsgMem[cReqBand] = dw.word;
					}
					
					if( rAreaSt == _Russia && cReqBand == _BandFM3) cReqBand = cBand = _BandFM3;
					else  cReqBand = cBand = _BandFM1;
					cPSNum = 1;
					ReadPresetMem(cReqBand,1);
				}
				else if(!bAM_FM && cFMASNum)
				{
					cPSNum = 1;
					cReqBand = _BandFM3;
					ReadPresetMem(cReqBand,1);		
					dw.word = cRadStatNum.word;
					dw.ByteH |= (cPSNum << 5);
					BandMsgMem[cReqBand] = dw.word;
					
					cReqBand = _BandFM2;
					ReadPresetMem(cReqBand,1);	
					dw.word = cRadStatNum.word;
					dw.ByteH |= (cPSNum << 5);
					BandMsgMem[cReqBand] = dw.word;

					if( rAreaSt == _Russia && cReqBand == _BandFM3) cReqBand = cBand = _BandFM3;
					else  cReqBand = cBand = _BandFM1;
					cPSNum = 1;
					ReadPresetMem(cReqBand,1);
				}
				else 
				{
					cReqBand = cBand = cBandBak;
					RestoreStatNum();
				}
				Radio_7786.CurrentBand = cReqBand;

			#if	1/*PS Enable*/
				if(!bAM_FM)	
				{
					if(rAreaSt == _Russia && cBand == _BandFM3)
						cReqBand = cBand = _BandFM3;
					else
						cReqBand = cBand = _BandFM1;
				}
				else 
				{
					if(rAreaSt == _Japan || rAreaSt == _Europe2)
						cReqBand = cBand;
					else
						cReqBand = cBand = _BandAM1;
				}
				BkupStatNum();
				cPSNum = 0;
				rRadMainSt = _RadPS;	
				rRadSubSt = _psWrPll;
			#else
				StartWrPll( );	/*start write PLL procedure*/
				RadSetToDl500ms(3);
			#endif
			//rRadSubSt = 0;




				
			#else
			bManuAuto = 0;
			rRadMainSt = _RadPS;	
			rRadSubSt = 1;
		   	bRadSeeking = False;
			if(!bAM_FM)	
			{
				if(cReqBand == _BandFMORT || rAreaSt == _Russia && cBand == _BandFM3)
					cReqBand = _BandFMORT;
				else
					cReqBand = _BandFM1;
			}
			else 	
			{
					cReqBand = _BandAM1;
			}
			cBandBak = cReqBand;
			cPSNum = 0;
			#endif
			return;
	}
} 

/*******************************************************************************\
* Function: start change radio band					 							*
* Input   : 																	*
* Output  : 																	*
\*-----------------------------------------------------------------------------*/			
void	StartChgBand(uchar b)
{
//	ROMuchar	*p;
//	TYPE_WORD FreData;
	//if(rRadMainSt <= _RadInit)	return;
	//HIT_ForeceMute(ON);
	SaveCurBandMsg( );	/* save current band messages */
	cBandBak = cBand;
	cBand = b;
	Radio_7786.CurrentBand = cBand;
	if(cBand > _BandFM3)
	{/* set AM band indicator */
		bAM_FM = True;
		Disable_RDS_Int();
	}
	else //if(bRdsAfDetEn)
	{
		bAM_FM = False;/* set FM band indicator */
		Enable_RDS_Int();
	}
	
	ReadBandMsg( );
    bManuAuto = 0;/*TUNE +/- key, reset to AUOT status*/
    bSTOff = True;
	bSTCkSt = 0;/*stable status*/
	rRadSubSt = 0;
	rRadMainSt = _ChgBand;
	Rad_X64ms = 256/64;
	bWtChgBand = 1;

	bRadSeeking = False;
	RadioBreak();
	ReportRadioBaseInfo();
	ReportRadioSignalinfo(0);
	ReportRadioPreListinfo();
}

#define	_StopSeekAction		1
#define	_StartPtySeek		2
#define	_RdsTraffic			3

/*******************************************************************************\
* Function: radio key analyse						 							*
* Input   : bNewKey,rKeyResult													*
* Output  : 																	*
\*-----------------------------------------------------------------------------*/			
void	RadKyAnaly(void )
{
	uchar	j = 0;
	uchar	s = 0;
	#if 1

	if(!bNewKey)	return;
	if(armsourceState != ARM_SOURCE_RADIO)	return;
	//if(rRadMainSt <= _RadInit /*&& !bEjtDiscAccOff*/)	return;

#if	_UartDebug
	Printf("\r\n rad key = :%x,test=%d,",rKeyResult,Testtime_x10ms);
#endif
#if	_DebugTestDly
	Printf("\r\n test1 = %d,Rad_X64ms= %d",Testtime_x10ms,rRadMainSt);
#endif

	if(!bRDSDs)	bRdsPtyEn = 0;

	switch(rKeyResult)
	{
		case   _kcPS:
       		if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);	
				if(rRadMainSt == _RadAS)
				radioinfoState1 &= (u8)(~BIT3);
			}

	       	if(rRadMainSt == _RadPS)// || rRadMainSt == _RadAS)
		    {/* already PS status, press PS key will exit PS status*/
				//if( cPSNum )	ReadPresetMem(cPSNum);
				//RestoreStatNum();
		       //	if(rRadSubSt != _psListen5S)
		       	{
		       		StartWrPll( );
		       	}
		       	RadSetToDl500ms(3);  
			}
			else
			{
				bManuAuto = 0;
				cBandBak = cBand;
			   	//bRadSeeking = True; /*set seeking for station flag */
	   			BkupStatNum( );
				#if 1
				if(!bAM_FM)	
				{
					if(rAreaSt == _Russia && cBand == _BandFM3)
						cReqBand = cBand = _BandFM3;
					else
						cReqBand = cBand = _BandFM1;
				}
				else 
				{
					if(rAreaSt == _Japan || rAreaSt == _Europe2)
						cReqBand = cBand;
					else
						cReqBand = cBand = _BandAM1;
				}
				#endif
				//rRadInfoID = _RadPreList;
				//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
				//RDSTxStatNum();
				cPSNum = 0;
				rRadMainSt = _RadPS;	
				rRadSubSt = _psWrPll;
				radioinfoState2 |= (u8)(BIT0);
				radioinfoState1 &= (u8)(~BIT4);
				ReportRadioBaseInfo();
				ReportRadioPreListinfo();
				//ArmTxCmd(C_RADIO, 0x0006);/*return Rad flag */
			}
	      	break;
	    	   
	    case  _kcAms:
       		if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
	       	}
			
			if(rRadMainSt == _RadPS)
			radioinfoState2 &= (u8)(~BIT0);
			bRadSeeking = 0;
			rRdsMainSt = _PtyOff;
			if(rRadMainSt == _RadAS)
			{ /*already AS status, press AS key will exit AS status */
			#if 0
			#else
	 			//if(bAM_FM)	cAMASNum++;
				//else cFMASNum++;
				//if( cPSNum )	ReadPresetMem(cPSNum);
				RestoreStatNum();
				RadSetToDl500ms(4);
				//Printf("\r\n as end:AMPresetMem[0] = :%04X, AMPresetMem[1] = :%04X, AMPresetMem[2] = :%04X, AMPresetMem[3] = :%04X, AMPresetMem[4] = :%04X",AMPresetMem[0][0],AMPresetMem[0][1],AMPresetMem[0][2],AMPresetMem[0][3],AMPresetMem[0][4]);

				//StartWrPll( );
			#endif
			}
			else
			{
				//SaveOriginalStat( );/*backup original station number */ 
		        BkupStatNum();
				bASStatPresent = 0;
				rRadMainSt= _RadAS;
				rRadSubSt = _asWrFirstFre;
				cRadStatNum.word = 0;//AS 从最低端开始扫台
				Rad_Seek_X4ms = 100/4;
				bManuAuto = 0;
				//bRadSeeking = True;
				bRadSeek2nd = 0;
				//bLOCEn = True;
				cReqPSNum = 0;
				bRadHopFreq = 0;
				cPSNum = 0;
				cBandBak = cBand;

				if(!bAM_FM)
				{
					if(rAreaSt == _Russia)
					{
						const	uint 	*pRomInt = 0;
						uint	*pRamInt;

						if(cBand == _BandFM3)
						{
							cFMOrtNum = 6;
							cReqBand = cBand = _BandFM3;
							//if(rAreaSt == _Russia)
							{
								pRomInt = FMORTStatInitTab;/*FM ORT*/
								pRamInt = &FMPresetMem[2][0];
								for( ; ; )
								{
									*pRamInt 		= *pRomInt;		
									if(pRamInt == &FMPresetMem[2][5])	break;
									++pRomInt;
									++pRamInt;
								}
							}
						}
						else
						{
							cFMASNum = 12;
							cReqBand = cBand = _BandFM1;
							pRomInt = FMStatInitRussiaTab;
							pRamInt = &FMPresetMem[0][0];
							for( ; ; )
							{
								*pRamInt 		= *pRomInt;		/*FM 1*/
								if(pRamInt == &FMPresetMem[1][5])	break;
								++pRomInt;
								++pRamInt;
							}
							
						}
					}
					else
					{
						cFMASNum = 18;
						cReqBand = cBand = _BandFM1;
						if(rRdsMainSt != _PtySeek)
						InitFMPresetStat();
					}
					
					for(j = 0;j<36;j++)
					FMAsMem[j] = 0;
					FMAsStat = 0;
				}
				else 
				{
					if(rAreaSt == _Japan || rAreaSt == _Europe2)
					{
						cReqBand = cBand;
						if(cBand == _BandAM1)
							cAMASNum = 6;
						else 
							cSWASNum = 6;
					}
					else
					{
						cReqBand = cBand = _BandAM1;
						cAMASNum = 12;
					}
					for(j = 0;j<24;j++)
					AMAsMem[j] = 0;
					AMAsStat = 0;
					InitAMPresetStat();
				}
				radioinfoState1 |= BIT3;
				radioinfoState1 &= (u8)(~BIT4);
				
				StartWrPll( );
				ReportRadioBaseInfo();
				ReportRadioSignalinfo(0);
				ReportRadioPreListinfo();

			}   

			break;
	    #if 1   
	    case   _kcScan:
			 cPSNum = 0;   
       		if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
	       	}

	       //else 
			 if(rRadMainSt == _RadScan)   
			 {/*already scan status, press SCAN key will exit scan status*/
				if(rRadSubSt != _rsListen5S)
				{
					StartWrPll( );
				}
			    	RadSetToDl500ms(5);
				SaveCurBandMsg( );
			 }
			else	
			{/*start SCAN mode*/
			#ifdef _EnableRadSi4730
				rRadSubSt = 1;
				rRadMainSt = _RadScan;
	   			bManuAuto = 0;
			   	bRadSeeking = True;/*set seeking for station flag*/
	   			SaveOriginalStat( );/*backup original station number*/  
				BkupStatNum( );
		        	cPSNum = 0;
				//ArmTxCmd(C_RADIO, 0x0006);/*return Rad flag */
			#else
				rRadSubSt = 1;
				rRadMainSt = _RadScan;
	   			bManuAuto = 0;
			   	bRadSeeking = True;/*set seeking for station flag*/
	   			SaveOriginalStat( );/*backup original station number*/  
				BkupStatNum( );
		        	cPSNum = 0;
				//ArmTxCmd(C_RADIO, 0x0006);/*return Rad flag */
			#endif
			}	
	      	break;
	      #endif	
	    case   _kcSeekIncManu:  
	    case   _kcSeekDecManu:  
			if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
	       	}

			if( !bManuAuto )	
			{
				//if(rBeepMode == _BeeP2nd)	StartBeepA( );
				//RadTmpDisp(_DpAutoManu,(uchar)(1000/64));
			}
			if(bRadSeeking)
			{
				//bPiSeeking = False;
				RestoreStatNum( );
				//StartWrPll( );
				RadSetToDl500ms(6);	
			}							
	    		bManuAuto = True;	/* 手动搜台 */
			Manu5STmr_X512ms = 5000/512;
			return;	
		
	    case   _kcSeekInc1:
	    case   _kcSeekDec1:	// 自动搜台
	           	cPSNum = 0;   

       		if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
	       	}

	       	//else
			if( !bManuAuto )
			{ // 自动搜台
				if(rKeyResult == _kcSeekDec1)	s = _AutoTDn;
				else	s = _AutoTUp;
			    
			    if(s != rRadMainSt)
			    {/* start auto tunning */
					SaveOriginalStat( );
					BkupStatNum( );
					rRadMainSt = s;
				    rRadSubSt = 1;
					bRadSeeking = True;
					bRadSeek2nd = 0;
					//ArmTxCmd(C_RADIO, 0x0006);/*return Rad flag */
			    }
			    else
			    {/* stop auto tunning */
				    RadSetToDl500ms(7);
					RestoreStatNum( );	/*add for rds*/
			    }
			   	bRadFreqChgd = 1;
			    break;
			}
	      	
	    case   _kcTuneInc:	//Short press "Tune +",手动向上搜台 */
	    case   _kcTuneDec:	//Short press "Tune -",手动向下搜台 */ 	
			Manu5STmr_X512ms = 5000/512;
       		cPSNum = 0;   
			if(bRadSeeking)
	       	{
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
				//if(rRadMainSt == _RadAS)
				//radioinfoState1 &= (u8)(~BIT3);
				RDSFmFreqChange();
	       	}
			
			bRadSeeking = False;
			RadioBreak();
	        if(rRadMainSt != _ManuTHFDl)
	        {// if not hopping frequency delay status, enable manual tunning
				s = rKeyResult;
				if(s == _kcSeekDec || s == _kcTuneDec)
				{
					if( StatNumDec(_SeekModeManu) )	
					{
						rRadMainSt = _ManuTHFDl;
					}
					else	
					{
						rRadMainSt = _RadDlToStdby;
						ReportRadioPreListinfo();
					}
				}
				else// if(s == _kcSeekInc || s == _kcTuneInc)
				{
					if( StatNumInc(_SeekModeManu) )
					{
						rRadMainSt = _ManuTHFDl;
					}
					else	
					{
						rRadMainSt = _RadDlToStdby;
						ReportRadioPreListinfo();
					}
				}
				StartWrPll( );
				Rad_X64ms = 600/64;
	    	}
	      	break;
			
	    case   _kcRdM6:	
	    	++s;
	    case   _kcRdM5:	
	    	++s;
	    case   _kcRdM4:	
	    	++s;
	    case   _kcRdM3:	
	    	++s;
	    case   _kcRdM2:	
	    	++s;
	    case   _kcRdM1:
	    	++s;
			if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				
				//if(rRadMainSt == _RadAS)
				//radioinfoState1 &= (u8)(~BIT3);
				//if(rRadMainSt == _RadPS)
				//radioinfoState2 &= (u8)(~BIT0);
				TunerSeekOff(SeekStopMuted);
	       	}
			
			RadioBreak();
			cReqPSNum = cPSNum = s;
			bRadSeeking = False;
			bManuAuto = 0;
			ReadPresetMem( cBand,cReqPSNum );	// 从相应存储位置读电台数据
			bRadFreqChgd = 1;
			bRadPsNumRead = 1;
			StartWrPll( );	// change PLL to new preset station
			//RadSetToDl500ms(8);
			//rRadInfoID = _RadPreList;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */

			bRadSeeking = False;
			Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
			ReportRadioBaseInfo();
			ReportRadioPreListinfo();
			//RadSetToDl500ms(uchar d)
			rRadMainSt = _RadDlToStdby; 
#if	_DebugTestDly
				Printf("\r\n test11 = %d,Rad_X64ms= %d",Testtime_x10ms,rRadMainSt);
#endif

			
		break;
		
	    case   _kcSeekInc:
	    case   _kcSeekDec:	// 自动搜台
			s = cPSNum ;
			j = cBand;
		#if 0
			if(rKeyResult == _kcSeekInc)
			{
				if(s++ >= 6)	s = 1;
			}
			else
			{
				if(s <= 1)	s = 6;
				else	s--;
			}
		#else/*switch band if cFMASNum/cAMASNum bigger than 6*/
	#if _DebugRad
			Printf("\r\n key1:cBand=%d,cPSNum=%d,cFMASNum=%d,cAMASNum=%d,FMAsStat=%d,AMAsStat=%d",cBand,cPSNum,cFMASNum,cAMASNum,FMAsStat,AMAsStat);
	#endif
			if(rKeyResult == _kcSeekInc)
			{
				if(s++ >= 6)	
				{
					s = 1;
					if(rAreaSt == _Russia && !bAM_FM)
					{
						 if(cBand == _BandFM3) 
							j = _BandFM3;
						 else
						 {
						 	++j;
							if(j > _BandFM2)	j = _BandFM1;
						 }
					}
					else if(bAM_FM && (rAreaSt == _Japan || rAreaSt == _Europe2))
					{

					}
					else
					{
						++j;
						if(bAM_FM)	
						{
							if(j > _BandAM2)	j = _BandAM1;
						}
						else
						{
							if(j > _BandFM3)	j = _BandFM1;
						}
					}
					if(rRadMainSt <= _RadInit)	return;
				}
				/*confirm if current Mn is a active stat,then switch band*/
				if(bAM_FM && (rAreaSt == _Japan || rAreaSt == _Europe2))
				{

				}
				else if(!bAM_FM && (rAreaSt == _Russia) && (cBand == _BandFM3))
				{
				
				}
				else if(!bAM_FM && (j*6+s > cFMASNum))
				{
					s = 1;
					j = _BandFM1;
				}
				else if(bAM_FM && ((j-_BandAM1)*6+s > cAMASNum))
				{
					s = 1;
					j = _BandAM1;
				}

			}
			else
			{
				if(s <= 1)	
				{
					s = 6;
					if(rAreaSt == _Russia && !bAM_FM)
					{
						if(cBand == _BandFM3)
							j = _BandFM3;
						else 
						{
							if(j) 	j--;
							else 
							{
								j = _BandFM2;
							}
						}
					}
					else if(rAreaSt == _Japan || rAreaSt == _Europe2)
					{

					}
					else if(bAM_FM)	
					{
						 if(--j < _BandAM1)	j = _BandAM2;
						 else j = _BandAM1;
					}
					else
					{
						if(j) 	j--;
						else 
						{
							j = _BandFM3;
						}
					}
					if(rRadMainSt <= _RadInit)	return;
				}
				else	
				{
					s--;
				}
				
				/*confirm if current Mn is a active stat,then switch band*/
				if(bAM_FM && (rAreaSt == _Japan || rAreaSt == _Europe2))
				{

				}
				else if(!bAM_FM && (rAreaSt == _Russia) && (cBand == _BandFM3))
				{
				
				}
				else if(!bAM_FM && (j*6+s > cFMASNum))
				{
					s = (cFMASNum-1)%6+1;
					j = (cFMASNum-1)/6;
				}
				else if(bAM_FM && ((j-_BandAM1)*6+s > cAMASNum))
				{
					s = (cAMASNum-1)%6+1;
					j = _BandAM1+(cAMASNum-1)/6;
				}

			}
		#endif
       		if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
	       	}
			cBandBak = cBand = j;
			cReqPSNum = cPSNum = s;
	#if _DebugRad
		Printf("\r\n key2:cBand=%d,cPSNum=%d,cFMASNum=%d,cAMASNum=%d,FMAsStat=%d,AMAsStat=%d",cBand,cPSNum,cFMASNum,cAMASNum,FMAsStat,AMAsStat);
	#endif			  				
			bRadSeeking = False;
			bManuAuto = 0;
			ReadPresetMem( cBand,cReqPSNum );	// 从相应存储位置读电台数据
			bRadPsNumRead = 1;
			bRadFreqChgd = 1;
			StartWrPll( );	// change PLL to new preset station
			RadSetToDl500ms(9);
	      	break;
				
	    case   _kcWrM6:
	    	++s;
	    case   _kcWrM5:	
	    	++s;
	    case   _kcWrM4:
	    	++s;
	    case   _kcWrM3:	
	    	++s;
	    case   _kcWrM2:
	    	++s;
	    case   _kcWrM1:	
	    	++s;
			
			cPSNum = s;  
			bRadPsNumRead = 1;
			WritePresetMem(cBand,cPSNum);	// 将当前电台存储到相应位置\
			
       		if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		if(!bRDSDs)	rRdsMainSt = _PtyOff;
				bRadSeeking = 0;
				if(rRadMainSt == _RadAS)
				radioinfoState1 &= (u8)(~BIT3);
				if(rRadMainSt == _RadPS)
				radioinfoState2 &= (u8)(~BIT0);
				TunerSeekOff(SeekStopUnmuted);
				StartWrPll( );	// change PLL to new preset station
				RadSetToDl500ms(9);
				break;
	       	}
	      	return;
#if 0
		case	_kcClrE2romL:
		#if _RadSysMem
			//ClearAllEerom( );
			for(i = 0;i < MAXWrE2pCnt;i++)
			WrEeromBuff[i] = 0xff;
			//InitVarsFromEerom();
			//ClearAllEerom();
			//MainDisp5S(_mDpClearE2p);
			TmpDp_X64ms = 0;
			cPSNumBak = cPSNum = 0;
			for(i = 0;i<6;i++)
			BandMsgMem[i] = 0; 
			RadioInit();
			bAM_FM = 0;
			cBand = 0;
			bManuAuto = 0;/*TUNE +/- key, reset to AUOT status*/
		    	bSTOff = True;
			bSTCkSt = 0;/*stable status*/
			cRadStatNum.word = 0;
			Rad_X64ms = 200/64;
			rMainVol = _MVolInit;
			rBalance = _SubVolZero;
			rFaderRear = _SubVolZero;	
			rAct = 0x77; //	0xA4;			
		  	rDigitSnd = 0;
			bLoudOn = 0;
			cHours = 0;
			cMinutes = 0;
			ClrUsbTocInfo();
			rFlhFlag.byte = 0x00;
			ClrCurDiscMsg();
			cDispSongHexNum.word = 1;
			Write7313();
			StartBeepA( );
			rRadMainSt = _RadReset;
		#endif
			return;

		case	_kcPty:
		  	//if(rSndSrc!=_SndSrcRad || rRdsMainSt==_Traffic)	return;
			if(bRadSeeking)
	    	{
	    		j = _StopSeekAction;
	    	}
			if(rRdsMainSt==_PtySeek || rRdsMainSt==_PiSeek || rRdsMainSt==_TpSeek || bRadSeeking)
			{/* Stop searching and restore previous station */
				RestoreStatNum( );
				StartWrPll( );
				RadSetToDl500ms(12);
				rRdsMainSt = _PtyOff;
				break;
			}

			if(rMainDpPoint != _mDpRdsPty)
			{
				CPL(bRdsPtyEn);
				//break;
			}
			else
			{
				if(bRdsPtyEn)
				{
					bSeekDn = 0;
					j = _StartPtySeek;
				}
			}
			//MainDisp5S(_mDpSource);
		break;

	#endif	 
		    case   _kcFM:
			#ifdef _EnableRadSi4730
				if(bAM_FM)	s = _BandFM1;
				else
				{
					s = cBand;
					++s;
					if(s > _BandFM3)	s = _BandFM1;
				}
				StartChgBand(s);
				bUserMute = 0;
				bManuAuto = 0;
				cBandBak = cBand;
			#else
				SaveCurBandMsg( );
	       		if(bRadSeeking)
		       	{/* in Pty on state, start pty searching */
					if(!bRDSDs) rRdsMainSt = _PtyOff;
					TunerSeekOff(SeekStopMuted);
		       	}
			    	s = cBand;
				++s;
				if(s > _BandAM2)	s = _BandFM1;
				StartChgBand(s);
				bUserMute = 0;
				bManuAuto = 0;
	    		#endif
	    	break;

		    case   _kcAM:
			#ifdef _EnableRadSi4730
				if(!bAM_FM)	s = _BandAM1;
				else
				{
					s = cBand;
					++s;
					if(s > _BandAM2)	s = _BandAM1;
				}
				StartChgBand(s);
				bUserMute = 0;
				bManuAuto = 0;
				cBandBak = cBand;
			#else
				SaveCurBandMsg( );
	       		if(bRadSeeking)
		       	{/* in Pty on state, start pty searching */
					if(!bRDSDs) rRdsMainSt = _PtyOff;
		       	}
			    	s = cBand;
				++s;
				if(s > _BandAM2)	s = _BandFM1;
				StartChgBand(s);
				bUserMute = 0;
				bManuAuto = 0;
	    		#endif
	    	break;
				

				
	    case   _kcBand:
	       	if(bRadSeeking)
	       	{/* in Pty on state, start pty searching */
	       		rRdsMainSt = _PtyOff;
				TunerSeekOff(SeekStopMuted);
	       	}
			
			//MuteTimeset(50);
		    s = cBand;
			++s;
			if(s > _BandAM2)	s = _BandFM1;
			StartChgBand(s);
			bUserMute = 0;
			bManuAuto = 0;
			cBandBak = cBand;
			//rRadInfoID = _RadPreList;
			//ArmTxCmd(0x81, 0xb4);
			//RDSTxStatNum();
			//RDSTxStatMEM();
	    	break;
			
		case	_kcLoc:
			//CPL(bLOCEn);
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
			return;
			
		case 	_kcST:
			if(bAM_FM)	return;
			//CPL(bMonoOn);
			#ifdef _EnableRadSi4730
			if(!bMonoOn)
				;//si47xx_set_property(0x3501, (u16)0x2D00); 
			else
				;//si47xx_set_property(0x3501, (u16)0x0000); 
			#else
			StartWrTEF6601All();
			#endif
			
			if(!bMonoOn)
			{
				bMonoOn = 1;
				radioinfoState1 &= (u8)(~BIT5);
			}
			else
			{
				bMonoOn = 0;
				radioinfoState1 |= BIT5;
			}
			radioinfoState1 &= (u8)(~BIT4);
			ReportRadioBaseInfo();
			HIT_ForeceMono();

			return;
			
		/*case	_kcShowVerL:
			MainDisp5S(_mDpShowVer);
			if(rBeepMode == _BeeP2nd)
			StartBeepA( );
			TmpDp_X64ms = 0;
			return;*/

		//case	_kcTEF6606AlignL:
			//if(rBeepMode == _BeeP2nd)
			//StartBeepA( );
			//RadTmpDisp(_Dp6606Align,(uchar)(5000/64));
			//return;

	    default:   return;	/*invalid keys return*/
	}
	
	HIT_ForeceMute(ON);
	if(!bRDSDs && rRadMainSt != _RadStdby)
	{
		for(j=0;j<8;j++)
		rRdsPsNameSto[j] = rRdsPsNameAct[j] = ' ';
		rRdsStoPty = 0;
		rPtyType = 0;
		rRdsStoPtyTemp = 0;
		RDSTxStatName();

		for(j=0;j<16;j++)
		{
			rRdsRTNameSto[4*j] = rRdsRTNameAct[4*j] = ' ';
			rRdsRTNameSto[4*j+1] = rRdsRTNameAct[4*j+1] = ' ';
			rRdsRTNameSto[4*j+2] = rRdsRTNameAct[4*j+2] = ' ';
			rRdsRTNameSto[4*j+3] = rRdsRTNameAct[4*j+3] = ' ';
		}

		RDSTxStatRT();
	#if _DebugPTY
		Printf("\r\n PTY rad key!:rPtyType=%x,rRdsStoPty=%x",rPtyType,rRdsStoPty);
	#endif			  				
	}
	#endif
}

/*******************************************************************************\
* Function   : Radio program initialization subroutine							*
* Description: 																	*
\*-----------------------------------------------------------------------------*/
void	RadioInit( )
{
	/*initialize preset station*/
	MuteOn();
	bSTCkSt = 0;
	bSTOff = 1;
	bMonoOn = 0;
	radioinfoState1 |= BIT5;
	cPSNum = 1;
	Radio_7786.CurrentBand = cBand = 0;
	cRadStatBak.word = cRadStatNum.word = 0;
	cAMASNum = 12;
	cFMASNum = 18;
	
	radioinfoState1 |= BIT7;
	
	if(rAreaSt == _Russia)	
	{
		cFMASNum = 12;
		cFMOrtNum = 6;
	}
	if((rAreaSt == _Japan || rAreaSt == _Europe2))
	{
		cAMASNum = 6;
		cSWASNum = 6;
	}

	InitPresetStat( );
	/*initialize TC9257*/
	BandMsgMem[0] = 0x2000;	// FM1
	BandMsgMem[1] = 0x2000;	// FM2
	BandMsgMem[2] = 0x2000;	// FM3
	BandMsgMem[3] = 0x2000;	// FMORT
	BandMsgMem[4] = 0x2000;	// AM1
	BandMsgMem[5] = 0x2000;	// AM2	
	StartChgBand(0);
	bWtChgBand = 0;
	RADIO_PWR_OFF;
	SysDelay(500);
	RADIO_PWR_ON;
	rRadMainSt = _RadPwrOff;
	//rRadSubSt = TUNER_STARTUP_REQ;
	if(!bRDSDs)	RDSInit();
	
}	

/*******************************************************************************\
*Function: calculate (station number)*ByteIn			 						*
*Input   : cRadStat																*
*Output  : 																		*
*Description: 乘积必须小于2bytes												*
\*-----------------------------------------------------------------------------*/			
uint	StatNumMulN(uchar d)
{	
	return( cRadStatNum.word * d);
}
#if 0
/*******************************************************************************\
* Function  : Get current station display frequency								*
* Input     :																	*						
* Output	: 5 digit BCD code, A,B,C,D,E(LSB)									*
*Description: 																	*	
\*-----------------------------------------------------------------------------*/			
void	GetDpFreq(void)
{
	uchar	const	*p;
	TYPE_WORD	HEXDispFreq;
	
	p = TunParmStartAddr( );/*start address in TabPoint(pTemp)*/
	p += 2;/*指向步长频率*/
	/* 读起始显示频率(WA),十六进制格式 */
	HEXDispFreq.ByteH = *(p + 1);
	HEXDispFreq.ByteL = *(p + 2);

	/*cRadStat*StepFreq --> */
	HEXDispFreq.word += StatNumMulN(*p);/*乘积在(BC)中*/

	/*转化为BCD码格式*/
	TwoHexTo5BCD(HEXDispFreq.word );/*Output: A,B,C,D,E*/
}
/* add for rds */
uchar   const	PtyDpCharTab[32][9] =
{
	"PTY NONE",			/*00 _PTY_NONE	*/
	"  NEWS  ",			/*01 _News		*/
	" AFFAIRS",  		/*02 _Affairs	*/
	"  INFO  ",			/*03 _Info		*/
	"  SPORT ",  		/*04 _Sport		*/
	" EDUCATE",			/*05 _Educate	*/
	"  DRAMA ",			/*06 _Drama		*/
	" CULTURE",			/*07 _Culture	*/
	" SCIENCE",			/*08 _Science	*/
	" VARIED ",			/*09 _Varied 	*/
	"  POP M ",			/*10 _PopM   	*/
	" ROCK M ",			/*11 _RockM  	*/
	" EASY M ",			/*12 _EasyM  	*/
	"LIGHT M ",			/*13 _LightM 	*/
	"CLASSICS",			/*14 _Classics  */ 
	"OTHER M ",			/*15 _OterM  	*/
	" WEATHER",			/*16 _Weather	*/
	"FINANCE ",			/*17 _Finance	*/
	"CHILDREN",			/*18 _Children  */ 
	" SOCIAL ",			/*19 _Social 	*/
	"RELIGION",			/*20 _Religion  */ 
	"PHONE IN",			/*21 _PhoneIn	*/
	" TRAVEL ",			/*22 _Travel 	*/
	" LEISURE",			/*23 _Leisure	*/
	"  JAZZ  ",			/*24 _Jazz   	*/
	" COUNTRY",			/*25 _Country	*/
	"NATION M",			/*26 _NationM	*/
	" OLDIES ",			/*27 _Oldies 	*/
	" FOLK M ",			/*28 _Folk  	*/
	"DOCUMENT",			/*29 _Document  */ 
	"  TEST  ",			/*30 _Test   	*/
	"  ALARM ",			/*31 _Alarm  	*/           
};

uchar   const	Dp_TRAFTab[ ]={"TRAFFIC"};
uchar   const	Dp_PISEEKTab[ ]={"PI "};
uchar   const	Dp_REGONTab[ ]={" REG ON"};
uchar   const	Dp_REGOFFTab[ ]={"REG OFF"};
uchar   const	Dp_EONLOTab[ ]={"EON LO"};
uchar   const	Dp_EONDXTab[ ]={"EON DX"};
uchar   const	Dp_EONTATab[ ]={" EON TA "};
uchar   const	Dp_NoTPTATab[ ]={"NO TP,TA"};
uchar   const	DpSeekTATab[ ] = {"TA "};
uchar   const	DpSeekTPTab[ ] = {"TP "};
uchar   const	PtyNotFdTab[ ] = {"NO  FND"};
uchar   const	DpPtySeekTab[ ] = {"PTY"};

/*******************************************************************************\
*Function: band infomation											 			*
*Input   : i(LCD char digital number)											*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	BandInfoDisp( )
{}

uchar	const	DpStationTab[ ]={"STATION"};
uchar	const	DpStoreTab[ ]={"STOREO"};
uchar	const	DpMonoTab[ ]={" MONO"};

uchar	const	DpAUTOTab[ ]={" AUTO"};
uchar	const	DpMANUTab[ ]={"MANUAL"};
//uchar	const	DpGAINTab[ ]={"GAIN"};
#endif

void	RadTmpDisp(uchar rTmpDpSt,uchar rTmpTime)
{
	//if(rSndSrc!=_SndSrcRad && rRdsMainSt != _Traffic)	return;	/*add for rds*/
	//rRadTmpDpSt = rTmpDpSt;
	//TmpDp_X64ms = rTmpTime;
	//TmpDpPoint = 0;
	//rMainDpPoint = _mDpSource;
	return;
}

void	RadioDpFreq(void)
{}



/*******************************************************************************\
* Function: set radio program to power off mode			 						*
* Input   : 																	*
* Output  : 																	*
*Description: radio power down or source change(exit radio), 					*
*this routine be called 														*
\*-----------------------------------------------------------------------------*/			
void	SetRadPwrDn( )
{
	#ifdef _EnableRadSi4730
	if(bRadSeeking)
	{
		RestoreStatNum();
		bRadSeeking = False;
		cPSNum = 0;
	}
	if(rRdsMainSt==_PiSeek || rRdsMainSt==_TpSeek || rRdsMainSt==_PtySeek)
	{/* Stop searching and restore previous station */

		rRdsMainSt = _PtyOff;
		RestoreStatNum( );	
	}
	if(rRdsAfSt==_AfWtPllLk/* || rRdsAfSt==_AfWtChkSt*/)	/* add for rds */
	{
   		RestoreStatNum( );
   		rRdsAfSt = 0;
   	}
	//bRadSignalFail = 0;
	bManuAuto = 0;
	bSTOff = True;
	bSTCkSt = 0;/*stable status*/   	
	#else
	if(bRadSeeking)
	{
		RestoreStatNum();
		bRadSeeking = False;
		cPSNum = 0;
	}
	bManuAuto = 0;
	/*add for rds*/
	if(rRdsMainSt==_PiSeek || rRdsMainSt==_TpSeek || rRdsMainSt==_PtySeek)
	{/* Stop searching and restore previous station */
		//if(bRdsPtyEn && !bAM_FM)	rRdsMainSt = _PtyOn;
		//else	
		rRdsMainSt = _PtyOff;
		RestoreStatNum( );	
    	/*StartWrPll( );
		RadSetToDl500ms(13);*/
	}
	if(rRdsAfSt==_AfWtPllLk || rRdsAfSt==_AfWtChkSt)	/* add for rds */
	{
   		RestoreStatNum( );
   		rRdsAfSt = 0;
   	}
	bSTOff = True;
	bSTCkSt = 0;/*stable status*/   	
	#endif
}

void	SetRadPwrOff( )
{
	if(bRadSeeking)
	{
		bRadSeeking = False;
		cPSNum = 0;
	}
	bManuAuto = 0;
	
	rRadSubSt = 1;
	rRadMainSt = _RadPwrOff;
	RADIO_PWR_OFF;
}

/*******************************************************************************\
* Function 	: Radio listen mode check											*
* Input		: rRadMainSt														*
* Output    : A(Yes or No)														*
*Description:1. Radio standby status											*
*			 2. PS display Frequency 5S											*
*			 3. Scan display Frequency 5S										*
\*-----------------------------------------------------------------------------*/
uchar	RadListenCk(void)
{
	//if(bRadSignalFail)		return(0);

	if(rRadMainSt == _RadStdby)	return( 1 );
	if(rRadMainSt == _RadPS)
	{
		 if(rRadSubSt == _psListen5S)	return( 1 );
	}
	else	if(rRadMainSt == _RadScan)
	{
		 if(rRadSubSt == _rsListen5S)	return( 1 );
	}
 	return( 0 );
}

/***************************************************************************************\
*	   Function: 读立体声指示标志														*
*	   Input   : Rad_X4ms																*
*          Output  : bSTOff																*
\*-------------------------------------------------------------------------------------*/			
void	STChk( )
{
	if(Rad_SMeter_X64ms || cRdsPiChkCnt)	return;
	Rad_SMeter_X64ms = 64/64;
	{
		Elite_GetAverageQuality(1);
		rRadMeterLevel = Radio_7786.Smeter;
		rSi475xDataBuff1[cDTSParmIndex++] = rRadMeterLevel;
		if(cDTSParmIndex >= 7)	cDTSParmIndex = 0;
	}
	if(Rad_SMeter_X256ms /*|| rSndSrc != _SndSrcRad*/)	return;
	Rad_SMeter_X256ms = 2000/256;
#if 1
	if(Radio_7786.F_Stereo && !bAM_FM)//means the pilot is higher than the threshold ST_PILOT_DET_TH
#else
	if(Radio_7786.HITDataBuff[3]==0x7f&&Radio_7786.HITDataBuff[4]==0xff&&Radio_7786.HITDataBuff[5]==0xfc)//means the pilot is higher than the threshold ST_PILOT_DET_TH
#endif

	{
		//Radio_7786.F_Stereo=1;
		
		radioinfoState1 |= BIT4;
		ReportRadioBaseInfo();
	}
	else
	{
		//Radio_7786.F_Stereo=0;
		
		radioinfoState1 &= (~BIT4);
		ReportRadioBaseInfo();
	}
	
}

/*******************************************************************************\
*Function: Preset station memory scan 				 							*
*Input   : rRadSubSt															*
*Output  : 																		*
*description : if station found, listen 5s, else scan next preset station		*
\*-----------------------------------------------------------------------------*/			
void	PSProc( ) 
{
	uchar j,s;
	
	switch(rRadSubSt)
	{
	   case	 0:	return;

	   case	 _psWrPll:
	   	/*increment preset number*/	
		#ifdef _EnableRadSi4730
			j = cBand;
			s = cPSNum;
				if(j*6+s >= cFMASNum && rAreaSt != _Russia && !bAM_FM ||s >= cFMOrtNum && (rAreaSt == _Russia) && (j == _BandFM3) && !bAM_FM  )
				{
					if(rAreaSt == _Russia)
					{
						 if(cBand == _BandFM3) 
							j = _BandFM3;
						 else
						 {
							j = _BandFM1;
						 }
					}
					else
						j = _BandFM1;
					s = 1;
					cBand = j;
					cPSNum = s;
#if _DebugRad
			Printf("\r\n PS fm end:Band=%d,PsNum=%d,cFMASNum=%d,cAMASNum=%d",j,s,cFMASNum,cAMASNum);
#endif
					//RestoreStatNum( );
					rRadSubSt = 0;
					ReadPresetMem(cBand,cPSNum);/*read station number*/
					StartWrPll( );	/*start write PLL procedure*/
					RadSetToDl500ms(16);
					return;
				}
				else if((j-_BandAM1)*6+s >= cAMASNum && bAM_FM ||s >= cSWASNum && (rAreaSt == _Japan || rAreaSt == _Europe2) && (j == _BandAM2) && bAM_FM )
				{
					//RestoreStatNum( );
					s = 1;
					cBand = j;
					if(rAreaSt == _Japan || rAreaSt == _Europe2);
					else
					cBand = _BandAM1;
					cPSNum = s;
#if _DebugRad
			Printf("\r\n PS am end:Band=%d,PsNum=%d,cSWASNum=%d,cAMASNum=%d",j,s,cSWASNum,cAMASNum);
#endif
					rRadSubSt = 0;
					ReadPresetMem(cBand,cPSNum);/*read station number*/
					StartWrPll( );	/*start write PLL procedure*/
					RadSetToDl500ms(16);
					return;
				}
				
				if(s++ >= 6)	/*switch band*/
				{
					s = 1;
					if(rAreaSt == _Russia && !bAM_FM)
					{
						 if(cBand == _BandFM3) 
							j = _BandFM3;
						 else
						 {
						 	++j;
							if(j > _BandFM2)	j = _BandFM1;
						 }
					}
					else if(bAM_FM && (rAreaSt == _Japan || rAreaSt == _Europe2))
					{

					}
					else
					{
						++j;
						if(bAM_FM)	
						{
							if(j > _BandAM2)	j = _BandAM1;
						}
						else
						{
							if(j > _BandFM3)	j = _BandFM1;
						}
					}
					/*confirm if current Mn is a active stat,then switch band*/
					if(bAM_FM && (rAreaSt == _Japan || rAreaSt == _Europe2))
					{

					}
					else if(!bAM_FM && (rAreaSt == _Russia) && (cBand == _BandFM3))
					{
					
					}
					else if(!bAM_FM && (j*6+s > cFMASNum))
					{
						s = 1;
						j = _BandFM1;
					}
					else if(bAM_FM && ((j-_BandAM1)*6+s > cAMASNum))
					{
						s = 1;
						j = _BandAM1;
					}
					
				}
#if _DebugRad
			Printf("\r\n PS stat:Band=%d,PsNum=%d,cFMASNum=%d,cAMASNum=%d",j,s,cFMASNum,cAMASNum);
#endif
			cBand = j;
			cPSNum = s;
			ReadPresetMem(j,s);/*read station number*/
		#else
		   	if((++cPSNum)>6)
			{
#if _DebugRad
			Printf("\r\n Restore last stat");
#endif
				RestoreStatNum( );
				RadSetToDl500ms(16);
				return;
		   	}
		   	ReadPresetMem(cBand,cPSNum);/*read station number*/
#if _DebugRad
			Printf("\r\n ps stat = :%2X cPSNum = %x",cRadStatNum.word,cPSNum);
#endif
		#endif
		bRadSeeking = False;
		StartWrPll( );	/*start write PLL procedure*/
		
		HIT_ForeceMute(ON);
		++rRadSubSt;
	   	return;

	   case	 _psWtPllStable:/*after write PLL, delay 300ms then check PLL stable*/	
	   	if( bPllStable )
	   	{
			Rad_X64ms = 200/64;
	   		++rRadSubSt;
	   	}
	   	return;
	   
	   case	_psPgmPllDl:
	   	if(!Rad_X64ms)
	   	{
	   		//StartChkStat( );
	   		++rRadSubSt;
		}
	   	return;
	   			
	   case	 _psWtChkSt:/* wait check station end */	
	   	//if(!bChkStatEn)
	   	{
	   		//if(bStatPresent)
	   		{/*valid station found, change to listen 5s*/
	   			//BkupStatNum( );
	   			
				HIT_ForeceMute(OFF);
	   			Rad_X64ms = 5000/64;
	   			++rRadSubSt;
			#ifdef _EnableRadSi4730
			#else
				StartWrTEF6601All();
			#endif
	   		}
	   		//else
	   		//{/* no station, continue to scan next*/	
	   		//	rRadSubSt = _psWrPll;
	   		//}
	   	}
#if _DebugRad
			Printf("\r\n ps 10s start = %2X,rAreaSt =  ",Rad_X64ms,rAreaSt);
#endif

	   	return;
	   			
	   case	 _psListen5S:/*PS listen 5s status*/	
	   	if( !Rad_X64ms )	
	   	{
	   	#ifdef _EnableRadSi4730
		
		#else
#if _DebugRad
			Printf("\r\n ps end:cBand = %2X,cPSNum = %X ",cBand,cPSNum);
#endif

			if(!bAM_FM)
			{
				if(rAreaSt == _Russia && cBand == _BandFM3)
				{
					cBand = _BandFM3;
				}
				else
				{
					if(cBand == _BandFM1&& cPSNum >= 6)
					{
						cBand = _BandFM2;
						cPSNum = 0;
					}
					else if(cBand == _BandFM2&& cPSNum >= 6)	
					{
						cBand = _BandFM3;
						cPSNum = 0;
					}
					else if(cBand == _BandFM3&& cPSNum >= 6)
					{
						//cBand = _BandFM1;
						cBand = cBandBak;
#if _DebugRad
			Printf("\r\n ps ok:%x ",cPSNum);
#endif

					}
				}
			}
			else
			{
				if(cBand == _BandAM1&& cPSNum >= 6)
				{
					if(rAreaSt == _Japan || rAreaSt == _Europe2)
						cBand = cBandBak;
					else
					{
						cBand = _BandAM2;
						cPSNum = 0;
					}
				}
				else if(cBand == _BandAM2&& cPSNum >= 6)	
				{
					//cBand = _BandAM1;
					cBand = cBandBak;
				}
			}
		#endif
			rRadSubSt = _psWrPll;	/*5s timer expired, terminate listen status*/
	   	}
	   	else
	   	{
	   		STChk( );
			//if(rRdsMainSt==_Traffic)
			//{
			//	/*RadSetToDl500ms( );*/
			//	bRadSeeking = False;
			//	rRadMainSt = _RadStdby;
			//}
	   		//if(bSync500ms)	bUpdateDisp = True;
	   	}
	   	return;
	}
} 

/*******************************************************************************\
* Function	: frequency increment auto tunning 				 					*
* Input   	: rRadSubSt															*
* Output  	: 																	*
*Description: if station found, stop auto tunning 								*
\*-----------------------------------------------------------------------------*/			
void	ATProc( ) 
{
	uchar	r,step;

	switch(rRadSubSt)
	{
	   case  _atWrPll:/* 电台改变一步并写锁相环 */
	   //	if(!bARMAudioPin && rArmAudioSt == _ArmAudioPlay || bTelCalling)
			//;
		//else if(!bSysMuteSt)	return;
		
#if 1
		if(Rad_X4ms)	return;
	   	if(rRadMainSt == _AutoTUp)
   		{
			r = StatNumInc(_SeekModeAuto);/* seeking up */
			Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
			TunerSeekOn(Radio_7786.Step);
   		}
	   	else	
   		{
			r = StatNumDec(_SeekModeAuto);/* seeking down */
			Radio_7786.Freq = GetRadFreOctData(cRadStatNum.word);
			TunerSeekOn(-Radio_7786.Step);
   		}
		Rad_X4ms = 10/4;
		rRadSubSt++;
#else
	 #ifdef _EnableRadSi4730	
		cPSNum = 0;
	   	if(rRadMainSt == _AutoTUp)	r = StatNumInc(_SeekModeAuto);/* seeking up */
	   	else	r = StatNumDec(_SeekModeAuto);/* seeking down */
		
		if( r )	bRadHopFreq = 1;// set hopping frequency flag
		++rRadSubSt;

	#else

		cPSNum = 0;
	   	if(rRadMainSt == _AutoTUp)	r = StatNumInc(_SeekModeAuto);/* seeking up */
	   	else	r = StatNumDec(_SeekModeAuto);/* seeking down */
		
		if( r )	bRadHopFreq = 1;// set hopping frequency flag
		++rRadSubSt;

		/*add for rds PTY and PI seek function*/
		if((cBand < _BandFM3) && OriginStNum.word==cRadStatNum.word && ((bRdsPtyEn && (armsourceState == ARM_SOURCE_RADIO)) || bPiSeeking || rRdsMainSt==_TpSeek))
		{/* finish one cycle of PTY or PI seeking*/
			if(bRdsPtyEn)
			{/*if pty on, end PTY searching*/
				//if(bPtyChged || rRdsMainSt==_TpSeek)
				//{/*if pty has been changed, disable it,otherwise keep the settings*/
				//	bPtyChged = False;
					bRdsPtyEn = False;
				//	RadDpUpdate(_DpRadRdsPs);
				//}
				//else
				//{
				////	/*rRdsMainSt = _PtyOn;*/
				//	RadDpUpdate(_DpRadPtyT);
				//}
				rRdsMainSt =_PtyOff;
				//RadTmpDisp(_DpRadNoPty,(uchar)(3000/64));/* display "PTY NONE" for 3s*/
			}
			else	if(rRdsMainSt==_TpSeek)
			{
				if(!bRadSeek2nd)
				{
					bRadSeek2nd = True;
					StartWrPll( );	/*start write PLL procedure*/
					return;
				}
				else
				{
					bRadSeek2nd = False;
					rRdsMainSt=_PtyOff;
					RdsTpSeek_X500ms = 30000/500;
				}
			}
			bPiSeeking = False;	/* Stop PI seeking and restore previous station */
			RestoreStatNum( );
			/*StartWrPll( );*/
			RadSetToDl500ms(17);
		}
	#endif
#endif

		StartWrPll( );	/*start write PLL procedure*/
	   	return;
	   			
	   case  _atWtPllLk:/* 自动寻台等待锁相环稳定状态 */	
	 	if(bPllStable)
	   	{/*now PLL is stable*/
			StartChkStat( );/*start check station */
	   		++rRadSubSt;
	   	}
	   	return;
	   			
	   case  _atWtChkSt:/* 自动寻台等待电台判别结束状态 */	   	
	   	if(!bChkStatEn)
	   	{/* station check completed */
	   	
			//ReportRadioSignalinfo(0);
	   		if(bStatPresent)
	   		{/* station found */
	   			rRadSubSt = 0;
			#if	0//_UartDebug
				Printf("\r\n AT sucess");
			#endif
	   		}
	   		else
	   		{/*no station, scan next station*/

				rRadSubSt = _atWrPll;
			#if	_UartDebug
				Printf("\r\n next stat  :%x,%x",OriginStNum.word,cRadStatNum.word);
			#endif

				if(OriginStNum.word == cRadStatNum.word)
				{/* Yes, SCAN 结束处理 */
					if(!bStatPresent)	RestoreStatNum( );
					else
					{
						StartWrPll( );
					}
					RadSetToDl500ms(18);
					bRadFreqChgd = 1;
				}
	   		}
	   	}
	   	return;
	}
} 

/*******************************************************************************\
* Function	: frequency increment scan station 				 					*
*  Input   	: 																	*
* Output   	: 																	*
*Description: if station found, listen 5s, else scan next preset frequency		*
\*-----------------------------------------------------------------------------*/			
void	ScanProc( )
{
	switch( rRadSubSt )
	{
	   case	 _rsWrPll:
	   	#ifdef _EnableRadSi4730
		RDSTimerInit( );
		#else
		RDSTimerInit( );
		#endif
	   	/* 电台加一并写锁相环 */
		if( StatNumInc(_SeekModeAuto) )	bRadHopFreq = 1;// set hopping frequency flag
		StartWrPll( );		/*start write PLL procedure*/
		++rRadSubSt;
	   	return;
	   			
	   case	 _rsWtPllStab:	
	   	if(bPllStable)
	   	{/*PLL is stable*/
	   		StartChkStat( );
	   		++rRadSubSt;/*convert to wait check station end*/
	   	}
	   	return;
	   			
	   case	 _rsWtChkSt:	/*wait check station end*/
	   	if(!bChkStatEn)
	   	{/*check station end process*/
	   		if(bStatPresent)
	   		{/* 收到电台处理 */
	   			Rad_X64ms = 5000/64;//delay 5s*/
	   			//if(bRadFreqChgd)	SaveCurBandMsg( );
	   			BkupStatNum( );
				#ifdef _EnableRadSi4730
				#else
				StartWrTEF6601All();
				#endif
	   			++rRadSubSt;/*convert to scan listen 5s status*/
	   		}
	   		else	
	   		{/* 没有收到台处理 */
	   		/* scan a completed cycle check */
	   			rRadSubSt = _rsWrPll;/*Scan next station*/
	   		}
	   	}
		if(OriginStNum.word == cRadStatNum.word)
		{/* Yes, SCAN 结束处理 */
			//if(!bStatPresent)	
			RestoreStatNum( );
			//if( cPSNum )	ReadPresetMem(cPSNum);
			RadSetToDl500ms(19);
			bRadFreqChgd = 1;
			#if	_UartDebug
				Printf("\r\n scan end  :%4x,%4x",OriginStNum.word,cRadStatNum.word);
			#endif
		}
	   	return;
	   			
	   case	 _rsListen5S:	/* scan mode, listen scaned station for 5s status*/
	   	if( !Rad_X64ms )
		{
			rRadSubSt = _rsWrPll;/*Scan next station*/
	   	}
		else 
		{
			/*ST station check*/
			STChk( );
			//if(rRdsMainSt==_Traffic)
			//{
			//	/*RadSetToDl500ms(20);*/
			//	bRadSeeking = False;
			//	rRadMainSt = _RadStdby;
			//}
			
			/*wink display sync check*/
	   		//if(bSync500ms)	bUpdateDisp = True;
	   	}
	   	return;
	}  
} 

/*******************************************************************************\
*	   Function: Program PLL Procedure					 						*
*	   Input   : 																*
*          Output  : 															*
\*-----------------------------------------------------------------------------*/			
void	Wr7786PllProc(void)
{
	if(!bPrgmPll)	return;
     
	switch(rPrgmPllSt)
	{
		case	0:/*write PLL*/
			if(Rad_X4ms) break;
#ifdef RDS_ENABLE
			if(!bRDSDs)
			{
				rRdsAfDetSt = 0;
				rRdsStoPtyTemp = 0;
				RDSFmFreqChange( );
				RDSTxStatName();
				RDSTxStatRT();
				rRdRDS_X4ms = 100/4;
				RdsMeasure_X32ms = 2000/32;
			}
			TunerSetAFCheck(Radio_7786.Freq);
#endif
			Rad_X4ms= 20/4;
			ReportRadioSignalinfo(0);
			Rad_SMeter_X64ms = 200/64;
			++rPrgmPllSt;
#if	_DebugTestDly
		    Printf("\r\n test2 = %d,Rad_X64ms= %d",Testtime_x10ms,rRadMainSt);
#endif
			break;

		case	1:/*Waiting for PLL Stable status*/
			if(!Rad_X4ms)
			{/*program PLL finished process*/
				if(!bRadSeeking || bRadWrPllEn)
				{
					bRadWrPllEn = 0;
					Radio7786SetFreq(Radio_7786.Freq);
					Rad_X4ms=300/4;
				}
				else
				{
					if(!bAM_FM)
						Rad_X4ms=50/4;
					else
						Rad_X4ms=100/4;
				}

				++rPrgmPllSt;
				
#if	_DebugTestDly
				Printf("\r\n test3 = %d,Rad_X4ms= %d",Testtime_x10ms,Rad_X4ms);
#endif
			}
			break;

		case	2:/*Waiting for PLL Stable status*/			
			if(!Rad_X4ms || !Radio_7786.F_SeqChg)
			{/*program PLL finished process*/
			
			#if	_DebugTestDly
					Printf("\r\n test4 = %d,Rad_X4ms= %d",Testtime_x10ms,Rad_X4ms);
			#endif
				bRadHopFreq = False;
				bPllStable = True;
				bPrgmPll = False;
				rPrgmPllSt = 0; 
				
				//HIT_CmdSetDiss(Diss_AM_Neutral, 0);
				if(!Rad_X4ms)
				{
					Printf("\r\n WrPll err:%d",Radio_7786.Freq);
					//rPrgmPllSt = 1;
					if(Radio_7786.F_TDBusy && Radio_7786.F_SeqChg && Radio_7786.F_TDCOLL)
					{
						RadioInit();
#if _DebugTestDly
						Printf("\r\n read seq:%d",Radio_7786.Freq);
#endif			
							
					}

				}

			}
			break;

	   	default:
			rPrgmPllSt = 0;
			break;
	}
	return;
}

void Wr7786CkStatProc(void)
{
//	TYPE_WORD rFreq, rStart;
	uchar rRadLevelAverage;
//	uchar const *p;
	


	switch(rSi475xCkStationSt)
	{
		case   _Si475xCkStIFC1:
			Radio_7786.F_GoodStation=0;
			//TunerReadSeekStatus();
			Elite_CmdReadSeekStatus();
			if(Radio_7786.DisplayFreq != Radio_7786.Freq )
			{
				Printf("\r\n write fre err:%x,%d,%d",cRadStatNum.word,Radio_7786.Freq,Radio_7786.DisplayFreq);
			}
			
			rRadMeterLevel = rRadLevelAverage = Radio_7786.Smeter;//AutoAlignDigitalFilter(rSi475xDataBuff1,2);  //RadLevel
			//Printf("\r\n FM chk:rRadLevelAverage=%x,rTempi=%x,fre=%d",rRadLevelAverage,_Si47xxAMRSSILevel,Radio_7786.DisplayFreq);
#if 1
			++rSi475xCkStationSt;

#else
			if(!bAM_FM)
			{
				if(bLOCEn)	rTempi = _Si47xxFMRSSILevelLoc;
				else	rTempi = _Si47xxFMRSSILevel;

				if(rRadLevelAverage >= rTempi) 
				{
					++rSi475xCkStationSt;
				#if _DebugSrchStat
					Printf("\r\n FM chk:rRadLevelAverage=%x,rTempi=%x,fre=%d",rRadLevelAverage,rTempi,Radio_7786.DisplayFreq);
				#endif

				}
				else		
				{
					rSi475xCkStationSt = 0;
				#if _DebugSrchStat
					Printf("\r\n FM chk1:rRadMeterLevel=%x,rTempi =%x,fre=%x",rRadLevelAverage,rTempi,Radio_7786.Freq);
				#endif

				}
			}
		   	 else
			{
				if(rRadLevelAverage >= _Si47xxAMRSSILevel)
				{
					++rSi475xCkStationSt;
				#if _DebugSrchStat
					Printf("\r\n AM Stat2:%2x",rRadLevelAverage);
				#endif
				}
				else	
				{
					rSi475xCkStationSt = 0;
				#if _DebugSrchStat
					Printf("\r\n AM No Stat2:%2x",rRadLevelAverage);
				#endif
				}
			}
#endif
			return;
			
		case   _Si475xCkStIFC2:
		//case   _Si475xCkStIFC3:
			rTempt = Radio_7786.Multipath;//AutoAlignDigitalFilter(rSi475xDataBuff2,5); //WAM
			rTempj = Radio_7786.Detuning;//AutoAlignDigitalFilter(rSi475xDataBuff3,5);	//USN
			
#if _DebugSrchStat
			Printf("\r\n FM3 chk:mul=%x,dtu=%x,ADJ=%x",rTempt,rTempj,Radio_7786.AdjChannel);
#endif

#if _manual_seek

#if 0

				if(!bAM_FM)
				{
					if(rTempt <= _Si47xxFMWAMRLevel && rTempj >= _Si47xxFMUSNRLevel) 
					{
						++rSi475xCkStationSt;
					#if _DebugSrchStat
						Printf("\r\n FM Stat3:%x,%x,%x,%x",rTempj,rTempt,rRAD_SNR,rRadFreOffset);
					#endif
					}
					else		
					{
						rSi475xCkStationSt = 0;
					#if _DebugSrchStat
						Printf("\r\n FM No Stat3:%2x,%2x,%2x,%2x",rTempj,rTempt,_Si47xxFMUSNRLevel,_Si47xxFMWAMRLevel);
					#endif
					}
				}
				else
				{
				#if 1
					++rSi475xCkStationSt;
				#else
					if(rTempj > _Si47xxAMUSNRLevel && rTempt < _Si47xxAMWAMRLevel) 
					{
						++rSi475xCkStationSt;
					#if _DebugSrchStat
						Printf("\r\n AM Stat3:%2x,%2x",rTempj,rTempt);
					#endif
					}
					else		
					{
						rSi475xCkStationSt = 0;
					#if _DebugSrchStat
						Printf("\r\n AM No Stat3:%2x,%2x",rTempj,rTempt);
					#endif
					}
				#endif
				}
#endif

#else
			GetRadPlayFre(Radio_7786.DisplayFreq);
			if(bRDSDs)
			{
				if(Radio_7786.F_GoodStation)
				{// FM:25k AM:8K
					++rSi475xCkStationSt;
					if(rRadMainSt == _RadAS && rRdsMainSt == _PtyOff)
						TunerSeekOff(SeekContinue);
					else 
						TunerSeekOff(SeekStopUnmuted);
				}
				else
				{
#if _DebugSrchStat
					Printf("\r\n No Stat21:%2x",rRAD_SNR);
#endif
				
					//bRadWritePll = 1;
	#if _manual_seek
	#else
					TunerSeekOff(SeekContinue);
	#endif
					rSi475xCkStationSt = 0;
				}

			}
			else 
			{
				if(Radio_7786.F_GoodStation)
				{
					if(rRadMainSt == _RadAS)
					{
						if(rRdsMainSt == _PtyOff)	TunerSeekOff(SeekContinue);
					}
					else 
						TunerSeekOff(SeekStopUnmuted);
					++rSi475xCkStationSt;
				}
				else
				{
#if _manual_seek
#else
					TunerSeekOff(SeekContinue);
#endif
					rSi475xCkStationSt = 0;

				}
			}

			
#endif
			return;
			
		case   _Si475xStationOk:
			#if	_DebugSrchStat
				Printf("\r\n Seek End:");
			#endif
			return;

		default:
			rSi475xCkStationSt = 0;
			break;
	}	          	            
}

/*******************************************************************************\
*Function: terminate station check procedure and set no radio station flag		*
*Input   : 																		*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	ChkNoStat( )
{
	ChkStatSt = _ChkStatInit;
	bStatPresent = False;
	bChkStatEn = False;
	//if(!bPiSeeking /*&& (rRdsAfSt != _AfWtChkSt)*/)		RDSFmFreqChange( );		/* add for rds */
	//if(rRdsAfSt != _AfWtChkSt)	RDSTimerInit( );
}
/***************************************************************************************\
*	   Function: terminate station check procedure and set radio station exist flag	*
*	   Input   : 									*
*          Output  : 									*
\*-------------------------------------------------------------------------------------*/			
void	ChkStatExist( )	/*add for rds*/
{
	ChkStatSt = _ChkStatInit;
	bPiSeeking = False;
#if	_DebugSrchStat
	Printf("\r\n Find Station.\n");
#endif

	bStatPresent = True;
	bChkStatEn = False;
}


void	Ck7786StatProc( )
{
	if(!bChkStatEn)	return;
	
	switch(ChkStatSt)
	{
		case   _ChkStatInit:
			bStatPresent = False;
			rSi475xCkStationSt = _Si475xCkStIFC1;
			++ChkStatSt;
			cDTSParmIndex = 0;
			bIFCnting = True;
			//Rad_ChkStat_X4ms = 50/4;
			Rad_X4ms = 20/4;
			return;
			
		case   _CkStStartDl:
			if(Rad_X4ms)	return;
			Wr7786CkStatProc();
		case _CkStStartD5:
			if(rSi475xCkStationSt == _Si475xStationOk)
			{
				if(rRadMainSt == _RadAS && rRdsMainSt != _PtySeek)
				{
					ChkStatExist( );
				}
				else if(!bRDSDs && (rRdsFlag.byte & 0x07) && !bAM_FM)	/* add for rds*/
				{
					if(0)	//cRadStatNum.bit.b0 && (rAreaSt == _Europe || rAreaSt == _Russia)) // if Step is 50Khz				
					{
						ChkNoStat( );
				#if	_DebugSrchStat
						Printf("\r\n No Rds Stat21:%2x",rRdsFlag.byte);
				#endif
						return;
					}
					
					rRdsSyncSt = 0;
					ChkStatSt = _CkWaitRds;
					Rad_ChkStat_X4ms = 2000/4;
				#if	_DebugSrchStat
						Printf("\r\n Wait Rds:%2x",rRdsSyncSt);
				#endif

				}
				else
				{
					ChkStatExist( );
				}			
			}
			else  if(rSi475xCkStationSt == 0)
			{
				ChkNoStat( );
			}
			break;
		
		case	_CkWaitRds:/* wait RDS */
			if(/*rRdsSyncSt>_LeadIn*/RDSGetDataBlock())
			{
			#if	_DebugSrchStat
				Printf("\r\n Chk Rds sync ok.\n");
			#endif
				//if(/*rRdsAfSt==_AfWtChkSt || */bPiSeeking)	Rad_ChkStat_X4ms = 200/4;		
				//else if(!bRdsTAEn)	Rad_ChkStat_X4ms = 500/4;	/* need more 100ms for receiving RDS signal */
				//else	
				Rad_ChkStat_X4ms = 1000/4;
				ChkStatSt = _CkRdsChk;
				rRdsSyncSt = 0;
			}
			else if(!Rad_ChkStat_X4ms)
			{
				TunerSeekOff(SeekContinue);
				ChkNoStat( );
			#if	_DebugSrchStat
				Printf("\r\n no Rds stat2:%2x,%2x",Rad_ChkStat_X4ms,rRdsSyncSt);
			#endif
			}
		break;
	
	case	_CkRdsChk:
			if(Rad_ChkStat_X4ms==0)
			{
				TunerSeekOff(SeekContinue);
				ChkNoStat( );
			#if	_DebugSrchStat
				Printf("\r\n no Rds stat3:%2x,%2x",Rad_ChkStat_X4ms,rRdsSyncSt);
			#endif
		    	}
			/* received rds signal */
			else 	if(/*rRdsPiCode.word>0 && rRdsSyncSt>_LeadIn*/RDSFlagPINew())
			{/* already received PI code*/
			


				if(bRdsPtyEn && (armsourceState == ARM_SOURCE_RADIO))
				{/* check PTY value */
					if(rRdsStoPtyTemp == rPtyType || FM_RDS_PTY == rPtyType)
					{/*Check if TA function is enabled*/
						ChkStatExist( );
						TunerSeekOff(SeekStopUnmuted);
					}
					else
					{
						//TunerSeekOff(SeekContinue);
					}
					return;
				}
				else	if(bRdsTAEn)
				{/* check TP signal */
					if(!bRdsTPPreVal && !bRdsTAPreVal)	return;	/*no tp or ta*/
					//if(!bRdsTAPreVal && rRdsMainSt==_TpSeek && !bRadSeek2nd)	return; /* no ta at first loop */
				}
				else	if(bPiSeeking/* || rRdsAfSt==_AfWtChkSt*/)
		   		{/* find PI */
				#if	_DebugSrchStat
					UartOutputString("Chk Rds PI:\n");
			 		UARTOutputChar( ConvNumToASCII(rRdsPiCodeH >> 4) );
			 		UARTOutputChar( ConvNumToASCII(rRdsPiCodeH & 0x0F) );
			 		UARTOutputChar( ConvNumToASCII(rRdsPiCodeL >> 4) );
			 		UARTOutputChar( ConvNumToASCII(rRdsPiCodeL & 0x0F) );
				#endif
		   			if(bRdsRegOn)
		   			{/* PI code must be completely matched */
						if(Rds_SwafFixPi.word == rRdsPiCode.word) /* Compare PI code */     
						{
		   					ChkStatExist( );
							  TunerSeekOff(SeekStopUnmuted);
						}
					}
					else
					{/* Region part can be different */
						if((Rds_SwafFixPiH&0xf0)==(rRdsPiCodeH&0xf0) && /* Compare PI code */     
						Rds_SwafFixPiL == rRdsPiCodeL)
						{
		   					ChkStatExist( );
							  TunerSeekOff(SeekStopUnmuted);
							
						}
					}
					//if(bStatPresent && cPSNum!=0)	WritePresetMem(cPSNum);	/* Update preset freq value */
		   			return;
		   		}
				ChkStatExist( );
				TunerSeekOff(SeekStopUnmuted);
			}
			return;					

		default:
			ChkStatSt = 0;
			bChkStatEn = 0;
			break;
	}	          	            
}

/*******************************************************************************\
*Function: Radio main procedure					 								*
*Input   : rRadMainSt															*
*Output  : 																		*
\*-----------------------------------------------------------------------------*/			
void	RadioProc( void)
{
//	ROMuchar *p;
//	TYPE_WORD FreData;
	if(rRadMainSt==_RadIdle || ((armsourceState != ARM_SOURCE_RADIO)  && (armsourceState != ARM_SOURCE_BT) && (bRDSDs || bAM_FM)))	return;
	else	if(rRadMainSt == _RadPwrOff)
	{
	   	if(/*bPwrOn && rPwrSt != _PwrAccOff &&*/bpoweronFlag && core.powerStatus == POWER_ON && (armsourceState != ARM_SOURCE_BT)/* &&  rARMMainSt == _ARMStandby*/)/*Power Up detection*/
	   	{/*radio program power up process*/
#if	_DebugRad
			Printf("\r\n Radio Start");
#endif
			Rad_X64ms = 500/64;
			rRadSubSt = TUNER_STARTUP_REQ;
			++rRadMainSt;/*enter radio initial status*/
			MuteTimeset(100);
	   	}
	   	return;	
	}

	/*Power down detection*/
	if(/*!bPwrOn || rPwrSt == _PwrAccOff ||!bpoweronFlag ||*/ core.powerStatus == POWER_ACC_OFF || (armsourceState != ARM_SOURCE_RADIO) && bRDSDs/*|| !bFirstPwrOn && rARMMainSt != _ARMStandby*/)
	{/*set radio power down*/
		SetRadPwrDn( );
		RadioBreak();
		rRadMainSt = _RadPwrOff;
#if	_DebugRad
			Printf("\r\n Radio pwr off");
#endif
		return;
	}
	
	if(rRadMainSt == _RadInit /*&& !bEjtDiscAccOff*/)			
	{
		//oRAD_ON(Bit_SET);
		if(/*Rad_X64ms || */bFirstPwrOn /*|| rARMMainSt != _ARMStandby*/)	
		return;
		//Si47xxInit( );
		TunerStartupSeq();
		if(rRadSubSt==0)
		{
	#if _DebugRad    	
			Printf("\r\n Radio4730 init1 ok,Rad_X64ms = %x",Rad_X64ms);
	#endif
		    StartWrPll( );
			++rRadMainSt;
			Rad_X64ms = 100/64;
		}
		return;
	}
	else
	{	
	#ifdef _EnableRadSi4730
		Wr7786PllProc();
		Ck7786StatProc();
	#else
		WrPllProc( );
		CkStatProc( );
	#endif
		RadKyAnaly( );
	}
		
	switch(rRadMainSt)	
	{
		case	_RadDlToStdby:
			if(Rad_X64ms==0 && bPllStable)
	   		{
	   			///if(Rad_X64ms || bFirstPwrOn || rARMMainSt != _ARMStandby)	
				//return;

				#if	_DebugRad
					Printf("\r\n radio  enter to _RadStdby");
				#endif
#if	_DebugTestDly
			   Printf("\r\n test5 = %d,Rad_X64ms= %d",Testtime_x10ms,Rad_X64ms);
#endif

				if(!bRDSDs && rRadMainSt != _RadStdby)
				{
				#if _DebugPTY
					Printf("\r\n PTY DlToStdby!:rPtyType=%2x,rRdsStoPty=%2x",rPtyType,rRdsStoPty);
				#endif			  				
					uchar j;
					for(j=0;j<8;j++)
					rRdsPsNameSto[j] = rRdsPsNameAct[j] = ' ';
					rRdsStoPty = 0;
					rRdsStoPtyTemp = 0;
					//if(bRadSeeking)
					TunerSeekOff(SeekStopUnmuted);

					for(j=0;j<16;j++)
					{
						rRdsRTNameSto[4*j] = rRdsRTNameAct[4*j] = ' ';
						rRdsRTNameSto[4*j+1] = rRdsRTNameAct[4*j+1] = ' ';
						rRdsRTNameSto[4*j+2] = rRdsRTNameAct[4*j+2] = ' ';
						rRdsRTNameSto[4*j+3] = rRdsRTNameAct[4*j+3] = ' ';
					}
					RDSTxStatName();
					RDSTxStatRT();
				}
				HIT_ForeceMute(OFF);
				++rRadMainSt;
				Rad_SMeter_X256ms = 0;
				Rad_X64ms = 100/64;
				MuteTimeset(10);
			}

	   		return;
	   			
		case	_RadStdby:	
			STChk( );
			return;

		case	_ManuTHFDl:/* Manual Tuning Hop frequency delay 500ms	*/
	   		if(Rad_X64ms == 0)
	   		{
				RadSetToDl500ms(21);
			}	
			return;
        	
        case	_ChgBand:
			if(Rad_X64ms == 0)
			{
				bWtChgBand = 0;
				bRadSeeking = False;
				rRadMainSt = _RadInit;
				
#if FM_NARROWBAND
				if (Radio_7786.Area==_Europe1||Radio_7786.Area==_USA||Radio_7786.Area==_Latin || Radio_7786.Area==_Russia && cBand != _BandFM3)
				{
				    Elite_CmdStartup(AlignVCO_EUUSA);
				}
				else if(Radio_7786.Area==_Japan || Radio_7786.Area==_Russia && cBand == _BandFM3)
				{
				    Elite_CmdStartup(AlignVCO_Japan);
				}
				else
				{
				    Elite_CmdStartup(AlignVCO);
				}
#else
				Elite_CmdStartup(AlignVCO); /*Normally use wide band application*/
#endif
				rRadSubSt = TUNER_STARTUP_WAIT_VCO_FE_ALIGN;
				Rad_X64ms= 0/64;
			}
			return;

		case	_RadPS:		
			PSProc( );
			return;

		case	_RadAS:	
			ASProc( );	
	   		if(rRadSubSt == 0)
	   		{/*a station found, seeking completed*/
				RadSetToDl500ms(23);
	   		}
			return;

		case	_AutoTUp:
		case 	_AutoTDn:
	   		ATProc( );
	   		if(rRadSubSt == 0)
	   		{/*a station found, seeking completed*/
				RadSetToDl500ms(24);
	   		}
	   		return;

		case	_RadScan:	
			ScanProc( );
			return;
	}
}
/*************************APICAL ELECTRONICS-TECHNOLOGY Co.,LTD****************************/
/*************************************End of file***************************************/

