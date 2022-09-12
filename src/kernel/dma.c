/*
 * dma.c
 */

#include "../../include/common/stdbool.h"
#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/string.h"
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/dma.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/mmio.h"

static void dma_interrupt_handler(void *unused)
{
    (void) unused;
    dma_control_status_register_t control_register;
    control_register.active = false;
    control_register.end = true;
    control_register.int_status = true;
    control_register.reset = true;
    mmio_write_out(DMA0_BASE + DMA_CS, &control_register, 1);
    __dmb();
}

void dma_init()
{
    dma_control_status_register_t control_register;
    control_register.reset = true;
    mmio_write_out(DMA0_BASE + DMA_CS, &control_register, 1);
    __dmb();
    register_irq_handler(DMA0_INT, NULL, NULL, dma_interrupt_handler, NULL);
}

static void set_control_block(dma_control_block_t *control_block, void *dest, const void *src, size_t bytes,
                              bool src_inc, bool src_ignore, bool wide, bool int_en)
{
    memset(control_block, 0, sizeof(dma_control_block_t));
    control_block->transfer_information.int_en = int_en;
    control_block->transfer_information.dest_inc = true;
    control_block->transfer_information.dest_width = wide;
    control_block->transfer_information.src_inc = src_inc;
    control_block->transfer_information.src_ignore = src_ignore;
    control_block->transfer_information.src_width = wide;
    control_block->source_ad = (size_t) src;
    control_block->dest_ad = (size_t) dest;
    control_block->txfr_len = bytes;
}

static void dma_start_transfer()
{
    dma_control_status_register_t control_register;
    control_register.active = true;
    mmio_write_out(DMA0_BASE + DMA_CS, &control_register, 1);
    __dmb();
    do {
        asm("wfi");
        __dmb();
        mmio_read_in(DMA0_BASE + DMA_CS, &control_register, 1);
    } while (control_register.active);
}

static void dma_transfer(void *dest, const void *src, size_t bytes, bool src_inc, bool src_ignore, bool wide)
{
    dma_control_block_t *control_block = DMA_CONTROL_BLOCK_BASE;
    set_control_block(control_block, dest, src, bytes, src_inc, src_ignore, wide, true);
    mmio_write_out(DMA0_BASE + DMA_CONBLK_AD, &control_block, 1);
    dma_start_transfer();
}

void dma_copy(void *dest, const void *src, size_t bytes)
{
    dma_transfer(dest, src, bytes, true, false, bytes % 16 == 0);
}

void dma_copy_multiple(void *dest, const void *src, size_t bytes, int ntimes, int seek)
{
    dma_control_block_t *cb = DMA_CONTROL_BLOCK_BASE;
    void *p = (void *)src;
    void *q = dest;
    bool wide = bytes % 16 == 0;
    dma_control_block_t *r = cb;
    int count = MAX_DMA_CONTROL_BLOCKS;
    while (ntimes--)
    {
        set_control_block(r, q, p, bytes, true, false, wide, false);
        count--;
        if (!count || !ntimes)
        {
            r->transfer_information.int_en = true;
            mmio_write_out(DMA0_BASE + DMA_CONBLK_AD, &cb, 1);
            dma_start_transfer();
        }
        if (ntimes)
        {
            if (count)
            {
                dma_control_block_t *s = r + 1;
                r->nextconbk = (uint32_t) s;
                r = s;
            }
            else
            {
                r = cb;
                count = MAX_DMA_CONTROL_BLOCKS;
            }
            q += seek;
            p += seek;
        }
    }
}

void dma_set(void *dest, uint32_t value, size_t bytes)
{
    uint32_t values[8];
    if (bytes % 16 == 0)
    {
        for (int i = 7; i >= 0; i--) values[i] = value;
        dma_transfer(dest, &values, bytes, false, false, true);
    }
    else
    {
        dma_transfer(dest, &value, bytes, false, false, false);
    }
}

void dma_zero(void *dest, size_t bytes)
{
    dma_transfer(dest, NULL, bytes, false, true, bytes % 16 == 0);
}
