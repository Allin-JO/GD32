#ifndef __STM32F10X_H
#define __STM32F10X_H

/*
 * GD32F303RCT6 compatibility header.
 * Keeps the original WHEELTEC application sources close to unchanged while
 * compiling against the GigaDevice GD32F30x standard peripheral library.
 */

#include "gd32f30x.h"
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

typedef volatile uint8_t  vu8;
typedef volatile uint16_t vu16;
typedef volatile uint32_t vu32;

typedef ControlStatus FunctionalState;
typedef ControlStatus ITStatus;

typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} BitAction;

/* Dummy legacy types referenced by unused ICM20948 declarations. */
typedef uint32_t GPIO_TypeDef;
typedef uint32_t SPI_TypeDef;

typedef enum
{
    GPIO_Speed_10MHz = 1,
    GPIO_Speed_2MHz,
    GPIO_Speed_50MHz
} GPIOSpeed_TypeDef;

typedef enum
{
    GPIO_Mode_AIN = 0,
    GPIO_Mode_IN_FLOATING,
    GPIO_Mode_IPD,
    GPIO_Mode_IPU,
    GPIO_Mode_Out_OD,
    GPIO_Mode_Out_PP,
    GPIO_Mode_AF_OD,
    GPIO_Mode_AF_PP
} GPIOMode_TypeDef;

typedef struct
{
    uint16_t GPIO_Pin;
    GPIOSpeed_TypeDef GPIO_Speed;
    GPIOMode_TypeDef GPIO_Mode;
} GPIO_InitTypeDef;

typedef struct
{
    uint16_t SPI_Direction;
    uint16_t SPI_Mode;
    uint16_t SPI_DataSize;
    uint16_t SPI_CPOL;
    uint16_t SPI_CPHA;
    uint16_t SPI_NSS;
    uint16_t SPI_BaudRatePrescaler;
    uint16_t SPI_FirstBit;
    uint16_t SPI_CRCPolynomial;
} SPI_InitTypeDef;

typedef struct
{
    uint8_t NVIC_IRQChannel;
    uint8_t NVIC_IRQChannelPreemptionPriority;
    uint8_t NVIC_IRQChannelSubPriority;
    FunctionalState NVIC_IRQChannelCmd;
} NVIC_InitTypeDef;

/* GPIO port base addresses used by the original bit-band macros. */
#define GPIOA_BASE  GPIOA
#define GPIOB_BASE  GPIOB
#define GPIOC_BASE  GPIOC
#define GPIOD_BASE  GPIOD
#define GPIOE_BASE  GPIOE
#define GPIOF_BASE  GPIOF
#define GPIOG_BASE  GPIOG

/* Legacy GPIO pin names. */
#define GPIO_Pin_0   GPIO_PIN_0
#define GPIO_Pin_1   GPIO_PIN_1
#define GPIO_Pin_2   GPIO_PIN_2
#define GPIO_Pin_3   GPIO_PIN_3
#define GPIO_Pin_4   GPIO_PIN_4
#define GPIO_Pin_5   GPIO_PIN_5
#define GPIO_Pin_6   GPIO_PIN_6
#define GPIO_Pin_7   GPIO_PIN_7
#define GPIO_Pin_8   GPIO_PIN_8
#define GPIO_Pin_9   GPIO_PIN_9
#define GPIO_Pin_10  GPIO_PIN_10
#define GPIO_Pin_11  GPIO_PIN_11
#define GPIO_Pin_12  GPIO_PIN_12
#define GPIO_Pin_13  GPIO_PIN_13
#define GPIO_Pin_14  GPIO_PIN_14
#define GPIO_Pin_15  GPIO_PIN_15

/* Legacy RCU peripheral aliases. */
#define RCC_APB2Periph_GPIOA   RCU_GPIOA
#define RCC_APB2Periph_GPIOB   RCU_GPIOB
#define RCC_APB2Periph_GPIOC   RCU_GPIOC
#define RCC_APB2Periph_GPIOD   RCU_GPIOD
#define RCC_APB2Periph_GPIOE   RCU_GPIOE
#define RCC_APB2Periph_AFIO    RCU_AF
#define RCC_APB2Periph_USART1  RCU_USART0
#define RCC_APB2Periph_ADC1    RCU_ADC0
#define RCC_APB2Periph_TIM8    RCU_TIMER7
#define RCC_APB1Periph_USART2  RCU_USART1
#define RCC_APB1Periph_USART3  RCU_USART2
#define RCC_APB1Periph_TIM2    RCU_TIMER1
#define RCC_APB1Periph_TIM3    RCU_TIMER2
#define RCC_APB1Periph_TIM4    RCU_TIMER3
#define RCC_APB1Periph_TIM5    RCU_TIMER4
#define RCC_PCLK2_Div6          RCU_CKADC_CKAPB2_DIV6

/* Legacy remap names used by the original sources. */
#define GPIO_PartialRemap_USART3  GPIO_USART2_PARTIAL_REMAP
#define GPIO_PartialRemap1_TIM2   GPIO_TIMER1_PARTIAL_REMAP1

void RCC_APB2PeriphClockCmd(uint32_t periph, FunctionalState state);
void RCC_APB1PeriphClockCmd(uint32_t periph, FunctionalState state);
void RCC_ADCCLKConfig(uint32_t adc_psc);
void GPIO_Init(uint32_t gpio_periph, GPIO_InitTypeDef *init);
void GPIO_PinRemapConfig(uint32_t remap, FunctionalState state);
void NVIC_PriorityGroupConfig(uint32_t group);
void NVIC_Init(NVIC_InitTypeDef *init);

#endif /* __STM32F10X_H */

