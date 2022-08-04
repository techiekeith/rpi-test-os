/*
 * peripheral.h
 */

#pragma once

#ifdef MODEL_1
#define PERIPHERAL_BASE     0x20000000
#else
#define PERIPHERAL_BASE     0x3F000000
#endif

#define PERIPHERAL_LENGTH   0x01000000

#define MAILBOX_OFFSET      0x0000B880
#define GPIO_OFFSET         0x00200000
#define UART0_OFFSET        0x00201000
