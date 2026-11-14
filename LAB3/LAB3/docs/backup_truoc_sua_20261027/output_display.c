#include "output_display.h"

static volatile int display_ns = 0;
static volatile int display_ew = 0;
static int scan_index = 0;

void display7SEG(int num)
{
    static const uint16_t digit[10] =
    {
        LED_A | LED_B | LED_C | LED_D | LED_E | LED_F,         /* 0 */
        LED_B | LED_C,                                       /* 1 */
        LED_A | LED_B | LED_D | LED_E | LED_G,                 /* 2 */
        LED_A | LED_B | LED_C | LED_D | LED_G,                 /* 3 */
        LED_B | LED_C | LED_F | LED_G,                        /* 4 */
        LED_A | LED_C | LED_D | LED_F | LED_G,                 /* 5 */
        LED_A | LED_C | LED_D | LED_E | LED_F | LED_G,         /* 6 */
        LED_A | LED_B | LED_C,                                /* 7 */
        LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G, /* 8 */
        LED_A | LED_B | LED_C | LED_D | LED_F | LED_G          /* 9 */
    };

    HAL_GPIO_WritePin(GPIOB, ALL_SEGMENTS, GPIO_PIN_SET);

    if (num >= 0 && num <= 9)
        HAL_GPIO_WritePin(GPIOB, digit[num], GPIO_PIN_RESET);
    else if (num == -1)
        HAL_GPIO_WritePin(GPIOB, LED_G, GPIO_PIN_RESET);
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
        EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D | EW_LED_E | EW_LED_F | EW_LED_G,
        EW_LED_A | EW_LED_B | EW_LED_C | EW_LED_D | EW_LED_F | EW_LED_G
    };

    HAL_GPIO_WritePin(GPIOB, EW_ALL_SEGMENTS, GPIO_PIN_SET);

    if (num >= 0 && num <= 9)
        HAL_GPIO_WritePin(GPIOB, digit[num], GPIO_PIN_RESET);
    else if (num == -1)
        HAL_GPIO_WritePin(GPIOB, EW_LED_G, GPIO_PIN_RESET);
}


void TrafficLight_Init(uint16_t ns_led, uint16_t ew_led)
{
    HAL_GPIO_WritePin(GPIOA, ALL_TRAFFIC_LEDS, GPIO_PIN_RESET);
    if (ns_led != 0 || ew_led != 0)
        HAL_GPIO_WritePin(GPIOA, ns_led | ew_led, GPIO_PIN_SET);
}

void output_init(void)
{
    /* HIGH tat transistor PNP chon digit. */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET);
    display_ns = 0;
    display_ew = 0;
    scan_index = 0;
    TrafficLight_Init(NS_RED, EW_RED);
}

void set_display(int ns, int ew)
{
    display_ns = ns;
    display_ew = ew;
}

/* Goi moi 1 ms. Hai huong quet cung hang chuc hoac cung hang don vi. */
void scan7SEG(void)
{
    int ns = display_ns;
    int ew = display_ew;

    /* Tat ca 4 digit truoc khi doi cac segment. */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET);

    if (scan_index == 0)
    {
        if (ns < 0) display7SEG(-1);
        else display7SEG(ns / 10);

        if (ew < 0) display7SEG_EW(-1);
        else display7SEG_EW(ew / 10);

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);				// turn on hang chuc
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
        scan_index = 1;
    }
    else
    {
        if (ns < 0) display7SEG(-1);
        else display7SEG(ns % 10);

        if (ew < 0) display7SEG_EW(-1);
        else display7SEG_EW(ew % 10);

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);			// turn on hang don vi
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
        scan_index = 0;
    }
}
