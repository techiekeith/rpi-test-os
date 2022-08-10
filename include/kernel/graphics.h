/*
 * graphics.h
 */

#pragma once

#include <common/stdint.h>

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define COLOR_DEPTH 8
#define DEFAULT_BACKGROUND_COLOR 0
#define DEFAULT_FOREGROUND_COLOR 239

void write_pixel(uint32_t x, uint32_t y, const uint32_t color);

void graphics_putc(int c);

void clear_framebuffer();
void cursor_home();
void show_palette();

void set_display_mode(int width, int height, int depth);
void set_default_display_mode();
void graphics_init();
