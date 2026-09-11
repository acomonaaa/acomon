/**
 * @file actuator.h
 * @brief 执行器与声光报警板级封装
 */
#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

#include <stdint.h>

typedef enum {
    ACT_FAN = 0,
    ACT_PUMP,
    ACT_GROW_LIGHT,
    ACT_ALARM,      /* 声光一体 */
    ACT_MAX
} actuator_id_t;

void actuator_init(void);
void actuator_set(actuator_id_t id, uint8_t on);
void actuator_set_light_duty(uint8_t duty_pct); /* 0~100，控制任务内软 PWM 推进 */
void actuator_pwm_tick(void);                   /* 由 controlTask 高频调用 */
void actuator_alarm_set(uint8_t on);

#endif /* __ACTUATOR_H__ */
