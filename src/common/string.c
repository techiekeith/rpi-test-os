/*
 * string.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/io.h"

void memset(void *dest, int value, size_t bytes)
{
    for (size_t i = 0; i < bytes; i++)
    {
        ((uint8_t *)dest)[i] = value;
    }
}

static void memcpy_rev(void *dest, const void *src, size_t bytes)
{
    size_t i = bytes;
    do {
        i--;
        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    } while (i != 0);
}

static void memcpy_fwd(void *dest, const void *src, size_t bytes)
{
    for (size_t i = 0; i < bytes; i++)
    {
        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    }
}

void memcpy(void *dest, const void *src, size_t bytes)
{
    if (!bytes || src == dest) return;
    if (dest > src)
        memcpy_rev(dest, src, bytes);
    if (src > dest)
        memcpy_fwd(dest, src, bytes);
}

void memmove(void *dest, const void *src, size_t bytes)
{
    memcpy(dest, src, bytes);
}

size_t strlen(const char *s)
{
    char *p = (char *)s;
    while (*p++);
    p--;
    return (size_t)(p - s);
}

char *strchr(const char *s, int c)
{
    char *p = (char *)s;
    while (*p && *p != c) p++;
    return *p ? p : NULL;
}

int strcmp(const char *s1, const char *s2)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    while ((*p1 | *p2) && (*p1 == *p2))
    {
        p1++;
        p2++;
    }
    return *p1 - *p2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    int i = n;
    while (i > 0 && (*p1 | *p2) && (*p1 == *p2))
    {
        i--;
        p1++;
        p2++;
    }
    return i && *p1 - *p2;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    for (; i < n; i++)
    {
        dest[i] = '\0';
    }
    return dest;
}
