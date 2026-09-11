/**
 * @file system_data.h
 * @brief 全局数据字典（多任务共享黑板）+ 互斥锁
 */
#ifndef __SYSTEM_DATA_H__
#define __SYSTEM_DATA_H__

#include <stdint.h>
#include "cmsis_os2.h"

typedef enum {
    CLOUD_OFFLINE = 0,
    CLOUD_CONNECTING,
    CLOUD_ONLINE,
    CLOUD_BACKOFF
} cloud_state_t;

typedef struct {
    /* 1. 传感器（sensorTask 写） */
    float    temp;
    float    humi;
    uint32_t light;
    float    co2;

    /* 2. 阈值（UI/云端写，control 读） */
    float    thresh_temp;
    float    thresh_humi;
    uint32_t thresh_light;
    float    thresh_co2;

    /* 3. 执行器状态（control 写，UI/云读） */
    uint8_t  fan_status;
    uint8_t  pump_status;
    uint8_t  light_pwm;      /* 0-100% */

    /* 4. 模式：1=自动 0=手动 */
    uint8_t  auto_mode;

    /* 5. 报警 */
    uint8_t  alarm_active;
    uint8_t  alarm_mask;     /* bit0温 bit1湿 bit2光 bit3CO2 */

    /* 6. 云 */
    uint8_t  cloud_state;    /* cloud_state_t */
    uint8_t  cloud_cmd_pending;
    uint32_t last_cmd_seq;
    uint32_t uplink_ok;
    uint32_t uplink_fail;
    uint32_t cmd_recv;
    uint32_t cmd_dup_drop;

    /* 7. 运行 */
    uint32_t uptime_s;
} SystemData_t;

extern SystemData_t g_SysData;
extern osMutexId_t  g_DataMutex;

void SystemData_Init(void);

/* 短临界区拷贝/写回辅助，避免业务层到处裸锁 */
void SystemData_Snapshot(SystemData_t *out);
void SystemData_SetMode(uint8_t auto_mode);
void SystemData_SetActuator(uint8_t fan, uint8_t pump, uint8_t light_pct);
void SystemData_SetThresholds(float t, float h, uint32_t lux, float co2);

#endif /* __SYSTEM_DATA_H__ */
