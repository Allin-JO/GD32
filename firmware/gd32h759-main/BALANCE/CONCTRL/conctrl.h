#ifndef __CONCTRL_H
#define __CONCTRL_H

#include "sys.h"

typedef struct {
    float Encoder;
    float Motor_Pwm;
    float Target;
} Motor_parameter;

typedef struct {
    int A;
    int B;
    int C;
    int D;
} Encoder;

extern Motor_parameter MOTOR_A, MOTOR_B, MOTOR_C, MOTOR_D;
extern Encoder OriginalEncoder;

void Set_Pwm(int motor_a, int motor_b, int motor_c, int motor_d);
void Get_Velocity_Form_Encoder(void);
int Incremental_PI_A(float Encoder, float Target);
int Incremental_PI_B(float Encoder, float Target);
int Incremental_PI_C(float Encoder, float Target);
int Incremental_PI_D(float Encoder, float Target);
void Reset_PI(void);
void Key_Scan(void);
void Motion_Control_Step(void);

#endif /* __CONCTRL_H */
