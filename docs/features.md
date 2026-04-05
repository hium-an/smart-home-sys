# Mô tả chức năng — Smart Home System

> **Cách scale:** Thêm chức năng mới bằng cách copy một khối `---` bên dưới, điền thông tin, và tăng số thứ tự.
> Mỗi chức năng gồm: tên, mã hiệu, mô tả, chân dùng, trạng thái hiển thị, hành vi.

---

## F1 — Màn hình chào + Loading Bar

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `main.c` → `Welcome()`, `lcd.c` → `LCD_LoadingBar()`, `sound.c` → `Sound_Welcome()` |
| **Chân liên quan** | LCD (P3.4–P3.7, P2.6, P2.7), SOUND (P2.5) |

**Mô tả:**
Khi khởi động, hệ thống hiển thị tên và tác giả trên LCD 16×2, sau đó chạy hiệu ứng loading bar 16 khối (mỗi khối cách 100 ms). Kết thúc loading, SPEAKER phát 3 nốt nhạc chào mừng.

**Hành vi:**
```
LCD dòng 0: "SMART HOME SYS"
LCD dòng 1: "   by Hieu    "
→ Loading bar xuất hiện dần (16 ký tự 0xFF)
→ Sound_Welcome(): 3 nốt tần số khác nhau
→ LCD xóa, chuyển sang F3 (đăng nhập)
```

---

## F2 — Đồng hồ MM:SS

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `main.c` → `Timer0_ISR()`, `Display_Clock()` |
| **Chân liên quan** | LCD (hiển thị), Timer0 (ngắt 1 ms) |

**Mô tả:**
Timer0 tạo ngắt mỗi 1 ms, đếm giây và phút. Đồng hồ hiển thị ở góc phải dòng 0 LCD (cột 10–15) sau khi đăng nhập thành công, cập nhật mỗi giây.

**Hành vi:**
```
Timer0 ISR (1 ms) → g_ms++ → khi đủ 1000 ms → g_sec++ → g_tick_1s = 1
Main loop (khi g_unlocked && g_tick_1s):
  LCD cột 10, dòng 0: "MM:SS"
```

---

## F3 — Đăng nhập mật khẩu 4 số

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `security.c` → `Security_Login()`, `Security_ChangePass()`, `Security_Lockdown()` |
| **Chân liên quan** | BTN1–BTN4 (P1.0–P1.3), SOUND (P2.5), LCD |

**Mô tả:**
Người dùng nhập 4 số bằng 4 nút bấm (BTN1=1, BTN2=2, BTN3=3, BTN4=4). Mật khẩu mặc định là `{1, 2, 3, 4}`. Mỗi lần nhấn hiện dấu `*` trên LCD. Sai 3 lần → lockdown.

**Hành vi:**
```
LCD dòng 0: "NHAP MAT KHAU:"
LCD dòng 1: "****" (dấu * tăng dần theo số phím nhấn)

Đúng → g_unlocked = 1, mở cửa, bíp 3 lần
Sai   → LCD "SAI MAT KHAU! | Con lai: X"
Sai 3 lần → F3.2 Lockdown
```

**F3.1 — Đổi mật khẩu:**
```
Giữ BTN1 (P1.0) trong 3 giây → vào chế độ đổi mật khẩu
Nhập mật khẩu mới → xác nhận lại → lưu vào RAM
```

**F3.2 — Lockdown:**
```
BUZZER + LED-R kêu liên tục (Sound_AlarmTick — tần số quét)
LCD: "!! BAO DONG !!"
Thoát: reset vi điều khiển (RST)
```

---

## F4 — Mở/đóng cửa (Motor + Encoder)

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `door.c` → `Door_Open()`, `Door_Close()`, `INT0_ISR()` |
| **Chân liên quan** | MOTOR_IN1 (P2.3), MOTOR_IN2 (P2.2), ENCODER/INT0 (P3.2), BTN_DOOR_CLOSE (P1.2), L293D |

**Mô tả:**
Sau đăng nhập thành công, động cơ DC quay thuận qua L293D để mở cửa. Encoder đếm xung qua INT0 ISR. Khi đủ `DOOR_PULSES` (50 xung), motor dừng. Người dùng bấm nút BTN_DOOR_CLOSE (P1.2, active-low) để đóng cửa — motor quay ngược.

**Hành vi:**
```
Door_Open() — khi MCU1 gửi SIG_UNLOCK:
  MOTOR_IN1=1, MOTOR_IN2=0 → quay thuận
  Đợi encoder >= 50 xung (INT0 ISR đếm)
  MOTOR_IN1=0, MOTOR_IN2=0 → dừng, bíp 3 lần

Door_Close() — khi nhấn BTN_DOOR_CLOSE (P1.2 = 0):
  MOTOR_IN1=0, MOTOR_IN2=1 → quay ngược
  Đợi encoder >= 50 xung
  Dừng, bíp 2 lần, log "CUA:DONG"
```

---

## F5 — Giám sát nhiệt độ (7-SEG 2 số)

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `sensor.c` → `Sensor_CheckButtons()`, `main.c` → `Display_Temp_7SEG()` |
| **Chân liên quan** | BTN_TEMP_UP (P1.0), BTN_TEMP_DN (P1.1), P0 (data), SEG_DIG1/2 (P2.0, P2.1) — MCU3 |

**Mô tả:**
Nhiệt độ mô phỏng bằng 2 nút bấm (tăng/giảm 1°C mỗi lần nhấn, khoảng 0–50°C). Hiển thị liên tục trên LED 7 đoạn 2 số bằng kỹ thuật multiplexing (quét từng digit ~2 ms).

**Hành vi:**
```
BTN_TEMP_UP (P1.0) nhấn → g_temperature++ (tối đa 50)
BTN_TEMP_DN (P1.1) nhấn → g_temperature-- (tối thiểu 0)
Main loop → Display_Temp_7SEG(g_temperature):
  Digit 1 (chục): P0 = SEG_TABLE[tens],  SEG_DIG1=1, SEG_DIG2=0, delay 2ms
  Digit 2 (đơn vị): P0 = SEG_TABLE[units], SEG_DIG1=0, SEG_DIG2=1, delay 2ms
```

---

## F6 — Quạt tự động theo nhiệt độ

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `main.c` → `Auto_Fan()` |
| **Chân liên quan** | FAN (P1.7), qua transistor 2N2222 |

**Mô tả:**
Quạt bật tự động khi nhiệt độ vượt ngưỡng `TEMP_FAN_ON` (26°C) và tắt khi xuống dưới `TEMP_FAN_OFF` (25°C). Có hysteresis để tránh bật/tắt liên tục.

**Hành vi:**
```
g_temperature >= 26°C → FAN = 1, log "QUAT: BAT"
g_temperature <= 25°C → FAN = 0, log "QUAT: TAT"
```

---

## F7 — Đèn tự động theo ánh sáng môi trường

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `main2.c` → `Light_Logic()`, `Check_Brightness_Buttons()`, `Display_Bright()` |
| **Chân liên quan** | LIGHT_SW (P1.2), BULB (P2.4), BTN_BRIGHT_UP (P1.4), BTN_BRIGHT_DN (P1.5), BRT_DIG1-4 (P2.2, P2.3, P2.5, P2.6) |

**Mô tả:**
Công tắc SWITCH (P1.2) mô phỏng ánh sáng môi trường. Khi gạt về GND (trời tối), đèn tự bật. Khi hở mạch (trời sáng), đèn tự tắt. Người dùng điều chỉnh 5 mức độ sáng bằng BTN_BRIGHT_UP/DN (P1.4, P1.5). Độ sáng hiển thị **liên tục** trên 7-SEG 4 số riêng biệt.

**Hành vi:**
```
LIGHT_SW = 0 (đóng về GND = tối) → BULB bật theo PWM
LIGHT_SW = 1 (hở mạch = sáng)    → BULB tắt, g_brightness reset = 0

BTN_BRIGHT_UP (P1.4) nhấn → g_brightness++ (tối đa 5 = 100%)
BTN_BRIGHT_DN (P1.5) nhấn → g_brightness-- (tối thiểu 0 = tắt)

7-SEG 4 số hiển thị liên tục (chia sẻ P0 với 7-SEG nhiệt độ):
  Digit 1: chữ "b" (brightness)
  Digit 2: hàng trăm (1 hoặc tắt)
  Digit 3: hàng chục
  Digit 4: hàng đơn vị
  VD: cấp 3 → "b 60", cấp 5 → "b100", cấp 0 → "b  0"

PWM (Timer0 ISR, 50 Hz):
  pwm_counter (0..19) → BULB = 1 nếu pwm < g_brightness*4
  g_brightness: 0→0%, 1→20%, 2→40%, 3→60%, 4→80%, 5→100%
```

---

## F8 — Log UART có nhãn thời gian

| Mục | Chi tiết |
|:----|:---------|
| **Trạng thái** | ✅ Hoàn thành |
| **Module** | `uart.c` → `UART_Log()`, `UART_SendStr()` |
| **Chân liên quan** | TXD (P3.0), Timer1 (baud 9600) |

**Mô tả:**
Toàn bộ sự kiện hệ thống (đăng nhập, quạt, đèn, cửa) được ghi log qua UART 9600 8N1 với nhãn thời gian `[MM:SS]`. Xem log bằng Virtual Terminal trong Proteus.

**Hành vi:**
```
UART_Log("EVENT", mm, ss):
  Gửi "[MM:SS] EVENT\r\n" ra P3.0
  Ví dụ: "[01:23] QUAT: BAT (nhiet do cao)"

Cài đặt Virtual Terminal trong Proteus:
  Baud: 9600 | Data: 8 | Parity: None | Stop: 1
```

---

## Cách thêm chức năng mới

1. Copy một khối `---` bên trên làm template.
2. Đặt mã hiệu tiếp theo (F9, F10, ...).
3. Điền bảng thông tin, mô tả, hành vi.
4. Thêm hàm tương ứng vào `src/` và khai báo trong `inc/`.
5. Gọi hàm trong `main()` super-loop.
6. Cập nhật trạng thái sang ✅ khi hoàn thành.
