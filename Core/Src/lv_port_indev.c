/**
 * @file lv_port_indev.c
 * @brief LVGL输入设备端口 - 触摸驱动对接
 */

/*********************
 *      INCLUDES
 *********************/
#include "../Inc/lv_port_indev.h"
#include "touch.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_indev_t *indev_touchpad = NULL;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /* 触摸硬件已在 touch_init() 中初始化，此处不再重复调用 */

    /* 创建一个触摸输入设备 */
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, touchpad_read);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief 触摸板读取回调函数
 * @param indev  输入设备指针
 * @param data   输入设备数据指针
 * @note  [Fix9] 区分 TOUCH_ERROR 和 TOUCH_EMPTY，避免边缘抖动
 */
static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;
    static uint8_t last_pressed = 0;

    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t res = touch_scan(&x, &y);

    switch (res)
    {
        case TOUCH_EOK:
            /* 有效触摸按下 */
            last_x = x;
            last_y = y;
            last_pressed = 1;

            data->point.x = last_x;
            data->point.y = last_y;
            data->state = LV_INDEV_STATE_PRESSED;
            break;

        case TOUCH_ERROR:
            /* 坐标越界但手指可能仍在屏幕上，保持按下状态 */
            if (last_pressed)
            {
                data->point.x = last_x;
                data->point.y = last_y;
                data->state = LV_INDEV_STATE_PRESSED;  /* 保持按下，避免抖动 */
            }
            else
            {
                data->state = LV_INDEV_STATE_RELEASED;
            }
            break;

        case TOUCH_EMPTY:
        default:
            /* 无触摸，释放 */
            if (last_pressed)
            {
                /* 释放时返回最后一次按下的坐标 */
                data->point.x = last_x;
                data->point.y = last_y;
                data->state = LV_INDEV_STATE_RELEASED;
                last_pressed = 0;
            }
            else
            {
                data->state = LV_INDEV_STATE_RELEASED;
            }
            break;
    }
}
