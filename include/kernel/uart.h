/*
 * uart.h
 */

#pragma once

#include "../common/stdint.h"
#include "peripheral.h"

/* The base address for UART. */
#define UART0_BASE      (peripheral_base + UART0_OFFSET)

/* The offsets for reach register for the UART. */
#define UART0_DR        (UART0_BASE + 0x00)
#define UART0_RSRECR    (UART0_BASE + 0x04)
#define UART0_FR        (UART0_BASE + 0x18)
#define UART0_ILPR      (UART0_BASE + 0x20)
#define UART0_IBRD      (UART0_BASE + 0x24)
#define UART0_FBRD      (UART0_BASE + 0x28)
#define UART0_LCRH      (UART0_BASE + 0x2C)
#define UART0_CR        (UART0_BASE + 0x30)
#define UART0_IFLS      (UART0_BASE + 0x34)
#define UART0_IMSC      (UART0_BASE + 0x38)
#define UART0_RIS       (UART0_BASE + 0x3C)
#define UART0_MIS       (UART0_BASE + 0x40)
#define UART0_ICR       (UART0_BASE + 0x44)
#define UART0_DMACR     (UART0_BASE + 0x48)
#define UART0_ITCR      (UART0_BASE + 0x80)
#define UART0_ITIP      (UART0_BASE + 0x84)
#define UART0_ITOP      (UART0_BASE + 0x88)
#define UART0_TDR       (UART0_BASE + 0x8C)

typedef union uart_flags
{
    struct
    {
        uint8_t clear_to_send: 1;
        uint8_t data_set_ready: 1;
        uint8_t data_carrier_detected: 1;
        uint8_t busy: 1;
        uint8_t receive_queue_empty: 1;
        uint8_t transmit_queue_full: 1;
        uint8_t receive_queue_full: 1;
        uint8_t transmit_queue_empty: 1;
        uint8_t ring_indicator: 1;
        uint32_t padding: 23;
    };
    uint32_t as_int;
} uart_flags_t;

typedef union uart_control
{
    struct
    {
        uint8_t uart_enabled: 1;
        uint8_t sir_enabled: 1;
        uint8_t sir_low_power_mode: 1;
        uint8_t reserved: 4;
        uint8_t loop_back_enabled: 1;
        uint8_t transmit_enabled: 1;
        uint8_t receive_enabled: 1;
        uint8_t data_transmit_ready: 1;
        uint8_t request_to_send: 1;
        uint8_t out1: 1;
        uint8_t out2: 1;
        uint8_t rts_hardware_flow_control_enabled: 1;
        uint8_t cts_hardware_flow_control_enabled: 1;
        uint16_t padding;
    };
    uint32_t as_int;
} uart_control_t;

#define UART_CLOCK_FREQUENCY 250000000
#define INTEGER_BAUD_RATE(baud_rate) ((UART_CLOCK_FREQUENCY/(16*baud_rate)) / 64)
#define FRACTION_BAUD_RATE(baud_rate) ((UART_CLOCK_FREQUENCY/(16*baud_rate)) % 64)
#define BAUD_RATE 300

#define UART_FIFO_BUFFER_SIZE 1024
#define UART_RX_FIFO 0x3f0
#define UART_RX_FIFO_BUFFER 0x400

typedef struct {
    int start;
    int end;
    short buffer_full;
    short hw_fifo_full;
    unsigned char *buffer;
} uart_fifo_t;

void uart_init();
void uart_enable_interrupts();
void uart_putc(int c);
unsigned char uart_getc();
