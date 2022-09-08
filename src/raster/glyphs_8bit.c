/*
 * glyphs_8bit.c - routines for 8x8 raster graphics
 */

#include "../../include/common/stdint.h"
#include "../../include/raster/bbc_micro.h"
#include "../../include/raster/bbc_micro_unicode_map.h"

uint8_t *get_bbc_micro_glyph(int c)
{
    int glyph_number = 95; /* filled square */
    for (int block = 0; block < BBC_MICRO_UNICODE_BLOCKS; block++)
    {
        if (c >= bbc_micro_unicode_block_index[block].start && c <= bbc_micro_unicode_block_index[block].end)
        {
            glyph_number = bbc_micro_unicode_block_index[block].block[c - bbc_micro_unicode_block_index[block].start];
        }
    }
    return bbc_micro_raster_charset[glyph_number];
}
