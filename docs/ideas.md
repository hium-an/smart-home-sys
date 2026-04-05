# Ghi chú ý tưởng — Smart Home System

> File này để ghi lại ý tưởng mở rộng, cải tiến, và thử nghiệm.
> Không phải tất cả ý tưởng đều cần làm — đây là nơi brainstorm tự do.
> Khi quyết định làm, chuyển sang `features.md` với mã F9, F10...

---

## Ý tưởng mở rộng chức năng

### 💡 Tự động đóng cửa sau X giây
- Sau khi `Door_Open()` thành công, dùng Timer0 đếm thêm 30 giây rồi gọi `Door_Close()`
- Thêm biến `g_door_open_time` đếm giây trong Timer0 ISR
- Hiển thị countdown trên LCD: `DONG CUA: 28s`

### 💡 Đổi mật khẩu từ UART
- Người dùng gõ lệnh `PASS:1234` qua Virtual Terminal
- `uart.c` nhận ký tự vào buffer, parse lệnh, gọi `Security_ChangePass()`
- Cần thêm UART_Receive() polling hoặc dùng ngắt RI

### 💡 Hiển thị nhiệt độ trên LCD thay vì 7-SEG
- Khi nhấn giữ BTN_TEMP_UP 2 giây → chuyển chế độ xem nhiệt độ trên LCD dòng 1
- `LCD: "NHIET DO: 30 C"`
- 7-SEG vẫn chạy multiplexing nền

### 💡 Đếm số lần vào/ra
- Mỗi lần mở cửa thành công → `g_entry_count++`
- Hiển thị trên UART: `[MM:SS] VAO LAN: 3`
- Reset khi tắt nguồn (RAM không có pin)

### 💡 Chế độ ban đêm tự động
- Từ phút 0→6h (mô phỏng bằng đếm g_min): BULB tự bật ở mức 60%, FAN tắt
- Cần thêm hằng số `NIGHT_START_MIN`, `NIGHT_END_MIN` trong `config.h`

### 💡 Dùng LED-BIBY làm chỉ thị trạng thái hệ thống
- LED-BIBY (LED nhấp nháy) gắn vào một chân dự phòng
- Nhấp nháy chậm (1Hz): hệ thống đang chờ đăng nhập
- Nhấp nháy nhanh (5Hz): báo động
- Sáng liên tục: đã đăng nhập thành công

---

## Ý tưởng cải tiến kỹ thuật

### 🔧 Lưu mật khẩu bền hơn
- AT89C52 không có EEPROM nội — hiện tại mật khẩu mất khi reset
- Giải pháp: thêm IC EEPROM ngoài (24C02, giao tiếp I2C) vào P1 bit-bang
- Chưa có trong BOM — đánh dấu để bổ sung nếu cần nâng điểm

### 🔧 Tách BUZZER và SPEAKER ra 2 chân riêng
- Hiện tại BUZZER + SPEAKER cùng P2.5 → không thể phát 2 loại âm thanh đồng thời
- Nếu tách: BUZZER → P2.5, SPEAKER → chân khác (cần kiểm tra chân trống)
- Lợi ích: báo động BUZZER trong khi SPEAKER vẫn phát nhạc nền

### 🔧 Thêm nút RESET mật khẩu bằng tổ hợp phím
- Giữ BTN2 + BTN3 cùng lúc 5 giây → reset mật khẩu về `{1,2,3,4}`
- Không cần reset cả chip khi quên mật khẩu

### 🔧 PWM quạt nhiều mức (thay vì ON/OFF)
- Hiện tại FAN chỉ bật/tắt hoàn toàn
- Nếu dùng software PWM tương tự BULB: FAN chạy 3 mức theo nhiệt độ
  - 26–30°C → 33% (mát nhẹ)
  - 30–40°C → 66%
  - >40°C → 100%
- Cần thêm logic trong Timer0 ISR và thêm biến `g_fan_speed`

---

## Ý tưởng linh kiện chưa dùng (có trong BOM)

| Linh kiện | Ý tưởng sử dụng |
|:----------|:----------------|
| `LED-B` | Chỉ thị trạng thái Bluetooth/UART đang nhận dữ liệu |
| `LED-BIBY` | LED nhấp nháy chỉ thị trạng thái hệ thống (xem ý tưởng trên) |
| `SWITCH 2` | Chế độ ban đêm thủ công (gạt ON = bật chế độ đêm) |
| `BC557` (PNP) | Thay thế BC547 nếu cần logic nghịch (active-high trigger) |
| `2N1711` | Transistor NPN điện áp cao — dùng cho tải 12V nếu nâng nguồn |
| `DS1802` | Chiết áp kỹ thuật số — điều chỉnh âm lượng SPEAKER qua SPI/I2C |
| `7SEG-MPX4-CA` | Hiển thị thêm thông tin (4 chữ số): thời gian HHMM hoặc nhiệt độ + độ sáng |
| `MOTOR-DC` | Thêm motor thứ 2 (ví dụ: rèm cửa tự động) |

---

## Ghi chú nhanh

<!-- Ghi chú tự do, xóa/thêm thoải mái -->

- [ ] Kiểm tra Sound_AlarmTick có nghe rõ trong Proteus không (có thể cần chỉnh tần số)
- [ ] Thử tăng DOOR_PULSES lên 100 nếu motor quay quá nhanh trong mô phỏng
- [ ] Xem xét LED-YELLOW có nhấp nháy đủ nhanh để thấy trong mô phỏng không (tốc độ quét 7-SEG ~250Hz)
