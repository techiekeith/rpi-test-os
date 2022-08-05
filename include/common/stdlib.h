/*
 * stdlib.h
 */

#pragma once

#include <common/stddef.h>

typedef struct {
    int quot;
    int rem;
} div_t;

int abs(int value);
div_t div(int numerator, int denominator);
int quotient(int numerator, int denominator);
int remainder(int numerator, int denominator);

char *itoa(int num, int base);
int atoi(char *num);

void *malloc(size_t size);
void free(void *ptr);

#ifdef SOFTWARE_DIVISION
#define DIV(x,y) quotient(x, y)
#define MOD(x,y) remainder(x, y)
#else
#define DIV(x,y) x/y
#define MOD(x,y) x%y
#endif
