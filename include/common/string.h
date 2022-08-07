/*
 * string.h
 */

#pragma once

#include <common/stddef.h>

void memcpy(void *dest, const void *src, size_t bytes);
void memmove(void *dest, const void *src, size_t bytes);
void memset(void *dest, int value, size_t bytes);
