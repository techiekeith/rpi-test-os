/*
 * saa5050_glyphs.h
 */

#pragma once

#include <common/stdint.h>

#define SOURCE_GLYPHS               96
#define SOURCE_GLYPH_ROWS           9

#define BOX_GRAPHICS                0x100
#define SEPARATED_FLAG              0x40
#define DOUBLE_HEIGHT_GRAPHICS      0x200
#define LOW_FLAG                    0x100
#define MAX_GRAPHICS                0x400

#define GLYPH_HEIGHT                20
#define GLYPH_WIDTH                 12

#define FL_DOUBLE_HEIGHT            0x01
#define FL_LOW_CHARACTER_GRAPHICS   0x02
#define FL_BOX_GRAPHICS             0x04
#define FL_SEPARATED_BOX_GRAPHICS   0x08
#define FL_CONCEAL                  0x10
#define FL_FLASH                    0x20
#define FL_HELD_GRAPHICS            0x40
#define FL_DOUBLE_HEIGHT_LINE       0x80

#define SEPARATED_BOX_GRAPHICS(x)   ((x & FL_SEPARATED_BOX_GRAPHICS) << 3)
#define LOW_CHARACTER_GRAPHICS(x)   ((x & FL_LOW_CHARACTER_GRAPHICS) << 7)

typedef uint16_t saa5050flags_t;

void generate_saa5050_glyphs();
uint16_t *get_saa5050_glyph(int c, saa5050flags_t flags);
