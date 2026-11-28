#include "command_parser.h"
#include "uart_rx.h"
#include <string.h>

/* WAIT_START: cho dau !. COLLECT: thu noi dung cho den dau #. */
typedef enum { WAIT_START, COLLECT } ParserState;
static ParserState state;
static char token[4];             /* Toi da 3 ky tu RST va 1 ky tu ket thuc \0. */
static uint8_t length;
static uint32_t seen_dropped;
static uint32_t seen_errors;
/* Hai bien nay trao doi giua 2 FSM trong main, khong phai giua main va ISR. */
uint8_t command_flag;
Command command_data;

void CommandParser_Init(void)
{
    state = WAIT_START;
    length = 0;
    command_flag = 0;
    command_data = CMD_NONE;
    seen_dropped = uart_rx_dropped;
    seen_errors = uart_rx_errors;
}

void command_parser_fsm(void)
{
    uint8_t byte;
    /* Neu mat byte/loi UART thi khung dang thu khong con dang tin cay. */
    if (seen_dropped != uart_rx_dropped || seen_errors != uart_rx_errors) {
        seen_dropped = uart_rx_dropped;
        seen_errors = uart_rx_errors;
        state = WAIT_START;
        length = 0;
        UART_RxFlush();
    }
    /* Khong ghi de lenh chua xu ly; moi luot chi doc toi da 1 byte. */
    if (command_flag || !UART_RxPop(&byte)) return;

    /* Dau ! moi luon cho bat dau lai, ke ca khi lenh truoc dang do. */
    if (byte == '!') {
        state = COLLECT;
        length = 0;
        return;
    }
    if (state != COLLECT) return; /* Bo qua nhieu va CR/LF ngoai khung lenh. */

    if (byte == '#') {
        token[length] = '\0';    /* Tao chuoi C hop le truoc khi strcmp. */
        /* strcmp tra 0 khi 2 chuoi bang nhau; co phan biet hoa/thuong. */
        if (strcmp(token, "RST") == 0) {
            command_data = CMD_RST; /* Noi dung lenh. */
            command_flag = 1;
        } else if (strcmp(token, "OK") == 0) {
            command_data = CMD_OK;
            command_flag = 1;
        }
        state = WAIT_START;
        length = 0;
        return;
    }
    /* Luon chua 1 o cho ky tu ket thuc; qua dai thi huy khung. */
    if (length < sizeof(token) - 1u) {
        token[length++] = (char)byte;
    } else {
        state = WAIT_START;
        length = 0;
    }
}