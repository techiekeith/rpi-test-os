/*
 * div64.S
 */

.globl __aeabi_ldivmod
.globl __aeabi_uldivmod

/*
 * __aeabi_ldivmod
 *
 * operands: numerator in r0 (LSB) / r1 (MSB), denominator in r2 (LSB) / r3 (MSB)
 * returns:  quotient in r0 (LSB) / r1 (MSB),  remainder in r2 (LSB) / r3 (MSB)
 */
__aeabi_ldivmod:
    push    { r4, r5, lr }      /* start function */
    mov     r5, #-1
    mov     r4, #0              /* neither operand is negative */
    teq     r1, r3              /* do operands differ in signedness? */
    orrmi   r4, #1              /* set "operand signs differ" flag in r3 */
    movs    r1, r1              /* set Negative flag if numerator is negative */
    bpl     skip_abs_r0_r1
    orrmi   r4, #2              /* set "numerator is negative" flag in r3 */
    eor     r0, r5
    adds    r0, #1
    eor     r1, r5
    adc     r1, #0              /* make numerator a positive integer */
skip_abs_r0_r1:
    movs    r3, r3              /* set Negative flag if denominator is negative */
    bpl     skip_abs_r2_r3
    eor     r2, r5
    adds    r2, #1
    eor     r3, r5
    adc     r3, #0              /* make denominator a positive integer */
skip_abs_r2_r3:
    bl      __aeabi_uldivmod
    tst     r4, #1              /* were operand signs different? */
    beq     skip_neg_r0_r1
    eor     r0, r5
    adds    r0, #1
    eor     r1, r5
    adc     r1, #0              /* make quotient a negative integer */
skip_neg_r0_r1:
    tst     r4, #2              /* was numerator negative? */
    beq     skip_neg_r2_r3
    eor     r2, r5
    adds    r2, #1
    eor     r3, r5
    adc     r3, #0              /* make remainder a negative integer */
skip_neg_r2_r3:
    pop     { r4, r5, pc }      /* end function */
    .size	__aeabi_ldivmod, .-__aeabi_ldivmod

/*
 * __aeabi_uldivmod
 *
 * operands: numerator in r0 (LSB) / r1 (MSB), denominator in r2 (LSB) / r3 (MSB)
 * returns:  quotient in r0 (LSB) / r1 (MSB),  remainder in r2 (LSB) / r3 (MSB)
 */
__aeabi_uldivmod:
    push    { r4-r7, lr }       /* start function */
    push    { r0-r3 }           /* numerator -> remainder, denominator -> comparison value */
    pop     { r2-r5 }
    mov     r7, #0              /* next bit to set in quotient */
    mov     r6, #1
    mov     r1, r7              /* set quotient initially to zero */
    mov     r0, r7
    orrs    ip, r4, r5          /* check if denominator is zero */
    beq     divzero             /* return on division by zero */
loop1:
    orrs    ip, r5, r7          /* if either bit or comparison value go negative, break out of loop */
    bmi     loop2
    cmp     r5, r3              /* if comparison value is greater than or equal to remainder, break */
    bhi     loop2
    bne     shift_left
    cmp     r4, r2
    bhi     loop2
    beq     loop2
shift_left:
    lsls    r4, #1              /* shift LSB of comparison value, push bit 31 into carry */
    adc     r5, r5              /* shift MSB of comparison value */
    lsls    r6, #1              /* shift LSB of bit, push bit 31 into carry */
    adc     r7, r7              /* shift MSB of bit */
    b       loop1
loop2:
    cmp     r5, r3
    bhi     skip2               /* if remainder MSB is less than comparison value MSB, skip over update_quotient */
    bne     update_quotient
    cmp     r4, r2
    bhi     skip2               /* if remainder LSB is less than comparison value LSB, skip over update_quotient */
update_quotient:
    orr     r0, r6              /* set bit in quotient LSB */
    orr     r1, r7              /* set bit in quotient MSB */
    subs    r2, r4              /* subtract comparison value from remainder LSB, set carry/borrow */
    sbc     r3, r5              /* subtract comparison value from remainder MSB with carry/borrow */
skip2:
    lsrs    r5, #1              /* shift MSB of comparison value */
    mov     r4, r4, rrx         /* rotate LSB of comparison value with carry */
    lsrs    r7, #1              /* shift MSB of bit */
    movs    r6, r6, rrx         /* rotate LSB of bit with carry */
    bcc     loop2               /* while there are still bits to process */
divzero:
    pop     { r4-r7, pc }       /* end function */
    .size	__aeabi_uldivmod, .-__aeabi_uldivmod
