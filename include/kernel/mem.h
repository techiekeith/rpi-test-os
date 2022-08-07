/*
 * mem.h
 */

#pragma once

#include <common/stddef.h>
#include <common/stdint.h>
#include <kernel/atag.h>
#include <kernel/list.h>

#define PAGE_SIZE 4096
#define KERNEL_HEAP_SIZE (1024*1024*32)

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


uint64_t mem_init(atag_t *atags);

void *alloc_page();
void free_page(void *ptr);

void *kmalloc(size_t bytes);
void kfree(void *ptr);
