/*
 * lcd.c  --  Driver LCD 16x2, giao tiep 4-bit
 *
 * So do chan:
 *   D4=P3.4  D5=P3.5  D6=P3.6  D7=P3.7
 *   RS=P2.6  E=P2.7   RW noi dat GND (luon o che do ghi)
 */

#include "config.h"
#include "lcd.h"

/* ----------------------------------------------------------------
 *  Ham noi bo: delay don gian
 * ---------------------------------------------------------------- */
static void lcd_delay_ms(unsigned int ms)
{
    unsigned int i, j;
    /* Vong lap long: moi ms ~ 125 lan lap vong trong voi 11.0592 MHz */
    for (i = 0; i < ms; i++)
        for (j = 0; j < 125; j++);
}

static void lcd_delay_us(unsigned char us)
{
    unsigned char i, j;
    /* Tre ngan: moi us ~ 2 lan lap */
    for (i = 0; i < us; i++)
        for (j = 0; j < 2; j++);
}

/* ----------------------------------------------------------------
 *  Ham noi bo: gui 4 bit len 4 chan data (nibble thap cua tham so)
 * ---------------------------------------------------------------- */
static void lcd_send4(unsigned char nibble)
{
    /* Gan tung bit vao chan tuong ung */
    LCD_D4 = (nibble >> 0) & 1;
    LCD_D5 = (nibble >> 1) & 1;
    LCD_D6 = (nibble >> 2) & 1;
    LCD_D7 = (nibble >> 3) & 1;
}

/* ----------------------------------------------------------------
 *  Ham noi bo: tao xung Enable de LCD chot du lieu
 * ---------------------------------------------------------------- */
static void lcd_pulse_e(void)
{
    LCD_E = 1;          /* Keo E len cao */
    lcd_delay_us(2);    /* Cho LCD kip nhan */
    LCD_E = 0;          /* Ha E xuong -> LCD chot du lieu */
    lcd_delay_us(5);    /* Cho LCD xu ly */
}

/* ----------------------------------------------------------------
 *  Ham noi bo: gui 1 byte (lenh hoac du lieu) theo giao thuc 4-bit
 *  rs = 0: lenh, rs = 1: du lieu
 * ---------------------------------------------------------------- */
static void lcd_write_byte(unsigned char val, bit rs)
{
    LCD_RS = rs;            /* Chon thanh ghi lenh hoac du lieu */

    /* Gui nibble cao truoc (bit 7..4) */
    lcd_send4(val >> 4);
    lcd_pulse_e();

    /* Gui nibble thap sau (bit 3..0) */
    lcd_send4(val & 0x0F);
    lcd_pulse_e();
}

/* ----------------------------------------------------------------
 *  LCD_Init  --  Khoi dong LCD theo trinh tu chuan HD44780
 * ---------------------------------------------------------------- */
void LCD_Init(void)
{
    /* Dat tat ca chan dieu khien ve 0 truoc */
    LCD_RS = 0;
    LCD_E  = 0;

    /* Cho nguon on dinh it nhat 15 ms sau khi cap dien */
    lcd_delay_ms(20);

    /* Buoc 1: gui 0x03 3 lan de reset LCD ve che do 8-bit */
    lcd_send4(0x03); lcd_pulse_e(); lcd_delay_ms(5);
    lcd_send4(0x03); lcd_pulse_e(); lcd_delay_ms(1);
    lcd_send4(0x03); lcd_pulse_e(); lcd_delay_ms(1);

    /* Buoc 2: chuyen sang che do 4-bit */
    lcd_send4(0x02); lcd_pulse_e(); lcd_delay_ms(1);

    /* Buoc 3: cau hinh LCD: 4-bit, 2 dong, font 5x8 */
    lcd_write_byte(0x28, 0); lcd_delay_ms(1);

    /* Buoc 4: bat hien thi, tat con tro, tat nhap nhay */
    lcd_write_byte(0x0C, 0); lcd_delay_ms(1);

    /* Buoc 5: xoa man hinh */
    lcd_write_byte(0x01, 0); lcd_delay_ms(2);

    /* Buoc 6: che do nhap: tang con tro, khong dich man hinh */
    lcd_write_byte(0x06, 0); lcd_delay_ms(1);
}

/* ----------------------------------------------------------------
 *  LCD_Clear  --  Xoa toan bo man hinh, dua con tro ve (0,0)
 * ---------------------------------------------------------------- */
void LCD_Clear(void)
{
    lcd_write_byte(0x01, 0);    /* Lenh xoa man hinh (Clear Display) */
    lcd_delay_ms(2);             /* Lenh nay can 1.52 ms de hoan thanh */
}

/* ----------------------------------------------------------------
 *  LCD_GotoXY  --  Di chuyen con tro den vi tri (col, row)
 *  col: 0-15 (cot), row: 0 (dong tren) hoac 1 (dong duoi)
 * ---------------------------------------------------------------- */
void LCD_GotoXY(unsigned char col, unsigned char row)
{
    /* Dia chi DDRAM: dong 0 bat dau 0x80, dong 1 bat dau 0xC0 */
    unsigned char addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_write_byte(addr, 0);    /* Gui lenh Set DDRAM Address */
    lcd_delay_ms(1);
}

/* ----------------------------------------------------------------
 *  LCD_PutChar  --  Hien thi 1 ky tu ASCII
 * ---------------------------------------------------------------- */
void LCD_PutChar(unsigned char c)
{
    lcd_write_byte(c, 1);   /* rs=1: ghi vao DDRAM (hien thi ky tu) */
}

/* ----------------------------------------------------------------
 *  LCD_PutStr  --  Hien thi chuoi ky tu ket thuc bang '\0'
 * ---------------------------------------------------------------- */
void LCD_PutStr(char *s)
{
    while (*s)
        lcd_write_byte((unsigned char)*s++, 1);
}

/* ----------------------------------------------------------------
 *  LCD_PutNum  --  Hien thi so nguyen khong dau, bo sung so 0
 *  n      : gia tri can hien thi
 *  digits : so chu so muon in (vi du digits=2: in "05" thay vi "5")
 * ---------------------------------------------------------------- */
void LCD_PutNum(unsigned int n, unsigned char digits)
{
    unsigned int div = 1;
    unsigned char i;
    /* Tinh luy thua 10^(digits-1) de lay tung chu so tu trai sang */
    for (i = 1; i < digits; i++) div *= 10;
    for (i = 0; i < digits; i++) {
        lcd_write_byte('0' + (n / div) % 10, 1);
        div /= 10;
    }
}

/* ----------------------------------------------------------------
 *  LCD_LoadingBar  --  Hieu ung thanh tien trinh (loading bar)
 *  row      : dong hien thi (0 hoac 1)
 *  len      : so o can lam day (1-16)
 *  delay_ms : thoi gian cho giua moi o (ms)
 * ---------------------------------------------------------------- */
void LCD_LoadingBar(unsigned char row, unsigned char len, unsigned int step_ms)
{
    unsigned char i;
    LCD_GotoXY(0, row);
    for (i = 0; i < len; i++) {
        lcd_write_byte(0xFF, 1);    /* 0xFF = ky tu khoi dac (full block) */
        lcd_delay_ms(step_ms);       /* Dung lai de tao hieu ung tung buoc */
    }
}
