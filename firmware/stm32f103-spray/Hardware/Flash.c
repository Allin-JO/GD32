#include "Flash.h"

/* 存储地址: STM32F103C8T6 共64KB Flash, 最后一页(1KB)起始地址 */
#define FLASH_TARE_BASE   0x0800FC00
#define FLASH_TARE_MAGIC  0xA5A5A5A5   /* 魔数, 用于判断是否已写过 */

/**
 * @brief  从Flash读取保存的去皮偏移量
 * @param  offset 输出参数: 去皮偏移量
 * @retval 1=读取成功(之前保存过), 0=Flash中无有效数据
 */
uint8_t Flash_LoadTareOffset(int32_t *offset)
{
	if (*(volatile uint32_t *)FLASH_TARE_BASE == FLASH_TARE_MAGIC)
	{
		*offset = (int32_t)(*(volatile uint32_t *)(FLASH_TARE_BASE + 4));
		return 1;
	}
	return 0;
}

/**
 * @brief  将去皮偏移量写入Flash (会先擦除该页)
 * @param  offset 去皮偏移量
 */
void Flash_SaveTareOffset(int32_t offset)
{
	FLASH_Unlock();
	FLASH_ErasePage(FLASH_TARE_BASE);
	FLASH_ProgramWord(FLASH_TARE_BASE, FLASH_TARE_MAGIC);       /* 魔数 */
	FLASH_ProgramWord(FLASH_TARE_BASE + 4, (uint32_t)offset);   /* 去皮值 */
	FLASH_Lock();
}
