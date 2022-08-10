/*
 * source_glyphs.h
 */

#pragma once

static uint8_t saa505x_source_glyphs[SOURCE_GLYPHS][SOURCE_GLYPH_ROWS] = {
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
  /* SAA5051/2/5: 0x23 */
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
  { 030, 004, 030, 004, 031, 003, 005, 007, 001 }, /* U+00BE VULGAR FRACTION THREE QUARTERS */
  { 000, 004, 000, 037, 000, 004, 000, 000, 000 }, /* U+00F7 DIVISION SIGN */
  { 037, 037, 037, 037, 037, 037, 037, 000, 000 }, /* U+25A0 BLACK SQUARE */
  /* [0x60] SAA5051/2/5: 0x27 */
  { 004, 004, 010, 000, 000, 000, 000, 000, 000 }, /* U+2019 RIGHT SINGLE QUOTATION MARK */
  /* [0x61] SAA5051/2: 0x2C */
  { 000, 000, 000, 000, 000, 010, 010, 020, 000 }, /* U+002C COMMA (SAA5051/2) */
  /* [0x62] SAA5051/2: 0x2E */
  { 000, 000, 000, 000, 000, 014, 014, 000, 000 }, /* U+002E FULL STOP (SAA5051/2) */
  /* [0x63] SAA5051/2: 0x3A */
  { 000, 000, 000, 010, 000, 000, 010, 000, 000 }, /* U+003A COLON (SAA5051/2) */
  /* [0x64] SAA5051/2: 0x3B */
  { 000, 000, 010, 000, 000, 010, 010, 020, 000 }, /* U+003B SEMICOLON (SAA5051/2) */
  /* [0x65] SAA5051: 0x40 */
  { 016, 021, 020, 016, 021, 016, 001, 021, 016 }, /* U+00A7 SECTION SIGN */
  /* [0x66] SAA5051/2: 0x5B */
  { 012, 000, 016, 021, 037, 021, 021, 000, 000 }, /* U+00C4 LATIN CAPITAL LETTER A WITH DIAERESIS */
  /* [0x67] SAA5051/2: 0x5C */
  { 012, 000, 016, 021, 021, 021, 016, 000, 000 }, /* U+00D6 LATIN CAPITAL LETTER O WITH DIAERESIS */
  /* [0x68] SAA5051: 0x5D; SAA5052: 0x5E */
  { 012, 000, 021, 021, 021, 021, 016, 000, 000 }, /* U+00DC LATIN CAPITAL LETTER U WITH DIAERESIS */
  /* [0x69] SAA5051/5: 0x5E */
  { 004, 012, 021, 000, 000, 000, 000, 000, 000 }, /* U+005E CIRCUMFLEX ACCENT */
  /* [0x6a] SAA5051/2/5: 0x5F */
  { 000, 000, 000, 000, 000, 000, 037, 000, 000 }, /* U+005F LOW LINE */
  /* [0x6b] SAA5051: 0x60; SAA5053: 0x5B */
  { 006, 011, 006, 000, 000, 000, 000, 000, 000 }, /* U+00B0 DEGREE */
  /* [0x6c] SAA5051/2: 0x7B */
  { 012, 000, 016, 001, 017, 021, 017, 000, 000 }, /* U+00E4 LATIN SMALL LETTER A WITH DIAERESIS */
  /* [0x6d] SAA5051/2: 0x7C */
  { 000, 012, 000, 016, 021, 021, 016, 000, 000 }, /* U+00F6 LATIN SMALL LETTER O WITH DIAERESIS */
  /* [0x6e] SAA5051: 0x7D; SAA5052: 0x7E */
  { 000, 012, 000, 021, 021, 021, 017, 000, 000 }, /* U+00FC LATIN SMALL LETTER U WITH DIAERESIS */
  /* [0x6f] SAA5051: 0x7E */
  { 014, 022, 022, 026, 021, 021, 026, 020, 020 }, /* U+00DF LATIN SMALL LETTER SHARP S */
  /* [0x70] SAA5052: 0x24 */
  { 000, 000, 021, 016, 012, 016, 021, 000, 000 }, /* U+00A4 CURRENCY SIGN */
  /* [0x71] SAA5052/3: 0x40 */
  { 002, 004, 037, 020, 036, 020, 037, 000, 000 }, /* U+00C9 LATIN CAPITAL LETTER E WITH ACUTE */
  /* [0x72] SAA5052: 0x44 */
  { 016, 011, 011, 011, 011, 011, 016, 000, 000 }, /* U+0044 LATIN CAPITAL LETTER D (SAA5052) */
  /* [0x73] SAA5052: 0x4C */
  { 010, 010, 010, 010, 010, 010, 017, 000, 000 }, /* U+004C LATIN CAPITAL LETTER L (SAA5052) */
  /* [0x74] SAA5052: 0x5D */
  { 004, 000, 016, 021, 037, 021, 021, 000, 000 }, /* U+00C5 LATIN CAPITAL LETTER A WITH RING ABOVE */
  /* [0x75] SAA5052: 0x60; SAA5053: 0x40; SAA5054: 0x23 */
  { 002, 004, 016, 021, 037, 020, 016, 000, 000 }, /* U+00E9 LATIN SMALL LETTER E WITH ACUTE */
  /* [0x76] SAA5052: 0x7D */
  { 004, 000, 016, 001, 017, 021, 017, 000, 000 }, /* U+00E5 LATIN SMALL LETTER A WITH RING ABOVE */
  /* [0x77] SAA5053: 0x5C */
  { 000, 000, 017, 020, 020, 020, 017, 002, 004 }, /* U+00E7 LATIN SMALL LETTER C WITH CEDILLA (SAA5053) */
  /* [0x78] SAA5053: 0x60 */
  { 010, 004, 021, 021, 021, 021, 017, 000, 000 }, /* U+00F9 LATIN SMALL LETTER U WITH GRAVE (SAA5053) */
  /* [0x79] SAA5053: 0x7B; SAA5054: 0x40 */
  { 010, 004, 016, 001, 017, 021, 017, 000, 000 }, /* U+00E0 LATIN SMALL LETTER A WITH GRAVE */
  /* [0x7a] SAA5053: 0x7C */
  { 010, 004, 000, 016, 021, 021, 016, 000, 000 }, /* U+00F2 LATIN SMALL LETTER O WITH GRAVE */
  /* [0x7b] SAA5053: 0x7D; SAA5054: 0x60 */
  { 010, 004, 016, 021, 037, 020, 016, 000, 000 }, /* U+00E8 LATIN SMALL LETTER E WITH GRAVE */
  /* [0x7c] SAA5053: 0x7E */
  { 010, 004, 000, 014, 004, 004, 016, 000, 000 }, /* U+00EC LATIN SMALL LETTER I WITH GRAVE */
  /* [0x7d] SAA5054: 0x24 */
  { 012, 000, 014, 004, 004, 004, 016, 000, 000 }, /* U+00EF LATIN SMALL LETTER I WITH DIAERESIS */
  /* [0x7e] SAA5054: 0x5B */
  { 012, 000, 016, 021, 037, 020, 016, 000, 000 }, /* U+00EB LATIN SMALL LETTER E WITH DIAERESIS */
  /* [0x7f] SAA5054: 0x5C */
  { 004, 012, 016, 021, 037, 020, 016, 000, 000 }, /* U+00EA LATIN SMALL LETTER E WITH CIRCUMFLEX */
  /* [0x80] SAA5054: 0x5D */
  { 004, 002, 021, 021, 021, 021, 017, 000, 000 }, /* U+00F9 LATIN SMALL LETTER U WITH GRAVE (SAA5054) */
  /* [0x81] SAA5054: 0x5E */
  { 004, 012, 000, 014, 004, 004, 016, 000, 000 }, /* U+00EE LATIN SMALL LETTER I WITH CIRCUMFLEX */
  /* [0x82] SAA5054: 0x7B */
  { 004, 012, 016, 001, 017, 021, 017, 000, 000 }, /* U+00E2 LATIN SMALL LETTER A WITH CIRCUMFLEX */
  /* [0x83] SAA5054: 0x7C */
  { 004, 012, 016, 021, 021, 021, 016, 000, 000 }, /* U+00F4 LATIN SMALL LETTER O WITH CIRCUMFLEX */
  /* [0x84] SAA5054: 0x7D */
  { 004, 012, 000, 021, 021, 021, 017, 000, 000 }, /* U+00FB LATIN SMALL LETTER U WITH CIRCUMFLEX */
  /* [0x85] SAA5054: 0x7E */
  { 000, 000, 017, 020, 020, 020, 017, 002, 006 }, /* U+00E7 LATIN SMALL LETTER C WITH CEDILLA (SAA5054) */
  /* [0x86] SAA5055: 0x5B */
  { 017, 010, 010, 010, 010, 010, 017, 000, 000 }, /* U+005B LEFT SQUARE BRACKET */
  /* [0x87] SAA5055: 0x5C */
  { 000, 020, 010, 004, 002, 001, 000, 000, 000 }, /* U+005C REVERSE SOLIDUS */
  /* [0x88] SAA5055: 0x5D */
  { 036, 002, 002, 002, 002, 002, 036, 000, 000 }, /* U+005D RIGHT SQUARE BRACKET */
  /* [0x89] SAA5055: 0x60 */
  { 004, 004, 002, 000, 000, 000, 000, 000, 000 }, /* U+2018 LEFT SINGLE QUOTATION MARK */
  /* [0x8a] SAA5055: 0x7B */
  { 003, 004, 004, 010, 004, 004, 003, 000, 000 }, /* U+007B LEFT CURLY BRACKET */
  /* [0x8b] SAA5055: 0x7C */
  { 004, 004, 004, 000, 004, 004, 004, 000, 000 }, /* U+00A6 BROKEN BAR */
  /* [0x8c] SAA5055: 0x7D */
  { 030, 004, 004, 002, 004, 004, 030, 000, 000 }, /* U+007D RIGHT CURLY BRACKET */
  /* [0x8d] SAA5055: 0x7E */
  { 010, 025, 002, 000, 000, 000, 000, 000, 000 }, /* U+007E TILDE */
  /* [0x8e] SAA5056: 0x60 */
  { 000, 021, 011, 025, 022, 021, 021, 000, 000 }, /* U+05D0 HEBREW LETTER ALEF */
  /* [0x8f] SAA5056: 0x61 */
  { 000, 016, 002, 002, 002, 002, 037, 000, 000 }, /* U+05D1 HEBREW LETTER BET */
  /* [0x90] SAA5056: 0x62 */
  { 000, 003, 001, 001, 003, 005, 011, 000, 000 }, /* U+05D2 HEBREW LETTER GIMEL */
  /* [0x91] SAA5056: 0x63 */
  { 000, 037, 002, 002, 002, 002, 002, 000, 000 }, /* U+05D3 HEBREW LETTER DALET */
  /* [0x92] SAA5056: 0x64 */
  { 000, 037, 001, 001, 021, 021, 021, 000, 000 }, /* U+05D4 HEBREW LETTER HE */
  /* [0x93] SAA5056: 0x65 */
  { 000, 014, 004, 004, 004, 004, 004, 000, 000 }, /* U+05D5 HEBREW LETTER VAV */
  /* [0x94] SAA5056: 0x66 */
  { 000, 016, 004, 010, 004, 004, 004, 000, 000 }, /* U+05D6 HEBREW LETTER ZAYIN */
  /* [0x95] SAA5056: 0x67 */
  { 000, 037, 021, 021, 021, 021, 021, 000, 000 }, /* U+05D7 HEBREW LETTER HET */
  /* [0x96] SAA5056: 0x68 */
  { 000, 021, 023, 025, 021, 021, 037, 000, 000 }, /* U+05D8 HEBREW LETTER TET */
  /* [0x97] SAA5056: 0x69 */
  { 000, 014, 004, 000, 000, 000, 000, 000, 000 }, /* U+05D9 HEBREW LETTER YOD */
  /* [0x98] SAA5056: 0x6a */
  { 000, 037, 001, 001, 001, 001, 001, 001, 000 }, /* U+05DA HEBREW LETTER FINAL KAF */
  /* [0x99] SAA5056: 0x6b */
  { 000, 037, 001, 001, 001, 001, 037, 000, 000 }, /* U+05DB HEBREW LETTER KAF */
  /* [0x9a] SAA5056: 0x6c */
  { 020, 037, 001, 001, 001, 002, 014, 000, 000 }, /* U+05DC HEBREW LETTER LAMED */
  /* [0x9b] SAA5056: 0x6d */
  { 000, 037, 021, 021, 021, 021, 037, 000, 000 }, /* U+05DD HEBREW LETTER FINAL MEM */
  /* [0x9c] SAA5056: 0x6e */
  { 000, 026, 011, 021, 021, 021, 027, 000, 000 }, /* U+05DE HEBREW LETTER MEM */
  /* [0x9d] SAA5056: 0x6f */
  { 000, 014, 004, 004, 004, 004, 004, 004, 004 }, /* U+05DF HEBREW LETTER FINAL NUN */
  /* [0x9e] SAA5056: 0x70 */
  { 000, 006, 002, 002, 002, 002, 016, 000, 000 }, /* U+05E0 HEBREW LETTER NUN */
  /* [0x9f] SAA5056: 0x71 */
  { 000, 037, 011, 021, 021, 021, 016, 000, 000 }, /* U+05E1 HEBREW LETTER SAMEKH */
  /* [0xa0] SAA5056: 0x72 */
  { 000, 011, 011, 011, 011, 012, 034, 000, 000 }, /* U+05E2 HEBREW LETTER AYIN */
  /* [0xa1] SAA5056: 0x73 */
  { 000, 037, 011, 015, 001, 001, 001, 001, 000 }, /* U+05E3 HEBREW LETTER FINAL PE */
  /* [0xa2] SAA5056: 0x74 */
  { 000, 037, 011, 015, 001, 001, 037, 000, 000 }, /* U+05E4 HEBREW LETTER PE */
  /* [0xa3] SAA5056: 0x75 */
  { 000, 031, 012, 014, 010, 010, 010, 010, 000 }, /* U+05E5 HEBREW LETTER FINAL TSADI */
  /* [0xa4] SAA5056: 0x76 */
  { 000, 021, 021, 012, 004, 002, 037, 000, 000 }, /* U+05E6 HEBREW LETTER TSADI */
  /* [0xa5] SAA5056: 0x77 */
  { 000, 037, 001, 011, 011, 012, 010, 010, 000 }, /* U+05E7 HEBREW LETTER QOF */
  /* [0xa6] SAA5056: 0x78 */
  { 000, 037, 001, 001, 001, 001, 001, 000, 000 }, /* U+05E8 HEBREW LETTER RESH */
  /* [0xa7] SAA5056: 0x79 */
  { 000, 025, 025, 025, 031, 021, 036, 000, 000 }, /* U+05E9 HEBREW LETTER SHIN */
  /* [0xa8] SAA5056: 0x7a */
  { 000, 017, 011, 011, 011, 011, 031, 000, 000 }, /* U+05EA HEBREW LETTER TAV */
  /* [0xa9] SAA5056: 0x7b */
  { 000, 000, 025, 025, 016, 000, 000, 000, 000 }, /* ...... OLD ISRAELI SHEKEL SIGN */
  /* [0xaa] SAA5057: 0x26 */
  { 000, 000, 021, 021, 035, 025, 035, 000, 000 }, /* U+044B CYRILLIC SMALL LETTER YERU */
  /* [0xab] SAA5057: 0x40 */
  { 022, 025, 025, 035, 025, 025, 022, 000, 000 }, /* U+042E CYRILLIC CAPITAL LETTER YU */
  /* [0xac] SAA5057: 0x41 */
  { 016, 021, 021, 021, 037, 021, 021, 000, 000 }, /* U+0410 CYRILLIC CAPITAL LETTER A */
  /* [0xad] SAA5057: 0x42 */
  { 037, 020, 020, 037, 021, 021, 037, 000, 000 }, /* U+0411 CYRILLIC CAPITAL LETTER BE */
  /* [0xae] SAA5057: 0x43 */
  { 022, 022, 022, 022, 022, 022, 037, 001, 000 }, /* U+0426 CYRILLIC CAPITAL LETTER TSE */
  /* [0xaf] SAA5057: 0x44 */
  { 006, 012, 012, 012, 012, 012, 037, 021, 000 }, /* U+0414 CYRILLIC CAPITAL LETTER DE */
  /* [0xb0] SAA5057: 0x45 */
  { 037, 020, 020, 036, 020, 020, 037, 000, 000 }, /* U+0415 CYRILLIC CAPITAL LETTER IE */
  /* [0xb1] SAA5057: 0x46 */
  { 004, 037, 025, 025, 025, 037, 004, 000, 000 }, /* U+0424 CYRILLIC CAPITAL LETTER EF */
  /* [0xb2] SAA5057: 0x47 */
  { 037, 020, 020, 020, 020, 020, 020, 000, 000 }, /* U+0413 CYRILLIC CAPITAL LETTER GHE */
  /* [0xb3] SAA5057: 0x48 */
  { 021, 021, 012, 004, 012, 021, 021, 000, 000 }, /* U+0425 CYRILLIC CAPITAL LETTER HA */
  /* [0xb4] SAA5057: 0x49 */
  { 021, 021, 023, 025, 031, 021, 021, 000, 000 }, /* U+0418 CYRILLIC CAPITAL LETTER I */
  /* [0xb5] SAA5057: 0x4a */
  { 025, 021, 023, 025, 031, 021, 021, 000, 000 }, /* U+0419 CYRILLIC CAPITAL LETTER SHORT I */
  /* [0xb6] SAA5057: 0x4b */
  { 021, 022, 024, 030, 024, 022, 021, 000, 000 }, /* U+041A CYRILLIC CAPITAL LETTER KA */
  /* [0xb7] SAA5057: 0x4c */
  { 007, 011, 011, 011, 011, 011, 031, 000, 000 }, /* U+041B CYRILLIC CAPITAL LETTER EL */
  /* [0xb8] SAA5057: 0x4d */
  { 021, 033, 025, 025, 021, 021, 021, 000, 000 }, /* U+041C CYRILLIC CAPITAL LETTER EM */
  /* [0xb9] SAA5057: 0x4e */
  { 021, 021, 021, 037, 021, 021, 021, 000, 000 }, /* U+041D CYRILLIC CAPITAL LETTER EN */
  /* [0xba] SAA5057: 0x4f */
  { 016, 021, 021, 021, 021, 021, 016, 000, 000 }, /* U+041E CYRILLIC CAPITAL LETTER O */
  /* [0xbb] SAA5057: 0x50 */
  { 037, 021, 021, 021, 021, 021, 021, 000, 000 }, /* U+041F CYRILLIC CAPITAL LETTER PE */
  /* [0xbc] SAA5057: 0x51 */
  { 017, 021, 021, 017, 005, 011, 021, 000, 000 }, /* U+042F CYRILLIC CAPITAL LETTER YA */
  /* [0xbd] SAA5057: 0x52 */
  { 036, 021, 021, 036, 020, 020, 020, 000, 000 }, /* U+0420 CYRILLIC CAPITAL LETTER ER */
  /* [0xbe] SAA5057: 0x53 */
  { 016, 021, 020, 020, 020, 021, 016, 000, 000 }, /* U+0421 CYRILLIC CAPITAL LETTER ES */
  /* [0xbf] SAA5057: 0x54 */
  { 037, 004, 004, 004, 004, 004, 004, 000, 000 }, /* U+0422 CYRILLIC CAPITAL LETTER TE */
  /* [0xc0] SAA5057: 0x55 */
  { 021, 021, 021, 037, 001, 001, 037, 000, 000 }, /* U+0423 CYRILLIC CAPITAL LETTER U */
  /* [0xc1] SAA5057: 0x56 */
  { 025, 025, 025, 016, 025, 025, 025, 000, 000 }, /* U+0416 CYRILLIC CAPITAL LETTER ZHE */
  /* [0xc2] SAA5057: 0x57 */
  { 036, 021, 021, 036, 021, 021, 036, 000, 000 }, /* U+0412 CYRILLIC CAPITAL LETTER VE */
  /* [0xc3] SAA5057: 0x58 */
  { 020, 020, 020, 037, 021, 021, 037, 000, 000 }, /* U+042C CYRILLIC CAPITAL LETTER SOFT SIGN */
  /* [0xc4] SAA5057: 0x59 */
  { 030, 010, 010, 017, 011, 011, 017, 000, 000 }, /* U+042A CYRILLIC CAPITAL LETTER HARD SIGN */
  /* [0xc5] SAA5057: 0x5a */
  { 016, 021, 001, 006, 001, 021, 016, 000, 000 }, /* U+0417 CYRILLIC CAPITAL LETTER ZE */
  /* [0xc6] SAA5057: 0x5b */
  { 025, 025, 025, 025, 025, 025, 037, 000, 000 }, /* U+0428 CYRILLIC CAPITAL LETTER SHA */
  /* [0xc7] SAA5057: 0x5c */
  { 014, 022, 001, 007, 001, 022, 014, 000, 000 }, /* U+042D CYRILLIC CAPITAL LETTER E */
  /* [0xc8] SAA5057: 0x5d */
  { 025, 025, 025, 025, 025, 025, 037, 001, 000 }, /* U+0429 CYRILLIC CAPITAL LETTER SHCHA */
  /* [0xc9] SAA5057: 0x5e */
  { 021, 021, 021, 037, 001, 001, 001, 000, 000 }, /* U+0427 CYRILLIC CAPITAL LETTER CHE */
  /* [0xca] SAA5057: 0x5f */
  { 021, 021, 021, 035, 025, 025, 035, 000, 000 }, /* U+042B CYRILLIC CAPITAL LETTER YERU */
  /* [0xcb] SAA5057: 0x60 */
  { 000, 000, 022, 025, 035, 025, 022, 000, 000 }, /* U+044E CYRILLIC SMALL LETTER YU */
  /* [0xcc] SAA5057: 0x61 */
  { 000, 000, 016, 001, 017, 021, 017, 000, 000 }, /* U+0430 CYRILLIC SMALL LETTER A */
  /* [0xcd] SAA5057: 0x62 */
  { 016, 020, 036, 021, 021, 021, 036, 000, 000 }, /* U+0431 CYRILLIC SMALL LETTER BE */
  /* [0xce] SAA5057: 0x63 */
  { 000, 000, 022, 022, 022, 022, 037, 001, 000 }, /* U+0446 CYRILLIC SMALL LETTER TSE */
  /* [0xcf] SAA5057: 0x64 */
  { 000, 000, 006, 012, 012, 012, 037, 021, 000 }, /* U+0434 CYRILLIC SMALL LETTER DE */
  /* [0xd0] SAA5057: 0x65 */
  { 000, 000, 016, 021, 037, 020, 016, 000, 000 }, /* U+0435 CYRILLIC SMALL LETTER IE */
  /* [0xd1] SAA5057: 0x66 */
  { 000, 004, 016, 025, 025, 025, 016, 004, 000 }, /* U+0444 CYRILLIC SMALL LETTER EF */
  /* [0xd2] SAA5057: 0x67 */
  { 000, 000, 037, 020, 020, 020, 020, 000, 000 }, /* U+0433 CYRILLIC SMALL LETTER GHE */
  /* [0xd3] SAA5057: 0x68 */
  { 000, 000, 021, 012, 004, 012, 021, 000, 000 }, /* U+0445 CYRILLIC SMALL LETTER HA */
  /* [0xd4] SAA5057: 0x69 */
  { 000, 000, 021, 023, 025, 031, 021, 000, 000 }, /* U+0438 CYRILLIC SMALL LETTER I */
  /* [0xd5] SAA5057: 0x6a */
  { 000, 004, 021, 023, 025, 031, 021, 000, 000 }, /* U+0439 CYRILLIC SMALL LETTER SHORT I */
  /* [0xd6] SAA5057: 0x6b */
  { 000, 000, 021, 022, 034, 022, 021, 000, 000 }, /* U+043A CYRILLIC SMALL LETTER KA */
  /* [0xd7] SAA5057: 0x6c */
  { 000, 000, 007, 011, 011, 011, 031, 000, 000 }, /* U+043B CYRILLIC SMALL LETTER EL */
  /* [0xd8] SAA5057: 0x6d */
  { 000, 000, 021, 033, 025, 021, 021, 000, 000 }, /* U+043C CYRILLIC SMALL LETTER EM */
  /* [0xd9] SAA5057: 0x6e */
  { 000, 000, 021, 021, 037, 021, 021, 000, 000 }, /* U+043D CYRILLIC SMALL LETTER EN */
  /* [0xda] SAA5057: 0x6f */
  { 000, 000, 016, 021, 021, 021, 016, 000, 000 }, /* U+043E CYRILLIC SMALL LETTER O */
  /* [0xdb] SAA5057: 0x70 */
  { 000, 000, 037, 021, 021, 021, 021, 000, 000 }, /* U+043F CYRILLIC SMALL LETTER PE */
  /* [0xdc] SAA5057: 0x71 */
  { 000, 000, 017, 021, 017, 005, 031, 000, 000 }, /* U+044F CYRILLIC SMALL LETTER YA */
  /* [0xdd] SAA5057: 0x72 */
  { 000, 000, 036, 021, 021, 021, 036, 020, 020 }, /* U+0440 CYRILLIC SMALL LETTER ER */
  /* [0xde] SAA5057: 0x73 */
  { 000, 000, 016, 021, 020, 021, 016, 000, 000 }, /* U+0441 CYRILLIC SMALL LETTER ES */
  /* [0xdf] SAA5057: 0x74 */
  { 000, 000, 037, 004, 004, 004, 004, 000, 000 }, /* U+0442 CYRILLIC SMALL LETTER TE */
  /* [0xe0] SAA5057: 0x75 */
  { 000, 000, 021, 021, 021, 021, 017, 001, 016 }, /* U+0443 CYRILLIC SMALL LETTER U */
  /* [0xe1] SAA5057: 0x76 */
  { 000, 000, 025, 025, 016, 025, 025, 000, 000 }, /* U+0436 CYRILLIC SMALL LETTER ZHE */
  /* [0xe2] SAA5057: 0x77 */
  { 000, 000, 036, 021, 036, 021, 036, 000, 000 }, /* U+0432 CYRILLIC SMALL LETTER VE */
  /* [0xe3] SAA5057: 0x78 */
  { 000, 000, 020, 020, 036, 021, 036, 000, 000 }, /* U+044C CYRILLIC SMALL LETTER SOFT SIGN */
  /* [0xe4] SAA5057: 0x79 */
  { 000, 000, 030, 010, 016, 011, 016, 000, 000 }, /* U+044A CYRILLIC SMALL LETTER HARD SIGN */
  /* [0xe5] SAA5057: 0x7a */
  { 000, 000, 016, 021, 006, 021, 016, 000, 000 }, /* U+0437 CYRILLIC SMALL LETTER ZE */
  /* [0xe6] SAA5057: 0x7b */
  { 000, 000, 025, 025, 025, 025, 037, 000, 000 }, /* U+0448 CYRILLIC SMALL LETTER SHA */
  /* [0xe7] SAA5057: 0x7c */
  { 000, 000, 014, 022, 006, 022, 014, 000, 000 }, /* U+044D CYRILLIC SMALL LETTER E */
  /* [0xe8] SAA5057: 0x7d */
  { 000, 000, 025, 025, 025, 025, 037, 001, 000 }, /* U+0449 CYRILLIC SMALL LETTER SHCHA */
  /* [0xe9] SAA5057: 0x7e */
  { 000, 000, 021, 021, 021, 017, 001, 000, 000 }, /* U+0447 CYRILLIC SMALL LETTER CHE */

  /* Basic Latin symbols not properly represented in SAA505x hardware */
  { 004, 002, 000, 000, 000, 000, 000, 000, 000 }, /* [0x00ea] U+0060 GRAVE ACCENT */
  { 004, 004, 004, 004, 004, 004, 004, 000, 000 }, /* [0x00eb] U+007C VERTICAL BAR */

  /* Latin-1 Supplement symbols not properly represented in SAA505x hardware */
  { 004, 000, 004, 004, 004, 004, 004, 000, 000 }, /* [0x00ec] U+00A1 INVERTED EXCLAMATION MARK */
  { 000, 004, 017, 024, 024, 024, 017, 004, 000 }, /* [0x00ed] U+00A2 CENT SIGN */
  { 021, 012, 004, 037, 004, 004, 004, 000, 000 }, /* [0x00ee] U+00A5 YEN SIGN */
  { 012, 000, 000, 000, 000, 000, 000, 000, 000 }, /* [0x00ef] U+00A8 DIAERESIS */
  { 016, 021, 027, 031, 027, 021, 016, 000, 000 }, /* [0x00f0] U+00A9 COPYRIGHT SIGN */
  { 006, 001, 007, 011, 007, 000, 017, 000, 000 }, /* [0x00f1] U+00AA FEMININE ORDINAL INDICATOR */
  { 000, 005, 012, 024, 012, 005, 000, 000, 000 }, /* [0x00f2] U+00AB LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
  { 000, 000, 037, 001, 001, 000, 000, 000, 000 }, /* [0x00f3] U+00AC NOT SIGN */
  { 000, 000, 000, 016, 000, 000, 000, 000, 000 }, /* [0x00f4] U+00AD SOFT HYPHEN */
  { 016, 035, 033, 035, 033, 021, 016, 000, 000 }, /* [0x00f5] U+00AE REGISTERED SIGN */
  { 037, 000, 000, 000, 000, 000, 000, 000, 000 }, /* [0x00f6] U+00AF MACRON */
  { 000, 004, 004, 037, 004, 004, 037, 000, 000 }, /* [0x00f7] U+00B1 PLUS-MINUS SIGN */
  { 014, 002, 004, 010, 016, 000, 000, 000, 000 }, /* [0x00f8] U+00B2 SUPERSCRIPT TWO */
  { 014, 002, 014, 002, 014, 000, 000, 000, 000 }, /* [0x00f9] U+00B3 SUPERSCRIPT THREE */
  { 002, 004, 000, 000, 000, 000, 000, 000, 000 }, /* [0x00fa] U+00B4 ACUTE ACCENT */
  { 000, 000, 022, 022, 022, 022, 035, 020, 020 }, /* [0x00fb] U+00B5 MICRO SIGN */
  { 017, 035, 035, 035, 015, 005, 005, 005, 005 }, /* [0x00fc] U+00B6 PILCROW SIGN */
  { 000, 000, 000, 004, 000, 000, 000, 000, 000 }, /* [0x00fd] U+00B7 MIDDLE DOT */
  { 000, 000, 000, 000, 000, 000, 000, 004, 014 }, /* [0x00fe] U+00B8 CEDILLA */
  { 020, 020, 020, 020, 020, 000, 000, 000, 000 }, /* [0x00ff] U+00B9 SUPERSCRIPT ONE */
  { 006, 011, 011, 011, 006, 000, 017, 000, 000 }, /* [0x0100] U+00BA MASCULINE ORDINAL INDICATOR */
  { 000, 024, 012, 005, 012, 024, 000, 000, 000 }, /* [0x0101] U+00BB RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
  { 004, 000, 004, 004, 010, 021, 016, 000, 000 }, /* [0x0102] U+00BF INVERTED QUESTION MARK */
  { 010, 004, 016, 021, 037, 021, 021, 000, 000 }, /* [0x0103] U+00C0 LATIN CAPITAL LETTER A WITH GRAVE */
  { 002, 004, 016, 021, 037, 021, 021, 000, 000 }, /* [0x0104] U+00C1 LATIN CAPITAL LETTER A WITH ACUTE */
  { 004, 012, 016, 021, 037, 021, 021, 000, 000 }, /* [0x0105] U+00C2 LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
  { 005, 012, 016, 021, 037, 021, 021, 000, 000 }, /* [0x0106] U+00C3 LATIN CAPITAL LETTER A WITH TILDE */
  { 017, 024, 024, 036, 024, 024, 027, 000, 000 }, /* [0x0107] U+00C6 LATIN CAPITAL LETTER AE */
  { 016, 021, 020, 020, 020, 021, 016, 004, 014 }, /* [0x0108] U+00C7 LATIN CAPITAL LETTER C WITH CEDILLA */
  { 010, 004, 037, 020, 036, 020, 037, 000, 000 }, /* [0x0109] U+00C8 LATIN CAPITAL LETTER E WITH GRAVE */
  { 004, 012, 037, 020, 036, 020, 037, 000, 000 }, /* [0x010a] U+00CA LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
  { 012, 000, 037, 020, 036, 020, 037, 000, 000 }, /* [0x010b] U+00CB LATIN CAPITAL LETTER E WITH DIAERESIS */
  { 010, 004, 016, 004, 004, 004, 016, 000, 000 }, /* [0x010c] U+00CC LATIN CAPITAL LETTER I WITH GRAVE */
  { 002, 004, 016, 004, 004, 004, 016, 000, 000 }, /* [0x010d] U+00CD LATIN CAPITAL LETTER I WITH ACUTE */
  { 004, 012, 000, 016, 004, 004, 016, 000, 000 }, /* [0x010e] U+00CE LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
  { 012, 000, 016, 004, 004, 004, 016, 000, 000 }, /* [0x010f] U+00CF LATIN CAPITAL LETTER I WITH DIAERESIS */
  { 016, 011, 011, 035, 011, 011, 016, 000, 000 }, /* [0x0110] U+00D0 LATIN CAPITAL LETTER ETH */
  { 012, 024, 021, 031, 025, 023, 021, 000, 000 }, /* [0x0111] U+00D1 LATIN CAPITAL LETTER N WITH TILDE */
  { 010, 004, 016, 021, 021, 021, 016, 000, 000 }, /* [0x0112] U+00D2 LATIN CAPITAL LETTER O WITH GRAVE */
  { 002, 004, 016, 021, 021, 021, 016, 000, 000 }, /* [0x0113] U+00D3 LATIN CAPITAL LETTER O WITH ACUTE */
  { 004, 012, 016, 021, 021, 021, 016, 000, 000 }, /* [0x0114] U+00D4 LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
  { 005, 012, 016, 021, 021, 021, 016, 000, 000 }, /* [0x0115] U+00D5 LATIN CAPITAL LETTER O WITH TILDE */
  { 000, 000, 012, 004, 012, 000, 000, 000, 000 }, /* [0x0116] U+00D7 MULTIPLICATION SIGN */
  { 016, 021, 023, 025, 031, 021, 016, 000, 000 }, /* [0x0117] U+00D8 LATIN CAPITAL LETTER O WITH STROKE */
  { 010, 004, 021, 021, 021, 021, 016, 000, 000 }, /* [0x0118] U+00D9 LATIN CAPITAL LETTER U WITH GRAVE */
  { 002, 004, 021, 021, 021, 021, 016, 000, 000 }, /* [0x0119] U+00DA LATIN CAPITAL LETTER U WITH ACUTE */
  { 004, 012, 021, 021, 021, 021, 016, 000, 000 }, /* [0x011a] U+00DB LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
  { 002, 004, 021, 012, 004, 004, 004, 000, 000 }, /* [0x011b] U+00DD LATIN CAPITAL LETTER Y WITH ACUTE */
  { 034, 010, 016, 011, 016, 010, 034, 000, 000 }, /* [0x011c] U+00DE LATIN CAPITAL LETTER THORN */
  { 002, 004, 016, 001, 017, 021, 017, 000, 000 }, /* [0x011d] U+00E1 LATIN SMALL LETTER A WITH ACUTE */
  { 005, 012, 016, 001, 017, 021, 017, 000, 000 }, /* [0x011e] U+00E3 LATIN SMALL LETTER A WITH TILDE */
  { 000, 000, 016, 005, 016, 024, 017, 000, 000 }, /* [0x011f] U+00E6 LATIN SMALL LETTER AE */
  { 000, 000, 017, 020, 020, 020, 017, 004, 014 }, /* [0x0120] U+00E7 LATIN SMALL LETTER C WITH CEDILLA */
  { 002, 004, 000, 014, 004, 004, 016, 000, 000 }, /* [0x0121] U+00ED LATIN SMALL LETTER I WITH ACUTE */
  { 002, 007, 002, 016, 022, 022, 014, 000, 000 }, /* [0x0122] U+00F0 LATIN SMALL LETTER ETH */
  { 012, 024, 036, 021, 021, 021, 021, 000, 000 }, /* [0x0123] U+00F1 LATIN SMALL LETTER N WITH TILDE */
  { 002, 004, 000, 016, 021, 021, 016, 000, 000 }, /* [0x0124] U+00F3 LATIN SMALL LETTER O WITH ACUTE */
  { 004, 012, 000, 016, 021, 021, 016, 000, 000 }, /* [0x0125] U+00F4 LATIN SMALL LETTER O WITH CIRCUMFLEX */
  { 005, 012, 000, 016, 021, 021, 016, 000, 000 }, /* [0x0126] U+00F5 LATIN SMALL LETTER O WITH TILDE */
  { 000, 000, 017, 023, 025, 031, 036, 000, 000 }, /* [0x0127] U+00F8 LATIN SMALL LETTER O WITH STROKE */
  { 010, 004, 000, 021, 021, 021, 017, 000, 000 }, /* [0x0128] U+00F9 LATIN SMALL LETTER U WITH GRAVE */
  { 002, 004, 000, 021, 021, 021, 017, 000, 000 }, /* [0x0129] U+00FA LATIN SMALL LETTER U WITH ACUTE */
  { 002, 004, 000, 021, 021, 021, 017, 001, 016 }, /* [0x012a] U+00FD LATIN SMALL LETTER Y WITH ACUTE */
  { 030, 010, 010, 016, 011, 011, 016, 010, 034 }, /* [0x012b] U+00FE LATIN SMALL LETTER THORN */
  { 000, 012, 000, 021, 021, 021, 017, 001, 016 }, /* [0x012c] U+00FF LATIN SMALL LETTER Y WITH DIAERESIS */
  { 000, 000, 000, 000, 000, 000, 037, 000, 037 }, /* [0x012d] U+2017 DOUBLE LOW LINE */
  { 000, 000, 000, 000, 000, 000, 004, 004, 010 }, /* [0x012e] U+201A SINGLE LOW-9 QUOTATION MARK */
  { 002, 004, 004, 000, 000, 000, 000, 000, 000 }, /* [0x012f] U+201B SINGLE HIGH_REVERSED-9 QUOTATION MARK */
  { 022, 022, 011, 000, 000, 000, 000, 000, 000 }, /* [0x0130] U+201C LEFT DOUBLE QUOTATION MARK */
  { 011, 011, 022, 000, 000, 000, 000, 000, 000 }, /* [0x0131] U+201D RIGHT DOUBLE QUOTATION MARK */
  { 000, 000, 000, 000, 000, 000, 011, 011, 022 }, /* [0x0132] U+201E DOUBLE LOW-9 QUOTATION MARK */
  { 011, 022, 022, 000, 000, 000, 000, 000, 000 }, /* [0x0133] U+201F DOUBLE HIGH_REVERSED-9 QUOTATION MARK */
  { 004, 016, 004, 004, 004, 004, 004, 000, 000 }, /* [0x0134] U+2020 DAGGER */
  { 004, 016, 004, 004, 004, 016, 004, 000, 000 }, /* [0x0135] U+2021 DOUBLE DAGGER */
  { 000, 000, 000, 004, 016, 004, 000, 000, 000 }, /* [0x0136] U+2022 BULLET */
  { 000, 000, 010, 014, 016, 014, 010, 000, 000 }, /* [0x0137] U+2023 TRIANGULAR BULLET */
  { 000, 000, 000, 000, 000, 000, 004, 000, 000 }, /* [0x0138] U+2024 ONE DOT LEADER */
  { 000, 000, 000, 000, 000, 000, 012, 000, 000 }, /* [0x0139] U+2025 TWO DOT LEADER */
  { 000, 000, 000, 000, 000, 000, 025, 000, 000 }, /* [0x013a] U+2026 HORIZONTAL ELLIPSIS */
  { 000, 000, 000, 004, 000, 000, 000, 000, 000 }, /* [0x013b] U+2027 HYPHENATION POINT */

  /* Arrows not properly represented in SAA505x hardware */
  { 000, 004, 004, 025, 016, 004, 000, 000, 000 }, /* [0x013c] U+2193 DOWNWARDS ARROW */

  /* Geometric shapes not properly represented in SAA505x hardware */
  { 037, 021, 021, 021, 021, 021, 037, 000, 000 }, /* [0x013d] U+25A1 WHITE SQUARE */

  /* Replacement character for invalid UTF-8 byte sequence */
  { 016, 021, 035, 031, 037, 033, 016, 000, 000 }, /* [0x013e] U+FFFD REPLACEMENT CHARACTER */

  /* Currency symbols */
  { 007, 010, 036, 010, 036, 010, 007, 000, 000 }, /* [0x013f] U+20AC EURO SIGN */

  /* Placeholder for undefined glyphs (this glyph must come last) */
  { 000, 016, 002, 006, 000, 004, 000, 000, 000 }, /* [......] ...... undefined glyph */
};
