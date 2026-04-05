/*
 * sound.h  --  Dieu khien SPEAKER (P2.4)
 *
 * Tat ca am thanh (bip, melody, bao dong) deu qua SPEAKER
 */

#ifndef SOUND_H
#define SOUND_H

/* Bip n lan ngan - dung khi nhan nut, mo cua */
void Sound_Beep(unsigned char n);

/* Phat giai dieu chao mung khi khoi dong */
void Sound_Welcome(void);

/* 1 nhip bao dong (siren) - goi lap lai trong lockdown */
void Sound_AlarmTick(void);

#endif /* SOUND_H */
