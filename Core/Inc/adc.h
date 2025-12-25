/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "delay.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define LPF_I_STOP_A		0.00314159265f
#define LPF_I_STOP_B		0.99685840735f

#define LPF_I_RUN_A			0.71539f  				//一阶低通滤波
#define LPF_I_RUN_B			0.28461f

typedef struct {
	float VBUS;		//母线电压
	float	Temperature;		//温度
	float PhaseU_Curr;		//V相电流
	float PhaseV_Curr;		//W相电流
	s16		Offset_PhaseV_Curr;		//V�����ƫ��ֵ
	s16 	Offset_PhaseW_Curr;		//W�����ƫ��ֵ
} ADC_Sample;


#define ADC_Sample_DEFAULTS		{0,0,0,0,0,0}			//ADC采样清零
#define ADC_Sample_Filt_DEFAULTS		{0.0f,0.0f,0.0f,0.0f,0,0}			//ADC采样清零

extern uint16_t	ADC_Value[4];	//ADC采样原始值

extern ADC_Sample	ADC_Sample_Para;
extern ADC_Sample	ADC_Sample_Filt_Para;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

void ADC_Sample_Offset(void);
void Get_ADC(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

