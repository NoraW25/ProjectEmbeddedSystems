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
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
uint8_t state = 0;
CAN_RxHeaderTypeDef rxHeader;
uint8_t rxData[8];
volatile int datacheck = 0;

volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;

volatile int clockOn = 0;
volatile uint8_t dotsDisplayed = 0;
volatile uint8_t dotsFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

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
	uint8_t boardDisplay[32][64][3]; //[Row][Column][Color-RGB]

	void drawImage_dot(int sc, int sr) {
		static const uint8_t dotData[8][8] = {
			0,0,1,1,1,1,0,0,
			0,1,1,1,1,1,1,0,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			0,1,1,1,1,1,1,0,
			0,0,1,1,1,1,0,0
		};
		for (int r=0; r < 8; r++) {
			for (int c=0; c < 8; c++) {
				boardDisplay[r+sr][c+sc][0] =dotData[c][r];
			}
		}
	}

	void drawVoid(int sc, int sr, int xSize, int ySize) {
		for (int r=0; r < ySize; r++) {
					for (int c=0; c < xSize; c++) {
						boardDisplay[r+sr][c+sc][0] =0;
					}
				}
	}

	void drawNumber_zero(int sc, int sr) {
		static const uint8_t zeroData[24][12] = {
				0,0,0,0,1,1,1,1,0,0,0,0,
				0,0,0,1,1,1,1,1,1,0,0,0,
				0,0,1,1,1,1,1,1,1,1,0,0,
				0,0,1,1,1,0,0,1,1,1,0,0,
				0,1,1,1,0,0,0,0,1,1,1,0,
				0,1,1,1,0,0,0,0,1,1,1,0,
				0,1,1,1,0,0,0,0,1,1,1,0,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				1,1,1,0,0,0,0,0,0,1,1,1,
				0,1,1,1,0,0,0,0,1,1,1,0,
				0,1,1,1,0,0,0,0,1,1,1,0,
				0,1,1,1,0,0,0,0,1,1,1,0,
				0,0,1,1,1,0,0,1,1,1,0,0,
				0,0,1,1,1,1,1,1,1,1,0,0,
				0,0,0,1,1,1,1,1,1,0,0,0,
				0,0,0,0,1,1,1,1,0,0,0,0
		};
		for (int r=0; r < 24; r++) {
			for (int c=0; c < 12; c++) {
				boardDisplay[r+sr][c+sc][0] =zeroData[r][c];
			}
		}
	}

	void drawNumber_one(int sc, int sr) {
		static const uint8_t oneData[24][12] = {
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,1,1,1,1,1,0,0,0,0,
						0,0,1,1,1,1,1,1,0,0,0,0,
						0,1,1,1,1,1,1,1,0,0,0,0,
						1,1,1,1,1,1,1,1,0,0,0,0,
						1,1,1,1,1,1,1,1,0,0,0,0,
						1,1,1,1,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,0,0,0,1,1,1,1,0,0,0,0,
						0,1,1,1,1,1,1,1,1,1,1,0,
						1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1
				};
				for (int r=0; r < 24; r++) {
					for (int c=0; c < 12; c++) {
						boardDisplay[r+sr][c+sc][0] =oneData[r][c];
					}
				}
	}

	void drawNumber_two(int sc, int sr) {
		static const uint8_t twoData[24][12] = {
		    0,0,0,1,1,1,1,0,0,0,0,0,
		    0,0,1,1,1,1,1,1,0,0,0,0,
		    0,1,1,1,1,1,1,1,1,0,0,0,
		    1,1,1,1,1,1,1,1,1,1,0,0,
		    1,1,1,1,1,1,1,1,1,1,0,0,
		    0,0,0,0,0,0,1,1,1,1,0,0,
		    0,0,0,0,0,1,1,1,1,0,0,0,
		    0,0,0,0,1,1,1,1,0,0,0,0,
		    0,0,0,1,1,1,1,0,0,0,0,0,
		    0,0,1,1,1,1,0,0,0,0,0,0,
		    0,1,1,1,1,0,0,0,0,0,0,0,
		    1,1,1,1,0,0,0,0,0,0,0,0,
		    1,1,1,0,0,0,0,0,0,0,0,0,
		    1,1,0,0,0,0,0,0,0,0,0,0,
		    1,1,0,0,0,0,0,0,0,0,0,0,
		    1,1,0,0,0,0,0,0,0,0,0,0,
		    1,1,1,1,1,1,1,1,1,1,0,0,
		    1,1,1,1,1,1,1,1,1,1,1,0,
		    1,1,1,1,1,1,1,1,1,1,1,0,
		    1,1,1,1,1,1,1,1,1,1,1,0,
		    1,1,1,1,1,1,1,1,1,1,1,0,
		    1,1,1,1,1,1,1,1,1,1,1,0,
		    1,1,1,1,1,1,1,1,1,1,1,0,
		    0,0,0,0,0,0,0,0,0,0,0,0
		};
			for (int r=0; r < 24; r++) {
				for (int c=0; c < 12; c++) {
					boardDisplay[r+sr][c+sc][0] =twoData[r][c];
				}
			}
		}

	void drawNumber_three(int sc, int sr) {
	    static const uint8_t threeData[24][12] = {
	        0,1,1,1,1,1,1,1,1,1,0,0,
	        1,1,1,1,1,1,1,1,1,1,1,0,
	        1,1,1,1,1,1,1,1,1,1,1,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,1,1,1,1,0,0,0,
	        0,0,0,0,1,1,1,1,0,0,0,0,
	        0,0,0,1,1,1,1,0,0,0,0,0,
	        0,0,1,1,1,1,0,0,0,0,0,0,
	        0,0,1,1,1,0,0,0,0,0,0,0,
	        0,0,1,1,1,1,1,0,0,0,0,0,
	        0,0,0,0,1,1,1,1,1,0,0,0,
	        0,0,0,0,0,1,1,1,1,1,0,0,
	        0,0,0,0,0,0,0,1,1,1,1,0,
	        0,0,0,0,0,0,0,0,1,1,1,0,
	        0,0,0,0,0,0,0,1,1,1,1,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,1,1,1,1,0,0,0,
	        0,0,0,0,1,1,1,1,0,0,0,0,
	        0,0,0,1,1,1,1,0,0,0,0,0,
	        0,1,1,1,1,1,1,1,1,1,0,0,
	        1,1,1,1,1,1,1,1,1,1,1,0,
	        1,1,1,1,1,1,1,1,1,1,1,0,
	        0,1,1,1,1,1,1,1,1,1,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0
	    };

	    for (int r = 0; r < 24; r++) {
	        for (int c = 0; c < 12; c++) {
	            boardDisplay[r + sr][c + sc][0] = threeData[r][c];
	        }
	    }
	}

	void drawNumber_four(int sc, int sr) {
	    static const uint8_t fourData[24][12] = {
	        0,0,0,0,1,1,1,1,0,0,0,0,
	        0,0,0,1,1,1,1,1,0,0,0,0,
	        0,0,1,1,1,1,1,1,0,0,0,0,
	        0,1,1,1,1,1,1,1,0,0,0,0,
	        1,1,1,1,1,1,1,1,0,0,0,0,

	        1,1,1,1,0,0,1,1,1,1,0,0,
	        1,1,1,1,0,0,1,1,1,1,0,0,
	        1,1,1,1,0,0,1,1,1,1,0,0,
	        1,1,1,1,0,0,1,1,1,1,0,0,
	        1,1,1,1,0,0,1,1,1,1,0,0,

	        1,1,1,1,0,0,1,1,1,1,0,0,
	        1,1,1,1,0,0,1,1,1,1,0,0,

	        1,1,1,1,1,1,1,1,1,1,1,1,  // middle bar
	        1,1,1,1,1,1,1,1,1,1,1,1,

	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0
	    };

	    for (int r = 0; r < 24; r++) {
	        for (int c = 0; c < 12; c++) {
	            boardDisplay[r + sr][c + sc][0] = fourData[r][c];
	        }
	    }
	}

	void drawNumber_five(int sc, int sr) {
	    static const uint8_t fiveData[24][12] = {
	        // top bar
	        1,1,1,1,1,1,1,1,1,1,1,1,
	        1,1,1,1,1,1,1,1,1,1,1,1,

	        // upper left vertical
	        1,1,1,1,0,0,0,0,0,0,0,0,
	        1,1,1,1,0,0,0,0,0,0,0,0,
	        1,1,1,1,0,0,0,0,0,0,0,0,
	        1,1,1,1,0,0,0,0,0,0,0,0,

	        // middle bar
	        1,1,1,1,1,1,1,1,1,1,0,0,
	        1,1,1,1,1,1,1,1,1,1,0,0,

	        // lower right vertical
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,0,1,1,1,1,0,0,

	        // bottom bar
	        1,1,1,1,1,1,1,1,1,1,1,1,
	        1,1,1,1,1,1,1,1,1,1,1,1,

	        // padding rows (to reach 24 total)
	        0,0,0,0,0,0,0,0,0,0,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0
	    };

	    for (int r = 0; r < 24; r++) {
	        for (int c = 0; c < 12; c++) {
	            boardDisplay[r + sr][c + sc][0] = fiveData[r][c];
	        }
	    }
	}

	void drawNumber_six(int sc, int sr) {
			static const uint8_t oneData[24][12] = {
							0,0,0,0,0,0,0,0,0,1,1,1,
							0,0,0,0,0,0,0,0,1,1,1,0,
							0,0,0,0,0,0,0,1,1,1,0,0,
							0,0,0,0,0,0,1,1,1,0,0,0,
							0,0,0,0,0,1,1,1,0,0,0,0,
							0,0,0,0,1,1,1,0,0,0,0,0,
							0,0,0,1,1,1,0,0,0,0,0,0,
							0,0,0,1,1,1,0,0,0,0,0,0,
							0,0,1,1,1,0,0,0,0,0,0,0,
							0,0,1,1,1,0,0,0,0,0,0,0,
							0,1,1,1,0,0,0,0,0,0,0,0,
							0,1,1,1,1,1,1,1,1,0,0,0,
							1,1,1,1,1,1,1,1,1,1,0,0,
							1,1,1,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,0,0,1,1,1,1,1,
							1,1,1,1,0,0,0,0,1,1,1,1,
							1,1,1,0,0,0,0,0,0,1,1,1,
							1,1,1,0,0,0,0,0,0,1,1,1,
							1,1,1,0,0,0,0,0,0,1,1,1,
							1,1,1,1,0,0,0,0,1,1,1,1,
							1,1,1,1,1,0,0,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,1,1,0,0,
							0,0,0,1,1,1,1,1,1,0,0,0
					};
					for (int r=0; r < 24; r++) {
						for (int c=0; c < 12; c++) {
							boardDisplay[r+sr][c+sc][0] =oneData[r][c];
						}
					}
		}

	void drawNumber_seven(int sc, int sr) {
	    static const uint8_t sevenData[24][12] = {
	        1,1,1,1,1,1,1,1,1,1,1,1,
	        1,1,1,1,1,1,1,1,1,1,1,1,
	        1,1,1,1,1,1,1,1,1,1,1,1,
	        0,0,0,0,0,0,1,1,1,1,0,0,
	        0,0,0,0,0,1,1,1,0,0,0,0,
	        0,0,0,0,1,1,0,0,0,0,0,0,
	        0,0,0,1,1,0,0,0,0,0,0,0,
	        0,0,1,1,0,0,0,0,0,0,0,0,
	        0,1,1,0,0,0,0,0,0,0,0,0,
	        1,1,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0,
	        1,0,0,0,0,0,0,0,0,0,0,0
	    };
	    for (int r = 0; r < 24; r++) {
	        for (int c = 0; c < 12; c++) {
	            boardDisplay[r + sr][c + sc][0] = sevenData[r][c];
	        }
	    }
	}

	void drawNumber_eight(int sc, int sr) {
	    static const uint8_t eightData[24][12] = {
	        0,0,0,1,1,1,1,1,1,0,0,0,
	        0,0,1,1,1,1,1,1,1,1,0,0,
	        0,1,1,1,0,0,0,0,1,1,1,0,
	        0,1,1,0,0,0,0,0,0,1,1,0,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        0,1,1,1,0,0,0,0,1,1,1,0,
	        0,0,1,1,1,0,0,1,1,1,0,0,
	        0,0,0,1,1,1,1,1,1,0,0,0,
	        0,0,1,1,1,0,0,1,1,1,0,0,
	        0,1,1,1,0,0,0,0,1,1,1,0,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        0,1,1,1,0,0,0,0,1,1,1,0,
	        0,0,1,1,1,1,1,1,1,1,0,0,
	        0,0,0,1,1,1,1,1,1,0,0,0,
	        0,0,0,0,1,1,1,1,0,0,0,0,
	        0,0,0,0,0,1,1,0,0,0,0,0,
	        0,0,0,0,0,1,1,0,0,0,0,0,
	        0,0,0,0,0,0,0,0,0,0,0,0
	    };

	    for (int r = 0; r < 24; r++) {
	        for (int c = 0; c < 12; c++) {
	            boardDisplay[r + sr][c + sc][0] = eightData[r][c];
	        }
	    }
	}

	void drawNumber_nine(int sc, int sr) {
	    static const uint8_t nineData[24][12] = {
	        0,0,0,1,1,1,1,1,1,0,0,0,
	        0,0,1,1,1,1,1,1,1,1,0,0,
	        0,1,1,1,1,1,0,0,1,1,1,0,
	        1,1,1,1,1,0,0,0,1,1,1,1,
	        1,1,1,1,0,0,0,0,1,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,0,0,0,0,0,0,1,1,1,
	        1,1,1,1,0,0,0,0,1,1,1,1,
	        1,1,1,1,1,0,0,1,1,1,1,1,
	        0,1,1,1,1,1,1,1,1,1,1,0,
	        0,0,1,1,1,1,1,1,1,1,0,0,
	        0,0,0,0,0,0,0,1,1,1,1,1,
	        0,0,0,0,0,0,0,0,1,1,1,1,
	        0,0,0,0,0,0,0,0,0,1,1,1,
	        0,0,0,0,0,0,0,0,0,1,1,1,
	        0,0,0,0,0,0,0,0,0,1,1,1,
	        0,0,0,0,0,0,0,0,1,1,1,1,
	        0,1,1,1,1,1,0,0,1,1,1,1,
	        0,0,1,1,1,1,1,1,1,1,0,0,
	        0,0,0,1,1,1,1,1,1,0,0,0,
	        0,0,0,0,0,0,1,1,1,0,0,0,
	        0,0,0,0,0,1,1,1,0,0,0,0,
	        0,0,0,0,1,1,1,0,0,0,0,0
	    };

	    for (int r = 0; r < 24; r++) {
	        for (int c = 0; c < 12; c++) {
	            boardDisplay[r + sr][c + sc][0] = nineData[r][c];
	        }
	    }
	}

	void printDigit(uint8_t digit, int x) {
		if (digit == 0) {
		    drawNumber_zero(x, 4);
		}
		else if (digit == 1) {
		    drawNumber_one(x, 4);
		}
		else if (digit == 2) {
		    drawNumber_two(x, 4);
		}
		else if (digit == 3) {
		    drawNumber_three(x, 4);
		}
		else if (digit == 4) {
		    drawNumber_four(x, 4);
		}
		else if (digit == 5) {
		    drawNumber_five(x, 4);
		}
		else if (digit == 6) {
		    drawNumber_six(x, 4);
		}
		else if (digit == 7) {
		    drawNumber_seven(x, 4);
		}
		else if (digit == 8) {
		    drawNumber_eight(x, 4);
		}
		else if (digit == 9) {
		    drawNumber_nine(x, 4);
		}
	}

	void printTime(uint8_t hours, uint8_t minutes) {
		uint8_t digit_1 = hours/10;
		uint8_t digit_2 = hours%10;
		uint8_t digit_3 = minutes/10;
		uint8_t digit_4 = minutes%10;
		printDigit(digit_1, 0);
		printDigit(digit_2, 14);
		printDigit(digit_3, 38);
		printDigit(digit_4, 52);
	}

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
  MX_TIM6_Init();
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

	CAN_FilterTypeDef filter;

	filter.FilterBank = 0;
	filter.FilterMode = CAN_FILTERMODE_IDMASK;
	filter.FilterScale = CAN_FILTERSCALE_32BIT;
	filter.FilterIdHigh = 0;
	filter.FilterIdLow = 0;
	filter.FilterMaskIdHigh = 0;
	filter.FilterMaskIdLow = 0;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter.FilterActivation = ENABLE;

	HAL_CAN_ConfigFilter(&hcan1, &filter);
	HAL_CAN_Start(&hcan1);
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)
			!= HAL_OK) {
		Error_Handler();
	}

	HAL_Delay(1000);

	for (int r = 0; r < 32; r++) {
		for (int c = 0; c < 64; c++) {
			boardDisplay[r][c][0] = 0;
			boardDisplay[r][c][1] = 0;
			boardDisplay[r][c][2] = 0;
		}
	}
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
		if (datacheck == 1) {
			datacheck = 0;
			drawImage_dot(28, 6);
			drawImage_dot(28, 18);

			drawNumber_eight(0,4);
			drawNumber_nine(14,4);
			drawNumber_six(38,4);
			drawNumber_seven(52,4);
		}
		else if (datacheck == 2) {
			datacheck = 0;
			if (clockOn) {
				printTime(rxData[0], rxData[1]);
			}
		}
		else if (datacheck == 3) {
			datacheck = 0;
			clockOn = !clockOn;
			if (clockOn) {
				HAL_TIM_Base_Start_IT(&htim6);
			}
			else {
				HAL_TIM_Base_Stop_IT(&htim6);
			}
		}

		if (dotsFlag) {
			dotsDisplayed = !dotsDisplayed;
			if (dotsDisplayed) {
				drawImage_dot(28, 6);
				drawImage_dot(28, 18);
			}else {
				drawVoid(28,6,8,8);
				drawVoid(28, 18, 8, 8);
			}
			dotsFlag = 0;
		}

		if (row >= 15) {
			row = 0;
		} else {
			row += 1;
		}
		int dRow = row;

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);

		for (int b = 0; b < 1; b++) {
			HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1000);

			HAL_GPIO_WritePin(GPIOA, matrix_LAT_Pin, GPIO_PIN_RESET);

			int reqThres = 1 << b;
			int row2 = row + 16;
			for (int c = 0; c < 64; c++) {
				if (boardDisplay[row][c][0] >= reqThres) {
					HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_SET);
				}

				if (boardDisplay[row2][c][0] >= reqThres) {
					HAL_GPIO_WritePin(GPIOA, matrix_R2_Pin, GPIO_PIN_SET);
				}

				if (boardDisplay[row][c][1] >= reqThres) {
					HAL_GPIO_WritePin(GPIOA, matrix_G1_Pin, GPIO_PIN_SET);
				}

				if (boardDisplay[row2][c][1] >= reqThres) {
					HAL_GPIO_WritePin(GPIOA, matrix_G2_Pin, GPIO_PIN_SET);
				}
				/*if (row==0) {
					HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, matrix_R2_Pin, GPIO_PIN_SET);
				}*/

				//Toggle clock
				HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, matrix_CLK_Pin, GPIO_PIN_RESET);

				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1000);

				//Turn pins off for next cycle
				HAL_GPIO_WritePin(GPIOA, matrix_R1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, matrix_G1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, matrix_B1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, matrix_R2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, matrix_G2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, matrix_B2_Pin, GPIO_PIN_RESET);
			}
			if (dRow & 1) {
						HAL_GPIO_WritePin(GPIOB, matrix_rA_Pin, GPIO_PIN_SET);
					} else {
						HAL_GPIO_WritePin(GPIOB, matrix_rA_Pin, GPIO_PIN_RESET);
					}
					if (dRow & 0b10) {
						HAL_GPIO_WritePin(GPIOB, matrix_rB_Pin, GPIO_PIN_SET);
					} else {
						HAL_GPIO_WritePin(GPIOB, matrix_rB_Pin, GPIO_PIN_RESET);
					}
					if (dRow & 0b100) {
						HAL_GPIO_WritePin(GPIOB, matrix_rC_Pin, GPIO_PIN_SET);
					} else {
						HAL_GPIO_WritePin(GPIOB, matrix_rC_Pin, GPIO_PIN_RESET);
					}
					if (dRow & 0b1000) {
						HAL_GPIO_WritePin(GPIOB, matrix_rD_Pin, GPIO_PIN_SET);
					} else {
						HAL_GPIO_WritePin(GPIOB, matrix_rD_Pin, GPIO_PIN_RESET);
					}
			//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250);
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
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
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 7200-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 5000;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, matrix_B2_Pin|matrix_G2_Pin|matrix_R2_Pin|matrix_B1_Pin
                          |matrix_G1_Pin|matrix_R1_Pin|matrix_CLK_Pin|matrix_LAT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, matrix_rA_Pin|matrix_rD_Pin|LD3_Pin|matrix_rC_Pin
                          |matrix_rB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : matrix_B2_Pin matrix_G2_Pin matrix_R2_Pin matrix_B1_Pin
                           matrix_G1_Pin matrix_R1_Pin matrix_CLK_Pin matrix_LAT_Pin */
  GPIO_InitStruct.Pin = matrix_B2_Pin|matrix_G2_Pin|matrix_R2_Pin|matrix_B1_Pin
                          |matrix_G1_Pin|matrix_R1_Pin|matrix_CLK_Pin|matrix_LAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : matrix_rA_Pin matrix_rD_Pin LD3_Pin matrix_rC_Pin
                           matrix_rB_Pin */
  GPIO_InitStruct.Pin = matrix_rA_Pin|matrix_rD_Pin|LD3_Pin|matrix_rC_Pin
                          |matrix_rB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK) {
		Error_Handler();
	}
	if ((rxHeader.StdId == 610 && rxHeader.RTR == 0)) {
		datacheck = 1;
	}
	else if (rxHeader.StdId == 620 && rxHeader.RTR == 0) {
		datacheck = 3;
	}
	else if (rxHeader.StdId == 410 && rxHeader.RTR == 0) {
		datacheck = 2;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		dotsFlag = 1;
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
