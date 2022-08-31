/*
 * glyphs_8bit.c - routines for 8x8 raster graphics
 */

#include "../../include/common/stdint.h"
#include "../../include/raster/bbc_micro.h"

uint8_t *get_bbc_micro_glyph(int c)
{
    return bbc_micro_raster_charset[(c < 32 || c > 126) ? 0 : c - 32];
}
