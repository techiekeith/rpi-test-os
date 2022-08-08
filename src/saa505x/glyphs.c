/*
 * glyphs.c
 */

#include <common/stdint.h>
#include <saa505x/glyphs.h>
#include <saa505x/source_glyphs.h>
#include <saa505x/unicode_map.h>

static uint16_t saa505x_glyphs[MAX_GRAPHICS][GLYPH_HEIGHT];

static void generate_character_glyph(int c)
{
    int glyph_number = c + 128;
    int high_glyph_number = glyph_number + SOURCE_GLYPHS;
    int low_glyph_number = high_glyph_number + SOURCE_GLYPHS;
    uint16_t raw_glyph[GLYPH_HEIGHT];
    // Double up every source row to generate a 12x20 glyph
    // The first two rows are always blank (not stored in source)
    raw_glyph[0] = 0;
    raw_glyph[1] = 0;
    for (int row = 0; row < SOURCE_GLYPH_ROWS; row++)
    {
        uint16_t pixels = saa505x_source_glyphs[c][row];
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
    for (int row = 0; row < GLYPH_HEIGHT; row++)
    {
        uint16_t bits = raw_glyph[row];
        // Add quarter dots if needed
        if (row > 2 && row < GLYPH_HEIGHT - 1)
        {
            uint16_t bits2, mask, bit, bit2, bit3, bit4;
            // Work out which source row to compare to. Even scanline = previous row, odd scanline = next row
            if (row % 2 == 0)
            {
                bits2 = raw_glyph[row - 1];
            }
            else
            {
                bits2 = raw_glyph[row + 1];
            }
            for (int col = 1; col < 11; col++)
            {
                mask = 0x800 >> col;
                bit = bits & mask;
                // If bit is already set, we don't need to do anything
                if (bit == 0)
                {
                    bit2 = bits2 & mask;
                    // Compare with the next or previous bit
                    if (col % 2 == 0)
                    {
                        // Left side of source pixel: compare with previous pixel on both rows
                        bit3 = bits & (mask << 1);
                        bit4 = bits2 & (mask << 1);
                    }
                    else
                    {
                        // Right side of source pixel: compare with next pixel on both rows
                        bit3 = bits & (mask >> 1);
                        bit4 = bits2 & (mask >> 1);
                    }
                    // If bit is on a diagonal (bits 2 and 3 are set but bit 4 is clear), set it
                    if (bit2 != 0 && bit3 != 0 && bit4 == 0)
                    {
                        bits |= mask;
                    }
                }
            }
        }
        saa505x_glyphs[glyph_number][row] = bits;
        if (row < 10)
        {
            saa505x_glyphs[high_glyph_number][row * 2] = bits;
            saa505x_glyphs[high_glyph_number][row * 2 + 1] = bits;
        }
        else
        {
            saa505x_glyphs[low_glyph_number][row * 2 - 20] = bits;
            saa505x_glyphs[low_glyph_number][row * 2 - 19] = bits;
        }
    }
}

static void generate_box_glyphs(int c) {
    uint16_t top = 0x3c0 * (c & 1) | 15 * ((c >> 1) & 1);
    uint16_t middle = 0x3c0 * ((c >> 2) & 1) | 15 * ((c >> 3) & 1);
    uint16_t bottom = 0x3c0 * ((c >> 4) & 1) | 15 * ((c >> 5) & 1);
    uint16_t pixels = top;
    uint16_t pixels2 = pixels | pixels << 2;
    for (int row = 0; row < 20; row++)
    {
        if (row == 4 || row == 12 || row == 18)
        {
            pixels = 0;
        }
        else if (row == 6)
        {
            pixels = middle;
            pixels2 = pixels | pixels << 2;
        }
        else if (row == 14)
        {
            pixels = bottom;
            pixels2 = pixels | pixels << 2;
        }
        saa505x_glyphs[c][row] = pixels;
        saa505x_glyphs[c + 64][row] = pixels2;
    }
}

void generate_saa505x_glyphs() {
    for (int c = 0; c < SOURCE_GLYPHS; c++)
    {
        if (c < 64)
        {
              generate_box_glyphs(c);
        }
        generate_character_glyph(c);
    }
}

uint16_t *get_saa505x_glyph(int c) {
    if ((c & 0x1ff80) == 0xf500)
    {
        return saa505x_glyphs[c & 0x7f];
    }
    int glyph_number = 0;
    int offset = 128;
    // High/low portions of double-height graphics
    if (c >= 0xf0000) {
        offset += SOURCE_GLYPHS;
        if (c & 0x100000) {
            offset += SOURCE_GLYPHS;
        }
        c &= 0xffff;
    }
    for (int block = 0; block < UNICODE_BLOCKS; block++)
    {
        if (c >= unicode_block_index[block].start && c <= unicode_block_index[block].end)
        {
            glyph_number = unicode_block_index[block].block[c - unicode_block_index[block].start];
        }
    }
    return saa505x_glyphs[glyph_number + offset];
}
