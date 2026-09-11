/**
 * @file health_app.c
 * @brief 看门狗 + 任务健康监控
 *
 * 策略：所有已注册任务在超时窗口内更新心跳才喂 IWDG。
 * 任一任务卡死 → 停止喂狗 → 硬件复位（面试：可讲故障隔离与复位原因）。
 */
#include "health_app.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include <string.h>

static volatile uint32_t s_beat_ms[APP_HB_NUM];
static volatile uint8_t  s_active[APP_HB_NUM];
static IWDG_HandleTypeDef s_hiwdg;
static uint8_t s_iwdg_ok;

void health_monitor_init(void)
{
    memset((void *)s_beat_ms, 0, sizeof(s_beat_ms));
    memset((void *)s_active, 0, sizeof(s_active));

    s_hiwdg.Instance = IWDG;
    s_hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    /* LSI ~32kHz / 256 = 125Hz → 4s ≈ 500 */
    s_hiwdg.Init.Reload = 499;
    if (HAL_IWDG_Init(&s_hiwdg) == HAL_OK) {
        s_iwdg_ok = 1;
    }
}

void health_beat(uint32_t slot)
{
    if (slot >= APP_HB_NUM) return;
    s_beat_ms[slot] = osKernelGetTickCount();
    s_active[slot] = 1;
}

void health_unregister(uint32_t slot)
{
    if (slot >= APP_HB_NUM) return;
    s_active[slot] = 0;
}

void StartHealthTask(void *argument)
{
    (void)argument;
    for (;;) {
        uint32_t now = osKernelGetTickCount();
        uint8_t all_ok = 1;
        for (uint32_t i = 0; i < APP_HB_NUM; i++) {
            if (!s_active[i]) continue;          /* 未注册槽位不参与 */
            uint32_t beat = s_beat_ms[i];
            if (beat == 0 || (now - beat) > APP_HEARTBEAT_TIMEOUT_MS) {
                all_ok = 0;
                break;
            }
        }

        if (all_ok && s_iwdg_ok) {
            HAL_IWDG_Refresh(&s_hiwdg);
        }
        /* all_ok==0：故意不刷新，等待 IWDG 复位 */

        osDelay(APP_HEALTH_PERIOD_MS);
    }
}
