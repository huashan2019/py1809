/**
  ******************************************************************************
  * @file    ADC/ADC_Sequencer/Src/stm32f0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "config.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup ADC_Sequencer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}


/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  
}

void EXTI0_1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    { 
    	bIRInt = 1;
    	RemoteInt();
		EXTI_ClearITPendingBit(EXTI_Line1);
    }

}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external line 4_15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    { 
#ifdef RDS_ENABLE  
	#if (TDA7703_7705_TUNER||TDA7706_TUNER||TDA7786_TUNER)
		RDSInt();
	#endif
#endif
		EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
		bIRInt = 0;
		RemoteInt();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

void TIM2_IRQHandler(void)
{
	TIM2->SR = reset;
	if(b1sWinkDpOn)
	{
		//TEST_OFF;
		//b1sWinkDpOn=0;
	}
	else
	{
		//TEST_ON;
		//b1sWinkDpOn=1;
	}

	//RemoteInt();
}

void TIM14_IRQHandler(void)
{
	static u8 syspopcnt;
	static u8 syspopcnt_x4ms;
	u8 tempTimerTable[5]= {0x01,0x02,0x04,0x08,0x10};

	TIM14->SR = reset;
	bCanTimeour++;
	bCan4Timeour++;

	if(Testtime_x100ms++ >= 3)
	{
		Testtime_x100ms= 0;

	}
	
	//DetecBat();
	if (core.accStatus == ACC_SLEEP)
	{
		syspopcnt++;
		if (syspopcnt > 9)
		{
		
			syspopcnt = reset;
			btime1s = set;
		}
		return;
	}
	syspopcnt++;
	if (syspopcnt > 4)	
	{
		syspopcnt = reset;		
	}
	system10ms |= tempTimerTable[syspopcnt];
	
	syspopcnt_x4ms++;
	if (syspopcnt_x4ms > 1)	
	{
		if(b1sWinkDpOn)
		{
			//TEST_OFF;
			//b1sWinkDpOn=0;
		}
		else
		{
			//TEST_ON;
			//b1sWinkDpOn=1;
		}
		syspopcnt_x4ms = 0;
		if(Rad_X4ms)		Rad_X4ms--;
		if(Rad_Err_X4ms)		Rad_Err_X4ms--;
		if(rRdRDS_X4ms)		rRdRDS_X4ms--;
		if(Rmt_X4ms)		Rmt_X4ms--;
		if(Rad_ChkStat_X4ms)	--Rad_ChkStat_X4ms;
		if(Rad_Seek_X4ms)	--Rad_Seek_X4ms;
		
		Testtime_x10ms++;
	}
}

void USART3_4_IRQHandler(void)
{
#if 0
if (USART_GetFlagStatus(USART4, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
{
	USART_ReceiveData(USART4);
	USART_ClearFlag(USART4, USART_FLAG_ORE);
}

	if(USART_GetITStatus(USART4,USART_IT_RXNE) != RESET)
{
	usart4Receivebuff[usart4Receivesave++]  = USART_ReceiveData(USART4);
	usart4Receivesave &= USART4_RECEIVE_SUB;
	USART_ClearITPendingBit(USART4, USART_IT_RXNE);

}

#else
	u32 tflag;
	u32 bitpos = 0, itmask = 0;
    //if (USART_GetFlagStatus(USART4, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		//USART_ReceiveData(USART4);
		//USART_ClearFlag(USART4, USART_FLAG_ORE);
	}

	tflag = USART4->ISR;
	if(tflag & 0x00000020)
	{
		usart4Receivebuff[usart4Receivesave++] = (u8)(USART4->RDR);
		usart4Receivesave &= USART4_RECEIVE_SUB;
	}
	else if(tflag & 0x00000080)
	{
		if(usart4SendIndex != usart4SendSave)
		{
			USART4->TDR = usart4Sendbuff[usart4SendIndex++];
			usart4SendIndex &= USART4_SEND_SUB;
			usart4Sendbusy = set;
		}
		else
		{
			usart4Sendbusy = reset;
			USART4->CR1 &= 0xFFFFFF7F;
		}
	}
	return;
#endif
}

void USART2_IRQHandler(void)
{
	u32 tflag;
	u32 bitpos = 0, itmask = 0;
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	}

	tflag = USART2->ISR;
	if(tflag & 0x00000020)
	{
		usart2Receivebuff[usart2Receivesave++] = (u8)(USART2->RDR);
		usart2Receivesave &= USART2_RECEIVE_SUB;
	}
	else if(tflag & 0x00000080)
	{
		if(usart2SendIndex != usart2SendSave)
		{
			USART2->TDR = usart2Sendbuff[usart2SendIndex++];
			usart2SendIndex &= USART2_SEND_SUB;
			usart2Sendbusy = set;
		}
		else
		{
			usart2Sendbusy = reset;
			USART2->CR1 &= 0xFFFFFF7F;
		}
	}
	return;
}
void USART1_IRQHandler(void)
{
	u32 tflag;
	u32 bitpos = 0, itmask = 0;
  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}

	tflag = USART1->ISR;
  	if(tflag & 0x00000020)
  	{
  		bCanTimeour = reset;
		usart1Receivebuff[usart1Receivesave++] = (u8)(USART1->RDR);	
		usart1Receivesave &= USART1_RECEIVE_SUB;
  	}
  	else if(tflag & 0x00000080)
  	{
		if(usart1SendIndex != usart1SendSave)
		{
			USART1->TDR = usart1SendData[usart1SendIndex++];
			usart1SendIndex &= USART1_SEND_SUB;
			usart1Sendbusy = set;
		}
		else
		{
			usart1Sendbusy = reset;
			USART1->CR1 &= 0xFFFFFF7F;
		}
  	}
	return;
}

/**************11****************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/
void RTC_IRQHandler(void)
{
	
	if( (RTC_IT_ALRA) != RESET)
	{
			RTC_TimeShow();
		    RTC_ClearITPendingBit(RTC_IT_ALRA);
		    EXTI_ClearITPendingBit(EXTI_Line17);
	}
	if(RTC_GetITStatus(RTC_IT_TS) != RESET)
	{
		//bRTCSync1S = 1;
		RTC_ClearITPendingBit(RTC_IT_TS);
		//EXTI_ClearITPendingBit(EXTI_Line17);
	} 
}


void CEC_CAN_IRQHandler(void)
{
	CanRxMsg RxMessage={0};
	Can_Data_T CanData;
	CAN_Receive(CAN, CAN_FIFO0, &RxMessage);
	CanData.ExtId = RxMessage.ExtId;
	CanData.StdId = RxMessage.StdId;
	CanData.DLC   = RxMessage.DLC;
	CanData.IDE   = RxMessage.IDE;
	CanData.RTR   = RxMessage.RTR;
	sch_memcpy(CanData.Data,RxMessage.Data,8);
	Can_Rx_DataPro(SCH_Can0, &CanData);
	CAN_ClearITPendingBit(CAN,CAN_IT_FMP0);  /* 清除挂起中断 */
}

