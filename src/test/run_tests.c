/*
 * run_tests.c - entry point for tests.
 */

#include <common/stdint.h>
#include <common/stdio.h>

uint32_t *__get_stack_pointer();

void test_math();
void test_utf8();

void run_tests()
{
    debug_printf("test_math SP: %p\n", __get_stack_pointer());
    test_math();
    debug_printf("test_utf8 SP: %p\n", __get_stack_pointer());
    test_utf8();
    debug_printf("~run_tests SP: %p\n", __get_stack_pointer());
}
