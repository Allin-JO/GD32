#include "motor.h"

// 初始化直流减速电机控制正反转的引脚
void Motor_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    STBY = 1;
}

void Motor_mode(u8 mode) {
    if (mode == 0) {
        AIN1 = 0, AIN2 = 1;
        BIN1 = 1, BIN2 = 0;
        CIN1 = 0, CIN2 = 1;
        DIN1 = 1, DIN2 = 0;
    } else {
        AIN1 = 1, AIN2 = 0;
        BIN1 = 0, BIN2 = 1;
        CIN1 = 1, CIN2 = 0;
        DIN1 = 0, DIN2 = 1;
    }
}
