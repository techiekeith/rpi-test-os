/*
 * charset.c
 */

#include "../../include/common/stdio.h"

static void show_chars(int start, int end)
{
    for (int i = 0; i <= end - start; i++) {
        if (i != 0 && i % 32 == 0) puts("\r\n");
        putc(start + i);
    }
}

void show_charset()
{
    puts("\r\nBasic Latin:\r\n");
    show_chars(0x20, 0x7f);
    puts("\r\n\r\nLatin-1 Supplement:\r\n");
    show_chars(0xa0, 0xff);
    puts("\r\n\r\nCyrillic:\r\n");
    show_chars(0x410, 0x44f);
    puts("\r\n\r\nHebrew:\r\n");
    show_chars(0x5d0, 0x5ea);
    puts("\r\n\r\nGeneral Punctuation:\r\n");
    show_chars(0x2010, 0x2027);
    puts("\r\n\r\nEuro  Arrows  Shapes  Repl.\r\n");
    show_chars(0x20ac, 0x20ac);
    puts("     ");
    show_chars(0x2190, 0x2193);
    puts("    ");
    show_chars(0x25a0, 0x25a1);
    puts("      ");
    show_chars(0xfffd, 0xfffe);
    puts("\r\n\r\nPUA U+F500..7F (SAA505x box graphics):\r\n");
    show_chars(0xf500, 0xf57f);
    puts("\r\n\r\nPUA U+F580..8B (deprecated SAA505x characters):\r\n");
    show_chars(0xf580, 0xf58b);
    puts("\r\n");
}
