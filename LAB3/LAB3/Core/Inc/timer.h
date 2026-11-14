#ifndef TIMER_H
#define TIMER_H

#include "main.h"

/* Bai 4: doi thanh 1, 10 hoac 100, roi build lai. Clock van la HSI 8 MHz. */
#ifndef TIMER_CYCLE_MS
#define TIMER_CYCLE_MS 10
#endif

void timer_start(void);
uint32_t timer_get_time(void);
void timer_tick_1ms(void);

#endif
