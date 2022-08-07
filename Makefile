RASPI_MODEL?=2

ifeq ($(RASPI_MODEL),1)
	CPU=arm1176jzf-s
	DIRECTIVES=-D BCM2835
	COMPILER_PREFIX=arm-none-eabi-
	QEMU_SUFFIX=arm
	MACHINE=raspi1ap
	MEMORY=512M
	SOFTWARE_MATH_LIB=software_math/32bit software_math/64bit
else
	CPU=cortex-a7
	DIRECTIVES=
	COMPILER_PREFIX=arm-none-eabi-
	QEMU_SUFFIX=arm
	MACHINE=raspi2b
	MEMORY=1G
	SOFTWARE_MATH_LIB=software_math/64bit
endif

CC=$(COMPILER_PREFIX)gcc
OBJCOPY=$(COMPILER_PREFIX)objcopy

SRC_DIR=src
BUILD_DIR=build/$(MACHINE)
DIST_DIR=dist

SUBDIRS=common kernel saa5050 shell test $(SOFTWARE_MATH_LIB)

C_FILES=$(wildcard $(SRC_DIR)/*.c $(foreach subdir, $(SUBDIRS), $(SRC_DIR)/$(subdir)/*.c))
ASM_FILES=$(wildcard $(SRC_DIR)/*.S $(foreach subdir, $(SUBDIRS), $(SRC_DIR)/$(subdir)/*.S))
OBJ_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES+=$(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)
C_ASM_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.S)

FLAGS=$(DIRECTIVES) -mcpu=$(CPU) -fpic -ffreestanding -Iinclude
CFLAGS=$(FLAGS) -std=gnu99 -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib

all: raspi1ap raspi2b

raspi1ap:
	RASPI_MODEL=1 make build

raspi2b:
	RASPI_MODEL=2 make build

assembly: $(C_ASM_FILES)

build: $(DIST_DIR)/$(MACHINE).bin

$(DIST_DIR)/$(MACHINE).bin: $(DIST_DIR)/$(MACHINE).elf
	$(OBJCOPY) $(DIST_DIR)/$(MACHINE).elf -O binary $(DIST_DIR)/$(MACHINE).bin

$(DIST_DIR)/$(MACHINE).elf: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	mkdir -p $(DIST_DIR)
	$(CC) -T $(SRC_DIR)/linker.ld -o $(DIST_DIR)/$(MACHINE).elf $(LDFLAGS) $(OBJ_FILES)

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
