/*
 * acorn.h
 */

#pragma once

#include "../common/stdint.h"

uint8_t acorn_raster_charset[228][8] = {
        /* 0-95: Original BBC Microcomputer raster characters (MOS 1.20) */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+0020 SPACE */
        { 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00 }, /* U+0021 EXCLAMATION MARK */
        { 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+0022 QUOTATION MARK */
        { 0x36, 0x36, 0x7f, 0x36, 0x7f, 0x36, 0x36, 0x00 }, /* U+0023 NUMBER SIGN */
        { 0x0c, 0x3f, 0x68, 0x3e, 0x0b, 0x7e, 0x18, 0x00 }, /* U+0024 DOLLAR SIGN */
        { 0x60, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x06, 0x00 }, /* U+0025 PERCENT SIGN */
        { 0x38, 0x6c, 0x6c, 0x38, 0x6d, 0x66, 0x3b, 0x00 }, /* U+0026 AMPERSAND */
        { 0x0c, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+2019 RIGHT SINGLE QUOTATION MARK */
        { 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00 }, /* U+0028 LEFT PARENTHESIS */
        { 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00 }, /* U+0029 RIGHT PARENTHESIS */
        { 0x00, 0x18, 0x7e, 0x3c, 0x7e, 0x18, 0x00, 0x00 }, /* U+002A ASTERISK */
        { 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00 }, /* U+002B PLUS SIGN */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30 }, /* U+002C COMMA */
        { 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00 }, /* U+002D HYPHEN-MINUS */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 }, /* U+002E FULL STOP */
        { 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x00 }, /* U+002F SOLIDUS */
        { 0x3c, 0x66, 0x6e, 0x7e, 0x76, 0x66, 0x3c, 0x00 }, /* U+0030 DIGIT ZERO */
        { 0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00 }, /* U+0031 DIGIT ONE */
        { 0x3c, 0x66, 0x06, 0x0c, 0x18, 0x30, 0x7e, 0x00 }, /* U+0032 DIGIT TWO */
        { 0x3c, 0x66, 0x06, 0x1c, 0x06, 0x66, 0x3c, 0x00 }, /* U+0033 DIGIT THREE */
        { 0x0c, 0x1c, 0x3c, 0x6c, 0x7e, 0x0c, 0x0c, 0x00 }, /* U+0034 DIGIT FOUR */
        { 0x7e, 0x60, 0x7c, 0x06, 0x06, 0x66, 0x3c, 0x00 }, /* U+0035 DIGIT FIVE */
        { 0x1c, 0x30, 0x60, 0x7c, 0x66, 0x66, 0x3c, 0x00 }, /* U+0036 DIGIT SIX */
        { 0x7e, 0x06, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00 }, /* U+0037 DIGIT SEVEN */
        { 0x3c, 0x66, 0x66, 0x3c, 0x66, 0x66, 0x3c, 0x00 }, /* U+0038 DIGIT EIGHT */
        { 0x3c, 0x66, 0x66, 0x3e, 0x06, 0x0c, 0x38, 0x00 }, /* U+0039 DIGIT NINE */
        { 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00 }, /* U+003A COLON */
        { 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x30 }, /* U+003B SEMICOLON */
        { 0x0c, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0c, 0x00 }, /* U+003C LESS-THAN SIGN */
        { 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00 }, /* U+003D EQUALS SIGN */
        { 0x30, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x30, 0x00 }, /* U+003E GREATER-THAN SIGN */
        { 0x3c, 0x66, 0x0c, 0x18, 0x18, 0x00, 0x18, 0x00 }, /* U+003F QUESTION MARK */
        { 0x3c, 0x66, 0x6e, 0x6a, 0x6e, 0x60, 0x3c, 0x00 }, /* U+0040 COMMERCIAL AT */
        { 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00 }, /* U+0041 LATIN CAPITAL LETTER A */
        { 0x7c, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x7c, 0x00 }, /* U+0042 LATIN CAPITAL LETTER B */
        { 0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x00 }, /* U+0043 LATIN CAPITAL LETTER C */
        { 0x78, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x78, 0x00 }, /* U+0044 LATIN CAPITAL LETTER D */
        { 0x7e, 0x60, 0x60, 0x7c, 0x60, 0x60, 0x7e, 0x00 }, /* U+0045 LATIN CAPITAL LETTER E */
        { 0x7e, 0x60, 0x60, 0x7c, 0x60, 0x60, 0x60, 0x00 }, /* U+0046 LATIN CAPITAL LETTER F */
        { 0x3c, 0x66, 0x60, 0x6e, 0x66, 0x66, 0x3c, 0x00 }, /* U+0047 LATIN CAPITAL LETTER G */
        { 0x66, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00 }, /* U+0048 LATIN CAPITAL LETTER H */
        { 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00 }, /* U+0049 LATIN CAPITAL LETTER I */
        { 0x3e, 0x0c, 0x0c, 0x0c, 0x0c, 0x6c, 0x38, 0x00 }, /* U+004A LATIN CAPITAL LETTER J */
        { 0x66, 0x6c, 0x78, 0x70, 0x78, 0x6c, 0x66, 0x00 }, /* U+004B LATIN CAPITAL LETTER K */
        { 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x00 }, /* U+004C LATIN CAPITAL LETTER L */
        { 0x63, 0x77, 0x7f, 0x6b, 0x6b, 0x63, 0x63, 0x00 }, /* U+004D LATIN CAPITAL LETTER M */
        { 0x66, 0x66, 0x76, 0x7e, 0x6e, 0x66, 0x66, 0x00 }, /* U+004E LATIN CAPITAL LETTER N */
        { 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* U+004F LATIN CAPITAL LETTER O */
        { 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0x00 }, /* U+0050 LATIN CAPITAL LETTER P */
        { 0x3c, 0x66, 0x66, 0x66, 0x6a, 0x6c, 0x36, 0x00 }, /* U+0051 LATIN CAPITAL LETTER Q */
        { 0x7c, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0x66, 0x00 }, /* U+0052 LATIN CAPITAL LETTER R */
        { 0x3c, 0x66, 0x60, 0x3c, 0x06, 0x66, 0x3c, 0x00 }, /* U+0053 LATIN CAPITAL LETTER S */
        { 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* U+0054 LATIN CAPITAL LETTER T */
        { 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* U+0055 LATIN CAPITAL LETTER U */
        { 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00 }, /* U+0056 LATIN CAPITAL LETTER V */
        { 0x63, 0x63, 0x6b, 0x6b, 0x7f, 0x77, 0x63, 0x00 }, /* U+0057 LATIN CAPITAL LETTER W */
        { 0x66, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x66, 0x00 }, /* U+0058 LATIN CAPITAL LETTER X */
        { 0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x00 }, /* U+0059 LATIN CAPITAL LETTER Y */
        { 0x7e, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x7e, 0x00 }, /* U+005A LATIN CAPITAL LETTER Z */
        { 0x7c, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7c, 0x00 }, /* U+005B LEFT SQUARE BRACKET */
        { 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x00, 0x00 }, /* U+005C REVERSE SOLIDUS */
        { 0x3e, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3e, 0x00 }, /* U+005D RIGHT SQUARE BRACKET */
        { 0x18, 0x3c, 0x66, 0x42, 0x00, 0x00, 0x00, 0x00 }, /* U+005E CIRCUMFLEX ACCENT */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff }, /* U+005F LOW LINE */
        { 0x1c, 0x36, 0x30, 0x7c, 0x30, 0x30, 0x7e, 0x00 }, /* U+00A3 POUND SIGN */
        { 0x00, 0x00, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00 }, /* U+0061 LATIN SMALL LETTER A */
        { 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x7c, 0x00 }, /* U+0062 LATIN SMALL LETTER B */
        { 0x00, 0x00, 0x3c, 0x66, 0x60, 0x66, 0x3c, 0x00 }, /* U+0063 LATIN SMALL LETTER C */
        { 0x06, 0x06, 0x3e, 0x66, 0x66, 0x66, 0x3e, 0x00 }, /* U+0064 LATIN SMALL LETTER D */
        { 0x00, 0x00, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00 }, /* U+0065 LATIN SMALL LETTER E */
        { 0x1c, 0x30, 0x30, 0x7c, 0x30, 0x30, 0x30, 0x00 }, /* U+0066 LATIN SMALL LETTER F */
        { 0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x3c }, /* U+0067 LATIN SMALL LETTER G */
        { 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x00 }, /* U+0068 LATIN SMALL LETTER H */
        { 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3c, 0x00 }, /* U+0069 LATIN SMALL LETTER I */
        { 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x70 }, /* U+006A LATIN SMALL LETTER J */
        { 0x60, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0x00 }, /* U+006B LATIN SMALL LETTER K */
        { 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00 }, /* U+006C LATIN SMALL LETTER L */
        { 0x00, 0x00, 0x36, 0x7f, 0x6b, 0x6b, 0x63, 0x00 }, /* U+006D LATIN SMALL LETTER M */
        { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x00 }, /* U+006E LATIN SMALL LETTER N */
        { 0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* U+006F LATIN SMALL LETTER O */
        { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60 }, /* U+0070 LATIN SMALL LETTER P */
        { 0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x07 }, /* U+0071 LATIN SMALL LETTER Q */
        { 0x00, 0x00, 0x6c, 0x76, 0x60, 0x60, 0x60, 0x00 }, /* U+0072 LATIN SMALL LETTER R */
        { 0x00, 0x00, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x00 }, /* U+0073 LATIN SMALL LETTER S */
        { 0x30, 0x30, 0x7c, 0x30, 0x30, 0x30, 0x1c, 0x00 }, /* U+0074 LATIN SMALL LETTER T */
        { 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x00 }, /* U+0075 LATIN SMALL LETTER U */
        { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00 }, /* U+0076 LATIN SMALL LETTER V */
        { 0x00, 0x00, 0x63, 0x6b, 0x6b, 0x7f, 0x36, 0x00 }, /* U+0077 LATIN SMALL LETTER W */
        { 0x00, 0x00, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x00 }, /* U+0078 LATIN SMALL LETTER X */
        { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x06, 0x3c }, /* U+0079 LATIN SMALL LETTER Y */
        { 0x00, 0x00, 0x7e, 0x0c, 0x18, 0x30, 0x7e, 0x00 }, /* U+007A LATIN SMALL LETTER Z */
        { 0x0c, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0c, 0x00 }, /* U+007B LEFT CURLY BRACKET */
        { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 }, /* U+00A6 BROKEN BAR */
        { 0x30, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x30, 0x00 }, /* U+007D RIGHT CURLY BRACKET */
        { 0x31, 0x6b, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* U+007E TILDE */
        { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }, /* U+2588 FULL BLOCK */
        /* 96-223: BBC Master additional raster characters (MOS 3.21) */
        { 0x66, 0x00, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00 }, /* [0x0060] U+00C4 LATIN CAPITAL LETTER A WITH DIAERESIS */
        { 0x3c, 0x66, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00 }, /* [0x0061] U+00C5 LATIN CAPITAL LETTER A WITH RING ABOVE */
        { 0x3f, 0x66, 0x66, 0x7f, 0x66, 0x66, 0x67, 0x00 }, /* [0x0062] U+00C6 LATIN CAPITAL LETTER AE */
        { 0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x60 }, /* [0x0063] U+00C7 LATIN CAPITAL LETTER C WITH CEDILLA */
        { 0x0c, 0x18, 0x7e, 0x60, 0x7c, 0x60, 0x7e, 0x00 }, /* [0x0064] U+00C9 LATIN CAPITAL LETTER E WITH ACUTE */
        { 0x66, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* [0x0065] U+00D6 LATIN CAPITAL LETTER O WITH DIAERESIS */
        { 0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* [0x0066] U+00DC LATIN CAPITAL LETTER U WITH DIAERESIS */
        { 0x7e, 0xc3, 0x9d, 0xb1, 0x9d, 0xc3, 0x7e, 0x00 }, /* [0x0067] U+00A9 COPYRIGHT SIGN */
        { 0x00, 0x18, 0x38, 0x7f, 0x38, 0x18, 0x00, 0x00 }, /* [0x0068] U+2190 LEFTWARDS ARROW */
        { 0x00, 0x18, 0x1c, 0xfe, 0x1c, 0x18, 0x00, 0x00 }, /* [0x0069] U+2192 RIGHTWARDS ARROW */
        { 0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00 }, /* [0x006a] U+2193 DOWNWARDS ARROW */
        { 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18 }, /* [0x006b] U+2191 UPWARDS ARROW */
        { 0x30, 0x18, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00 }, /* [0x006c] U+00E0 LATIN SMALL LETTER A WITH GRAVE */
        { 0x30, 0x18, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00 }, /* [0x006d] U+00E8 LATIN SMALL LETTER E WITH GRAVE */
        { 0x66, 0x00, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00 }, /* [0x006e] U+00EB LATIN SMALL LETTER E WITH DIAERESIS */
        { 0x3c, 0x66, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00 }, /* [0x006f] U+00EA LATIN SMALL LETTER E WITH CIRCUMFLEX */
        { 0x66, 0x00, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00 }, /* [0x0070] U+00E4 LATIN SMALL LETTER A WITH DIAERESIS */
        { 0x3c, 0x66, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00 }, /* [0x0071] U+00E5 LATIN SMALL LETTER A WITH RING ABOVE */
        { 0x00, 0x00, 0x3f, 0x0d, 0x3f, 0x6c, 0x3f, 0x00 }, /* [0x0072] U+00E6 LATIN SMALL LETTER AE */
        { 0x00, 0x00, 0x3c, 0x66, 0x60, 0x66, 0x3c, 0x60 }, /* [0x0073] U+00E7 LATIN SMALL LETTER C WITH CEDILLA */
        { 0x0c, 0x18, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00 }, /* [0x0074] U+00E9 LATIN SMALL LETTER E WITH ACUTE */
        { 0x66, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* [0x0075] U+00F6 LATIN SMALL LETTER O WITH DIAERESIS */
        { 0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x00 }, /* [0x0076] U+00FC LATIN SMALL LETTER U WITH DIAERESIS */
        { 0x30, 0x18, 0x00, 0x38, 0x18, 0x18, 0x3c, 0x00 }, /* [0x0077] U+00EC LATIN SMALL LETTER I WITH GRAVE */
        { 0x3c, 0x66, 0x00, 0x38, 0x18, 0x18, 0x3c, 0x00 }, /* [0x0078] U+00EE LATIN SMALL LETTER I WITH CIRCUMFLEX */
        { 0x30, 0x18, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00 }, /* [0x0079] U+00F2 LATIN SMALL LETTER O WITH GRAVE */
        { 0x3c, 0x66, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00 }, /* [0x007a] U+00F4 LATIN SMALL LETTER O WITH CIRCUMFLEX */
        { 0x30, 0x18, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x00 }, /* [0x007b] U+00F9 LATIN SMALL LETTER U WITH GRAVE */
        { 0x3c, 0x66, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x00 }, /* [0x007c] U+00FB LATIN SMALL LETTER U WITH CIRCUMFLEX */
        { 0x66, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x06, 0x3c }, /* [0x007d] U+00FF LATIN SMALL LETTER Y WITH DIAERESIS */
        { 0x00, 0x66, 0x3c, 0x66, 0x66, 0x3c, 0x66, 0x00 }, /* [0x007e] U+00A4 CURRENCY SIGN */
        { 0x3c, 0x60, 0x3c, 0x66, 0x3c, 0x06, 0x3c, 0x00 }, /* [0x007f] U+00A7 SECTION SIGN */
        { 0x3c, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* [0x0080] U+00B0 DEGREE */
        { 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18 }, /* [0x0081] U+257B BOX DRAWINGS HEAVY DOWN */
        { 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00 }, /* [0x0082] U+257A BOX DRAWINGS HEAVY RIGHT */
        { 0x00, 0x00, 0x00, 0x1f, 0x18, 0x18, 0x18, 0x18 }, /* [0x0083] U+250F BOX DRAWINGS HEAVY DOWN AND RIGHT */
        { 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00 }, /* [0x0084] U+2578 BOX DRAWINGS HEAVY LEFT */
        { 0x00, 0x00, 0x00, 0xf8, 0x18, 0x18, 0x18, 0x18 }, /* [0x0085] U+2513 BOX DRAWINGS HEAVY DOWN AND LEFT */
        { 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00 }, /* [0x0086] U+2501 BOX DRAWINGS HEAVY HORIZONTAL */
        { 0x00, 0x00, 0x00, 0xff, 0x18, 0x18, 0x18, 0x18 }, /* [0x0087] U+2533 BOX DRAWINGS HEAVY DOWN AND HORIZONTAL */
        { 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00 }, /* [0x0088] U+2579 BOX DRAWINGS HEAVY UP */
        { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, /* [0x0089] U+2503 BOX DRAWINGS HEAVY VERTICAL */
        { 0x18, 0x18, 0x18, 0x1f, 0x00, 0x00, 0x00, 0x00 }, /* [0x008a] U+2517 BOX DRAWINGS HEAVY UP AND RIGHT */
        { 0x18, 0x18, 0x18, 0x1f, 0x18, 0x18, 0x18, 0x18 }, /* [0x008b] U+2523 BOX DRAWINGS HEAVY VERTICAL AND RIGHT */
        { 0x18, 0x18, 0x18, 0xf8, 0x00, 0x00, 0x00, 0x00 }, /* [0x008c] U+251B BOX DRAWINGS HEAVY UP AND LEFT */
        { 0x18, 0x18, 0x18, 0xf8, 0x18, 0x18, 0x18, 0x18 }, /* [0x008d] U+252B BOX DRAWINGS HEAVY VERTICAL AND LEFT */
        { 0x18, 0x18, 0x18, 0xff, 0x00, 0x00, 0x00, 0x00 }, /* [0x008e] U+253B BOX DRAWINGS HEAVY UP AND HORIZONTAL */
        { 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18 }, /* [0x008f] U+254B BOX DRAWINGS HEAVY VERTICAL AND HORIZONTAL */
        { 0x00, 0x00, 0x00, 0x07, 0x0c, 0x18, 0x18, 0x18 }, /* [0x0090] U+256D BOX DRAWINGS LIGHT ARC DOWN AND RIGHT */
        { 0x00, 0x00, 0x00, 0xe0, 0x30, 0x18, 0x18, 0x18 }, /* [0x0091] U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
        { 0x18, 0x18, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00 }, /* [0x0092] U+2570 BOX DRAWINGS LIGHT ARC UP AND RIGHT */
        { 0x18, 0x18, 0x30, 0xe0, 0x00, 0x00, 0x00, 0x00 }, /* [0x0093] U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
        { 0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3c, 0x00 }, /* [0x0094] U+00BF INVERTED QUESTION MARK */
        { 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* [0x0095] U+00A1 INVERTED EXCLAMATION MARK */
        { 0x36, 0x6c, 0x00, 0x66, 0x76, 0x6e, 0x66, 0x00 }, /* [0x0096] U+00D1 LATIN CAPITAL LETTER N WITH TILDE */
        { 0x36, 0x6c, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x00 }, /* [0x0097] U+00F1 LATIN SMALL LETTER N WITH TILDE */
        { 0x18, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* [0x0098] U+2020 DAGGER */
        { 0x18, 0x7e, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x00 }, /* [0x0099] U+2021 DOUBLE DAGGER */
        { 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* [0x009a] U+0027 APOSTROPHE */
        { 0x30, 0x18, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* [0x009b] U+0060 GRAVE ACCENT */
        { 0x3f, 0x7b, 0x7b, 0x3b, 0x1b, 0x1b, 0x1f, 0x00 }, /* [0x009c] U+00B6 PILCROW SIGN */
        { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 }, /* [0x009d] U+00B7 MIDDLE DOT */
        { 0x03, 0x03, 0x06, 0x06, 0x76, 0x1c, 0x0c, 0x00 }, /* [0x009e] U+221A SQUARE ROOT */
        { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 }, /* [0x009f] U+2592 MEDIUM SHADE */
        { 0x3e, 0x63, 0x67, 0x6b, 0x73, 0x63, 0x3e, 0x00 }, /* [0x00a0] U+00D8 LATIN CAPITAL LETTER O WITH STROKE */
        { 0x1c, 0x36, 0x63, 0x63, 0x7f, 0x63, 0x63, 0x00 }, /* [0x00a1] U+0391 GREEK CAPITAL LETTER ALPHA */
        { 0x7e, 0x33, 0x33, 0x3e, 0x33, 0x33, 0x7e, 0x00 }, /* [0x00a2] U+0392 GREEK CAPITAL LETTER BETA */
        { 0x7f, 0x63, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00 }, /* [0x00a3] U+0393 GREEK CAPITAL LETTER GAMMA */
        { 0x1c, 0x1c, 0x36, 0x36, 0x63, 0x63, 0x7f, 0x00 }, /* [0x00a4] U+0394 GREEK CAPITAL LETTER DELTA */
        { 0x7f, 0x33, 0x30, 0x3e, 0x30, 0x33, 0x7f, 0x00 }, /* [0x00a5] U+0395 GREEK CAPITAL LETTER EPSILON */
        { 0x7e, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x7e, 0x00 }, /* [0x00a6] U+0396 GREEK CAPITAL LETTER ZETA */
        { 0x77, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x77, 0x00 }, /* [0x00a7] U+0397 GREEK CAPITAL LETTER ETA */
        { 0x3e, 0x63, 0x63, 0x7f, 0x63, 0x63, 0x3e, 0x00 }, /* [0x00a8] U+0398 GREEK CAPITAL LETTER THETA */
        { 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00 }, /* [0x00a9] U+0399 GREEK CAPITAL LETTER IOTA */
        { 0x63, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0x63, 0x00 }, /* [0x00aa] U+039A GREEK CAPITAL LETTER KAPPA */
        { 0x1c, 0x1c, 0x36, 0x36, 0x63, 0x63, 0x63, 0x00 }, /* [0x00ab] U+039B GREEK CAPITAL LETTER LAMDA */
        { 0x63, 0x77, 0x7f, 0x6b, 0x63, 0x63, 0x63, 0x00 }, /* [0x00ac] U+039C GREEK CAPITAL LETTER MU */
        { 0x63, 0x73, 0x7b, 0x6f, 0x67, 0x63, 0x63, 0x00 }, /* [0x00ad] U+039D GREEK CAPITAL LETTER NU */
        { 0x7e, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00 }, /* [0x00ae] U+039E GREEK CAPITAL LETTER XI */
        { 0x3e, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3e, 0x00 }, /* [0x00af] U+039F GREEK CAPITAL LETTER OMICRON */
        { 0x7f, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00 }, /* [0x00b0] U+03A0 GREEK CAPITAL LETTER PI */
        { 0x7e, 0x33, 0x33, 0x3e, 0x30, 0x30, 0x78, 0x00 }, /* [0x00b1] U+03A1 GREEK CAPITAL LETTER RHO */
        { 0x7f, 0x63, 0x30, 0x18, 0x30, 0x63, 0x7f, 0x00 }, /* [0x00b2] U+03A3 GREEK CAPITAL LETTER SIGMA */
        { 0x7e, 0x5a, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* [0x00b3] U+03A4 GREEK CAPITAL LETTER TAU */
        { 0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x3c, 0x00 }, /* [0x00b4] U+03A5 GREEK CAPITAL LETTER UPSILON */
        { 0x3e, 0x08, 0x3e, 0x6b, 0x3e, 0x08, 0x3e, 0x00 }, /* [0x00b5] U+03A6 GREEK CAPITAL LETTER PHI */
        { 0x63, 0x63, 0x36, 0x1c, 0x36, 0x63, 0x63, 0x00 }, /* [0x00b6] U+03A7 GREEK CAPITAL LETTER CHI */
        { 0x3e, 0x08, 0x6b, 0x6b, 0x3e, 0x08, 0x3e, 0x00 }, /* [0x00b7] U+03A8 GREEK CAPITAL LETTER PSI */
        { 0x3e, 0x63, 0x63, 0x63, 0x36, 0x36, 0x63, 0x00 }, /* [0x00b8] U+03A9 GREEK CAPITAL LETTER OMEGA */
        { 0x7f, 0x63, 0x63, 0x36, 0x36, 0x1c, 0x1c, 0x00 }, /* [0x00b9] U+2207 NABLA */
        { 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x7e, 0x00 }, /* [0x00ba] U+00B1 PLUS-MINUS SIGN */
        { 0x00, 0x7e, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18 }, /* [0x00bb] U+2213 MINUS-OR-PLUS SIGN */
        { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* [0x00bc] U+007C VERTICAL BAR */
        { 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00 }, /* [0x00bd] U+2016 DOUBLE VERTICAL LINE */
        { 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* [0x00be] U+222A UNION */
        { 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00 }, /* [0x00bf] U+2229 INTERSECTION */
        { 0x00, 0x03, 0x3e, 0x67, 0x6b, 0x73, 0x3e, 0x60 }, /* [0x00c0] U+00F8 LATIN SMALL LETTER O WITH STROKE */
        { 0x00, 0x00, 0x3b, 0x6e, 0x66, 0x6e, 0x3b, 0x00 }, /* [0x00c1] U+03B1 GREEK SMALL LETTER ALPHA */
        { 0x1e, 0x33, 0x33, 0x3e, 0x33, 0x33, 0x3e, 0x60 }, /* [0x00c2] U+03B2 GREEK SMALL LETTER BETA */
        { 0x00, 0x00, 0x66, 0x36, 0x1c, 0x18, 0x30, 0x30 }, /* [0x00c3] U+03B3 GREEK SMALL LETTER GAMMA */
        { 0x3c, 0x60, 0x30, 0x3c, 0x66, 0x66, 0x3c, 0x00 }, /* [0x00c4] U+03B4 GREEK SMALL LETTER DELTA */
        { 0x00, 0x00, 0x1e, 0x30, 0x1c, 0x30, 0x1e, 0x00 }, /* [0x00c5] U+03B5 GREEK SMALL LETTER EPSILON */
        { 0x3e, 0x0c, 0x18, 0x30, 0x60, 0x60, 0x3e, 0x06 }, /* [0x00c6] U+03B6 GREEK SMALL LETTER ZETA */
        { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x06, 0x06 }, /* [0x00c7] U+03B7 GREEK SMALL LETTER ETA */
        { 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x3c, 0x00 }, /* [0x00c8] U+03B8 GREEK SMALL LETTER THETA */
        { 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x0c, 0x00 }, /* [0x00c9] U+03B9 GREEK SMALL LETTER IOTA */
        { 0x00, 0x00, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0x00 }, /* [0x00ca] U+03BA GREEK SMALL LETTER KAPPA */
        { 0x60, 0x30, 0x18, 0x1c, 0x36, 0x63, 0x63, 0x00 }, /* [0x00cb] U+03BB GREEK SMALL LETTER LAMDA */
        { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x3e, 0x60 }, /* [0x00cc] U+03BC GREEK SMALL LETTER MU */
        { 0x00, 0x00, 0x63, 0x33, 0x1b, 0x1e, 0x1c, 0x00 }, /* [0x00cd] U+03BD GREEK SMALL LETTER NU */
        { 0x3c, 0x60, 0x60, 0x3c, 0x60, 0x60, 0x3e, 0x06 }, /* [0x00ce] U+03BE GREEK SMALL LETTER XI */
        { 0x00, 0x00, 0x3e, 0x63, 0x63, 0x63, 0x3e, 0x00 }, /* [0x00cf] U+03BF GREEK SMALL LETTER OMICRON */
        { 0x00, 0x00, 0x7f, 0x36, 0x36, 0x36, 0x36, 0x00 }, /* [0x00d0] U+03C0 GREEK SMALL LETTER PI */
        { 0x00, 0x00, 0x3c, 0x66, 0x66, 0x7c, 0x60, 0x60 }, /* [0x00d1] U+03C1 GREEK SMALL LETTER RHO */
        { 0x00, 0x00, 0x3f, 0x66, 0x66, 0x66, 0x3c, 0x00 }, /* [0x00d2] U+03C3 GREEK SMALL LETTER SIGMA */
        { 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x0c, 0x00 }, /* [0x00d3] U+03C4 GREEK SMALL LETTER TAU */
        { 0x00, 0x00, 0x73, 0x33, 0x33, 0x33, 0x1e, 0x00 }, /* [0x00d4] U+03C5 GREEK SMALL LETTER UPSILON */
        { 0x00, 0x00, 0x3e, 0x6b, 0x6b, 0x3e, 0x18, 0x18 }, /* [0x00d5] U+03C6 GREEK SMALL LETTER PHI */
        { 0x00, 0x00, 0x66, 0x36, 0x1c, 0x1c, 0x36, 0x33 }, /* [0x00d6] U+03C7 GREEK SMALL LETTER CHI */
        { 0x00, 0x00, 0x63, 0x6b, 0x6b, 0x3e, 0x18, 0x18 }, /* [0x00d7] U+03C8 GREEK SMALL LETTER PSI */
        { 0x00, 0x00, 0x36, 0x63, 0x6b, 0x7f, 0x36, 0x00 }, /* [0x00d8] U+03C9 GREEK SMALL LETTER OMEGA */
        { 0x38, 0x0c, 0x06, 0x3e, 0x66, 0x66, 0x3c, 0x00 }, /* [0x00d9] U+2202 PARTIAL DIFFERENTIAL */
        { 0x00, 0x31, 0x6b, 0x46, 0x00, 0x7f, 0x00, 0x00 }, /* [0x00da] U+2243 ASYMPTOTICALLY EQUAL TO */
        { 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00 }, /* [0x00db] U+2261 IDENTICAL TO */
        { 0x07, 0x1c, 0x70, 0x1c, 0x07, 0x00, 0x7f, 0x00 }, /* [0x00dc] U+2264 LESS-THAN OR EQUAL TO */
        { 0x06, 0x0c, 0x7e, 0x18, 0x7e, 0x30, 0x60, 0x00 }, /* [0x00dd] U+2260 NOT EQUAL TO */
        { 0x70, 0x1c, 0x07, 0x1c, 0x70, 0x00, 0x7f, 0x00 }, /* [0x00de] U+2265 GREATER-THAN OR EQUAL TO*/
        { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }, /* [0x00df] U+2588 FULL BLOCK (repeated) */
        /* General Punctuation */
        { 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00 }, /* [0x00e0] U+2014 EM DASH */
        /* Geometric Shapes */
        { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00 }, /* [0x00e1] U+25A0 BLACK SQUARE */
        { 0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7f, 0x00 }, /* [0x00e2] U+25A1 WHITE SQUARE */
        /* Replacement character */
        { 0xc3, 0x99, 0xf3, 0xe7, 0xe7, 0xff, 0xe7, 0xff }, /* [0x00e3] U+FFFD REPLACEMENT CHARACTER */
};
