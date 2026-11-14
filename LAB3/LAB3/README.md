# Lab 3 - bản cơ bản theo EX5 Lab 1

Bản riêng để học, dùng if/else, switch/case, biến int, mảng và hàm GPIO HAL.
Không dùng struct trạng thái, bitmask sự kiện, con trỏ hàm hoặc tầng driver chung.

Mở MDK-ARM/LAB3_CO_BAN.uvprojx bằng Keil. Đã build bằng Arm Compiler6.23:
Mã C đã biên dịch lại và liên kết thành công ngày 27/10/2026. Có HEX trong Firmware.
Xem docs/KIEM_TRA_VA_SUA_CODE_20261027.md để đọc các chỉnh sửa mới.

Đây là cùng mạch Lab3 đã thiết kế từ EX5, không phải mạch EX5 nguyên trạng:
- Đèn HIGH sáng: NS R/Y/G=PA2/3/4; EW R/Y/G=PA5/6/7.
- Segment LOW sáng: NS a-g=PB0..6; EW a-g=PB7..13.
- Ba nút pull-up: PA8/9/10, nhấn nối GND.
- Chọn digit qua PNP, LOW bật: NS chục/đơn vị=PB14/15;
  EW chục/đơn vị=PA11/12. Common anode không nối trực tiếp VCC.
- HSI8MHz; TIM2 mặc định10ms PSC799/ARR99; SysTick1ms; giữ NOJTAG/SWD.

File người học viết:
1. output_display.c/h: giữ hai bảng mã và TrafficLight_Init của Lab1,
   thêm set_display lưu số và scan7SEG chọn hàng chục/đơn vị.
2. input_reading.c/h: đọc3 mẫu10ms liên tiếp để chống dội, phát hiện cạnh nhấn.
3. timer.c/h: đếm ms từ TIM2; SysTick kiểm tra mỗi1ms, quét digit5ms, đọc nút10ms.
4. input_processing.c/h: mode, bốn pha, nháp/lưu và nhấp nháy250ms.
main.c, tim.c và các file hệ thống giữ cấu trúc CubeMX.

Trong main sau MX_GPIO_Init và MX_TIM2_Init:
input_init(); output_init(); fsm_init(); timer_start();
Trong while(1): fsm_run();
SysTick_Handler sau HAL_IncTick(): timer_tick_1ms();
TIM2_IRQHandler gọi HAL_TIM_IRQHandler(&htim2).

Không ghép cùng app.c/lab_timer.c của bản trước, tránh trùng callback và hai
FSM cùng điều khiển LED. Giải nén toàn bộ trước khi mở .uvprojx.

Nguyên tắc:
- R=G+Y, mặc định5/2/3; SET tự tính lại màu liên quan để giữ quan hệ này.
- BTN1 đổi mode1->2->3->4->1. BTN2 tăng1..99, BTN3 lưu RAM.
- Chưa lưu đã đổi mode thì bỏ nháp; giữ INC hơn 1 giây sẽ tự tăng mỗi500ms.
- Một lần nhấn nhận sau3 mẫu ổn định, khoảng20-30ms; main phải chạy liên tục.
- button_pressed_event chỉ gọi từ main, một lần mỗi lượt fsm_run cho mỗi nút.
  Không dùng HAL_Delay/chờ nhả trong vòng main. Nếu main bị chặn lâu, có thể
  bỏ lỡ lần nhấn-nhả nhanh vì bản cơ bản không dùng hàng đợi sự kiện.
- Các số hiển thị là biến volatile dùng chung main/ngắt. Cập nhật hai cặp độc
  lập nên tại lúc đổi số có thể có một chu kỳ quét trộn số cũ/mới rất ngắn;
  bản cơ bản chấp nhận điều này, không thêm critical section hay buffer đôi.
- Đổi TIMER_CYCLE_MS trong timer.h để thử1/10/100ms, clock vẫn8MHz.
  SysTick giữ1ms giúp digit/nút và2Hz không phụ thuộc tickTIM2.
- Không dùng HEX mới với mạch cũ chưa có đường chọn digit.

Đã kiểm tra logic trên GCC với HAL giả cho1/10/100ms: pha/đếm lùi, chống dội,
giữ nút,2Hz, nháp/lưu,99->1, sai cấu hình/khôi phục và mứcLOW của quét.
Chưa thử Proteus hoặc bo thật. Không có .pdsprj Lab3 mới trong gói này.

Bản sửa MODE/INC/SET mới nhất: docs/SUA_MODE_INC_SET_20261027.md. HEX rõ tên: Firmware/LAB3_CO_BAN_NUT_MODE_INC_SET.hex.
