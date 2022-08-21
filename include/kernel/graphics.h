/*
 * graphics.h
 */

#pragma once

#include "../common/stdbool.h"
#include "../common/stdint.h"
#include "framebuffer.h"

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define COLOR_DEPTH 8
#define DEFAULT_BACKGROUND_COLOR 0
#define DEFAULT_FOREGROUND_COLOR 239

extern uint32_t colors[PALETTE_COLORS];

void set_background_color(int color);
void set_foreground_color(int color);
void write_pixel(uint32_t x, uint32_t y, const uint32_t color);
void graphics_putc(int c);
void set_display_mode(int width, int height, int depth);
void graphics_init(bool channel_mode);
