/*
 * utf8.c
 */

#include <common/stddef.h>
#include <common/stdint.h>

static char default_utf8_buffer[5];

#define REPLACEMENT_CHARACTER 0xfffd

/**
 * Encodes a code point into a UTF-8 sequence. Invalid code points (U+D800..U+DFFF or above U+10FFFF)
 * are converted to U+FFFD REPLACEMENT CHARACTER before encoding.
 *
 * @param c the character to encode
 * @param buffer the buffer into which to place the encoded byte sequence; if NULL, uses a static buffer instead
 * @return the pointer to the buffer
 */
char *utf8_encode(int c, char *buffer)
{
    if (!buffer) buffer = default_utf8_buffer;
    char *p = buffer;

    if (c >= 0x11000 || (c >= 0xd800 && c <= 0xdfff))
    {
        c = REPLACEMENT_CHARACTER;
    }
    if (c >= 0x10000)
    {
        *p++ = 0xf0 | (c >> 18);
        *p++ = 0x80 | ((c >> 12) & 0x3f);
        *p++ = 0x80 | ((c >> 6) & 0x3f);
        *p++ = 0x80 | (c & 0x3f);
    }
    else if (c >= 0x800)
    {
        *p++ = 0xe0 | (c >> 12);
        *p++ = 0x80 | ((c >> 6) & 0x3f);
        *p++ = 0x80 | (c & 0x3f);
    }
    else if (c >= 0x80)
    {
        *p++ = 0xc0 | (c >> 6);
        *p++ = 0x80 | (c & 0x3f);
    }
    else
    {
        *p++ = c;
    }

    *p = '\0';
    return buffer;
}

/**
 * Decodes a UTF-8 byte sequence.
 * Sets 'next' to point to the character following the sequence.
 * If 'sequence' does not point to a valid UTF-8 byte sequence, returns U+FFFD REPLACEMENT CHARACTER.
 */
int utf8_decode(const char *s, char **next)
{
    uint8_t *p = (uint8_t *)s;
    uint8_t c0 = *p++;
    *next = p;
    if (!(c0 & 0x80))
        return (int)c0; // ASCII character
    if (c0 < 0xc2 || c0 > 0xf4)
        return REPLACEMENT_CHARACTER; // Not a valid start character
    uint8_t c1 = *p++;
    if ((c1 & 0xc0) != 0x80)
        return REPLACEMENT_CHARACTER; // Not a continuation character
    if (c0 < 0xe0)
    {
        *next = p;
        return ((c0 & 0x1f) << 6) | (c1 & 0x3f); // Valid 2-byte sequence
    }
    uint8_t c2 = *p++;
    if ((c2 & 0xc0) != 0x80)
        return REPLACEMENT_CHARACTER; // Not a continuation character
    if ((c0 == 0xe0) && ((c1 & 0xe0) == 0x80))
        return REPLACEMENT_CHARACTER; // overlong 3-byte sequence
    if ((c0 == 0xed) && ((c1 & 0xe0) == 0xa0))
        return REPLACEMENT_CHARACTER; // U+D800..U+DFFF are not valid UTF-8
    if (c0 < 0xf0)
    {
        *next = p;
        return ((c0 & 0xf) << 12) | ((c1 & 0x3f) << 6) | (c2 & 0x3f); // Valid 3-byte sequence
    }
    uint8_t c3 = *p++;
    if ((c3 & 0xc0) != 0x80)
        return REPLACEMENT_CHARACTER; // Not a continuation character
    if ((c0 == 0xf0) && ((c1 & 0xf0) == 0x80))
        return REPLACEMENT_CHARACTER; // overlong 4-byte sequence
    if ((c0 == 0xf4) && ((c1 & 0xf0) != 0x80))
        return REPLACEMENT_CHARACTER; // UTF-8 ends at U+10FFFF
    *next = p;
    return ((c0 & 7) << 18) | ((c1 & 0x3f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f); // Valid 4-byte sequence
}

size_t utf8_strlen(const char *s)
{
    size_t i;
    char *p = (char *)s;
    char *q;
    for (i = 0; *p; i++)
    {
        utf8_decode(p, &q);
        p = q;
    }
    return i;
}
