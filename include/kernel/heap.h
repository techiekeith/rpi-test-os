/*
 * heap.h
 */

#pragma once

#include "../common/stddef.h"
#include "../common/stdint.h"

#define KERNEL_HEAP_SIZE    1048576

typedef struct heap_segment
{
    char consumer[16];
    struct heap_segment *next;
    struct heap_segment *prev;
    uint32_t is_allocated;
    size_t segment_size;  /* Includes this header */
} heap_segment_t;

void heap_init();
void *heap_alloc(const char *consumer, size_t bytes);
void heap_free(void *ptr);
