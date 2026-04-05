/*
 * main.c  --  Smart Home System - diem vao chinh
 *
 * Vi dieu khien : AT89C52 @ 11.0592 MHz
 * Chuc nang     :
 *   1. Khoi tao he thong + man hinh chao (loading bar)
 *   2. Dong ho MM:SS (Timer0 ISR 1ms)
 *   3. Bao mat: nhap mat khau 4 so mo cua, sai 3 lan -> bao dong
 *   3.1 Doi mat khau: giu BTN1 (P1.0) 3 giay
 *   4. Dieu khien cua: mo/dong qua dong co L293D + encoder
 *   5. Giam sat nhiet do: nut tang/giam, hien thi 7-SEG 2 so
 *   6. Quat tu dong: bat khi nhiet do > TEMP_FAN_ON
 *   7. Chieu sang tu dong: LDR -> bat/tat bong den
 *   8. Log UART 9600 co nhan thoi gian [MM:SS]
 *
 * Super-loop, khong RTOS.
 */

#include "config.h"
#include "lcd.h"
#include "uart.h"
#include "door.h"
#include "sensor.h"
#include "security.h"
#include "sound.h"

/* ================================================================
 *  BIEN TOAN CUC
 * ================================================================ */

/* Dong ho: cap nhat trong Timer0_ISR */
volatile unsigned int  g_ms  = 0;      /* Dem mili-giay (0..999) */
volatile unsigned char g_sec = 0;      /* Giay (0..59) */
volatile unsigned char g_min = 0;      /* Phut (0..59) */
volatile bit           g_tick_1s = 0;  /* Co: set moi giay, clear trong main */

/* Trang thai he thong sau khi dang nhap thanh cong */
bit g_unlocked = 0;

/* Do sang den (0-5): 0=tat, 1=20%, 2=40%, 3=60%, 4=80%, 5=100% */
unsigned char g_brightness = BRIGHT_INIT;

/* Bang ma 7-SEG anode chung (active-low segment) */
static unsigned char code SEG_TABLE[10] = {
    SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
    SEG_5, SEG_6, SEG_7, SEG_8, SEG_9
};

/* ================================================================
 *  KHAI BAO TRUOC CAC HAM NOI BO
 * ================================================================ */
void Timer0_Init(void);
void Welcome(void);
void Display_Clock(void);
void Display_Temp_7SEG(unsigned char temp);
void Auto_Fan(void);
void Light_Logic(void);
bit Check_Hold_BTN1(void);
void Check_Brightness_Buttons(void);
void Display_Brightness(void);

/* ================================================================
 *  ISR: TIMER 0  --  Ngat moi 1 ms, dem dong ho MM:SS
 * ================================================================ */
void Timer0_ISR(void) interrupt 1
{
    /* Nap lai gia tri de tao ngat chinh xac 1 ms tiep theo */
    TH0 = T0_HIGH;
    TL0 = T0_LOW;

    /* --- PWM dieu chinh do sang den (chu ky 20ms) ---
       g_brightness = 0..5 -> duty = 0/4/8/12/16/20 ms trong 20ms */
    {
        static unsigned char pwm = 0;
        pwm++;
        if (pwm >= PWM_PERIOD) pwm = 0;
        /* Neu pwm < (g_brightness*4): BULB bat, nguoc lai tat */
        BULB = (pwm < (g_brightness * 4)) ? 1 : 0;
    }

    /* Tang bien dem mili-giay */
    g_ms++;
    if (g_ms >= 1000) {     /* Du 1000 ms = 1 giay */
        g_ms = 0;
        g_sec++;
        g_tick_1s = 1;      /* Bao cho main loop biet da qua 1 giay */

        if (g_sec >= 60) {  /* Du 60 giay = 1 phut */
            g_sec = 0;
            g_min++;
            if (g_min >= 60) g_min = 0; /* Quay vong sau 60 phut */
        }
    }
}

/* ================================================================
 *  Timer0_Init  --  Khoi dong Timer0 che do 1ms interrupt
 * ================================================================ */
void Timer0_Init(void)
{
    TMOD |= 0x01;   /* Timer0: Mode 1 (16-bit, khong tu lap lai) */
    TH0   = T0_HIGH;
    TL0   = T0_LOW;
    ET0   = 1;      /* Cho phep ngat Timer0 */
    TR0   = 1;      /* Bat Timer0 chay */
}

/* ================================================================
 *  Welcome  --  Man hinh chao + hieu ung loading bar
 * ================================================================ */
void Welcome(void)
{
    /* Dong 0: ten he thong */
    LCD_GotoXY(0, 0); LCD_PutStr("SMART HOME SYS");

    /* Dong 1: ten tac gia */
    LCD_GotoXY(0, 1); LCD_PutStr("   by Hieu    ");

    /* Hieu ung loading bar tren dong 1: 16 khoi, moi khoi cach 100 ms */
    LCD_LoadingBar(1, 16, 100);

    LCD_Clear();           /* Xoa man hinh sau hieu ung */
    Sound_Welcome();       /* Phat giai dieu chao mung qua Speaker+Buzzer */
}

/* ================================================================
 *  Display_Clock  --  Hien thi MM:SS tren LCD goc phai tren (cot 10)
 * ================================================================ */
void Display_Clock(void)
{
    LCD_GotoXY(10, 0);          /* Di den cot 10, dong 0 */
    LCD_PutNum(g_min, 2);       /* In phut 2 chu so */
    LCD_PutChar(':');
    LCD_PutNum(g_sec, 2);       /* In giay 2 chu so */
}

/* ================================================================
 *  Display_Temp_7SEG  --  Hien thi nhiet do 2 chu so tren 7-SEG
 *  Goi lien tuc trong main loop de duy tri do sang (multiplexing)
 * ================================================================ */
void Display_Temp_7SEG(unsigned char temp)
{
    unsigned char tens  = temp / 10;    /* Lay chu so hang chuc */
    unsigned char units = temp % 10;    /* Lay chu so hang don vi */
    unsigned char i;

    /* --- Hien thi digit hang CHUC (DIG1) --- */
    P0       = SEG_TABLE[tens];     /* Dat ma phan doan len bus P0 */
    SEG_DIG1 = 1;                   /* Bat digit hang chuc */
    SEG_DIG2 = 0;                   /* Tat digit hang don vi */
    /* Tre ~2 ms de mat nguoi nhin thay (toc do quet >= 50 Hz) */
    for (i = 0; i < 2; i++) { unsigned char j; for (j=0;j<125;j++); }

    /* --- Hien thi digit hang DON VI (DIG2) --- */
    P0       = SEG_TABLE[units];
    SEG_DIG1 = 0;
    SEG_DIG2 = 1;
    for (i = 0; i < 2; i++) { unsigned char j; for (j=0;j<125;j++); }

    /* Tat ca digit de tranh bong ma (ghosting) */
    SEG_DIG1 = 0;
    SEG_DIG2 = 0;
}

/* ================================================================
 *  Check_Brightness_Buttons  --  Tang/giam do sang den (BTN_BRIGHT)
 *  Goi trong main loop; LDR van dieu khien on/off tu dong nhung
 *  muc sang do g_brightness quyet dinh (0=tat han, 5=sang toi da).
 * ================================================================ */
void Check_Brightness_Buttons(void)
{
    static unsigned int dbounce = 0;    /* Bien chong rung don gian */

    if (BTN_BRIGHT_UP == 0) {
        dbounce++;
        if (dbounce == 20) {             /* Xac nhan sau ~20ms */
            if (g_brightness < BRIGHT_MAX) {
                g_brightness++;
                Sound_Beep(1);           /* Bip 1 lan bao nhan nut */
                Display_Brightness();    /* Cap nhat LCD ngay */
                UART_Log("SANG:TANG", g_min, g_sec);
            }
        }
        if (dbounce > 25) dbounce = 25; /* Giu nguyen khi giu nut */
    } else if (BTN_BRIGHT_DN == 0) {
        dbounce++;
        if (dbounce == 20) {
            if (g_brightness > 0) {
                g_brightness--;
                Sound_Beep(1);
                Display_Brightness();
                UART_Log("SANG:GIAM", g_min, g_sec);
            }
        }
        if (dbounce > 25) dbounce = 25;
    } else {
        dbounce = 0;                     /* Nha nut: reset dem */
    }
}

/* ================================================================
 *  Display_Brightness  --  Hien thi do sang tren LCD dong 1 ben trai
 *  "SANG:XX%  MM:SS"
 * ================================================================ */
void Display_Brightness(void)
{
    unsigned char pct = BRIGHT_PCT(g_brightness);   /* 0,20,40,60,80,100 */
    LCD_GotoXY(0, 1);
    LCD_PutStr("SANG:");
    LCD_PutNum(pct, 3);                 /* In 3 chu so co tro so 0 */
    LCD_PutChar('%');
    LCD_PutChar(' ');
}

/* ================================================================
 *  Auto_Fan  --  Quat tu dong dua vao nhiet do
 * ================================================================ */
void Auto_Fan(void)
{
    static bit fan_on = 0;  /* Luu trang thai hien tai de tranh log lap lai */

    if (g_temperature >= TEMP_FAN_ON && !fan_on) {
        FAN   = 1;          /* Bat quat */
        fan_on = 1;
        UART_Log("QUAT:BAT", g_min, g_sec);
    } else if (g_temperature <= TEMP_FAN_OFF && fan_on) {
        FAN   = 0;          /* Tat quat */
        fan_on = 0;
        UART_Log("QUAT:TAT", g_min, g_sec);
    }
}

/* ================================================================
 *  Light_Logic  --  Bat/tat bong den dua vao cong tac anh sang (LIGHT_SW)
 *  LIGHT_SW = 0 (OFF): troi toi -> bat den tu dong
 *  LIGHT_SW = 1 (ON) : troi sang -> tat den
 *  Hien thi trang thai "TROI: TOI" / "TROI:SANG" tren LCD dong 0 (khi da mo khoa)
 * ================================================================ */
void Light_Logic(void)
{
    static bit bulb_on = 0;

    if (!Sensor_IsLight() && !bulb_on) {
        BULB    = 1;        /* Troi toi -> bat den */
        bulb_on = 1;
        UART_Log("DEN:BAT", g_min, g_sec);
        if (g_unlocked) {
            LCD_GotoXY(0, 0); LCD_PutStr("TROI: TOI ");
        }
    } else if (Sensor_IsLight() && bulb_on) {
        BULB    = 0;        /* Troi sang -> tat den */
        bulb_on = 0;
        UART_Log("DEN:TAT", g_min, g_sec);
        if (g_unlocked) {
            LCD_GotoXY(0, 0); LCD_PutStr("TROI:SANG ");
        }
    }
}

/* ================================================================
 *  Check_Hold_BTN1  --  Phat hien giu BTN1 >= HOLD_CHANGE_MS ms
 *  Tra ve 1 neu giu du lau, 0 neu la nhan ngan hoac khong nhan
 * ================================================================ */
bit Check_Hold_BTN1(void)
{
    unsigned int count = 0;     /* Dem ms giu nut */
    unsigned int i, j;

    if (BTN1 != 0) return 0;   /* BTN1 chua duoc nhan */

    /* Dem thoi gian giu (moi vong ~ 1 ms) */
    while (BTN1 == 0) {
        for (i = 0; i < 1; i++)
            for (j = 0; j < 125; j++);
        count++;
        if (count >= HOLD_CHANGE_MS) {
            while (BTN1 == 0);  /* Cho nha nut truoc khi xu ly */
            return 1;           /* Giu du lau: kich hoat doi mat khau */
        }
    }

    return 0;   /* Nhan ngan: chua du thoi gian */
}

/* ================================================================
 *  MAIN
 * ================================================================ */
void main(void)
{
    /* --- Khoi tao tat ca module --- */
    UART_Init();        /* UART phai khoi tao truoc (Timer1) */
    Timer0_Init();      /* Timer0 cho dong ho 1 ms */
    LCD_Init();         /* Khoi dong LCD */
    Door_Init();        /* Dong co + encoder INT0 */
    Sensor_Init();      /* Nhiet do ban dau */
    Security_Init();    /* Reset dem sai, tat bao dong */

    /* Tat tat ca chap hanh truoc */
    FAN  = 0;
    BULB = 0;
    SOUND = 0;
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;

    /* Bat ngat toan cuc (cung duoc set trong Door_Init, dam bao lai) */
    EA = 1;

    UART_SendStr("=== SMART HOME - Group One ===\r\n");

    /* Hien thi do sang ban dau tren LCD dong 1 */
    Display_Brightness();

    /* --- Man hinh chao --- */
    Welcome();

    /* --- Vong lap chinh --- */
    while (1) {

        /* 1. Quet 7-SEG lien tuc de duy tri hien thi nhiet do */
        Display_Temp_7SEG(g_temperature);

        /* 2. Kiem tra nut nhiet do */
        Sensor_CheckButtons();

        /* 3. Quat tu dong theo nhiet do */
        Auto_Fan();

        /* 4. Den tu dong theo cam bien anh sang */
        Light_Logic();

        /* 4b. Nut dieu chinh do sang (BTN_BRIGHT_UP/DN thay the POTENTIOMETER) */
        Check_Brightness_Buttons();

        /* 5. Kiem tra giu BTN1 de doi mat khau (uu tien truoc khi nhap pass) */
        if (Check_Hold_BTN1()) {
            Security_ChangePass();
            /* Sau khi doi xong: hien lai man hinh nhap mat khau */
        }

        /* 6. Neu chua mo khoa: hien man hinh dang nhap */
        if (!g_unlocked) {
            /* Hien man hinh dang nhap va ket qua */
            if (Security_Login()) {
                /* Dang nhap thanh cong */
                g_unlocked = 1;
                LCD_Clear();
                LCD_GotoXY(0, 0); LCD_PutStr("CHAO MUNG!");
                LCD_GotoXY(0, 1); LCD_PutStr("MO CUA...");
                UART_Log("DANG NHAP:OK", g_min, g_sec);
                Door_Open();
                Sound_Beep(3);          /* 3 tieng bip khi mo cua (Speaker + Buzzer) */
                UART_Log("CUA:MO", g_min, g_sec);
            } else {
                /* Sai mat khau */
                LCD_Clear();
                LCD_GotoXY(0, 0); LCD_PutStr("SAI MAT KHAU!");
                LCD_GotoXY(0, 1);
                LCD_PutStr("Con lai: ");
                LCD_PutNum(MAX_FAIL - g_fail_count, 1);
                UART_Log("DANG NHAP:SAI", g_min, g_sec);

                /* Doi 1.5 giay hien thong bao, van quet 7-SEG trong luc cho */
                {
                    unsigned int w;
                    for (w = 0; w < 750; w++) {
                        Display_Temp_7SEG(g_temperature);
                    }
                }

                /* Kiem tra vuot nguong sai toi da */
                if (g_fail_count >= MAX_FAIL) {
                    Security_Lockdown();    /* Loop vo han ben trong */
                }
            }
        }

        /* 7. Cap nhat dong ho LCD moi giay (khi da mo khoa) */
        if (g_unlocked && g_tick_1s) {
            g_tick_1s = 0;          /* Xoa co de khong cap nhat lai lan nua */
            Display_Clock();        /* In MM:SS goc phai dong 0 */
        }

        /* 8. Neu cua dang mo va can dong (vi du: sau 30 giay tu dong dong) */
        /* [Mo rong tuong lai: them timer dong cua tu dong] */
    }
}
