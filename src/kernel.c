#include <stdint.h>
#include "uart.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    /* Declare as unused */
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    uart_puts("Hello, kernel World!\r\n");

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
}
