/*
 * framebuffer.h
 */

#pragma once

#include "../common/stdbool.h"
#include "../common/stdint.h"

#define PALETTE_COLORS 256
extern uint32_t palette[PALETTE_COLORS];

typedef struct
{
    uint8_t red: 8;
    uint8_t green: 8;
    uint8_t blue: 8;
    uint8_t alpha: 8;
} __attribute__ ((packed)) rgb_t;

typedef enum
{
    DEFAULT_PALETTE_MODE = 0,
    RGB_685_LEVELS,
    RGB_685_LEVELS_WITH_GREYSCALE,
    RGB_685_LEVELS_WITH_SPECTRUM_COLORS,
    RGB_332_BITS,
    LAST_PALETTE_MODE = RGB_332_BITS
} palette_mode_t;

typedef enum
{
    DEFAULT_RGB_MODE = 0,
    ARGB32,
    ABGR32,
    LAST_RGB_MODE = ABGR32
} rgb_mode_t;

typedef struct
{
    uint32_t physical_width;
    uint32_t physical_height;
    uint32_t virtual_width;
    uint32_t virtual_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t virtual_x_offset;
    uint32_t virtual_y_offset;
    void *address;
    uint32_t size;
} fb_init_t;

typedef struct framebuffer_info
{
    bool mode_discovered;
    bool channel_mode;
    bool rgb;
    palette_mode_t palette_mode;
    rgb_mode_t rgb_mode;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t bpp;
    uint32_t pitch;
    uint32_t char_width;
    uint32_t char_height;
    int32_t columns;
    int32_t rows;
    int32_t current_column; // Text cursor column position
    int32_t current_row; // Text cursor row position
    bool cursor_enabled; // True if software text cursor is enabled
    bool cursor_visible; // True if software text cursor is currently visible
    uint8_t cursor_top; // Should be between zero and (fbinfo.char_height - 1)
    uint8_t cursor_bottom; // Should be equal to or greater than cursor_row_start, and less than (fbinfo.char_height - 1)
    uint8_t cursor_left; // Should be between zero and (fbinfo.char_width - 1)
    uint8_t cursor_right; // Should be equal to or greater than cursor_row_start, and less than (fbinfo.char_width - 1)
    volatile uint8_t *buf;
    uint32_t buf_size;
} framebuffer_info_t;

extern framebuffer_info_t fbinfo;

void framebuffer_init();
int set_pixel_order(bool rgb);
int init_palette();
int set_display_dimensions(uint32_t width, uint32_t height, uint32_t depth);
