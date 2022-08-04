/*
 * stdio.h
 */

#pragma once

#define OUTPUT_CHANNEL_UART     0
#define OUTPUT_CHANNEL_GPU      1

void set_input_channel(int channel);
void set_output_channel(int channel);

char getc(void);
void putc(char c);

void puts(const char *s);

/* This version of gets copies until newline, replacing newline with null char, or until buflen, whichever comes first */
void gets(char *buf, int buflen);

void printf(const char *fmt, ...);
