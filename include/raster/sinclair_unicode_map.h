/*
 * bbc_micro_unicode_map.h
 */

#pragma once

#include "../kernel/charset.h"

/*
 * Basic Latin (U+0020-007F)
 */
static int zx_basic_latin[0x60] = {
        0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x075, 0x008, 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f,
        0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017, 0x018, 0x019, 0x01a, 0x01b, 0x01c, 0x01d, 0x01e, 0x01f,
        0x020, 0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x027, 0x028, 0x029, 0x02a, 0x02b, 0x02c, 0x02d, 0x02e, 0x02f,
        0x030, 0x031, 0x032, 0x033, 0x034, 0x035, 0x036, 0x037, 0x038, 0x039, 0x03a, 0x03b, 0x03c, 0x03d, 0x076, 0x03f,
        0x077, 0x041, 0x042, 0x043, 0x044, 0x045, 0x046, 0x047, 0x048, 0x049, 0x04a, 0x04b, 0x04c, 0x04d, 0x04e, 0x04f,
        0x050, 0x051, 0x052, 0x053, 0x054, 0x055, 0x056, 0x057, 0x058, 0x059, 0x05a, 0x05b, 0x05c, 0x05d, 0x05e, -0x01
};

/*
 * Latin-1 Supplement (U+00A0-00FF)
 */
static int zx_latin1_supplement[0x20] = {
        0x000, -0x01, -0x01, 0x040, -0x01, -0x01, -0x01, -0x01, -0x01, 0x05f, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, 0x007, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01
};

/*
 * General Punctuation (U+2000-206F) -- block starts at U+2010
 */
static int zx_general_punctuation[0x18] = {
        -0x01, -0x01, -0x01, -0x01, 0x07b, -0x01, -0x01, -0x01, 0x077, 0x007, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01,
        -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01, -0x01
};

/*
 * Arrows (U+2190-21FF)
 */
static int zx_arrows[4] = {
        0x078, 0x03e, 0x079, 0x07a
};

/*
 * Block Elements (U+2580-259F)
 */
static int zx_block_elements[32] = {
        0x062, -0x01, -0x01, -0x01, 0x06b, -0x01, -0x01, -0x01, 0x06e, -0x01, -0x01, -0x01, 0x069, -0x01, -0x01, -0x01,
        0x064, -0x01, 0x06f, -0x01, -0x01, -0x01, 0x067, 0x063, 0x061, 0x06d, 0x065, 0x06a, 0x066, 0x060, 0x068, 0x06c
};

/*
 * Geometric Shapes (U+25A0-25A1)
 */
static int zx_geometric_shapes[2] = {
        0x07c, 0x07d
};

/*
 * Specials (U+FFF0-FFFF) -- block starts at U+FFFD
 */
static int zx_specials[1] = {
        0x07e
};

/*
 * Legacy Computing Symbols (U+1FB00-1FBFF) -- block starts at U+1FB8E
 */
static int zx_legacy_computing_symbols[5] = {
        0x070, 0x071, 0x072, 0x073, 0x074
};

#define SINCLAIR_UNICODE_BLOCKS 8

unicode_block_idx_t sinclair_unicode_block_index[SINCLAIR_UNICODE_BLOCKS] = {
        { .start =  0x0020, .end =  0x007f, .block = zx_basic_latin },
        { .start =  0x00a0, .end =  0x00bf, .block = zx_latin1_supplement },
        { .start =  0x2010, .end =  0x2027, .block = zx_general_punctuation },
        { .start =  0x2190, .end =  0x2193, .block = zx_arrows },
        { .start =  0x2580, .end =  0x259f, .block = zx_block_elements },
        { .start =  0x25a0, .end =  0x25a1, .block = zx_geometric_shapes },
        { .start =  0xfffd, .end =  0xfffd, .block = zx_specials },
        { .start = 0x1fb8e, .end = 0x1fb92, .block = zx_legacy_computing_symbols },
};
