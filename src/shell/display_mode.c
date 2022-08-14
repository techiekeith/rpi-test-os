/*
 * debug.c
 */

#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/io.h"

DEBUG_INIT("display_mode");

static void mode_syntax()
{
    puts("\r\nSyntax: mode <width> <height> <depth>\r\n");
}

void display_mode(int argc, char **argv)
{
    DEBUG_START("display_mode");
    if (argc < 4)
    {
        mode_syntax();
        DEBUG_END();
        return;
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int depth = atoi(argv[3]);
    if (width < 640 || width > 3840)
    {
        printf("\r\nInvalid width '%s'. Valid values are between 640 and 3840.\r\n", argv[1]);
    }
    else if (height < 400 || height > 2160)
    {
        printf("\r\nInvalid height '%s'. Valid values are between 400 and 2160.\r\n", argv[2]);
    }
    else if (depth % 8 || depth < 8 || depth > 32)
    {
        printf("\r\nInvalid depth '%s'. Valid values are 8, 16, 24 and 32.\r\n", argv[3]);
    }
    else
    {
        debug_printf("Mode: %dx%d, %d bpp\r\n", width, height, depth);
        set_display_mode(width, height, depth);
    }
    DEBUG_END();
}
