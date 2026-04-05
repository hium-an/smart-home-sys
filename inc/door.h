/*
 * door.h  --  Dieu khien cua (dong co DC qua L293D + encoder)
 *             MOTOR_IN1 = P2.3, MOTOR_IN2 = P2.2
 *             Encoder xung vao INT0 (P3.2)
 */

#ifndef DOOR_H
#define DOOR_H

/* Trang thai cua (1 = dang mo, 0 = dang dong) */
extern bit g_door_open;

/* Co bao ket cua (1 = bi ket) */
extern bit g_door_jammed;

/* Khoi tao module: dat dong co ve 0, cau hinh ngat INT0 */
void Door_Init(void);

/* Mo cua: quay thuan DOOR_PULSES xung encoder roi dung */
/* Tra ve 1 = thanh cong, 0 = bi ket */
bit Door_Open(void);

/* Dong cua: quay nguoc DOOR_PULSES xung encoder roi dung */
/* Tra ve 1 = thanh cong, 0 = bi ket */
bit Door_Close(void);

#endif /* DOOR_H */
