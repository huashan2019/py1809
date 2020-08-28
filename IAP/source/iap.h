
#ifndef _IAP_H
#define _IAP_H







#define IAP_STATUS_IDLE			1
#define IAP_STATUS_START			2
#define IAP_STATUS_UPDATE			3
#define IAP_STATUS_END			4






void IapInit(void);
void IapRun(void);
bool IapRead(void);
void CopyIapdata(void);
u8 ASCII_Init(u8,u8);
void SysDelay(u32);
void ReportIapNext(u8);



#endif

