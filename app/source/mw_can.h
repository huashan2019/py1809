///=========================
#ifndef __MW_CAN_H
#define __MW_CAN_H



#define CAN0_FUNC    ENABLE
#define CAN1_FUNC    DISABLE

///=================================================

typedef enum
{
	Can_Rx,
	Can_Tx
}Can_RT;

typedef enum
{
	SCH_Can0,
	SCH_Can1,
	SCH_Can_Num
}Can_T;


/*typedef struct  
{
	SCH_U32 Id;
	SCH_U8 IDE;
	SCH_U8 RTR;
	SCH_U8 DLC;
	SCH_U8 FMI;
	SCH_U8 Data[8];
}Can_Data_T;*/


extern SCH_U16 CanTxCnt(Can_T can);
extern SCH_U16 CanRxCnt(Can_T can);
extern SCH_BOOL CanTxData(Can_T can, Can_Data_T *const data, SCH_U16 Len);
extern SCH_BOOL CanRxData(Can_T can, Can_Data_T *data, SCH_U16 Len);
extern void Can_Tx_DataPro(Can_T can);
extern void Can_Rx_DataPro(Can_T can,Can_Data_T *const data);
extern void SysCanExit(Can_T can);
extern SCH_BOOL SysCanInit(Can_T can);
#endif
