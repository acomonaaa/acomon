//
// Created by 高冲 on 26-4-21.
//

#ifndef DHT11_H
#define DHT11_H
#include "main.h"
/* * 简化引脚操作的宏定义
 * 开漏模式下：
 * 输出 0 -> 拉低总线
 * 输出 1 -> 释放总线（由外部电阻上拉），此时可读取电平
 */
#define DHT11_PIN_OUT_LOW HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET)
#define DHT11_PIN_OUT_HIGH HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)
#define DHT11_PIN_IN HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)

void delay_us(uint8_t us);
uint8_t DHT11_CHECK(void);
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humidity, uint8_t *check);
void DHT11_start(void);
uint8_t DHT11_read_byte(uint8_t *out);
uint8_t DHT11_read_data(uint8_t *temperature, uint8_t *humidity);

#endif //DHT11_H
