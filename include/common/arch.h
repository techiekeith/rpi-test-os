/*
 * arch.h
 */

#ifdef __ARM_64BIT_STATE
#if (__ARM_64BIT_STATE == 1)
#define __WORD_SIZE 64
#else
#define __WORD_SIZE 32
#endif
#else
#ifdef __arm__
#define __WORD_SIZE 32
#else
/* CLion inspection doesn't seem to provide any preprocessor defines. "Works on my machine". YMMV. */
#define __WORD_SIZE 64
#endif
#endif
