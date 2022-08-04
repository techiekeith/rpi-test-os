/*
 * stdio.c
 */

#include <kernel/gpu.h>
#include <kernel/uart.h>
#include <common/stdarg.h>
#include <common/stdio.h>
#include <common/stdlib.h>

static int input_channel, output_channel;

void set_input_channel(int channel)
{
    input_channel = channel;
}

void set_output_channel(int channel)
{
    output_channel = channel;
}

char getc(void)
{
    return uart_getc();
}

void putc(char c)
{
    if (output_channel == OUTPUT_CHANNEL_UART)
    {
        uart_putc(c);
    }
    else if (output_channel == OUTPUT_CHANNEL_GPU)
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

void printf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);

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

    va_end(args);
}
