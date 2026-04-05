/*
 * config.h  --  Cau hinh chung cho 3 MCU
 *
 * Keil Project 1: Define: PROJ_MCU1  (Security/LCD)
 * Keil Project 2: Define: PROJ_MCU2  (Door/Sound/UART)
 * Keil Project 3: Define: PROJ_MCU3  (Environ)
 *
 * MCU1 (Security)  : LCD + Nut mat khau + Tin hieu GPIO -> MCU2
 * MCU2 (Door)      : Motor + Encoder + Sound + UART log
 * MCU3 (Environ)   : 7-SEG + Fan + Light + BULB PWM
 *
 * Giao tiep MCU1 -> MCU2:
 *   MCU1.P1.4 (SIG_UNLOCK) ---wire--- MCU2.P1.1 (SIG_UNLOCK_IN)
 *   MCU1.P1.5 (SIG_ALARM)  ---wire--- MCU2.P1.0 (SIG_ALARM_IN)
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <REGX52.H>

/* ================================================================
 *  HANG SO DUNG CHUNG (ca 3 MCU deu dung)
 * ================================================================ */
#define T0_HIGH        0xFC   /* Timer0 reload cao (1ms @ 11.0592MHz) */
#define T0_LOW         0x67   /* Timer0 reload thap */
#define T1_BAUD        0xFD   /* Timer1 baud 9600 */

#define PASS_LEN          4   /* So chu so mat khau */
#define MAX_FAIL          3   /* So lan sai toi da */
#define HOLD_CHANGE_MS 3000   /* Giu BTN1 bao lau de doi pass (ms) */
#define DOOR_PULSES       5   /* Xung encoder de mo/dong cua (giam de test nhanh) */

#define TEMP_FAN_ON      26   /* Nhiet do bat quat (do C) */
#define TEMP_FAN_OFF     25   /* Nhiet do tat quat (do C) */
#define TEMP_INIT        30   /* Nhiet do khoi tao */
#define TEMP_MAX         50
#define TEMP_MIN          0

#define BRIGHT_MAX        5   /* Cap do sang toi da */
#define BRIGHT_INIT       3   /* Cap do sang khoi tao (60%) */
#define BRIGHT_PCT(b) ((b)*20)
#define PWM_PERIOD       20   /* Chu ky PWM = 20ms (50Hz) */

/* ================================================================
 *  BANG MA 7-SEG (ANODE CHUNG, ACTIVE-LOW)
 * ================================================================ */
#define SEG_0  0xC0
#define SEG_1  0xF9
#define SEG_2  0xA4
#define SEG_3  0xB0
#define SEG_4  0x99
#define SEG_5  0x92
#define SEG_6  0x82
#define SEG_7  0xF8
#define SEG_8  0x80
#define SEG_9  0x90
#define SEG_b  0x83   /* Chu 'b' de hien thi cap do sang */
#define SEG_OFF 0xFF

/* ================================================================
 *  MCU1 -- SECURITY (LCD + Mat khau + GPIO tin hieu)
 *  Bien dich: Define: PROJ_MCU1
 *
 *  Chuc nang: LCD hien thi + 4 nut mat khau
 *             Gui tin hieu mo cua / bao dong sang MCU2 qua GPIO
 * ================================================================ */
#ifdef PROJ_MCU1

sbit BTN1        = P1^0;   /* Mat khau so 1 */
sbit BTN2        = P1^1;   /* Mat khau so 2 */
sbit BTN3        = P1^2;   /* Mat khau so 3 */
sbit BTN4        = P1^3;   /* Mat khau so 4 */
sbit SIG_UNLOCK  = P1^4;   /* Output -> MCU2: ra lenh mo cua */
sbit SIG_ALARM   = P1^5;   /* Output -> MCU2: ra lenh bao dong */
sbit SIG_WRONG   = P1^6;   /* Output -> MCU2: bao nhap sai mat khau */
sbit BTN_ALARM_OFF = P1^7;  /* Nut bam tat bao dong (active-low) */

sbit DOOR_JAM_SW = P2^0;   /* Switch gia lap ket cua (active-low = ket) */
sbit SIG_JAM     = P2^1;   /* Output -> MCU2: bao ket cua */
sbit BTN_CHPASS  = P2^2;   /* Nut bam giu 3s = doi mat khau (active-low) */
sbit LCD_RS      = P2^6;
sbit LCD_E       = P2^7;

sbit LCD_D4      = P3^4;
sbit LCD_D5      = P3^5;
sbit LCD_D6      = P3^6;
sbit LCD_D7      = P3^7;

#endif /* PROJ_MCU1 */

/* ================================================================
 *  MCU2 -- DOOR / SOUND / UART
 *  Bien dich: Define: PROJ_MCU2
 *
 *  Chuc nang: Dieu khien cua (Motor+Encoder) + Am thanh (Buzzer+Speaker)
 *             + UART log. Nhan tin hieu tu MCU1 qua GPIO.
 * ================================================================ */
#ifdef PROJ_MCU2

sbit SIG_ALARM_IN   = P1^0;   /* Input <- MCU1: lenh bao dong */
sbit SIG_UNLOCK_IN  = P1^1;   /* Input <- MCU1: lenh mo cua */
sbit BTN_DOOR_CLOSE = P1^2;   /* Nut bam dong cua (active-low) */
sbit SIG_WRONG_IN   = P1^3;   /* Input <- MCU1: bao nhap sai */
sbit SIG_JAM_IN     = P1^4;   /* Input <- MCU1: bao ket cua */
/* P1.5-P1.7: du phong MCU2 */

sbit MOTOR_IN1     = P2^3;   /* L293D IN1 (pin 2) — quay thuan (mo cua) */
sbit MOTOR_IN2     = P2^2;   /* L293D IN2 (pin 7) — quay nguoc (dong cua) */
sbit SPEAKER       = P2^4;   /* SPEAKER (qua BC547) — tat ca am thanh */
/* P2.5: du phong MCU2 */

/* P3.1 = TXD (phan cung UART, khong khai bao sbit) */
/* P3.2 = INT0/ENCODER (phan cung ngat, khong khai bao sbit) */

#endif /* PROJ_MCU2 */

/* ================================================================
 *  MCU3 -- ENVIRONMENT CONTROL
 *  Bien dich: Define: PROJ_MCU3
 *
 *  Chuc nang: 7-SEG nhiet do + Quat tu dong + Den BULB (PWM)
 *             SWITCH anh sang + BTN nhiet do + BTN do sang
 * ================================================================ */
#ifdef PROJ_MCU3

sbit BTN_TEMP_UP   = P1^0;   /* Tang nhiet do mo phong */
sbit BTN_TEMP_DN   = P1^1;   /* Giam nhiet do mo phong */
sbit LIGHT_SW      = P1^2;   /* Cong tac anh sang (SWITCH 1) */
sbit FAN           = P1^3;   /* Quat tu dong */
sbit BTN_BRIGHT_UP = P1^4;   /* Tang do sang den */
sbit BTN_BRIGHT_DN = P1^5;   /* Giam do sang den */
/* P1.6-P1.7: du phong MCU3 */

sbit SEG_DIG1      = P2^0;   /* 7-SEG nhiet do: digit hang chuc */
sbit SEG_DIG2      = P2^1;   /* 7-SEG nhiet do: digit hang don vi */
sbit BRT_DIG1      = P2^2;   /* 7-SEG do sang: digit 1 (chu 'b') */
sbit BRT_DIG2      = P2^3;   /* 7-SEG do sang: digit 2 (tram) */
sbit BULB          = P2^4;   /* Bong den (PWM tu Timer0 ISR) */
sbit BRT_DIG3      = P2^5;   /* 7-SEG do sang: digit 3 (chuc) */
sbit BRT_DIG4      = P2^6;   /* 7-SEG do sang: digit 4 (don vi) */

/* P3.1 = TXD (UART -> Virtual Terminal) */
/* P3.3, P3.4-P3.7: du phong MCU3 */

#endif /* PROJ_MCU3 */

#endif /* CONFIG_H */
