@echo off
:: Build script cho SDCC (Small Device C Compiler)
:: Chay file nay trong thu muc goc cua project

echo === Building SmartHome with SDCC ===

:: Tao thu muc build neu chua co
if not exist build mkdir build

:: Bien dich tung file .c thanh .rel (relocatable object)
echo Compiling modules...
sdcc -mmcs51 --model-small -I inc -c src\lcd.c      -o build\lcd.rel
sdcc -mmcs51 --model-small -I inc -c src\uart.c     -o build\uart.rel
sdcc -mmcs51 --model-small -I inc -c src\door.c     -o build\door.rel
sdcc -mmcs51 --model-small -I inc -c src\sensor.c   -o build\sensor.rel
sdcc -mmcs51 --model-small -I inc -c src\security.c -o build\security.rel
sdcc -mmcs51 --model-small -I inc -c src\sound.c    -o build\sound.rel

:: Lien ket tat ca vao file HEX (main.c lien ket cuoi cung)
echo Linking...
sdcc -mmcs51 --model-small -I inc src\main.c ^
     build\lcd.rel build\uart.rel build\door.rel ^
     build\sensor.rel build\security.rel build\sound.rel ^
     -o build\SmartHome.ihx

:: Kiem tra ket qua
if exist build\SmartHome.ihx (
    echo.
    echo === BUILD THANH CONG ===
    echo File HEX: build\SmartHome.ihx
    echo Nap file nay vao AT89C52 trong Proteus.
) else (
    echo.
    echo === BUILD THAT BAI ===
    echo Kiem tra loi o tren.
)

pause
