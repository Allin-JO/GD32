/**
 ******************************************************************************
 * @file    Hardware/Servo.c
 * @brief   总线舵机驱动 (基于STM32F103C8T6标准库)
 *
 * @note    通信协议: UART 半双工单线, 115200bps, 8N1
 *           数据帧: 0x55 0x55 ID Length Cmd [Params...] Checksum
 *           Checksum = ~(ID + Length + Cmd + Prm1 + ... + PrmN) 取低字节
 *
 * 硬件连接 (通过幻尔 BusLinker 调试板):
 *   STM32 PA9 (USART1_TX) ──→ BusLinker RX
 *   STM32 PA10(USART1_RX) ──→ BusLinker TX
 *   STM32 GND               ──→ BusLinker GND
 *   BusLinker 舵机接口       ──→ 总线舵机
 *
 * 供电说明:
 *   BusLinker 有独立电源输入口(单独供电)
 *   BusLinker 的 5V 引脚是输出(给 STM32 供电用, 可选, 不需要从32给5V)
 *   舵机需外部电源供电(6-8.4V), 通过 BusLinker 舵机接口提供
 *
 *   舵机ID#1: 位置范围 0~350
 *   舵机ID#2: 位置范围 0~800
 ******************************************************************************
 */

#include "Servo.h"
#include "Delay.h"

/* 内部函数声明 */
static void SERVO_SendByte(uint8_t byte);
static uint8_t SERVO_CalcChecksum(uint8_t *data, uint8_t len);
static void SERVO_SendPacket(uint8_t *data, uint8_t len);

/**
 * @brief  舵机初始化 (USART1 全双工模式)
 *         PA9:  USART1_TX  → BusLinker RX
 *         PA10: USART1_RX  → BusLinker TX
 *         BusLinker 内部处理半双工转换, STM32 端用全双工即可
 *         波特率: 115200, 8数据位, 1停止位, 无校验
 */
void SERVO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 和 GPIOA 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* PA9 配置为复用推挽输出 (USART1_TX) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PA10 配置为浮空输入 (USART1_RX) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置 USART1 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);

	/* 使能 USART1 */
	USART_Cmd(USART1, ENABLE);
}

/**
 * @brief  发送一个字节
 */
static void SERVO_SendByte(uint8_t byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, byte);
}

/**
 * @brief  计算校验和
 *         Checksum = ~(所有数据字节累加和的低8位)
 */
static uint8_t SERVO_CalcChecksum(uint8_t *data, uint8_t len)
{
	uint16_t sum = 0;
	uint8_t i;
	for (i = 0; i < len; i++)
	{
		sum += data[i];
	}
	return ~((uint8_t)(sum & 0xFF));
}

/**
 * @brief  发送一帧完整的舵机指令
 * @param  data: 数据缓冲区 (ID + Length + Cmd + Params)
 * @param  len:  数据长度 (不含帧头, 不含校验和)
 *
 * 帧格式: 0x55 0x55 [data...] [Checksum]
 * 发送完最后一字节后等待 TC 标志, 确保移位寄存器发送完成
 */
static void SERVO_SendPacket(uint8_t *data, uint8_t len)
{
	uint8_t checksum;
	uint8_t i;

	checksum = SERVO_CalcChecksum(data, len);

	/* 发送帧头 */
	SERVO_SendByte(SERVO_FRAME_HEADER);  /* 0x55 */
	SERVO_SendByte(SERVO_FRAME_HEADER);  /* 0x55 */

	/* 发送数据 */
	for (i = 0; i < len; i++)
	{
		SERVO_SendByte(data[i]);
	}

	/* 发送校验和 */
	SERVO_SendByte(checksum);

	/* 等待发送完成 (移位寄存器空) */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

	/*
	 * 通信协议要求: 发完一条指令后需等待舵机执行完毕
	 * 再发下一条, 否则指令会被丢弃
	 * 这里不做全局延时, 由上层根据 time_ms 参数自行控制
	 */
}

/**
 * @brief  舵机移动到指定角度 (立即执行)
 * @param  id:      舵机 ID (0~253, 254=广播)
 * @param  angle:   目标角度/位置 (0~1000, 对应0~240°物理角度)
 *                  舵机1范围 0~350, 舵机2范围 0~800
 * @param  time_ms: 运动时间 (0~30000 毫秒)
 *
 * 指令: SERVO_MOVE_TIME_WRITE (1)
 * 帧: 0x55 0x55 [ID][0x07][0x01][angleL][angleH][timeL][timeH][CRC]
 */
void SERVO_MoveTimeWrite(uint8_t id, uint16_t angle, uint16_t time_ms)
{
	uint8_t data[7];

	data[0] = id;                              /* ID */
	data[1] = 7;                               /* Length = 1(ID) + 1(Len) + 1(Cmd) + 4(Param) */
	data[2] = SERVO_MOVE_TIME_WRITE;           /* Cmd = 1 */
	data[3] = (uint8_t)(angle & 0xFF);         /* 角度低字节 */
	data[4] = (uint8_t)((angle >> 8) & 0xFF);  /* 角度高字节 */
	data[5] = (uint8_t)(time_ms & 0xFF);       /* 时间低字节 */
	data[6] = (uint8_t)((time_ms >> 8) & 0xFF);/* 时间高字节 */

	SERVO_SendPacket(data, 7);
}

/**
 * @brief  舵机移动到指定角度 (等待START指令)
 * @note    发送此指令后舵机不会立即转动,
 *           需调用 SERVO_MoveStart() 后才会开始转动
 */
void SERVO_MoveTimeWaitWrite(uint8_t id, uint16_t angle, uint16_t time_ms)
{
	uint8_t data[7];

	data[0] = id;
	data[1] = 7;
	data[2] = SERVO_MOVE_TIME_WAIT_WRITE;      /* Cmd = 7 */
	data[3] = (uint8_t)(angle & 0xFF);
	data[4] = (uint8_t)((angle >> 8) & 0xFF);
	data[5] = (uint8_t)(time_ms & 0xFF);
	data[6] = (uint8_t)((time_ms >> 8) & 0xFF);

	SERVO_SendPacket(data, 7);
}

/**
 * @brief  开始移动 (配合 SERVO_MoveTimeWaitWrite 使用)
 */
void SERVO_MoveStart(uint8_t id)
{
	uint8_t data[3];

	data[0] = id;
	data[1] = 3;                               /* Length = 3 */
	data[2] = SERVO_MOVE_START;                /* Cmd = 11 */

	SERVO_SendPacket(data, 3);
}

/**
 * @brief  立即停止舵机转动
 */
void SERVO_MoveStop(uint8_t id)
{
	uint8_t data[3];

	data[0] = id;
	data[1] = 3;
	data[2] = SERVO_MOVE_STOP;                 /* Cmd = 12 */

	SERVO_SendPacket(data, 3);
}

/**
 * @brief  舵机上电 / 掉电
 * @param  load: 0=掉电(卸力), 1=上电(加力)
 *
 * 指令: SERVO_LOAD_OR_UNLOAD_WRITE (31)
 * 帧: 0x55 0x55 [ID][0x04][0x1F][load][CRC]
 */
void SERVO_LoadOrUnload(uint8_t id, uint8_t load)
{
	uint8_t data[4];

	data[0] = id;
	data[1] = 4;
	data[2] = SERVO_LOAD_OR_UNLOAD_WRITE;      /* Cmd = 31 */
	data[3] = load ? 1 : 0;                     /* 0=掉电, 1=上电 */

	SERVO_SendPacket(data, 4);
}

/**
 * @brief  设置舵机角度限制 (掉电保存)
 * @param  min_angle: 最小角度 (0~1000)
 * @param  max_angle: 最大角度 (0~1000), 必须 > min_angle
 *
 * 指令: SERVO_ANGLE_LIMIT_WRITE (20)
 */
void SERVO_AngleLimitWrite(uint8_t id, uint16_t min_angle, uint16_t max_angle)
{
	uint8_t data[7];

	data[0] = id;
	data[1] = 7;
	data[2] = SERVO_ANGLE_LIMIT_WRITE;         /* Cmd = 20 */
	data[3] = (uint8_t)(min_angle & 0xFF);
	data[4] = (uint8_t)((min_angle >> 8) & 0xFF);
	data[5] = (uint8_t)(max_angle & 0xFF);
	data[6] = (uint8_t)((max_angle >> 8) & 0xFF);

	SERVO_SendPacket(data, 7);
}

/**
 * @brief  修改舵机 ID (掉电保存)
 * @param  new_id: 新 ID (0~253)
 *
 * 指令: SERVO_ID_WRITE (13)
 * 注意: 一次只能接一个舵机, 否则所有舵机 ID 都会被改成相同的值
 */
void SERVO_IdWrite(uint8_t id, uint8_t new_id)
{
	uint8_t data[4];

	data[0] = id;
	data[1] = 4;
	data[2] = SERVO_ID_WRITE;                  /* Cmd = 13 */
	data[3] = new_id;

	SERVO_SendPacket(data, 4);
}

/**
 * @brief  设置舵机角度偏差 (不保存)
 * @param  offset: 偏差值 -125~125, 对应 -30°~30°
 *
 * 指令: SERVO_ANGLE_OFFSET_ADJUST (17)
 * 注意: 该指令不保存到Flash, 掉电丢失
 *       若需永久保存, 先用此指令调整, 再发 SERVO_ANGLE_OFFSET_WRITE (18)
 */
void SERVO_SetAngleOffset(uint8_t id, int8_t offset)
{
	uint8_t data[4];

	data[0] = id;
	data[1] = 4;
	data[2] = SERVO_ANGLE_OFFSET_ADJUST;       /* Cmd = 17 */
	data[3] = (uint8_t)offset;                  /* signed char → unsigned char */

	SERVO_SendPacket(data, 4);
}

/**
 * @brief  舵机 LED 控制 (掉电保存)
 * @param  on_off: 0=灭, 1=亮 (默认0)
 */
void SERVO_LedCtrl(uint8_t id, uint8_t on_off)
{
	uint8_t data[4];

	data[0] = id;
	data[1] = 4;
	data[2] = SERVO_LED_CTRL_WRITE;            /* Cmd = 33 */
	data[3] = on_off ? 1 : 0;

	SERVO_SendPacket(data, 4);
}
