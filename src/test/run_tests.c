/*
 * run_tests.c - entry point for tests.
 */

#include "../../include/common/string.h"
#include "../../include/kernel/io.h"

DEBUG_INIT("run_tests");

void test_math();
void test_utf8();
void test_printf();

void run_tests(int argc, char **argv)
{
    DEBUG_START("run_tests");
    if (argc == 1 || !strcmp(argv[1], "math")) test_math();
    if (argc == 1 || !strcmp(argv[1], "utf8")) test_utf8();
    if (argc == 1 || !strcmp(argv[1], "printf")) test_printf();
    DEBUG_END();
}
