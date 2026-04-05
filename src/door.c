/*
 * door.c  --  Dieu khien cua cuon (dong co DC qua L293D + encoder)
 *
 * So do chan:
 *   MOTOR_IN1 = P2.3  (L293D IN1, pin 2)
 *   MOTOR_IN2 = P2.2  (L293D IN2, pin 7)
 *   Encoder   -> P3.2 (INT0, kich hoat canh xuong)
 *
 * Nguyen ly:
 *   Mo cua : IN1=1, IN2=0  (quay thuan)
 *   Dong cu: IN1=0, IN2=1  (quay nguoc)
 *   Dung   : IN1=0, IN2=0
 *
 * Encoder dem xung qua ngat INT0, dong co dung khi du DOOR_PULSES xung.
 */

#include "config.h"
#include "door.h"

/* Bien toan cuc: trang thai cua (1=dang mo, 0=dang dong) */
bit g_door_open = 0;

/* Co bao ket cua */
bit g_door_jammed = 0;

/* Bien dem xung encoder, cap nhat trong ISR INT0 */
static volatile unsigned char s_encoder = 0;

/* ----------------------------------------------------------------
 *  ISR INT0  --  Ngat ngoai 0 (P3.2): dem moi xung encoder
 * ---------------------------------------------------------------- */
void INT0_ISR(void) interrupt 0
{
    s_encoder++;    /* Tang dem moi khi co xung tu encoder */
}

/* ----------------------------------------------------------------
 *  Door_Init  --  Khoi tao module dieu khien cua
 * ---------------------------------------------------------------- */
void Door_Init(void)
{
    MOTOR_IN1 = 0;  /* Tat dong co truoc (tranh dong co giat khi moi bat) */
    MOTOR_IN2 = 0;

    s_encoder = 0;  /* Reset dem xung */
    g_door_open = 0;

    /* Cau hinh ngat ngoai INT0 (P3.2) */
    IT0 = 1;        /* Kich hoat theo canh xuong (falling edge) */
    EX0 = 1;        /* Cho phep ngat ngoai 0 */
    EA  = 1;        /* Cho phep ngat toan cuc */
}

/* ----------------------------------------------------------------
 *  Door_Open  --  Mo cua: quay thuan den khi du DOOR_PULSES xung
 * ---------------------------------------------------------------- */
bit Door_Open(void)
{
    if (g_door_open) return 1;  /* Cua da mo roi */

    if (!g_door_jammed) {       /* Lan dau: reset encoder */
        s_encoder = 0;
    }
    g_door_jammed = 0;          /* Clear co ket */

    MOTOR_IN1 = 1;              /* Kich quay thuan */
    MOTOR_IN2 = 0;

    while (s_encoder < DOOR_PULSES) {
        if (SIG_JAM_IN == 1) {  /* MCU1 bao ket cua */
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            g_door_jammed = 1;
            return 0;           /* That bai: bi ket */
        }
    }

    MOTOR_IN1 = 0;              /* Dung dong co */
    g_door_open = 1;
    return 1;
}

/* ----------------------------------------------------------------
 *  Door_Close  --  Dong cua: quay nguoc den khi du DOOR_PULSES xung
 * ---------------------------------------------------------------- */
bit Door_Close(void)
{
    if (!g_door_open) return 1; /* Cua da dong roi */

    if (!g_door_jammed) {       /* Lan dau: reset encoder */
        s_encoder = 0;
    }
    g_door_jammed = 0;

    MOTOR_IN1 = 0;
    MOTOR_IN2 = 1;              /* Kich quay nguoc */

    while (s_encoder < DOOR_PULSES) {
        if (SIG_JAM_IN == 1) {
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            g_door_jammed = 1;
            return 0;
        }
    }

    MOTOR_IN2 = 0;              /* Dung dong co */
    g_door_open = 0;
    return 1;
}
