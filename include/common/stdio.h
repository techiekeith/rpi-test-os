/*
 * stdio.h
 */

#pragma once

#include <common/stdarg.h>

char getc();
void putc(int c);

void puts(const char *s);

/* This version of gets copies until newline, replacing newline with null char, or until buflen, whichever comes first */
void gets(char *buf, int buflen);

void vprintf(const char *fmt, va_list args);
void printf(const char *fmt, ...);
void debug_printf(const char * fmt, ...);
