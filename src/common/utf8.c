/*
 * utf8.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/string.h"

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

/*
 * Converts a UCS-16 string (such as is found in USB strings) into UTF-8, up to a maximum number of bytes
 * including the terminating NUL character. The resulting string will always be NUL-terminated, and truncated
 * a UTF-8 byte sequence boundary if necessary to fit within the specified target buffer size.
 */
void ucs16_to_utf8(char *utf8_buffer, uint16_t *ucs16_string, size_t utf8_buffer_size, size_t ucs16_string_length)
{
    char *p = utf8_buffer;
    int bytes_remaining = utf8_buffer_size;
    uint16_t code_point = 0;
    uint16_t surrogate = 0;
    for (size_t i = 0; i < ucs16_string_length; i++)
    {
        if (surrogate == 0)
        {
            if ((ucs16_string[i] & 0xfc00) == 0xd800)
            {
                surrogate = ucs16_string[i];
                continue;
            }
            else if ((code_point & 0xfc00) == 0xdc00)
            {
                // Unpaired low surrogate, use placeholder
                code_point = REPLACEMENT_CHARACTER;
            }
            else
            {
                code_point = ucs16_string[i];
            }
        }
        else
        {
            if ((ucs16_string[i] & 0xfc00) == 0xdc00)
            {
                code_point = 0x10000 | ((surrogate & 0x3ff) << 10) | (ucs16_string[i] & 0x3ff);
            }
            else
            {
                // Unpaired high surrogate, use placeholder and rewind
                code_point = REPLACEMENT_CHARACTER;
                i--;
            }
            surrogate = 0;
        }
        int size;
        if (bytes_remaining > 4)
        {
            utf8_encode(code_point, p);
            size = strlen(p);
        }
        else
        {
            utf8_encode(code_point, default_utf8_buffer);
            size = strlen(default_utf8_buffer);
            if (size >= bytes_remaining) break;
            strncpy(p, default_utf8_buffer, size);
        }
        if (!size) size++;
        p += size;
        bytes_remaining -= size;
    }
    *p = '\0';
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
    *next = (char *)p;
    if (!(c0 & 0x80))
        return (int)c0; // ASCII character
    if (c0 < 0xc2 || c0 > 0xf4)
        return REPLACEMENT_CHARACTER; // Not a valid start character
    uint8_t c1 = *p++;
    if ((c1 & 0xc0) != 0x80)
        return REPLACEMENT_CHARACTER; // Not a continuation character
    if (c0 < 0xe0)
    {
        *next = (char *)p;
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
        *next = (char *)p;
        return ((c0 & 0xf) << 12) | ((c1 & 0x3f) << 6) | (c2 & 0x3f); // Valid 3-byte sequence
    }
    uint8_t c3 = *p++;
    if ((c3 & 0xc0) != 0x80)
        return REPLACEMENT_CHARACTER; // Not a continuation character
    if ((c0 == 0xf0) && ((c1 & 0xf0) == 0x80))
        return REPLACEMENT_CHARACTER; // overlong 4-byte sequence
    if ((c0 == 0xf4) && ((c1 & 0xf0) != 0x80))
        return REPLACEMENT_CHARACTER; // UTF-8 ends at U+10FFFF
    *next = (char *)p;
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
