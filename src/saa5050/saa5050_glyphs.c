/*
 * saa5050_glyphs.c
 */

#include <common/stdint.h>
#include <saa5050/saa5050_source_glyphs.h>
#include <saa5050/saa5050_glyphs.h>

static uint16_t saa5050_glyphs[MAX_GRAPHICS][GLYPH_HEIGHT];

static void generate_character_glyph(int c) {
    int high_glyph_number = DOUBLE_HEIGHT_GRAPHICS | c;
    int low_glyph_number = high_glyph_number | LOW_FLAG;
    uint16_t raw_glyph[GLYPH_HEIGHT];
    // Double up every source row to generate a 12x20 glyph
    // The first two rows are always blank (not stored in source)
    raw_glyph[0] = 0;
    raw_glyph[1] = 0;
    for (int row = 0; row < SOURCE_GLYPH_ROWS; row++) {
        uint16_t pixels = saa5050_source_glyphs[c][row];
        pixels = ((pixels & 0x10) << 4
            | (pixels & 8) << 3
            | (pixels & 4) << 2
            | (pixels & 2) << 1
            | (pixels & 1));
        pixels |= pixels << 1;
        raw_glyph[row * 2 + 2] = pixels;
        raw_glyph[row * 2 + 3] = pixels;
    }
    // We now have a 12x20 glyph where each source pixel is represented by two pixels
    for (int row = 0; row < GLYPH_HEIGHT; row++) {
        uint16_t bits = raw_glyph[row];
        // Add quarter dots if needed
        if (row > 2 && row < GLYPH_HEIGHT - 1) {
            uint16_t bits2, mask, bit, bit2, bit3, bit4;
            // Work out which source row to compare to. Even scanline = previous row, odd scanline = next row
            if (row % 2 == 0) {
                bits2 = raw_glyph[row - 1];
            } else {
                bits2 = raw_glyph[row + 1];
            }
            for (int col = 1; col < 11; col++) {
                mask = 0x800 >> col;
                bit = bits & mask;
                // If bit is already set, we don't need to do anything
                if (bit == 0) {
                    bit2 = bits2 & mask;
                    // Compare with the next or previous bit
                    if (col % 2 == 0) {
                        // Left side of source pixel: compare with previous pixel on both rows
                        bit3 = bits & (mask << 1);
                        bit4 = bits2 & (mask << 1);
                    } else {
                        // Right side of source pixel: compare with next pixel on both rows
                        bit3 = bits & (mask >> 1);
                        bit4 = bits2 & (mask >> 1);
                    }
                    // If bit is on a diagonal (bits 2 and 3 are set but bit 4 is clear), set it
                    if (bit2 != 0 && bit3 != 0 && bit4 == 0) {
                        bits |= mask;
                    }
                }
            }
        }
        saa5050_glyphs[c][row] = bits;
        if (row < 10) {
            saa5050_glyphs[high_glyph_number][row * 2] = bits;
            saa5050_glyphs[high_glyph_number][row * 2 + 1] = bits;
        } else {
            saa5050_glyphs[low_glyph_number][row * 2 - 20] = bits;
            saa5050_glyphs[low_glyph_number][row * 2 - 19] = bits;
        }
    }
}

static void generate_box_glyphs(int c) {
    int contiguous_glyph_number = BOX_GRAPHICS | c;
    int separated_glyph_number = contiguous_glyph_number | SEPARATED_FLAG;
    uint16_t top = 0x3c0 * (c & 1) | 15 * ((c >> 1) & 1);
    uint16_t middle = 0x3c0 * ((c >> 2) & 1) | 15 * ((c >> 3) & 1);
    uint16_t bottom = 0x3c0 * ((c >> 4) & 1) | 15 * ((c >> 5) & 1);
    uint16_t pixels = top;
    uint16_t pixels2 = pixels | pixels << 2;
    for (int row = 0; row < 20; row++) {
        if (row == 4 || row == 12 || row == 18) {
            pixels = 0;
        } else if (row == 6) {
            pixels = middle;
            pixels2 = pixels | pixels << 2;
        } else if (row == 14) {
            pixels = bottom;
            pixels2 = pixels | pixels << 2;
        }
        saa5050_glyphs[separated_glyph_number][row] = pixels;
        saa5050_glyphs[contiguous_glyph_number][row] = pixels2;
    }
}

void generate_saa5050_glyphs() {
    for (int c = 0; c < SOURCE_GLYPHS; c++) {
        generate_character_glyph(c);
        if (c < 64) {
              generate_box_glyphs(c);
        }
    }
}

uint16_t *get_saa5050_glyph(int c, saa5050flags_t flags) {
    int idx = c & 0x7f;
    if (idx >= 32) {
        idx -= 32;
        if ((flags & FL_BOX_GRAPHICS) && ((idx & 0x20) == 0)) {
            idx = (idx & 0x1f) | ((idx & 0x40) >> 1) | BOX_GRAPHICS
              | SEPARATED_BOX_GRAPHICS(flags);
        } else {
            if (flags & FL_DOUBLE_HEIGHT) {
              idx |= DOUBLE_HEIGHT_GRAPHICS | LOW_CHARACTER_GRAPHICS(flags);
            }
        }
    } else {
        idx = 0;
    }
    return saa5050_glyphs[idx];
}
