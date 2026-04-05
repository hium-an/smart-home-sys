# BOM — Danh sách linh kiện (Bill of Materials)

> **Cách đọc bảng:**
> - **Trạng thái** ✅ = đang dùng | ⬜ = chưa dùng | ❌ = đã thay thế bằng thứ khác
> - **Proteus Name** = tên gõ vào ô Pick Devices (P) trong Proteus
> - Khi thêm linh kiện mới: đổi ⬜ → ✅, điền Số lượng + Chân + Chức năng, rồi thêm mục giới thiệu ở Phần 2

---

## Phần 1 — Bảng tổng hợp tất cả linh kiện

| # | Tên BOM gốc | Proteus Name | SL | Chân kết nối | Chức năng trong hệ thống | Trạng thái |
|:--|:-----------|:------------|:--:|:-------------|:------------------------|:-----------:|
| 1 | AT89C52 | AT89C52 | 1 | VCC=40, GND=20, EA=31 | Vi điều khiển chính | ✅ |
| 2 | LM016L | LM016L | 1 | RS=P2.6, E=P2.7, D4-D7=P3.4-P3.7 | LCD 16×2 hiển thị trạng thái + đồng hồ | ✅ |
| 3 | 7SEG-MPX2-CA | 7SEG-MPX2-CA | 1 | Seg=P0, DIG1=P2.0, DIG2=P2.1 | Hiển thị nhiệt độ 2 chữ số | ✅ |
| 4 | MOTOR-ENCODER | MOTOR-ENCODER | 1 | Qua L293D, xung→P3.2 | Động cơ mở/đóng cửa + đếm xung | ✅ |
| 5 | L293D | L293D | 1 | IN1=P2.2, IN2=P2.3 | Driver cầu H điều chiều motor | ✅ |
| 6 | SPEAKER | SPEAKER | 1 | Collector BC547 (P2.5) | Phát giai điệu chào + âm mở cửa | ✅ |
| 7 | BUZZER | BUZZER | 1 | Collector BC547 (P2.5, // SPEAKER) | Còi báo động lockdown | ✅ |
| 8 | 2N2222 | 2N2222 | 6 | Base qua 1kΩ | Đệm dòng FAN, BULB, digit 7SEG | ✅ |
| 9 | BC547 | BC547 | 1 | P2.5 qua 1kΩ | Driver dòng cao cho SPEAKER+BUZZER | ✅ |
| 10 | FAN1-TABLE | FAN | 1 | Collector 2N2222, P1.7 | Quạt tự động theo nhiệt độ | ✅ |
| 11 | BULB | BULB | 1 | Collector 2N2222, P2.4 (PWM) | Đèn chiếu sáng điều chỉnh 5 mức | ✅ |
| 12 | LED-G | LED-GREEN | 1 | // BULB qua 220Ω | Chỉ thị đèn đang bật | ✅ |
| 13 | LED-R | LED-RED | 1 | // BUZZER qua 220Ω | Chỉ thị báo động | ✅ |
| 14 | LED-YELLOW | LED-YELLOW | 1 | // digit select 7SEG | Chỉ thị 7SEG đang quét | ✅ |
| 15 | BUTTON | BUTTON | 8 | P1.0-P1.5 (MCU1,3) | Mật khẩu (×4), nhiệt độ (×2), độ sáng (×2) | ✅ |
| 16 | LDR | LDR | 1 | Phân áp → LM358 IN+ | Cảm biến ánh sáng | ✅ |
| 17 | LM358 | LM358 | 1 | OUT→P1.6 | So sánh LDR → tín hiệu số cho MCU | ✅ |
| 18 | 1N4007 | 1N4007 | 2 | // ngược motor và FAN | Diode chống ngược EMF | ✅ |
| 19 | RES-220 | RES | 12 | Segment, LED, đèn nền LCD | Giới hạn dòng LED ~10mA | ✅ |
| 20 | MINRES1K | RES | 8 | Base transistor | Giới hạn dòng điều khiển base | ✅ |
| 21 | MINRES10K | RES | 10 | Pull-up P0 (×8), phân áp LDR, contrast LCD | Kéo lên, phân áp | ✅ |
| 22 | POWER SOURCE DC | POWER | 1 | VCC toàn mạch | Nguồn 5V DC | ✅ |
| 23 | VIRTUAL TERMINAL | VIRTUAL TERMINAL | 1 | P3.0 (TXD) | Xem log UART trên Proteus | ✅ |
| 24 | 7SEG-MPX4-CA | 7SEG-MPX4-CA | — | — | Hiển thị 4 chữ số (giờ HHMM) — cần thêm 2 chân digit | ⬜ |
| 25 | 7SEG-COM-ANODE | 7SEG-COM-ANODE | — | — | 1 digit đơn thay thế MPX2 | ⬜ |
| 26 | DS1802 | DS1802 | — | — | Chiết áp số SPI — điều chỉnh volume speaker | ⬜ |
| 27 | BC557 | BC557 | — | — | PNP — driver digit select kiểu kéo lên | ⬜ |
| 28 | 2N1711 | 2N1711 | — | — | NPN điện áp cao — tải 12V | ⬜ |
| 29 | MMBTA42 | MMBTA42 | — | — | NPN điện áp cao SMD | ⬜ |
| 30 | LED-B | LED-BLUE | — | — | LED xanh dương — chỉ thị thêm | ⬜ |
| 31 | LED-BIBY | LED-BIBY | — | — | LED nháy — chỉ thị trạng thái | ⬜ |
| 32 | SWITCH 1 | SWITCH | — | — | Công tắc mode hoặc nguồn tổng | ⬜ |
| 33 | SWITCH 2 | SWITCH | — | — | Công tắc dự phòng | ⬜ |
| 34 | POT-HG | POT-HG | — | — | Đã thay bằng 2 nút bấm BTN_BRIGHT | ❌ |
| 35 | POTENTIOMETER | POT | — | — | Đã thay bằng 2 nút bấm BTN_BRIGHT | ❌ |
| 36 | 3005P-1-101 | — | — | — | Biến trở tinh chỉnh PCB | ⬜ |
| 37 | 3WATT4K7 | RES | — | — | Điện trở công suất 4.7kΩ — tải dòng lớn | ⬜ |
| 38 | CHIPRES1K | RES | — | — | Điện trở dán 1kΩ | ⬜ |
| 39 | CHIPRES4K7 | RES | — | — | Điện trở dán 4.7kΩ | ⬜ |
| 40 | CAP-GN | CAP | — | — | Tụ lọc nguồn 100nF | ⬜ |
| 41 | BATTERY 12V | BATTERY | — | — | Nguồn 12V cho motor công suất lớn | ⬜ |
| 42 | BATTERY-PP3 | BATTERY | — | — | Pin 9V dự phòng | ⬜ |
| 43 | CELL-AA | CELL | — | — | Pin AA 1.5V | ⬜ |
| 44 | FAN2-TABLE | FAN | — | — | Quạt loại 2 thay thế FAN1 | ⬜ |
| 45 | FAN1-COOLING | FAN | — | — | Quạt làm mát CPU | ⬜ |
| 46 | FAN1-EXHAUST | FAN | — | — | Quạt hút | ⬜ |
| 47 | FAN1-PEDESTAL | FAN | — | — | Quạt cây | ⬜ |
| 48 | MOTOR-DC | MOTOR | — | — | Động cơ DC không encoder | ⬜ |
| 49 | MOTOR | MOTOR | — | — | Động cơ DC tổng quát | ⬜ |
| 50 | SD | — | — | — | Thẻ nhớ SD — lưu log dài hạn | ⬜ |
| 51 | PUSH-BUTTON | BUTTON | — | — | Tương tự BUTTON | ⬜ |
| 52 | POWER SOURCE AC | — | — | — | Nguồn xoay chiều — không dùng trong mô phỏng | ⬜ |

---

## Phần 2 — Giới thiệu chi tiết các linh kiện đang dùng (✅)

---

### 1. AT89C52 — Vi điều khiển chính

*(hình — chụp IC AT89C52 thực tế hoặc datasheet pinout)*

**Giới thiệu:**
AT89C52 là vi điều khiển 8-bit thuộc họ 8051 của hãng Atmel. Có 8 KB Flash, 256 byte RAM, 32 chân I/O chia 4 cổng (P0–P3), 3 timer (Timer 0/1/2), 1 UART và 2 ngắt ngoài.

**Thông số cơ bản:**
- Nguồn: 4.0V – 5.5V
- Tần số: tối đa 24 MHz (dùng 11.0592 MHz cho UART chính xác)
- Flash: 8 KB | RAM: 256 byte | Gói: DIP-40

**Công dụng trong hệ thống:**
Là "bộ não" điều phối toàn bộ 8 chức năng: đọc nút bấm, điều khiển LCD/7-SEG/motor/quạt/đèn, giao tiếp UART, xử lý mật khẩu và đồng hồ thời gian.

---

### 2. LM016L — Màn hình LCD 16×2

*(hình — chụp LCD hiển thị "SMART HOME SYS" trong Proteus)*

**Giới thiệu:**
LCD 16×2 hiển thị 2 dòng, mỗi dòng 16 ký tự ASCII. Chip điều khiển tương thích HD44780. Hỗ trợ giao tiếp 4-bit (tiết kiệm 4 chân so với 8-bit).

**Thông số cơ bản:**
- Nguồn: 5V | Giao tiếp: 4-bit (6 chân: D4–D7, RS, E)
- Tương phản: chỉnh qua biến trở nối chân VO (pin 3)

**Công dụng trong hệ thống:**
- Dòng 1: trạng thái hệ thống (chào mừng / nhập mật khẩu / cảnh báo) + đồng hồ MM:SS góc phải
- Dòng 2: độ sáng đèn "SANG: XX%"

---

### 3. 7SEG-MPX2-CA — LED 7 đoạn 2 số (Anode chung)

*(hình — chụp 7SEG hiển thị "30" trong Proteus)*

**Giới thiệu:**
Module 2 chữ số LED 7 đoạn loại anode chung (common anode). Để sáng một đoạn cần kéo cathode xuống mức thấp (active-low). Dùng kỹ thuật multiplexing để quét luân phiên từng digit.

**Thông số cơ bản:**
- Loại: Anode chung | Điện áp mỗi đoạn: ~2V, dòng ~10mA
- Cần điện trở 220Ω giới hạn dòng mỗi đoạn

**Công dụng trong hệ thống:**
Hiển thị nhiệt độ (0–50°C). Quét 2 digit luân phiên mỗi 2ms → mắt người thấy cả 2 sáng liên tục.

---

### 4. MOTOR-ENCODER — Động cơ DC có encoder

*(hình — chụp motor encoder kết nối L293D trong Proteus)*

**Giới thiệu:**
Động cơ DC tích hợp encoder phát xung mỗi khi trục quay một góc nhất định, cho phép đo chính xác góc quay hoặc số vòng.

**Thông số cơ bản:**
- Nguồn motor: 5V–12V DC
- Xung encoder: tín hiệu số 0/1
- Dự án dùng: 50 xung = mở/đóng cửa hoàn toàn

**Công dụng trong hệ thống:**
Kéo cơ cấu cửa mở/đóng. Xung encoder vào INT0 (P3.2) để đếm chính xác hành trình, tránh kẹt cơ học.

---

### 5. L293D — IC điều khiển động cơ cầu H

*(hình — chụp IC L293D và sơ đồ chân)*

**Giới thiệu:**
IC driver 2 kênh cầu H, điều khiển chiều quay động cơ DC. Tích hợp diode flyback bảo vệ. Chịu 600mA/kênh.

**Thông số cơ bản:**
- VCC1 (logic): 5V | VCC2 (motor): 4.5V–36V
- Dòng tối đa: 600mA/kênh (peak 1.2A)

**Công dụng trong hệ thống:**
Nhận lệnh từ P2.2/P2.3 để đảo chiều motor cửa:

| IN1 | IN2 | Motor |
|:---:|:---:|:------|
| 1 | 0 | Quay thuận → mở cửa |
| 0 | 1 | Quay ngược → đóng cửa |
| 0 | 0 | Dừng |

---

### 6. SPEAKER — Loa phát âm thanh

*(hình — chụp loa nhỏ và dạng sóng PWM trong Proteus)*

**Giới thiệu:**
Loa điện động chuyển tín hiệu điện xoay chiều thành âm thanh. Khác với buzzer, loa phát được nhiều tần số khác nhau tùy tín hiệu đầu vào (PWM).

**Thông số cơ bản:**
- Trở kháng: 8Ω hoặc 16Ω
- Cần tín hiệu AC/PWM, cần transistor đệm dòng

**Công dụng trong hệ thống:**
Phát giai điệu chào khi khởi động (3 nốt tăng dần), âm báo mở cửa (3 tiếng bíp), âm xác nhận điều chỉnh độ sáng.

---

### 7. BUZZER — Còi điện tử

*(hình — chụp buzzer 5V và ký hiệu trong Proteus)*

**Giới thiệu:**
Buzzer tích hợp mạch dao động bên trong — chỉ cần cấp DC là phát tiếng kêu cố định. Mắc song song với SPEAKER trên cùng transistor BC547 (P2.5).

**Thông số cơ bản:**
- Nguồn: 5V DC | Tần số âm: cố định ~2–4 kHz | Dòng: 20–40mA

**Công dụng trong hệ thống:**
Báo động lockdown khi nhập sai mật khẩu 3 lần — kết hợp cùng SPEAKER tạo âm báo động mạnh hơn.

---

### 8. Transistor 2N2222 — NPN đa dụng

*(hình — chụp TO-92 2N2222 và sơ đồ chân B-C-E)*

**Giới thiệu:**
Transistor NPN silicon phổ biến nhất, dùng làm công tắc điện tử. Khi Base được kéo lên qua điện trở → Collector–Emitter dẫn điện (ON).

**Thông số cơ bản:**
- VCE max: 40V | IC max: 600mA | hFE: 75–300 | Gói: TO-92

**Công dụng trong hệ thống (6 cái):**
- ×2 digit select 7SEG (P2.0, P2.1)
- ×1 FAN (P1.7)
- ×1 BULB (P2.4)
- ×2 dự phòng / LED chỉ thị

---

### 9. BC547 — NPN dòng audio

*(hình — chụp TO-92 BC547)*

**Giới thiệu:**
Transistor NPN tương tự 2N2222 nhưng hệ số khuếch đại β cao hơn (110–800), phù hợp hơn cho tín hiệu âm thanh tần số cao.

**Thông số cơ bản:**
- VCE max: 45V | IC max: 100mA | hFE: 110–800 | Gói: TO-92

**Công dụng trong hệ thống:**
Driver riêng cho SPEAKER + BUZZER (P2.5). β cao giúp khuếch đại tín hiệu PWM âm thanh rõ hơn 2N2222.

---

### 10. FAN — Quạt làm mát

*(hình — chụp quạt DC trong Proteus đang quay)*

**Giới thiệu:**
Quạt DC mini mô phỏng hệ thống điều hòa/làm mát phòng. Điều khiển on/off qua transistor 2N2222.

**Thông số cơ bản:**
- Nguồn: 5V DC | Dòng: 100–300mA

**Công dụng trong hệ thống:**
Tự động bật khi nhiệt độ > 26°C, tắt khi ≤ 25°C. Log trạng thái qua UART.

---

### 11. BULB — Bóng đèn chiếu sáng

*(hình — chụp bóng đèn sáng/tắt trong Proteus)*

**Giới thiệu:**
Bóng đèn sợi đốt mô phỏng hệ thống chiếu sáng. Điều khiển qua transistor 2N2222 với tín hiệu PWM để điều chỉnh độ sáng.

**Công dụng trong hệ thống:**
- Tự động bật/tắt theo cảm biến LDR
- Điều chỉnh 5 mức độ sáng (0/20/40/60/80/100%) bằng 2 nút bấm
- Software PWM 50Hz trong Timer0 ISR

---

### 12. LED-G — Chỉ thị đèn bật (xanh lá)

*(hình — chụp LED xanh sáng trong mạch)*

**Công dụng trong hệ thống:**
Mắc song song với BULB (qua 220Ω) — sáng khi đèn nhà đang bật, giúp nhìn thấy trạng thái đèn trực quan.

---

### 13. LED-R — Chỉ thị báo động (đỏ)

*(hình — chụp LED đỏ nhấp nháy khi báo động)*

**Công dụng trong hệ thống:**
Mắc song song với BUZZER (qua 220Ω) — nhấp nháy đỏ đồng thời khi còi kêu báo động lockdown.

---

### 14. LED-YELLOW — Chỉ thị 7-SEG đang hoạt động

*(hình — chụp LED vàng nhấp nháy nhanh)*

**Công dụng trong hệ thống:**
Mắc song song với transistor digit select — nhấp nháy 50Hz chỉ thị 7-SEG đang quét. Trong thực tế dùng làm chỉ thị "hệ thống đang chạy".

---

### 15. BUTTON — Nút bấm (×8)

*(hình — chụp 8 nút bấm bố trí trên schematic Proteus)*

**Giới thiệu:**
Nút bấm SPST 4 chân. Khi nhấn nối chân vi điều khiển xuống GND — MCU đọc mức thấp = đang nhấn. Port 1 và 3 có pull-up nội nên không cần thêm điện trở kéo lên.

**Công dụng trong hệ thống:**

| Nút | Chân | Chức năng |
|:----|:-----|:----------|
| BTN1 | P1.0 | Mật khẩu chữ số '1' / giữ 3s = đổi mật khẩu |
| BTN2 | P1.1 | Mật khẩu chữ số '2' |
| BTN3 | P1.2 | Mật khẩu chữ số '3' |
| BTN4 | P1.3 | Mật khẩu chữ số '4' |
| BTN_TEMP_UP | P1.0 (MCU3) | Tăng nhiệt độ mô phỏng |
| BTN_TEMP_DN | P1.1 (MCU3) | Giảm nhiệt độ mô phỏng |
| BTN_BRIGHT_UP | P1.4 (MCU3) | Tăng độ sáng đèn (thay potentiometer) |
| BTN_BRIGHT_DN | P1.5 (MCU3) | Giảm độ sáng đèn (thay potentiometer) |

---

### 16. LDR — Cảm biến ánh sáng

*(hình — chụp LDR và mạch comparator LM358 trong Proteus)*

**Giới thiệu:**
LDR (Light Dependent Resistor) — điện trở thay đổi theo cường độ ánh sáng: tối → điện trở cao (~1MΩ), sáng → điện trở thấp (~1kΩ). Kết hợp mạch phân áp + LM358 để số hóa tín hiệu.

**Công dụng trong hệ thống:**
Phát hiện trời tối → tự động bật BULB. Ngưỡng sáng/tối điều chỉnh qua biến trở 10kΩ nối LM358 IN−.

---

### 17. LM358 — Op-Amp so sánh

*(hình — chụp LM358 và đồ thị IN+/IN−/OUT)*

**Giới thiệu:**
IC op-amp 2 kênh, nguồn đơn. Dùng ở chế độ comparator (không hồi tiếp): IN+ > IN− → OUT = HIGH; IN+ < IN− → OUT = LOW.

**Thông số cơ bản:**
- Nguồn: 3V–32V (single supply) | 2 op-amp trong 1 IC DIP-8

**Công dụng trong hệ thống:**
Đã thay thế bằng SWITCH trên P1.2 (MCU3) để mô phỏng ánh sáng. LM358 không còn sử dụng.

---

### 18. 1N4007 — Diode bảo vệ flyback

*(hình — chụp diode 1N4007 mắc song song motor)*

**Giới thiệu:**
Diode chỉnh lưu silicon chịu 1000V ngược, 1A. Dùng làm "freewheeling diode" hấp thụ điện áp ngược từ cuộn cảm khi transistor tắt đột ngột.

**Thông số cơ bản:**
- VR max: 1000V | IF max: 1A | Vf: ~0.7V

**Công dụng trong hệ thống:**
Mắc ngược song song với MOTOR và FAN — bảo vệ transistor 2N2222 khỏi bị đánh thủng bởi điện áp cảm ứng.

---

### 19–21. Điện trở (RES-220 / MINRES1K / MINRES10K)

*(hình — chụp các điện trở trên schematic Proteus)*

| Loại | Giá trị | Vị trí | Mục đích |
|:-----|:--------|:-------|:---------|
| RES-220 | 220 Ω | LED, đoạn 7-SEG, đèn nền LCD | Giới hạn dòng ~10mA |
| MINRES1K | 1 kΩ | Base transistor | Giới hạn dòng điều khiển ~4mA |
| MINRES10K | 10 kΩ | Pull-up P0 (×8), phân áp LDR, contrast LCD | Kéo lên / phân áp |

> **Tại sao Port 0 cần pull-up 10kΩ?** Port 0 là open-drain, không có pull-up nội → nếu thiếu, mức logic 1 bị "trôi nổi", 7-SEG hiển thị sai.

---

### 22. POWER SOURCE DC — Nguồn 5V

*(hình — ký hiệu nguồn DC trong Proteus)*

**Công dụng trong hệ thống:**
Trong Proteus dùng terminal **POWER (VCC)** và **GND** ảo để cấp 5V cho toàn bộ mạch. Thực tế dùng adapter 5V/2A hoặc mạch ổn áp LM7805.

---

### 23. VIRTUAL TERMINAL — Cổng UART ảo

*(hình — chụp Virtual Terminal Proteus hiển thị log hệ thống)*

**Giới thiệu:**
Mô phỏng thiết bị đầu cuối nối tiếp trong Proteus, tương đương PuTTY hoặc Serial Monitor.

**Cấu hình:** Baud=9600, Data=8, Parity=None, Stop=1

**Công dụng trong hệ thống:**
Hiển thị log thời gian thực:
```
=== SMART HOME SYS - Group One ===
[00:00] DANG NHAP: THANH CONG
[00:01] CUA: DA MO
[00:45] QUAT: BAT (nhiet do cao)
[01:10] DEN: BAT (troi toi)
[02:30] DO SANG TANG
```

---

## Cách thêm linh kiện mới

1. Trong bảng Phần 1: đổi ⬜ → ✅, điền SL + Chân + Chức năng
2. Cập nhật `inc/config.h` — thêm khai báo `sbit` cho chân mới
3. Viết code trong module tương ứng hoặc tạo file `.c` mới trong `src/`
4. Cập nhật `docs/wiring_guide.md` — thêm sơ đồ đấu nối
5. Thêm mục giới thiệu vào Phần 2 của file này
6. Cập nhật `progress.md` — đánh dấu hoàn thành
