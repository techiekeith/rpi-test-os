/*
 * mem.h
 */

#pragma once

#include "../common/stddef.h"
#include "../common/stdint.h"
#include "list.h"

#define PAGE_SIZE           65536

typedef struct
{
	uint8_t allocated: 1;			// This page is allocated to something
	uint8_t kernel_page: 1;			// This page is a part of the kernel
	uint8_t kernel_heap_page: 1;	// This page is a part of the kernel
	uint32_t reserved: 29;
} page_flags_t;

typedef struct page
{
	uint32_t vaddr_mapped;			// The virtual address that maps to this page
	page_flags_t flags;
	DEFINE_LINK(page);
} page_t;

void mem_init(uint64_t mem_size);
void *alloc_page();
void free_page(void *ptr);
