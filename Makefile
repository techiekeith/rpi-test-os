RASPI_MODEL?=2
# CSUD=1
# CSUD_PORT=1

ifeq ($(RASPI_MODEL),1)
	DIRECTIVES=-D BCM2835
	COMPILER_PREFIX=arm-none-eabi-
	QEMU_SUFFIX=arm
	MACHINE=raspi1ap
	MEMORY=512M
	ARCH_FLAGS=-mfpu=vfp -mfloat-abi=hard -mcpu=arm1176jzf-s
	SOFTWARE_MATH_LIB=software_math/32bit software_math/64bit
else
	DIRECTIVES=
	COMPILER_PREFIX=arm-none-eabi-
	QEMU_SUFFIX=arm
	MACHINE=raspi2b
	MEMORY=1G
	ARCH_FLAGS=-mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7
	SOFTWARE_MATH_LIB=software_math/64bit
endif

CC=$(COMPILER_PREFIX)gcc
OBJCOPY=$(COMPILER_PREFIX)objcopy

SRC_DIR=src
DIST_DIR=dist

ifeq ($(CSUD),1)
	SUBDIRS=common kernel saa505x shell test csud $(SOFTWARE_MATH_LIB)
	DRIVER_FLAGS=-D CSUD
else ifeq ($(CSUD_PORT),1)
	SUBDIRS=common kernel saa505x shell test kernel/usb kernel/usb/device $(SOFTWARE_MATH_LIB)
	DRIVER_FLAGS=-D CSUD -D CSUD_PORT
else
	SUBDIRS=common kernel saa505x shell test $(SOFTWARE_MATH_LIB)
	DRIVER_FLAGS=
endif

ifeq ($(DEBUG),1)
	DEBUG_FLAGS=-ggdb
	BINARY=$(MACHINE)_g
	BUILD_DIR=build/$(MACHINE)/debug
else
	DEBUG_FLAGS=
	BINARY=$(MACHINE)
	BUILD_DIR=build/$(MACHINE)/release
endif

FLAGS=$(DEBUG_FLAGS) $(DIRECTIVES) $(DRIVER_FLAGS) $(ARCH_FLAGS) -fpic -ffreestanding -Iinclude
CFLAGS=$(FLAGS) -std=gnu99 -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib

C_FILES=$(wildcard $(SRC_DIR)/*.c $(foreach subdir, $(SUBDIRS), $(SRC_DIR)/$(subdir)/*.c))
ASM_FILES=$(wildcard $(SRC_DIR)/*.S $(foreach subdir, $(SUBDIRS), $(SRC_DIR)/$(subdir)/*.S))
OBJ_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES+=$(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)
C_ASM_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.S)

all: pi1 pi2

pi1: raspi1ap-debug raspi1ap

pi2: raspi2b-debug raspi2b

raspi1ap:
	RASPI_MODEL=1 make build

raspi2b:
	RASPI_MODEL=2 make build

raspi1ap-debug:
	RASPI_MODEL=1 DEBUG=1 make build

raspi2b-debug:
	RASPI_MODEL=2 DEBUG=1 make build

assembly: $(C_ASM_FILES)

build: $(DIST_DIR)/$(BINARY).bin

$(DIST_DIR)/$(BINARY).bin: $(DIST_DIR)/$(BINARY).elf
	$(OBJCOPY) $(DIST_DIR)/$(BINARY).elf -O binary $(DIST_DIR)/$(BINARY).bin

$(DIST_DIR)/$(BINARY).elf: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	mkdir -p $(DIST_DIR)
	$(CC) -T $(SRC_DIR)/linker.ld -o $(DIST_DIR)/$(BINARY).elf $(LDFLAGS) $(OBJ_FILES)

clean:
	rm -rf build dist

DEP_FILES=$(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(CC) $(FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c $(BUILD_DIR)/%_c.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_c.S: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -S -MMD -c $< -o $@
