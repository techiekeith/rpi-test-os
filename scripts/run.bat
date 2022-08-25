set machine=%1
set build=%2

if "%machine%"=="" (
    set machine=raspi2b
)
if "%build%"=="" (
    set build=release
)

if "%machine%"=="raspi1ap" (
    set memory=512M
) else (
    set memory=1G
)
if "%build%"=="release" (
    set debug_opts=
    set window_title=%machine%
    set image=dist\%machine%.elf
) else (
    set debug_opts=-s -S
    set window_title=%machine% GDB
    set image=dist\%machine%_g.elf
)

"C:\Program Files\qemu\qemu-system-arm.exe" ^
    %debug_opts% ^
    -M %machine% ^
    -m %memory% ^
    -name "%window_title%" ^
    -serial stdio ^
    -usb ^
    -device usb-kbd ^
    -kernel %image%
