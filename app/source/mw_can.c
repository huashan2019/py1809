/*
***************************************************************************************************
**  Copyright   : 
**  Project     : STM32F103
**  File        : mw_can.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/

#include "config.h"


#if CAN0_FUNC == ENABLE
#define MAX_CAN0_TX_BUF       10
Can_Data_T Can0_TX_buf[MAX_CAN0_TX_BUF];
QUEUE_T CAN0_TX_QUEUE = {0,0,0,MAX_CAN0_TX_BUF,sizeof(Can_Data_T),Can0_TX_buf};
#define MAX_CAN0_RX_BUF       10
Can_Data_T Can0_RX_buf[MAX_CAN0_RX_BUF];
QUEUE_T CAN0_RX_QUEUE = {0,0,0,MAX_CAN0_RX_BUF,sizeof(Can_Data_T),Can0_RX_buf};
#endif
#if CAN1_FUNC == ENABLE
#define MAX_CAN1_TX_BUF       10
Can_Data_T Can1_TX_buf[MAX_CAN1_TX_BUF];
QUEUE_T CAN1_TX_QUEUE = {0,0,0,MAX_CAN1_TX_BUF,sizeof(Can_Data_T),Can1_TX_buf};
#define MAX_CAN1_RX_BUF       10
Can_Data_T Can1_RX_buf[MAX_CAN0_RX_BUF];
QUEUE_T CAN1_RX_QUEUE = {0,0,0,MAX_CAN1_RX_BUF,sizeof(Can_Data_T),Can1_RX_buf};
#endif


QUEUE_T *const CanBufAddr[][2] = 
{
#if CAN0_FUNC == ENABLE
	&CAN0_RX_QUEUE,&CAN0_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if CAN1_FUNC == ENABLE
	&CAN1_RX_QUEUE,&CAN1_TX_QUEUE,
#else
	NULL,NULL,
#endif
};
SCH_U16 bsystem10msflag;

CAN_TypeDef *Can_Arry[]={CAN};
///========================================================
void CanBufInit(Can_T can,Can_RT TxRx)
{
	if(CanBufAddr[can][TxRx])
		Queue_Init(CanBufAddr[can][TxRx]);
}
SCH_U16 CanBufCnt(Can_T can,Can_RT TxRx)
{
	QUEUE_T *pCanBuf;
	pCanBuf = CanBufAddr[can][TxRx];
	if(pCanBuf)
		return Queue_Cnt(pCanBuf);
	return 0;
}
SCH_BOOL CanGetFromBuf(Can_T can, Can_RT TxRx, Can_Data_T *data, SCH_U16 Len)
{
	QUEUE_T *pCanBuf;
	pCanBuf = CanBufAddr[can][TxRx];
	if(pCanBuf)
		return Queue_Out(pCanBuf, data, Len);
	return FALSE;
}
SCH_BOOL CanPutToBuf(Can_T can, Can_RT TxRx, Can_Data_T *const data, SCH_U16 Len)
{
	QUEUE_T *pCanBuf;
	pCanBuf = CanBufAddr[can][TxRx];
	if(pCanBuf)
		return Queue_In(pCanBuf, data, Len);
	return FALSE;
}
/********************************************************************************
**  Function	: CanRxCnt
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 CanRxCnt(Can_T can)
{
	return CanBufCnt(can,Can_Rx);
}
/********************************************************************************
**  Function	: CanTxCnt
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 CanTxCnt(Can_T can)
{
	return CanBufCnt(can,Can_Tx);
}
/********************************************************************************
**  Function	: CanRxData
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL CanRxData(Can_T can, Can_Data_T *data, SCH_U16 Len)
{
	return CanGetFromBuf(can,Can_Rx,data,Len);
}
/********************************************************************************
**  Function	: CanTxData
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL CanTxData(Can_T can, Can_Data_T *const data, SCH_U16 Len)
{
	if(CanPutToBuf(can,Can_Tx,data,Len))
	{
		Can_Tx_DataPro(can);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************************
**  Function	: Can_Rx_DataPro
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: NULL
********************************************************************************/
void Can_Rx_DataPro(Can_T can,Can_Data_T *const data)
{
	CanPutToBuf(can,Can_Rx,data,1);
}
/********************************************************************************
**  Function	: Can_Tx_DataPro
**  Author		: 
**  Created on	: 20161101
**  Description	: 
**  Return		: NULL
********************************************************************************/
void Can_Tx_DataPro(Can_T can)
{
	Can_Data_T data;
	if(CanTxCnt(can) == 0)
	{
		CAN_ITConfig(Can_Arry[can], CAN_IT_TME, DISABLE);
		return;
	}
	if(CanGetFromBuf(can,Can_Tx,&data,1))
	{
		CAN_Transmit(Can_Arry[can], &data);
	}
}
/********************************************************************************
**  Function	: SysCanExit
**  Author		: wenjunHu
**  Created on	: 20170901
**  Description	:
**  Return		: BOOL
********************************************************************************/
void SysCanExit(Can_T can)
{
	switch(can)
	{
		case SCH_Can0:
			break;
		default:break;
	}
}



 /**
  * @file   CAN_Configuration
  * @brief  Configures the different GPIO ports.
  * @param  ��
  * @retval ��
  */
void GPIO_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOD, ENABLE);	
	/* CAN1 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_4); 
	/* Configure CAN pin: RX--PB8*/									             
	/* Configure CAN pin: TX--PB9*/									             
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);     
	
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);     

	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);

	
}
 
  /**
   * @file	 NVIC_Configuration
   * @brief  Configures the nested vectored interrupt controller.
   * @param  ��
   * @retval ��
   */
 void NVIC_Configuration(void)
 {
	 NVIC_InitTypeDef NVIC_InitStructure; 
	 /* Enable the USART1 Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;   
	 NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
 }


 /**
  * @file   CAN_Configuration
  * @brief  Configures the CAN
  * @param  ��
  * @retval ��
  */
void CAN_Configuration(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  
  NVIC_Configuration();

  GPIO_Configuration();
  /* CAN register init */
  CAN_DeInit(CAN);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE; /* ʱ�䴥����ֹ, ʱ�䴥����CANӲ�����ڲ���ʱ����������ұ����ڲ���ʱ��� */
  CAN_InitStructure.CAN_ABOM = ENABLE; /* �Զ����߽�ֹ���Զ����ߣ�һ��Ӳ����ص�128��11������λ�����Զ��˳�����״̬��������Ҫ����趨������˳� */
  CAN_InitStructure.CAN_AWUM = DISABLE; /* �Զ����ѽ�ֹ���б�������ʱ���Զ��˳�����	*/
  CAN_InitStructure.CAN_NART = ENABLE; /* �����ش�, �������һֱ�����ɹ�ֹ������ֻ��һ�� */
  CAN_InitStructure.CAN_RFLM = DISABLE; /* ����FIFO����, 1--��������յ��µı���ժ��Ҫ��0--���յ��µı����򸲸�ǰһ����	*/
  CAN_InitStructure.CAN_TXFP = ENABLE;  /* �������ȼ�  0---�ɱ�ʶ������  1---�ɷ�������˳�����	*/
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; /* ģʽ	*/
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      /* ����ͬ������ֻ��canӲ�����ڳ�ʼ��ģʽʱ���ܷ�������Ĵ��� */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;      /* ʱ���1 */
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;      /* ʱ���2 */
  CAN_InitStructure.CAN_Prescaler = 8;         /* ������Ԥ��Ƶ�� */  
 
  if (CAN_Init(CAN,&CAN_InitStructure) == CANINITFAILED) 		
  {
		//printf("CAN_InitStatus_Failed");
     													                  
  }	


  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)0x1314<<3)&0xFFFF0000)>>16; 			  //Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((u32)0x1413<<3)&0xFFFF0000)>>16; 		  //��������16λÿλ����ƥ��
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((u32)0x1413<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;			  //��������16λÿλ����ƥ��
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0 ; 		  //��������������FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;		  //ʹ�ܹ�����
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FilterInitStructure.CAN_FilterNumber = 2;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)0x000003C0<<3)&0xFFFF0000)>>16; 			  //Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)0x000003C0<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((u32)0x000003C2<<3)&0xFFFF0000)>>16;		  //��������16λÿλ����ƥ��
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((u32)0x000003C2<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;	  //��������16λÿλ����ƥ��
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0 ; 		  //��������������FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;		  //ʹ�ܹ�����
  CAN_FilterInit(&CAN_FilterInitStructure);









  
  CAN_ITConfig(CAN,CAN_IT_FMP0, ENABLE);   /* �Һ��ж�, �����жϺ��fifo�ı��ĺ����ͷű������жϱ�־ */
  //CAN_ITConfig(CAN,CAN_IT_FF0, ENABLE);   /* �Һ��ж�, �����жϺ��fifo�ı��ĺ����ͷű������жϱ�־ */
}




/********************************************************************************
**  Function	: SysCanInit
**  Author		: 
**  Created on	: 20170407
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL SysCanInit(Can_T can)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef   CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	switch(can)
	{
		case SCH_Can0:
#if CAN0_FUNC == ENABLE
			CAN_Configuration();	
#endif
			break;
		case SCH_Can1:
			break;
		default:
			return FALSE;
	}
	CanBufInit(can, Can_Tx);
	CanBufInit(can, Can_Rx);
	return TRUE;
}


