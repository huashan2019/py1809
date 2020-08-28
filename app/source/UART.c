/*******************************************************************************\
*      UART communication interface driver program.								*
*             Language           : C.											*
*   Author:  TECHNICAL Co.,LTD                        		     				*
\*******************************************************************************/ 

#define	UART_ROOT

/*******************************************************************************\
*      UART communication interface driver program.								*
*             Language           : C.											*
*   Author:  STT ELECTRONICS TECHNICAL Co.,LTD                 		     		*
\*******************************************************************************/ 


#include	"config.h"
#include 	<stdarg.h>


const uchar NumToASCII_Tab[ ]=
{// math number: 0~9 and A~F  to stand ASCII convert table
	/*0x00*/'0',
	/*0x01*/'1',
	/*0x02*/'2',
	/*0x03*/'3',
	/*0x04*/'4',
	/*0x05*/'5',
	/*0x06*/'6',
	/*0x07*/'7',
	/*0x08*/'8',
	/*0x09*/'9',
	/*0x0A*/'A',
	/*0x0B*/'B',
	/*0x0C*/'C',
	/*0x0D*/'D',
	/*0x0E*/'E',
	/*0x0F*/'F',
};
//////////////////////////////////////////////////////
// 		===    	local equivalence define	=== 	//
//////////////////////////////////////////////////////
void SetUartTxEn(u8 Com)
{
		switch(Com)
		{
			#if 1
		case 1:
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			break;
		case 2:
			USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
			break;
		case 3:
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			break;
		case 4:
			USART_ITConfig(USART4, USART_IT_TXE, ENABLE);
			break;
		case 5:
			//USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
			break;
		#endif
		}
}
void ClearUartTxEn(u8 Com)
{
	switch(Com)
		{
			#if 1
		case 1:
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USART1, USART_IT_TXE);
			break;
		case 2:
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USART2, USART_IT_TXE);
			break;
		case 3:
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USART3, USART_IT_TXE);
			break;
		case 4:
			USART_ITConfig(USART4, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USART4, USART_IT_TXE);
			break;
		case 5:
			//USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
			//USART_ClearITPendingBit(UART5, USART_IT_TXE);
			break;
		#endif
		}
}

#if	_UartDebug

/*****************************************************************************
Function    : UART contorl register initialization
Input       : 
Output      :  
Description	: 
\*----------------------------------------------------------------------------*/


void	DebugComInit(void)
{
	#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_ClockInitTypeDef	USART_ClockInitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

	RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  /* USARTx Pins configuration **************************************************/  
  /* Connect pin to Periph */
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);    
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); 


 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_DeInit(USART1);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	
	USART_ClockInit(USART1, &USART_ClockInitStructure);
	
  
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	rTxBuffRdPnt = rTxBuffWrPnt=0;
	cUARTRxByte =0;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	ClearUartTxEn(1);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	#endif
}

void UartDebug_Int(void)
{   
    uchar d=0;  
    	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_ORE);

	}

    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
	#if 0
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	#else
		d = USART_ReceiveData(USART1);
		rUARTRxBuff[cUARTRxByte] = d;
		cUARTRxByte++;
		if(cUARTRxByte > 31)	cUARTRxByte = 0;
		UARTRx_X4ms = 100/4;// restart UART Rx timer
	#endif	

		//USART_SendData(USART1,d);
	}
	// Debug print infomation
	if(USART_GetITStatus(USART1,USART_IT_TXE) != RESET)
	{
		USART_SendData( USART1, rUARTTxBuff[rTxBuffRdPnt++]) ;
		if(rTxBuffRdPnt >= DEBUGTX)	rTxBuffRdPnt = 0;
		if( rTxBuffRdPnt == rTxBuffWrPnt )	
		{
		    //rTxBuffRdPnt = 0;
		    //rTxBuffWrPnt = 0;
		    ClearUartTxEn(1);
		}
	}
}

//=============================================================================
//                                                                           
//=============================================================================
#define _outchar	UARTOutputChar

void DoPrint( const char *fmt, va_list ap )
{
	char  ch=0;
	char  i=0;
	long  value=0;
	char   fl_zero=0;
	unsigned char  fl_len=0;
	unsigned char  cnt=0;
	u32 mask=1;

	char *ptr;

	while(1) {
		
		//----- Find Formatter % -----

		switch( ch = *fmt++ ) {
			case 0:		return;
			case '%':	if( *fmt != '%' ) break;
						fmt++;
			default:	_outchar( ch );
						continue;
		}

		//----- Get Count -------------
		
		fl_zero = 0;
		cnt = 0;

		ch = *fmt++;

		if( ch=='0' ) {
			fl_zero = 1;
			ch = *fmt++;
			cnt = ch - '0';
			ch = *fmt++;
		}
		else if( ch>='0' && ch<='9' ) {
			cnt = ch - '0';
			ch = *fmt++;
		}

		//----- Get char(B) / int / long(L) ----------------

		fl_len = 2;

		switch(ch) {
		case 'l':
		case 'L':	ch = *fmt++;	fl_len = 4;		break;
		case 'b':
		case 'B':	ch = *fmt++;	fl_len = 1;		break;
		}		

		//----- Get Type Discriptor -----
		
		switch( ch ) {

			case 'd':
			case 'u':

				switch (fl_len) {
				case 1:
					if( ch=='d' ) value = (char)va_arg( ap, char );
					else          value = (unsigned char)va_arg( ap, unsigned char );
					break;

				case 2:
					if( ch=='d' ) value = (int)va_arg( ap,  int );
					else          value = (int)va_arg( ap, int );
					break;

				case 4:
					if( ch=='d' ) value = (long)va_arg( ap, long );
					else          value = (u32)va_arg( ap, u32 );
					break;
				}

				if( value<0 ) {
					_outchar('-');
					value = value*(-1);
				}

				if(cnt==0) {
					if( value==0 ) { _outchar('0'); continue; }

					for(cnt=0, mask=1; cnt<10; cnt++) {
						if( (value/mask)==0 ) break;
						mask = mask*10;
					}
				}

				for(i=0, mask=1; i<cnt-1; i++) mask = mask*10;

				while(1) {
					ch = (value / mask) + '0';
					if( ch=='0' && fl_zero==0 && mask!=1 ) ch=' ';
					else fl_zero = 1;
					_outchar(ch);

					value = value % (mask);
					mask = mask / 10;
					
					if( mask==0 )
						break;
				}
				continue;

			case 'x':
			case 'X':

				switch (fl_len) {
				case 1:	value = (unsigned char)va_arg( ap, unsigned char );		break;
				case 2:	value = (int)va_arg( ap, int );		break;
				case 4:	value = (u32)va_arg( ap, u32 );		break;
				}

				if(cnt==0) cnt = fl_len*2;

				for(i=0; i<cnt; i++) {
					_outchar( NumToASCII_Tab[(value >> (cnt-i-1)*4) & 0x000f] );
				}
				continue;

			case 's':

				ptr = (char *)va_arg( ap, char* );
				while(*ptr!='\0')
					_outchar(*ptr++);
				continue;

			case 'c':
				value = va_arg( ap, char );
					_outchar((unsigned char)value);
				continue;

			default:
				value = (int)va_arg( ap, int );
				continue;
		}
	}
}

void Printf( const char *fmt, ... )
{
	va_list ap;

	va_start(ap, fmt);
	DoPrint( fmt, ap );
	va_end( ap );
}

uchar	ConvNumToASCII(uchar n)
{
	if( n > 0x0F )	return(n);
	else		return( NumToASCII_Tab[n] );
}

/*****************************************************************************
* Function      : UART send one byte 
* description	: 直接通过串口发送
\*----------------------------------------------------------------------------*/
void	UARTOutputChar(uchar	d)
{
#if 1
    rUARTTxBuff[rTxBuffWrPnt++] = d;
    if(rTxBuffWrPnt>=DEBUGTX)
        rTxBuffWrPnt=0;
	SetUartTxEn(1);
#endif	

}

/*******************************************************************************\
*Function   : fill string to UART Tx queue.										*
*Input		: 1. string                                                         *
*			  2. queue offset                                                   *
*Output		: Tx queue offset	                                                *
*Description:                                                                   *
\*-----------------------------------------------------------------------------*/
void	UartOutputString(const uchar *p)
{
#if 0
	uchar	d;
	for( ; ; )
	{
		d = *(p++);
		if(d == '\0')	break;
		rUARTTxBuff[rTxBuffWrPnt++] = d;
		if(rTxBuffWrPnt>=DEBUGTX)
			rTxBuffWrPnt=0;
	}
	SetUartTxEn(1);
#endif
}

void UartOutHex(unsigned long num)
{
	//
#if 1
	unsigned char j=0,i =0;
	unsigned char temp[8]={0,0,0,0,0,0,0,0};
	long val=0;
	const unsigned char HEX[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	//hex not over than 0x7FFFFFFF
	//return;
	val = num;
	val =val&0x7FFFFFFF;
	if(val > 0xFFFF)
		{
			j = 8;
			temp[0]=(unsigned char)((val &0xF0000000)>>28);
			temp[1]=(unsigned char)((val &0x0F000000)>>24);
			temp[2]=(unsigned char)((val &0x00F00000)>>20);
			temp[3]=(unsigned char)((val &0x000F0000)>>16);
			temp[4]=(unsigned char)((val &0x0000F000)>>12);
			temp[5]=(unsigned char)((val &0x00000F00)>>8);
			temp[6]=(unsigned char)((val &0x000000F0)>>4);
			temp[7]=(unsigned char)((val &0x0000000F));
		}
	else if((val <=0xFFFF) &&(val > 0xFF))
		{
			j = 4;
			temp[0]=(unsigned char)((val &0xF000)>>12);
			temp[1]=(unsigned char)((val &0x0F00)>>8);
			temp[2]=(unsigned char)((val &0x00F0)>>4);
			temp[3]=(unsigned char)((val &0x000F));
		}
	else if(val <= 0xFF)
		{
			j = 2;
			temp[0]=(unsigned char)((val &0xF0)>>4);
			temp[1]=(unsigned char)((val &0x0F));
		}

	for(i = 0  ;i< j; i ++)
		{
			UARTOutputChar(HEX[temp[i]]);
		}
	
	// UARTOutputChar(' ');

#endif
}
/**************************************************
//input :num must not bigger than 65536;
***************************************************/
void 	UartOutDec(unsigned int num)
{
	
#if 1
	unsigned int temp =0;
	unsigned int temp2 = 0;
	unsigned char i = 0;
	unsigned char tmp[5]={'0','0','0','0','0'};
	//return;

	temp = num%10;
	temp2 = num/10;
	tmp[0]=NumToASCII_Tab[temp];

	if(temp2 == 0)
		goto END;
	temp = temp2%10;
	temp2 = (temp2/10);
	tmp[1]=NumToASCII_Tab[temp];

	if(temp2 == 0)
		goto END;

	temp =temp2%10;
	temp2=(temp2/10);
	tmp[2] = NumToASCII_Tab[temp];

	if(temp2 == 0)
		goto END;
		
	temp =temp2%10;
	temp2=(temp2/10);
	tmp[3] = NumToASCII_Tab[temp];

	if(temp2 == 0)
		goto END;
	
	temp =temp2%10;
	temp2=(temp2/10);
	tmp[4] = NumToASCII_Tab[temp];

	
	END:
	for(i = 0 ;i<5;i++)
		{
			UARTOutputChar(tmp[4-i]);
		}

	UARTOutputChar(' ');

		
#endif
}
	
/*******************************************************************************\
*Function   : move array data to UART Tx queue.									*
*Input		: 1. array address                                                  *
*			  2. data number                                                    *
*Output		: 					                                                *
*Description:                                                                   *
\*-----------------------------------------------------------------------------*/
void	UartOutputArray(const uchar *p,uchar c)
{
//	uchar	d;
	#if 0
	for( ; ; )
	{
		d = *p;
		if(d == 0)	d = ' ';
		rUARTTxBuff[rTxBuffWrPnt++] = d;
		if(rTxBuffWrPnt>=DEBUGTX)
				rTxBuffWrPnt=0;
		--c;
		if(c == 0)	break;
		++p;
	}
	SetUartTxEn(1);
	#endif
}


/*****************************************************************************
Function    : decoder command sent check
Input       : none
Output      : 0: not sent, 1 ~ 0xff: sent 
Description	: 
\*----------------------------------------------------------------------------*/
uchar	ChkUartTxing(void)
{
	if(rTxBuffRdPnt != rTxBuffWrPnt)	return( 1 );
	else	return( 0 );
}

/*****************************************************************************
        Function        : UART communication Tx procedure 
        description		: 
----------------------------------------------------------------------------*/
void	UARTTxProc(void)
{

		if( rTxBuffRdPnt == rTxBuffWrPnt )	
		{
			ClearUartTxEn(1);
			return;
		}

		if(rTxBuffRdPnt != rTxBuffWrPnt)
		{
			USART_SendData( USART1, rUARTTxBuff[rTxBuffRdPnt++]) ;
			if(rTxBuffRdPnt>=DEBUGTX)
				rTxBuffRdPnt=0;
		}
}





/*****************************************************************************
 Function    : Uart receive buffer string match check
 Input       : expected string memory start address
 Output      : 0: not match, 1: match
 Discription :
\*----------------------------------------------------------------------------*/
uchar	RxedStringMatch(const uchar *p)
{
	uchar	i,d;
	
	for(i = 0; i < sizeof(rUARTRxBuff); )
	{
		d = *(p++);
		if(d == '\n')	return(1);
		if( rUARTRxBuff[i++] != d )	return(0);
	}
	
	return(1);
}

/*****************************************************************************
Function    : convert ASCII to num
Input       : 
Output      :  
Description	: 
\*---------------------------------------------------------------------------*/
uchar	ConvASCIIToNum(uchar ASCII)
{
	if( ASCII > 0x66 )	return( 0 );
	else	if( ASCII > 0x60)	return( ASCII - 0x57 );	// a ~ f
	else	if( ASCII > 0x46)	return( 0 );
	else	if( ASCII > 0x40)	return( ASCII - 0x37 );	// A ~ F
	else	if( ASCII > 0x39)	return( 0 );
	else	if( ASCII > 0x2F)	return( ASCII - 0x30 );	// 0 ~ 9
	else return 0;
}


/*****************************************************************************
Function    : UART simulate generate key
Input       : 
Output      :  
Description	: 
---------------------------------------------------------------------------*/
uchar	rTestDvdCtrlPin;
void	TestDvd( )
{
	if(!bSync16ms)	return;
	switch(rTestDvdCtrlPin)
	{
	case	0x10:	// eject disc

		bDeckDoing = 1;
		break;
	
	case	0x11:
	//	oEjectDisc = 1;
		break;
	
	case	0x12:
		rTestDvdCtrlPin = 0x30;
		return;	
	
	case	0x20:	// load disc
		bDeckDoing = 1;
		break;
	
	case	0x21:
	//	oLoadDisc = 1;
		break;
	
	case	0x22:	
		rTestDvdCtrlPin = 0x30;
		return;
	
	case	0x30:	
//		rTestDvdCtrlPin;
		bDeckDoing = 0;
	//	oEjectDisc = 0;
	//	oLoadDisc = 0;
		rTestDvdCtrlPin = 0;
		return;
		
	
	default:	return;
	}
	rTestDvdCtrlPin++;
}

void	UartSimuKy(void)
{
	if( !bUartRxNewCmd )	return;
		
	// UART received PC command analyse
	if(RxedStringMatch("Test Uart Debug\n"))
	{
		Printf("\r\n Get Message,Test OK!");
	}
	else	return;
}

/*****************************************************************************
*Function	: UART communication Rx procedure 
*description: 
\*----------------------------------------------------------------------------*/
void	UARTRxProc(void)
{
#if	0
    u8 i;
#endif
	bUartRxNewCmd = 0;
	if( !bUartRxSt )
	{// UART Rx idle
		if(cUARTRxByte != 0)
		{//Rx started
			bUartRxSt = TRUE;//change to Rx busy status
		}
		else	if( UARTRx_X4ms == 0 )
		{
////			if( UART2SR & 0xF0)	i = RDBUF2;
			UARTRx_X4ms = 100/4;
		}
	}
	else
	{// UART Rxing
		if(UARTRx_X4ms != 0)	return;
		// Rx one frame end
		//DBGMSG("\r\n Rx one frame");
#if	0
		Printf("\r\n Rxed: \n");
		// analy the received data
		for(i = 0; ; )
		{
			rUARTTxBuff[rTxBuffWrPnt++] = rUARTRxBuff[i++];
			if(i == sizeof(rUARTRxBuff) )	break;
		}
		rUARTTxBuff[rTxBuffWrPnt++] = 0;
		rUARTTxBuff[rTxBuffWrPnt++] = 0;
		rUARTTxBuff[rTxBuffWrPnt++] = 0;
#endif		
		bUartRxSt = FALSE;
		cUARTRxByte = 0;// start new frame
		bUartRxNewCmd = 1;
	}
}

void PutString(unsigned char *s){
   while (*s != '\0') {
   	UARTOutputChar(*s++);
   }
}

void PutHex(unsigned long num)
{
	unsigned char j=0,i =0;
	unsigned char temp[8];
	long val=0;
	const unsigned char HEX[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	//hex not over than 0x7FFFFFFF
	val = num;
	val =val&0x7FFFFFFF;
	if(val > 0xFFFF)
		{
			j = 8;
			temp[0]=(unsigned char)((val &0xF0000000)>>28);
			temp[1]=(unsigned char)((val &0x0F000000)>>24);
			temp[2]=(unsigned char)((val &0x00F00000)>>20);
			temp[3]=(unsigned char)((val &0x000F0000)>>16);
			temp[4]=(unsigned char)((val &0x0000F000)>>12);
			temp[5]=(unsigned char)((val &0x00000F00)>>8);
			temp[6]=(unsigned char)((val &0x000000F0)>>4);
			temp[7]=(unsigned char)((val &0x0000000F));
		}
	else if((val <=0xFFFF) &&(val > 0xFF))
		{
			j = 4;
			temp[0]=(unsigned char)((val &0xF000)>>12);
			temp[1]=(unsigned char)((val &0x0F00)>>8);
			temp[2]=(unsigned char)((val &0x00F0)>>4);
			temp[3]=(unsigned char)((val &0x000F));
		}
	else if(val <= 0xFF)
		{
			j = 2;
			temp[0]=(unsigned char)((val &0xF0)>>4);
			temp[1]=(unsigned char)((val &0x0F));
		}

	for(i = 0  ;i< j; i ++)
		{
			UARTOutputChar(HEX[temp[i]]);
		}
}
/*******************************************************************************  
* Function Name  : fputc  
* Description    : Retargets the C library printf function to the USART.  
* Input          : None  
* Output         : None  
* Return         : None  
*******************************************************************************/   
int fputc(int ch, FILE *f)   
{   
  /* Place your implementation of fputc here */   
  /* e.g. write a character to the USART */  

    USART_SendData(USART1, (u8) ch); 
    /* Loop until the end of transmission */   
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)   
    {   
    }   

    
  return ch;   
}   
#endif
/******************************End of file**************************************/



/******************************End of file**************************************/
