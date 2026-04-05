/*
 * security.c  --  Dang nhap mat khau, khoa he thong, doi mat khau
 *
 * Mat khau 4 chu so (so tu 1-4 tuong ung voi 4 nut bam):
 *   BTN1 = P1.0 -> chu so 1
 *   BTN2 = P1.1 -> chu so 2
 *   BTN3 = P1.2 -> chu so 3
 *   BTN4 = P1.3 -> chu so 4
 *
 * Mat khau mac dinh: {1, 2, 3, 4}
 * Mat khau luu trong RAM (bien static) - reset khi mat dien.
 * Sai MAX_FAIL lan lien tuc -> goi Security_Lockdown().
 *
 * Luu y: Module nay chi dung LCD de hien thi.
 *        Am thanh + UART do MCU2 xu ly (nhan tin hieu qua GPIO).
 */

#include "config.h"
#include "lcd.h"
#include "security.h"

/* Mat khau hien tai (luu trong RAM) */
static unsigned char s_password[PASS_LEN] = {1, 2, 3, 4};

/* So lan nhap sai lien tuc */
unsigned char g_fail_count = 0;

/* ----------------------------------------------------------------
 *  Ham noi bo: delay ms
 * ---------------------------------------------------------------- */
static void sec_delay(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 125; j++);
}

/* ----------------------------------------------------------------
 *  Ham noi bo: cho nut bam va tra ve ma so tuong ung (1-4)
 *  Tra ve 0 neu khong co nut nao duoc nhan
 * ---------------------------------------------------------------- */
static unsigned char read_key(void)
{
    if (BTN1 == 0) { sec_delay(20); if (BTN1 == 0) { while (BTN1 == 0); return 1; } }
    if (BTN2 == 0) { sec_delay(20); if (BTN2 == 0) { while (BTN2 == 0); return 2; } }
    if (BTN3 == 0) { sec_delay(20); if (BTN3 == 0) { while (BTN3 == 0); return 3; } }
    if (BTN4 == 0) { sec_delay(20); if (BTN4 == 0) { while (BTN4 == 0); return 4; } }
    return 0;
}

/* ----------------------------------------------------------------
 *  Ham noi bo: cho nhap du PASS_LEN chu so, hien thi '*' tren LCD
 * ---------------------------------------------------------------- */
static void collect_digits(unsigned char *buf)
{
    unsigned char i = 0, key;
    while (i < PASS_LEN) {
        key = read_key();
        if (key != 0) {
            buf[i] = key;
            LCD_PutChar('*');
            i++;
        }
    }
}

/* ----------------------------------------------------------------
 *  Security_Init  --  Khoi tao module bao mat
 * ---------------------------------------------------------------- */
void Security_Init(void)
{
    g_fail_count = 0;
}

/* ----------------------------------------------------------------
 *  Security_Login  --  Hien thi man hinh nhap mat khau, xu ly nhap
 *  Tra ve 1 = dung mat khau, 0 = sai
 * ---------------------------------------------------------------- */
bit Security_Login(void)
{
    unsigned char input[PASS_LEN];
    unsigned char i;
    bit ok = 1;

    LCD_Clear();
    LCD_GotoXY(0, 0); LCD_PutStr("NHAP MAT KHAU:");
    LCD_GotoXY(0, 1);

    collect_digits(input);

    for (i = 0; i < PASS_LEN; i++) {
        if (input[i] != s_password[i]) {
            ok = 0;
            break;
        }
    }

    if (!ok) {
        g_fail_count++;
    } else {
        g_fail_count = 0;
    }

    return ok;
}

/* ----------------------------------------------------------------
 *  Security_Lockdown  --  Khoa he thong sau khi sai qua nhieu lan
 *  Hien thi canh bao tren LCD, loop vo han.
 *  MCU2 se nhan tin hieu SIG_ALARM va phat am thanh bao dong.
 * ---------------------------------------------------------------- */
void Security_Lockdown(void)
{
    LCD_Clear();
    LCD_GotoXY(0, 0); LCD_PutStr("!! BAO DONG !!");
    LCD_GotoXY(0, 1); LCD_PutStr("KHOA HE THONG");

    /* Cho nhan BTN_ALARM_OFF (P1.7) de tat bao dong */
    while (BTN_ALARM_OFF == 1);
    sec_delay(20);   /* Chong doi phim */
    while (BTN_ALARM_OFF == 0);

    /* Tat bao dong, reset trang thai */
    SIG_ALARM = 0;
    g_fail_count = 0;

    LCD_Clear();
    LCD_GotoXY(0, 0); LCD_PutStr("DA TAT BAO DONG");
    sec_delay(1500);
}

/* ----------------------------------------------------------------
 *  Security_ChangePass  --  Doi mat khau moi
 *  Nhap mat khau moi 2 lan, neu khop thi luu lai
 * ---------------------------------------------------------------- */
void Security_ChangePass(void)
{
    unsigned char new_pass[PASS_LEN];
    unsigned char confirm[PASS_LEN];
    unsigned char i;
    bit match = 1;

    LCD_Clear();
    LCD_GotoXY(0, 0); LCD_PutStr("MAT KHAU MOI:");
    LCD_GotoXY(0, 1);
    collect_digits(new_pass);

    LCD_Clear();
    LCD_GotoXY(0, 0); LCD_PutStr("XAC NHAN PASS:");
    LCD_GotoXY(0, 1);
    collect_digits(confirm);

    for (i = 0; i < PASS_LEN; i++) {
        if (new_pass[i] != confirm[i]) {
            match = 0;
            break;
        }
    }

    if (match) {
        for (i = 0; i < PASS_LEN; i++)
            s_password[i] = new_pass[i];

        LCD_Clear();
        LCD_GotoXY(0, 0); LCD_PutStr("DA DOI PASS!");
    } else {
        LCD_Clear();
        LCD_GotoXY(0, 0); LCD_PutStr("PASS KHONG");
        LCD_GotoXY(0, 1); LCD_PutStr("KHOP! THU LAI");
    }

    sec_delay(2000);
}
