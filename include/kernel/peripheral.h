/*
 * peripheral.h
 */

#pragma once

#include "../common/stddef.h"

extern size_t peripheral_base;

#define PERIPHERAL_LENGTH       0x01000000

#define SYSTEM_TIMER_OFFSET     0x00003000
#define DMA_OFFSET              0x00007000
#define INTERRUPTS_OFFSET       0x0000b000
#define ARM_TIMER_OFFSET        0x0000b400
#define MAILBOX_OFFSET          0x0000b880
#define GPIO_OFFSET             0x00200000
#define UART0_OFFSET            0x00201000
#define HOST_CONTROLLER_OFFSET  0x00980000
#define DMA15_OFFSET            0x00E05000
