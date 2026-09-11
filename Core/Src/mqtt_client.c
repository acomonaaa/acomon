#include "mqtt_client.h"
#include "at_sm.h"
#include "app_config.h"

void mqtt_client_init(void)
{
    at_sm_init();
}

uint8_t mqtt_connected(void)
{
    return at_sm_is_online();
}

int mqtt_publish(const char *topic, const char *payload)
{
    return at_sm_publish(topic, payload);
}

void mqtt_subscribe_defaults(void)
{
    /* 状态机 SUB 阶段已发送订阅；此处语义化占位便于扩展多主题 */
}
