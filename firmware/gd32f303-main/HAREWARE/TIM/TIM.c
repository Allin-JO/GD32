#include "TIM.h"

/* Four-channel PWM on TIMER7: 72 MHz / 7200 = 10 kHz. */
void TIM8_Init(u16 arr, u16 psc) {
    GPIO_InitTypeDef gpio;
    timer_parameter_struct time_base;
    timer_oc_parameter_struct oc;

    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_TIMER7);

    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);

    timer_deinit(TIMER7);

    time_base.prescaler = psc;
    time_base.alignedmode = TIMER_COUNTER_EDGE;
    time_base.counterdirection = TIMER_COUNTER_UP;
    time_base.clockdivision = TIMER_CKDIV_DIV1;
    time_base.period = arr;
    time_base.repetitioncounter = 0;
    timer_init(TIMER7, &time_base);

    oc.outputstate = TIMER_CCX_ENABLE;
    oc.outputnstate = TIMER_CCXN_DISABLE;
    oc.ocpolarity = TIMER_OC_POLARITY_HIGH;
    oc.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    oc.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    oc.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER7, TIMER_CH_0, &oc);
    timer_channel_output_config(TIMER7, TIMER_CH_1, &oc);
    timer_channel_output_config(TIMER7, TIMER_CH_2, &oc);
    timer_channel_output_config(TIMER7, TIMER_CH_3, &oc);

    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_0, 0);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_1, 0);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, 0);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_3, 0);

    timer_channel_output_mode_config(TIMER7, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_3, TIMER_OC_MODE_PWM0);

    timer_channel_output_shadow_config(TIMER7, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_3, TIMER_OC_SHADOW_ENABLE);

    timer_auto_reload_shadow_enable(TIMER7);
    timer_primary_output_config(TIMER7, ENABLE);
    timer_enable(TIMER7);
}
