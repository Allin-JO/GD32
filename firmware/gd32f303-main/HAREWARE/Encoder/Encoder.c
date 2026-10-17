#include "Encoder.h"

static void Encoder_GPIO_Init(void) {
    GPIO_InitTypeDef gpio;

    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    gpio.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &gpio);
}

static void Encoder_Timer_Init(uint32_t timer_periph) {
    timer_parameter_struct time_base;

    timer_deinit(timer_periph);
    time_base.prescaler = 0;
    time_base.alignedmode = TIMER_COUNTER_EDGE;
    time_base.counterdirection = TIMER_COUNTER_UP;
    time_base.clockdivision = TIMER_CKDIV_DIV1;
    time_base.period = 65535;
    time_base.repetitioncounter = 0;
    timer_init(timer_periph, &time_base);

    timer_quadrature_decoder_mode_config(timer_periph, TIMER_ENCODER_MODE2, TIMER_IC_POLARITY_RISING,
                                         TIMER_IC_POLARITY_RISING);
    timer_counter_value_config(timer_periph, 0);
    timer_enable(timer_periph);
}

void Encoder_TIM2_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_TIMER1);

    gpio_pin_remap_config(GPIO_TIMER1_PARTIAL_REMAP1, ENABLE);
    Encoder_GPIO_Init();
    Encoder_Timer_Init(TIMER1);
}

void Encoder_TIM3_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER2);

    Encoder_GPIO_Init();
    Encoder_Timer_Init(TIMER2);
}

void Encoder_TIM4_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_TIMER3);

    Encoder_GPIO_Init();
    Encoder_Timer_Init(TIMER3);
}

void Encoder_TIM5_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER4);

    Encoder_GPIO_Init();
    Encoder_Timer_Init(TIMER4);
}

int Read_Encoder(u8 TIMX) {
    int encoder_value;

    switch (TIMX) {
    case 2:
        encoder_value = (int16_t)timer_counter_read(TIMER1);
        timer_counter_value_config(TIMER1, 0);
        break;
    case 3:
        encoder_value = (int16_t)timer_counter_read(TIMER2);
        timer_counter_value_config(TIMER2, 0);
        break;
    case 4:
        encoder_value = (int16_t)timer_counter_read(TIMER3);
        timer_counter_value_config(TIMER3, 0);
        break;
    case 5:
        encoder_value = (int16_t)timer_counter_read(TIMER4);
        timer_counter_value_config(TIMER4, 0);
        break;
    default:
        encoder_value = 0;
        break;
    }

    return encoder_value;
}
