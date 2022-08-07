/*
 * string.c
 */

#include <common/stdlib.h>
#include <common/string.h>

void memcpy(void *dest, const void *src, size_t bytes)
{
    char *d = dest;
    const char *s = src;
    while (bytes--)
    {
        *d++ = *s++;
    }
}

void memmove(void *dest, const void *src, size_t bytes)
{
    char *p = malloc(bytes);
    memcpy(p, src, bytes);
    memcpy(dest, p, bytes);
    free(p);
}

void memset(void *dest, int value, size_t bytes)
{
    char *d = dest;
    while (bytes--)
    {
        *d++ = value;
    }
}
