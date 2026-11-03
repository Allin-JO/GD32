#ifndef __LED_H
#define __LED_H

#include "sys.h"

void LED_Init(void);
void LED_Flash(u8 time);
void LED_Switch(u8 state);
void LED_Toggle(void);

#endif /* __LED_H */
