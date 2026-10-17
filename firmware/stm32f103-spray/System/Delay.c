#include "Delay.h"
#include <rtthread.h>

#define DWT_DEMCR (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)

static uint8_t s_dwt_ready = 0;
static uint32_t s_cycles_per_us = 72;

static void Delay_DwtInit(void) {
    if (!s_dwt_ready) {
        DWT_DEMCR |= (1UL << 24);
        DWT_CYCCNT = 0;
        DWT_CTRL |= 1UL;
        s_cycles_per_us = SystemCoreClock / 1000000U;
        s_dwt_ready = 1;
    }
}

uint32_t Delay_GetTick(void) { return (uint32_t)rt_tick_get(); }

void Delay_us(uint32_t us) {
    uint32_t start;
    uint32_t ticks;

    Delay_DwtInit();
    start = DWT_CYCCNT;
    ticks = us * s_cycles_per_us;

    while ((uint32_t)(DWT_CYCCNT - start) < ticks) {
        ;
    }
}

void Delay_ms(uint32_t ms) {
    if (rt_thread_self() != RT_NULL) {
        rt_thread_mdelay((rt_int32_t)ms);
    } else {
        while (ms--) {
            Delay_us(1000);
        }
    }
}

void Delay_s(uint32_t s) {
    while (s--) {
        Delay_ms(1000);
    }
}
