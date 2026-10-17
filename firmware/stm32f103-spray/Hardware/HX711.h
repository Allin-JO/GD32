#ifndef __HX711_H
#define __HX711_H

#include "stm32f10x.h"

/* ============================================================
 * HX711 引脚定义 (融合版)
 * 注意: 原秤工程 DOUT 在 PA1, 但 PA1 已给喷雾模块使用,
 *       所以 DOUT 改到 PB1, SCK 保持 PA0
 * ============================================================ */
#define HX711_SCK_PORT     GPIOA
#define HX711_SCK_PIN      GPIO_Pin_0
#define HX711_DOUT_PORT    GPIOB
#define HX711_DOUT_PIN     GPIO_Pin_1

/* SCK引脚操作宏 */
#define HX711_SCK_LOW()    GPIO_ResetBits(HX711_SCK_PORT, HX711_SCK_PIN)
#define HX711_SCK_HIGH()   GPIO_SetBits(HX711_SCK_PORT, HX711_SCK_PIN)

/* DOUT引脚读取宏 */
#define HX711_DOUT_READ()  GPIO_ReadInputDataBit(HX711_DOUT_PORT, HX711_DOUT_PIN)

/* 函数声明 */
void HX711_Init(void);
int32_t HX711_ReadValue(void);
int32_t HX711_ReadAverage(uint8_t times);
void HX711_Tare(uint8_t times);
int32_t HX711_GetWeight(void);
void HX711_SetScale(float scale);
float HX711_GetScale(void);
void HX711_SetOffset(int32_t offset);
int32_t HX711_GetOffset(void);

#endif
