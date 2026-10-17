#include "conctrl.h"

Motor_parameter MOTOR_A, MOTOR_B, MOTOR_C, MOTOR_D;
Encoder OriginalEncoder;

static float PI_Bias_A = 0, PI_Pwm_A = 0, PI_Last_A = 0;
static float PI_Bias_B = 0, PI_Pwm_B = 0, PI_Last_B = 0;
static float PI_Bias_C = 0, PI_Pwm_C = 0, PI_Last_C = 0;
static float PI_Bias_D = 0, PI_Pwm_D = 0, PI_Last_D = 0;

void Set_Pwm(int motor_a, int motor_b, int motor_c, int motor_d) {
    if (motor_a < 0) {
        AIN1 = 1;
        AIN2 = 0;
        PWMA = -motor_a;
    } else {
        AIN1 = 0;
        AIN2 = 1;
        PWMA = motor_a;
    }

    if (motor_b < 0) {
        BIN1 = 1;
        BIN2 = 0;
        PWMB = -motor_b;
    } else {
        BIN1 = 0;
        BIN2 = 1;
        PWMB = motor_b;
    }

    if (motor_c < 0) {
        CIN1 = 1;
        CIN2 = 0;
        PWMC = -motor_c;
    } else {
        CIN1 = 0;
        CIN2 = 1;
        PWMC = motor_c;
    }

    if (motor_d < 0) {
        DIN1 = 1;
        DIN2 = 0;
        PWMD = -motor_d;
    } else {
        DIN1 = 0;
        DIN2 = 1;
        PWMD = motor_d;
    }
}

void Get_Velocity_Form_Encoder(void) {
    int encoder_a, encoder_b, encoder_c, encoder_d;

    OriginalEncoder.A = Read_Encoder(2);
    OriginalEncoder.B = Read_Encoder(4);
    OriginalEncoder.C = Read_Encoder(3);
    OriginalEncoder.D = Read_Encoder(5);

    encoder_a = -OriginalEncoder.A;
    encoder_b = OriginalEncoder.B;
    encoder_c = -OriginalEncoder.C;
    encoder_d = -OriginalEncoder.D;

    MOTOR_A.Encoder = encoder_a * CONTROL_FREQUENCY * Wheel_perimeter / Encoder_precision;
    MOTOR_B.Encoder = encoder_b * CONTROL_FREQUENCY * Wheel_perimeter / Encoder_precision;
    MOTOR_C.Encoder = encoder_c * CONTROL_FREQUENCY * Wheel_perimeter / Encoder_precision;
    MOTOR_D.Encoder = encoder_d * CONTROL_FREQUENCY * Wheel_perimeter / Encoder_precision;
}

void Reset_PI(void) {
    PI_Bias_A = 0;
    PI_Pwm_A = 0;
    PI_Last_A = 0;
    PI_Bias_B = 0;
    PI_Pwm_B = 0;
    PI_Last_B = 0;
    PI_Bias_C = 0;
    PI_Pwm_C = 0;
    PI_Last_C = 0;
    PI_Bias_D = 0;
    PI_Pwm_D = 0;
    PI_Last_D = 0;
}

int Incremental_PI_A(float encoder, float target) {
    PI_Bias_A = target - encoder;
    PI_Pwm_A += Velocity_KP * (PI_Bias_A - PI_Last_A) + Velocity_KI * PI_Bias_A;
    if (PI_Pwm_A > 7200) PI_Pwm_A = 7200;
    if (PI_Pwm_A < -7200) PI_Pwm_A = -7200;
    PI_Last_A = PI_Bias_A;
    return (int)PI_Pwm_A;
}

int Incremental_PI_B(float encoder, float target) {
    PI_Bias_B = target - encoder;
    PI_Pwm_B += Velocity_KP * (PI_Bias_B - PI_Last_B) + Velocity_KI * PI_Bias_B;
    if (PI_Pwm_B > 7200) PI_Pwm_B = 7200;
    if (PI_Pwm_B < -7200) PI_Pwm_B = -7200;
    PI_Last_B = PI_Bias_B;
    return (int)PI_Pwm_B;
}

int Incremental_PI_C(float encoder, float target) {
    PI_Bias_C = target - encoder;
    PI_Pwm_C += Velocity_KP * (PI_Bias_C - PI_Last_C) + Velocity_KI * PI_Bias_C;
    if (PI_Pwm_C > 7200) PI_Pwm_C = 7200;
    if (PI_Pwm_C < -7200) PI_Pwm_C = -7200;
    PI_Last_C = PI_Bias_C;
    return (int)PI_Pwm_C;
}

int Incremental_PI_D(float encoder, float target) {
    PI_Bias_D = target - encoder;
    PI_Pwm_D += Velocity_KP * (PI_Bias_D - PI_Last_D) + Velocity_KI * PI_Bias_D;
    if (PI_Pwm_D > 7200) PI_Pwm_D = 7200;
    if (PI_Pwm_D < -7200) PI_Pwm_D = -7200;
    PI_Last_D = PI_Bias_D;
    return (int)PI_Pwm_D;
}

void Key_Scan(void) {
    int key;

    key = click_N_Double(50);
    if (key == 1) {
        if (MOTOR_A.Target >= 0) {
            MOTOR_A.Target = -0.5f;
            MOTOR_B.Target = -0.8f;
            MOTOR_C.Target = -1.1f;
            MOTOR_D.Target = -1.4f;
        } else {
            MOTOR_A.Target = 0.5f;
            MOTOR_B.Target = 0.8f;
            MOTOR_C.Target = 1.1f;
            MOTOR_D.Target = 1.4f;
        }
    }
}

/* One 10 ms speed-control cycle, executed by the RT-Thread motion thread. */
void Motion_Control_Step(void) {
    if (++Count >= 50) {
        Count = 0;
        LED = ~LED; /* 500 ms heartbeat */
    }

    Key_Scan();
    Get_Velocity_Form_Encoder();

    MOTOR_A.Motor_Pwm = Incremental_PI_A(MOTOR_A.Encoder, MOTOR_A.Target);
    MOTOR_B.Motor_Pwm = Incremental_PI_B(MOTOR_B.Encoder, MOTOR_B.Target);
    MOTOR_C.Motor_Pwm = Incremental_PI_C(MOTOR_C.Encoder, MOTOR_C.Target);
    MOTOR_D.Motor_Pwm = Incremental_PI_D(MOTOR_D.Encoder, MOTOR_D.Target);

    Set_Pwm((int)MOTOR_A.Motor_Pwm, (int)MOTOR_B.Motor_Pwm, (int)MOTOR_C.Motor_Pwm, (int)MOTOR_D.Motor_Pwm);
}
