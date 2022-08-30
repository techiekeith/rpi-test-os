/*
 * panic.c - print some useful debugging information if something unexpected happens
 */

#include "../../include/kernel/io.h"

void panic()
{
    debug_printf("kernel panic\r\n\r\n*** PANIC ***\r\n\r\n");
    debug_stacktrace();
}
