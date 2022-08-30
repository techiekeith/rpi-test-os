/*
 * io.c
 */

#include "../../include/common/stdarg.h"
#include "../../include/common/stdio.h"
#include "../../include/common/string.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/heap.h"

static int input_channel, output_channel;
int debug_counter;
#define DEBUG_ENTRY_SIZE 64
typedef struct debug_entry {
    char filename[DEBUG_ENTRY_SIZE];
    char function[DEBUG_ENTRY_SIZE];
    struct debug_entry *parent;
} debug_entry_t;
static debug_entry_t *debug_stack = NULL;

int get_input_channel()
{
    return input_channel;
}

int get_output_channel()
{
    return output_channel;
}

void set_input_channel(int channel)
{
    input_channel = channel;
}

void set_output_channel(int channel)
{
    output_channel = channel;
}

void debug_push(const char *filename, const char *function)
{
    struct debug_entry *new_entry = heap_alloc("kernel_debug", sizeof(struct debug_entry));
    if (new_entry != NULL)
    {
        strncpy(new_entry->filename, filename, DEBUG_ENTRY_SIZE - 1);
        strncpy(new_entry->function, function, DEBUG_ENTRY_SIZE - 1);
        new_entry->parent = debug_stack;
        debug_stack = new_entry;
    }
}

void debug_pop(bool correctly)
{
    if (!correctly) debug_printf("exited without calling DEBUG_END().\r\n");
    if (debug_stack != NULL)
    {
        struct debug_entry *old_entry = debug_stack;
        debug_stack = old_entry->parent;
        heap_free(old_entry);
    }
}

void debug_vprintf(const char *fmt, va_list args)
{
    int channel = output_channel;
    if (output_channel != OUTPUT_CHANNEL_UART)
    {
        output_channel = OUTPUT_CHANNEL_UART;
    }
    if (debug_stack != NULL)
    {
        puts(debug_stack->filename);
        puts("::");
        puts(debug_stack->function);
        puts(" .. ");
    }
    vprintf(fmt, args);
    if (channel != OUTPUT_CHANNEL_UART)
    {
        output_channel = channel;
    }
}

void debug_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    debug_vprintf(fmt, args);
    va_end(args);
}

void debug_writeline()
{
    int channel = output_channel;
    if (output_channel != OUTPUT_CHANNEL_UART)
    {
        output_channel = OUTPUT_CHANNEL_UART;
    }
    puts("\r\n");
    if (channel != OUTPUT_CHANNEL_UART)
    {
        output_channel = channel;
    }
}

void debug_hexdump(void *start, void *end)
{
    void *p = start;
    int i = 0;
    while (p < end) {
        if (!i) {
            printf("\r\n%p : ", p);
        }
        printf("%02x ", *((uint8_t *)p));
        i++;
        i %= 16;
        p++;
    }
    puts("\r\n");
}

void *__get_stack_pointer();
void _start();

void debug_stacktrace()
{
    debug_hexdump(__get_stack_pointer(), (void *)_start);
}
