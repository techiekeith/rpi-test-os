/*
 * main.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/arm_timer.h"
#include "../../include/kernel/atag.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/kernel/uart.h"
#include "../../include/kernel/usb.h"

void copy_exception_table();
int shell();

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    /* Declare as unused */
    (void) r0;
    (void) r1;

    uart_init();

    // Get size of memory
    uint64_t mem_size = get_mem_size((atag_t *)atags);

//    puts("Initializing Memory Module.\n");
//    uint64_t mem_size = mem_init(mem_size);

    puts("Initializing interrupts\r\n");
    interrupts_init();
    uart_enable_interrupts();

    puts("Initializing system timer\r\n");
    system_timer_init();

    puts("Initializing ARM timer\r\n");
    arm_timer_init();

    puts("Initializing kernel heap\r\n");
    heap_init();

    puts("Initializing graphics\r\n");
    graphics_init();

    puts("Initializing USB host controller\r\n");
    usb_init();

    int halted = 0;
    while (!halted)
    {
        puts("Starting shell.\r\n");
        set_output_channel(OUTPUT_CHANNEL_GRAPHICS);
        printf("\f\r\nRaspberry Pi %ldK\r\n\r\n", mem_size >> 10);
        halted = shell();
        set_output_channel(OUTPUT_CHANNEL_UART);
        puts("Shell stopped.\r\n");
    }

    puts("System halted.\r\n");
}
