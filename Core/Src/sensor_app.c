/**
 * @file sensor_app.c
 * @brief 1s 周期采集：仿真/DHT11 → 滑动平均 → 写字典 → 环形缓冲 → 心跳
 */
#include "sensor_app.h"
#include "cmsis_os2.h"
#include "system_data.h"
#include "history_ring.h"
#include "health_app.h"
#include "app_config.h"

#include <math.h>
#include <string.h>

/* 滑动平均状态 */
static float s_sma_temp[APP_SMA_WINDOW];
static float s_sma_humi[APP_SMA_WINDOW];
static float s_sma_co2[APP_SMA_WINDOW];
static uint32_t s_sma_light[APP_SMA_WINDOW];
static uint32_t s_sma_idx;
static uint32_t s_sma_filled;

static float sma_f(float *buf, uint32_t n)
{
    float sum = 0.0f;
    for (uint32_t i = 0; i < n; i++) sum += buf[i];
    return sum / (float)n;
}

static uint32_t sma_u32(uint32_t *buf, uint32_t n)
{
    uint32_t sum = 0;
    for (uint32_t i = 0; i < n; i++) sum += buf[i];
    return sum / n;
}

#if APP_SENSOR_SIM
/* 仿真：慢变正弦 + 小噪声，便于看滤波与曲线 */
static void sim_sample(float *t, float *h, float *c, uint32_t *lux)
{
    float phase = (float)osKernelGetTickCount() * 0.0002f;
    *t   = 26.0f + 4.0f * sinf(phase) + ((int)(osKernelGetTickCount() % 7) - 3) * 0.05f;
    *h   = 55.0f + 10.0f * sinf(phase * 0.7f + 1.0f) + ((int)(osKernelGetTickCount() % 5) - 2) * 0.1f;
    *c   = 600.0f + 200.0f * sinf(phase * 0.5f + 2.0f);
    *lux = (uint32_t)(400.0f + 200.0f * sinf(phase * 0.9f + 0.5f));
}
#endif

void StartSensorTask(void *argument)
{
    (void)argument;
    uint32_t tick_div = 0;

    for (;;) {
        float raw_t, raw_h, raw_c;
        uint32_t raw_lux;

#if APP_SENSOR_SIM
        sim_sample(&raw_t, &raw_h, &raw_c, &raw_lux);
#else
        /* 真实 DHT11 路径：读失败则沿用上一次滤波值 */
        {
            uint8_t t8 = 0, h8 = 0;
            if (DHT11_read_data(&t8, &h8) == 0) {
                raw_t = (float)t8;
                raw_h = (float)h8;
            } else {
                raw_t = g_SysData.temp;
                raw_h = g_SysData.humi;
            }
            raw_c   = g_SysData.co2;
            raw_lux = g_SysData.light;
        }
#endif
        /* 滑动平均 */
        s_sma_temp[s_sma_idx] = raw_t;
        s_sma_humi[s_sma_idx] = raw_h;
        s_sma_co2[s_sma_idx]  = raw_c;
        s_sma_light[s_sma_idx]= raw_lux;
        s_sma_idx = (s_sma_idx + 1) % APP_SMA_WINDOW;
        if (s_sma_filled < APP_SMA_WINDOW) s_sma_filled++;

        float ft = sma_f(s_sma_temp, s_sma_filled);
        float fh = sma_f(s_sma_humi, s_sma_filled);
        float fc = sma_f(s_sma_co2, s_sma_filled);
        uint32_t fl = sma_u32(s_sma_light, s_sma_filled);

        if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
            g_SysData.temp  = ft;
            g_SysData.humi  = fh;
            g_SysData.co2   = fc;
            g_SysData.light = fl;
            if ((tick_div % 10) == 0) {
                g_SysData.uptime_s++;
            }
            osMutexRelease(g_DataMutex);
        }

        history_sample_t hs;
        hs.ts_ms = osKernelGetTickCount();
        hs.temp  = ft;
        hs.humi  = fh;
        hs.co2   = fc;
        hs.light = fl;
        hs.seq   = 0;
        history_ring_push(&hs);

        tick_div++;
        health_beat(APP_HB_SENSOR);
        osDelay(APP_SENSOR_PERIOD_MS);
    }
}
