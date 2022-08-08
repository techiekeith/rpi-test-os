# rpi-test-os

Experimental repo for building a Raspberry Pi OS.

At the moment it's pretty much just this - https://github.com/jsandler18/raspi-kernel - with some reorganisation.

SAA505x glyphs are courtesy of Mullard.

## Prerequisites

* [Arm GNU Toolchain (bare-metal)](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads)
* [QEMU](https://www.qemu.org/)

Linux/WSL2:
```
sudo apt-get install gdb gcc-arm-none-eabi binutils-arm-none-eabi qemu qemu-system-arm
```

## CLion build and run configurations

Add the WSL toolchain:

* Go to `Settings` > `Build, Execution, Deployment` > `Toolchains`
* Select the `Add` icon to add a toolchain
* Select `WSL` from the drop-down

Set up custom build configurations as follows:

_Raspberry Pi Model B_

* Add a new Custom Build Application
* Select `Configure Custom Build Targets`
* Name: `Raspberry Pi Model B`
* For Target, configure a custom build target as follows:
  * Name: `Raspberry Pi Model B`
  * Toolchain: `WSL`
  * Build: `raspi1ap`
  * Clean: `clean`
* Executable: `C:\Program Files\qemu\qemu-system-arm.exe`
* Program arguments: `-M raspi1ap -m 512M -serial stdio -kernel dist\raspi1ap.elf`
* Working directory: `$ContentRoot$`

_Raspberry Pi 2_

* Add a new Custom Build Application
* Select `Configure Custom Build Targets`
* Name: `Raspberry Pi 2`
* For Target, configure a custom build target as follows:
    * Name: `Raspberry Pi 2`
    * Toolchain: `WSL`
    * Build: `raspi2b`
    * Clean: `clean`
* Executable: `C:\Program Files\qemu\qemu-system-arm.exe`
* Program arguments: `-M raspi2b -m 1G -serial stdio -kernel dist\raspi2b.elf`
* Working directory: `$ContentRoot$`

## Resources

* [Building an Operating System for the Raspberry Pi - jsandler18](https://jsandler18.github.io/)
* [Raspberry Pi Bare Bones - OSDev Wiki](https://wiki.osdev.org/Raspberry_Pi_Bare_Bones)
* [Baking Pi - Course by Alex Chadwick](https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/)
* [ARM Instruction Set](https://iitd-plos.github.io/col718/ref/arm-instructionset.pdf)
* [Chadderz's Simple USB Driver for Raspberry Pi](https://github.com/Chadderz121/csud)
* [Mullard SAA505x emulator](https://github.com/techiekeith/mullard-saa505x)
