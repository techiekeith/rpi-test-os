/*
 * mem.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/string.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/mem.h"

DEBUG_INIT("mem");

extern uint8_t __end;

DEFINE_LIST(page);
IMPLEMENT_LIST(page);

page_t *all_pages_array = NULL;
uint32_t num_pages;
uint32_t page_array_len;
page_list_t free_pages;

void mem_init(uint64_t mem_size)
{
    uint32_t kernel_pages, i;

    DEBUG_START("mem_init");

    /* Get the total number of pages */
    num_pages = (uint32_t)(mem_size / PAGE_SIZE);

    /* Allocate space for all those pages' metadata.  Start this block just after the kernel image is finished */
    page_array_len = sizeof(page_t) * num_pages;
    size_t endp = (size_t)&__end;
    all_pages_array = (page_t *)(endp + KERNEL_HEAP_SIZE);
    memset(all_pages_array, 0, (size_t) page_array_len);
    INITIALIZE_LIST(free_pages);

    /* Iterate over all pages and mark them with the appropriate flags */
    /* Start with kernel pages */
    kernel_pages = endp / PAGE_SIZE;
    for (i = 0; i < kernel_pages; i++)
    {
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;    /* Identity map the kernel pages */
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_page = 1;
    }
    /* Reserve 1 MB for the kernel heap */
    for(; i < kernel_pages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++)
    {
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;    /* Identity map the kernel pages */
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_heap_page = 1;
    }
    /* Map the rest of the pages as unallocated, and add them to the free list */
    for (; i < num_pages; i++)
    {
        all_pages_array[i].flags.allocated = 0;
        append_page_list(&free_pages, &all_pages_array[i]);
    }

    DEBUG_END();
}

void *alloc_page()
{
    page_t *page;
    void *page_mem;

    DEBUG_START("alloc_page");

    if (size_page_list(&free_pages) == 0)
    {
        DEBUG_END();
        return 0;
    }

    /* Get a free page */
    page = pop_page_list(&free_pages);
    page->flags.kernel_page = 1;
    page->flags.allocated = 1;

    /* Get the address the physical page metadata refers to */
    page_mem = (void *)((page - all_pages_array) * PAGE_SIZE);

    /* Zero out the page, big security flaw to not do this :) */
    memset(page_mem, 0, PAGE_SIZE);

    DEBUG_END();

    return page_mem;
}

void free_page(void *ptr)
{
    page_t *page;

    DEBUG_START("free_page");

    /* Get page metadata from the physical address */
    page = all_pages_array + ((size_t)ptr / PAGE_SIZE);

    /* Mark the page as free */
    page->flags.allocated = 0;
    append_page_list(&free_pages, page);

    DEBUG_END();
}


