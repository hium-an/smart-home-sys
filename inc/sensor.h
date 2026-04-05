/*
 * sensor.h  --  Cam bien nhiet do (mo phong nut bam) + anh sang (mo phong cong tac)
 *               BTN_TEMP_UP = P1.0, BTN_TEMP_DN = P1.1
 *               LIGHT_SW    = P1.2 (SWITCH 1: 0=troi toi, 1=troi sang)
 */

#ifndef SENSOR_H
#define SENSOR_H

/* Nhiet do hien tai (°C), cap nhat boi Sensor_CheckButtons() */
extern unsigned char g_temperature;

/* Khoi tao module: dat nhiet do mac dinh = TEMP_INIT */
void Sensor_Init(void);

/* Doc trang thai anh sang: tra ve 1=sang, 0=toi */
bit Sensor_IsLight(void);

/* Kiem tra 2 nut tang/giam nhiet do, cap nhat g_temperature
   Goi trong main loop de xu ly lien tuc */
void Sensor_CheckButtons(void);

#endif /* SENSOR_H */
