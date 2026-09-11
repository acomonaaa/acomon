/**
 * @file cloud_sync.c
 * @brief cloudTask：AT 轮询 + 上报 + 续传 + 命令处理
 */
#include "cloud_sync.h"
#include "cmsis_os2.h"
#include "at_sm.h"
#include "mqtt_client.h"
#include "json_mini.h"
#include "history_ring.h"
#include "system_data.h"
#include "health_app.h"
#include "app_config.h"
#include "l610_uart.h"
#include <stdio.h>
#include <string.h>

#define CLOUD_POLL_MS   20
#define UPLINK_PERIOD   5000   /* 在线时周期上报 ms */
#define RETX_BATCH      4      /* 单次续传条数 */

static uint32_t s_last_uplink_ms;
static uint32_t s_last_ack_ms;
static uint32_t s_pending_ack_seq;
static uint8_t s_ack_waiting;
static char s_ack_cmd[32];

static void apply_cloud_cmd(const cloud_cmd_t *cmd)
{
    if (strcmp(cmd->cmd, "set_mode") == 0 && cmd->has_mode) {
        SystemData_SetMode(cmd->auto_mode);
    } else if (strcmp(cmd->cmd, "set_threshold") == 0 && cmd->has_thr) {
        SystemData_t s;
        SystemData_Snapshot(&s);
        SystemData_SetThresholds(
            (cmd->thr_temp > 0.0f) ? cmd->thr_temp : s.thresh_temp,
            (cmd->thr_humi > 0.0f) ? cmd->thr_humi : s.thresh_humi,
            (cmd->thr_light > 0) ? cmd->thr_light : s.thresh_light,
            (cmd->thr_co2 > 0.0f) ? cmd->thr_co2 : s.thresh_co2);
    } else if (strcmp(cmd->cmd, "set_actuator") == 0 && cmd->has_act) {
        SystemData_t s;
        SystemData_Snapshot(&s);
        /* 强制手动执行云指令 */
        SystemData_SetMode(0);
        SystemData_SetActuator(cmd->fan, cmd->pump, cmd->light_pct);
    } else if (strcmp(cmd->cmd, "force_report") == 0) {
        s_last_uplink_ms = 0;
    }
}

static void handle_downlink(void)
{
    char json[256];
    if (!at_sm_take_publish(json, sizeof(json))) return;

    cloud_cmd_t cmd;
    if (json_parse_cmd(json, &cmd) != 0) return;

    /* seq 去重：重复直接丢弃 */
    uint32_t last_seq = 0;
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        last_seq = g_SysData.last_cmd_seq;
        if (cmd.seq != 0 && cmd.seq <= last_seq) {
            g_SysData.cmd_dup_drop++;
            osMutexRelease(g_DataMutex);
            return;
        }
        if (cmd.seq != 0) g_SysData.last_cmd_seq = cmd.seq;
        g_SysData.cmd_recv++;
        osMutexRelease(g_DataMutex);
    }

    apply_cloud_cmd(&cmd);

    /* 发 ACK 并启动超时确认窗口 */
    char ack[128];
    json_pack_ack(ack, sizeof(ack), cmd.cmd, cmd.seq, 0);
    mqtt_publish(APP_CLOUD_TOPIC_CMD_RSP, ack);
    s_pending_ack_seq = cmd.seq;
    snprintf(s_ack_cmd, sizeof(s_ack_cmd), "%s", cmd.cmd);
    s_ack_waiting = 1;
    s_last_ack_ms = osKernelGetTickCount();
}

static int uplink_one(const history_sample_t *s)
{
    char body[256];
    SystemData_t snap;
    SystemData_Snapshot(&snap);
    json_pack_properties(body, sizeof(body),
                         s->temp, s->humi, s->co2, s->light,
                         snap.fan_status, snap.pump_status, snap.light_pwm,
                         snap.auto_mode, snap.alarm_active, s->seq);
    if (mqtt_publish(APP_CLOUD_TOPIC_PROP, body)) {
        history_ring_mark_sent(s->seq);
        return 1;
    }
    return 0;
}

static void uplink_flow(void)
{
    if (!mqtt_connected()) return;

    uint32_t now = osKernelGetTickCount();

    /* 优先续传未发送；失败立即停，避免断链窗口假推进游标 */
    history_sample_t hs;
    uint32_t sent = history_ring_next_unsent(&hs);
    if (sent != 0) {
        for (uint32_t i = 0; i < RETX_BATCH; i++) {
            if (sent == 0) break;
            if (!uplink_one(&hs)) break;
            sent = history_ring_next_unsent(&hs);
        }
    } else if (now - s_last_uplink_ms >= UPLINK_PERIOD) {
        history_sample_t one[1];
        if (history_ring_snapshot(one, 1) == 1) {
            if (uplink_one(&one[0])) {
                s_last_uplink_ms = now;
            }
        }
    }
}

void cloud_sync_init(void)
{
#if !APP_CLOUD_SIM
    l610_uart_init();
#endif
    mqtt_client_init();
    s_last_uplink_ms = 0;
    s_ack_waiting = 0;
}

void StartCloudTask(void *argument)
{
    (void)argument;
    for (;;) {
        at_sm_poll();
        handle_downlink();
        uplink_flow();

        /* ACK 超时：视为失败，等待重连/重发（seq 仍保留，云端可重推） */
        if (s_ack_waiting && (osKernelGetTickCount() - s_last_ack_ms) > APP_CMD_ACK_TIMEOUT_MS) {
            s_ack_waiting = 0;
        }

        health_beat(APP_HB_CLOUD);
        osDelay(CLOUD_POLL_MS);
    }
}
