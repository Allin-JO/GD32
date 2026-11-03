#ifndef __TIM_H
#define __TIM_H

#include "sys.h"

#define PWMA  TIMER_CH1CV(BOARD_PWM_TIMER)
#define PWMB  TIMER_CH0CV(BOARD_PWM_TIMER)
#define PWMC  TIMER_CH3CV(BOARD_PWM_TIMER)
#define PWMD  TIMER_CH2CV(BOARD_PWM_TIMER)

void TIM0_Init(u16 arr, u16 psc);

#endif /* __TIM_H */
