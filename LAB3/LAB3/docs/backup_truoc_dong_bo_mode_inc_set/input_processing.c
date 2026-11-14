#include "input_processing.h"
#include "input_reading.h"
#include "output_display.h"
#include "timer.h"

int mode = 1;
int red_time = 5;
int yellow_time = 2;
int green_time = 3;
int edit_value = 0;
int phase = 0;
int ns_count = 0;
int ew_count = 0;

static uint32_t last_second = 0;
static uint32_t last_blink = 0;
static int blink_on = 1;

static void show_phase(void)
{
    switch (phase)
    {
    case 0:
        ns_count = green_time;
        ew_count = red_time;
        TrafficLight_Init(NS_GREEN, EW_RED);
        break;
    case 1:
        ns_count = yellow_time;
        ew_count = yellow_time;
        TrafficLight_Init(NS_YELLOW, EW_RED);
        break;
    case 2:
        ns_count = red_time;
        ew_count = green_time;
        TrafficLight_Init(NS_RED, EW_GREEN);
        break;
    case 3:
        ns_count = yellow_time;
        ew_count = yellow_time;
        TrafficLight_Init(NS_RED, EW_YELLOW);
        break;
    }
    set_display(ns_count, ew_count);
}

static void start_normal(void)
{
    last_second = timer_get_time();
    phase = 0;

    if (red_time != green_time + yellow_time)
    {
        TrafficLight_Init(NS_RED, EW_RED);
        set_display(-1, -1);
        return;
    }

    show_phase();
}

static void normal_run(void)
{
    if (red_time != green_time + yellow_time) return;

    if (timer_get_time() - last_second >= 1000)
    {
        last_second = last_second + 1000;
        ns_count--;
        ew_count--;

			if ((phase == 0 || phase == 1) && ns_count == 0)				//ns :green / yellow
        {
            phase++;											// doi sang den khac
            show_phase();
        }
        else if ((phase == 2 || phase == 3) && ew_count == 0)
        {
            phase++;
            if (phase > 3) phase = 0;
            show_phase();
        }

        set_display(ns_count, ew_count);
    }
}

static void show_setting_lights(void)
{
    if (blink_on == 0)
        TrafficLight_Init(0, 0);
    else if (mode == 2)
        TrafficLight_Init(NS_RED, EW_RED);
    else if (mode == 3)
        TrafficLight_Init(NS_YELLOW, EW_YELLOW);
    else if (mode == 4)
        TrafficLight_Init(NS_GREEN, EW_GREEN);
}

static void change_mode(void)
{
    mode++;
    if (mode > 4) mode = 1;

    if (mode == 1)
    {
        start_normal();
    }
    else
    {
        if (mode == 2) edit_value = red_time;
        else if (mode == 3) edit_value = yellow_time;
        else edit_value = green_time;

        blink_on = 1;
        last_blink = HAL_GetTick();
        show_setting_lights();
        set_display(edit_value, mode);
    }
}

void fsm_init(void)
{
    mode = 1;
    red_time = 5;
    yellow_time = 2;
    green_time = 3;
    edit_value = 0;
    start_normal();
}

void fsm_run(void)
{
    int button1 = button_pressed_event(0);
    int button2 = button_pressed_event(1);
    int button3 = button_pressed_event(2);

    if (button1 == 1)
    {
			change_mode();									// change mode 1: normal_run 		2: change red_time		3: change yellow_time		4: change green_time
        return;
    }

    if (mode == 1)											// mode == 1 -> chay binh thuong 
    {
        normal_run();
    }
    else																// mode != 1 -> chinh thoi gian -> button2
    {
        if (button2 == 1)
        {
            edit_value++;
            if (edit_value > 99) edit_value = 1;
        }

        if (button3 == 1)
        {
            if (mode == 2) red_time = edit_value;
            else if (mode == 3) yellow_time = edit_value;
            else green_time = edit_value;
        }

        if (HAL_GetTick() - last_blink >= 250)										// nhap nhay den dang chinh easy to define led
        {
            last_blink = last_blink + 250;
            if (blink_on == 1) blink_on = 0;
            else blink_on = 1;																

            show_setting_lights();
        }

        set_display(edit_value, mode);
    }
}
