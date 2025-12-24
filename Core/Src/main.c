/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FOC_Control.h"
#include "arm_math.h"
#include "delay.h"
#include "rs485.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_gpio.h"
#include "sys.h"
#include "vofa.h"
#include "stdio.h"
#include "bldc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
vu8 key_value_last;
vu8 key_value_now;

bool K0_flag = 0;
bool K1_flag = 0;
bool k2_flag = 0;
bool k3_flag = 0;
bool k4_flag = 0;
bool SMO_flag;
bool SMO_Switch_flag;
bool HFI_Init_Angle_flag = 0;
bool Init_Over = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void check_key(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
extern ADC_Sample ADC_Sample_Filt_Para;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;
extern ADC_HandleTypeDef hadc1;
extern BLDC_Control bldcControl;
extern BLDC_Status bldcStatus;
extern void (*motorDrv[6])(void);
int32_t tempARR_Tim5=0;
uint8_t tempARR_Flag=0;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	delay_init(168);
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_SPI3_Init();
	MX_TIM1_Init();
	MX_TIM5_Init();
	MX_TIM6_Init();
	/* USER CODE BEGIN 2 */

	RS485_Init(&huart1);

	//	Motor_Init();
	//Commit测试
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_ADCEx_InjectedStart_IT(&hadc1);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	//	SMO_Switch_flag = 0;
	//	Init_Over = 1;
	//	SMO_flag = 1;



	while (1) {

		if (Motor.M_State == IF_Control) {
			pi_spd.Kp = 0.007f;
			pi_spd.Ki = 0.002f; // 0.0001*10 / 0.2   T*SpeedLoopPrescaler/0.2
		}

		HAL_Delay(50);

		// RS485_Send((uint8_t *)("Hello World!!\n"), 14);

		check_key();

		char str[60]={0},str1[10]={0},str2[10]={0},str3[10]={0},str4[10],str5[10]={0};
		sprintf(str1,"%5.2f",ADC_Sample_Filt_Para.VBUS);
		sprintf(str2,"%6.2f",ADC_Sample_Filt_Para.PhaseU_Curr);
		sprintf(str3,"%6.2f",ADC_Sample_Filt_Para.PhaseV_Curr);
		sprintf(str4,"%5ld",2000000/(htim6.Instance->ARR+1));

		strcat(str,"Vbus:");
		strcat(str,str1);
		strcat(str,",U:");
		strcat(str,str2);
		strcat(str,",V:");
		strcat(str,str3);
		strcat(str,",f:");
		strcat(str,str4);
		strcat(str,"Hz\n");

		RS485_Send((uint8_t *)(&str), sizeof(str));

		/****************************
		 * 9V母线电压下，启动频率1500Hz，增大频率至2300Hz时达到运行峰值
		 *启动成功后，增大电压至12V，最大频率可至3300Hz
		 *
		 */

		//		char str[50]={0};
		//		sprintf(str,"%d",bldcStatus.step);
		//		strcat(str,"\n");
		//		RS485_Send((uint8_t *)(str), 2);


		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void check_key(void) {
	key_value_last = key_value_now;
	key_value_now = KEY_Scan(0);
	if ((key_value_now == 0xff) && (key_value_last != 0xff)) {
		if (key_value_last == 1) {				//key0
			K0_flag = !K0_flag;
			if (K0_flag) {
				BLDC_Run();
				LED1 = 0;
			} else {
				BLDC_Stop();
				LED1 = 1;
			}
		} else if (key_value_last == 2) {		//key1
			K1_flag = !K1_flag;
			bldcControl.duty-=0.05f;
		} else if (key_value_last == 3) {		//key2
			k2_flag = !k2_flag;
			LED2 = !LED2;
			bldcControl.duty+=0.05f;
		} else if (key_value_last == 4) {		//key3
			k3_flag = !k3_flag;
			tempARR_Tim5=-50;
			tempARR_Flag=1;
		} else if (key_value_last == 5) {		//key4
			k4_flag = !k4_flag;
			tempARR_Tim5=50;
			tempARR_Flag=1;
		}
	}
}


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line
number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	 */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
