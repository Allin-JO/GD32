#include <rtthread.h>
#include <rthw.h>
#include "board.h"
#include "misc.h"

#define RT_HEAP_SIZE (8 * 1024)

ALIGN(8) static rt_uint8_t s_rt_heap[RT_HEAP_SIZE];

void SysTick_Handler(void) {
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}

void rt_hw_board_init(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* 1 ms system tick */
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

    /* Both USARTs are reserved for the servo bus and host protocol. */
    rt_system_heap_init(s_rt_heap, s_rt_heap + sizeof(s_rt_heap));
}
