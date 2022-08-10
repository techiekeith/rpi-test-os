/*
 * mmio.c - memory-mapped I/O
 */

#include "../../include/common/stdint.h"

/* Memory-Mapped I/O output */
void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

/* Memory-Mapped I/O input */
uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}
