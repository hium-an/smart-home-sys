/*
 * main2.c  --  MCU3: Environment Control
 *
 * Chuc nang:
 *   F5. Hien thi nhiet do tren 7-SEG 2 so (multiplexing)
 *   F6. Quat tu dong theo nhiet do (BTN_TEMP_UP/DN mo phong)
 *   F7. Den BULB tu dong theo SWITCH anh sang + PWM do sang
 *       7-SEG 4 so hien thi do sang: "b" + phan tram (VD: b 60)
 *   F8b. UART log su kien MCU3 ra Virtual Terminal
 *
 * Keil: Project -> Options -> C51 -> Define: PROJ_MCU3
 * Files: main2.c  sensor.c  uart.c
 *
 * Luu y: BTN_BRIGHT_UP/DN da doi sang P1.4/P1.5
 *        P3.1 = TXD dung cho UART
 */

#include "config.h"
#include "sensor.h"
#include "uart.h"

/* ================================================================
 *  BIEN TOAN CUC
 * ================================================================ */
unsigned char g_brightness = BRIGHT_INIT;  /* Cap do sang (0-5) */

volatile unsigned int  g_ms  = 0;
volatile unsigned char g_sec = 0;
volatile unsigned char g_min = 0;

/* Bang ma 7-SEG anode chung */
static unsigned char code SEG_TABLE[10] = {
    SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
    SEG_5, SEG_6, SEG_7, SEG_8, SEG_9
};

/* ================================================================
 *  ISR TIMER0  --  PWM cho BULB (chu ky 20ms = 50Hz) + dem thoi gian
 * ================================================================ */
void Timer0_ISR(void) interrupt 1
{
    static unsigned char pwm = 0;
    TH0 = T0_HIGH;
    TL0 = T0_LOW;

    /* PWM dieu khien BULB */
    pwm++;
    if (pwm >= PWM_PERIOD) pwm = 0;
    BULB = (pwm < (g_brightness * 4)) ? 1 : 0;

    /* Dem dong ho MM:SS cho UART log */
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
    EA    = 1;
}

/* Delay noi bo (~1ms moi vong) */
static void seg_delay(void)
{
    unsigned char i;
    for (i = 0; i < 5; i++) {
        unsigned char j;
        for (j = 0; j < 125; j++);
    }
}

/* Tat tat ca digit (ca 2 display) */
static void SEG_AllOff(void)
{
    SEG_DIG1 = 0; SEG_DIG2 = 0;
    BRT_DIG1 = 0; BRT_DIG2 = 0; BRT_DIG3 = 0; BRT_DIG4 = 0;
}

/* Hien thi nhiet do 2 chu so tren 7-SEG nhiet do */
static void Display_Temp(void)
{
    /* Digit hang chuc */
    SEG_AllOff();
    P0       = SEG_TABLE[g_temperature / 10];
    SEG_DIG1 = 1;
    seg_delay();

    /* Digit hang don vi */
    SEG_AllOff();
    P0       = SEG_TABLE[g_temperature % 10];
    SEG_DIG2 = 1;
    seg_delay();

    SEG_DIG2 = 0;
}

/* Hien thi do sang tren 7-SEG 4 so: "b" + phan tram
   VD: cap 3 = 60% -> hien "b 60"
       cap 5 = 100% -> hien "b100" */
static void Display_Bright(void)
{
    unsigned int pct;
    unsigned char hundreds, tens, units;

    pct = (unsigned int)g_brightness * 20;
    hundreds = pct / 100;
    tens     = (pct / 10) % 10;
    units    = pct % 10;

    /* Digit 1: chu "b" */
    SEG_AllOff();
    P0       = SEG_b;
    BRT_DIG1 = 1;
    seg_delay();

    /* Digit 2: hang tram (1 hoac tat) */
    SEG_AllOff();
    if (hundreds > 0) {
        P0 = SEG_TABLE[hundreds];
    } else {
        P0 = SEG_OFF;           /* Tat digit neu = 0 */
    }
    BRT_DIG2 = 1;
    seg_delay();

    /* Digit 3: hang chuc */
    SEG_AllOff();
    P0       = SEG_TABLE[tens];
    BRT_DIG3 = 1;
    seg_delay();

    /* Digit 4: hang don vi */
    SEG_AllOff();
    P0       = SEG_TABLE[units];
    BRT_DIG4 = 1;
    seg_delay();

    BRT_DIG4 = 0;
}

/* Quat tu dong theo nhiet do */
static void Auto_Fan(void)
{
    static bit fan_on = 0;
    if (g_temperature >= TEMP_FAN_ON && !fan_on) {
        FAN    = 1;
        fan_on = 1;
        UART_Log("QUAT:BAT", g_min, g_sec);
    } else if (g_temperature <= TEMP_FAN_OFF && fan_on) {
        FAN    = 0;
        fan_on = 0;
        UART_Log("QUAT:TAT", g_min, g_sec);
    }
}

/* Den tu dong theo SWITCH anh sang */
static void Light_Logic(void)
{
    static bit bulb_on = 0;
    if (!Sensor_IsLight() && !bulb_on) {
        bulb_on = 1;
        g_brightness = BRIGHT_INIT;
        UART_Log("DEN:BAT (toi)", g_min, g_sec);
    } else if (Sensor_IsLight() && bulb_on) {
        bulb_on    = 0;
        g_brightness = 0;
        UART_Log("DEN:TAT (sang)", g_min, g_sec);
    }
}

/* Tang/giam do sang (debounce giong nut nhiet do) */
static void bright_debounce(void)
{
    unsigned char i, j;
    for (i = 0; i < 20; i++)
        for (j = 0; j < 125; j++);
}

static void Check_Brightness_Buttons(void)
{
    if (BTN_BRIGHT_UP == 0) {
        bright_debounce();
        if (BTN_BRIGHT_UP == 0) {
            if (g_brightness < BRIGHT_MAX) {
                g_brightness++;
                UART_Log("SANG:TANG", g_min, g_sec);
            }
            while (BTN_BRIGHT_UP == 0);
        }
    }

    if (BTN_BRIGHT_DN == 0) {
        bright_debounce();
        if (BTN_BRIGHT_DN == 0) {
            if (g_brightness > 0) {
                g_brightness--;
                UART_Log("SANG:GIAM", g_min, g_sec);
            }
            while (BTN_BRIGHT_DN == 0);
        }
    }
}

/* ================================================================
 *  MAIN
 * ================================================================ */
void main(void)
{
    Timer0_Init();
    UART_Init();
    Sensor_Init();
    FAN  = 0;
    BULB = 0;

    UART_SendStr("=== MCU3: ENVIRON ===\r\n");

    while (1) {
        /* Hien thi nhiet do lien tuc tren 7-SEG 2 so */
        Display_Temp();

        /* Hien thi do sang lien tuc tren 7-SEG 4 so */
        Display_Bright();

        /* Xu ly nut tang/giam nhiet do */
        Sensor_CheckButtons();

        /* Quat tu dong */
        Auto_Fan();

        /* Den tu dong theo anh sang */
        Light_Logic();

        /* Tang/giam do sang */
        Check_Brightness_Buttons();
    }
}
