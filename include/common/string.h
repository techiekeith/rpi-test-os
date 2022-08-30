/*
 * string.h
 */

#pragma once

#include "stddef.h"

int memcmp(const void *s1, const void *s2, size_t n);
void memcpy(void *dest, const void *src, size_t bytes);
void memmove(void *dest, const void *src, size_t bytes);
void memset(void *dest, int value, size_t bytes);

char *strcat(char *dest, const char *src);
char *strchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
size_t strlen(const char *s);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
