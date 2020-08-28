/*******************************************************************************\
*      UART communication interface head file.									*
*             Language           : C.											*
*   Author:  TECHNICAL Co.,LTD                        		     				*
\*******************************************************************************/ 
#ifdef   UART_ROOT
#define  UART_EXT
#else
#define  UART_EXT     extern
#endif

#if	_UartDebug
#define   DEBUGRX   512
UART_EXT uchar	rUARTTxBuff[DEBUGRX];
#define   DEBUGTX   512
UART_EXT uchar 	rUARTRxBuff[DEBUGTX];
UART_EXT uchar	cUARTRxByte;
UART_EXT uint	rTxBuffWrPnt;
UART_EXT uint	rTxBuffRdPnt;
UART_EXT uchar	UARTRx_X4ms;
UART_EXT uchar	cUARTTx_X4ms;

#endif
UART_EXT TYPE_BYTE	rUARTFlag;
#define	bUartRxSt		rUARTFlag.bit.b0
#define	bUartTxSt		rUARTFlag.bit.b1
#define	bUartRxNewCmd	rUARTFlag.bit.b2
#define	bFLcdTxInfo     	rUARTFlag.bit.b3
#define	bArmRxInfo     	rUARTFlag.bit.b4
#define   bIIcMsgSt		rUARTFlag.bit.b5
#define   bArmTxInfo		rUARTFlag.bit.b6
#define   bFLcdRxInfo		rUARTFlag.bit.b7

UART_EXT 	void 	SetUartTxEn(uint8_t Com);
UART_EXT 	void 	ClearUartTxEn(uint8_t Com);
UART_EXT	void		UARTOutputChar(uchar	d);
UART_EXT	void 	UartOutputString(const uchar *p);
UART_EXT	uchar  	ConvNumToASCII(uchar n);
UART_EXT	uchar 	ChkUartTxing(void);
UART_EXT  	void 	UartOutHex(unsigned long num);
UART_EXT  	void 	UartOutDec(unsigned int num);
UART_EXT  	void		UartSimuKy(void);
UART_EXT  	void 	Printf ( const char *fmt, ... );
UART_EXT 	void		UARTTxProc(void);
UART_EXT 	void		UARTRxProc(void);
UART_EXT 	void 	PutString(unsigned char *s);
UART_EXT 	void 	PutHex(unsigned long num);
UART_EXT 	void		DebugComInit(void);
UART_EXT 	void 	UartDebug_Int(void);
/*******************************End of file***************************************/
