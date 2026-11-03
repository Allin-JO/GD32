#include "TIM.h"

void TIM0_Init(u16 arr, u16 psc)
{
    timer_parameter_struct time_base;
    timer_oc_parameter_struct oc;

    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_TIMER0);

    board_gpio_af_init(BOARD_PWM_PORT, BOARD_PWM_CH0_PIN, BOARD_PWM_AF);
    board_gpio_af_init(BOARD_PWM_PORT, BOARD_PWM_CH1_PIN, BOARD_PWM_AF);
    board_gpio_af_init(BOARD_PWM_PORT, BOARD_PWM_CH2_PIN, BOARD_PWM_AF);
    board_gpio_af_init(BOARD_PWM_PORT, BOARD_PWM_CH3_PIN, BOARD_PWM_AF);

    timer_deinit(BOARD_PWM_TIMER);
    timer_struct_para_init(&time_base);
    time_base.prescaler = psc;
    time_base.alignedmode = TIMER_COUNTER_EDGE;
    time_base.counterdirection = TIMER_COUNTER_UP;
    time_base.clockdivision = TIMER_CKDIV_DIV1;
    time_base.period = arr;
    time_base.repetitioncounter = 0;
    timer_init(BOARD_PWM_TIMER, &time_base);

    timer_channel_output_struct_para_init(&oc);
    oc.outputstate = TIMER_CCX_ENABLE;
    oc.outputnstate = TIMER_CCXN_DISABLE;
    oc.ocpolarity = TIMER_OC_POLARITY_HIGH;
    oc.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    oc.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    oc.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(BOARD_PWM_TIMER, TIMER_CH_0, &oc);
    timer_channel_output_config(BOARD_PWM_TIMER, TIMER_CH_1, &oc);
    timer_channel_output_config(BOARD_PWM_TIMER, TIMER_CH_2, &oc);
    timer_channel_output_config(BOARD_PWM_TIMER, TIMER_CH_3, &oc);

    timer_channel_output_pulse_value_config(BOARD_PWM_TIMER, TIMER_CH_0, 0);
    timer_channel_output_pulse_value_config(BOARD_PWM_TIMER, TIMER_CH_1, 0);
    timer_channel_output_pulse_value_config(BOARD_PWM_TIMER, TIMER_CH_2, 0);
    timer_channel_output_pulse_value_config(BOARD_PWM_TIMER, TIMER_CH_3, 0);

    timer_channel_output_mode_config(BOARD_PWM_TIMER, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(BOARD_PWM_TIMER, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(BOARD_PWM_TIMER, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(BOARD_PWM_TIMER, TIMER_CH_3, TIMER_OC_MODE_PWM0);

    timer_channel_output_shadow_config(BOARD_PWM_TIMER, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(BOARD_PWM_TIMER, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(BOARD_PWM_TIMER, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(BOARD_PWM_TIMER, TIMER_CH_3, TIMER_OC_SHADOW_ENABLE);

    timer_auto_reload_shadow_enable(BOARD_PWM_TIMER);
    timer_primary_output_config(BOARD_PWM_TIMER, ENABLE);
    timer_enable(BOARD_PWM_TIMER);
}
