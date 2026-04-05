/*
 * uart.c  --  UART 9600 baud, 8N1
 *             Dung Timer1 Mode 2 (8-bit tu lap lai)
 *             P3.0 = TXD  ->  Virtual Terminal trong Proteus
 */

#include "config.h"
#include "uart.h"

/* ----------------------------------------------------------------
 *  UART_Init  --  Khoi tao UART va Timer1
 * ---------------------------------------------------------------- */
void UART_Init(void)
{
    /* Cau hinh Timer1: Mode 2 (8-bit tu lap lai) de tao baud rate */
    TMOD |= 0x20;       /* Dat bit T1M1=1 de chon Mode 2 cho Timer1 */

    /* Nap gia tri lai: 0xFD tuong ung 9600 baud voi thach anh 11.0592 MHz */
    TH1  = T1_BAUD;     /* Thanh ghi nap lai (tu dong copy vao TL1 moi overflow) */
    TL1  = T1_BAUD;     /* Gia tri bat dau dem */

    /* Cau hinh UART: Mode 1 (8-bit UART), cho phep nhan (REN=1) */
    SCON = 0x50;        /* SCON = 0101 0000: SM0=0, SM1=1, REN=1 */

    TR1  = 1;           /* Bat Timer1 chay */
}

/* ----------------------------------------------------------------
 *  UART_SendChar  --  Gui 1 ky tu qua UART
 * ---------------------------------------------------------------- */
void UART_SendChar(char c)
{
    SBUF = c;           /* Nap ky tu vao thanh ghi dem phat */
    while (!TI);        /* Cho den khi TI=1 (da gui xong ky tu) */
    TI   = 0;           /* Xoa co TI de chuan bi gui ky tu tiep theo */
}

/* ----------------------------------------------------------------
 *  UART_SendStr  --  Gui chuoi ky tu
 * ---------------------------------------------------------------- */
void UART_SendStr(char *s)
{
    while (*s)
        UART_SendChar(*s++);    /* Gui tung ky tu den khi gap '\0' */
}

/* ----------------------------------------------------------------
 *  Ham noi bo: in so 2 chu so (bo sung so 0 phia truoc)
 * ---------------------------------------------------------------- */
static void uart_print2(unsigned char n)
{
    UART_SendChar('0' + n / 10);    /* Chu so hang chuc */
    UART_SendChar('0' + n % 10);    /* Chu so hang don vi */
}

/* ----------------------------------------------------------------
 *  UART_Log  --  Gui ban ghi log co nhan thoi gian
 *  Dinh dang xuat: "[MM:SS] msg\r\n"
 *  mm = phut hien tai, ss = giay hien tai
 * ---------------------------------------------------------------- */
void UART_Log(char *msg, unsigned char mm, unsigned char ss)
{
    UART_SendChar('[');
    uart_print2(mm);        /* In phut: 2 chu so */
    UART_SendChar(':');
    uart_print2(ss);        /* In giay: 2 chu so */
    UART_SendChar(']');
    UART_SendChar(' ');
    UART_SendStr(msg);      /* In noi dung thong bao */
    UART_SendStr("\r\n");   /* Xuong dong */
}
