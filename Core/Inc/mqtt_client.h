/**
 * @file mqtt_client.h
 * @brief 基于 AT 的薄 MQTT 客户端（连接/订阅/发布委托 at_sm）
 */
#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#include <stdint.h>

void mqtt_client_init(void);
uint8_t mqtt_connected(void);
int mqtt_publish(const char *topic, const char *payload); /* 1=成功 */
void mqtt_subscribe_defaults(void);

#endif /* __MQTT_CLIENT_H__ */
