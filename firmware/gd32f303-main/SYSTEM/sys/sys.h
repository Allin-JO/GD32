#ifndef __SYS_H
#define __SYS_H
#include "stm32f10x.h"
/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V1.0
修改时间：2023-01-04

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update：2023-01-04

All rights reserved
***********************************************/

// 0,不支持ucos
// 1,支持ucos
#define SYSTEM_SUPPORT_OS 0 // 定义系统文件夹是否支持UCOS

// 位带操作,实现51类似的GPIO控制功能
// 具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
// IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 12) // 0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE + 12) // 0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE + 12) // 0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE + 12) // 0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 8) // 0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE + 8) // 0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE + 8) // 0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE + 8) // 0x40011E08

// IO口操作,只对单一的IO口!
// 确保n的值小于16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)  // 输入

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)  // 输入

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)  // 输入

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // 输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)  // 输入

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) // 输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)  // 输入

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) // 输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)  // 输入

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) // 输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)  // 输入
/////////////////////////////////////////////////////////////////////////////
// Ex_NVIC_Config专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6
#define SERVO_INIT 1500
#define FTIR 1 // 下降沿触发
#define RTIR 2 // 上升沿触发

// 圆周率
#define Pi 3.14159265358979f
// Encoder data reading frequency
// 编码器数据读取频率
#define CONTROL_FREQUENCY 100
/**********************************电机相关参数********************************/
// The encoder octave depends on the encoder initialization Settings
// 编码器倍频数，取决于编码器初始化设置
#define EncoderMultiples 4
// Motor_gear_ratio
// 电机减速比
#define HALL_30F 30
// Number_of_encoder_lines
// 编码器精度
#define Hall_13 13
//
// 霍尔电机每转动一圈能读取到的读数
#define Encoder_precision 1560.0f

// Mecanum wheel tire diameter series
// 麦轮轮胎直径
#define Mecanum_75 0.075
//
// 麦轮周长（直径*Pi）
#define Wheel_perimeter Mecanum_75 *Pi

#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "KEY.h"
#include "LED.h"
#include "usart_x.h"
#include "TIM.h"
#include "I2C.h"
#include "ICM20948.h"
#include "conctrl.h"
#include "oled.h"
#include "motor.h"
#include "Encoder.h"
#include "show.h"
#include "cmd.h"

extern int Count;
extern u16 adc_val;
extern u32 voltage;
extern int encoder_A, encoder_B, encoder_C, encoder_D;
extern float Velocity_KP, Velocity_KI;

// JTAG模式设置定义
#define JTAG_SWD_DISABLE 0X02
#define SWD_ENABLE 0X01
#define JTAG_SWD_ENABLE 0X00

/////////////////////////////////////////////////////////////////
void Stm32_Clock_Init(u8 PLL);                                                                      // 时钟初始化
void Sys_Soft_Reset(void);                                                                          // 系统软复位
void Sys_Standby(void);                                                                             // 待机模式
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);                                          // 设置偏移地址
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);                                                    // 设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group); // 设置中断
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM); // 外部中断配置函数(只对GPIOA~G)
void JTAG_Set(u8 mode);
//////////////////////////////////////////////////////////////////////////////
// 以下为汇编函数
void WFI_SET(void);      // 执行WFI指令
void INTX_DISABLE(void); // 关闭所有中断
void INTX_ENABLE(void);  // 开启所有中断
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#endif
