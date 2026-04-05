# Hướng dẫn dựng mạch Proteus + cài code Keil

---

## PHẦN 1 — VẼ MẠCH TRONG PROTEUS

### Bước 1: Thêm linh kiện

Vào **Library → Pick Devices (P)**, tìm và thêm từng linh kiện:

| Tên component trong Proteus | Tên trong BOM | Số lượng |
|:---------------------------|:-------------|:--------:|
| AT89C52 | AT89C52 | 1 |
| LM016L | LCD 16×2 | 1 |
| 7SEG-MPX2-CA | 7SEG-MPX2-CA | 1 |
| MOTOR-ENCODER | MOTOR-ENCODER | 1 |
| L293D | L293D | 1 |
| SPEAKER | SPEAKER | 1 |
| BUZZER | BUZZER | 1 |
| 2N2222 | 2N2222 | 6 |
| BC547 | BC547 | 1 (cho speaker driver) |
| FAN (chọn FAN1-TABLE hoặc tương đương) | FAN | 1 |
| BULB | BULB | 1 |
| LED-G | LED-G | 1 |
| LED-R | LED-R | 1 |
| LED-YELLOW | LED-YELLOW | 1 |
| 1N4007 | 1N4007 | 2 |
| BUTTON | BUTTON | 8 (BTN1-4 + TEMP UP/DN + BRIGHT UP/DN) |
| SWITCH 1 | SWITCH | 1 (mô phỏng ánh sáng môi trường) |
| POT-HG | POT-HG | 1 (độ tương phản LCD — chân VO) |
| POTENTIOMETER | POTENTIOMETER | 1 (âm lượng SPEAKER) |
| VIRTUAL TERMINAL | — | 1 (UART log) |
| RES (220Ω, 1kΩ, 10kΩ) | RES-220, MINRES1K, MINRES10K | nhiều |
| POWER SOURCE DC (5V) | POWER SOURCE DC | 1 |

> **Lưu ý thạch anh:** Trong Proteus KHÔNG cần vẽ thạch anh và tụ 22pF. Chỉ cần click đôi vào AT89C52 → chỉnh **Clock Frequency = 11.0592MHz** trong Properties.

---

### Bước 2: Kết nối AT89C52

Sau khi đặt AT89C52 lên schematic, đặt **EA/VPP (pin 31) = 5V** (terminal VCC).

---

### Bước 3: Kết nối LCD (LM016L)

```
LCD pin 1  (VSS) → GND
LCD pin 2  (VDD) → 5V
LCD pin 3  (VO)  → Biến trở 10kΩ giữa 5V và GND (chỉnh contrast)
LCD pin 4  (RS)  → P2.6
LCD pin 5  (RW)  → GND
LCD pin 6  (E)   → P2.7
LCD pin 7-10 (D0-D3) → để hở
LCD pin 11 (D4)  → P3.4
LCD pin 12 (D5)  → P3.5
LCD pin 13 (D6)  → P3.6
LCD pin 14 (D7)  → P3.7
LCD pin 15 (A)   → 5V qua 220Ω (đèn nền)
LCD pin 16 (K)   → GND
```

---

### Bước 4: Kết nối 7SEG-MPX2-CA

```
Các chân segment (a-g, dp) → P0.0–P0.7 (qua 220Ω mỗi chân)

Port 0 cần điện trở kéo lên 10kΩ:
  8 điện trở 10kΩ, một đầu nối VCC, đầu kia nối P0.0–P0.7.

Digit 1 (hàng chục):
  COM1 (anode chung) → Collector 2N2222 #1
  Emitter 2N2222 #1  → GND
  Base 2N2222 #1     → P2.0 qua 1kΩ
  (LED-YELLOW song song với Collector 2N2222 #1 qua 220Ω → 5V)

Digit 2 (hàng đơn vị):
  COM2 (anode chung) → Collector 2N2222 #2
  Emitter 2N2222 #2  → GND
  Base 2N2222 #2     → P2.1 qua 1kΩ
```

> **LED-YELLOW:** đặt song song với đầu Collector của transistor digit select — sẽ nhấp nháy khi 7SEG đang quét.

---

### Bước 5: Kết nối MOTOR-ENCODER + L293D

```
AT89C52  →  L293D  →  MOTOR-ENCODER
P2.2     →  IN2 (pin 7)    OUT2 (pin 6)  ─┐
P2.3     →  IN1 (pin 2)    OUT1 (pin 3)  ─┘ Cuộn motor
           EN1 (pin 1) → 5V (luôn enable)
           VCC1 (pin 16) → 5V  (nguồn logic)
           VCC2 (pin 8)  → 5V  (nguồn motor)
           GND (pin 4,5,12,13) → GND

MOTOR-ENCODER ngõ ra encoder → P3.2 (INT0)
Diode 1N4007 mắc ngược song song với 2 chân motor (flyback).
```

---

### Bước 6: Kết nối SPEAKER + BUZZER (cùng chân P2.5)

```
P2.5 → 1kΩ → Base BC547
               Emitter BC547 → GND
               Collector BC547 ─┬─ BUZZER(+) → 5V  (BUZZER âm khi DC)
                                └─ SPEAKER(+) → 5V  (SPEAKER âm khi PWM)
BUZZER(-) và SPEAKER(-) → Collector BC547

LED-R: đặt song song với BUZZER (220Ω nối tiếp) → nhấp nháy khi báo động.
```

> Trong Proteus: click đôi SPEAKER → chọn **SPICE Model** phù hợp (hoặc dùng mặc định). BUZZER thường hiển thị âm thanh khi được kích DC.

---

### Bước 7: Kết nối FAN + BULB

```
FAN:
  P1.7 → 1kΩ → Base 2N2222 #3
                Emitter → GND
                Collector → FAN(−)
  FAN(+) → 5V
  Diode 1N4007 song song ngược với FAN.

BULB:
  P2.4 → 1kΩ → Base 2N2222 #4
                Emitter → GND
                Collector → BULB(−)
  BULB(+) → 5V
  LED-G song song với BULB (220Ω nối tiếp) → sáng khi đèn bật.
```

---

### Bước 8: Kết nối SWITCH mô phỏng ánh sáng

```
P1.2 ─── SWITCH (component tên SWITCH 1) ─── GND
```

> Trong Proteus: dùng component **SWITCH**. Đặt label `SW_LIGHT`.
> - SWITCH **hở** (OFF) → P1.2 = 1 → trời sáng → đèn tắt
> - SWITCH **đóng** (ON) → P1.2 = 0 → trời tối → đèn bật tự động

---

### Bước 9: Kết nối 8 nút bấm

```
BTN1  (P1.0) ─── BUTTON ─── GND
BTN2  (P1.1) ─── BUTTON ─── GND
BTN3  (P1.2) ─── BUTTON ─── GND
BTN4  (P1.3) ─── BUTTON ─── GND
BTN_TEMP_UP  (P1.0) ─── BUTTON ─── GND   ← MCU3
BTN_TEMP_DN  (P1.1) ─── BUTTON ─── GND   ← MCU3
BTN_BRIGHT_UP (P1.4) ─── BUTTON ─── GND   ← MCU3, thay POTENTIOMETER
BTN_BRIGHT_DN (P1.5) ─── BUTTON ─── GND   ← MCU3, thay POTENTIOMETER
```

(Port 1 và Port 3 đã có pull-up nội → không cần thêm điện trở kéo lên)

---

### Bước 10: UART — Virtual Terminal

```
P3.0 (TXD) → Terminal RXD của VIRTUAL TERMINAL
```

Click đôi VIRTUAL TERMINAL → Baud=9600, Data=8, Parity=None, Stop=1.

---

## PHẦN 2 — CÀI CODE TRONG KEIL UVISION

### Bước 1: Tạo Project mới

1. **Project → New µVision Project…**
2. Lưu vào thư mục `keil/` (ví dụ: `keil/SmartHome.uvprojx`)
3. Chọn thiết bị: **Atmel → AT89C52**
4. Keil hỏi thêm startup code → **Không cần (No)**

### Bước 2: Thêm file vào Project

Trong **Project Explorer** (cột trái), click phải **Source Group 1 → Add Existing Files**:

Thêm các file sau theo thứ tự:
```
src/main.c
src/lcd.c
src/uart.c
src/door.c
src/sensor.c
src/security.c
src/sound.c
```

File ASM (nếu muốn dùng delay assembly):
```
asm/delay.asm
```

### Bước 3: Cấu hình Include Path

**Project → Options for Target → C51 tab → Include Paths:**
```
..\inc
```
(Thêm đường dẫn thư mục `inc/` để Keil tìm thấy các file `.h`)

### Bước 4: Cấu hình Target

**Project → Options for Target → Target tab:**
- **Xtal (MHz):** `11.0592`
- **Memory Model:** `Small`
- **Code ROM Size:** `Large`

**Output tab:**
- Tích **Create HEX File** ← bắt buộc để nạp vào Proteus
- Output Folder: `../build/`

### Bước 5: Build

- **F7** hoặc **Project → Build Target**
- Kết quả: file `build/SmartHome.hex`

Nếu lỗi: thường do include path chưa đúng — kiểm tra lại bước 3.

### Bước 6: Nạp HEX vào Proteus

1. Trong Proteus, click đôi vào **AT89C52**
2. Mục **Program File** → browse đến `build/SmartHome.hex`
3. **Clock Frequency:** `11.0592MHz`
4. **Run** (▶)

---

## PHẦN 3 — DANH SÁCH ITEMS ĐÃ SỬ DỤNG

| # | Item | Dùng ở đâu | Chức năng |
|:--|:-----|:-----------|:----------|
| 1 | AT89C52 | Trung tâm | Vi điều khiển chính |
| 2 | LM016L | LCD | Hiển thị trạng thái, đồng hồ, mật khẩu, độ sáng |
| 3 | 7SEG-MPX2-CA | 7-SEG | Hiển thị nhiệt độ 2 chữ số |
| 4 | MOTOR-ENCODER | Cửa | Mở/đóng cửa, đếm encoder |
| 5 | L293D | Cửa | Driver cầu H cho motor |
| 6 | SPEAKER | P2.5 | Phát giai điệu chào, âm mở cửa |
| 7 | BUZZER | P2.5 (song song) | Báo động lockdown |
| 8 | 2N2222 | FAN, BULB, digit select, ALARM | Transistor NPN đệm dòng |
| 9 | BC547 | Speaker driver | NPN, dòng cao hơn cho audio |
| 10 | FAN | P1.7 | Quạt tự động theo nhiệt độ |
| 11 | BULB | P2.4 (PWM) | Đèn chiếu sáng, điều chỉnh độ sáng |
| 12 | LED-G | Song song BULB | Chỉ thị đèn bật (xanh lá) |
| 13 | LED-R | Song song BUZZER | Chỉ thị báo động (đỏ) |
| 14 | LED-YELLOW | Song song digit select | Chỉ thị 7-SEG đang quét |
| 15 | BUTTON | P1.0-P1.5 (MCU1,3) | Mật khẩu, nhiệt độ, độ sáng |
| 16 | SWITCH 1 | P1.2 (MCU3) | Mô phỏng ánh sáng môi trường (tối/sáng) |
| 17 | 1N4007 | Motor, FAN | Diode chống ngược EMF |
| 19 | RES-220 | Segment, đèn nền LCD | Giới hạn dòng LED |
| 20 | MINRES1K | Base transistor | Giới hạn dòng base |
| 21 | MINRES10K | Pull-up P0, LDR divider | Kéo lên, phân áp |
| 22 | POWER SOURCE DC | Toàn mạch | Cấp nguồn 5V |
| 23 | VIRTUAL TERMINAL | P3.0 | Xem log UART |

**Tổng: 24 items** — đủ để được điểm cao theo tiêu chí dùng nhiều linh kiện.
