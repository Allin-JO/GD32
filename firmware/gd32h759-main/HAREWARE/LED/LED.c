#include "LED.h"

void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOE);
    board_gpio_output_init(BOARD_LED_PORT, BOARD_LED_PIN);
    gpio_bit_reset(BOARD_LED_PORT, BOARD_LED_PIN);
}

void LED_Flash(u8 time)
{
    static int led_delay = 0;

    if (time == 0)
    {
        gpio_bit_reset(BOARD_LED_PORT, BOARD_LED_PIN);
    }
    else if (++led_delay == time)
    {
        LED_Toggle();
        led_delay = 0;
    }
}

void LED_Switch(u8 state)
{
    if (state == 0)
    {
        gpio_bit_reset(BOARD_LED_PORT, BOARD_LED_PIN);
    }
    else
    {
        gpio_bit_set(BOARD_LED_PORT, BOARD_LED_PIN);
    }
}

void LED_Toggle(void)
{
    if (gpio_output_bit_get(BOARD_LED_PORT, BOARD_LED_PIN) == SET)
    {
        gpio_bit_reset(BOARD_LED_PORT, BOARD_LED_PIN);
    }
    else
    {
        gpio_bit_set(BOARD_LED_PORT, BOARD_LED_PIN);
    }
}
