/*
 * sensor.c  --  Cam bien anh sang va nhiet do
 *
 * Anh sang (mo phong cong tac):
 *   P1.2 = LIGHT_SW (SWITCH 1 trong Proteus)
 *   Gat cong tac ON (1) = troi sang, OFF (0) = troi toi.
 *   Thay the LDR + LM358 vi 2 linh kien nay khong co trong BOM.
 *   Ket qua: 1 = co sang, 0 = toi
 *
 * Nhiet do (mo phong):
 *   Khong co cam bien nhiet thuc (AT89C52 khong co ADC).
 *   Nguoi dung nhan BTN_TEMP_UP (P1.0) / BTN_TEMP_DN (P1.1) de
 *   tang giam gia tri nhiet do ao, dung de kiem tra quat tu dong.
 */

#include "config.h"
#include "sensor.h"
#include "uart.h"

extern volatile unsigned char g_min;
extern volatile unsigned char g_sec;

/* Nhiet do hien tai (°C), khoang TEMP_MIN..TEMP_MAX */
unsigned char g_temperature;

/* ----------------------------------------------------------------
 *  Ham noi bo: delay chong rung nut bam (~20 ms)
 * ---------------------------------------------------------------- */
static void sens_debounce(void)
{
    unsigned char i, j;
    for (i = 0; i < 20; i++)
        for (j = 0; j < 125; j++);
}

/* ----------------------------------------------------------------
 *  Sensor_Init  --  Khoi tao module cam bien
 * ---------------------------------------------------------------- */
void Sensor_Init(void)
{
    g_temperature = TEMP_INIT;  /* Dat nhiet do ban dau ve mac dinh */
}

/* ----------------------------------------------------------------
 *  Sensor_IsLight  --  Doc trang thai cong tac anh sang (LIGHT_SW)
 *  Tra ve 1 = sang (cong tac gat ON = troi sang)
 *  Tra ve 0 = toi  (cong tac gat OFF = troi toi)
 * ---------------------------------------------------------------- */
bit Sensor_IsLight(void)
{
    return (bit)(LIGHT_SW);  /* Doc truc tiep chan P1.2 (SWITCH 1) */
}

/* ----------------------------------------------------------------
 *  Sensor_CheckButtons  --  Xu ly nut tang/giam nhiet do
 *  Goi lien tuc trong main loop de bat phim kip thoi
 * ---------------------------------------------------------------- */
void Sensor_CheckButtons(void)
{
    /* Kiem tra nut TANG nhiet do */
    if (BTN_TEMP_UP == 0) {
        sens_debounce();                    /* Chong rung tiep xuc */
        if (BTN_TEMP_UP == 0) {            /* Xac nhan nut that su duoc nhan */
            if (g_temperature < TEMP_MAX) {
                g_temperature++;            /* Tang nhiet do them 1 do */
                UART_Log("NHIET DO:TANG", g_min, g_sec);
            }
            while (BTN_TEMP_UP == 0);      /* Cho den khi nha nut */
        }
    }

    /* Kiem tra nut GIAM nhiet do */
    if (BTN_TEMP_DN == 0) {
        sens_debounce();
        if (BTN_TEMP_DN == 0) {
            if (g_temperature > TEMP_MIN) {
                g_temperature--;            /* Giam nhiet do them 1 do */
                UART_Log("NHIET DO:GIAM", g_min, g_sec);
            }
            while (BTN_TEMP_DN == 0);
        }
    }
}
