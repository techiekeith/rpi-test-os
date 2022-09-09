/*
 * glyphs_8bit.h - routines for 8x8 raster graphics
 */

#include "../../include/common/stdint.h"

#define RASTER_8BIT_GLYPH_WIDTH     8
#define RASTER_8BIT_GLYPH_HEIGHT    8

uint8_t *get_acorn_glyph(int c);
uint8_t *get_sinclair_glyph(int c);
