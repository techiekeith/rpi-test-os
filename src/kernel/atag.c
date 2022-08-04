/*
 * atag.c
 */
 
#include <common/stdio.h>
#include <common/stdlib.h>
#include <kernel/atag.h>

uint32_t get_mem_size(atag_t *tag)
{
   while (tag->tag != NONE)
   {
       if (tag->tag == MEM)
       {
           puts("Memory size: ");
           puts(itoa(tag->mem.size));
           putc('\n');
           return tag->mem.size;
       }
       else
       {
           puts("Tag: ");
           puts(itoa(tag->tag));
           putc('\n');
           char *p = (char *)tag;
           for (uint32_t i = 0; i < tag->tag_size; i++)
           {
               int a = (int) *p;
               puts(itoa(a));
               p++;
           }
           putc('\n');
       }
       tag = (atag_t *)(((uint32_t *)tag) + tag->tag_size);
   }
   return 0;
}
