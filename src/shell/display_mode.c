/*
 * debug.c
 */

#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/io.h"

DEBUG_INIT("display_mode");

static void mode_syntax()
{
    puts("\r\nSyntax: mode [<width> <height> <depth>]\r\n");
}

static void output_syntax()
{
    puts("\r\nSyntax: out [(fb|uart|all)]\r\n");
}

static void set_graphics_dma_syntax()
{
    puts("\r\nSyntax: gfxdma [(on|off)]\r\n");
}

static void show_display_mode()
{
    printf("Display mode: %dx%d, %d bpp.\r\n", fbinfo.width, fbinfo.height, fbinfo.depth);
    printf("Framebuffer size: %d kB.\r\n", fbinfo.buf_size / 1024);
    printf("Palette mode %d, RGB mode %d.\r\n", fbinfo.palette_mode, fbinfo.rgb_mode);
    printf("Character dimensions: %dx%d.\r\n", fbinfo.char_width, fbinfo.char_height);
    printf("Text resolution: %dx%d.\r\n", fbinfo.columns, fbinfo.rows);
    printf("Text cursor: current_row %d, current_column %d, enabled %d, visible %d, (%d,%d)->(%d,%d).\r\n",
           fbinfo.current_row, fbinfo.current_column, fbinfo.cursor_enabled, fbinfo.cursor_visible,
           fbinfo.cursor_left, fbinfo.cursor_top, fbinfo.cursor_right, fbinfo.cursor_bottom);
}

void display_mode(int argc, char **argv)
{
    DEBUG_START("display_mode");
    if (argc == 1)
    {
        show_display_mode();
        DEBUG_END();
        return;
    }
    if (argc < 4)
    {
        mode_syntax();
        DEBUG_END();
        return;
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int depth = atoi(argv[3]);
    if (width < 256 || width > 3840)
    {
        printf("\r\nInvalid width '%s'. Valid values are between 256 and 3840.\r\n", argv[1]);
    }
    else if (height < 192 || height > 2160)
    {
        printf("\r\nInvalid height '%s'. Valid values are between 192 and 2160.\r\n", argv[2]);
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
        printf("Output channel 'uart' is %s.\r\n", get_output_channel() & OUTPUT_CHANNEL_UART);
        printf("Output channel 'fb' is %s.\r\n", get_output_channel() & OUTPUT_CHANNEL_GRAPHICS);
    }
    else if (!strcmp("uart", argv[1]))
    {
        set_output_channel(OUTPUT_CHANNEL_UART);
    }
    else if (!strcmp("fb", argv[1]))
    {
        set_output_channel(OUTPUT_CHANNEL_GRAPHICS);
    }
    else if (!strcmp("all", argv[1]))
    {
        set_output_channel(OUTPUT_CHANNEL_ALL);
    }
    else
    {
        output_syntax();
    }
    DEBUG_END();
}

void set_graphics_dma(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Graphics DMA is %s.\r\n", is_dma_enabled() ? "enabled" : "disabled");
        return;
    }
    if (strcmp("on", argv[1]) && strcmp("off", argv[1]))
    {
        set_graphics_dma_syntax();
    }
    set_dma_enabled(!strcmp("on", argv[1]));
}
