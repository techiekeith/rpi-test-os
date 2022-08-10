/*
 * framebuffer.h
 */

#pragma once

#include "../common/stdint.h"

#define PALETTE_COLORS 256
extern uint32_t palette[PALETTE_COLORS];

typedef struct framebuffer_info
{
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t bpp;
    uint32_t pitch;
    uint32_t char_width;
    uint32_t char_height;
    int32_t columns;
    int32_t rows;
    int32_t current_column;
    int32_t current_row;
    volatile uint8_t *buf;
    uint32_t buf_size;
} framebuffer_info_t;

extern framebuffer_info_t fbinfo;

void framebuffer_init();
int init_palette();
int set_display_dimensions(int width, int height, int depth, int char_width, int char_height);
