/*
 * string.h
 */

#pragma once

#include "stddef.h"

void memcpy(void *dest, const void *src, size_t bytes);
void memmove(void *dest, const void *src, size_t bytes);
void memset(void *dest, int value, size_t bytes);

size_t strlen(const char *s);
char *strchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
