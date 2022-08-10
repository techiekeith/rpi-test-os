/*
 * stdio.h
 */

#pragma once

#include <common/stdarg.h>

char getc();
void putc(int c);
void puts(const char *s);

void vprintf(const char *fmt, va_list args);
void printf(const char *fmt, ...);
void debug_printf(const char * fmt, ...);
