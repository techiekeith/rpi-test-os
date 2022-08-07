/*
 * gpu.h
 */

#pragma once

#include <common/stdint.h>

void gpu_init();

void write_pixel(uint32_t x, uint32_t y, const uint32_t color);

void gpu_putc(char c);

void clear_framebuffer();
void cursor_home();
void show_palette();
