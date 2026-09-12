/**
 * @file health_app.h
 * @brief 任务心跳登记 + IWDG 喂狗策略
 */
#ifndef __HEALTH_APP_H__
#define __HEALTH_APP_H__

#include <stdint.h>
#include "app_config.h"

void health_monitor_init(void);
void health_beat(uint32_t slot);                 /* 任务心跳 */
void health_unregister(uint32_t slot);           /* 自删除任务注销（如 defaultTask） */
void StartHealthTask(void *argument);            /* CMSIS 线程入口 */

#endif /* __HEALTH_APP_H__ */
