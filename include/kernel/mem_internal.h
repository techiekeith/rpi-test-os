/*
 * mem_internal.h - memory copying routines
 *
 * These routines generally expect pointers to be word-aligned,
 * Data Abort exceptions will likely result if not.
 */

void __memory_set_byte(void *dest, int value, uint32_t bytes);
void __memory_set_short(void *dest, int value, uint32_t bytes);
void __memory_set_int(void *dest, int value, uint32_t bytes);
void __memory_set_int24(void *dest, int value, uint32_t bytes);
int __memory_copy_forwards(void *dest, const void *src, uint32_t bytes);
int __memory_copy_backwards(void *dest, const void *src, uint32_t bytes);

#define MAX_BLOCK_COPY 0x40000000
