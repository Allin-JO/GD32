#include "cmd.h"
#include <rthw.h>

/*
 * Motion mapping validated on the four-wheel differential chassis:
 *   A = right rear, B = right front, C = left front, D = left rear
 *   forward : all wheels forward
 *   backward: all wheels reverse
 *   left    : right side forward + left side reverse
 *   right   : right side reverse + left side forward
 */
static volatile CarState s_car_state = {CAR_STOP, MOTOR_SPEED_DEFAULT};

void Car_State_Get(u8 *command, int *speed_mm_s) {
    rt_base_t level = rt_hw_interrupt_disable();
    *command = s_car_state.command;
    *speed_mm_s = s_car_state.speed_mm_s;
    rt_hw_interrupt_enable(level);
}

void Car_State_Set(u8 command, int speed_mm_s) {
    rt_base_t level;

    if (speed_mm_s < 0) {
        speed_mm_s = 0;
    }
    if (speed_mm_s > SPEED_MAX) {
        speed_mm_s = SPEED_MAX;
    }

    level = rt_hw_interrupt_disable();
    s_car_state.command = command;
    s_car_state.speed_mm_s = speed_mm_s;
    rt_hw_interrupt_enable(level);
}

void Car_Control(u8 command, int speed_mm_s) {
    static u8 last_command = 0xFF;
    static int last_speed = -1;
    float v;

    if (speed_mm_s < 0) {
        speed_mm_s = 0;
    }
    if (speed_mm_s > SPEED_MAX) {
        speed_mm_s = SPEED_MAX;
    }
    v = (float)speed_mm_s * 0.001f;

    /* Remove old incremental-PI output when direction or speed changes. */
    if (command != last_command || speed_mm_s != last_speed) {
        Reset_PI();
        last_command = command;
        last_speed = speed_mm_s;
    }

    switch (command) {
    case CAR_FORWARD:
        MOTOR_A.Target = v;
        MOTOR_B.Target = -v;
        MOTOR_C.Target = v;
        MOTOR_D.Target = v;
        break;

    case CAR_BACKWARD:
        MOTOR_A.Target = -v;
        MOTOR_B.Target = v;
        MOTOR_C.Target = -v;
        MOTOR_D.Target = -v;
        break;

    case CAR_LEFT:
        MOTOR_A.Target = v;
        MOTOR_B.Target = -v;
        MOTOR_C.Target = -v;
        MOTOR_D.Target = -v;
        break;

    case CAR_RIGHT:
        MOTOR_A.Target = -v;
        MOTOR_B.Target = v;
        MOTOR_C.Target = v;
        MOTOR_D.Target = v;
        break;

    case CAR_STOP:
    default:
        MOTOR_A.Target = 0.0f;
        MOTOR_B.Target = 0.0f;
        MOTOR_C.Target = 0.0f;
        MOTOR_D.Target = 0.0f;
        break;
    }
}
