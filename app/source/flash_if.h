
#ifndef _FLASH_IF_H
#define _FLASH_IF_H







#define BOOT_FLAG_ADDR			((uint32_t)0x08001000)
#define FLASH_IF_PRG_TIMEOUT	((uint32_t)0x000B0000)


/* define the address from where user application will be loaded,
   the application address should be a start sector address */
#define APPLICATION_ADDRESS     ((u32)0x08001800)

/******************************************************************************/
/*                                                                            */
/*                      FLASH and Option Bytes Registers                      */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FLASH_ACR register  ******************/
#define  FLASH_ACR_LATENCY                   ((uint32_t)0x00000001)        /*!< LATENCY bit (Latency) */

#define  FLASH_ACR_PRFTBE                    ((uint32_t)0x00000010)        /*!< Prefetch Buffer Enable */
#define  FLASH_ACR_PRFTBS                    ((uint32_t)0x00000020)        /*!< Prefetch Buffer Status */

/******************  Bit definition for FLASH_KEYR register  ******************/
#define  FLASH_KEYR_FKEYR                    ((uint32_t)0xFFFFFFFF)        /*!< FPEC Key */

/*****************  Bit definition for FLASH_OPTKEYR register  ****************/
#define  FLASH_OPTKEYR_OPTKEYR               ((uint32_t)0xFFFFFFFF)        /*!< Option Byte Key */

/******************  FLASH Keys  **********************************************/
#define FLASH_FKEY1                          ((uint32_t)0x45670123)        /*!< Flash program erase key1 */
#define FLASH_FKEY2                          ((uint32_t)0xCDEF89AB)        /*!< Flash program erase key2: used with FLASH_PEKEY1
                                                                                to unlock the write access to the FPEC. */
                                                               
#define FLASH_OPTKEY1                        ((uint32_t)0x45670123)        /*!< Flash option key1 */
#define FLASH_OPTKEY2                        ((uint32_t)0xCDEF89AB)        /*!< Flash option key2: used with FLASH_OPTKEY1 to
                                                                                unlock the write access to the option byte block */

/******************  Bit definition for FLASH_SR register  *******************/
#define  FLASH_SR_BSY                        ((uint32_t)0x00000001)        /*!< Busy */
#define  FLASH_SR_PGERR                      ((uint32_t)0x00000004)        /*!< Programming Error */
#define  FLASH_SR_WRPERR                     ((uint32_t)0x00000010)        /*!< Write Protection Error */
#define  FLASH_SR_EOP                        ((uint32_t)0x00000020)        /*!< End of operation */

/*******************  Bit definition for FLASH_CR register  *******************/
#define  FLASH_CR_PG                         ((uint32_t)0x00000001)        /*!< Programming */
#define  FLASH_CR_PER                        ((uint32_t)0x00000002)        /*!< Page Erase */
#define  FLASH_CR_MER                        ((uint32_t)0x00000004)        /*!< Mass Erase */
#define  FLASH_CR_OPTPG                      ((uint32_t)0x00000010)        /*!< Option Byte Programming */
#define  FLASH_CR_OPTER                      ((uint32_t)0x00000020)        /*!< Option Byte Erase */
#define  FLASH_CR_STRT                       ((uint32_t)0x00000040)        /*!< Start */
#define  FLASH_CR_LOCK                       ((uint32_t)0x00000080)        /*!< Lock */
#define  FLASH_CR_OPTWRE                     ((uint32_t)0x00000200)        /*!< Option Bytes Write Enable */
#define  FLASH_CR_ERRIE                      ((uint32_t)0x00000400)        /*!< Error Interrupt Enable */
#define  FLASH_CR_EOPIE                      ((uint32_t)0x00001000)        /*!< End of operation interrupt enable */
#define  FLASH_CR_OBL_LAUNCH                 ((uint32_t)0x00002000)        /*!< Option Bytes Loader Launch */

/*******************  Bit definition for FLASH_AR register  *******************/
#define  FLASH_AR_FAR                        ((uint32_t)0xFFFFFFFF)        /*!< Flash Address */

/******************  Bit definition for FLASH_OBR register  *******************/
#define  FLASH_OBR_OPTERR                    ((uint32_t)0x00000001)        /*!< Option Byte Error */
#define  FLASH_OBR_RDPRT1                    ((uint32_t)0x00000002)        /*!< Read protection Level 1 */
#define  FLASH_OBR_RDPRT2                    ((uint32_t)0x00000004)        /*!< Read protection Level 2 */

#define  FLASH_OBR_USER                      ((uint32_t)0x00003700)        /*!< User Option Bytes */
#define  FLASH_OBR_IWDG_SW                   ((uint32_t)0x00000100)        /*!< IWDG SW */
#define  FLASH_OBR_nRST_STOP                 ((uint32_t)0x00000200)        /*!< nRST_STOP */
#define  FLASH_OBR_nRST_STDBY                ((uint32_t)0x00000400)        /*!< nRST_STDBY */
#define  FLASH_OBR_nBOOT1                    ((uint32_t)0x00001000)        /*!< nBOOT1 */
#define  FLASH_OBR_VDDA_MONITOR              ((uint32_t)0x00002000)        /*!< VDDA power supply supervisor */

void FLASH_if_Init(void);
void FLASH_if_Unlock(void);
void FLASH_if_Lock(void);
void FLASH_if_ClearFlag(u32);
bool FLASH_if_Writehalfword(uint32_t,uint16_t);
bool FLASH_if_WaitForLastOperation(u32);
bool FLASH_if_ErasePage(u32);






#endif

