/*
 * string.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/mem_internal.h"

#if (DEBUG_MEMCPY == 1)
#include "../../include/kernel/io.h"
#endif

int memcmp(const void *s1, const void *s2, size_t n)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    int i = n;
    while ((i > 0) && (*p1 == *p2))
    {
        i--;
        p1++;
        p2++;
    }
    return i && *p1 - *p2;
}

static void memcpy_rev(void *dest, const void *src, size_t bytes)
{
    // Fast copy if regions are word-aligned
    if (((size_t)dest & 3) == 0 && ((size_t)src & 3) == 0)
    {
#if (DEBUG_MEMCPY == 1)
        int rv;
#endif
        dest += bytes;
        src += bytes;
        uint32_t partial = bytes % MAX_BLOCK_COPY;
        if (partial) {
            dest -= partial;
            src -= partial;
            bytes -= partial;
#if (DEBUG_MEMCPY == 1)
            rv = __memory_copy_backwards(dest, src, partial);
            debug_printf("__memory_copy_backwards(%p, %p, 0x%x) returned value 0x%x\r\n",
                         dest, src, partial, rv);
#else
            __memory_copy_backwards(dest, src, partial);
#endif
        }
        while (bytes) {
            dest -= MAX_BLOCK_COPY;
            src -= MAX_BLOCK_COPY;
            bytes -= MAX_BLOCK_COPY;
#if (DEBUG_MEMCPY == 1)
            rv = __memory_copy_backwards(dest, src, MAX_BLOCK_COPY);
            debug_printf("__memory_copy_backwards(%p, %p, 0x%x) returned value 0x%x\r\n",
                         dest, src, MAX_BLOCK_COPY, rv);
#else
            __memory_copy_backwards(dest, src, MAX_BLOCK_COPY);
#endif
        }
        return;
    }
    // Slow copy if regions are not word-aligned
    size_t i = bytes;
    do {
        i--;
        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    } while (i != 0);
}

static void memcpy_fwd(void *dest, const void *src, size_t bytes)
{
    // Fast copy if regions are word-aligned
    if (((size_t)dest & 3) == 0 && ((size_t)src & 3) == 0)
    {
#if (DEBUG_MEMCPY == 1)
        int rv;
#endif
        uint32_t partial = bytes % MAX_BLOCK_COPY;
        bytes -= partial;
        while (bytes) {
#if (DEBUG_MEMCPY == 1)
            rv = __memory_copy_forwards(dest, src, MAX_BLOCK_COPY);
            debug_printf("__memory_copy_forwards(%p, %p, 0x%x) returned value 0x%x\r\n",
                         dest, src, MAX_BLOCK_COPY, rv);
#else
            __memory_copy_forwards(dest, src, MAX_BLOCK_COPY);
#endif
            dest += MAX_BLOCK_COPY;
            src += MAX_BLOCK_COPY;
            bytes -= MAX_BLOCK_COPY;
        }
#if (DEBUG_MEMCPY == 1)
        rv = __memory_copy_forwards(dest, src, partial);
        debug_printf("__memory_copy_forwards(%p, %p, 0x%x) returned value 0x%x\r\n",
                     dest, src, (uint32_t)bytes, rv);
#else
        __memory_copy_forwards(dest, src, partial);
#endif
        return;
    }
    // Slow copy if regions are not word-aligned
    for (size_t i = 0; i < bytes; i++)
    {
        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    }
}

void memcpy(void *dest, const void *src, size_t bytes)
{
    // if bytes is zero or src and dest are equal, memcpy is a no-op, so do nothing
    if (!bytes || src == dest) return;
#if (DEBUG_MEMCPY == 1)
    debug_printf("=> memcpy(%p, %p, 0x%lx)\r\n", dest, src, bytes);
#endif
    if (dest > src)
        memcpy_rev(dest, src, bytes);
    if (src > dest)
        memcpy_fwd(dest, src, bytes);
}

void memmove(void *dest, const void *src, size_t bytes)
{
    memcpy(dest, src, bytes);
}

void memset(void *dest, int value, size_t bytes)
{
    while (((size_t)dest & 3) != 0 && bytes > 0)
    {
        *((uint8_t *)dest++) = value;
        bytes--;
    }
    while (bytes >= MAX_BLOCK_COPY) {
        __memory_set_byte(dest, value, MAX_BLOCK_COPY);
        dest += MAX_BLOCK_COPY;
        bytes -= MAX_BLOCK_COPY;
    }
    if (bytes > 0)
    {
        __memory_set_byte(dest, value, (uint32_t)bytes);
    }
}

char *strcat(char *dest, const char *src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

char *strchr(const char *s, int c)
{
    char *p = (char *)s;
    while (*p && *p != c) p++;
    return *p == c ? p : NULL;
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

char *strcpy(char *dest, const char *src)
{
    char *p = dest;
    for (char *q = (char *)src; *q != '\0'; p++, q++)
    {
        *p = *q;
    }
    *p = '\0';
    return dest;
}

size_t strlen(const char *s)
{
    return (size_t)(strchr(s, '\0') - s);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    int i = n;
    while ((i > 0) && (*p1 | *p2) && (*p1 == *p2))
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
