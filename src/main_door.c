/*
 * main_door.c  --  MCU2: Door / Sound / UART
 *
 * Chuc nang:
 *   F4. Mo/dong cua qua Motor + Encoder (INT0)
 *   F8. Log UART 9600 co nhan thoi gian [MM:SS]
 *   F9. Am thanh chao mung, bip, bao dong
 *
 * Nhan tin hieu tu MCU1 qua GPIO:
 *   SIG_UNLOCK_IN  (P1.1) = 1 -> mo cua + bip
 *   SIG_ALARM_IN   (P1.0) = 1 -> bao dong lien tuc
 *   SIG_WRONG_IN   (P1.3) = 1 -> nhap sai mat khau, bip 2 lan
 *   BTN_DOOR_CLOSE (P1.2) = 0 -> dong cua (active-low)
 *   Bao dong tat khi MCU1 ha SIG_ALARM ve 0 (BTN_ALARM_OFF o MCU1 P1.7)
 *
 * Keil: Project -> Options -> C51 -> Define: PROJ_MCU2
 * Files: main_door.c  door.c  sound.c  uart.c
 */

#include "config.h"
#include "door.h"
#include "sound.h"
#include "uart.h"

/* ================================================================
 *  BIEN TOAN CUC
 * ================================================================ */
volatile unsigned int  g_ms  = 0;
volatile unsigned char g_sec = 0;
volatile unsigned char g_min = 0;

/* ================================================================
 *  ISR TIMER0  --  Ngat 1ms, dem dong ho MM:SS (cho UART_Log)
 * ================================================================ */
void Timer0_ISR(void) interrupt 1
{
    TH0 = T0_HIGH;
    TL0 = T0_LOW;

    g_ms++;
    if (g_ms >= 1000) {
        g_ms = 0;
        g_sec++;
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

/* ================================================================
 *  MAIN
 * ================================================================ */
void main(void)
{
    bit prev_unlock = 0;
    bit prev_close  = 0;
    bit prev_wrong  = 0;

    UART_Init();
    Timer0_Init();
    Door_Init();

    SPEAKER   = 0;
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    EA        = 1;

    UART_SendStr("=== MCU2: DOOR/SOUND ===\r\n");
    Sound_Welcome();
    UART_Log("KHOI DONG XONG", g_min, g_sec);

    while (1) {
        /* Kiem tra tin hieu bao dong tu MCU1 (uu tien cao nhat) */
        if (SIG_ALARM_IN == 1) {
            UART_Log("BAO DONG!", g_min, g_sec);
            while (SIG_ALARM_IN == 1) {    /* Keu cho den khi MCU1 tat SIG_ALARM */
                Sound_AlarmTick();
            }
            SPEAKER = 0;
            UART_Log("BAO DONG:TAT", g_min, g_sec);
        }

        /* Kiem tra tin hieu nhap sai tu MCU1 (canh len: 0 -> 1) */
        if (SIG_WRONG_IN == 1 && !prev_wrong) {
            prev_wrong = 1;
            Sound_Beep(2);
            UART_Log("NHAP SAI", g_min, g_sec);
        }
        if (SIG_WRONG_IN == 0) {
            prev_wrong = 0;
        }

        /* Kiem tra tin hieu mo cua tu MCU1 (canh len: 0 -> 1) */
        if (SIG_UNLOCK_IN == 1 && !prev_unlock) {
            prev_unlock = 1;
            UART_Log("DANG NHAP:OK", g_min, g_sec);
            while (!Door_Open()) {
                /* Bi ket: bip lien tuc cho den khi het ket */
                UART_Log("KET CUA (MO)!", g_min, g_sec);
                while (SIG_JAM_IN == 1) {
                    Sound_Beep(1);
                }
                SPEAKER = 0;
                UART_Log("HET KET -> MO TIEP", g_min, g_sec);
                /* Quay lai Door_Open() de chay tiep */
            }
            Sound_Beep(2);
            UART_Log("CUA:MO", g_min, g_sec);
        }
        if (SIG_UNLOCK_IN == 0) {
            prev_unlock = 0;
        }

        /* Kiem tra nut dong cua (active-low, canh xuong: 1 -> 0) */
        if (BTN_DOOR_CLOSE == 0 && !prev_close) {
            prev_close = 1;
            if (g_door_open) {
                while (!Door_Close()) {
                    UART_Log("KET CUA (DONG)!", g_min, g_sec);
                    while (SIG_JAM_IN == 1) {
                        Sound_Beep(1);
                    }
                    SPEAKER = 0;
                    UART_Log("HET KET -> DONG TIEP", g_min, g_sec);
                }
                Sound_Beep(2);
                UART_Log("CUA:DONG", g_min, g_sec);
            }
        }
        if (BTN_DOOR_CLOSE == 1) {
            prev_close = 0;
        }
    }
}
