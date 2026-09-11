/**
 * @file l610_uart.c
 * @brief USART1 115200 8N1，RX 中断写入环形缓冲
 */
#include "l610_uart.h"
#include "app_config.h"
#include "stm32f4xx_hal.h"
#include <string.h>

UART_HandleTypeDef huart1;

static uint8_t s_rx_ring[APP_UART_RX_BUF];
static volatile uint16_t s_rx_head, s_rx_tail;
static volatile uint8_t s_rx_byte; /* 静态 IT 接收缓冲，禁止用栈变量 */

void l610_uart_init(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef g = {0};
    g.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    g.Mode = GPIO_MODE_AF_PP;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    g.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &g);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = APP_L610_BAUD;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);

    HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    s_rx_head = s_rx_tail = 0;
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&s_rx_byte, 1);
}

/* ISR：单字节收，存入 ring；满则丢弃最新 */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART1) return;
    uint16_t next = (uint16_t)((s_rx_head + 1) % APP_UART_RX_BUF);
    if (next != s_rx_tail) {
        s_rx_ring[s_rx_head] = s_rx_byte;
        s_rx_head = next;
    }
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&s_rx_byte, 1);
}

uint32_t l610_uart_write(const uint8_t *data, uint32_t len)
{
    if (data == NULL || len == 0) return 0;
    if (HAL_UART_Transmit(&huart1, (uint8_t *)data, (uint16_t)len, 200) != HAL_OK) {
        return 0;
    }
    return len;
}

uint32_t l610_uart_read(uint8_t *out, uint32_t max_len)
{
    uint32_t n = 0;
    while (n < max_len && s_rx_tail != s_rx_head) {
        out[n++] = s_rx_ring[s_rx_tail];
        s_rx_tail = (uint16_t)((s_rx_tail + 1) % APP_UART_RX_BUF);
    }
    return n;
}

uint32_t l610_uart_rx_available(void)
{
    if (s_rx_head >= s_rx_tail) return s_rx_head - s_rx_tail;
    return APP_UART_RX_BUF - s_rx_tail + s_rx_head;
}
