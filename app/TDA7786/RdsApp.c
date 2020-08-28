
#include	"config.h"

#if	_UartDebug
	#define	_DebugSeeKPiStart		0
	#define	_DebugTAKy			    0
#endif

void	RDSTimerInit( )
{
	RdsPty_X128ms = 2000/128;
	RdsTraffic_X64ms = 0;
	RdsMsAf_X500ms = 30000/512;
	//RdsMsAf_X128ms = 2000/128;
	RdsMeasure_X32ms = 1000/32;
	RdsSync_X8ms = 800/8;
	RdsReceive_X32ms = 33;
	RdsTpSeek_X500ms = 5000/512;
	RdsPiSeek_X500ms = 90000/512;
	bTrafficOn = False;
	bEonTrafficOn = False;
  	bPtyAlarm = False;
	bUserTaDisable = False;
	cRdsAfDet = 0;
	rRdsAfSt = _AfIdle;
	rRdsSmeterBuff.word = 0;
	cSmeterCnt = 0;
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_FmFreqChange
INPUT/OUTPUT : New frequency/offset to tune / None.
DESCRIPTION  : RDS data transfer for frequency changing.
COMMENTS     : TOO LARGE CODE !!!
-----------------------------------------------------------------------------*/
void RDSFmFreqChange( )
{
	uchar	i,j;

	if(bRDSDs)	return;
	//if(bAM_FM || bPiSeeking)	return;		/*Rds data does not be cleared in Am band*/
  	/*if (RDS_SwafPiSearchStop(newfreq))             /* If PI search mode result.*/
  	/*  return;
  	/* Stop search at the same previous tuned freq. ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  	/*if (newfreq == Rds_CurrentBand.af[0])
  	/*{
  	/*  if ( ( ValBit(Rds_CurrentBand.ctrl,offsetbit) &&  newoffset)
  	/*    || (!ValBit(Rds_CurrentBand.ctrl,offsetbit) && !newoffset) )
  	/*    return;                                 /* Keep the actual stored data.*/
  	/*}
  	/*RDS_DataPrsUpdateInit();         /* Force new freq to be updated by preset.*/
  	RDSStop( );                                         /* The RDS is disabled.*/
  	/*RDS_FmQuickTransfer();            /* Quick DAF/BAND/PRESET update transfer.*/
  	RDSAfInit( );                        /* FM dynamic AF list initialisation.*/
  	/*RDS_SwafWeightInit();/* FM weight init after a new current AF list setting.*/
  	rRdsPiCodeL = rRdsPiCodeH = _NoPi;
  	//Rds_SwafFixPiH = Rds_SwafFixPiL = _NoPi;   /* Fixed PI for AF switch reset.*/
  	/*RDS_DataInitCurrentBand();                           /* Current band reset.*/
  	/*Rds_CurrentBand.af[0] = newfreq;            /* Set the new tuned frequency.*/
  	/*if (newoffset) SetBit(Rds_CurrentBand.ctrl,offsetbit);   /* New offset set.*/
  	/*else           ClrBit(Rds_CurrentBand.ctrl,offsetbit);					*/
  	RDSDataClear( );	/* Clear all the RDS Data.*/
	//fmRdsStatus(1);
	
  	rRdsRevdAfNum = 0;
  	rRdsStorAfNb = 0;	/* AF Stored list reset*/
  	//if((!bBand0PSAvail&&cBand==0)||(!bBand1PSAvail&&cBand==1)
  	//||(!bBand2PSAvail&&cBand==2))	bRd24C04Flg = False;
	
	if(!bNewKey || rKeyResult != _kcBand)
	{
  		if(cBand==0)	bBand0PSAvail = False;
		else	if(cBand==1)	bBand1PSAvail = False;
		else	if(cBand==2)	bBand2PSAvail = False;
	}
	RDSTimerInit( );
	bRdsAfAvail = False;
	bRdsPiAvail = False;
	bInvalidPi = False;
	rRdsPsStat.byte = 0;
  	for (i=0; i < 8;)
  	{
  		rRdsPsNameSto[i] = rRdsPsNameAct[i] = ' ';//0x00;
  		++i;
  	}
	for(j=0;j<16;j++)
	{
		rRdsRTNameSto[4*j] = rRdsRTNameAct[4*j] = ' ';
		rRdsRTNameSto[4*j+1] = rRdsRTNameAct[4*j+1] = ' ';
		rRdsRTNameSto[4*j+2] = rRdsRTNameAct[4*j+2] = ' ';
		rRdsRTNameSto[4*j+3] = rRdsRTNameAct[4*j+3] = ' ';
	}
	//RDSTxStatName();
	//RDSTxStatRT();
  	rRdsTaTpStat.byte = 0;
  	bTpFlag = False;
  	cRdsTpCnt = 0;
	rRdsPtyStat.byte = 0;
	rRdsStoPty = 0;
  	RDSStart( );					/* The RDS is enabled.*/
  	RDSEonInit( );                  /* Initialisation of the RDS EON variables.*/
  	/*RDS_DataSearchPreset(); /* Check if current freq is one of the band preset.*/
	if(bAM_FM || (!bRdsPtyEn && rRdsMainSt!=_TpSeek))/*	RadDpUpdate(_DpRadPty);
	else*/	RadDpUpdate(_DpRadFreq);
	/*else	if(bRdsPtyEn)	RadDpUpdate(_DpRadPtyT);*/
	//if(rRdsMainSt!=_PtySeek)	bRdsPtyEn = False;
	
	bRdsPSAvail = 0;	//add by Jason 2013-8-17 17:33
	
	//bTpStat = 0;
}


/***************************************************************************************\
*	   Function: read RDS information from preset memory				*
*	   Input   : cPSNum									*
*      Output  : PI code, AF list and PS name									*
\*-------------------------------------------------------------------------------------*/			
void	ReadRdsPreMem(uchar ps)
{
	uchar	l;
	
	if(bRDSDs)	return;
		
#ifndef	_OnlyReduceRAM		
	ps = ps + cBand*7;
#else
	ps = 0; 
#endif	
	Rds_SwafFixPi.word = RDSPreMem[ps].RDSPi; /* Store  PI code.*/     

	for(l = 0; l<_AfMemLength; ++l)
	{
		rRdsDafFmFreq[l] = RDSPreMem[ps].RDSAf[l];
	}
	if(rRdsDafFmFreq[0] != 0)
	{
		rRdsRevdAfNum = _MaxAfList;
		rRdsStorAfNb = _MaxAfList;
	}
	
	for(l = 0; l<8; ++l)
	{
		rRdsPsNameSto[l] = RDSPreMem[ps].RDSPs[l];
	}
	if(rRdsPsNameSto[0] != 0 && rRdsPsNameSto[0] != 0xff)
	{
		bRdsPSAvail = True;
	}
	if(!bRdsPtyEn)
	{
		if(bRdsPSAvail)	rRadDispSt = _DpRadRdsPs;/*RadDpUpdate(_DpRadRdsPs);	/*Update PS display*/
		else	rRadDispSt = _DpRadFreq;
	}
	//bUpdateDisp = True;
}

/***************************************************************************************\
*	   Function: read RDS information from preset memory				*
*	   Input   : cPSNum									*
*      Output  : PI code, AF list and PS name									*
\*-------------------------------------------------------------------------------------*/			
void	WriteRdsPreMem(uchar ps)
{
	uchar	l;

	if(bRDSDs)	return;
#ifndef	_OnlyReduceRAM
	ps = ps + cBand*7;
#else
	ps = 0;
#endif	
	RDSPreMem[ps].RDSPi = Rds_SwafFixPi.word; /* Store  PI code.*/     
	for(l = 0; l<_AfMemLength; ++l)
	{
		RDSPreMem[ps].RDSAf[l] = rRdsDafFmFreq[l];
	}
	for(l = 0; l<8; ++l)
	{
		RDSPreMem[ps].RDSPs[l] = rRdsPsNameSto[l];
	}	

}

//uchar CheckPrePICode(uchar *pBand, uchar *PNum)
//{
//	uchar	i, j;
//
//	j = 1;
//	i = _BandFM1;
//	for( ; ; )
//	{
//		if(RDSPreMem[j + i*7].RDSPi == Rds_SwafFixPi.word)
//		{
//			*pBand = i;
//			*PNum = j;
//			return True;
//		}
//		j++;
//		if(j > 6)
//		{
//			i++;
//			j = 1;
//		}
//		if((i<=cBand) && (j<cPSNum))	;
//		else break;	
//	}
//	return False;
//}

void ClrRDSParms(void)
{// add by Jason 2013-1-18 11:59	
	rRdsMainSt = rRdsSubSt = rRdsAfSt = 0;
	rRdsFlag.byte = rRdsFlag1.byte = rRdsFlag2.byte = rRdsFlag3.byte = 0;
	rRdsFlag4.byte = rRdsFlag5.byte = rRdsFlag6.byte = 0;
  	rRdsPiCodeL = rRdsPiCodeH = _NoPi;
  	Rds_SwafFixPiH = Rds_SwafFixPiL = _NoPi;
  	rRdsPsStat.byte = 0;
  	rRdsRTFlag = rRdsRTStat.byte = 0;
  	rRdsExtStat.byte = 0;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_FmMainInit
INPUT/OUTPUT : None.
DESCRIPTION  : RDS data initialisation.
COMMENTS     : Call only one time after reset.
-----------------------------------------------------------------------------*/
void RDSInit( )
{
	uchar i;
	ClrRDSParms( );
	
	bRdsEnBak = 0;
	bEonTaTAEn = 0;/*lhs 2015 07 15*/
  	//RDSDataInitAllPreset( );        /* Init of the PRESET and BAND memory.*/
  	//RDSSwafMainInit( );             /* Init of the AF SWITCH data  memory.*/
  	RDSAfInit( );                  /* Init of the DYNAMIC AF data memory.*/
  	//RDSAfInit( );               /* Init of the AF data  memory.*/
	/*if(rAreaSt == _Europe || rAreaSt == _Russia)*/  	
	bRdsAfDetEn = 0;
	rPtyType = _None;
  	//if(rAreaSt == _Russia)	
  	bRDSDs = True;
  	
  //rTAVol = _TaVolInit;				/*Traffic volume initialize*/
	//bRdsAfDetEn = 1;
	//bRdsTAEn = 1;
	bRdsPtyEn = 0;
	bRdsRegOn = 0;
	
  	bEonTaMute = False;
  	//rPtyMType = _PopM;              /* Init of the PTY code data memory.*/
  	//rPtySType = _None;
  	RDSEonInit( );              /* Initialisation of the RDS EON variables.*/
/*  	RDSCtInit( );                   /* Initialisation of the RDS CT  variables.*/
  	RDSDataClear( );                /* Clear all the RDS Data.*/
/*  	RDSDataSearchPreset( ); /* Check if current freq is one of the band preset.*/

	//rdsBlerMax[0] = CORRECTED_THREE_TO_FIVE; // Block A
	//rdsBlerMax[1] = CORRECTED_ONE_TO_TWO;    // Block B
	//rdsBlerMax[2] = CORRECTED_THREE_TO_FIVE; // Block C
	//rdsBlerMax[3] = CORRECTED_THREE_TO_FIVE; // Block D
	//FM_RDS_RdsValid[0]    = 0;
	//FM_RDS_RdsValid[1]    = 0;
	//FM_RDS_RdsValid[2]    = 0;
	//FM_RDS_RdsValid[3]    = 0;
	// Reset Debug Group counters
	//for (i=0; i<32; i++)
	{
	   // RdsGroupCounters[i] = 0;
	}

}

/***************************************************************************************\
*	   Function: Restore RDS prevoius status.		 									*
*	   Input   : 																		*
*      Output  : 																		*
*      description : Traffic mode processing
\*-------------------------------------------------------------------------------------*/			
void	RestoreRdsSt( )
{
	if(rRdsMainSt==_PiSeek || rRdsMainSt==_TpSeek || rRdsMainSt==_PtySeek)
	{
		bPiSeeking = False;	/* Stop seeking and restore previous station */
		RestoreStatNum( );
		//if(bRdsPtyEn && !bAM_FM)	rRdsMainSt = _PtyOn;
		//else	
			rRdsMainSt = _PtyOff;
	}
	return;
}

extern	uchar   const	PtyDpCharTab[32][9];

#if 0
/*******************************2004-8-31 9:51******************************************\
*	   Function: RDS PTY table						 									*			*
*	   Input   : bNewKey,rKeyResult														*
*          Output  : 																	*
\*-------------------------------------------------------------------------------------*/			
uchar	const	PTY_M_Tab[ ]=
{/*PTY music option table*/
	_PopM,_RockM,
	_EasyM,_LightM,
	_Classics,_OtherM,
	_Jazz,_Country,
	_NationM,_Oldies,
	_FolkM,_FolkM,
};

#define	_MaxMusicType	11

uchar	const	PTY_S_Tab[ ]=
{/*PTY speech option table*/
	_News,_Affairs,_Info,
	_Sport,_Educate,_Drama,
	_Culture,_Science,_Varied,
	_Weather,_Finance,_Children,
	_Social,_Religion,_PhoneIn,
	_Travel,_Leisure,_Document,
};
#define	_MaxSpeechType	17


uchar   const	PsNoneTab[ ] = {"PS  NONE"};

void	RdsStDisp( )
{
	uchar	i,*p;
	
	switch(rRdsDispSt)
	{
		case	_RdsDpClk:	/*clock*/
			//ClockDisp( );
			return;

		case	_RdsDpPs:	/*ps*/
			if(bRdsPSAvail)
			{
				p = &rLCDChar1;
				for(i=0;;)
				{
					*p = rRdsPsNameSto[i];
					++p;
					if((++i)==8)	break;
				}
			}
			else	;//DispCharN(PsNoneTab, &rLCDChar1);
			return;
			
		case	_RdsDpFreq:	/*freq*/
			// BAND display
			RadioDpFreq( );
			return;
			
		case	_RdsDpPty:	/*pty*/
			//DispCharN(&PtyDpCharTab[rRdsStoPty][0],&rLCDChar1);
			return;
			
		case	_RdsDpSrc:/* source */
			return;
	}
	return;
}
#endif

/*******************************2004-8-31 9:51******************************************\
*	   Function: RDS PTY table						 									*			*
*	   Input   : bNewKey,rKeyResult														*
*          Output  : 																	*
\*-------------------------------------------------------------------------------------*/			
void RdsKeyAnaly( )
{
	uchar	i;
#if 1
	if(!bNewKey || bRDSDs)	return;

	if(bAM_FM/* && rSndSrc==_SndSrcRad*/)	return;
	if(rAreaSt == _Russia && cBand == _BandFM3)	return;

	//if(rSndSrc == _SndSrcRad)
	//{
	//	if(rKeyResult==_kcAf)	rKeyResult = _kcAfL;
	//	else	if(rKeyResult==_kcTa)	rKeyResult = _kcTaL;
	//	else	if(rKeyResult==_kcPty)	rKeyResult = _kcPtyL;
	//}

	if((rKeyResult ==_kcAF || rKeyResult ==_kcTA  || rKeyResult ==_kcPTY || rKeyResult ==_kcReg))	//	&& rSndSrc == _SndSrcRad)
	{/* AF or TA key pressed */
		//if(rSndSrc == _AVSrcFlh && rKeyResult==_kcAf)	return;
		RestoreAfSt( );
		if(rKeyResult == _kcAF)
		{/* toggle AF function */
			//if(bAM_FM)	bRdsAfDetEn = False;
			//CPL(bRdsAfDetEn);
			bRdsAfDetEn=!bRdsAfDetEn;
			// Disable by Jason 2013-1-18 11:02
			//if(!bRdsAfDetEn)	
			//{
			//	if(rRdsFlag1.byte&0x1c)	;			// add by Jason 2012-10-22 18:09
			//	else	rRdsMainSt = _PtyOff;
			//}
			//bUpdateDisp = True;
			RdsMsAf_X500ms = 30000/512;
			RdsPiSeek_X500ms = 90000/512;
			if(bRadSeeking)
			{
				bPiSeeking = False;
				RestoreStatNum( );
				//StartWrPll( );
				RadSetToDl500ms(20);	

			}
			
			if(!bRdsAfDetEn/*bRadSeeking*/)
			{/* Stop searching and restore previous station */
				if(rRdsMainSt == _PiSeek)
					rPiSeekSt = _PiSeekEnd;
				else
					rRdsMainSt = _PtyOff;
				//Disable_RDS_Int();
			}	
			else if(bRdsAfDetEn)
			{
				rRdsMainSt = _PiSeek;
				rPiSeekSt = _PiSeekWait;
				Rds_SwafFixPiBak.word = rRdsPiCode.word;
				BkupStatNum();
				//cRadStatNum.word = 0;
				RdsPiSeek_X500ms = 90000/512;	//125000/500;	/* Set 90s timer */
				//Enable_RDS_Int();
			}

			if(!bRdsAfDetEn)
			{
				rdsinfoState3 &= (u8)(~BIT6);
			}
			else
			{
				rdsinfoState3 |= (u8)(BIT6);
			}

			ReportRadioBaseInfo();
			//RDSTxRDSFlag();			
		}
		else if(rKeyResult == _kcPTY)
		{
			if(!rPtyType)	return;
			bPiSeeking = False;
			if(bRadSeeking)
			RestoreStatNum( );
			else
			StartWrPll( );
			RadSetToDl500ms(22);	
			bRdsPtyEn = 1;
			bPtyChged = True;	/* Regard as pty changed */
			rRdsMainSt = _PtySeek;		/* PtyOn-->PtySeek */
			rPtySeekSt = _PtySeekWait;
			
			rdsinfoState3 |= (u8)(BIT5);
			
			ReportRadioBaseInfo();
		}
		else if(rKeyResult ==_kcReg)
		{
			//CPL(bRdsRegOn);
			bRdsRegOn=!bRdsRegOn;
			bPiSeeking = False;
			if(bRadSeeking)
			RestoreStatNum( );
			else
			StartWrPll( );
			RadSetToDl500ms(22);	
			rRdsMainSt = _PtyOff;
			//rSysInfoID = 0X06;
			//ArmTxCmd(0x81, 0xB9);
			//RDSTxRDSFlag();
		}
		else/*rKeyResult == _kcTa*/
		{/* toggle TA function and display item */
			if(rRdsMainSt==_PtySeek || rRdsMainSt==_PiSeek)
			{/* Stop searching and restore previous station */
	#if _UartDebug
		Printf("\r\n PTY,_TpSeek cancel:rPtyType=%2x,rRdsMainSt=%2x",rPtyType,rRdsMainSt);
	#endif			  				
				if(bRadSeeking)
				RestoreStatNum( );
				else
				StartWrPll( );
				RadSetToDl500ms(21);
				rRdsMainSt = _PtyOff;
				//rRadInfoID = _RadFre;
				//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
				//RDSTxStatNum();
				RDSTxPTYType();
				
				rdsinfoState3 &= (u8)(~BIT5);
				
				ReportRadioBaseInfo();
	#if _UartDebug
		Printf("\r\n PTY cancel:rPtyType=%2x,rRdsStoPty=%2x",rPtyType,rRdsStoPty);
	#endif			  				
				return;
			}
			else 
			{/* TA ON */
			#if 0
				if(rRdsFlag1.byte&0x1c || bRdsTAEn /*bTrafficOn || bEonTrafficOn || bPtyAlarm*/)
				{/*end tp station play */
					if(rRdsMainSt == _Traffic)
					{/*already in traffic mode*/
			    			if(bRdsPtyEn)	RadDpUpdate(_DpRadPtyT);
			    			else	RadDpUpdate(_DpRadRdsPs);
			    			bUserTaDisable = True;
			    			rTrafficSt = _TrafficEnd;	
			    			if(	bEonTrafficOn )	bMsgEndEonTa = 1;
					#if	_DebugTAKy
						Printf("\r\n TA-On to UserDisable. \n");
					#endif
		    			}
		    			else
			    		{
				    		/*bPtyAlarm = False;*/
			    			bRdsTAEn = False;
			    			bUserTaDisable = False;
						//rSysInfoID = 0X06;
						//ArmTxCmd(0x81, 0xB9);
						RDSTxRDSFlag();
					#if	_DebugTAKy
						Printf("\r\n TA-On to TA-Off. \n");
					#endif		    			
			    		}
		    		}
		    		else
			#endif		
				//if(rRdsFlag1.byte&0x1c || bRdsTAEn )
			    	{
					if(rRdsMainSt !=_TpSeek && !bRdsTAEn)
					{/*TP SEEK START*/
						#if	_DebugTAKy
							Printf("\r\n TA-Off to TAEn:%2x",rRdsMainSt);
						#endif		    				
			    			bRdsTAEn = True;
			    			bUserTaDisable = False;
						RadDpUpdate(_DpRadTaSeek);
						SaveOriginalStat( ); /* backup current station for seeking*/
						bRadSeek2nd = False;
						rRdsMainSt = _TpSeek;
						rTpSeekSt = _TpSeekWait;
						if(rRadMainSt != _RadStdby)	bRadSeeking = 0;
						//rSysInfoID = 0X06;
						//ArmTxCmd(0x81, 0xB9);
						RDSTxRDSFlag();
		    			}
					else
			    		{/*already in tp seek mode*/
				    		/*bPtyAlarm = False;*/
			    			bRdsTAEn = False;
			    			bUserTaDisable = False;
						//rSysInfoID = 0X06;
						//ArmTxCmd(0x81, 0xB9);
						RDSTxRDSFlag();

					#if	_DebugTAKy
						Printf("\r\n TA-On to TA-Off. \n");
					#endif		    			
			    		}
					if(!bRdsTAEn)
					{
						rdsinfoState3 &= (u8)(~BIT4);
					}
					else
					{
						rdsinfoState3 |= (u8)(BIT4);
					}
					ReportRadioBaseInfo();
						return;	    					    		
			    	}
			}
		}
		
		if(bAM_FM)
		{/* Switch to FM band */
			SaveCurBandMsg( );
			bAM_FM = False;
			RDSFmFreqChange( );
			if(bRdsPtyEn)
			{
				cBand = cBandBackup;
				rRdsMainSt = _PtyOff;
			}
			else	cBand = 0;
			bRadSeeking = False;
			ReadBandMsg( );
			bManuAuto = False;/*TUNE +/- key, reset to AUTO status*/
			rRadMainSt = _RadInit;
			rRadSubSt = 0;
		}
		bEonTaMute = False;
		if(armsourceState == ARM_SOURCE_RADIO)	bUserMute = False;
		//TmpDp_X64ms = 0;
		/*MainDisp5S(_mDpSource);
		/*TmpDp_X24ms = 0;*/
	}
	//else	if(/*bAM_FM || */rSndSrc!=_SndSrcRad || rRdsMainSt==_Traffic)	return;
	else return;

	
	bEonTaMute = False;
	
	//MainDisp5S(_mDpSource);
	/*StartBeepA( );*/
	return;
#endif
}

void PTYSelPorc(void)
{
	#if 0
	if(rMainDpPoint == _mDpRdsPty)
	{// digital sound displaying status
		if((bNewKey && (rKeyResult == _kcPtyL)) || !DpTmr_X256ms)
		{
			DpTmr_X256ms = 0;
			MainDpUpdate(_mDpSource);
		}
		else if(bVolEncTurned)
		{
			if((rSndSrc == _SndSrcDVD && cSongSrchMode) || (rSndSrc == _SndSrcUSB && cSongSrchMode))		return;
			if(bVolEncDec)
			{
				if(rPtyType > _PtyMin)	rPtyType--;
				else	rPtyType = _PtyMax;
			}
			else
			{
				rPtyType++;
				if(rPtyType > _PtyMax)	rPtyType = _PtyMin;
			}
			MainDisp5S(_mDpRdsPty);
		}
	}
	else
	{
		if(bNewKey && rKeyResult == _kcPtyL)
		{
			if(bRadSeeking)
			{
				bPiSeeking = False;
				RestoreStatNum( );
				RadSetToDl500ms( );	
				rRdsMainSt = _PtyOff;
			}
			else
			{
				bRdsPtyEn = 1;
				MainDisp5S(_mDpRdsPty);
			}
		}
	}
	#endif
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RdsProc
INPUT/OUTPUT : None
DESCRIPTION  : Rds main proc.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void	RdsProc( )
{
	bMsgEndEonTa = 0;
	
	AfDetProc( );		/*Search AF frequency*/
    
	RdsExtrProc( );		/*Extract RDS data*/
    
	RdsKeyAnaly( );		/*Rds key analysis*/

	if(bAM_FM || bRDSDs)	return;	/* Rds does not work in AM/MW band */
	if(rAreaSt == _Russia && cBand == _BandFM3)	return;
	
	//PTYSelPorc( );
	
	EonTADetProc( );

	if(bRdsAfDetEn)
	{/* AF enable */
		//if((rRdsSyncSt < _BlkSync) && bSync500ms && rRdsAfSt==0)
		{	
			//bUpdateDisp = True;	/*if sync failed, blink AF icon*/
		}
	}
	
	//if(bRdsTAEn)
	//{/*TA enable*/
	//	if(RdsTpSeek_X500ms==0 && rRdsMainSt != _TpSeek)
	//	{
	//		bTpFlag = False;
	//		if(bTAAlarmSeek)
	//		{/* TA Alarm enable */
	//			if(bSync128ms)
	//			{
	//				CPL(bTaAlarmFlg);
	//				if(!bTaAlarmFlg)	RdsTpSeek_X500ms = 90000/500;
	//				bNewKey = True;
	//				StartBeepA( );
	//				//RadTmpDisp(_DpRadNoTpTa,(uchar)(3000/64));// 3000/32
	//				bNewKey = False;
	//			}
	//		}
	//		else
	//		{/* TA seek enable */
	//			RestoreAfSt( );
	//			if(rRdsMainSt!=_PiSeek && rRdsMainSt!=_PtySeek && rRadMainSt == _RadStdby)// rRadMainSt!=_RadPS && rRadMainSt!=_RadScan)
	//			{/* not in pi seek or pty search state */
	//				/*if(rRdsMainSt != _PtyOn)*/	RadDpUpdate(_DpRadTaSeek);
	//				if(rRdsMainSt == _PtyOn)
	//				{
	//	    			bRdsPtyEn = False;
	//	    			bPtyChged = False;
	//				}
	//				SaveOriginalStat( ); /* backup current station for seeking*/
	//				rRdsMainSt = _TpSeek;
	//				rTpSeekSt = _TpSeekWait;
	//				if(bReturn90s) RdsTpSeek_X500ms = 90000/500;
	//				else 		   RdsTpSeek_X500ms = 30000/500;	
	//			}
	//		}
	//	}
	//}
	
	switch(rRdsMainSt)
	{
		case	_PtyOff:
			//if(!Rad_X64ms && bRdsPtyEn)
			//{
			//	bRdsPtyEn = False;
			//	RadDpUpdate(_DpRadRdsPs);
			//}
			MeasureAfList( );
 			if(!bMeasureAfSig && bRdsAfDetEn)
			{/* Af detect enable and not in af search status */
				if(RdsPiSeek_X500ms==0 && rRdsMainSt!=_PiSeek 
			   	&& Rds_SwafFixPiH!=_NoPi && Rds_SwafFixPiL!=_NoPi)
				{/* if no rds signal in 2mins, start search a new sation with same PI */
					/*start PI seeking*/
					rRdsMainSt = _PiSeek;
					rPiSeekSt = _PiSeekWait;
					Rds_SwafFixPiBak.word = _NoPi;
					SaveOriginalStat( );	/* backup current station */
					BkupStatNum();
					RdsPiSeek_X500ms = 90000/512;	//125000/500;	/* Set 90s timer */
					//rRadInfoID = _RadPreList;
					//ArmTxCmd(0x81, 0xB4);
				}
			}
			break;
		
		case	_PtyOn:
			MeasureAfList( );
			if(RdsPty_X128ms!=0 || bAM_FM || (armsourceState != ARM_SOURCE_RADIO))	break;
			RestoreAfSt( );
			/*if(rRdsAfSt==_AfWtPllLk || rRdsAfSt==_AfWtChkSt)
			/*{
			/*	RestoreStatNum( );
			/*	/*rRdsAfSt = 0;
			/*	bMeasureAfSig = False;*/
			/*}*/
			bPtyChged = True;	/* Regard as pty changed */
			++rRdsMainSt;		/* PtyOn-->PtySeek */
			rPtySeekSt = _PtySeekWait;
			/*start auto tunning*/
			break;				
			
		case	_PtySeek:	
			PtySeekProc( );
			break;
		
		case 	_TpSeek:	
			TpSeekProc( );
			break;
			
		case 	_PiSeek:
			PiSeekProc( );
			break;

		case 	_Traffic:	/* include EON TA and Alarm status */
			MeasureAfList( );
			TrafficProc( );
			break;
	}
}

