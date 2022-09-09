/*
 * fifo.c
 */

#include "../../include/common/stdbool.h"
#include "../../include/common/stdint.h"
#include "../../include/common/string.h"
#include "../../include/kernel/fifo.h"

void fifo_init(fifo_t *fifo, unsigned char *buffer, uint32_t size)
{
    memset(fifo, 0, sizeof(fifo_t));
    memset(buffer, 0, size);
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->empty = true;
}

bool fifo_add(fifo_t *fifo, unsigned char value)
{
    if (fifo->full) return false;
    fifo->buffer[fifo->end++] = value;
    fifo->end %= fifo->size;
    fifo->empty = false;
    if (fifo->end == fifo->start) fifo->full = true;
    return true;
}

bool fifo_get(fifo_t *fifo, unsigned char *value)
{
    if (fifo->empty) return false;
    *value = fifo->buffer[fifo->start++];
    fifo->start %= fifo->size;
    fifo->full = false;
    if (fifo->end == fifo->start) fifo->empty = true;
    return true;
}
