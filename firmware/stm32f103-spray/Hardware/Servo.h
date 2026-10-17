#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"

/* ============================================================
 *  总线舵机通信协议 - 命令定义
 *  UART 半双工单线, 115200bps
 *  数据帧格式: 0x55 0x55 ID Length Cmd Prm1...PrmN Checksum
 *  Checksum = ~(ID + Length + Cmd + Prm1 + ... + PrmN)
 * ============================================================ */

/* 帧头 */
#define SERVO_FRAME_HEADER          0x55

/* ---- 写命令 (舵机不返回数据) ---- */
#define SERVO_MOVE_TIME_WRITE       1    /* 角度+时间移动(立即执行), 数据长度7 */
#define SERVO_MOVE_TIME_WAIT_WRITE  7    /* 角度+时间移动(等待START), 数据长度7 */
#define SERVO_MOVE_START            11   /* 开始移动(配合WAIT_WRITE), 数据长度3 */
#define SERVO_MOVE_STOP             12   /* 停止移动, 数据长度3 */
#define SERVO_ID_WRITE              13   /* 修改舵机ID, 数据长度4 */
#define SERVO_ANGLE_OFFSET_ADJUST   17   /* 角度偏差调整(不保存), 数据长度4 */
#define SERVO_ANGLE_OFFSET_WRITE    18   /* 角度偏差写入(保存), 数据长度3 */
#define SERVO_ANGLE_LIMIT_WRITE     20   /* 角度限制写入, 数据长度7 */
#define SERVO_VIN_LIMIT_WRITE       22   /* 电压限制写入, 数据长度7 */
#define SERVO_TEMP_MAX_LIMIT_WRITE  24   /* 温度上限写入, 数据长度4 */
#define SERVO_OR_MOTOR_MODE_WRITE   29   /* 舵机/马达模式写入, 数据长度7 */
#define SERVO_LOAD_OR_UNLOAD_WRITE  31   /* 上电/掉电写入, 数据长度4 */
#define SERVO_LED_CTRL_WRITE        33   /* LED控制写入, 数据长度4 */
#define SERVO_LED_ERROR_WRITE       35   /* LED故障报警写入, 数据长度4 */

/* ---- 读命令 (舵机返回数据) ---- */
#define SERVO_MOVE_TIME_READ        2    /* 读取角度+时间 */
#define SERVO_MOVE_TIME_WAIT_READ   8    /* 读取等待角度+时间 */
#define SERVO_ID_READ               14   /* 读取ID */
#define SERVO_ANGLE_OFFSET_READ     19   /* 读取角度偏差 */
#define SERVO_ANGLE_LIMIT_READ      21   /* 读取角度限制 */
#define SERVO_VIN_LIMIT_READ        23   /* 读取电压限制 */
#define SERVO_TEMP_MAX_LIMIT_READ   25   /* 读取温度上限 */
#define SERVO_TEMP_READ             26   /* 读取当前温度 */
#define SERVO_VIN_READ              27   /* 读取当前电压 */
#define SERVO_POS_READ              28   /* 读取当前位置 */
#define SERVO_OR_MOTOR_MODE_READ    30   /* 读取舵机/马达模式 */
#define SERVO_LOAD_OR_UNLOAD_READ   32   /* 读取上电/掉电状态 */
#define SERVO_LED_CTRL_READ         34   /* 读取LED状态 */
#define SERVO_LED_ERROR_READ        36   /* 读取LED故障 */
#define SERVO_DIS_READ              48   /* 读取旋转距离(4096/圈) */

/* 广播ID */
#define SERVO_BROADCAST_ID          254

/* ============================================================
 *  函数声明
 * ============================================================ */

void SERVO_Init(void);
void SERVO_MoveTimeWrite(uint8_t id, uint16_t angle, uint16_t time_ms);
void SERVO_MoveTimeWaitWrite(uint8_t id, uint16_t angle, uint16_t time_ms);
void SERVO_MoveStart(uint8_t id);
void SERVO_MoveStop(uint8_t id);
void SERVO_LoadOrUnload(uint8_t id, uint8_t load);      /* 0=掉电, 1=上电 */
void SERVO_AngleLimitWrite(uint8_t id, uint16_t min_angle, uint16_t max_angle);
void SERVO_IdWrite(uint8_t id, uint8_t new_id);
void SERVO_SetAngleOffset(uint8_t id, int8_t offset);    /* -125~125, 对应 -30°~30° */
void SERVO_LedCtrl(uint8_t id, uint8_t on_off);          /* 0=灭, 1=亮 */

#endif /* __SERVO_H */
