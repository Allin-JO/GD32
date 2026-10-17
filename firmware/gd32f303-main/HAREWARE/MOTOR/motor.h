#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define STBY PCout(2)

#define BIN1 PCout(12)
#define BIN2 PDout(2)

#define AIN1 PBout(4)
#define AIN2 PBout(5)

#define DIN1 PCout(13)
#define DIN2 PCout(14)

#define CIN1 PBout(8)
#define CIN2 PBout(9)

void Motor_Init(void);
void Motor_mode(u8 mode);

#endif
