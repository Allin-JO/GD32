#include "Pump.h"

/**
 * @brief  水泵模块初始化 (PA4 推挽输出, 默认关闭)
 */
void Pump_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 GPIOA 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* PA4 配置为推挽输出 */
	GPIO_InitStructure.GPIO_Pin = PUMP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PUMP_PORT, &GPIO_InitStructure);

	/* 默认关闭 (低电平) */
	GPIO_ResetBits(PUMP_PORT, PUMP_PIN);
}

/**
 * @brief  开启水泵 (PA4 输出高电平)
 */
void Pump_ON(void)
{
	GPIO_SetBits(PUMP_PORT, PUMP_PIN);
}

/**
 * @brief  关闭水泵 (PA4 输出低电平)
 */
void Pump_OFF(void)
{
	GPIO_ResetBits(PUMP_PORT, PUMP_PIN);
}

/**
 * @brief  获取水泵状态
 * @retval 1=工作中, 0=关闭
 */
uint8_t Pump_GetState(void)
{
	return (GPIO_ReadOutputDataBit(PUMP_PORT, PUMP_PIN) == Bit_SET) ? 1 : 0;
}
