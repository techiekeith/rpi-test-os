/*
 * div32.S
 */

.globl __aeabi_idiv
.globl __aeabi_idivmod
.globl __aeabi_uidiv
.globl __aeabi_uidivmod
.align 2

/*
 * __aeabi_idiv
 *
 * operands: numerator in r0, denominator in r1
 * returns:  quotient in r0
 */
__aeabi_idiv:
    push    { r1, lr }          /* start function */
    bl      __aeabi_idivmod
    pop     { r1, pc }          /* end function */

/*
 * __aeabi_uidiv
 *
 * operands: numerator in r0, denominator in r1
 * returns:  quotient in r0
 */
__aeabi_uidiv:
    push    { r1, lr }          /* start function */
    bl      __aeabi_uidivmod
    pop     { r1, pc }          /* end function */

/*
 * __aeabi_idivmod
 *
 * operands: numerator in r0, denominator in r1
 * returns:  quotient in r0,  remainder in r1
 */
__aeabi_idivmod:
    push    { r4, lr }          /* start function */
    mov     r4, #0              /* assume neither operand is negative to start with */
    teq     r0, r1              /* do operands differ in signedness? */
    orrmi   r4, #1              /* set "operand signs differ" flag in r4 */
    movs    r0, r0              /* set Negative flag if numerator is negative */
    orrmi   r4, #2              /* set "numerator is negative" flag in r4 */
    negmi   r0, r0              /* make numerator a positive integer */
    movs    r1, r1              /* set Negative flag if denominator is negative */
    negmi   r1, r1              /* make denominator a positive integer */
    bl      __aeabi_uidivmod
    tst     r4, #1              /* were operand signs different? */
    negne   r0, r0              /* if so, negate quotient */
    tst     r4, #2              /* was numerator negative? */
    negne   r1, r1              /* if so, negate remainder */
    pop     { r4, pc }          /* end function */

/*
 * __aeabi_uidivmod
 *
 * operands: numerator in r0, denominator in r1
 * returns:  quotient in r0,  remainder in r1
 */
__aeabi_uidivmod:
    push    { r4, r5, lr }      /* start function */
    mov     r5, #1              /* next bit to set in quotient */
    movs    r4, r1              /* set comparison value initially to value of denominator */
    mov     r1, r0              /* set remainder initially to value of numerator */
    mov     r0, #0              /* set quotient initially to zero */
    beq     divzero             /* return if denominator is zero */
loop1:
    cmp     r4, r1              /* if comparison value is greater than or equal to remainder, break */
    bpl     loop2
    lsl     r4, #1              /* shift comparison value */
    lsls    r5, #1              /* shift bit */
    bpl     loop1               /* loop until we reach bit 30 */
loop2:
    cmp     r1, r4
    bmi     skip2               /* if remainder is less than comparison value, skip next two instructions */
    orr     r0, r5              /* set bit in quotient */
    sub     r1, r4              /* subtract comparison value from remainder */
skip2:
    lsr     r4, #1              /* shift comparison value */
    lsrs    r5, #1              /* shift bit */
    bne     loop2
divzero:
    pop     { r4, r5, pc }      /* end function */
