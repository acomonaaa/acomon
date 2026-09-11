/**
 * @file control_app.c
 * @brief 200ms 闭环控制：滞回阈值 + 双模式 + 声光报警 + 软 PWM
 */
#include "control_app.h"
#include "cmsis_os2.h"
#include "system_data.h"
#include "actuator.h"
#include "health_app.h"
#include "app_config.h"

/* 滞回输出锁存：避免在阈值附近抖动 */
static uint8_t s_fan_on;
static uint8_t s_light_on;
static uint8_t s_hum_on;
static uint8_t s_co2_on;

static uint8_t hyst_need_fan(float temp, float thr)
{
    if (temp > thr) s_fan_on = 1;
    else if (temp < thr - APP_HYST_TEMP_C) s_fan_on = 0;
    return s_fan_on;
}

static uint8_t hyst_need_pump(float humi, float thr)
{
    /* 湿度过低灌溉 */
    if (humi < thr) s_hum_on = 1;
    else if (humi > thr + APP_HYST_HUMI_PCT) s_hum_on = 0;
    return s_hum_on;
}

static uint8_t hyst_need_light(uint32_t lux, uint32_t thr)
{
    if (lux < thr) s_light_on = 1;
    else if (lux > thr + (uint32_t)APP_HYST_LIGHT_PCT) s_light_on = 0;
    return s_light_on;
}

static uint8_t hyst_need_vent(float co2, float thr)
{
    if (co2 > thr) s_co2_on = 1;
    else if (co2 < thr - APP_HYST_CO2_PPM) s_co2_on = 0;
    return s_co2_on;
}

void StartControlTask(void *argument)
{
    (void)argument;
    actuator_init();

    for (;;) {
        SystemData_t snap;
        SystemData_Snapshot(&snap);

        uint8_t fan = 0, pump = 0, light_pct = 0, alarm = 0, mask = 0;

        if (snap.auto_mode) {
            if (hyst_need_fan(snap.temp, snap.thresh_temp) ||
                hyst_need_vent(snap.co2, snap.thresh_co2)) {
                fan = 1;
            }
            pump = hyst_need_pump(snap.humi, snap.thresh_humi);
            if (hyst_need_light(snap.light, snap.thresh_light)) {
                light_pct = 80;
            } else {
                light_pct = 0;
            }

            if (snap.temp > snap.thresh_temp + APP_HYST_TEMP_C) mask |= 0x01;
            if (snap.humi < snap.thresh_humi - APP_HYST_HUMI_PCT) mask |= 0x02;
            if (snap.light < snap.thresh_light) mask |= 0x04;
            if (snap.co2 > snap.thresh_co2 + APP_HYST_CO2_PPM) mask |= 0x08;
            alarm = mask ? 1 : 0;
        } else {
            fan      = snap.fan_status;
            pump     = snap.pump_status;
            light_pct= snap.light_pwm;
            alarm    = snap.alarm_active;
            /* 手动模式：mask 与 active 保持一致（bit0=手动报警） */
            mask     = alarm ? 0x01 : 0;
        }

        actuator_set(ACT_FAN, fan);
        actuator_set(ACT_PUMP, pump);
        actuator_set_light_duty(light_pct);
        actuator_alarm_set(alarm);
        actuator_pwm_tick();

        if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
            g_SysData.fan_status   = fan;
            g_SysData.pump_status  = pump;
            g_SysData.light_pwm    = light_pct;
            g_SysData.alarm_active = alarm;
            g_SysData.alarm_mask   = mask;
            osMutexRelease(g_DataMutex);
        }

        health_beat(APP_HB_CONTROL);
        osDelay(APP_CONTROL_PERIOD_MS);
    }
}
