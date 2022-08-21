param (
    [Parameter()][string]$machine = "raspi2b",
    [Parameter()][string]$build = "release"
)

if ($machine -eq "raspi1ap") {
    $memory = "512M"
} else {
    $memory = "1G"
}
if ($build -eq "release") {
    $debug_opts = @()
    $window_title = "${machine}"
    $image = "dist\${machine}.elf"
} else {
    $debug_opts = @("-s", "-S")
    $window_title = "${machine} GDB"
    $image = "dist\${machine}_g.elf"
}

[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
& "C:\Program Files\qemu\qemu-system-arm.exe" `
    $debug_opts `
    -M $machine `
    -m $memory `
    -name "$window_title" `
    -serial stdio `
    -usb `
    -device usb-kbd `
    -kernel $image
