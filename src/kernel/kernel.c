/*
 * kernel.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <kernel/uart.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    char buf[256];
    /* Declare as unused */
    (void) r0;
    (void) r1;

    uart_init();

    if (atags)
    {
        puts("Initializing Memory Module\n");
        mem_init((atag_t *)atags);
    }

    puts("Hello, kernel World!\r\n");

    while (1)
    {
        gets(buf, 256);
        puts(buf);
        putc('\n');
    }
}