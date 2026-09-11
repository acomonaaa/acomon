/**
 * @file json_mini.c
 */
#include "json_mini.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint32_t json_pack_properties(char *buf, uint32_t cap,
                              float t, float h, float c, uint32_t lux,
                              uint8_t fan, uint8_t pump, uint8_t light_pct,
                              uint8_t auto_mode, uint8_t alarm, uint32_t seq)
{
    if (buf == NULL || cap < 32) return 0;
    int n = snprintf(buf, cap,
        "{\"services\":[{\"service_id\":\"agri\",\"properties\":{"
        "\"temperature\":%.1f,\"humidity\":%.1f,\"co2\":%.0f,\"lux\":%u,"
        "\"fan\":%u,\"pump\":%u,\"light\":%u,\"auto\":%u,\"alarm\":%u,\"seq\":%u}}]}",
        (double)t, (double)h, (double)c, (unsigned)lux,
        fan, pump, light_pct, auto_mode, alarm, (unsigned)seq);
    if (n <= 0 || (uint32_t)n >= cap) return 0;
    return (uint32_t)n;
}

uint32_t json_pack_ack(char *buf, uint32_t cap, const char *cmd, uint32_t seq, int result)
{
    if (buf == NULL || cap < 16) return 0;
    int n = snprintf(buf, cap,
        "{\"cmd\":\"%s\",\"seq\":%u,\"result\":%d}",
        cmd ? cmd : "ack", (unsigned)seq, result);
    if (n <= 0 || (uint32_t)n >= cap) return 0;
    return (uint32_t)n;
}

int json_get_str(const char *json, const char *key, char *out, uint32_t cap)
{
    if (json == NULL || key == NULL || out == NULL || cap == 0) return 0;
    char pat[48];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p) return 0;
    p = strchr(p + strlen(pat), ':');
    if (!p) return 0;
    p++;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '"') {
        p++;
        uint32_t i = 0;
        while (*p && *p != '"' && i + 1 < cap) out[i++] = *p++;
        out[i] = '\0';
        return 1;
    }
    return 0;
}

int json_get_num(const char *json, const char *key, double *val)
{
    if (json == NULL || key == NULL || val == NULL) return 0;
    char pat[48];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p) return 0;
    p = strchr(p + strlen(pat), ':');
    if (!p) return 0;
    p++;
    while (*p == ' ' || *p == '\t') p++;
    if (strncmp(p, "true", 4) == 0) { *val = 1.0; return 1; }
    if (strncmp(p, "false", 5) == 0) { *val = 0.0; return 1; }
    char *end = NULL;
    double d = strtod(p, &end);
    if (end == p) return 0;
    *val = d;
    return 1;
}

int json_parse_cmd(const char *json, cloud_cmd_t *out)
{
    if (json == NULL || out == NULL) return -1;
    memset(out, 0, sizeof(*out));

    if (!json_get_str(json, "cmd", out->cmd, sizeof(out->cmd))) {
        /* 兼容 properties 里的 action */
        if (!json_get_str(json, "action", out->cmd, sizeof(out->cmd))) return -1;
    }
    double seq = 0;
    json_get_num(json, "seq", &seq);
    out->seq = (uint32_t)seq;

    if (strcmp(out->cmd, "set_mode") == 0) {
        double a = 0;
        /* params.auto 或 auto */
        if (json_get_num(json, "auto", &a)) {
            out->auto_mode = (uint8_t)(a != 0);
            out->has_mode = 1;
        }
    } else if (strcmp(out->cmd, "set_threshold") == 0) {
        double v;
        if (json_get_num(json, "temp", &v)) { out->thr_temp = (float)v; out->has_temp = 1; out->has_thr = 1; }
        if (json_get_num(json, "humi", &v)) { out->thr_humi = (float)v; out->has_humi = 1; out->has_thr = 1; }
        if (json_get_num(json, "light", &v)) { out->thr_light = (uint32_t)v; out->has_light = 1; out->has_thr = 1; }
        if (json_get_num(json, "co2", &v)) { out->thr_co2 = (float)v; out->has_co2 = 1; out->has_thr = 1; }
    } else if (strcmp(out->cmd, "set_actuator") == 0) {
        double v;
        if (json_get_num(json, "fan", &v)) { out->fan = (uint8_t)(v != 0); out->has_act = 1; }
        if (json_get_num(json, "pump", &v)) { out->pump = (uint8_t)(v != 0); out->has_act = 1; }
        if (json_get_num(json, "light", &v)) { out->light_pct = (uint8_t)v; out->has_act = 1; }
    } else if (strcmp(out->cmd, "force_report") == 0) {
        out->has_force = 1;
    } else {
        return -1;
    }
    return 0;
}
