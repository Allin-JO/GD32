#ifndef __HOST_H
#define __HOST_H

#include "stm32f10x.h"

void Host_Init(void);
void Host_IRQHandler(void);
void Host_Process(void);
void Host_SendWeight(int32_t weight);
uint8_t Host_GetControl(void);
uint8_t Host_GetNewCmd(void);
void Host_ClearNewCmd(void);
uint8_t Host_GetTareRequest(void);
void Host_ClearTareRequest(void);

#endif /* __HOST_H */
