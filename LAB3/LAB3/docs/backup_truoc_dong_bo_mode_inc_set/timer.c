#include "timer.h"
#include "tim.h"
#include "input_reading.h"
#include "output_display.h"

static volatile uint32_t time_ms = 0;
static volatile int ready = 0;
static int button_counter = 0;
static int scan_counter = 0;

void timer_start(void)
{
    ready = 0;
    time_ms = 0;
    button_counter = 0;
    scan_counter = 0;

    /* Clock TIM2 = 8 MHz, sau chia con 10 kHz. */
    HAL_TIM_Base_Stop_IT(&htim2);
    htim2.Init.Prescaler = 799;
    htim2.Init.Period = TIMER_CYCLE_MS * 10 - 1; /* Chu ky ngat TIM2, ms. */
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) Error_Handler();
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) Error_Handler();

    ready = 1;
}

uint32_t timer_get_time(void)
{
    return time_ms;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
        time_ms = time_ms + TIMER_CYCLE_MS;
}

/* Goi tu SysTick sau HAL_IncTick(). */		// de hien thi gia tri cua den lien tuc
void timer_tick_1ms(void)
{
    if (ready == 0) return;

    /* Quet moi 5 ms: moi digit duoc lam tuoi 100 lan/giay. */
    scan_counter++;
    if (scan_counter >= 5)
    {
        scan_counter = 0;
        scan7SEG();
    }

    /* Doc tat ca nut moi 10 ms. */
    button_counter++;
    if (button_counter >= 10)
    {
        button_counter = 0;
        button_reading();
    }
}
