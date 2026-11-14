#ifndef MOCK_MAIN_H
#define MOCK_MAIN_H
#include <stdint.h>
#include <stdlib.h>
#define GPIO_PIN_0 1U
#define GPIO_PIN_1 2U
#define GPIO_PIN_2 4U
#define GPIO_PIN_3 8U
#define GPIO_PIN_4 16U
#define GPIO_PIN_5 32U
#define GPIO_PIN_6 64U
#define GPIO_PIN_7 128U
#define GPIO_PIN_8 256U
#define GPIO_PIN_9 512U
#define GPIO_PIN_10 1024U
#define GPIO_PIN_11 2048U
#define GPIO_PIN_12 4096U
#define GPIO_PIN_13 8192U
#define GPIO_PIN_14 16384U
#define GPIO_PIN_15 32768U
#define GPIO_PIN_RESET 0
#define GPIO_PIN_SET 1
typedef int GPIO_PinState;
typedef struct {uint32_t IDR,ODR;} GPIO_TypeDef;
extern GPIO_TypeDef ga,gb;
#define GPIOA (&ga)
#define GPIOB (&gb)
typedef struct {uint32_t CFGR;} RCC_TypeDef;
extern RCC_TypeDef rcc;
#define RCC (&rcc)
#define RCC_CFGR_PPRE1 0x700U
#define TIM2 ((void*)2)
#define HAL_OK 0
#define TIM_FLAG_UPDATE 1
typedef struct {void *Instance;struct {uint32_t Prescaler,Period;} Init;} TIM_HandleTypeDef;
static inline uint32_t __get_PRIMASK(void){return 0;}
static inline void __disable_irq(void){}
static inline void __set_PRIMASK(uint32_t x){(void)x;}
static inline int HAL_GPIO_ReadPin(GPIO_TypeDef *p,uint16_t pin){return (p->IDR&pin)!=0;}
void HAL_GPIO_WritePin(GPIO_TypeDef *p,uint16_t pin,int value);
uint32_t HAL_GetTick(void);
static inline uint32_t HAL_RCC_GetPCLK1Freq(void){return 8000000U;}
static inline int HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *t){(void)t;return 0;}
static inline int HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *t){(void)t;return 0;}
static inline int HAL_TIM_Base_Init(TIM_HandleTypeDef *t){(void)t;return 0;}
#define __HAL_TIM_SET_COUNTER(t,n) ((void)0)
#define __HAL_TIM_CLEAR_FLAG(t,n) ((void)0)
static inline void Error_Handler(void){abort();}
#endif
