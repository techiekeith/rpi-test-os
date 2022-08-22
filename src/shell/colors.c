/*
 * colors.c
 */

#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/graphics.h"

void background_color(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Syntax: bgcolor <#>\r\n");
        return;
    }
    int color = atoi(argv[1]);
    set_background_color(color);
}

void foreground_color(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Syntax: bgcolor <#>\r\n");
        return;
    }
    int color = atoi(argv[1]);
    set_foreground_color(color);
}

int set_pixels(int argc, char **argv)
{
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "rgb") && strcmp(argv[1], "bgr")))
    {
        printf("\r\nSyntax: pixels (rgb|bgr)\r\n");
        return;
    }
    if (argc == 2)
    {
        set_pixel_order(strcmp(argv[1], "bgr"));
    }
    printf("\r\nPixel order: %s.\r\n", fbinfo.rgb ? "RGB" : "BGR");
}
int set_palette_mode(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("\r\nSyntax: palette (<palette-mode> <rgb-mode>)\r\n");
        return;
    }
    int palette_mode = atoi(argv[1]);
    int rgb_mode = atoi(argv[2]);
    if (palette_mode < DEFAULT_PALETTE_MODE || palette_mode > LAST_PALETTE_MODE) palette_mode = DEFAULT_PALETTE_MODE;
    if (rgb_mode < DEFAULT_RGB_MODE || rgb_mode > LAST_RGB_MODE) rgb_mode = DEFAULT_RGB_MODE;
    fbinfo.palette_mode = palette_mode;
    fbinfo.rgb_mode = rgb_mode;
    init_palette();
    return 0;
}

void show_palette(int argc, char **argv)
{
    if (argc > 1)
    {
        int rv = set_palette_mode(argc, argv);
        if (rv < 0) return;
    }
    putc(12);
    int count = 0;
    int h, w;
    int max_columns = 20;
    int max_rows = PALETTE_COLORS / max_columns + (PALETTE_COLORS % max_columns != 0);
    int normal_row_height = fbinfo.height / max_rows;
    h = normal_row_height;
    w = fbinfo.width / max_columns;
    for (int row = 0; row < max_rows; row++)
    {
        if (PALETTE_COLORS - count < max_columns)
        {
            max_columns = PALETTE_COLORS - count;
            w = fbinfo.width / max_columns;
            h = fbinfo.height - row * normal_row_height;
        }
        for (int column = 0; column < max_columns; column++)
        {
            if (count < PALETTE_COLORS)
            {
                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        write_pixel(x + column * w, y + row * normal_row_height, colors[count]);
                    }
                }
                count++;
            }
        }
    }
}

