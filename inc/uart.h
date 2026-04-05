/*
 * uart.h  --  UART 9600 8N1 (Timer1 Mode 2)
 *             P3.0 = TXD -> Virtual Terminal trong Proteus
 */

#ifndef UART_H
#define UART_H

/* Khoi tao UART: 9600 baud, 8N1 dung Timer1 */
void UART_Init(void);

/* Gui 1 ky tu */
void UART_SendChar(char c);

/* Gui chuoi ky tu */
void UART_SendStr(char *s);

/* Gui ban ghi log co nhan thoi gian: "[MM:SS] msg\r\n"
   mm = phut hien tai, ss = giay hien tai */
void UART_Log(char *msg, unsigned char mm, unsigned char ss);

#endif /* UART_H */
