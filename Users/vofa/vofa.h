#ifndef __VOFA_H
#define __VOFA_H

#include "main.h"
#include "usart.h"
#include "math.h"
#include "delay.h"
#include "FOC_Control.h"


void JustFloat_Send(float *arr,uint8_t ChannelNum);
void VOFA_USART_PI_Adjust(uint8_t *DataBuff ,uint8_t on);								//vofa+串口调试																					

#endif


