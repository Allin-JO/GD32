#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define MOTOR_ID_A     0
#define MOTOR_ID_B     1
#define MOTOR_ID_C     2
#define MOTOR_ID_D     3

void Motor_Init(void);
void Motor_mode(u8 mode);
void Motor_SetDirection(u8 motor, u8 reverse);

#endif /* __MOTOR_H */
