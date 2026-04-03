/**
 ****************************************************************************************************
 * @file        touch.h
 * @author      正点原子团队(ALIENTEK) - 适配版
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0280模块触摸驱动代码
 * @note        完全参考正点原子官方代码
 ****************************************************************************************************
 */

#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "main.h"

/* 触摸相关引脚定义 - 请根据实际接线修改 */
#define TOUCH_PEN_GPIO_PORT         GPIOB
#define TOUCH_PEN_GPIO_PIN          GPIO_PIN_1
#define TOUCH_PEN_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define TOUCH_SPI_MI_GPIO_PORT      GPIOB
#define TOUCH_SPI_MI_GPIO_PIN       GPIO_PIN_2
#define TOUCH_SPI_MI_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define TOUCH_SPI_MO_GPIO_PORT      GPIOF
#define TOUCH_SPI_MO_GPIO_PIN       GPIO_PIN_11
#define TOUCH_SPI_MO_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

#define TOUCH_SPI_TCS_GPIO_PORT     GPIOC
#define TOUCH_SPI_TCS_GPIO_PIN      GPIO_PIN_13
#define TOUCH_SPI_TCS_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

#define TOUCH_SPI_CLK_GPIO_PORT     GPIOB
#define TOUCH_SPI_CLK_GPIO_PIN      GPIO_PIN_0
#define TOUCH_SPI_CLK_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* IO操作 */
#define TOUCH_READ_PEN()            HAL_GPIO_ReadPin(TOUCH_PEN_GPIO_PORT, TOUCH_PEN_GPIO_PIN)
#define TOUCH_SPI_READ_MI()         HAL_GPIO_ReadPin(TOUCH_SPI_MI_GPIO_PORT, TOUCH_SPI_MI_GPIO_PIN)
#define TOUCH_SPI_MO(x)             do{ x ? HAL_GPIO_WritePin(TOUCH_SPI_MO_GPIO_PORT, TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_SET) : \
                                           HAL_GPIO_WritePin(TOUCH_SPI_MO_GPIO_PORT, TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_RESET); \
                                      }while(0)
#define TOUCH_SPI_TCS(x)            do{ x ? HAL_GPIO_WritePin(TOUCH_SPI_TCS_GPIO_PORT, TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_SET) : \
                                           HAL_GPIO_WritePin(TOUCH_SPI_TCS_GPIO_PORT, TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_RESET); \
                                      }while(0)
#define TOUCH_SPI_CLK(x)            do{ x ? HAL_GPIO_WritePin(TOUCH_SPI_CLK_GPIO_PORT, TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_SET) : \
                                           HAL_GPIO_WritePin(TOUCH_SPI_CLK_GPIO_PORT, TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_RESET); \
                                      }while(0)

/* 触摸参数定义 */
#define TOUCH_READ_TIMES            10      /* 连续读取次数 */
#define TOUCH_READ_DISCARD          2       /* 丢弃的最大最小值个数 */

/* 触摸误差范围，两次读取的差值小于此值认为读数有效 */
/* [Fix6] 增大阈值从50到100，避免SPI时序不稳定时频繁重试 */
/* 对于12位ADC(0-4095)，100约为2.4%，更宽容 */
#ifndef TOUCH_READ_RANGE
#define TOUCH_READ_RANGE            100
#endif

/* 触摸坐标交换 - 如果X和Y轴反了，启用此选项 */
#define TOUCH_SWAP_XY               0

/* LCD显示方向 - 必须与 lcd.c 中 LCD_SetDirection() 的参数一致！ */
/* 0: 竖屏(240x320), 1: 横屏(320x240) */
#define LCD_DIRECTION               1

/* 错误代码 */
#define TOUCH_EOK                   0       /* 没有错误 */
#define TOUCH_ERROR                 1       /* 错误 */
#define TOUCH_EMPTY                 2       /* 无触摸 */

/* 函数声明 */
void touch_init(void);                     /* 触摸初始化 */
uint8_t touch_scan(uint16_t *x, uint16_t *y);  /* 触摸扫描 */
uint8_t touch_is_pressed(void);            /* 检查触摸是否按下 */
uint8_t touch_test(uint16_t *x_adc, uint16_t *y_adc);  /* 触摸硬件测试 */

#endif /* __TOUCH_H__ */
