#ifndef __USART_X_H
#define __USART_X_H

#include "sys.h"

#define USART2_RX_LEN 64
#define USART2_BAUD 9600
#define USART3_BAUD 9600

void USART_RX_Init(void);
void usart1_send(u8 data);
void usart2_send(u8 data);
void usart3_send(u8 data);
void Usart1_Init(u32 bound);
void Usart2_Init(u32 bound);
void Usart3_Init(u32 bound);

void usart2_send_string(u8 *str);
void usart3_send_string(u8 *str);
void usart_send_ack(u8 *str);
void usart_send_ack_byte(u8 data);
void usart_send_ack_int(int num);
void usart2_send_int(int num);
u8 USART2_Parse_Int(u8 *key, int *val);
u8 USART2_Parse_Command(u8 *cmd, u8 max_len);
void USART2_Process(void);

int USART1_IRQHandler(void);
int USART2_IRQHandler(void);

extern u8 USART2_RX_BUF[USART2_RX_LEN];
extern u16 USART2_RX_CNT;
extern u8 USART2_RX_FLAG;

#endif /* __USART_X_H */
