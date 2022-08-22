/*
 * main.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/arm_timer.h"
#include "../../include/kernel/board.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/kernel/uart.h"

int shell();
#ifdef CSUD
#ifdef CSUD_PORT
#include "../../include/kernel/usb/usb.h"
#else
void UsbInitialise();
#endif
#endif

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t board_id)
{
    // Discover the peripheral base address
    discover_peripheral_base(board_id);

    // Initialise the UART - must be done before we can print anything
    uart_init();
    printf("Board ID is %p.\r\n", board_id);
    printf("Peripheral base address is at %p.\r\n", peripheral_base);

    // Initialise the heap - must be done before we can call a lot of things
    puts("Initializing kernel heap.\r\n");
    heap_init();

    // Discover RAM capacity
    size_t mem_size = get_mem_size();
    printf("Memory available: %dK.\r\n", mem_size >> 10);

//    puts("Initializing Memory Module.\n");
//    uint64_t mem_size = mem_init(mem_size);

    puts("Initializing interrupts.\r\n");
    interrupts_init();
    uart_enable_interrupts();

    puts("Initializing timers.\r\n");
    system_timer_init();
    arm_timer_init();

    puts("Initializing graphics.\r\n");
    graphics_init();

#ifdef CSUD
#ifdef CSUD_PORT
    puts("Initializing USB host controller (CSUD port).\r\n");
    usb_init();
#else
    puts("Initializing USB host controller (CSUD).\r\n");
    UsbInitialise();
#endif
#endif

    int halted = 0;
    while (!halted)
    {
        puts("Starting shell.\r\n");
//        set_output_channel(OUTPUT_CHANNEL_GRAPHICS);
#if (__WORD_SIZE == 64)
        printf("\f\r\nRaspberry Pi %ldK\r\n\r\n", mem_size >> 10);
#else
        printf("\f\r\nRaspberry Pi %dK\r\n\r\n", mem_size >> 10);
#endif
        halted = shell();
//        set_output_channel(OUTPUT_CHANNEL_UART);
        puts("Shell stopped.\r\n");
    }

    puts("System halted.\r\n");
}
