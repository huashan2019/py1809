
#ifndef _USART4_H
#define _USART4_H







#define USART4_SEND_MAX					0x100
#define USART4_SEND_SUB					0x0FF
#define USART4_RECEIVE_MAX				0x100
#define USART4_RECEIVE_SUB					0x0FF



extern u8 usart4Sendbuff[USART4_SEND_MAX];
extern u16 usart4SendSave;
extern u16 usart4SendIndex;
extern u8 usart4Sendbusy;
extern u16 CancommandReceiveget;

extern u8 usart4Receivebuff[USART4_RECEIVE_MAX];
extern u16 usart4Receivesave;
extern u16 usart4Receiveget;
extern volatile u8 bCanTimeour;
extern u32 canbaudRate;
extern u8 IO_CAN_R_EXIST;
extern u8 IO_CAN_R_EXIST_Timer;

void VM4CanInit(void);
void ProcessVM4Can(void);
void ReadArmCanStr(u8 len);
void Uart4SendStr(u8 *p, u16 len);
//void SetLinUartBaud(u32 baudRate);
void ReadVM4ArmCanStr(u8 len);


#endif

