/*
 * stdio.c
 */

#include <common/stdarg.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <kernel/graphics.h>
#include <kernel/io.h>
#include <kernel/uart.h>

char getc()
{
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

void puts(const char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        putc(str[i]);
    }
}

void gets(char *buf, int buflen)
{
    int i;
    char c;
    // Leave a spot for null char in buffer
    for (i = 0; (c = getc()) != '\r' && buflen > 1; i++, buflen--)
    {
        putc(c);
        buf[i] = c;
    }

    putc('\r');
    putc('\n');
    buf[i] = '\0';
}

void vprintf(const char *fmt, va_list args)
{
    for (; *fmt != '\0'; fmt++)
    {
        if (*fmt == '%')
        {
            char modifier = *(++fmt);
            if (!modifier) return;
            int align = 0;
            int pad = 0;
            int width = 0;
            int unsign = 0;
            int length = 0;
            int radix = 2;
            if (modifier == '-')
            {
                align = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == '0')
            {
                pad = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            while (modifier >= '0' && modifier <= '9')
            {
                width *= 10;
                width += modifier - '0';
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == 'u')
            {
                unsign = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            if (modifier == 'l')
            {
                length = 1;
                modifier = *(++fmt);
                if (!modifier) return;
            }
            switch (modifier)
            {
                case 'p':
                    putc('0');
                    putc('x');
                case 'x':
                    radix += 6;
                case 'd':
                    radix += 2;
                case 'o':
                    radix += 6;
                case 'b':
                    puts(
                            length ? (
                                    unsign ? ultoa(va_arg(args, unsigned long long), radix)
                                    : ltoa(va_arg(args, long long), radix))
                            : (
                                    unsign ? uitoa(va_arg(args, unsigned int), radix)
                                    : itoa(va_arg(args, int), radix)));
                    break;
                case 's':
                    puts(va_arg(args, char *));
                    break;
                case 'c':
                    putc(va_arg(args, char));
                    break;
                case '%':
                    putc('%');
                    break;
            }
        } else putc(*fmt);
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
