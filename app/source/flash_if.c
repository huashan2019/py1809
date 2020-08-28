/**
  ******************************************************************************
  * @file		flash_if.c
  * @author	WYD
  * @version	V1.0.0
  * @date	2015.04.01
  * @brief   
*/



#include "config.h"



void FLASH_if_Init(void)
{ 
  /* Unlock the Program memory */
  FLASH_if_Unlock();

  /* Clear all FLASH flags */  
  FLASH_if_ClearFlag(FLASH_SR_EOP | FLASH_SR_WRPERR | FLASH_SR_PGERR | FLASH_SR_BSY);   
}
u16 FLASH_if_Readhalfword(u32 addr)
{
	u32 tempflashdata;
	
	tempflashdata = *(__IO u32*)addr;
	return (u16)(tempflashdata);
}

bool FLASH_if_Writehalfword(uint32_t faddr,uint16_t fdata)
{
	bool status;
	u32 tempBY;
	
	status = FLASH_if_WaitForLastOperation(FLASH_IF_PRG_TIMEOUT);
	if (status == TRUE)
	{
		FLASH->CR |= FLASH_CR_PG;
		*(volatile u16*)faddr = fdata;
		status = FLASH_if_WaitForLastOperation(FLASH_IF_PRG_TIMEOUT);
		FLASH->CR &= ~FLASH_CR_PG;
	}
	return status;
}
bool FLASH_if_ErasePage(u32 Page_Address)
{
	bool status;
	/* Wait for last operation to be completed */
	status = FLASH_if_WaitForLastOperation(FLASH_IF_PRG_TIMEOUT);

	if(status == TRUE)
	{
		/* If the previous operation is completed, proceed to erase the page */
		FLASH->CR |= FLASH_CR_PER;
		FLASH->AR  = Page_Address;
		FLASH->CR |= FLASH_CR_STRT;

		/* Wait for last operation to be completed */
		status = FLASH_if_WaitForLastOperation(FLASH_IF_PRG_TIMEOUT);

		/* Disable the PER Bit */
		FLASH->CR &= ~FLASH_CR_PER;
	}

	/* Return the Erase Status */
	return status;
}

void FLASH_if_Unlock(void)
{
	/* Unlocking the program memory access */
	FLASH->KEYR = FLASH_FKEY1;
	FLASH->KEYR = FLASH_FKEY2;
}
void FLASH_if_Lock(void)
{
    /* Set the LOCK Bit to lock the FLASH control register and program memory access */
    FLASH->CR |= FLASH_CR_LOCK;
}
void FLASH_if_ClearFlag(u32 FLASH_FLAG)
{
    /* Clear the flags */
    FLASH->SR = FLASH_FLAG;
}

bool FLASH_if_WaitForLastOperation(u32 timeout)
{
	while(timeout--)
	{
		if((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY)
		{
			continue;
		}
        else if ((FLASH->SR & (u32)FLASH_SR_WRPERR) != (u32)0x00)
        {
            continue;
        }
        else if ((FLASH->SR & (u32)(FLASH_SR_PGERR)) != (u32)0x00)
        {
            continue;
        }
		else
			return TRUE;
	}
	return FALSE;
}


