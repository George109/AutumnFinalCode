/*
 * uart.h
 *
 *  Created on: 2018Äê11ÔÂ6ÈÕ
 *      Author: 90303
 */

#ifndef UART_H_
#define UART_H_

#include "stm32l4xx.h"
#include "stm32l476g_discovery.h"

UART_HandleTypeDef huart2;
void MX_USART2_UART_Init(void);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);

#endif /* UART_H_ */
