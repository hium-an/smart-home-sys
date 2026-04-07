# Danh sách Chức năng (Features)

| STT | Chức năng (Feature) | Code / Function | Trạng thái | Mô tả hoạt động | Yêu cầu Phần cứng / Ghi chú |
|:---|:---|:---|:---|:---|:---|
| **1.** | **Khởi tạo hệ thống** | `LCD_Init()`, `Welcome()` | ✅ Done | Hiệu ứng loading bar + "SMART HOME SYS by Hieu" | Cần điện trở kéo lên 10k cho Port 0. |
| **2.** | **Đồng hồ thời gian** | `Timer0_ISR()`, `Display_Clock()` | ✅ Done | Timer0 ngắt mỗi 1ms, đếm MM:SS, hiển thị góc phải LCD. | Timer0 Mode 1, 11.0592 MHz |
| **3.** | **Bảo mật & Đăng nhập** | Tích hợp trong `main()` | ✅ Done | Nhập 4 số để mở cửa. Sai 3 lần → `Security_Lockdown()` báo động. | |
| **3.1** | Đổi mật khẩu | `Change_Pass_Mode()` | ✅ Done | Giữ BTN1 (P1.0) 3 giây → nhập pass mới + xác nhận. | Lưu trong RAM (biến tĩnh) |
| **4.** | **Điều khiển cửa** | `Door_Control()` | ✅ Done | Motor quay mở/đóng cửa + đếm encoder + UART log. | Dùng 2N2222 đệm dòng cho Motor. |
| **5.** | **Giám sát nhiệt độ** | `Check_Temp_Buttons()`, Timer0 ISR | ✅ Done | 2 nút P3.3/P3.4 tăng/giảm. LED 7 thanh 2 số (shared P0). | 7SEG-MPX2-CA, P2.2/P2.3 digit select |
| **6.** | **Điều hòa tự động** | `Auto_Fan()` | ✅ Done | Quạt tự bật khi > 25°C, tắt khi ≤ 25°C. Log qua UART. | FAN = P1.7 |
| **7.** | **Chiếu sáng** | `Light_Logic()` | ✅ Done | LDR (P1.5) → tối thì bật BULB (P1.6), sáng thì tắt. Log qua UART. | |
| **8.** | **Log Terminal** | `UART_Log()` | ✅ Done | UART 9600 8N1 via Timer1. Log có timestamp [MM:SS]. | P3.0 (TXD) → Virtual Terminal |




Nhiệm vụ:
Quy trình thiết kế và lập trình hệ thống nhúng (Workflow)
Bước 1: Tiếp nhận yêu cầu & Danh sách linh kiện

Bạn cung cấp danh sách linh kiện (BOM) được phép sử dụng và mô tả chức năng, kịch bản mong muốn của hệ thống.
Tôi sẽ xác nhận rằng các mong muốn của bạn có thể thực hiện được với các linh kiện đó. (Nếu thiếu linh kiện thiết yếu, tôi sẽ báo cáo và đề xuất giải pháp để bạn quyết định).
Bước 2: Thiết kế kiến trúc & Sơ đồ đấu nối (Hardware Design)

Tôi sẽ phân bổ các chân kết nối (Pinout) cho vi điều khiển (VD: Chân nào dùng cho giao tiếp I2C, SPI, chân nào cấu hình PWM, ADC, GPIO...).
Đề xuất bảng thiết kế kết nối điện (Wiring table) thật chi tiết giữa các module để bạn có thể lắp ráp thực tế hoặc vẽ mạch.
Bước 3: Lên phương án thuật toán & Cấu trúc mã nguồn (Software Architecture)

Tôi sẽ phác thảo cấu trúc chương trình (VD: Dùng RTOS hay Super-Loop, cấu trúc thư mục code, các thư viện sẽ sử dụng, lưu đồ thuật toán).
Bước 4: Viết mã nguồn (Coding) & Giải thích

Tôi sẽ bắt tay lập trình từng tính năng (Feature-by-Feature) dựa vào kiến trúc đã chốt ở Bước 3.
Các đoạn mã sẽ được chú thích (comment) rõ ràng. Bạn có thể chép code vào phần mềm để biên dịch thử.
Bước 5: Tổng hợp, Debug & Viết Báo cáo (Final Report)

Hỗ trợ bạn sửa lỗi (nếu có lỗi trong quá trình biên dịch hoặc chạy thực tế).
Khi hệ thống chạy ổn định, tôi sẽ viết một Báo cáo mô tả chi tiết những gì đã làm, nguyên lý vận hành, thông số cấu hình và cách sử dụng code.




Danh sách linh kiện (Unique Items)
1N4007 (Diode chỉnh lưu)

2N1711 (Transistor NPN)

2N2222 (Transistor NPN phổ biến)

3005P-1-101 (Biến trở tinh chỉnh)

3WATT4K7 (Điện trở công suất 3W - 4.7kΩ)

7SEG-COM-ANODE (LED 7 đoạn Anode chung)

7SEG-MPX2-CA (LED 7 đoạn 2 số, Anode chung)

7SEG-MPX4-CA (LED 7 đoạn 4 số, Anode chung)

AT89C51 (Vi điều khiển họ 8051)

AT89C52 (Vi điều khiển họ 8051, bộ nhớ lớn hơn C51)

BATTERY 12V (Nguồn pin/ắc quy 12V)

BATTERY-PP3 (Pin 9V chuẩn PP3)

BC547 (Transistor NPN)

BC557 (Transistor PNP)

BULB (Bóng đèn sợi đốt)

BUTTON (Nút nhấn đơn)

CAP-GN (Tụ điện nói chung)

CELL-AA (Pin tiểu AA 1.5V)

CHIPRES1K (Điện trở dán 1kΩ)

CHIPRES4K7 (Điện trở dán 4.7kΩ)

DS1802 (Chiết áp kỹ thuật số)

FAN1-COOLING (Quạt làm mát loại 1)

FAN1-EXHAUST (Quạt hút loại 1)

FAN1-PEDESTAL (Quạt cây loại 1)

FAN1-TABLE (Quạt bàn loại 1)

FAN2-COOLING (Quạt làm mát loại 2)

FAN2-EXHAUST (Quạt hút loại 2)

FAN2-PEDESTAL (Quạt cây loại 2)

FAN2-TABLE (Quạt bàn loại 2)

L293D (IC điều khiển động cơ cầu H)

LED-B (LED màu xanh dương - Blue)

LED-BIBY (LED nháy hoặc LED hai màu)

LED-G (LED màu xanh lá - Green)

LED-R (LED màu đỏ - Red)

LED-RED (LED màu đỏ - tương tự LED-R)

LED-YELLOW (LED màu vàng)

LM016L (Màn hình LCD 16x2)

MINRES1K (Điện trở nhỏ 1kΩ)

MINRES10K (Điện trở nhỏ 10kΩ)

MMBTA42 (Transistor NPN điện áp cao)

MOTOR (Động cơ DC nói chung)

MOTOR-DC (Động cơ điện một chiều)

MOTOR-ENCODER (Động cơ có bộ đếm vòng quay)

POT-HG (Biến trở có độ chia cao)

POTENTIOMETER (Biến trở/Chiết áp)

POWER SOURCE AC (Nguồn xoay chiều)

POWER SOURCE DC (Nguồn một chiều)

PUSH-BUTTON (Nút nhấn)

RES (Điện trở nói chung)

RES-220 (Điện trở 220Ω)

SD (Có thể là thẻ nhớ SD hoặc Diode đặc biệt tùy thư viện)

SWITCH 1 (Công tắc loại 1)

SWITCH 2 (Công tắc loại 2)




Code mau: 
Bai1:
#include <AT89X52.H>
#include <stdio.h>

#define led1 P3_7
#define button P3_2

void delay(int x) {
	int i,j;
	for(i = 0; i<x; i++) {
		for(j=0; j<125; j++);
	}
}

void main() {
	while (1) {
		if (button == 0) {
			led1 = 1;
		} else {
			led1 = 0;
		}
	} 
	/*
	while (1) {
		led1 = 0;
		delay(1000);
		led1 = 1;
		delay(1000);
	}  */
}

Bai2:
#include <AT89X52.H>

#define button P1_0

char led_7_thanh[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void delay(int x) {
    int i, j;
    for(i = 0; i < x; i++) {
        for(j = 0; j < 125; j++);
    }
}

void main() {
    int count = 0;
    P0 = 0xFF;

	delay(500);
	// Truong hop 1 bam nut dem so
    while(1) {
        P0 = led_7_thanh[count];

        if (button == 0) {
            count++;
            if (count > 9) {
                count = 0; 
            }
			while(button == 0);
        }
    }	 
	// Truong hop 2 tu dong dem so
	/*	
	while(1) {
        P0 = led_7_thanh[count];
		delay(500);
		count++;
		if (count > 9) {
			count = 0;
		}
    }
	*/  
}


Bai3:

#include <REGX52.H>

// Dinh nghia pin
#define LED1 P2_0
#define LED2 P2_1
#define LED3 P2_2
#define LED4 P2_3

char LED_7_THANH[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
int so = 0;
int nghin, tram, chuc, dv;

// PLT SOLUTIONS
void delay_ms(int x) // Ham tre x (ms)
{
    int i, j;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < 125; j++);
    }
}

void hienthi_LED_7_THANH(int x){
    // Buoc 1: Tach cac so hang: donvi, chuc, tram, nghin
    nghin = x / 1000;
    tram = (x % 1000) / 100;
    chuc = (x % 100) / 10;
    dv = x % 10;
    
    // Buoc 2
    LED1 = 0; // off
    LED2 = 0; // off
    LED3 = 0; // off
    LED4 = 0; // off
    
    P0 = LED_7_THANH[nghin]; LED1 = 1; delay_ms(2); LED1 = 0;
    P0 = LED_7_THANH[tram];  LED2 = 1; delay_ms(2); LED2 = 0;
    P0 = LED_7_THANH[chuc];  LED3 = 1; delay_ms(2); LED3 = 0;
    P0 = LED_7_THANH[dv];    LED4 = 1; delay_ms(2); LED4 = 0;
}

void main(){
    so = 3010;
    // Vi du dung nut nhan de tang/giam
    while(1){
        hienthi_LED_7_THANH(so);
        
        if(P1_0 == 0){ // nhan nut "Tang"
            so++;
            while(P1_0 == 0){}
        }
        
        if(P1_7 == 0){ // nhan nut "Giam"
            so--;
            while(P1_7 == 0){}
        }
        
        // Hien thi bien So:
        hienthi_LED_7_THANH(so);
    }
}

Bai4:

#include <REGX52.H>

// Dinh nghia pin
#define LED1 P2_0
#define LED2 P2_1
#define LED3 P2_2
#define LED4 P2_3

// Dinh nghia nua nhan
#define onButton P1_0
#define offButton P1_7

// Dinh nghia LED don
#define LED_DO P2_5
#define LED_VANG P2_4

char LED_7_THANH[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; 
int so = 2001;

int he_thong_chay = 0;

// PLT SOLUTIONS
void delay_ms(int x) // Ham tre x (ms)
{
    int i, j;
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < 125; j++);
    }
}

void hienthi_LED_7_THANH(int x) {
    int nghin, tram, chuc, dv;
    
    // Buoc 1: Tach cac so hang: donvi, chuc, tram, nghin
    nghin = x / 1000; 
    tram = (x % 1000) / 100; 
    chuc = (x % 100) / 10; 
    dv = x % 10; 
    
    // Buoc 2 
    LED1 = 0; // off 
    LED2 = 0; // off 
    LED3 = 0; // off 
    LED4 = 0; // off 
    
    P0 = LED_7_THANH[nghin]; LED1 = 1; delay_ms(2); LED1 = 0; 
    P0 = LED_7_THANH[tram];  LED2 = 1; delay_ms(2); LED2 = 0; 
    P0 = LED_7_THANH[chuc];  LED3 = 1; delay_ms(2); LED3 = 0;
    P0 = LED_7_THANH[dv];    LED4 = 1; delay_ms(2); LED4 = 0;
}

int xung = 0;
void ngatINT0(void) interrupt 0
{
    xung++;
}

int thoi_gian = 0; 
float tocdo = 0; 

// Ham phuc vu ngat Timer 0 
void timer0(void) interrupt 1 
{
    thoi_gian++; 
    if (thoi_gian == 20) { 
        tocdo = (float)xung * 60 / 100;
        thoi_gian = 0; 
        xung = 0; 
    }
    TH0 = 0x3C;
    TL0 = 0xB0;
    TR0 = 1; // Start lai Timer 0 
}

void main() {

    // Buoc 1: Setup LED 7 thanh
    // Buoc 2: Dung ngat ngoai INT0 de dem xung ra tu Encoder
    
    // Khai bao ngat ngoai 0
    EX0 = 1;
    IT0 = 1;
    EA = 1;
    
    // Buoc 3: Khai bao ngat Timer
    TMOD = 0x01;
    // Dat gia tri ban dau cho 2 thanh ghi cua T0:
    TH0 = 0x3C; 
    TL0 = 0xB0; 
    
    // Khai bao ngat Timer 0
    ET0 = 1;
    
    // Start timer 0 
    TR0 = 1;
  
    // Ban dau tat het LED don
    LED_DO = 0;
    LED_VANG = 0;

    while (1) {
        // 1. Kiem tra nut ON
        if (onButton == 0) {
            delay_ms(20); // Chong rung
            if (onButton == 0) he_thong_chay = 1;
        }

        // 2. Kiem tra nut OFF
        if (offButton == 0) {
            delay_ms(20); // Chong rung
            if (offButton == 0) {
                he_thong_chay = 0;
                LED_DO = 0;   // Tat het den khi OFF
                LED_VANG = 0;
            }
        }

        // 3. Xu ly logic den bao khi he thong dang Bat
        if (he_thong_chay) {
            if (tocdo <= 50) {
                LED_DO = 1;   // Toc do thap thi den do sang
                LED_VANG = 0;
            } else {
                LED_DO = 0;   // Toc do > 50 thi den do tat
                LED_VANG = 1; // Den vang sang
            }
            hienthi_LED_7_THANH((int)tocdo); 
        } else {
            // Neu he thong tat thi LED 7 doan hien 0000 hoac tat han
            hienthi_LED_7_THANH(0);
        }
    }
}

Bai5:

#include <REGX51.H>
#include <stdio.h>

// --- DINH NGHIA CHUNG ---
#define LINE_1      0x80
#define LINE_2      0xC0
#define CLEAR_LCD   0x01

// ============================================================
//                   C?U HÌNH CHO LCD 1 (Port 0 & Port 2)
// ============================================================
#define LCD1_D4  P0_4
#define LCD1_D5  P0_5
#define LCD1_D6  P0_6
#define LCD1_D7  P0_7
#define LCD1_RS  P2_0
#define LCD1_RW  P2_1
#define LCD1_E   P2_2

// ============================================================
//                   C?U HÌNH CHO LCD 2 (Port 2 & Port 3)
// ============================================================
// D?a trên so d? m?ch b?n g?i:
#define LCD2_D4  P0_0
#define LCD2_D5  P0_1
#define LCD2_D6  P0_2
#define LCD2_D7  P0_3
#define LCD2_RS  P2_5
#define LCD2_RW  P2_4
#define LCD2_E   P2_3

// --- CAC HAM DIEN KHIEU (DELAY) ---
void delay_us(unsigned int Time)
{
    unsigned int i, j;
    for (i = 0; i < Time; i++)
        for (j = 0; j < 2; j++);
}

void delay_ms(unsigned int Time)
{
    unsigned int i, j;
    for (i = 0; i < Time; i++)
        for (j = 0; j < 125; j++);
}

// ============================================================
//                   CÁC HÀM CHO LCD 1
// ============================================================
void lcd1_enable(void)
{
    LCD1_E = 1; delay_us(3); LCD1_E = 0; delay_us(5);
}

void lcd1_send_4bit_data(unsigned char cX)
{
    LCD1_D4 = cX & 0x01;
    LCD1_D5 = (cX >> 1) & 1;
    LCD1_D6 = (cX >> 2) & 1;
    LCD1_D7 = (cX >> 3) & 1;
}

void lcd1_send_command(unsigned char cX)
{
    lcd1_send_4bit_data(cX >> 4);
    lcd1_enable();
    lcd1_send_4bit_data(cX);
    lcd1_enable();
}

void lcd1_init(void)
{
    lcd1_send_4bit_data(0x00); delay_ms(200);
    LCD1_RS = 0; LCD1_RW = 0; LCD1_E = 0;
    lcd1_send_4bit_data(0x03); lcd1_enable(); lcd1_enable(); lcd1_enable();
    lcd1_send_4bit_data(0x02); lcd1_enable();
    lcd1_send_command(0x2C);
    lcd1_send_command(0x0C);
    lcd1_send_command(0x06);
    lcd1_send_command(CLEAR_LCD);
}

void lcd1_gotoxy(unsigned char x, unsigned char y)
{
    unsigned char address;
    if (!y) address = (LINE_1 + x);
    else address = (LINE_2 + x);
    delay_ms(1);
    lcd1_send_command(address);
    delay_ms(1);
}

void lcd1_putchar(unsigned int cX)
{
    LCD1_RS = 1;
    lcd1_send_command(cX);
    LCD1_RS = 0;
}

void lcd1_puts(char *s)
{
    while (*s) { lcd1_putchar(*s); s++; }
}

// ============================================================
//                   CÁC HÀM CHO LCD 2 (VI?T M?I)
// ============================================================
void lcd2_enable(void)
{
    LCD2_E = 1; delay_us(3); LCD2_E = 0; delay_us(5);
}

// Ham gui du lieu ra P2.4 -> P2.7
void lcd2_send_4bit_data(unsigned char cX)
{
    LCD2_D4 = cX & 0x01;
    LCD2_D5 = (cX >> 1) & 1;
    LCD2_D6 = (cX >> 2) & 1;
    LCD2_D7 = (cX >> 3) & 1;
}

void lcd2_send_command(unsigned char cX)
{
    lcd2_send_4bit_data(cX >> 4); // Gui 4 bit cao
    lcd2_enable();
    lcd2_send_4bit_data(cX);      // Gui 4 bit thap
    lcd2_enable();
}

void lcd2_init(void)
{
    lcd2_send_4bit_data(0x00); delay_ms(200);
    LCD2_RS = 0; LCD2_RW = 0; LCD2_E = 0;
    
    // Reset LCD
    lcd2_send_4bit_data(0x03); lcd2_enable(); lcd2_enable(); lcd2_enable();
    
    // Chuyen sang che do 4 bit
    lcd2_send_4bit_data(0x02); lcd2_enable();
    
    lcd2_send_command(0x28); // Giao thuc 4 bit, 2 hang, 5x8 (0x28 chuan hon 0x2C)
    lcd2_send_command(0x0C); // Bat hien thi, tat con tro
    lcd2_send_command(0x06); // Tu dong tang con tro
    lcd2_send_command(CLEAR_LCD);
}

void lcd2_gotoxy(unsigned char x, unsigned char y)
{
    unsigned char address;
    if (!y) address = (LINE_1 + x);
    else address = (LINE_2 + x);
    delay_ms(1);
    lcd2_send_command(address);
    delay_ms(1);
}

void lcd2_putchar(unsigned int cX)
{
    LCD2_RS = 1; // Chon thanh ghi du lieu
    lcd2_send_command(cX);
    LCD2_RS = 0;
}

void lcd2_puts(char *s)
{
    while (*s) { lcd2_putchar(*s); s++; }
}

// ============================================================
//                        CHUONG TRÌNH CHÍNH
// ============================================================
void main() {
    // Khoi tao ca 2 man hinh
    lcd1_init();
    lcd2_init();
    
    while(1) {
        // --- HIEN THI TREN LCD 1 ---
        lcd1_gotoxy(0,0);
        lcd1_puts("DAI HOC SPKT");
        lcd1_gotoxy(0,1);
        lcd1_puts("CHAO CAC BAN");

        // --- HIEN THI TREN LCD 2 ---
        lcd2_gotoxy(0,0);
        lcd2_puts("LCD SO 2");
        lcd2_gotoxy(0,1);
        lcd2_puts("DANG HOAT DONG");

        delay_ms(1000); // Cho 1 giay

        // --- THAY DOI NOI DUNG ---
        
        // Thay doi LCD 1
        lcd1_send_command(CLEAR_LCD); // Xoa LCD 1 truoc khi ghi moi
        lcd1_gotoxy(0,0);
        lcd1_puts("PLT SOLUTIONS");
        lcd1_gotoxy(0,1);
        lcd1_puts("HE THONG NHUNG");

        // Thay doi LCD 2
        lcd2_send_command(CLEAR_LCD); // Xoa LCD 2 truoc khi ghi moi
        lcd2_gotoxy(0,0);
        lcd2_puts("KET NOI THANH CONG");
        lcd2_gotoxy(0,1);
        lcd2_puts("NGON!");
        
        delay_ms(1000);
        
        // Xoa ca 2 man hinh de vong lap sau viet lai tu dau cho sach
        lcd1_send_command(CLEAR_LCD);
        lcd2_send_command(CLEAR_LCD);
    }
}

Bai 7 file.c co FAN

#include <reg51.h>

sbit FAN = P2^0;

// Button
sbit K1 = P1^0;
sbit K2 = P1^1;
sbit K3 = P1^2;
sbit K4 = P1^3;
sbit K5 = P1^4;
sbit K6 = P1^5;
sbit K7 = P1^6;
sbit K8 = P1^7;

char password[] = {1,2,3,4,5,6};
char input[6];
unsigned char index = 0;

// UART g?i 1 ký t?
void send_char(char c) {
    SBUF = c;
    while(TI == 0);
    TI = 0;
}

// g?i chu?i
void send_string(char *s) {
    while(*s) {
        send_char(*s++);
    }
}

// delay
void delay_ms(int t) {
    int i,j;
    for(i=0;i<t;i++)
        for(j=0;j<123;j++);
}

// d?c phím
char read_key() {
    if(K1==0){ while(K1==0); delay_ms(20); return 1; }
    if(K2==0){ while(K2==0); delay_ms(20); return 2; }
    if(K3==0){ while(K3==0); delay_ms(20); return 3; }
    if(K4==0){ while(K4==0); delay_ms(20); return 4; }
    if(K5==0){ while(K5==0); delay_ms(20); return 5; }
    if(K6==0){ while(K6==0); delay_ms(20); return 6; }
    if(K7==0){ while(K7==0); delay_ms(20); return 7; }
    if(K8==0){ while(K8==0); delay_ms(20); return 8; }
    return 0;
}

// ki?m tra pass
bit check_pass() {
    unsigned char i;
    for(i=0;i<6;i++) {
        if(input[i] != password[i])
            return 0;
    }
    return 1;
}

// reset màn hình
void show_welcome() {
    send_string("\r\n====================\r\n");
    send_string("  HE THONG KHOA \r\n");
    send_string("====================\r\n");
    send_string("Nhap mat khau: ");
}

void main() {
    // UART init
    TMOD = 0x20;
    TH1  = -3;
    SCON = 0x50;
    TR1  = 1;

    FAN = 0;

    show_welcome();

    while(1) {
        char key = read_key();

        if(key != 0) {
            input[index++] = key;

            send_char('*'); // ?n ký t? nh?p

            if(index == 6) {
                if(check_pass()) {
                    send_string("\r\n-> DUNG MAT KHAU!");
                    send_string("\r\nMO CUA - BAT QUAT\r\n");
                    FAN = 1;
                } else {
                    send_string("\r\n-> SAI MAT KHAU!\r\n");
                    FAN = 0;
                }

                delay_ms(1000);

                index = 0;
                show_welcome(); // reset UI
            }
        }
    }
}

Bai 7 file .c khong FAN

#include <reg52.h>

// Khai bao mang cac ky tu can truyen
char code chuoi[] = {'0','1','2','3','4','5','6','7'};

void TRUYEN(char c) {
    SBUF = c;
    while (TI == 0);
    TI = 0;
}

void main() {
    unsigned char i;
    TMOD = 0x20;
    TH1 = 0xFD;
    SCON = 0x50;
    TR1 = 1;

    while(1) {
        // Quet qua 8 chan cua Port 1
        for(i = 0; i < 8; i++) {
            // Kiem tra tung bit cua P1
            if (((P1 >> i) & 0x01) == 0) { 
                while(((P1 >> i) & 0x01) == 0); // Cho nha nut
                TRUYEN(chuoi[i]);
            }
        }
    }
}

Bai7 file .asm

ORG 00H
MAIN:
    MOV TMOD,#20H    ; CHON TIMER 1 + CHE DO 2 - CHE DO 8 BIT TU LAP LAI
    MOV TH1,#-3      ; toc do baud 9600
    MOV SCON,#50H    ; CHON MODE 1 8 BIT CUA UART // 50h=0101 0000b
    SETB TR1
lap:
    JNB P1.0,KTRA1
    JNB P1.1,KTRA2
    JNB P1.2,KTRA3
    JNB P1.3,KTRA4
    JNB P1.4,KTRA5
    JMP lap
KTRA1:
    JNB P1.0,$
    MOV A,#'P'
    CALL TRUYEN
    JMP lap
KTRA2:
    JNB P1.1,$
    MOV A,#'L'
    CALL TRUYEN
    JMP lap
KTRA3:
    JNB P1.2,$
    MOV A,#'T'       // a chua ky tu D
    CALL TRUYEN
    JMP lap
KTRA4:
    JNB P1.3,$
    MOV A,#'S'
    CALL TRUYEN
    JMP lap
KTRA5:
    JNB P1.4,$
    MOV A,#'O'
    CALL TRUYEN
    JMP lap
TRUYEN:
    MOV SBUF,A       // a=D
    JNB TI,$
    CLR TI
    RET
END

BAI 9: 
#include <reg51.h>

sbit b1 = P1^0;
sbit b2 = P1^1;
sbit b3 = P1^2;
sbit b4 = P1^3;
sbit b5 = P1^4;
sbit b6 = P1^5;
sbit b7 = P1^6;

sbit LCD_D4 = P0^4;
sbit LCD_D5 = P0^5;
sbit LCD_D6 = P0^6;
sbit LCD_D7 = P0^7;
sbit LCD_RS = P2^5;
sbit LCD_RW = P2^6;
sbit LCD_E  = P2^7;

#define LINE_1 0x80
#define LINE_2 0xC0
#define CLEAR_LCD 0x01

sbit LED_G = P2^0;
sbit FAN   = P3^6;
// MOTOR qua L293D: IN1=P3.7, IN2=P0.0
sbit MOTOR_IN1 = P3^7;  // L293D IN1 (pin 17)
sbit MOTOR_IN2 = P0^0;  // L293D IN2 (can pull-up 4.7k)
sbit BTN_OPEN  = P0^1;  // nut mo may lanh (can pull-up 4.7k)
sbit BTN_CLOSE = P0^2;  // nut dong may lanh (can pull-up 4.7k)

sbit SEG_A = P3^2;
sbit SEG_B = P3^3;
sbit SEG_C = P3^4;
sbit SEG_D = P3^5;
sbit SEG_E = P2^1;
sbit SEG_F = P2^2;
sbit SEG_G = P2^3;
sbit DIG1  = P2^4;
sbit DIG2  = P1^7;

unsigned char code seg_code[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned char temperature = 30;

char password[5] = "1243";
char input[5];
unsigned char index = 0;
bit system_unlocked = 0;

char new_pass[5];
char confirm_pass[5];
unsigned char new_pass_index = 0;
bit changing_password = 0;
bit confirm_phase = 0;

void UART_Init(void);
void UART_Send(char dulieu);
unsigned char compare_string(char *str1, char *str2);
void delay(int x);
void LCD1602_enable(void);
void LCD1602_send_4bit_data(unsigned char cX);
void LCD1602_send_command(unsigned char cX);
void LCD1602_init(void);
void LCD1602_gotOxy(unsigned char x, unsigned char y);
void LCD1602_clear(void);
void LCD1602_putchar(unsigned char cX);
void LCD1602_puts(char *s);
void displayLCDWelcome(void);
void displayLCDWrongPassword(void);
void checkPasswordFromButtons(void);
void controlFanMotor(void);
void controlLED(void);
void stopMotor(void);
void openAC(void);
void closeAC(void);
void blinkLED5Times(void);
void changePasswordProcess(void);
void handleButtonInChangePass(char digit);
void display_seg(unsigned char num);
void display_temperature_7seg(void);

void delay(int x) {
    int i, j;
    for (i = 0; i < x; i++){
        for (j = 0; j < 125; j++);
    }
}

void UART_Init(void) {
    TMOD = 0x20;
    TH1 = 0xFD;
    SCON = 0x50;
    TR1 = 1;
}

void UART_Send(char dulieu) {
    SBUF = dulieu;
    while(TI == 0);
    TI = 0;
}

unsigned char compare_string(char *str1, char *str2) {
    unsigned char i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    if(str1[i] == '\0' && str2[i] == '\0') {
        return 1;
    }
    return 0;
}

void blinkLED5Times(void) {
    unsigned char i;
    for(i = 0; i < 10; i++) {
        LED_G = !LED_G;
        delay(100);
    }
    LED_G = 0;
}

void display_seg(unsigned char num) {
    unsigned char pattern = ~seg_code[num];
    SEG_A = pattern & 0x01;
    SEG_B = (pattern >> 1) & 0x01;
    SEG_C = (pattern >> 2) & 0x01;
    SEG_D = (pattern >> 3) & 0x01;
    SEG_E = (pattern >> 4) & 0x01;
    SEG_F = (pattern >> 5) & 0x01;
    SEG_G = (pattern >> 6) & 0x01;
}

void display_temperature_7seg(void) {
    unsigned char tens = temperature / 10;
    unsigned char units = temperature % 10;
    DIG2 = 0;
    DIG1 = 1;
    display_seg(tens);
    delay(1);
    DIG1 = 0;
    DIG2 = 1;
    display_seg(units);
    delay(1);
    DIG2 = 0;
}

void controlLED(void) {
    if(system_unlocked) {
        LED_G = 1;
    } else {
        LED_G = 0;
    }
}

void stopMotor(void) { MOTOR_IN1 = 0; MOTOR_IN2 = 0; }

void openAC(void) {
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("MO MAY LANH...");
    MOTOR_IN1 = 1; MOTOR_IN2 = 0;  // quay phai
    delay(15000);
    stopMotor();
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("MAY LANH: MO");
    delay(1000);
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("NHAP MAT KHAU:");
    LCD1602_gotOxy(0, 1);
    LCD1602_puts("____");
}

void closeAC(void) {
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("DONG MAY LANH..");
    MOTOR_IN1 = 0; MOTOR_IN2 = 1;  // quay trai
    delay(5000);
    stopMotor();
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("MAY LANH: DONG");
    delay(1000);
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("NHAP MAT KHAU:");
    LCD1602_gotOxy(0, 1);
    LCD1602_puts("____");
}

void controlFanMotor(void) {
    if(temperature >= 35) {
        FAN = 1;
    } else {
        FAN = 0;
    }
}

void LCD1602_enable(void){
    LCD_E = 1;
    delay(3);
    LCD_E = 0;
    delay(5);
}

void LCD1602_send_4bit_data(unsigned char cX){
    LCD_D4 = cX & 0x01;
    LCD_D5 = (cX >> 1) & 1;
    LCD_D6 = (cX >> 2) & 1;
    LCD_D7 = (cX >> 3) & 1;
}

void LCD1602_send_command(unsigned char cX){
    LCD1602_send_4bit_data(cX >> 4);
    LCD1602_enable();
    LCD1602_send_4bit_data(cX);
    LCD1602_enable();
}

void LCD1602_init(void){
    LCD1602_send_4bit_data(0x00);
    delay(200);
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_E = 0;
    LCD1602_send_4bit_data(0x03);
    LCD1602_enable();
    LCD1602_enable();
    LCD1602_enable();
    LCD1602_send_4bit_data(0x02);
    LCD1602_enable();
    LCD1602_send_command(0x28);
    LCD1602_send_command(0x80);
    LCD1602_send_command(0x0C);
    LCD1602_send_command(0x06);
    LCD1602_send_command(CLEAR_LCD);
}

void LCD1602_gotOxy(unsigned char x, unsigned char y){
    unsigned char address;
    if(!y) address = (LINE_1 + x);
    else  address = (LINE_2 + x);
    delay(3);
    LCD1602_send_command(address);
    delay(1);
}

void LCD1602_clear(void) {
    LCD1602_send_command(CLEAR_LCD);
}

void LCD1602_putchar(unsigned char cX) {
    LCD_RS = 1;
    LCD1602_send_command(cX);
    LCD_RS = 0;
}

void LCD1602_puts(char *s) {
    while(*s) {
        LCD1602_putchar(*s);
        s++;
    }
}

void displayLCDWelcome(void) {
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("CHAO MUNG BAN!");
}

void displayLCDWrongPassword(void) {
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("SAI MAT KHAU!");
    delay(1000);
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("NHAP MAT KHAU:");
    LCD1602_gotOxy(0, 1);
    LCD1602_puts("____");
}

void checkPasswordFromButtons(void) {
    if(index == 4) {
        input[4] = '\0';
        if(compare_string(input, password)) {
            UART_Send('\r');
            UART_Send('\n');
            UART_Send('O');
            UART_Send('K');
            UART_Send('\r');
            UART_Send('\n');
            system_unlocked = 1;
            displayLCDWelcome();
        } else {
            UART_Send('\r');
            UART_Send('\n');
            UART_Send('E');
            UART_Send('R');
            UART_Send('R');
            UART_Send('\r');
            UART_Send('\n');
            UART_Send('\r');
            UART_Send('\n');
            system_unlocked = 0;
            blinkLED5Times();
            displayLCDWrongPassword();
        }
        index = 0;
    }
}

void handleButtonInChangePass(char digit) {
    if(!confirm_phase) {
        if(new_pass_index < 4) {
            new_pass[new_pass_index] = digit;
            LCD1602_gotOxy(new_pass_index, 1);
            LCD1602_putchar('*');
            new_pass_index++;
        }
        if(new_pass_index == 4) {
            new_pass[4] = '\0';
            delay(500);
            confirm_phase = 1;
            new_pass_index = 0;
            LCD1602_clear();
            LCD1602_gotOxy(0, 0);
            LCD1602_puts("XAC NHAN PASS:");
            LCD1602_gotOxy(0, 1);
            LCD1602_puts("____");
        }
    } else {
        if(new_pass_index < 4) {
            confirm_pass[new_pass_index] = digit;
            LCD1602_gotOxy(new_pass_index, 1);
            LCD1602_putchar('*');
            new_pass_index++;
        }
        if(new_pass_index == 4) {
            confirm_pass[4] = '\0';
            if(compare_string(new_pass, confirm_pass)) {
                password[0] = new_pass[0];
                password[1] = new_pass[1];
                password[2] = new_pass[2];
                password[3] = new_pass[3];
                password[4] = '\0';
                LCD1602_clear();
                LCD1602_gotOxy(0, 0);
                LCD1602_puts("DOI PASS OK!");
                delay(1500);
            } else {
                LCD1602_clear();
                LCD1602_gotOxy(0, 0);
                LCD1602_puts("PASS KHONG");
                LCD1602_gotOxy(0, 1);
                LCD1602_puts("KHOP! THU LAI");
                delay(1500);
            }
            changing_password = 0;
            confirm_phase = 0;
            new_pass_index = 0;
            index = 0;
            LCD1602_clear();
            LCD1602_gotOxy(0, 0);
            LCD1602_puts("NHAP MAT KHAU:");
            LCD1602_gotOxy(0, 1);
            LCD1602_puts("____");
        }
    }
}

void changePasswordProcess(void) {
    unsigned int hold_count = 0;
    unsigned char second = 0;
    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("GIU NUT 5...");
    while(b5 == 0 && hold_count < 300) {
        delay(1);
        hold_count++;
        if(hold_count == 100) {
            second = 1;
            LCD1602_gotOxy(0, 1);
            LCD1602_puts("1...");
        }
        if(hold_count == 200) {
            second = 2;
            LCD1602_gotOxy(0, 1);
            LCD1602_puts("2...");
        }
        if(hold_count == 300) {
            second = 3;
            LCD1602_gotOxy(0, 1);
            LCD1602_puts("3...");
        }
    }
    while(b5 == 0);
    if(second >= 3) {
        changing_password = 1;
        confirm_phase = 0;
        new_pass_index = 0;
        delay(500);
        LCD1602_clear();
        LCD1602_gotOxy(0, 0);
        LCD1602_puts("NHAP PASS MOI:");
        LCD1602_gotOxy(0, 1);
        LCD1602_puts("____");
    } else {
        LCD1602_clear();
        LCD1602_gotOxy(0, 0);
        LCD1602_puts("NHAP MAT KHAU:");
        LCD1602_gotOxy(0, 1);
        LCD1602_puts("____");
        {
            unsigned char k;
            for(k = 0; k < index; k++) {
                LCD1602_gotOxy(k, 1);
                LCD1602_putchar('*');
            }
        }
    }
}

void main(void) {
    UART_Init();
    LCD1602_init();
    FAN = 0;
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    LED_G = 0;
    DIG1 = 0;
    DIG2 = 0;
    BTN_OPEN = 1;   // cho phep doc (Port 0 open-drain)
    BTN_CLOSE = 1;
    MOTOR_IN2 = 0;  // P0.0 output init
    system_unlocked = 0;

    LCD1602_clear();
    LCD1602_gotOxy(0, 0);
    LCD1602_puts("NHAP MAT KHAU:");
    LCD1602_gotOxy(0, 1);
    LCD1602_puts("____");

    while(1) {
        if(changing_password) {
            if(b1 == 0) { delay(10); if(b1 == 0) { while(b1 == 0); handleButtonInChangePass('1'); } }
            if(b2 == 0) { delay(10); if(b2 == 0) { while(b2 == 0); handleButtonInChangePass('2'); } }
            if(b3 == 0) { delay(10); if(b3 == 0) { while(b3 == 0); handleButtonInChangePass('3'); } }
            if(b4 == 0) { delay(10); if(b4 == 0) { while(b4 == 0); handleButtonInChangePass('4'); } }
        } else {
            if(b1 == 0) {
                delay(10);
                if(b1 == 0) {
                    while(b1 == 0);
                    UART_Send('1');
                    if(index < 4) { input[index++] = '1'; LCD1602_gotOxy(index-1, 1); LCD1602_putchar('*'); }
                    checkPasswordFromButtons();
                }
            }
            if(b5 == 0) { delay(10); if(b5 == 0) { changePasswordProcess(); } }
            if(b2 == 0) {
                delay(10);
                if(b2 == 0) {
                    while(b2 == 0);
                    UART_Send('2');
                    if(index < 4) { input[index++] = '2'; LCD1602_gotOxy(index-1, 1); LCD1602_putchar('*'); }
                    checkPasswordFromButtons();
                }
            }
            if(b3 == 0) {
                delay(10);
                if(b3 == 0) {
                    while(b3 == 0);
                    UART_Send('3');
                    if(index < 4) { input[index++] = '3'; LCD1602_gotOxy(index-1, 1); LCD1602_putchar('*'); }
                    checkPasswordFromButtons();
                }
            }
            if(b4 == 0) {
                delay(10);
                if(b4 == 0) {
                    while(b4 == 0);
                    UART_Send('4');
                    if(index < 4) { input[index++] = '4'; LCD1602_gotOxy(index-1, 1); LCD1602_putchar('*'); }
                    checkPasswordFromButtons();
                }
            }
        }

        if(b6 == 0) { delay(10); if(b6 == 0) { while(b6 == 0); if(temperature < 99) temperature++; } }
        if(b7 == 0) { delay(10); if(b7 == 0) { while(b7 == 0); if(temperature > 0) temperature--; } }

        if(BTN_OPEN == 0) { delay(10); if(BTN_OPEN == 0) { while(BTN_OPEN == 0); openAC(); } }
        if(BTN_CLOSE == 0) { delay(10); if(BTN_CLOSE == 0) { while(BTN_CLOSE == 0); closeAC(); } }

        display_temperature_7seg();
        controlFanMotor();
        controlLED();
    }
}


Bai mo rong: 

#include <reg52.h>

sbit BTN_UP    = P1^0; 
sbit BTN_DOWN  = P1^7;
sbit LCD_RS    = P2^6; 
sbit LCD_E     = P2^7; 
sbit LCD_D4    = P3^4;
sbit LCD_D5    = P3^5;
sbit LCD_D6    = P3^6;
sbit LCD_D7    = P3^7;
sbit FAN       = P3^0;
sbit DIG1      = P2^0;
sbit DIG2      = P2^1;

#define SEG_PORT P0

char LED_7_THANH[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

int temp = 30;
bit lcd_need_update = 1;

void delay_ms(int x) {
    int i, j;
    for(i = 0; i < x; i++)
        for(j = 0; j < 125; j++);
}

void delay(unsigned int t) { while(t--); }

// --- QUET LED 7 THANH ---
void hienthi_LED_7_THANH(int x) {
    int chuc, dv;
    if(x > 99) x = 99;
    if(x < 0)  x = 0;

    chuc = x / 10;
    dv   = x % 10;

    // Hien thi hang chuc
    SEG_PORT = LED_7_THANH[chuc];
    DIG1 = 1;
    delay_ms(1); // Giam thoi gian delay de quet nhanh hon
    DIG1 = 0;

    // Hien thi hang don vi
    SEG_PORT = LED_7_THANH[dv];
    DIG2 = 1;
    delay_ms(1);
    DIG2 = 0;
}

// --- DIEU KHIEN LCD (DA LOAI BO LENH TAT LED) ---
void LCD_Enable(void) { 
    LCD_E=1; 
    delay(50); // Giam thoi gian cho de tranh chiem dung CPU qua lau
    LCD_E=0; 
    delay(50); 
}

void LCD_Send4Bit(unsigned char Data) {
    LCD_D4 = (Data >> 0) & 0x01;
    LCD_D5 = (Data >> 1) & 0x01;
    LCD_D6 = (Data >> 2) & 0x01;
    LCD_D7 = (Data >> 3) & 0x01;
}

void LCD_Command(unsigned char cmd) {
    // DA XOA CAC DONG TAT DIG1, DIG2 O DAY
    LCD_RS = 0;
    LCD_Send4Bit(cmd >> 4); LCD_Enable();
    LCD_Send4Bit(cmd);      LCD_Enable();
}

void LCD_Data(unsigned char dat) {
    LCD_RS = 1;
    LCD_Send4Bit(dat >> 4); LCD_Enable();
    LCD_Send4Bit(dat);      LCD_Enable();
}

void LCD_Init() {
    LCD_Send4Bit(0x00); delay(2000);
    LCD_RS = 0;
    LCD_Send4Bit(0x03); LCD_Enable(); delay(500);
    LCD_Send4Bit(0x03); LCD_Enable(); delay(500);
    LCD_Send4Bit(0x03); LCD_Enable();
    LCD_Send4Bit(0x02); LCD_Enable();
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x01);
    delay(2000);
}

void LCD_Puts(char *s) { while(*s) LCD_Data(*s++); }

void Display_LCD() {
    if(!lcd_need_update) return;
    
    LCD_Command(0x80);
    LCD_Puts("NHIET DO: ");
    LCD_Data((temp / 10) + 48);
    LCD_Data((temp % 10) + 48);

    LCD_Command(0xC0);
    if(temp >= 35) {
        LCD_Puts("BAT QUAT       ");
        FAN = 1;
    } else {
        LCD_Puts("TAT QUAT       ");
        FAN = 0;
    }
    lcd_need_update = 0;
}

void main() {
    FAN      = 0;
    SEG_PORT = 0xFF;
    DIG1     = 0;
    DIG2     = 0;

    LCD_Init();
    
    while(1) {
        // Xu ly nut Bam
        if(BTN_UP == 0) {
            if(temp < 99) temp++;
            lcd_need_update = 1;
            while(BTN_UP == 0) hienthi_LED_7_THANH(temp); // LED van sang khi dang giu nut
        }

        if(BTN_DOWN == 0) {
            if(temp > 0) temp--;
            lcd_need_update = 1;
            while(BTN_DOWN == 0) hienthi_LED_7_THANH(temp); // LED van sang khi dang giu nut
        }

        // Luon luon quet LED truoc
        hienthi_LED_7_THANH(temp);
        
        // C?p nh?t LCD (LCD chi update khi can, va khong con lenh tat LED ben trong)
        Display_LCD();
    }
}
