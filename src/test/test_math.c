/*
 * test.c
 */

#include <common/stdint.h>
#include <common/stdio.h>

#define VALUES_MAX 10
static int a_count = 0, b_count = 0;
static int a_values[VALUES_MAX] = { 22, 23, -19, -45, 12, -49,
                                    0x7fffffff, 0x80000000, 0x7fffffff, 0x80000000 };
static int b_values[VALUES_MAX] = { 7, -4, 9, -6, 0, 0,
                                    1, 1, 0x7fffffff, 0x80000000 };
#define LVALUES_MAX 15
static int la_count = 0, lb_count = 0;
static long long la_values[LVALUES_MAX] = { 22, 23, -19, -45, 12, -49,
                                            0x7fffffffLL, 0x80000000LL,
                                            0x7fffffffLL, 0x80000000LL,
                                            0xffffffffLL, 0x100000000LL,
                                            0x1ffffffffLL, 0x7fffffffffffffffLL,
                                            0x8000000000000000LL };
static long long lb_values[LVALUES_MAX] = { 7, -4, 9, -6, 0, 0,
                                            1, 1,
                                            0x7fffffffLL, 0x80000000LL,
                                            1, 1, 1, 1, 1 };

static void printhex(unsigned long long number, int digits)
{
    debug_printf("Number: %lx, digits: %d\n", number, digits);
    for (int i = digits - 1; i >= 0; i--)
    {
        int digit = (number >> (4LL * i)) & 15;
        putc(digit + (digit < 10 ? 48 : 87));
    }
}

static void divide64_eabi(unsigned long long numerator,
                   unsigned long long denominator,
                   unsigned long long *quotient,
                   unsigned long long *remainder)
{
    unsigned long long quot = numerator / denominator;
    *quotient = quot;
    unsigned long long rem = numerator % denominator;
    *remainder = rem;
}

static void divide64(unsigned long long numerator,
              unsigned long long denominator,
              unsigned long long *quotient,
              unsigned long long *remainder)
{
    unsigned int quot_l = 0; // r0
    unsigned int quot_h = 0; // r1
    unsigned int rem_l = numerator & 0xffffffffU; // r2
    unsigned int rem_h = (numerator >> 32ULL) & 0xffffffffU; // r3
    unsigned int cmp_l = denominator & 0xffffffffU; // r4
    unsigned int cmp_h = (denominator >> 32ULL) & 0xffffffffU; // r5
    unsigned int bit_l = 1; // r6
    unsigned int bit_h = 0; // r7

    if (!(cmp_l | cmp_h))
    {
        puts("Divide by zero\r\n");
        return;
    }

    /*
     * Keep going until we either hit the bitfield's most signifiant bit, or the comparison value
     * is greater than or equal to the remainder.
     */
    while (!(bit_h & 0x80000000U) && !(cmp_h & 0x80000000U)
        && (cmp_h < rem_h || (cmp_h == rem_h && cmp_l < rem_l)))
    {
        cmp_h <<= 1U;
        cmp_h |= ((cmp_l >> 31U) & 1);
        cmp_l <<= 1U;
        bit_h <<= 1U;
        bit_h |= ((bit_l >> 31U) & 1);
        bit_l <<= 1U;
    }

    /*
     * Go back through the bit field, adding to the quotient and subtracting from the remainder as we go.
     */
    while (bit_h | bit_l)
    {
        if (cmp_h < rem_h || (cmp_h == rem_h && cmp_l <= rem_l))
        {
            quot_l |= bit_l;
            quot_h |= bit_h;
            unsigned int result = rem_l - cmp_l;
            if (result > rem_l)
            {
                rem_h--;
            }
            rem_h -= cmp_h;
            rem_l = result;
        }
        bit_l >>= 1U;
        bit_l &= 0x7fffffffU;
        bit_l |= (bit_h << 31U);
        bit_h >>= 1U;
        bit_h &= 0x7fffffffU;
        cmp_l >>= 1U;
        cmp_l &= 0x7fffffffU;
        cmp_l |= (cmp_h << 31U);
        cmp_h >>= 1U;
        cmp_h &= 0x7fffffffU;
    }

    unsigned long long quot = quot_h;
    quot <<= 32ULL;
    quot |= quot_l;
    *quotient = quot;
    unsigned long long rem = rem_h;
    rem <<= 32ULL;
    rem |= rem_l;
    *remainder = rem;
}

static int get_a()
{
    if (a_count == VALUES_MAX) a_count = 0;
    return a_values[a_count++];
}

static int get_b()
{
    if (b_count == VALUES_MAX) b_count = 0;
    return b_values[b_count++];
}

static long long lget_a()
{
    if (la_count == LVALUES_MAX) la_count = 0;
    return (long long) la_values[la_count++];
}

static long long lget_b()
{
    if (lb_count == LVALUES_MAX) lb_count = 0;
    return (long long) lb_values[lb_count++];
}

static void print_idiv_results(int a, int b, int c)
{
    printf("test_idiv: %d / %d = %d (%s)\r\n", a, b, c,
            (b * c + a % b == a) ? "PASS" : "FAIL");
}

static void test_idiv()
{
    signed int a, b, c;
    for (int i = 0; i < VALUES_MAX; i++)
    {
        a = get_a(); b = get_b();
        c = a / b;
        print_idiv_results(a, b, c);
    }
}

static void print_idivmod_results(int a, int b, int c, int d)
{
    printf("test_idivmod: %d / %d = %d r%d (%s)\r\n", a, b, c, d,
           (b * c + d == a) ? "PASS" : "FAIL");
}

static void test_idivmod()
{
    signed int a, b, c, d;
    for (int i = 0; i < VALUES_MAX; i++)
    {
        a = get_a(); b = get_b();
        c = a / b;
        d = a % b;
        print_idivmod_results(a, b, c, d);
    }
}

static void print_ldivmod_results(long long a, long long b, long long c, long long d)
{
    printf("test_ldivmod: %ld / %ld = %ld r%ld (%s)\r\n", a, b, c, d,
           (b * c + d == a) ? "PASS" : "FAIL");
}

static void test_ldivmod()
{
    signed long long a, b, c, d;
    for (int i = 0; i < LVALUES_MAX; i++)
    {
        a = lget_a(); b = lget_b();
        c = a / b;
        d = a % b;
        print_ldivmod_results(a, b, c, d);
    }
}

static long long div10_test_cases[18] = { 0, 1, 0x12345678, 0x89abcdef, 0x7fffffffLL, 0x80000000LL,
                                          0xfffffffeLL, 0xffffffffLL, 0x100000000LL, 0x17fffffffLL,
                                          0x180000000LL, 0x1fffffffeLL, 0x1ffffffffLL, 0x200000000LL,
                                          0x7fffffffffffffffLL, 0x8000000000000000LL,
                                          0xfffffffffffffffeLL, 0xffffffffffffffffLL };

static void test_divide_by_ten()
{
    unsigned long long quot, rem;
    debug_printf("In test_div10()\n");
    for (int i = 0; i < 18; i++) {
        debug_printf("i: %d test_case: 0x%016lx\n", i, div10_test_cases[i]);
        puts("u32: ");
        unsigned int test_case = div10_test_cases[i];
        printhex((unsigned long long)test_case, 8);
        puts("\tu64: ");
        printhex(div10_test_cases[i], 16);
        divide64(div10_test_cases[i], 10LL, &quot, &rem);
        puts("\tdiv10 (C) : ");
        printhex(quot, 16);
        putc(':');
        printhex(rem, 16);
        divide64_eabi(div10_test_cases[i], 10LL, &quot, &rem);
        puts("\tdiv10(ARM): ");
        printhex(quot, 16);
        putc(':');
        printhex(rem, 16);
        putc('\r');
        putc('\n');
    }
}

uint32_t *__get_stack_pointer();

void test_math()
{
    debug_printf("test_idiv SP: %p\n", __get_stack_pointer());
    test_idiv();
    debug_printf("test_idivmod SP: %p\n", __get_stack_pointer());
    test_idivmod();
    debug_printf("test_ldivmod SP: %p\n", __get_stack_pointer());
    test_ldivmod();
    debug_printf("test_divide_by_ten SP: %p\n", __get_stack_pointer());
    test_divide_by_ten();
    debug_printf("~test_math SP: %p\n", __get_stack_pointer());
}
