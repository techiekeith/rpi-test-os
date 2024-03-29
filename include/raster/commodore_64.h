/*
 * commodore_64.h
 */

#pragma once

#include "../common/stdint.h"

uint8_t commodore_64_raster_charset[172][8] = {
        /* C64 US Lower */
        { 0x3c, 0x66, 0x6e, 0x6e, 0x60, 0x62, 0x3c, 0x00 }, /* U+0040 COMMERCIAL AT */
        { 0x00, 0x00, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00 }, /* U+0061 LATIN SMALL LETTER A */
        { 0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x7c, 0x00 }, /* U+0062 LATIN SMALL LETTER B */
        { 0x00, 0x00, 0x3c, 0x60, 0x60, 0x60, 0x3c, 0x00 }, /* U+0063 LATIN SMALL LETTER C */
        { 0x00, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3e, 0x00 }, /* U+0064 LATIN SMALL LETTER D */
        { 0x00, 0x00, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00 }, /* U+0065 LATIN SMALL LETTER E */
        { 0x00, 0x0e, 0x18, 0x3e, 0x18, 0x18, 0x18, 0x00 }, /* U+0066 LATIN SMALL LETTER F */
        { 0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x7c }, /* U+0067 LATIN SMALL LETTER G */
        { 0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x00 }, /* U+0068 LATIN SMALL LETTER H */
        { 0x00, 0x18, 0x00, 0x38, 0x18, 0x18, 0x3c, 0x00 }, /* U+0069 LATIN SMALL LETTER I */
        { 0x00, 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x3c }, /* U+006A LATIN SMALL LETTER J */
        { 0x00, 0x60, 0x60, 0x6c, 0x78, 0x6c, 0x66, 0x00 }, /* U+006B LATIN SMALL LETTER K */
        { 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00 }, /* U+006C LATIN SMALL LETTER L */
        { 0x00, 0x00, 0x66, 0x7f, 0x7f, 0x6b, 0x63, 0x00 }, /* U+006D LATIN SMALL LETTER M */
        { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x00 }, /* U+006E LATIN SMALL LETTER N */
        { 0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* U+006F LATIN SMALL LETTER O */
        { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60 }, /* U+0070 LATIN SMALL LETTER P */
        { 0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x06 }, /* U+0071 LATIN SMALL LETTER Q */
        { 0x00, 0x00, 0x7c, 0x66, 0x60, 0x60, 0x60, 0x00 }, /* U+0072 LATIN SMALL LETTER R */
        { 0x00, 0x00, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x00 }, /* U+0073 LATIN SMALL LETTER S */
        { 0x00, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x0e, 0x00 }, /* U+0074 LATIN SMALL LETTER T */
        { 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x00 }, /* U+0075 LATIN SMALL LETTER U */
        { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00 }, /* U+0076 LATIN SMALL LETTER V */
        { 0x00, 0x00, 0x63, 0x6b, 0x7f, 0x3e, 0x36, 0x00 }, /* U+0077 LATIN SMALL LETTER W */
        { 0x00, 0x00, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x00 }, /* U+0078 LATIN SMALL LETTER X */
        { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x0c, 0x78 }, /* U+0079 LATIN SMALL LETTER Y */
        { 0x00, 0x00, 0x7e, 0x0c, 0x18, 0x30, 0x7e, 0x00 }, /* U+007A LATIN SMALL LETTER Z */
        { 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00 }, /* U+005B LEFT SQUARE BRACKET */
        { 0x0c, 0x12, 0x30, 0x7c, 0x30, 0x62, 0xfc, 0x00 }, /* U+00A3 POUND SIGN */
        { 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00 }, /* U+005D RIGHT SQUARE BRACKET */
        { 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18 }, /* U+2191 UPWARDS ARROW */
        { 0x00, 0x10, 0x30, 0x7f, 0x7f, 0x30, 0x10, 0x00 }, /* U+2190 LEFTWARDS ARROW */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+0020 SPACE */
        { 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00 }, /* U+0021 EXCLAMATION MARK */
        { 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+0022 QUOTATION MARK */
        { 0x66, 0x66, 0xff, 0x66, 0xff, 0x66, 0x66, 0x00 }, /* U+0023 NUMBER SIGN */
        { 0x18, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x18, 0x00 }, /* U+0024 DOLLAR SIGN */
        { 0x62, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x46, 0x00 }, /* U+0025 PERCENT SIGN */
        { 0x3c, 0x66, 0x3c, 0x38, 0x67, 0x66, 0x3f, 0x00 }, /* U+0026 AMPERSAND */
        { 0x06, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+00B4 ACUTE ACCENT */
        { 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00 }, /* U+0028 LEFT PARENTHESIS */
        { 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00 }, /* U+0029 RIGHT PARENTHESIS */
        { 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00 }, /* U+002A ASTERISK */
        { 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00 }, /* U+002B PLUS SIGN */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30 }, /* U+002C COMMA */
        { 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00 }, /* U+002D HYPHEN-MINUS */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 }, /* U+002E FULL STOP */
        { 0x00, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00 }, /* U+002F SOLIDUS */
        { 0x3c, 0x66, 0x6e, 0x76, 0x66, 0x66, 0x3c, 0x00 }, /* U+0030 DIGIT ZERO */
        { 0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7e, 0x00 }, /* U+0031 DIGIT ONE */
        { 0x3c, 0x66, 0x06, 0x0c, 0x30, 0x60, 0x7e, 0x00 }, /* U+0032 DIGIT TWO */
        { 0x3c, 0x66, 0x06, 0x1c, 0x06, 0x66, 0x3c, 0x00 }, /* U+0033 DIGIT THREE */
        { 0x06, 0x0e, 0x1e, 0x66, 0x7f, 0x06, 0x06, 0x00 }, /* U+0034 DIGIT FOUR */
        { 0x7e, 0x60, 0x7c, 0x06, 0x06, 0x66, 0x3c, 0x00 }, /* U+0035 DIGIT FIVE */
        { 0x3c, 0x66, 0x60, 0x7c, 0x66, 0x66, 0x3c, 0x00 }, /* U+0036 DIGIT SIX */
        { 0x7e, 0x66, 0x0c, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* U+0037 DIGIT SEVEN */
        { 0x3c, 0x66, 0x66, 0x3c, 0x66, 0x66, 0x3c, 0x00 }, /* U+0038 DIGIT EIGHT */
        { 0x3c, 0x66, 0x66, 0x3e, 0x06, 0x66, 0x3c, 0x00 }, /* U+0039 DIGIT NINE */
        { 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00 }, /* U+003A COLON */
        { 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30 }, /* U+003B SEMICOLON */
        { 0x0e, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0e, 0x00 }, /* U+003C LESS-THAN SIGN */
        { 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00 }, /* U+003D EQUALS SIGN */
        { 0x70, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x70, 0x00 }, /* U+003E GREATER-THAN SIGN */
        { 0x3c, 0x66, 0x06, 0x0c, 0x18, 0x00, 0x18, 0x00 }, /* U+003F QUESTION MARK */
        { 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00 }, /* U+2500 BOX DRAWINGS LIGHT HORIZONTAL */
        { 0x18, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00 }, /* U+0041 LATIN CAPITAL LETTER A */
        { 0x7c, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x7c, 0x00 }, /* U+0042 LATIN CAPITAL LETTER B */
        { 0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x00 }, /* U+0043 LATIN CAPITAL LETTER C */
        { 0x78, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x78, 0x00 }, /* U+0044 LATIN CAPITAL LETTER D */
        { 0x7e, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7e, 0x00 }, /* U+0045 LATIN CAPITAL LETTER E */
        { 0x7e, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00 }, /* U+0046 LATIN CAPITAL LETTER F */
        { 0x3c, 0x66, 0x60, 0x6e, 0x66, 0x66, 0x3c, 0x00 }, /* U+0047 LATIN CAPITAL LETTER G */
        { 0x66, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00 }, /* U+0048 LATIN CAPITAL LETTER H */
        { 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00 }, /* U+0049 LATIN CAPITAL LETTER I */
        { 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x6c, 0x38, 0x00 }, /* U+004A LATIN CAPITAL LETTER J */
        { 0x66, 0x6c, 0x78, 0x70, 0x78, 0x6c, 0x66, 0x00 }, /* U+004B LATIN CAPITAL LETTER K */
        { 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x00 }, /* U+004C LATIN CAPITAL LETTER L */
        { 0x63, 0x77, 0x7f, 0x6b, 0x63, 0x63, 0x63, 0x00 }, /* U+004D LATIN CAPITAL LETTER M */
        { 0x66, 0x76, 0x7e, 0x7e, 0x6e, 0x66, 0x66, 0x00 }, /* U+004E LATIN CAPITAL LETTER N */
        { 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* U+004F LATIN CAPITAL LETTER O */
        { 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0x00 }, /* U+0050 LATIN CAPITAL LETTER P */
        { 0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x0e, 0x00 }, /* U+0051 LATIN CAPITAL LETTER Q */
        { 0x7c, 0x66, 0x66, 0x7c, 0x78, 0x6c, 0x66, 0x00 }, /* U+0052 LATIN CAPITAL LETTER R */
        { 0x3c, 0x66, 0x60, 0x3c, 0x06, 0x66, 0x3c, 0x00 }, /* U+0053 LATIN CAPITAL LETTER S */
        { 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* U+0054 LATIN CAPITAL LETTER T */
        { 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* U+0055 LATIN CAPITAL LETTER U */
        { 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00 }, /* U+0056 LATIN CAPITAL LETTER V */
        { 0x63, 0x63, 0x63, 0x6b, 0x7f, 0x77, 0x63, 0x00 }, /* U+0057 LATIN CAPITAL LETTER W */
        { 0x66, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x66, 0x00 }, /* U+0058 LATIN CAPITAL LETTER X */
        { 0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x00 }, /* U+0059 LATIN CAPITAL LETTER Y */
        { 0x7e, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x7e, 0x00 }, /* U+005A LATIN CAPITAL LETTER Z */
        { 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18 }, /* U+253C BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL */
        { 0xc0, 0xc0, 0x30, 0x30, 0xc0, 0xc0, 0x30, 0x30 }, /* U+1FB8C LEFT HALF MEDIUM SHADE */
        { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, /* U+2502 BOX DRAWINGS LIGHT VERTICAL */
        { 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc }, /* U+1FB95 CHECKER BOARD FILL */
        { 0x33, 0x99, 0xcc, 0x66, 0x33, 0x99, 0xcc, 0x66 }, /* U+1FB98 UPPER LEFT TO LOWER RIGHT FILL */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+0020 SPACE (repeat!) */
        { 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0 }, /* U+258C LEFT HALF BLOCK */
        { 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff }, /* U+2584 LOWER HALF BLOCK */
        { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+2594 UPPER ONE EIGHTH BLOCK */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff }, /* U+2581 LOWER ONE EIGHTH BLOCK */
        { 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, /* U+258F LEFT ONE EIGHTH BLOCK */
        { 0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33 }, /* U+2592 MEDIUM SHADE */
        { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }, /* U+2595 RIGHT ONE EIGHTH BLOCK */
        { 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x33, 0x33 }, /* U+1FB8F LOWER HALF MEDIUM SHADE */
        { 0xcc, 0x99, 0x33, 0x66, 0xcc, 0x99, 0x33, 0x66 }, /* U+1FB99 UPPER RIGHT TO LOWER LEFT FILL */
        { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }, /* U+1FB87 RIGHT ONE QUARTER BLOCK */
        { 0x18, 0x18, 0x18, 0x1f, 0x1f, 0x18, 0x18, 0x18 }, /* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
        { 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f }, /* U+2597 QUADRANT LOWER RIGHT */
        { 0x18, 0x18, 0x18, 0x1f, 0x1f, 0x00, 0x00, 0x00 }, /* U+2514 BOX DRAWINGS LIGHT UP AND RIGHT */
        { 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x18, 0x18, 0x18 }, /* U+2510 BOX DRAWINGS LIGHT DOWN AND LEFT */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff }, /* U+2582 LOWER ONE QUARTER BLOCK */
        { 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x18, 0x18, 0x18 }, /* U+250C BOX DRAWINGS LIGHT DOWN AND RIGHT */
        { 0x18, 0x18, 0x18, 0xff, 0xff, 0x00, 0x00, 0x00 }, /* U+2534 BOX DRAWINGS LIGHT UP AND HORIZONTAL */
        { 0x00, 0x00, 0x00, 0xff, 0xff, 0x18, 0x18, 0x18 }, /* U+252C BOX DRAWINGS LIGHT DOWN AND HORIZONTAL */
        { 0x18, 0x18, 0x18, 0xf8, 0xf8, 0x18, 0x18, 0x18 }, /* U+2524 BOX DRAWINGS LIGHT VERTICAL AND LEFT */
        { 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, /* U+258E LEFT ONE QUARTER BLOCK */
        { 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0 }, /* U+258D LEFT THREE EIGHTHS BLOCK */
        { 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07 }, /* U+1FB8A RIGHT THREE EIGHTHS BLOCK */
        { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+1FB82 UPPER ONE QUARTER BLOCK */
        { 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+1FB83 UPPER THREE EIGHTHS BLOCK */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff }, /* U+2583 LOWER THREE EIGHTHS BLOCK */
        { 0x01, 0x03, 0x06, 0x6c, 0x78, 0x70, 0x60, 0x00 }, /* U+2713 CHECK MARK */
        { 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0 }, /* U+2596 QUADRANT LOWER LEFT */
        { 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00 }, /* U+259D QUADRANT UPPER RIGHT */
        { 0x18, 0x18, 0x18, 0xf8, 0xf8, 0x00, 0x00, 0x00 }, /* U+2518 BOX DRAWINGS LIGHT UP AND LEFT */
        { 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00 }, /* U+2598 QUADRANT UPPER LEFT */
        { 0xf0, 0xf0, 0xf0, 0xf0, 0x0f, 0x0f, 0x0f, 0x0f }, /* U+259A QUADRANT UPPER LEFT AND LOWER RIGHT */
        /* C64 US Upper, characters 65-90 (various symbols, block and box graphics) */
        { 0x08, 0x1c, 0x3e, 0x7f, 0x7f, 0x1c, 0x3e, 0x00 }, /* [0x0080] U+2660 BLACK SPADE SUIT */
        { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, /* [0x0081] U+1FB72 VERTICAL ONE EIGHTH BLOCK-4 */
        { 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00 }, /* [0x0082] U+1FB78 HORIZONTAL ONE EIGHTH BLOCK-4 */
        { 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 }, /* [0x0083] U+1FB77 HORIZONTAL ONE EIGHTH BLOCK-3 */
        { 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* [0x0084] U+1FB76 HORIZONTAL ONE EIGHTH BLOCK-2 */
        { 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00 }, /* [0x0085] U+1FB7A HORIZONTAL ONE EIGHTH BLOCK-6 */
        { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }, /* [0x0086] U+1FB71 VERTICAL ONE EIGHTH BLOCK-3 */
        { 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c }, /* [0x0087] U+1FB74 VERTICAL ONE EIGHTH BLOCK-6 */
        { 0x00, 0x00, 0x00, 0xe0, 0xf0, 0x38, 0x18, 0x18 }, /* [0x0088] U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
        { 0x18, 0x18, 0x1c, 0x0f, 0x07, 0x00, 0x00, 0x00 }, /* [0x0089] U+2570 BOX DRAWINGS LIGHT ARC UP AND RIGHT */
        { 0x18, 0x18, 0x38, 0xf0, 0xe0, 0x00, 0x00, 0x00 }, /* [0x008a] U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
        { 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff }, /* [0x008b] U+1FB7C LEFT AND LOWER ONE EIGHTH BLOCK */
        { 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x03 }, /* [0x008c] U+2572 BOX DRAWINGS LIGHT DIAGONAL UPPER RIGHT TO LOWER LEFT */
        { 0x03, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 0xc0 }, /* [0x008d] U+2571 BOX DRAWINGS LIGHT DIAGONAL UPPER LEFT TO LOWER RIGHT */
        { 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, /* [0x008e] U+1FB7D LEFT AND UPPER ONE EIGHTH BLOCK */
        { 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }, /* [0x008f] U+1FB7E RIGHT AND UPPER ONE EIGHTH BLOCK */
        { 0x00, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c, 0x00 }, /* [0x0090] U+26AB MEDIUM BLACK CIRCLE */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00 }, /* [0x0091] U+1FB7B HORIZONTAL ONE EIGHTH BLOCK-7 */
        { 0x36, 0x7f, 0x7f, 0x7f, 0x3e, 0x1c, 0x08, 0x00 }, /* [0x0092] U+2665 BLACK HEART SUIT */
        { 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60 }, /* [0x0093] U+1FB70 VERTICAL ONE EIGHTH BLOCK-2 */
        { 0x00, 0x00, 0x00, 0x07, 0x0f, 0x1c, 0x18, 0x18 }, /* [0x0094] U+256D BOX DRAWINGS LIGHT ARC DOWN AND RIGHT */
        { 0xc3, 0xe7, 0x7e, 0x3c, 0x3c, 0x7e, 0xe7, 0xc3 }, /* [0x0095] U+2573 BOX DRAWINGS LIGHT DIAGONAL CROSS */
        { 0x00, 0x3c, 0x7e, 0x66, 0x66, 0x7e, 0x3c, 0x00 }, /* [0x0096] U+26AA MEDIUM WHITE CIRCLE */
        { 0x18, 0x18, 0x66, 0x66, 0x18, 0x18, 0x3c, 0x00 }, /* [0x0097] U+2663 BLACK CLUB SUIT */
        { 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06 }, /* [0x0098] U+1FB75 VERTICAL ONE EIGHTH BLOCK-7 */
        { 0x08, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c, 0x08, 0x00 }, /* [0x0099] U+2666 BLACK DIAMOND SUIT */
        /* C64 US Upper, characters 94-95 (pi, upper-right triangle) */
        { 0x00, 0x00, 0x03, 0x3e, 0x76, 0x36, 0x36, 0x00 }, /* [0x009a] U+03C0 GREEK SMALL LETTER PI */
        { 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01 }, /* [0x009b] U+25E5 BLACK UPPER RIGHT TRIANGLE */
        /* C64 US Upper, character 105 (upper-left triangle) */
        { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80 }, /* [0x009c] U+25E4 BLACK UPPER LEFT TRIANGLE */
        /* C64 US Upper, character 122 (right/lower 1/8 block) */
        { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff }, /* [0x009d] U+1FB7F RIGHT AND LOWER ONE EIGHTH BLOCK */
        /* Substitute ASCII reverse solidus character */
        { 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x00 }, /* [0x009e] U+005C REVERSE SOLIDUS */
        /* Symbols derived from SuperPET versions */
        { 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x7a, 0x60, 0x60 }, /* [0x009f] U+00B5 MICRO SIGN */
        { 0x00, 0x18, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x00 }, /* [0x00a0] U+005E CIRCUMFLEX ACCENT */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe }, /* [0x00a1] U+005F LOW LINE */
        { 0x18, 0x0c, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* [0x00a2] U+0060 GRAVE ACCENT */
        { 0x1c, 0x30, 0x30, 0x60, 0x30, 0x30, 0x1c, 0x00 }, /* [0x00a3] U+007B LEFT CURLY BRACKET */
        { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* [0x00a4] U+007C VERTICAL BAR */
        { 0x38, 0x0c, 0x0c, 0x06, 0x0c, 0x0c, 0x38, 0x00 }, /* [0x00a5] U+007D RIGHT CURLY BRACKET */
        { 0x00, 0x00, 0x00, 0x62, 0xd6, 0x8c, 0x00, 0x00 }, /* [0x00a6] U+007E TILDE */
        /* Substitute ASCII apostrophe character */
        { 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* [0x00a7] U+0027 APOSTROPHE */
        /* General Punctuation */
        { 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00 }, /* [0x00a8] U+2014 EM DASH */
        /* Geometric Shapes */
        { 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x00 }, /* [0x00a9] U+25A0 BLACK SQUARE */
        { 0xfe, 0x82, 0x82, 0x82, 0x82, 0x82, 0xfe, 0x00 }, /* [0x00aa] U+25A1 WHITE SQUARE */
        /* Replacement character */
        { 0xc3, 0x99, 0xf9, 0xf3, 0xe7, 0xff, 0xe7, 0xff }, /* [0x00ab] U+FFFD REPLACEMENT CHARACTER */
};
