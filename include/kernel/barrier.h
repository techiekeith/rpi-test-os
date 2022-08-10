/*
 * barrier.h - Memory barrier macros
 */
#pragma once

#ifdef BCM2835
//raspi is ARMv6. It does not have ARMv7 DMB/DSB/ISB, so go through CP15.
#define __isb() __asm__ __volatile__ ("mcr p15, 0, %0, c7,  c5, 4" : : "r" (0) : "memory")
#define __dmb() __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory")
//ARMv6 DSB (DataSynchronizationBarrier): also known as DWB (drain write buffer / data write barrier) on ARMv5
#define __dsb() __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory")
//use dmb() around the accesses. ("before write, after read")
#else
#define __isb() __asm__ __volatile__ ("isb")
#define __dmb() __asm__ __volatile__ ("dmb")
#define __dsb() __asm__ __volatile__ ("dsb")
#endif
