/*
 * glyphs.h
 */

#pragma once

#include "../common/stdint.h"

#define SAA505X_SOURCE_GLYPHS       321
#define SAA505X_SOURCE_GLYPH_ROWS   9
#define SAA505X_MAX_GRAPHICS        (SAA505X_SOURCE_GLYPHS * 3 + 128)

#define SAA505X_GLYPH_HEIGHT        20
#define SAA505X_GLYPH_WIDTH         12

void generate_saa505x_glyphs();
uint16_t *get_saa505x_glyph(int c);
