/*
 * stdlib.c
 */

#include <common/stddef.h>
#include <common/stdlib.h>
#include <kernel/mem.h>

#define INT32_SIGN_BIT (int)0x80000000
#define TOP_COMPARE_BIT 0x40000000

inline int abs(int value)
{
    return value < 0 ? -value : value;
}

inline div_t div(int numerator, int denominator)
{
    div_t result;
#ifdef SOFTWARE_DIVISION
    result.quot = 0;
    result.rem = abs(numerator);
    int compare = abs(denominator);
    if (compare)
    {
        int sign = (numerator & INT32_SIGN_BIT) == (denominator & INT32_SIGN_BIT) ? 1 : -1;
        int bit = 1;
        while (compare < result.rem && bit != TOP_COMPARE_BIT)
        {
            bit <<= 1;
            compare <<= 1;
        }
        while (bit != 0)
        {
            if (result.rem >= compare)
            {
                result.quot |= bit;
                result.rem -= compare;
            }
            bit >>= 1;
            compare >>= 1;
        }
        if (sign < 0)
        {
            result.quot = -result.quot;
        }
    }
    if (numerator < 0)
    {
        result.rem = -result.rem;
    }
#else
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;
#endif
    return result;
}

inline int quotient(int numerator, int denominator)
{
#ifdef SOFTWARE_DIVISION
    div_t result = div(numerator, denominator);
    return result.quot;
#else
    return numerator / denominator;
#endif
}

inline int remainder(int numerator, int denominator)
{
#ifdef SOFTWARE_DIVISION
    div_t result = div(numerator, denominator);
    return result.rem;
#else
    return numerator % denominator;
#endif
}

char *itoa(int num, int base)
{
    static char intbuf[32];
    int j = 0, isneg = 0, i;
    div_t division_result;

    if (num == 0)
    {
        intbuf[0] = '0';
        intbuf[1] = '\0';
        return intbuf;
    }

    if (base == 10 && num < 0)
    {
        isneg = 1;
        num = -num;
    }

    i = num;

    while (i != 0)
    {
       division_result = div(i, base);
       intbuf[j++] = division_result.rem < 10 ? '0' + division_result.rem : 'a' + division_result.rem - 10;
       i = division_result.quot;
    }

    if (isneg)
    {
        intbuf[j++] = '-';
    }

    intbuf[j] = '\0';
    j--;
    i = 0;
    while (i < j)
    {
        isneg = intbuf[i];
        intbuf[i] = intbuf[j];
        intbuf[j] = isneg;
        i++;
        j--;
    }

    return intbuf;
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
