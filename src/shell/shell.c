/*
 * shell.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>
#include <kernel/framebuffer.h>
#include <kernel/graphics.h>
#include <kernel/mailbox.h>

// To get size of kernel
void _start();
extern uint8_t __end;

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

void cmd_charset()
{
    puts("\r\nBasic Latin:");
    for (int i = 0x20; i < 0x7f; i++) {
        if (i % 32 == 0) puts("\r\n");
        putc(i);
    }
    puts("\r\n\r\nLatin-1 Supplement:");
    for (int i = 0xa0; i <= 0xff; i++) {
        if (i % 32 == 0) puts("\r\n");
        putc(i);
    }
    puts("\r\n\r\nCyrillic:");
    for (int i = 0x410; i <= 0x44f; i++) {
        if (i % 32 == 16) puts("\r\n");
        putc(i);
    }
    puts("\r\n\r\nHebrew:\r\n");
    for (int i = 0x5d0; i <= 0x5ea; i++) {
        putc(i);
    }
    puts("\r\n\r\nGeneral Punctuation:\r\n");
    for (int i = 0x2010; i <= 0x2027; i++) {
        putc(i);
    }
    puts("\r\n\r\nEuro  Arrows  Shapes  Repl.\r\n");
    for (int i = 0x20ac; i <= 0x20ac; i++) {
        putc(i);
    }
    puts("     ");
    for (int i = 0x2190; i <= 0x2193; i++) {
        putc(i);
    }
    puts("    ");
    for (int i = 0x25a0; i <= 0x25a1; i++) {
        putc(i);
    }
    puts("      ");
    for (int i = 0xfffd; i <= 0xfffe; i++) {
        putc(i);
    }
    puts("\r\n\r\nPUA U+F500..7F (SAA505x box graphics):");
    for (int i = 0xf500; i <= 0xf57f; i++) {
        if (i % 32 == 0) puts("\r\n");
        putc(i);
    }
    puts("\r\n\r\nPUA U+F580..8B (deprecated SAA505x characters):\r\n");
    for (int i = 0xf580; i <= 0xf58b; i++) {
        putc(i);
    }
    puts("\r\n");
}

void cmd_debug()
{
    puts("\r\n");
    printf("%20s | %-8s | %-8s\r\n", "Memory Usage", "Start", "End");
    printf("%20s-+-%-8s-+-%-9s\r\n", "--------------------", "--------", "---------");
    printf("%20s | %08x | %08x\r\n", "kernel", &_start, &__end - 1);
    printf("%20s | %08x | %08x\r\n", "framebuffer", fbinfo.buf, fbinfo.buf + fbinfo.buf_size - 1);
    printf("%20s | %08x | %08x\r\n", "peripherals", PERIPHERAL_BASE, PERIPHERAL_BASE + PERIPHERAL_LENGTH - 1);
    printf("%20s | %08x |\r\n", "(mailbox)", PERIPHERAL_BASE + MAILBOX_OFFSET);
    printf("%20s | %08x |\r\n", "(GPIO)", PERIPHERAL_BASE + GPIO_OFFSET);
    printf("%20s | %08x |\r\n", "(UART)", PERIPHERAL_BASE + UART0_OFFSET);
}

void cmd_help()
{
    puts("\r\nCommands:\r\n");
    puts("\tclear - clear the screen\r\n");
    puts("\tcharset - show character set\r\n");
    puts("\tdebug - show debug information\r\n");
    puts("\thalt - halt the shell\r\n");
    puts("\thelp - this text\r\n");
    puts("\tmode <width> <height> <depth> - set display mode\r\n");
    puts("\tpalette - show colour palette\r\n");
    puts("\treset - reset the shell\r\n");
    puts("\ttest - run tests\r\n");
}

void mode_syntax()
{
    puts("\r\nSyntax: mode <width> <height> <depth>\r\n");
}

void cmd_mode(char *args)
{
    int width, height, depth;
    char *arg1 = strchr(args, ' ');
    if (arg1)
    {
        *arg1++ = '\0';
        width = atoi(args);
        char *arg2 = strchr(arg1, ' ');
        if (arg2)
        {
            *arg2++ = '\0';
            height = atoi(arg1);
            depth = atoi(arg2);
            if (width < 640 || width > 3840)
            {
                printf("\r\nInvalid width '%s'. Valid values are between 640 and 3840.\r\n", args);
            }
            else if (height < 400 || height > 2160)
            {
                printf("\r\nInvalid height '%s'. Valid values are between 400 and 2160.\r\n", arg1);
            }
            else if (depth % 8 || depth < 8 || depth > 32)
            {
                printf("\r\nInvalid depth '%s'. Valid values are 8, 16, 24 and 32.\r\n", arg2);
            }
            else
            {
                set_display_mode(width, height, depth);
            }
            return;
        }
    }
    mode_syntax();
}

void cmd_palette()
{
    show_palette();
    cursor_home();
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

int shell(uint64_t mem_size)
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
            escape();
        }
        else if (!strcmp(command_buffer, "clear"))
        {
            cmd_clear();
        }
        else if (!strcmp(command_buffer, "charset"))
        {
            cmd_charset();
        }
        else if (!strcmp(command_buffer, "debug"))
        {
            cmd_debug();
        }
        else if (!strcmp(command_buffer, "help"))
        {
            cmd_help();
        }
        else if (!strcmp(command_buffer, "mode"))
        {
            mode_syntax();
        }
        else if (!strncmp(command_buffer, "mode ", 5))
        {
            cmd_mode(command_buffer + 5);
        }
        else if (!strcmp(command_buffer, "palette"))
        {
            cmd_palette();
        }
        else if (!strcmp(command_buffer, "test"))
        {
            cmd_test();
        }
        else if (strcmp(command_buffer, "") && strcmp(command_buffer, "halt") && strcmp(command_buffer, "reset"))
        {
            bad_command();
        }
    }
    return strcmp(command_buffer, "reset");
}