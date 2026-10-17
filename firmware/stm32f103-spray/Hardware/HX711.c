#include "HX711.h"
#include "Delay.h"

/* 校准参数 */
static int32_t g_Offset = 0;    // 零点偏移值（皮重）
static float   g_Scale = 1.0f;  // 比例系数 (g / 原始值)

/**
  * @brief  HX711初始化GPIO
  * @param  无
  * @retval 无
  * @note   融合版: SCK=PA0, DOUT=PB1 (PA1已给喷雾)
  */
void HX711_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIOA和GPIOB时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    /* SCK引脚配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin  = HX711_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HX711_SCK_PORT, &GPIO_InitStructure);

    /* DOUT引脚配置为浮空输入 */
    GPIO_InitStructure.GPIO_Pin  = HX711_DOUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HX711_DOUT_PORT, &GPIO_InitStructure);

    /* SCK初始化为低电平 */
    HX711_SCK_LOW();
}

/**
  * @brief  读取HX711原始值（24位有符号数）
  * @param  无
  * @retval 24位原始ADC值（int32_t，范围约±8,388,607）
  * @note   使用通道A增益128模式，需要25个时钟脉冲
  */
int32_t HX711_ReadValue(void)
{
    uint32_t data = 0;
    uint8_t i;

    /* 等待DOUT变为低电平（数据就绪） */
    while (HX711_DOUT_READ() == Bit_SET);

    /* 读取24位数据，MSB先 */
    for (i = 0; i < 24; i++)
    {
        HX711_SCK_HIGH();
        Delay_us(1);
        /* 在SCK上升沿读取DOUT */
        if (HX711_DOUT_READ() == Bit_SET)
        {
            data |= (uint32_t)(1UL << (23 - i));
        }
        HX711_SCK_LOW();
        Delay_us(1);
    }

    /* 第25个脉冲：设置下一次转换的通道和增益
       此处选择通道A，增益128（HX711默认） */
    HX711_SCK_HIGH();
    Delay_us(1);
    HX711_SCK_LOW();
    Delay_us(1);

    /* 将24位有符号数转换为32位有符号数 */
    if (data & 0x800000)  // 负数（最高位为1）
    {
        data |= 0xFF000000;  // 符号扩展到32位
    }

    return (int32_t)data;
}

/**
  * @brief  多次读取取平均值
  * @param  times 读取次数
  * @retval 平均值
  */
int32_t HX711_ReadAverage(uint8_t times)
{
    int64_t sum = 0;
    uint8_t i;

    for (i = 0; i < times; i++)
    {
        sum += HX711_ReadValue();
    }

    return (int32_t)(sum / times);
}

/**
  * @brief  去皮（设置零点偏移）
  * @param  times 采样次数（建议10~20次取平均）
  * @retval 无
  */
void HX711_Tare(uint8_t times)
{
    g_Offset = HX711_ReadAverage(times);
}

/**
  * @brief  获取当前重量 (g)
  * @param  无
  * @retval 重量值（单位：克，需要根据实际传感器校准）
  * @note   计算公式: 重量 = (原始值 - 零点偏移) / 比例系数
  */
int32_t HX711_GetWeight(void)
{
    int32_t raw;
    float weight;

    raw = HX711_ReadAverage(5);
    weight = (float)(raw - g_Offset) / g_Scale;

    /* 过滤微小的负值抖动 */
    if (weight < 0.0f && weight > -0.5f)
    {
        weight = 0.0f;
    }

    return (int32_t)weight;
}

/**
  * @brief  设置比例系数
  * @param  scale 比例系数 (原始值/g)
  * @retval 无
  * @note   校准方法：放置已知重量的砝码，读取原始值，
  *          scale = (原始值 - 零点偏移) / 已知重量
  */
void HX711_SetScale(float scale)
{
    g_Scale = scale;
}

/**
  * @brief  获取比例系数
  * @param  无
  * @retval 当前比例系数
  */
float HX711_GetScale(void)
{
    return g_Scale;
}

/**
  * @brief  设置零点偏移
  * @param  offset 零点原始值
  * @retval 无
  */
void HX711_SetOffset(int32_t offset)
{
    g_Offset = offset;
}

/**
  * @brief  获取零点偏移
  * @param  无
  * @retval 当前零点偏移值
  */
int32_t HX711_GetOffset(void)
{
    return g_Offset;
}
