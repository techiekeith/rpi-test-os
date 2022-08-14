/*
 * stdio.h
 */

#pragma once

#include "stdarg.h"

char getc();
void putc(int c);
void puts(const char *s);

void vprintf(const char *fmt, va_list args);
void printf(const char *fmt, ...);
