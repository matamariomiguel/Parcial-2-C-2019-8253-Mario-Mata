#ifndef UART_APP_H
#define UART_APP_H

#include <stdint.h>
#include <stdbool.h>

void     Uart_Init(void);
bool     Uart_ReadByte(uint8_t *b);      /* no bloqueante */
bool     Uart_TakeRxLost(void);          /* true si se perdieron bytes de RX */
uint32_t Uart_RxOverrunCount(void);      /* overrun de hardware */
uint32_t Uart_RxLostCount(void);         /* bytes perdidos por buffer lleno */

#endif
