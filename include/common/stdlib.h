/*
 * stdlib.h
 */

#pragma once

#include "stddef.h"

char *ultoa(unsigned long long number, int radix, char *buffer);
char *ltoa(long long number, int radix, char *buffer);
char *uitoa(unsigned int number, int radix, char *buffer);
char *itoa(int number, int radix, char *buffer);

long long strtoll(const char *nptr, char **endptr, int base);
long strtol(const char *nptr, char **endptr, int base);
int atoi(const char *nptr);

int abs(int j);
long labs(long j);
long long llabs(long long j);

void *malloc(size_t size);
void free(void *ptr);

/* Normally these would live in sys/param.h */
#define MIN(a, b, __type)   ({ __type compare1 = (a); __type compare2 = (b); compare1 < compare2 ? compare1 : compare2; })
#define MAX(a, b, __type)   ({ __type compare1 = (a); __type compare2 = (b); compare1 > compare2 ? compare1 : compare2; })
