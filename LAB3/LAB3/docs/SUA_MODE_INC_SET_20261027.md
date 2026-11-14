# Sửa MODE / INC / SET theo source tham khảo - 27/10/2026

Các file chính thay đổi: Core/Src/input_reading.c, Core/Inc/input_reading.h, Core/Src/input_processing.c. Timer và mạch giữ chân của project bạn: nút PA8/PA9/PA10; đèn PA2..PA7 sáng mức HIGH; segment PB0..PB13; chọn digit PB14/PB15/PA11/PA12 mức LOW.

Đối chiếu LAB3_TRAFFIC_SOURCE_FINAL.zip:
- Source tham khảo dùng đèn giao thông active LOW và chân PA1..PA6. Project này giữ active HIGH và PA2..PA7 theo mạch LAB3.pdsprj của bạn.
- Đã thêm ghi nhớ sự kiện nhấn ở hàm đọc nút; sự kiện không bị mất nếu nút đã nhả trước khi main lấy sự kiện. Vẫn dùng ba mẫu liên tiếp cách nhau 10 ms.
- INC tăng một lần khi nhấn; giữ hơn 1 giây thì tự lặp mỗi 500 ms, lần lặp đầu khoảng 1,5 giây như source tham khảo.
- SET ở mode 2: lưu đỏ và tính xanh = đỏ - vàng, chỉ nhận đỏ > vàng.
- SET ở mode 3: lưu vàng và tính xanh = đỏ - vàng, chỉ nhận vàng < đỏ.
- SET ở mode 4: lưu xanh và tính đỏ = xanh + vàng, chỉ nhận tổng <= 99.
- Giá trị không hợp lệ: trả màn hình về giá trị đã lưu. SET không tự chuyển mode; dùng MODE để chuyển tiếp.
- Mỗi màu đang chỉnh bật/tắt mỗi 250 ms (2 Hz). Nút MODE giữ lâu chỉ chuyển một lần.

Đã chạy kiểm thử HAL giả với TIM2 1/10/100 ms: MODE/INC/SET, ba màu nhấp nháy đúng 2 Hz, giữ INC tự tăng, ghi nhớ sự kiện nhấn, lưu thời gian đúng R=G+Y, từ chối giá trị sai, đếm lùi, quét và tràn bộ đếm thời gian. Tất cả PASS. Đã biên dịch với Arm Compiler 6.23 và liên kết thành công; log LINK_NUT_MODE_INC_SET.log.

Chưa xác nhận lại bằng thao tác trong Proteus vì người dùng nhấn Esc dừng điều khiển ứng dụng. Các phép thử trên máy chứng minh logic, chưa chứng minh nguyên nhân duy nhất khiến nút trong phiên Proteus trước đó không nhận.

Nạp lại đúng bản:
1. Nhấn STOP trong Proteus (không chỉ Pause).
2. Chọn U1 > Program File = Firmware/LAB3_CO_BAN_NUT_MODE_INC_SET.hex.
3. Nhấn Run lại từ đầu. Khi mô phỏng chạy chậm, giữ mỗi nút đủ 30 ms thời gian mô phỏng (có thể cần giữ chuột lâu hơn ngoài đời).

HEX chuẩn MDK-ARM/Objects/LAB3_CO_BAN.hex cũng đã được cập nhật cùng nội dung. Cả thư mục gốc và thư mục giải nén đang mở trong Keil đã đồng bộ. Khi Keil hỏi file thay đổi từ bên ngoài, chọn Reload.
