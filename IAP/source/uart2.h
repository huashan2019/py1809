
#ifndef _UART2_H
#define _UART2_H



#define USART2_SEND_MAX					(u16)0x100
#define USART2_SEND_SUB					(u16)0x0FF
#define USART2_RECEIVE_MAX				(u16)0x100
#define USART2_RECEIVE_SUB					(u16)0x0FF





extern u8 usart2Sendbuff[USART2_SEND_MAX];
extern u16 usart2SendSave;
extern u16 usart2SendIndex;
extern u8 usart2Sendbusy;

extern u8 usart2Receivebuff[USART2_RECEIVE_MAX];
extern u16 usart2Receivesave;
extern u16 usart2Receiveget;

void Uart2Init(void);
void Uart2SendStr(u8*,u16);


#endif

