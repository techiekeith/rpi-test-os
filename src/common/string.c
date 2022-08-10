/*
 * string.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>

void __memory_set(void *dest, int value, uint32_t bytes);
int __memory_copy_forwards(void *dest, const void *src, uint32_t bytes);
int __memory_copy_backwards(void *dest, const void *src, uint32_t bytes);
void panic();

#define MAX_BLOCK_COPY 0x40000000

void memset(void *dest, int value, size_t bytes)
{
    while (bytes > MAX_BLOCK_COPY) {
        __memory_set(dest, value, MAX_BLOCK_COPY);
        dest += MAX_BLOCK_COPY;
        bytes -= MAX_BLOCK_COPY;
    }
    __memory_set(dest, value, (uint32_t)bytes);
}

static void memcpy_rev(void *dest, const void *src, size_t bytes)
{
    int rv;
    dest += bytes;
    src += bytes;
    uint32_t partial = bytes % MAX_BLOCK_COPY;
    if (partial) {
        dest -= partial;
        src -= partial;
        bytes -= partial;
        rv = __memory_copy_backwards(dest, src, partial);
        debug_printf("__memory_copy_backwards(%p, %p, 0x%x) returned value 0x%x\n",
                     dest, src, partial, rv);
    }
    while (bytes) {
        dest -= MAX_BLOCK_COPY;
        src -= MAX_BLOCK_COPY;
        bytes -= MAX_BLOCK_COPY;
        rv = __memory_copy_backwards(dest, src, MAX_BLOCK_COPY);
        debug_printf("__memory_copy_backwards(%p, %p, 0x%x) returned value 0x%x\n",
                     dest, src, MAX_BLOCK_COPY, rv);
    }
}

static void memcpy_fwd(void *dest, const void *src, size_t bytes)
{
    int rv;
    while (bytes > MAX_BLOCK_COPY) {
        rv = __memory_copy_forwards(dest, src, MAX_BLOCK_COPY);
        debug_printf("__memory_copy_forwards(%p, %p, 0x%x) returned value 0x%x\n",
                     dest, src, MAX_BLOCK_COPY, rv);
        dest += MAX_BLOCK_COPY;
        src += MAX_BLOCK_COPY;
        bytes -= MAX_BLOCK_COPY;
    }
    rv = __memory_copy_forwards(dest, src, (uint32_t)bytes);
    debug_printf("__memory_copy_forwards(%p, %p, 0x%x) returned value 0x%x\n",
                 dest, src, (uint32_t)bytes, rv);
}

void memcpy(void *dest, const void *src, size_t bytes)
{
    debug_printf("=> memcpy(%p, %p, 0x%lx)\n", dest, src, bytes);
    if (bytes > 0x800000) {
        panic();
        return;
    }
    // if bytes is zero or src and dest are equal, memcpy is a no-op, so do nothing
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
