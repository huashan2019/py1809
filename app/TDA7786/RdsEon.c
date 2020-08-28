/*******************************************************************************\
*	  	 	RDS EON data program file							*
* Author:  SAMWELL TECHNICAL Co.,LTD                                 			*
*------------------------------------------------------------------------------*/
#define	RDSEon_ROOT
#include	"config.h"

#if	_UartDebug
	#define	_DebugEon			1
#endif

uchar rRdsAfIdx;
//TYPE_WORD	TmpRadStatNum;

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonAfInit
INPUT/OUTPUT : EON element index / None.
DESCRIPTION  : Init a selected AF list in the EON structure.
COMMENTS     : 
-----------------------------------------------------------------------------*/
void	RDSEonAfInit(uchar eonidx)
{
	uchar	i;
	
   	/*RdsEonBlkC.Flag[i] = 0;*/
   	for(i=0; i<_EonAfLength; ++i)
		RDSEonBlkData[eonidx].Af[i] = _FillerCode;
	//RDSEonBlkData[eonidx].False = 0;
  	return;
}     

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonAfInsert
INPUT/OUTPUT : EON Idx and new AF / None.
DESCRIPTION  : Insert a new AF in the EON structure if needed.
COMMENTS     : 
-----------------------------------------------------------------------------*/
void RDSEonAfInsert (uchar reonidx, uchar reonfreq)
{
	uchar	*p,*p1,d;
	
  	if (reonfreq > 204) return;		/* Not a correct AF.*/
  	//d = reonidx;
  	d = sizeof( EonData);
  	//p = &(RDSEonBlkData[reonidx].Af[0]);
  	p = (uchar *)RDSEonBlkData;
  	reonidx = (reonidx*d + 2);
	p += reonidx;
	p1 = p;
	
#if 0
  	if ( (*p != reonfreq)  /* New AF not yet in the list.*/
  	  && (*(p+1) != reonfreq)
  	  && (*(p+2) != reonfreq) )
	{
		*(p+3) = *(p+2);   /* Inserted by rolling.*/
		*(p+2) = *(p+1);
		*(p+1) = *p;
		*p = reonfreq;
#if _DebugEon
	Printf("\r\n Eon Af Insert: \n");
	// UARTOutputChar( ConvNumToASCII(d >> 4) );
	// UARTOutputChar( ConvNumToASCII(d & 0x0F) );
	// UARTOutputChar(':');
	// UARTOutputChar( ConvNumToASCII(reonfreq >> 4) );
	// UARTOutputChar( ConvNumToASCII(reonfreq & 0x0F) );	
	// UARTOutputChar('\n');	
#endif		
	}
#else
	for(d=0; d<_EonAfLength; d++)
	{
		if(*p == reonfreq)	return;
		else if(*p == _FillerCode)	break;
		else p++;
	}
	
	for( ; ; )
	{
		if(p == p1) break;
		*p = *(p-1);
		p--;
	}
	*p = reonfreq;
	
#endif

  	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonExtPi
INPUT/OUTPUT : SR3-SR2 registers values / None.
DESCRIPTION  : Extract current received EON PI code from Block D / 14A.
COMMENTS     : Extract EON PI from SR3 (Country-Reg).
               Extract EON PI from SR2   (Id Number).
-----------------------------------------------------------------------------*/
void	RDSEonPiAfInsert( )
{
	uchar	i;
	if(bEonAfDet)	return;

	for (i=0 ; i<_EONStructSize ; i++)   /* Check EON structure list.*/
	{
	  	if (RDSEonBlkData[i].Pi == _NoPi)            /* PI not yet in the list.*/
	  	  break;
	  	if(RDSEonBlkData[i].Pi == rRdsCurSR32.word)         /* PI exists in EON list ?*/
	  	{
	  	 	RDSEonAfInsert(i,rRdsEonData1);                            /* Store EON freq.*/
	  	  	RDSEonAfInsert(i,rRdsEonData2);
	  	  	return;
	  	}
	  	
	}
	rRdsEonIdx = i;
	/* PI not yet in the list.*/
	if (rRdsEonIdx == _EONStructSize) /* EON list end reached w/o finding PI code.*/
	{
		rRdsEonIdx = 0;
	}
	
  	RDSEonAfInit(rRdsEonIdx);                 /* Clear the previous AF list.*/
  	
	RDSEonBlkData[rRdsEonIdx].Pi = rRdsCurSR32.word;             /* Store EON PI code high.*/
#if _DebugEon
	Printf("\r\n Eon PI Insert: x%",RDSEonBlkData[i].Pi);
#endif	
	RDSEonAfInsert(rRdsEonIdx,rRdsEonData1);              /* Store EON freq.*/
	RDSEonAfInsert(rRdsEonIdx,rRdsEonData2);
/*	RDS_DataPresetEonUpdate(pih,pil,af1);       /* Preset update with EON data.*/
/*	RDS_DataPresetEonUpdate(pih,pil,af2);		*/

	//if((++rRdsEonIdx) == _EONStructSize)	rRdsEonIdx = 0;

	return;
}

//extern void RDS_Insert_AF(char W,char *DE,char A);/*frequency,AF index,max count of AF*/
///****************************************
//register  : WA,BC,HL,DE
//****************************************/
//void RDS_Insert_AF(char W,char *DE,char A)/*frequency,AF index,max count of AF*/
//{
//	if(*DE>=A)	/* reach max count */
//		return();
//	if(*DE==0)
//	{/* empty af list */
//		if( (W!=0&&W<=204) || W==250 )
//		{/* The first effective af frequency */
//			*DE=1;
//			++DE;
//			*DE=W;
//		}
//		return();
//	}
//	
//	if(*DE > 1)/*Add by  11/27/02*/
//	/*RDS_AF_Clear = False;/*Enable AF feature*/
//	
//	if( (W!=0&&W<=204) || W==250 )
//	{
//		B=(*DE);	/* Current AF count */
//		HL=DE+1;	/* First Af Address */
//		for(C=0;C<B;++C)
//		{/* AF由小到大排列 */
//			if(*HL>=W)
//				break;
//			++HL;
//		}
//		if(*HL==W)
//			return();
//		if(C==B)
//			*HL=W;
//		else
//		{/* 重新排序 */
//			PUSH(W);
//			A=*DE;
//			HL=DE;
//			HL+=A;
//			for(;C<B;++C)
//			{
//				A=*HL++;
//				*HL=A;
//				HL-=2;
//			}
//			++HL;
//			POP(W);
//			*HL=W;
//		}
//		++(*DE);/* AF index+1 */
//	}
//	return();
//}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonExtPi
INPUT/OUTPUT : SR3-SR2 registers values / None.
DESCRIPTION  : Extract current received EON PI code from Block D / 14A.
COMMENTS     : Extract EON PI from SR3 (Country-Reg).
               Extract EON PI from SR2   (Id Number).
-----------------------------------------------------------------------------*/
void RDSExtEonAf( )
{
  	/*RDS_EonAvailableCntSet();              /* EON is available on that station.*/
  	
  	switch (rRdsEonVar)
  	{
  		case 5: /* EON Variante 5: Mapped freq 1.*/
  		case 6: /* EON Variante 6: Mapped freq 2.*/
  		case 7: /* EON Variante 7: Mapped freq 3.*/
  		case 8: /* EON Variante 8: Mapped freq 4.*/
  		/* Method B frequencies */
			/*if(RadAreaSt( )==_Europe1)
			/*{/* The step is 50k. the AF value should be multiplied by 2*/
			/*	A = rRdsEonData1;
			/*	W = 0;
			/*	BC = WA << 1;
			/*	if(B != cRadStatH || C != cRadStatL)	return;
			/*}
  		  	/*else	if (cRadStatL != rRdsEonData1)    return;    /* Wrong Tuned freq.*/
  		  	rRdsEonData1 = _FillerCode;
  		case 4: /* EON Variante 4: Method A frequencies............................*/  	
			RDSEonPiAfInsert( );
  			break;
  			 
  		case 0x0D: /* EON Variante 0D: EON PTY CODE */ 
  		  	/*A = rRdsEonData1>>3;
  		  	RdsEonBlkC.Pty[rRdsEonIdx] = A;
  		  	A = rRdsEonIdx+1;
  		  	W = 0;
  		  	rRdsEonIdx = WA % _EonMaxSize;  /* Update rolling index.*/
  	  		//if((rRdsEonTaSt == _RdsEonTaOn) && (!(rRdsEonData2 & 0x01)))     /* EON TA Not already active.*/
  	    	//	rRdsEonTaSt = _RdsEonTaEnd;                    /* EON TA Request.*/  	  		  		
  		  	break;                                                                   
  	} 
  	return;
}


/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonExtTa
INPUT/OUTPUT : SR2 register value / None.
DESCRIPTION  : Extract current received EON TA info from Block B / 14B.
COMMENTS     : 
-----------------------------------------------------------------------------*/
void 	RDSExtEonTa( )
{
  	/*RDS_EonAvailableCntSet();              /* EON is available on that station.*/
  	
  	if((rRdsCurSR2&0x18) == 0x18)                      /* EON TA flag active.*/
  	{/* TP(ON) and TA(ON) all on, indicate that EON TA info is being transfered */
  		RdsTraffic_X64ms = 3000/64;
#if _DebugEon
	Printf("\r\n EonTAP:rRdsEonTaSt=%x",rRdsEonTaSt);
#endif  		
  	  	if (rRdsEonTaSt == _RdsEonTaWait)     /* EON TA Not already active.*/
  	    	rRdsEonTaSt = _RdsEonTaReq;                    /* EON TA Request.*/
  	}
  //	else
  //	{
  //	  	if((rRdsEonTaSt == _RdsEonTaOn) && (!(rRdsEonData2 & 0x01)))     /* EON TA Not already active.*/
  //	    	rRdsEonTaSt = _RdsEonTaEnd;                    /* EON TA Request.*/    		
  //	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonElntSearch
INPUT/OUTPUT : EON PI / Index in EON struct list.
DESCRIPTION  : Search an element in the EON structure.
COMMENTS     : 
-----------------------------------------------------------------------------*/
uchar RDSEonElntSearch( )
{
	uchar i = 0;
#if _DebugEon
	Printf("\r\n Chk Eon PI");
#endif	
	for (i=0 ; i<_EONStructSize ;++i )					/* Check EON structure list.*/
	{
		if (RDSEonBlkData[i].Pi == _NoPi)				/* PI not in the list.*/
		{
#if _DebugEon
	Printf("\r\n PI not In Eon list");
#endif	
			break;
		}
		if (RDSEonBlkData[i].Pi == rRdsCurSR32.word)		/* PI exists in EON list ?*/
		{
			rRdsEonTaIdx = i;
#if _DebugEon
	Printf("\r\n PI In Eon list :%4x",RDSEonBlkData[i].Pi);
#endif	
			return(1);
		}
	}
	return(0);	/* Not found.*/
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonExtTaPi
INPUT/OUTPUT : SR3-SR2 registers values / None.
DESCRIPTION  : Extract current received EON TA PI code from Block D / 14B.
COMMENTS     : Extract EON PI from SR3 (Country-Reg).
               Extract EON PI from SR2   (Id Number).
-----------------------------------------------------------------------------*/
void RDSExtEonTaPi( )
{
  	/*RDS_EonAvailableCntSet();				/* EON is available on that station.*/
  	if(bEonAfDet)	return;
  		
  	if (rRdsEonTaSt == _RdsEonTaReq) /*...................EON TA request.*/
  	{   /* Search if the EON PI code is inside the EON struct list.*/
#if _DebugEon
	Printf("\r\n Cur EonPI: %4x",rRdsCurSR32.word);	
#endif  		  		
  	  	if (RDSEonElntSearch( ))			/* Check if PI is in EON list.*/
  	  	{
#if _DebugEon
	Printf("\r\n Eon TA Active \n"); 
#endif		  	  		
  	  	  	//rRdsEonTaSt = _RdsEonTaActIf;
  	  	  	/* Ta enable and not in traffic or alarm mode */
  	  	  	if(bRdsTAEn && !bEonTrafficOn && rRdsMainSt!=_Traffic && bEonTaTAEn)
  	  	  	{
  	  	  		/*RDSEonTaSwitchTo( );*/
  	  	  		//bEonTrafficOn = True;
				//rRdsMainSt = _Traffic;	/* Switch to EON TA status */
				//rTrafficSt = _TrafficInit;
				rRdsEonTaSt = _RdsEonTaActIf;
				bEonAfDet = 1;
				//cOldRadStatL = cRadStatL;
				//cOldRadStatH = cRadStatH;
				BkupStatNum( ); /* backup current station */
				rRdsAfIdx = 0;
#if _DebugEon
	Printf("\r\n Eon TA Start. \n");
#endif		
				return;
			}
  	  	}
  	  	rRdsEonTaSt = _RdsEonTaWait;			/* EON TA Request aborted.*/
  	}
	return;
}

 /*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonInit
INPUT/OUTPUT : None.
DESCRIPTION  : Initialisation of the RDS EON variables.
COMMENTS     : 
-----------------------------------------------------------------------------*/
void RDSEonInit( )
{
	uchar i;

  	/*RDS_EonAvailableCntClr();                  /* EON not available by default.*/
  	rRdsEonIdx = 0;
  	/* Initialisation of the EON extraction data...............................*/
  	rRdsEonVar = 0xFF;
  	rRdsEonData1 = 0xFF;
  	rRdsEonData2 = 0xFF;
  	
  	/* Initialisation of the EON TA management.................................*/
  	rRdsEonTaSt = _RdsEonTaWait;
  	rRdsEonTaIdx = 0;
  	bRdsEONAvail = False;
  	rRdsEonFlag.byte = 0;
  	
	for(i=0; i<_EONStructSize; i++)
	{
		RDSEonBlkData[i].Pi = _NoPi;
	}
  	/* Initialisation of the EON PTY management.................................*/
  	/*rRdsEonPtySt = _RdsEonPtyWait;
  	rRdsEonPtyIdx = 0;*/
#if _DebugEon
	Printf("\r\n Eon Init. \n");
#endif  	
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonTaSwitchTo
INPUT/OUTPUT : None / 0: aborted - 1: success.
DESCRIPTION  : Check the corresponding frequency which should broadcast TA and
               switch temporary to this frequency if OK. Return the status of
               the switching.
COMMENTS     : 
-----------------------------------------------------------------------------*/
//void RDSEonTaSwitchTo ( )
//{
//	uchar	*p,i;
//	TYPE_WORD rfreq;
//	
//    /* Set this EON station false next minutes to avoid noisy switching.*/
//    /*RDS_EonFalseSet(rRdsEonTaIdx);*/
//    p = &(RDSEonBlkData[rRdsEonTaIdx].Af);
//	rfreq.word = 0;
//	for (i=0;;)   /* Switch to the correct station if possible.*/
//	{
//		p += i;
//		rfreq.byte[0] = *p;
//#if _DebugEon
//	Printf("\r\n Eon SW:  \n");
//	// UARTOutputChar( ConvNumToASCII((rfreq.byte[0] >> 4) & 0x0F) );
//	// UARTOutputChar( ConvNumToASCII((rfreq.byte[0] >> 0) & 0x0F) );	
//	// UARTOutputChar('\n');
//#endif  					
//			
//		if (rfreq.byte[0] > 0 && rfreq.byte[0] < 205)
//		{/* a valid station */
//			/cOldRadStatL = cRadStatL;
//			//cOldRadStatH = cRadStatH;
//			
//			/*BkupStatNum( );	/* backup current station */
//			
//			//if(RadAreaSt( )==_Europe1)
//			{/* The step is 50k. the AF value should be multiplied by 2*/
//				rfreq.word <<= 1;
//				cRadStatNum.word = rfreq.word;
//			}
//			StartWrPll( );
//			RadSetToDl500ms( );
//			rRdsEonTaSt = _RdsEonTaOn;  /* EON TA switch succedded.*/
//			RDSDataClear( );
//			rRdsPsStat.byte = 0;
//#if 0//_DebugEon
//	Printf("\r\n Eon SW. \n");
//#endif  			
//			return;
//		}
//		else	if((++i)>_EonAfLength)	break;
//	}
//	rRdsEonTaSt = _RdsEonTaWait;
//	return;
//}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_EonTaComeBack
INPUT/OUTPUT : None.
DESCRIPTION  : Force to switch back to the original source after a EON TA
               switch.
COMMENTS     : 
-----------------------------------------------------------------------------*/
void RDSEonTaSwitchBack ( )
{
	//cRadStatNum.word = cOldRadStatNum.word;
	/*RestoreStatNum( );*/
	//StartWrPll( );
	//RadSetToDl500ms( );
	#if _EnableRadSi4730
	RestoreStatNum1( );
	//WriteSi4730Pll( );
	
	Radio7786SetFreq(Radio_7786.Freq);
	#else
	WritePll(_6601Preset);
	#endif
	rRdsEonTaSt = _RdsEonTaWait;
	return;
}

//#if 0
/***************************************************************************************\
*   Function: AF frequency process														*
*   Input   : rRdsAfSt																	*
*   Output  : 																			*
*   description : if the signal strength of new station is stronger than current		*
				 station, replace it.
*-------------------------------------------------------------------------------------*/			
void	EonTADetProc( ) 
{
	TYPE_WORD	rfreq;
	uchar	i, j;
	
	if(!bEonAfDet)	return;

	switch(rRdsEonTaSt)
	{
	case  _RdsEonTaActIf:
		//if(!bSysMuteSt)	break;

		for ( ; ; )	/* Switch to the correct station if possible.*/
		{
			rfreq.word = 0;
			rfreq.byte[0] = RDSEonBlkData[rRdsEonTaIdx].Af[rRdsAfIdx];
		#if _DebugEon
			Printf("\r\n Eon Stat:%x",rfreq.word);
		#endif  					
				
			if (rfreq.byte[0] > 0 && rfreq.byte[0] < 205)
			{/* a valid station */
				if(rAreaSt == _Europe1 || (rAreaSt==_Russia && cBand != _BandFM3))
				{/* The step is 50k. the AF value should be multiplied by 2*/
					rfreq.word <<= 1;
					cRadStatNum.word = rfreq.word;
				}
				#if _EnableRadSi4730
				//WriteSi4730Pll( );
				GetRadFreOctData(cRadStatNum.word);
				Radio7786SetFreq(Radio_7786.Freq);
				#else
				EonWrPll(rfreq.word );
				#endif
				//RadSetToDl500ms( );
				RDSDataClear( );
				rRdsEonTaSt = _RdsEonPiChk;
				rRdsPiCodeL = rRdsPiCodeH = _NoPi;
				RDSStart( );
				RDSTimerInit( );
				bRdsAfAvail = False;
				bRdsPiAvail = False;
				bInvalidPi = False;
				//rRdsPsStat.byte = 0;
			  	//for (i=0; i < 8;)
			  	//{
			  	//	rRdsPsNameAct[i] = 0x00;
			  	//	rRdsPsNameSto[i] = 0x00;
			  	//	++i;
			  	//}
			  	rRdsTaTpStat.byte = 0;
			  	bTpFlag = False;
			  	cRdsTpCnt = 0;
				rRdsPtyStat.byte = 0;
				//rRdsStoPty = 0;		
				//bRdsPSAvail = 0;
				//bTpStat = 0;						
				rRdsEon_X64ms = 400/64;
	#if DebugEon
		Printf("\r\n Eon Chk. \n");
	#endif  			
				return;
			}
			else 
			{
				++rRdsAfIdx;
				if(rRdsAfIdx >= _EonAfLength)
				{
					rRdsEonTaSt = _RdsEonTaEnd;
					break;
				}
			}
		}
	  	break;
	  	
	case _RdsEonPiChk:
		if(bRdsPiAvail)
		{
			rRdsEon_X64ms = 3000/64;
			rRdsEonTaSt = _RdsEonChkSt;
	#if _DebugEon
			Printf("\r\n Eon PI Exist. \n");
	#endif			
		}
		else if(!rRdsEon_X64ms)
		{
			++rRdsAfIdx;
			if((rRdsAfIdx)>=_EonAfLength)	rRdsEonTaSt = _RdsEonTaEnd;
			else rRdsEonTaSt = _RdsEonTaActIf;
		}
	  	break;
	  	
	case _RdsEonChkSt:
	  	if(bRdsTPPreVal && bRdsTAPreVal)
	  	{
	  		rRdsEonTaSt = _RdsEonTaOn;
  	 		bEonTrafficOn = True;
			rRdsMainSt = _Traffic;	/* Switch to EON TA status */
			rTrafficSt = _TrafficInit;	
			RdsTraffic_X64ms = 3000/64;
	#if _DebugEon
			Printf("\r\n EonTA fnd:%x",rRdsEon_X64ms);			
	#endif			   			
		}
		else if(!rRdsEon_X64ms)
		{
	#if _DebugEon
			Printf("\r\n Next Eon  AF. \n");
	#endif
			++rRdsAfIdx;
			if((rRdsAfIdx)>=_EonAfLength)	rRdsEonTaSt = _RdsEonTaEnd;
			else rRdsEonTaSt = _RdsEonTaActIf;
		}
	  	break;
	
	case  _RdsEonTaOn:
		if(bRdsTPPreVal && bRdsTAPreVal && bEonTaTAEn && !bMsgEndEonTa) return;

	case  _RdsEonTaEnd:
		bEonAfDet = 0;
		RDSEonTaSwitchBack( );
		RDSFmFreqChange();
#if _DebugEon
		Printf("\r\n Eon Det End. \n");
#endif		
		break;
	}
}

/*********************STT ELECTRONICS-TECHNOLOGY Co.,LTD********************************/
/*****************************End of file***************************************/
