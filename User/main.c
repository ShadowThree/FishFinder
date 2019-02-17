#include "stm32f0xx.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"

#define N 8
#define T DATA_BUFF/N
extern uint8_t result[DATA_BUFF];
uint32_t results = 0;
uint32_t distance = 0;
extern uint8_t flag;
uint8_t max = 0, count = 0;
uint16_t t = 0, mi = 0, sum[3];
uint32_t sumPrevage = 0;
void InitRCC(void);
void InitNVIC(void);
void EXTI_Config(void);
void delay_ms(uint16_t t);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	InitRCC();
	InitNVIC();
	InitGPIOs();
	EXTI_Config();
	InitUart1();
	ADC_Config();
	InitTIMs();
	//ADC_StartOfConversion(ADC1);
	while(1)
	{
		/******************************************************************/
		if(flag)
		{
			flag = 0;
			results = 0;
			max = 0;
			for(mi = 0; mi < N; mi++)   // get the first N data's sum
			{
				sum[0] += result[mi];
			}
			for(mi = N; mi < N*2; mi++)  // get the second N data's sum
			{
				sum[1] += result[mi];
			}
			
			t = 2;
			while(1)
			{
					for(; t < T; t++)      // N is mean the number of the data in each group, T is mean the number of group.
					{
						for(mi = N*t; mi < N*(t + 1); mi++)
						{
							sum[2] += result[mi];
						}
						if((sum[1] >= sum[0]) && (sum[1] >= sum[2]))
						{
							max = result[N*(t-1)];
							results = N*(t-1) + 1;
							sum[0] = 0;
							sum[1] = 0;
							sum[2] = 0;
							break;
						}
						else
						{
							sum[0] = sum[1];
							sum[1] = sum[2];
							sum[2] = 0;
						}
					}
					if(t == T)
					{
						results = 0;
						max = 0;
						break;
					}
					for(mi = 1; mi < N; mi++)
					{
						if(max < result[N*(t-1) + mi])
						{
							max = result[N*(t-1) + mi];
							results = N*(t-1) + mi + 1;
						}
					}
					if(max > 30)
						break;
			}
			
			
			sumPrevage = 0;
			for(mi = 0; mi < results; mi++)
			{
				sumPrevage += result[mi];
			}
			sumPrevage /= results;
			
			  /******************** 发送所有数据 ******
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, 0x00);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, 0xff);
			for(mi = 0; mi < 1024; mi++)
			{
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1,  result[mi]);
			}
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, 0xff);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, 0x00);
				*****************************/

			results = results*81/160;     // results*81*1500*100/12/1000000/2;  计算距离，单位是厘米(采样率81个周期)
			results = results*31/30;      // calibration by real data
			if(results < 20)
				results = 0;
				/*********************  发送距离和最大值  ***********/
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 'M');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '=');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, (uint8_t)(max / 100) + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, (uint8_t)(max % 100 / 10) + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, (uint8_t)(max % 10) + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '\r');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '\n');
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 'D');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '=');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, results / 100 + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '.');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, results % 100 / 10 + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, results % 10 + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 'm');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '\r');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '\n');
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 'P');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '=');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, (uint8_t)(sumPrevage / 100) + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, (uint8_t)(sumPrevage % 100 / 10) + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, (uint8_t)(sumPrevage % 10) + 0x30);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '\r');
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, '\n');
				/****************************/
		}
	}
}

/**
  * @brief  Enable the interrupt
  * @param  None
  * @retval None
  * @name
  */
void InitNVIC(void)
{
	/* Enable and configure RCC global IRQ channel, will be used to manage HSE ready 
     and PLL ready interrupts. 
     These interrupts are enabled in stm32f0xx_it.c file **********************/
	 NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel = RCC_CRS_IRQn;    /* PLL selected as system clock */
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;    /* PLL selected as system clock */
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
  
   /* USART1 IRQ Channel configuration */
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
	/* Configure and enable ADC1 interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
	
	/* Configure NVIC for ADC */
	/* (7) Enable Interrupt on ADC */
	/* (8) Set priority for ADC */
	NVIC_EnableIRQ(ADC1_IRQn); /* (7) */
	NVIC_SetPriority(ADC1_COMP_IRQn,0); /* (8) */
}

/**
  * @brief  PLL selected as system clock(48M), HSI/2 selected as PLL input clock
  * @param  None
  * @retval None
  * @name
  */
void InitRCC(void)
{
	/* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCOSEL[2:0], MCOPRE[2:0] and PLLNODIV bits */
  RCC->CFGR &= (uint32_t)0x08FFB80C;   /* HCLK(APB clock) not divided, SYSCLK(AHB clock) not divided, HSI selected as system clock. */
  
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;    /* PLL off, Clock security system disabled (clock detector OFF), HSE oscillator OFF */

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;   /* HSE crystal oscillator not bypassed */

  /* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits,  PLL input clock*2 */
  RCC->CFGR &= (uint32_t)0xFFC0FFFF;   /* HSI/2 selected as PLL input clock,  */

  /* Reset PREDIV1[3:0] bits */
  RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;  /*  PREDIV input clock not divided */

  /* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
  RCC->CFGR3 &= (uint32_t)0xFFFFFEAD;   /*  SYSCLK selected as USART1 clock source (default), HSI clock selected as I2C1 clock source (default) */

  /* Reset HSI14 bit */
  RCC->CR2 &= (uint32_t)0xFFFFFFFE;  /* HSI14 oscillator OFF */

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;  /*  */

  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
  
	/*************************************************SetSysClock()********************************************************/
	//__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/

    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;  /* Prefetch Buffer Enable; One wait state, if 24 MHz < SYSCLK = 48 MHz */
 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;   
      
    /* PCLK = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

    /* PLL configuration = HSI / 2 * 12 = 48 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL12);
            
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
}

/**
  * @brief  Extenal interrupt configuration
  * @param  None
  * @retval None
  * @name
  */
void EXTI_Config(void)   
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  delay
  * @param  t
  * @retval None
  * @name
  */
void delay_ms(uint16_t t)
{
	uint16_t i, j;
	for(; t > 0; t--)
	{
		for(i = 30; i > 0; i--)
		{
			for(j = 500; j > 0; j--);
		}
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


 

 

 
