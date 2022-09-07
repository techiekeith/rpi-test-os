/*
 * graphics.c
 */

#include "../../include/common/stdbool.h"
#include "../../include/common/stdint.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mem_internal.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/saa505x/glyphs.h"
#include "../../include/raster/glyphs_8bit.h"

static int background_color = DEFAULT_BACKGROUND_COLOR;
static int foreground_color = DEFAULT_FOREGROUND_COLOR;
static get_glyph_8bit_f get_glyph_8bit;
static get_glyph_16bit_f get_glyph_16bit;
static bool initialized;
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
    if (!fbinfo.buf) return;
    uint32_t location = y * fbinfo.pitch + x * fbinfo.bpp;
    if (location >= fbinfo.buf_size) return;
    uint32_t real_color = color;
    if (fbinfo.cursor_enabled && fbinfo.cursor_visible &&
        x >= fbinfo.current_column * fbinfo.char_width + fbinfo.cursor_left &&
        x <= fbinfo.current_column * fbinfo.char_width + fbinfo.cursor_right &&
        y >= fbinfo.current_row * fbinfo.char_height + fbinfo.cursor_top &&
        y <= fbinfo.current_row * fbinfo.char_height + fbinfo.cursor_bottom)
    {
        real_color = ~real_color;
    }
    fbinfo.buf[location++] = real_color & 0xff;
    if (fbinfo.bpp > 1) fbinfo.buf[location++] = (real_color >> 8) & 0xff;
    if (fbinfo.bpp > 2) fbinfo.buf[location++] = (real_color >> 16) & 0xff;
    if (fbinfo.bpp > 3) fbinfo.buf[location++] = (real_color >> 24) & 0xff;
}

static void invert_pixel(uint32_t x, uint32_t y)
{
    uint32_t location = y * fbinfo.pitch + x * fbinfo.bpp;
    fbinfo.buf[location] = ~fbinfo.buf[location];
    if (fbinfo.bpp > 1) fbinfo.buf[location + 1] = ~fbinfo.buf[location + 1];
    if (fbinfo.bpp > 2) fbinfo.buf[location + 2] = ~fbinfo.buf[location + 2];
    if (fbinfo.bpp > 3) fbinfo.buf[location + 3] = ~fbinfo.buf[location + 3];
}

static void toggle_cursor()
{
    if (!fbinfo.buf ||
        fbinfo.current_column < 0 || fbinfo.current_column >= fbinfo.columns ||
        fbinfo.current_row < 0 || fbinfo.current_row >= fbinfo.rows)
    {
        return;
    }
    int top = fbinfo.current_row * fbinfo.char_height;
    int left = fbinfo.current_column * fbinfo.char_width;
    for (int h = fbinfo.cursor_top; h <= fbinfo.cursor_bottom; h++)
    {
        for (int w = fbinfo.cursor_left; w <= fbinfo.cursor_right; w++)
        {
            invert_pixel(left + w, top + h);
        }
    }
}

void enable_cursor()
{
    if (fbinfo.cursor_enabled) return;
    if (fbinfo.cursor_visible) toggle_cursor();
    fbinfo.cursor_enabled = true;
}

bool disable_cursor()
{
    DISABLE_INTERRUPTS();
    bool enabled = fbinfo.cursor_enabled;
    fbinfo.cursor_enabled = false;
    ENABLE_INTERRUPTS();
    if (enabled && fbinfo.cursor_visible) toggle_cursor();
    return enabled;
}

void blink_cursor()
{
    fbinfo.cursor_visible = !fbinfo.cursor_visible;
    if (fbinfo.cursor_enabled) toggle_cursor();
}

static void write_char_at_cursor_8bit(int c)
{
    if (fbinfo.current_column < 0 || fbinfo.current_column >= fbinfo.columns ||
        fbinfo.current_row < 0 || fbinfo.current_row >= fbinfo.rows)
    {
        return;
    }
    const uint8_t *bitmap = get_glyph_8bit(c);
    for (int h = fbinfo.char_height - 1; h >= 0; h--)
    {
        uint8_t mask = 1;
        for (int w = fbinfo.char_width - 1; w >= 0; w--)
        {
            write_pixel(fbinfo.current_column * fbinfo.char_width + w,
                        fbinfo.current_row * fbinfo.char_height + h,
                        bitmap[h] & mask ? colors[foreground_color] : colors[background_color]);
            mask <<= 1;
        }
    }
}

static void write_char_at_cursor_16bit(int c)
{
    if (fbinfo.current_column < 0 || fbinfo.current_column >= fbinfo.columns ||
        fbinfo.current_row < 0 || fbinfo.current_row >= fbinfo.rows)
    {
        return;
    }
    const uint16_t *bitmap = get_glyph_16bit(c);
    for (int h = fbinfo.char_height - 1; h >= 0; h--)
    {
        uint16_t mask = 1;
        for (int w = fbinfo.char_width - 1; w >= 0; w--)
        {
            write_pixel(fbinfo.current_column * fbinfo.char_width + w,
                fbinfo.current_row * fbinfo.char_height + h,
                bitmap[h] & mask ? colors[foreground_color] : colors[background_color]);
            mask <<= 1;
        }
    }
}

static void write_char_at_cursor(int c)
{
    fbinfo.char_width <= 8 ? write_char_at_cursor_8bit(c) : write_char_at_cursor_16bit(c);
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

static void clear_framebuffer_area(void *buffer, size_t size)
{
    uint32_t color = colors[background_color];
    switch(fbinfo.depth)
    {
        case 8:
            __memory_set_byte(buffer, color, size);
            break;
        case 16:
            __memory_set_short(buffer, color, size);
            break;
        case 24:
            __memory_set_int24(buffer, color, size);
            break;
        case 32:
            __memory_set_int(buffer, color, size);
    }
}

static void clear_framebuffer()
{
    clear_framebuffer_area((void *)fbinfo.buf, fbinfo.buf_size);
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
    clear_framebuffer_area((void *)fbinfo.buf, row_size);
}

static void scroll_up()
{
    uint32_t row_size = fbinfo.pitch * fbinfo.char_height;
    size_t all_but_one_rows = fbinfo.buf_size - row_size;
    memmove((void *)fbinfo.buf, (const void *)(fbinfo.buf + row_size), all_but_one_rows);
    clear_framebuffer_area((void *)(fbinfo.buf + all_but_one_rows), row_size);
}

void graphics_putc(int c)
{
    if (!initialized) return;
    bool cursor_enabled = disable_cursor();

    int32_t num_rows = fbinfo.height / fbinfo.char_height;

    switch (c)
    {
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

    if (cursor_enabled) enable_cursor();
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
            // This may need revisiting
            colors[i] = fbinfo.channel_mode ?
                    (palette[i] & 0xff00ff00) | ((palette[i] & 0xff0000) >> 16) | ((palette[i] & 0xff) << 16)
                    : palette[i];
        }
    }
}

static void set_default_display_mode()
{
    debug_printf("Setting default display mode.\r\n");
    set_display_mode(DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_DEPTH);
}

static void set_charset(uint32_t width, uint32_t height)
{
    fbinfo.char_width = width;
    fbinfo.char_height = height;
    fbinfo.columns = fbinfo.width / width;
    fbinfo.rows = fbinfo.height / height;
    fbinfo.current_column = 0;
    fbinfo.current_row = 0;
}

static void set_cursor_rectangle(uint8_t top, uint8_t right, uint8_t bottom, uint8_t left)
{
    fbinfo.cursor_top = top;
    fbinfo.cursor_right = right;
    fbinfo.cursor_bottom = bottom;
    fbinfo.cursor_left = left;
}

void set_8bit_charset(uint32_t width, uint32_t height, get_glyph_8bit_f get_glyph_function)
{
    bool cursor_enabled = disable_cursor();
    get_glyph_8bit = get_glyph_function;
    set_charset(width, height);
    set_cursor_rectangle(0, width - 1, height - 1, 0);
    if (cursor_enabled) enable_cursor();
}

void set_16bit_charset(uint32_t width, uint32_t height, get_glyph_16bit_f get_glyph_function)
{
    bool cursor_enabled = disable_cursor();
    get_glyph_16bit = get_glyph_function;
    set_charset(width, height);
    set_cursor_rectangle(0, width - 1, height - 1, 0);
    if (cursor_enabled) enable_cursor();
}

void set_display_mode(int width, int height, int depth)
{
    if (!initialized)
    {
        debug_printf("You must initialize graphics before setting the display mode.\r\n");
        return;
    }

    bool cursor_enabled = disable_cursor();

    debug_printf("Setting display mode to %dx%d, %d bpp.\r\n", width, height, depth);
    if (set_display_dimensions(width, height, depth) < 0)
    {
        debug_printf("Setting default display mode instead.\r\n");
        if (set_display_dimensions(DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_DEPTH) < 0)
        {
            debug_printf("Cannot set display.\r\n");
        }
    }
    set_8bit_charset(RASTER_8BIT_GLYPH_WIDTH, RASTER_8BIT_GLYPH_HEIGHT, get_bbc_micro_glyph);
//    set_16bit_charset(SAA505X_GLYPH_WIDTH, SAA505X_GLYPH_HEIGHT, get_saa505x_glyph);
    if (fbinfo.channel_mode && depth == 8)
    {
        init_palette();
    }
    init_colors(depth);
    clear_framebuffer();
    cursor_home();

    if (cursor_enabled) enable_cursor();
}

void graphics_init()
{
    if (initialized)
    {
        debug_printf("Graphics is already initialized.\r\n");
        return;
    }
    generate_saa505x_glyphs();
    framebuffer_init();
    set_pixel_order(false);
    init_palette();
    initialized = true;
    set_default_display_mode();
    register_interval_handler("blink_cursor", blink_cursor, CURSOR_BLINK_INTERVAL, true);
    enable_cursor();
}
