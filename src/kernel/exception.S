/*
 * exception.S - Exception jump and vector tables
 * Shamelessly cribbed from Jake Sandler and Brian Sidebotham.
 */

.globl move_exception_tables

/* copy the exception tables to address 0x00000000 */
move_exception_tables:
    push    {r4-r11, lr}
    ldr     r0, =exception_jump_table
    mov     r1, #0
    ldmia   r0!, {r4-r11}
    stmia   r1!, {r4-r11}
    ldmia   r0!, {r4-r11}
    stmia   r1!, {r4-r11}
    pop     {r4-r11, pc}
    .size	move_exception_tables, .-move_exception_tables

exception_jump_table:
    ldr     pc, _reset_vector
    ldr     pc, _undefined_instruction_vector
    ldr     pc, _software_interrupt_vector
    ldr     pc, _prefetch_abort_vector
    ldr     pc, _data_abort_vector
    nop     /* reserved */
    ldr     pc, _interrupt_vector
    ldr     pc, _fast_interrupt_vector
    .size	exception_jump_table, .-exception_jump_table

exception_vector_table:
_reset_vector:
    .word   _start
_undefined_instruction_vector:
    .word   undefined_instruction_wrapper
_software_interrupt_vector:
    .word   software_interrupt_wrapper
_prefetch_abort_vector:
    .word   prefetch_abort_wrapper
_data_abort_vector:
    .word   data_abort_wrapper
_unused_vector:
    .word   0 /* reserved */
_interrupt_vector:
    .word   interrupt_wrapper
_fast_interrupt_vector:
    .word   fast_interrupt_wrapper
    .size	exception_vector_table, .-exception_vector_table

undefined_instruction_wrapper:
    sub     lr, lr, #4      /* ARM exception quirk: adjust return address to be one instruction back */
    srsdb   sp!, #0x13      /* store lr, spsr and cpsr to the 0x13 (supervisor mode) stack */
    cpsid   if, #0x13       /* switch to supervisor mode with interrupts disabled */
    push    {r0-r4, ip, lr} /* store all the caller-save registers, together with r4 since we're about to use it */
    and     r4, sp, #4      /* adjust sp if needed to make it double-word aligned */
    sub     sp, sp, r4
    bl      undefined_instruction_handler
    add     sp, sp, r4      /* adjust sp back again */
    pop     {r0-r4, ip, lr} /* restore all the caller-save registers and r4 */
    rfeia   sp!             /* restore the cpsr and return to address in stored lr */
    .size	undefined_instruction_wrapper, .-undefined_instruction_wrapper

software_interrupt_wrapper:
    sub     lr, lr, #4      /* ARM exception quirk: adjust return address to be one instruction back */
    srsdb   sp!, #0x13      /* store lr, spsr and cpsr to the 0x13 (supervisor mode) stack */
    cpsid   if, #0x13       /* switch to supervisor mode with interrupts disabled */
    push    {r0-r4, ip, lr} /* store all the caller-save registers, together with r4 since we're about to use it */
    and     r4, sp, #4      /* adjust sp if needed to make it double-word aligned */
    sub     sp, sp, r4
    bl      software_interrupt_handler
    add     sp, sp, r4      /* adjust sp back again */
    pop     {r0-r4, ip, lr} /* restore all the caller-save registers and r4 */
    rfeia   sp!             /* restore the cpsr and return to address in stored lr */
    .size	software_interrupt_wrapper, .-software_interrupt_wrapper

prefetch_abort_wrapper:
    sub     lr, lr, #4      /* ARM exception quirk: adjust return address to be one instruction back */
    srsdb   sp!, #0x13      /* store lr, spsr and cpsr to the 0x13 (supervisor mode) stack */
    cpsid   if, #0x13       /* switch to supervisor mode with interrupts disabled */
    push    {r0-r4, ip, lr} /* store all the caller-save registers, together with r4 since we're about to use it */
    and     r4, sp, #4      /* adjust sp if needed to make it double-word aligned */
    sub     sp, sp, r4
    bl      prefetch_abort_handler
    add     sp, sp, r4      /* adjust sp back again */
    pop     {r0-r4, ip, lr} /* restore all the caller-save registers and r4 */
    rfeia   sp!             /* restore the cpsr and return to address in stored lr */
    .size	prefetch_abort_wrapper, .-prefetch_abort_wrapper

data_abort_wrapper:
    sub     lr, lr, #4      /* ARM exception quirk: adjust return address to be one instruction back */
    srsdb   sp!, #0x13      /* store lr, spsr and cpsr to the 0x13 (supervisor mode) stack */
    cpsid   if, #0x13       /* switch to supervisor mode with interrupts disabled */
    push    {r0-r4, ip, lr} /* store all the caller-save registers, together with r4 since we're about to use it */
    and     r4, sp, #4      /* adjust sp if needed to make it double-word aligned */
    sub     sp, sp, r4
    bl      data_abort_handler
    add     sp, sp, r4      /* adjust sp back again */
    pop     {r0-r4, ip, lr} /* restore all the caller-save registers and r4 */
    rfeia   sp!             /* restore the cpsr and return to address in stored lr */
    .size	data_abort_wrapper, .-data_abort_wrapper

interrupt_wrapper:
    sub     lr, lr, #4      /* ARM exception quirk: adjust return address to be one instruction back */
    srsdb   sp!, #0x13      /* store lr, spsr and cpsr to the 0x13 (supervisor mode) stack */
    cpsid   if, #0x13       /* switch to supervisor mode with interrupts disabled */
    push    {r0-r4, ip, lr} /* store all the caller-save registers, together with r4 since we're about to use it */
    and     r4, sp, #4      /* adjust sp if needed to make it double-word aligned */
    sub     sp, sp, r4
    bl      interrupt_handler
    add     sp, sp, r4      /* adjust sp back again */
    pop     {r0-r4, ip, lr} /* restore all the caller-save registers and r4 */
    rfeia   sp!             /* restore the cpsr and return to address in stored lr */
    .size	interrupt_wrapper, .-interrupt_wrapper

fast_interrupt_wrapper:
    sub     lr, lr, #4      /* ARM exception quirk: adjust return address to be one instruction back */
    srsdb   sp!, #0x13      /* store lr, spsr and cpsr to the 0x13 (supervisor mode) stack */
    cpsid   if, #0x13       /* switch to supervisor mode with interrupts disabled */
    push    {r0-r4, ip, lr} /* store all the caller-save registers, together with r4 since we're about to use it */
    and     r4, sp, #4      /* adjust sp if needed to make it double-word aligned */
    sub     sp, sp, r4
    bl      fast_interrupt_handler
    add     sp, sp, r4      /* adjust sp back again */
    pop     {r0-r4, ip, lr} /* restore all the caller-save registers and r4 */
    rfeia   sp!             /* restore the cpsr and return to address in stored lr */
    .size	fast_interrupt_wrapper, .-fast_interrupt_wrapper
