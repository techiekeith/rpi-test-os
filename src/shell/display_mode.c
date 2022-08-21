/*
 * debug.c
 */

#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/io.h"

DEBUG_INIT("display_mode");

static void gfx_syntax()
{
    puts("\r\nSyntax: gfx <fb|prop>\r\n");
}

static void mode_syntax()
{
    puts("\r\nSyntax: mode <width> <height> <depth>\r\n");
}

static void output_syntax()
{
    puts("\r\nSyntax: out <fb|uart>\r\n");
}

void init_graphics(int argc, char **argv)
{
    DEBUG_START("init_graphics");
    if (argc < 2)
    {
        gfx_syntax();
    }
    else if (!strcmp(argv[1], "fb"))
    {
        graphics_init(true);
    }
    else if (!strcmp(argv[1], "prop"))
    {
        graphics_init(false);
    }
    else
    {
        gfx_syntax();
    }
    DEBUG_END();
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

void set_output(int argc, char **argv)
{
    DEBUG_START("set_output");
    if (argc < 2)
    {
        output_syntax();
    }
    else if (!strcmp("oldfb", argv[1]))
    {
        set_output_channel(OUTPUT_CHANNEL_GRAPHICS);
    }
    else if (!strcmp("fb", argv[1]))
    {
        set_output_channel(OUTPUT_CHANNEL_GRAPHICS);
    }
    else if (!strcmp("uart", argv[1]))
    {
        set_output_channel(OUTPUT_CHANNEL_UART);
    }
    else
    {
        output_syntax();
    }
    DEBUG_END();
}