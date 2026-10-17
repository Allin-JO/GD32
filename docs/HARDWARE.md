# 硬件与引脚说明

## 1. GD32F303RCT6 运动主控

| 功能 | 引脚/外设 | 说明 |
|---|---|---|
| 电机 A 编码器 | PA15 / PB3，TIMER1 | 右后轮 |
| 电机 A 方向 | PB5 / PB4 | 正反转控制 |
| 电机 A PWM | PC7，TIMER7 CH1 | 10 kHz |
| 电机 B 编码器 | PB6 / PB7，TIMER3 | 右前轮 |
| 电机 B 方向 | PD2 / PC12 | 正反转控制 |
| 电机 B PWM | PC6，TIMER7 CH0 | 10 kHz |
| 电机 C 编码器 | PA7 / PA6，TIMER2 | 左前轮 |
| 电机 C 方向 | PB8 / PB9 | 正反转控制 |
| 电机 C PWM | PC9，TIMER7 CH3 | 10 kHz |
| 电机 D 编码器 | PA1 / PA0，TIMER4 | 左后轮 |
| 电机 D 方向 | PC13 / PC14 | 正反转控制 |
| 电机 D PWM | PC8，TIMER7 CH2 | 10 kHz |
| 驱动使能 | PC2 | STBY |
| LED | PB14 | 运行心跳 |
| 按键 | PB15 | 调试/方向切换 |
| OLED SCLK | PB13 | 软件 SPI |
| OLED SDIN | PB12 | 软件 SPI |
| OLED RST / RS | PC4 / PA5 | 控制脚 |
| ICM20948 I2C | PB10 / PB11 | 软件 I2C |
| 电压采样 | PC1，ADC0 CH11 | 电池电压分压 |
| 调试串口 | PA9 / PA10，USART0 | 115200-8-N-1 |
| ESP8266 | PA2 / PA3，USART1 | 9600-8-N-1 |
| 蓝牙/备用 | PC10 / PC11，USART2 | 9600-8-N-1，部分重映射 |

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
- GD32 与 STM32 共地；USART 交叉连接 TX/RX。
- 雾化、水泵等执行器建议保留续流/隔离电路。
- 消毒液选用合规产品，结构与密封件需满足对应场景的卫生和安全要求。
