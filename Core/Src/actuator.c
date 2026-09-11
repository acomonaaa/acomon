/**
 * @file actuator.c
 * @brief GPIO 执行器 + 补光灯软 PWM
 */
#include "actuator.h"
#include "main.h"

static uint8_t s_duty;
static uint8_t s_pwm_phase;
static uint8_t s_alarm_on;

void actuator_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOG_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOG, FAN_Pin | PUMP_Pin | ALARM_LED_Pin | BUZZER_Pin | GROW_LIGHT_Pin,
                      GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = FAN_Pin | PUMP_Pin | ALARM_LED_Pin | BUZZER_Pin | GROW_LIGHT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    s_duty = 0;
    s_pwm_phase = 0;
    s_alarm_on = 0;
}

void actuator_set(actuator_id_t id, uint8_t on)
{
    GPIO_PinState st = on ? GPIO_PIN_SET : GPIO_PIN_RESET;
    switch (id) {
    case ACT_FAN:        HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, st); break;
    case ACT_PUMP:       HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, st); break;
    case ACT_GROW_LIGHT: /* 由 PWM tick 驱动 */ s_duty = on ? 100 : 0; break;
    case ACT_ALARM:      actuator_alarm_set(on); break;
    default: break;
    }
}

void actuator_set_light_duty(uint8_t duty_pct)
{
    if (duty_pct > 100) duty_pct = 100;
    s_duty = duty_pct;
}

void actuator_pwm_tick(void)
{
    /* 200ms 任务内 10 步相位 → 等效 5Hz 软 PWM，足够演示补光调光 */
    s_pwm_phase = (uint8_t)((s_pwm_phase + 1) % 10);
    uint8_t on = (s_pwm_phase * 10 < s_duty) ? 1 : 0;
    HAL_GPIO_WritePin(GROW_LIGHT_GPIO_Port, GROW_LIGHT_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void actuator_alarm_set(uint8_t on)
{
    s_alarm_on = on;
    HAL_GPIO_WritePin(ALARM_LED_GPIO_Port, ALARM_LED_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
