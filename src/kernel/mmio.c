/*
 * mmio.c - memory-mapped I/O
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"

/* Memory-Mapped I/O output */
void mmio_write(size_t reg, size_t data)
{
    *(volatile size_t *)reg = data;
}

void mmio_write_with_mask(size_t reg, volatile void *buffer, uint32_t mask)
{
    mmio_write(reg, *((size_t *)buffer) & mask);
}

void mmio_write_out(size_t reg, volatile void *buffer, size_t words)
{
    volatile size_t *word_aligned_buffer = (size_t *)buffer;
    volatile size_t *memory_address = (size_t *)reg;
    for (; words; words--)
    {
        *memory_address++ = *word_aligned_buffer++;
    }
}

/* Memory-Mapped I/O input */
uint32_t mmio_read(size_t reg)
{
    return *(volatile size_t *)reg;
}

void mmio_read_in(size_t reg, volatile void *buffer, size_t words)
{
    volatile size_t *word_aligned_buffer = (size_t *)buffer;
    volatile size_t *memory_address = (size_t *)reg;
    for (; words; words--)
    {
        *word_aligned_buffer++ = *memory_address++;
    }
}
