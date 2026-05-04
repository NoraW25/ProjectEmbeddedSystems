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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHT3X_ADDR 0x44 << 1
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void I2C_Scan(void) {
    for (uint8_t addr = 1; addr < 127; addr++) { //Loopt alle geldige I2C adressen door (start bij addres 1, zijn 7-bit -> bereik 0x01 tot 0x7E (1-126) -> scan alle mogelijke slaves op bus
        if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 1, 10) == HAL_OK) { //pointer naar I2C handle (I2C1), linkershift van 1 bit, HAL verwacht 8-bit adres, 1 (aantal pogingen, trials, 10 (timeout in milliseconden), == HAL_OK -> functie ACK van slave
            printf("FOUND I2C DEVICE at 0x%02X\r\n", addr); //0x%02X = minstens 2 cijfers met voorloopnul, \r\n = carriage return + newline -> correcte terminalweergave
        }
    }
}
HAL_StatusTypeDef SHT3X_Read(float *temp, float *hum) // Geeft HAL status terug, float *temp -> pointer naar variabele waarin temp komt, float *hum -> pointer naar variable waarin luchtvochtigheid (meerdere waarden teruggeven)
{
	uint8_t cmd[2] = {0x24, 0x00}; //I2C commando voor SHT3x: 0x2400 = Single shot measurement, high repeatability, clock stretching disabled (opgesplitst in 2 bytes, want I2C byte-gebaseerd
	uint8_t data[6]; //buffer ontvangen data sensor, 6 bytes (temp MSB, temp LSB, CRC (temp), humidity MSB, humidity LSB, CRC(hum)

	if (HAL_I2C_Master_Transmit(&hi2c1, SHT3X_ADDR, cmd, 2, 50) != HAL_OK) //stuurt meetcommando naar sensor (&hi2c1 -> I2C peripheral, SHT3X_ADDR -> I2C adres van sensor, cmd -> commandoarray, 2 -> aantal bytes, 50 - timeout in ms)
		return HAL_ERROR;

	HAL_Delay(20);

	if (HAL_I2C_Master_Receive(&hi2c1, SHT3X_ADDR, data, 6, 50) != HAL_OK) //leest 6 bytes van sensor, zelfde I2C-adres, data opgeslagen in data[], fout -> HAL_ERROR
		return HAL_ERROR;

	uint16_t rawT = (data[0] << 8) | data[1]; //combineert 2 bytes -> 1 16-bit ruwe waarde, data[0] = MSB(bovenste 8 bits), data[1] = LSB (onderste 8 bits), data[0] = hoge byte -> 8 bits naar links -> plek maken voor data[1]
	*temp = -45.0f + 175.0f * ((float) rawT / 65535.0f); //Formule T= -45 + 175 x raw / 65535 (resultaat in graden Celsius, opgeslagen via pointer temp

	uint16_t rawRH = (data[3] << 8) | data[4]; //van twee losse bytes -> 1 16-bit ruwe humidity waarde, data[3] = hum MSB, data[4] = hum LSB, 16-bit (bereik = 0 -> 65535)

	*hum = 100.0f * ((float) rawRH / 65535.0f);

	return HAL_OK;

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  I2C_Scan();
  HAL_Delay(2000);
  float temperature = 0.0f;
  float humidity = 0.0f;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (SHT3X_Read(&temperature, &humidity) == HAL_OK)
	      {
	          printf("Temp: %.2f C, Hum: %.2f %%\r\n", temperature, humidity);
	      }
	      else
	      {
	          printf("SHT3x read error!\r\n");
	      }

	      HAL_Delay(1000);


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

/* USER CODE BEGIN 4 */

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
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
