/*
 * memory.c
 */

#include "../../include/common/stdbool.h"
#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/dma.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/mailbox.h"
#include "../../include/kernel/mem.h"
#include "../../include/kernel/peripheral.h"

extern const char *divider;

// To get size of kernel
void *__get_stack_pointer();
void _start();
extern uint8_t __end;
extern page_t *all_pages_array;
extern heap_segment_t *heap_segment_list_head;

static void show_kernel_heap_usage()
{
    char buffer[31];
    char buffer2[16];
    uint8_t *heap_start = (uint8_t *)heap_segment_list_head;
    printf(" %30s | %08x | %08x\r\n", "Kernel heap", heap_start, heap_start + KERNEL_HEAP_SIZE - 1);
    uint8_t *segment_start;
    uint8_t *segment_end;
    heap_segment_t *next_segment;
    for (heap_segment_t *segment = heap_segment_list_head; segment != NULL; segment = next_segment)
    {
        if ((uint8_t *)segment >= heap_start + KERNEL_HEAP_SIZE)
        {
            printf("segment->next should be NULL, but is not!\r\n");
            break;
        }
        else
        {
            segment_start = (uint8_t *)segment;
            segment_end = segment_start + segment->segment_size;
            next_segment = segment->next;
            if (segment->is_allocated)
            {
                int segment_count = 1;
                while (next_segment != NULL
                    && next_segment->is_allocated
                    && !strcmp(next_segment->consumer, segment->consumer))
                {
                    segment_end += next_segment->segment_size;
                    next_segment = next_segment->next;
                    segment_count++;
                }
                strcpy(buffer, segment->consumer);
                if (segment_count != 1)
                {
                    // We could use a sprintf() function here
                    strcpy(buffer2, " <");
                    itoa(segment_count, 10, buffer2 + 2);
                    strcat(buffer2, ">");
                    int idx = strlen(buffer);
                    int idx2 = strlen(buffer2);
                    if (idx + idx2 > 30) idx = 30 - idx2;
                    strcpy(buffer + idx, buffer2);
                }
                set_foreground_color(235); // yellow = allocated
            }
            else
            {
                strcpy(buffer, "Free kernel heap space");
                set_foreground_color(35); // green = free
            }
            set_foreground_color(segment->is_allocated ? 235 : 35); // yellow = allocated, green = free
            printf(" %30s | %08x | %08x\r\n", buffer, segment_start, segment_end - 1);
        }
    }
    set_foreground_color(239); // White
}

static void show_arm_memory_usage(void *arm_memory_start, void *arm_memory_end)
{
    (void) arm_memory_start; // XXX This is assumed to be zero, for now
    uint8_t *sp = (uint8_t *)__get_stack_pointer();
    uint8_t *heap_start = (uint8_t *)heap_segment_list_head;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    uint8_t *last = &__end - 1;
#pragma GCC diagnostic pop
    printf(" %30s | %08x | %08x\r\n", "Exception Table", 0, 63);
    printf(" %30s | %08x | %08x\r\n", "Reserved", 64, 0x37f);
    printf(" %30s | %08x | %08x\r\n", "FIFO queues", 0x380, 0xbff);
    printf(" %30s | %08x | %08x\r\n", "Reserved", 0xc00, 0xfff);
    printf(" %30s | %08x | %08x\r\n", "DMA control blocks", 0x1000, 0x1fff);
    printf(" %30s | %08x | %08x\r\n", "Reserved", 0x2000, sp - 1);
    printf(" %30s | %08x | %08x\r\n", "Stack", sp, &_start - 1);
    printf(" %30s | %08x | %08x\r\n", "Kernel", &_start, last);
    if (all_pages_array)
    {
        printf(" %30s | %08x | %08x\r\n", "Page Table", all_pages_array, heap_start - 1);
    }
    show_kernel_heap_usage();
    printf(" %30s | %08x | %08x\r\n", "Unallocated RAM", heap_start + KERNEL_HEAP_SIZE, arm_memory_end);
}

static void show_videocore_memory_usage(void *vc_memory_start, void *vc_memory_end)
{
    (void) vc_memory_end;
    uint8_t *framebuffer = (uint8_t *)fbinfo.buf;
    if (framebuffer > (uint8_t *)vc_memory_start)
    {
        set_foreground_color(200); // Red
        printf(" %30s | %08x | %08x\r\n", "VideoCore RAM", vc_memory_start, framebuffer - 1);
    }
    if (framebuffer < (uint8_t *)peripheral_base)
    {
        set_foreground_color(220); // Orange
        printf(" %30s | %08x | %08x\r\n", "Framebuffer", framebuffer, framebuffer + fbinfo.buf_size - 1);
    }
    if (framebuffer + fbinfo.buf_size < (uint8_t *)peripheral_base)
    {
        set_foreground_color(200); // Red
        printf(" %30s | %08x | %08x\r\n", "VideoCore RAM", framebuffer + fbinfo.buf_size, peripheral_base - 1);
    }
    set_foreground_color(175); // Dark orange
    printf(" %30s | %08x | %08x\r\n", "Peripherals", peripheral_base, peripheral_base + PERIPHERAL_LENGTH - 1);
    printf(" %30s | %08x |\r\n", "(System timer)", peripheral_base + SYSTEM_TIMER_OFFSET);
    printf(" %30s | %08x |\r\n", "(Interrupts)", peripheral_base + INTERRUPTS_OFFSET);
    printf(" %30s | %08x |\r\n", "(ARM timer)", peripheral_base + ARM_TIMER_OFFSET);
    printf(" %30s | %08x |\r\n", "(Mailbox)", peripheral_base + MAILBOX_OFFSET);
    printf(" %30s | %08x |\r\n", "(GPIO)", peripheral_base + GPIO_OFFSET);
    printf(" %30s | %08x |\r\n", "(UART)", peripheral_base + UART0_OFFSET);
    printf(" %30s | %08x |\r\n", "(USB host controller)", peripheral_base + HOST_CONTROLLER_OFFSET);
    if ((uint8_t *)(peripheral_base + PERIPHERAL_LENGTH) < framebuffer)
    {
        set_foreground_color(200); // Red
        printf(" %30s | %08x | %08x\r\n", "VideoCore RAM", framebuffer + fbinfo.buf_size, peripheral_base - 1);
    }
    if (framebuffer > (uint8_t *)peripheral_base)
    {
        set_foreground_color(220); // Orange
        printf(" %30s | %08x | %08x\r\n", "Framebuffer", framebuffer, framebuffer + fbinfo.buf_size - 1);
    }
    set_foreground_color(239); // White
}

void show_memory_usage(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    property_message_tag_t tags[3];
    tags[0].proptag = HW_GET_ARM_MEMORY;
    tags[1].proptag = HW_GET_VIDEOCORE_MEMORY;
    tags[2].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv < 0)
    {
        printf("\r\nMemory size read error\r\n");
        return;
    }
    puts("\r\n");
    printf(" %30s | %-8s | %-8s\r\n", "Memory Usage", "Start", "End");
    printf("%.32s+%.10s+%.10s\r\n", divider, divider, divider);
    void *arm_memory_start = tags[0].value_buffer.memory_block.base_address;
    void *arm_memory_end = arm_memory_start + tags[0].value_buffer.memory_block.size - 1;
    set_foreground_color(219); // Pale green
    printf(" %30s | %08x | %08x\r\n", "[ARM Memory]", arm_memory_start, arm_memory_end);
    set_foreground_color(239); // White
    show_arm_memory_usage(arm_memory_start, arm_memory_end);
    printf("%.32s+%.10s+%.10s\r\n", divider, divider, divider);
    void *vc_memory_start = tags[1].value_buffer.memory_block.base_address;
    void *vc_memory_end = vc_memory_start + tags[1].value_buffer.memory_block.size - 1;
    set_foreground_color(219); // Pale green
    printf(" %30s | %08x | %08x\r\n", "[VideoCore Memory]", vc_memory_start, vc_memory_end);
    set_foreground_color(239); // White
    show_videocore_memory_usage(vc_memory_start, vc_memory_end);
}

static void memory_dump(size_t start, size_t end)
{
    char text[129];
    int i = 0;
    int columns = 8;
    if (fbinfo.columns >= 75) columns = 16;
    if (fbinfo.columns >= 139) columns = 32;
    text[columns] = '\0';
    size_t real_start = start - (start & 3);
    for (size_t p = real_start; p < end; p += 4) {
        size_t word = *((size_t *)p);
        for (int j = 0; j < 4; j++)
        {
            if (p + j >= end) break;
            uint8_t c = word & 0xff;
            word >>= 8;
            if (p + j < start) continue;
            i %= columns;
            if (!i) {
                if (p != start) printf(" |%s|", text);
                printf("\r\n%08x  ", p);
            }
            printf("%02x ", c);
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
    memory_dump((size_t) __get_stack_pointer(), (size_t) _start);
}

static void dump_syntax()
{
    puts("\r\nSyntax: dump (stack|<start_addr>) (<end_addr>|+<size>)\r\n");
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

static void peek_syntax()
{
    puts("\r\nSyntax: peek[32] <addr>\r\n");
}

static void peek_memory(int argc, char **argv, bool word)
{
    if (argc < 2)
    {
        peek_syntax();
        return;
    }
    size_t address = (size_t) strtol(argv[1], NULL, 0);
    if (word)
    {
        uint32_t value = *((uint32_t *)address);
        printf("Word @ %p: %d / 0x%08x\r\n", address, value, value);
    }
    else
    {
        uint8_t value = *((uint8_t *)address);
        printf("Byte @ %p: %d / 0x%02x / 0b%08b\r\n", address, value, value, value);
    }
}

void peek_memory_8bit(int argc, char **argv)
{
    peek_memory(argc, argv, false);
}

void peek_memory_32bit(int argc, char **argv)
{
    peek_memory(argc, argv, true);
}

static void poke_syntax()
{
    puts("\r\nSyntax: poke[32] <addr> <value>\r\n");
}

static void poke_memory(int argc, char **argv, bool word)
{
    if (argc < 3)
    {
        poke_syntax();
        return;
    }
    size_t address = (size_t) strtol(argv[1], NULL, 0);
    int value = strtol(argv[2], NULL, 0);
    if (word)
    {
        *((uint32_t *)address) = value;
    }
    else
    {
        *((uint8_t *)address) = value;
    }
}

void poke_memory_8bit(int argc, char **argv)
{
    poke_memory(argc, argv, false);
}

void poke_memory_32bit(int argc, char **argv)
{
    poke_memory(argc, argv, true);
}

static void dma_copy_syntax()
{
    puts("\r\nSyntax: dmacopy <src> <dest> <size>\r\n");
}

void dma_copy_bytes(int argc, char **argv)
{
    if (argc < 4)
    {
        dma_copy_syntax();
        return;
    }
    size_t src = (size_t) strtol(argv[1], NULL, 0);
    if (!src)
    {
        dma_copy_syntax();
        return;
    }
    size_t dst = (size_t) strtol(argv[2], NULL, 0);
    if (!dst)
    {
        dma_copy_syntax();
        return;
    }
    size_t bytes = (size_t) strtol(argv[3], NULL, 0);
    if (!bytes)
    {
        dma_copy_syntax();
        return;
    }
    dma_copy((void *)dst, (void *)src, bytes);
}
