/*
 * delay.S - busy-wait delay loop
 */

.globl delay

delay:
    ldr     r1, =#176
    nop
delay_inner_loop:
    subs    r1, r1, #1
    bne     delay_inner_loop
    subs    r0, r0, #1
    bpl     delay
    bx      lr
    .size   delay, .-delay
