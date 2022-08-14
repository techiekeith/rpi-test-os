/*
 * uart.c - routines for Mini-UART (on GPIO)
 */

#include "../../include/common/stddef.h"
#include "../../include/common/string.h"
#include "../../include/common/utf8.h"
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/gpio.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/mmio.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/kernel/uart.h"

static uart_control_t control;
static uart_fifo_t *uart_rx_fifo;

static void uart_read()
{
    volatile uart_flags_t flags;
    flags.as_int = mmio_read(UART0_FR);
    if (flags.receive_queue_full) {
        uart_rx_fifo->hw_fifo_full++;
    }
    while (!flags.receive_queue_empty)
    {
        if (uart_rx_fifo->buffer_full) break;
        uart_rx_fifo->buffer[uart_rx_fifo->end++] = mmio_read(UART0_DR);
        uart_rx_fifo->end %= UART_FIFO_BUFFER_SIZE;
        if (uart_rx_fifo->end == uart_rx_fifo->start) uart_rx_fifo->buffer_full = 1;
        flags.as_int = mmio_read(UART0_FR);
    }
}

static void uart_interrupt_handler()
{
}

static void uart_interrupt_clearer()
{
    __dmb();
    mmio_write(UART0_ICR, (1 << 4)); // Clear interrupt
    uart_read();
    __dmb();
}

/*
 * Set integer & fractional part of baud rate.
 * Divider = UART_CLOCK/(16 * Baud)
 * Fraction part register = (Fractional part * 64) + 0.5
 * UART_CLOCK = 3000000.
 */
void uart_set_baud_rate(int baud_rate)
{
    mmio_write(UART0_IBRD, INTEGER_BAUD_RATE(baud_rate));
    mmio_write(UART0_FBRD, FRACTION_BAUD_RATE(baud_rate));
}

void uart_init() {
    /* Disable UART0. */
    __dmb();
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

    /* Set baud rate. */
    mmio_write(UART0_IBRD, INTEGER_BAUD_RATE(BAUD_RATE));
    mmio_write(UART0_FBRD, FRACTION_BAUD_RATE(BAUD_RATE));

    /* Enable FIFO & 8 bit data transmission (1 stop bit, no parity). */
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    /* Mask all interrupts. */
    mmio_write(UART0_IMSC, 0);

    /* Enable UART0, receive & transfer part of UART. */
    control.uart_enabled = 1;
    control.transmit_enabled = 1;
    control.receive_enabled = 1;
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

    /* Initialize RX FIFO. */
    uart_rx_fifo = (uart_fifo_t *)UART_RX_FIFO;
    memset(uart_rx_fifo, 0, sizeof(uart_fifo_t));
    uart_rx_fifo->buffer = (unsigned char *)UART_RX_FIFO_BUFFER;
    memset(uart_rx_fifo->buffer, 0, UART_FIFO_BUFFER_SIZE);
}

void uart_enable_interrupts()
{
    /* Register UART interrupt handler. */
    register_irq_handler(UART_INT, uart_interrupt_handler, uart_interrupt_clearer);

    /* Set interrupt FIFO level. */
    mmio_write(UART0_IFLS, 0);

    /* Unmask RX DATA interrupts. */
    mmio_write(UART0_IMSC, (1 << 4));
}

void uart_putc(int c)
{
    volatile uart_flags_t flags;
    /* Wait for UART to become ready to transmit. */
    do {
        flags.as_int = mmio_read(UART0_FR);
    }
    while (flags.transmit_queue_full);

    char buffer[5];
    utf8_encode(c, buffer);
    __dmb();
    for (char *p = buffer; *p; p++) {
        mmio_write(UART0_DR, *p);
    }
}

unsigned char uart_getc()
{
    unsigned char rx_char;
    while (1)
    {
        if (uart_rx_fifo->start != uart_rx_fifo->end)
        {
            rx_char = uart_rx_fifo->buffer[uart_rx_fifo->start++];
            uart_rx_fifo->buffer_full = 0;
            uart_rx_fifo->start %= UART_FIFO_BUFFER_SIZE;
            break;
        }
        asm("wfi");
    }
    return rx_char;
}
