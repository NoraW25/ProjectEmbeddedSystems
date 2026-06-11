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
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STEPS_PER_REV 20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
CAN_RxHeaderTypeDef rxHeader;
uint8_t rxData[8];
volatile int datacheck = 0;
int lastReportedSteps = 0;
int revolutions = 0;

typedef struct {
	uint16_t pin;
	GPIO_PinState state;
	char label[5];
} Button;

typedef struct {
	Button* buttonIncrease;
	Button* buttonDecrease;
	int state;
} Encoder;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
int8_t readEncoder(void);
void sendCan(int value);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void sendCan(int value) {
    // Wacht tot er een vrije mailbox is
    uint32_t timeout = HAL_GetTick() + 10;  // max 10ms wachten
    while (!HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) && HAL_GetTick() < timeout);

    CAN_TxHeaderTypeDef header;
    uint32_t mailbox;
    uint8_t data[2];

    header.StdId = 0x1FE;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;
    header.DLC = 2;

    data[0] = (value >> 8) & 0xFF;
    data[1] = value & 0xFF;

    if (HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox) != HAL_OK) {
        char err[] = "Mailbox vol!\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)err, strlen(err), HAL_MAX_DELAY);
    }
}
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  CAN_FilterTypeDef filter;
  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterIdHigh = 0x0000;  // Accepteer alle IDs (of specifieke IDs als je dat wilt)
  filter.FilterIdLow = 0x0000;
  filter.FilterMaskIdHigh = 0x0000;
  filter.FilterMaskIdLow = 0x0000;
  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  filter.FilterActivation = ENABLE;

  HAL_CAN_ConfigFilter(&hcan1, &filter);
  HAL_CAN_Start(&hcan1);
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
	  Error_Handler();
  }


  Button buttons[3] = {};
  buttons[0].pin = encoder_Knop_Pin;
  buttons[0].state = HAL_GPIO_ReadPin(GPIOA, encoder_Knop_Pin);
  sprintf(buttons[0].label, "enbu");

  buttons[1].pin = encoder_Decrease_Pin;
  buttons[1].state = HAL_GPIO_ReadPin(GPIOA, encoder_Decrease_Pin);
  sprintf(buttons[1].label, "ende");

  buttons[2].pin = encoder_Increase_Pin;
  buttons[2].state = HAL_GPIO_ReadPin(GPIOA, encoder_Increase_Pin);
  sprintf(buttons[2].label, "enin");

  Encoder encoder = {&buttons[2], &buttons[1], 0};

  char initString[] = "System Started\n\r";
  HAL_UART_Transmit(&huart2, (uint8_t*)initString, strlen(initString), HAL_MAX_DELAY);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Encoder uitlezen
	      static uint8_t lastState = 0;
	      uint8_t a = HAL_GPIO_ReadPin(GPIOA, encoder_Increase_Pin);
	      uint8_t b = HAL_GPIO_ReadPin(GPIOA, encoder_Decrease_Pin);
	      uint8_t currentState = (a << 1) | b;

	      int8_t delta = 0;
	      if ((lastState == 0b00 && currentState == 0b01) ||
	          (lastState == 0b01 && currentState == 0b11) ||
	          (lastState == 0b11 && currentState == 0b10) ||
	          (lastState == 0b10 && currentState == 0b00))
	      {
	          delta = +1;
	      }
	      else if ((lastState == 0b00 && currentState == 0b10) ||
	               (lastState == 0b10 && currentState == 0b11) ||
	               (lastState == 0b11 && currentState == 0b01) ||
	               (lastState == 0b01 && currentState == 0b00))
	      {
	          delta = -1;
	      }
	      lastState = currentState;

	      // Als encoder is gedraaid
	      if (delta != 0)
	      {
	          lastReportedSteps += delta;

	          // UART debug: elke stap
	          char buf[50];
	          sprintf(buf, "Delta: %+d | Totaal: %d | Rev: %d\r\n", delta, lastReportedSteps, revolutions);
	          HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

	          // CAN verzenden elke 10 stappen vooruit
	          if (lastReportedSteps >= STEPS_PER_REV)
	          {
	              revolutions++;
	              lastReportedSteps -= STEPS_PER_REV;
	              sendCan(revolutions);

	              char canBuf[40];
	              sprintf(canBuf, "CAN verstuurd: rev = %d\r\n", revolutions);
	              HAL_UART_Transmit(&huart2, (uint8_t*)canBuf, strlen(canBuf), HAL_MAX_DELAY);
	          }
	          // CAN verzenden elke 10 stappen achteruit
	          else if (lastReportedSteps <= -STEPS_PER_REV)
	          {
	              revolutions--;
	              lastReportedSteps += STEPS_PER_REV;
	              sendCan(revolutions);

	              char canBuf[40];
	              sprintf(canBuf, "CAN verstuurd: rev = %d\r\n", revolutions);
	              HAL_UART_Transmit(&huart2, (uint8_t*)canBuf, strlen(canBuf), HAL_MAX_DELAY);
	          }
	      }
	      // Knop uitlezen
	      static GPIO_PinState lastKnopState = GPIO_PIN_SET;
	      GPIO_PinState knopState = HAL_GPIO_ReadPin(GPIOA, encoder_Knop_Pin);

	      if (knopState == GPIO_PIN_RESET && lastKnopState == GPIO_PIN_SET)
	      {
	          HAL_Delay(50); // debounce

	          // Abort oude berichten
	          HAL_CAN_AbortTxRequest(&hcan1, CAN_TX_MAILBOX0);
	          HAL_CAN_AbortTxRequest(&hcan1, CAN_TX_MAILBOX1);
	          HAL_CAN_AbortTxRequest(&hcan1, CAN_TX_MAILBOX2);

	          CAN_TxHeaderTypeDef header;
	          uint32_t mailbox;
	          uint8_t data[1];

	          header.StdId = 0x1FF;
	          header.IDE = CAN_ID_STD;
	          header.RTR = CAN_RTR_DATA;
	          header.DLC = 1;
	          data[0] = 0x01;

	          HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox);

	          char buf[50];
	          sprintf(buf, "Knop CAN: %s\r\n", status == HAL_OK ? "OK" : "FOUT");
	          HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
	      }
	      lastKnopState = knopState;

	      HAL_Delay(5);

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
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
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 4;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : encoder_Knop_Pin encoder_Decrease_Pin encoder_Increase_Pin */
  GPIO_InitStruct.Pin = encoder_Knop_Pin|encoder_Decrease_Pin|encoder_Increase_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/* CAN Receive Callback */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK) {
        if (rxHeader.StdId == 0x1FE) {  // Controleer op ID 0x310
            datacheck = 1;
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
  while (1)
  {
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
