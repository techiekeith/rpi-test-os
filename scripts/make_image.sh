#!/usr/bin/env bash

# Move to project root
cd `dirname $0`
cd ..

LOSETUP=/usr/sbin/losetup

LOOPBACK_DEVICE=/dev/loop0
LOOPBACK_PARTITION=/dev/loop0p1

BOOT_SOURCES=boot
FIRMWARE_BOOT_SOURCES=../firmware/boot

DIST_DIR=dist
BOOT_DEST=$DIST_DIR/bootfs
KERNEL_SRC=$DIST_DIR/raspi1ap.bin
KERNEL_DEST=$BOOT_DEST/kernel.img
TMP_IMAGE=/tmp/rpi_sd_card.img
IMAGE=$DIST_DIR/rpi_sd_card.img

if [ ! -x $LOSETUP ]; then
  echo "The command 'losetup' is needed to run this program."
  exit 1
fi

# Create a disk image with an MBR and padded with zeroes to 256MB
# TODO It would be nicer to script fdisk instead of using a boot sector binary with a fixed-size partition table
dd if=$BOOT_SOURCES/mbr_sector of=$TMP_IMAGE bs=512 count=1
dd if=/dev/zero of=$TMP_IMAGE seek=1 bs=512 count=524287

# Move the image to the dist folder
mv $TMP_IMAGE $IMAGE

# Format the image filesystem
sudo losetup -Pfv $IMAGE
sudo mkfs -t vfat $LOOPBACK_PARTITION

# Mount the filesystem
sudo mkdir -p $BOOT_DEST
sudo mount -t vfat $LOOPBACK_PARTITION $BOOT_DEST

# Copy boot files
sudo cp --recursive --preserve=timestamps $FIRMWARE_BOOT_SOURCES/overlays $BOOT_DEST/
sudo cp --preserve=timestamps $FIRMWARE_BOOT_SOURCES/*.{bin,broadcom,dat,dtb,elf} $BOOT_DEST/
sudo cp --preserve=timestamps $BOOT_SOURCES/*.txt $BOOT_DEST/
sudo cp --preserve=timestamps $KERNEL_SRC $KERNEL_DEST

# Unmount the filesystem
sudo umount $LOOPBACK_PARTITION
sudo losetup -d $LOOPBACK_DEVICE
