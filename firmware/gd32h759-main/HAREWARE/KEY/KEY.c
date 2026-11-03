#include "KEY.h"

void Key_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOD);
    board_gpio_input_pullup_init(BOARD_KEY_PORT, BOARD_KEY_PIN);
}

u8 click(void)
{
    static u8 flag_key = 1;

    if (flag_key && KEY_READ() == RESET)
    {
        flag_key = 0;
        return 1;
    }
    else if (KEY_READ() == SET)
    {
        flag_key = 1;
    }
    return 0;
}

u8 click_N_Double(u8 time)
{
    static u8 flag_key, count_key, double_key;
    static u16 count_single, forever_count;

    if (KEY_READ() == RESET)
    {
        forever_count++;
    }
    else
    {
        forever_count = 0;
    }

    if ((KEY_READ() == RESET) && (flag_key == 0))
    {
        flag_key = 1;
    }

    if (count_key == 0)
    {
        if (flag_key == 1)
        {
            double_key++;
            count_key = 1;
        }
        if (double_key == 2)
        {
            double_key = 0;
            count_single = 0;
            return 2;
        }
    }

    if (KEY_READ() == SET)
    {
        flag_key = 0;
        count_key = 0;
    }

    if (double_key == 1)
    {
        count_single++;
        if ((count_single > time) && (forever_count < time))
        {
            double_key = 0;
            count_single = 0;
            return 1;
        }
        if (forever_count > time)
        {
            double_key = 0;
            count_single = 0;
        }
    }
    return 0;
}
