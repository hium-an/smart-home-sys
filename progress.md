# Tiến độ thực hiện — Smart Home System

> Cập nhật lần cuối: 2026-04-02 | Nhóm: Group One | HKII

---

## ✅ Đã hoàn thành

### Tổ chức project
- [x] Cập nhật `CLAUDE.md` sang AT89C52 (bỏ ARM Cortex-M)
- [x] Tạo cấu trúc thư mục: `src/`, `inc/`, `asm/`, `samples/`, `docs/`

### Headers (`inc/`)
- [x] `config.h` — định nghĩa toàn bộ chân, hằng số Timer, ngưỡng
- [x] `lcd.h` — API LCD 16×2
- [x] `uart.h` — API UART log
- [x] `door.h` — API điều khiển cửa
- [x] `sensor.h` — API cảm biến
- [x] `security.h` — API bảo mật

### Source code (`src/`)
- [x] `lcd.c` — driver LCD 4-bit (D4-D7 = P3.4-P3.7, RS=P2.6, E=P2.7)
- [x] `uart.c` — UART 9600 8N1 + UART_Log với timestamp
- [x] `door.c` — motor L293D + encoder INT0 ISR
- [x] `sensor.c` — SWITCH 1 mô phỏng ánh sáng (thay LDR+LM358) + nhiệt độ mô phỏng nút bấm
- [x] `security.c` — đăng nhập 4 số, lockdown, đổi mật khẩu
- [x] `main.c` — super-loop tích hợp 8 chức năng + Timer0 ISR 1ms
- [x] `asm/delay.asm` — hàm delay chính xác theo chu kỳ máy

### Code mẫu (`samples/`)
- [x] `bai1_led_button.c` — LED + nút bấm cơ bản
- [x] `bai2_7seg_dem.c` — 7-SEG 1 digit, đếm nút bấm
- [x] `bai3_7seg_4chu.c` — 7-SEG 4 digit, multiplexing
- [x] `bai4_encoder_motor.c` — encoder + Timer0 + đo tốc độ
- [x] `bai5_dual_lcd.c` — 2 LCD độc lập
- [x] `bai6_uart_fan.c` — UART + mật khẩu + quạt
- [x] `bai7_uart_buttons.c` — gửi ký tự UART từ nút bấm (C)
- [x] `bai7_uart.asm` — gửi ký tự UART từ nút bấm (Assembly)
- [x] `bai9_full_system.c` — hệ thống đầy đủ 1 file (mẫu tham khảo)
- [x] `bai10_mo_rong.c` — 7-SEG và LCD tách biệt hoàn toàn

### Tài liệu (`docs/`)
- [x] `wiring_guide.md` — bảng pinout + sơ đồ đấu nối chi tiết từng ngoại vi
- [x] `code_explained.md` — giải thích code từng module, tính toán timer/UART

---

## 🔄 Chưa thực hiện / Còn lại

### Mở rộng (đã hoàn thành thêm)
- [x] Thêm BTN_BRIGHT_UP (P3.1), BTN_BRIGHT_DN (P3.3) thay thế POT
- [x] Software PWM 5 mức độ sáng cho BULB (trong Timer0 ISR)
- [x] Module sound.c: SPEAKER + BUZZER cùng P2.5, giai điệu chào + bip cửa + bao động
- [x] LED-G (song song BULB), LED-R (song song BUZZER), LED-YELLOW (song song digit)
- [x] docs/proteus_keil_setup.md — hướng dẫn dựng mạch + cài Keil đầy đủ
- [x] docs/features.md — mô tả 8 chức năng đã làm, scalable (thêm F9, F10...)

### Keil uVision project
- [ ] Tạo file `.uvprojx` trong `keil/` và thêm tất cả file vào project
- [ ] Cấu hình target: AT89C52, 11.0592 MHz, 8 KB Flash
- [ ] Biên dịch thử và kiểm tra kích thước code thực tế

### Proteus simulation
- [ ] Vẽ sơ đồ mạch trong Proteus theo `docs/wiring_guide.md`
- [ ] Nạp file `.hex` và chạy mô phỏng
- [ ] Kiểm tra từng chức năng trên Proteus

### Kiểm thử từng chức năng
- [ ] F1: Màn hình chào + loading bar
- [ ] F2: Đồng hồ MM:SS trên LCD
- [ ] F3: Đăng nhập mật khẩu 4 số
- [ ] F3.1: Đổi mật khẩu (giữ BTN1 3s)
- [ ] F3.2: Lockdown sau 3 lần sai
- [ ] F4: Mở/đóng cửa qua encoder
- [ ] F5: Hiển thị nhiệt độ 7-SEG
- [ ] F6: Quạt tự động theo nhiệt độ
- [ ] F7: Đèn tự động theo LDR
- [ ] F8: Log UART với timestamp

### Báo cáo
- [ ] Điền nội dung vào `docs/report-group-one.docx`
- [ ] Chèn ảnh Proteus vào `docs/code_explained.md`

---

## 📌 Ghi chú kỹ thuật

| Vấn đề | Giải pháp |
|:-------|:----------|
| AT89C52 không có ADC | Mô phỏng nhiệt độ bằng nút bấm tăng/giảm |
| LDR + LM358 không có trong BOM | Thay bằng SWITCH 1 (P1.6) gạt tay mô phỏng trời tối/sáng |
| P0 là open-drain | Cần 8 điện trở kéo lên 10 kΩ cho bus 7-SEG |
| 7-SEG và LCD chia sẻ port | Dự án dùng P0 cho 7-SEG, LCD dùng P3.4-P3.7 (tách hoàn toàn) |
| Timer0 Mode 1 không tự nạp | Phải nạp lại TH0:TL0 bằng tay trong mỗi ISR |
| Mật khẩu mất khi reset | Lưu trong RAM (biến static) — chấp nhận được với bài học |
| Code size ước tính ~3 KB | Còn ~5 KB dư trong 8 KB Flash của AT89C52 |

---

## 🗂️ Cấu trúc file hiện tại

```
smart-home-sys/
├── CLAUDE.md              ← Hướng dẫn cho Claude Code
├── progress.md            ← File này
├── task.md                ← Yêu cầu gốc + code mẫu gốc
├── inc/
│   ├── config.h           ← Cấu hình phần cứng trung tâm
│   ├── lcd.h
│   ├── uart.h
│   ├── door.h
│   ├── sensor.h
│   └── security.h
├── src/
│   ├── main.c             ← Điểm vào chính
│   ├── lcd.c
│   ├── uart.c
│   ├── door.c
│   ├── sensor.c
│   └── security.c
├── asm/
│   └── delay.asm
├── samples/
│   ├── bai1_led_button.c
│   ├── bai2_7seg_dem.c
│   ├── bai3_7seg_4chu.c
│   ├── bai4_encoder_motor.c
│   ├── bai5_dual_lcd.c
│   ├── bai6_uart_fan.c
│   ├── bai7_uart_buttons.c
│   ├── bai7_uart.asm
│   ├── bai9_full_system.c
│   └── bai10_mo_rong.c
├── docs/
│   ├── wiring_guide.md    ← Đấu nối chi tiết từng chân
│   ├── code_explained.md  ← Giải thích code từng module
│   └── report-group-one.docx
├── keil/                  ← (chưa có — cần tạo project Keil)
├── sim/                   ← (chưa có — cần vẽ Proteus)
└── build/                 ← Git-ignored
```
