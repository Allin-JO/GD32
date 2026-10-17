#ifndef __BOARD_H__
#define __BOARD_H__

#include <stm32f10x.h>

#define STM32_SRAM_SIZE (48)
#define STM32_SRAM_END (0x20000000 + STM32_SRAM_SIZE * 1024)

void rt_hw_board_init(void);

#endif /* __BOARD_H__ */
