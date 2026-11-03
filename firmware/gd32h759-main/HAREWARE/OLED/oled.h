/***********************************************
 * OLED bit-bang interface for GD32H759.
 * Adjust BOARD_OLED_* pins in BOARD/board_h759.h.
 ***********************************************/

#ifndef __OLED_H
#define __OLED_H

#include "sys.h"

#define OLED_SDIN_Clr()  gpio_bit_reset(BOARD_OLED_SDIN_PORT, BOARD_OLED_SDIN_PIN)
#define OLED_SDIN_Set()  gpio_bit_set(BOARD_OLED_SDIN_PORT, BOARD_OLED_SDIN_PIN)
#define OLED_SCLK_Clr()  gpio_bit_reset(BOARD_OLED_SCLK_PORT, BOARD_OLED_SCLK_PIN)
#define OLED_SCLK_Set()  gpio_bit_set(BOARD_OLED_SCLK_PORT, BOARD_OLED_SCLK_PIN)
#define OLED_RS_Clr()    gpio_bit_reset(BOARD_OLED_RS_PORT, BOARD_OLED_RS_PIN)
#define OLED_RS_Set()    gpio_bit_set(BOARD_OLED_RS_PORT, BOARD_OLED_RS_PIN)
#define OLED_RST_Clr()   gpio_bit_reset(BOARD_OLED_RST_PORT, BOARD_OLED_RST_PIN)
#define OLED_RST_Set()   gpio_bit_set(BOARD_OLED_RST_PORT, BOARD_OLED_RST_PIN)

#define OLED_CMD  0
#define OLED_DATA 1

#define CNSizeWidth  16
#define CNSizeHeight 16

extern u8 OLED_GRAM[128][8];

void OLED_WR_Byte(u8 dat, u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size);
void OLED_ShowString(u8 x, u8 y, const char *p);
void OLED_Refresh_GRAM_Line(u8 line);
void OLED_ShowCHinese(u8 x, u8 y, u8 no, u8 font_width, u8 font_height);
void OLED_Set_Pos(unsigned char x, unsigned char y);

#endif
