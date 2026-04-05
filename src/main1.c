/*
 * main1.c  --  MCU1: Security (LCD + Mat khau + GPIO tin hieu)
 *
 * Chuc nang:
 *   F1. Man hinh chao + loading bar
 *   F2. Dong ho MM:SS tren LCD (Timer0 ISR 1ms)
 *   F3. Dang nhap mat khau 4 so (BTN1-4)
 *   F3.1 Doi mat khau (giu BTN1 3 giay)
 *   F3.2 Lockdown sau 3 lan sai -> SIG_ALARM=1
 *   Khi dang nhap thanh cong -> SIG_UNLOCK=1 -> MCU2 mo cua
 *
 * Keil: Project -> Options -> C51 -> Define: PROJ_MCU1
 * Files: main1.c  lcd.c  security.c
 */

#include "config.h"
#include "lcd.h"
#include "security.h"

/* ================================================================
 *  BIEN TOAN CUC
 * ================================================================ */
volatile unsigned int  g_ms     = 0;
volatile unsigned char g_sec    = 0;
volatile unsigned char g_min    = 0;
volatile bit           g_tick_1s = 0;
bit                    g_unlocked = 0;
bit                    g_door_state = 0;   /* 0=dong, 1=mo (theo doi trang thai cua) */

/* ================================================================
 *  ISR TIMER0  --  Ngat 1ms, dem dong ho MM:SS
 * ================================================================ */
void Timer0_ISR(void) interrupt 1
{
    TH0 = T0_HIGH;
    TL0 = T0_LOW;

    g_ms++;
    if (g_ms >= 1000) {
        g_ms = 0;
        g_sec++;
        g_tick_1s = 1;
        if (g_sec >= 60) {
            g_sec = 0;
            g_min++;
            if (g_min >= 60) g_min = 0;
        }
    }
}

/* ================================================================
 *  HAM NOI BO
 * ================================================================ */
static void Timer0_Init(void)
{
    TMOD |= 0x01;
    TH0   = T0_HIGH;
    TL0   = T0_LOW;
    ET0   = 1;
    TR0   = 1;
}

static void Welcome(void)
{
    LCD_GotoXY(0, 0); LCD_PutStr("SMART HOME SYS");
    LCD_GotoXY(0, 1); LCD_PutStr("  Group One   ");
    LCD_LoadingBar(1, 16, 100);
    LCD_Clear();
}

static void Display_Clock(void)
{
    LCD_GotoXY(10, 0);
    LCD_PutNum(g_min, 2);
    LCD_PutChar(':');
    LCD_PutNum(g_sec, 2);
}

/* Delay ~1ms noi bo */
static void hold_delay1ms(void)
{
    unsigned char i, j;
    for (i = 0; i < 1; i++)
        for (j = 0; j < 125; j++);
}

/* Kiem tra giu BTN_CHPASS (P2.2) 3 giay, hien dem 1->2->3 tren LCD */
static bit Check_Hold_ChPass(void)
{
    unsigned int count = 0;
    unsigned char sec_shown = 0;

    if (BTN_CHPASS != 0) return 0;

    /* Hien thi man hinh dem */
    LCD_Clear();
    LCD_GotoXY(0, 0); LCD_PutStr("GIU DE DOI PASS");
    LCD_GotoXY(0, 1); LCD_PutStr("Dem: ");

    while (BTN_CHPASS == 0) {
        hold_delay1ms();
        count++;

        /* Moi 1000ms hien thi so tiep theo */
        if (count == 1000 && sec_shown < 1) {
            sec_shown = 1;
            LCD_GotoXY(5, 1); LCD_PutStr("1 ");
        } else if (count == 2000 && sec_shown < 2) {
            sec_shown = 2;
            LCD_GotoXY(5, 1); LCD_PutStr("1 2 ");
        } else if (count == 3000 && sec_shown < 3) {
            sec_shown = 3;
            LCD_GotoXY(5, 1); LCD_PutStr("1 2 3");
        }

        if (count >= HOLD_CHANGE_MS) {
            while (BTN_CHPASS == 0);  /* Cho tha nut */
            return 1;
        }
    }

    /* Tha som -> huy, xoa man hinh dem */
    LCD_Clear();
    return 0;
}

/* ================================================================
 *  MAIN
 * ================================================================ */
void main(void)
{
    Timer0_Init();
    LCD_Init();
    Security_Init();

    SIG_UNLOCK = 0;   /* Khoi tao tin hieu GPIO ve 0 */
    SIG_ALARM  = 0;
    SIG_WRONG  = 0;
    SIG_JAM    = 0;
    EA         = 1;

    Welcome();

    while (1) {
        /* Kiem tra giu BTN_CHPASS (P2.2) 3s doi mat khau */
        if (Check_Hold_ChPass()) {
            Security_ChangePass();
        }

        /* Man hinh dang nhap */
        if (!g_unlocked) {
            SIG_WRONG = 0;      /* Reset tin hieu sai truoc khi nhap */
            if (Security_Login()) {
                g_unlocked = 1;
                g_door_state = 1;
                SIG_UNLOCK = 1;   /* Bao MCU2 mo cua */
                LCD_Clear();
                LCD_GotoXY(0, 0); LCD_PutStr("CHAO MUNG!");
                LCD_GotoXY(0, 1); LCD_PutStr("CUA DA MO");
            } else {
                LCD_Clear();
                LCD_GotoXY(0, 0); LCD_PutStr("SAI MAT KHAU!");
                LCD_GotoXY(0, 1); LCD_PutStr("Con lai: ");
                LCD_PutNum(MAX_FAIL - g_fail_count, 1);

                /* Pulse SIG_WRONG de MCU2 bip bao sai */
                SIG_WRONG = 1;

                if (g_fail_count >= MAX_FAIL) {
                    SIG_ALARM = 1;   /* Bao MCU2 bat bao dong */
                    Security_Lockdown();
                }
            }
        }

        /* Kiem tra switch ket cua (active-low: 0 = ket) */
        if (g_unlocked && DOOR_JAM_SW == 0) {
            SIG_JAM = 1;       /* Bao MCU2 */
            LCD_Clear();
            LCD_GotoXY(0, 0); LCD_PutStr("!! KET CUA !!");
            LCD_GotoXY(0, 1); LCD_PutStr("KIEM TRA CUA");
            while (DOOR_JAM_SW == 0);   /* Cho den khi het ket */
            SIG_JAM = 0;
            LCD_Clear();
            LCD_GotoXY(0, 0); LCD_PutStr("HET KET CUA");
            if (g_door_state) {
                LCD_GotoXY(0, 1); LCD_PutStr("CUA DANG MO...");
            } else {
                LCD_GotoXY(0, 1); LCD_PutStr("CUA DANG DONG..");
            }
        }

        /* Cap nhat dong ho moi giay */
        if (g_unlocked && g_tick_1s) {
            g_tick_1s = 0;
            Display_Clock();
        }
    }
}
