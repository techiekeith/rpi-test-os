/*
 * stdlib.h
 */

#pragma once

#include <common/stddef.h>

int abs(int j);
long labs(long j);
long long llabs(long long j);

char *ultoa(unsigned long long number, int radix, char *buffer);
char *ltoa(long long number, int radix, char *buffer);
char *uitoa(unsigned int number, int radix, char *buffer);
char *itoa(int number, int radix, char *buffer);

int atoi(char *num);

void *malloc(size_t size);
void free(void *ptr);
