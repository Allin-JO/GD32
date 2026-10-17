#include "sys.h"

void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset) { SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80); }

void WFI_SET(void) { __ASM volatile("wfi"); }

void INTX_DISABLE(void) { __ASM volatile("cpsid i"); }

void INTX_ENABLE(void) { __ASM volatile("cpsie i"); }

void JTAG_Set(u8 mode) {
    rcu_periph_clock_enable(RCU_AF);

    if (mode == 2) {
        gpio_pin_remap_config(GPIO_SWJ_DISABLE_REMAP, ENABLE);
    } else if (mode == 1) {
        gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
    }
}

void RCC_APB2PeriphClockCmd(uint32_t periph, FunctionalState state) {
    if (state != DISABLE) {
        rcu_periph_clock_enable((rcu_periph_enum)periph);
    } else {
        rcu_periph_clock_disable((rcu_periph_enum)periph);
    }
}

void RCC_APB1PeriphClockCmd(uint32_t periph, FunctionalState state) {
    if (state != DISABLE) {
        rcu_periph_clock_enable((rcu_periph_enum)periph);
    } else {
        rcu_periph_clock_disable((rcu_periph_enum)periph);
    }
}

void RCC_ADCCLKConfig(uint32_t adc_psc) { rcu_adc_clock_config(adc_psc); }

void GPIO_Init(uint32_t gpio_periph, GPIO_InitTypeDef *init) {
    uint32_t mode;
    uint32_t speed;

    switch (init->GPIO_Mode) {
    case GPIO_Mode_AIN:
        mode = GPIO_MODE_AIN;
        break;
    case GPIO_Mode_IN_FLOATING:
        mode = GPIO_MODE_IN_FLOATING;
        break;
    case GPIO_Mode_IPD:
        mode = GPIO_MODE_IPD;
        break;
    case GPIO_Mode_IPU:
        mode = GPIO_MODE_IPU;
        break;
    case GPIO_Mode_Out_OD:
        mode = GPIO_MODE_OUT_OD;
        break;
    case GPIO_Mode_Out_PP:
        mode = GPIO_MODE_OUT_PP;
        break;
    case GPIO_Mode_AF_OD:
        mode = GPIO_MODE_AF_OD;
        break;
    case GPIO_Mode_AF_PP:
        mode = GPIO_MODE_AF_PP;
        break;
    default:
        mode = GPIO_MODE_IN_FLOATING;
        break;
    }

    switch (init->GPIO_Speed) {
    case GPIO_Speed_10MHz:
        speed = GPIO_OSPEED_10MHZ;
        break;
    case GPIO_Speed_2MHz:
        speed = GPIO_OSPEED_2MHZ;
        break;
    default:
        speed = GPIO_OSPEED_50MHZ;
        break;
    }

    gpio_init(gpio_periph, mode, speed, init->GPIO_Pin);
}

void GPIO_PinRemapConfig(uint32_t remap, FunctionalState state) {
    gpio_pin_remap_config(remap, (state != DISABLE) ? ENABLE : DISABLE);
}

void NVIC_PriorityGroupConfig(uint32_t group) { nvic_priority_group_set(group); }

void NVIC_Init(NVIC_InitTypeDef *init) {
    if (init->NVIC_IRQChannelCmd != DISABLE) {
        nvic_irq_enable(init->NVIC_IRQChannel, init->NVIC_IRQChannelPreemptionPriority,
                        init->NVIC_IRQChannelSubPriority);
    } else {
        nvic_irq_disable(init->NVIC_IRQChannel);
    }
}
