#include "Encoder.h"

static void encoder_timer_init(uint32_t timer_periph)
{
    timer_parameter_struct time_base;

    timer_deinit(timer_periph);
    timer_struct_para_init(&time_base);
    time_base.prescaler = 0;
    time_base.alignedmode = TIMER_COUNTER_EDGE;
    time_base.counterdirection = TIMER_COUNTER_UP;
    time_base.clockdivision = TIMER_CKDIV_DIV1;
    time_base.period = 65535;
    time_base.repetitioncounter = 0;
    timer_init(timer_periph, &time_base);

    timer_quadrature_decoder_mode_config(timer_periph,
                                         TIMER_QUAD_DECODER_MODE2,
                                         TIMER_IC_POLARITY_RISING,
                                         TIMER_IC_POLARITY_RISING);
    timer_counter_value_config(timer_periph, 0);
    timer_enable(timer_periph);
}

void Encoder_TIM2_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_TIMER1);

    board_gpio_af_init(BOARD_ENC1_PORT_A, BOARD_ENC1_PIN_A, BOARD_ENC1_AF);
    board_gpio_af_init(BOARD_ENC1_PORT_B, BOARD_ENC1_PIN_B, BOARD_ENC1_AF);
    encoder_timer_init(BOARD_ENC1_TIMER);
}

void Encoder_TIM3_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER2);

    board_gpio_af_init(BOARD_ENC2_PORT_A, BOARD_ENC2_PIN_A, BOARD_ENC2_AF);
    board_gpio_af_init(BOARD_ENC2_PORT_B, BOARD_ENC2_PIN_B, BOARD_ENC2_AF);
    encoder_timer_init(BOARD_ENC2_TIMER);
}

void Encoder_TIM4_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_TIMER3);

    board_gpio_af_init(BOARD_ENC3_PORT_A, BOARD_ENC3_PIN_A, BOARD_ENC3_AF);
    board_gpio_af_init(BOARD_ENC3_PORT_B, BOARD_ENC3_PIN_B, BOARD_ENC3_AF);
    encoder_timer_init(BOARD_ENC3_TIMER);
}

void Encoder_TIM5_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER4);

    board_gpio_af_init(BOARD_ENC4_PORT_A, BOARD_ENC4_PIN_A, BOARD_ENC4_AF);
    board_gpio_af_init(BOARD_ENC4_PORT_B, BOARD_ENC4_PIN_B, BOARD_ENC4_AF);
    encoder_timer_init(BOARD_ENC4_TIMER);
}

int Read_Encoder(u8 TIMX)
{
    int encoder_value;
    uint32_t timer_periph;

    switch (TIMX)
    {
        case 2: timer_periph = BOARD_ENC1_TIMER; break;
        case 3: timer_periph = BOARD_ENC2_TIMER; break;
        case 4: timer_periph = BOARD_ENC3_TIMER; break;
        case 5: timer_periph = BOARD_ENC4_TIMER; break;
        default: return 0;
    }

    encoder_value = (int16_t)timer_counter_read(timer_periph);
    timer_counter_value_config(timer_periph, 0);
    return encoder_value;
}
