#include "uart.h"

/**
  * @brief  Initialize all GPIO we need.
  * @param  None
  * @retval None
  */
void InitUart1(void)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  /* Enable 8xUSARTs Receive interrupts */
  USART_Cmd(USART1, ENABLE);  /* Enable the 8xUSARTs */
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

/**
  * @brief  UART send byte
  * @param  None
  * @retval None
  */
 void UART_send_byte(uint8_t byte)
 {
		while(!((USART1->ISR)&(1<<7)))   // waiting for send finished
		{}
    USART1->TDR = byte;   // send a byte
 }
 
 /**
  * @brief  UART send byte
  * @param  None
  * @retval None
  */
void UART_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART1->ISR)&(1<<7)));
		USART1->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}

uint8_t UART_Recive(void)
{
	while(!(USART1->ISR & (1<<5)));  // waiting reserve data
	return(USART1->RDR); 
}
