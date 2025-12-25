/*
 * User.h
 *
 *  Created on: Nov 13, 2025
 *      Author: X
 */

#ifndef INCLUDE_USER_H_
#define INCLUDE_USER_H_

#include "main.h"

#ifdef HAL_TIM_MODULE_ENABLED
#define USER_TIM_ENABLE
#endif

#define USER_RS485_CH340N       1
#define USER_RS485_MAX485       0

#define HAL_UART_MODULE_ENABLED	//使用HAL硬件UART，会引入相关.h头文件

#ifdef HAL_UART_MODULE_ENABLED
#define USER_RS485_UART1_ENABLE 0
#define USER_RS485_UART1_DMA
#endif

#if USER_RS485_MAX485==1
#define _RS485_RE_Pin		    RS485_RE_Pin
#define _RS485_RE_GPIO_Port	    RS485_RE_GPIO_Port
#endif


#endif /* INCLUDE_USER_H_ */
