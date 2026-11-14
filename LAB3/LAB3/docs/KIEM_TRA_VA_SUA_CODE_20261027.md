# Kiểm tra code ngày 27/10/2026

Đã sửa trực tiếp trong project LAB3_CO_BAN_FROM_EX5, giữ nguyên sơ đồ chân.

1. input_reading.c: trước đây chỉ so sánh sample0 với sample1, nên sample2 không tham gia chống dội. Đã yêu cầu cả ba mẫu liên tiếp giống nhau trước khi cập nhật stable.
2. timer.c: trước đây quét LED và đọc nút chung mỗi 10 ms, tương ứng 50 Hz cho mỗi digit. Đã tách scan_counter để quét mỗi 5 ms (100 Hz/digit); button_counter vẫn đọc nút mỗi 10 ms. TIM2 vẫn chu kỳ TIMER_CYCLE_MS, mặc định 10 ms.
3. Sửa chú thích chu kỳ TIM2 và chu kỳ quét LED cho khớp code.

FSM và bảng mã 7 đoạn được giữ nguyên. Bộ thời gian chạy bình thường phải thỏa red_time = green_time + yellow_time; nếu sai code chủ động bật đỏ hai hướng và hiện -- --.

Đã chạy kiểm thử logic với TIM2 1/10/100 ms: chuyển pha/đếm giây, chống dội ba mẫu (kể cả hai mẫu LOW chưa được công nhận), giữ nút, tăng/lưu/bỏ nháp, giới hạn 1..99, nhấp nháy 2 Hz, quét active LOW và tắt digit trước khi đổi segment. Cả ba cấu hình đều PASS. Đây là kiểm thử HAL giả trên máy tính, chưa chạy project này trên bo thật hoặc Proteus trong lượt sửa này.

Đã biên dịch lại toàn bộ 22 file C bằng Arm Compiler 6.23 và liên kết thành công. Log: BUILD_SUA_CODE_20261027.log và LINK_SUA_CODE_20261027.log.

HEX: Firmware/LAB3_CO_BAN.hex; Keil xuất tại MDK-ARM/Objects/LAB3_CO_BAN.hex. Project đã bật Create HEX File. Mở MDK-ARM/LAB3_CO_BAN.uvprojx, cho Keil nạp lại file đã thay đổi từ đĩa, rồi nhấn F7 để build.

Chân của bản này: đèn PA2..PA7; nút PA8/PA9/PA10; segment PB0..PB13; chọn digit PB14/PB15 và PA11/PA12. File HEX này không khớp mạch LAB3_FROM_EX5_PROTEUS_COMPLETE, vốn có ánh xạ GPIO khác. Cần dùng mạch đấu đúng sơ đồ chân này.

Bản sao các file trước khi sửa nằm trong docs/backup_truoc_sua_20261027.
