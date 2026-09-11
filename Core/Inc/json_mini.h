/**
 * @file json_mini.h
 * @brief 极简 JSON 打包/解析（属性上报 + 云命令）
 */
#ifndef __JSON_MINI_H__
#define __JSON_MINI_H__

#include <stdint.h>

/** 打包一帧属性上报，返回写入长度；失败返回 0 */
uint32_t json_pack_properties(char *buf, uint32_t cap,
                              float t, float h, float c, uint32_t lux,
                              uint8_t fan, uint8_t pump, uint8_t light_pct,
                              uint8_t auto_mode, uint8_t alarm, uint32_t seq);

/** 打包 ACK */
uint32_t json_pack_ack(char *buf, uint32_t cap, const char *cmd, uint32_t seq, int result);

/**
 * 解析云命令。
 * 支持: set_mode / set_threshold / set_actuator / force_report
 * 返回 0 成功；seq 输出到 out_seq
 */
typedef struct {
    char cmd[32];
    uint32_t seq;
    /* set_mode */
    uint8_t auto_mode;
    /* set_threshold */
    float thr_temp, thr_humi, thr_co2;
    uint32_t thr_light;
    /* set_actuator */
    uint8_t fan, pump;
    uint8_t light_pct;
    uint8_t has_mode, has_thr, has_act, has_force;
} cloud_cmd_t;

int json_parse_cmd(const char *json, cloud_cmd_t *out);

/** 从 "key":value 粗提取（数字/bool/字符串值，不含引号） */
int json_get_num(const char *json, const char *key, double *val);
int json_get_str(const char *json, const char *key, char *out, uint32_t cap);

#endif /* __JSON_MINI_H__ */
