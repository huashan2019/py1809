 /**
   ******************************************************************************
   * @file	  RTC/RTC_Calendar/main.c 
   * @author  MCD Application Team
   * @version V1.3.0
   * @date	  16-January-2014
   * @brief   Main program body
   ******************************************************************************
   * @attention
   *
   * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
   *
   * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
   * You may not use this file except in compliance with the License.
   * You may obtain a copy of the License at:
   *
   *		http://www.st.com/software_license_agreement_liberty_v2
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
#define	CK_ROOT	
#include "config.h"
 
 /** @addtogroup STM32F0xx_StdPeriph_Examples
   * @{
   */
 
 /** @addtogroup RTC_Calendar
   * @{
   */
 
 /* Private typedef -----------------------------------------------------------*/
 /* Private define ------------------------------------------------------------*/
 /* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_ENABLE   1 /* LSE used as RTC source clock */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
 /*#define RTC_CLOCK_SOURCE_LSI */ // LSI used as RTC source clock. The RTC Clock
								 // may varies due to LSI frequency dispersion
#define	 EVAL_COM1	 USART1
 
#define BKP_VALUE    0x32F0 
 /* Private macro -------------------------------------------------------------*/
 /* Private variables ---------------------------------------------------------*/
 __IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
 
 /* Private function prototypes -----------------------------------------------*/
 static void RTC_Config(void);
 /* Private functions ---------------------------------------------------------*/
 
 /**
   * @brief  Main program.
   * @param  None
   * @retval None
   */
 
 /**
   * @brief  Configure the RTC peripheral by selecting the clock source.
   * @param  None
   * @retval None
   */
 static void RTC_Config(void)
 {
   RTC_AlarmTypeDef  RTC_AlarmStructure;
 
   /* Enable the PWR clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
 
   /* Allow access to RTC */
   PWR_BackupAccessCmd(ENABLE);
	 
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
 /* The RTC Clock may varies due to LSI frequency dispersion. */   
   /* Enable the LSI OSC */ 
   RCC_LSICmd(ENABLE);
 
   /* Wait till LSI is ready */  
   while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
   {
   }
 
   /* Select the RTC Clock Source */
   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
   
   SynchPrediv = 0x18F;
   AsynchPrediv = 0x63;
 
#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
   /* Enable the LSE OSC */
   RCC_LSEConfig(RCC_LSE_ON);
 
   /* Wait till LSE is ready */  
   while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
   {
   }
 
   /* Select the RTC Clock Source */
   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
   
   SynchPrediv = 0xFF;
   AsynchPrediv = 0x7F;
 
#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
   
   /* Enable the RTC Clock */
   RCC_RTCCLKCmd(ENABLE);
 
   /* Wait for RTC APB registers synchronisation */
   RTC_WaitForSynchro();
 }
 
 /**
   * @brief  Returns the time entered by user, using Hyperterminal.
   * @param  None
   * @retval None
   */
 void RTC_TimeRegulate(void)
 {
#if 1
   RTC_DateTypeDef	RTC_DateStructure;
   RTC_TimeTypeDef RTC_TimeStructure;
   RTC_AlarmTypeDef  RTC_AlarmStructure;
   uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
   uint32_t tmp_week = 0xFF, tmp_month = 0xFF, tmp_data = 0xFF, tmp_year = 0xFF;
 
   Printf("\n\r==============Time Settings=====================================\n\r");
   RTC_TimeStructure.RTC_H12	 = RTC_H12_AM;
   Printf("  Please Set Hours:\n\r");
   while (tmp_hh == 0xFF)
   {
	 tmp_hh = armtime.hour;//USART_Scanf(23);
	 RTC_TimeStructure.RTC_Hours = tmp_hh;
   }
   Printf("  %d\n\r", tmp_hh);
   
   Printf("  Please Set Minutes:\n\r");
   while (tmp_mm == 0xFF)
   {
	 tmp_mm = armtime.minute;//USART_Scanf(59);
	 RTC_TimeStructure.RTC_Minutes = tmp_mm;
   }
   Printf("  %d\n\r", tmp_mm);
   
   Printf("  Please Set Seconds:\n\r");
   while (tmp_ss == 0xFF)
   {
	 tmp_ss = armtime.second;//USART_Scanf(59);
	 RTC_TimeStructure.RTC_Seconds = tmp_ss;
   }
   Printf("  %d\n\r", tmp_ss);
 
   /* Configure the RTC time register */
   if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
   {
	 Printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
   } 
   else
   {
	 Printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
	 RTC_TimeShow();
 
	 RTC_ITConfig(RTC_IT_TS, ENABLE);
 
	 /* Indicator for the RTC configuration */
     RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
   }
 
 
   Printf("  Please Set week:\n\r");
   while (tmp_week== 0xFF)
   {
	 tmp_week= armtime.week;//USART_Scanf(7);
	 RTC_DateStructure.RTC_WeekDay= tmp_week;
   }
   Printf("  %d\n\r", tmp_week);
   
   Printf("  Please Set month:\n\r");
   while (tmp_month== 0xFF)
   {
	 tmp_month= armtime.month;//USART_Scanf(12);
	 RTC_DateStructure.RTC_Month = tmp_month;
   }
   Printf("  %d\n\r", tmp_month);
   
   Printf("  Please Set data:\n\r");
   while (tmp_data== 0xFF)
   {
	 tmp_data= armtime.date;//USART_Scanf(31);
	 RTC_DateStructure.RTC_Date= tmp_data;
   }
   Printf("  %d\n\r", tmp_data);
 
   Printf("  Please Set year:\n\r");
   while (tmp_year== 0xFF)
   {
	tmp_year = armtime.year%100;
	RTC_DateStructure.RTC_Year= tmp_year;
   }
   Printf("  %d\n\r", tmp_year);
   
   /* Configure the RTC time register */
   if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
   {
	 Printf("\n\r>> !! RTC Set Data failed. !! <<\n\r");
   } 
   else
   {
	 Printf("\n\r>> !! RTC Set Data success. !! <<\n\r");
	 RTC_DateShow();
   RTC_TimeStampCmd(RTC_TimeStampEdge_Rising, ENABLE);
 
	 /* Indicator for the RTC configuration */
	 RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
   }
 
#if 0
   tmp_hh = 0xFF;
   tmp_mm = 0xFF;
   tmp_ss = 0xFF;
 
   /* Disable the Alarm A */
   RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
 
   Printf("\n\r==============Alarm A Settings=====================================\n\r");
   RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
   Printf("  Please Set Alarm Hours:\n\r");
   while (tmp_hh == 0xFF)
   {
	 tmp_hh = USART_Scanf(23);
	 RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
   }
   Printf("  %2d\n\r", tmp_hh);
   
   Printf("  Please Set Alarm Minutes:\n\r");
   while (tmp_mm == 0xFF)
   {
	 tmp_mm = USART_Scanf(59);
	 RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
   }
   Printf("  %2d\n\r", tmp_mm);
   
   Printf("  Please Set Alarm Seconds:\n\r");
   while (tmp_ss == 0xFF)
   {
	 tmp_ss = USART_Scanf(59);
	 RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
   }
   Printf("  %2d", tmp_ss);
 
   /* Set the Alarm A */
   RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
   RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;	
   RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
 
   /* Configure the RTC Alarm A register */
   RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
   Printf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
   RTC_AlarmShow();
 
   /* Enable the RTC Alarm A Interrupt */
   RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
   /* Enable the alarm	A */
   RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  #endif
  #endif
 }
 
 /**
   * @brief  Display the current time on the Hyperterminal.
   * @param  None
   * @retval None
   */
 void RTC_TimeShow(void)
 {
   RTC_TimeTypeDef RTC_TimeStructure;
   /* Get the current Time */
   RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  #if	_UartDebug
   Printf("\n\r  The current time is :	h = %d:m = %d:s = %d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
#endif
 }
 
 /**
   * @brief  Display the current date on the Hyperterminal.
   * @param  None
   * @retval None
   */
 void RTC_DateShow(void)
 {
   RTC_DateTypeDef RTC_DateStructure;
   /* Get the current Date */
   RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  #if	_UartDebug
   Printf("\n\r  The current data is :	y = %d:m = %d:d = %d:w = %d \n\r",RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_WeekDay);
#endif
 }
 
 /**
   * @brief  Display the current time on the Hyperterminal.
   * @param  None
   * @retval None
   */
 void RTC_AlarmShow(void)
 {
   RTC_AlarmTypeDef  RTC_AlarmStructure;
   /* Get the current Alarm */
   RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  #if	_UartDebug
   Printf("\n\r  The current alarm is :  %d:%d:%d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
#endif
 }
 
 /**
   * @brief  Gets numeric values from the hyperterminal.
   * @param  None
   * @retval None
   */
 #if 0
 uint8_t USART_Scanf(uint32_t value)
 {
   uint32_t index = 0;
   uint32_t tmp[2] = {0, 0};
 
   while (index < 2)
   {
	 /* Loop until RXNE = 1 */
#if _UartDebug
	 while (rUARTRxBuff[index] == 0)
	 {}
	 tmp[index++] =rUARTRxBuff[index];
#else
	 while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
	 {}
	 tmp[index++] = (USART_ReceiveData(EVAL_COM1));
#endif	
	 if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
	 {
    #if	_UartDebug
	   Printf("\n\r Please enter valid number between 0 and 9 \n\r");
	#endif
	   index--;
	 }
   }
#if _UartDebug
   rTxBuffRdPnt = rTxBuffWrPnt = cUARTRxByte = 0;
   rUARTRxBuff[0] = rUARTRxBuff[1] = 0;
#endif
   /* Calculate the Corresponding value */
   index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
   /* Checks */
   if (index > value)
   {
  #if	_UartDebug
	 Printf("\n\r Please enter valid number between 0 and %d \n\r", value);
  #endif
	 return 0xFF;
   }
   return index;
 }
 #endif
#ifdef  USE_FULL_ASSERT
 
 /**
   * @brief  Reports the name of the source file and the source line number
   *		 where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
 void assert_failed(uint8_t* file, uint32_t line)
 { 
   /* User can add his own implementation to report the file name and line number,
	  ex: Printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
   /* Infinite loop */
   while (1)
   {
   }
 }
#endif

#define PPM_PER_STEP  0.9536743 //10^6/2^20.
#define PPM_PER_SEC   0.3858025 //10^6/(30d*24h*3600s).
 u16 FastSecPer30days = 60;//360; //菜单输入。117只用于演示。
 
 
 void RTC_Calibration(void)
 {
   float Deviation = 0.0;
   u8 CalibStep = 0;
   
   Deviation = FastSecPer30days * PPM_PER_SEC; //得到ppm误差
   Deviation /= PPM_PER_STEP; //得到校准值的浮点数
   CalibStep = (u8)Deviation; // 得到校准值的整形数
   if(Deviation >= (CalibStep + 0.5))
	 CalibStep += 1; //四舍五入
   if(CalibStep > 127) 
	 CalibStep = 127; // 校准值应在0—127之间
   
   RTC_SmoothCalibConfig(RTC_SmoothCalibPeriod_32sec,RTC_SmoothCalibPlusPulses_Reset,20);
	 
 }
 

 /**
   * @}
   */
 void RTC_Proc(void)
 {
   RTC_InitTypeDef RTC_InitStructure;
   NVIC_InitTypeDef  NVIC_InitStructure;
   EXTI_InitTypeDef  EXTI_InitStructure;
 
   /* Output a message on Hyperterminal using Printf function */
  // Printf("\n\r  *********************** RTC Hardware Calendar Example ***********************\n\r");
   
   if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
   {  
	 /* RTC configuration  */
	 RTC_Config();
 	 RTC_Calibration();
	 /* Configure the RTC data register and RTC prescaler */
	 RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
	 RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
	 RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
	 /* Check on RTC init */
	 if (RTC_Init(&RTC_InitStructure) == ERROR)
	 {
    #if	_UartDebug
	   Printf("\n\r 	   /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
	#endif
	 }
 
	 /* Configure the time register */
	 RTC_TimeRegulate(); 
   }
   else
   {
	 /* Check if the Power On Reset flag is set */
	 if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	 {
#if	_UartDebug
	   Printf("\r\n Power On Reset occurred....\n\r");
#endif
	 }
	 /* Check if the Pin Reset flag is set */
	 else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	 {
    #if	_UartDebug
	   Printf("\r\n External Reset occurred....\n\r");
	#endif
	 }
    #if	_UartDebug
	 Printf("\n\r No need to configure RTC....\n\r");
    #endif
	 /* Enable the PWR clock */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
 
	 /* Allow access to RTC */
	 PWR_BackupAccessCmd(ENABLE);
 
 #ifdef RTC_CLOCK_SOURCE_LSI
	 /* Enable the LSI OSC */ 
	 RCC_LSICmd(ENABLE);
 #endif /* RTC_CLOCK_SOURCE_LSI */
 
	 /* Wait for RTC APB registers synchronisation */
	 RTC_WaitForSynchro();
   }
   
 }
 /**
   * @}
   */
 
 /*******************STT ELECTRONICS TECHNOLOGY Co.,LTD***********************/
 /*****************************End of file************************************/
