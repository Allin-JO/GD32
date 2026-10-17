#include "show.h"

void show(void) {
    adc_val = Get_adc_Average(11, 1); // 采样1次即可, 减少主循环耗时(原为2次+delay_ms(5)*2)
    voltage = adc_val * 3.3 * 11 * 100 / 4096;
    memset(OLED_GRAM, 0, 128 * 8 * sizeof(u8));
    /*************************第二行************************/
    OLED_ShowString(00, 10, "A:");
    if (MOTOR_A.Encoder > 0)
        OLED_ShowString(18, 10, "+"), OLED_ShowNumber(30, 10, MOTOR_A.Encoder * 1000, 5, 12);
    else
        OLED_ShowString(18, 10, "-"), OLED_ShowNumber(30, 10, -MOTOR_A.Encoder * 1000, 5, 12);

    if (MOTOR_A.Target > 0)
        OLED_ShowString(78, 10, "+"), OLED_ShowNumber(90, 10, MOTOR_A.Target * 1000, 5, 12);
    else
        OLED_ShowString(78, 10, "-"), OLED_ShowNumber(90, 10, -MOTOR_A.Target * 1000, 5, 12);
    /*************************第三行************************/
    OLED_ShowString(00, 20, "B:");
    if (MOTOR_B.Encoder > 0)
        OLED_ShowString(18, 20, "+"), OLED_ShowNumber(30, 20, MOTOR_B.Encoder * 1000, 5, 12);
    else
        OLED_ShowString(18, 20, "-"), OLED_ShowNumber(30, 20, -MOTOR_B.Encoder * 1000, 5, 12);

    if (MOTOR_B.Target > 0)
        OLED_ShowString(78, 20, "+"), OLED_ShowNumber(90, 20, MOTOR_B.Target * 1000, 5, 12);
    else
        OLED_ShowString(78, 20, "-"), OLED_ShowNumber(90, 20, -MOTOR_B.Target * 1000, 5, 12);
    /*************************第四行************************/
    OLED_ShowString(00, 30, "C:");
    if (MOTOR_C.Encoder > 0)
        OLED_ShowString(18, 30, "+"), OLED_ShowNumber(30, 30, MOTOR_C.Encoder * 1000, 5, 12);
    else
        OLED_ShowString(18, 30, "-"), OLED_ShowNumber(30, 30, -MOTOR_C.Encoder * 1000, 5, 12);

    if (MOTOR_C.Target > 0)
        OLED_ShowString(78, 30, "+"), OLED_ShowNumber(90, 30, MOTOR_C.Target * 1000, 5, 12);
    else
        OLED_ShowString(78, 30, "-"), OLED_ShowNumber(90, 30, -MOTOR_C.Target * 1000, 5, 12);
    /*************************第五行************************/
    OLED_ShowString(00, 40, "D:");
    if (MOTOR_D.Encoder > 0)
        OLED_ShowString(18, 40, "+"), OLED_ShowNumber(30, 40, MOTOR_D.Encoder * 1000, 5, 12);
    else
        OLED_ShowString(18, 40, "-"), OLED_ShowNumber(30, 40, -MOTOR_D.Encoder * 1000, 5, 12);

    if (MOTOR_D.Target > 0)
        OLED_ShowString(78, 40, "+"), OLED_ShowNumber(90, 40, MOTOR_D.Target * 1000, 5, 12);
    else
        OLED_ShowString(78, 40, "-"), OLED_ShowNumber(90, 40, -MOTOR_D.Target * 1000, 5, 12);

    /*************************第六行************************/

    OLED_ShowString(54, 50, "Vol:");
    OLED_ShowNumber(84, 50, voltage / 100, 2, 12);
    OLED_ShowString(96, 50, ".");
    OLED_ShowNumber(102, 50, voltage % 100, 2, 12);
    OLED_ShowString(114, 50, "V");

    OLED_Refresh_Gram();
}
