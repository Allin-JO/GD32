#ifndef __SPRAY_H
#define __SPRAY_H

#include "stm32f10x.h"

/* ============================================================
 *  喷雾模块控制
 *  PA1 输出: 高电平 = 喷雾开启, 低电平 = 喷雾关闭
 *  (注意: PA1 原本是 LED1, 现在用于喷雾, 不要再调用 LED_Init)
 * ============================================================ */

#define SPRAY_PORT   GPIOA
#define SPRAY_PIN    GPIO_Pin_1

void Spray_Init(void);
void Spray_ON(void);
void Spray_OFF(void);
uint8_t Spray_GetState(void);   /* 1=开启, 0=关闭 */

#endif
