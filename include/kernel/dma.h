/*
 * dma.h
 */

#pragma once

#include "../common/stdbool.h"
#include "../common/stdint.h"
#include "peripheral.h"

#define DMA_CONTROL_BLOCK_BASE  (void *)0x1000
#define MAX_DMA_CONTROL_BLOCKS  128

#define DMA0_BASE               (peripheral_base + DMA_OFFSET)
#define DMA15_BASE              (peripheral_base + DMA15_OFFSET)

#define DMA_CS                  0x00 // Control and status register (RW)
#define DMA_CONBLK_AD           0x04 // Control block address (RW) - must be 256-bit aligned
#define DMA_TI                  0x08
#define DMA_SOURCE_AD           0x0c
#define DMA_DEST_AD             0x10
#define DMA_TXFR_LEN            0x14
#define DMA_STRIDE              0x18
#define DMA_NEXTCONBK           0x1c
#define DMA_DEBUG               0x20

typedef struct {
    uint8_t int_en: 1; // INTEN - interrupt enable
    uint8_t td_mode: 1; // TDMODE - 2D mode
    uint8_t bit_2: 1; // not used
    uint8_t wait_resp: 1; // WAIT_RESP - wait for a write response
    uint8_t dest_inc: 1; // DEST_INC - destination address increment
    uint8_t dest_width: 1; // DEST_WIDTH - destination transfer width (0 = 32-bit, 1 = 128-bit)
    uint8_t dest_dreq: 1; // DEST_DREQ - control destination writes with DREQ (0 = no effect, 1 = gate writes)
    uint8_t dest_ignore: 1; // DEST_IGNORE - ignore writes (0 = write data, 1 = do not perform writes)

    uint8_t src_inc: 1; // SRC_INC - source address increment
    uint8_t src_width: 1; // SRC_WIDTH - source transfer width (0 = 32-bit, 1 = 128-bit)
    uint8_t src_dreq: 1; // SRC_DREQ - control source reads with DREQ (0 = no effect, 1 = gate reads)
    uint8_t src_ignore: 1; // SRC_IGNORE - ignore reads (0 = write data, 1 = do not perform reads, zero writes)
    uint8_t burst_length: 4; // BURST_LENGTH - burst length of DMA transfers

    uint16_t permap: 5; // PERMAP - peripheral mapping
    uint16_t waits: 5; // WAITS - add wait cycles
    uint16_t no_wide_bursts: 1; // NO_WIDE_BURSTS - don't do wide writes as a 2 beat burst
    uint16_t bits_27_31: 5; // not used
} __attribute__((packed)) transfer_information_t;

typedef struct {
    transfer_information_t transfer_information;
    uint32_t source_ad; // SOURCE_AD - source address
    uint32_t dest_ad; // DEST_AD - destination address
    uint32_t txfr_len; // TXFR_LEN - transfer length
    uint32_t stride; // STRIDE - 2D mode stride
    uint32_t nextconbk; // NEXTCONBK - next control block address
    uint32_t reserved_6; // reserved - set to zero
    uint32_t reserved_7; // reserved - set to zero
} __attribute__((packed)) dma_control_block_t;

typedef struct {
    uint8_t active: 1; // ACTIVE - activate the DMA (RW)
    uint8_t end: 1; // END - DMA end flag (W1C)
    uint8_t int_status: 1; // INT - interrupt status (W1C)
    uint8_t dreq: 1; // DREQ - DREQ state (RO)
    uint8_t paused: 1; // PAUSED - DMA paused state (RO)
    uint8_t dreq_stops_dma: 1; // DREQ_STOPS_DMA - DMA paused by DREQ state (RO)
    uint8_t waiting_for_outstanding_writes: 1; // WAITING_FOR_OUTSTANDING_WRITES (RO)
    uint8_t bit_7: 1; // reserved

    uint8_t error: 1; // ERROR - DMA error (RO)
    uint8_t bits_9_15: 7; // reserved

    uint8_t priority: 4; // PRIORITY - AXI priority level (RW)
    uint8_t panic_priority: 4; // PANIC_PRIORITY - AXI panic priority level (RW)

    uint8_t bits_24_27: 4; // reserved
    uint8_t wait_for_outstanding_writes: 1; // WAIT_FOR_OUTSTANDING_WRITES (RW)
    uint8_t disdebug: 1; // DISDEBUG - disable debug pause signal (RW)
    uint8_t abort: 1; // ABORT - abort DMA (W1SC)
    uint8_t reset: 1; // RESET - DMA channel reset (W1SC)
} __attribute__((packed)) dma_control_status_register_t;

void dma_init();
void dma_copy(void *dest, const void *src, size_t bytes);
void dma_copy_multiple(void *dest, const void *src, size_t bytes, int ntimes, int seek);
void dma_set(void *dest, uint32_t value, size_t bytes);
void dma_zero(void *dest, size_t bytes);
