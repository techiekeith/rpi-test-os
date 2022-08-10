/*
 * stdio.c
 */

#include <common/stdarg.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>
#include <common/utf8.h>
#include <kernel/graphics.h>
#include <kernel/io.h>
#include <kernel/uart.h>

char getc()
{
    // TODO get from USB keyboard
    return uart_getc();
}

void putc(int c)
{
    int channel = get_output_channel();
    if (channel == OUTPUT_CHANNEL_UART)
    {
        uart_putc(c);
    }
    else if (channel == OUTPUT_CHANNEL_GRAPHICS)
    {
        graphics_putc(c);
    }
}

void print_truncated_string_with_offset(const char *str, size_t n, int offset)
{
    size_t i;
    char *p = (char *)str;
    char *q;
    for (i = 0; (!n || i < n) && *p; i++)
    {
        putc(utf8_decode(p, &q) + offset);
        p = q;
    }
}

void puts(const char *str)
{
    print_truncated_string_with_offset(str, 0, 0);
}

void print_string_with_modifiers(const char *str, int align, int pad_char, int double_height, size_t min_width, size_t max_width)
{
    int offset = double_height ? 0xe0000 + (double_height << 16) : 0;
    pad_char += offset;
    if (align == 1) print_truncated_string_with_offset(str, max_width, offset);
    size_t length = utf8_strlen(str);
    if (align == 2)
    {
        for (size_t count = 0; count <= (min_width - length) / 2; count++)
        {
            putc(pad_char);
            length++;
        }
        print_truncated_string_with_offset(str, max_width, offset);
    }
    while (length++ < min_width) putc(pad_char);
    if (align == 0) print_truncated_string_with_offset(str, max_width, offset);
}

void vprintf(const char *fmt, va_list args)
{
    char buffer[256];
    int double_height = 0;
    while (*fmt)
    {
        if (*fmt == '%')
        {
            char modifier = *(++fmt);
            if (!modifier) return;
            int align = 0;
            int pad_char = ' ';
            size_t min_width = 0;
            size_t max_width = 0;
            int unsigned_number = 0;
            int long_number = 0;
            int radix = 2;
            int suppress = 0;
            if (modifier == '^') /* top row for double-height characters */
            {
                double_height = 1;
            }
            if (modifier == '_') /* bottom row for double-height characters */
            {
                double_height = 2;
            }
            if (modifier == '=') /* restore normal height */
            {
                double_height = 0;
            }
            if (modifier == '-') /* left align */
            {
                align = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == '|') /* centre align */
            {
                align = 2;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == '0') /* zero padding; only makes sense with %b, %d, %o or %x */
            {
                pad_char = '0';
                modifier = *(++fmt);
                if (!modifier) return;
            }
            while (modifier >= '0' && modifier <= '9') /* minimum field width */
            {
                min_width *= 10;
                min_width += modifier - '0';
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == '.') /* field width or precision separator */
            {
                modifier = *(++fmt);
                if (!modifier) return;
                while (modifier >= '0' && modifier <= '9')
                {
                    max_width *= 10;
                    max_width += modifier - '0';
                    modifier = *(++fmt);
                    if (!modifier) return;
                }
            }
            if (modifier == 'u') /* unsigned integer; only makes sense with %b, %d, %o or %x */
            {
                unsigned_number = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == 'l') /* long integer; only makes sense with %b, %d, %o or %x */
            {
                long_number = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            char *str = buffer;
            char *p = str;
            *p = '\0';
            switch (modifier)
            {
                case 'p': /* pointer */
                    *p++ = '0';
                    *p++ = 'x';
                    // check for 32- or 64-bit and set long_number accordingly?
                    /* fall through */
                case 'x': /* hexadecimal */
                    radix += 6;
                    /* fall through */
                case 'd': /* decimal */
                    radix += 2;
                    /* fall through */
                case 'o': /* octal */
                    radix += 6;
                    /* fall through */
                case 'b': /* binary */
                    long_number ? (
                            unsigned_number ? ultoa(va_arg(args, unsigned long long), radix, p)
                            : ltoa(va_arg(args, long long), radix, p))
                    : (
                            unsigned_number ? uitoa(va_arg(args, unsigned int), radix, p)
                            : itoa(va_arg(args, int), radix, p));
                    break;
                case 's': /* string */
                    str = va_arg(args, char *);
                    break;
                case 'c': /* code point */
                    utf8_encode(va_arg(args, int), buffer);
                    break;
                case '%': /* literal percent */
                    *p++ = '%';
                    *p++ = '\0';
                    break;
                default:
                    suppress = 1;
                    *p = '\0';
            }
            if (!suppress)
                print_string_with_modifiers(str, align, pad_char, double_height, min_width, max_width);
            fmt++;
        }
        else
        {
            putc(utf8_decode(fmt, (char **)&fmt));
        }
    }
}

void printf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void debug_printf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int channel = get_output_channel();
    if (channel != OUTPUT_CHANNEL_UART)
    {
        set_output_channel(OUTPUT_CHANNEL_UART);
    }
    vprintf(fmt, args);
    if (channel != OUTPUT_CHANNEL_UART)
    {
        set_output_channel(channel);
    }
    va_end(args);
}
