/*
 * div32.h
 */

typedef struct { int quot; int rem; } idiv_return;
typedef struct { unsigned quot; unsigned rem; } uidiv_return;

signed int __aeabi_idiv(signed int numerator, signed int denominator);
unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator);
idiv_return __aeabi_idivmod(signed int numerator, signed int denominator);
uidiv_return __aeabi_uidivmod(unsigned int numerator, unsigned int denominator);
