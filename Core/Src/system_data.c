#include "system_data.h"
#include <string.h>

SystemData_t g_SysData;
osMutexId_t  g_DataMutex;

void SystemData_Init(void)
{
    memset(&g_SysData, 0, sizeof(g_SysData));
    g_SysData.temp = 25.0f;
    g_SysData.humi = 50.0f;
    g_SysData.light = 400;
    g_SysData.co2 = 420.0f;

    g_SysData.thresh_temp  = 30.0f;
    g_SysData.thresh_humi  = 40.0f;
    g_SysData.thresh_light = 50;
    g_SysData.thresh_co2   = 1000.0f;

    g_SysData.fan_status   = 0;
    g_SysData.pump_status  = 0;
    g_SysData.light_pwm    = 0;
    g_SysData.auto_mode    = 1;
    g_SysData.cloud_state  = CLOUD_OFFLINE;

    if (g_DataMutex == NULL) {
        g_DataMutex = osMutexNew(NULL);
    }
}

void SystemData_Snapshot(SystemData_t *out)
{
    if (out == NULL) return;
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        *out = g_SysData;
        osMutexRelease(g_DataMutex);
    }
}

void SystemData_SetMode(uint8_t auto_mode)
{
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        g_SysData.auto_mode = auto_mode ? 1 : 0;
        osMutexRelease(g_DataMutex);
    }
}

void SystemData_SetActuator(uint8_t fan, uint8_t pump, uint8_t light_pct)
{
    if (light_pct > 100) light_pct = 100;
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        g_SysData.fan_status  = fan ? 1 : 0;
        g_SysData.pump_status = pump ? 1 : 0;
        g_SysData.light_pwm   = light_pct;
        osMutexRelease(g_DataMutex);
    }
}

void SystemData_SetThresholds(float t, float h, uint32_t lux, float co2)
{
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        g_SysData.thresh_temp  = t;
        g_SysData.thresh_humi  = h;
        g_SysData.thresh_light = lux;
        g_SysData.thresh_co2   = co2;
        osMutexRelease(g_DataMutex);
    }
}
