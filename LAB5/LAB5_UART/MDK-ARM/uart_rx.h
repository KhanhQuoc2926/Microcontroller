#ifndef UART_RX_H
#define UART_RX_H
#include "main.h"

/* Bo dem chan doan: dem byte bi bo va so lan gap loi nhan. */
extern volatile uint32_t uart_rx_dropped;
extern volatile uint32_t uart_rx_errors;
void UART_RxInit(void);
void UART_RxService(void);
uint8_t UART_RxPop(uint8_t *byte);
void UART_RxFlush(void);
#endif