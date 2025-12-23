#ifndef __VOFA_H
#define __VOFA_H

#include "main.h"
#include "usart.h"
#include "math.h"
#include "delay.h"
#include "FOC_Control.h"

typedef union     //浮点数据与四字节的转换 参考https://blog.csdn.net/ls667/article/details/50811519
{
	float fdata;
	unsigned long ldata;
}FloatLongType;

typedef union 		//浮点数与四字节的转换
{	
	float f_data;
	u8 		c_data[4];
}FloatToChar;

extern FloatToChar fc;

void JustFloat_Send(float *fdata, u16 fdata_num, USART_TypeDef *Usart_choose);
void VOFA_USART_PI_Adjust(uint8_t *DataBuff ,uint8_t on);								//vofa+串口调试																					

#endif


