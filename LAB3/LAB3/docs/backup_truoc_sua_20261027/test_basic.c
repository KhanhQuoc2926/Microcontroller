#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "timer.h"
#include "input_processing.h"
#include "input_reading.h"
#include "output_display.h"
GPIO_TypeDef ga={.IDR=0x700},gb;
RCC_TypeDef rcc;
TIM_HandleTypeDef htim2={.Instance=TIM2};
static uint32_t now;
static int ghost_changes;
uint32_t HAL_GetTick(void){return now;}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *t);
void HAL_GPIO_WritePin(GPIO_TypeDef *p,uint16_t pin,int value){
 if(p==GPIOB&&(pin&0x3FFFU)&&((ga.ODR&0x1800U)!=0x1800U||(gb.ODR&0xC000U)!=0xC000U))ghost_changes++;
 if(value)p->ODR|=pin;else p->ODR&=~pin;
}
static unsigned lights(void){return (ga.ODR>>2)&63U;}
static void step(int ms){for(int i=0;i<ms;i++){
 now++;timer_tick_1ms();if(now%TIMER_CYCLE_MS==0)HAL_TIM_PeriodElapsedCallback(&htim2);fsm_run();
 if(mode==1)assert((lights()&(4|32))!=(4|32));
}}
static void press(int n){ga.IDR&=~(1U<<(n+8));step(40);ga.IDR|=1U<<(n+8);step(40);}
int main(void){
 timer_start();input_init();output_init();fsm_init();ghost_changes=0;
 assert(htim2.Init.Prescaler==799&&htim2.Init.Period==TIMER_CYCLE_MS*10-1);
 assert(ns_count==3&&ew_count==5&&lights()==12);
 step(1000);assert(ns_count==2&&ew_count==4);
 step(2000);assert(phase==1&&ns_count==2&&ew_count==2&&lights()==10);
 step(2000);assert(phase==2&&ns_count==5&&ew_count==3&&lights()==33);
 step(3000);assert(phase==3&&ns_count==2&&ew_count==2&&lights()==17);
 step(2000);assert(phase==0&&lights()==12);
 for(int i=0;i<3;i++){ga.IDR&=~256U;step(5);ga.IDR|=256U;step(5);}step(40);assert(mode==1);
 ga.IDR&=~256U;step(2000);ga.IDR|=256U;step(40);assert(mode==2);
 unsigned last=lights();int transitions=0;
 for(int i=0;i<1000;i++){step(1);unsigned current=lights();assert(current==0||current==9);if(current!=last){transitions++;last=current;}}
 assert(transitions==4);
 press(1);assert(edit_value==6&&red_time==5);press(0);press(0);press(0);press(0);assert(mode==2&&edit_value==5);
 press(1);press(2);assert(red_time==6);press(0);press(0);press(0);assert(mode==1&&lights()==9);step(1000);assert(lights()==9);
 press(0);press(0);press(1);press(2);assert(yellow_time==3);press(0);press(0);assert(mode==1&&lights()==12);
 press(0);for(int i=0;i<93;i++)press(1);assert(edit_value==99);press(1);assert(edit_value==1);press(2);assert(red_time==1);
 for(int i=0;i<4;i++){step(1);unsigned ns=(~gb.ODR)&127U,ew=((~gb.ODR)>>7)&127U;
  if(!(gb.ODR&GPIO_PIN_14)){assert(ns==0x3F&&ew==0x3F);assert(!(ga.ODR&GPIO_PIN_11));}
  else {assert(ns==0x06&&ew==0x5B);assert(!(ga.ODR&GPIO_PIN_12));}}
 assert(ghost_changes==0);
 ga.IDR=0;step(40);ga.IDR=0x700;step(40);assert(mode==3&&edit_value==3&&yellow_time==3);
 /* Rebase the blink clock by entering mode4 near rollover. */
 now=UINT32_MAX-1000U;
 press(0);assert(mode==4);last=lights();transitions=0;
 for(int i=0;i<2000;i++){step(1);unsigned current=lights();if(current!=last){transitions++;last=current;}}
 assert(transitions==8);
 printf("PASS basic tick=%d: countdown/phases, bounce/hold, 2Hz, edit/save/discard, range99->1, error/recover, activeLOW scan, no ghost writes, button priority\n",TIMER_CYCLE_MS);
}
