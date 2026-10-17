#include <rtthread.h>
#include <rthw.h>
#include "board.h"
#include "sys.h"
#include "usart_x.h"

#define RT_HEAP_SIZE (24 * 1024)

ALIGN(8) static rt_uint8_t s_rt_heap[RT_HEAP_SIZE];

void SysTick_Handler(void) {
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}

void rt_hw_console_output(const char *str) {
    while (*str) {
        if (*str == '\n') {
            usart1_send('\r');
        }
        usart1_send((u8)*str++);
    }
}

void rt_hw_board_init(void) {
    NVIC_PriorityGroupConfig(NVIC_PRIGROUP_PRE2_SUB2);

    /* USART1 is the debug console: PA9/PA10, 115200-8-N-1 */
    Usart1_Init(115200);

    /* System tick: 1 ms */
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

    /* Dedicated 24 KiB heap keeps the port independent of linker symbols. */
    rt_system_heap_init(s_rt_heap, s_rt_heap + sizeof(s_rt_heap));
}
