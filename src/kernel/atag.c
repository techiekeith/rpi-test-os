/*
 * atag.c
 */
 
#include <common/stdint.h>
#include <kernel/atag.h>

uint64_t get_mem_size(atag_t *tag)
{
    if (tag)
    {
        while (tag->tag != NONE)
        {
            if (tag->tag == MEM)
            {
                return (uint64_t)(tag->mem.size);
            }
            tag = (atag_t *)(((uint32_t *)tag) + tag->tag_size);
        }
    }
#if (MACHINE == raspi1ap)
    return 0x20000000ULL; /* 512MB defined in run.bat */
#else
    return 0x40000000ULL; /* 1GB defined in run.bat */
#endif
}
