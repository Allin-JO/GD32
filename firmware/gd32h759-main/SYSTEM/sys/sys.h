#ifndef __SYS_H
#define __SYS_H

#include "gd32h7xx.h"
#include "board_h759.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

#define Pi                    3.14159265358979f
#define CONTROL_FREQUENCY     100
#define Encoder_precision     1560.0f
#define Wheel_perimeter       (0.075f * Pi)

extern int Count;
extern u16 adc_val;
extern u32 voltage;
extern float Velocity_KP;
extern float Velocity_KI;

#include "delay.h"
#include "adc.h"
#include "KEY.h"
#include "LED.h"
#include "usart_x.h"
#include "TIM.h"
#include "conctrl.h"
#include "oled.h"
#include "motor.h"
#include "Encoder.h"
#include "show.h"
#include "cmd.h"

#endif /* __SYS_H */
