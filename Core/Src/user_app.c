/**
 * @file user_app.c
 * @brief 业务层集中初始化入口 - 所有 FreeRTOS 任务/OS 对象在此创建
 */
#include "user_app.h"
#include "cmsis_os2.h"

#include "system_data.h"
#include "sensor_app.h"
#include "control_app.h"
#include "history_ring.h"
#include "health_app.h"
#include "cloud_sync.h"
#include "app_config.h"

void User_App_Init(void)
{
    SystemData_Init();
    history_ring_init();
    health_monitor_init();

    const osThreadAttr_t sensor_attr = {
        .name = "sensorTask",
        .stack_size = 768 * 4, /* 3KB */
        .priority = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(StartSensorTask, NULL, &sensor_attr);

    const osThreadAttr_t control_attr = {
        .name = "controlTask",
        .stack_size = 512 * 4,
        .priority = (osPriority_t)osPriorityAboveNormal,
    };
    osThreadNew(StartControlTask, NULL, &control_attr);

    const osThreadAttr_t cloud_attr = {
        .name = "cloudTask",
        .stack_size = 768 * 4, /* 3KB */
        .priority = (osPriority_t)osPriorityBelowNormal,
    };
    osThreadNew(StartCloudTask, NULL, &cloud_attr);

    const osThreadAttr_t health_attr = {
        .name = "healthTask",
        .stack_size = 512 * 4,
        .priority = (osPriority_t)osPriorityHigh,
    };
    osThreadNew(StartHealthTask, NULL, &health_attr);

    cloud_sync_init();
}
