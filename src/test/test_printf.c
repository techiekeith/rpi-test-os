/*
 * test_utf8.c - entry point for tests.
 */

#include "../../include/common/stdio.h"
#include "../../include/common/utf8.h"

void test_printf()
{
    printf("Format percent symbol: %%\r\n");
    printf("  Align left: | %-20s |\r\n", "Hello, world!");
    printf("Align centre: | %|20s |\r\n", "Hello, world!");
    printf("Align centre: | %|20s |\r\n", "Hello, world!!");
    printf(" Align right: | %20s |\r\n", "Hello, world!");
    printf("Format decimal with 'd' (positive): %d\r\n", 12345678);
    printf("Format decimal with 'd' (negative): %d\r\n", -12345678);
    printf("Format decimal with 'i' (positive): %i\r\n", 12345678);
    printf("Format decimal with 'i' (negative): %i\r\n", -12345678);
    printf("Format unsigned decimal (positive): %u\r\n", 12345678);
    printf("Format unsigned decimal (negative): %u\r\n", -12345678);
    printf("Format decimal with 'ld' (positive): %ld\r\n", 12345678L);
    printf("Format decimal with 'ld' (negative): %ld\r\n", -12345678L);
    printf("Format unsigned decimal with 'lu': %lu\r\n", 12345678L);
    printf("Format decimal with 'lld' (positive): %lld\r\n", 1234567890123456LL);
    printf("Format decimal with 'lld' (negative): %lld\r\n", -1234567890123456LL);
    printf("Format unsigned decimal with 'llu': %llu\r\n", 1234567890123456LL);
    printf("Format binary: %b\r\n", 0x96); // 10010110
    printf("Format octal: %o\r\n", 01234567);
    printf("Format hexadecimal: %x\r\n", 0xdeadbeef);
    printf("Format pointer: %p\r\n", (void *)0x01c0ffee);
}
