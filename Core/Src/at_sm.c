/**
 * @file at_sm.c
 * @brief L610 AT 状态机实现
 *
 * 应答（OK/ERROR）与 URC（+CMQTTPUBLISH 等）分流；
 * 失败指数退避；APP_CLOUD_SIM=1 时用仿真应答驱动全流程。
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
static uint32_t s_state_enter_ms;
static uint32_t s_backoff_ms = APP_BACKOFF_MIN_MS;
static char s_line[256];
static uint32_t s_line_len;
static char s_pending_cmd[128];
static char s_pub_topic[128];
static char s_pub_payload[192];
static uint8_t s_pub_pending;

static char s_urc_payload[256];
static uint8_t s_urc_ready;

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
    s_state = AT_ST_INIT;
    s_backoff_ms = APP_BACKOFF_MIN_MS;
    s_line_len = 0;
    s_urc_ready = 0;
    s_pub_pending = 0;
    enter(AT_ST_INIT);
}

void at_sm_send_cmd(const char *cmd)
{
    if (cmd == NULL) return;
    snprintf(s_pending_cmd, sizeof(s_pending_cmd), "%s\r\n", cmd);
#if APP_CLOUD_SIM
    /* 仿真：不真正写 UART，由 poll 注入 OK */
    (void)0;
#else
    l610_uart_write((const uint8_t *)s_pending_cmd, (uint32_t)strlen(s_pending_cmd));
#endif
    enter(AT_ST_WAIT_OK);
}

static void schedule_backoff(void)
{
    s_backoff_ms <<= 1;
    if (s_backoff_ms > APP_BACKOFF_MAX_MS) s_backoff_ms = APP_BACKOFF_MAX_MS;
    enter(AT_ST_BACKOFF);
}

static void handle_urc_line(const char *line)
{
    /* +CMQTTPUBLISH: <mid>,0,<topic_len>,<topic>,<payload_len>,<payload> */
    const char *p = strstr(line, "+CMQTTPUBLISH:");
    if (p == NULL) return;
    /* 简化解析：从第一个 '{' 取 JSON 负载 */
    const char *brace = strchr(p, '{');
    if (brace) {
        size_t n = strlen(brace);
        if (n >= sizeof(s_urc_payload)) n = sizeof(s_urc_payload) - 1;
        memcpy(s_urc_payload, brace, n);
        s_urc_payload[n] = '\0';
        s_urc_ready = 1;
    }
}

static void handle_line(const char *line)
{
    if (line == NULL || line[0] == '\0') return;

    if (strncmp(line, "+CMQTTPUBLISH:", 14) == 0) {
        handle_urc_line(line);
        return;
    }
    if (strcmp(line, "OK") == 0) {
        /* 按当前状态推进 */
        switch (s_state) {
        case AT_ST_WAIT_OK:
            /* 根据上一条命令决定下一状态：用 pending 粗判 */
            if (strstr(s_pending_cmd, "AT+CPIN?")) enter(AT_ST_CEREG);
            else if (strstr(s_pending_cmd, "AT+CEREG")) enter(AT_ST_MQTT_CFG);
            else if (strstr(s_pending_cmd, "AT+CMQTTSTART") || strstr(s_pending_cmd, "AT+CMQTTACCQ"))
                enter(AT_ST_MQTT_CONN);
            else if (strstr(s_pending_cmd, "AT+CMQTTCONNECT")) enter(AT_ST_MQTT_SUB);
            else if (strstr(s_pending_cmd, "AT+CMQTTSUB")) enter(AT_ST_ONLINE);
            else if (strstr(s_pending_cmd, "AT+CMQTTPUB")) {
                if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
                    g_SysData.uplink_ok++;
                    osMutexRelease(g_DataMutex);
                }
                enter(AT_ST_ONLINE);
            } else enter(AT_ST_ONLINE);
            if (s_state == AT_ST_ONLINE) s_backoff_ms = APP_BACKOFF_MIN_MS;
            break;
        default:
            break;
        }
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
            s_line_len = 0; /* 溢出丢弃 */
        }
    }
#endif
}

#if APP_CLOUD_SIM
static void sim_step(void)
{
    uint32_t now = osKernelGetTickCount();
    uint32_t elapsed = now - s_state_enter_ms;

    /* 周期性模拟云端下行 force_report，便于演示去重/ACK（不抢本地模式） */
    static uint32_t last_cmd_ms;
    if (s_state == AT_ST_ONLINE && (now - last_cmd_ms) > 15000) {
        last_cmd_ms = now;
        snprintf(s_urc_payload, sizeof(s_urc_payload),
                 "{\"cmd\":\"force_report\",\"seq\":%u,\"params\":{}}",
                 (unsigned)(now / 1000));
        s_urc_ready = 1;
    }

    switch (s_state) {
    case AT_ST_INIT:
        if (elapsed > 50) {
            snprintf(s_pending_cmd, sizeof(s_pending_cmd), "AT");
            enter(AT_ST_WAIT_OK);
            /* 下一 poll 直接 OK 推进到 CPIN */
        }
        break;
    case AT_ST_WAIT_OK:
        if (elapsed > 30) {
            handle_line("OK");
        }
        break;
    case AT_ST_CPIN:
        if (elapsed > 50) {
            snprintf(s_pending_cmd, sizeof(s_pending_cmd), "AT+CEREG?");
            enter(AT_ST_WAIT_OK);
        }
        break;
    case AT_ST_CEREG:
        if (elapsed > 50) {
            snprintf(s_pending_cmd, sizeof(s_pending_cmd), "AT+CMQTTSTART");
            enter(AT_ST_WAIT_OK);
        }
        break;
    case AT_ST_MQTT_CFG:
        if (elapsed > 50) {
            snprintf(s_pending_cmd, sizeof(s_pending_cmd), "AT+CMQTTCONNECT");
            enter(AT_ST_WAIT_OK);
        }
        break;
    case AT_ST_MQTT_CONN:
        if (elapsed > 50) {
            snprintf(s_pending_cmd, sizeof(s_pending_cmd), "AT+CMQTTSUB");
            enter(AT_ST_WAIT_OK);
        }
        break;
    case AT_ST_MQTT_SUB:
        if (elapsed > 50) {
            snprintf(s_pending_cmd, sizeof(s_pending_cmd), "AT");
            enter(AT_ST_WAIT_OK);
            /* handle_line(OK) 会把含 CMQTTSUB 的 pending 升到 ONLINE */
        }
        break;
    case AT_ST_ONLINE:
        break;
    case AT_ST_BACKOFF:
        /* 退避结束后重连；不在此重置退避间隔，仅 ONLINE 成功后重置 → 真正 1s→60s */
        if (elapsed > s_backoff_ms) {
            enter(AT_ST_INIT);
        }
        break;
    default:
        break;
    }
}
#endif

void at_sm_poll(void)
{
    at_sm_feed();
#if APP_CLOUD_SIM
    sim_step();
#endif

    /* 超时：WAIT_OK 停留过久视为失败 */
    if (s_state == AT_ST_WAIT_OK) {
        if ((osKernelGetTickCount() - s_state_enter_ms) > 2000) {
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
    snprintf(s_pub_topic, sizeof(s_pub_topic), "%s", topic);
    snprintf(s_pub_payload, sizeof(s_pub_payload), "%s", payload);
    if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
        g_SysData.uplink_ok++;
        osMutexRelease(g_DataMutex);
    }
    return 1;
#else
    /* 真机路径：发送后由 OK 推进；此处按在线即尝试成功交由上层语义简化 */
    char cmd[320];
    int plen = (int)strlen(payload);
    snprintf(cmd, sizeof(cmd), "AT+CMQTTPUB=0,0,%d,%s", plen, payload);
    at_sm_send_cmd(cmd);
    (void)s_pub_topic;
    (void)s_pub_payload;
    return 1;
#endif
}
