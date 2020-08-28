
#ifndef _BSP_H
#define _BSP_H



// PB14
#define TFT_ON				{GPIOB->BSRR |= BIT14;}
#define TFT_OFF				{GPIOB->BSRR |= BIT30;}
#define TFT_STATUS_ON			((GPIOB->ODR & BIT14) == BIT14)
#define TFT_STATUS_OFF		((GPIOB->ODR & BIT14) == reset)

// PC13
#define SetArmRstHi();			{GPIOC->MODER &= 0xF3FFFFFF; GPIOC->MODER |= 0x04000000;GPIOC->BSRR |= BIT13;}
#define SetArmRstLow();			{GPIOC->MODER &= 0xF3FFFFFF; GPIOC->MODER |= 0x04000000;GPIOC->BSRR |= BIT29;}
#define SetArmRstFloating();		{GPIOC->MODER &= 0xF3FFFFFF;}

// PB3
#define SetArmPowOn()			{GPIOB->BSRR |= BIT3;}
#define SetArmPowOff()			{GPIOB->BSRR |= BIT19;}
// PB4
#define CORE_POWER_ON		{GPIOB->BSRR |= BIT4;}
#define CORE_POWER_OFF		{GPIOB->BSRR |= BIT20;}
// PB6
#define MUTE_CTL_ON			{GPIOB->BSRR |= BIT22;}
#define MUTE_CTL_OFF			{GPIOB->BSRR |= BIT6;}
// PC2
#define ARM_SET_SLEEP			{GPIOC->BSRR |= BIT18;}
#define ARM_SET_WAKEUP		{GPIOC->BSRR |= BIT2;}

// PB5
#define ACC_PULL_ON			{GPIOB->BSRR |= BIT5;}
#define ACC_PULL_OFF			{GPIOB->BSRR |= BIT21;}
#define READ_ACC_PULLON		(GPIOB->ODR & BIT5)




void ReSystemInit(void);
void BspInit(void);
void SystemPowerOn(void);



#endif

