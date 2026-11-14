#include "main.h"
#include "input_reading.h"

static uint16_t button_pin[3] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10};
static GPIO_PinState sample0[3];
static GPIO_PinState sample1[3];
static GPIO_PinState sample2[3];
static volatile GPIO_PinState stable[3];
static GPIO_PinState previous[3];

void input_init(void)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        sample0[i] = GPIO_PIN_SET;
        sample1[i] = GPIO_PIN_SET;
        sample2[i] = GPIO_PIN_SET;
        stable[i] = GPIO_PIN_SET;
        previous[i] = GPIO_PIN_SET;
    }
}

/* Lay 3 mau lien tiep, cach nhau 10 ms. */
void button_reading(void)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        sample2[i] = sample1[i];
        sample1[i] = sample0[i];
        
        if (HAL_GPIO_ReadPin(GPIOA, button_pin[i]) == GPIO_PIN_RESET)
            sample0[i] = GPIO_PIN_RESET;
        else
            sample0[i] = GPIO_PIN_SET;

        if (sample0[i] == sample1[i] && sample1[i] == sample2[i])
            stable[i] = sample0[i];
    }
}

int button_pressed_event(int index)
{
    GPIO_PinState current;
    int result = 0;

    if (index < 0 || index > 2) return 0;

    current = stable[index];
    if (previous[index] == GPIO_PIN_SET && current == GPIO_PIN_RESET)
        result = 1;

    previous[index] = current;
    return result;
}
