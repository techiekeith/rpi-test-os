/*
 * atag.c
 */
 
#include <common/stdio.h>
#include <common/stdlib.h>
#include <kernel/atag.h>

uint32_t get_mem_size(atag_t *tag)
{
    if (!tag)
    {
        return 1024 * 1024 * 1024; /* 1GB defined in run.bat */
    }
    while (tag->tag != NONE)
    {
        if (tag->tag == MEM)
        {
            return tag->mem.size;
        }
        tag = (atag_t *)(((uint32_t *)tag) + tag->tag_size);
    }
    return 0;
}
