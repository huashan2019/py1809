

#include "config.h"





void BspInit(void)
{
	RCC->APB2RSTR = 0x00075A01;
	RCC->APB2RSTR = 0x00000000;
	RCC->APB1RSTR = 0x00624913;
	RCC->APB1RSTR = 0x00000000;
	RCC->AHBENR |= (u32)(BIT17 | BIT18|BIT19|BIT20|BIT0);// enable port A/B/C/D, DMA
	
	GPIOA->MODER = 0x28000000;
	GPIOB->MODER = 0x00000000;
	GPIOC->MODER = 0x00000000;
	GPIOD->MODER = 0x00000000;

	/****** OUTPUT **********/
	// PC2,PC13(ARM_RST)
	GPIOC->MODER |= 0x04000010;			// PC13 as output mode
	GPIOC->BSRR |= (u16)(BIT2|BIT29);		// low
	// PB3,PB4,PB5,PB6,PB14
	GPIOB->MODER |= 0x10001540;
	GPIOB->BSRR |= (u16)(BIT19|BIT20|BIT21|BIT22|BIT30);
}
void SystemPowerOn(void)
{
	ARM_SET_WAKEUP;
	SetArmRstLow();
	SysDelay(160);
	CORE_POWER_ON;
	SysDelay(160);
	SetArmPowOn();
	ACC_PULL_ON;
}

void ReSystemInit(void)
{
	u32 timeout;
   	/* Enable HSI */   
	RCC->CR = 0x00000001;
	RCC->CFGR &= (uint32_t)0xF8FFB80C;
	timeout = 2000;
	while((!(RCC->CR & 0x00000002)) && (timeout--));
	RCC->CFGR2 = RCC_CFGR_PPRE_DIV1;
	/* Enable HSE */   
	RCC->CR |= 0x00010000;
	timeout = 2000;
	while((!(RCC->CR & 0x00020000)) && (timeout--));
	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR |= FLASH_ACR_PRFTBE;// | FLASH_ACR_LATENCY;
	/* HCLK = SYSCLK */
	RCC->CFGR |= (u32)RCC_CFGR_HPRE_DIV1;
	/* PCLK = HCLK */
	RCC->CFGR |= (u32)RCC_CFGR_PPRE_DIV1;
	/* PLL configuration = HSE * 2 = 16 MHz */
	RCC->CFGR &= (u32)((u32)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL));
	RCC->CFGR |= (u32)(RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PPRE_DIV1 | RCC_CFGR_PLLMUL2);

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait till PLL is ready */
	while((RCC->CR & RCC_CR_PLLRDY) == 0)
	{
	}

	/* Select PLL as system clock source */
	RCC->CFGR &= (u32)((u32)~(RCC_CFGR_SW));
	RCC->CFGR |= (u32)RCC_CFGR_SW_PLL;

	/* Wait till PLL is used as system clock source */
	while ((RCC->CFGR & (u32)RCC_CFGR_SWS) != (u32)RCC_CFGR_SWS_PLL)
	{
	}
}

