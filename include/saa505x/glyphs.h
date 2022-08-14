/*
 * glyphs.h
 */

#pragma once

#include "../common/stdint.h"

#define SOURCE_GLYPHS               321
#define SOURCE_GLYPH_ROWS           9
#define MAX_GRAPHICS                (SOURCE_GLYPHS * 3 + 128)

#define GLYPH_HEIGHT                20
#define GLYPH_WIDTH                 12

void generate_saa505x_glyphs();
uint16_t *get_saa505x_glyph(int c);
