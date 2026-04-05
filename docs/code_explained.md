# Giải thích Code — Smart Home System

> **Lưu ý hình ảnh:** Các mục có ghi *(hình — tự thêm)* là nơi bạn chụp/chèn ảnh thực tế từ Proteus hoặc phần cứng vào.

---

## 1. Tổng quan kiến trúc

*(hình — sơ đồ khối hệ thống: các module và mối liên hệ)*

Hệ thống sử dụng **super-loop** (vòng lặp chính không RTOS):

```
main()
├── Khởi tạo tất cả module
├── Màn hình chào (Welcome + loading bar)
└── while(1) — vòng lặp chính
    ├── [liên tục] Quét 7-SEG nhiệt độ
    ├── [liên tục] Kiểm tra nút nhiệt độ
    ├── [liên tục] Quạt tự động (Auto_Fan)
    ├── [liên tục] Đèn tự động (Light_Logic)
    ├── [kiểm tra] Giữ BTN1 → đổi mật khẩu
    ├── [nếu chưa mở khóa] Đăng nhập
    │   ├── Đúng → mở cửa, bật chế độ hoạt động
    │   └── Sai  → thông báo, kiểm tra giới hạn sai
    └── [mỗi giây] Cập nhật đồng hồ LCD
```

**Ngắt đang sử dụng:**

| Nguồn ngắt | Vector | Mô tả |
|:-----------|:------:|:------|
| Timer0 ISR | 1 | Đếm 1 ms → cập nhật MM:SS |
| INT0 ISR   | 0 | Đếm xung encoder động cơ cửa |

---

## 2. Module `inc/config.h` — Cấu hình phần cứng

File trung tâm, **tất cả module khác** include vào đây.

**Tại sao quan trọng:** Nếu cần thay chân (ví dụ dời FAN từ P1.7 sang P1.6), chỉ cần sửa một chỗ trong `config.h`, không phải tìm trong từng file `.c`.

Ví dụ khai báo pin bằng `sbit`:
```c
sbit FAN = P1^7;   /* Khai báo P1.7 với tên FAN — Keil C51 extension */
```

**Tính toán Timer0 reload:**
- Tần số thạch anh: 11.0592 MHz
- 1 chu kỳ máy = 12 / 11.0592 MHz ≈ 1.085 µs
- Cần đếm 1 ms = 921 chu kỳ máy
- Giá trị nạp: 65536 − 921 = 64615 = **0xFC67**

**Tính toán UART baud 9600:**
- TH1 = 256 − (11059200 / 12 / 32 / 9600) = 256 − 3 = **253 = 0xFD**

---

## 3. Module LCD (`src/lcd.c`)

*(hình — chụp LCD hiển thị màn hình đăng nhập trong Proteus)*

### 3.1 Giao tiếp 4-bit là gì?

LCD HD44780 cần 8 bit dữ liệu (D0–D7), nhưng ở chế độ 4-bit:
- Chỉ dùng **D4–D7** (4 chân)
- Mỗi byte được gửi **2 lần**: nibble cao trước, nibble thấp sau
- **Tiết kiệm 4 chân** vi điều khiển

### 3.2 Luồng khởi tạo

```
Chờ 20ms (nguồn ổn định)
→ Gửi 0x03 ba lần (reset về 8-bit, đặc điểm của HD44780)
→ Gửi 0x02 (chuyển sang 4-bit)
→ Gửi 0x28 (4-bit, 2 dòng, font 5×8)
→ Gửi 0x0C (bật hiển thị, tắt con trỏ)
→ Gửi 0x01 (xóa màn hình)
→ Gửi 0x06 (tăng địa chỉ tự động, không cuộn)
```

### 3.3 Gửi 1 byte theo giao thức 4-bit

```c
static void lcd_write_byte(unsigned char val, bit rs)
{
    LCD_RS = rs;          /* Chọn lệnh (0) hay dữ liệu (1) */
    lcd_send4(val >> 4);  /* Gửi 4 bit cao */
    lcd_pulse_e();        /* Kéo E lên → xuống để LCD chốt */
    lcd_send4(val & 0xF); /* Gửi 4 bit thấp */
    lcd_pulse_e();
}
```

### 3.4 Địa chỉ DDRAM

| Vị trí | Địa chỉ |
|:-------|:--------|
| Dòng 1, cột 0 | 0x80 |
| Dòng 2, cột 0 | 0xC0 |
| Dòng 1, cột n | 0x80 + n |
| Dòng 2, cột n | 0xC0 + n |

---

## 4. Module UART (`src/uart.c`)

*(hình — chụp Virtual Terminal Proteus hiển thị log hệ thống)*

### 4.1 Cấu hình Timer1 Mode 2

Timer1 Mode 2 là **8-bit tự nạp lại**: khi tràn (overflow), giá trị trong TH1 tự động copy vào TL1. Rất phù hợp để tạo baud rate ổn định.

```
TMOD |= 0x20;   ← Bit T1M1=1: Timer1 Mode 2
TH1  = 0xFD;   ← Giá trị nạp lại (xác định baud rate)
SCON = 0x50;   ← SM1=1: UART Mode 1 (8N1), REN=1: cho phép nhận
TR1  = 1;      ← Bật Timer1
```

### 4.2 Gửi 1 ký tự

```c
SBUF = c;          /* Nạp ký tự → UART bắt đầu truyền tự động */
while (!TI);       /* Chờ TI=1 (truyền xong) */
TI = 0;            /* Xóa cờ để truyền ký tự tiếp */
```

### 4.3 Định dạng log

```
[MM:SS] NỘI DUNG\r\n
```
Ví dụ: `[02:15] QUAT: BAT (nhiet do cao)`

---

## 5. Module Đồng hồ — Timer0 ISR (`src/main.c`)

*(hình — chụp LCD góc phải hiển thị MM:SS)*

### 5.1 Cấu hình Timer0 Mode 1

```
TMOD |= 0x01;   ← Timer0 Mode 1 (16-bit, không tự nạp lại)
TH0  = 0xFC;
TL0  = 0x67;    ← Nạp 0xFC67 = 64615, đếm thêm 921 → tràn sau 1 ms
ET0  = 1;       ← Cho phép ngắt Timer0
TR0  = 1;       ← Bật Timer0
```

### 5.2 ISR Timer0

```c
void Timer0_ISR(void) interrupt 1
{
    TH0 = T0_HIGH;    /* Nạp lại giá trị thủ công (Mode 1 không tự nạp) */
    TL0 = T0_LOW;
    g_ms++;
    if (g_ms >= 1000) {    /* 1000 ms = 1 giây */
        g_ms = 0;
        g_sec++;
        g_tick_1s = 1;     /* Báo cho main loop biết đã qua 1 giây */
        if (g_sec >= 60) { g_sec = 0; g_min++; }
        if (g_min >= 60)   g_min = 0;
    }
}
```

**Lưu ý:** Mỗi lần vào ISR phải **nạp lại TH0:TL0** vì Timer0 Mode 1 không tự nạp lại như Mode 2.

---

## 6. Module Bảo mật (`src/security.c`)

*(hình — chụp màn hình nhập mật khẩu và màn hình báo động)*

### 6.1 Nhập mật khẩu

- 4 nút bấm (BTN1–BTN4) đại diện cho 4 chữ số (1, 2, 3, 4).
- Mỗi lần nhấn: lưu vào mảng `input[]`, hiển thị `*` trên LCD.
- Sau 4 ký tự: so sánh từng phần tử với `s_password[]`.

### 6.2 Đổi mật khẩu (giữ BTN1 ≥ 3 giây)

```
Giữ BTN1 → đếm ms tăng dần
→ Nếu đủ HOLD_CHANGE_MS (3000ms) và BTN1 được nhả
→ Gọi Security_ChangePass()
   ├── Nhập mật khẩu mới (4 số)
   ├── Nhập xác nhận (4 số)
   ├── Khớp → lưu vào s_password[]
   └── Không khớp → thông báo lỗi
```

### 6.3 Khóa hệ thống (Lockdown)

Khi sai ≥ MAX_FAIL (3) lần liên tiếp:
- Bật ALARM (còi nhấp nháy liên tục)
- Hiển thị "!! BAO DONG !!" trên LCD
- **Vòng lặp vô hạn** — chỉ thoát bằng reset vi điều khiển

---

## 7. Module Điều khiển cửa (`src/door.c`)

*(hình — chụp mạch motor L293D + encoder trong Proteus)*

### 7.1 L293D — Cầu H

| IN1 | IN2 | Động cơ |
|:---:|:---:|:--------|
| 1 | 0 | Quay thuận (mở cửa) |
| 0 | 1 | Quay ngược (đóng cửa) |
| 0 | 0 | Dừng |
| 1 | 1 | Hãm (không khuyến nghị) |

### 7.2 Đếm xung Encoder qua INT0

```c
void INT0_ISR(void) interrupt 0
{
    s_encoder++;    /* Mỗi xung encoder tăng biến đếm */
}
```

Door_Open() bật động cơ, chờ đến khi `s_encoder >= DOOR_PULSES` (50 xung), rồi dừng.

---

## 8. Module Cảm biến (`src/sensor.c`)

*(hình — chụp mạch LDR + LM358 trong Proteus)*

### 8.1 Nhiệt độ mô phỏng

AT89C52 **không có ADC**. Nhiệt độ được mô phỏng bằng 2 nút bấm tăng/giảm, lưu trong biến `g_temperature`. Đây là kỹ thuật thường dùng trong bài thực hành nhúng khi chưa có cảm biến thực.

### 8.2 LDR số hóa

Dùng **LM358** làm bộ so sánh (comparator):
- `IN+`: điện áp phân áp LDR (thay đổi theo ánh sáng)
- `IN−`: điện áp tham chiếu từ biến trở (ngưỡng sáng/tối)
- `OUT → P1.6`: 1 = đủ sáng, 0 = tối

---

## 9. Module 7-SEG (`src/main.c` — hàm `Display_Temp_7SEG`)

*(hình — chụp 7-SEG hiển thị nhiệt độ trong Proteus)*

### 9.1 Multiplexing 2 digit

Mắt người có "lưu ảnh" ~20 ms. Nếu tốc độ quét ≥ 50 Hz (mỗi digit ≤ 10 ms), mắt thấy cả 2 digit sáng cùng lúc.

```
Bước 1: P0 = mã chữ số hàng chục
         SEG_DIG1 = 1, SEG_DIG2 = 0  → digit hàng chục sáng
         Delay 2 ms
         SEG_DIG1 = 0

Bước 2: P0 = mã chữ số hàng đơn vị
         SEG_DIG2 = 1, SEG_DIG1 = 0  → digit hàng đơn vị sáng
         Delay 2 ms
         SEG_DIG2 = 0
```

### 9.2 Bảng mã anode chung

| Chữ số | Mã hex | Giải thích bit |
|:------:|:------:|:--------------|
| 0 | 0xC0 | a-f bật, g tắt |
| 1 | 0xF9 | b,c bật |
| 2 | 0xA4 | a,b,g,e,d bật |
| ... | ... | ... |

Các bit `1` = tắt segment (anode chung, active-low).

---

## 10. Quạt tự động & Đèn tự động (`src/main.c`)

### Auto_Fan

```c
if (g_temperature >= TEMP_FAN_ON && !fan_on)  → bật quạt, log UART
if (g_temperature <= TEMP_FAN_OFF && fan_on)  → tắt quạt, log UART
```

Dùng biến `fan_on` để tránh ghi log lặp lại mỗi vòng lặp (chỉ log khi trạng thái **thay đổi**).

### Light_Logic

```c
if (!Sensor_IsLight() && !bulb_on) → tối → bật đèn
if ( Sensor_IsLight() &&  bulb_on) → sáng → tắt đèn
```

---

## 11. Kiểm tra kích thước code (AT89C52 = 8 KB Flash)

Ước tính khi biên dịch với Keil C51 tối ưu cấp độ 2:

| Module | Kích thước ước tính |
|:-------|:-------------------:|
| lcd.c | ~500 bytes |
| uart.c | ~250 bytes |
| door.c | ~300 bytes |
| sensor.c | ~200 bytes |
| security.c | ~600 bytes |
| main.c | ~800 bytes |
| Overhead (startup, libs) | ~400 bytes |
| **Tổng** | **~3050 bytes** |

→ **Còn ~5 KB dư**, an toàn cho việc mở rộng thêm tính năng.
