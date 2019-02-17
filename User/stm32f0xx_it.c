/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_exti.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  EXTI_Line5
  * @param  None
  * @retval None
  */
int k = 0;
void delay_5ms(void)
{
	int i = 12000;
	for(; i > 0; i--);
}
void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		delay_5ms();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)           
		{  
				// LED toggle
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
			{
				GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
				//GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
			}
			else
			{
				GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
				//GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
			}
			//GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
		}	
		
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
//  while (1)
//  {
//  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles ADC1 global interrupts requests.
  * @param  None
  * @retval None
  */
uint8_t result[DATA_BUFF];
uint16_t indexADC = 0;
uint8_t flag = 0;
void ADC1_IRQHandler(void) //_COMP
{
  //if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
  //{
		result[indexADC++] = ADC1->DR;
		//result[indexADC-1] = indexADC % 255;
		if(indexADC == DATA_BUFF)
		{
			ADC_StopOfConversion(ADC1);
			indexADC = 0;
			flag = 1;
		}
    /********************************************************************
    result[indexADC++] = ADC1->DR; //ADC_GetConversionValue(ADC1);
		if(indexADC == DATA_BUFF)
		{
			indexADC = 0;
			//ADC_StopOfConversion(ADC1);
			ADC_Cmd(ADC1, DISABLE);
			flag = 1;
		}
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		*************************************************************************/
  //}
}

void CEC_IRQHandler(void)
{
	return;
}

/**
  * @brief  This function handles EXTI0_IRQHandler Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
}

/**
  * @brief  This function handles High speed ext Handler.
  * @param  None
  * @retval None
  */
void RCC_CRS_IRQHandler(void)
{
	if((RCC->CIR & RCC_CIR_HSERDYF) != 0)
	{
		RCC->CIR |= RCC_CIR_HSERDYC;   /*!< HSE Ready Interrupt Clear */
		
		RCC->CFGR = ((RCC->CFGR & (~RCC_CFGR_SW)) | RCC_CFGR_SW_PLL);   // PLL selected as system clock
	}
}

/**
  * @brief  This function handles TIM3_IRQHandler Handler.
  * @param  None
  * @retval None
  */

void TIM3_IRQHandler(void)
{
}

unsigned long i = 0;
void TIM14_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  
	i++;
	if( i == 36326)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
		TIM_Cmd(TIM3, ENABLE);
		// 开第二个定时器
	}
	else if(i == 36335)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != Bit_RESET);
		while( (GPIOA->IDR & GPIO_Pin_6) != (uint32_t)Bit_RESET );
		//TIM_Cmd(TIM3, DISABLE);
		TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
		//  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);        // A6 is in AF mode,so can't be changed by write.
		ADC_StartOfConversion(ADC1);  // start ADC conversion
		i = 0;
	}	
}

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	/******************************** Receive the sended data **************************************/
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		// waiting for send finished.
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, USART_ReceiveData(USART1));
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
