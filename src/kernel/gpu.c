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
uint32_t palette[PALETTE_COLORS + 1];
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
            background_color = remainder(background_color, PALETTE_COLORS);
            debug_printf("bgcolor %d\n", background_color);
            break;
        case 6:
            foreground_color++;
            foreground_color = remainder(foreground_color, PALETTE_COLORS);
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
            fbinfo.chars_x = 0;
            fbinfo.chars_y = 0;
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

void init_palette(void)
{
    int red, green, blue;
    for (int i = 0; i < PALETTE_COLORS; i++)
    {
        if (i < 240)
        {
            div_t red_div = div(i, 40);
            div_t green_div = div(red_div.rem, 5);
            div_t blue_div = div(green_div.rem * 255, 4);
            green_div = div(green_div.quot * 255, 7);
            red = red_div.quot * 51;
            green = green_div.quot + (green_div.rem >> 2);
            blue = blue_div.quot + (blue_div.rem >> 1);
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

void show_palette(void)
{
    const int max_columns = 16;
    int count = 0;
    div_t rows_div = div(PALETTE_COLORS, max_columns);
    int max_rows = rows_div.quot + (rows_div.rem != 0);
    int w = quotient(DISPLAY_WIDTH, max_columns);
    int h = quotient(DISPLAY_HEIGHT, max_rows);
    for (int row = 0; row < max_rows; row++)
    {
        for (int column = 0; column < max_columns; column++)
        {
            if (count < PALETTE_COLORS)
            {
                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        write_pixel(x + column * w, y + row * h, colors[count]);
                    }
                }
                count++;
            }
        }
    }
}

void gpu_init(void)
{
    generate_saa5050_glyphs();
    init_palette();
    framebuffer_init();
    clear_framebuffer();
    show_palette();
}
