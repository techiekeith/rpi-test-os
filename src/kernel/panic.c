/*
 * panic.c - print some useful debugging information if something unexpected happens
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/io.h"

void *__get_stack_pointer();
void _start();

void panic()
{
    void *stack = __get_stack_pointer();
    puts("\r\n\r\n*** PANIC ***\r\n\r\n");
    debug_printf("\r\n\r\n*** PANIC ***\r\n\r\n");
    int i = 0;
    while (stack < (void *)(_start)) {
        if (!i) {
            debug_printf("\r\n%p : ", stack);
        }
        debug_printf("%08ux ", *((uint8_t *)stack));
        i++;
        i %= 16;
        stack++;
    }
    debug_printf("\r\n");
}
