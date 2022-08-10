/*
 * uart.c - routines for Mini-UART (on GPIO)
 */

#include <common/stddef.h>
#include <common/stdlib.h>
#include <common/string.h>
#include <common/utf8.h>
#include <kernel/delay.h>
#include <kernel/gpio.h>
#include <kernel/mmio.h>
#include <kernel/uart.h>

void uart_init()
{
    uart_control_t control;

    /* Disable UART0. */
    memset(&control, 0, 4);
    mmio_write(UART0_CR, 0x00000000);
    /* Setup the GPIO pin 14 && 15. */

    /* Disable pull up/down for all GPIO pins & delay for 150 cycles. */
    mmio_write(GPPUD, 0x00000000);
    delay(150);

    /* Disable pull up/down for pin 14,15 & delay for 150 cycles. */
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    /* Write 0 to GPPUDCLK0 to make it take effect. */
    mmio_write(GPPUDCLK0, 0x00000000);

    /* Clear pending interrupts. */
    mmio_write(UART0_ICR, 0x7FF);

    /*
     * Set integer & fractional part of baud rate.
     * Divider = UART_CLOCK/(16 * Baud)
     * Fraction part register = (Fractional part * 64) + 0.5
     * UART_CLOCK = 3000000; Baud = 115200.
     */

    /* Divider = 3000000 / (16 * 115200) = 1.627 = ~1. */
    mmio_write(UART0_IBRD, 1);
    /* Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40. */
    mmio_write(UART0_FBRD, 40);

    /* Enable FIFO & 8 bit data transmissio (1 stop bit, no parity). */
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    /* Mask all interrupts. */
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    /* Enable UART0, receive & transfer part of UART. */
    control.uart_enabled = 1;
    control.transmit_enabled = 1;
    control.receive_enabled = 1;
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

uart_flags_t read_flags()
{
    uart_flags_t flags;
    flags.as_int = mmio_read(UART0_FR);
    return flags;
}

void uart_putc(int c)
{
    uart_flags_t flags;
    /* Wait for UART to become ready to transmit. */
    do {
        flags = read_flags();
    }
    while (flags.transmit_queue_full);

    char buffer[5];
    utf8_encode(c, buffer);
    for (char *p = buffer; *p; p++) {
        mmio_write(UART0_DR, *p);
    }
}

unsigned char uart_getc()
{
    uart_flags_t flags;
    /* Wait for UART to have received something. */
    do {
        flags = read_flags();
    }
    while (flags.receive_queue_empty);
    return mmio_read(UART0_DR);
}

void uart_puts(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i ++)
        uart_putc((unsigned char)str[i]);
}

