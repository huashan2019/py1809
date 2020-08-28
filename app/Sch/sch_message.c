/*
***************************************************************************************************
**  Copyright   : 
**  Project     : STM32F103
**  File        : sch_message.c
**  Description : This file is the SCH.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "sch_config.h"

#define QUEUE_LENGTH      10

MESSAGE Queue[NUMOFMODE][QUEUE_LENGTH];
QUEUE_T MESSAGE_QUEUE[NUMOFMODE] = 
{
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[POWER_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[ARM_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[AUTO_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[CAN_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[LIN_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[RADAR_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[MAIN_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[TUNER_MODULE]},
	{0,0,0,QUEUE_LENGTH,sizeof(MESSAGE),&Queue[MMI_MODULE]}
};
///==================================================
SCH_BOOL PostMessage(MSG_MODULE Module, SCH_U8 ID, SCH_U16 param)
{
	MESSAGE Msg;
	Msg.ID=ID;
	Msg.prm=param;
	return Queue_In(&MESSAGE_QUEUE[Module], &Msg, 1);
}
SCH_BOOL InsertMessage(MSG_MODULE Module,SCH_U8 ID,SCH_U16 param)
{
	MESSAGE Msg;
	Msg.ID=ID;
	Msg.prm=param;
	return Queue_Insert(&MESSAGE_QUEUE[Module], &Msg, 1);
}
SCH_BOOL GetMessage(MSG_MODULE Module,MESSAGE *Msg)
{
	return Queue_Out(&MESSAGE_QUEUE[Module], Msg, 1);
}
void ClearMessage(MSG_MODULE Module)
{
	Queue_Init(&MESSAGE_QUEUE[Module]);
}
void ClearAllModeMessage(void)
{
	SCH_U8 i=0;
	for(i=0;i<NUMOFMODE;i++)
		ClearMessage((MSG_MODULE)i);
}

