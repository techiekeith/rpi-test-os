/*
 * glyphs_8bit.c - routines for 8x8 raster graphics
 */

#include "../../include/common/stdint.h"
#include "../../include/raster/acorn.h"
#include "../../include/raster/acorn_unicode_map.h"
#include "../../include/raster/sinclair.h"
#include "../../include/raster/sinclair_unicode_map.h"

static uint8_t *get_raster_glyph(int c, int default_glyph_number, int max_blocks,
                                 unicode_block_idx_t *block_index, uint8_t charset[][8])
{
    int glyph_number = default_glyph_number;
    for (int block = 0; block < max_blocks; block++)
    {
        if (c >= block_index[block].start && c <= block_index[block].end)
        {
            glyph_number = block_index[block].block[c - block_index[block].start];
        }
    }
    return charset[glyph_number];
}

uint8_t *get_acorn_glyph(int c)
{
    return get_raster_glyph(c, 0x0e2 /* U+25A1 WHITE SQUARE */, ACORN_UNICODE_BLOCKS,
                            acorn_unicode_block_index, acorn_raster_charset);
}

uint8_t *get_sinclair_glyph(int c)
{
    return get_raster_glyph(c, 0x07d /* U+25A1 WHITE SQUARE */, SINCLAIR_UNICODE_BLOCKS,
                            sinclair_unicode_block_index, sinclair_raster_charset);
}