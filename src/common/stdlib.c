/*
 * stdlib.c
 */

#include "../../include/common/limits.h"
#include "../../include/common/stddef.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/heap.h"

/* maximum size for 64-bit binary plus sign and terminating NUL. */
#define BUFFER_SIZE 66
static char ntoa_buffer[BUFFER_SIZE];

static char *ntoa(unsigned long long number, unsigned long long radix, int sign, char *buffer)
{
    if (radix < 2 || radix > 36) return NULL;
    if (buffer == NULL)
    {
        buffer = ntoa_buffer;
    }

    int p = 0, digit;
    while (p == 0 || (number && p < BUFFER_SIZE))
    {
        digit = (int)(number % radix);
        number /= radix;
        buffer[p++] = digit + (digit < 10 ? 48 : 87);
    }
    if (sign)
    {
        buffer[p++] = '-';
    }
    buffer[p--] = '\0';

    int q = 0;
    while (p > q)
    {
        digit = buffer[p];
        buffer[p--] = buffer[q];
        buffer[q++] = digit;
    }

    return buffer;
}

char *ultoa(unsigned long long number, int radix, char *buffer)
{
    return ntoa(number, radix, 0, buffer);
}

char *ltoa(long long number, int radix, char *buffer)
{
    unsigned long long cast = number;
    int sign = cast >> 63ULL;
    return ntoa(sign ? -cast : cast, radix, sign, buffer);
}

char *uitoa(unsigned int number, int radix, char *buffer)
{
    return ntoa((unsigned long long) number, radix, 0, buffer);
}

char *itoa(int number, int radix, char *buffer)
{
    unsigned long long cast = number;
    int sign = cast >> 63ULL;
    return ntoa(sign ? -cast : cast, radix, sign, buffer);
}

long long strtoll(const char *str, char **endptr, int base)
{
    long long res = 0;
    int sign = 1;
    char *p = (char *)str;

    if (base && (base < 2 || base > 36))
    {
        return 0;
    }

    if (*p == '-')
    {
        sign = -1;
        p++;
    }
    if (!base)
    {
        if (*p == '0')
        {
            p++;
            base = 8;
            if ((*p & 0x5f) == 'X')
            {
                base <<= 1;
                p++;
            }
            if ((*p & 0x5f) == 'B')
            {
                base >>= 2;
                p++;
            }
        }
        else
        {
            base = 10;
        }
    }
    while (((*p >= '0') && (*p <= ((base > 10) ? '9' : ('/' + base))))
        || ((base > 10) && ((*p & 0x5f) >= 'A' && (*p & 0x5f) <= ('6' + base))))
    {
        long long next = res * base + sign * (*p - ((*p & 0x40) ? ((*p & 0x20) + '7') : '0'));
        if (res < 0 && next > res) next = LLONG_MIN;
        if (res > 0 && next < res) next = LLONG_MAX;
        res = next;
        p++;
    }
    if (endptr != NULL) *endptr = p;
    return res;
}

long strtol(const char *str, char **endptr, int base)
{
#if (__WORD_SIZE == 64)
    return strtoll(str, endptr, base);
#else
    return CLAMP(strtoll(str, endptr, base), INT_MIN, INT_MAX, int);
#endif
}

long long atoll(const char *nptr)
{
    return strtoll(nptr, NULL, 10);
}

long atol(const char *nptr)
{
    return strtol(nptr, NULL, 10);
}

int atoi(const char *nptr)
{
    return (int) strtol(nptr, NULL, 10);
}

int abs(int j)
{
    return j < 0 ? -j : j;
}

long labs(long j)
{
    return j < 0L ? -j : j;
}

long long llabs(long long j)
{
    return j < 0LL ? -j : j;
}

void *malloc(size_t bytes)
{
    return bytes ? heap_alloc("malloc", bytes) : NULL;
}

void free(void *ptr)
{
    heap_free(ptr);
}
