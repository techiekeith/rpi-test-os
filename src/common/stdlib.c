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

void iprinthex(unsigned int number)
{
    printhex((unsigned long long) number);
}

void printhex(unsigned long long number)
{
    unsigned long long digit = number & 15;
    unsigned long long next = (number >> 4ULL) & 0xfffffffffffffffLL;
    if (next) printhex(next);
    putc(digit + (digit < 10 ? 48 : 87));
}

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

char *ultoa(unsigned long long number, int radix)
{
    return ntoa(number, radix, 0, NULL);
}

char *ltoa(long long number, int radix)
{
    unsigned long long cast = number;
    int sign = cast >> 63ULL;
    return ntoa(sign ? -cast : cast, radix, sign, NULL);
}

char *uitoa(unsigned int number, int radix)
{
    return ntoa((unsigned long long) number, radix, 0, NULL);
}

char *itoa(int number, int radix)
{
    unsigned long long cast = number;
    int sign = cast >> 63ULL;
    return ntoa(sign ? -cast : cast, radix, sign, NULL);
}

int atoi(char *num)
{
    int res = 0, power = 0, digit, i;
    char * start = num;

    // Find the end
    while (*num >= '0' && *num <= '9')
    {
        num++;     
    }

    num--;

    while (num != start)
    {
        digit = *num - '0'; 
        for (i = 0; i < power; i++)
        {
            digit *= 10;
        }
        res += digit;
        power++;
        num--;
    }

    return res;
}

void *malloc(size_t bytes)
{
    return bytes ? kmalloc(bytes) : NULL;
}

void free(void *ptr)
{
    kfree(ptr);
}
