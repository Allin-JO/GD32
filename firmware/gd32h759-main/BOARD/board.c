#include <rtthread.h>
#include <rthw.h>
#include "board_h759.h"
#include "sys.h"
#include "usart_x.h"

#define RT_HEAP_SIZE    (64 * 1024)

ALIGN(8) static rt_uint8_t s_rt_heap[RT_HEAP_SIZE];

void SysTick_Handler(void)
{
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}

void rt_hw_console_output(const char *str)
{
    while (*str)
    {
        if (*str == '\n')
        {
            usart1_send('\r');
        }
        usart1_send((u8)*str++);
    }
}

void rt_hw_board_init(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    /* Cortex-M7 caches must be enabled before normal application execution. */
    SCB_EnableICache();
    SCB_EnableDCache();

    Usart1_Init(115200);

    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

    rt_system_heap_init(s_rt_heap, s_rt_heap + sizeof(s_rt_heap));
}
