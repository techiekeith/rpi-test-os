/*
 * stddef.h
 */

#include "arch.h"

#pragma once

#define NULL ((void *)0)

#if (__WORD_SIZE == 64)
typedef unsigned long long size_t;
#else
typedef unsigned int size_t;
#endif
