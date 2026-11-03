#ifndef __CMD_H
#define __CMD_H

#include "sys.h"
#include <rtthread.h>

#define CAR_STOP 0
#define CAR_FORWARD 1
#define CAR_BACKWARD 2
#define CAR_LEFT 3
#define CAR_RIGHT 4

#define MOTOR_SPEED_DEFAULT 50 /* 0.05 m/s, retained from validated prototype */
#define SPEED_MAX 3000         /* 3.0 m/s upper bound for JSON input */

typedef struct {
    u8 command;
    int speed_mm_s;
} CarState;

void Car_State_Get(u8 *command, int *speed_mm_s);
void Car_State_Set(u8 command, int speed_mm_s);
void Car_Control(u8 command, int speed_mm_s);

#endif /* __CMD_H */
