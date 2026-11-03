#ifndef __BOARD_H759_H
#define __BOARD_H759_H

#include "gd32h7xx.h"

/* GD32H759I-START reference mapping. Adjust for a custom H759 board. */
#define BOARD_LED_PORT          GPIOE
#define BOARD_LED_PIN           GPIO_PIN_0
#define BOARD_KEY_PORT          GPIOD
#define BOARD_KEY_PIN           GPIO_PIN_15

#define BOARD_CONSOLE_USART     USART0
#define BOARD_CONSOLE_TX_PORT   GPIOF
#define BOARD_CONSOLE_TX_PIN    GPIO_PIN_4
#define BOARD_CONSOLE_RX_PORT   GPIOF
#define BOARD_CONSOLE_RX_PIN    GPIO_PIN_5
#define BOARD_CONSOLE_AF        GPIO_AF_4

#define BOARD_ESP_USART         USART1
#define BOARD_ESP_TX_PORT       GPIOA
#define BOARD_ESP_TX_PIN        GPIO_PIN_2
#define BOARD_ESP_RX_PORT       GPIOA
#define BOARD_ESP_RX_PIN        GPIO_PIN_3
#define BOARD_ESP_AF            GPIO_AF_7

#define BOARD_BT_USART          USART2
#define BOARD_BT_TX_PORT        GPIOC
#define BOARD_BT_TX_PIN         GPIO_PIN_10
#define BOARD_BT_RX_PORT        GPIOC
#define BOARD_BT_RX_PIN         GPIO_PIN_11
#define BOARD_BT_AF             GPIO_AF_7

#define BOARD_PWM_TIMER         TIMER0
#define BOARD_PWM_PORT          GPIOC
#define BOARD_PWM_AF            GPIO_AF_1
#define BOARD_PWM_CH0_PIN       GPIO_PIN_6
#define BOARD_PWM_CH1_PIN       GPIO_PIN_7
#define BOARD_PWM_CH2_PIN       GPIO_PIN_8
#define BOARD_PWM_CH3_PIN       GPIO_PIN_9

#define BOARD_ENC1_TIMER        TIMER1
#define BOARD_ENC1_PORT_A       GPIOA
#define BOARD_ENC1_PIN_A        GPIO_PIN_15
#define BOARD_ENC1_PORT_B       GPIOB
#define BOARD_ENC1_PIN_B        GPIO_PIN_3
#define BOARD_ENC1_AF           GPIO_AF_1

#define BOARD_ENC2_TIMER        TIMER2
#define BOARD_ENC2_PORT_A       GPIOA
#define BOARD_ENC2_PIN_A        GPIO_PIN_6
#define BOARD_ENC2_PORT_B       GPIOA
#define BOARD_ENC2_PIN_B        GPIO_PIN_7
#define BOARD_ENC2_AF           GPIO_AF_1

#define BOARD_ENC3_TIMER        TIMER3
#define BOARD_ENC3_PORT_A       GPIOB
#define BOARD_ENC3_PIN_A        GPIO_PIN_6
#define BOARD_ENC3_PORT_B       GPIOB
#define BOARD_ENC3_PIN_B        GPIO_PIN_7
#define BOARD_ENC3_AF           GPIO_AF_1

#define BOARD_ENC4_TIMER        TIMER4
#define BOARD_ENC4_PORT_A       GPIOA
#define BOARD_ENC4_PIN_A        GPIO_PIN_0
#define BOARD_ENC4_PORT_B       GPIOA
#define BOARD_ENC4_PIN_B        GPIO_PIN_1
#define BOARD_ENC4_AF           GPIO_AF_1

#define BOARD_MOTOR_A_PORT      GPIOB
#define BOARD_MOTOR_A_IN1_PIN   GPIO_PIN_4
#define BOARD_MOTOR_A_IN2_PIN   GPIO_PIN_5
#define BOARD_MOTOR_B_PORT      GPIOD
#define BOARD_MOTOR_B_IN1_PIN   GPIO_PIN_2
#define BOARD_MOTOR_B_IN2_PORT  GPIOC
#define BOARD_MOTOR_B_IN2_PIN   GPIO_PIN_12
#define BOARD_MOTOR_C_PORT      GPIOB
#define BOARD_MOTOR_C_IN1_PIN   GPIO_PIN_8
#define BOARD_MOTOR_C_IN2_PIN   GPIO_PIN_9
#define BOARD_MOTOR_D_PORT      GPIOC
#define BOARD_MOTOR_D_IN1_PIN   GPIO_PIN_13
#define BOARD_MOTOR_D_IN2_PIN   GPIO_PIN_14
#define BOARD_MOTOR_STBY_PORT   GPIOE
#define BOARD_MOTOR_STBY_PIN    GPIO_PIN_5

#define BOARD_OLED_SCLK_PORT    GPIOE
#define BOARD_OLED_SCLK_PIN     GPIO_PIN_2
#define BOARD_OLED_SDIN_PORT    GPIOE
#define BOARD_OLED_SDIN_PIN     GPIO_PIN_4
#define BOARD_OLED_RST_PORT     GPIOE
#define BOARD_OLED_RST_PIN      GPIO_PIN_3
#define BOARD_OLED_RS_PORT      GPIOE
#define BOARD_OLED_RS_PIN       GPIO_PIN_6

#define BOARD_ADC_PERIPH        ADC2
#define BOARD_ADC_PORT          GPIOC
#define BOARD_ADC_PIN           GPIO_PIN_2
#define BOARD_ADC_CHANNEL       ADC_CHANNEL_0

void board_gpio_output_init(uint32_t port, uint32_t pin);
void board_gpio_input_pullup_init(uint32_t port, uint32_t pin);
void board_gpio_analog_init(uint32_t port, uint32_t pin);
void board_gpio_af_init(uint32_t port, uint32_t pin, uint32_t af);

#endif /* __BOARD_H759_H */
