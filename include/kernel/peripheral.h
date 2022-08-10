/*
 * peripheral.h
 */

#pragma once

#ifdef BCM2835
#define PERIPHERAL_BASE         0x20000000
#else
#define PERIPHERAL_BASE         0x3F000000
#endif

#define PERIPHERAL_LENGTH       0x01000000

#define INTERRUPTS_OFFSET       0x0000B000
#define ARM_TIMER_OFFSET        0x0000B400
#define MAILBOX_OFFSET          0x0000B880
#define GPIO_OFFSET             0x00200000
#define UART0_OFFSET            0x00201000
#define SYSTEM_TIMER_OFFSET     0x00203000
