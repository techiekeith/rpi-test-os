# rpi-test-os

Experimental repo for building a Raspberry Pi OS.

At the moment it's pretty much just this - https://github.com/jsandler18/raspi-kernel - with some reorganisation.

SAA505x glyphs are courtesy of Mullard.

## Prerequisites

* [Arm GNU Toolchain (bare-metal)](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads)
* [QEMU](https://www.qemu.org/)

Linux/WSL2:
```
sudo apt-get install gdb binutils-arm-none-eabi qemu qemu-system-arm
```

## Resources

* https://github.com/jsandler18/raspi-kernel
