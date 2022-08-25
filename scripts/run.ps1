param (
    [Parameter()][string]$system = "arm",
    [Parameter()][string]$machine = "raspi2b",
    [Parameter()][string]$image = "",
    [Parameter()][string]$build = "release"
)

if ($machine -eq "raspi1ap") {
    $memory = "512M"
} else {
    $memory = "1G"
}
if ($image -eq "") {
    $image = $machine
}
if ($build -eq "release") {
    $debug_opts = @()
    $window_title = "${machine}"
    $image = "dist\${machine}.elf"
} else {
    $debug_opts = @("-s", "-S")
    $window_title = "${machine} - ${image} GDB"
    $image = "dist\${image}_g.elf"
}

[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
& "C:\Program Files\qemu\qemu-system-${system}.exe" `
    $debug_opts `
    -M $machine `
    -m $memory `
    -name "$window_title" `
    -serial stdio `
    -usb `
    -device usb-kbd `
    -kernel $image
