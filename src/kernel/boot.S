/*
 * boot.S - boot code
 */

/* To keep this in the first portion of the binary. */
.section ".text.boot"

/* Make _start global. */
.globl _start

/*
 * Entry point for the kernel.
 * r15 -> should begin execution at 0x8000.
 * r0 -> 0x00000000
 * r1 -> 0x00000C42
 * r2 -> 0x00000100 - start of ATAGS
 * preserve these registers as argument for kernel_main
 */
_start:
    /* shut down all cores except core 0 */
#ifndef BCM2835
    mrc     p15, #0, r1, c0, c0, #5
    and     r1, r1, #3          /* only interested in b0-1 of r1 */
    cmp     r1, #0              /* is this core 0? */
    bne     halt                /* if not, go to halt */
#endif

    /*
     * initialize stack pointer - this will grow downwards from 0x8000
     * kernel loads into memory at 0x8000 so this keeps the stack in a safe place
     */
    mov     sp, #0x8000

    /* initialize the BSS (uninitialized C global variables) to zero */
    ldr     r4, =__bss_start    /* start of uninitialized global variable page */
    ldr     r9, =__bss_end      /* end of uninitialized global variable page */
    mov     r5, #0
    mov     r6, #0
    mov     r7, #0
    mov     r8, #0
    b       2f                  /* branch forward to 2 */

1:
    stmia   r4!, {r5-r8}        /* write R5-R8 into memory at R4, increment R4 and update R4 with result */

2:
    cmp     r4, r9              /* have we reached the end yet? */
    blo     1b                  /* branch back to 1 */

    ldr     r3, =main
    blx     r3                  /* start the kernel */

halt:
    wfe                         /* wait for event */
    b       halt
