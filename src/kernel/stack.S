/*
 * stack.S - functions for manipulating the stack pointer
 */

.global __get_stack_pointer

__get_stack_pointer:
    mov     r0, sp
    bx      lr

__set_stack_pointer:
    mov     sp, r0
    b       shell
