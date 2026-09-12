/**
 * @file    DHT11.c
 * @brief   DHT11 单总线驱动（微秒延时用 DWT，不占用 TIM6 时基）
 */
#include "DHT11.h"

/* ---- DWT 微秒延时（Cortex-M4 DWT->CYCCNT）---- */
static void dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint8_t us)
{
    static uint8_t inited;
    if (!inited) {
        dwt_init();
        inited = 1;
    }
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = (uint32_t)us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < ticks) {
    }
}

void DHT11_start(void)
{
    DHT11_PIN_OUT_LOW;
    HAL_Delay(20);
    DHT11_PIN_OUT_HIGH;
    delay_us(40);
}

uint8_t DHT11_CHECK(void)
{
    uint8_t retry = 0;
    while (DHT11_PIN_IN == GPIO_PIN_SET && retry < 100) {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 0;
    retry = 0;
    while (DHT11_PIN_IN == GPIO_PIN_RESET && retry < 100) {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 0;
    retry = 0;
    while (DHT11_PIN_IN == GPIO_PIN_SET && retry < 100) {
        retry++;
        delay_us(1);
    }
    return (retry < 100) ? 1 : 0;
}

uint8_t DHT11_read_byte(uint8_t *out)
{
    uint8_t i, dat = 0;
    if (out == NULL) return 1;

    for (i = 0; i < 8; i++) {
        uint8_t retry = 0;
        /* 等低电平结束（bit 前导 50us） */
        while (DHT11_PIN_IN == GPIO_PIN_RESET) {
            if (++retry > 100) return 1;
            delay_us(1);
        }
        delay_us(40);
        if (DHT11_PIN_IN == GPIO_PIN_SET) {
            dat |= (uint8_t)(0x80U >> i);
        }
        retry = 0;
        /* 等高电平结束（bit 间隙） */
        while (DHT11_PIN_IN == GPIO_PIN_SET) {
            if (++retry > 100) return 1;
            delay_us(1);
        }
    }
    *out = dat;
    return 0;
}

uint8_t DHT11_read_data(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t buf[5];
    uint8_t i;

    if (temperature == NULL || humidity == NULL) return 1;

    DHT11_start();
    if (!DHT11_CHECK()) return 1;

    for (i = 0; i < 5; i++) {
        if (DHT11_read_byte(&buf[i]) != 0) return 1;
    }
    if ((uint8_t)(buf[0] + buf[1] + buf[2] + buf[3]) != buf[4]) return 1;

    *humidity   = buf[0];
    *temperature = buf[2];
    return 0;
}

/* 兼容旧符号 */
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humidity, uint8_t *check)
{
    uint8_t t = 0, h = 0;
    uint8_t r = DHT11_read_data(&t, &h);
    if (temp) *temp = t;
    if (humidity) *humidity = h;
    if (check) *check = (r == 0) ? (uint8_t)(t + h) : 0;
    return r;
}
