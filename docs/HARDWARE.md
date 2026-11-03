# 硬件与引脚说明

## 1. GD32H759IMT6 运动主控

当前工程以 **GD32H759I-START** 扩展排针为参考板，所有可调整项集中在：

`firmware/gd32h759-main/BOARD/board_h759.h`

| 功能 | 参考引脚/外设 | 说明 |
|---|---|---|
| 调试串口 | PF4 / PF5，USART0，AF4 | 115200-8-N-1 |
| ESP8266 | PA2 / PA3，USART1，AF7 | 9600-8-N-1 |
| 蓝牙/备用 | PC10 / PC11，USART2，AF7 | 9600-8-N-1 |
| PWM A | PC7，TIMER0 CH1，AF1 | 10 kHz |
| PWM B | PC6，TIMER0 CH0，AF1 | 10 kHz |
| PWM C | PC9，TIMER0 CH3，AF1 | 10 kHz |
| PWM D | PC8，TIMER0 CH2，AF1 | 10 kHz |
| 编码器 A | PA15 / PB3，TIMER1 | 正交解码 |
| 编码器 B | PA6 / PA7，TIMER2 | 正交解码 |
| 编码器 C | PB6 / PB7，TIMER3 | 正交解码 |
| 编码器 D | PA0 / PA1，TIMER4 | 正交解码 |
| 电机 A 方向 | PB4 / PB5 | 正反转 |
| 电机 B 方向 | PD2 / PC12 | 正反转 |
| 电机 C 方向 | PB8 / PB9 | 正反转 |
| 电机 D 方向 | PC13 / PC14 | 正反转 |
| 驱动使能 | PE5 | STBY |
| LED | PE0 | 运行心跳 |
| 按键 | PD15 | 用户按键 |
| OLED SCLK | PE2 | 软件 SPI |
| OLED SDIN | PE4 | 软件 SPI |
| OLED RST / RS | PE3 / PE6 | 控制脚 |
| 电压采样 | PC2，ADC2 CH0 | 电池分压 |

> H759 的 AF 编号和 TIMER0 通道映射必须按实际芯片数据手册/开发板原理图复核。

## 2. STM32F103C8T6 喷洒控制板

| 功能 | 引脚/外设 | 说明 |
|---|---|---|
| 总线舵机 | PA9 / PA10，USART1 | 115200，连接 BusLinker/舵机总线 |
| 上位机 | PA2 / PA3，USART2 | 9600，JSON 控制与重量上报 |
| 雾化模块 | PA1 | 高电平开启 |
| 补液泵 | PA4 | 高电平工作，MOS 驱动 |
| HX711 SCK | PA0 | 称重时钟 |
| HX711 DOUT | PB1 | 称重数据 |
| OLED | PB8 / PB9 | I2C/软件 I2C |
| 去皮值存储 | Flash 末页 0x0800FC00 | 掉电保存 |

## 3. 电源与安全

- 电机与舵机使用独立电源，避免启动电流拉低 MCU 电源。
- GD32H759 与 STM32F103 共地；USART 交叉连接 TX/RX。
- 雾化、水泵等执行器建议保留续流/隔离电路。
- 消毒液选用合规产品，结构与密封件需满足对应场景的卫生和安全要求。
