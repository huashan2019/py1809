///=========================
#ifndef __APP_CAN_H
#define __APP_CAN_H


typedef enum
{
	M2C_IDLE,		
	M2C_TEST,
	M2C_AAC,
	M2C_DATA,
	M2C_TOUCH_XY,
	M2C_YMD_TIME
}M2C_ID;

#define CAN                 ((CAN_TypeDef *) CAN_BASE)



extern SCH_U8 AAC_DataRx[13];
extern SCH_U8 AAC_DataTx[13];

extern SCH_U8 AAC_AirHeatL;
extern SCH_U8 AAC_AirHeatR;

extern void CanDataRxPro(void);
extern void TASK_Can_Pro(void);
extern void ReportCANData(SCH_U8 num);
extern void ReportAirHeatData(SCH_U8 type,SCH_U8 numL,SCH_U8 numR);


#endif
