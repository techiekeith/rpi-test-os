/*
 * mem_internal.S - memory copy
 * void __memory_set(void *dest, int value, uint32_t bytes);
 * void __memory_copy(void *dest, const void *src, uint32_t bytes);
 * void __memory_copy_backwards(void *dest, const void *src, uint32_t bytes);
 */

.globl __memory_set
.globl __memory_copy_forwards
.globl __memory_copy_backwards

__memory_set:
    push    { r0-r10, lr }
    mov     r3, r1
    mov     r4, r1
    mov     r5, r1
    mov     r6, r1
    mov     r7, r1
    mov     r8, r1
    mov     r9, r1
    mov     r10, r1
    cmp     r2, #32
    bls     ms_copy_words_check
ms_copy_chunks_loop:
    stmia   r0!, { r3-r10 }
    sub     r2, #32
    cmp     r2, #32
    bge     ms_copy_chunks_loop
ms_copy_words_check:
    cmp     r2, #4
    bls     ms_copy_bytes_check
ms_copy_words_loop:
    stmia   r0!, { r3 }
    sub     r2, #4
    cmp     r2, #4
    bge     ms_copy_words_loop
ms_copy_bytes_check:
    cmp     r2, #0
    beq     ms_ret
ms_copy_bytes_loop:
    strb    r3, [r0]
    add     r0, #1
    add     r1, #1
    subs    r2, #1
    bne     ms_copy_bytes_loop
ms_ret:
    pop     { r0-r10, pc }

__memory_copy_forwards:
    push    { r1-r11, lr }
    mov     r11, #0                 // keep track of what copy methods were used, for testing/debugging
    cmp     r2, #32                 // do we have at least 32 bytes to copy?
    bls     mcf_copy_words_check    // if not, skip fastest copy method
    sub     r3, r1, r0
    cmp     r3, #32                 // are src and dest less than 32 bytes apart?
    bls     mcf_copy_words_check    // if so, skip fastest copy method
    orr     r11, #4                 // 4 = fastest copy method
mcf_copy_chunks_loop:
    ldmia   r1!, { r3-r10 }
    stmia   r0!, { r3-r10 }
    sub     r2, #32
    cmp     r2, #32
    bge     mcf_copy_chunks_loop
mcf_copy_words_check:
    cmp     r2, #4                  // do we have at least 4 bytes to copy?
    bls     mcf_copy_bytes_check
    sub     r3, r1, r0
    cmp     r3, #4                  // are src and dest less than 4 bytes apart?
    bls     mcf_copy_bytes_check    // if so, use slowest copy method
    orr     r11, #2                 // 2 = slower copy method
mcf_copy_words_loop:
    ldmia   r1!, { r3 }
    stmia   r0!, { r3 }
    sub     r2, #4
    cmp     r2, #4
    bge     mcf_copy_words_loop
mcf_copy_bytes_check:
    cmp     r2, #0
    beq     mcf_ret
    orr     r11, #1                 // 1 = slowest copy method
mcf_copy_bytes_loop:
    ldrb    r3, [r1]
    strb    r3, [r0]
    add     r0, #1
    add     r1, #1
    subs    r2, #1
    bne     mcf_copy_bytes_loop
mcf_ret:
    mov     r0, r11                 // set information about copy methods used in return value
    pop     { r1-r11, pc }

// Memory copy backwards. dest should be greater than src
// For efficient copying, dest should be at least 32 bytes (8 words) more than src
__memory_copy_backwards:
    push    { r1-r11, lr }
    mov     r11, #0                 // keep track of what copy methods were used, for testing/debugging
    add     r0, r2                  // Set destination pointer to the end of the destination block
    add     r1, r2                  // Set source pointer to the end of the source block
    ands    r3, r2, #3              // Are we copying an exact number of words?
    beq     mcb_copy_words_check    // If so, try a copying a word at a time
    eor     r2, r3                  // Reduce r2 to an exact multiple of 4 bytes
    orr     r11, #1                 // return value 1 = slowest copy method
mcb_copy_bytes_loop:
    sub     r1, #1                  // Decrement before copy, as we're counting down
    sub     r0, #1
    ldrb    r4, [r1]                // Copy one byte at a time, up to 3 bytes
    strb    r4, [r0]
    subs    r3, #1
    bne     mcb_copy_bytes_loop     // ... until we can try copying a word at a time
mcb_copy_words_check:
    sub     r3, r0, r1
    cmp     r3, #4                  // are src and dest at least 4 bytes apart?
    bge     mcb_faster_copy         // if so, use faster copy method
mcb_slowest_copy_loop:
    sub     r1, #1                  // Decrement before copy, as we're counting down
    sub     r0, #1
    ldrb    r4, [r1]                // Copy one byte at a time
    strb    r4, [r0]
    subs    r2, #1
    bne     mcb_slowest_copy_loop   // ... until there are no more bytes to copy
    orr     r11, #8                 // return value 8 = forced slowest copy method
    b       mcb_ret
mcb_faster_copy:
    ands    r3, r2, #28             // Are we copying an exact multiple of 32 bytes?
    beq     mcb_copy_chunks_check   // If so, try copying in 32-byte chunks
    eor     r2, r3                  // Reduce r2 to an exact multiple of 32 bytes
    orr     r11, #2                 // return value 1 = slower copy method
mcb_copy_words_loop:
    ldmdb   r1!, { r4 }             // Decrement before copy, as we're counting down
    stmdb   r0!, { r4 }
    subs    r3, #4
    bne     mcb_copy_words_loop     // ... until we can try copying 32 bytes at a time
mcb_copy_chunks_check:
    sub     r3, r0, r1
    cmp     r3, #32                 // are src and dest at least 32 bytes apart?
    bge     mcb_fastest_copy        // if so, use fastest copy method
    orr     r11, #16                // return value 16 = forced slower copy method
mcb_slower_copy_loop:
    ldmdb   r1!, { r4 }             // Decrement before copy, as we're counting down
    stmdb   r0!, { r4 }
    subs    r2, #4
    bne     mcb_slower_copy_loop
    mov     r0, #2                  // return value 2 = slower copy method used
    b       mcb_ret
mcb_fastest_copy:
    cmp     r2, #0
    beq     mcb_ret
    orr     r11, #4                 // return value 4 = fastest copy method
mcb_copy_chunks_loop:
    ldmdb   r1!, { r3-r10 }         // Decrement before copy, as we're counting down
    stmdb   r0!, { r3-r10 }
    subs    r2, #32
    bne     mcb_copy_chunks_loop    // Keep going until we run out of chunks to copy
mcb_ret:
    mov     r0, r11                 // set information about copy methods used in return value
    pop     { r1-r11, pc }
