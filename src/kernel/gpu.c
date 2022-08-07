/*
 * gpu.c
 */

#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>
#include <kernel/framebuffer.h>
#include <kernel/gpu.h>
#include <kernel/mailbox.h>
#include <kernel/mem.h>
#include <kernel/uart.h>
#include <saa5050/saa5050_glyphs.h>

int background_color = DEFAULT_BACKGROUND_COLOR;
int foreground_color = DEFAULT_FOREGROUND_COLOR;
uint32_t palette[PALETTE_COLORS];
uint32_t colors[PALETTE_COLORS];

void write_pixel(uint32_t x, uint32_t y, const uint32_t color)
{
    uint8_t *location = (uint8_t *)fbinfo.buf + y * fbinfo.pitch + x * BYTES_PER_PIXEL;
    memcpy(location, &color, BYTES_PER_PIXEL);
}

void write_char_at_cursor(char c)
{
    if (fbinfo.chars_x < 0 || fbinfo.chars_x >= fbinfo.chars_width ||
        fbinfo.chars_y < 0 || fbinfo.chars_y >= fbinfo.chars_height)
    {
        return;
    }
    const uint16_t *bitmap = get_saa5050_glyph(c, 0);
    uint8_t w, h;
    uint16_t mask;
    for (w = 0; w < GLYPH_WIDTH; w++)
    {
        for (h = 0; h < GLYPH_HEIGHT; h++)
        {
            mask = 1 << (GLYPH_WIDTH - w - 1);
            write_pixel(fbinfo.chars_x * GLYPH_WIDTH + w,
                fbinfo.chars_y * GLYPH_HEIGHT + h,
                bitmap[h] & mask ? colors[foreground_color] : colors[background_color]);
        }
    }
}

void move_cursor_backwards()
{
    fbinfo.chars_x--;
    if (fbinfo.chars_x < 0)
    {
        fbinfo.chars_x = fbinfo.chars_width - 1;
        fbinfo.chars_y--;
    }
}

void move_cursor_forwards()
{
    fbinfo.chars_x++;
    if (fbinfo.chars_x == fbinfo.chars_width)
    {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
    }
}

void clear_framebuffer()
{
    for (uint32_t y = 0; y < fbinfo.height; y++)
    {
        for (uint32_t x = 0; x < fbinfo.width; x++)
        {
            write_pixel(x, y, colors[background_color]);
        }
    }
}

void cursor_home()
{
    fbinfo.chars_x = 0;
    fbinfo.chars_y = 0;
}

void scroll_down()
{
    uint32_t row_size = fbinfo.pitch * GLYPH_HEIGHT;
    uint32_t all_but_one_rows = fbinfo.buf_size - row_size;
    memmove((uint8_t *)fbinfo.buf + row_size, (uint8_t *)fbinfo.buf, all_but_one_rows);
    memset((uint8_t *)fbinfo.buf, 0, row_size);
}

void scroll_up()
{
    uint32_t row_size = fbinfo.pitch * GLYPH_HEIGHT;
    uint32_t all_but_one_rows = fbinfo.buf_size - row_size;
    memmove((uint8_t *)fbinfo.buf, (uint8_t *)fbinfo.buf + row_size, all_but_one_rows);
    memset((uint8_t *)fbinfo.buf + all_but_one_rows, 0, row_size);
}

void gpu_putc(char c)
{
    int32_t num_rows = fbinfo.height / GLYPH_HEIGHT;

    switch (c)
    {
        case 2:
            background_color++;
            background_color %= PALETTE_COLORS;
            debug_printf("bgcolor %d\n", background_color);
            break;
        case 6:
            foreground_color++;
            foreground_color %= PALETTE_COLORS;
            debug_printf("fgcolor %d\n", foreground_color);
            break;
        case 8:
            move_cursor_backwards();
            break;
        case 127:
            move_cursor_backwards();
            write_char_at_cursor(32);
            break;
        case 9:
            move_cursor_forwards();
            break;
        case 10:
            fbinfo.chars_y++;
            break;
        case 11:
            fbinfo.chars_y--;
            break;
        case 12:
            clear_framebuffer();
            cursor_home();
            break;
        case 13:
            fbinfo.chars_x = 0;
            break;
        default:
            if (c >= 32 && c < 127)
            {
                write_char_at_cursor(c);
                move_cursor_forwards();
            }
    }

    if (fbinfo.chars_y < 0)
    {
        scroll_down();
        fbinfo.chars_y++;
    }

    if (fbinfo.chars_y == num_rows)
    {
        scroll_up();
        fbinfo.chars_y--;
    }
}

void init_palette()
{
    int red, green, blue, green_v, blue_v;
    for (int i = 0; i < PALETTE_COLORS; i++)
    {
        if (i < 240)
        {
            red = (i / 40) * 51;
            green_v = ((i % 40) / 5) * 255;
            blue_v = (i % 5) * 255;
            green = (green_v / 7) + ((green_v % 7) >> 2);
            blue = (blue_v / 4) + ((blue_v % 4) >> 1);
        }
        else
        {
            blue = i & 15;
            blue |= blue << 4;
            red = green = blue;
        }
        palette[i] = red | (green << 8) | (blue << 16) | 0xff000000;
#if (COLORDEPTH == 8)
        colors[i] = i;
#elif (COLORDEPTH == 16)
        colors[i] = ((palette[i] & 0xf80000) >> 19) | ((palette[i] & 0xfc00) >> 5) | ((palette[i] & 0xf8) << 8);
#elif (COLORDEPTH == 24 || COLORDEPTH == 32)
        colors[i] = palette[i];
#endif
    }
}

void show_palette()
{
    int count = 0;
    int h, w;
    int max_columns = 20;
    int max_rows = PALETTE_COLORS / max_columns + (PALETTE_COLORS % max_columns != 0);
    int normal_row_height = DISPLAY_HEIGHT / max_rows;
    h = normal_row_height;
    w = DISPLAY_WIDTH / max_columns;
    for (int row = 0; row < max_rows; row++)
    {
        if (PALETTE_COLORS - count < max_columns)
        {
            max_columns = PALETTE_COLORS - count;
            w = DISPLAY_WIDTH / max_columns;
            h = DISPLAY_HEIGHT - row * normal_row_height;
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

void gpu_init()
{
    generate_saa5050_glyphs();
    init_palette();
    framebuffer_init();
    clear_framebuffer();
    show_palette();
}
