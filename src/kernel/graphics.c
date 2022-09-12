/*
 * graphics.c
 */

#include "../../include/common/stdbool.h"
#include "../../include/common/stdint.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/charset.h"
#include "../../include/kernel/dma.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/graphics.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mem_internal.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/raster/glyphs_8bit.h"
#include "../../include/saa505x/glyphs.h"

static int background_color = DEFAULT_BACKGROUND_COLOR;
static int foreground_color = DEFAULT_FOREGROUND_COLOR;
static int border_color = DEFAULT_BACKGROUND_COLOR;
static int border_size = 0;
static get_glyph_8bit_f get_glyph_8bit;
static get_glyph_16bit_f get_glyph_16bit;
static bool initialized;
uint32_t colors[PALETTE_COLORS];

static void write_pixel(uint32_t x, uint32_t y, const uint32_t color)
{
    uint32_t location = (y + border_size) * fbinfo.pitch + (x + border_size) * fbinfo.bpp;
    uint16_t *p16;
    uint32_t *p32;
    switch (fbinfo.bpp)
    {
        case 1:
            fbinfo.buf[location] = color;
            break;
        case 2:
            p16 = (uint16_t *)(fbinfo.buf + location);
            *p16 = color;
            break;
        case 3:
            fbinfo.buf[location++] = color & 0xff;
            fbinfo.buf[location++] = (color >> 8) & 0xff;
            fbinfo.buf[location] = (color >> 16);
            break;
        case 4:
            p32 = (uint32_t *)(fbinfo.buf + location);
            *p32 = color;
            break;
    }
}

static void invert_pixel(uint32_t x, uint32_t y)
{
    uint32_t location = (y + border_size) * fbinfo.pitch + (x + border_size) * fbinfo.bpp;
    uint16_t *p16;
    uint32_t *p32;
    switch (fbinfo.bpp)
    {
        case 1:
            if (fbinfo.buf[location] >= 240)
            {
                fbinfo.buf[location] = fbinfo.buf[location] ^ 15;
            }
            else
            {
                fbinfo.buf[location] = 239 - fbinfo.buf[location];
            }
            break;
        case 2:
            p16 = (uint16_t *)(fbinfo.buf + location);
            *p16 = ~*p16;
            break;
        case 3:
            fbinfo.buf[location] = ~fbinfo.buf[location];
            fbinfo.buf[location + 1] = ~fbinfo.buf[location + 1];
            fbinfo.buf[location + 2] = ~fbinfo.buf[location + 2];
            break;
        case 4:
            p32 = (uint32_t *)(fbinfo.buf + location);
            *p32 = ~*p32;
            break;
    }
}

static void toggle_cursor()
{
    int top = fbinfo.current_row * fbinfo.char_height;
    int left = fbinfo.current_column * fbinfo.char_width;
    int bottom = top + fbinfo.cursor_bottom;
    int right = left + fbinfo.cursor_right;
    top += fbinfo.cursor_top;
    left += fbinfo.cursor_left;
    for (int h = top; h <= bottom; h++)
    {
        for (int w = left; w <= right; w++)
        {
            invert_pixel(w, h);
        }
    }
}

void set_background_color(int color)
{
    background_color = color;
}

void set_foreground_color(int color)
{
    foreground_color = color;
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

static void blink_cursor(int handle, void **args)
{
    // Consume unused varargs
    (void) handle;
    (void) args;

    // Blink the cursor
    fbinfo.cursor_visible = !fbinfo.cursor_visible;
    if (fbinfo.cursor_enabled) toggle_cursor();
}

static void write_char_8bit(int c)
{
    int x = fbinfo.current_column * fbinfo.char_width;
    int y = fbinfo.current_row * fbinfo.char_height;
    int fgcolor = colors[foreground_color];
    int bgcolor = colors[background_color];
    const uint8_t *bitmap = get_glyph_8bit(c);
    for (int h = fbinfo.char_height - 1; h >= 0; h--)
    {
        uint8_t bits = bitmap[h];
        for (int w = fbinfo.char_width - 1; w >= 0; w--)
        {
            write_pixel(x + w, y + h, bits & 1 ? fgcolor : bgcolor);
            bits >>= 1;
        }
    }
}

static void write_char_16bit(int c)
{
    int x = fbinfo.current_column * fbinfo.char_width;
    int y = fbinfo.current_row * fbinfo.char_height;
    int fgcolor = colors[foreground_color];
    int bgcolor = colors[background_color];
    const uint16_t *bitmap = get_glyph_16bit(c);
    for (int h = fbinfo.char_height - 1; h >= 0; h--)
    {
        uint16_t bits = bitmap[h];
        for (int w = fbinfo.char_width - 1; w >= 0; w--)
        {
            write_pixel(x + w, y + h, bits & 1 ? fgcolor : bgcolor);
            bits >>= 1;
        }
    }
}

static void (*write_char_at_cursor)(int);

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

void clear_framebuffer_area(void *buffer, uint32_t color, size_t size)
{
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

static void clear_border()
{
    if (border_size <= 0) return;
    size_t border_height_bytes = fbinfo.pitch * border_size;
    size_t border_width_bytes = fbinfo.bpp * border_size;
    void *buffer = (void *) fbinfo.buf;
    uint32_t color = colors[border_color];
    clear_framebuffer_area(buffer, color, border_height_bytes);
    buffer += border_height_bytes;
    for (uint32_t i = border_size; i < fbinfo.height - border_size; i++)
    {
        clear_framebuffer_area(buffer, color, border_width_bytes);
        clear_framebuffer_area(buffer + fbinfo.pitch - border_width_bytes, color, border_width_bytes);
        buffer += fbinfo.pitch;
    }
    clear_framebuffer_area(buffer, color, border_height_bytes);
}

static void clear_framebuffer()
{
    clear_framebuffer_area((void *)fbinfo.buf, colors[background_color], fbinfo.buf_size);
    clear_border();
}

static void cursor_home()
{
    fbinfo.current_column = 0;
    fbinfo.current_row = 0;
}

static void move_or_copy_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, int dx, int dy, bool copy)
{
    if ((!dx && !dy) || !w || !h) return;
    int src_start_x = x;
    int src_start_y = y;
    int dst_start_x = x + dx;
    int dst_start_y = y + dy;
    uint32_t rect_x = w;
    uint32_t rect_y = h;

    if (dst_start_x < 0)
    {
        rect_x += dst_start_x;
        src_start_x -= dst_start_x;
        dst_start_x = 0;
    }
    if (dst_start_y < 0)
    {
        rect_y += dst_start_y;
        src_start_y -= dst_start_y;
        dst_start_y = 0;
    }
    if (src_start_x + rect_x > fbinfo.width)
    {
        rect_x = fbinfo.width - src_start_x;
    }
    if (src_start_y + rect_y > fbinfo.height)
    {
        rect_y = fbinfo.height - src_start_y;
    }
    if (dst_start_x + rect_x > fbinfo.width)
    {
        rect_x = fbinfo.width - dst_start_x;
    }
    if (dst_start_y + rect_y > fbinfo.height)
    {
        rect_y = fbinfo.height - dst_start_y;
    }

    int row_size = fbinfo.bpp * rect_x;
    int blank_size = fbinfo.bpp * dx;
    void *src = (void *)fbinfo.buf + fbinfo.pitch * src_start_y + fbinfo.bpp * src_start_x;
    void *dst = (void *)fbinfo.buf + fbinfo.pitch * dst_start_y + fbinfo.bpp * dst_start_x;
    int increment = fbinfo.pitch;
    if (dy > 0 || (dy == 0 && dx > 0))
    {
        src += fbinfo.pitch * (rect_y - 1);
        dst += fbinfo.pitch * (rect_y - 1);
        increment = -increment;
    }
    dma_copy_multiple(dst, src, row_size, rect_y, increment);

    if (!copy)
    {
        if (dx != 0)
        {
            for (int i = rect_y; i; i--)
            {
                clear_framebuffer_area(src + ((dx > 0) ? 0 : (rect_x - abs(dx))), colors[background_color], blank_size);
            }
        }
        if (dy != 0)
        {
            void *blank = (void *)fbinfo.buf + fbinfo.pitch * src_start_y + fbinfo.bpp * src_start_x;
            if (dy < 0)
            {
                blank += fbinfo.pitch * (rect_y - abs(dy));
            }
            for (int i = abs(dy); i; i--)
            {
                clear_framebuffer_area(blank, colors[background_color], row_size);
                blank += fbinfo.pitch;
            }
        }
    }
}

static void scroll_down()
{
    if (!border_size) // Faster move for borderless display mode
    {
        uint32_t row_size = fbinfo.pitch * fbinfo.char_height;
        size_t all_but_one_rows = fbinfo.buf_size - row_size;
        dma_copy((void *)fbinfo.buf + row_size, (const void *)fbinfo.buf, all_but_one_rows);
        clear_framebuffer_area((void *)fbinfo.buf, colors[background_color], row_size);
        return;
    }
    move_or_copy_rect(border_size,
                      border_size,
                      fbinfo.width - (border_size * 2),
                      fbinfo.height - (border_size * 2 + fbinfo.char_height),
                      0,
                      fbinfo.char_height,
                      false);
}

static void scroll_up()
{
    if (!border_size) // Faster move for borderless display mode
    {
        uint32_t row_size = fbinfo.pitch * fbinfo.char_height;
        size_t all_but_one_rows = fbinfo.buf_size - row_size;
        dma_copy((void *)fbinfo.buf, (const void *)(fbinfo.buf + row_size), all_but_one_rows);
        clear_framebuffer_area((void *)(fbinfo.buf + all_but_one_rows), colors[background_color], row_size);
        return;
    }
    move_or_copy_rect(border_size,
                      border_size + fbinfo.char_height,
                      fbinfo.width - (border_size * 2),
                      fbinfo.height - (border_size * 2 + fbinfo.char_height),
                      0,
                      -fbinfo.char_height,
                      false);
}

void graphics_putc(int c)
{
    if (!initialized) return;
    bool cursor_enabled = disable_cursor();

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

    if (fbinfo.current_row == fbinfo.rows)
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

static void set_text_dimensions()
{
    fbinfo.columns = (fbinfo.width - border_size * 2) / fbinfo.char_width;
    fbinfo.rows = (fbinfo.height - border_size * 2) / fbinfo.char_height;
}

static void set_cursor_rectangle(uint8_t top, uint8_t right, uint8_t bottom, uint8_t left)
{
    fbinfo.cursor_top = top;
    fbinfo.cursor_right = right;
    fbinfo.cursor_bottom = bottom;
    fbinfo.cursor_left = left;
}

void set_border(int new_border_color, int new_border_size)
{
    border_color = new_border_color;
    border_size = new_border_size;
    clear_border();
    set_text_dimensions();
}

int get_border_size()
{
    return border_size > 0 ? border_size : 0;
}

static void set_8bit_charset(uint32_t width, uint32_t height, get_glyph_8bit_f get_glyph_function)
{
    bool cursor_enabled = disable_cursor();
    write_char_at_cursor = write_char_8bit;
    get_glyph_8bit = get_glyph_function;
    fbinfo.char_width = width;
    fbinfo.char_height = height;
    set_cursor_rectangle(0, width - 1, height - 1, 0);
    if (cursor_enabled) enable_cursor();
}

static void set_16bit_charset(uint32_t width, uint32_t height, get_glyph_16bit_f get_glyph_function)
{
    bool cursor_enabled = disable_cursor();
    write_char_at_cursor = write_char_16bit;
    get_glyph_16bit = get_glyph_function;
    fbinfo.char_width = width;
    fbinfo.char_height = height;
    set_cursor_rectangle(0, width - 1, height - 1, 0);
    if (cursor_enabled) enable_cursor();
}

void set_charset(charset_t charset)
{
    switch (charset)
    {
        case SAA505X_CHARSET:
            set_16bit_charset(SAA505X_GLYPH_WIDTH, SAA505X_GLYPH_HEIGHT, get_saa505x_glyph);
            break;
        case ACORN_CHARSET:
            set_8bit_charset(RASTER_8BIT_GLYPH_WIDTH, RASTER_8BIT_GLYPH_HEIGHT, get_acorn_glyph);
            break;
        case COMMODORE_PET_CHARSET:
            set_8bit_charset(RASTER_8BIT_GLYPH_WIDTH, RASTER_8BIT_GLYPH_HEIGHT, get_commodore_pet_glyph);
            break;
        case COMMODORE_64_CHARSET:
            set_8bit_charset(RASTER_8BIT_GLYPH_WIDTH, RASTER_8BIT_GLYPH_HEIGHT, get_commodore_64_glyph);
            break;
        case SINCLAIR_CHARSET:
            set_8bit_charset(RASTER_8BIT_GLYPH_WIDTH, RASTER_8BIT_GLYPH_HEIGHT, get_sinclair_glyph);
            break;
    }
    set_text_dimensions();
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
    set_text_dimensions();
    if (depth == 8)
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
    set_charset(SAA505X_CHARSET);
    set_default_display_mode();
    register_interval_handler("blink_cursor", CURSOR_BLINK_INTERVAL, true, blink_cursor, 0, NULL);
    enable_cursor();
}
