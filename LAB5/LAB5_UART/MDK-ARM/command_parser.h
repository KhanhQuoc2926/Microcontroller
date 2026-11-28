#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include "main.h"

typedef enum {
    CMD_NONE = 0,
    CMD_RST,
    CMD_OK
} Command;

/* flag = 1: co lenh hoan chinh; data cho biet RST hay OK. */
extern uint8_t command_flag;
extern Command command_data;
void CommandParser_Init(void);
void command_parser_fsm(void);
#endif