/**
 * @file lv_port_indev.h
 *
 */

#ifndef LV_PORT_INDEV_H
#define LV_PORT_INDEV_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief 初始化LVGL输入设备
 */
void lv_port_indev_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_PORT_INDEV_H */
