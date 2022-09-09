/*
 * fifo.h
 */

#include "../common/stdbool.h"
#include "../common/stdint.h"

#pragma once

typedef struct {
    uint32_t size;              /* 0-3 */
    uint32_t start;             /* 4-7 */
    uint32_t end;               /* 8-11 */
    struct {
        bool empty: 1;
        bool full: 1;
        uint32_t __unused_bits: 30;
    };                          /* 12-15 */
    unsigned char *buffer;      /* 16-19 or 16-23, depending on word size */
} fifo_t;

#define UART_RX_FIFO                (fifo_t *)0x380
#define USB_RX_FIFO                 (fifo_t *)0x398

#define UART_RX_FIFO_BUFFER         (uint8_t *)0x400
#define UART_RX_FIFO_BUFFER_SIZE    (uint16_t)0x400

#define USB_RX_FIFO_BUFFER          (uint8_t *)0x800
#define USB_RX_FIFO_BUFFER_SIZE     (uint16_t)0x400

void fifo_init(fifo_t *fifo, uint8_t *buffer, uint32_t buffer_size);
bool fifo_add(fifo_t *fifo, uint8_t value);
bool fifo_get(fifo_t *fifo, uint8_t *value);
