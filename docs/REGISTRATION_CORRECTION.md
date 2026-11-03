# 平台信息说明

## 报名平台

- 主控平台：**GD32H759**
- 喷洒控制：**STM32F103C8T6**
- 软件平台：**RT-Thread 4.1.1**

## 当前代码状态

本仓库当前包含 GD32F303RCT6 参考工程和 STM32F103C8T6 喷洒工程。GD32H759 是 Cortex-M7 平台，外设和引脚与 GD32F303 不同，H759 固件需要根据实际开发板型号、原理图和引脚表进行移植。

需要确认的硬件信息：

- GD32H759 开发板型号（GD32H759I-EVAL、GD32H759I-START 或自定义板）
- 4 路电机 PWM 引脚和定时器通道
- 4 路编码器引脚和定时器
- USART0/1/2 引脚
- OLED、ADC、电机方向/使能引脚

确认后可将 `firmware/gd32f303-main/` 迁移为 `firmware/gd32h759-main/`，并同步更新 Keil 工程、RT-Thread 移植层和投稿文档。

## 代码仓库

https://github.com/Allin-JO/GD32
