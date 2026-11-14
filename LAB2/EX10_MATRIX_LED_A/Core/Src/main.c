#include "main.h"

/* USER CODE BEGIN PD */

#define LED_A GPIO_PIN_0
#define LED_B GPIO_PIN_1
#define LED_C GPIO_PIN_2
#define LED_D GPIO_PIN_3
#define LED_E GPIO_PIN_4
#define LED_F GPIO_PIN_5
#define LED_G GPIO_PIN_6

#define ALL_LEDS (LED_A | LED_B | LED_C | LED_D | \
                  LED_E | LED_F | LED_G)

#define EN0_PIN GPIO_PIN_6
#define EN1_PIN GPIO_PIN_7
#define EN2_PIN GPIO_PIN_8
#define EN3_PIN GPIO_PIN_9

#define ALL_EN (EN0_PIN | EN1_PIN | EN2_PIN | EN3_PIN)

#define DOT_PIN     GPIO_PIN_4
#define RED_LED_PIN GPIO_PIN_5

#define ENM0_PIN GPIO_PIN_2
#define ENM1_PIN GPIO_PIN_3
#define ENM2_PIN GPIO_PIN_10
#define ENM3_PIN GPIO_PIN_11
#define ENM4_PIN GPIO_PIN_12
#define ENM5_PIN GPIO_PIN_13
#define ENM6_PIN GPIO_PIN_14
#define ENM7_PIN GPIO_PIN_15

#define ALL_ENM (ENM0_PIN | ENM1_PIN | ENM2_PIN | ENM3_PIN | \
                 ENM4_PIN | ENM5_PIN | ENM6_PIN | ENM7_PIN)

#define ALL_ROWS (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
                  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
                  GPIO_PIN_14 | GPIO_PIN_15)

#define TIMER_CYCLE 10

/* USER CODE END PD */

/* Private variables */
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {0, 0, 0, 0};

int hour = 15;
int minute = 8;
int second = 50;

const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;

/*
 * Moi phan tu chua du lieu cua mot cot.
 * Bit 0 ung voi ROW0, bit 7 ung voi ROW7.
 * Bit bang 1 la diem can sang.
 */
uint8_t matrix_buffer[8] =
{
    0x00, 0x7E, 0x09, 0x09,
    0x09, 0x7E, 0x00, 0x00
};

int timer0_counter = 0;
int timer1_counter = 0;
int timer2_counter = 0;
int timer3_counter = 0;

int timer0_flag = 0;
int timer1_flag = 0;
int timer2_flag = 0;
int timer3_flag = 0;

/* USER CODE END PV */

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN 0 */

void display7SEG(int num)
{
    static const uint16_t digit[10] =
    {
        LED_A | LED_B | LED_C | LED_D | LED_E | LED_F,
        LED_B | LED_C,
        LED_A | LED_B | LED_D | LED_E | LED_G,
        LED_A | LED_B | LED_C | LED_D | LED_G,
        LED_B | LED_C | LED_F | LED_G,
        LED_A | LED_C | LED_D | LED_F | LED_G,
        LED_A | LED_C | LED_D | LED_E | LED_F | LED_G,
        LED_A | LED_B | LED_C,
        LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G,
        LED_A | LED_B | LED_C | LED_D | LED_F | LED_G
    };

    /* Common anode: HIGH tat, LOW sang */
    HAL_GPIO_WritePin(GPIOB, ALL_LEDS, GPIO_PIN_SET);

    if (num >= 0 && num <= 9)
    {
        HAL_GPIO_WritePin(GPIOB, digit[num], GPIO_PIN_RESET);
    }
}

void update7SEG(int index)
{
    /* Tat ca 4 LED truoc khi doi du lieu */
    HAL_GPIO_WritePin(GPIOA, ALL_EN, GPIO_PIN_SET);

    if (index < 0 || index >= MAX_LED)
    {
        return;
    }

    display7SEG(led_buffer[index]);

    switch (index)
    {
        case 0:
            HAL_GPIO_WritePin(GPIOA, EN0_PIN, GPIO_PIN_RESET);
            break;

        case 1:
            HAL_GPIO_WritePin(GPIOA, EN1_PIN, GPIO_PIN_RESET);
            break;

        case 2:
            HAL_GPIO_WritePin(GPIOA, EN2_PIN, GPIO_PIN_RESET);
            break;

        case 3:
            HAL_GPIO_WritePin(GPIOA, EN3_PIN, GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}

void updateClockBuffer(void)
{
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

void updateClock(void)
{
    second++;

    if (second >= 60)
    {
        second = 0;
        minute++;

        if (minute >= 60)
        {
            minute = 0;
            hour++;

            if (hour >= 24)
            {
                hour = 0;
            }
        }
    }

    updateClockBuffer();
}

void setRow(int col)
{
    uint16_t row_data;

    row_data = (uint16_t)((uint16_t)matrix_buffer[col] << 8);

    /* ROW HIGH: tat diem LED */
    HAL_GPIO_WritePin(GPIOB, ALL_ROWS, GPIO_PIN_SET);

    /* ROW LOW: bat cac diem co bit 1 */
    if (row_data != 0)
    {
        HAL_GPIO_WritePin(GPIOB, row_data, GPIO_PIN_RESET);
    }
}

void updateLEDMatrix(int index)
{
    /*
     * Theo so do cua ban:
     * ENM HIGH: tat cot.
     * ENM LOW: chon cot.
     */

    HAL_GPIO_WritePin(GPIOA, ALL_ENM, GPIO_PIN_SET);

    if (index < 0 || index >= MAX_LED_MATRIX)
    {
        return;
    }

    setRow(index);

    switch (index)
    {
        case 0:
            HAL_GPIO_WritePin(GPIOA, ENM0_PIN, GPIO_PIN_RESET);
            break;

        case 1:
            HAL_GPIO_WritePin(GPIOA, ENM1_PIN, GPIO_PIN_RESET);
            break;

        case 2:
            HAL_GPIO_WritePin(GPIOA, ENM2_PIN, GPIO_PIN_RESET);
            break;

        case 3:
            HAL_GPIO_WritePin(GPIOA, ENM3_PIN, GPIO_PIN_RESET);
            break;

        case 4:
            HAL_GPIO_WritePin(GPIOA, ENM4_PIN, GPIO_PIN_RESET);
            break;

        case 5:
            HAL_GPIO_WritePin(GPIOA, ENM5_PIN, GPIO_PIN_RESET);
            break;

        case 6:
            HAL_GPIO_WritePin(GPIOA, ENM6_PIN, GPIO_PIN_RESET);
            break;

        case 7:
            HAL_GPIO_WritePin(GPIOA, ENM7_PIN, GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}

void shiftMatrixLeft(void)
{
    int i;
    uint8_t first_column;

    /* Luu cot dau tien */
    first_column = matrix_buffer[0];

    /* Dich du lieu sang trai mot cot */
    for (i = 0; i < MAX_LED_MATRIX - 1; i++)
    {
        matrix_buffer[i] = matrix_buffer[i + 1];
    }

    /* Dua cot dau ve cuoi de tao hieu ung chay vong */
    matrix_buffer[MAX_LED_MATRIX - 1] = first_column;
}

void setTimer0(int duration)
{
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

void setTimer1(int duration)
{
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration)
{
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

void setTimer3(int duration)
{
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}

void timer_run(void)
{
    if (timer0_counter > 0)
    {
        timer0_counter--;

        if (timer0_counter == 0)
        {
            timer0_flag = 1;
        }
    }

    if (timer1_counter > 0)
    {
        timer1_counter--;

        if (timer1_counter == 0)
        {
            timer1_flag = 1;
        }
    }

    if (timer2_counter > 0)
    {
        timer2_counter--;

        if (timer2_counter == 0)
        {
            timer2_flag = 1;
        }
    }

    if (timer3_counter > 0)
    {
        timer3_counter--;

        if (timer3_counter == 0)
        {
            timer3_flag = 1;
        }
    }
}

/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */

  /* Tat LED 7 doan */
  HAL_GPIO_WritePin(GPIOA, ALL_EN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, ALL_LEDS, GPIO_PIN_SET);

  /* Tat DOT va LED do */
  HAL_GPIO_WritePin(GPIOA, DOT_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, RED_LED_PIN, GPIO_PIN_SET);

  /* Tat LED ma tran */
  HAL_GPIO_WritePin(GPIOA, ALL_ENM, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, ALL_ROWS, GPIO_PIN_SET);

  updateClockBuffer();

  /* Hien thi LED dau tien */
  update7SEG(0);
  index_led = 1;

  /* Hien thi cot dau tien */
  updateLEDMatrix(0);
  index_led_matrix = 1;

  setTimer0(1000);
  setTimer1(250);
  setTimer2(10);
  setTimer3(200);

  HAL_TIM_Base_Start_IT(&htim2);

  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* Cap nhat dong ho va DOT */
    if (timer0_flag == 1)
    {
      setTimer0(1000);

      HAL_GPIO_TogglePin(GPIOA, DOT_PIN);

      updateClock();
    }

    /* Quet LED 7 doan */
    if (timer1_flag == 1)
    {
      setTimer1(250);

      update7SEG(index_led);

      index_led++;

      if (index_led >= MAX_LED)
      {
        index_led = 0;
      }
    }

    /* Quet LED ma tran */
    if (timer2_flag == 1)
    {
      setTimer2(10);

      /*
       * Dich chu o dau luot quet moi.
       * Nhu vay 8 cot su dung cung mot buffer.
       */
      if (index_led_matrix == 0 && timer3_flag == 1)
      {
        setTimer3(200);

        shiftMatrixLeft();
      }

      updateLEDMatrix(index_led_matrix);

      index_led_matrix++;

      if (index_led_matrix >= MAX_LED_MATRIX)
      {
        index_led_matrix = 0;
      }
    }
  }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue =
      RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                         FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload =
      TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

  if (HAL_TIM_ConfigClockSource(&htim2,
                               &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(
          &htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Giai phong cac chan JTAG/SWD de dung lam GPIO */
  __HAL_AFIO_REMAP_SWJ_DISABLE();

  /* Dat muc tat truoc khi cau hinh output */
  HAL_GPIO_WritePin(
      GPIOA,
      ALL_EN | ALL_ENM | DOT_PIN | RED_LED_PIN,
      GPIO_PIN_SET);

  HAL_GPIO_WritePin(
      GPIOB,
      ALL_LEDS | ALL_ROWS,
      GPIO_PIN_SET);

  GPIO_InitStruct.Pin =
      ALL_EN | ALL_ENM | DOT_PIN | RED_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ALL_LEDS | ALL_ROWS;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        timer_run();
    }
}

/* USER CODE END 4 */

void Error_Handler(void)
{
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif