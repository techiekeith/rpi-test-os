/*
 * io.h
 */

#pragma once

#include "../common/stdarg.h"
#include "../common/stdbool.h"

#define OUTPUT_CHANNEL_UART     0
#define OUTPUT_CHANNEL_GRAPHICS 1

int get_input_channel();
int get_output_channel();
void set_input_channel(int channel);
void set_output_channel(int channel);
void debug_push(const char *filename, const char *function);
void debug_vprintf(const char *fmt, va_list args);
void debug_printf(const char *fmt, ...);
void debug_writeline();
void debug_pop(bool correctly);
void debug_hexdump(void *start, void *end);
void debug_stacktrace();

extern int debug_counter;
#define DEBUG_INIT(__filename__) static char *__debug_filename = __filename__
#define DEBUG_START(__function__) \
    debug_push(__debug_filename, __function__); \
    int __debug_counter = debug_counter++
#define DEBUG_END() \
    while (__debug_counter != --debug_counter) debug_pop(false); \
    debug_pop(true);
