/*
 * commodore_64_unicode_map.h
 */

#pragma once

#include "../kernel/charset.h"

/*
 * Basic Latin (U+0020-007F)
 */
static int c64_basic_latin[0x60] = {
        0x020, 0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x0a7, 0x028, 0x029, 0x02a, 0x02b, 0x02c, 0x02d, 0x02e, 0x02f,
        0x030, 0x031, 0x032, 0x033, 0x034, 0x035, 0x036, 0x037, 0x038, 0x039, 0x03a, 0x03b, 0x03c, 0x03d, 0x03e, 0x03f,
        0x000, 0x041, 0x042, 0x043, 0x044, 0x045, 0x046, 0x047, 0x048, 0x049, 0x04a, 0x04b, 0x04c, 0x04d, 0x04e, 0x04f,
        0x050, 0x051, 0x052, 0x053, 0x054, 0x055, 0x056, 0x057, 0x058, 0x059, 0x05a, 0x01b, 0x09e, 0x01d, 0x0a0, 0x0a1,
        0x0a2, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007, 0x008, 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f,
        0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017, 0x018, 0x019, 0x01a, 0x0a3, 0x0a4, 0x0a5, 0x0a6, -0x01
};

/*
 * Latin-1 Supplement (U+00A0-00FF)
 */
static int c64_latin1_supplement[0x20] = {
        0x020, -0x01, -0x01, 0x01c, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, 0x027, 0x09f, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01
};

/*
 * Greek (U+0380-04FF) -- block starts at U+03C0
 */
static int c64_greek[1] = {
        0x09a
};

/*
 * General Punctuation (U+2000-206F) -- block starts at U+2010
 */
static int c64_general_punctuation[0x18] = {
        -0x01, -0x01, -0x01, -0x01, 0x0a8, -0x01, -0x01, -0x01, 0x0a2, 0x027, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01
};

/*
 * Arrows (U+2190-21FF)
 */
static int c64_arrows[4] = {
        0x01f, 0x01e, -0x01, -0x01
};

/*
 * Box Drawings (U+2500-257F)
 */
static int c64_box_drawings[0x80] = {
        0x040, -0x01, 0x05d, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, 0x070, -0x01, -0x01, -0x01,
        0x06e, -0x01, -0x01, -0x01, 0x06d, -0x01, -0x01, -0x01, 0x07d, -0x01, -0x01, -0x01, 0x06b, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, 0x073, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, 0x072, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, 0x071, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, 0x05b, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, 0x094, 0x088, 0x08a,
        0x089, 0x08d, 0x08c, 0x095, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01
};

/*
 * Block Elements (U+2580-259F)
 */
static int c64_block_elements[0x20] = {
        -0x01, 0x064, 0x06f, 0x079, 0x062, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, 0x061, 0x075, 0x074, 0x065,
        -0x01, -0x01, 0x066, -0x01, 0x063, 0x067, 0x07b, 0x06c, 0x07e, -0x01, 0x07f, -0x01, -0x01, 0x07c, -0x01, -0x01
};

/*
 * Geometric Shapes (U+25A0-25A1)
 */
static int c64_geometric_shapes[2] = {
        0x0a9, 0x0aa
};

/*
 * Geometric Shapes (U+25A0-25A1) -- this block starts at U+25E4
 */
static int c64_geometric_shapes_25e4[2] = {
        0x09c, 0x09b
};

/*
 * Miscellaneous Symbols (U+2600-26FF) -- block 1 starts at U+2660
 */
static int c64_misc_symbols_1[7] = {
        0x080, -0x01, -0x01, 0x097, -0x01, 0x092, 0x099
};

/*
 * Miscellaneous Symbols (U+2600-26FF) -- block 2 starts at U+26aa
 */
static int c64_misc_symbols_2[2] = {
        0x096, 0x090
};

/*
 * Dingbats (U+2700-27BF) -- block starts at U+2713
 */
static int c64_dingbats[1] = {
        0x07a
};

/*
 * Specials (U+FFF0-FFFF) -- block starts at U+FFFD
 */
static int c64_specials[1] = {
        0x0ab
};

/*
 * Legacy Computing Symbols (U+1FB00-1FBFF) -- block starts at U+1FB70
 */
static int c64_legacy_computing_symbols[0x30] = {
        0x093, 0x086, 0x081, -0x01, 0x087, 0x098, 0x084, 0x083, 0x082, -0x01, 0x085, 0x091, 0x08b, 0x08e, 0x08f, 0x09d,
        -0x01, -0x01, 0x077, 0x078, -0x01, -0x01, -0x01, 0x06a, -0x01, -0x01, 0x076, -0x01, 0x05c, -0x01, -0x01, 0x068,
        -0x01, -0x01, -0x01, -0x01, -0x01, 0x05e, -0x01, -0x01, 0x05f, 0x069, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01
};

#define COMMODORE_64_UNICODE_BLOCKS 14

unicode_block_idx_t commodore_64_unicode_block_index[COMMODORE_64_UNICODE_BLOCKS] = {
        { .start =  0x0020, .end =  0x007f, .block = c64_basic_latin },
        { .start =  0x00a0, .end =  0x00bf, .block = c64_latin1_supplement },
        { .start =  0x03c0, .end =  0x03c0, .block = c64_greek },
        { .start =  0x2010, .end =  0x2027, .block = c64_general_punctuation },
        { .start =  0x2190, .end =  0x2193, .block = c64_arrows },
        { .start =  0x2500, .end =  0x257f, .block = c64_box_drawings },
        { .start =  0x2580, .end =  0x259f, .block = c64_block_elements },
        { .start =  0x25a0, .end =  0x25a1, .block = c64_geometric_shapes },
        { .start =  0x25e4, .end =  0x25e5, .block = c64_geometric_shapes_25e4 },
        { .start =  0x2660, .end =  0x2666, .block = c64_misc_symbols_1 },
        { .start =  0x26aa, .end =  0x26ab, .block = c64_misc_symbols_2 },
        { .start =  0x2713, .end =  0x2713, .block = c64_dingbats },
        { .start =  0xfffd, .end =  0xfffd, .block = c64_specials },
        { .start = 0x1fb70, .end = 0x1fb9f, .block = c64_legacy_computing_symbols },
};
