/*
 * RS485.c
 *
 *  Created on: Feb 27, 2025
 *      Author: X
 */
#include <_USART.h>
#include <user.h>
#include <rs485.h>

#ifdef HAL_UART_MODULE_ENABLED
void Usart_Init(UART_HandleTypeDef *huart,uint8_t *pData,uint16_t Size)
{
	HAL_Delay(5);
	if (HAL_UART_GetState(huart)==HAL_UART_STATE_READY)
	{
#ifdef USER_RS485_UART2_DMA
		HAL_UARTEx_ReceiveToIdle_DMA(huart, pData, Size);
#endif
	}
}

void Usart_Send_IT(UART_HandleTypeDef *huart,const uint8_t *pData, uint16_t Size)
{
	HAL_UART_AbortReceive_IT(huart);
	HAL_Delay(5);
	HAL_UART_Transmit_IT(huart, pData, Size);
}

void Usart_Send_DMA(UART_HandleTypeDef *huart,const uint8_t *pData, uint16_t Size)
{
	HAL_UART_AbortReceive_IT(huart);
	HAL_Delay(5);
	HAL_UART_Transmit_DMA(huart, pData, Size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
#ifdef USER_RS485_UART2_ENABLE
		RS485_TxCallback(huart);
#endif
	}else if(huart->Instance == USART3)
	{
//		WiFi_TxCallback(huart);
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART2 && huart->RxEventType==HAL_UART_RXEVENT_IDLE)             /* 接收完成 */
	{
#ifdef USER_RS485_UART2_ENABLE
		RS485_RxCallback(huart, Size);
#endif
	}else if(huart->Instance == USART3 && huart->RxEventType==HAL_UART_RXEVENT_IDLE)             /* 接收未完 */
	{
//		WiFi_RxCallback(huart, Size);
	}
}
#endif

