#include "Spray.h"

/**
 * @brief  喷雾模块初始化 (PA1 推挽输出, 默认关闭)
 */
void Spray_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 GPIOA 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* PA1 配置为推挽输出 */
	GPIO_InitStructure.GPIO_Pin = SPRAY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPRAY_PORT, &GPIO_InitStructure);

	/* 默认关闭 (低电平) */
	GPIO_ResetBits(SPRAY_PORT, SPRAY_PIN);
}

/**
 * @brief  开启喷雾 (PA1 输出高电平)
 */
void Spray_ON(void)
{
	GPIO_SetBits(SPRAY_PORT, SPRAY_PIN);
}

/**
 * @brief  关闭喷雾 (PA1 输出低电平)
 */
void Spray_OFF(void)
{
	GPIO_ResetBits(SPRAY_PORT, SPRAY_PIN);
}

/**
 * @brief  获取喷雾状态
 * @retval 1=开启, 0=关闭
 */
uint8_t Spray_GetState(void)
{
	return (GPIO_ReadOutputDataBit(SPRAY_PORT, SPRAY_PIN) == Bit_SET) ? 1 : 0;
}
