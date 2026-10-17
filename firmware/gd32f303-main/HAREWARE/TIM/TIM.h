#ifndef __TIM_H
#define __TIM_H

#include "sys.h"

/*
 * C12H motor PWM mapping on GD32 TIMER7 (STM32 TIM8 equivalent):
 *   PWMA = CH1, PWMB = CH0, PWMC = CH3, PWMD = CH2
 */
#define PWMA TIMER_CH1CV(TIMER7)
#define PWMB TIMER_CH0CV(TIMER7)
#define PWMC TIMER_CH3CV(TIMER7)
#define PWMD TIMER_CH2CV(TIMER7)

void TIM8_Init(u16 arr, u16 psc);

#endif /* __TIM_H */
