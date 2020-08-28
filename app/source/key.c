/*
***************************************************************************************
                                     		 Key

                       (c) Copyright Mesada Technology Co., Ltd.
                                 All Rights Reserved
	File		: key.c
	By			: wanyuedong
	Version	: v1.00
***************************************************************************************
*/

//=========================== Includes ===========================

#include "config.h"



u8 scanpanelkeyLeftStatus;
u8 scanpanelkeyRightStatus;

u8 bencodekeydisflag;		// 如果power按键按下，不解码声音旋钮
u8 volEncodeCurrentState;
u8 tunEncodeCurrentState;

u8 bpowerkey;
u8 bwaitpowerkey;
u8 rKeyResult;

KEYBUFF_TYPE keybuff[KEYSAVE_MAX];
u8 keysaveindex;
u8 keygetindex;
u8 bpowerkeypresstime;


//=================================================================
//Function Nam: KeyInit()
//Descript:
//=================================================================
void KeyInit(void)
{
	keysaveindex = reset;
	keygetindex = reset;
	bpowerkey = reset;
	bwaitpowerkey = reset;
	bpowerkeypresstime = reset;
}
//=================================================================
//Function Nam: ProcessKey()
//Descript:
//=================================================================
void ProcessPanelKey(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	ScanKey();
    if (keygetindex == keysaveindex)		return;
	if ((core.applink) && (HasAcc()))
		TransmitKey();	

	keybuff[keygetindex].flag = reset;
	keygetindex++;
	keygetindex &= (u8)KEYSAVE_SUB;
}
//=================================================================
//Function Nam: ScanKey()
//Descript:
//=================================================================
void ScanKey(void)
{
	if (!HasAcc())		return;
	ScanPanelkey();
	ScanVolEncodekey();
	//ScanTunEncodekey();
}
//=================================================================
//Function Nam: ScanPanelkey()
//Descript:
//=================================================================
void ScanPanelkey(void)
{
	static u8 panelleftkeyold,panelrightkeyold;
	static u8 bleftlongpresstime,brightlongpresstime;
	static u8 bleftkeylongmem,brightkeylongmem;
	static u8 bflashLevelCntL,bflashLevelCntH;
	static u8 bvolumkeyFlag;
	static u8 bvollongpresstime;
	
	u8 gkey;
	
	if (!bkey10ms)		return;
	if (!accexistFlag)	return;
	
	#ifdef USE_SKEY
	return;
	#endif
	gkey = adcvalueBuff[0] >> 2;
	switch(scanpanelkeyLeftStatus)
	{
		case SCANPANELKEY_IDLE:
			if (gkey < ISPANELKEY_VALUE)
			{
				scanpanelkeyLeftStatus = SCANPANELKEY_PRESS1;
				bleftlongpresstime = reset;
				panelleftkeyold = gkey;
			}
			break;
		case SCANPANELKEY_PRESS1:
			if (gkey == panelleftkeyold)
			{
				scanpanelkeyLeftStatus = SCANPANELKEY_PRESS2;
			}
			else
				scanpanelkeyLeftStatus = SCANPANELKEY_IDLE;
			break;
		case SCANPANELKEY_PRESS2:
			if (gkey == panelleftkeyold)
			{
				scanpanelkeyLeftStatus = SCANPANELKEY_LONG;
				bleftlongpresstime = reset;
				bleftkeylongmem = gkey;
				bvolumkeyFlag = reset;
			}
			else
			{
				scanpanelkeyLeftStatus = SCANPANELKEY_IDLE;
			}
			break;
		case SCANPANELKEY_LONG:
			bleftlongpresstime++;
			if (bleftlongpresstime > 120)
			{
				//bleftlongpresstime = reset;
#if 0
				if (gkey <= ISPANELKEY_LEVEL0)
				{
					scanpanelkeyLeftStatus = SCANPANELKEY_RELEASE_SYSUPDATE;
					break;
				}
				else
#endif
					
				{
					Savekey(bleftkeylongmem,AIN0,1);				// long press
				}
				scanpanelkeyLeftStatus = SCANPANELKEY_RELEASE;
			}
			
			if (gkey >= ISPANELKEY_VALUE)
			{
				if (bvolumkeyFlag)
				{
					
				}
				else
				{
					Savekey(bleftkeylongmem,AIN0,0);			// short press
				}
				scanpanelkeyLeftStatus = SCANPANELKEY_RELEASE;
			}
			break;
		case SCANPANELKEY_RELEASE_SYSUPDATE:
			if (gkey >= ISPANELKEY_VALUE)
			{
				scanpanelkeyLeftStatus = SCANPANELKEY_IDLE;
			}
			if (gkey <= ISPANELKEY_LEVEL0)
			{
				if (core.oslink)
				{
					ReportARM2UPDate();
                    scanpanelkeyLeftStatus = SCANPANELKEY_RELEASE;
				}
			}
			break;
		case SCANPANELKEY_RELEASE:
			if (gkey >= ISPANELKEY_VALUE)
			{
				scanpanelkeyLeftStatus = SCANPANELKEY_IDLE;
				if (bleftlongpresstime > 120)
				{
					
					bleftlongpresstime = reset;
					SaveLongkeyRelease(0xff,AIN0,1);          // long press release send 0xff for long key end flag
				}
			}
			break;
		default:
			scanpanelkeyLeftStatus = SCANPANELKEY_IDLE;
			break;
	}
#if 0
	gkey = adcvalueBuff[1] >> 2;

	switch(scanpanelkeyRightStatus)
	{
		case SCANPANELKEY_IDLE:
			if (gkey < ISPANELKEY_VALUE)
			{
				scanpanelkeyRightStatus = SCANPANELKEY_PRESS1;
				panelrightkeyold = gkey;
				//gpslongpresstime = reset;
			}
			if (bencodekeydisflag)
				bencodekeydisflag = reset;
			break;
		case SCANPANELKEY_PRESS1:
			if (gkey == panelrightkeyold)
			{
				scanpanelkeyRightStatus = SCANPANELKEY_PRESS2;
			}
			else
				scanpanelkeyRightStatus = SCANPANELKEY_IDLE;
			break;
		case SCANPANELKEY_PRESS2:
			if (gkey == panelrightkeyold)
			{
				bencodekeydisflag = set;
				scanpanelkeyRightStatus = SCANPANELKEY_LONG;
				brightlongpresstime = reset;
				brightkeylongmem = gkey;
			}
			else
			{
				scanpanelkeyRightStatus = SCANPANELKEY_IDLE;
			}
			break;
		case SCANPANELKEY_LONG:
			brightlongpresstime++;
			if (brightlongpresstime > 120)
			{
				Savekey(panelrightkeyold,AIN1,1);			//long press
				scanpanelkeyRightStatus = SCANPANELKEY_RELEASE;
			}
			if (gkey >= ISPANELKEY_VALUE)
			{
				Savekey(panelrightkeyold,AIN1,0);			//short press
				scanpanelkeyRightStatus = SCANPANELKEY_RELEASE;
			}
			break;

		case SCANPANELKEY_RELEASE:
			if (gkey >= ISPANELKEY_VALUE)
			{
				scanpanelkeyRightStatus = SCANPANELKEY_IDLE;
				break;
			}
			break;
		default:
			scanpanelkeyRightStatus = SCANPANELKEY_IDLE;
			break;
	}
#endif
}
//=================================================================
//Function Nam: ScanVolEncodekey()
//Descript:
//=================================================================
void ScanVolEncodekey(void)
{
	static u8 encodekeyold;
	static u8 gkey1,gkey2;

	encodekeyold++;
	if (encodekeyold & 1)
	{
		gkey1 = adcvalueBuff[2] >> 2;
		return;
	}
	else
	{
		gkey2 = adcvalueBuff[2] >> 2;

		if (gkey1 != gkey2)		return;
	}
	//if(gkey1 != gkey2)
	//Printf("\r\n vol:k1=%d,k2=%d",gkey1,gkey2);
	if (gkey1 > VOL_LEVEL3_MAX)
		volEncodeCurrentState = 3;
	else if (gkey1 < VOL_LEVEL0_MAX)
		volEncodeCurrentState = 1;
	else if (gkey1 > VOL_LEVEL1_MAX)
	{
		if (volEncodeCurrentState == 3)
		{
			volEncodeCurrentState = reset;
			if (!bencodekeydisflag)
				{
				SaveEncodekey(VOLKEY_ARM_DEC);
				
				//Printf("\r\n vol:INC1 k1=%d,k2=%d",gkey1,gkey2);
				}
			
		}
		else if (volEncodeCurrentState == 1)
		{
			volEncodeCurrentState = reset;
			if (!bencodekeydisflag)
				{
				SaveEncodekey(VOLKEY_ARM_INC);
				//Printf("\r\n vol:DEC1 k1=%d,k2=%d",gkey1,gkey2);
				}
		}
	}
	else if (gkey1 < VOL_LEVEL1_MAX)
	{
		if (volEncodeCurrentState == 3)
		{
			volEncodeCurrentState = reset;
			if (!bencodekeydisflag)
				{
				SaveEncodekey(VOLKEY_ARM_INC);
				//Printf("\r\n vol:INC2 k1=%d,k2=%d",gkey1,gkey2);
				}
		}
		else if (volEncodeCurrentState == 1)
		{
			volEncodeCurrentState = reset;
			if (!bencodekeydisflag)
				{
				SaveEncodekey(VOLKEY_ARM_DEC);
				//Printf("\r\n vol:DEC2 k1=%d,k2=%d",gkey1,gkey2);
				}
		}
	}
}
//=================================================================
//Function Nam: ScanTunEncodekey()
//Descript:
//=================================================================
void ScanTunEncodekey(void)
{
	static u8 encodekeyold;
	static u8 tkey1,tkey2;

	encodekeyold++;
	if (encodekeyold & 1)
	{
		//tkey1 = adcvalueBuff[2] >> 2;
		return;
	}
	else
	{
		//tkey2 = adcvalueBuff[2] >> 2;

		if (tkey1 != tkey2)		return;
	}

	if (tkey1 > TUN_LEVEL3_MAX)
		tunEncodeCurrentState = 3;
	else if (tkey1 < TUN_LEVEL0_MAX)
		tunEncodeCurrentState = 1;
	else if (tkey1 > TUN_LEVEL1_MAX)
	{
		if (tunEncodeCurrentState == 3)
		{
			tunEncodeCurrentState = reset;
			SaveEncodekey(TUNKEY_ARM_INC);
		}
		else if (tunEncodeCurrentState == 1)
		{
			tunEncodeCurrentState = reset;
			SaveEncodekey(TUNKEY_ARM_DEC);
		}
	}
	else if (tkey1 < VOL_LEVEL1_MAX)
	{
		if (tunEncodeCurrentState == 3)
		{
			tunEncodeCurrentState = reset;
			SaveEncodekey(TUNKEY_ARM_DEC);
		}
		else if (tunEncodeCurrentState == 1)
		{
			tunEncodeCurrentState = reset;
			SaveEncodekey(TUNKEY_ARM_INC);
		}
	}
}

//=================================================================
//Function Nam: Savekey(u8,u8)
//Descript:
//=================================================================
void Savekey(u8 gkey,u8 ch,u8 flag)
{
	u8 index;

	if (!accexistFlag)			return;

	keybuff[keysaveindex].key = KEY_SYS_NULL;

	if (flag == reset)			//short press
	{
		if (ch == AIN1)			//ADKEY2
		{
			if (gkey <= ISPANELKEY_LEVEL0)				// EJECT
			{
				keybuff[keysaveindex].key = KEY_SYS_MUTE;
			}
			else if (gkey <= ISPANELKEY_LEVEL1)			// MENU
			{
				keybuff[keysaveindex].key = KEY_SYS_RADIO;
			}
			else if (gkey <= ISPANELKEY_LEVEL2)			// PLAY/PAUSE
			{
//				keybuff[keysaveindex].key = KEY_DOOR_RIGHT;
			}
			else if (gkey <= ISPANELKEY_LEVEL3)			// SEEK+
			{
//				keybuff[keysaveindex].key = KEY_DOOR_LEFT;
			}
		}
		else					//ADKEY1
		{
			if (gkey <= ISPANELKEY_LEVEL0)			// POWER
			{
				if (bwaitpowerkey)
					keybuff[keysaveindex].key = KEY_SYS_POWER;
				else
					keybuff[keysaveindex].key = KEY_SYS_MUTE;
			}
			else if (gkey <= ISPANELKEY_LEVEL1)		// BACK
			{
				keybuff[keysaveindex].key = KEY_SYS_NAV;
			}
			else if (gkey <= ISPANELKEY_LEVEL2)		// SETUP
			{
				keybuff[keysaveindex].key = KEY_SYS_HOME;
			}
			else if (gkey <= ISPANELKEY_LEVEL3)		// HOME
			{
				keybuff[keysaveindex].key = KEY_SYS_MODE;
			}
			else if (gkey <= ISPANELKEY_LEVEL4)		// SETUP
			{
				keybuff[keysaveindex].key = _kcCamera;
			}
			else if (gkey <= ISPANELKEY_LEVEL5)		// NEXT
			{
				keybuff[keysaveindex].key = KEY_SYS_NEXT;
			}
			else if (gkey <= ISPANELKEY_LEVEL6)		// PRE
			{
				keybuff[keysaveindex].key = KEY_SYS_PREV;
			}
		}
	}
	else						// long press
	{
		if (ch == AIN1)			//ADKEY2
		{
		    if (gkey <= ISPANELKEY_LEVEL0)
		    {

			}
			else if (gkey <= ISPANELKEY_LEVEL1)
			{

			}
			else if (gkey <= ISPANELKEY_LEVEL2)
			{

			}
		  else if (gkey <= ISPANELKEY_LEVEL3)
			{
			}	
		}
		else
		{
			  if (gkey <= ISPANELKEY_LEVEL0)
			  {
				keybuff[keysaveindex].key = KEY_SYS_POWER;
			  }
			  else if (gkey <= ISPANELKEY_LEVEL1)
			  {
			
			  }
			  else if (gkey <= ISPANELKEY_LEVEL2)
			  {
			
			  }
			  else if (gkey <= ISPANELKEY_LEVEL3)
			  {
			  
			  }
			  else if (gkey <= ISPANELKEY_LEVEL4)
			  {
			
			  }
			  else if (gkey <= ISPANELKEY_LEVEL5)
			  {
				  keybuff[keysaveindex].key = KEY_SYS_FASTF;
			  }
			  else if (gkey <= ISPANELKEY_LEVEL6)
			  {
				  keybuff[keysaveindex].key = KEY_SYS_FASTB;
			  }

				
		}
	}
	if (keybuff[keysaveindex].key == KEY_SYS_NULL)
		return;
	keybuff[keysaveindex].flag = set;
	keysaveindex++;
	keysaveindex &= (u8)(KEYSAVE_MAX - 1);
}

void SaveLongkeyRelease(u8 gkey,u8 ch,u8 flag)
{
	u8 index;

	if (!accexistFlag)			return;

	keybuff[keysaveindex].key = gkey;

	if (keybuff[keysaveindex].key == KEY_SYS_NULL)
		return;
	keybuff[keysaveindex].flag = set;
	keysaveindex++;
	keysaveindex &= (u8)(KEYSAVE_MAX - 1);
}

//=================================================================
//Function Nam: SaveEncodekey(u8)
//Descript:
//=================================================================
void SaveEncodekey(u8 gkey)
{
	keybuff[keysaveindex].key1 = reset;
	keybuff[keysaveindex].key = gkey;
	keybuff[keysaveindex].flag = set;

	keysaveindex++;
	keysaveindex &= (u8)(KEYSAVE_MAX - 1);
}
//=================================================================
//Function Nam: SaveRecombinationkey(u8)
//Descript:
//=================================================================
void SaveRecombinationkey(u8 gkey)
{
	keybuff[keysaveindex].key1 = gkey;
	keybuff[keysaveindex].key = 0x90;
	keybuff[keysaveindex].flag = set;

	keysaveindex++;
	keysaveindex &= (u8)(KEYSAVE_MAX - 1);
}

//=================================================================
//Function Nam: TransmitKey()
//Descript:
//=================================================================
void TransmitKey(void)
{
	u8 transmitbuff[20],check,index;

	check = reset;
	index = reset;
	
	transmitbuff[index++] = 0x02;
	transmitbuff[index] = 0x81;	check ^= transmitbuff[index++];
	transmitbuff[index] = 0xB2;	check ^= transmitbuff[index++];
	transmitbuff[index] = 0x06;	check ^= transmitbuff[index++];
	transmitbuff[index] = 0x00;		//key status
	check ^= transmitbuff[index++];
	transmitbuff[index] = keybuff[keygetindex].key;
	check ^= transmitbuff[index++];
	
	{
		transmitbuff[index] = keybuff[keygetindex].key1; check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
		transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
	}


	transmitbuff[index] = 0x00; check ^= transmitbuff[index++];
	
	transmitbuff[index++] = check;
	ArmUartSenstr(transmitbuff,index);
	//Uart1SendStr(transmitbuff,index);
}

