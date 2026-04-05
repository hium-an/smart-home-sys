/*
 * lcd.h  --  Driver LCD 16x2 (giao tiep 4-bit)
 *            Chan data: P3.4-P3.7 (D4-D7)
 *            Chan dieu khien: P2.6 (RS), P2.7 (E), RW noi dat GND
 */

#ifndef LCD_H
#define LCD_H

/* Khoi tao LCD (goi 1 lan khi bat dau) */
void LCD_Init(void);

/* Xoa man hinh, dua con tro ve goc trai dong 1 */
void LCD_Clear(void);

/* Di chuyen con tro: col=0..15, row=0..1 */
void LCD_GotoXY(unsigned char col, unsigned char row);

/* In 1 ky tu ASCII */
void LCD_PutChar(unsigned char c);

/* In chuoi ky tu (ket thuc bang '\0') */
void LCD_PutStr(char *s);

/* In so nguyen khong dau, digits=so chu so (bo tro 0) */
/* Vi du: LCD_PutNum(5, 2) in "05" */
void LCD_PutNum(unsigned int n, unsigned char digits);

/* Hieu ung loading bar: in 'len' khoi tren dong 'row', moi khoi delay_ms ms */
void LCD_LoadingBar(unsigned char row, unsigned char len, unsigned int delay_ms);

#endif /* LCD_H */
