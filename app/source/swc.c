/*
***************************************************************************************
                                     		 SWC

                       (c) Copyright Mesada Technology Co., Ltd.
                                 All Rights Reserved
	File		: SWC.c
	By			: wanyuedong
	Version	: v1.00
***************************************************************************************
*/

//=========================== Includes ===========================

#include "config.h"

u8 scanswckeyStatus;
KEY_INFO AD_SW_Info;
KEY_INFO AD_SW2_Info;

#define ADC1_CH_NUM         2  ///要转换的通道数目


SCH_U8 CH_Arry[ADC1_CH_NUM]={AD_SW1_CHANNEL,AD_SW2_CHANNEL};
KEY_AD_DATA AD_SW_Tab2[AD_SW_MAX_NUM] =
{
	{0x00,0x000, 0xff, UICC_VOLUME_UP,			UICC_VOLUME_UP,		0},
	{0x00,0x000, 0xff, UICC_VOLUME_DOWN,		UICC_VOLUME_DOWN,	0},
	{0x00,0x000, 0xff, UICC_PRE, 	   			UICC_SEEKDN, 		1},
	{0x00,0x000, 0xff, UICC_NEXT,				UICC_SEEKUP,		1},
	{0x00,0x000, 0xff, UICC_MUTE,				UICC_MUTE,			0},
	{0x00,0x000, 0xff, UICC_HOME,  			    UICC_HOME,			0},
	{0x00,0x000, 0xff, UICC_BT_ACPTCALL, 	    UICC_BT_ACPTCALL, 	0},
	{0x00,0x000, 0xff, UICC_BT_HUNGUPCALL, 	    UICC_BT_HUNGUPCALL,	0},
	{0x00,0x000, 0xff, UICC_BACK, 		   		UICC_BACK, 			0},
	{0x00,0x000, 0xff, UICC_MODE, 				UICC_MODE,		    0},
	{0x00,0x000, 0xff, UICC_PLAY, 				UICC_PLAY, 			0},
	{0x00,0x000, 0xff, UICC_NAV, 				UICC_NAV,		    0},
	{0x00,0x000, 0xff, UICC_BAND, 			    UICC_BAND, 			0},
	{0x00,0x000, 0xff, UICC_POWER, 	    	    UICC_POWER,		    0},
	{0x00,0x000, 0xff, UICC_VOICE, 			    UICC_VOICE, 		0},
	{0x00,0x000, 0xff, UICC_EQ, 				UICC_EQ,		    0},
	{0x00,0x000, 0xff, UICC_LOUD, 				UICC_LOUD,		    0},
	
};

KEY_AD_DATA AD_SW_Tab[AD_SW_MAX_NUM] =
{
	{0x00,0x000, 0xff, UICC_VOLUME_UP,			UICC_VOLUME_UP,		0},
	{0x00,0x000, 0xff, UICC_VOLUME_DOWN,		UICC_VOLUME_DOWN,	0},
	{0x00,0x000, 0xff, UICC_PRE, 	   			UICC_SEEKDN, 		1},
	{0x00,0x000, 0xff, UICC_NEXT,				UICC_SEEKUP,		1},
	{0x00,0x000, 0xff, UICC_MUTE,				UICC_MUTE,			0},
	{0x00,0x000, 0xff, UICC_HOME,  			    UICC_HOME,			0},
	{0x00,0x000, 0xff, UICC_BT_ACPTCALL, 	    UICC_BT_ACPTCALL, 	0},
	{0x00,0x000, 0xff, UICC_BT_HUNGUPCALL, 	    UICC_BT_HUNGUPCALL,	0},
	{0x00,0x000, 0xff, UICC_BACK, 		   		UICC_BACK, 			0},
	{0x00,0x000, 0xff, UICC_MODE, 				UICC_MODE,		    0},
	{0x00,0x000, 0xff, UICC_PLAY, 				UICC_PLAY, 			0},
	{0x00,0x000, 0xff, UICC_NAV, 				UICC_NAV,		    0},
	{0x00,0x000, 0xff, UICC_BAND, 			    UICC_BAND, 			0},
	{0x00,0x000, 0xff, UICC_POWER, 	    	    UICC_POWER,		    0},
	{0x00,0x000, 0xff, UICC_VOICE, 			    UICC_VOICE, 		0},
	{0x00,0x000, 0xff, UICC_EQ, 				UICC_EQ,		    0},
	{0x00,0x000, 0xff, UICC_LOUD, 				UICC_LOUD,		    0},
	
};


///====================================================================================================================
void PostKeyCode(SCH_U8 KeyCode,SCH_U8 type)
{
	//if(SysPower.nPowerState == POWER_NORMAL_RUN&&Get_CCFL_POWER_Flag == OFF)
	//	PostMessage(MMI_MODULE,UICC_SCREEN_OPEN,NONE);
	//if(Get_REV_Flag==OFF)
	//	PostMessage(MMI_MODULE,KeyCode,type);	
}
///====================================================================================================================
void ClrADC_Data(KEY_AD_DATA *pTable,SCH_U8 Max_Cnt)
{
	SCH_U8 index;
	for(index=0; index < Max_Cnt; index++)
		pTable[index].KeyADC_Data = 0x0000;
}
void SetADC_Data(KEY_AD_DATA *pTable,SCH_U8 Max_Cnt)
{
	SCH_U8 index;
	for(index=0; index < Max_Cnt; index++)
		pTable[index].KeyADC_Data = pTable[index].KeyADC_DefaultData;
}
SCH_BOOL GetKeyNum(UICC_KEY_CODE KeyCode, SCH_U8 *Num, SCH_U8 Max_Cnt, KEY_AD_DATA *pTable)
{
	SCH_U8 index;
	for(index=0;index < Max_Cnt;index++)
	{
		if(KeyCode == pTable[index].ShortKeyCode)
		{
			*Num = index;
			return TRUE;
		}
	}
	return FALSE;
}

SCH_BOOL ADC_GetData(SCH_U8 u8Channel,SCH_U16 *ADC_Data)
{
	SCH_U8 index;
	if(IsInArray_U8(u8Channel,CH_Arry,ADC1_CH_NUM,&index))
	{
		if(u8Channel == AD_SW1_CHANNEL)
			*ADC_Data = adcvalueBuff[4] >> 2;
		else
			*ADC_Data = adcvalueBuff[5] >> 2;
		return TRUE;
	}
	return FALSE;
}

SCH_BOOL GetKeyData2(SCH_U8 ad_ch,SCH_U16 *Data)
{
	static SCH_U8 index;
	static SCH_U16 ADC_Value[10];
	SCH_U16 i,temp;
	if(!ADC_GetData(ad_ch,&ADC_Value[index]))
		return FALSE;
	if(IsInData(ADC_Value[index],ADC_DATA_MAX,PANEL_STEP))
	{
		index = 0x00;
		return FALSE;		
	}
	//if(!IsInData(ADC_Value[index],ADC_Value[0],PANEL_STEP))
	{ 
		//index = 0x00;
		//return FALSE;
	}
	if(++index >= dim(ADC_Value))
	{
	
		temp = GetAverage_U16(ADC_Value,dim(ADC_Value));
		for(i = 0;i<AD_SW_MAX_NUM;i++)
		{
			if(ad_ch == AD_SW2_CHANNEL && IsInData(temp,AD_SW_Tab[i].KeyADC_Data,PANEL_STEP))
			{ 
				//AD_SW_Tab[i].KeyStudyFlag = 0;
				//AD_SW_Tab[i].KeyADC_Data = 0x00ff;
			}
		}
		
		*Data = temp;
		index = 0x00;
		return TRUE;
	}
	return FALSE;
}


SCH_BOOL GetKeyData(SCH_U8 ad_ch,SCH_U16 *Data)
{
	static SCH_U8 index;
	static SCH_U16 ADC_Value[10];
	SCH_U16 i,temp;
	if(!ADC_GetData(ad_ch,&ADC_Value[index]))
		return FALSE;
	if(IsInData(ADC_Value[index],ADC_DATA_MAX,PANEL_STEP))
	{
		index = 0x00;
		return FALSE;		
	}
	//if(!IsInData(ADC_Value[index],ADC_Value[0],PANEL_STEP))
	{ 
		//index = 0x00;
		//return FALSE;
	}
	if(++index >= dim(ADC_Value))
	{
	
		temp = GetAverage_U16(ADC_Value,dim(ADC_Value));
		for(i = 0;i<AD_SW_MAX_NUM;i++)
		{
			if(ad_ch == AD_SW1_CHANNEL && IsInData(temp,AD_SW_Tab2[i].KeyADC_Data,PANEL_STEP))
			{ 
				//AD_SW_Tab2[i].KeyStudyFlag = 0;
				//AD_SW_Tab2[i].KeyADC_Data = 0x00ff;
			}
		}
		
		*Data = temp;
		index = 0x00;
		return TRUE;
	}
	return FALSE;
}


SCH_BOOL GetSWStudyKeyNum(void)
{
	SCH_U8 i;
	for(i = 0;i < AD_SW_MAX_NUM;i++)
	{
		if(AD_SW_Tab[i].KeyStudyFlag != 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}

SCH_BOOL GetSW2StudyKeyNum(void)
	{
		SCH_U8 i;
		for(i = 0;i < AD_SW_MAX_NUM;i++)
		{
			if(AD_SW_Tab2[i].KeyStudyFlag != 0)
			{ 
				return TRUE;
			}
		}
		return FALSE;
	}

void ResetStudyKeyNum(void)
{
	SCH_U8 i;
	for(i = 0;i < AD_SW_MAX_NUM;i++)
	{
		AD_SW_Tab[i].KeyStudyFlag = 0;
		AD_SW_Tab[i].KeyADC_Data = 0;
		AD_SW_Tab2[i].KeyStudyFlag = 0;
		AD_SW_Tab2[i].KeyADC_Data = 0;
	}
}

///======================================================================================================Init========
void AD_SW2_Init(void)
{
	sch_memset(&AD_SW2_Info,0x00,sizeof(AD_SW2_Info));
	SetADC_Data(AD_SW_Tab,AD_SW_MAX_NUM);
}
void Panel_Init(void)
{
	AD_SW2_Init();
}

///=========================================================================================================key scan=====
/********************************************************************************
**  Function      : KeyTimer
**  Author        : wenjunHu
**  Created on    : 20170804
**  Description   : ADC Key DelayTime
**  Return        : void
********************************************************************************/
void KeyTimer(KEY_INFO *KeyInfo)
{
	if(KeyInfo->HoldTime)
	{
		if(btime10ms)	KeyInfo->HoldTime--;
		//Printf("key timer %d ",KeyInfo->HoldTime);

		if(0==KeyInfo->HoldTime)
		{
			if(0==KeyInfo->KeyProperty)
			{
				//PostKeyCode(KeyInfo->ShortKeyCode,PANEL);
				SaveEncodekey(KeyInfo->ShortKeyCode);
			}
			else if(1==KeyInfo->KeyProperty)
			{
				//PostKeyCode(KeyInfo->LongKeyCode,PANEL);
				SaveEncodekey(KeyInfo->LongKeyCode);
				KeyInfo->F_HoldPress=1;
			}
			else if(2==KeyInfo->KeyProperty)
			{
				//PostKeyCode(KeyInfo->LongKeyCode,PANEL);
				SaveEncodekey(KeyInfo->LongKeyCode);
				KeyInfo->F_HoldPress=1;
				KeyInfo->HoldTime=T80MS_8;
			} 
		}
	}
}
/********************************************************************************
**  Function      : ClrKeyInfo
**  Author        : wenjunHu
**  Created on    : 20170804
**  Description   : Clear KeyInfo
**  Return        : void
********************************************************************************/
void ClrKeyInfo(KEY_INFO *KeyInfo)
{
	STUDY_STATE StudyState;
	StudyState = KeyInfo->StudyState;
	sch_memset(KeyInfo,0x00,sizeof(KEY_INFO));
	KeyInfo->StudyState = StudyState;
}
/********************************************************************************
**  Function      : KeyScan
**  Author        : wenjunHu
**  Created on    : 20170803
**  Description   : ADC Key State  up and down
**  Return        : void
********************************************************************************/
void KeyScan(KEY_INFO *KeyInfo)
{
	if(KeyInfo->KeyStatus==Key_Down)/////press down
	{
		if(KeyInfo->ShortKeyCode!=KeyInfo->BkKeyCode)
		{
			KeyInfo->BkKeyCode = KeyInfo->ShortKeyCode;
			if(KeyInfo->KeyProperty==0)/////
			{
				KeyInfo->HoldTime=T96MS_8;
			}
			else if(KeyInfo->KeyProperty==1||KeyInfo->KeyProperty==2)
			{
				KeyInfo->HoldTime=T1S_8;
				KeyInfo->F_HoldPress=0;
			}
		}
	}
	else if(KeyInfo->KeyStatus==Key_Up)///UP
	{
		if(2==KeyInfo->KeyProperty||1==KeyInfo->KeyProperty)
		{
			if(0==KeyInfo->F_HoldPress)
			{
				if(KeyInfo->HoldTime <= (T1S_8-T96MS_8))	
					SaveEncodekey(KeyInfo->ShortKeyCode);

			}
			if(1==KeyInfo->F_HoldPress && KeyInfo->HoldTime == 0 && 1==KeyInfo->KeyProperty)
			{
				SaveEncodekey(0xff);
			}
		}
		ClrKeyInfo(KeyInfo);				  
	}	
}
/********************************************************************************
**  Function      : Key_Scan_AD
**  Author        : wenjunHu
**  Created on    : 20170803
**  Description   : ADC Key Function
**  Return        : void
********************************************************************************/

void Key_Scan_AD2(SCH_U8 ad_ch, SCH_U8 Max_Cnt, KEY_AD_DATA *pTable, KEY_INFO *KeyInfo)
{
	SCH_U8 index;			///
  SCH_U16 ADC_Value; 
	if(!ADC_GetData(ad_ch,&ADC_Value))
		return;	
	
	//if(KeyInfo->KeyStatus == Key_Up && IsInData(ADC_Value,ADC_DATA_MAX,PANEL_STEP))
	//{
	//	return ;		
	//}
	
	
	for(index=0;index<Max_Cnt;index++)
    {
		if(IsInData(ADC_Value,pTable[index].KeyADC_Data,PANEL_STEP))
    	{
    		if(pTable[index].KeyStudyFlag != 0x02 && ad_ch == AD_SW2_CHANNEL)
				break;
			KeyInfo->KeyStatus = Key_Down;
			KeyInfo->ShortKeyCode = pTable[index].ShortKeyCode;
			KeyInfo->LongKeyCode  = pTable[index].LongKeyCode;
			KeyInfo->KeyProperty  = pTable[index].KeyProperty;
    		break;
    	}
    }
	if(index == Max_Cnt)
		KeyInfo->KeyStatus = Key_Up;
	{
		KeyScan(&AD_SW2_Info);
		KeyTimer(&AD_SW2_Info);
	}

}

/********************************************************************************
**  Function      : Key_Scan_AD
**  Author        : wenjunHu
**  Created on    : 20170803
**  Description   : ADC Key Function
**  Return        : void
********************************************************************************/
void Key_Scan_AD(SCH_U8 ad_ch, SCH_U8 Max_Cnt, KEY_AD_DATA *pTable, KEY_INFO *KeyInfo)
{
	SCH_U8 index;			///
    SCH_U16 ADC_Value; 
	if(!ADC_GetData(ad_ch,&ADC_Value))
		return;	
	
	//if(KeyInfo->KeyStatus == Key_Up && IsInData(ADC_Value,ADC_DATA_MAX,PANEL_STEP))
	//{
	//	return ;		
	//}
	
	
	for(index=0;index<Max_Cnt;index++)
    {
		if(IsInData(ADC_Value,pTable[index].KeyADC_Data,PANEL_STEP))
    	{
    		if(pTable[index].KeyStudyFlag != 0x01 && ad_ch == AD_SW1_CHANNEL)
				break;
			KeyInfo->KeyStatus = Key_Down;
			KeyInfo->ShortKeyCode = pTable[index].ShortKeyCode;
			KeyInfo->LongKeyCode  = pTable[index].LongKeyCode;
			KeyInfo->KeyProperty  = pTable[index].KeyProperty;
    		break;
    	}
    }
	if(index == Max_Cnt)
		KeyInfo->KeyStatus = Key_Up;
	{
		KeyScan(&AD_SW_Info);
		KeyTimer(&AD_SW_Info);
	}

}

void AD_SW2_Key_Scan(void)
{
	if(AD_SW_Info.StudyState == STUDY_OK || AD_SW2_Info.StudyState == STUDY_OK)
	{
		Key_Scan_AD(AD_SW1_CHANNEL,AD_SW_MAX_NUM,AD_SW_Tab,&AD_SW_Info);
		Key_Scan_AD2(AD_SW2_CHANNEL,AD_SW_MAX_NUM,AD_SW_Tab2,&AD_SW2_Info);
	}
	else if(AD_SW2_Info.StudyState == STUDY_ING)
	{
		if(AD_SW2_Info.StudyKeyCode)
		{
			if(GetKeyData(AD_SW1_CHANNEL,&AD_SW_Tab[AD_SW2_Info.StudyNum].KeyADC_Data))
			{
				//PostMessage(ARM_MODULE,M2A_STUDY_KEY_CMD,SCH_WORD(AD_SW2_Info.StudyKeyCode,0x03));
				AD_SW_Tab[AD_SW2_Info.StudyNum].KeyStudyFlag = 1;
				AD_SW2_Info.StudyKeyCode = 0;
				//AD_SW2_Info.StudyState = STUDY_OK;
				ReportSWCTable();
			}
					
			if(GetKeyData2(AD_SW2_CHANNEL,&AD_SW_Tab2[AD_SW2_Info.StudyNum].KeyADC_Data))
			{
				//PostMessage(ARM_MODULE,M2A_STUDY_KEY_CMD,SCH_WORD(AD_SW2_Info.StudyKeyCode,0x03));
				AD_SW_Tab2[AD_SW2_Info.StudyNum].KeyStudyFlag = 2;
				AD_SW2_Info.StudyKeyCode = 0;
				//AD_SW2_Info.StudyState = STUDY_OK;
				ReportSWCTable();
			}
		}
	}
	else if(AD_SW2_Info.StudyState == NO_STUDY)
	{
		///KeyScan1();
		ScanSwckey();
	}
}

//=================================================================
//Function Nam: SwcInit()
//Descript:
//=================================================================
void SwcInit(void)
{
	scanswckeyStatus = SCANSWCKEY_IDLE;
}
//=================================================================
//Function Nam: ProcessSwc()
//Descript:
//=================================================================
void ProcessSwc(void)
{
	AD_SW2_Key_Scan();
}

//=================================================================
//Function Nam: ScanSwckey()
//Descript:
//=================================================================
void ScanSwckey(void)
{
	static u8 swckeyreadlast;
	static u8 bswclongpresstime;
	static u8 bswckeymem;
	
	u8 gkey;
	
	if (!bswc10ms)		return;
	if (!accexistFlag)	return;
	
	gkey = adcvalueBuff[4] >> 2;
	switch(scanswckeyStatus)
	{
		case SCANSWCKEY_IDLE:
			if (gkey < ISSWCKEY_VALUE)
			{
				scanswckeyStatus = SCANSWCKEY_PRESS1;
				bswclongpresstime = reset;
				swckeyreadlast = gkey;
			}
			break;
		case SCANSWCKEY_PRESS1:
			if (gkey == swckeyreadlast)
			{
				scanswckeyStatus = SCANSWCKEY_LONG;
				bswckeymem = gkey;
			}
			else
			{
				scanswckeyStatus = SCANSWCKEY_IDLE;
			}
			break;
		case SCANSWCKEY_LONG:
			bswclongpresstime++;
			if (bswckeymem <= ISSWCKEY_LEVEL2)
			{
				if (bswclongpresstime > 50)
				{
					bswclongpresstime = 40;
					if (bswckeymem <= ISSWCKEY_LEVEL1)
						SaveEncodekey(KEY_SYS_VOLDN);
					else
						SaveEncodekey(KEY_SYS_VOLUP);
				}
			}
			if (bswclongpresstime > 120)
			{
				bswclongpresstime = reset;
				Saveswckey(bswckeymem,1);				// long press
				scanswckeyStatus = SCANSWCKEY_RELEASE;
			}
			if (gkey >= ISSWCKEY_VALUE)
			{
				Saveswckey(bswckeymem,0);				// short press
				scanswckeyStatus = SCANSWCKEY_RELEASE;
			}
			break;
		case SCANSWCKEY_RELEASE:
			if (gkey >= ISSWCKEY_VALUE)
			{
				scanswckeyStatus = SCANSWCKEY_IDLE;
			}
			break;
		default:
			scanswckeyStatus = SCANSWCKEY_IDLE;
			break;
	}

}

void Saveswckey(u8 keyad,u8 presstype)
{
	if (keyad <= ISSWCKEY_LEVEL1)			// vol-
	{
		SaveEncodekey( KEY_SYS_NEXT);
	}
	else if (keyad <= ISSWCKEY_LEVEL2)		// vol+
	{
		SaveEncodekey(KEY_SYS_PREV);
	}
	else if (keyad <= ISSWCKEY_LEVEL3)		// next
	{
		SaveEncodekey(KEY_SYS_VOLUP);
	}
	else if (keyad <= ISSWCKEY_LEVEL4)		// mode
	{
		SaveEncodekey(KEY_SYS_VOLDN);
	}
	else if (keyad <= ISSWCKEY_LEVEL5)		// prev
	{
		SaveEncodekey(KEY_SYS_BT);
	}
	else if (keyad <= ISSWCKEY_LEVEL6)		// mute/phone off
	{
		SaveRecombinationkey(KEY_SYS_RECOMBINATION_PHONEOFF);
	}
	else if (keyad <= ISSWCKEY_LEVEL7)		// phone on
	{
		SaveRecombinationkey(KEY_SYS_RECOMBINATION_PHONEON);
	}
}

