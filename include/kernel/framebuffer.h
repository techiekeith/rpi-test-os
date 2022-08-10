/*
 * framebuffer.h
 */

#pragma once

#include <common/stdint.h>

#define PALETTE_COLORS 256
extern uint32_t palette[PALETTE_COLORS];

typedef struct framebuffer_info
{
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t bpp;
    uint32_t pitch;
    int32_t chars_width;
    int32_t chars_height;
    int32_t chars_x;
    int32_t chars_y;
    void *buf;
    uint32_t buf_size;
} framebuffer_info_t;

extern framebuffer_info_t fbinfo;

void framebuffer_init();
int framebuffer_set(int width, int height, int depth);
