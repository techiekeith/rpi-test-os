/*
 * string.h
 */

#pragma once

#include <common/stddef.h>

void memcpy(void *dest, const void *src, size_t bytes);
void memmove(void *dest, const void *src, size_t bytes);
void memset(void *dest, int value, size_t bytes);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
