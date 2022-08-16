/*
 * graphics.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mem_internal.h"
#include "../../include/saa505x/glyphs.h"

static int background_color = DEFAULT_BACKGROUND_COLOR;
static int foreground_color = DEFAULT_FOREGROUND_COLOR;
uint32_t colors[PALETTE_COLORS];

void set_background_color(int color)
{
    background_color = color;
}

void set_foreground_color(int color)
{
    foreground_color = color;
}

void write_pixel(uint32_t x, uint32_t y, const uint32_t color)
{
    if (fbinfo.buf)
    {
        int location = y * fbinfo.pitch + x * fbinfo.bpp;
        fbinfo.buf[location++] = color & 0xff;
        if (fbinfo.bpp > 1) fbinfo.buf[location++] = (color >> 8) & 0xff;
        if (fbinfo.bpp > 2) fbinfo.buf[location++] = (color >> 16) & 0xff;
    }
}

static void write_char_at_cursor(int c)
{
    if (fbinfo.current_column < 0 || fbinfo.current_column >= fbinfo.columns ||
        fbinfo.current_row < 0 || fbinfo.current_row >= fbinfo.rows)
    {
        return;
    }
    const uint16_t *bitmap = get_saa505x_glyph(c);
    uint8_t w, h;
    uint16_t mask;
    for (w = 0; w < fbinfo.char_width; w++)
    {
        for (h = 0; h < fbinfo.char_height; h++)
        {
            mask = 1 << (fbinfo.char_width - w - 1);
            write_pixel(fbinfo.current_column * fbinfo.char_width + w,
                fbinfo.current_row * fbinfo.char_height + h,
                bitmap[h] & mask ? colors[foreground_color] : colors[background_color]);
        }
    }
}

static void move_cursor_backwards()
{
    fbinfo.current_column--;
    if (fbinfo.current_column < 0)
    {
        fbinfo.current_column = fbinfo.columns - 1;
        fbinfo.current_row--;
    }
}

static void move_cursor_forwards()
{
    fbinfo.current_column++;
    if (fbinfo.current_column == fbinfo.columns)
    {
        fbinfo.current_column = 0;
        fbinfo.current_row++;
    }
}

static void clear_framebuffer()
{
    switch(fbinfo.depth)
    {
        case 8:
            __memory_set_byte(fbinfo.buf, colors[background_color], fbinfo.buf_size);
            break;
        case 16:
            __memory_set_short(fbinfo.buf, colors[background_color], fbinfo.buf_size);
            break;
        case 24:
            __memory_set_int24(fbinfo.buf, colors[background_color], fbinfo.buf_size);
            break;
        case 32:
            __memory_set_int(fbinfo.buf, colors[background_color], fbinfo.buf_size);
    }
}

static void cursor_home()
{
    fbinfo.current_column = 0;
    fbinfo.current_row = 0;
}

static void scroll_down()
{
    uint32_t row_size = fbinfo.pitch * fbinfo.char_height;
    size_t all_but_one_rows = fbinfo.buf_size - row_size;
    memmove((void *)fbinfo.buf + row_size, (const void *)fbinfo.buf, all_but_one_rows);
    memset((void *)fbinfo.buf, 0, row_size);
}

static void scroll_up()
{
    uint32_t row_size = fbinfo.pitch * fbinfo.char_height;
    size_t all_but_one_rows = fbinfo.buf_size - row_size;
    memmove((void *)fbinfo.buf, (const void *)(fbinfo.buf + row_size), all_but_one_rows);
    memset((void *)(fbinfo.buf + all_but_one_rows), 0, row_size);
}

void graphics_putc(int c)
{
    int32_t num_rows = fbinfo.height / fbinfo.char_height;

    switch (c)
    {
        case 2:
            background_color++;
            background_color %= PALETTE_COLORS;
            debug_printf("bgcolor %d\r\n", background_color);
            break;
        case 6:
            foreground_color++;
            foreground_color %= PALETTE_COLORS;
            debug_printf("fgcolor %d\r\n", foreground_color);
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
            fbinfo.current_row++;
            break;
        case 11:
            fbinfo.current_row--;
            break;
        case 12:
            clear_framebuffer();
            cursor_home();
            break;
        case 13:
            fbinfo.current_column = 0;
            break;
        default:
            if ((c >= 32 && c < 127) || (c >= 160))
            {
                write_char_at_cursor(c);
                move_cursor_forwards();
            }
    }

    if (fbinfo.current_row < 0)
    {
        scroll_down();
        fbinfo.current_row++;
    }

    if (fbinfo.current_row == num_rows)
    {
        scroll_up();
        fbinfo.current_row--;
    }
}

static void init_colors(int depth)
{
    for (int i = 0; i < PALETTE_COLORS; i++)
    {
        if (depth == 8)
        {
            colors[i] = i;
        }
        else if (depth == 16)
        {
            colors[i] = ((palette[i] & 0xf80000) >> 19) | ((palette[i] & 0xfc00) >> 5) | ((palette[i] & 0xf8) << 8);
        }
        else
        {
            colors[i] = palette[i];
        }
    }
}

static void set_default_display_mode()
{
    debug_printf("Setting default display mode.\r\n");
    set_display_mode(DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_DEPTH);
}

void set_display_mode(int width, int height, int depth)
{
    debug_printf("Setting display mode to %dx%d, %d bpp.\r\n", width, height, depth);
//    if (bcm2835_set_display_dimensions(width, height, depth, GLYPH_WIDTH, GLYPH_HEIGHT) < 0)
    if (set_display_dimensions(width, height, depth, GLYPH_WIDTH, GLYPH_HEIGHT) < 0)
    {
        debug_printf("Setting default display mode instead.\r\n");
//        if (bcm2835_set_display_dimensions(DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_DEPTH, GLYPH_WIDTH, GLYPH_HEIGHT) < 0)
        if (set_display_dimensions(DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_DEPTH, GLYPH_WIDTH, GLYPH_HEIGHT) < 0)
        {
            debug_printf("Cannot set display.\r\n");
        }
    }
    init_colors(depth);
    clear_framebuffer();
    cursor_home();
}

void graphics_init()
{
    generate_saa505x_glyphs();
    framebuffer_init();
    init_palette();
    set_default_display_mode();
}
