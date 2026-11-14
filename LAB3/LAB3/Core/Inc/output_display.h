#ifndef OUTPUT_DISPLAY_H
#define OUTPUT_DISPLAY_H

#include "main.h"

#define NS_RED       GPIO_PIN_2
#define NS_YELLOW    GPIO_PIN_3
#define NS_GREEN     GPIO_PIN_4
#define EW_RED       GPIO_PIN_5
#define EW_YELLOW    GPIO_PIN_6
#define EW_GREEN     GPIO_PIN_7
#define ALL_TRAFFIC_LEDS (NS_RED | NS_YELLOW | NS_GREEN | EW_RED | EW_YELLOW | EW_GREEN)

#define LED_A GPIO_PIN_0
#define LED_B GPIO_PIN_1
#define LED_C GPIO_PIN_2
#define LED_D GPIO_PIN_3
#define LED_E GPIO_PIN_4
#define LED_F GPIO_PIN_5
#define LED_G GPIO_PIN_6
#define ALL_SEGMENTS (LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G)

#define EW_LED_A GPIO_PIN_7
#define EW_LED_B GPIO_PIN_8
#define EW_LED_C GPIO_PIN_9
#define EW_LED_D GPIO_PIN_10
#define EW_LED_E GPIO_PIN_11
#define EW_LED_F GPIO_PIN_12
#define EW_LED_G GPIO_PIN_13
#define EW_ALL_SEGMENTS (EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D | EW_LED_E | EW_LED_F | EW_LED_G)

void output_init(void);
void display7SEG(int num);
void display7SEG_EW(int num);
void TrafficLight_Init(uint16_t ns_led, uint16_t ew_led);
void set_display(int ns, int ew);
void scan7SEG(void);

#endif
