/*
 * security.h  --  Bao mat: dang nhap mat khau 4 so, khoa he thong, doi mat khau
 *                 Nut bam: BTN1=P1.0 BTN2=P1.1 BTN3=P1.2 BTN4=P1.3
 *                 Bao dong: MCU2 xu ly qua tin hieu SIG_ALARM (GPIO)
 */

#ifndef SECURITY_H
#define SECURITY_H

/* Khoi tao module bao mat: reset dem sai, nap mat khau mac dinh */
void Security_Init(void);

/* Hien thi man hinh nhap mat khau, cho nguoi dung nhap 4 so.
   Tra ve 1 = dung mat khau, 0 = sai.
   Tu dong tang dem sai khi sai; goi Security_Lockdown() ben ngoai neu can. */
bit Security_Login(void);

/* Khoa he thong: bat ALARM, hien thi canh bao, loop vo han.
   Thoat bang cach reset vi dieu khien. */
void Security_Lockdown(void);

/* Quy trinh doi mat khau: nhap mat khau moi + xac nhan.
   Ket qua ghi vao RAM (mat khau tinh), khong luu qua reset. */
void Security_ChangePass(void);

/* So lan nhap sai hien tai (doc de kiem tra nguong ben ngoai) */
extern unsigned char g_fail_count;

#endif /* SECURITY_H */
