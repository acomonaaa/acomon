//
// Created by 高冲 on 26-3-31.
//

#ifndef __LCD_H
#define __LCD_H

#include "main.h"

#define LCD_WIDTH       240
#define LCD_HEIGHT      320

/* FSMC Bank4 地址 */
#define LCD_FSMC_BANK   0x6C000000
#define LCD_CMD_ADDR    (LCD_FSMC_BANK | (((1U << 6) - 1) << 1))
#define LCD_DAT_ADDR    (LCD_FSMC_BANK | ((1U << 6) << 1))

#define LCD_CMD_REG     (*(volatile uint16_t *)LCD_CMD_ADDR)
#define LCD_DAT_REG     (*(volatile uint16_t *)LCD_DAT_ADDR)

static inline void LCD_WriteCmd(uint16_t cmd)  { LCD_CMD_REG = cmd; }
static inline void LCD_WriteData(uint16_t dat) { LCD_DAT_REG = dat; }
static inline void LCD_WriteReg(uint16_t reg, uint16_t dat) { LCD_CMD_REG = reg; LCD_DAT_REG = dat; }

static inline uint16_t LCD_ReadData(void)
{
    __NOP(); __NOP();
    return LCD_DAT_REG;
}

/* 核心函数 */
uint8_t LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_BacklightOn(void);
void LCD_BacklightOff(void);
void LCD_BacklightToggle(void);
void LCD_SetDirection(uint8_t dir);
void LCD_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
uint16_t LCD_GetWidth(void);
uint16_t LCD_GetHeight(void);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

/* 颜色定义 */
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0

#endif

