#include "adc.h"

void ADC1_Init(void) {
    GPIO_InitTypeDef gpio;

    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6); /* 72 MHz / 6 = 12 MHz */

    gpio.GPIO_Pin = GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);

    adc_deinit(ADC0);
    adc_mode_config(ADC_MODE_FREE);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, DISABLE);

    adc_enable(ADC0);
    adc_calibration_enable(ADC0);
}

u16 Get_Adc1(u8 ch) {
    adc_regular_channel_config(ADC0, 0, ch, ADC_SAMPLETIME_239POINT5);
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

    while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET) {
        ;
    }

    return (u16)adc_regular_data_read(ADC0);
}

u16 Get_adc_Average(u8 ch, u8 count) {
    u32 temp_val = 0;
    u8 i;

    for (i = 0; i < count; i++) {
        temp_val += Get_Adc1(ch);
        delay_ms(5);
    }

    return (u16)(temp_val / count);
}
