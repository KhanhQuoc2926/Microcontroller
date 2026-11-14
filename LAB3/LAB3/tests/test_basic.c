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
static void blink_test(unsigned colour,int duration,int expected){
 unsigned last=lights();int transitions=0;
 for(int i=0;i<duration;i++){step(1);unsigned current=lights();assert(current==0||current==colour);if(current!=last){transitions++;last=current;}}
 assert(transitions==expected);
}
static void valid_times(void){assert(red_time==green_time+yellow_time);}
int main(void){
 timer_start();input_init();
 /* Events survive press+release while the main loop has not consumed them. */
 for(int n=0;n<3;n++){
  ga.IDR&=~(1U<<(n+8));for(int k=0;k<3;k++)button_reading();
  ga.IDR|=1U<<(n+8);for(int k=0;k<3;k++)button_reading();
  assert(button_pressed_event(n)==1);assert(button_pressed_event(n)==0);
 }
 input_init();output_init();fsm_init();ghost_changes=0;
 assert(htim2.Init.Prescaler==799&&htim2.Init.Period==TIMER_CYCLE_MS*10-1);
 assert(ns_count==3&&ew_count==5&&lights()==12);
 step(1000);assert(ns_count==2&&ew_count==4);
 step(2000);assert(phase==1&&ns_count==2&&ew_count==2&&lights()==10);
 step(2000);assert(phase==2&&ns_count==5&&ew_count==3&&lights()==33);
 step(3000);assert(phase==3&&ns_count==2&&ew_count==2&&lights()==17);
 step(2000);assert(phase==0&&lights()==12);
 ga.IDR&=~256U;step(20);ga.IDR|=256U;step(40);assert(mode==1);
 for(int i=0;i<3;i++){ga.IDR&=~256U;step(5);ga.IDR|=256U;step(5);}step(40);assert(mode==1);
 /* MODE held: one transition only. */
 ga.IDR&=~256U;step(2000);ga.IDR|=256U;step(40);assert(mode==2);
 blink_test(9,1000,4);
 press(1);assert(edit_value==6&&red_time==5);
 press(2);assert(red_time==6&&green_time==4&&yellow_time==2);valid_times();
 press(0);assert(mode==3&&edit_value==2);blink_test(18,1000,4);
 press(1);press(2);assert(yellow_time==3&&green_time==3&&red_time==6);valid_times();
 press(0);assert(mode==4&&edit_value==3);blink_test(36,1000,4);
 press(1);press(2);assert(green_time==4&&red_time==7&&yellow_time==3);valid_times();
 press(0);assert(mode==1&&ns_count==4&&ew_count==7&&lights()==12);
 step(1000);assert(ns_count==3&&ew_count==6);
 press(0);assert(mode==2&&edit_value==7);
 /* INC initial press + one repeat after holding about 1.5 seconds. */
 ga.IDR&=~512U;step(1700);ga.IDR|=512U;step(40);assert(edit_value==9);
 press(2);assert(red_time==9&&green_time==6);valid_times();
 while(edit_value<99){press(1);}
 press(1);assert(edit_value==1);
 press(2);assert(red_time==9&&edit_value==9);valid_times();
 press(1);assert(edit_value==10);press(0);assert(mode==3&&edit_value==3);
 for(int i=0;i<6;i++){press(1);}
 assert(edit_value==9);
 press(2);assert(yellow_time==3&&edit_value==3);valid_times();
 press(0);assert(mode==4&&edit_value==6);
 for(int i=0;i<91;i++){press(1);}
 assert(edit_value==97);
 press(2);assert(green_time==6&&edit_value==6);valid_times();
 press(1);press(2);assert(green_time==7&&red_time==10);valid_times();
 press(0);assert(mode==1&&ns_count==7&&ew_count==10);
 ga.IDR=0;step(40);ga.IDR=0x700;step(40);assert(mode==2&&edit_value==10);
 now=UINT32_MAX-1000U;press(0);assert(mode==3);blink_test(18,2000,8);
 set_display(12,34);
 for(int i=0;i<2;i++){
  scan7SEG();unsigned ns=(~gb.ODR)&127U,ew=((~gb.ODR)>>7)&127U;
  if(!(gb.ODR&GPIO_PIN_14)){assert(ns==0x06&&ew==0x4F);assert(!(ga.ODR&GPIO_PIN_11));}
  else {assert(ns==0x5B&&ew==0x66);assert(!(ga.ODR&GPIO_PIN_12));}
 }
 assert(ghost_changes==0);
 printf("PASS tick=%d: MODE/INC/SET, retained press events, 3-sample debounce, held INC repeat, R=G+Y save/reject, 2Hz all colours, countdown, range/wrap, scan, timer rollover\n",TIMER_CYCLE_MS);
}
