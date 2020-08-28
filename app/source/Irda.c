#define		Irda_ROOT
#include "config.h"

#define 	TOTAL_KEYS		0x100

uint16_t m_iIrCode;
uint8_t m_iCodeType;
#define REPET_EN_BREAK 1

#define IRDA_NUM	2
uchar g_IrDAKeyMap[][IRDA_NUM] =
{
    { 0x0B, KEY_SYS_UP  	},  // power
	{ 0x0C, KEY_SYS_DOWN},	// mode
    { 0x0f, _kcCamera			},  // photo
	//{ 0x01, KEY_SYS_UP		},	// up
	{ 0x1b, KEY_SYS_BT},	// photo
	
    { 0x4A, KEY_SYS_HOME},  // left
	{ 0x13, KEY_SYS_ENTER	},	// enter 

    { 0x14, KEY_SYS_NAV},  // ESC/Stop
	{ 0x07, KEY_SYS_DOWN},	// down
	{ 0x08, KEY_SYS_TUNERIGHT 	},	// goto
	{ 0x09, KEY_SYS_TUNELEFT},	// photo

    { 0x0a, KEY_SYS_MUTE},  // movie
    { 0x0d,KEY_SYS_MODE },  // 快退
	//{ 0x16, KEY_SYS_EQ},	// 快退
		    
    { 0x01, IR_NUM1     },  // 1  
    { 0x02, IR_NUM2     },  // 2
    { 0x03, IR_NUM3     },  // 3
    { 0x04, IR_NUM4     },  // 4
	{ 0x05, IR_NUM5     },  // 5
    { 0x06, IR_NUM6     },  // 6

	{ 0x16, _kcAudio1  },  // 16:9/4:3
    { 0x11, _kcAudio2     },  // end 
};

uchar g_SWIrDAKeyMap[][IRDA_NUM] =
{
    { 0x16, KEY_SYS_MUTE},  // 
	{ 0x13, KEY_SYS_MODE},	// 
    { 0x0B, KEY_SYS_NEXT},  // 
	{ 0x0A, KEY_SYS_PREV},	// 
    { 0x0E, KEY_SYS_PLAY},  // 
	{ 0x14, KEY_SYS_VOLUP},	// 
	{ 0x15, KEY_SYS_VOLDN},	//

	
    { 0x88, 0xd0},  // 
	{ 0x98, 0xd1},	// 
    { 0xa1, 0xd2},  // 
	{ 0xa2, 0xd3},	// 
    { 0xb1, 0xd4},  // 
	{ 0xb2, 0xd5},	// 
	{ 0xb3, 0xd6},	//

	
    { 0xb4, 0xd7},  // 
	{ 0xb5, 0xd8},	// 
    { 0xb6, 0xd9},  // 
	{ 0xb7, 0xda},	// 
	{ 0xc0, 0xdb},	// 
    { 0xc1, 0xdc},  //
    
	//{ 0xb2, KEY_SYS_VOLUP},	// 
	//{ 0xb3, KEY_SYS_VOLDN},	//
};

uint8_t IS_IR_HIGH()
{
	return iIR_IN;
}

uint8_t IS_IR_LOW()
{
	return !(iIR_IN);
}

int8_t Delay_us_break(u16 us)
{
	#if 0
  	u16 TIMCounter = (u16)(us);
//  	TIM_Cmd(TIM4, ENABLE);
 // 	TIM_SetCounter(TIM4, TIMCounter);
  	while (TIMCounter > 1)
  	{
  //  	TIMCounter = TIM_GetCounter(TIM4);
  		if(pfnBreak)
  		{
		    if(pfnBreak())
		    {
		//        TIM_Cmd(TIM4, DISABLE);
		        return -1;
		    }
  		}
  	}
  //	TIM_Cmd(TIM4, DISABLE);
	#endif
  	return 0;
}

void IR_EXTI8_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
#if 1
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Initializes the EXTI_InitStructure */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;//|EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;//|EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(/*EXTI_Line0 |*/ EXTI_Line1 /*|EXTI_Line4*/);

#endif
}

void IR_EXTI7_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
#if 1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Initializes the EXTI_InitStructure */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;//|EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;//|EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(/*EXTI_Line0 |*/ EXTI_Line7 /*|EXTI_Line4*/);

#endif
}

void Timer2Init()
{
#if 1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef		 NVIC_InitStructure;
	//使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道被使能
	//NVIC_Init(&NVIC_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = 100;//自动重装载值10--定时时间(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler = 16-1;//预分频值，+1为分频系数 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//使能TIM2中断源
	TIM_Cmd(TIM2, ENABLE); 	//使能TIMx外设	
#endif
}

void IrdaInit()
{
	Timer2Init();
	IR_EXTI7_Config();
	IR_EXTI8_Config();
}

void IrdaDeInit()
{
	NVIC_InitTypeDef		 NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Initializes the EXTI_InitStructure */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;//|EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;//|EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(/*EXTI_Line0 |*/ EXTI_Line1 /*|EXTI_Line4*/);

}
#if 0
int8_t IRDecoder()
{
//	uint8_t i = 0;
//	uint16_t TIMCounter = 0;
//	uint8_t addr1 = 0, addr2 = 0;
//	uint8_t code1 = 0, code2 = 0;

  #if 1
	switch(rIrdRecSt)
	{
		case 0:
			rIrdRecSt++;
			TIM_SetAutoreload(TIM4,0XFFFF);
			TIM_SetCounter(TIM4, 0x0);
		    	TIM_Cmd(TIM4, ENABLE);
			//TIMCounter = TIM_GetCounter(TIM4);
			break;
			
		case 1:
			TIM_Cmd(TIM4, DISABLE);
			//TIMCounter = 
		  TIM_GetCounter(TIM4);
			rIrdRecSt = 0;
			break;
			
		case 2:
			rIrdRecSt = 0;
			break;
			
	}
  #else
	// 1. 等待8.6ms低电平
	if(0 != Delay_us_break(8500, IS_IR_HIGH))  //8500
	{
		#if REPET_EN_BREAK
		#else
		m_iIrCode = -1;
		#endif
		return -1;
	}
	
reget:
	//usWait(IR_TIMER, 400, NULL);
	//等到9ms低电平变高。已经由上面的延时跳过
	while(IS_IR_LOW());

	// 2. 判断是否为重复码.在1.8ms内出现低电平则为无效码
	if(0 != Delay_us_break(1600, IS_IR_LOW))
	{
		#if REPET_EN_BREAK
		#else
		m_iIrCode = -1;
		#endif
		return -2;
	}
	
	//在800us内出现低电平则为重复码。
	m_iCodeType = CODE_NEW;
	if(0 != Delay_us_break(1000, IS_IR_LOW))
		m_iCodeType = CODE_REPEAT;
	
	if(m_iCodeType == CODE_REPEAT)  //是重复码则退出
	{
		//while(IS_IR_LOW());  // 等待停止信号结束.//8500  7500
     	//return 0;
	  	if(-1 != Delay_us_break(7500, IS_IR_HIGH))//判断是否为起始条件
	  	{
	     	m_iIrCode = -1;
	     	goto reget;
	  	}
		return 0;
	}
	else	// 否则等待变低(数据)
	{
		while(IS_IR_HIGH());
	}
				
    // 读取地址码原码
	addr1 = 0;
	for (i=0; i<8; i++)
    {
		while(IS_IR_LOW());  // 等到数据的低电平过去.
		if(0!=Delay_us_break(1000, IS_IR_LOW))	//等待800us
		{
			//表示得到一个'L'
			//addr1 |= 0<<i;
		}
		else 		// 如果是高则表示收到一个'H'
		{
			addr1 |= 1<<i;
			while(IS_IR_HIGH());		// 等待此H' 过去。
		}
    }
		
    // 读取地址码反码
    addr2 = 0;
	for (i=0; i<8; i++)
    {
		while(IS_IR_LOW());  // 等到数据的低电平过去.
		if(0!=Delay_us_break(1000, IS_IR_LOW))	//等待800us
		{
			//表示得到一个'L'
			//addr2 |= 0<<i;
		}
		else 		// 如果是高则表示收到一个'H'
		{
			addr2 |= 1<<i;
			while(IS_IR_HIGH());		// 等待此H' 过去。
		}
    }	
	
    // 检查地址码是否有效
	if((addr1 != (0xFF-addr2+0x01))&&(addr1 != (0xFF-addr2)))
    {
		m_iIrCode = -1;
		//m_iIrCode = addr1<<8 | addr2;	
		return -3;
    }

    // 读取用户码原码
    code1 = 0;
	for (i=0; i<8; i++)
    {
		while(IS_IR_LOW());  // 等到数据的低电平过去.
		if(0!=Delay_us_break(1000, IS_IR_LOW))	//等待800us
		{
			//表示得到一个'L'
			//code1 |= 0<<i;
		}
		else 		// 如果是高则表示收到一个'H'
		{
			code1 |= 1<<i;
			while(IS_IR_HIGH());		// 等待此H' 过去。
		}
    }	
	
    // 读取用户码反码
    code2 = 0;
	for (i=0; i<8; i++)
    {
		while(IS_IR_LOW());  // 等到数据的低电平过去.
		if(0!=Delay_us_break(1000, IS_IR_LOW))	//等待800us
		{
			//表示得到一个'L'
			//code2 |= 0<<i;
		}
		else 		// 如果是高则表示收到一个'H'
		{
			code2 |= 1<<i;
			while(IS_IR_HIGH());		// 等待此H' 过去。
		}
    }	
	
	while(IS_IR_LOW());  // 等待停止信号结束.
	
    // 检查用户码是否有效
    if (code1 != (0xFF-code2))
    {
		m_iIrCode = -1;
		//m_iIrCode  = code1<<8 | ~code2;;
		return -4;
    }
	
    // 根据地址码和用户码生成按键代码
    m_iIrCode = (addr1 << 8) | (code1 << 0);
	#endif
	return 0;	
	
}
#endif


void IrdaIntrProbe()
{
#if 0
	static u16 ReptCnt = 0;
	int8_t ret = IRDecoder(); 

	#if Apk_Debug
	//Printf("\r\n IrdaIntrProbe:%d--0x%x\r\n",ret,m_iIrCode);
	#endif
	if(m_iCodeType == CODE_NEW)
		ReptCnt = 0;
	else
		ReptCnt++;
	
  	if(ret == 0) //数据有效
  	{
		if((m_iCodeType == CODE_NEW)||((m_iCodeType == CODE_REPEAT)&&((ReptCnt%4) == 0)))
		{
			doIrdaFunc();
		}
  	}
	
	EXTI_ClearITPendingBit(EXTI_Line8);
#endif
}




/*****************************************************************************
        Function        : Decode remote code.
        Output		: bRmtCodeRdy or bRmtRptKey
        Description     : 1. Falling edge on INT3(P4.6,pin24) cause interruption occured.
                          2. TC3 source clock cycle 128us   
                          3. LSB first.   
----------------------------------------------------------------------------*/
void    RemoteInt( )
{
	//unsigned char A,W;
	unsigned int A,Rmt_time_cnt;
	TIM_Cmd(TIM2, DISABLE);
	Rmt_time_cnt = TIM_GetCounter(TIM2);
			
	/* remote head code signal check */
	if(Rmt_time_cnt > _LeadCodeMax)
	{/* restart TC3 */
		Remote_cnt_st = 1;
		Rmt_time_cnt = 0;
	}
	else	if(Rmt_time_cnt>_LeadCodeMin /*&& rRmtSt == _RmtWtHeadCode*/)
	{/* _LeadCodeMin < pulse width< _LeadCodeMax , lead code*/
		bRmtCodeRdy = False;
		bRmtRptKey = False;
		cRmtDataBits = 0;
		Rmt_X4ms = 80/4;//140/5;/*max time is 76.3ms 140/4*/
		Rmt_time_cnt = 0;
		rRmtSt = _RmtWtFrameEnd;
	}
	/* remote repeat code signal check */
	else	if(Rmt_time_cnt>_RptCodeMin)
	{/* _RptCodeMin < pulse width< _RptCodeMax , repeat code*/
		bRmtRptKey = True;
		Remote_cnt_st = 1;
		Rmt_time_cnt = 0;
	}
	/* remote custom/data code signal check */
	else	if(rRmtSt==_RmtWtFrameEnd && Rmt_time_cnt>_Data0Min && Rmt_time_cnt<_Data1Max)
	{/* _Data0Min < width of the Pulse < _Data1Max, Data 0 or 1 */
        	if(Rmt_time_cnt < _Data0Max) bRemoteData = 0; // This bit is 0
        	else 	bRemoteData = 1; // This bit 1
        	// Save received bit
        	A = RxRemoteBuff[3] >> 1;
        	if(bRemoteData) A |= 0x80;
        	else A &= 0x7f;
        	RxRemoteBuff[3] = A;

        	A = (++cRmtDataBits) & 0x7; // 8 bits(One byte received)?

		if(A == 0)
		{
    		A = (cRmtDataBits-1)>>3;
    		RxRemoteBuff[A] = RxRemoteBuff[3];
    		if(cRmtDataBits > _TotalBits-1)
    		{// Rx A frame data end
    	    	bRmtCodeRdy = True; // Generate reomte ready message
    	    	cRmtDataBits = 0;   // Reset remote Rx counter
				Remote_cnt_st = 0;
				Rmt_time_cnt = 0xffff;
    		}
		}
	Rmt_time_cnt = 0;
	}
	#if 1
	TIM_SetAutoreload(TIM2,0XFFFF);
	TIM_SetCounter(TIM2, 0x0);
    TIM_Cmd(TIM2, ENABLE);
	#endif

}

/*------------------------------------------------------------------------*\
;       Function   : convert remote code to system key No..
;       Input      : 
;       Output     : bNewKey,rKeyResult.
;       Description:
;------------------------------------------------------------------------*/
unsigned char	const 	RmtKyReformTab[ ]=
{
#if 0

	SYS_KEY_POWER,
	0x00

	_kcPower,		/* 00h: */
	_kcSeekInc,		/* 01h: */
	_kcVolInc,  	/* 02h: */
	_kcScan,    	/* 03h: */
                	
	_kcRdM1,		/* 04h: */
	_kcSeekDec,		/* 05h: */
	_kcVolDec,  	/* 06h: */
	_kcCD,      	/* 07h: */
                	
	_kcRdM2,		/* 08h: */
	_kcCDC,		/* 09h: */
	_kcSound,   	/* 0ah: */
#ifdef CFG_LC75817
#else
//	_kcCDCDC,       	/* 0bh: */20061031 lhs
#endif

	_kcRdM3,		/* 0ch: */
	_kcSource,		/* 0dh: */
	_kcLoud,    	/* 0eh: */
#ifdef CFG_LC75817
#else
//	_kcRadio,       	/* 0fh: */20061031 lhs
#endif

	_kcRdM4,		/* 10h: */
	_kcDispS,		/* 11h: */
#ifdef CFG_LC75817
#else
	_kcLocDx,   	/* 12h: */
#endif	
//	_kcAM,       	/* 13h: */20061031 lhs
                	
	_kcRdM5,		/* 14h: */
	_kcMute,    	/* 15h: */
#ifdef CFG_LC75817
#else
	_kcMono,    	/* 16h: */
#endif	
//	_kcFM,       	/* 17h: */20061031 lhs
                	
	_kcRdM6,		/* 18h: */
	_kcPS,      	/* 19h: */
	_kcTuneInc, 	/* 1ah: */
	0x00,       	/* 1bh: */
                	
	_kcBand,		/* 1ch: */
	_kcAudMode, 	/* 1dh: */
	_kcTuneDec, 	/* 1eh: */
#endif
	0x00        	/* 1fh: */
	
};

/*------------------------------------------------------------------------*\
;       Function   : reset for remote decode.
;       Input      : 
;       Description:
;------------------------------------------------------------------------*/
void	RstRmtDecode( )
{
	rRmtSt = _RmtWtHeadCode;
	bRmtCodeRdy = False;
	bRmtRptKey = False;
	bRmtKyRpting = False;
	Remote_cnt_st = 0;
	Rmt_time_cnt = 0xffff;
	return;
}
#if 1
uchar	SpKyCk(uchar rKyValue )
{
	switch(rKyValue)
	{
		case KEY_SYS_VOLUP:
		case KEY_SYS_VOLDN:
		case KEY_SYS_UP:
		case KEY_SYS_DOWN:
			return _OnlyShortKy;
			
		default:
			return _NormalKy;
			break;
	}

}

uchar	RptKyCk(uchar rKyValue)
{
	switch(rKyValue)
	{
		case KEY_SYS_VOLUP:
		case KEY_SYS_VOLDN:
			return 1;
			
		default:
			break;
	}
}

/*------------------------------------------------------------------------*\
;       Function   : Process remote data, Rxed by remote external interruption.
;       Input      : 
;       Output     : bNewKey,rKeyResult.
;       Description:
;------------------------------------------------------------------------*/
void    RemoteProc(void)
{
	unsigned char A = 0,rKyValue = 0,i,keylen = 0;
	
	switch(rRmtSt)
	{
	    case  _RmtWtHeadCode:/* wait remote head code */
			return;
	    
	    
	    case  _RmtWtFrameEnd:/* wait for remote new key code */
		if(Rmt_X4ms==0)
		{/* timer expired or incorrect data, reset the procedure */
			RstRmtDecode( );
	    }
		else	if(bRmtCodeRdy)
		{/* New remote key received */
			bRmtCodeRdy = False;

			/* check the data */
			A = rRmtKyCode;
			A += rRmtKyCodeC;
			if(A!=0xff || rRmtCustomL != 0x42 && rRmtCustomH != 0xbd/*|| rRmtCustomL != _CustomCode1*/)
			{/* error remote code or custom code unmatch */
				RstRmtDecode( );
				return;
			}
			
			if(rRmtKyCode == 0)	
			{/*invalid remote key*/
				RstRmtDecode( );
				return;
			}
			#if 1
				if(bIRInt)
				{
					keylen = sizeof(g_IrDAKeyMap)/IRDA_NUM;
					for(i = 0; i < keylen; i++)
					{
						if(g_IrDAKeyMap[i][0] == rRmtKyCode)
						{
							rKyValue = g_IrDAKeyMap[i][IRDA_NUM-1];
							break;
						}
					}
				}
				else
				{
					keylen = sizeof(g_SWIrDAKeyMap)/IRDA_NUM;
					for(i = 0; i < keylen; i++)
					{
						if(g_SWIrDAKeyMap[i][0] == rRmtKyCode)
						{
							rKyValue = g_SWIrDAKeyMap[i][IRDA_NUM-1];
							break;
						}
					}
				}
			#else
			/* convert remote key code to system key code */
			if(rRmtKyCode <= 0x0f)
				rKyValue = switch_remote_code1(rRmtKyCode);
			else if(rRmtKyCode <= 0x1f)
				{
				rRmtKyCode -= 0x10;
				rKyValue = switch_remote_code2(rRmtKyCode);
				}
			else if(rRmtKyCode <= 0x2f)break;
				//rKyValue = switch_remote_code1(rRmtKyCode);
			else if(rRmtKyCode <= 0x3f)break;
				//rKyValue = switch_remote_code1(rRmtKyCode);
			else if(rRmtKyCode <= 0x4f)
				{
				rRmtKyCode -= 0x40;
				rKyValue = switch_remote_code3(rRmtKyCode);
				}
			else if(rRmtKyCode <= 0x5f)
				{
				rRmtKyCode -= 0x50;
				rKyValue = switch_remote_code4(rRmtKyCode);
				}
			//A &= 0x5f;
			//rKyValue = RmtKyReformTab[A];
			#endif
			RmtKyBak = rKyValue;/*backup remote key*/

			
			/*special key check*/
			A = SpKyCk( rKyValue );				
			
			if(A==_DsTheKy)	
			{/*current status, the key is disable*/
				RstRmtDecode( );
			}
			else if(A==_OnlyShortKy)
			{/*only short key*/
				if(RptKyCk(rKyValue)== 1)
				{
					rRmtSt = _RmtWtStartRpt;
					Rmt_X64ms = 600/64;
					Rmt_X4ms = 280/4;
				}
				else RstRmtDecode( );
				break;/* to generate key message */
			}
			else/*	if(A==_NormalKy)*/
			{
				Rmt_X64ms = 1000/64;
			    Rmt_X4ms = 280/4;
				rRmtSt = _RmtLongKyCfm ;
			}
		}
	    	return;
	    

	    
	    case  _RmtLongKyCfm:/* long key confirm status */
			if(Rmt_X64ms==0)
			{/* long key confirm pass */
			
				RmtKyBak += TOTAL_KEYS;
				if(RmtKyBak == KEY_SYS_TUNELEFT + TOTAL_KEYS || RmtKyBak == KEY_SYS_TUNERIGHT + TOTAL_KEYS  || RmtKyBak == KEY_SYS_MUTE + TOTAL_KEYS)	
				{
					rRmtSt = _RmtPrsing;
				}
				else	RstRmtDecode( );/* no long key repeat enable */
				break;
			}
		#if 1
			else	if(bRmtRptKey)
			{/* one frame repeat code received */
			    bRmtRptKey = False;
			    Rmt_X4ms = 280/4;
			}
		#endif	
			else	if(Rmt_X4ms==0)
			{/* remote key released */
				RstRmtDecode( );
				break;
			}
	    	return;    	

	    case  _RmtWtStartRpt:/* wait for start repeat */
	    	if(Rmt_X64ms==0)
	    	{/* change to repeat status */
	    		bRmtKyRpting = True;
	    		++rRmtSt;/* change to repeating status 
	    		break;*/
	    	}
	    	/* stop remote code repeat check */
	    	else	if(bRmtRptKey)	 
	    	{
	    		bRmtRptKey = False;
	    		Rmt_X4ms = 280/4;
	    	}
			else	if(Rmt_X4ms==0) RstRmtDecode( );
			return;
			  
			    	
	    case  _RmtRpting:/* ;::::::::::: remote code repeat status ::::::::::: */
			if(bRmtRptKey)
			{	/* convert remote key code to system key code */
				bRmtRptKey = False;
				Rmt_X4ms = 280/4;
				break;
	    	}
			else if(Rmt_X4ms==0) 
			{
				RstRmtDecode( );
			}
			return;	
	
	   case  _RmtPrsing:/* remote key is pressing but repeat disable */
			if(bRmtRptKey)
			{	/* convert remote key code to system key code */
				RmtKyBak = 0xff;
				bRmtRptKey = False;
				Rmt_X4ms = 280/4;
	    	}
			else	if(Rmt_X4ms==0) 
			{
				SaveEncodekey(0xff);
				RstRmtDecode( );
			}
	   		return;
	}
	
	/* genreate new key message */
	if(RmtKyBak == KEY_SYS_MUTE)
	{
		switch(RmtKyBak)
		{
			case KEY_SYS_MUTE: 
				if(core.powerStatus == POWER_OFF)
					SaveEncodekey(KEY_SYS_POWER);
				else
					SaveEncodekey(KEY_SYS_MUTE);
				break;
				
			default:
				break;
		}
		return;
	}
	else if(RmtKyBak == KEY_SYS_TUNELEFT + TOTAL_KEYS || RmtKyBak == KEY_SYS_TUNERIGHT + TOTAL_KEYS  || RmtKyBak == KEY_SYS_MUTE + TOTAL_KEYS)
	{
		if(RmtKyBak == KEY_SYS_TUNELEFT + TOTAL_KEYS)		RmtKyBak = KEY_SYS_FASTB;
		if(RmtKyBak == KEY_SYS_TUNERIGHT + TOTAL_KEYS)		RmtKyBak = KEY_SYS_FASTF;
		if(RmtKyBak == KEY_SYS_MUTE + TOTAL_KEYS)
		{
			if(core.powerStatus == POWER_ON)
			{
				RmtKyBak = KEY_SYS_POWER;
			}
		}
		SaveEncodekey(RmtKyBak);
	}
	else
	{
		//rKeyResult = RmtKyBak;
		//bNewKey = True;
		SaveEncodekey(RmtKyBak);
		//if ((core.applink) && (HasAcc()))
		//TransmitKey();	
		//keybuff[keygetindex].flag = reset;
	}	
	return;
}
#endif



