/*
 * shell.c
 */

#include <common/stdint.h>
#include <common/stdio.h>
#include <common/string.h>
#include <kernel/graphics.h>

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
    puts("\r\n\r\nArrows:\r\n");
    for (int i = 0x2190; i <= 0x2193; i++) {
        putc(i);
    }
    puts("\r\n\r\nGeometric Shapes:\r\n");
    for (int i = 0x25a0; i <= 0x25a1; i++) {
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

void cmd_help()
{
    puts("Commands:\r\n");
    puts("\tclear - clear the screen\r\n");
    puts("\tcharset - show character set\r\n");
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
            puts("\r\nEscape\r\n");
        }
        else if (!strcmp(command_buffer, "clear"))
        {
            cmd_clear();
        }
        else if (!strcmp(command_buffer, "charset"))
        {
            cmd_charset();
        }
        else if (!strcmp(command_buffer, "help"))
        {
            cmd_help();
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
            puts("Bad command\r\n");
        }
    }
    return strcmp(command_buffer, "reset");
}