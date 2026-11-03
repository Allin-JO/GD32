#include "board_h759.h"

void board_gpio_output_init(uint32_t port, uint32_t pin)
{
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, pin);
}

void board_gpio_input_pullup_init(uint32_t port, uint32_t pin)
{
    gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pin);
}

void board_gpio_analog_init(uint32_t port, uint32_t pin)
{
    gpio_mode_set(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pin);
}

void board_gpio_af_init(uint32_t port, uint32_t pin, uint32_t af)
{
    gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, pin);
    gpio_af_set(port, af, pin);
}
