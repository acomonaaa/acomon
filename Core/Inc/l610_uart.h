/**
 * @file l610_uart.h
 * @brief L610 模组 USART1 驱动（中断收发环形缓冲）
 */
#ifndef __L610_UART_H__
#define __L610_UART_H__

#include <stdint.h>

void l610_uart_init(void);
uint32_t l610_uart_write(const uint8_t *data, uint32_t len);
uint32_t l610_uart_read(uint8_t *out, uint32_t max_len);
uint32_t l610_uart_rx_available(void);

#endif /* __L610_UART_H__ */
