/*
 * mmio.c - memory-mapped I/O
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"

/* Memory-Mapped I/O output */
void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

void mmio_write_with_mask(uint32_t reg, volatile void *buffer, uint32_t mask)
{
    mmio_write(reg, *((uint32_t *)buffer) & mask);
}

void mmio_write_out(uint32_t reg, volatile void *buffer, size_t words)
{
    volatile uint32_t *word_aligned_buffer = (uint32_t *)buffer;
    volatile uint32_t *memory_address = (uint32_t *)reg;
    for (; words; words--)
    {
        *memory_address++ = *word_aligned_buffer++;
    }
}

/* Memory-Mapped I/O input */
uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

void mmio_read_in(uint32_t reg, volatile void *buffer, size_t words)
{
    volatile uint32_t *word_aligned_buffer = (uint32_t *)buffer;
    volatile uint32_t *memory_address = (uint32_t *)reg;
    for (; words; words--)
    {
        *word_aligned_buffer++ = *memory_address++;
    }
}
