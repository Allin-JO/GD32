#ifndef __PUMP_H
#define __PUMP_H

#include "stm32f10x.h"

/* ============================================================
 *  水泵模块 (MOS管驱动)
 *  PA4 输出: 高电平=工作(抽水), 低电平=关闭
 *  引脚可按实际接线修改
 * ============================================================ */

#define PUMP_PORT   GPIOA
#define PUMP_PIN    GPIO_Pin_4

void Pump_Init(void);
void Pump_ON(void);
void Pump_OFF(void);
uint8_t Pump_GetState(void);   /* 1=工作中, 0=关闭 */

#endif
