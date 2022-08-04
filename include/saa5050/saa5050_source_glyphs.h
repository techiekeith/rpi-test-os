/*
 * saa5050_source_glyphs.h
 */

#pragma once

#include <saa5050/saa5050_glyphs.h>

static uint8_t saa5050_source_glyphs[SOURCE_GLYPHS][SOURCE_GLYPH_ROWS] = {
  { 000, 000, 000, 000, 000, 000, 000, 000, 000 }, /* U+0020 SPACE */
  { 004, 004, 004, 004, 004, 000, 004, 000, 000 }, /* U+0021 EXCLAMATION MARK */
  { 012, 012, 012, 000, 000, 000, 000, 000, 000 }, /* U+0022 QUOTATION MARK */
  { 006, 011, 010, 034, 010, 010, 037, 000, 000 }, /* U+00A3 POUND SIGN */
  { 016, 025, 024, 016, 005, 025, 016, 000, 000 }, /* U+0024 DOLLAR SIGN */
  { 030, 031, 002, 004, 010, 023, 003, 000, 000 }, /* U+0025 PERCENT SIGN */
  { 010, 024, 024, 010, 025, 022, 015, 000, 000 }, /* U+0026 AMPERSAND */
  { 004, 004, 004, 000, 000, 000, 000, 000, 000 }, /* U+0027 APOSTROPHE */
  { 002, 004, 010, 010, 010, 004, 002, 000, 000 }, /* U+0028 LEFT PARENTHESIS */
  { 010, 004, 002, 002, 002, 004, 010, 000, 000 }, /* U+0029 RIGHT PARENTHESIS */
  { 004, 025, 016, 004, 016, 025, 004, 000, 000 }, /* U+002A ASTERISK */
  { 000, 004, 004, 037, 004, 004, 000, 000, 000 }, /* U+002B PLUS SIGN */
  { 000, 000, 000, 000, 000, 000, 004, 004, 010 }, /* U+002C COMMA */
  { 000, 000, 000, 016, 000, 000, 000, 000, 000 }, /* U+002D HYPHEN-MINUS */
  { 000, 000, 000, 000, 000, 000, 004, 000, 000 }, /* U+002E FULL STOP */
  { 000, 001, 002, 004, 010, 020, 000, 000, 000 }, /* U+002F SOLIDUS */
  { 004, 012, 021, 021, 021, 012, 004, 000, 000 }, /* U+0030 DIGIT ZERO */
  { 004, 014, 004, 004, 004, 004, 016, 000, 000 }, /* U+0031 DIGIT ONE */
  { 016, 021, 001, 006, 010, 020, 037, 000, 000 }, /* U+0032 DIGIT TWO */
  { 037, 001, 002, 006, 001, 021, 016, 000, 000 }, /* U+0033 DIGIT THREE */
  { 002, 006, 012, 022, 037, 002, 002, 000, 000 }, /* U+0034 DIGIT FOUR */
  { 037, 020, 036, 001, 001, 021, 016, 000, 000 }, /* U+0035 DIGIT FIVE */
  { 006, 010, 020, 036, 021, 021, 016, 000, 000 }, /* U+0036 DIGIT SIX */
  { 037, 001, 002, 004, 010, 010, 010, 000, 000 }, /* U+0037 DIGIT SEVEN */
  { 016, 021, 021, 016, 021, 021, 016, 000, 000 }, /* U+0038 DIGIT EIGHT */
  { 016, 021, 021, 017, 001, 002, 014, 000, 000 }, /* U+0039 DIGIT NINE */
  { 000, 000, 004, 000, 000, 000, 004, 000, 000 }, /* U+003A COLON */
  { 000, 000, 004, 000, 000, 004, 004, 010, 000 }, /* U+003B SEMICOLON */
  { 002, 004, 010, 020, 010, 004, 002, 000, 000 }, /* U+003C LESS-THAN SIGN */
  { 000, 000, 037, 000, 037, 000, 000, 000, 000 }, /* U+003D EQUALS SIGN */
  { 010, 004, 002, 001, 002, 004, 010, 000, 000 }, /* U+003E GREATER-THAN SIGN */
  { 016, 021, 002, 004, 004, 000, 004, 000, 000 }, /* U+003F QUESTION MARK */
  { 016, 021, 027, 025, 027, 020, 016, 000, 000 }, /* U+0040 COMMERCIAL AT */
  { 004, 012, 021, 021, 037, 021, 021, 000, 000 }, /* U+0041 LATIN CAPITAL LETTER A */
  { 036, 021, 021, 036, 021, 021, 036, 000, 000 }, /* U+0042 LATIN CAPITAL LETTER B */
  { 016, 021, 020, 020, 020, 021, 016, 000, 000 }, /* U+0043 LATIN CAPITAL LETTER C */
  { 036, 021, 021, 021, 021, 021, 036, 000, 000 }, /* U+0044 LATIN CAPITAL LETTER D */
  { 037, 020, 020, 036, 020, 020, 037, 000, 000 }, /* U+0045 LATIN CAPITAL LETTER E */
  { 037, 020, 020, 036, 020, 020, 020, 000, 000 }, /* U+0046 LATIN CAPITAL LETTER F */
  { 016, 021, 020, 020, 023, 021, 017, 000, 000 }, /* U+0047 LATIN CAPITAL LETTER G */
  { 021, 021, 021, 037, 021, 021, 021, 000, 000 }, /* U+0048 LATIN CAPITAL LETTER H */
  { 016, 004, 004, 004, 004, 004, 016, 000, 000 }, /* U+0049 LATIN CAPITAL LETTER I */
  { 001, 001, 001, 001, 001, 021, 016, 000, 000 }, /* U+004A LATIN CAPITAL LETTER J */
  { 021, 022, 024, 030, 024, 022, 021, 000, 000 }, /* U+004B LATIN CAPITAL LETTER K */
  { 020, 020, 020, 020, 020, 020, 037, 000, 000 }, /* U+004C LATIN CAPITAL LETTER L */
  { 021, 033, 025, 025, 021, 021, 021, 000, 000 }, /* U+004D LATIN CAPITAL LETTER M */
  { 021, 021, 031, 025, 023, 021, 021, 000, 000 }, /* U+004E LATIN CAPITAL LETTER N */
  { 016, 021, 021, 021, 021, 021, 016, 000, 000 }, /* U+004F LATIN CAPITAL LETTER O */
  { 036, 021, 021, 036, 020, 020, 020, 000, 000 }, /* U+0050 LATIN CAPITAL LETTER P */
  { 016, 021, 021, 021, 025, 022, 015, 000, 000 }, /* U+0051 LATIN CAPITAL LETTER Q */
  { 036, 021, 021, 036, 024, 022, 021, 000, 000 }, /* U+0052 LATIN CAPITAL LETTER R */
  { 016, 021, 020, 016, 001, 021, 016, 000, 000 }, /* U+0053 LATIN CAPITAL LETTER S */
  { 037, 004, 004, 004, 004, 004, 004, 000, 000 }, /* U+0054 LATIN CAPITAL LETTER T */
  { 021, 021, 021, 021, 021, 021, 016, 000, 000 }, /* U+0055 LATIN CAPITAL LETTER U */
  { 021, 021, 021, 012, 012, 004, 004, 000, 000 }, /* U+0056 LATIN CAPITAL LETTER V */
  { 021, 021, 021, 025, 025, 025, 012, 000, 000 }, /* U+0057 LATIN CAPITAL LETTER W */
  { 021, 021, 012, 004, 012, 021, 021, 000, 000 }, /* U+0058 LATIN CAPITAL LETTER X */
  { 021, 021, 012, 004, 004, 004, 004, 000, 000 }, /* U+0059 LATIN CAPITAL LETTER Y */
  { 037, 001, 002, 004, 010, 020, 037, 000, 000 }, /* U+005A LATIN CAPITAL LETTER Z */
  { 000, 004, 010, 037, 010, 004, 000, 000, 000 }, /* U+2190 LEFTWARDS ARROW */
  { 020, 020, 020, 020, 026, 001, 002, 004, 007 }, /* U+00BD VULGAR FRACTION ONE HALF */
  { 000, 004, 002, 037, 002, 004, 000, 000, 000 }, /* U+2192 RIGHTWARDS ARROW */
  { 000, 004, 016, 025, 004, 004, 000, 000, 000 }, /* U+2191 UPWARDS ARROW */
  { 012, 012, 037, 012, 037, 012, 012, 000, 000 }, /* U+0023 NUMBER SIGN */
  { 000, 000, 000, 037, 000, 000, 000, 000, 000 }, /* U+2014 EM DASH */
  { 000, 000, 016, 001, 017, 021, 017, 000, 000 }, /* U+0061 LATIN SMALL LETTER A */
  { 020, 020, 036, 021, 021, 021, 036, 000, 000 }, /* U+0062 LATIN SMALL LETTER B */
  { 000, 000, 017, 020, 020, 020, 017, 000, 000 }, /* U+0063 LATIN SMALL LETTER C */
  { 001, 001, 017, 021, 021, 021, 017, 000, 000 }, /* U+0064 LATIN SMALL LETTER D */
  { 000, 000, 016, 021, 037, 020, 016, 000, 000 }, /* U+0065 LATIN SMALL LETTER E */
  { 002, 004, 004, 016, 004, 004, 004, 000, 000 }, /* U+0066 LATIN SMALL LETTER F */
  { 000, 000, 017, 021, 021, 021, 017, 001, 016 }, /* U+0067 LATIN SMALL LETTER G */
  { 020, 020, 036, 021, 021, 021, 021, 000, 000 }, /* U+0068 LATIN SMALL LETTER H */
  { 004, 000, 014, 004, 004, 004, 016, 000, 000 }, /* U+0069 LATIN SMALL LETTER I */
  { 004, 000, 004, 004, 004, 004, 004, 004, 010 }, /* U+006A LATIN SMALL LETTER J */
  { 010, 010, 011, 012, 014, 012, 011, 000, 000 }, /* U+006B LATIN SMALL LETTER K */
  { 014, 004, 004, 004, 004, 004, 016, 000, 000 }, /* U+006C LATIN SMALL LETTER L */
  { 000, 000, 032, 025, 025, 025, 025, 000, 000 }, /* U+006D LATIN SMALL LETTER M */
  { 000, 000, 036, 021, 021, 021, 021, 000, 000 }, /* U+006E LATIN SMALL LETTER N */
  { 000, 000, 016, 021, 021, 021, 016, 000, 000 }, /* U+006F LATIN SMALL LETTER O */
  { 000, 000, 036, 021, 021, 021, 036, 020, 020 }, /* U+0070 LATIN SMALL LETTER P */
  { 000, 000, 017, 021, 021, 021, 017, 001, 001 }, /* U+0071 LATIN SMALL LETTER Q */
  { 000, 000, 013, 014, 010, 010, 010, 000, 000 }, /* U+0072 LATIN SMALL LETTER R */
  { 000, 000, 017, 020, 016, 001, 036, 000, 000 }, /* U+0073 LATIN SMALL LETTER S */
  { 004, 004, 016, 004, 004, 004, 002, 000, 000 }, /* U+0074 LATIN SMALL LETTER T */
  { 000, 000, 021, 021, 021, 021, 017, 000, 000 }, /* U+0075 LATIN SMALL LETTER U */
  { 000, 000, 021, 021, 012, 012, 004, 000, 000 }, /* U+0076 LATIN SMALL LETTER V */
  { 000, 000, 021, 021, 025, 025, 012, 000, 000 }, /* U+0077 LATIN SMALL LETTER W */
  { 000, 000, 021, 012, 004, 012, 021, 000, 000 }, /* U+0078 LATIN SMALL LETTER X */
  { 000, 000, 021, 021, 021, 021, 017, 001, 016 }, /* U+0079 LATIN SMALL LETTER Y */
  { 000, 000, 037, 002, 004, 010, 037, 000, 000 }, /* U+007A LATIN SMALL LETTER Z */
  { 020, 020, 020, 020, 021, 003, 005, 007, 001 }, /* U+00BC VULGAR FRACTION ONE QUARTER */
  { 012, 012, 012, 012, 012, 012, 012, 000, 000 }, /* U+2016 DOUBLE VERTICAL LINE */
  { 030, 004, 030, 004, 031, 003, 005, 007, 001 }, /* U+00BC VULGAR FRACTION THREE QUARTERS */
  { 000, 004, 000, 037, 000, 004, 000, 000, 000 }, /* U+00F6 DIVISION SIGN */
  { 037, 037, 037, 037, 037, 037, 037, 000, 000 }, /* U+25A0 BLACK SQUARE */
};
