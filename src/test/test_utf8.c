/*
 * test_utf8.c - entry point for tests.
 */

#include "../../include/common/stdio.h"
#include "../../include/common/utf8.h"

void test_utf8()
{
    char buffer[15];
    utf8_encode(0x20ac, buffer);
    printf("U+20AC (EURO SIGN) -> UTF-8: %x %x %x\r\n", (int)buffer[0], (int)buffer[1], (int)buffer[2]);
    ucs16_to_utf8(buffer, u"\u20ac\u20ac\u20ac", 15, 4);
    printf("Converted from UCS-16: %s\r\n", buffer);
    printf("Euro symbol: as_escaped_hex=\xe2\x82\xac as_escaped_unicode=\u20ac in_string_arg=%s in_char_arg=%c\r\n",
           "\xe2\x82\xac", 0x20ac);
    printf("Replacement character: %c\r\n", 0xfffd);
    printf("Unknown character: %c\r\n", 0xfffe);
    printf("Double height, unicode  (hi) = "
           "\U000f0048\U000f0065\U000f006c\U000f006c\U000f006f\U000f002c\U000f0020"
           "\U000f0077\U000f006f\U000f0072\U000f006c\U000f0064\U000f0021\r\n");
    printf("Double height, unicode (low) = "
           "\U00100048\U00100065\U0010006c\U0010006c\U0010006f\U0010002c\U00100020"
           "\U00100077\U0010006f\U00100072\U0010006c\U00100064\U00100021\r\n");
    printf("Double height, format   (hi) = %^%s%=\r\n", "Hello, world!");
    printf("Double height, format  (low) = %_%s%=\r\n", "Hello, world!");
}
