/*
 * RS485.h
 *
 *  Created on: Feb 27, 2025
 *      Author: X
 */

#ifndef __RS485_H_
#define __RS485_H_

#include <user.h>



#ifdef USER_RS485_UART1_ENABLE
#if USER_RS485_MAX485==1
#define RS485_RE(x)  HAL_GPIO_WritePin(_RS485_RE_GPIO_Port, _RS485_RE_Pin,x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#elif USER_RS485_CH340N==1
#define RS485_RE(x)  ;
#endif

void RS485_Send(const uint8_t *pData, uint16_t Size);

void RS485_Init(UART_HandleTypeDef *huart);
void RS485_TxCallback(UART_HandleTypeDef *huart);
void RS485_RxCallback(UART_HandleTypeDef *huart,uint16_t RecvSize);
#endif

#endif /* SRC_HARDWARE_RS485_RS485_H_ */
