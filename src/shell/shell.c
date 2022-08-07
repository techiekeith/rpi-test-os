/*
 * shell.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <common/string.h>
#include <kernel/gpu.h>

void run_tests();

#define COMMAND_BUFFER_SIZE 1024
char command_buffer[COMMAND_BUFFER_SIZE];

void bad_command()
{
    puts("\r\nBad command\r\n");
}

void escape()
{
    puts("\r\nEscape\r\n");
}

void cmd_clear()
{
    clear_framebuffer();
    cursor_home();
}

void cmd_halt()
{
    set_output_channel(OUTPUT_CHANNEL_UART);
    puts("System halted.\n");

    __asm("\ncmd_halt_loop:\n\twfe\n\tb cmd_halt_loop");
}

void cmd_help()
{
    puts("Commands:\r\n");
    puts("\tclear - clear the screen\r\n");
    puts("\thalt - halt the shell\r\n");
    puts("\thelp - this text\r\n");
    puts("\tpalette - show colour palette\r\n");
    puts("\treset - reset the shell\r\n");
    puts("\ttest - run tests\r\n");
    puts("\r\n");
}

void cmd_palette()
{
    show_palette();
    cursor_home();
}

void cmd_reset()
{
    set_output_channel(OUTPUT_CHANNEL_GPU);
}

void cmd_test()
{
    run_tests();
}

void prompt()
{
    putc('*');
}

int getline(char *buf, int buflen)
{
    int i = 0;
    char c;
    do
    {
        c = getc();
        if ((c == 8 || c == 127) && i > 0)
        {
            putc(127);
            i--;
        }
        if (c == 27)
        {
            i = -1;
        }
        if ((c >= 32 && c < 127) && i < buflen - 1)
        {
            putc(c);
            buf[i] = c;
            i++;
        }
    }
    while (c != '\n' && c != '\r' && c != 27);
    if (i >= 0) buf[i] = 0;
    putc('\r');
    putc('\n');
    return i;
}

void shell(uint64_t mem_size)
{
    cmd_clear();
    printf("\r\nRaspberry Pi %ldK\r\n\r\n", mem_size >> 10);
    command_buffer[0] = 0;
    while (strcmp(command_buffer, "halt") && strcmp(command_buffer, "reset"))
    {
        prompt();
        int i = getline(command_buffer, COMMAND_BUFFER_SIZE);
        if (i < 0)
        {
            puts("\r\nEscape\r\n");
        }
        else if (!strcmp(command_buffer, "clear"))
        {
            cmd_clear();
        }
        else if (!strcmp(command_buffer, "halt"))
        {
            cmd_halt();
        }
        else if (!strcmp(command_buffer, "help"))
        {
            cmd_help();
        }
        else if (!strcmp(command_buffer, "palette"))
        {
            cmd_palette();
        }
        else if (!strcmp(command_buffer, "reset"))
        {
            cmd_reset();
        }
        else if (!strcmp(command_buffer, "test"))
        {
            cmd_test();
        }
        else
        {
            puts("Bad command\r\n");
        }
    }
}