#include <common/stdlib.h>
#include <kernel/framebuffer.h>
#include <kernel/gpu.h>
#include <kernel/mem.h>
#include <kernel/mailbox.h>


static const pixel_t WHITE = {0xff, 0xff, 0xff};
static const pixel_t BLACK = {0x00, 0x00, 0x00};

void write_pixel(uint32_t x, uint32_t y, const pixel_t *pix)
{
    uint8_t *location = (uint8_t *)fbinfo.buf + y * fbinfo.pitch + x * BYTES_PER_PIXEL;
    memcpy(location, pix, BYTES_PER_PIXEL);
}

void gpu_putc(char c)
{
    uint8_t w, h;
    uint16_t mask;
    const uint16_t *bitmap = get_saa5050_glyph(c, 0);
    uint32_t i, num_rows = fbinfo.height / GLYPH_HEIGHT;

    // shift everything up one row
    if (fbinfo.chars_y >= num_rows)
    {
        // Copy a whole character row into the one above it
        for (i = 0; i < num_rows - 1; i++)
        {
            memcpy((uint8_t *)fbinfo.buf + fbinfo.pitch * i * GLYPH_HEIGHT, (uint8_t *)fbinfo.buf + fbinfo.pitch * (i + 1) * GLYPH_HEIGHT, fbinfo.pitch * GLYPH_HEIGHT);
        }
        // zero out the last row
        bzero((uint8_t *)fbinfo.buf + fbinfo.pitch * i * GLYPH_HEIGHT, fbinfo.pitch * GLYPH_HEIGHT);
        fbinfo.chars_y--;
    }

    if (c == '\n')
    {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
        return;
    }

    for (w = 0; w < GLYPH_WIDTH; w++)
    {
        for (h = 0; h < GLYPH_HEIGHT; h++)
        {
            mask = 1 << (GLYPH_WIDTH - w - 1);
            write_pixel(fbinfo.chars_x * GLYPH_WIDTH + w, fbinfo.chars_y * GLYPH_HEIGHT + h, bitmap[h] & mask ? &WHITE : &BLACK);
        }
    }

    fbinfo.chars_x++;
    if (fbinfo.chars_x > fbinfo.chars_width)
    {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
    }
}

void gpu_init(void)
{
    generate_saa5050_glyphs();

    // Aparantly, this sometimes does not work, so try in a loop
    while(framebuffer_init());

    // clear screen
    for (uint32_t j = 0; j < fbinfo.height; j++)
    {
        for (uint32_t i = 0; i < fbinfo.width; i++)
        {
            write_pixel(i, j, &BLACK);
        }
    }
}
