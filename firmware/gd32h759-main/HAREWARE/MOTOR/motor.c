#include "motor.h"

static void motor_pair(uint32_t port1, uint32_t pin1,
                       uint32_t port2, uint32_t pin2,
                       uint8_t reverse)
{
    if (reverse)
    {
        gpio_bit_set(port1, pin1);
        gpio_bit_reset(port2, pin2);
    }
    else
    {
        gpio_bit_reset(port1, pin1);
        gpio_bit_set(port2, pin2);
    }
}

void Motor_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);

    board_gpio_output_init(BOARD_MOTOR_A_PORT, BOARD_MOTOR_A_IN1_PIN | BOARD_MOTOR_A_IN2_PIN);
    board_gpio_output_init(BOARD_MOTOR_B_PORT, BOARD_MOTOR_B_IN1_PIN);
    board_gpio_output_init(BOARD_MOTOR_B_IN2_PORT, BOARD_MOTOR_B_IN2_PIN);
    board_gpio_output_init(BOARD_MOTOR_C_PORT, BOARD_MOTOR_C_IN1_PIN | BOARD_MOTOR_C_IN2_PIN);
    board_gpio_output_init(BOARD_MOTOR_D_PORT, BOARD_MOTOR_D_IN1_PIN | BOARD_MOTOR_D_IN2_PIN);
    board_gpio_output_init(BOARD_MOTOR_STBY_PORT, BOARD_MOTOR_STBY_PIN);

    gpio_bit_set(BOARD_MOTOR_STBY_PORT, BOARD_MOTOR_STBY_PIN);
}

void Motor_mode(u8 mode)
{
    Motor_SetDirection(MOTOR_ID_A, mode);
    Motor_SetDirection(MOTOR_ID_B, mode);
    Motor_SetDirection(MOTOR_ID_C, mode);
    Motor_SetDirection(MOTOR_ID_D, mode);
}

void Motor_SetDirection(u8 motor, u8 reverse)
{
    switch (motor)
    {
        case MOTOR_ID_A:
            motor_pair(BOARD_MOTOR_A_PORT, BOARD_MOTOR_A_IN1_PIN,
                       BOARD_MOTOR_A_PORT, BOARD_MOTOR_A_IN2_PIN, reverse);
            break;
        case MOTOR_ID_B:
            motor_pair(BOARD_MOTOR_B_PORT, BOARD_MOTOR_B_IN1_PIN,
                       BOARD_MOTOR_B_IN2_PORT, BOARD_MOTOR_B_IN2_PIN, reverse);
            break;
        case MOTOR_ID_C:
            motor_pair(BOARD_MOTOR_C_PORT, BOARD_MOTOR_C_IN1_PIN,
                       BOARD_MOTOR_C_PORT, BOARD_MOTOR_C_IN2_PIN, reverse);
            break;
        case MOTOR_ID_D:
            motor_pair(BOARD_MOTOR_D_PORT, BOARD_MOTOR_D_IN1_PIN,
                       BOARD_MOTOR_D_PORT, BOARD_MOTOR_D_IN2_PIN, reverse);
            break;
        default:
            break;
    }
}
