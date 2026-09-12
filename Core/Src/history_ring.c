/**
 * @file history_ring.c
 * @brief 环形缓冲实现（互斥锁保护）
 *
 * 设计要点：
 *  - 固定 CAP，满时覆盖最旧样本（环形语义）
 *  - sent_seq：已成功上报到云端的最大 seq；next_unsent = sent_seq+1
 *  - 若被覆盖导致断档，next_unsent 自动跳到当前最旧 seq，避免续传脏数据
 */
#include "history_ring.h"
#include "app_config.h"
#include <string.h>

static history_sample_t s_buf[APP_HISTORY_CAP];
static uint32_t s_head;          /* 下一写入下标 */
static uint32_t s_count;
static uint32_t s_next_seq;      /* 下一个分配的 seq（从 1 起） */
static uint32_t s_sent_seq;      /* 已确认发送的最大 seq */
static osMutexId_t s_mutex;

void history_ring_init(void)
{
    memset(s_buf, 0, sizeof(s_buf));
    s_head = 0;
    s_count = 0;
    s_next_seq = 1;
    s_sent_seq = 0;
    if (s_mutex == NULL) {
        s_mutex = osMutexNew(NULL);
    }
}

void history_ring_push(const history_sample_t *s)
{
    if (s == NULL) return;
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return;

    s_buf[s_head].seq     = s_next_seq++;
    s_buf[s_head].ts_ms   = s->ts_ms;
    s_buf[s_head].temp    = s->temp;
    s_buf[s_head].humi    = s->humi;
    s_buf[s_head].co2     = s->co2;
    s_buf[s_head].light   = s->light;

    s_head = (s_head + 1) % APP_HISTORY_CAP;
    if (s_count < APP_HISTORY_CAP) {
        s_count++;
    } else {
        /* 覆盖了最旧样本：若 sent_seq 落后于被覆盖的 seq，提升游标防止重发已丢样本 */
        uint32_t oldest_seq = s_buf[s_head].seq;
        if (s_sent_seq + 1 < oldest_seq) {
            s_sent_seq = oldest_seq - 1;
        }
    }
    osMutexRelease(s_mutex);
}

static uint32_t idx_of_oldest(void)
{
    return (s_head + APP_HISTORY_CAP - s_count) % APP_HISTORY_CAP;
}

uint32_t history_ring_snapshot(history_sample_t *out, uint32_t max_cnt)
{
    if (out == NULL || max_cnt == 0) return 0;
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return 0;

    uint32_t n = (s_count < max_cnt) ? s_count : max_cnt;
    /* 取最新 n 条 */
    uint32_t start = idx_of_oldest();
    uint32_t skip  = s_count - n;
    for (uint32_t i = 0; i < n; i++) {
        out[i] = s_buf[(start + skip + i) % APP_HISTORY_CAP];
    }
    osMutexRelease(s_mutex);
    return n;
}

uint32_t history_ring_range(history_sample_t *out, uint32_t from_seq, uint32_t max_cnt)
{
    if (out == NULL || max_cnt == 0) return 0;
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return 0;

    uint32_t n = 0;
    uint32_t start = idx_of_oldest();
    for (uint32_t i = 0; i < s_count && n < max_cnt; i++) {
        const history_sample_t *e = &s_buf[(start + i) % APP_HISTORY_CAP];
        if (e->seq >= from_seq) {
            out[n++] = *e;
        }
    }
    osMutexRelease(s_mutex);
    return n;
}

void history_ring_mark_sent(uint32_t seq)
{
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return;
    if (seq > s_sent_seq) {
        s_sent_seq = seq;
    }
    osMutexRelease(s_mutex);
}

uint32_t history_ring_next_unsent(history_sample_t *out)
{
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return 0;

    uint32_t want = s_sent_seq + 1;
    uint32_t found_seq = 0;
    uint32_t start = idx_of_oldest();
    for (uint32_t i = 0; i < s_count; i++) {
        const history_sample_t *e = &s_buf[(start + i) % APP_HISTORY_CAP];
        if (e->seq >= want) {
            if (out) *out = *e;
            found_seq = e->seq;
            break;
        }
    }
    /* 断档（want 已被覆盖）：把 sent 提到最旧-1，下一轮从最旧续传 */
    if (found_seq == 0 && s_count > 0) {
        uint32_t oldest = s_buf[start].seq;
        if (oldest > s_sent_seq + 1) {
            s_sent_seq = oldest - 1;
        }
    }
    osMutexRelease(s_mutex);
    return found_seq;
}

uint32_t history_ring_count(void)
{
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return 0;
    uint32_t c = s_count;
    osMutexRelease(s_mutex);
    return c;
}

uint32_t history_ring_last_seq(void)
{
    if (osMutexAcquire(s_mutex, osWaitForever) != osOK) return 0;
    uint32_t s = (s_next_seq == 0) ? 0 : (s_next_seq - 1);
    osMutexRelease(s_mutex);
    return s;
}
