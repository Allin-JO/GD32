#include "delay.h"
#include <rtthread.h>

#define DWT_DEMCR (*(volatile unsigned int *)0xE000EDFC)
#define DWT_CTRL (*(volatile unsigned int *)0xE0001000)
#define DWT_CYCCNT (*(volatile unsigned int *)0xE0001004)

static u32 fac_us = 72;

void delay_init(u8 SYSCLK) {
    (void)SYSCLK;

    /* Enable the Cortex-M3 DWT cycle counter for short busy-wait delays. */
    DWT_DEMCR |= (1UL << 24);
    DWT_CYCCNT = 0;
    DWT_CTRL |= 1UL;
    fac_us = SystemCoreClock / 1000000U;
}

void delay_us(u32 nus) {
    u32 start = DWT_CYCCNT;
    u32 ticks = nus * fac_us;

    while ((u32)(DWT_CYCCNT - start) < ticks) {
        ;
    }
}

void delay_ms(u16 nms) {
    if (rt_thread_self() != RT_NULL) {
        rt_thread_mdelay((rt_int32_t)nms);
    } else {
        while (nms--) {
            delay_us(1000);
        }
    }
}
