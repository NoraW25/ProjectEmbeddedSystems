/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_CAN1_Init();
	MX_TIM1_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, matrix_B1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, matrix_G1_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOA, matrix_R2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, matrix_B2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, matrix_G2_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOB, matrix_rA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, matrix_rB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, matrix_rC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, matrix_rD_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_RESET);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	HAL_Delay(1000);

	int row = 0;

	/*HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_SET);
	 HAL_Delay(1);
	 HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_SET);
	 HAL_Delay(1);
	 HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_RESET);
	 HAL_Delay(1);
	 HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_SET);
	 HAL_Delay(1);
	 HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_RESET);*/
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250);
		if (row >= 15) {
			row = 0;
		} else {
			row += 1;
		}
		if (row & 1) {
			HAL_GPIO_WritePin(GPIOB, matrix_rA_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, matrix_rA_Pin, GPIO_PIN_RESET);
		}
		if (row & 0b10) {
			HAL_GPIO_WritePin(GPIOB, matrix_rB_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, matrix_rB_Pin, GPIO_PIN_RESET);
		}
		if (row & 0b100) {
			HAL_GPIO_WritePin(GPIOB, matrix_rC_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, matrix_rC_Pin, GPIO_PIN_RESET);
		}
		if (row & 0b1000) {
			HAL_GPIO_WritePin(GPIOB, matrix_rD_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, matrix_rD_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_RESET);

		for (int c = 0; c < 64; c++) {
			if (c&1) {
				HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_SET);
			}
			else {
				HAL_GPIO_WritePin(GPIOA, matrix_B1_Pin, GPIO_PIN_SET);
			}
			HAL_GPIO_WritePin(GPIOA, matrix_G2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, matrix_R2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, matrix_B1_Pin, GPIO_PIN_RESET);
		}
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 16;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}

	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief CAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN1_Init(void) {

	/* USER CODE BEGIN CAN1_Init 0 */

	/* USER CODE END CAN1_Init 0 */

	/* USER CODE BEGIN CAN1_Init 1 */

	/* USER CODE END CAN1_Init 1 */
	hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 16;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN CAN1_Init 2 */

	/* USER CODE END CAN1_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 1000;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 500;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			matrix_B2_Pin | matrix_G2_Pin | matrix_R2_Pin | matrix_B1_Pin
					| matrix_G1_Pin | matrix_R1_Pin | matrix_CLK_Pin
					| matrix_LAT_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			matrix_rA_Pin | matrix_rD_Pin | LD3_Pin | matrix_rC_Pin
					| matrix_rB_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : matrix_B2_Pin matrix_G2_Pin matrix_R2_Pin matrix_B1_Pin
	 matrix_G1_Pin matrix_R1_Pin matrix_CLK_Pin matrix_LAT_Pin */
	GPIO_InitStruct.Pin = matrix_B2_Pin | matrix_G2_Pin | matrix_R2_Pin
			| matrix_B1_Pin | matrix_G1_Pin | matrix_R1_Pin | matrix_CLK_Pin
			| matrix_LAT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : matrix_rA_Pin matrix_rD_Pin LD3_Pin matrix_rC_Pin
	 matrix_rB_Pin */
	GPIO_InitStruct.Pin = matrix_rA_Pin | matrix_rD_Pin | LD3_Pin
			| matrix_rC_Pin | matrix_rB_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
