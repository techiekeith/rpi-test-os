/*
 * run_tests.c - entry point for tests.
 */

#include "../../include/kernel/io.h"

DEBUG_INIT("run_tests");

void test_math();
void test_utf8();

void run_tests()
{
    DEBUG_START("run_tests");
    test_math();
    test_utf8();
    DEBUG_END();
}
