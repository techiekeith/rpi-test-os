/*
 * boot.S - boot code
 */

/* To keep this in the first portion of the binary. */
.section ".text.boot"

/* Make _start and halt global. */
.globl      _start
.globl      halt

/*
 * Entry point for the kernel.
 * r15 -> should begin execution at 0x8000.
 */
_start:
    mrc     p15, #0, r0, c0, c0, #0
    ldr     r1, board_id_mask
    and     r0, r1, r0, lsr #4
    ldr     r1, rpi1_board_id
    cmp     r0, r1
    beq     enable_fpu

    /* shut down all cores except core 0 */
disable_cores:
    mrc     p15, #0, r4, c0, c0, #5
    and     r4, r4, #3          /* only interested in b0-1 of r4 */
    cmp     r4, #0              /* is this core 0? */
    bne     halt                /* if not, go to halt */

    /*
     * enable FPU (https://forums.raspberrypi.com/viewtopic.php?t=11183)
     */
enable_fpu:
    mrc     p15, #0, r4, c1, c0, #2
    orr     r4, r4, #0x300000            /* single precision */
    orr     r4, r4, #0xC00000            /* double precision */
    mcr     p15, #0, r4, c1, c0, #2
    mov     r4, #0x40000000
    fmxr    fpexc, r4

    /*
     * initialize stack pointer - this will grow downwards from 0x8000
     * kernel loads into memory at 0x8000 so this keeps the stack in a safe place
     */
    ldr     sp, =_start

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

    ldr     r3, =kernel_main
    blx     r3                  /* start the kernel */

halt:
    wfe                         /* wait for event */
    b       halt

    .size	_start, .-_start

board_id_mask:
    .word   0xfff
rpi1_board_id:
    .word   0xb76
