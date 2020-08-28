/**
  ******************************************************************************
  * @file		main.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.02.05
  * @brief   
*/

/*****************************************
 nvic priority map

 rtc	---9(default)
 usart1 ---10
 usart2 ---11
 Tim14	---12
 Tim15	---13
 
*****************************************/

#include "config.h"


typedef  void (*pFunction)(void);
pFunction Jump_To_Iap;
u32 IAPJumpAddress;

#define IAP_ADDRESS			((u32)0x08000000)
#define SYSCFG_MemoryRemap_SRAM                 0x03

u16 adcvalueBuff[ADC_CH_MAX];
EXTI_InitTypeDef EXTI_InitStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
////=================================================================================================================
#define UART1_BAUDRATE       115200
#define UART2_BAUDRATE       115200
#define UART3_BAUDRATE       9600//115200
#define UART4_BAUDRATE       115200

void Bsp_UART_Init(void)
{
	SysUartInit(SCH_Uart1,UART1_BAUDRATE);
	SysUartInit(SCH_Uart2,UART2_BAUDRATE);
	//SysUartInit(SCH_Uart3,UART3_BAUDRATE); 
	SysUartInit(SCH_Uart4,UART4_BAUDRATE); 
}


void BspInit(void)
{
	RCC->APB2RSTR = (u32)0x00075A01;
	RCC->APB2RSTR = (u32)0x00000000;
	RCC->APB1RSTR = (u32)0x02624913;
	RCC->APB1RSTR = (u32)0x00000000;
	RCC->AHBENR |= (u32)(BIT17|BIT18|BIT19|BIT20|BIT0);// enable port A/B/C/D, DMA
	
	GpioInit();
	Time14Init();
	AdcInit();
	//GalaInit();
	PWM_Init();
	Bsp_UART_Init();
}
void GpioInit(void)
{
	GPIOA->MODER = (u32)0x28000000;
	GPIOB->MODER = (u32)0x00000000;
	GPIOC->MODER = (u32)0x00000000;
	GPIOD->MODER = (u32)0x00000000;

	// PA0,PA1,PA4,PA5,PA6,PB0 as Anolog mode
	GPIOA->MODER |= (u32)0x0000330F;
	GPIOB->MODER |= (u32)0x00000003;
	
	/****** OUTPUT **********/
	// PC0,PC1,PC2,PC4,PC9,PC13
	GPIOC->MODER |= (u32)0x04040315;
	GPIOC->BSRR |= (u32)(BIT18|BIT20|BIT25|BIT29);
	// PB1~PB6,PB10,PB11,PB14
	GPIOB->MODER |= (u32)0x11505554;
	GPIOB->OSPEEDR |= (u32)0x00F00000;		// PB10,PB11 HIGH SPEED
	GPIOB->OTYPER |= (u32)(BIT10|BIT11); 	// open-drain
	#ifndef _MUTE_REVERSE
	GPIOB->BSRR |= (u32)(BIT6|BIT17|BIT18|BIT19|BIT20|BIT21|BIT26|BIT27|BIT30);
	#else
	GPIOB->BSRR |= (u32)(BIT12|BIT17|BIT18|BIT19|BIT20|BIT21|BIT22|BIT26|BIT27|BIT30);
	#endif
	// PA2,PA3,PA11,PA12,PA15
	GPIOA->MODER |= (u32)0x41400450;
	GPIOA->BSRR |= (u32)(BIT18|BIT19|BIT21|BIT27|BIT28|BIT31);
}

void Uart4Init(void)
{

}

void Uart4Deinit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	USART_DeInit(USART4);
	USART_ITConfig(USART4, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART4, USART_IT_TXE, DISABLE);	
	USART_ClearFlag(USART4,USART_FLAG_TC);
	USART_ClearITPendingBit(USART4,USART_FLAG_TC);
	USART_Cmd(USART4, DISABLE);
	/*USART1_TX ->PA9  USART1_RX ->PA10*/		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;	   //	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
	GPIO_Init(GPIOC, &GPIO_InitStructure);		 //

}


void Uart2Init(void)
{
	// PA2_TX,PA3_RX
	GPIOA->MODER &= (u32)0xFFFFFF0F;
	GPIOA->MODER |= (u32)0x000000A0;			// AF mode
	GPIOA->OSPEEDR |= (u32)0x000000F0;		// High speed
	GPIOA->AFR[0] &= (u32)0xFFFF00FF;
	GPIOA->AFR[0] |= (u32)0x00001100;			// AF1 (PA2 as uart2_tx, PA3 as uart2_rx)
	
	RCC->APB1RSTR |= (u32)BIT17;
	RCC->APB1RSTR &= ~(u32)BIT17;
	RCC->APB1ENR |= (u32)BIT17;			// enable uart2 clk
	NVIC->ISER[0] |= (u32)0x10000000;		// enable uart2 interrupt
	NVIC->IP[7] &= (u32)0xFFFFFF00;			// NVIC_IRQChannel = 28     28/4 = 7      high1  priority
	NVIC->IP[7] |= (u32)0x0000000A;			// interrupt priority 10
	USART2->CR1 = (u32)0x0000002C;
	// fpclk = 16000000, baud = 16000000/57600/16(OVER8 = 0) = 17.36111
	// DIV_Mantissa = 17 = 11h
	// DIV_Fraction = 0.36111*16 = 5.78 = 6
	USART2->BRR = (u32)0x116;
	USART2->CR1 |= (u32)0x00000001;
	USART2->CR1 |= (u32)0x00000080;
}

void Uart2Deinit(void)
{
	RCC->APB1RSTR |= (u32)BIT17;
	RCC->APB1RSTR &= ~(u32)BIT17;
	RCC->APB1ENR &= ~(u32)BIT17;
	GPIOA->MODER &= 0xFFFFFF0F; // PA2,PA3
	GPIOA->AFR[0] &= 0xFFFF00FF;
	GPIOA->MODER |= 0x00000050;
	GPIOA->BSRR |= (u32)(BIT18|BIT19);
}
void Uart1Init(void)
{
	// PA9_TX_CAN,PA10_RX_CAN
	GPIOA->MODER |= (u32)0x00280000;			// PA9 PA10 as AF mode
	GPIOA->OSPEEDR |= (u32)0x003C0000;		// High Speed
	GPIOA->AFR[1] &= (u32)0xFFFFF00F;
	GPIOA->AFR[1] |= (u32)0x00000110;			// AF1 (PA9 as uart1_tx, PA10 as uart1_rx)
	
	RCC->APB2RSTR |= (u32)BIT14;
	RCC->APB2RSTR &= ~(u32)BIT14;
	RCC->APB2ENR |= (u32)BIT14;			// enable uart1 clk
	NVIC->ISER[0] |= (u32)0x08000000;		// enable uart1 interrupt
	NVIC->IP[6] &= (u32)0x00FFFFFF;			// NVIC_IRQChannel = 27     27/4 = 6.75 = 6      high1  priority
	NVIC->IP[6] |= (u32)0x0B000000;			// interrupt priority 11
	USART1->CR1 = (u32)0x0000002C;
	// fpclk = 16000000, baud = 16000000/38400
	USART1->BRR = (u32)0x1A0;
	USART1->CR1 |= (u32)0x00000001;
}
void Uart1Deinit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC->APB2RSTR |= (u32)BIT14;
	RCC->APB2RSTR &= ~(u32)BIT14;
	RCC->APB2ENR &= ~(u32)BIT14;
	GPIOA->MODER &= 0xFFC3FFFF; // PA9,PA10
	GPIOA->AFR[1] &= 0xFFC3FFFF;
	GPIOA->MODER |= 0x00140000;
	GPIOA->BSRR |= (u32)(BIT25|BIT26);

	
	/*USART1_TX ->PA9  USART1_RX ->PA10*/		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;	   //	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
	GPIO_Init(GPIOA, &GPIO_InitStructure);		 //

}

void Time14Init(void)
{
	RCC->APB1ENR |= BIT8;
	NVIC->ISER[0] |= BIT19;				// enableTim14 interrupt
	NVIC->IP[4] &= 0x00FFFFFF;			// NVIC_IRQChannel = 19     19/4 = 4      high2  priority
	NVIC->IP[4] |= 0x0C000000;			// interrupt priority 12

	TIM14->ARR = 2000;
	TIM14->PSC = (u16)(16-1);			// 16Mhz/(PSC+1) = 1Mhz
	TIM14->CNT = reset;
	TIM14->DIER |= 0x0001;
	TIM14->CR1 |= 0x0001;
}
void Time14Deinit(void)
{
	RCC->APB1RSTR |= (u32)BIT8;
	RCC->APB1RSTR &= ~(u32)BIT8;
	RCC->APB1ENR &= ~(u32)BIT8;
}
void AdcInit(void)
{
	u32 timeout;

	RCC->APB2ENR |= (u32)BIT9;	// enable ADC clock
	RCC->AHBENR |= (u32)BIT0;	// enable DMA

	ADC1->CR = (u32)0x80000000;
	timeout = 2000;
	while((ADC1->CR & 0x80000000) && (timeout--));
	ADC1->CFGR1 |= (u32)0x00002008;	// continuous mode, right ALIGN, 10bits
	ADC1->CFGR1 |= (u32)0x00000001;	// DMA enable
	ADC1->CFGR1 |= (u32)0x00000002;	// DMA circular mode 
	ADC1->CFGR2 = (u32)0x80000000;		// PCLK/4 = (16/4 = 4MHZ)
	ADC1->SMPR = (u32)0x00000001;//0x00000007;		// 7.5 ADC clock cycles
	ADC1->CHSELR |= (u32)(ADC_CH_PA0|ADC_CH_PA1|ADC_CH_PA4|ADC_CH_PA6|ADC_CH_PB0|ADC_CH_PC4);

	DMA1_Channel1->CCR |= (u32)0x000005A0;
	DMA1_Channel1->CNDTR = (u32)ADC_CH_MAX;		// ADC channel to transfer
	DMA1_Channel1->CPAR = (u32)(&ADC1->DR);
	DMA1_Channel1->CMAR = (u32)adcvalueBuff;
	
	ADC1->CR |= (u32)0x00000001;
	ADC1->CR |= (u32)0x00000004;
	DMA1_Channel1->CCR |= (u32)0x00000001;
}
void AdcDeinit(void)
{
	ADC1->CR = reset;
	RCC->APB2RSTR |= BIT9;
	RCC->APB2RSTR &= ~(u32)BIT9;
	DMA1_Channel1->CCR = reset;
	DMA1_Channel1->CNDTR = reset;
	DMA1_Channel1->CPAR = reset;
	DMA1_Channel1->CMAR = reset;
	RCC->APB2ENR &= ~(u32)BIT9;
}
void GalaInit(void)
{
	u32 tmp = 0;
	#if 0
	// PB7_INT
	//GPIOB->MODER &= (u32)0xFFFF3FFF;
	//GPIOB->MODER |= (u32)0x00000000;		// AF mode
	//GPIOB->OSPEEDR |= (u32)0x0000C000;		// High speed
	//GPIOB->PUPDR |= (u32)0x00004000;		//Pull up/down on

	
	/* Initializes the EXTI_InitStructure */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;//|EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;//|EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(/*EXTI_Line0 |*/ EXTI_Line3 /*|EXTI_Line4*/);
#endif
}

void GalaDeInit(void)
{
	// PB7_INT
	//GPIOB->MODER &= (u32)0xFFFF3FFF;
	//GPIOB->MODER |= (u32)0x00000000;			// AF mode
	//GPIOB->OSPEEDR |= (u32)0x0000C000;			// High speed
	//GPIOB->PUPDR |= (u32)0x00000000;			//no Pull up/down
}

void PWM_Init(void)
{
#if 0
	uint16_t TimerPeriod = 0;
	uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA, GPIOB and GPIOE Clocks enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* GPIOB Configuration: Channel 2N and 3N as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_2); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_2);

	/* TIM1 Configuration ---------------------------------------------------
	Generate 7 PWM signals with 4 different duty cycles:
	TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)	  
	=> TIM1CLK = PCLK2 = SystemCoreClock
	TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
	SystemCoreClock is set to 8 MHz for STM32F0xx devices

	The objective is to generate 7 PWM signal at 1 KHz:
	 - TIM1_Period = (SystemCoreClock / 1000) - 1
	The channel 1 and channel 1N duty cycle is set to 50%
	The Timer pulse is calculated as follows:
	 - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

	Note: 
	SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
	Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	function to update SystemCoreClock variable value. Otherwise, any configuration
	based on this variable will be incorrect. 
	----------------------------------------------------------------------- */
	/* Compute the value to be set in ARR regiter to generate signal frequency at 2 Khz */
	TimerPeriod = (SystemCoreClock/ 500 ) - 1;/*PLL 16M 1KHZ*/
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
	//Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
	Channel1Pulse = TimerPeriod;

	/* TIM16 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);/*channel 1 PB13*/

	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);/*channel 2 PB14*/

	
	TIM_ARRPreloadConfig(TIM1, DISABLE);
	oPWM_R_OFF;
	oPWM_L_OFF;
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	#endif
}

void PWM_DeInit()
{
	// PB14_INT
	//GPIOB->MODER &= (u32)0xCFFFFFFF;
	//GPIOB->MODER |= (u32)0x00000000;		// AF mode
	//GPIOB->OSPEEDR |= (u32)0x30000000;		// High speed
	//GPIOB->PUPDR |= (u32)0x10000000;		//Pull up/down on

	TIM_Cmd(TIM1, DISABLE);
}

void SystemClkDown(void)
{
	u32 timeout;
   	/* Enable HSI */   
	RCC->CR = 0x00000001;
	timeout = 2000;
	while((!(RCC->CR & 0x00000002)) && (timeout--));
	/* Enable HSE */   
	RCC->CR |= 0x00010000;
	timeout = 2000;
	while((!(RCC->CR & 0x00020000)) && (timeout--));
	RCC->CFGR2 = RCC_CFGR_PPRE_DIV1;
	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR |= FLASH_ACR_PRFTBE;// | FLASH_ACR_LATENCY;
	/* HCLK = SYSCLK */
	RCC->CFGR |= (u32)RCC_CFGR_HPRE_DIV16;
	/* PCLK = HCLK */
	RCC->CFGR |= (u32)RCC_CFGR_PPRE_DIV1;    

	/* Select HSE as system clock source */
	RCC->CFGR &= (u32)((u32)~(RCC_CFGR_SW));
	RCC->CFGR |= (u32)RCC_CFGR_SW_HSE;

	/* Wait till HSE is used as system clock source */
	while ((RCC->CFGR & (u32)RCC_CFGR_SWS) != (u32)RCC_CFGR_SWS_HSE)
	{
	}

	RCC->CR  &= 0xFFFFFFFE;	// HSI OFF
}

void WWDGReset(void)
{
	RCC->APB1ENR |= BIT11;
	WWDG->CR = 0xC0;
	while(1);
}
u16 bootflag;

void McuIapStart(void)
{

	NVIC->ISER[0] = reset;		// NIVC->DeInit()
	
	IrdaDeInit();
	Disable_RDS_Int();
	EXTI_ClearITPendingBit(EXTI_Line1);
	EXTI_ClearITPendingBit(EXTI_Line5);
	
	TIM_Cmd(TIM2, DISABLE); 	//使能TIMx外设
	Time14Deinit();
	AdcDeinit();
	GalaDeInit();
	PWM_DeInit();
	Uart4Deinit();
	Uart1Deinit();
	RCC->AHBENR &= 0xFFFFFFFE;				// disable DMA clock
	USART2->CR3 = 0;
	
	USART_ClearFlag(USART1, USART_FLAG_ORE);
	USART_ClearFlag(USART2, USART_FLAG_ORE);
	USART_ClearFlag(USART4, USART_FLAG_ORE);

	FLASH_if_Init();
	FLASH_if_ErasePage(BOOT_FLAG_ADDR);
	FLASH_if_Writehalfword(BOOT_FLAG_ADDR,0x8317);
	FLASH_if_Lock();
	
	
	bootflag = FLASH_if_Readhalfword(BOOT_FLAG_ADDR);
	//VectorRemap(IAP_ADDRESS);
	IAPJumpAddress = *(__IO uint32_t*) (IAP_ADDRESS + 4);
	Jump_To_Iap = (pFunction) IAPJumpAddress;
	__set_MSP(*(__IO u32*) IAP_ADDRESS);
	Jump_To_Iap();
	while(1);
}
void VectorRemap(u32 addr)
{
    u32 i = 0;

    /* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  
    /* Copy the vector table from the Flash (mapped at the base of the application
    load address 0x08001800) to the base address of the SRAM at 0x20000000. */       
    for(i = 0; i < 48; i++)
    {
        *((u32*)(0x20000000 + (i << 2)))= *(__IO u32*)(addr + (i<<2));
    }
    /* Enable the SYSCFG peripheral clock*/ 
    RCC->APB2ENR |= (u32)(RCC_APB2ENR_SYSCFGEN); 
    /* Remap SRAM at 0x00000000 */
    SYSCFG->CFGR1 |= (u32)(SYSCFG_MemoryRemap_SRAM);
}

void SysDelay(u32 delay)	// 1ms
{
	u32 i,j;

	for (i = 0; i < delay; i++)
	{
		for (j = 0; j < 2200; j++);
	}
}
void SysDelay10us(u32 delay)
{
	u32 i,j;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;


}

