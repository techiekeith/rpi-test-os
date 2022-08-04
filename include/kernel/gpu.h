/*
 * gpu.h
 */

#pragma once

#include <common/stdint.h>
#include <kernel/peripheral.h>
#include <saa5050/saa5050_glyphs.h>

typedef struct pixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

void gpu_init(void);

void write_pixel(uint32_t x, uint32_t y, const pixel_t *pixel);

void gpu_putc(char c);
