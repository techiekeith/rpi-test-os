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
    return *p1 - *p2;
}
