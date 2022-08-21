/*
 * atag.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/atag.h"

size_t get_mem_size(atag_t *tag) {
    size_t mem = 0;
    if (tag) {
        while (tag->tag != NONE) {
            switch (tag->tag) {
                case CORE:
                    printf("atag [CORE]: %08x ... .\r\n", tag->data);
                    break;
                case MEM:
                    printf("atag [MEM]: start %08x size %08x.\r\n", tag->mem.start, tag->mem.size);
                    mem = (size_t)(tag->mem.size);
                    break;
                case INITRD2:
                    printf("atag [INITRD2]: start %08x size %08x.\r\n", tag->initrd2.start, tag->initrd2.size);
                    break;
                case CMDLINE:
                    printf("atag [CMDLINE]: command: \"");
                    for (char *p = tag->cmdline.line; *p != '\0' && *p != '\r' && *p != '\n'; p++)
                    {
                        putc(*p);
                    }
                    printf("\".\r\n");
                    break;
                default:
                    printf("atag [%08x]: %08x .... .", tag->tag, tag->data);
                    break;
            }
            tag = (atag_t *) (((uint32_t *) tag) + tag->tag_size);
            printf("atag: %x.\r\n", tag->mem.size);
        }
    }
    if (mem == 0) {
#ifdef BCM2835
        mem = 0x20000000ULL; /* 512MB defined in run configuration */
#else
        mem = 0x40000000ULL; /* 1GB defined in run configuration */
#endif
        printf("atag [NO MEM]: size %08x.\r\n", mem);
    }
    return mem;
}
