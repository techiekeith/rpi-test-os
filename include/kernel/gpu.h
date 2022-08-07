/*
 * gpu.h
 */

#pragma once

#include <common/stdint.h>

void gpu_init(void);

void write_pixel(uint32_t x, uint32_t y, const uint32_t color);

void gpu_putc(char c);
