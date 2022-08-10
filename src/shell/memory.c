/*
 * memory.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/mem.h"
#include "../../include/kernel/peripheral.h"

extern const char *divider;

// To get size of kernel
uint32_t *__get_stack_pointer();
void _start();
extern uint8_t __end;
extern page_t *all_pages_array;
extern heap_segment_t *heap_segment_list_head;

void show_memory_usage()
{
    uint8_t *sp = (uint8_t *)__get_stack_pointer();
    uint8_t *heap_start = (uint8_t *)heap_segment_list_head;
    uint8_t *framebuffer = (uint8_t *)fbinfo.buf;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    uint8_t *last = &__end - 1;
#pragma GCC diagnostic pop
    puts("\r\n");
    printf(" %30s | %-8s | %-8s\r\n", "Memory Usage", "Start", "End");
    printf("%.32s+%.10s+%.10s\r\n", divider, divider, divider);
    printf(" %30s | %08x | %08x\r\n", "Exception Table", 0, 63);
    printf(" %30s | %08x | %08x\r\n", "Reserved", 64, sp - 1);
    printf(" %30s | %08x | %08x\r\n", "Stack", sp, &_start - 1);
    printf(" %30s | %08x | %08x\r\n", "Kernel", &_start, last);
    printf(" %30s | %08x | %08x\r\n", "Kernel heap", heap_start, heap_start + KERNEL_HEAP_SIZE - 1);
    uint8_t *segment_start = heap_start;
    uint8_t *segment_end;
    for (heap_segment_t *segment = heap_segment_list_head; segment; segment = segment->next)
    {
        segment_end = segment_start + segment->segment_size;
        set_foreground_color(segment->is_allocated ? 235 : 35); // yellow = allocated, green = free
        printf(" %30s | %08x | %08x\r\n",
               segment->is_allocated ? segment->consumer : "Free kernel heap space",
               segment_start, segment_end - 1);
        segment_start = segment_end;
    }
    set_foreground_color(239); // White
    if (all_pages_array)
    {
        printf(" %30s | %08x | %08x\r\n", "Page Table", all_pages_array, heap_start - 1);
    }
    if (framebuffer < (uint8_t *)PERIPHERAL_BASE)
    {
        set_foreground_color(220); // Orange
        printf(" %30s | %08x | %08x\r\n", "Framebuffer", framebuffer, framebuffer + fbinfo.buf_size - 1);
    }
    set_foreground_color(200); // Red
    if (framebuffer + fbinfo.buf_size < (uint8_t *)PERIPHERAL_BASE)
    {
        printf(" %30s | %08x | %08x\r\n", "VideoCore RAM", framebuffer + fbinfo.buf_size, PERIPHERAL_BASE - 1);
    }
    printf(" %30s | %08x | %08x\r\n", "Peripherals", PERIPHERAL_BASE, PERIPHERAL_BASE + PERIPHERAL_LENGTH - 1);
    printf(" %30s | %08x |\r\n", "(interrupts)", PERIPHERAL_BASE + INTERRUPTS_OFFSET);
    printf(" %30s | %08x |\r\n", "(mailbox)", PERIPHERAL_BASE + MAILBOX_OFFSET);
    printf(" %30s | %08x |\r\n", "(GPIO)", PERIPHERAL_BASE + GPIO_OFFSET);
    printf(" %30s | %08x |\r\n", "(UART)", PERIPHERAL_BASE + UART0_OFFSET);
    if ((uint8_t *)(PERIPHERAL_BASE + PERIPHERAL_LENGTH) < framebuffer)
    {
        printf(" %30s | %08x | %08x\r\n", "VideoCore RAM", framebuffer + fbinfo.buf_size, PERIPHERAL_BASE - 1);
    }
    if (framebuffer > (uint8_t *)PERIPHERAL_BASE)
    {
        set_foreground_color(220); // Orange
        printf(" %30s | %08x | %08x\r\n", "Framebuffer", framebuffer, framebuffer + fbinfo.buf_size - 1);
    }
    set_foreground_color(239); // White
}

static void memory_dump(uint32_t start, uint32_t end)
{
    char text[129];
    int i = 0;
    int columns = 8;
    if (fbinfo.columns >= 75) columns = 16;
    if (fbinfo.columns >= 139) columns = 32;
    text[columns] = '\0';
    uint32_t real_start = start - (start & 3);
    for (uint32_t p = real_start; p < end; p += 4) {
        uint32_t word = *((uint32_t *)p);
        for (int j = 0; j < 4; j++)
        {
            if (p + j >= end) break;
            uint8_t c = word & 0xff;
            word >>= 8;
            if (p + j < start) continue;
            i %= columns;
            if (!i) {
                if (p != start) printf(" |%s|", text);
                printf("\r\n%08ux  ", p);
            }
            printf("%02ux ", c);
            text[i++] = ((c >= 0x20) && (c < 0x7f)) ? c : '.';
        }
    }
    while (i != columns)
    {
        puts("   ");
        text[i++] = '\0';
    }
    printf(" |%s|\r\n", text);
}

void stack_dump()
{
    memory_dump((uint32_t) __get_stack_pointer(), (uint32_t) _start);
}

static void dump_syntax()
{
    puts("\r\nSyntax: (stack|<start_addr>) (<end_addr>|+<size>)\r\n");
}

void show_dump(int argc, char **argv)
{
    if (argc < 2)
    {
        dump_syntax();
        return;
    }
    if (!strcmp(argv[1], "stack"))
    {
        stack_dump();
        return;
    }
    if (argc != 3)
    {
        dump_syntax();
        return;
    }
    long start = strtol(argv[1], NULL, 0);
    long end;
    if (argv[2][0] == '+')
    {
        end = start + strtol(argv[2] + 1, NULL, 0);
    }
    else
    {
        end = strtol(argv[2], NULL, 0);
    }
    memory_dump(start, end);
}