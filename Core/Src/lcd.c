//
// Created by 高冲 on 26-3-31.
//
#include "lcd.h"
#include <string.h>
#include <stdlib.h>

extern SRAM_HandleTypeDef hsram1;

static uint16_t lcd_width  = LCD_WIDTH;
static uint16_t lcd_height = LCD_HEIGHT;
static uint16_t lcd_id = 0;

static uint16_t LCD_ReadID(void)
{
    uint16_t id;

    LCD_WriteCmd(0xD3);
    LCD_ReadData();
    LCD_ReadData();
    id = LCD_ReadData() << 8;
    id |= LCD_ReadData() & 0x00FF;

    if (id == 0x9341) return id;

    LCD_WriteCmd(0x04);
    LCD_ReadData();
    LCD_ReadData();
    id = LCD_ReadData() << 8;
    id |= LCD_ReadData() & 0x00FF;

    return id;
}

void LCD_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_WriteCmd(0x2A);
    LCD_WriteData(x1 >> 8); LCD_WriteData(x1 & 0xFF);
    LCD_WriteData(x2 >> 8); LCD_WriteData(x2 & 0xFF);

    LCD_WriteCmd(0x2B);
    LCD_WriteData(y1 >> 8); LCD_WriteData(y1 & 0xFF);
    LCD_WriteData(y2 >> 8); LCD_WriteData(y2 & 0xFF);

    LCD_WriteCmd(0x2C);
}

static void LCD_RegInit_ILI9341(void)
{
    LCD_WriteCmd(0xCF);
    LCD_WriteData(0x00); LCD_WriteData(0xC1); LCD_WriteData(0x30);
    LCD_WriteCmd(0xED);
    LCD_WriteData(0x64); LCD_WriteData(0x03); LCD_WriteData(0x12); LCD_WriteData(0x81);
    LCD_WriteCmd(0xE8);
    LCD_WriteData(0x85); LCD_WriteData(0x10); LCD_WriteData(0x7A);
    LCD_WriteCmd(0xCB);
    LCD_WriteData(0x39); LCD_WriteData(0x2C); LCD_WriteData(0x00); LCD_WriteData(0x34); LCD_WriteData(0x02);
    LCD_WriteCmd(0xF7); LCD_WriteData(0x20);
    LCD_WriteCmd(0xEA); LCD_WriteData(0x00); LCD_WriteData(0x00);
    LCD_WriteCmd(0xC0); LCD_WriteData(0x1B);
    LCD_WriteCmd(0xC1); LCD_WriteData(0x01);
    LCD_WriteCmd(0xC5); LCD_WriteData(0x30); LCD_WriteData(0x30);
    LCD_WriteCmd(0xC7); LCD_WriteData(0xB7);
    LCD_WriteCmd(0x36); LCD_WriteData(0x48);
    LCD_WriteCmd(0x3A); LCD_WriteData(0x55);
    LCD_WriteCmd(0xB1); LCD_WriteData(0x00); LCD_WriteData(0x1A);
    LCD_WriteCmd(0xB6); LCD_WriteData(0x0A); LCD_WriteData(0xA2);
    LCD_WriteCmd(0xF2); LCD_WriteData(0x00);
    LCD_WriteCmd(0x26); LCD_WriteData(0x01);
    LCD_WriteCmd(0xE0);
    LCD_WriteData(0x0F); LCD_WriteData(0x2A); LCD_WriteData(0x28); LCD_WriteData(0x08);
    LCD_WriteData(0x0E); LCD_WriteData(0x08); LCD_WriteData(0x54); LCD_WriteData(0xA9);
    LCD_WriteData(0x43); LCD_WriteData(0x0A); LCD_WriteData(0x0F); LCD_WriteData(0x00);
    LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0x00);
    LCD_WriteCmd(0xE1);
    LCD_WriteData(0x00); LCD_WriteData(0x15); LCD_WriteData(0x17); LCD_WriteData(0x07);
    LCD_WriteData(0x11); LCD_WriteData(0x06); LCD_WriteData(0x2B); LCD_WriteData(0x56);
    LCD_WriteData(0x3C); LCD_WriteData(0x05); LCD_WriteData(0x10); LCD_WriteData(0x0F);
    LCD_WriteData(0x3F); LCD_WriteData(0x3F); LCD_WriteData(0x0F);
    LCD_WriteCmd(0x2B);
    LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0x01); LCD_WriteData(0x3F);
    LCD_WriteCmd(0x2A);
    LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0xEF);
    LCD_WriteCmd(0x11);
    HAL_Delay(100);
    LCD_WriteCmd(0x29);
}

static void LCD_RegInit_R61580V3(void)
{
    LCD_WriteCmd(0x11); HAL_Delay(120);
    LCD_WriteCmd(0x36); LCD_WriteData(0x00);
    LCD_WriteCmd(0x3A); LCD_WriteData(0x05);
    LCD_WriteCmd(0xB2);
    LCD_WriteData(0x0C); LCD_WriteData(0x0C); LCD_WriteData(0x00); LCD_WriteData(0x33); LCD_WriteData(0x33);
    LCD_WriteCmd(0xB7); LCD_WriteData(0x35);
    LCD_WriteCmd(0xBB); LCD_WriteData(0x32);
    LCD_WriteCmd(0xC0); LCD_WriteData(0x0C);
    LCD_WriteCmd(0xC2); LCD_WriteData(0x01);
    LCD_WriteCmd(0xC3); LCD_WriteData(0x10);
    LCD_WriteCmd(0xC4); LCD_WriteData(0x20);
    LCD_WriteCmd(0xC6); LCD_WriteData(0x0F);
    LCD_WriteCmd(0xD0); LCD_WriteData(0xA4); LCD_WriteData(0xA1);
    LCD_WriteCmd(0xE0);
    LCD_WriteData(0xD0); LCD_WriteData(0x00); LCD_WriteData(0x02); LCD_WriteData(0x07);
    LCD_WriteData(0x0A); LCD_WriteData(0x28); LCD_WriteData(0x32); LCD_WriteData(0x44);
    LCD_WriteData(0x42); LCD_WriteData(0x06); LCD_WriteData(0x0E); LCD_WriteData(0x12);
    LCD_WriteData(0x14); LCD_WriteData(0x17);
    LCD_WriteCmd(0xE1);
    LCD_WriteData(0xD0); LCD_WriteData(0x00); LCD_WriteData(0x02); LCD_WriteData(0x07);
    LCD_WriteData(0x0A); LCD_WriteData(0x28); LCD_WriteData(0x31); LCD_WriteData(0x54);
    LCD_WriteData(0x47); LCD_WriteData(0x0E); LCD_WriteData(0x1C); LCD_WriteData(0x17);
    LCD_WriteData(0x1B); LCD_WriteData(0x1E);
    LCD_WriteCmd(0x2A);
    LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0xEF);
    LCD_WriteCmd(0x2B);
    LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0x01); LCD_WriteData(0x3F);
    LCD_WriteCmd(0x29);
}

void LCD_SetDirection(uint8_t dir)
{
    uint16_t reg36val;
    switch (dir) {
        case 0: lcd_width = 240; lcd_height = 320; reg36val = 0x0008; break;
        case 1: lcd_width = 320; lcd_height = 240; reg36val = 0x0068; break;
        case 2: lcd_width = 320; lcd_height = 240; reg36val = 0x00A8; break;
        case 3: lcd_width = 240; lcd_height = 320; reg36val = 0x00C8; break;
        default: return;
    }
    LCD_WriteReg(0x36, reg36val);
}

void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint32_t total = (uint32_t)(ex - sx + 1) * (ey - sy + 1);
    LCD_SetWindow(sx, sy, ex, ey);
    for (uint32_t i = 0; i < total; i++) {
        LCD_WriteData(color);
    }
}

void LCD_Clear(uint16_t color)
{
    LCD_Fill(0, 0, lcd_width - 1, lcd_height - 1, color);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetWindow(x, y, x, y);
    LCD_WriteData(color);
}

void LCD_DisplayOn(void)  { LCD_WriteCmd(0x29); }
void LCD_DisplayOff(void) { LCD_WriteCmd(0x28); }
void LCD_BacklightOn(void)  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); }
void LCD_BacklightOff(void) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); }
void LCD_BacklightToggle(void) { HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15); }

uint16_t LCD_GetWidth(void)  { return lcd_width; }
uint16_t LCD_GetHeight(void) { return lcd_height; }

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t dx = abs((int16_t)x2 - (int16_t)x1);
    int16_t dy = -abs((int16_t)y2 - (int16_t)y1);
    int16_t sx = x1 < x2 ? 1 : -1;
    int16_t sy = y1 < y2 ? 1 : -1;
    int16_t err = dx + dy;

    while (1) {
        LCD_DrawPoint(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int16_t e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void LCD_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
    LCD_DrawLine(x2, y2, x1, y2, color);
    LCD_DrawLine(x1, y2, x1, y1, color);
}

void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t x = 0, y = r;
    int16_t d = 3 - 2 * r;
    while (x <= y) {
        LCD_DrawPoint(x0 + x, y0 + y, color);
        LCD_DrawPoint(x0 - x, y0 + y, color);
        LCD_DrawPoint(x0 + x, y0 - y, color);
        LCD_DrawPoint(x0 - x, y0 - y, color);
        LCD_DrawPoint(x0 + y, y0 + x, color);
        LCD_DrawPoint(x0 - y, y0 + x, color);
        LCD_DrawPoint(x0 + y, y0 - x, color);
        LCD_DrawPoint(x0 - y, y0 - x, color);
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

uint8_t LCD_Init(void)
{
    /* 拉低NE4片选 */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);

    LCD_BacklightOff();

    lcd_id = LCD_ReadID();
    if (lcd_id == 0x9341) {
        LCD_RegInit_ILI9341();
    } else if (lcd_id == 0x8552) {
        LCD_RegInit_R61580V3();
    } else {
        /* ID 读取失败，默认尝试 ILI9341 初始化 */
        LCD_RegInit_ILI9341();
    }

    LCD_SetDirection(0);
    LCD_Clear(COLOR_WHITE);
    LCD_DisplayOn();
    LCD_BacklightOn();

    return 0;
}
