#ifndef UART_COMMUNICATION_H
#define UART_COMMUNICATION_H
#include "main.h"

/* Bo dem loi de theo doi trong debugger, khong gui them goi ngoai giao thuc. */
extern uint32_t adc_read_errors;
extern uint32_t uart_tx_errors;
void UART_Communication_Init(void);
void uart_communication_fsm(void);
#endif