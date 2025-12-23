/*
 * UART.h
 *
 *  Created on: Mar 4, 2025
 *      Author: X
 */

#ifndef SRC_BUS_USART_USART_H_
#define SRC_BUS_USART_USART_H_


#include <user.h>

#ifdef HAL_UART_MODULE_ENABLED
void Usart_Init(UART_HandleTypeDef *huart,uint8_t *pData,uint16_t Size);
void Usart_Send_IT(UART_HandleTypeDef *huart,const uint8_t *pData, uint16_t Size);
void Usart_Send_DMA(UART_HandleTypeDef *huart,const uint8_t *pData, uint16_t Size);
#endif

#endif /* SRC_HARDWARE_UART_UART_H_ */
