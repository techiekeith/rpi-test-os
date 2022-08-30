/*
 * board.h
 */

#pragma once

#include "../common/stddef.h"
#include "../common/stdint.h"

#define BOARD_ID_RPI0_1         0xb76
#define BOARD_ID_RPI2           0xc07
#define BOARD_ID_RPI3           0xd03
#define BOARD_ID_RPI4           0xd08

#define PERIPHERAL_BASE_RPI0_1  0x20000000
#define PERIPHERAL_BASE_RPI2_3  0x3f000000
#define PERIPHERAL_BASE_RPI4    0xfe000000

size_t get_peripheral_base(uint32_t board_id);
size_t get_mem_size();
uint64_t get_serial_number();
uint8_t *get_mac_address();

extern size_t peripheral_base;
extern uint64_t serial_number;
extern uint8_t mac_address[6];
