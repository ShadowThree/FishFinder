#ifndef _UART_H_
#define _UART_H_

#include "stm32f0xx.h"

void InitUart1(void);
void UART_send_byte(uint8_t byte);
void UART_Send(uint8_t *Buffer, uint32_t Length);
uint8_t UART_Recive(void);

#endif
