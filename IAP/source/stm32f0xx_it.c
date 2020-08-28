/**
  ******************************************************************************
  * @file    stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "config.h"


/** @addtogroup STM32F0-Discovery_Demo
  * @{
  */

/** @addtogroup STM32F0XX_IT
  * @brief Interrupts driver modules
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
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    return;
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  return;
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
    return;
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
    return;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    return;
}

void FLASH_IRQHandler(void)
{
	return;
}
void USART2_IRQHandler(void)
{
	u32 tflag;
	u32 bitpos = 0, itmask = 0;

	tflag = USART2->ISR;
  	if(tflag & 0x00000020)
  	{
		bitpos = USART_IT_RXNE >> 0x10;
		itmask = ((u32)0x01 << (u32)bitpos);
		USART2->ICR = (u32)itmask;
		usart2Receivebuff[usart2Receivesave++] = (u8)(USART2->RDR);
		usart2Receivesave &= USART2_RECEIVE_SUB;
  	}
  	if(tflag & 0x00000080)
  	{
		USART2->ICR = USART_IT_TXE;
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
	if (tflag & BIT3)
	{
		USART2->ICR |= BIT3;
	}
	return;

}





void EXTI0_1_IRQHandler(void)
{
    //if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    { 
	//	EXTI_ClearITPendingBit(EXTI_Line1);
    }
	SysDelay(10);
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
  //  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    { 
		//EXTI_ClearITPendingBit(EXTI_Line5);
    }
  SysDelay(10);

}

void TIM2_IRQHandler(void)
{
	TIM2->SR = reset;
	SysDelay(10);

}

void USART3_4_IRQHandler(void)
{
	SysDelay(10);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
