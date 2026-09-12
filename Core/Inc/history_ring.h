/**
 * @file history_ring.h
 * @brief 定长环形缓冲：本地曲线 + 断网续传（双用途）
 */
#ifndef __HISTORY_RING_H__
#define __HISTORY_RING_H__

#include <stdint.h>
#include "cmsis_os2.h"

typedef struct {
    uint32_t seq;       /* 单调递增序号，云 ACK/去重用 */
    uint32_t ts_ms;     /* 采样时刻 osKernelGetTickCount */
    float    temp;
    float    humi;
    float    co2;
    uint32_t light;
} history_sample_t;

void history_ring_init(void);

/** 推入一条新样本，seq 自增；满则覆盖最旧并回退未发送游标保护 */
void history_ring_push(const history_sample_t *s);

/** 按下标 0=最旧..count-1 最新 拷出 count 条（内部加锁） */
uint32_t history_ring_snapshot(history_sample_t *out, uint32_t max_cnt);

/** 读取 [from_idx, to_idx] 闭区间（环形逻辑下标），用于续传 */
uint32_t history_ring_range(history_sample_t *out, uint32_t from_seq, uint32_t max_cnt);

/** 将 seq 标记为已发送（游标推进） */
void history_ring_mark_sent(uint32_t seq);

/** 下一个待发送的 seq；若无未发送数据返回 0 */
uint32_t history_ring_next_unsent(history_sample_t *out);

uint32_t history_ring_count(void);
uint32_t history_ring_last_seq(void);

#endif /* __HISTORY_RING_H__ */
