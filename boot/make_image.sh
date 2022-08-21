#!/usr/bin/env bash

cd `dirname $0`

LOSETUP=/usr/sbin/losetup
IMAGE=rpi_sd_card.img

if [ ! -x $LOSETUP ]; then
  echo "The command 'losetup' is needed to run this program."
  exit 1
fi

# Create 256MB boot disk
dd if=./mbr_sector of=../dist/$IMAGE bs=512 count=1
dd if=/dev/zero of=../dist/$IMAGE seek=1 bs=512 count=524287
sudo losetup -Pfv ../dist/$IMAGE
sudo mkfs -t vfat /dev/loop0p1
sudo mkdir -p ../dist/bootfs
sudo mount -t vfat /dev/loop0p1 ../dist/bootfs
sudo cp --preserve=timestamps *.{bin,broadcom,dat,elf,txt} ../dist/bootfs/
sudo cp --preserve=timestamps ../dist/raspi1ap.bin ../dist/bootfs/kernel.img
sudo umount /dev/loop0p1
sudo losetup -d /dev/loop0
