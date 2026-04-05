/*
 * sound.c  --  Dieu khien SPEAKER (P2.4)
 *
 * SPEAKER (P2.4): tat ca am thanh (bip, melody, bao dong)
 * Qua transistor BC547, dieu khien bang PWM tan so
 */

#include "config.h"
#include "sound.h"

/* Ham noi bo: tre don gian (don vi: luong vong lap) */
static void snd_delay(unsigned int ticks)
{
    unsigned int i;
    for (i = 0; i < ticks; i++);
}

/* Ham noi bo: tre ms */
static void snd_delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 125; j++);
}

/* Ham noi bo: phat tone tren SPEAKER (PWM tan so)
   half_period_ticks: nua chu ky (don vi vong lap snd_delay)
   duration_ms: thoi gian phat (ms) */
static void snd_tone(unsigned int half_period_ticks, unsigned int duration_ms)
{
    unsigned long total;
    unsigned long i;
    total = (unsigned long)duration_ms * 111UL / half_period_ticks;
    for (i = 0; i < total; i++) {
        SPEAKER = 1;
        snd_delay(half_period_ticks);
        SPEAKER = 0;
        snd_delay(half_period_ticks);
    }
}

/* ----------------------------------------------------------------
 *  Sound_Beep  --  Bip n lan ngan (tone ~1kHz, 60ms)
 * ---------------------------------------------------------------- */
void Sound_Beep(unsigned char n)
{
    unsigned char i;
    for (i = 0; i < n; i++) {
        snd_tone(110, 60);      /* ~1kHz trong 60ms */
        snd_delay_ms(120);      /* Nghi 120ms */
    }
}

/* ----------------------------------------------------------------
 *  Sound_Welcome  --  Giai dieu chao mung 3 not tang dan
 * ---------------------------------------------------------------- */
void Sound_Welcome(void)
{
    snd_tone(85, 30);
    snd_delay_ms(150);
    snd_tone(63, 30);
    snd_delay_ms(150);
    snd_tone(48, 30);
    SPEAKER = 0;
}

/* ----------------------------------------------------------------
 *  Sound_AlarmTick  --  1 nhip bao dong (siren sweep thap -> cao)
 * ---------------------------------------------------------------- */
void Sound_AlarmTick(void)
{
    unsigned char f;
    /* Sweep tan so tu thap len cao */
    for (f = 50; f > 6; f -= 2) {
        SPEAKER = 1;
        snd_delay(f);
        SPEAKER = 0;
        snd_delay(f);
    }
    SPEAKER = 0;
    snd_delay_ms(100);         /* Nghi 100ms giua cac nhip */
}
