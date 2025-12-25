/*
 * RS485.c
 *
 *  Created on: Feb 27, 2025
 *      Author: X
 */
#include "_USART.h"
#include "rs485.h"

uint8_t RS485_Rx_buffer[256];
uint8_t RS485_RecvSize = 0;

#ifdef USER_RS485_UART1_ENABLE
UART_HandleTypeDef *_RS485_huart;

void RS485_Init(UART_HandleTypeDef *huart) {
  _RS485_huart = huart;
#if USER_RS485_MAX485 == 1
  RS485_RE(0);
  HAL_Delay(5);
#endif
  Usart_Init(huart, RS485_Rx_buffer, 256);
}

void RS485_Send(const uint8_t *pData, uint16_t Size) {
#if USER_RS485_MAX485 == 1
  RS485_RE(1);
  HAL_Delay(5);
#endif
#ifdef USER_RS485_UART1_DMA
  Usart_Send_DMA(_RS485_huart, pData, Size);
#endif
}

void RS485_TxCallback(UART_HandleTypeDef *huart) {
#if USER_RS485_MAX485 == 1
  RS485_RE(0);
#endif
#ifdef USER_RS485_UART1_DMA
  HAL_UARTEx_ReceiveToIdle_DMA(huart, RS485_Rx_buffer, 256);
#endif
}

void RS485_RxCallback(UART_HandleTypeDef *huart, uint16_t RecvSize) {
#ifdef USER_RS485_UART1_DMA
  HAL_UARTEx_ReceiveToIdle_DMA(huart, RS485_Rx_buffer, 256);
  RS485_RecvSize = RecvSize;
#endif
}
#endif
