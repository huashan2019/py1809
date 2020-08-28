/**
  ******************************************************************************
  * @file		main.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.02.05
  * @brief   
*/



#include "config.h"

u8 usart2Sendbuff[USART2_SEND_MAX];
u16 usart2SendSave;
u16 usart2SendIndex;
u8 usart2Sendbusy;

u8 usart2Receivebuff[USART2_RECEIVE_MAX];
u16 usart2Receivesave;
u16 usart2Receiveget;

void Uart2Init(void)
{
	usart2SendSave = reset;
	usart2SendIndex = reset;
	usart2Sendbusy = reset;
	usart2Receivesave = reset;
	usart2Receiveget = reset;
	
	RCC->APB1RSTR |= BIT17;
	RCC->APB1RSTR &= ~(u32)BIT17;
	RCC->APB1ENR &= ~(u32)BIT17;
	RCC->AHBENR &= 0xFFFFFFFE;				// disable DMA clock
	// PA2_TX,PA3_RX
	GPIOA->MODER |= 0x000000A0;			// AF mode
	GPIOA->OSPEEDR |= 0x000000F0;		// High speed
	GPIOA->AFR[0] &= 0xFFFF00FF;
	GPIOA->AFR[0] |= 0x00001100;			// AF1 (PA2 as uart2_tx, PA3 as uart2_rx)

	USART2->CR3 = 0x0;
	USART2->CR1 &= 0xFFFFFFFE;
	RCC->APB1RSTR |= BIT17;
	RCC->APB1RSTR &= ~(u32)BIT17;
	RCC->APB1ENR |= BIT17;			// enable uart2 clk
	NVIC->ISER[0] |= 0x10000000;		// enable uart2 interrupt
	//NVIC->IP[7] &= 0xFFFFFF00;			// NVIC_IRQChannel = 28     28/4 = 7      high1  priority
	//NVIC->IP[7] |= 0x0000000A;			// interrupt priority 10
	USART2->CR1 = 0x0000002C;
	// fpclk = 16000000, baud = 16000000/57600/16(OVER8 = 0) = 17.36111
	// DIV_Mantissa = 17 = 11h
	// DIV_Fraction = 0.36111*16 = 5.78 = 6
	USART2->BRR = 0x116;
	USART2->CR1 |= 0x00000001;
	USART2->CR1 |= 0x00000080;
}

void Uart2SendStr(u8 *p, u16 len)
{
	u16 i;

	for (i = 0; i < len; i++)
	{
		usart2Sendbuff[usart2SendSave++] = *p++;
		usart2SendSave &= USART2_SEND_SUB;
	}

	if (!usart2Sendbusy)
	{
		USART2->CR1 |= 0x00000080;
	}
}

