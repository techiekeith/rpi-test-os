RASPI_MODEL?=2

ifeq ($(RASPI_MODEL),1)
	CPU=arm1176jzf-s
	DIRECTIVES=-D MODEL_1
	COMPILER_PREFIX=arm-none-eabi-
	QEMU_SUFFIX=arm
	MACHINE=raspi1ap
	MEMORY=512M
else
	CPU=cortex-a7
	DIRECTIVES=
	COMPILER_PREFIX=arm-none-eabi-
	QEMU_SUFFIX=arm
	MACHINE=raspi2b
	MEMORY=1G
endif

CC=$(COMPILER_PREFIX)gcc
OBJCOPY=$(COMPILER_PREFIX)objcopy

SRC_DIR=src
BUILD_DIR=build/$(MACHINE)
DIST_DIR=dist/$(MACHINE)

SUBDIRS=common kernel kernel/$(MACHINE) saa5050

C_FILES=$(wildcard $(SRC_DIR)/*.c $(foreach subdir, $(SUBDIRS), $(SRC_DIR)/$(subdir)/*.c))
ASM_FILES=$(wildcard $(SRC_DIR)/*.S $(foreach subdir, $(SUBDIRS), $(SRC_DIR)/$(subdir)/*.S))
OBJ_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES+=$(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

C_ASM_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.S)

FLAGS=$(DIRECTIVES) -mcpu=$(CPU) -fpic -ffreestanding -Iinclude
CFLAGS=$(FLAGS) -std=gnu99 -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib

all: build

assembly: $(C_ASM_FILES)

run: build
	# qemu-system-$(QEMU_SUFFIX) -M $(MACHINE) -m $(MEMORY) -serial stdio -kernel dist/myos.elf
	@echo Run 'run.bat' in a Windows shell.

build: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	mkdir -p $(DIST_DIR)
	$(CC) -T $(SRC_DIR)/linker.ld -o $(DIST_DIR)/myos.elf $(LDFLAGS) $(OBJ_FILES)
	$(OBJCOPY) $(DIST_DIR)/myos.elf -O binary $(DIST_DIR)/myos.bin

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
