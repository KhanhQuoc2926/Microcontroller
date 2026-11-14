#include "main.h"
#include "input_reading.h"

static uint16_t button_pin[3] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10};
static GPIO_PinState sample0[3];
static GPIO_PinState sample1[3];
static GPIO_PinState sample2[3];
static volatile GPIO_PinState stable[3];
/* So lan nhan da duoc chong doi va so lan main da xu ly. */
static volatile uint32_t press_count[3];
static uint32_t handled_count[3];
static int hold_counter[3];
static volatile uint32_t repeat_count[3];
static uint32_t handled_repeat_count[3];

void input_init(void)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        sample0[i] = GPIO_PIN_SET;
        sample1[i] = GPIO_PIN_SET;
        sample2[i] = GPIO_PIN_SET;
        stable[i] = GPIO_PIN_SET;
        press_count[i] = 0;
        handled_count[i] = 0;
        hold_counter[i] = 0;
        repeat_count[i] = 0;
        handled_repeat_count[i] = 0;
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
        {
            /* Ghi nho lan nhan ngay khi trang thai on dinh chuyen 1 -> 0. */
            if (stable[i] == GPIO_PIN_SET && sample0[i] == GPIO_PIN_RESET)
                press_count[i]++;

            stable[i] = sample0[i];
        }

        /* Moi lan doc cach nhau 10 ms. Lan lap dau sau khoang 1,5 s. */
        if (stable[i] == GPIO_PIN_RESET)
        {
            hold_counter[i]++;
            if (hold_counter[i] >= 150)
            {
                hold_counter[i] = 100; /* Lap tiep sau 50 lan doc = 500 ms. */
                repeat_count[i]++;
            }
        }
        else
        {
            hold_counter[i] = 0;
        }
    }
}

/* Chi goi tu main. Lan nhan van duoc giu lai du nut da nha. */
int button_pressed_event(int index)
{
    if (index < 0 || index > 2) return 0;

    if (handled_count[index] != press_count[index])
    {
        handled_count[index]++;
        return 1;
    }

    return 0;
}

/* Su kien giu nut: sau 1 giay, tu lap moi 500 ms. */
int button_repeat_event(int index)
{
    if (index < 0 || index > 2) return 0;

    if (handled_repeat_count[index] != repeat_count[index])
    {
        handled_repeat_count[index]++;
        return 1;
    }

    return 0;
}
