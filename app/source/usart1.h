
#ifndef _USART1_H
#define _USART1_H







#define USART1_SEND_MAX					0x100
#define USART1_SEND_SUB					0x0FF
#define USART1_RECEIVE_MAX				0x100
#define USART1_RECEIVE_SUB					0x0FF



extern u8 usart1SendData[USART1_SEND_MAX];
extern u16 usart1SendSave;
extern u16 usart1SendIndex;
extern u8 usart1Sendbusy;

extern u8 usart1Receivebuff[USART1_RECEIVE_MAX];
extern u16 usart1Receivesave;
extern u16 usart1Receiveget;
extern volatile u8 bCanTimeour;
extern volatile u8 bCan4Timeour;
extern u32 canbaudRate;

void VMCanInit(void);
void ProcessVMCan(void);
void ReadArmCanStr(u8 len);
void Uart1SendStr(u8 *p, u16 len);
void SetLinUartBaud(u32 baudRate);


#endif

