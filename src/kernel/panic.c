/*
 * panic.c - print some useful debugging information if something unexpected happens
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/io.h"

uint32_t *__get_stack_pointer();
void _start();

void panic()
{
    uint32_t *stack = __get_stack_pointer();
    puts("\r\n\r\n*** PANIC ***\r\n\r\n");
    debug_printf("\r\n\r\n*** PANIC ***\r\n\r\n");
    int i = 0;
    while ((void *)(stack) < (void *)(_start)) {
        if (!i) {
            debug_printf("\r\n%p : ", stack);
        }
        debug_printf("%08ux ", *stack);
        i++;
        i %= 16;
        stack++;
    }
    debug_printf("\r\n");
}
