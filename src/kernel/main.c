/*
 * kernel.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <kernel/atag.h>
#include <kernel/gpu.h>
#include <kernel/mem.h>
#include <kernel/uart.h>

void shell(uint64_t mem_size);

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    /* Declare as unused */
    (void) r0;
    (void) r1;

    uart_init();

    puts("Initializing Memory Module.\n");
    uint64_t mem_size = mem_init((atag_t *)atags);

    puts("Initializing GPU.\n");
    gpu_init();

    puts("System started.\r\n");

    while (1)
    {
        set_output_channel(OUTPUT_CHANNEL_GPU);
        shell(mem_size);
        set_output_channel(OUTPUT_CHANNEL_UART);
        puts("Shell reset.\n");
    }
}
