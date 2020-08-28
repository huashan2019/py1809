
#ifndef _BSP_H
#define _BSP_H





#define DI				__set_PRIMASK(1)
#define EI					__set_PRIMASK(0)

#define ADC_CH_MAX			6


#define ADC_CH_PA0			0x00000001		// ADC_IN0
#define ADC_CH_PA1			0x00000002		// ADC_IN1
#define ADC_CH_PA2			0x00000004		// ADC_IN2
#define ADC_CH_PA3			0x00000008		// ADC_IN3
#define ADC_CH_PA4			0x00000010		// ADC_IN4
#define ADC_CH_PA5			0x00000020		// ADC_IN5
#define ADC_CH_PA6			0x00000040		// ADC_IN6
#define ADC_CH_PA7			0x00000080		// ADC_IN7
#define ADC_CH_PB0			0x00000100		// ADC_IN8
#define ADC_CH_PB1			0x00000200		// ADC_IN9
#define ADC_CH_PC0			0x00000400		// ADC_IN10
#define ADC_CH_PC1			0x00000800		// ADC_IN11
#define ADC_CH_PC2			0x00001000		// ADC_IN12
#define ADC_CH_PC3			0x00002000		// ADC_IN13
#define ADC_CH_PC4			0x00004000		// ADC_IN14
#define ADC_CH_PC5			0x00008000		// ADC_IN15



// PC5
#define IO_ACC_EXIST			((GPIOC->IDR & BIT5) == reset)
// PC9
#define EXT_CAMERA_PWR_ON		{GPIOC->BSRR |= BIT9;}
#define EXT_CAMERA_PWR_OFF		{GPIOC->BSRR |= BIT25;}
// PB12
#define TEST_ON					MUTE_CTL_ON//{GPIOB->BSRR |= BIT12;}
#define TEST_OFF				MUTE_CTL_OFF//{GPIOB->BSRR |= BIT28;}

// PB14
#define TFT_ON					{GPIOB->BSRR |= BIT14;}
#define TFT_OFF					{GPIOB->BSRR |= BIT30;}
#define TFT_STATUS_ON			((GPIOB->ODR & BIT14) == BIT14)
#define TFT_STATUS_OFF			((GPIOB->ODR & BIT14) == reset)

// PC13
#define SetArmRstHi();			{GPIOC->MODER &= 0xF3FFFFFF; GPIOC->MODER |= 0x04000000;GPIOC->BSRR |= BIT13;}
#define SetArmRstLow();			{GPIOC->MODER &= 0xF3FFFFFF; GPIOC->MODER |= 0x04000000;GPIOC->BSRR |= BIT29;}
#define SetArmRstFloating();	{GPIOC->MODER &= 0xF3FFFFFF;}

// PB10
#define GPIO_I2C_SCL_RESET		{GPIOB->BSRR |= BIT26;}
#define GPIO_I2C_SCL_SET		{GPIOB->BSRR |= BIT10;}
#define Pin_Radio_I2CClk_In		(GPIOB->IDR & BIT10)

//PB11
#define GPIO_I2C_SDA_RESET		{GPIOB->BSRR |= BIT27;}
#define GPIO_I2C_SDA_SET		{GPIOB->BSRR |= BIT11;}
#define READ_I2C_SDA			(GPIOB->IDR & BIT11)
#define GPIO_I2C_SDA_INPUT		GPIO_I2C_SDA_SET
// PB3
#define SetArmPowOn()			{GPIOB->BSRR |= BIT3;}
#define SetArmPowOff()			{GPIOB->BSRR |= BIT19;}
#define IFArmpowOn				(GPIOB->ODR & BIT3)
// PB4
#define CORE_POWER_ON			{GPIOB->BSRR |= BIT4;}
#define CORE_POWER_OFF			{GPIOB->BSRR |= BIT20;}
// PB5
#define ACC_PULL_ON				{GPIOB->BSRR |= BIT5;}
#define ACC_PULL_OFF			{GPIOB->BSRR |= BIT21;}
#define READ_ACC_PULLON			(GPIOB->ODR & BIT5)
// PB6
#ifdef _MUTE_REVERSE
#define MUTE_CTL_ON				{GPIOB->BSRR |= BIT22;}
#define MUTE_CTL_OFF			{GPIOB->BSRR |= BIT6;}
#else
#define MUTE_CTL_ON				{GPIOB->BSRR |= BIT6;}
#define MUTE_CTL_OFF			{GPIOB->BSRR |= BIT22;}
#endif
//PB13
#define oPWM_L_ON				{GPIOB->BSRR |= BIT13;}
#define oPWM_L_OFF				{GPIOB->BSRR |= BIT29;}
//PB14
#define oPWM_R_ON				{GPIOB->BSRR |= BIT14;}
#define oPWM_R_OFF				{GPIOB->BSRR |= BIT30;}

// PC1
#define iIR_IN					((GPIOC->IDR & BIT1)/* == reset*/)
//#define oIR_ON					{GPIOB->BSRR |= BIT7;}
//#define oIR_OFF					{GPIOB->BSRR |= BIT23;}

// PC2
#define ARM_SET_SLEEP			{GPIOC->BSRR |= BIT18;}
#define ARM_SET_WAKEUP			{GPIOC->BSRR |= BIT2;}
// PC3
#define ARM_WAKEUP_REP			(GPIOC->IDR & BIT3)
// PC4
#define AMP_STANDBY_ON			{GPIOC->BSRR |= BIT8;}
#define AMP_STANDBY_OFF			{GPIOC->BSRR |= BIT20;}
// PD2
#define TJA_SET_STANDBY			{GPIOD->MODER &= 0xFFFFFFCF;GPIOD->MODER |= 0x00000010;GPIOD->BSRR |= BIT2;}
#define TJA_SET_NORMAL			{GPIOD->MODER &= 0xFFFFFFCF;GPIOD->MODER |= 0x00000010;GPIOD->BSRR |= BIT18;}
#define TJA_SET_FLOATING		{GPIOD->MODER &= 0xFFFFFFCF;}

// PA5
#define RADIO_INT_L		{GPIOA->BSRR |= BIT21;}
#define RADIO_INT_H		{GPIOA->BSRR |= BIT5;}

// PA11
#define RADIO_Pin_RSTN_L		{GPIOA->BSRR |= BIT27;}
#define RADIO_Pin_RSTN_H		{GPIOA->BSRR |= BIT11;}
//#define Pin_RSTN_L				RADIO_Pin_RSTN_L
//#define Pin_RSTN_H				RADIO_Pin_RSTN_H


// PA12
#define RADIO_Pin_RDS_L			{GPIOA->MODER &= 0xFCFFFFFF;GPIOA->MODER |= 0x01000000;GPIOA->BSRR |= BIT28;}
#define RADIO_Pin_RDS_H			{GPIOA->MODER &= 0xFCFFFFFF;GPIOA->MODER |= 0x01000000;GPIOA->BSRR |= BIT12;}
#define RADIO_PD_RDS_IN			{GPIOA->MODER &= 0xFCFFFFFF;}
#define RADIO_PINT_RDS			{}

//PB1
#define RADIO_PWR_ON			{GPIOB->BSRR |= BIT1;}
#define RADIO_PWR_OFF			{GPIOB->BSRR |= BIT17;}

// PB2
#define EX_ANT_OPEN				{GPIOB->BSRR |= BIT2;}
#define EX_ANT_CLOSE			{GPIOB->BSRR |= BIT18;}

// PA15
#define EX_AMP_OPEN				{GPIOA->BSRR |= BIT15;}
#define EX_AMP_CLOSE			{GPIOA->BSRR |= BIT31;}

// PC10 BACK CAR
//#define IO_CAM_EXIST			((GPIOC->IDR & BIT10) == reset)
#define IO_CAM_EXIST			((GPIOC->IDR & BIT6) == reset)
// PC12 PARKING
#define IO_PARK_EXIST			((GPIOC->IDR & BIT12) == reset)
// PC11 CAR ILL
#define IO_ILL_EXIST			((GPIOC->IDR & BIT7) == reset)


#define PinIn_RDSDATA	


extern u16 adcvalueBuff[ADC_CH_MAX];

void BspInit(void);
void GpioInit(void);
void Uart2Init(void);
void Uart2Deinit(void);
void Time14Init(void);
void Time14Deinit(void);
void AdcInit(void);
void AdcDeinit(void);
void GalaInit(void);
void GalaDeInit(void);

void PWM_Init(void);
void PWM_DeInit(void);

void SystemClkDown(void);
void WWDGReset(void);
void McuIapStart(void);
void VectorRemap(u32);
void SysDelay(u32);
void SysDelay10us(u32);



#endif


