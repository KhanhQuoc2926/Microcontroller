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
/* Traffic LEDs */
#define NS_RED       GPIO_PIN_2
#define NS_YELLOW    GPIO_PIN_3
#define NS_GREEN     GPIO_PIN_4

#define EW_RED       GPIO_PIN_5
#define EW_YELLOW    GPIO_PIN_6
#define EW_GREEN     GPIO_PIN_7

#define ALL_TRAFFIC_LEDS (NS_RED | NS_YELLOW | NS_GREEN | \
                          EW_RED | EW_YELLOW | EW_GREEN)

/* 7-segment */
#define LED_A GPIO_PIN_0
#define LED_B GPIO_PIN_1
#define LED_C GPIO_PIN_2
#define LED_D GPIO_PIN_3
#define LED_E GPIO_PIN_4
#define LED_F GPIO_PIN_5
#define LED_G GPIO_PIN_6

#define ALL_SEGMENTS (LED_A | LED_B | LED_C | LED_D | \
                      LED_E | LED_F | LED_G)
											
											
#define EW_LED_A GPIO_PIN_7
#define EW_LED_B GPIO_PIN_8
#define EW_LED_C GPIO_PIN_9
#define EW_LED_D GPIO_PIN_10
#define EW_LED_E GPIO_PIN_11
#define EW_LED_F GPIO_PIN_12
#define EW_LED_G GPIO_PIN_13

#define EW_ALL_SEGMENTS (EW_LED_A | EW_LED_B | EW_LED_C | \
                         EW_LED_D | EW_LED_E | EW_LED_F | \
                         EW_LED_G)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display7SEG(int num)
{
    static const uint16_t digit[10] =
    {
        LED_A | LED_B | LED_C | LED_D | LED_E | LED_F,          // 0
        LED_B | LED_C,                                          // 1
        LED_A | LED_B | LED_D | LED_E | LED_G,                  // 2
        LED_A | LED_B | LED_C | LED_D | LED_G,                  // 3
        LED_B | LED_C | LED_F | LED_G,                          // 4
        LED_A | LED_C | LED_D | LED_F | LED_G,                  // 5
        LED_A | LED_C | LED_D | LED_E | LED_F | LED_G,          // 6
        LED_A | LED_B | LED_C,                                  // 7
        LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G,  // 8
        LED_A | LED_B | LED_C | LED_D | LED_F | LED_G           // 9
    };

    HAL_GPIO_WritePin(GPIOB, ALL_SEGMENTS, GPIO_PIN_SET);		// TAT LED

    if (num >= 0 && num <= 9)
    {
        HAL_GPIO_WritePin(GPIOB, digit[num], GPIO_PIN_RESET);
    }
}

void display7SEG_EW(int num)
{
    static const uint16_t digit[10] =
    {
        EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D | EW_LED_E | EW_LED_F,
        EW_LED_B | EW_LED_C,
        EW_LED_A | EW_LED_B | EW_LED_D | EW_LED_E | EW_LED_G,
        EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D | EW_LED_G,
        EW_LED_B | EW_LED_C | EW_LED_F | EW_LED_G,
        EW_LED_A | EW_LED_C | EW_LED_D | EW_LED_F | EW_LED_G,
        EW_LED_A | EW_LED_C | EW_LED_D | EW_LED_E | EW_LED_F | EW_LED_G,
        EW_LED_A | EW_LED_B | EW_LED_C,
        EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D |
        EW_LED_E | EW_LED_F | EW_LED_G,
        EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D |
        EW_LED_F | EW_LED_G
    };

    HAL_GPIO_WritePin(GPIOB, EW_ALL_SEGMENTS, GPIO_PIN_SET);

    if (num >= 0 && num <= 9)
    {
        HAL_GPIO_WritePin(GPIOB, digit[num], GPIO_PIN_RESET);
    }
}

void TrafficLight_Init(uint16_t ns_led, uint16_t ew_led)
{
    HAL_GPIO_WritePin(
        GPIOA,
        ALL_TRAFFIC_LEDS,
        GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        GPIOA,
        ns_led | ew_led,
        GPIO_PIN_SET
    );
}

void CountDown(int ns_seconds, int ew_seconds, int duration)
{
    for (int i = 0; i < duration; i++)
    {
        display7SEG(ns_seconds - i);
        display7SEG_EW(ew_seconds - i);

        HAL_Delay(1000);
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		TrafficLight_Init(NS_GREEN, EW_RED);
    CountDown(3, 5, 3);

    TrafficLight_Init(NS_YELLOW, EW_RED);
    CountDown(2, 2, 2);

    TrafficLight_Init(NS_RED, EW_GREEN);
    CountDown(5, 3, 3);

    TrafficLight_Init(NS_RED, EW_YELLOW);
    CountDown(2, 2, 2);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);

  /*Configure GPIO pins : PA2 PA3 PA4 PA5
                           PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB12 PB13 PB3
                           PB4 PB5 PB6 PB7
                           PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9;
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
