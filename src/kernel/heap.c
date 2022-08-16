/*
 * heap.c - kernel heap functions
 */

#include "../../include/common/stdint.h"
#include "../../include/common/string.h"
#include "../../include/kernel/heap.h"

extern uint8_t __end;

heap_segment_t *heap_segment_list_head;

/*
 * Initialise the heap.
 */
void heap_init()
{
    heap_segment_list_head = (heap_segment_t *) &__end;
    memset(heap_segment_list_head, 0, sizeof(heap_segment_t));
    heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
}

/*
 * implement heap_alloc as a linked list of allocated segments.
 * Segments should be 4 byte aligned.
 * Use best fit algorithm to find an allocation
 */
void *heap_alloc(const char *consumer, size_t bytes)
{
    heap_segment_t *curr, *best = NULL;
    int diff, best_diff = 0x7fffffff; /* Max signed int */

    /* Add the header to the number of bytes we need and make the size 4 byte aligned */
    size_t total_bytes = bytes + sizeof(heap_segment_t);
    total_bytes += total_bytes % 16 ? 16 - (total_bytes % 16) : 0;

    /* Find a segment that best matches the requested size */
    for (curr = heap_segment_list_head; curr != NULL; curr = curr->next)
    {
        diff = curr->segment_size - total_bytes;
        if (!curr->is_allocated && diff < best_diff && diff >= 0)
        {
            best = curr;
            best_diff = diff;
        }
    }

    /* There must be no free memory right now :( */
    if (best == NULL)
    {
        return NULL;
    }

    /*
     * If the best difference we could come up with was large, split up this segment into two.
     * Since our segment headers are rather large, the criterion for splitting the segment is that
     * when split, the segment not being requested should be twice a header size
     */
    if (best_diff > (int)(2 * sizeof(heap_segment_t)))
    {
        memset(((char *)(best)) + total_bytes, 0, sizeof(heap_segment_t));
        curr = best->next;
        best->next = (heap_segment_t *)(((char *)(best)) + total_bytes);
        best->next->next = curr;
        best->next->prev = best;
        best->next->segment_size = best->segment_size - total_bytes;
        best->segment_size = total_bytes;
        strncpy(best->consumer, consumer, 15);
        best->consumer[15] = '\0';
    }

    best->is_allocated = 1;

    return best + 1;
}

/*
 * Releases previously allocated kernel heap memory.
 */
void heap_free(void *ptr)
{
    heap_segment_t *seg;

    if (!ptr)
    {
        return;
    }

    seg = ((heap_segment_t *)ptr) - 1;
    seg->is_allocated = 0;

    /* try to coalesce segements to the left */
    while (seg->prev != NULL && !seg->prev->is_allocated)
    {
        seg->prev->next = seg->next;
        seg->prev->segment_size += seg->segment_size;
        seg->next->prev = seg->prev;
        seg = seg->prev;
    }
    /* try to coalesce segments to the right */
    while (seg->next != NULL && !seg->next->is_allocated)
    {
        seg->next->next->prev = seg;
        seg->segment_size += seg->next->segment_size;
        seg->next = seg->next->next;
    }
}
