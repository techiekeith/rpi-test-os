/*
 * arch.h
 */

#ifdef __ARM_64BIT_STATE
#if __ARM_64BIT_STATE == 1
#define __WORDSIZE 64
#else
#define __WORDSIZE 32
#endif
#else
#define __WORDSIZE 32
#endif