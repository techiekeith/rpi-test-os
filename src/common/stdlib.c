/*
 * stdlib.c
 */

#include <common/stddef.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <kernel/mem.h>

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

int atoi(char *str)
{
    int res = 0, sign = 0;
    char *p = str;

    if (*p == '-')
    {
        sign = 1;
        p++;
    }
    while (*p >= '0' && *p <= '9')
    {
        res = res * 10 + *p - '0';
        p++;
    }

    return sign ? -res : res;
}

void *malloc(size_t bytes)
{
    return bytes ? kmalloc(bytes) : NULL;
}

void free(void *ptr)
{
    kfree(ptr);
}
