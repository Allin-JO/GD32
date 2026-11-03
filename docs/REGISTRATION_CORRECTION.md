# GD32H759 平台说明

## 报名平台

- 运动主控：**GD32H759IMT6**
- 喷洒控制：**STM32F103C8T6**
- 软件平台：**RT-Thread 4.1.1**

## 代码与工程

当前仓库已经包含 GD32H759 主控工程：

- 工程：`firmware/gd32h759-main/MDK-ARM/GD32H759_RTThread.uvprojx`
- MCU：`GD32H759IMT6`（Cortex-M7）
- 固件库：GD32H7xx 标准外设库
- RT-Thread：Cortex-M7 移植层
- 参考板：GD32H759I-START
- 编译结果：**0 errors / 0 warnings**
- Code 43,534 B / RO-data 7,090 B / RW-data 104 B / ZI-data 72,032 B

板级端口、引脚和 AF 映射集中在：

`firmware/gd32h759-main/BOARD/board_h759.h`

如果实际使用自定义 H759 板，只需按原理图修改该文件，并完成 H759 实机上电复测。

## 代码仓库

https://github.com/Allin-JO/GD32
