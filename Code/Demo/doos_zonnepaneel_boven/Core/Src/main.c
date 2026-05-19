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
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MCP23017_ADDR  (0x21 << 1)  // HAL verwacht 8-bit adres

#define IODIRA   0x00
#define IODIRB   0x01
#define GPIOA_REG 0x12
#define GPIOB_REG 0x13

#define CAN_ID_TEMPERATURE   0xD2
#define CAN_ID_CO2           0xDC
#define CAN_ID_HUMIDITY    0xE6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;//struct met alle instellingen voor de CANBUS

I2C_HandleTypeDef hi2c1;//struct met alle instellingen voor de i2c

UART_HandleTypeDef huart2; //struct voor alle instellingen voor de huart.

/* USER CODE BEGIN PV */
CAN_RxHeaderTypeDef rxHeader;//struct met alle instellingen voor de canbus rx header
uint8_t rxData[8];//Binnenkomende CANBUS data.
volatile int datacheck = 0;//om te zien of er data binnen is gekomen
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void MCP23017_Init(void);
void readSensorData();
void LEDBar_Set();//welke parameter?
void LEDBar_CO2(int co2);
void LEDBar_Temp(double temp);
void LEDBar_CO2(int co2);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C1_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  MCP23017_Init();

  CAN_FilterTypeDef filter;

  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterIdHigh = 0;
  filter.FilterIdLow = 0;
  filter.FilterMaskIdHigh = 0;//accepteert momenteel alle CANBUS berichten.
  filter.FilterMaskIdLow = 0;
  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  filter.FilterActivation = ENABLE;

  HAL_CAN_ConfigFilter(&hcan1, &filter);
  HAL_CAN_Start(&hcan1); //start de CANBUS
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
	  Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  LEDBar_AllOn();//Voor Testen


	  //VOOR HET TESTEN.
//	  uint8_t invoer[5] = {0};
//	    HAL_UART_Transmit(&huart2, (uint8_t*)"Geef CO2 waarde (400-2000): \r\n", 30, 100);
//	    HAL_UART_Receive(&huart2, invoer, 4, HAL_MAX_DELAY);  // wacht op 4 karakters
//
//	    int co2 = atoi((char*)invoer);  // "1500" → 1500
//
//	    char terugkoppeling[40];
//	    sprintf(terugkoppeling, "CO2: %d ppm\r\n", co2);
//	    HAL_UART_Transmit(&huart2, (uint8_t*)terugkoppeling, strlen(terugkoppeling), 100);
//	    LEDBar_CO2(co2);


	  if (datacheck){
		  datacheck = 0;
		  if (rxHeader.StdId == CAN_ID_TEMPERATURE) {//Temperatuur
			  //Check nog met Vere hoe je je temperatuur ontvangt.
			  LEDBar_Temp(temp);
		  }
		  else if(rxHeader.StdId == CAN_ID_CO2){//CO2
			  int co2 = (rxData[0] << 8) | rxData[1];//Co2 data past niet in 1 byte.
			  LEDBar_CO2(co2);
		  }
		  else if(rxHeader.StdId == CAN_ID_HUMIDITY){//Humidity
			  int humidity = rxData[0];
			  LEDBar_Humidity(humidity);
		  }
	  }
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
  hcan1.Init.AutoBusOff = ENABLE;
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00B07CB4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK) {
		Error_Handler();
		//leest ontvangen data uit.
	}
	if ((rxHeader.StdId == CAN_ID_TEMPERATURE && rxHeader.RTR == 0)||(rxHeader.StdId == CAN_ID_CO2 && rxHeader.RTR == 0)
			||(rxHeader.StdId == CAN_ID_HUMIDITY && rxHeader.RTR == 0)) {
		//checkt de message ID.
		datacheck = 1;
	}
}



void MCP23017_Init(void) {
    uint8_t data[2];

    // GPIOA: alle 8 pinnen als OUTPUT (PA0-PA7 = LED)
    data[0] = IODIRA;
    data[1] = 0x00;  // 0x00 = alles output
    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, data, 2, HAL_MAX_DELAY);

    // GPIOB: PB0 en PB1 als OUTPUT (2 LEDs)
    data[0] = IODIRB;
    data[1] = 0x00;  // 0x00 = alles output (PB2-PB7 ongebruikt, maakt niet uit)
    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, data, 2, HAL_MAX_DELAY);
}




void LEDBar_CO2(int co2){
	uint8_t co2Data [2];

	if (co2<=400)
	{
	    co2Data[1]=0x00;
	    co2Data[0]=GPIOA_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);

	    co2Data[1]=0x00;
	    co2Data[0]=GPIOB_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);
	}
	else if(co2>400&&co2<=1760)
	{
		co2Data[1]=0xFF;
		co2Data[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);

	    co2Data[1]=0x00;
	    co2Data[0]=GPIOB_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);
	}
	else if(co2>1760&&co2<=1840)
	{
		co2Data[1]=0x00;
		co2Data[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);

		co2Data[1]=0x01;
		co2Data[0]=GPIOB_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);
	}
	else{

		co2Data[1]=0x00;
		co2Data[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);

		co2Data[1]=0x02;
		co2Data[0]=GPIOB_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, co2Data, 2, HAL_MAX_DELAY);
	}
}

void LEDBar_Temp(double temp){
	uint8_t tempData [2];

	if (temp<=16.00)
	{
		tempData[1]=0x01;
		tempData[0]=GPIOA_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);

	    tempData[1]=0x00;
	    tempData[0]=GPIOB_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);
	}
	else if(temp>16.00&&temp<=21.00)
	{
		tempData[1]=0xFF;
		tempData[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);

		tempData[1]=0x00;
		tempData[0]=GPIOB_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);
	}
	else if(temp>21.00&&temp<=23.00)
	{
		tempData[1]=0x00;
		tempData[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);

		tempData[1]=0x01;
		tempData[0]=GPIOB_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);
	}
	else{

		tempData[1]=0x00;
		tempData[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);

		tempData[1]=0x02;
		tempData[0]=GPIOB_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, tempData, 2, HAL_MAX_DELAY);
	}
}

void LEDBar_Humidity(int humidity){
	uint8_t humidityData [2];

	if (humidity<=20)
	{
		humidityData[1]=0x00;
		humidityData[0]=GPIOA_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);

	    humidityData[1]=0x00;
	    humidityData[0]=GPIOB_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);
	}
	else if(humidity>20&&humidity<=60)
	{
		humidityData[1]=0xFF;
		humidityData[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);

		humidityData[1]=0x00;
		humidityData[0]=GPIOB_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);
	}
	else if(humidity>60&&humidity<=79)
	{
		humidityData[1]=0x00;
		humidityData[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);

		humidityData[1]=0x01;
		humidityData[0]=GPIOB_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);
	}
	else{

		humidityData[1]=0x00;
		humidityData[0]=GPIOA_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);

		humidityData[1]=0x02;
		humidityData[0]=GPIOB_REG;
		HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, humidityData, 2, HAL_MAX_DELAY);
	}
}



//void LEDBar_AllOn(void) {
//    uint8_t data[2];
//
//    // Zet PA0-PA7 aan (8 LEDs)
//    data[0] = GPIOA_REG;
//    data[1] = 0xFF;  // alle 8 bits HIGH
//    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, data, 2, HAL_MAX_DELAY);
//
//    // Zet PB0 en PB1 aan (2 LEDs)
//    data[0] = GPIOB_REG;
//    data[1] = 0x03;  // bit0 en bit1 HIGH = 0b00000011
//    HAL_I2C_Master_Transmit(&hi2c1, MCP23017_ADDR, data, 2, HAL_MAX_DELAY);
//}


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
