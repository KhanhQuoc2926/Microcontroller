#include "uart_rx.h"
extern UART_HandleTypeDef huart2;

/* Ring buffer 30 o, dung duoc 29 o vi chua 1 o de phan biet day/rong. */
#define RX_SIZE 30u
static uint8_t rx_byte;
/* Cac bien nay duoc dung chung giua ngat UART va main. */
static volatile uint8_t rx_buffer[RX_SIZE];
static volatile uint8_t rx_write;
static volatile uint8_t rx_read;
static volatile uint8_t rx_rearm_pending;
volatile uint32_t uart_rx_dropped;
volatile uint32_t uart_rx_errors;

/* Dang ky nhan 1 byte bang ngat; ham tra ve ngay, khong cho du lenh. */
static void RxArm(void)
{
    if (HAL_UART_Receive_IT(&huart2, &rx_byte, 1) != HAL_OK) {
        uart_rx_errors++;
        rx_rearm_pending = 1;
    }
}

/* Chi goi mot lan sau MX_USART2_UART_Init, khong goi lai trong while. */
void UART_RxInit(void)
{
    rx_write = 0;
    rx_read = 0;
    rx_rearm_pending = 0;
    uart_rx_dropped = 0;
    uart_rx_errors = 0;
    RxArm();
}

/* Xu ly yeu cau bat lai nhan o main neu lan truoc that bai. */
void UART_RxService(void)
{
    if (rx_rearm_pending) {
        rx_rearm_pending = 0;
        RxArm();
    }
}

/* Chay trong ngat: chi cat byte vao buffer va bat nhan tiep.
 * Khong parse lenh, do ADC hay cho truyen UART trong callback nay. */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART2) return;
    /* Den cuoi mang thi quay ve o 0. next cham read nghia la buffer day. */
    uint8_t next = (uint8_t)((rx_write + 1u) % RX_SIZE);
    if (next == rx_read) {
        uart_rx_dropped++;       /* Bo byte moi, bao parser huy khung bi thieu. */
    } else {
        rx_buffer[rx_write] = rx_byte;
        rx_write = next;         /* Chi cong bo vi tri moi sau khi ghi xong byte. */
    }
    RxArm();
}

/* Bao loi cho main phuc hoi nhan va huy lenh dang thu do. */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        uart_rx_errors++;
        rx_rearm_pending = 1;
    }
}

/* Tra 1 neu lay duoc byte cu nhat; tra 0 neu rong. Khong doi byte moi. */
uint8_t UART_RxPop(uint8_t *byte)
{
    if (rx_read == rx_write) return 0;
    *byte = rx_buffer[rx_read];   /* Ghi ket qua vao bien cua ben goi. */
    rx_read = (uint8_t)((rx_read + 1u) % RX_SIZE);
    return 1;
}

void UART_RxFlush(void)
{
    /* Khoa ngat ngan khi bo du lieu cu; sau do khoi phuc dung trang thai cu. */
    uint32_t saved_mask = __get_PRIMASK();
    __disable_irq();
    rx_read = rx_write;           /* Hai index bang nhau: buffer duoc xem la rong. */
    __set_PRIMASK(saved_mask);
}