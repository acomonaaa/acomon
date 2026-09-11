/**
 * @file at_sm.c
 * @brief L610 AT 状态机实现
 *
 * 设计：
 *  - 每次 at_sm_send_cmd_next(cmd, next_on_ok) 记录「OK 后应进入的状态」，
 *    避免用命令字符串猜状态（上一轮 SIM 链路坍缩根因）。
 *  - INIT 起始：SIM 与真机共用同一命令序列 AT → CPIN → CEREG → MQTT_CFG
 *    → MQTT_CONN → MQTT_SUB → ONLINE。
 *  - 指数退避仅在 ONLINE 成功后重置；BACKOFF 结束回 INIT 重连。
 *  - 应答（OK/ERROR）与 URC（+CMQTTPUBLISH）分流。
 */
#include "at_sm.h"
#include "l610_uart.h"
#include "app_config.h"
#include "system_data.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static at_state_t s_state = AT_ST_INIT;
static at_state_t s_next_on_ok = AT_ST_ONLINE;
static uint32_t s_state_enter_ms;
static uint32_t s_backoff_ms = APP_BACKOFF_MIN_MS;
#if !APP_CLOUD_SIM
static char s_line[256];
static uint32_t s_line_len;
#endif
static char s_pending_cmd[420];

static char s_urc_payload[256];
static uint8_t s_urc_ready;
static uint8_t s_publish_inflight; /* 真机 pub 等待 OK */

static void set_cloud_ui(uint8_t st)
{
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        g_SysData.cloud_state = st;
        osMutexRelease(g_DataMutex);
    }
}

static void enter(at_state_t st)
{
    s_state = st;
    s_state_enter_ms = osKernelGetTickCount();
    set_cloud_ui((st == AT_ST_ONLINE) ? CLOUD_ONLINE :
                 (st == AT_ST_BACKOFF) ? CLOUD_BACKOFF :
                 (st == AT_ST_ERROR)   ? CLOUD_OFFLINE : CLOUD_CONNECTING);
}

void at_sm_init(void)
{
    s_backoff_ms = APP_BACKOFF_MIN_MS;
#if !APP_CLOUD_SIM
    s_line_len = 0;
#endif
    s_urc_ready = 0;
    s_publish_inflight = 0;
    s_pending_cmd[0] = '\0';
    enter(AT_ST_INIT);
}

void at_sm_send_cmd_next(const char *cmd, at_state_t next_on_ok)
{
    if (cmd == NULL) return;
    s_next_on_ok = next_on_ok;
    snprintf(s_pending_cmd, sizeof(s_pending_cmd), "%s\r\n", cmd);
#if !APP_CLOUD_SIM
    l610_uart_write((const uint8_t *)s_pending_cmd, (uint32_t)strlen(s_pending_cmd));
#endif
    enter(AT_ST_WAIT_OK);
}

static void schedule_backoff(void)
{
    s_publish_inflight = 0;
    s_backoff_ms <<= 1;
    if (s_backoff_ms > APP_BACKOFF_MAX_MS) s_backoff_ms = APP_BACKOFF_MAX_MS;
    enter(AT_ST_BACKOFF);
}

static void handle_urc_line(const char *line)
{
    const char *p = strstr(line, "+CMQTTPUBLISH:");
    if (p == NULL) return;
    const char *brace = strchr(p, '{');
    if (brace) {
        size_t n = strlen(brace);
        if (n >= sizeof(s_urc_payload)) n = sizeof(s_urc_payload) - 1;
        memcpy(s_urc_payload, brace, n);
        s_urc_payload[n] = '\0';
        s_urc_ready = 1;
    }
}

static void on_ok(void)
{
    if (s_publish_inflight) {
        s_publish_inflight = 0;
        if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
            g_SysData.uplink_ok++;
            osMutexRelease(g_DataMutex);
        }
    }
    enter(s_next_on_ok);
    if (s_state == AT_ST_ONLINE) {
        s_backoff_ms = APP_BACKOFF_MIN_MS;
    }
}

static void handle_line(const char *line)
{
    if (line == NULL || line[0] == '\0') return;

    if (strncmp(line, "+CMQTTPUBLISH:", 14) == 0) {
        handle_urc_line(line);
        return;
    }
    /* OK/ERROR 只在等待应答时驱动状态机；BACKOFF/ONLINE 忽略迟到应答 */
    if (s_state != AT_ST_WAIT_OK) {
        return;
    }
    if (strcmp(line, "OK") == 0) {
        on_ok();
        return;
    }
    if (strncmp(line, "ERROR", 5) == 0 || strncmp(line, "+CME ERROR", 10) == 0) {
        schedule_backoff();
        return;
    }
}

void at_sm_feed(void)
{
#if APP_CLOUD_SIM
    return;
#else
    uint8_t chunk[64];
    uint32_t n = l610_uart_read(chunk, sizeof(chunk));
    for (uint32_t i = 0; i < n; i++) {
        char c = (char)chunk[i];
        if (c == '\r') continue;
        if (c == '\n') {
            s_line[s_line_len] = '\0';
            handle_line(s_line);
            s_line_len = 0;
        } else if (s_line_len + 1 < sizeof(s_line)) {
            s_line[s_line_len++] = c;
        } else {
            s_line_len = 0;
        }
    }
#endif
}

/** 状态机「应当发出的下一条初始化/连接命令」；SIM 与真机共用 */
static void kick_state_command(void)
{
    switch (s_state) {
    case AT_ST_INIT:
        at_sm_send_cmd_next("AT", AT_ST_CPIN);
        break;
    case AT_ST_CPIN:
        at_sm_send_cmd_next("AT+CPIN?", AT_ST_CEREG);
        break;
    case AT_ST_CEREG:
        at_sm_send_cmd_next("AT+CEREG?", AT_ST_MQTT_CFG);
        break;
    case AT_ST_MQTT_CFG:
        at_sm_send_cmd_next("AT+CMQTTSTART", AT_ST_MQTT_CONN);
        break;
    case AT_ST_MQTT_CONN:
        at_sm_send_cmd_next("AT+CMQTTCONNECT=0,1800", AT_ST_MQTT_SUB);
        break;
    case AT_ST_MQTT_SUB:
        at_sm_send_cmd_next("AT+CMQTTSUB=0,1,\"" APP_CLOUD_TOPIC_CMD_SUB "\",1", AT_ST_ONLINE);
        break;
    case AT_ST_BACKOFF:
        if ((osKernelGetTickCount() - s_state_enter_ms) > s_backoff_ms) {
            enter(AT_ST_INIT);
        }
        break;
    default:
        break;
    }
}

#if APP_CLOUD_SIM
static void sim_step(void)
{
    uint32_t now = osKernelGetTickCount();
    uint32_t elapsed = now - s_state_enter_ms;

    /* 周期模拟云端下行 force_report（不抢本地模式） */
    static uint32_t last_cmd_ms;
    if (s_state == AT_ST_ONLINE && (now - last_cmd_ms) > 15000) {
        last_cmd_ms = now;
        snprintf(s_urc_payload, sizeof(s_urc_payload),
                 "{\"cmd\":\"force_report\",\"seq\":%u,\"params\":{}}",
                 (unsigned)(now / 1000));
        s_urc_ready = 1;
    }

    /* INIT/CPIN/...：到点发命令；WAIT_OK：注入 OK */
    if (s_state == AT_ST_WAIT_OK) {
        if (elapsed > 30) {
            handle_line("OK");
        }
        return;
    }
    if (s_state == AT_ST_ONLINE || s_state == AT_ST_BACKOFF) {
        kick_state_command();
        return;
    }
    /* 初始化中间态：稍等再发，便于观察状态迁移 */
    if (elapsed > 40) {
        kick_state_command();
    }
}
#endif

void at_sm_poll(void)
{
    at_sm_feed();
#if APP_CLOUD_SIM
    sim_step();
#else
    /* 真机：INIT 与中间态主动发起下一条命令；WAIT_OK 只等应答 */
    if (s_state != AT_ST_WAIT_OK && s_state != AT_ST_ONLINE) {
        kick_state_command();
    }
#endif

    if (s_state == AT_ST_WAIT_OK) {
        uint32_t to = 2000;
        if (s_publish_inflight) {
            to = 3000;
        } else if (s_next_on_ok == AT_ST_MQTT_SUB || s_next_on_ok == AT_ST_ONLINE) {
            /* CONNECT/SUB 阶段可能更慢 */
            to = 8000;
        }
        if ((osKernelGetTickCount() - s_state_enter_ms) > to) {
            schedule_backoff();
        }
    }
}

at_state_t at_sm_state(void) { return s_state; }

uint8_t at_sm_is_online(void) { return s_state == AT_ST_ONLINE; }

uint32_t at_sm_backoff_ms(void) { return s_backoff_ms; }

int at_sm_take_publish(char *out, uint32_t max_len)
{
    if (!s_urc_ready || out == NULL) return 0;
    uint32_t n = (uint32_t)strlen(s_urc_payload);
    if (n >= max_len) n = max_len - 1;
    memcpy(out, s_urc_payload, n);
    out[n] = '\0';
    s_urc_ready = 0;
    return 1;
}

int at_sm_publish(const char *topic, const char *payload)
{
    if (topic == NULL || payload == NULL) return 0;
    if (!at_sm_is_online()) {
        if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
            g_SysData.uplink_fail++;
            osMutexRelease(g_DataMutex);
        }
        return 0;
    }
#if APP_CLOUD_SIM
    (void)topic;
    (void)payload;
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        g_SysData.uplink_ok++;
        osMutexRelease(g_DataMutex);
    }
    return 1;
#else
    /* 真机：写入 UART 并等 OK；不允许并发第二条 */
    if (s_state != AT_ST_ONLINE || s_publish_inflight) return 0;
    {
        char cmd[400];
        int plen = (int)strlen(payload);
        snprintf(cmd, sizeof(cmd), "AT+CMQTTPUB=0,0,%d,%s", plen, payload);
        s_publish_inflight = 1;
        at_sm_send_cmd_next(cmd, AT_ST_ONLINE);
    }
    (void)topic;
    return 1;
#endif
}
