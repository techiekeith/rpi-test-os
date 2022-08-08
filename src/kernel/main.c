/*
 * kernel.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <kernel/atag.h>
#include <kernel/graphics.h>
#include <kernel/io.h>
#include <kernel/mem.h>
#include <kernel/uart.h>

int shell(uint64_t mem_size);

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

    puts("Initializing Graphics.\n");
    graphics_init();

    int halted = 0;
    while (!halted)
    {
        puts("Starting shell.\n");
        set_output_channel(OUTPUT_CHANNEL_GRAPHICS);
        halted = shell(mem_size);
        set_output_channel(OUTPUT_CHANNEL_UART);
        puts("Shell stopped.\n");
    }

    puts("System halted.\n");
//    __asm("\ncmd_halt_loop:\n\twfe\n\tb cmd_halt_loop");
}
