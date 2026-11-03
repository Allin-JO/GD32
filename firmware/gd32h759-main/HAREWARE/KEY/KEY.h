#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY_READ()  gpio_input_bit_get(BOARD_KEY_PORT, BOARD_KEY_PIN)

void Key_Init(void);
u8 click(void);
u8 click_N_Double(u8 time);

#endif /* __KEY_H */
