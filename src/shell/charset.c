/*
 * charset.c
 */

#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/graphics.h"

static void charset_syntax()
{
    printf("Syntax: charset [<#>]\r\n");
}

static void show_chars(int start, int end)
{
    for (int i = 0; i <= end - start; i++) {
        if (i != 0 && i % 32 == 0) puts("\r\n");
        putc(start + i);
    }
}

static void show_charset()
{
    puts("\r\nBasic Latin:\r\n");
    show_chars(0x20, 0x7e);
    puts("\r\n\r\nLatin-1 Supplement:\r\n");
    show_chars(0xa0, 0xff);
    puts("\r\n\r\nGreek:\r\n");
    show_chars(0x390, 0x3cf);
    puts("\r\n\r\nCyrillic:\r\n");
    show_chars(0x410, 0x44f);
    puts("\r\n\r\nHebrew:\r\n");
    show_chars(0x5d0, 0x5ea);
    puts("\r\n\r\nGeneral Punctuation:\r\n");
    show_chars(0x2010, 0x2027);
    puts("\r\n\r\nMathematical Operators:\r\n");
    show_chars(0x2200, 0x226f);
    puts("\r\n\r\nBox Drawings:\r\n");
    show_chars(0x2500, 0x257f);
    puts("\r\n\r\nEuro  Arrows  Blocks  Shapes  Repl.\r\n");
    show_chars(0x20ac, 0x20ac);
    puts("     ");
    show_chars(0x2190, 0x2193);
    puts("    ");
    show_chars(0x2592, 0x2592);
    puts("       ");
    show_chars(0x25a0, 0x25a1);
    puts("      ");
    show_chars(0xfffd, 0xfffe);
    puts("\r\n\r\nPUA U+F500..7F (SAA505x box graphics):\r\n");
    show_chars(0xf500, 0xf57f);
    puts("\r\n\r\nPUA U+F580..8B (deprecated SAA505x characters):\r\n");
    show_chars(0xf580, 0xf58b);
    puts("\r\n");
}

void show_or_set_charset(int argc, char **argv)
{
    switch (argc)
    {
        case 1:
            show_charset();
            break;
        case 2:
            charset_t charset = atoi(argv[1]);
            set_charset(charset);
            putc('\f');
            break;
        default:
            charset_syntax();
    }
}