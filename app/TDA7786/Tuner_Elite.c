/*********************************************************
	Project:		ST Elite(TDA7786) Tuner SW control
	File name:	Tuner_elite.h
	Description: 
	Write/Modify:	Yete HUANG
	Time:		March-2014
*********************************************************/

#include "config.h"
#include "TDA7786M&TDA7786_Cdata_OM_Dance_SW502_default.c"
TunerStuct Tuner;
Radio7786_stuct Radio_7786;
unsigned char FmIdx;
unsigned char AmIdx;
RDSStuct RDS;


/*********************************************
	Function:		RDSInt
	Description:
	Write/Modify:	Yete HUANG
	Time:		March,2008
*********************************************/
void RDSInt(void)
{
	F_RDSInt=1;
}
/*********************************************
	Function:		Disable_RDS_Int
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-March
*********************************************/
void Disable_RDS_Int(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

#if TDA7703_7705_TUNER
	//HIT_CmdWriteMem(0x00f802,0x00000009);
#else
   #if TDA7786_TUNER && NewCommand
      Elite_CmdWriteMem(0x014003,0x0C0C0300);
   #else
      HIT_CmdWriteMem(0x014003,0x0C0C0300);
   #endif
#endif

	/* Initializes the EXTI_InitStructure */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//|EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;//|EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(/*EXTI_Line0 |*/ EXTI_Line5 /*|EXTI_Line4*/);

	//PD_RDSInt=1;
	//PO1_RDSInt=0;
	//PO2_RDSInt=0;//RDSInt
}

/*********************************************
	Function:		Enable_RDS_Int
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-March
*********************************************/
void Enable_RDS_Int(void)
{
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

#if TDA7703_7705_TUNER
	HIT_CmdWriteMem(0x00f802,0x00000001);
	HIT_CmdWriteMem(0x014003,0x0000c040);
#else
   #if TDA7786_TUNER && NewCommand
	   Elite_CmdWriteMem(0x014003,0x0C0C1300);
   #else
       HIT_CmdWriteMem(0x014003,0x0C0C1300);
   #endif
#endif

	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	  /* Select HSI as system clock source */
	/* Initializes the EXTI_InitStructure */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//|EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC_ClearIRQChannelPendingBit(EXTI3_IRQChannel);  
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;//|EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(/*EXTI_Line0 |*/ EXTI_Line5 /*|EXTI_Line4*/);

	//PD_RDSInt=0;
	//PO1_RDSInt=0;
	//PO2_RDSInt=1;//RDSInt
	//Pin_RDSInt=1;
}
/*********************************************************
				File End
*********************************************************/

//#if (TDA7703_7705_TUNER||TDA7706_TUNER||TDA7786_TUNER)

void RDSDataInitPlus(void)
{
	/*Reset RDS decoder to speed up the resynchronization*/
	#if(TDA7786_TUNER &&NewCommand)
	   Elite_CmdWriteMem(0x00f80a,0x00000000);
	#else
	   HIT_CmdWriteMem(0x00f80a,0x00000000);
	#endif
		
	RDS.BlockCnt=0;
	//	RDS.SyncBlock=0;
	RDS.LastBlockCnt=0;
}

/*********************************************
	Function:		RDSDataInit
	Description:	
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:37:24
*********************************************/
void RDSDataInit(void)
{
	unsigned char i,j;

#if (TDA7703_7705_TUNER||TDA7706_TUNER||TDA7786_TUNER )
	RDSDataInitPlus();
#else
	REAL_SyncCnt=0;
	REAL_SyncBits=0;
#endif
	
	RDS.F_GetBlockA=0;
	RDS.F_GetBlockB=0;
	RDS.F_GetBlockC=0;
	RDS.F_GetBlockCp=0;
	RDS.F_GetBlockD=0;
	REAL_F_BlockA=0;
	REAL_F_BlockB=0;
	REAL_F_BlockC=0;
	REAL_F_BlockCp=0;
	REAL_F_BlockD=0;
	
	REAL_F_RDSStation=0;
	//REAL_F_RDSStationSlow=0;
	REAL_F_GetOneGroup=0;

	REAL_F_GetNewPI=0;
	REAL_PILast2=0;
	REAL_PILast=0;
	REAL_PI=0;
	REAL_PISeekTimer=0;
	REAL_PTY=0;
	REAL_PTYLast=0;
	FM_RDS_PTY = 0;
	FM_RDS_TP = 0;
	FM_RDS_PI = 0;
	
	REAL_F_GetNewPTY=0;
	REAL_TA=0;
	REAL_TALast=0;
	REAL_TP=0;
	REAL_TPLast=0;
	REAL_F_GetNewTP=0;
	REAL_F_GetNewTA=0;
	#if 0
	REAL_EONTA=0;
	REAL_EONTALast=0;
	REAL_EONTP=0;
	REAL_EONTPLast=0;
	REAL_F_GetNewEONTP=0;
	REAL_F_GetNewEONTA=0;
	REAL_F_PSGet0=0;
	REAL_F_PSGet1=0;
	REAL_F_PSGet2=0;
	REAL_F_PSGet3=0;
	REAL_F_PSExist=0;
	REAL_F_GetNewPS=0;
	REAL_PSCRC=0;
	REAL_PSCRCTemp=0;
	REAL_F_GetNewCT=0;
	REAL_MaxLevel=0;
	
	RDSAFDisable100ms(50);

	if(RDSCheckPISeeking())return;

	REAL_PIBackup=0;
	
	if(!REAL_F_InEONTA)
	{
		for(i=1;i<25;i++)
		{
			REAL_AF[i]=0xFF;
		}
		REAL_F_AFMethodB=0;
		REAL_AFMethod[0]=0xFF;
		REAL_AFMethod[1]=0xFF;		
	}
	#ifdef EON_ENABLE
	for(i=0;i<25;i++)
	{
		REAL_EONAF[i]=0xFF;
	}
	#endif
	if(FreqInAM(Radio.Freq)||FreqInJapanFM(Radio.Freq)||FreqInWBFM(Radio.Freq))
		REAL_AF[0]=0xFF;
	else
		REAL_AF[0]=ChangFreq16BitTo8Bit(Radio.Freq);
	if(!REAL_F_InEONTA)
		REAL_AFNum=1;
	#ifdef EON_ENABLE
	REAL_EONAFNum=0;
	REAL_EONPI=0;
	for(j=0;j<EONBuffSize;j++)
	{
		REAL_EONSave_EONPI[j]=0;
		REAL_EONSave_EONAFNum[j]=0;
		for(i=0;i<25;i++)
		{
			REAL_EONSave_EONAF[j][i]=0xFF;
		}
		for(i=0;i<8;i++)
		{
			REAL_EONSave_EONPS[j][i]=0;
		}
	}
	REAL_EONCheck=0;
	REAL_EONMode=EON_MODE_IDLE;
	REAL_F_NoMatchEON=0;
	REAL_F_InEONTA=0;
	#endif
	REAL_F_GetNewAF=0;
	REAL_F_GetAFList=0;

	//BreakAFCheck();
	#endif
}


/*********************************************
	Function:		AFCheckInit
	Description:	
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:37:23
*********************************************/
void AFCheckInit(void)
{
#if (TDA7703_7705_TUNER||TDA7706_TUNER||TDA7786_TUNER )
	RDSDataInitPlus();
#else
	REAL_SyncCnt=0;
	REAL_SyncBits=0;
#endif
	
	RDS.F_GetBlockA=0;
	RDS.F_GetBlockB=0;
	RDS.F_GetBlockC=0;
	RDS.F_GetBlockCp=0;
	RDS.F_GetBlockD=0;
	REAL_F_BlockA=0;
	REAL_F_BlockB=0;
	REAL_F_BlockC=0;
	REAL_F_BlockCp=0;
	REAL_F_BlockD=0;

	REAL_F_GetNewPI=0;
	REAL_PILast2=0;
	REAL_PILast=0;
	REAL_PI=0;
	
	REAL_F_RDSStation=0;
	REAL_F_GetOneGroup=0;
}

unsigned char RDSGetDataBlock( void)
{
	if(REAL_F_RDSStation)
		return(1);
	else
		return(0);
}

void RDSSetDataBlock(unsigned char input)
{
	if(input==0)
		REAL_F_RDSStation=0;
	else
		REAL_F_RDSStation=1;
}
unsigned char RDSFlagPINew(void)
{
	if(REAL_F_GetNewPI&&REAL_PI!=0)
		return(1);
	else
		return(0);
}

/*********************************************
	Function:		RDSFetchBlockData
	Description:
	Write/Modify:	Yete HUANG
	Time:		2009-Oct
*********************************************/

void RDSFetchBlockData(void)
{
	UCharInt temp;
	unsigned char dok, syn, z;
	if(F_RDSInt)
	{
		F_RDSInt=0;
#if(TDA7786_TUNER &&NewCommand)
		Elite_CmdReadMem(0x00f800, 1);
#else
		HIT_CmdReadMem(0x00f800, 1);
#endif

		RDS.RDSData[0]=Radio_7786.HITDataBuff[3];
		RDS.RDSData[1]=Radio_7786.HITDataBuff[4];
		RDS.RDSData[2]=Radio_7786.HITDataBuff[5];
		RDS.RDSData[3]=Radio_7786.HITDataBuff[8];

		temp.byte[0]=RDS.RDSData[2];// temp.byte[1]=L, byte[0]=H in hiware
		temp.byte[1]=RDS.RDSData[3];

		dok = (RDS.RDSData[0]&0x2)>>1;
		z     = (RDS.RDSData[1]&0x80)>>7;
		syn = RDS.RDSData[0]&0x01;

#if _DebugRdRdsData
		if(dok && z && syn)
		Printf("\r\n RdsINT:D0=%X,D1=%X,D2=%X,D3=%X",RDS.RDSData[0],RDS.RDSData[1],RDS.RDSData[2],RDS.RDSData[3]);
#endif
		
		//rRdsGbsStat.BlkSync = 1;
		//rRdsSyncSt = _BlkSync;
#ifdef HIT_RADIO_DEBUG
		/*
		DEBUG_HIT_SENDSTRING("QualCount :");
		DEBUG_HIT_OUTVAL(RDS.RDSData[1]&0x0f);
		DEBUG_HIT_SENDSTRING("     ");
		DEBUG_HIT_SENDSTRING("DOK :");
		DEBUG_HIT_OUTVAL((RDS.RDSData[0]&0x2)>>1);
		DEBUG_HIT_SENDSTRING("     ");
		DEBUG_HIT_SENDSTRING("Z :");
		DEBUG_HIT_OUTVAL((RDS.RDSData[1]&0x80)>>7);
		DEBUG_HIT_SENDSTRING("     ");
		DEBUG_HIT_SENDSTRING("SYN :");
		DEBUG_HIT_OUTVAL(RDS.RDSData[0]&0x01);
		DEBUG_HIT_SENDSTRING("\n");
		*/
#endif

#if 0

		
#else
	if((RDS.RDSData[1]&0x30)==0x0)
	{
		RDS.BlockCnt=1;
		if(RDS.BlockCnt<RDS.LastBlockCnt&&RDS.LastBlockCnt!=0&&(RDS.F_GetBlockA||RDS.F_GetBlockB||RDS.F_GetBlockC||RDS.F_GetBlockCp||RDS.F_GetBlockD))
		{
			HITPushToLast();
		}

		if((RDS.RDSData[0]==0x3) &&((RDS.RDSData[1]&0x80)==0x80))//CP=0; BM=0, SYN=1,DOK=1,Z=1
		{
			RDS.F_GetBlockA=1;
			RDS.BlockA=temp.Dbyte;
			RDS.F_RDSStation=1;
			//rRdsBlock = _BlockA;
		}
		else
		{
			RDS.F_GetBlockA=0;
		}

		RDS.F_GetBlockB=0;
		RDS.F_GetBlockC=0;
		RDS.F_GetBlockCp=0;
		RDS.F_GetBlockD=0;
	}
	else if((RDS.RDSData[1]&0x30)==0x10)
	{
		RDS.BlockCnt=2;
		if(RDS.BlockCnt<RDS.LastBlockCnt&&RDS.LastBlockCnt!=0&&(RDS.F_GetBlockA||RDS.F_GetBlockB||RDS.F_GetBlockC||RDS.F_GetBlockCp||RDS.F_GetBlockD))
		{
			HITPushToLast();
		}
		if((RDS.RDSData[0]==0x3) &&((RDS.RDSData[1]&0x80)==0x80))//CP=0; BM=0, SYN=1,DOK=1,Z=1
		{
			RDS.F_GetBlockB=1;
			RDS.BlockB=temp.Dbyte;
			RDS.F_RDSStation=1;
			//rRdsBlock = _BlockB;
		}
		else
		{
			RDS.F_GetBlockA=0;
			RDS.F_GetBlockB=0;
		}
		RDS.F_GetBlockC=0;
		RDS.F_GetBlockCp=0;
		RDS.F_GetBlockD=0;
	}

	else if((RDS.RDSData[1]&0x30)==0x20)
	{
		RDS.BlockCnt=3;
		if(RDS.BlockCnt<RDS.LastBlockCnt&&RDS.LastBlockCnt!=0&&(RDS.F_GetBlockA||RDS.F_GetBlockB||RDS.F_GetBlockC||RDS.F_GetBlockCp||RDS.F_GetBlockD))
		{
			HITPushToLast();
		}
		if((RDS.RDSData[0]==0x3) &&((RDS.RDSData[1]&0x80)==0x80))//CP=0; BM=0, SYN=1,DOK=1,Z=1
		{
			RDS.F_GetBlockC=1;
			RDS.F_GetBlockCp=1;
			RDS.BlockC=temp.Dbyte;
			RDS.BlockCp=RDS.BlockC;
			
			//rRdsBlock = _BlockC;
			RDS.F_RDSStation=1;
		}
		else
		{
			RDS.F_GetBlockA=0;
			RDS.F_GetBlockB=0;
			RDS.F_GetBlockC=0;
			RDS.F_GetBlockCp=0;
			RDS.BlockC=0;
		}
		RDS.F_GetBlockD=0;
	}

	else if((RDS.RDSData[1]&0x30)==0x30)
	{
		RDS.BlockCnt=4;
		if(RDS.BlockCnt<RDS.LastBlockCnt&&RDS.LastBlockCnt!=0&&(RDS.F_GetBlockA||RDS.F_GetBlockB||RDS.F_GetBlockC||RDS.F_GetBlockCp||RDS.F_GetBlockD))
		{
			HITPushToLast();
		}
		if((RDS.RDSData[0]==0x3) &&((RDS.RDSData[1]&0x80)==0x80))//CP=0; BM=0, SYN=1,DOK=1,Z=1
		{
			RDS.F_GetBlockD=1;
			RDS.BlockD=temp.Dbyte;
			RDS.F_RDSStation=1;
			
			//rRdsBlock = _BlockD;
		}
		else
		{
			RDS.F_GetBlockA=0;
			RDS.F_GetBlockB=0;
			RDS.F_GetBlockC=0;
			RDS.F_GetBlockCp=0;
			RDS.F_GetBlockD=0;
			RDS.BlockD=0;
		}
	}


		RDS.LastBlockCnt=RDS.BlockCnt;

		if(RDS.F_RDSStation)
			RDSSetDataBlock(1);
		else
			RDSSetDataBlock(0);
	}
#endif

}


void RDSSetBlockData(unsigned char Bflags,unsigned int BlockA,unsigned int BlockB,unsigned int BlockC,unsigned int BlockCp,unsigned int BlockD)
{
	TYPE_WORD temp;
	u8 tmp;
	#if _DebugRdRdsData
	Printf("\r\n RDSdata:%x,A=%X,B=%X,C=%X,Cp=%X,D=%X",Bflags,BlockA,BlockB,BlockC,BlockCp,BlockD);
	#endif
	if((Bflags & y00000001)==0)
	{
		REAL_F_BlockA=0;
		FM_RDS_BleA = 0xff;
	}
	else
	{
		REAL_F_BlockA=1;
		FM_RDS_BleA = 0;
		temp.word =  BlockA;
		tmp = temp.byte[0];
		temp.byte[0] = temp.byte[1];
		temp.byte[1] = tmp;
		FM_RDS_BlockA = temp.word;
		FM_RDS_PI = FM_RDS_BlockA;
	}

	if((Bflags & y00000010)==0)
	{
		REAL_F_BlockB=0;
		FM_RDS_BleB = 0xff;
	}
	else
	{
		REAL_F_BlockB=1;
		FM_RDS_BleB = 0;
		temp.word =  BlockB;
		tmp = temp.byte[0];
		temp.byte[0] = temp.byte[1];
		temp.byte[1] = tmp;
		FM_RDS_BlockB = temp.word;
		
		FM_RDS_TP = FM_RDS_BlockB & 0x0400;
		FM_RDS_PTY	= (FM_RDS_BlockB >> 5) & 0x001f;
	}

	if((Bflags & y00000100)==0)
	{
		REAL_F_BlockC=0;
		FM_RDS_BleC = 0xff;
	}
	else
	{
		REAL_F_BlockC=1;
		FM_RDS_BleC = 0;
		temp.word =  BlockC;
		tmp = temp.byte[0];
		temp.byte[0] = temp.byte[1];
		temp.byte[1] = tmp;
		FM_RDS_BlockC = temp.word;
	}

	if((Bflags & y00001000)==0)
	{
		REAL_F_BlockCp=0;
		FM_RDS_BleCp = 0xff;
	}
	else 
	{
		REAL_F_BlockCp=1;
		FM_RDS_BleCp = 0;
		temp.word =  BlockCp;
		tmp = temp.byte[0];
		temp.byte[0] = temp.byte[1];
		temp.byte[1] = tmp;
		FM_RDS_BlockCp = temp.word;
		
	}

	if((Bflags & y00010000)==0)
	{
		REAL_F_BlockD=0;
		FM_RDS_BleD = 0xff;
	}
	else
	{
		REAL_F_BlockD=1;
		FM_RDS_BleD = 0;
		temp.word =  BlockD;
		tmp = temp.byte[0];
		temp.byte[0] = temp.byte[1];
		temp.byte[1] = tmp;
		FM_RDS_BlockD = temp.word;
	}

	REAL_F_GetOneGroup=1;
	rRdsBlock = _BlockA;
}



void HITPushToLast(void)
{
	unsigned char blockflags;
	RDS.BlockLastA=RDS.BlockA;
	RDS.BlockLastB=RDS.BlockB;
	RDS.BlockLastC=RDS.BlockC;
	RDS.BlockLastCp=RDS.BlockCp;
	RDS.BlockLastD=RDS.BlockD;

	RDS.F_BlockA=RDS.F_GetBlockA;
	RDS.F_BlockB=RDS.F_GetBlockB;
	RDS.F_BlockC=RDS.F_GetBlockC;
	RDS.F_BlockCp=RDS.F_GetBlockCp;
	RDS.F_BlockD=RDS.F_GetBlockD;

	RDS.F_GetBlockA=0;
	RDS.F_GetBlockB=0;
	RDS.F_GetBlockC=0;
	RDS.F_GetBlockCp=0;
	RDS.F_GetBlockD=0;


	blockflags=0;
	if(RDS.F_BlockA)
		blockflags|=y00000001;
	if(RDS.F_BlockB)
		blockflags|=y00000010;
	if(RDS.F_BlockC)
		blockflags|=y00000100;
	if(RDS.F_BlockCp)
		blockflags|=y00001000;
	if(RDS.F_BlockD)
		blockflags|=y00010000;

	RDSSetBlockData(blockflags,RDS.BlockLastA,RDS.BlockLastB,RDS.BlockLastC,RDS.BlockLastCp,RDS.BlockLastD);

}

void Radio7786SetFreq(unsigned int setfreq)
{
	TunerSetFreq(setfreq);
		
}


/*********************************************
	Function:		TunerInit
	Description:    Initialize the pins of MCU which are used for tuner
*********************************************/
void TunerInit(void)
{
	/*
	pin configure:
	Reset: RSTN(mcu out(master),RDSint(mcu out), MISO(SPI)(mcu out)
	RDS: RDSINT(mcu in)
	I2C: SCL(I2C), SDA(I2C)
	*/
	//unsigned char i,j,k;

}

//=================================================================
//Function Nam: RadioAutoScanStart()
//Descript:
//=================================================================
void Radio7786AutoScanStart(void)
{
	u8 index;
	#if 0
	if (Radio_7786.WorkMode != RADIO_WORK_IDLE)
	{
		Radio7786Break();
	}
	else
	{
		radioinfoState1 |= BIT3;
		radioinfoState1 &= (u8)(~BIT4);
		
		if (Radio_7786.CurrentBand < FM_BAND_NUM)
		{
			if (Radio_7786.Area != AREA_OIRT)
			{
				Radio_7786.CurrentBand = 0;
			}
			else
			{
				if (Radio_7786.CurrentBand != 1)
				{
					Radio_7786.CurrentBand = 0;
				}
			}
		}
		else
		{
			Radio_7786.CurrentBand = 3;
		}
		Radio_7786.Freq = Radio_7786.MinFreq;
		Radio_7786.BackupFreq = Radio_7786.MaxFreq;
		//Radio_7786.WorkMode = RADIO_WORK_ASS;
		//Radio_7786.Direction = 1;
//		radioworktimer10ms = 10;
		MuteTimeset(100);
		SysDelay(2);
		Radio7786SetFreq(Radio_7786.Freq);
		ReportRadioBaseInfo();
		ReportRadioSignalinfo(0);
	}
	#endif
}

/*********************************************
	Function:		HIT_CmdSetDiss
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-3
*********************************************/
void HIT_CmdSetDiss(unsigned char mode, unsigned char bandwidth)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SetDiss;
	Radio_7786.HITDataBuff[3]=0x0;//Para1
	Radio_7786.HITDataBuff[4]=0x0;
	Radio_7786.HITDataBuff[5]=mode;

	if(mode==Diss_FM_Manual ||mode==Diss_FM_Special)
	{
		Radio_7786.HITDataBuff[6]=0x0;//Para2
		Radio_7786.HITDataBuff[7]=0x0;
		Radio_7786.HITDataBuff[8]=bandwidth;
		Elite_CmdWriteRead(cmdcode,2,1);
	}
	else
	{
		Elite_CmdWriteRead(cmdcode,1,1);
	}
}


/*********************************************
Function:		TunerStatusUpdate
Description:    Read out the tuner current quality information and stereo indicator and/or some debug status
*********************************************/
void TunerStatusUpdate(void)
{
	Elite_GetAverageQuality(3);
#ifdef Elite_RADIO_DEBUG
	Wait10us(5);
	Elite_CmdReadMem(0x01a3fe, 1); //read out ifbw
	Radio_7786.IFBW = Radio_7786.HITDataBuff[5];
	Wait10us(5);
	Elite_CmdReadTDSR();
#endif
}

/*********************************************
Function:		TunerStartupSeq
Description:   Tuner start up sequence(reset, download boot code)
*********************************************/
void TunerStartupSeq(void)
{
	switch (rRadSubSt)
	{
		case TUNER_STARTUP_REQ:
			RADIO_PWR_ON;
			RADIO_INT_L;
			RADIO_Pin_RSTN_L;
			rRadSubSt++;
			Tuner.WaitTimer10ms = 30;
			break;
			
		case TUNER_STARTUP_WAIT:
			if(Tuner.WaitTimer10ms!=0)
				break;
			RADIO_Pin_RSTN_H;
			rRadSubSt++;
			Tuner.WaitTimer10ms = 1;
			break;
			
		case TUNER_STARTUP_WAIT1:
			if(Tuner.WaitTimer10ms!=0)
				break;
			Elite_BootcodeDownload();
			RADIO_INT_H;
			rRadSubSt=TUNER_STARTUP_WAIT_DSPINIT;
			Tuner.WaitTimer10ms = 1;	
			break;
			
		case TUNER_STARTUP_WAIT_DSPINIT:
			if(Tuner.WaitTimer10ms!=0)
				break;
			Radio_7786.HITAddressBuff[0]=0x01;
			Radio_7786.HITAddressBuff[1]=0x90;
			Radio_7786.HITAddressBuff[2]=0xEB;
			Elite_DirectRead(1,ModeXX1);
			if(Radio_7786.HITDataBuff[0]==0xAF&&Radio_7786.HITDataBuff[1]==0xFE&&Radio_7786.HITDataBuff[2]==0x42&&Radio_7786.HITDataBuff[3]==0x00)
			{
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
				rRadSubSt=TUNER_STARTUP_WAIT_VCO_FE_ALIGN;
				Tuner.WaitTimer10ms=5;
				#if _UartDebug
					Printf("Radio start ok!");
				#endif
				break;
			}
			rRadSubSt=TUNER_STARTUP_REQ;
			
		#if _UartDebug
				Printf("Radio start err!");
		#endif
			break;
			
		case TUNER_STARTUP_WAIT_VCO_FE_ALIGN:
			if(Tuner.WaitTimer10ms!=0)
				break;

			TunerChangeBand(Radio_7786.CurrentBand,Radio_7786.Freq);


			//if(!Radio_7786.F_TDBusy/*||RadioWorkTimer10ms==0*/)
			{
				if(cBand <= _BandFM3)
				{
					if((Radio_7786.Area==_Japan || Radio_7786.Area==_Russia && cBand == _BandFM3))
						;
					else //if(bRdsAfDetEn)//disable RDS int not in FM band
					{
						Enable_RDS_Int();
					}
				}
				else 
				{
					Disable_RDS_Int();
				}
				
				rRadSubSt= TUNER_STARTUP_IDLE;

			}
				
			break;
			
		case TUNER_STARTUP_IDLE:
			break;
	}
}

/*********************************************
Function:		TunerChangeBand
Description:    Change band control sequence(change to the specified band and tune to the specified frequency as well)
*********************************************/
void TunerChangeBand( unsigned char  bandreq, unsigned int freq )
{
	unsigned long MinFreq,MaxFreq,SetFreq;
	//Change to FM band from other band
	unsigned char i;
	if(bandreq <= _BandFM3)
	{
		/*optional, in case the FE registers need to be different compared to the default setting in FM band */
		Elite_CmdSetFEReg(FM,0,10);
		//Elite_CmdSetFEReg(FM,15,1);
		MinFreq=(unsigned long)Radio_7786.MinFreq*10;
		MaxFreq=(unsigned long)Radio_7786.MaxFreq*10;
		SetFreq=(unsigned long)freq*10;
		/*set the band's range and tune to the frequency in this band as well*/
		Elite_CmdChangeBand(bandcode_FM,MinFreq,MaxFreq,SetFreq);

		Elite_CmdWriteBeCoeff((unsigned char*)FMCustomWSPData);

		/*Enable option funtion e.g. Musica*/
		Elite_CmdMuSICA(0);
#ifdef AUTOSEEK
		/*SetSeekParameter if use auto seek method in ELITE*/
		Elite_CmdSetSeekTH(0x500, 0xBA0000, 0x280000, 0x040000, 0x140000, 0xC30000, 0xCD0000, 10);
#endif

	}
	#if 0
	else if(bandreq == CHANGE_WX)
	{
		/*optional, in case the FE registers need to be different compared to the default setting in WX band*/
		//Elite_CmdSetFEReg(WB,0,10);
		//Elite_CmdSetFEReg(WB,15,1);
		MinFreq=(unsigned long)Radio_7786.MinFreq+100000;
		MaxFreq=(unsigned long)Radio_7786.MaxFreq+100000;
		SetFreq=(unsigned long)freq+100000;
		/*set the band's range and tune to the frequency in this band as well*/
		Elite_CmdChangeBand(bandcode_WB,MinFreq,MaxFreq,SetFreq);
		do
		{
			Elite_CmdReadTDSR();
		}while(Radio_7786.F_SeqChg);

		Elite_CmdWriteBeCoeff((unsigned char*)WXCustomWSPData);
#ifdef AUTOSEEK
		/*SetSeekParameter if use auto seek method in ELITE*/
		Elite_CmdSetSeekTH(0x500, 0xBA0000, 0x7FFFFF, 0x040000, 0x7FFFFF, 0x7FFFFF, 0x7FFFFF, 10);
#endif
	}
	#endif
	//Change to AM band from other band
	else if(bandreq <= _BandAM2/*|| bandreq == CHANGE_SW*/ )
	{
		/*optional, in case the FE registers need to be different compared to the default setting in AM band */
		Elite_CmdSetFEReg(AM,0,10);
		//Elite_CmdSetFEReg(AM,15,1);
		//if(FreqInSWAM(freq))
		{
			//MinFreq=(unsigned long)Radio_7786.MinFreq+100000;
			//MaxFreq=(unsigned long)Radio_7786.MaxFreq+100000;
			//SetFreq=(unsigned long)freq+100000;
			//MinFreq=(unsigned long)((Radio_7786.MinFreq-1384)*5);
			//MaxFreq=(unsigned long)((Radio_7786.MaxFreq-1384)*5);
			//SetFreq=(unsigned long)freq;
		}
		//else
		{
			MinFreq=(unsigned long)Radio_7786.MinFreq;
			MaxFreq=(unsigned long)Radio_7786.MaxFreq;
			SetFreq=(unsigned long)freq;
		}

		/*set the band's range and tune to the frequency in this band as well*/
		if(Radio_7786.Area==AREA_EUROPE||Radio_7786.Area==AREA_JAPAN)
		{
			Elite_CmdChangeBand(bandcode_AM_EU_JP,MinFreq,MaxFreq,SetFreq);
		}
		else
		{
			Elite_CmdChangeBand(bandcode_AM_US,MinFreq,MaxFreq,SetFreq);
		}

		Elite_CmdWriteBeCoeff((unsigned char*) AMCustomWSPData);
#ifdef AUTOSEEK
		/*SetSeekParameter if use auto seek method in ELITE*/
		Elite_CmdSetSeekTH(0x500, 0xB00000, 0x7FFFFF, 0x100000, 0x7FFFFF, 0x7FFFFF, 0x7FFFFF, 10);
#endif
	}

#if (STA680)
	if(HD_Enable_flag == 1)//switch to fm
	{
		if(bandreq == CHANGE_FM)
		{
			HDCmd_BandSelect(1);
		}
		else if(bandreq != CHANGE_WX)//switch to am
		{
			HDCmd_BandSelect(0);
		}
		//sta680 send 0x01
		HDCmd_IBOC_Acquire();
		//same to hit2 gui
		Wait1ms(20);
	}
#endif
}

/*********************************************
Function:		TunerSetFreq
Description:    Change frequencey control sequence(change to the specified requency in current band)
*********************************************/
void TunerSetFreq(unsigned int freq)
{
	unsigned int i;
	unsigned long setfreq;
	static unsigned char temp = 200;

	if(FreqInWBFM(freq))
		setfreq=(unsigned long)freq+100000;
	else if(FreqInFM(freq))
		setfreq=(unsigned long)freq*10;
	else if(FreqInSWAM(freq))
		setfreq=(unsigned long)((freq-1384)*5);
	else
		setfreq=(unsigned long)freq;

	Elite_CmdChangeFreq(setfreq);
	
	if(FreqInFM(Radio_7786.Freq))
	{
		Radio_7786.FMBand[FmIdx].BandFreq=Radio_7786.Freq;

	#ifdef RDS_ENABLE
			RDSDataInit();
	#endif
		
	}

#if (STA680)
	if(HD_Enable_flag == 1)
	{
		//sta680 send 0x01
		HDCmd_IBOC_Acquire();
		Wait1ms(20);
		//clear display buffer
		for(i=0; i<BUFLEN; i++)
		{
			hd_buf[i] = 0;
			hd_buf1[i] = 0;
		}
	}
#endif
}

/*********************************************
Function:		TunerSetFreq
Description:    Change frequencey control sequence(change to the specified requency in current band)
*********************************************/
void TunerSetAFStart(unsigned int freq)
{
	unsigned int i;
	unsigned long setfreq;
	static unsigned char temp = 200;

	if(FreqInWBFM(freq))
		setfreq=(unsigned long)freq+100000;
	else if(FreqInFM(freq))
		setfreq=(unsigned long)freq*10;
	else if(FreqInSWAM(freq))
		setfreq=(unsigned long)((freq-1384)*5);
	else
		setfreq=(unsigned long)freq;

	Elite_CmdAFStart(setfreq);
	AFCheckInit();

}

/*********************************************
Function:		TunerSetFreq
Description:    Change frequencey control sequence(change to the specified requency in current band)
*********************************************/
void TunerSetAFCheck(unsigned int freq)
{
	unsigned int i;
	unsigned long setfreq;
	static unsigned char temp = 200;

	if(FreqInWBFM(freq))
		setfreq=(unsigned long)freq+100000;
	else if(FreqInFM(freq))
		setfreq=(unsigned long)freq*10;
	else if(FreqInSWAM(freq))
		setfreq=(unsigned long)((freq-1384)*5);
	else
		setfreq=(unsigned long)freq;

	Elite_CmdAFCheck(setfreq);
	AFCheckInit();

}



/*********************************************
Function:		TunerSetFreq
Description:    Change frequencey control sequence(change to the specified requency in current band)
*********************************************/
void TunerSetAFEnd(unsigned int freq)
{
	unsigned int i;
	unsigned long setfreq;
	static unsigned char temp = 200;
	Rad_Err_X4ms = 1000/4;

	if(FreqInWBFM(freq))
		setfreq=(unsigned long)freq+100000;
	else if(FreqInFM(freq))
		setfreq=(unsigned long)freq*10;
	else if(FreqInSWAM(freq))
		setfreq=(unsigned long)((freq-1384)*5);
	else
		setfreq=(unsigned long)freq;

	Elite_CmdAFEnd(setfreq);
	AFCheckInit();

}


/*********************************************
Function:		TunerSetFreq
Description:    Change frequencey control sequence(change to the specified requency in current band)
*********************************************/
void TunerSetAFChangeFreq(unsigned int freq)
{
	unsigned int i;
	unsigned long setfreq;
	static unsigned char temp = 200;

	if(FreqInWBFM(freq))
		setfreq=(unsigned long)freq+100000;
	else if(FreqInFM(freq))
		setfreq=(unsigned long)freq*10;
	else if(FreqInSWAM(freq))
		setfreq=(unsigned long)((freq-1384)*5);
	else
		setfreq=(unsigned long)freq;

	Elite_CmdChangeAFFreq(setfreq);

#if (STA680)
	if(HD_Enable_flag == 1)
	{
		//sta680 send 0x01
		HDCmd_IBOC_Acquire();
		Wait1ms(20);
		//clear display buffer
		for(i=0; i<BUFLEN; i++)
		{
			hd_buf[i] = 0;
			hd_buf1[i] = 0;
		}
	}
#endif
	AFCheckInit();

}

/*********************************************
Function:		TunerReadSeekStatus
Description:    read out the seek status(frequency,quality info and seek mode status)
*********************************************/
void TunerReadAFSeekStatus(void)
{
	//Elite_CmdReadSeekStatus();
	Elite_CmdReadAFQual();
}

/*********************************************
Function:		TunerSeekOn
Description:    Activate Seek mode(seekon)
*********************************************/
void TunerSeekOn(signed long seekstep)
{
	Rad_Err_X4ms = 1000/4;

#ifdef 0//AUTOSEEK
		Elite_CmdStartAutoSeek(seekstep,WaitCmdEnd,NewAutoSeek);
#else
		Elite_CmdStartManualSeek(seekstep);
#endif

}
/*********************************************
Function:		TunerSeekOff
Description:    exit from seek mode(seekon)
*********************************************/
void TunerSeekOff(unsigned char SeekEndOption)
{
	Elite_CmdSeekEnd(SeekEndOption);
}

/*********************************************
Function:		TunerReadSeekStatus
Description:    read out the seek status(frequency,quality info and seek mode status)
*********************************************/
void TunerReadSeekStatus(void)
{
	Elite_CmdReadSeekStatus();
}

#define PILOT_THS_ADDR 0X01B0D6
#define TDA7786_MUTE_ADDR 0x01B4F2

void HIT_ForeceMono()
{
    static unsigned long pilotth;
    //static char flag=0;
    if(bMonoOn)  //mono
    {
	    Elite_CmdReadMem(PILOT_THS_ADDR, 1);
	    pilotth=Radio_7786.HITDataBuff[3];
	    pilotth <<= 8;
	    pilotth |=Radio_7786.HITDataBuff[4];
	    pilotth <<= 8;
	    pilotth |=Radio_7786.HITDataBuff[5];
	    Elite_CmdWriteMem(PILOT_THS_ADDR, 0x000000);
    }
    else //restore original pilot threshold
    {
        Elite_CmdWriteMem(PILOT_THS_ADDR, pilotth);	
    }
}


	
void HIT_ForeceMute(u8 mute)
{
    static unsigned long r7786mute;
    if(mute)  //mono
    {
	    Elite_CmdReadMem(TDA7786_MUTE_ADDR, 1);
	    r7786mute=Radio_7786.HITDataBuff[3];
	    r7786mute <<= 8;
	    r7786mute |=Radio_7786.HITDataBuff[4];
	    r7786mute <<= 8;
	    r7786mute |=Radio_7786.HITDataBuff[5];
	    Elite_CmdWriteMem(TDA7786_MUTE_ADDR, 0xFFA62D);/*4ms mute*/
    }
    else 
    {
        Elite_CmdWriteMem(TDA7786_MUTE_ADDR, 0x0059D3);/*4ms unmute*/	
    }
}

/*********************************************
Function:      HIT_DSPStuckCheck
Description:   Check if the DSP stuck happened.
Parameter:     None
Return Value: 
                  1 : DSP stuck happened
                  0 : DSP works normally  
*********************************************/
int Elite_DSPStuckCheck(void)
{
	unsigned char Smeter_log[3];
	if (Radio_7786.CurrentBand < _BandAM1)   //FM band
	{           
		Elite_CmdReadMem(0x01A0F9,1); 			 // FM: FM_smeter_log, address 0x01A0F9
	}
	else  
	{          
		Elite_CmdReadMem(0x01A1B1,1);			 //AM: AM_smeter_log, address 0x01A1B1
	}   

	Smeter_log[0] = Radio_7786.HITDataBuff[3];
	Smeter_log[1] = Radio_7786.HITDataBuff[4];
	Smeter_log[2] = Radio_7786.HITDataBuff[5]; 

	//Read out SYSCO_REG1 (0x14001)
	Elite_CmdReadMem(0x14001,2);
	  
	if (Radio_7786.HITDataBuff[3] != 0xB7 || Radio_7786.HITDataBuff[4] != 0xC2 || Radio_7786.HITDataBuff[5] != 0x7F || Radio_7786.HITDataBuff[8] != 0x7E )
	  {
		return 1;
	  }

	//Check if  SYSCO_REG1 (bit31~bit8) == Smeter_log
	if( Radio_7786.HITDataBuff[3] == Smeter_log[0] && 
	    Radio_7786.HITDataBuff[4] == Smeter_log[1] && 
	    Radio_7786.HITDataBuff[5] == Smeter_log[2] )      
	{
		return 1;
	}
      
	return 0;
}



/*********************************************
	Function:		Elite_BootcodeDownload
	Description:	Download the boot code data with direct write method
*********************************************/
void Elite_BootcodeDownload(void)
{
	unsigned char num;
	unsigned int i,j;
	i=0;

	while(I2CBootData[i]!=0xff)
	{
		num=I2CBootData[i];
		I2C3Start();
		I2C3Out8Bit(ELITETUNER_I2C_ADDR);//chipaddress
	  	I2C3Out8Bit((I2CBootData[i+1]&0x01)|0xe0);//addr1
	  	I2C3Out8Bit(I2CBootData[i+2]);
		I2C3Out8Bit(I2CBootData[i+3]);
		for(j=i+4;j<i+4*num+4;j++)
		{
			I2C3Out8Bit(I2CBootData[j]);
		}
		I2C3Stop();
		i=i+4*num+4;
	}
}



/*********************************************
	Function:		Elite_GetAverageQuality
	Description:	Get the reception average quality, AverageNum <256
*********************************************/
void Elite_GetAverageQuality (unsigned char AverageNum)
{
	/* use cmd to read back the quality information*/
	unsigned int temp1,temp2,temp3,temp4;
	unsigned char i;

	temp1=0;
	temp2=0;
	temp3=0;
	temp4=0;

	if(FreqInWBFM(Radio_7786.Freq))
	{
		for(i=0; i<AverageNum; i++)
		{
			Elite_CmdReadTunerStatus(bandcode_Read_WB);
			temp1=temp1+Radio_7786.Smeter;
			temp2=temp2+Radio_7786.Detuning;
			//Wait10us(5);
			//SysWait1Us(void)
			//SysWaitUs(50);
		}
		Radio_7786.Smeter=temp1/AverageNum;
		Radio_7786.Detuning=temp2/AverageNum;
		
	}
	else if(FreqInFM(Radio_7786.Freq))
	{  
		for(i=0; i<AverageNum; i++)
		{
			Elite_CmdReadTunerStatus(bandcode_Read_FM);
			temp1=temp1+Radio_7786.Smeter;
			temp2=temp2+Radio_7786.Detuning;
			temp3=temp3+Radio_7786.Multipath;
			temp4=temp4+Radio_7786.AdjChannel;
			//Wait10us(5);
			//SysWaitUs(50);
		}
		Radio_7786.Smeter=temp1/AverageNum;
		Radio_7786.Detuning=temp2/AverageNum;
		Radio_7786.Multipath=temp3/AverageNum;
		Radio_7786.AdjChannel=temp4/AverageNum;
	}
	else
	{
	   for(i=0; i<AverageNum; i++)
	   {
		   Elite_CmdReadTunerStatus(bandcode_Read_AM);
		   temp1=temp1+Radio_7786.Smeter;
		   temp2=temp2+Radio_7786.Detuning;
		  //Wait10us(5);
		   
		   //SysWaitUs(50);
	   }
	   Radio_7786.Smeter=temp1/AverageNum;
	   Radio_7786.Detuning=temp2/AverageNum;
	}
}

/*********************************************
	Function:		Elite_CmdWriteRead
	Description:	Write data to or read data from Elite by commands
*********************************************/
void Elite_CmdWriteRead(unsigned char cmdcode,  unsigned char para_num, unsigned char rece_num)
{
	Radio_7786.HITAddressBuff[0]=0x01;//Send command address 0x019000
	Radio_7786.HITAddressBuff[1]=0x90;
	Radio_7786.HITAddressBuff[2]=0x00;

	Radio_7786.HITDataBuff[0]=(cmdcode&0xf0)>>4;
	Radio_7786.HITDataBuff[1]=(cmdcode&0x0f)<<4;
	Radio_7786.HITDataBuff[2]=(para_num+1) & 0x1f;

	Elite_DirectWrite(para_num+1, ModeXX4, 1);
	if(rece_num>=1)
	{
		Radio_7786.HITAddressBuff[2]=0xEB;//command read address 0x0190eb
		Elite_DirectRead(rece_num, ModeXX4);		//rece_num
	}
}


/*********************************************
	Function:		Elite_DirectWrite
	Description:	Directly Write data to Elite
*********************************************/
void Elite_DirectWrite(unsigned char DataNum, unsigned char mode, unsigned char cmd)
{
	unsigned int i,j;
	unsigned int checksum[3]={0,0,0};
	//DI;//asm sim;
		I2C3Start();
		I2C3Out8Bit(ELITETUNER_I2C_ADDR);//chipaddress
		switch(mode)
		{
			case ModeXX1:
			  	I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0x80);//mode+addr1
			  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
				I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[0]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[1]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[2]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[3]);
				break;
#if 0
			case ModeXX2:
			  	I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0x90);//mode+addr1
			  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
				I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[0]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[1]);
			  	I2C3Out8Bit(Radio_7786.HITDataBuff[2]);
				break;
			case ModeXX3:
			  	I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0xe0);//mode+addr1
			  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
				I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
				for(i=0;i<DataNum;i++)
				{
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*4]);
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*4+1]);
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*4+2]);
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*4+3]);
					if(cmd)
					{
						for(j=3; j>=1;j--)
						{
						 	checksum[j]=checksum[j]+Radio_7786.HITDataBuff[i*4+j];
							if (checksum[j] >= 256)
							{
					                    checksum[j - 1] = checksum[j - 1] + 1;
					                    checksum[j] = checksum[j] - 256;
							}
						}
						 checksum[0]=checksum[0]+Radio_7786.HITDataBuff[i*4];
						 if (checksum[0]>=256)
						 	checksum[0]=checksum[0]-256;

					}
				}
				if(cmd)
				{
					I2C3Out8Bit(checksum[0]&0xff);
					I2C3Out8Bit(checksum[1]&0xff);
					I2C3Out8Bit(checksum[2]&0xff);
					I2C3Out8Bit(checksum[3]&0xff);

				}
				break;
#endif
			case ModeXX4:
			  	I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0xf0);//mode+addr1
			  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
				I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
				for(i=0;i<DataNum;i++)
				{
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*3]);
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*3+1]);
					I2C3Out8Bit(Radio_7786.HITDataBuff[i*3+2]);

					if(cmd)
					{
						for(j=2; j>=1;j--)
						{
						 	checksum[j]=checksum[j]+Radio_7786.HITDataBuff[i*3+j];
							if (checksum[j] >= 256)
							{
					                    checksum[j - 1] = checksum[j - 1] + 1;
					                    checksum[j] = checksum[j] - 256;
							}
						}
						 checksum[0]=checksum[0]+Radio_7786.HITDataBuff[i*3];
						 if (checksum[0]>=256)
						 	checksum[0]=checksum[0]-256;

					}
				}
				if(cmd)
				{
					I2C3Out8Bit(checksum[0]&0xff);
					I2C3Out8Bit(checksum[1]&0xff);
					I2C3Out8Bit(checksum[2]&0xff);
				}
				break;
		}
		I2C3Stop();
	//EI;//asm rim;
}
/*********************************************
	Function:		Elite_DirectRead
	Description:	Directly read data from Elite
*********************************************/
void Elite_DirectRead(int DataNum, unsigned char mode)	//bool cmd)
{
	unsigned int i;
	//DI;//asm sim;
	int tmp=DataNum;
	I2C3Start();
	I2C3Out8Bit(ELITETUNER_I2C_ADDR);//chipaddress/write
	switch(mode)
	{
		case ModeXX1:
	  		I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0x0);//mode+addr1
		  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
			I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
			I2C3Stop();
			//Wait10us(5);
			
			SysWaitUs(4);
			I2C3Start();
	  		I2C3Out8Bit(ELITETUNER_I2C_ADDR+1);//chipaddress/read
			//Wait10us(5);
			
			SysWaitUs(4);
			Radio_7786.HITDataBuff[0]=I2C3In8Bit();
			I2C3Ack();
			Radio_7786.HITDataBuff[1]=I2C3In8Bit();
			I2C3Ack();
			Radio_7786.HITDataBuff[2]=I2C3In8Bit();
			I2C3Ack();
			Radio_7786.HITDataBuff[3]=I2C3In8Bit();
			I2C3Ack2();
			break;
#if 0
		case ModeXX2:
	  		I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0x10);//mode+addr1
		  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
			I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
			I2C3Stop();
			Wait10us(5);
			I2C3Start();
	  		I2C3Out8Bit(ELITETUNER_I2C_ADDR+1);//chipaddress/read
			Wait10us(5);
			Radio_7786.HITDataBuff[0]=I2C3In8Bit();
			I2C3Ack();
			Radio_7786.HITDataBuff[1]=I2C3In8Bit();
			I2C3Ack();
			Radio_7786.HITDataBuff[2]=I2C3In8Bit();
			I2C3Ack2();
			break;

		case ModeXX3:
	  		I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0x60);//mode+addr1
		  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
			I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
			I2C3Stop();
			Wait10us(5);
			I2C3Start();
	  		I2C3Out8Bit(ELITETUNER_I2C_ADDR+1);//chipaddress/read
			Wait10us(5);
			for(i=0;i<DataNum;i++)
			{
				Radio_7786.HITDataBuff[i*4]=I2C3In8Bit();
				I2C3Ack();
				Radio_7786.HITDataBuff[i*4+1]=I2C3In8Bit();
				I2C3Ack();
				Radio_7786.HITDataBuff[i*4+2]=I2C3In8Bit();
				I2C3Ack();
				Radio_7786.HITDataBuff[i*4+3]=I2C3In8Bit();
				if(i==DataNum-1)
				{
					I2C3Ack2();
				}
				else
				{
					I2C3Ack();
				}
			}
			break;
#endif

		case ModeXX4:
	  		I2C3Out8Bit((Radio_7786.HITAddressBuff[0]&0x01)|0x70);//mode+addr1
		  	I2C3Out8Bit(Radio_7786.HITAddressBuff[1]);
			I2C3Out8Bit(Radio_7786.HITAddressBuff[2]);
			I2C3Stop();
			//Wait10us(5);
			
			SysWaitUs(4);
			I2C3Start();
			I2C3Out8Bit(ELITETUNER_I2C_ADDR+1);//chipaddress/read
			//Wait10us(5);
			SysWaitUs(4);

			if(tmp == -1)
			{
				Radio_7786.HITDataBuff[0]=I2C3In8Bit();
				I2C3Ack();
				Radio_7786.HITDataBuff[1]=I2C3In8Bit();
				I2C3Ack();
				Radio_7786.HITDataBuff[2]=I2C3In8Bit();
				I2C3Ack();

				tmp = Radio_7786.HITDataBuff[2]&0x1f;
				
				for(i=1;i<tmp+1;i++)
				{
					Radio_7786.HITDataBuff[i*3]=I2C3In8Bit();
					I2C3Ack();
					Radio_7786.HITDataBuff[i*3+1]=I2C3In8Bit();
					I2C3Ack();
					Radio_7786.HITDataBuff[i*3+2]=I2C3In8Bit();
					if(i==tmp)
					{
						I2C3Ack2();
					}
					else
					{
						I2C3Ack();
					}
				}
			}
			else
			{
				for(i=0;i<tmp;i++)
				{
					Radio_7786.HITDataBuff[i*3]=I2C3In8Bit();
					I2C3Ack();
					Radio_7786.HITDataBuff[i*3+1]=I2C3In8Bit();
					I2C3Ack();
					Radio_7786.HITDataBuff[i*3+2]=I2C3In8Bit();
					if(i==tmp-1)
					{
						I2C3Ack2();
					}
					else
					{
						I2C3Ack();
					}
				}
			}
			break;
	}
	I2C3Stop();
	//EI;//asm rim;
}
/*********************************************
	Function:		Elite_CmdStartup
	Description:   system start up command with parameters depends on application
*********************************************/
void Elite_CmdStartup(unsigned char AlignMode)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_Startup;
	Radio_7786.HITDataBuff[3]=0x0;//Para1
	Radio_7786.HITDataBuff[4]=0x0;
	Radio_7786.HITDataBuff[5]=AlignMode;
	Elite_CmdWriteRead(cmdcode,1,1);
	
	Rad_Err_X4ms = 1000/4;
	do
	{
		SysDelay(10);
		Elite_CmdReadTDSR();
	}while(Radio_7786.F_SeqChg && Rad_Err_X4ms);
	if(!Rad_Err_X4ms) RadioInit();

}

/*********************************************
	Function:		Elite_CmdChangeBand
	Description:	change band command
*********************************************/
void Elite_CmdChangeBand(unsigned char band, unsigned long minfreq, unsigned long maxfreq,unsigned long freq)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ChangeBand;
	
	Radio_7786.HITDataBuff[3]=0x00;//para 1
	Radio_7786.HITDataBuff[4]=0x00;
	Radio_7786.HITDataBuff[5]=band;
	
	Radio_7786.HITDataBuff[6]=(unsigned char)((minfreq&0xFF0000)>>16);//Para 2
	Radio_7786.HITDataBuff[7]=(unsigned char)((minfreq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[8]=(unsigned char)(minfreq&0x0000FF);
	
	Radio_7786.HITDataBuff[9]=(unsigned char)((maxfreq&0xFF0000)>>16);//Para 3
	Radio_7786.HITDataBuff[10]=(unsigned char)((maxfreq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[11]=(unsigned char)(maxfreq&0x0000FF);

	
	Radio_7786.HITDataBuff[12]=(unsigned char)((freq&0xFF0000)>>16);//Para4
	Radio_7786.HITDataBuff[13]=(unsigned char)((freq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[14]=(unsigned char)(freq&0x0000FF);

	Elite_CmdWriteRead(cmdcode,4,1);
	
	Rad_Err_X4ms = 1000/4;
	do
	{
		SysDelay(10);
		Elite_CmdReadTDSR();
	}while(Radio_7786.F_SeqChg && Rad_Err_X4ms);
	if(!Rad_Err_X4ms) RadioInit();

}

/*********************************************
	Function:		Elite_CmdChangeFreq
	Description:	Tune to specific frequency command
*********************************************/
void Elite_CmdChangeFreq(unsigned long freq)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ChangeFreq;
	
	Radio_7786.HITDataBuff[3]=(unsigned char)((freq&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((freq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(freq&0x0000FF);

	Elite_CmdWriteRead(cmdcode,1,1);
	
	Rad_Err_X4ms = 1000/4;
	do
	{
		SysDelay(10);
		Elite_CmdReadTDSR();
	}while(Radio_7786.F_SeqChg && Rad_Err_X4ms);
	if(!Rad_Err_X4ms) RadioInit();
}

/*********************************************
	Function:		Elite_CmdChangeFreq
	Description:	Tune to specific frequency command
*********************************************/
void Elite_CmdChangeAFFreq(unsigned long freq)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_AFSwitch;
	
	Radio_7786.HITDataBuff[3]=(unsigned char)((freq&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((freq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(freq&0x0000FF);

	Elite_CmdWriteRead(cmdcode,1,1);
}


/*********************************************
	Function:		Cmd_ReadTunerStatus
	Description:    Get the tuner quality information and the stereo indicator command
*********************************************/
void Elite_CmdReadTunerStatus(unsigned char band)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadTunerStatus;

	Radio_7786.HITDataBuff[3]=0x00;//para 1
	Radio_7786.HITDataBuff[4]=0x00;
	Radio_7786.HITDataBuff[5]=band;

	if (band==bandcode_Read_FM)
	{
		Elite_CmdWriteRead(cmdcode,1,2);
		Radio_7786.Smeter=Radio_7786.HITDataBuff[3];
		Radio_7786.Detuning=Radio_7786.HITDataBuff[4];
		Radio_7786.Multipath=Radio_7786.HITDataBuff[6];
		Radio_7786.AdjChannel=Radio_7786.HITDataBuff[7];
		if(Radio_7786.HITDataBuff[5]==1)
		{
			Radio_7786.F_Stereo=1;
		}
		else
		{
			Radio_7786.F_Stereo=0;
		}
	}
	else
	{
		Elite_CmdWriteRead(cmdcode,1,2);
		Radio_7786.Smeter=Radio_7786.HITDataBuff[3];
		Radio_7786.Detuning=Radio_7786.HITDataBuff[4];
	}
}

void Elite_CmdWriteTunerSTStatus(void)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadSTStatus;

	Radio_7786.HITDataBuff[3]=0x00;//para 1
	Radio_7786.HITDataBuff[4]=0xb0;
	Radio_7786.HITDataBuff[5]=0xd6;

	if (!bAM_FM)
	{
		Elite_CmdWriteRead(cmdcode,1,3);
	}
}

/*********************************************
	Function:		Elite_CmdSetFEReg
	Description:	Set Front-end Registers command in case customer want to change the FE registers to be different from default values
*********************************************/
void Elite_CmdSetFEReg(unsigned char band, unsigned char start, unsigned char num)
{
	unsigned char cmdcode, i;
	cmdcode=CmdCode_SetFEReg;

	if(num==0)
	{
		Elite_CmdWriteRead(cmdcode, 0,1);
	}
	else
	{
		//set FE registers to user-specified values, starting from specified subaddress
		Radio_7786.HITDataBuff[3]=0x00;//para 1---Subaddress
		Radio_7786.HITDataBuff[4]=0x00;
		Radio_7786.HITDataBuff[5]=start;

		if(start==15)
		{
			if(band==FM)
			{
				Radio_7786.HITDataBuff[6]=FMFEReg[10*3];
				Radio_7786.HITDataBuff[7]=FMFEReg[10*3+1];
				Radio_7786.HITDataBuff[8]=FMFEReg[10*3+2];
			}
			else if(band==WB)
			{
				Radio_7786.HITDataBuff[6]=WXFEReg[10*3];
				Radio_7786.HITDataBuff[7]=WXFEReg[10*3+1];
				Radio_7786.HITDataBuff[8]=WXFEReg[10*3+2];
			}

			else
			{

				Radio_7786.HITDataBuff[6]=AMFEReg[10*3];
				Radio_7786.HITDataBuff[7]=AMFEReg[10*3+1];
				Radio_7786.HITDataBuff[8]=AMFEReg[10*3+2];

			}
		}
		else
		{
			if(band==FM)
			{
				for(i=0; i<num; i++)
				{
					Radio_7786.HITDataBuff[6+i*3]=FMFEReg[start*3+i*3];
					Radio_7786.HITDataBuff[7+i*3]=FMFEReg[start*3+i*3+1];
					Radio_7786.HITDataBuff[8+i*3]=FMFEReg[start*3+i*3+2];
				}
			}

			else if(band==WB)
			{
				for(i=0; i<num; i++)
				{
					Radio_7786.HITDataBuff[6+i*3]=WXFEReg[start*3+i*3];
					Radio_7786.HITDataBuff[7+i*3]=WXFEReg[start*3+i*3+1];
					Radio_7786.HITDataBuff[8+i*3]=WXFEReg[start*3+i*3+2];
				}
			}


			else
			{
				for(i=0; i<num; i++)
				{
					Radio_7786.HITDataBuff[6+i*3]=AMFEReg[start*3+i*3];
					Radio_7786.HITDataBuff[7+i*3]=AMFEReg[start*3+i*3+1];
					Radio_7786.HITDataBuff[8+i*3]=AMFEReg[start*3+i*3+2];
				}
			}
		}
		Elite_CmdWriteRead(cmdcode, num+1,1);
	}

}



/*********************************************
	Function:		Elite_CmdMuSICA
	Description:	Enable/disable Musica function command (Musica feature is to improve a lot the field performance in multi-path interfere condition)
*********************************************/
void Elite_CmdMuSICA(unsigned char val)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_MuSICA;

	Radio_7786.HITDataBuff[3]=0x0;
	Radio_7786.HITDataBuff[4]=0x0;
	Radio_7786.HITDataBuff[5]=val;
	Elite_CmdWriteRead(cmdcode,1,1);
}


/*********************************************
	Function:		Cmd_ReadTDSR
	Description:   Read out the tuner driver status command
*********************************************/
void Elite_CmdReadTDSR(void)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadTDS;
	
	Elite_CmdWriteRead(cmdcode,0,2);
	
	if(Radio_7786.HITDataBuff[5]&0x40)
		Radio_7786.F_TDBusy=1;
	else
		Radio_7786.F_TDBusy=0;
	
	if(Radio_7786.HITDataBuff[4]&0x01)
		Radio_7786.F_TDCOLL=1;
	else
		Radio_7786.F_TDCOLL=0;


	if(Radio_7786.HITDataBuff[3]&0x01)
		Radio_7786.F_SeqChg=1;
	else
		Radio_7786.F_SeqChg=0;
}


/*********************************************
	Function:		Cmd_ReadTDSR
	Description:   Read out the tuner driver status command
*********************************************/
void Elite_CmdAFStart(unsigned long freq)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_AFStart;
	
	Radio_7786.HITDataBuff[3]=(unsigned char)((freq&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((freq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(freq&0x0000FF);

	Elite_CmdWriteRead(cmdcode,1,1);
}

/*********************************************
	Function:		Cmd_ReadTDSR
	Description:   Read out the tuner driver status command
*********************************************/
void Elite_CmdAFEnd(unsigned long freq)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_AFEnd;
	
	Radio_7786.HITDataBuff[3]=(unsigned char)((freq&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((freq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(freq&0x0000FF);

	Elite_CmdWriteRead(cmdcode,1,1);
}

/*********************************************
	Function:		Cmd_ReadTDSR
	Description:   Read out the tuner driver status command
*********************************************/
void Elite_CmdAFCheck(unsigned long freq)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_AFCheck;
	
	Radio_7786.HITDataBuff[3]=(unsigned char)((freq&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((freq&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(freq&0x0000FF);

	Elite_CmdWriteRead(cmdcode,1,1);
}


/*********************************************
	Function:		Cmd_ReadTDSR
	Description:   Read out the tuner driver status command
*********************************************/
void Elite_CmdReadAFQual(void)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadRDSQual;
	
	Elite_CmdWriteRead(cmdcode,0,4);
	
	Radio_7786.Smeter=Radio_7786.HITDataBuff[9];
	Radio_7786.Detuning=Radio_7786.HITDataBuff[11];
	Radio_7786.Multipath=Radio_7786.HITDataBuff[6];
	Radio_7786.AdjChannel=Radio_7786.HITDataBuff[7];

}

/*********************************************
	Function:		Cmd_ReadTDSR
	Description:   Read out the tuner driver status command
*********************************************/
void Elite_CmdReadAFMeasure(void)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_AFMeasure;
	
	Elite_CmdWriteRead(cmdcode,0,1);

}



/*********************************************
	Function:		Elite_CmdSetSeekTH
	Description:    It's used to program the measurement time and the thresholds of the station detector in seek mode. tmeas = M_CYC/45600 seconds
				fieldstrength:  				      high for good reception quality
				adjacent channel/detune/multipath:	low for good reception quality
				The AM adj
*********************************************/

void Elite_CmdSetSeekTH(unsigned long MeasCycle, unsigned long fieldstrengthTH, unsigned long AdjTH, unsigned long DetuneTH, unsigned long MultipathTH, unsigned long FSscalingminTH,unsigned long FSscalingmaxTH, unsigned long AvgNum)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SetSeekTH;

	Radio_7786.HITDataBuff[3]=(unsigned char)((MeasCycle&0xFF0000)>>16);//Para1   Tmeas=MeasCycle/45600= MeasCycle*0.02193ms
	Radio_7786.HITDataBuff[4]=(unsigned char)((MeasCycle&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(MeasCycle&0x0000FF);

	Radio_7786.HITDataBuff[6]=(unsigned char)((fieldstrengthTH&0xFF0000)>>16);//Para2
	Radio_7786.HITDataBuff[7]=(unsigned char)((fieldstrengthTH&0x00FF00)>>8);
	Radio_7786.HITDataBuff[8]=(unsigned char)(fieldstrengthTH&0x0000FF);

	Radio_7786.HITDataBuff[9]=(unsigned char)((AdjTH&0xFF0000)>>16);//Para3
	Radio_7786.HITDataBuff[10]=(unsigned char)((AdjTH&0x00FF00)>>8);
	Radio_7786.HITDataBuff[11]=(unsigned char)(AdjTH&0x0000FF);

	Radio_7786.HITDataBuff[12]=(unsigned char)((DetuneTH&0xFF0000)>>16);//Para4
	Radio_7786.HITDataBuff[13]=(unsigned char)((DetuneTH&0x00FF00)>>8);
	Radio_7786.HITDataBuff[14]=(unsigned char)(DetuneTH&0x0000FF);

	Radio_7786.HITDataBuff[15]=(unsigned char)((MultipathTH&0xFF0000)>>16);//Para5
	Radio_7786.HITDataBuff[16]=(unsigned char)((MultipathTH&0x00FF00)>>8);
	Radio_7786.HITDataBuff[17]=(unsigned char)(MultipathTH&0x0000FF);

	Radio_7786.HITDataBuff[18]=(unsigned char)((FSscalingminTH&0xFF0000)>>16);//Para6
	Radio_7786.HITDataBuff[19]=(unsigned char)((FSscalingminTH&0x00FF00)>>8);
	Radio_7786.HITDataBuff[20]=(unsigned char)(FSscalingminTH&0x0000FF);

	Radio_7786.HITDataBuff[21]=(unsigned char)((FSscalingmaxTH&0xFF0000)>>16);//Para7
	Radio_7786.HITDataBuff[22]=(unsigned char)((FSscalingmaxTH&0x00FF00)>>8);
	Radio_7786.HITDataBuff[23]=(unsigned char)(FSscalingmaxTH&0x0000FF);

	Radio_7786.HITDataBuff[24]=(unsigned char)((AvgNum&0xFF0000)>>16);//Para8
	Radio_7786.HITDataBuff[25]=(unsigned char)((AvgNum&0x00FF00)>>8);
	Radio_7786.HITDataBuff[26]=(unsigned char)(AvgNum&0x0000FF);

	Elite_CmdWriteRead(cmdcode,8,1);
}




/*********************************************
	Function:		Elite_CmdStartManualSeek
	Description:    It's used to manual seek(A decrement can be achieved by sending a negative increment step parameter.)
*********************************************/
void Elite_CmdStartManualSeek(signed long FreqIncrement)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_StartManuSeek;

	Radio_7786.HITDataBuff[3]=(unsigned char)((FreqIncrement&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((FreqIncrement&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(FreqIncrement&0x0000FF);

	Elite_CmdWriteRead(cmdcode,1,1);
}

/*********************************************
	Function:		Elite_CmdStartAutoSeek
	Description:   The AutoSeek command starts an automatic seek operation
*********************************************/
void Elite_CmdStartAutoSeek(signed long FreqIncrement, unsigned char AutoSeekEnd, unsigned char AutoSeekMethod)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_StartAutoSeek;

	Radio_7786.HITDataBuff[3]=(unsigned char)((FreqIncrement&0xFF0000)>>16);//Para1
	Radio_7786.HITDataBuff[4]=(unsigned char)((FreqIncrement&0x00FF00)>>8);
	Radio_7786.HITDataBuff[5]=(unsigned char)(FreqIncrement&0x0000FF);

	Radio_7786.HITDataBuff[6]=0x0;//Para2
	Radio_7786.HITDataBuff[7]=0x0;
	Radio_7786.HITDataBuff[8]=AutoSeekEnd;//AutoSeekEnd=1: automatically unmute the audio and leave the seek mode after a search stop condition has been detected

	Radio_7786.HITDataBuff[9]=0x0;//Para3
	Radio_7786.HITDataBuff[10]=0x0;
	Radio_7786.HITDataBuff[11]=AutoSeekMethod;//0:old auto seek (not advisable);1:new auto seek (advised)

	Elite_CmdWriteRead(cmdcode,3,1);
}

/*********************************************
	Function:		Elite_CmdSeekEnd
	Description:    The SeekEnd command stops or continues an automatic or manual seek cycle.
*********************************************/
void Elite_CmdSeekEnd(unsigned char SeekEndAction)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SeekEnd;

	Radio_7786.HITDataBuff[3]=0x0;//Para1
	Radio_7786.HITDataBuff[4]=0x0;
	Radio_7786.HITDataBuff[5]=SeekEndAction&0x03;//Seek action code: 01-Continue, 10-StopMuted, 11-StopUnmuted

	Elite_CmdWriteRead(cmdcode,1,1);
}



/*********************************************
	Function:		Elite_CmdReadSeekStatus
	Description:	read out Seek status
*********************************************/
void Elite_CmdReadSeekStatus(void)
{
	unsigned char cmdcode;
	unsigned long frequency;
	unsigned int i;
	unsigned char total;
	unsigned char ACp;
	unsigned long Smeter;
	unsigned long Detuning;
	unsigned long Multipath;
	unsigned long AdjChannel;
		
	Smeter=0;
	Detuning=0;
	Multipath=0;
	AdjChannel=0;
	//Wait1ms(20);
	//SysWaitMs(20);
		
#ifdef 0//AUTOSEEK
	cmdcode=CmdCode_ReadSeekStatus;
	Radio_7786.HITDataBuff[3]=0x0;//Para1
	Radio_7786.HITDataBuff[4]=0x0;
	Radio_7786.HITDataBuff[5]=SeekStatus_RstQaul;
	//Radio_7786.HITDataBuff[5]=SeekStatus_NoRstQaul;
	Elite_CmdWriteRead(cmdcode, 1, 6);
	Radio_7786.Smeter=Radio_7786.HITDataBuff[12];
	Radio_7786.Detuning=Radio_7786.HITDataBuff[14];
	Radio_7786.Multipath=Radio_7786.HITDataBuff[9];
	Radio_7786.AdjChannel=Radio_7786.HITDataBuff[10];
	#if 1
	frequency=((unsigned long)Radio_7786.HITDataBuff[6]<<16)&0xFF0000;//RF frequency
	frequency=(((unsigned long)Radio_7786.HITDataBuff[7]<<8)&0x00FF00) |frequency;
	frequency=((unsigned long)Radio_7786.HITDataBuff[8]&0x0000FF) |frequency;
	if(FreqInWBFM(frequency-100000))
		Radio_7786.DisplayFreq=frequency-100000;
	else if(FreqInFM(frequency/10))
		Radio_7786.DisplayFreq=frequency/10;
	else if(FreqInSWAM(frequency/5+1384))
		Radio_7786.DisplayFreq=frequency/5+1384;
	else
		Radio_7786.DisplayFreq=frequency;
	#endif
	if(Radio_7786.HITDataBuff[3]&SeekStatus_Seekok)
		Radio_7786.F_GoodStation=1;
	else
		Radio_7786.F_GoodStation=0;
	if(Radio_7786.HITDataBuff[3]&SeekStatus_FullCircle)
		Radio_7786.F_SeekFullCycle=1;
	else
		Radio_7786.F_SeekFullCycle=0;

#else
	total=10;
	
	for(i=0; i<total; i++)
	{		
		cmdcode=CmdCode_ReadSeekStatus;
		Radio_7786.HITDataBuff[3]=0x0;//Para1
		Radio_7786.HITDataBuff[4]=0x0;
		if (i==total-1)//only reset the quality detector in the last time	
			Radio_7786.HITDataBuff[5]=SeekStatus_RstQaul;
		else
			Radio_7786.HITDataBuff[5]=SeekStatus_NoRstQaul;

		Elite_CmdWriteRead(cmdcode, 1, 1+2+2+1);
		Radio_7786.Smeter=Radio_7786.HITDataBuff[12];
		Radio_7786.Detuning=Radio_7786.HITDataBuff[14];
		Radio_7786.Multipath=Radio_7786.HITDataBuff[9];
		Radio_7786.AdjChannel=Radio_7786.HITDataBuff[10];
#if 1
		frequency=((unsigned long)Radio_7786.HITDataBuff[6]<<16)&0xFF0000;//RF frequency
		frequency=(((unsigned long)Radio_7786.HITDataBuff[7]<<8)&0x00FF00) |frequency;
		frequency=((unsigned long)Radio_7786.HITDataBuff[8]&0x0000FF) |frequency;
		if(FreqInWBFM(frequency-100000))
			Radio_7786.DisplayFreq=frequency-100000;
		else if(FreqInFM(frequency/10))
			Radio_7786.DisplayFreq=frequency/10;
		else if(FreqInSWAM(frequency/5+1384))
			Radio_7786.DisplayFreq=frequency/5+1384;
		else
			Radio_7786.DisplayFreq=frequency;
#endif		
		Smeter+=Radio_7786.Smeter;
		Detuning+=Radio_7786.Detuning;
		Multipath+=Radio_7786.Multipath;
		AdjChannel+=Radio_7786.AdjChannel;
	}
	
	Smeter/=total;
	Multipath/=total;
	AdjChannel/=total;
	Detuning;
	
	if(Smeter<FSscalingthrmin)
		ACp=AdjChannel;
	else if(Smeter>FSscalingthrmax)
		//ACp=0;
		ACp=AdjChannel*Adjfactor;
	else
		//ACp=AdjChannel*(FSscalingthrmax-Smeter)/(FSscalingthrmax-FSscalingthrmin);
		ACp=(AdjChannel-AdjChannel*Adjfactor)*(FSscalingthrmax-Smeter)/(FSscalingthrmax-FSscalingthrmin)+AdjChannel*Adjfactor;
	
	if((Radio_7786.CurrentBand<_BandAM1 && ((!bLOCEn/* || rRadMainSt == _RadAS*/) && Smeter>=0xbb/*186 &&*/ || bLOCEn && Smeter>=0xd5/*186 &&*/) && ACp<=45 && Multipath<= 5 && Detuning<=6*total)    
	||(Radio_7786.CurrentBand>=_BandAM1 && ((!bLOCEn/* || rRadMainSt == _RadAS*/) && Smeter>= 0xaa/*164*/ || bLOCEn && Smeter>=0xc4/*186 &&*/) && Detuning<16*total))	//customized seek threshold with using assisted manual seek method

	{
		Radio_7786.F_GoodStation=1;
		if((FreqInFM(Radio_7786.Freq)&&!FreqInJapanFM(Radio_7786.Freq)&&!FreqInWBFM(Radio_7786.Freq))&&(!bRDSDs && rRdsMainSt != _PtyOff || bRdsAfDetEn|| bRdsTAEn))
		RDSDataInit();//Need further confirm
	}
	else
	{
		Radio_7786.F_GoodStation=0;
	}


	if(Radio_7786.BackupFreq == Radio_7786.DisplayFreq)
	{

		Radio_7786.F_SeekFullCycle=1;
	}
	else
	{
		Radio_7786.F_SeekFullCycle=0;
	}

	if(!Radio_7786.F_GoodStation&&!Radio_7786.F_SeekFullCycle)
	{
		//Elite_CmdSeekEnd(SeekContinue);
		//Wait1ms(20);
		
		//SysWaitMs(20);
	}

#endif

	Radio_7786.F_TunerDispReq=1;
}



/*********************************************
	Function:		Elite_Cmd_IBOC_Blender
	Description:   Enable IBOC blend
*********************************************/

void Elite_Cmd_IBOC_Blender(void)
{
	Radio_7786.HITDataBuff[3]=0x0;//Para1
	Radio_7786.HITDataBuff[4]=0x0;
	Radio_7786.HITDataBuff[5]=0x01;
	Elite_CmdWriteRead(CmdCode_IBOC_Blender,1,1);
}




/*********************************************
	Function:		Elite_CmdWriteMem
	Description:   Wrtie data to the memory cells
*********************************************/
unsigned char Elite_CmdWriteMem( unsigned long menAddr, unsigned long menData)
{
	unsigned char cmdcode;
	if( menAddr >= 0x01a000 && menAddr <= 0x01bfff)
	{
		cmdcode = CmdCode_Writemen;
		Radio_7786.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;//Para 1
		Radio_7786.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio_7786.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio_7786.HITDataBuff[6]=(menData & 0x00ff0000)>>16;//Para 1=====The HIT also need to change this
		Radio_7786.HITDataBuff[7]=(menData & 0x0000ff00)>>8;
		Radio_7786.HITDataBuff[8]=menData & 0x000000ff;
		Elite_CmdWriteRead(cmdcode,2,1);
	}
	else
	{
		cmdcode = CmdCode_WriteDMAmem;
		Radio_7786.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;    //Para 1  memory address
		Radio_7786.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio_7786.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio_7786.HITDataBuff[6] = (menData & 0xff000000)>>24;//Para 2
		Radio_7786.HITDataBuff[7] = (menData & 0x00ff0000)>>16;
		Radio_7786.HITDataBuff[8] = (menData & 0x0000ff00)>>8;
		Radio_7786.HITDataBuff[9] = Radio_7786.HITDataBuff[7];
		Radio_7786.HITDataBuff[10] = Radio_7786.HITDataBuff[8] ;
		Radio_7786.HITDataBuff[11] = menData & 0x000000ff;
		Elite_CmdWriteRead(cmdcode,3,1);
	}
}


/*********************************************
	Function:		Elite_CmdReadMem
	Description:   Read data from the memory cells
*********************************************/
unsigned char Elite_CmdReadMem(unsigned long menAddr, unsigned char Numword)
{
	unsigned char cmdcode;
	if( menAddr >= 0x01a000 && menAddr <= 0x01bfff)
	{
		cmdcode = CmdCode_Readmen;
		Radio_7786.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;//Para 1
		Radio_7786.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio_7786.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio_7786.HITDataBuff[6]=0x0;//Para2, this vaule must be <=8
		Radio_7786.HITDataBuff[7]=0x0;
		Radio_7786.HITDataBuff[8]=Numword;
		Elite_CmdWriteRead(cmdcode,2,Numword+1);//command header+numword+checksum
	}
	else
	{
		cmdcode = CmdCode_ReadDMAmem;
		Radio_7786.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;//Para 1
		Radio_7786.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio_7786.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio_7786.HITDataBuff[6]=0x0;//Para2, this vaule must be <=8
		Radio_7786.HITDataBuff[7]=0x0;
		Radio_7786.HITDataBuff[8]=Numword;
		Elite_CmdWriteRead(cmdcode,2,Numword*2+1);//command header+numword+checksum
	}
}
/*********************************************
	Function:		Elite_CmdWriteBeCoeff
	Description:   Write data in array(FMCustomWSPData, AMCustomWSPData and WBCustomWSPData) to Elite by write-mem command
*********************************************/
void Elite_CmdWriteBeCoeff( unsigned char* p_be_coeff)
{
	unsigned char num, num_word;
	unsigned int i;
	unsigned char addr1, addr2, addr3;
	unsigned long addr;
	unsigned char cmdcode;
	unsigned char offset;
	unsigned long addr_range;
	while( *p_be_coeff != 0xff )
	{
		num = num_word = *p_be_coeff;
		addr1 = *(p_be_coeff+1);
		addr2 = *(p_be_coeff+2);
		addr3 = *(p_be_coeff+3);
		offset = 0;
		addr_range = *(p_be_coeff + 1);
		addr_range <<=8;
		addr_range |= *(p_be_coeff + 2);
		addr_range <<=8;
		addr_range |= *(p_be_coeff + 3);
		if( addr_range <= 0x019fff  || (addr_range >= 0x01c000 && addr_range <= 0x01ffff ))
		{
			cmdcode = CmdCode_WriteDMAmem;
		}
		else	if( addr_range >= 0x01a000 && addr_range <= 0x01afff)
		{
			cmdcode = CmdCode_Writemen;
		}
		else if( addr_range >= 0x01b000 && addr_range <= 0x01bfff )
		{
			cmdcode = CmdCode_Writemen;
		}

		if(cmdcode == CmdCode_WriteDMAmem)
		{
			for(i = 0; i < num; i++)
			{
				Radio_7786.HITDataBuff[3]=addr1;//Para 1  memory address
				Radio_7786.HITDataBuff[4]=addr2;
				Radio_7786.HITDataBuff[5]=addr3;
				Radio_7786.HITDataBuff[6] = *(p_be_coeff+4+i*4);//Para 2
				Radio_7786.HITDataBuff[7] = *(p_be_coeff+5+i*4);
				Radio_7786.HITDataBuff[8] = *(p_be_coeff+6+i*4);
				Radio_7786.HITDataBuff[9] = Radio_7786.HITDataBuff[7];
				Radio_7786.HITDataBuff[10] = Radio_7786.HITDataBuff[8] ;
				Radio_7786.HITDataBuff[11] = *(p_be_coeff+7+i*4);
				Elite_CmdWriteRead(cmdcode,3,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr++;
				addr1 = (addr & 0x00ff0000)>>16;
				addr2 = (addr & 0x0000ff00)>>8;
				addr3 = addr & 0x000000ff;
			}
			p_be_coeff += num_word *4 +4;
		}
		else if(cmdcode == CmdCode_Writemen)
		{
			while(num > 7)
			{
				Radio_7786.HITDataBuff[3] = addr1;		//Para 1  memory address
				Radio_7786.HITDataBuff[4] = addr2;
				Radio_7786.HITDataBuff[5] = addr3;
				for(i=0; i<7; i++)
				{
					Radio_7786.HITDataBuff[6+i*3] = *(p_be_coeff+4+i*4+offset);//Para 2
					Radio_7786.HITDataBuff[7+i*3] = *(p_be_coeff+5+i*4+offset);
					Radio_7786.HITDataBuff[8+i*3] = *(p_be_coeff+6+i*4+offset);
				}
				Elite_CmdWriteRead(cmdcode,8,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr += 7;
				addr1 = (addr & 0x00ff0000)>>16;
				addr2 = (addr & 0x0000ff00)>>8;
				addr3 = addr & 0x000000ff;
				offset += 4*7;
				num -= 7;
			}
			Radio_7786.HITDataBuff[3]=addr1;//Para 1	memory address
			Radio_7786.HITDataBuff[4]=addr2;
			Radio_7786.HITDataBuff[5]=addr3;
			for(i=0; i<num; i++)
			{
				Radio_7786.HITDataBuff[6+i*3]=*(p_be_coeff+4+i*4+offset);//Para 2
				Radio_7786.HITDataBuff[7+i*3]=*(p_be_coeff+5+i*4+offset);
				Radio_7786.HITDataBuff[8+i*3]=*(p_be_coeff+6+i*4+offset);
			}
			Elite_CmdWriteRead(cmdcode,num+1,1);
			p_be_coeff += num_word *4 +4;
		}
	}
}



/*********************************************
	Function:		Tuner10msTimer
	Description:   10ms timer used in tuner control
*********************************************/
void Tuner10msTimer(void)
{
	if(Tuner.WaitTimer10ms>0)Tuner.WaitTimer10ms--;
	if(RdsSync_X8ms>0)	RdsSync_X8ms--;
	
}

//----------------------------------------------------------------
// Function Name  : I2C_delay
// Description    : none
//----------------------------------------------------------------
void I2C_delay(void)
{
	u8 i;
	
	for(i = 0; i < 1; i++);
}



void I2C3Start(void)
{
	GPIO_I2C_SDA_SET;
	GPIO_I2C_SCL_SET;
	I2C_delay();
	GPIO_I2C_SDA_RESET;
	I2C_delay();
	GPIO_I2C_SCL_RESET;
	I2C_delay();
}


void I2C3Stop(void)
{
	GPIO_I2C_SCL_RESET;
	GPIO_I2C_SDA_RESET;
	I2C_delay();
	//I2C_delay();
	GPIO_I2C_SCL_SET;
	I2C_delay();
	//I2C_delay();
	GPIO_I2C_SDA_SET;
	I2C_delay();
}





unsigned char I2C3Out8Bit(unsigned char outbyte)
{
	u8 i,timeout,dat;
	dat = outbyte;
	GPIO_I2C_SCL_RESET;
	I2C_delay();

	for(i = 0; i < 8; i ++)
	{
		GPIO_I2C_SCL_RESET;
		I2C_delay();
		if (dat & 0x80)
		{
			GPIO_I2C_SDA_SET;
		}
		else
		{
			GPIO_I2C_SDA_RESET;
		}
		
		I2C_delay();
		
		GPIO_I2C_SCL_SET;
		I2C_delay();

		dat <<= 1;
	}
	GPIO_I2C_SCL_RESET;
	I2C_delay();
	// check the acknowledge
	GPIO_I2C_SDA_INPUT; 			// Configure SDIO as open-drain
	I2C_delay();
	GPIO_I2C_SCL_SET;
	I2C_delay();
	timeout = 20;
	while(timeout--)
	{
		if (!READ_I2C_SDA)
		{
			break;
		}
		else
			I2C_delay();
	}
}


unsigned char I2C3In8Bit(void)
{
	u8 i,dat;

	GPIO_I2C_SCL_RESET;
	I2C_delay(); 
	GPIO_I2C_SDA_INPUT;
	I2C_delay();
	I2C_delay();
	dat = 0;
	
	for(i = 0; i < 8; i ++)
	{
		GPIO_I2C_SCL_RESET;
		I2C_delay();	  
		
		GPIO_I2C_SCL_SET;
		I2C_delay();
		
		dat <<= 1;
		
		if(READ_I2C_SDA)
		{
			dat |= 1;		
		}
	}
	
	return dat;
}


void I2C3Ack(void)
{
#if 1
    GPIO_I2C_SCL_RESET;
	SysDelay10us(1);
	GPIO_I2C_SDA_RESET;
	SysDelay10us(1);
	GPIO_I2C_SCL_SET;
	SysDelay10us(1);
	GPIO_I2C_SCL_RESET;
	SysDelay10us(1);

	;
	SysDelay10us(1);
#else
	I2CAck();
#endif
}

void I2C3Ack2(void)
{
	unsigned char i;

#if 1
    GPIO_I2C_SCL_RESET;
	SysDelay10us(1);

	GPIO_I2C_SDA_SET;

	SysDelay10us(1);
	;
	GPIO_I2C_SCL_SET;
	for(i=0;(i<10)&&(!Pin_Radio_I2CClk_In);i++)
		SysWaitUs(2);
	SysDelay10us(1);
	GPIO_I2C_SCL_RESET;
	SysDelay10us(1);
	;
	SysDelay10us(1);
#else
I2CAck();
#endif

}


