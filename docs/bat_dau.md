# Hướng dẫn bắt đầu từ đầu — Smart Home System

> Hệ thống dùng **3 AT89C52**: MCU1 (Security), MCU2 (Door/Sound), MCU3 (Environ).
> Tạo **3 Keil project riêng** → build ra **3 file HEX** → nạp vào **3 chip** trong Proteus.

---

## BƯỚC 1 — TẠO 3 PROJECT TRONG KEIL UVISION

### 1.1 Tạo Project 1 — MCU1 (Security)

1. Mở **Keil µVision**
2. Menu **Project → New µVision Project…**
3. Lưu vào: `keil/security/` → đặt tên `Security` → **Save**
4. Chọn thiết bị: **Atmel → AT89C52** → **OK** → hỏi startup → **No**

#### Thêm file vào Project 1:

1. Click phải **Source Group 1 → Add Existing Files…**
2. Thêm **3 file** sau:

```
src/main1.c
src/lcd.c
src/security.c
```

#### Cấu hình Project 1:

**Project → Options for Target → tab C51:**
- **Include Paths:** `..\..\inc`
- **Define:** `PROJ_MCU1`
- **Optimize:** `8` | **Emphasis:** `Favor Size`

**Tab Target:** Xtal = `11.0592` | Memory = `Small` | ROM = `Large`

**Tab Output:** ☑ Create HEX File | Name: `Security`

→ **F7** build → output: `keil\security\Security.hex`

---

### 1.2 Tạo Project 2 — MCU2 (Door/Sound/UART)

1. **Project → New µVision Project…**
2. Lưu vào: `keil/door/` → đặt tên `Door` → **Save**
3. Chọn **AT89C52** → **No** startup

#### Thêm file vào Project 2:

```
src/main_door.c
src/door.c
src/sound.c
src/uart.c
```

#### Cấu hình Project 2:

**Project → Options for Target → tab C51:**
- **Include Paths:** `..\..\inc`
- **Define:** `PROJ_MCU2`
- **Optimize:** `8` | **Emphasis:** `Favor Size`

**Tab Target:** Xtal = `11.0592` | Memory = `Small` | ROM = `Large`

**Tab Output:** ☑ Create HEX File | Name: `Door`

→ **F7** build → output: `keil\door\Door.hex`

---

### 1.3 Tạo Project 3 — MCU3 (Environment Control)

1. **Project → New µVision Project…**
2. Lưu vào: `keil/environ/` → đặt tên `Environ` → **Save**
3. Chọn **AT89C52** → **No** startup

#### Thêm file vào Project 3:

```
src/main2.c
src/sensor.c
```

#### Cấu hình Project 3:

**Project → Options for Target → tab C51:**
- **Include Paths:** `..\..\inc`
- **Define:** `PROJ_MCU3`
- **Optimize:** `8` | **Emphasis:** `Favor Size`

**Tab Target:** Xtal = `11.0592` | Memory = `Small` | ROM = `Large`

**Tab Output:** ☑ Create HEX File | Name: `Environ`

→ **F7** build → output: `keil\environ\Environ.hex`

---

## BƯỚC 2 — TẠO PROJECT VÀ VẼ MẠCH TRONG PROTEUS

### 2.1 Tạo project Proteus

1. Mở **Proteus Design Suite**
2. **File → New Project…**
3. Đặt tên: `SmartHome`, lưu vào thư mục `sim/`
4. Các tùy chọn: chọn mặc định (Schematic only, không PCB) → **Finish**

---

### 2.2 Thêm linh kiện vào schematic

Nhấn phím **P** (Pick Devices) hoặc click biểu tượng kính lúp bên trái.

| Gõ vào ô tìm kiếm | SL | Ghi chú |
|:-------------------|:--:|:--------|
| `AT89C52` | **3** | MCU1 (Security) + MCU2 (Door) + MCU3 (Environ) |
| `LM016L` | 1 | LCD 16×2 — nối MCU1 |
| `7SEG-MPX2-CA` | 1 | LED 7 đoạn 2 số anode chung — nối MCU3 |
| `MOTOR-ENCODER` | 1 | Motor có encoder — nối MCU2 |
| `L293D` | 1 | IC cầu H — nối MCU2 |
| `SPEAKER` | 1 | Loa — nối MCU2 |
| `BUZZER` | 1 | Còi — nối MCU2 |
| `2N2222` | 4 | DIG1, DIG2 (MCU3) + FAN, BULB (MCU3) |
| `BC547` | 1 | Driver SPEAKER + BUZZER — MCU2 |
| `FAN1-TABLE` | 1 | Quạt — nối MCU3 |
| `BULB` | 1 | Bóng đèn — nối MCU3 |
| `LED-G` | 1 | Song song BULB |
| `LED-R` | 1 | Song song BUZZER (báo động) |
| `LED-YELLOW` | 1 | Song song digit select 7-SEG |
| `BUTTON` | 8 | 4 cái nối MCU1, 4 cái nối MCU3 |
| `SWITCH` | 1 | Mô phỏng ánh sáng — MCU3 P1.2 |
| `POT-HG` | 1 | Độ tương phản LCD (chân VO) — MCU1 |
| `POTENTIOMETER` | 1 | Âm lượng SPEAKER — MCU2 |
| `1N4007` | 2 | Flyback FAN + MOTOR |
| `RES` | nhiều | 220Ω, 1kΩ, 10kΩ |
| `VIRTUAL TERMINAL` | 1 | Terminal ảo UART — MCU2 |

---

### 2.3 Cấu hình 3 chip AT89C52

Làm lần lượt cho **từng chip**:

1. Click đôi vào chip **AT89C52** trên schematic
2. Trong cửa sổ **Component Properties**:
   - **Clock Frequency:** `11.0592MHz`
   - **Program File:** ← để trống, nạp HEX ở Bước 3
3. **OK**

> Đặt tên label để phân biệt: `MCU1` (Security), `MCU2` (Door), `MCU3` (Environ).

---

### 2.4 Kết nối mạch

> Xem chi tiết từng chân và số pin vật lý trong `docs/wiring_guide.md`.

**MCU1 — Security (LCD + Mật khẩu + GPIO tín hiệu):**

```
① LCD (LM016L)
  LCD.VO  → Wiper POT-HG (chân 1→GND, chân 3→VCC)
  LCD.RS  → MCU1.P2.6  |  LCD.E  → MCU1.P2.7
  LCD.RW  → GND
  LCD.D4–D7 → MCU1.P3.4–P3.7

② Nút mật khẩu
  MCU1.P1.0–P1.3 → 4 × BUTTON → GND   [BTN1–BTN4]

③ Tín hiệu GPIO → MCU2
  MCU1.P1.4 (SIG_UNLOCK) ───wire─── MCU2.P1.1 (SIG_UNLOCK_IN)
  MCU1.P1.5 (SIG_ALARM)  ───wire─── MCU2.P1.0 (SIG_ALARM_IN)
```

**MCU2 — Door / Sound / UART:**

```
④ MOTOR-ENCODER + L293D
  MCU2.P2.2 → L293D.IN2  |  MCU2.P2.3 → L293D.IN1
  L293D.OUT1/OUT2 → MOTOR-ENCODER
  Encoder xung → MCU2.P3.2 (INT0)
  Diode 1N4007 flyback song song motor.

⑤ SPEAKER + BUZZER + LED-R (qua BC547)
  MCU2.P2.5 → 1kΩ → Base BC547 → Emitter → GND
  Collector → BUZZER(+), POTENTIOMETER(chân 1), LED-R(qua 220Ω) → VCC
  Wiper POTENTIOMETER → SPEAKER(+) → VCC

⑥ UART
  MCU2.P3.1 (TXD) → RXD Virtual Terminal
  Virtual Terminal: Baud=9600, 8N1

⑦ Tín hiệu GPIO ← MCU1
  MCU2.P1.0 (SIG_ALARM_IN)  ← MCU1.P1.5
  MCU2.P1.1 (SIG_UNLOCK_IN) ← MCU1.P1.4
```

**MCU3 — Environment Control:**

```
⑧ 7-SEG-MPX2-CA (hiển thị nhiệt độ)
  MCU3.P0.0–P0.7 → 8 × 220Ω → Segment a–dp   (chung bus P0)
  MCU3.P0.0–P0.7 → 8 × 10kΩ → VCC   (pull-up bắt buộc)
  MCU3.P2.0 → 1kΩ → Base 2N2222 → Emitter → COM1 (digit hàng chục), Collector → VCC
  MCU3.P2.1 → 1kΩ → Base 2N2222 → Emitter → COM2 (digit hàng đơn vị), Collector → VCC

⑧b 7-SEG-MPX4-CA (hiển thị độ sáng — chung bus P0)
  Segment a–dp: nối chung bus P0 qua 220Ω (giống 7-SEG nhiệt độ)
  MCU3.P2.2 → 1kΩ → Base 2N2222 → Emitter → COM1 (chữ 'b'), Collector → VCC
  MCU3.P2.3 → 1kΩ → Base 2N2222 → Emitter → COM2 (hàng trăm), Collector → VCC
  MCU3.P2.5 → 1kΩ → Base 2N2222 → Emitter → COM3 (hàng chục), Collector → VCC
  MCU3.P2.6 → 1kΩ → Base 2N2222 → Emitter → COM4 (hàng đơn vị), Collector → VCC
  Hiển thị VD: "b 60" = cấp 3 (60%), "b100" = cấp 5 (100%)

⑨ FAN + BULB
  MCU3.P1.3 → 1kΩ → Base 2N2222 #3 → FAN(−)  |  FAN(+) → VCC
  MCU3.P2.4 → 1kΩ → Base 2N2222 #4 → BULB(−) |  BULB(+) → VCC
  LED-G song song BULB (qua 220Ω)
  Diode 1N4007 flyback song song FAN.

⑩ SWITCH ánh sáng
  MCU3.P1.2 ─── SWITCH ─── GND   (hở=sáng, đóng=tối)

⑪ Nút nhiệt độ + độ sáng
  MCU3.P1.0 → BUTTON → GND   [BTN_TEMP_UP]
  MCU3.P1.1 → BUTTON → GND   [BTN_TEMP_DN]
  MCU3.P1.4 → BUTTON → GND   [BTN_BRIGHT_UP]
  MCU3.P1.5 → BUTTON → GND   [BTN_BRIGHT_DN]
```

**Nguồn (áp dụng cả 3 chip):**

```
VCC (pin 40) → 5V  |  GND (pin 20) → GND  |  EA (pin 31) → VCC
```

---

## BƯỚC 3 — NẠP FILE HEX VÀO PROTEUS VÀ CHẠY

### 3.1 Nạp HEX cho từng chip

**MCU1:**
1. Click đôi vào chip **MCU1** (AT89C52 #1) trên schematic
2. **Program File** → Browse đến: `keil\security\Security.hex` → **Open**
3. **OK**

**MCU2:**
1. Click đôi vào chip **MCU2** (AT89C52 #2) trên schematic
2. **Program File** → Browse đến: `keil\door\Door.hex` → **Open**
3. **OK**

**MCU3:**
1. Click đôi vào chip **MCU3** (AT89C52 #3) trên schematic
2. **Program File** → Browse đến: `keil\environ\Environ.hex` → **Open**
3. **OK**

### 3.2 Chạy mô phỏng

- Nhấn nút **▶ Play** (góc dưới trái Proteus) hoặc **F5**
- Cả 3 chip chạy song song trong cùng 1 simulation.

### 3.3 Kiểm tra theo thứ tự

| Khi chạy | Thiết bị | Kết quả mong đợi |
|:---------|:---------|:-----------------|
| Mới bật | MCU1 | LCD hiện "SMART HOME SYS / Group One" + loading bar |
| Mới bật | MCU2 | SPEAKER/BUZZER phát 3 nốt chào |
| Mới bật | MCU2 | Virtual Terminal: `=== MCU2: DOOR/SOUND ===` |
| Nhấn BTN1→BTN4 | MCU1 | LCD hiện `NHAP MAT KHAU: ****` |
| Nhập đúng {1,2,3,4} | MCU1+MCU2 | LCD "CHAO MUNG! CUA DA MO", MCU2 motor quay + bíp 2 lần |
| Nhập sai (lần 1, 2) | MCU1+MCU2 | LCD "SAI MAT KHAU!", MCU2 bíp 2 lần |
| Nhập sai 3 lần | MCU1+MCU2 | LCD "BAO DONG!", MCU2 BUZZER kêu liên tục |
| Nhấn BTN_ALARM_OFF (P1.7) | MCU1 | Tắt báo động, quay về nhập mật khẩu |
| Nhấn BTN_DOOR_CLOSE (P1.2 MCU2) | MCU2 | Motor quay ngược đóng cửa + bíp 2 lần |
| Gạt SWITCH kẹt cửa (P2.0 MCU1) | MCU1+MCU2 | LCD "KET CUA!", MCU2 bíp liên tục, motor dừng |
| Giữ BTN1 3 giây | MCU1 | Chế độ đổi mật khẩu |
| Mới bật | MCU3 | 7-SEG 2 số hiện `30` (nhiệt độ), 7-SEG 4 số hiện `b 60` (độ sáng) |
| Mới bật | MCU3 | Virtual Terminal: `=== MCU3: ENVIRON ===` |
| Nhấn BTN_TEMP_UP/DN | MCU3 | 7-SEG 2 số thay đổi nhiệt độ |
| Nhiệt độ ≥ 26°C | MCU3 | FAN bật tự động |
| Nhấn BTN_BRIGHT_UP/DN (P1.4/P1.5) | MCU3 | 7-SEG 4 số cập nhật độ sáng liên tục, BULB thay đổi PWM |
| Gạt SWITCH → GND (P1.2 MCU3) | MCU3 | BULB bật tự động |

---

## SƠ ĐỒ LUỒNG CÔNG VIỆC (tóm tắt)

```
[Keil — Project 1]       [Keil — Project 2]       [Keil — Project 3]
  keil/security/            keil/door/               keil/environ/
  Define: PROJ_MCU1         Define: PROJ_MCU2        Define: PROJ_MCU3
  main1.c + lcd.c           main_door.c + door.c     main2.c + sensor.c
  + security.c              + sound.c + uart.c
  F7 Build                  F7 Build                 F7 Build
  → Security.hex            → Door.hex               → Environ.hex
         │                       │                         │
         └───────────────────────┼─────────────────────────┘
                                 ↓
              [Proteus — sim/SmartHome.DSN]
                AT89C52 #1 ← Security.hex  (MCU1)
                AT89C52 #2 ← Door.hex      (MCU2)
                AT89C52 #3 ← Environ.hex   (MCU3)
                F5 Run → cả 3 chip chạy song song

  Giao tiếp GPIO:
    MCU1.P1.4 ──→ MCU2.P1.1  (SIG_UNLOCK: mở cửa)
    MCU1.P1.5 ──→ MCU2.P1.0  (SIG_ALARM: báo động)
```
