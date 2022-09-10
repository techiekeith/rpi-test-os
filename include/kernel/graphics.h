/*
 * graphics.h
 */

#pragma once

#include "../common/stdbool.h"
#include "../common/stdint.h"
#include "charset.h"
#include "framebuffer.h"

#define DISPLAY_WIDTH               640
#define DISPLAY_HEIGHT              480
#define COLOR_DEPTH                 8
#define DEFAULT_BACKGROUND_COLOR    0
#define DEFAULT_FOREGROUND_COLOR    239
#define CURSOR_BLINK_INTERVAL       50

typedef uint8_t *(*get_glyph_8bit_f)(int);
typedef uint16_t *(*get_glyph_16bit_f)(int);

extern uint32_t colors[PALETTE_COLORS];

void set_background_color(int color);
void set_foreground_color(int color);
void set_8bit_charset(uint32_t width, uint32_t height, get_glyph_8bit_f get_glyph_function);
void set_16bit_charset(uint32_t width, uint32_t height, get_glyph_16bit_f get_glyph_function);
void write_pixel(uint32_t x, uint32_t y, const uint32_t color);
void graphics_putc(int c);
void set_border(int new_border_color, int new_border_size);
int get_border_size();
void set_charset(charset_t charset);
void set_display_mode(int width, int height, int depth);
void graphics_init();
