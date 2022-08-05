/*
 * stdio.c
 */

#include <common/stdarg.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <kernel/gpu.h>
#include <kernel/io.h>
#include <kernel/uart.h>

char getc(void)
{
    return uart_getc();
}

void putc(char c)
{
    int channel = get_output_channel();
    if (channel == OUTPUT_CHANNEL_UART)
    {
        uart_putc(c);
    }
    else if (channel == OUTPUT_CHANNEL_GPU)
    {
        gpu_putc(c);
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
            switch (*(++fmt))
            {
                case '%':
                    putc('%');
                    break;
                case 'd':
                    puts(itoa(va_arg(args, int), 10));
                    break;
                case 'x':
                    puts(itoa(va_arg(args, int), 16));
                    break;
                case 'p':
                    putc('0');
                    putc('x');
                    puts(itoa(va_arg(args, int), 16));
                    break;
                case 's':
                    puts(va_arg(args, char *));
                    break;
                case 'c':
                    putc(va_arg(args, char));
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
