/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern bool SMO_flag;
extern bool SMO_Switch_flag;
extern bool HFI_Init_Angle_flag;
extern bool Init_Over;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PIN_VBUS_Pin GPIO_PIN_4
#define PIN_VBUS_GPIO_Port GPIOC
#define PIN_IU_Pin GPIO_PIN_0
#define PIN_IU_GPIO_Port GPIOB
#define PIN_IV_Pin GPIO_PIN_1
#define PIN_IV_GPIO_Port GPIOB
#define PIN_UL_Pin GPIO_PIN_13
#define PIN_UL_GPIO_Port GPIOB
#define PIN_VL_Pin GPIO_PIN_14
#define PIN_VL_GPIO_Port GPIOB
#define PIN_WL_Pin GPIO_PIN_15
#define PIN_WL_GPIO_Port GPIOB
#define PIN_UH_Pin GPIO_PIN_8
#define PIN_UH_GPIO_Port GPIOA
#define PIN_VH_Pin GPIO_PIN_9
#define PIN_VH_GPIO_Port GPIOA
#define PIN_WH_Pin GPIO_PIN_10
#define PIN_WH_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_6
#define RS485_TX_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_7
#define RS485_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
