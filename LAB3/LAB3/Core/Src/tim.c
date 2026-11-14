#include "tim.h"
TIM_HandleTypeDef htim2;
void MX_TIM2_Init(void) {
    htim2.Instance=TIM2;
    htim2.Init.Prescaler=799;
    htim2.Init.CounterMode=TIM_COUNTERMODE_UP;
    htim2.Init.Period=99;
    htim2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&htim2)!=HAL_OK) Error_Handler();
}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance==TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn,2,0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
}
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance==TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
    }
}
