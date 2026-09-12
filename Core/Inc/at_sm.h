/**
 * @file at_sm.h
 * @brief L610 AT 命令状态机：应答与 URC 分流 + 指数退避
 */
#ifndef __AT_SM_H__
#define __AT_SM_H__

#include <stdint.h>

typedef enum {
    AT_ST_INIT = 0,
    AT_ST_WAIT_OK,
    AT_ST_CPIN,
    AT_ST_CEREG,
    AT_ST_MQTT_CFG,
    AT_ST_MQTT_CONN,
    AT_ST_MQTT_SUB,
    AT_ST_ONLINE,
    AT_ST_BACKOFF,
    AT_ST_ERROR
} at_state_t;

typedef enum {
    AT_EVT_NONE = 0,
    AT_EVT_OK,
    AT_EVT_ERROR,
    AT_EVT_URC_PUBLISH,
    AT_EVT_TIMEOUT
} at_event_t;

void at_sm_init(void);
void at_sm_poll(void);                 /* cloudTask 周期调用 */
at_state_t at_sm_state(void);
uint8_t at_sm_is_online(void);
uint32_t at_sm_backoff_ms(void);

/** 发送一条 AT 命令并进入 WAIT_OK；OK 后进入 next_on_ok */
void at_sm_send_cmd_next(const char *cmd, at_state_t next_on_ok);

/** 从 UART 拉字节喂给状态机（内部调用） */
void at_sm_feed(void);

/** 提取最近一条 MQTT publish URC 负载（QoS0 上报） */
int at_sm_take_publish(char *out, uint32_t max_len);

/** 发布 payload 到 topic；返回 1=成功，0=失败 */
int at_sm_publish(const char *topic, const char *payload);

#endif /* __AT_SM_H__ */
