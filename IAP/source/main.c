/**
  ******************************************************************************
  * @file		main.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.02.05
  * @brief   
*/



#include "config.h"


typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
void VectorRemap(u32 addr);
#define IAP_ADDRESS			((u32)0x08000000)
#define SYSCFG_MemoryRemap_SRAM                 0x03

void main(void)
{
	u16 bootflag;
	
	bootflag = FLASH_if_Readhalfword(BOOT_FLAG_ADDR);

	if (bootflag == 0x8317)
	{
		VectorRemap(IAP_ADDRESS);
		FLASH_if_EraseApp();
		if ((USART2->CR1& 1) == reset)
		{
			ReSystemInit();
			BspInit();
			SystemPowerOn();
			SysDelay(160);
			SetArmRstHi();
			TFT_ON;
		}
		IapRun(); 
		FLASH_if_IAPEnd(0xFFFF);
		RCC->APB1ENR |= 0x00000800;
		WWDG->CR = 0xBF;
	}
	else
	{
		if (((*(__IO u32*)FLASH_APP_START_ADDR) & 0x2FFE0000 ) == 0x20000000)
		{ 
			JumpAddress = *(__IO u32*) (FLASH_APP_START_ADDR + 4);
			Jump_To_Application = (pFunction) JumpAddress;
			__set_MSP(*(__IO u32*) FLASH_APP_START_ADDR);
			Jump_To_Application();
		}
	}
	while(1)
	{
	}
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
    RCC->APB2ENR |= (u32)(RCC_APB2Periph_SYSCFG); 
    /* Remap SRAM at 0x00000000 */
    SYSCFG->CFGR1 |= (u32)(SYSCFG_MemoryRemap_SRAM);
}
void SystemInit(void)
{
	return;
}

