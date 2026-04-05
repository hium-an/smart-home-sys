# Hướng dẫn đấu nối mạch — Smart Home System

**Vi điều khiển:** 3 × AT89C52 (DIP-40) | **Thạch anh:** 11.0592 MHz | **Nguồn:** 5V DC

> Hệ thống dùng **3 chip riêng biệt** trong cùng 1 sơ đồ Proteus:
> - **MCU1** — Security: LCD + Bảo mật + Tín hiệu GPIO → MCU2
> - **MCU2** — Door/Sound: Cửa + Âm thanh + UART log (nhận GPIO ← MCU1)
> - **MCU3** — Environment Control: 7-SEG + Quạt + Đèn + Nhiệt độ + Độ sáng

---

## 1. Phân bổ chân

> Cột **Pin#** là số chân vật lý trên IC DIP-40.

### MCU1 — Security (LCD + Mật khẩu + GPIO tín hiệu)

| Port | Chân | Pin# | Tín hiệu | Chiều | Mô tả |
|:-----|:-----|:----:|:---------|:------|:------|
| P1 | P1.0 | **1** | BTN1 | IN | Nút mật khẩu số 1 / giữ 3s = đổi pass |
| P1 | P1.1 | **2** | BTN2 | IN | Nút mật khẩu số 2 |
| P1 | P1.2 | **3** | BTN3 | IN | Nút mật khẩu số 3 |
| P1 | P1.3 | **4** | BTN4 | IN | Nút mật khẩu số 4 |
| P1 | P1.4 | **5** | SIG_UNLOCK | OUT | → MCU2.P1.1: ra lệnh mở cửa |
| P1 | P1.5 | **6** | SIG_ALARM | OUT | → MCU2.P1.0: ra lệnh báo động |
| P2 | P2.6 | **27** | LCD_RS | OUT | LCD Register Select |
| P2 | P2.7 | **28** | LCD_E | OUT | LCD Enable |
| P3 | P3.4 | **14** | LCD_D4 | OUT | LCD Data bit 4 |
| P3 | P3.5 | **15** | LCD_D5 | OUT | LCD Data bit 5 |
| P3 | P3.6 | **16** | LCD_D6 | OUT | LCD Data bit 6 |
| P3 | P3.7 | **17** | LCD_D7 | OUT | LCD Data bit 7 |
| — | VCC | **40** | VCC | PWR | 5V |
| — | GND | **20** | GND | PWR | GND |
| — | EA | **31** | EA/VPP | IN | Kéo lên VCC (dùng Flash nội) |

### MCU2 — Door / Sound / UART

| Port | Chân | Pin# | Tín hiệu | Chiều | Mô tả |
|:-----|:-----|:----:|:---------|:------|:------|
| P1 | P1.0 | **1** | SIG_ALARM_IN | IN | ← MCU1.P1.5: nhận lệnh báo động |
| P1 | P1.1 | **2** | SIG_UNLOCK_IN | IN | ← MCU1.P1.4: nhận lệnh mở cửa |
| P2 | P2.2 | **23** | MOTOR_IN2 | OUT | L293D IN2 (pin 7) — quay ngược (đóng cửa) |
| P2 | P2.3 | **24** | MOTOR_IN1 | OUT | L293D IN1 (pin 2) — quay thuận (mở cửa) |
| P2 | P2.5 | **26** | SOUND | OUT | BUZZER + SPEAKER (qua BC547) |
| P3 | P3.1 | **11** | TXD | OUT | UART → Virtual Terminal |
| P3 | P3.2 | **12** | ENCODER/INT0 | IN | Xung encoder động cơ cửa |
| — | VCC | **40** | VCC | PWR | 5V |
| — | GND | **20** | GND | PWR | GND |
| — | EA | **31** | EA/VPP | IN | Kéo lên VCC (dùng Flash nội) |

### MCU3 — Environment Control

| Port | Chân | Pin# | Tín hiệu | Chiều | Mô tả |
|:-----|:-----|:----:|:---------|:------|:------|
| P0 | P0.0 | **39** | SEG_a | OUT | Segment a của 7-SEG (qua 220 Ω, pull-up 10 kΩ) |
| P0 | P0.1 | **38** | SEG_b | OUT | Segment b |
| P0 | P0.2 | **37** | SEG_c | OUT | Segment c |
| P0 | P0.3 | **36** | SEG_d | OUT | Segment d |
| P0 | P0.4 | **35** | SEG_e | OUT | Segment e |
| P0 | P0.5 | **34** | SEG_f | OUT | Segment f |
| P0 | P0.6 | **33** | SEG_g | OUT | Segment g |
| P0 | P0.7 | **32** | SEG_dp | OUT | Segment dp |
| P1 | P1.0 | **1** | BTN_TEMP_UP | IN | Nút tăng nhiệt độ mô phỏng |
| P1 | P1.1 | **2** | BTN_TEMP_DN | IN | Nút giảm nhiệt độ mô phỏng |
| P1 | P1.2 | **3** | LIGHT_SW | IN | SWITCH ánh sáng (hở=sáng, đóng GND=tối) |
| P1 | P1.3 | **4** | FAN | OUT | Quạt tự động (qua 2N2222) |
| P1 | P1.4 | **5** | BTN_BRIGHT_UP | IN | Nút tăng độ sáng đèn |
| P1 | P1.5 | **6** | BTN_BRIGHT_DN | IN | Nút giảm độ sáng đèn |
| P2 | P2.0 | **21** | SEG_DIG1 | OUT | 7-SEG nhiệt độ: digit hàng chục |
| P2 | P2.1 | **22** | SEG_DIG2 | OUT | 7-SEG nhiệt độ: digit hàng đơn vị |
| P2 | P2.2 | **23** | BRT_DIG1 | OUT | 7-SEG độ sáng: digit 1 (chữ 'b') |
| P2 | P2.3 | **24** | BRT_DIG2 | OUT | 7-SEG độ sáng: digit 2 (trăm) |
| P2 | P2.4 | **25** | BULB | OUT | Bóng đèn (PWM 50 Hz qua Timer0 ISR) |
| P2 | P2.5 | **26** | BRT_DIG3 | OUT | 7-SEG độ sáng: digit 3 (chục) |
| P2 | P2.6 | **27** | BRT_DIG4 | OUT | 7-SEG độ sáng: digit 4 (đơn vị) |
| P3 | P3.1 | **11** | TXD | OUT | UART → Virtual Terminal |
| — | VCC | **40** | VCC | PWR | 5V |
| — | GND | **20** | GND | PWR | GND |
| — | EA | **31** | EA/VPP | IN | Kéo lên VCC (dùng Flash nội) |

---

## 2. Chi tiết đấu nối từng ngoại vi

### 2.1 Thạch anh & Nguồn (áp dụng cho cả 3 MCU)

```
AT89C52 (MCU1, MCU2, hoặc MCU3)
  Pin 19 (XTAL1) ──┬── thạch anh 11.0592 MHz ──┬── Pin 18 (XTAL2)
                   │                             │
                22 pF                         22 pF
                   │                             │
                  GND                           GND

  Pin 40 (VCC) ──── 5V
  Pin 20 (GND) ──── GND
  Pin  9 (RST) ──── 10 kΩ ──── VCC
               └─── 10 µF ──── GND   (reset tự động khi cấp nguồn)
  Pin 31 (EA)  ──── VCC         (bắt buộc: dùng Flash nội)
```

> Trong Proteus: **không cần vẽ thạch anh** — đặt `Clock Frequency = 11.0592MHz` trong Properties của chip.

---

### 2.2 LCD 16×2 (LM016L) — MCU1

> Giao tiếp 4-bit. **RW (pin 5) nối đất** — luôn ở chế độ ghi.

```
LCD                    MCU1 (AT89C52 #1)
  Pin 1  (VSS) ──── GND
  Pin 2  (VDD) ──── 5V
  Pin 3  (VO)  ──── Wiper của POT-HG
                        │                  │
                   Chân 1 (GND)      Chân 3 (5V)    (chỉnh độ tương phản)
  Pin 4  (RS)  ──── Pin 27 (P2.6)
  Pin 5  (RW)  ──── GND
  Pin 6  (E)   ──── Pin 28 (P2.7)
  Pin 7–10 (D0–D3) ─── để hở
  Pin 11 (D4)  ──── Pin 14 (P3.4)
  Pin 12 (D5)  ──── Pin 15 (P3.5)
  Pin 13 (D6)  ──── Pin 16 (P3.6)
  Pin 14 (D7)  ──── Pin 17 (P3.7)
  Pin 15 (A)   ──── 5V qua 220 Ω      (đèn nền — chỉ nối nếu Proteus hiện chân)
  Pin 16 (K)   ──── GND               (Proteus 8 thường không hiện pin 15/16, bỏ qua được)
```

---

### 2.3 Tín hiệu GPIO giữa MCU1 và MCU2

> 2 dây nối trực tiếp giữa MCU1 và MCU2, không cần điện trở.

```
MCU1 (Security)                    MCU2 (Door/Sound)
  Pin 5 (P1.4) ────────────────── Pin 2 (P1.1)   [SIG_UNLOCK]
  Pin 6 (P1.5) ────────────────── Pin 1 (P1.0)   [SIG_ALARM]
```

> - MCU1 xuất HIGH (1) trên SIG_UNLOCK khi mật khẩu đúng → MCU2 phát hiện và mở cửa + bíp.
> - MCU1 xuất HIGH (1) trên SIG_ALARM khi lockdown → MCU2 phát hiện và kêu báo động.
> - Bình thường cả 2 dây ở mức LOW (0).

---

### 2.4 LED 7 đoạn 2 số — Nhiệt độ (7SEG-MPX2-CA) — MCU3

> Anode chung, tích cực mức thấp ở chân segment.
> **Port 0 (pin 32–39) là open-drain — bắt buộc có 8 điện trở kéo lên 10 kΩ lên VCC.**
> **Transistor NPN phải nối kiểu emitter follower** (Collector→VCC, Emitter→COM).

```
7SEG-MPX2-CA           MCU3 (AT89C52 #3)
  Segment a ──── 220 Ω ──── Pin 39 (P0.0) ──── 10 kΩ ──── VCC
  Segment b ──── 220 Ω ──── Pin 38 (P0.1) ──── 10 kΩ ──── VCC
  Segment c ──── 220 Ω ──── Pin 37 (P0.2) ──── 10 kΩ ──── VCC
  Segment d ──── 220 Ω ──── Pin 36 (P0.3) ──── 10 kΩ ──── VCC
  Segment e ──── 220 Ω ──── Pin 35 (P0.4) ──── 10 kΩ ──── VCC
  Segment f ──── 220 Ω ──── Pin 34 (P0.5) ──── 10 kΩ ──── VCC
  Segment g ──── 220 Ω ──── Pin 33 (P0.6) ──── 10 kΩ ──── VCC
  Segment dp ─── 220 Ω ──── Pin 32 (P0.7) ──── 10 kΩ ──── VCC

Digit 1 — Anode chung (hàng chục):
  VCC ──── Collector 2N2222
            Emitter ──── COM1 (digit 1)
            Base ──── 1 kΩ ──── Pin 21 (P2.0)

Digit 2 — Anode chung (hàng đơn vị):
  VCC ──── Collector 2N2222
            Emitter ──── COM2 (digit 2)
            Base ──── 1 kΩ ──── Pin 22 (P2.1)
```

---

### 2.4b LED 7 đoạn 4 số — Độ sáng (7SEG-MPX4-CA) — MCU3

> Chung bus P0 với 7-SEG nhiệt độ (segment a–dp nối song song qua 220Ω).
> Hiển thị: "b" + phần trăm (VD: "b 60" = cấp 3, "b100" = cấp 5).

```
7SEG-MPX4-CA           MCU3 (AT89C52 #3)
  Segment a–dp: nối chung bus P0 qua 220Ω (giống 7-SEG nhiệt độ phía trên)

Digit 1 — chữ 'b' (brightness):
  VCC ──── Collector 2N2222
            Emitter ──── COM1 (digit 1)
            Base ──── 1 kΩ ──── Pin 23 (P2.2)

Digit 2 — hàng trăm:
  VCC ──── Collector 2N2222
            Emitter ──── COM2 (digit 2)
            Base ──── 1 kΩ ──── Pin 24 (P2.3)

Digit 3 — hàng chục:
  VCC ──── Collector 2N2222
            Emitter ──── COM3 (digit 3)
            Base ──── 1 kΩ ──── Pin 26 (P2.5)

Digit 4 — hàng đơn vị:
  VCC ──── Collector 2N2222
            Emitter ──── COM4 (digit 4)
            Base ──── 1 kΩ ──── Pin 27 (P2.6)
```

---

### 2.5 Nút bấm

> Active-low: nhấn = kéo chân về GND. Port 1 và Port 3 có pull-up nội.

**MCU1 — Mật khẩu (4 nút):**
```
MCU1
  Pin 1 (P1.0) ──── BUTTON ──── GND   [BTN1 — mật khẩu số 1 / giữ 3s = đổi pass]
  Pin 2 (P1.1) ──── BUTTON ──── GND   [BTN2 — mật khẩu số 2]
  Pin 3 (P1.2) ──── BUTTON ──── GND   [BTN3 — mật khẩu số 3]
  Pin 4 (P1.3) ──── BUTTON ──── GND   [BTN4 — mật khẩu số 4]
```

**MCU3 — Nhiệt độ + Độ sáng (4 nút):**
```
MCU3
  Pin  1 (P1.0) ──── BUTTON ──── GND   [BTN_TEMP_UP — tăng nhiệt độ]
  Pin  2 (P1.1) ──── BUTTON ──── GND   [BTN_TEMP_DN — giảm nhiệt độ]
  Pin  5 (P1.4) ──── BUTTON ──── GND   [BTN_BRIGHT_UP — tăng độ sáng]
  Pin  6 (P1.5) ──── BUTTON ──── GND   [BTN_BRIGHT_DN — giảm độ sáng]
```

---

### 2.6 Công tắc ánh sáng (SWITCH 1) — MCU3

> Thay cho LDR + LM358. Gạt tay mô phỏng trời sáng/tối.

```
MCU3
  Pin 3 (P1.2) ──── SWITCH 1 ──── GND
```

> Port 1 có pull-up nội:
> - SWITCH **hở** (OFF) → Pin 3 = 1 → **trời sáng** → đèn tắt
> - SWITCH **đóng** (ON) → Pin 3 = 0 → **trời tối** → đèn bật tự động

---

### 2.7 Quạt (FAN) — MCU3

```
MCU3
  Pin 4 (P1.3) ──── 1 kΩ ──── Base 2N2222 #3
                               Emitter ──────────── GND
                               Collector ──┬──── FAN (−)
                                           └──── Cathode 1N4007
                               FAN (+) ──── 5V
                               Anode 1N4007 ──── FAN (−)   [flyback, mắc ngược song song]
```

---

### 2.8 Bóng đèn (BULB) + LED-G — MCU3

```
MCU3
  Pin 25 (P2.4) ──── 1 kΩ ──── Base 2N2222 #4
                                Emitter ─────────────── GND
                                Collector ──┬──── BULB (−)
                                            └──── Cathode LED-G (qua 220 Ω)
                                BULB (+) ──── 5V
                                Anode LED-G ──── 5V qua 220 Ω
```

> PWM 50 Hz điều khiển độ sáng từ Timer0 ISR của MCU3. LED-G sáng khi đèn bật.

---

### 2.9 Động cơ DC cửa (L293D + Encoder) — MCU2

```
MCU2             L293D                       MOTOR-ENCODER
  Pin 23 (P2.2) ── IN2  (pin 7)    OUT2 (pin 6) ──┐
  Pin 24 (P2.3) ── IN1  (pin 2)    OUT1 (pin 3) ──┘ Cuộn motor
                   EN1  (pin 1) ──── VCC
                   VCC1 (pin 16) ─── 5V
                   VCC2 (pin 8)  ─── 5V
                   GND  (pin 4, 5, 12, 13) ── GND

  Pin 12 (P3.2/INT0) ──── Encoder xung ra

  Diode 1N4007 mắc ngược song song với 2 cực motor (flyback).
```

---

### 2.10 SPEAKER + BUZZER + LED-R — MCU2

```
MCU2
  Pin 26 (P2.5) ──── 1 kΩ ──── Base BC547
                                Emitter ─────────────── GND
                                Collector ──┬──── BUZZER (+) ──── 5V
                                            ├──── Wiper POTENTIOMETER ──── SPEAKER (+) ──── 5V
                                            └──── Anode LED-R (qua 220 Ω) ──── GND
                                BUZZER (−)  ──── Collector BC547
                                SPEAKER (−) ──── Collector BC547
                                Cathode LED-R ── Collector BC547

  POTENTIOMETER (âm lượng SPEAKER):
    Chân 1 (hoặc 3) ──── Collector BC547
    Wiper            ──── SPEAKER (+)
    Chân 3 (hoặc 1) ──── 5V
```

> - **BUZZER**: tín hiệu DC — báo động, bíp xác nhận
> - **SPEAKER**: PWM giai điệu — POTENTIOMETER điều chỉnh âm lượng thủ công
> - **LED-R**: đỏ khi lockdown
> - **POTENTIOMETER**: không cần MCU, xoay tay để tăng/giảm âm lượng

---

### 2.11 UART — Virtual Terminal — MCU2

```
MCU2
  Pin 11 (P3.1/TXD) ──── RXD của VIRTUAL TERMINAL
```

> Cài đặt trong Proteus (click đôi Virtual Terminal):
> ```
> Baud Rate : 9600
> Data Bits : 8
> Parity    : None
> Stop Bits : 1
> ```

---

## 3. Danh sách linh kiện cần thiết

| Linh kiện | SL | Ghi chú |
|:----------|:--:|:--------|
| AT89C52 (DIP-40) | **3** | MCU1 (Security) + MCU2 (Door) + MCU3 (Environ) |
| Thạch anh 11.0592 MHz | 3 | Mỗi MCU 1 cái |
| Tụ 22 pF | 6 | 2 cái mỗi thạch anh |
| Tụ 10 µF | 3 | Mạch reset tự động (mỗi MCU) |
| Điện trở 10 kΩ | 11 | 8 pull-up P0 + 3 RST |
| Điện trở 1 kΩ | 9 | Base transistors các loại |
| Điện trở 220 Ω | 11 | 8 segment + đèn nền LCD + LED-G + LED-R |
| Transistor 2N2222 | 4 | DIG1, DIG2, FAN, BULB |
| Transistor BC547 | 1 | SPEAKER + BUZZER driver |
| L293D | 1 | H-bridge động cơ cửa |
| Diode 1N4007 | 2 | Flyback FAN + MOTOR |
| LCD LM016L (16×2) | 1 | Màn hình — MCU1 |
| 7SEG-MPX2-CA | 1 | Hiển thị nhiệt độ 2 chữ số — MCU3 |
| MOTOR-ENCODER | 1 | Điều khiển cửa — MCU2 |
| FAN (quạt DC) | 1 | Tự động theo nhiệt độ — MCU3 |
| BULB (bóng đèn) | 1 | Chiếu sáng PWM — MCU3 |
| SPEAKER | 1 | Giai điệu chào + âm mở cửa — MCU2 |
| BUZZER | 1 | Báo động lockdown — MCU2 |
| LED-G | 1 | Chỉ thị đèn bật (song song BULB) |
| LED-R | 1 | Chỉ thị báo động (song song BUZZER) |
| LED-YELLOW | 1 | Chỉ thị 7-SEG quét (song song digit select) |
| BUTTON | 8 | 4 mật khẩu (MCU1) + 2 nhiệt độ + 2 độ sáng (MCU3) |
| SWITCH | 1 | Mô phỏng ánh sáng môi trường — MCU3 P1.2 |
| POT-HG | 1 | Độ tương phản LCD (chân VO) — MCU1 |
| POTENTIOMETER | 1 | Âm lượng SPEAKER — MCU2 |
| VIRTUAL TERMINAL | 1 | Log UART — MCU2 |

---

## 4. Lưu ý quan trọng

1. **Proteus dùng 1 file duy nhất** chứa 3 chip AT89C52. Mỗi chip load HEX riêng, chạy song song khi nhấn F5.
2. **MCU1 nối trực tiếp với MCU2** qua 2 dây GPIO (P1.4→P1.1, P1.5→P1.0). MCU3 hoạt động độc lập.
3. **Port 0 (P0) của MCU3 PHẢI có pull-up 10 kΩ** — P0 là open-drain, không có pull-up nội.
4. **EA/VPP (pin 31) PHẢI nối VCC** trên cả 3 chip — bắt buộc để chạy từ Flash nội.
5. **Không cần vẽ thạch anh trong Proteus** — chỉ cần đặt `Clock Frequency = 11.0592MHz` trong Properties mỗi chip.
6. **L293D cần 2 nguồn VCC** — VCC1 (pin 16) cho logic 5V, VCC2 (pin 8) cho motor.
7. **RW của LCD (pin 5) nối đất** — code không dùng chức năng đọc LCD.
8. **P3.1 của MCU2 và MCU3** = phần cứng TXD (UART). Cả 2 MCU đều dùng UART gửi log ra Virtual Terminal riêng.
