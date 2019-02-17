#include "adc.h"

void ADC_Config(void)
{  
  ADC_InitTypeDef    ADC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);   // ADC clock 12M
	
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;  /* Configure ADC Channel4 as analog input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  ADC_StructInit(&ADC_InitStructure);
	
  /* Configure the ADC1 in continous mode with a resolution equal to 8 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   // software triggle
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* Convert the ADC1 Channel 4 with 71.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_71_5Cycles);
 
	ADC_GetCalibrationFactor(ADC1);
	
  /* Enable End Of Conversion interupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  
	/* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
	
  /* ADC1 Start Conversion */ 
  //ADC_StartOfConversion(ADC1);
}
