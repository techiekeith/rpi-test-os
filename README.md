# rpi-test-os

Experimental repo for building a Raspberry Pi OS.

At the moment it's pretty much just this - https://github.com/jsandler18/raspi-kernel - with some reorganisation.

SAA505x glyphs are courtesy of Mullard.

## Prerequisites

> On Windows, I have used the Linux version of the Arm GNU Toolchain and GNU Make in WSL.
> I found this easier than trying to run GNU Make in Windows. YMMV.

> On Linux, use the resources below rather than any provided by your OS package manager.
> The package manager versions are likely to be far older and not as fully featured.

* [Arm GNU Toolchain (bare-metal)](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads)
* [QEMU](https://www.qemu.org/)

## Building and debugging in Windows with CLion and WSL

If you're building ARM images in WSL, add the toolchain to CLion as follows:

* Go to `Settings` > `Build, Execution, Deployment` > `Toolchains`
* If `WSL` is not listed:
  * Select the `Add` icon to add a toolchain
  * Select `WSL` from the drop-down
  * Move `WSL` to the top of the list

You'll also need a custom build configuration, because reasons:

* In the configuration drop-down, select 'Edit configurations'
* Add a new Makefile Application
* Select the `Configure Custom Build Targets` icon following the Target field
* Add a new Make target as follows
  * Name: `Make (WSL)`
  * Toolchain: `WSL`
  * Build: `all`
  * Clean: `clean`
* Select `Apply` then `OK` to get back to the Makefile Application configuration
* In the `Executable` field, enter `/usr/bin/bash`
* In the `Program arguments` field, enter `-c echo`
* Give the configuration a name e.g. `Build`

### Running a machine in QEMU

* Add a new Shell Script configuration for each of your machines
  * Name: The name of the machine (e.g. `Raspberry Pi Model B`)
  * Script text: the QEMU command to execute
  * Working directory: the path to the project directory
  * Before launch: select the `Build All` configuration

On Windows, the script text will be something like this for a Pi Model B
```pwsh
& 'C:\Program Files\qemu\qemu-system-arm.exe' -serial stdio -usb -usbdevice keyboard -M raspi1ap -name raspi1ap -m 512M -kernel dist\raspi1ap.elf
```
or for a Pi 2
```pwsh
& 'C:\Program Files\qemu\qemu-system-arm.exe' -serial stdio -usb -usbdevice keyboard -M raspi2b -name raspi2b -m 1G -kernel dist\raspi2b.elf
```

### Running the debug kernel

* Copy the existing Shell Script configuration for the machine you want to run in debug mode
  * Give it a slightly different name (e.g. `Raspberry Pi Model B (GDB)`)
  * Add the following command line arguments:
    * `-s` - this tells QEMU to start a GDB Server on port 1234 \
      _if you wish to use a different port, use the option `-gdb tcp::<port>` instead_
    * `-S` - this tells QEMU not to start the guest OS until a GDB client has connected to it
  * Change the value of the `-name` option to something slightly different like `raspi1ap-debug`
  * Change the value of the `-kernel` option to the debug kernel you selected in "Remote Debug" above \
    (e.g. `dist\raspi1ap_g.elf`)

### Setting up the Remote Debug client

* Add a new Remote Debug configuration
  * Name: The name of the machine (e.g. `Raspberry Pi Model B`)
  * Debugger: Bundled GDB (multiarch)
  * 'target remote' args: `localhost:1234`
  * Symbol file: the debug kernel for the machine, with a filename ending in `_g.elf` \
    (e.g. `$ProjectFileDir$\dist\raspi1ap_g.elf`)
  * Sysroot: the project directory

## Building and debugging in Linux

### Building

Ensure that GNU Make is installed and that the ARM Toolchain binaries are on your `PATH`.

```bash
cd path/to/project/dir
make
```

### Running

> I've not tried this yet in Linux, but you can probably work it out from the above CLion configurations.

### Debugging

> I've not tried this yet in Linux, but you can probably work it out from the above CLion configurations.

## Caveat

I have yet to test any of this on a real Raspberry Pi. It mostly works in QEMU, but that's no guarantee.

## Resources

* [Building an Operating System for the Raspberry Pi - jsandler18](https://jsandler18.github.io/)
* [Raspberry Pi Bare Bones - OSDev Wiki](https://wiki.osdev.org/Raspberry_Pi_Bare_Bones)
* [Baking Pi - Course by Alex Chadwick](https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/)
* [ARM Instruction Set](https://iitd-plos.github.io/col718/ref/arm-instructionset.pdf)
* [Chadderz's Simple USB Driver for Raspberry Pi](https://github.com/Chadderz121/csud)
* [Mullard SAA505x emulator](https://github.com/techiekeith/mullard-saa505x)
