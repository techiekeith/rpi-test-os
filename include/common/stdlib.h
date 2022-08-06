/*
 * stdlib.h
 */

#pragma once

#include <common/stddef.h>

char *ultoa(unsigned long long number, int radix);
char *ltoa(long long number, int radix);
char *uitoa(unsigned int number, int radix);
char *itoa(int number, int radix);

int atoi(char *num);

void *malloc(size_t size);
void free(void *ptr);
