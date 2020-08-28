
// clock head file

#ifdef   CK_ROOT
#define  CK_EXT
#else
#define  CK_EXT     extern
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_DateShow(void);

void RTC_AlarmShow(void);
//uint8_t USART_Scanf(uint32_t value);


