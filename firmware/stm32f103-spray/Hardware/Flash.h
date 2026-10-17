#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

/* ============================================================
 *  Flash 存储模块
 *  用于保存 HX711 去皮偏移量(空壶重量), 掉电不丢失
 *  使用 STM32F103C8T6 最后一页 Flash (1KB, 0x0800FC00)
 * ============================================================ */

uint8_t Flash_LoadTareOffset(int32_t *offset);  /* 返回1=有有效去皮值, 0=无 */
void Flash_SaveTareOffset(int32_t offset);

#endif
