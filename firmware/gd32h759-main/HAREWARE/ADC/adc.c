#include "adc.h"

void ADC1_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_ADC2);

    board_gpio_analog_init(BOARD_ADC_PORT, BOARD_ADC_PIN);

    adc_deinit(BOARD_ADC_PERIPH);
    adc_clock_config(BOARD_ADC_PERIPH, ADC_CLK_SYNC_HCLK_DIV6);
    adc_special_function_config(BOARD_ADC_PERIPH, ADC_SCAN_MODE, DISABLE);
    adc_special_function_config(BOARD_ADC_PERIPH, ADC_CONTINUOUS_MODE, DISABLE);
    adc_resolution_config(BOARD_ADC_PERIPH, ADC_RESOLUTION_12B);
    adc_data_alignment_config(BOARD_ADC_PERIPH, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(BOARD_ADC_PERIPH, ADC_REGULAR_CHANNEL, 1);
    adc_external_trigger_config(BOARD_ADC_PERIPH, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    adc_enable(BOARD_ADC_PERIPH);
    delay_ms(1);
    adc_calibration_mode_config(BOARD_ADC_PERIPH, ADC_CALIBRATION_OFFSET);
    adc_calibration_number(BOARD_ADC_PERIPH, ADC_CALIBRATION_NUM1);
    adc_calibration_enable(BOARD_ADC_PERIPH);
}

u16 Get_Adc1(u8 ch)
{
    adc_regular_channel_config(BOARD_ADC_PERIPH, 0, ch, 480);
    adc_software_trigger_enable(BOARD_ADC_PERIPH, ADC_REGULAR_CHANNEL);

    while (adc_flag_get(BOARD_ADC_PERIPH, ADC_FLAG_EOC) == RESET)
    {
        ;
    }

    return (u16)adc_regular_data_read(BOARD_ADC_PERIPH);
}

u16 Get_adc_Average(u8 ch, u8 count)
{
    u32 temp_val = 0;
    u8 i;

    for (i = 0; i < count; i++)
    {
        temp_val += Get_Adc1(ch);
        delay_ms(5);
    }

    return (u16)(temp_val / count);
}
