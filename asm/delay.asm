; delay.asm -- Ham tre chinh xac theo chu ky may (AT89C52, 11.0592 MHz)
;
; Ham: Delay_ms
;   Tham so : R7 = so mili-giay can tre (1..255)
;   Su dung  : R6, R7 (khong luu tru, caller phai bao quan neu can)
;   Goi tu C : extern void Delay_ms(unsigned char ms); [khai bao trong config.h]
;
; Chu ky may = 12 / 11.0592 MHz ~ 1.085 us
; 1 ms can khoang 921 chu ky may
; Vong lap trong (R6): 0->0 = 256 lan, moi lan 4 chu ky -> 1024 chu ky ~ 1.11 ms
; Dieu chinh: NOP de bu sai so

    ORG  0100H          ; Dat code o dia chi tranh vung vector ngat

PUBLIC Delay_ms
Delay_ms:
    MOV  R7, A          ; A nhan tham so ms tu C (quy uoc Keil C51)
OUTER:
    MOV  R6, #0FAH      ; Nap 250 vao R6 cho vong lap trong (~1ms moi lan)
INNER:
    NOP                 ; 1 chu ky, bu sai so
    NOP
    DJNZ R6, INNER      ; Giam R6, nhay lai INNER neu R6 != 0 (2 chu ky)
    DJNZ R7, OUTER      ; Giam R7 (ms dem nguoc), nhay lai neu chua xong
    RET                 ; Quay lai ham goi

    END
