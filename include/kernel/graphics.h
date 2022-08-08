/*
 * graphics.h
 */

#pragma once

#include <common/stdint.h>

void graphics_init();

void write_pixel(uint32_t x, uint32_t y, const uint32_t color);

void graphics_putc(int c);

void clear_framebuffer();
void cursor_home();
void show_palette();
