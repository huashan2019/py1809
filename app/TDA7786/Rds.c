/*******************************************************************************\
*	  	 	RDS sync and data program file							*
* Author:  SAMWELL TECHNICAL Co.,LTD                                 			*
*------------------------------------------------------------------------------*/
#define	RDS_ROOT
#include	"config.h"

#if	_UartDebug
	#define	_DebugRds			0
	#define	_DebugRdRdsData		0
	#define	_DebugPiSeek		0
	#define	_DebugTraff			0
	#define	_DebugInsetStat		0
	#define	_DebugAFJmp			0
	#define	_DebugTPTA			0
	#define	_DebugPSName		1
	#define	_DebugPTY			0
	#define _DebugRTName		1
	#define _DebugRTC			1
#endif
extern void ReportRadioSignalinfo(u8 info);

unsigned long		rds_data;
uchar		rRdsDataBits;

u8 rCrch,rcrcl;

/* ----------------------------------------------------------------------------
ROUTINE NAME : RDS_SyndCalc
INPUT/OUTPUT : None / Computed CRC.
DESCRIPTION  : RDS syndrome computation routine.
COMMENTS     : 
-----------------------------------------------------------------------------*/
#define Rdsdata16h  (u8)(rRdsSR >> 24)
#define Rdsdata16l  (u8)(rRdsSR >> 16)
#define Rdschks10h  (u8)(rRdsSR >> 8)
#define Rdschks10l  (u8)(rRdsSR >> 0)

#define RDS_SyndCalcMacro(HIB,LOB) { rCrch ^= HIB; \
                                     rcrcl ^= LOB; }
void 	RDSSyndCalc( ) 
{
    //rRdsSR
    rCrch = Rdsdata16h;
    rcrcl = Rdsdata16l;

    if (ValBit(Rdsdata16l,5)) RDS_SyndCalcMacro(0xB7,0x00)
    if (ValBit(Rdsdata16l,4)) RDS_SyndCalcMacro(0x5b,0x80)
    if (ValBit(Rdsdata16l,3)) RDS_SyndCalcMacro(0x2d,0xc0)
    if (ValBit(Rdsdata16l,2)) RDS_SyndCalcMacro(0xa1,0xc0)
    if (ValBit(Rdsdata16l,1)) RDS_SyndCalcMacro(0xe7,0xc0)
    if (ValBit(Rdsdata16l,0)) RDS_SyndCalcMacro(0xc4,0xc0)
    if (ValBit(Rdschks10h,7)) RDS_SyndCalcMacro(0xd5,0x40)
    if (ValBit(Rdschks10h,6)) RDS_SyndCalcMacro(0xdd,0x80)
    if (ValBit(Rdschks10h,5)) RDS_SyndCalcMacro(0x6e,0xc0)
    if (ValBit(Rdschks10h,4)) RDS_SyndCalcMacro(0x80,0x40)
    if (ValBit(Rdschks10h,3)) RDS_SyndCalcMacro(0xf7,0x00)
    if (ValBit(Rdschks10h,2)) RDS_SyndCalcMacro(0x7b,0x80)
    if (ValBit(Rdschks10h,1)) RDS_SyndCalcMacro(0x3d,0xc0)
    if (ValBit(Rdschks10h,0)) RDS_SyndCalcMacro(0xa9,0xc0)
    if (ValBit(Rdschks10l,7)) RDS_SyndCalcMacro(0xe3,0xc0)
    if (ValBit(Rdschks10l,6)) RDS_SyndCalcMacro(0xc6,0xc0)

    rRdsCrch = rCrch;
    rRdsCrcl = rcrcl;
}

/* ----------------------------------------------------------------------------
ROUTINE NAME : RDS_GbsSoftware
INPUT/OUTPUT : None.
DESCRIPTION  : Software GBS routine for RDS synchro and data extraction.
COMMENTS     : 
-----------------------------------------------------------------------------*/
#define SYND_C      0x97                /* The "RDS_SyndTable" element order */
#define SYND_D      0x96                /* is fixed by the hardware RDS GBS  */
#define SYND_A      0xf6                /* BL2:0 bits of the ST7285 for a    */
#define SYND_B      0xf5                /* full compatibility.               */
#define SYND_Cp     0xf3

uchar const	RDSSyndTable[ ] = {SYND_C,SYND_B,SYND_A,SYND_D,SYND_Cp};

void	RDSGbsSoftware( )
{
	uchar	i;
	if(rRdsSyncSt == _BlkSync)	--cRdsCnt;
	if(rRdsSyncSt < _BlkSync || ((rRdsSyncSt == _BlkSync) && (cRdsCnt == 0)))
	{
		cRdsCnt = 26;
		
		RDSSyndCalc( );	/*syndrome computation*/

     	/*check if is valid one	*/	
      	rRdsGbsStat.byte &= 0xe0;	/*rRdsGbsStat.VSI = False;*/
		
	if ((rRdsCrcl & 0xC0)==0) 
	{      /* The two CRC LSB must be zero.*/
		for(i=0; ; )
		{	
			if(rRdsCrch == RDSSyndTable[i])/*counting the block num from SYND CODE
											SYND_A,SYND_B,SYND_C,SYND_D,SYND_C'*/
			{	/*valid*/
				rRdsGbsStat.VSI = True;/*check sum ok and set valid flag true*/
				rRdsGbsStat.byte += i;
				break;
			}
			if(++i>4)	
			{
				rRdsGbsStat.byte += 0x7;
				return;
			}
		}
	}
    	else
    	{
    		rRdsGbsStat.byte += 0x7;
    		return;
    	}

    	if(rRdsSyncSt == _BlkSync && rRdsGbsStat.bit.b6 == rRdsGbsStat.bit.b0
    	   && rRdsGbsStat.bit.b7 == rRdsGbsStat.bit.b1)
    			rRdsGbsStat.ORD = True;
	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_StkPush
INPUT/OUTPUT : None.
DESCRIPTION  : Push BitCnt and Block in corresponding stacks.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDS_StkPush ( ) 
{
  	if (rRdsSyncSt == _BitSync)      /* During the bit synchronisation, the bit */
  	{                                                     /* counter is stored.*/
		cRdsBitStk[2] = cRdsBitStk[1];		/* Push BitCnt in Counter Stack.*/
		cRdsBitStk[1] = cRdsBitStk[0];
		cRdsBitStk[0] = cRdsBits;
  	}
 	rRdsBlkStk[2] = rRdsBlkStk[1];		/* Push Block in Block Stack.*/
	rRdsBlkStk[1] = rRdsBlkStk[0];
	rRdsBlkStk[0] = rRdsBlock;
  	
  	return;
}
	
/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ClearStk {0..2}
INPUT/OUTPUT : None.
DESCRIPTION  : Clear data, count and block stacks w/ EMPTY blocks.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDSClearStk0 ( ) 
{ /* Clear level 0, 1 & 2...................*/
  	rRdsBlkStk[0] = _Empty;            /* Clear last detected block information.*/
  	cRdsBitStk[0] = 0;
}

void RDSClearStk1 ( ) 
{  /* Clear level 1 & 2.....................*/
  	rRdsBlkStk[1] = _Empty;        /* Clear previous detected block information.*/
  	cRdsBitStk[1] = 0;
}

void RDSClearStk2 ( ) 
{  /* Clear level 2.........................*/
  	rRdsBlkStk[2] = _Empty;      /* Clear third last detected block information.*/
  	cRdsBitStk[2] = 0;
  	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DistOver01
INPUT/OUTPUT : None.
DESCRIPTION  : Bit counter overflow treatment, the bit distance between current
               and level 1 block is upper than 2 groups.
COMMENTS     : Only the current block is kept.
-----------------------------------------------------------------------------*/ 
extern	void RDSDistOver01 ( );

void RDSDistOver01 ( ) 
{
  	cRdsBits=0;      /* Reset bit counter to restart a detection on 2 groups.*/
  	cRdsBitStk[0]=0;                   /* Current block become the first block.*/
  	
  	RDSClearStk1( );    /* Clear level 1 and 2 of the cnt, blk and data stacks.*/
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DistOver02
INPUT/OUTPUT : None.
DESCRIPTION  : Bit counter overflow treatment, the bit distance between current
               and level 2 block is upper than 2 groups.
COMMENTS     : The level 2 block is reseted.
-----------------------------------------------------------------------------*/ 
extern	void RDSDistOver02 ( );
void RDSDistOver02 ( ) 
{
  	cRdsBits -= cRdsBitStk[1];	/* Bit counter is set on the level 1.*/
  	cRdsBitStk[0] = cRdsBits;      /* The current block become the 2sd block.*/
  	cRdsBitStk[1] = 0;        /* Level 1 block become the first detected block.*/
  	
  	RDSClearStk2( );          /* Clear level 2 of the cnt, blk and data stacks.*/
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_CheckOrder
INPUT/OUTPUT : Index of the last block to check=rds_idxprm,
               return 1 if the dist and the order are correct, 0 else.
DESCRIPTION  : Check if the distance and order between two blocks is correct.
COMMENTS     :     
-----------------------------------------------------------------------------*/ 
uchar	RDSCheckOrder (uchar rds_idxprm) 
{	
  	uchar rds_bitdist, rds_blkdist = 0;

	rds_bitdist = cRdsBits - cRdsBitStk[rds_idxprm]; /* Distance calculation.*/
	
	/* Check if the distance is a 26 multiple .................................*/
	while (rds_bitdist >= _BlkLength)
	{      /* Continue if the distance >= 26 */
		rds_blkdist++;
		rds_bitdist = rds_bitdist - _BlkLength;
	}
	if (rds_bitdist != 0) return(0);       /* The bit distance is not correct! */
	
	/* Check if the order between two blks is correct..........................*/
	return( !(((rRdsBlkStk[rds_idxprm]-rds_blkdist) ^ rRdsBlock) & 0x03) );
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_BlkSync
INPUT/OUTPUT : None.
DESCRIPTION  : Block synchronization achieved.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void 	RDSBlkSync( ) 
{
  	/* SOFTWARE BLK SYNC init............................................*/
  	/* CNA with 26 for BIT_SYNC mode.                                          */
  	cRdsCnt = _BlkLength;        /* Set counter for BLK_SYNC mode (26 bits).   */
 
   	/*cRdsBlks = rRdsGbsStat & 0x3;*/
  	
  	/* RDS DATA EXTRACT FROM CURRENT BLOCK.....................................*/
  	rRdsExtStat.byte = 0;              /*Data init at the begining of a new group.*/
  	rRdsCurSR2 = (uchar)(rRdsSR>>16); //rRdsSR[2];          /*Save the current RDS data info from the */ 
  	rRdsCurSR3 = (uchar)(rRdsSR>>24); //rRdsSR[3];         /*input data shift register           */ 

  	/* RDS SYNCHRO UPDATE .....................................................*/
  	rRdsSyncSt = _BlkSync;        /* From now, the block synchro is reached.*/
  	rRdsSyncLevel = _FirstSyncLevel;      /*height of the 1st blk synchro is 3.*/

	rRdsGbsStat.BlkSync = True;
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DataClear
INPUT/OUTPUT : None.
DESCRIPTION  : RDS Data clearing.
COMMENTS     :
-----------------------------------------------------------------------------*/ 
void RDSDataClear( )
{
  	/* RDS EXTRACTION CONTROL MANAGEMENT.......................................*/
  	rRdsGroup = 0xff;                  /* Current extracted group code reset.*/
  	rRdsExtStat.byte = 0;   /* RDS Data extraction general ctrl register cleared.*/
  	rRdsSyncLevel = 0;
  	
  	/* STORED RDS DATA INFORMATION INITIALIZATION..............................*/
	bRdsPiCtrl = False;
	
  	/*rRdsPsStat.byte = 0;*/
  	if(RdsReceive_X32ms==0)
  	{
		uchar j;
		rRdsTaTpStat.byte = 0;     /* RDS TA and TP info status/control reg. cleared.*/
		rRdsPtyStat.byte = 0;     /* RDS PTY information status/control reg.  reset.*/
		rRdsStoPty = 0;
		for(j=0;j<8;j++)
			rRdsPsNameSto[j] = rRdsPsNameAct[j] = ' ';
		rRdsStoPty = 0;
		rRdsStoPtyTemp = 0;

		for(j=0;j<16;j++)
		{
			rRdsRTNameSto[4*j] = rRdsRTNameAct[4*j] = ' ';
			rRdsRTNameSto[4*j+1] = rRdsRTNameAct[4*j+1] = ' ';
			rRdsRTNameSto[4*j+2] = rRdsRTNameAct[4*j+2] = ' ';
			rRdsRTNameSto[4*j+3] = rRdsRTNameAct[4*j+3] = ' ';
		}

		RDSTxPTYType();
		RDSTxStatRT();
	#if _DebugPTY
		Printf("\r\n No rds data,PTY TA TP clr!:rPtyType=%x,rRdsStoPty=%x,%x",rPtyType,rRdsStoPty,rRdsCurSR32.word);
	#endif	
	}
  	bRdsEONAvail = False;
  	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_BlkOk
INPUT/OUTPUT : None
DESCRIPTION  : Valid block synchronisation.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDS_BlkOk( ) 
{
	/* BLOCK SYNCHRO CRITERA MANAGEMENT........................................*/
	if (rRdsSyncLevel < 52)
	++rRdsSyncLevel;
	/* EXTRACT RDS DATA .......................................................*/
	rRdsCurSR2 = (uchar)(rRdsSR>>16); //rRdsSR[2];           /*Save the current RDS data info from the */ 
	rRdsCurSR3 = (uchar)(rRdsSR>>24); //rRdsSR[3];               /* shift register of the RDS GBS block.*/ 
 	
	/* EXTRACT RDS DATA .......................................................*/
	rRdsGbsStat.BlkSync = True;          /* Extract RDS data info from the current block.*/
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_BlkFail
INPUT/OUTPUT : None
DESCRIPTION  : Failed block synchronization.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDS_BlkFail ( ) 
{
 	 /* BLOCK SYNCHRO CRITERA MANAGEMENT........................................*/
	//rRdsGroup = 0xff;                  /* Current extracted group code reset.*/
	//rRdsExtStat.byte = 0;   /* RDS Data extraction general ctrl register cleared.*/
 	 if (rRdsSyncLevel > 0)
 	      --rRdsSyncLevel;

 	 if(rRdsSyncLevel == 0)
 	 { /* Block synchro lost => Restart in LEAD_IN mode.*/
 	   	/*cRdsCnt = 26;*/
 	   	rRdsSyncSt = _LeadIn;         /* Start state of the synchro state machine.*/
 	   //	RDSDataClear( );              /* RDS dynamic data initialization.*/
 	   	/*RdsSync_X3ms = 200/3;*/	
 	 }
 	 
/* 	 if (rRdsTaTpStat.byte&0x07)    /* The previous TP was valid.*/
/*  	    	--rRdsTaTpStat.byte;      /* The data become not valid.*/
/*  	 else	
/*  	 {
 	 	bRdsTPPreVal = False;
  	 	bUpdateDisp = True;
  	 }       
*/ 	 

 	 return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_SyncStateBit
INPUT/OUTPUT : None.
DESCRIPTION  : BIT SYNC Mode. This function is called each RDS clock during
               this mode. A software counter is incremented each call and is
               the base of the bit synchro calculation.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void	RDSSyncStateBit( ) 
{
  	++cRdsBits;                            /* Software bit counter increment.*/
  	rRdsBlock = rRdsGbsStat.byte & 0x07;  /* Extract block code from control/status reg.*/
  	
  	/*::::::::::::::::::::::: NO VALID SYNDROME FOUND :::::::::::::::::::::::::*/
  	if ( (rRdsBlock==_Wrong) || (rRdsBlock==_BlockE) ) 
  	{  	
  	  	/* Current block checking validity.*/
  	  	/* Check if the distance with the last right block is less than 2 groups.*/
  	  	if ((cRdsBits - cRdsBitStk[0]) > _BlkWindows)
  	  	  	RDSDistOver01();
  	  	return;
  	}
  
  	/*::::::::::::::::::::::::: VALID SYNDROME DETECTED :::::::::::::::::::::::*/
  	RDS_StkPush();           /* Store BitCnt and Block in corresponding stacks.*/  
  	/*................. Check with the very last block detected ...............*/
  	if (rRdsBlkStk[1] == _Empty) 
  	{       /* There is no previous detected block.*/
  	  	cRdsBits=0;        /* Reset bit counter to start a detection on 2 grps.*/
  	  	cRdsBitStk[0]=0;                 /* Current block become the first block.*/
  	  	return;
  	}
  	
  	if (RDSCheckOrder(1)) 
  	{ /* Check the BIT DISTANCE and BLOCK ORDER between */
  	  	RDSBlkSync( );          /* current and level 1 detected block. Set block */
  	  	return;                        /* index to be check with the current one.*/
  	}
  	if ((cRdsBits-cRdsBitStk[1]) > _BlkWindows) 
  	{      /* Check BIT DISTANCE */
  	  	RDSDistOver01( );                               /* between last 2 blocks.*/
  	  	return;            /* Reset the level 1&2 stack, keep only current block.*/
  	}
  	/*................. Check with the older saved block detected .............*/
  	if (rRdsBlkStk[2] == _Empty)         /* There is no previous detected block.*/
	{
  	  return;
  	} 
  	if (RDSCheckOrder(2)) 
  	{ /* Check the BIT DISTANCE and BLOCK ORDER between */
  	  	RDSBlkSync( );          /* current and level 2 detected block. Set block */
  	  	return;                        /* index to be check with the current one.*/
  	}
  	if ((cRdsBits-cRdsBitStk[2]) > _BlkWindows) 
  	{      /* Check BIT DISTANCE */
  	  	RDSDistOver02( );                      /* between current and 2sd blocks.*/
  	  	return;               /* Reset the stack level 2, keep stack level 0 & 1.*/
  	}
  	
  	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_SyncStateBlk
INPUT/OUTPUT : None
DESCRIPTION  : BLK_SYNC Mode : Block synchronization.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDSSyncStateBlk ( ) 
{
  	rRdsBlock = rRdsGbsStat.byte & 0x07;  /* Extract block code from control/status reg.*/
  	
  	/*::::::::::::::::::::::::::: GROUP SYNCHRO CHECK :::::::::::::::::::::::::*/
/*  	++cRdsBlks;                          /* Software block counter increment.*/
/*  	if (cRdsBlks == 4)                           /* Begining of a new group.*/
/*  	  cRdsBlks = 0;              /* Group synchro: Counter overflow 1 groups.*/
/*  	
/*  	if (cRdsBlks == 2)                           /* Begining of a new group. */
/*  	    rRdsExtStat.byte = 0;           /* Data init begin new blk A               .*/
  	
  	
  	/*::::::::::::::::::::::::::: BLOCK SYNCHRO CRITERA :::::::::::::::::::::::*/
  	RDS_StkPush();                        /* Push in stack block identificator.*/
  	
  	if (rRdsGbsStat.VSI && rRdsGbsStat.ORD)	
  		RDS_BlkOk( );                           /* Valid block synchronization.*/
  	
  	else     RDS_BlkFail( ); /* Wrong syndrome or bad order => Failed block synchro.*/
  		
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_GbsSynchro
INPUT/OUTPUT : None
DESCRIPTION  : RDS interrupt service.
COMMENTS     : Comes from Group & Block Synch cell.
-----------------------------------------------------------------------------*/ 
void	RDSGbsSynchro( )
{
	if(cRdsCnt==26)
	{
		switch(rRdsSyncSt)
		{
		  	case _LeadIn:	/*First RDS block detection*/
		  	
				/* SYNCHRO VARIABLES INIT .................................................*/
				cRdsBits = 0;                            /* Software bit counter reset.  */
				/* cRdsBlks = 0;                          /* Software block counter reset  .*/
				rRdsSyncLevel = 0;                          /* Block Synchro level reseted.*/

				RDSClearStk0();                   /* Clear data, counter and block stacks.*/
				rRdsBlock = _Empty;                                  /* Default Empty block.*/
		      	
				/* UPDATE THE SYNCHRO STATE ...............................................*/
				rRdsSyncSt = _BitSync;                   /* Next interrupt in BIT_SYNC mode.*/		 
				break;

			case _BitSync:	/*Bit synchronization*/
		
				RDSSyncStateBit( );
				break;
		     
			case _BlkSync:	/*synchronization*/
				RDSSyncStateBlk( );
				break;
		}	
	}	    	
	return;		    	
}		    
		
/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_Start
INPUT/OUTPUT : None.
DESCRIPTION  : RDS feature initialisation for a new synchronization.
COMMENTS     : This function can be used to force a new synchronization.
-----------------------------------------------------------------------------*/ 
void RDSStart( ) 
{
  	rRdsSyncSt = _LeadIn;           /* Start state of the synchro state machine.*/
  	
  	/* RDS DATA INIT ..........................................................*/
  	/*RDS_SwafTmpPiClear();                      /* Temp. PI for AF switch reset.*/
  	rRdsPiStat.byte = 0;    /* Force the RDS PI code to be not available.*/
  	rRdsSyncLevel = 0;                          /* Block Synchro level reseted.*/
  	
  	//rRdsChks10l=0x0;      /*initialization -- only test */
  	//rRdsChks10h=0x0;
  	//rRdsData16l=0x0;
  	//rRdsData16h=0x0;
  	rRdsSR = 0;
	
  	cRdsCnt=26;
  	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_Stop
INPUT/OUTPUT : None.
DESCRIPTION  : RDS feature temporary stopped.
COMMENTS     : Lost of the synchronisation.
-----------------------------------------------------------------------------*/ 
void RDSStop( ) 
{
  	rRdsSyncSt = _LeadIn;           /* Start state of the synchro state machine.*/
  	rRdsSyncLevel = 0;                          /* Block Synchro level reseted.*/
	return;
}



/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafFmInsert
INPUT/OUTPUT : Freq. to insert (aftoinsert) / None.
DESCRIPTION  : Insert a freq. in the dynamic FM AF list.
COMMENTS     : TOO LARGE CODE !!!  bytes...
-----------------------------------------------------------------------------*/ 
void	RDSInsertAF(uchar rfreq,uchar d)
{	
	uchar	i,j;
	rRdsAFStatBak = rfreq;

	#if _DebugInsetStat
		if(rfreq == 0)
		{
			Printf("\r\n Af err Ins: %2x,%2x",rRdsAFStatBak,d);
		}
		else
			Printf("\r\n Af Ins: %2x,%2x",rRdsAFStatBak,d);
	#endif		
	for(i=0;i<rRdsTotalAfNb;++i)    /* For all AF in dynamic AF memory.*/
	{
		if(rRdsDafFmFreq[i] == rfreq)     /* Freq. already in the list.*/
		{	
			rRdsDafFmFreqCnt[i] += 1;
			break;
		}
		if(rRdsDafFmFreq[i] == _FillerCode)          /* End of AF list detected.*/
		{
			++rRdsRevdAfNum;
			if(bRdsMethDet && bRdsMethB)	++rRdsRevdAfNum;
			rRdsDafFmFreq[i] = rfreq;	/* Add a new AF in the DAF list.*/
			rRdsDafFmFreqCnt[i] += 1;
	#if _DebugInsetStat
		if(rfreq == 0)
		{
			Printf("\r\n Af err Ins: %2x",rfreq);
		}
		else
			Printf("\r\n Af Ins: %2x",rfreq);
	#endif		
	
		#ifndef	_OnlyReduceRAM
			j = cPSNum + cBand*7;
		#else
			j = 0;
		#endif
			RDSPreMem[j].RDSAf[i] = rfreq;
			break;
		}
	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafExtHeader
INPUT/OUTPUT : None.
DESCRIPTION  : RDS AF header data extraction routine.
COMMENTS     : TOO LARGE CODE !!!
-----------------------------------------------------------------------------*/ 
void RDSExtAfHeader( )
{
	/* Update of the Backup critera of the dynamic FM AF list.*/
  	/*HL = rRdsDafFmCtrl;
  	/*for(C =0;C<_MaxAfList; )
  	/*{	
  	/*	A = *HL;
  	/*	A.3 = False;		/* AF becomes not received.*/
  	/*	*(HL++) = A;
  	/*	++C;
  	/*}*/
  	
	rRdsAfCtrl1.byte &= 0x80;
	/* Total AF nb storing (in the list).i.e. rRdsCurAfNb*/
	rRdsAfCtrl1.byte += (rRdsCurSR3 - 224);
	
#if _DebugInsetStat
	Printf("\r\n Af Total: %d",rRdsTotalAfNb);
	// UARTOutputChar( ConvNumToASCII(rRdsTotalAfNb >> 4) );
	// UARTOutputChar( ConvNumToASCII(rRdsTotalAfNb & 0x0F) );
	Printf("\r\n . \n");	
#endif
	if(rRdsTotalAfNb == 0)
	{
		rRdsRevdAfNum = 0;
		return;
	}
  	rRdsAfFirst = rRdsCurSR2;		/* First AF storing to detect the method.*/
  	RDSInsertAF(rRdsAfFirst,1);	/* Store the 1st AF of AF list no matter which method is used.*/
  	rRdsRevdAfNum = 1;   			/* Anticipate first AF storing in the dynamic list.*/

	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafInitHeader
INPUT/OUTPUT : None
DESCRIPTION  : Initialisation of the AF control register for a next new list.
COMMENTS     : TOO LARGE CODE !!! 
-----------------------------------------------------------------------------*/ 
void RDSAfInitHeader( )
{
  	rRdsAfCtrl1.byte = 0;                 /* Method A, no current AF list.*/
  	rRdsRevdAfNum = 0;                /* No AF stored from the current list.*/
  	
  	rRdsAfFirst = _FillerCode;       /* First AF disable: no freq.*/
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafRecuUpdate
INPUT/OUTPUT : None.
DESCRIPTION  : Update of the RDS AF reception control of the current AF list.
               (if not received then backup level decrement)
COMMENTS     : TOO LARGE CODE !!!
-----------------------------------------------------------------------------*/ 
//extern	void RDSDafRecuUpdate( );
//void RDSDafRecuUpdate( )
//{	
//  	/* Update of the Backup critera of the dynamic FM AF list.*/
//  	rRdsStorAfNb = rRdsTotalAfNb;
//
//  	/*HL = rRdsDafFmCtrl;*/
//  	DE = rRdsDafFmFreq;
//  	for (C=0;C<B; )		  
//  	{
//  	  	A = *HL;
//  	  	if (A.3 == False)    /* AF not received in list.*/
//  	    {
//  	    	if (*HL&0x07)           /* Backup not yet minimum.*/
//  	      		/* Backup critera decrement.*/
//  	      		--(*HL);
//  	      	else
//  	      	{
//  	      		*DE = _FillerCode;	/* */	
//  	      	}
//  		}
//  		++C;
//  		++HL;
//  		++DE;
//  	}
//  	return;
//}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafExtMethodA
INPUT/OUTPUT : None.
DESCRIPTION  : RDS AF data extraction with method A.
COMMENTS     : TOO LARGE CODE !!!
-----------------------------------------------------------------------------*/ 
void	RDSExtAfMethodA( )
{
	uchar	i,j;
	
  	RDSInsertAF(rRdsCurSR3,2);            /* Store 1st AF of current block.*/
  	++rRdsRevdAfNum;         /* One more AF of the current list was stored.*/
  	
  	if (rRdsRevdAfNum < rRdsTotalAfNb)/* AF nb to be received not reached.*/
  	{
  	  	RDSInsertAF(rRdsCurSR2,3);          /* Store 2sd AF of current block.*/
  	  	++rRdsRevdAfNum;       /* One more AF of the current list was stored.*/
  	  	
  	  	if (rRdsRevdAfNum < rRdsTotalAfNb)      /* AF nb to be received not */
  	  	  return;                      /* reached => Wait next AF to be received.*/
  	}
	rRdsStorAfNb = rRdsTotalAfNb;
  	if(!bRdsAfAvail)
	{
		bRdsAfAvail = True;
		i = cPSNum + cBand*7;
		if(rRdsStorAfNb>_AfMemLength || rRdsStorAfNb==0)	j = _AfMemLength;
		else	j = rRdsStorAfNb;
		for(j = 0; j<_AfMemLength; ++j)
		{
			RDSPreMem[i].RDSAf[j] = rRdsDafFmFreq[j];
		}
		//bWrRdsAf = True;
		bMeasureAfSig = True;
	}
  	/*RDSDafRecuUpdate( );		/* If AF not received then Backup level decrement.*/
  	RDSAfInitHeader( );		/* Header control getting ready for next AF list.*/
  	/* End of an AF list.*/
  	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafExtMethodB
INPUT/OUTPUT : None.
DESCRIPTION  : RDS AF data extraction with method B.
COMMENTS     : TOO LARGE CODE !!!
-----------------------------------------------------------------------------*/ 
void	RDSExtAfMethodB( )
{
	uchar	i,j;
	
  	if (rRdsCurSR3 == rRdsAfFirst)     /* 1st current block AF is tuned freq.*/
  	{
  	  	RDSInsertAF(rRdsCurSR2,4);         /* Store the 2sd current block AF.*/
  	  	//rRdsRevdAfNum += 2;         /* One more AF of current list was stored.*/
  	  
  	  	if (rRdsRevdAfNum >= rRdsTotalAfNb)		/* Received a complete AF list */
  	  	{
			rRdsStorAfNb = rRdsTotalAfNb;
  	  		rRdsStorAfNb >>= 1;
  	  		if(!bRdsAfAvail)
			{
				bRdsAfAvail = True;
				i = cPSNum + cBand*7;
				if(rRdsStorAfNb>_AfMemLength || rRdsStorAfNb==0)	j = _AfMemLength;
				else	j = rRdsStorAfNb;
				for(j = 0; j<_AfMemLength; ++j)
				{
					RDSPreMem[i].RDSAf[j] = rRdsDafFmFreq[j];
				}
				//bWrRdsAf = True;
				bMeasureAfSig = True;
			}
  	  	  	RDSAfInitHeader( );  /* Header control getting ready for next AF list.*/
  	  	  	bRdsMethB = True;              /* Set method B in the method flag.*/
  	  	}
  	  	return;
  	}
  	
  	if (rRdsCurSR2 == rRdsAfFirst)     /* 2sd current block AF is tuned freq.*/
  	{
  	  	RDSInsertAF(rRdsCurSR3,5);         /* Store the 1st current block AF.*/
  	  	//rRdsRevdAfNum += 2;         /* One more AF of current list was stored.*/
  	  	
  	  	if (rRdsRevdAfNum >= rRdsTotalAfNb)	/* Received a complete AF list */
  	  	{
  	  		if(!bRdsAfAvail)
			{
				rRdsStorAfNb = rRdsTotalAfNb;
				rRdsStorAfNb >>= 1;
				bRdsAfAvail = True;
				i = cPSNum + cBand*7;
				if(rRdsStorAfNb>_AfMemLength || rRdsStorAfNb==0)	j = _AfMemLength;
				else	j = rRdsStorAfNb;
				for(j = 0; j<_AfMemLength; ++j)
				{
					RDSPreMem[i].RDSAf[j] = rRdsDafFmFreq[j];
				}
				//bWrRdsAf = True;
				bMeasureAfSig = True;
			}
  	  	  	RDSAfInitHeader( );  /* Header control getting ready for next AF list.*/
  	  	  	bRdsMethB = True;              /* Set method B in the method flag.*/
  	  	}
  	  	return;
  	}
  	
  	/* AF list corrupted => Wait next AF list header.*/
  	
  	RDSAfInitHeader( );      /* Header control getting ready for next AF list.*/
  	bRdsMethB = True;                  /* Set method B in the method flag.*/
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafExt
INPUT/OUTPUT : None.
DESCRIPTION  : RDS AF data extraction main routine.
COMMENTS     : TOO LARGE CODE !!!
-----------------------------------------------------------------------------*/ 
void	RDSExtAf( )
{
	if(rRadMainSt!=_RadStdby && rRadMainSt!=_RadPS)	return;
	if(/*rRdsMainSt == _Traffic ||*/ bInvalidPi)	return;
  	/* AF list extraction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  	if (rRdsRevdAfNum && rRdsTotalAfNb)                         /* AF header already received.*/
  	{
  	  	if (!bRdsMethDet)         /* Extraction method not already detected.*/
  	  	{
  	  		if(rRdsTotalAfNb<3)
  	  		{
  	  	    	RDSExtAfMethodA( );    /* New AFs will be extracted with Method A.*/
  	  	    	return;
  	  	    }
  	  	  	if ( (rRdsCurSR3 < 205) && (rRdsCurSR2 < 205) )
  	  	  		/* More than one AF in current list & current block contains new FM AF.*/
  	  	  	{  	
  	  	  		bRdsMethDet = True;                      /* Current Method detected.*/  	  	    	
  	  	  		
  	  	    	if ( (rRdsCurSR3 == rRdsAfFirst) || (rRdsCurSR2 == rRdsAfFirst) )
  	  	    	{
  	  	    	/* Method B detected.*/
  	  	    		bRdsMethB = True;          /* Set method B in the method flag.*/
  	  	    		RDSExtAfMethodB( );    /* New AFs will be extracted with Method B.*/
  	  	    	}
  	  	    	else
  	  	    	/* Method A detected.*/
  	  	    	{
  	  	    		if (bRdsMethB) return;
  	  	    		/*RDSDafFmInsert(rRdsAfFirst);       /* Store the 1st AF of AF list.*/
  	  	    		RDSExtAfMethodA( );    /* New AFs will be extracted with Method A.*/
  	  	    	}
  	  	    	return;
  	  	  	}
  	  	}
  	  	else      /* More than one AF were already received for the current list.*/
  	  	{
  	  	  	if (rRdsCurSR3 < 205)           /* Current block contains new FM AFs.*/
  	  	  	{
  	  	  	  	if (bRdsMethB)
  	  	  	  	/* Method B detected.*/
  	  	  	  	{
  	  	  	  	  	RDSExtAfMethodB( );    /* New AFs will be extracted with Method B.*/
  	  	  	  	}
  	  	  	  	else
  	  	  	  	/* Method A detected.*/
  	  	  	  	{
  	  	  	  	  	RDSExtAfMethodA( );    /* New AFs will be extracted with Method A.*/
  	  	  	  	}
  	  	  	  	return;
  	  	  	}
  	  	}
  	}
  	/* AF list Header detection ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  	if ( (rRdsCurSR3 > 223) && (rRdsCurSR3 < 250) )     /* Header detected.*/
  	{
  		RDSExtAfHeader( );                 /* New header control initialisation.*/
  		return;
	}
  	
  	
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_DafInit
INPUT/OUTPUT : None
DESCRIPTION  : Initialisation of the dynamic AF data.
COMMENTS     : TOO LARGE CODE !!! 
-----------------------------------------------------------------------------*/ 
void RDSAfInit( )
{
	uchar	i;
	
	RDSAfInitHeader( );      /* AF control register init. for a next new list.*/
	rRdsStorAfNb = 0;
	
	/* FM Dynamic AFs initialisation.*/
	/*DE = rRdsDafFmCtrl;*/
	for(i=0; i<_MaxAfList;++i)         /* All Dynamic AF memory init.*/
	{
		rRdsDafFmFreq[i] = _FillerCode;
		rRdsDafFmFreqCnt[i] = 0;
	}
	return;
}
		

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtPty
INPUT/OUTPUT : None
DESCRIPTION  : The following algorithm manages the PTY flag extraction. To
               avoid any dummy received PTY code value, this algo change the
               stored PTY value only if 2 consecutive same PTY values are
               received.
COMMENTS     : Change flag is reset at the end of a user PTY value reading.    
-----------------------------------------------------------------------------*/ 
void RDSExtPty( )
{
	uchar	rptycurval;		/* RDS current PTY code information.*/
	
	if(bInvalidPi || bEonAfDet)	return;
	#if _EnableRadSi4730
	rptycurval = (rRdsCurSR32.word>>5) & 0x1f;
	#else
	rptycurval = (rRdsCurSR2 >> 5) | ((rRdsCurSR3 << 3) & 0x18);
	#endif
	if(rptycurval == RdsPtyValue)	/* Stored & Current PTY are the same.*/
	{
		rRdsStoPty = RdsPtyValue;
		if(bRdsPtyChange)
		{/* display new pty code */
			//bUpdateDisp = True;
			bRdsPtyChange = False;
			//bMsgPtyUpDate = 1;
			RDSTxPTYType();
	#if _DebugPTY
			Printf("\r\n PTY Chg!:rPtyType=%2x,rRdsStoPty=%2x,%4x",rPtyType,rRdsStoPty,rRdsCurSR32.word);
	#endif			  				
		}
		bRdsPtyCtrl = True;
		if(rptycurval == _Alarm)
		{/* Alarm anouce On */
			if(!bPtyAlarm && !bUserTaDisable && rRdsMainSt!=_Traffic)
			{/* if alarm information is released, enter traffic status */
				bPtyAlarm = True;
				rRdsMainSt = _Traffic;	/* Switch to traffic mode */
				rTrafficSt = _TrafficInit;
				RdsTraffic_X64ms = 3000/64;
			}
			else
			{
				bPtyAlarm = True;
				RdsTraffic_X64ms = 3000/64;
			}
		}
		else	if(bPtyAlarm)
		{/* Alarm anounce end */
			bPtyAlarm = False;
			//RdsTraffic_X64ms = 0000/64;
		}
		if(RdsPtyValue==rPtyType)
		{
			RdsPty_X128ms = 2000/128;
			if(rRdsMainSt==_PtyOn)	bPtyChged = False;
		}

	}
  	else                    /* Stored & Current PTY are different.*/
  	{
  		if(bRdsPtyCtrl)	
  		{

			 if(rRdsStoPtyTemp != rptycurval)
			 {
			 	rRdsStoPtyTemp = 0;
				bRdsPtyCtrl = False;
		#if	_DebugPTY
				Printf("\r\n PTY chg fail,%2x,%2x",rRdsStoPtyTemp,rptycurval);
		#endif	  								
			 }
			else  if(!RdsPty_X128ms )
  			{
				bRdsPtyCtrl = False;
				rRdsPtyStat.byte &= 0xe0;
  				rRdsPtyStat.byte += rptycurval;	 	/* New PTY value stored.*/  
  				bRdsPtyChange = True;	/* There is a PTY value change.*/
			}
  		}
  		else
  		{
  			if(RdsReceive_X32ms > 500/32)
  			{
  				bRdsPtyCtrl = True;
				rRdsStoPtyTemp = rptycurval;
				RdsPty_X128ms = 2000/128;
		#if	_DebugPTY
				Printf("\r\n PTY is:rRdsStoPtyTemp=%d,rRdsStoPty=%d",rRdsStoPtyTemp,rRdsStoPty);
		#endif	  								
  			}
	 	}
	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtTp
INPUT/OUTPUT : None
DESCRIPTION  : The following algorithm manages the TP flag extraction. To
               avoid any dummy received TP flag value, this algo change the
               stored TP value only if 7 consecutive same TP values are
               received.(7*86.7ms = 600ms)
COMMENTS     : Change flag is reset at the end of the user TP value reading.    
-----------------------------------------------------------------------------*/ 
void RDSExtTp( )
{	
	if(bInvalidPi)	return;
	
  	if (bRdsTPCurVal == bRdsTPPreVal)    /* Stored & Current TP are the same.*/
  	{
  		bRdsTPCtrl = True; 	/* 2 consecutive same TP => Data OK.*/
  		if(cRdsTpCnt<3)	++cRdsTpCnt;
		else	if(bRdsTPPreVal)
		{
			RdsTpSeek_X500ms = 90000/500;/*90s timer*/
			bTpFlag = True;
			//bTpStat = 1;
		}
	}
	else                                  /* Stored & Current TP are different.*/
	{  
		if (bRdsTPCtrl)    /* The previous TP was valid.*/
		{
			bRdsTPCtrl = False;      /* The data become not valid.*/
			cRdsTpCnt = 0;
		}
  	  	else     /* The previous TP was not valid => current TP and previous one */
  	  	{           /* are different but the current one is stored and set valid.*/
  	    	bRdsTPPreVal = bRdsTPCurVal;   /* New TP stored.*/
  	    	bRdsTPCtrl = True;  /* New stored data becomes valid.*/
  	    	bRdsTATPNew = True;    /* There is a TP value change.*/
  	    	//bUpdateDisp = True;
			cRdsTpCnt = 0;
			//bTpStat = 0;
	#if	_DebugTPTA
			if(bRdsTPCurVal)	Printf("\r\n TP Preset. \n");
			else 	Printf("\r\n TP Absent. \n");
	#endif
  	  	}
	}            
	return;
}

/*{
/*  	if (bRdsTPCurVal == bRdsTPPreVal)    /* Stored & Current TP are the same.*/  		
/*  		rRdsTaTpStat.byte &= 0xf8;	/* 7 consecutive same TP => Data OK.*/
/*    		 				  	  		
/*	else
/*	{
/*		if((rRdsTaTpStat.byte&0x07) < 0x07)	++rRdsTaTpStat.byte;
/*		else
/*		{	/* 7 consecutive no same TP => Data change.*/
/*			bRdsTPPreVal = bRdsTPCurVal;   /* New TP stored.*/
/*			bRdsTATPNew = True;    /* There is a TP value change.*/
/*			bUpdateDisp = True;
/*			rRdsTaTpStat.byte &= 0xf8;
/*		}
/*	}
/*	return;
/*}			
/*
/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtTa
INPUT/OUTPUT : None
DESCRIPTION  : The following algorithm manages the TA flag extraction. To
               avoid any dummy received TA flag value, this algo change the
               stored TA value only if 2 consecutive same TA values are
               received.
COMMENTS     : Change flag is reset at the end of the user TA value reading.    
-----------------------------------------------------------------------------*/ 
void RDSExtTa ( )
{
  	if (bRdsTACurVal == bRdsTAPreVal)    /* Stored & Current TA are the same.*/
  	{
  		bRdsTACtrl = True;   /* 2 consecutive same TA =>DataOK.*/
		if(bRdsTAPreVal)
		{/*TA flag on*/
			bTpFlag = True;
			RdsTpSeek_X500ms = 90000/500;
			if(!bTrafficOn)
			{
				if(bRdsTAEn && bRdsTPPreVal /*&& bStatPresent*/ && (rRadMainSt!=_AutoTUp))	bTrafficOn = True;
				if(bTrafficOn && !bUserTaDisable && rRdsMainSt != _Traffic)
				{/* if TA enabled, enter traffic status */
					rRdsMainSt = _Traffic;
					rTrafficSt = _TrafficInit;
					RdsTraffic_X64ms = 3000/64;
				}
			}
			else
			{
				bTrafficOn = True;
				if(bRdsTAEn)	RdsTraffic_X64ms = 3000/64;	/*Delay 3s to exit from traffic anouce mode */
			}
		}
		else	if(bTrafficOn || bEonTrafficOn)
		{
			bTrafficOn = False;
			bEonTrafficOn = False;
			//RdsTraffic_X64ms = 0000/64;
		}
  	}
  	else
  	{                                  /* Stored & Current TA are different.*/
		if (bRdsTACtrl)	 /* The previous TA was valid.*/
  	  	   bRdsTACtrl = False;     /* The data become not valid.*/
  	  	else     /* The previous TA was not valid => current TA and previous one */
  	  	{           /* are different but the current one is stored and set valid.*/
			bRdsTAPreVal = bRdsTACurVal;   /* New TA stored.*/
			bRdsTACtrl = True;  /* New stored data becomes valid.*/
			bRdsTATPNew = True;    /* There is a TA value change.*/
			/*if(bRdsTAPreVal && RdsTraffic_X64ms==0)
			/*{/*TA ON*/
			/*	if(!bEonTrafficOn && bRdsTPPreVal)
			/*	{/* Both TP and TA are set to 1 */
			/*		bTrafficOn = True;
			/*		if(bRdsTAEn)
			/*		{/* if TA enabled, enter traffic status */
			/*			RdsTraffic_X64ms = 3000/48;
			/*			rRdsMainSt = _Traffic;
			/*			rTrafficSt = _TrafficInit;
			/*		}
			/*	}
			/*}*/
	#if	_DebugTPTA
			if(bRdsTACurVal)	Printf("\r\n TA Preset. \n");
			else 	Printf("\r\n TA Absent. \n");
	#endif			
		}
	}
  	return;
}


/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtPs
INPUT/OUTPUT : None.
DESCRIPTION  : Extract PS information.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDSExtPs ( )
{
	uchar rPsFlg, rPsSeg;

	if(rRadMainSt!=_RadStdby && rRadMainSt!=_RadPS && rRadMainSt!=_RadScan || bEonAfDet)	return;

	/*if(rRdsMainSt == _Traffic && bEonTrafficOn)	return;*/

	if(bInvalidPi /*|| rRdsAfSt != 0*/)		return;	/* if not in stable state */

	rPsSeg = (rRdsPsStat.byte&0x03) + 1;/* PS index calculation.*/
	rPsFlg = 0x08;
	for ( ;rPsSeg>0; )
	{
		rPsFlg <<= 1;
		--rPsSeg;
	}
	rPsSeg = (rRdsPsStat.byte&0x03) << 1;
	if ((rRdsPsNameAct[rPsSeg]   != rRdsCurSR3)      /* Different new and */
	  || (rRdsPsNameAct[rPsSeg+1] != rRdsCurSR2))        /* previous char.*/
	{
		if (rRdsPsStat.byte & rPsFlg)     /* PS segment Already received.*/
	    	rRdsPsStat.byte &= 0x0F;       /* This is the 1st reception for a new PS.*/
	                                    /* The active PS becomes not available.*/
		rRdsPsNameAct[rPsSeg]   = rRdsCurSR3;         /* Extract character 1.*/
		rRdsPsNameAct[rPsSeg+1] = rRdsCurSR2;         /* Extract character 2.*/
		//rRdsPsStat.byte |= rPsFlg;        /* 2 1st new available PSN char.*/
	}
	else	//if (rRdsPsStat.byte & 0xf0)  /* One of the PS segment is already available.*/
	    rRdsPsStat.byte |= rPsFlg;      /* 2 1st new available PSN char.*/

	bRdsPSReady = False;     /* PS segment not more available.*/
	
	/* Storing of the active PS in the store one...............................*/
	if ((rRdsPsStat.byte & 0xf0) == 0xf0)       /* All the PS char are available */
	{ /* Copy active PS buffer in PS Store buffer (seen by the application).   */
		rRdsPsStat.byte &= 0x0f;        /* Active PS segments become not available.*/
		for (rPsSeg=0 ; rPsSeg<8 ; rPsSeg++)
		{
			if (rRdsPsNameAct[rPsSeg]<0x20) return;
		}
		rPsFlg = cPSNum + cBand*7;
		for (rPsSeg=0 ; rPsSeg<8 ; rPsSeg++)
		{
			if(rRdsPsNameSto[rPsSeg] != rRdsPsNameAct[rPsSeg])
			{
				bRdsPSNew = True;		/* New PS available */	
				rRdsPsNameSto[rPsSeg] = rRdsPsNameAct[rPsSeg];
				//rRadInfoID = _RadPs_Pty;
				//ArmTxCmd(0x81, 0xb4);/*return PTY */
				RDSTxStatName();
				RDSPreMem[rPsFlg].RDSPs[rPsSeg] = rRdsPsNameSto[rPsSeg];
			}
			rRdsPsNameAct[rPsSeg] = 0;
		}
		/* Calculate EEPROM offset address from 0x158 for PS store*/
		if(bRdsPSNew)
		{/* PS name has been changed */
			RDSTxStatName();
			__NOP();//bWrRdsPs = True;	/* Request to write into EEPROM */
     	}
     	bRdsPSAvail = True;	/* PS available for Display.*/
     	if(cBand==0)	bBand0PSAvail = True;
     	else	if(cBand==1)	bBand1PSAvail = True;
     	else	if(cBand==2)	bBand2PSAvail = True;
		if(rRdsMainSt == _Traffic)	return;
		if(!bAM_FM && !bRdsPtyEn && bRdsPSNew)
		{
		#if	_DebugPSName
			Printf("\r\n RDS PS:\n");
			for (rPsSeg=0 ; rPsSeg<8 ; rPsSeg++)
			{
				Printf("\r\n RDS PS:%2x",rRdsPsNameSto[rPsSeg]);
			 	// UARTOutputChar( ConvNumToASCII(rRdsPsNameSto[rPsSeg] >> 4) );
			 	// UARTOutputChar( ConvNumToASCII(rRdsPsNameSto[rPsSeg] & 0x0F) );
				// UARTOutputChar(' ');
			}
		#endif
			rRadDispSt = _DpRadRdsPs;/*RadDpUpdate(_DpRadRdsPs);	/*Update PS display*/
			//bUpdateDisp = True;
		}
		
	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtPs
INPUT/OUTPUT : None.
DESCRIPTION  : Extract PS information.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDSExtBkCRT ( )
{
	u32 rRTFlg;
	uchar rRTSeg,i;

	if(rRadMainSt!=_RadStdby && rRadMainSt!=_RadPS && rRadMainSt!=_RadScan || bEonAfDet)	return;

	if(bInvalidPi)		return;	/* if not in stable state */
	
	if(bRdsRTABAct && !bRdsRTABSto || !bRdsRTABAct && bRdsRTABSto)
	{
		for(i=0;i<16;i++)
		{
			rRdsRTNameAct[4*i] = ' ';
			rRdsRTNameAct[4*i+1] = ' ';
			rRdsRTNameAct[4*i+2] = ' ';
			rRdsRTNameAct[4*i+3] = ' ';
		}
		bRdsRTABSto = bRdsRTABAct;
	}
	rRTSeg = (rRdsRTStat.byte&0x0f);/* RT index calculation.*/
	rRTFlg = 0x00000001;
	for ( ;rRTSeg>0; )
	{
		rRTFlg <<= 1;
		--rRTSeg;
	}
	
	if(bRdsRTReady)
	{
		if(!bRdsRTABAct)/*ver A*/
		{
			rRTSeg = (rRdsRTStat.byte&0x0f) << 2;
			if ((rRdsRTNameAct[rRTSeg+0]   != rRdsCurSR3)	   /* Different new and */
			  || (rRdsRTNameAct[rRTSeg+1] != rRdsCurSR2))		 /* previous char.*/
			{
				//if (rRdsRTFlag & rRTFlg)	 /* RT segment Already received.*/
				//	rRdsRTFlag &= 0x00; 	  /* This is the 1st reception for a new RT.*/
												/* The active PS becomes not available.*/
				rRdsRTNameAct[rRTSeg+0] = rRdsCurSR3;		  /* Extract character 1.*/
				rRdsRTNameAct[rRTSeg+1] = rRdsCurSR2;		  /* Extract character 2.*/
				//RDSTxStatRT();
			}
			else	
			{
				rRdsRTFlag |= rRTFlg;	   /* 2 1st new available RTN char.*/
				
#if _DebugRTName
			Printf("\r\n RDS A_c Flag: %x",rRdsRTFlag);
#endif
			}
		}
	}

			/* Storing of the active RT in the store one...............................*/
	if (!bRdsRTABSto && rRdsRTFlag == 0xffff || bRdsRTABSto && rRdsRTFlag == 0xff)		 /* All the PS char are available */
	{ /* Copy active PS buffer in PS Store buffer (seen by the application).   */
		rRdsRTFlag = 0x00;        /* Active PS segments become not available.*/
		for (rRTSeg=0 ; rRTSeg<64 ; rRTSeg++)
		{
			if (rRdsRTNameAct[rRTSeg]<0x20) return;
		}
		rRTFlg = cPSNum + cBand*7;
		for (rRTSeg=0 ; rRTSeg<64; rRTSeg++)
		{
			if(rRdsRTNameSto[rRTSeg] != rRdsRTNameAct[rRTSeg])
			{
				bRdsRTNew = True;		/* New RT available */	
				rRdsRTNameSto[rRTSeg] = rRdsRTNameAct[rRTSeg];
				//RDSTxStatRT();
				RDSPreMem[rRTFlg].RDSRT[rRTSeg] = rRdsRTNameSto[rRTSeg];
			}
			rRdsRTNameAct[rRTSeg] = 0;
		}
		/* Calculate EEPROM offset address from 0x158 for RT store*/
		if(bRdsRTNew)
		{/* RT name has been changed */
			RDSTxStatRT();
			__NOP();//bWrRdsPs = True;	/* Request to write into EEPROM */
     	}
     	bRdsRTAvail = True;	/* RT available for Display.*/
     	//if(cBand==0)	bBand0RTAvail = True;
     	//else	if(cBand==1)	bBand1PSAvail = True;
     	//else	if(cBand==2)	bBand2PSAvail = True;
		if(rRdsMainSt == _Traffic)	return;
		if(!bAM_FM && !bRdsPtyEn && bRdsRTNew)
		{
		#if	_DebugRTName
			Printf("\r\n RDS CRT: ");
			for (rRTSeg=0 ; rRTSeg<64 ; rRTSeg++)
			{
				Printf("\r\n RDS RT:%x",rRdsRTNameSto[rRTSeg]);
			}
		#endif
		}
		
	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtPs
INPUT/OUTPUT : None.
DESCRIPTION  : Extract PS information.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RDSExtBkDRT ( )
{
	u32 rRTFlg;
	uchar rRTSeg,i;

	if(rRadMainSt!=_RadStdby && rRadMainSt!=_RadPS && rRadMainSt!=_RadScan || bEonAfDet)	return;

	if(bInvalidPi /*|| rRdsAfSt != 0*/)		return;	/* if not in stable state */

	if(bRdsRTABAct && !bRdsRTABSto || !bRdsRTABAct && bRdsRTABSto)
	{
		for(i=0;i<16;i++)
		{
			rRdsRTNameAct[4*i] = ' ';
			rRdsRTNameAct[4*i+1] = ' ';
			rRdsRTNameAct[4*i+2] = ' ';
			rRdsRTNameAct[4*i+3] = ' ';
		}
		bRdsRTABSto = bRdsRTABAct;
		
#if _DebugRTName
		Printf("\r\n RT VER CHG: ");
#endif
	}

	rRTSeg = (rRdsRTStat.byte&0x0f);/* RT index calculation.*/
	rRTFlg = 0x00000001;
	for ( ;rRTSeg>0; )
	{
		rRTFlg <<= 1;
		--rRTSeg;
	}
	if(bRdsRTReady)
	{
		if(!bRdsRTABAct)/*ver A*/
		{
			rRTSeg = (rRdsRTStat.byte&0x0f) << 2;
			if ((rRdsRTNameAct[rRTSeg+2]   != rRdsCurSR3)	   /* Different new and */
			  || (rRdsRTNameAct[rRTSeg+3] != rRdsCurSR2))		 /* previous char.*/
			{
				//if (rRdsRTFlag & rRTFlg)	  	/* RT segment Already received.*/
					//rRdsRTFlag &= 0x00;	   		/* This is the 1st reception for a new RT.*/
												/* The active PS becomes not available.*/
				rRdsRTNameAct[rRTSeg+2] = rRdsCurSR3; 		/* Extract character 1.*/
				rRdsRTNameAct[rRTSeg+3] = rRdsCurSR2; 		/* Extract character 2.*/
				//RDSTxStatRT();
			}
			else	
			{
				rRdsRTFlag |= rRTFlg;		/* 2 1st new available RTN char.*/
#if _DebugRTName
			Printf("\r\n RDS A_d Flag: %x",rRdsRTFlag);
#endif
			}
		}
		else
		{
			rRTSeg = (rRdsRTStat.byte&0x0f) << 1;
			if ((rRdsRTNameAct[rRTSeg]	 != rRdsCurSR3)    /* Different new and */
			  || (rRdsRTNameAct[rRTSeg+1] != rRdsCurSR2))		 /* previous char.*/
			{
				if (rRdsRTFlag & rRTFlg)	  /* RT segment Already received.*/
					rRdsRTFlag = 0x00;	   /* This is the 1st reception for a new RT.*/
												/* The active PS becomes not available.*/
				rRdsRTNameAct[rRTSeg] = rRdsCurSR3; 		/* Extract character 1.*/
				rRdsRTNameAct[rRTSeg+1] = rRdsCurSR2;		/* Extract character 2.*/
				RDSTxStatRT();
			}
			else	
			{
				rRdsRTFlag |= rRTFlg;		/* 2 1st new available RTN char.*/
#if _DebugRTName
			Printf("\r\n RDS B_d Flag: %x",rRdsRTFlag);
#endif
			}
		}

	}

	bRdsRTReady = False;     /* PS segment not more available.*/

		/* Storing of the active RT in the store one...............................*/
	if (!bRdsRTABSto && rRdsRTFlag == 0xffff || bRdsRTABSto && rRdsRTFlag == 0xff)       /* All the PS char are available */
	{ /* Copy active PS buffer in PS Store buffer (seen by the application).   */
		rRdsRTFlag = 0x00;        /* Active PS segments become not available.*/
		for (rRTSeg=0 ; rRTSeg<64 ; rRTSeg++)
		{
			if (rRdsRTNameAct[rRTSeg]<0x20) return;
		}
		rRTFlg = cPSNum + cBand*7;
		for (rRTSeg=0 ; rRTSeg<64 ; rRTSeg++)
		{
			if(rRdsRTNameSto[rRTSeg] != rRdsRTNameAct[rRTSeg])
			{
				bRdsRTNew = True;		/* New RT available */	
				rRdsRTNameSto[rRTSeg] = rRdsRTNameAct[rRTSeg];
				//RDSTxStatRT();
				RDSPreMem[rRTFlg].RDSRT[rRTSeg] = rRdsRTNameSto[rRTSeg];
			}
			rRdsRTNameAct[rRTSeg] = 0;
		}
		/* Calculate EEPROM offset address from 0x158 for RT store*/
		if(bRdsRTNew)
		{/* RT name has been changed */
			RDSTxStatRT();
			__NOP();//bWrRdsPs = True;	/* Request to write into EEPROM */
     	}
     	bRdsRTAvail = True;	/* RT available for Display.*/
     	//if(cBand==0)	bBand0RTAvail = True;
     	//else	if(cBand==1)	bBand1PSAvail = True;
     	//else	if(cBand==2)	bBand2PSAvail = True;
		if(rRdsMainSt == _Traffic)	return;
		if(!bAM_FM && !bRdsPtyEn && bRdsRTNew)
		{
		#if	_DebugRTName
			Printf("\r\n RDS RT: ");
			for (rRTSeg=0 ; rRTSeg<64; rRTSeg++)
			{
				Printf("\r\n RDS DRT:%x",rRdsRTNameSto[rRTSeg]);
			}
		#endif
			//rRadDispSt = _DpRadRdsPs;/*RadDpUpdate(_DpRadRdsPs);	/*Update PS display*/
			//bUpdateDisp = True;
		}
		
	}
	
	return;
}


/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtPi
INPUT/OUTPUT : None.
DESCRIPTION  : Extract PI code.
COMMENTS     : Corresponding to an extraction of a block A or C'.
-----------------------------------------------------------------------------*/ 
void RDSExtPi ( )
{
	uchar i;
	#if 0

	#else
  	if (rRdsPiCode.word == rRdsCurSR32.word)
  	{/* The new PI is the same with the previous stored one.*/
		bRdsPiCtrl = True;				/* PI counter is incremented.*/
		if(!bRdsPiAvail)
		{
			if(rRdsAfSt==0)
			{
				if(rRadMainSt!=_RadStdby && rRadMainSt!=_RadPS && rRadMainSt!=_RadScan)	return;
				if(rRdsPiCodeH == _NoPi || rRdsPiCodeL == _NoPi)	return;
				bRdsPiAvail = True;
				Rds_SwafFixPi.word = rRdsPiCode.word; /* Store fixed PI for AF switch check.*/     
			#if	_DebugRds
				Printf("\r\n PI CODE:%4X",rRdsPiCode.word);
			#endif
				
			#ifndef	_OnlyReduceRAM	
				i = cPSNum + cBand*7;
			#else
				i = 0;
			#endif
				RDSPreMem[i].RDSPi = Rds_SwafFixPi.word; /* Store  PI code.*/     
			
				//bWrRdsPi = True;	/* Requst to write into EEPROM */
			}
		}
		else
		{/* bRdsPiAvail = True */
			if(Rds_SwafFixPi.word != rRdsPiCode.word)	
			{/* Received an invalid PI code, it means maybe another RDS station with same freq interfered */
				bInvalidPi = True;
				if(!bRdsRegOn)
				{
					rRdsPiCodeH &= 0xf0;
					if((Rds_SwafFixPiH&0xf0)==rRdsPiCodeH
					   && Rds_SwafFixPiL==rRdsPiCodeL)
					{
						bInvalidPi = False;
						bRdsPiAvail = False;
					}
				}
			}
			else
			{
				bInvalidPi = False;
			}
			if(!bInvalidPi)
			{
				RdsPiSeek_X500ms = 90000/512;
				RdsSync_X8ms = 1200/8;
			}
			//{
			//	rRdsStoPty = 0;
			//	rRdsPtyStat.byte = 0;
			//	rRdsTaTpStat.byte = 0;
			//	bRdsEONAvail = False;
			//	bMeasureAfSig = True;
			//	rRdsAfSt = _AfWrPll;
			//}

		}
	}
  	else
  	{
		if(!bRdsPiCtrl)		/* PI counter null => new PI code stored.*/
		{
			rRdsPiCode.word = rRdsCurSR32.word;		/* Extract PI from SR3 and SR2.*/
			bRdsPiCtrl = True;					/* PI counter increment.*/
		}
		else	bRdsPiCtrl = False;				/* New PI seems to be wrong => decrement PI counter.*/
	}
	return;
	#endif
}
/*-----------------------------------------------------------------------------
ROUTINE NAME : RDSExtCT
INPUT/OUTPUT : None.
DESCRIPTION  : Extract clock-time and date information.
COMMENTS     : Corresponding to an extraction of a block C and D of group 4A.
-----------------------------------------------------------------------------*/ 
void	RDSExtCT( )
{
	uchar	i,UTCHour,UTCMin;

	UTCHour = rRdsUTCHour;
	UTCHour += (rRdsCurSR3&0xf0)>>4;
	if(UTCHour > 23)
	{
	
#if _DebugRTC
		Printf("\r\n RDS RTC H ERR: %d",UTCHour);
#endif
		return;
	}

	UTCMin = (uchar)((rRdsCurSR32.word & 0x0fc0) >> 6);
	if(UTCMin > 60)	
	{
	
#if _DebugRTC
			Printf("\r\n RDS RTC M ERR: %d",UTCMin);
#endif
		return;
	}

	i = rRdsCurSR2&0x1f;
	if(i>24)	return;
	i = i>>1;

	rRdsUTCMin = UTCMin;
	if(rRdsCurSR2&0x20)
	{ /* negative offset */ 
	
#if _DebugRTC
	Printf("\r\n RDS RTC N neg: %d",i);
#endif
		if(rRdsCurSR2&0x01)
		{  /* half hour */
			if(UTCMin < 30)
			{
				rRdsUTCMin += 30;
				if(UTCHour!=0)
					UTCHour -= 1;
				else
					UTCHour = 23;
			}
			else	rRdsUTCMin -= 30;	
		}
		else
		{
			if(UTCMin > 59)
			{
				UTCHour++;
				rRdsUTCMin = 0;
			}
		}

		if(UTCHour >= i)	rRdsUTCHour = UTCHour-i;
		else	rRdsUTCHour = UTCHour+24-i;
	}
	else
	{ /* positive offset */
#if _DebugRTC
	Printf("\r\n RDS RTC P neg: %d",i);
#endif
		if(rRdsCurSR2&0x01)
		{  /*half hour*/
			rRdsUTCMin += 30;
		}

		if(rRdsUTCMin > 59)
		{
			rRdsUTCMin -= 60;
			UTCHour += 1;
		}		
		i += UTCHour;
		if(i >= 24)	i -= 24;
		rRdsUTCHour = i;
	}
	bRdsCTAvail = True;

	if(bRdsClkEn)
	{
		armtime.hour= rRdsUTCHour;
		armtime.minute = rRdsUTCMin;
		armtime.second = 0;
		ReportDate(0);
	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtBlkB
INPUT/OUTPUT : None.
DESCRIPTION  : Extract block B information.
COMMENTS     : Extract B info = write it to message stack in RAM.
                - Group type code.
                - TP flag.
                - PTY.
-----------------------------------------------------------------------------*/ 
void RDSExtBlkB ( )
{
  	/*:::::: B   B L O C K   P E R M A N E N T   I N F O R M A T I O N S ::::::*/
  	if(bInvalidPi)	return;
  	/* EXTRACT GROUP TYPE CODE.................................................*/
  	rRdsGroup = (rRdsCurSR3 & 0xf8)>>3;             /* SR3 : g3 g2 g1 g0 AB X  X  X */

  	RDSExtTp( );                            /* Extract RDS TP information flag.*/
  	RDSExtPty( );                          /* Extract RDS PTY information code.*/
  	RdsExtr_X4ms = 84/4;
  	rRdsExtStat.byte = 0;
  	
  	/*:::::::::::::::: B BLOCK INFORMATION FROM A GROUP O(A/B) OR 15B :::::::::*/
  	switch(rRdsGroup)
  	{
  		case _Group0A:
  		case _Group0B:
  		/* Extract Program Name Segment .........................................*/
  	  			rRdsPsStat.byte &= 0xfc;
  	  			rRdsPsStat.byte += rRdsCurSR2&0x03;  /* SR2 : X  X  X  X  X  X  c1 c0 */
	  	 		bRdsPSReady = True;            /* PS segment available.*/

		case _Group2A:
		case _Group2B:				
  	  			rRdsRTStat.byte &= 0xe0;
  	  			rRdsRTStat.byte += rRdsCurSR2&0x1f;  /* SR2 : X  X  X  A/B  c3  c2  c1 c0 */
				bRdsRTReady  = True;
				break;

  	 	case _Group15B:
 	 			RDSExtTa( );                          /* Extract RDS TA information flag.*/
 	  			/*RDSExtMs();                          /* Extract RDS MS information flag.*/
				break;
			
  		case _Group14A:	/*:::::::::::::::: B BLOCK INFORMATION FROM A EON GROUP 14(A/B) :::::::::::*/
  	  			/* Extract current received variante code................................*/
				/*RDS_EonExtVariante(Rds_CurSR2);         /* SR2 : X  X  X  X  v3 v2 v1 v0 */
				
  	  			rRdsEonVar = rRdsCurSR2&0x0f;
	  	  		bRdsEONAvail = True;
    	  			bRdsEONBReady = True;         /* EON segment available.*/  	
				RDSTxRDSFlag();
  	  			break;

  		case _Group14B:
  	  			/* Extract current received EON TA state.................................*/
				RDSExtEonTa( );               /* SR2 : X  X  X  tp  ta X  X  X */
  	  			bRdsEONAvail = True;
    	  			bRdsEONBReady = True;         /* EON TA info available.*/  
				RDSTxRDSFlag();
  	  			break;
  	
  	  	default:	break;
  	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtBlkC
INPUT/OUTPUT : None.
DESCRIPTION  : Extract block C information.
COMMENTS     :     
-----------------------------------------------------------------------------*/ 
void RDSExtBlkC( )
{
  /*	if (RDS_SwafAfActive())                         /* If AF management active.*/
  /*  	return;                                                  /* => No action.*/

  	/*:::::::::::::::: C BLOCK INFORMATION FROM A GROUP 0A ::::::::::::::::::::*/
  	if (rRdsGroup == _Group0A && !bInvalidPi)
  	{  	/* Extract Alternative Frequency list from a 0A block.........................*/
  	  	RDSExtAf( );
  	  	return;
	}
	
	if ((rRdsGroup == _Group2A || rRdsGroup == _Group2B) && !bRdsRTABAct && bRdsRTReady)
  	{  	/* Extract Alternative Frequency list from a 0A block.........................*/
  	  	RDSExtBkCRT( );
  	  	return;
	}


	
  	/*:::::::::::::::: C BLOCK INFORMATION FROM A CT GROUP 4A :::::::::::::::::*/
  	if (rRdsGroup == _Group4A /*&& bRdsCTBReady*/)
  	{
  	  	/* Extract current received CT MJD bit 14..0 and Hour bit 4..............*/
		bRdsCTCReady = True;		/* CT Blk C available.*/
		if(rRdsCurSR2&0x01)	rRdsUTCHour = 0x10;
		else	rRdsUTCHour = 0x00;
		return;
  	}
  	
  	/*:::::::::::::::: C BLOCK INFORMATION FROM A EON GROUP 14A :::::::::::::::*/
  	if (rRdsGroup == _Group14A && bRdsEONBReady)
  	{
  	  	/* Extract current received EON information..............................*/
  	  	/*RDSEonExtInfo(Rds_CurSR3,Rds_CurSR2);*/
  	  	rRdsEonData1 = rRdsCurSR3;
  	  	rRdsEonData2 = rRdsCurSR2;
  	  	bRdsEONBReady = False;   /* Blk B segment not available.*/
  	  	bRdsEONCReady = True;	/* Blk C segment available.*/
  	}
	return;
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : RDS_ExtBlkD
INPUT/OUTPUT : None.
DESCRIPTION  : Extract block D information.
COMMENTS     : Corresponding to an extraction of a block D in a 0 Group.
               - Group 0: Extract Program Station Name.
-----------------------------------------------------------------------------*/ 
void RDSExtBlkD( )
{
  	/*:::::::::::::::: D BLOCK INFORMATION FROM A GROUP O (A/B) :::::::::::::::*/
  	if ((rRdsGroup == _Group0A || rRdsGroup == _Group0B) && bRdsPSReady)
  	{
  	  RDSExtPs();                  /* Extract the active Program Station Name.*/
  	}

	if ((rRdsGroup == _Group2A || rRdsGroup == _Group2B) && bRdsRTReady)
  	{  	/* Extract Alternative Frequency list from a 0A block.........................*/
  	  	RDSExtBkDRT( );
  	  	return;
	}
		
  	/*:::::::::::::::: D BLOCK INFORMATION FROM A CT GROUP 4A :::::::::::::::::*/
  	if ( rRdsGroup == _Group4A && bRdsCTCReady)
  	{
  	  /* Extract current received CT and Hour bit 3..0, Minutes and Offset.....*/
		bRdsCTCReady = False;    /* CT Blk B not more available.*/
		RDSExtCT( );
		return;
	}
  	
  	/*:::::::::::::::: D BLOCK INFORMATION FROM A EON GROUP 14(A/B) :::::::::::*/
  	if ( rRdsGroup == _Group14A && bRdsEONCReady )
  	{
  	  /* Extract current received EON PI code..................................*/
  	  bRdsEONCReady = False;   /* Blk C segment not available.*/
  	  RDSExtEonAf( );
  	  return;
  	}
  	if ( rRdsGroup == _Group14B && bRdsEONBReady )
  	{
  	  /* Extract current received EON TA state.................................*/
  	  bRdsEONBReady = False; /* EON TA info no more available.*/
  	  RDSExtEonTaPi( );
  	}
 	return;
}

void	RdsDataDecode( )
{
	uchar buffer[11], i;

	if(/*rPwrSt!=_PwrOn*/!accexistFlag || bAM_FM || (rRdsAfSt/*&&rRdsAfSt!=_AfWtChkSt*/))	return;
#if 1

	RDSFetchBlockData();
#else
	if (rRdRDS_X4ms==0)
	{
		rRdRDS_X4ms = 64/4;			// read the I2C buffer every 20ms
		FM_RDS_BlockA = 0;
		FM_RDS_BlockB = 0;
		FM_RDS_BlockC = 0;
		FM_RDS_BlockD = 0;
		FM_RDS_BleA    = 0 ;
		FM_RDS_BleB   = 0 ;
		FM_RDS_BleC   = 0 ;
		FM_RDS_BleD  = 0 ;
		rRdsSyncSt = 0;

	    // Get the RDS status from the part.
		fmRdsStatus(1, 0);
		
		
		//if(FM_RDS_RdsFifoUsed)
		//{
		//	FMRDSBlkSt = 1;
		//	FMRDSBlkSt = _BlockC;
		//}
		if( FM_RDS_RdsSync && !FM_RDS_GrpLost)		
		{
			FMRDSBlkSt = _BlockA;
			rRdsGbsStat.BlkSync = True;
			rRdsSyncSt = _BlkSync;
#if _DebugRdRdsData
	Printf("\r\n SYNC OK:FM_RDS_BlockA = :%4X,FM_RDS_BlockB = :%4x,FM_RDS_BlockC = :%4X,FM_RDS_BlockD = :%4X",FM_RDS_BlockA,FM_RDS_BlockB,FM_RDS_BlockC,FM_RDS_BlockD);
#endif
		}
		else if( FM_RDS_RdsSync && FM_RDS_GrpLost)		
		{
			FMRDSBlkSt = _BlockA;
			rRdsGbsStat.BlkSync = True;
			rRdsSyncSt = _BlkSync;
#if _DebugRdRdsData
	Printf("\r\n SYNC FAIL:FM_RDS_BlockA = :%4X,FM_RDS_BlockB = :%4x,FM_RDS_BlockC = :%4X,FM_RDS_BlockD = :%4X",FM_RDS_BlockA,FM_RDS_BlockB,FM_RDS_BlockC,FM_RDS_BlockD);
#endif
			
		}
	}
#endif
}


/*-----------------------------------------------------------------------------
ROUTINE NAME : RdsExtrProc
INPUT/OUTPUT : None
DESCRIPTION  : Rds extract proc.
COMMENTS     : 
-----------------------------------------------------------------------------*/ 
void RdsExtrProc( )
{
	uchar	i;

	if(!bPllStable || bWtChgBand || rRadMainSt <= /*_RadInit*/_RadDlToStdby || bRDSDs)	return;
			
	if(bChkStatEn && ChkStatSt < _CkWaitRds)	return;
	
	RdsDataDecode( );

	if(/*rRdsGbsStat.BlkSync*/REAL_F_GetOneGroup)
	{
		RdsReceive_X32ms = 1000/32;
		REAL_F_GetOneGroup = 0;
		
		if(!bPllStable || bWtChgBand)	return;
		
#if _DebugRdRdsData
		Printf("\r\n rRdsBlock:%x,A=%X,B=%X,C=%X,Cp=%X,D=%X",rRdsBlock,FM_RDS_BlockA,FM_RDS_BlockB,FM_RDS_BlockC,FM_RDS_BlockC,FM_RDS_BlockD);
#endif

		  if(REAL_F_BlockA)
		  {
			  // PI
			  REAL_PI=RDS.BlockLastA;
		  
			  if(REAL_PI==REAL_PILast && REAL_PILast!=REAL_PILast2)
			  {
				  REAL_F_GetNewPI=1;
				  if(REAL_PIBackup==0)
					  REAL_PIBackup=REAL_PI;
			  }
		  
			  REAL_PILast2=REAL_PILast;
			  REAL_PILast=REAL_PI;
			  //if(REAL_PI!=0 && REAL_PI==REAL_PIBackup && (RDSCheckPISeeking()==0) && RDSCheckAFEnable()
				 // &&(REAL_AFCheckMode==AFCHECK_MODE_IDLE))
			 // {
				 // REAL_PISeekTimer=PISeek100msTimer;
			 // }
		  }
		  
		 // if(REAL_AFCheckMode!=AFCHECK_MODE_IDLE)
		//	  return;
		  
		  if(REAL_F_BlockA && REAL_F_BlockB)
		  {
			  REAL_PI=RDS.BlockLastA;
		  
			  if(REAL_PI==REAL_PIBackup)
			  {
				  // PTY
				  REAL_PTY=(unsigned char)((RDS.BlockLastB& 0x03E0)>>5);
				  
				  if(REAL_PTYLast!=REAL_PTY)
				  {
					  REAL_PTYLast=REAL_PTY;
					  REAL_F_GetNewPTY=1;
				  }
		  
				  // TP
				  if(RDS.BlockLastB& 0x0400)
					  REAL_TP=1;
				  else
					  REAL_TP=0;
				  
				  if(REAL_TPLast!=REAL_TP)
				  {
					  REAL_TPLast=REAL_TP;
					  REAL_F_GetNewTP=1;
					  
					  RDS.F_GetNewTP = 1;
				  }
		  #if 0
				  // Group
				  REAL_Group=(unsigned char)(RDS.BlockLastB>>11);
		  
				  // TA
				  if(GROUP0 || GROUP15B)
				  {
					  if(RDS.BlockLastB & 0x0010)
						  REAL_TA=1;
					  else
						  REAL_TA=0;
					  
					  if(REAL_TALast!=REAL_TA)
					  {
						  REAL_TALast=REAL_TA;
						  REAL_F_GetNewTA=1;
					  }
				  }
		#endif
			  }
		  }
		  


		  /*:::::::::::::::::::::: RDS SYNCHRO STATE MACHINE ::::::::::::::::::::::::*/
  		switch (rRdsBlock)
  		{
  			case _BlockA:   /* BLOCK A ................................................*/
				//if( FM_RDS_RdsSync && !FM_RDS_BlockA)	return;	
				if(FM_RDS_BleA < 0x03) 
				{
	  			    rRdsExtStat.byte = 0;           /* Data init begin new blk A
	  			    /*; 下一块为BlockB, CNB1~CNB0为01*/
	  			    rRdsGbsStat.CNB0 = True;
				    rRdsGbsStat.CNB1 = False;
				    rRdsCurSR32.word = FM_RDS_BlockA;

	  			    RDSExtPi( );            /* Extract PI information from A and C' blocks.*/
	  			   // return;
				}
  			case _BlockB:   /* BLOCK B ................................................*/
  			    /*下一块为BlockC或Blockc, bCNB1~bCNB0为00*/
				//if( FM_RDS_RdsSync && !FM_RDS_BlockB)	return;	
				if(FM_RDS_BleB < 0x03)
				{
				    rRdsGbsStat.byte &= 0x3f;
				    rRdsCurSR32.word = FM_RDS_BlockB;
	  			    RDSExtBlkB( );          /* Extract GROUP type information from B block.*/
	  			    //return;
				}
  			case _BlockC:   /* BLOCK C ................................................*/
  			   /*下一块为BlockD, bCNB1~bCNB0为11*/
				//if( FM_RDS_RdsSync && !FM_RDS_BlockC)	return;	
			   if(FM_RDS_BleC < 0x03) 
			   	{
				    rRdsGbsStat.byte |= 0xc0;
				    rRdsCurSR32.word = FM_RDS_BlockC;
	  			    RDSExtBlkC();                    /* Extract information from C blocks.*/
	 			    //return;
			   	}
  			case _BlockCp:   /* BLOCK C' ...............................................*/
  			   /*下一块为BlockD, bCNB1~bCNB0为11*/
			    rRdsGbsStat.byte |= 0xc0;
  			    //RDSExtPi();            /* Extract PI information from A and C' blocks.*/
  			    //return;
  			case _BlockD:   /* BLOCK D ................................................*/
  			   /*下一块为BlockA, bCNB1~bCNB0为10*/
				//if( FM_RDS_RdsSync && !FM_RDS_BlockD)	return;	
			   if(FM_RDS_BleD < 0x03) 
			   	{
				    rRdsGbsStat.CNB1 = True;
				    rRdsGbsStat.CNB0 = False;
				    rRdsCurSR32.word = FM_RDS_BlockD;
	  			    RDSExtBlkD();                     /* Extract information from D block.*/
	  			    rRdsExtStat.byte = 0;
					rRdsGbsStat.BlkSync = False;
					rRdsBlock = 0;
			   	}
 			    return;                                                                       
   			default:        /* OTHERS .................................................*/
   			    /*下一块为BlockA, bCNB1~bCNB0为10*/
   			    rRdsExtStat.byte = 0;
 			    rRdsGbsStat.CNB1 = True;
 			    rRdsGbsStat.CNB0 = False;
   			    return;
   		}    
 	}
}


/***************************************************************************************\
*	   Function: RDS information display.		 									*
*	   Input   : 																		*
*      Output  : 																		*
*      description : RT, FREQ,PS,PTY
\*-------------------------------------------------------------------------------------*/			
void	RestoreAfSt( )
{
	if(rRdsAfSt != _AfIdle)
	{
		if(rRdsAfSt > _AfWrPll)
		{
			RestoreStatNum( );
			Radio7786SetFreq(Radio_7786.Freq);
			RdsTraffic_X64ms = 64/64;
			#if _EnableRadSi4730
			#else
			WritePll(_6601Preset);
			#endif
	#if _DebugRds
			Printf("\r\n Terminate AF Proc. \n");
	#endif			
		}
		rRdsAfSt = _AfIdle;
		bMeasureAfSig = False;
		RdsMsAf_X500ms = 30000/512;
		/*RdsMsAf_X96ms = 20000/96;*/
	}
	return;
}

/***************************************************************************************\
*	   Function: Measure the signal strengh of AF list frequency and choose the 		*
*				 strongest one. This routine is only called in pty off/pty on/traffic mode.
*	   Input   : 																		*
*      Output  : 																		*
*      description : The interval of measurement is decided by the signal of current 	*
*					 station (from 3s to 40s).											*
\*-------------------------------------------------------------------------------------*/			
void	MeasureAfList( )
{
	uchar	sMeter,t;
#if 0
switch(rRdsAfDetSt)
{
	case _AfDetLevIdle:
		#if 0
			if(bMeasureAfSig && (rRadMainSt!=_RadStdby || rRdsMainSt==_PtySeek 
		   		|| rRdsMainSt==_TpSeek || rRdsMainSt==_PiSeek
		   		|| !bRdsPiAvail))
			{
				bMeasureAfSig = 0;
				rRdsAfSt = 0;
				RDSFmFreqChange();	
				return;
			}
		#endif
			if(bRdsAfDetEn && rRadMainSt==_RadStdby &&( RdsMeasure_X32ms==0 
				||RdsMsAf_X500ms && bMeasureAfSig)&& rRdsAfSt==_AfIdle && !bEonAfDet)
			{
				RdsMeasure_X32ms = 2000/32;
				Rad_SMeter_X64ms = 64/64;
				cDTSParmIndex = 0;
				rRdsAfDetSt++;
			}
		break;
		
	case _AfDetLev:
		if(cDTSParmIndex >= 5 )
		{
			sMeter = AutoAlignDigitalFilter(rSi475xDataBuff1,5);  //RadLevel
			if(sMeter > 0xcb) 		t = 40000/512;	/* if sign al > 35db */
			else	if(sMeter > 0xc5) t = 30000/512;	/* if signal > 30db */
			else	if(sMeter > 0xbf) t = 20000/512;	/* if signal > 25db */
			else	if(sMeter > 0xb8) t = 10000/512;	/* if signal > 20db */
			else	if(sMeter > 0xb4) t = 5000/512;	/* if signal > 15db */
			else	t = 5000/512;	/* signal < 2.0v */
			

			if(RdsMsAf_X500ms > t)	RdsMsAf_X500ms = t;
			if(sMeter <= 0xb4) 
			{
				t =1024/512;
				RdsMsAf_X500ms = 0;
			}

			#if 1
			if(bRdsAfDetEn && bMeasureAfSig && bRdsPiAvail)
			{
				if(sMeter < 0xc5) 
				{
				
#if _DebugAFJmp
					Printf("\r\n S-AF1: %x,%x,%x",rCurStaSig,sMeter,rRdsPiCode.word);					
#endif							
					if(!RdsMsAf_X500ms)
					{/* smeter < 3.3v or RDS data receive time out(3s) */
						if(rRdsTotalAfNb && (rRdsRevdAfNum || rRdsStorAfNb) && bRdsPiAvail)
						{
						
							rCurStaSig = sMeter;
							BkupStatNum( ); /* backup current station */
							rRdsAfSt = _AfWrPll;
	#if _DebugAFJmp
							Printf("\r\n Meas AF Start:%x",rCurStaSig);
	#endif
						}
						else
						{
						}
						//bMeasureAfSig = 0;
						RdsMeasure_X32ms = 2000/32;
						rRdsAfDetSt = _AfDetLevIdle;

					}
				}
				else
				{
					
					//bMeasureAfSig = 0;
					RdsMeasure_X32ms = 5000/32;
					rRdsAfDetSt = _AfDetLevIdle;
				}
				return;
			}
			#endif

			rRdsAfDetSt++;
			cDTSParmIndex = 0;
		}
		break;
		
	case _AfDetLevEnd:
		if(cDTSParmIndex >= 5 )
		{
			sMeter = AutoAlignDigitalFilter(rSi475xDataBuff1,5);  //RadLevel
			
#if _DebugAFJmp
		Printf("\r\n S-AF2: %x,%x",rCurStaSig,sMeter);					
#endif							
			if(sMeter < 0xc5)
			{
				if(!RdsMsAf_X500ms)
				{/* smeter < 3.3v or RDS data receive time out(3s) */
					if(rRdsTotalAfNb || (rRdsRevdAfNum || rRdsStorAfNb) && bRdsPiAvail)
					{
					
						rCurStaSig = sMeter;
						BkupStatNum( ); /* backup current station */
						RdsMsAf_X500ms = 30000/512;
						rRdsAfSt = _AfWrPll;
					#if _DebugAFJmp
						Printf("\r\n Meas AF Start:%x",rCurStaSig);
					#endif
					}
					else
					{
					
					}
					RdsMeasure_X32ms = 2000/32;
					rRdsAfDetSt = _AfDetLevIdle;
				}
				else if(!RdsMeasure_X32ms)
				{
					RdsMeasure_X32ms = 2000/32;
					Rad_SMeter_X64ms = 64/64;
					cDTSParmIndex = 0;
					rRdsAfDetSt = _AfDetLev;
				}
			}
			else
			{
				RdsMeasure_X32ms = 5000/32;
				rRdsAfDetSt = _AfDetLevIdle;
			}
		}
		break;
		
	default:
		break;
}

#else	
	if(bRdsAfDetEn && rRadMainSt==_RadStdby && RdsMeasure_X16ms==0 && rRdsAfSt==_AfIdle)
	{/*change detect timer according to the signal strength of current station*/
		RdsMeasure_X16ms = 64/16;
		Elite_GetAverageQuality(1);
		rRadMeterLevel = Radio_7786.Smeter;
		rRdsSmeterBuff.word += rRadMeterLevel;
		if((++cSmeterCnt) > 9)
		{
			cSmeterCnt = 0;
			sMeter = rRdsSmeterBuff.word/10;	/* average value */
			rRdsSmeterBuff.word = 0;
		#if 0
			if(sMeter > 13)	t = 80000/500;/* if signal > 3.5v*/
			else	if(sMeter > 11)	t = 50000/500;	/* if signal > 3.3v*/
			else	if(sMeter > 9)	t = 40000/500;	/* if signal > 3.0v	*/
			else	if(sMeter > 6)	t = 30000/500;	/* if signal > 2.5v	*/
			else	if(sMeter > 3)	t = 20000/500;	/* if signal > 2.0v	*/
			else	t = 15000/500;	/* signal < 2.0v */
		#else
			if(sMeter > 0xcb)			t = 80000/500;	/* if signal > 35db */
			else	if(sMeter > 0xc5)	t = 50000/500;	/* if signal > 30db */
			else	if(sMeter > 0xbf)	t = 40000/500;	/* if signal > 25db	*/
			else	if(sMeter > 0xb8)	t = 30000/500;	/* if signal > 20db	*/
			else	if(sMeter > 0xb4)	t = 25000/500;	/* if signal > 15db	*/
			else	t = 15000/500;	/* signal < 2.0v */
		#endif
			rCurStaSig = sMeter;
	#if 0	//_DebugAFJmp
			UartOutputString("S-AF: \n");
			UARTOutputChar( ConvNumToASCII(rCurStaSig >> 4) );
			UARTOutputChar( ConvNumToASCII(rCurStaSig & 0x0F) );
			UARTOutputChar('\n');						
	#endif							
			if(bMeasureAfSig && bRdsPiAvail)
			{
				rRdsAfSt = _AfWrPll;

				return;
			}
			if(((RdsMsAf_X500ms==0 && rCurStaSig<0xc5) || ((RdsSync_X8ms==0)&&(cAfTimer_X128ms==0)))
			   && (rRdsRevdAfNum ||rRdsTotalAfNb || rRdsStorAfNb) && bRdsPiAvail)
			{/* smeter < 3.3v or RDS data receive time out(3s) */
				bMeasureAfSig = True;
				cAfTimer_X128ms = 15000/128;
				RdsMsAf_X500ms = 60000/500;
				rRdsAfSt = _AfWrPll;
	#if _DebugAFJmp
			//UartOutputString("Meas AF Start. \n");
	#endif
			}
			if((RdsMsAf_X500ms > t) || RdsMsAf_X500ms==0)	RdsMsAf_X500ms = t;
		}
	}
#endif
	return;
}

extern	void	WritePresetMem(uchar n);

/***************************************************************************************\
*   Function: AF frequency process														*
*   Input   : rRdsAfSt																	*
*   Output  : 																			*
*   description : if the signal strength of new station is stronger than current		*
				 station, replace it.
*-------------------------------------------------------------------------------------*/			
void	AfDetProc( ) 
{
	static TYPE_WORD	rfreq;
	uchar	i, j,s;
	#if 1
	if(!bMeasureAfSig || !bPllStable || bRDSDs)	return;
	if(rRadMainSt!=_RadStdby || rRdsMainSt==_PtySeek 
	   || rRdsMainSt==_TpSeek || rRdsMainSt==_PiSeek
	   || !bRdsPiAvail)
	{
		bMeasureAfSig = False;
		rRdsAfSt = 0;
		//AfMute7313(_UnMute);
		//si475x6xFMRX_set_volume(63);
		#if	_DebugAFJmp
			Printf("\r\n RDS DeMute:%2X,%4x",rRadMainSt,cRadStatNum.word);
		#endif
		return;
	}
	
	//if(Rds_SwafFixPiH==_NoPi || Rds_SwafFixPiL==_NoPi)	return;
	#endif
	switch(rRdsAfSt)
	{
		case  _AfIdle:
			break;
			
		case  _AfWrPll:	
			if(Rad_X4ms ) return;

	   		for(; ;)
	   		{
				rfreq.byte[0] = rRdsDafFmFreq[cRdsAfDet];
				
				if(/*rfreq.byte[0]==0 ||*/ rfreq.byte[0]>204 || cRdsAfDet>=_MaxAfList)
				{
					RestoreStatNum1( );	
					TunerSetAFChangeFreq(Radio_7786.Freq);
					cRdsAfDet=0;
					rRdsAfSt =0;
					bMeasureAfSig = False;
#if _DebugAFJmp
			Printf("\r\n Af over:rfreq.byte[0]=%4x,cRdsAfDet=%2X,rRdsStorAfNb =%2x,%2x",rfreq.byte[0],cRdsAfDet,rRdsStorAfNb,rRdsRevdAfNum);
#endif
					return;
				}
				rfreq.byte[1] = 0;	
				rfreq.word = GetRdsAFFreq(rfreq.word);
				if(rfreq.word == Radio_7786.Freq)
				{/*If stored station equal to current station or filler code,find next one*/
					++cRdsAfDet;
	   			}
	   			else	break;
				
				BkupStatNum( ); /* backup current station */
	   		}
			
#if _DebugAFJmp
			Printf("\r\n Af jump:Radfre =%d,cRdsAfDet=%d",rfreq.word,cRdsAfDet);
			for(i = 0;i<7;i++)
			{
				Printf("  %x-%x",i,rRdsDafFmFreq[i]);
			}
#endif
			++cRdsAfDet;	   		
			Rad_X4ms = 12/4;
			cDTSParmIndex = 0;
			++rRdsAfSt;
	   		break;
			
		case _AfWrPllDly:
			if(Rad_X4ms)	return;
			TunerSetAFCheck(rfreq.word);
			cRdsPiChkCnt = 0;
			Rad_X4ms = 8/4;
			++rRdsAfSt;
			break;
			
		case  _AfWtPllLk:
			
			if(Rad_X4ms)	return;
	   		/* PLL is stable*/
			TunerReadAFSeekStatus();
			
#if _DebugAFJmp
			Printf("\r\n Af jump new stat:lev = %d,multiple = %d,adj = %d",Radio_7786.Smeter,Radio_7786.Multipath,Radio_7786.AdjChannel);
#endif
#if 0
            if(cRdsAfDet >= rRdsStorAfNb)
            {
				RestoreStatNum1( ); 
				TunerSetAFChangeFreq(Radio_7786.Freq);
				Rad_X4ms =64 /4;
				cRdsAfDet=0;
				rRdsAfSt =0;
				bMeasureAfSig = False;
				bRdsAfAvail =  False;
				//rRdsAfSt = _AfSearchEnd;
			}
			else 
#endif				
			if(/*bRadAvailStat &&  */Radio_7786.Smeter > 0xba/*22db*/ && (Radio_7786.Smeter > rCurStaSig + 5) && Radio_7786.Multipath < 15)
	   		{/*if the signal of AF frequency is greater than current station,switch to AF */
				rRdsSyncLevel = 0;
				for(j=0;j<8;j++)
				rRdsPsNameSto[j] = rRdsPsNameAct[j] = ' ';
				for(j=0;j<16;j++)
				{
					rRdsRTNameSto[4*j] = rRdsRTNameAct[4*j] = ' ';
					rRdsRTNameSto[4*j+1] = rRdsRTNameAct[4*j+1] = ' ';
					rRdsRTNameSto[4*j+2] = rRdsRTNameAct[4*j+2] = ' ';
					rRdsRTNameSto[4*j+3] = rRdsRTNameAct[4*j+3] = ' ';
				}
				//RDSTxStatName();
				//RDSTxStatRT();
				Radio_7786.Freq = rfreq.word;
				TunerSetAFChangeFreq(rfreq.word);
				Rad_X4ms = 250/4;
				RdsMsAf_X500ms = 60000/500;
				rRdsAfSt = _AfSearchEnd;
#if _DebugAFJmp
			Printf("\r\n AF switch OK:rCurStaSig = %d,%d",rCurStaSig,Radio_7786.Smeter);
#endif
	   		}
			else
			{
				rRdsAfSt = _AfWrPll;
		#if	_DebugAFJmp
			Printf("\r\n AF switch next:rCurStaSig = %d,%d",rCurStaSig,Radio_7786.Smeter);
		#endif
			}
	   		break;
			
		case _AfWtChkSt:
			
			break;
			
	   	case  _AfSearchEnd:
			if(Rad_X4ms) return;
#if _DebugAFJmp
				Printf("\r\n switch end:rCurStaSig = %d,fre = %d",rCurStaSig,Radio_7786.Freq);
#endif
				rRdsAfSt = 0;	
				cRdsAfDet = 0;
				cPSNum = 0;
				//if(cPSNum!=0)	WritePresetMem(cPSNum); /* Update preset freq value */
				bMeasureAfSig = False;
			 	bRdsAfAvail =  False;
				cRdsPiChkCnt = 0;
				cDTSParmIndex = 0;
	   			RdsMsAf_X500ms = 30000/512;
				RdsMeasure_X32ms = 5000/32;
				TunerSetAFEnd(Radio_7786.Freq);
				GetRadPlayFre(Radio_7786.Freq);
					
				ReportRadioSignalinfo(0);
				ReportRadioPreListinfo();
	   			rRdsAfSt = _AfIdle;
	   		break;

	}
	return;
}

/***************************************************************************************\
*	   Function: PTY searching process 				 									*
*	   Input   : 																		*
*      Output  : 																		*
*      description : Find a station with same PTY code.									*
\*-------------------------------------------------------------------------------------*/			
void	PtySeekProc( )
{
	switch(rPtySeekSt)
	{
		case	_PtySeekWait:
			if(bRadSeeking && !bRDSDs)
	       	{/* in Pty on state, start pty searching */
				TunerSeekOff(SeekStopMuted);	
				if(rRadMainSt == _RadAS)
				radioinfoState1 &= (u8)(~BIT3);
			}
			
			bRadSeeking = 0;
			SaveOriginalStat( );	/* backup current station */
			BkupStatNum();
			//InitFMPresetStat();
		#if 1
				bASStatPresent = 0;
				rRadMainSt= _RadAS;
				rRadSubSt = _asWrFirstFre;
				cRadStatNum.word = 0;//AS 从最低端开始扫台
				bManuAuto = 0;
				//bRadSeeking = True;
				bRadSeek2nd = 0;
				//bLOCEn = True;
				cReqPSNum = 0;
				cPSNum = 0;
				cBandBak = cBand;
				//cFMASNum = 0;
				Rad_Seek_X4ms = 100/4;

				if(!bAM_FM)
				{
					uchar j;
					for(j = 0;j<36;j++)
					FMAsMem[j] = 0;
					FMAsStat = 0;
					InitFMPresetStat();
				}
				
				StartWrPll( );
				ReportRadioPreListinfo();
				++rPtySeekSt;
		#else
			if(bSeekDn)	rRadMainSt = _AutoTDn;
			else rRadMainSt = _AutoTUp;
			rRadSubSt = 1;
			bRadSeeking = True;		/*Start auto tuning*/
			cPSNum = 0;
			++rPtySeekSt;
			RDSTxStatMEM();
		#endif	
	#if	_DebugPTY
			Printf("\r\n PTY start. \n");
	#endif			
			break;
		
		case	_PtySeeking:
	   		if(!bRadSeeking)
	   		{
	   			rPtySeekSt = _PtySeekend;
	#if	_DebugPTY
			Printf("\r\n PTY End. \n");
	#endif							
	   		}
			break;
			
		case _PtySeekend:
			rRdsMainSt = _PtyOff;
			rPtyType = 0;
			bRdsPtyEn = 0;
			break;
	}
	return;
}

/***************************************************************************************\
*	   Function: TP searching process 				 									*
*	   Input   : rRdsSubSt																*
*      Output  : 																		*
*      description : if TP station found or user cancell TA function, stop TP searching.*
\*-------------------------------------------------------------------------------------*/			
void	TpSeekProc( )
{
	switch(rTpSeekSt)
	{
		case	_TpSeekWait:
			RdsTpSeek_X500ms = 90000/500;
			#if _EnableRadSi4730
				rRadMainSt = _AutoTUp;
				rRadSubSt = 1;
				cPSNum = 0;
				bRadSeeking = True;
			#else
			if(!bRadSeeking)
			{
			#if	_DebugRds
				Printf("\r\n tp start:\n");
			#endif
				rRadMainSt = _AutoTUp;
				rRadSubSt = 1;
				cPSNum = 0;
				bRadSeeking = True;
			}
			//RadDpUpdate(rRadDispSt);
			#endif
			++rTpSeekSt;
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
			break;
		
		case	_TpSeeking:
			if(!bRdsTAEn || !bRadSeeking)/* user disable TA or find a TP station */
			{/*stop auto tunning*/
			#if	_DebugRds
				Printf("\r\n TP seek end.\n");
			#endif
				if(bRadSeeking)
				{
					RestoreStatNum( );
					//StartWrPll( );
				}
				RadSetToDl500ms(18);
				RdsTpSeek_X500ms = 90000/500;/* Set TP searching timer */
				++rTpSeekSt;
			}
			break;
			
		case	_TpSeekEnd:
			rRdsMainSt = _PtyOff;
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
			//RadDpUpdate(_DpRadRdsPs);
			break;
	}
	return;
}

/***************************************************************************************\
*	   Function: PI searching process 				 									*
*	   Input   : PIH,PIL																*
*      Output  : 																		*
*      description : Find a station with same PI code.(switch on REG option)			*
\*-------------------------------------------------------------------------------------*/			
void	PiSeekProc( )
{
	switch(rPiSeekSt)
	{
		case	_PiSeekWait:
			//RadDpUpdate(_DpRadPiSeek);/* Display "PI SEEK" */
			SaveOriginalStat( );
			BkupStatNum();
			bPiSeeking = True;	/* set a seek pi request flag */
			rRdsPiCodeL = _NoPi;
			rRdsPiCodeH = _NoPi;
			rRadMainSt = _AutoTUp;	/* start auto searching */
			rRadSubSt = 1;
			//Si47xx_X16ms = 1000/16;
			bRadSeeking = True;
			++rPiSeekSt;
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
	#if	_DebugPiSeek
			Printf("\r\n PiSeek... %4x",OriginStNum.word );
	#endif			
			break;
		
		case	_PiSeeking:
			if(!bRadSeeking || !bPiSeeking)
			{
				bPiSeeking = False;
				++rPiSeekSt;
	#if	_DebugPiSeek
			Printf("\r\n PiSeek End:%4x",cRadStatNum.word);
	#endif							
			}
			break;
			
		case	_PiSeekEnd:
			/* find a expected station or finished searching a cycle */
			rRdsMainSt = _PtyOff;
			RadDpUpdate(_DpRadRdsPs);
			RdsPiSeek_X500ms = 90000/512;		/* Set 90s interval */
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
			break;
	}
	return;
}

/***************************************************************************************\
*	   Function: Traffic process routine			 									*
*	   Input   : 																		*
*      Output  : 																		*
*      description : Traffic mode processing
\*-------------------------------------------------------------------------------------*/			
void	TrafficProc( )
{
	#if 1
	switch(rTrafficSt)
	{
		case	_TrafficInit:
			//MainDpUpdate(_mDpSource);
			/*MainDpUpdate(_mDpSource);*/
			if(bTrafficOn && bRdsTAEn && !bEonAfDet)
			{/* Display "TRAFFIC" */	
				//RadDpUpdate(_DpRadTraffic);
			}
			else	if(bPtyAlarm)
			{
				bEWSAlarm = 1;
				RadDpUpdate(_DpRadAlarm);
				//rRadInfoID = _RadPs_Pty;
				//ArmTxCmd(0x81, 0xb4);/*return PTY */
				RDSTxStatName();
			}
		    else	if(bEonTrafficOn)
		    {/* Display "EON TA" */
		    	/*RDSEonTaSwitchTo( );*/
		    	RadDpUpdate(_DpRadEonTa);
		    	/*rTrafficSt = _TrafficWait;
		    	break;*/
		    }
			
			rdsinfoState3 |= (u8)(BIT2);
			ReportRadioBaseInfo();
			//if(rMainVol > rEonTaVol)	rEonTaActVol = rMainVol;
			//else	rEonTaActVol = rEonTaVol;
			bUserMute = False;
			/*bEonTaMute = False;*/
			//bTrafficChgBack = False;
			//if(rSndSrc != _SndSrcRad)
			//{
				bChgSrcToRad = True;	/* Need change source */
			//}
			//else
			//{
			//	rSndSrcBak =_SndSrcRad;
				///*RadSetToDl500ms( );*/Write7313( );
	#if	_DebugTraff
			Printf("\r\n Traff Start On-Rad. \n");
	#endif										
			//}
			rTrafficSt = _TrafficOn;
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
			break;
			
		/*case	_TrafficWait:
			if(*/
		    		
		    
		case	_TrafficOn:
			if(RdsTraffic_X64ms==0)
			{/* terminate traffic mode */
				++rTrafficSt;
	#if	_DebugTraff
			Printf("\r\n _TrafficEnd:%x",RdsTraffic_X64ms);
	#endif										
			}
			break;
		
		case	_TrafficEnd:
			if(!bNewKey || rKeyResult != _kcTA)
			{
				bPtyAlarm = False;
				bTrafficOn = False;
				bEonTrafficOn = False;
	#if	_DebugTraff
			Printf("\r\n _TrafficEnd1:%x",RdsTraffic_X64ms);
	#endif										
			}
			if(bRdsPtyEn && (armsourceState == ARM_SOURCE_RADIO))
			{
				RadDpUpdate(_DpRadPtyT);
				rRdsMainSt = _PtyOn;
			}
			else
			{
				RadDpUpdate(_DpRadRdsPs);
				rRdsMainSt = _PtyOff;
			}
			
			rdsinfoState3 &= ~(u8)(BIT2);
			ReportRadioBaseInfo();

			bUserMute = False;
			bEWSAlarm = 0;
			/*bEonTaMute = False;*/
			bUserTaDisable = 0;
			//rEonTaActVol = rEonTaVol;
			if(armsourceState != ARM_SOURCE_RADIO)	
				bChgSrcToBak = True;
			//else	;//Write7313( );
			//rRadInfoID = _RadFre;
			//ArmTxCmd(0x81, 0xb4);/*return Rad flag */
	#if	_DebugTraff
			Printf("\r\n Traff End. \n");
	#endif										
			break;
	}	
	
	return;
	#endif
}


