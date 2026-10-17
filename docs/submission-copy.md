# 净巡卫士——基于 GD32 + RT-Thread 的公共区域智能消杀移动装置

## 一句话简介

面向教室、楼道、公共座椅等中小型公共区域的低成本智能消杀小车，采用 GD32F303RCT6 运动主控 + STM32F103C8T6 喷洒控制 + RT-Thread 架构，实现四轮差速移动、机械臂雾化喷洒、消毒液余量监测、远程遥控、预约消杀与视频回传。

## 项目简介（可直接用于网站“项目介绍”栏）

“净巡卫士”是一套面向教室、楼道、公共座椅等中小型公共区域的智能消杀移动装置。系统由 GD32F303RCT6 运动主控板、STM32F103C8T6 喷洒控制板、ESP8266 通信桥、云服务、Qt 管理端和 Web 用户端/管理端组成，支持四轮差速移动、双总线舵机喷头调节、雾化喷洒、补液泵、HX711 余量监测、远程遥控、预约消杀、状态上报和视频回传。下位机固件基于 RT-Thread 4.1.1 重构：GD32 运动主控板以 10 ms 绝对周期线程执行四轮编码电机增量 PI 闭环，STM32 喷洒控制板以非阻塞状态机协调舵机、雾化、水泵和称重；串口中断只负责收字节与释放信号量，JSON 解析、称重和显示分别在独立线程运行。迁移后外设引脚、MQTT 主题和 JSON 协议保持兼容，GD32 与 STM32 双板工程均通过 Keil 编译验证，为后续上电复测、导航避障和产品化迭代提供了可扩展的实时软件基础。

## 技术亮点

- **GD32 国产主控 + RT-Thread**：主控板由 STM32F103RCT6 迁移为 GD32F303RCT6，使用 GD32F30x 标准外设库和 RT-Thread Cortex-M4 端口。
- **双板任务化架构**：GD32 运动主控与 STM32 喷洒执行分别使用独立线程和统一 1 ms 时基，避免单板任务互相阻塞。
- **10 ms 绝对周期控制**：使用 `rt_thread_delay_until()` 保持固定节拍，避免执行时间累积造成周期漂移。
- **事件驱动命令链路**：USART 中断只组帧并释放信号量，JSON 解析在线程中完成，降低中断负担。
- **非阻塞喷洒状态机**：舵机到位等待、雾化启停、补液周期和称重上报并行运行，解决原程序中 1.5 s 阻塞延时问题。
- **余量监测与掉电保护**：HX711 实时称重，去皮值写入 Flash，支持余量不足提示与周期补液。
- **云边端业务闭环**：ESP8266 + MQTT + 云服务 + PC/Web 多端，支持预约、执行、状态、视频和任务归档。

## GD32 迁移说明

- 主控 MCU：`GD32F303RCT6`，与 `STM32F103RCT6` 引脚兼容。
- 固件库：`GD32F30x` 标准外设库。
- RT-Thread 端口：`Cortex-M4`。
- 当前主频：`72 MHz` 兼容模式，保持原 `10 kHz` PWM 频率和 PI 参数；GD32F303 硬件支持更高主频，后续可在完成时序与电机参数复测后提升。
- 外设映射：`TIM2/3/4/5/8 → TIMER1/2/3/4/7`，`USART1/2/3 → USART0/1/2`，`ADC1 → ADC0`。
- 外设引脚、MQTT 主题和 JSON 协议保持不变。

## RT-Thread 使用说明

- 版本：RT-Thread 4.1.1。
- 系统节拍：1000 Hz（1 ms）。
- GD32 主控线程：`motion`（优先级 8，10 ms）、`command`（优先级 9，事件驱动）、`display`（优先级 11，100 ms）。
- STM32 喷洒板线程：`command`（优先级 7）、`control`（优先级 8，10 ms）、`weight`（优先级 10，3 s/去皮请求）、`display`（优先级 11，100 ms）。
- 关键 API：`rt_thread_create`、`rt_thread_startup`、`rt_thread_delay_until`、`rt_sem_init`、`rt_sem_take`、`rt_sem_release`、`rt_tick_get`、`rt_system_heap_init`、`rt_interrupt_enter/leave`。

## 硬件组成

- 运动主控板：GD32F303RCT6（C12H 底盘兼容）。
- 喷洒控制板：STM32F103C8T6 最小系统板。
- 驱动电机：JGA25-370 编码直流电机 ×4。
- 总线舵机：ZP10S ×2。
- 称重：HX711 + 应变式传感器。
- 无线：NodeMCU ESP8266。
- 视觉：K230。
- 执行：超声波雾化模块、补液水泵。

## 通信协议

运动指令：

```json
{"command":"forward"}
{"command":"backward"}
{"command":"left"}
{"command":"right"}
{"command":"stop"}
{"command":"forward","speed":2000}
{"speed":20}
```

喷洒控制：

```json
{"control":1}
{"control":0}
{"tare":1}
{"weight":1234}
```

MQTT 主题：`spray/weight`、`spray/control`、`device/wdlmj/control`。

## 验证状态

- GD32F303 主控板：Keil ARMCC V5.06 编译通过，0 错误 / 0 警告。
- STM32F103 喷洒板：Keil ARMCC V5.06 编译通过，0 错误 / 1 条 RT-Thread 移植层提示。
- 外设引脚、MQTT 主题与 JSON 协议保持不变。
- 迁移后的整机时序建议按投稿文档第 6.2 节完成上电复测。

## 关键词

GD32F303；RT-Thread；智能消杀；STM32F103；物联网；增量 PI；任务调度；公共区域；云边端协同
