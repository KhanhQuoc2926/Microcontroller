#include "uart_communication.h"
#include "command_parser.h"
#include "sensor.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;
/* IDLE: cho yeu cau. WAIT_ACK: da gui ADC, cho !OK#. */
typedef enum { COMM_IDLE, COMM_WAIT_ACK } CommState;
static CommState state;
static char packet[20];          /* Giu nguyen goi ASCII de gui lai khi timeout. */
static uint16_t packet_length;   /* So byte can truyen, khong tinh ky tu \0. */
static uint32_t last_tx_ms;      /* Moc thoi gian tinh tu lan thu gui gan nhat. */
uint32_t adc_read_errors;
uint32_t uart_tx_errors;

/* Chi gui goi da luu; khong do lai ADC khi gui lai. Ham nay chay o main. */
static void SendSavedPacket(void)
{
    if (HAL_UART_Transmit(&huart2, (uint8_t *)packet,
                          packet_length, 100) != HAL_OK) {
        uart_tx_errors++;
    }
    /* Ke ca gui loi cung doi 3 giay truoc lan thu tiep, tranh gui lien tuc. */
    last_tx_ms = HAL_GetTick();
}

void UART_Communication_Init(void)
{
    state = COMM_IDLE;
    packet_length = 0;
    last_tx_ms = 0;
    adc_read_errors = 0;
    uart_tx_errors = 0;
}

void uart_communication_fsm(void)
{
    Command command = CMD_NONE;
    /* Lay lenh roi xoa co de moi lenh chi duoc xu ly mot lan. */
    if (command_flag) {
        command = command_data;
        command_flag = 0;
        command_data = CMD_NONE;
    }

    switch (state) {
    case COMM_IDLE:
        /* Chi !RST# trong IDLE moi tao mau ADC va phien giao tiep moi. */
        if (command == CMD_RST) {
            uint32_t sample;
            /* Neu do loi thi khong gui so 0 gia: 0 co the la gia tri ADC hop le. */
            if (Sensor_ReadOnce(&sample) != HAL_OK) {
                adc_read_errors++;
                break;
            }
            /* Tao chuoi; n la so ky tu can co, khong tinh \0. CR/LF de xuong dong. */
            int n = snprintf(packet, sizeof(packet), "!ADC=%lu#\r\n",
                             (unsigned long)sample);
            /* Khong truyen chuoi bi loi dinh dang hoac bi cat vi thieu cho. */
            if (n <= 0 || n >= (int)sizeof(packet)) {
                uart_tx_errors++;
                break;
            }
            packet_length = (uint16_t)n;
            state = COMM_WAIT_ACK;
            SendSavedPacket();
        }
        break;

    case COMM_WAIT_ACK:
        /* Uu tien ACK: nhan !OK# thi ket thuc phien, khong gui lai. */
        if (command == CMD_OK) {
            state = COMM_IDLE;
        } else if ((uint32_t)(HAL_GetTick() - last_tx_ms) >= 3000u) {
            /* Qua 3000 ms chua ACK: gui lai dung packet cu, khong lay mau moi. */
            SendSavedPacket();
        }
        /* !RST# trong WAIT_ACK bi bo qua de giu du lieu cua phien hien tai. */
        break;

    default:
        state = COMM_IDLE;
        break;
    }
}