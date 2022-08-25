#!/usr/bin/env bash
while getopts "m:b:" arg; do
  case $arg in
    m)
      machine=$OPTARG
      ;;
    b)
      build=$OPTARG
      ;;
    *)
      echo "Syntax: $0 [-m raspi1ap|raspi2b] [-b debug|release]"
  esac
done

if [ "x$machine" = "x" ]; then
  machine=raspi2b
fi
if [ "x$build" = "x" ]; then
  build=release
fi

if [ "$machine" = "raspi1ap" ]; then
  memory=512M
else
  memory=1G
fi
if [ "$build" = "release" ]; then
  debug_opts=
  window_title=$machine
  image=dist/${machine}.elf
else
  debug_opts='-s -S'
  window_title="$machine (GDB)"
  image=dist/${machine}_g.elf
fi

qemu-system-arm \
  $debug_opts \
  -M $machine \
  -m $memory \
  -name "$window_title" \
  -serial stdio \
  -usb \
  -device usb-kbd \
  -kernel $image
