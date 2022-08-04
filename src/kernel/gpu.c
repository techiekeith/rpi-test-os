#include <common/stdlib.h>
#include <kernel/framebuffer.h>
#include <kernel/gpu.h>
#include <kernel/mem.h>
#include <kernel/mailbox.h>
#include <kernel/uart.h>


static const pixel_t WHITE = {0xff, 0xff, 0xff};
static const pixel_t BLACK = {0x00, 0x00, 0x00};

void write_pixel(uint32_t x, uint32_t y, const pixel_t *pix)
{
    uint8_t *location = (uint8_t *)fbinfo.buf + y * fbinfo.pitch + x * BYTES_PER_PIXEL;
    memcpy(location, pix, BYTES_PER_PIXEL);
}

void write_char_at_cursor(char c)
{
    if (fbinfo.chars_x < 0 || fbinfo.chars_x >= fbinfo.chars_width ||
        fbinfo.chars_y < 0 || fbinfo.chars_y >= fbinfo.chars_height)
    {
        return;
    }
    const uint16_t *bitmap = get_saa5050_glyph(c, 0);
    uint8_t w, h;
    uint16_t mask;
    for (w = 0; w < GLYPH_WIDTH; w++)
    {
        for (h = 0; h < GLYPH_HEIGHT; h++)
        {
            mask = 1 << (GLYPH_WIDTH - w - 1);
            write_pixel(fbinfo.chars_x * GLYPH_WIDTH + w, fbinfo.chars_y * GLYPH_HEIGHT + h, bitmap[h] & mask ? &WHITE : &BLACK);
        }
    }
}

void move_cursor_backwards()
{
    fbinfo.chars_x--;
    if (fbinfo.chars_x < 0)
    {
        fbinfo.chars_x = fbinfo.chars_width - 1;
        fbinfo.chars_y--;
    }
}

void move_cursor_forwards()
{
    fbinfo.chars_x++;
    if (fbinfo.chars_x == fbinfo.chars_width)
    {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
    }
}

void gpu_putc(char c)
{
    int32_t num_rows = fbinfo.height / GLYPH_HEIGHT;

    switch (c)
    {
        case 8:
            move_cursor_backwards();
            break;
        case 127:
            move_cursor_backwards();
            write_char_at_cursor(32);
            break;
        case 9:
            move_cursor_forwards();
            break;
        case 10:
            fbinfo.chars_y++;
            break;
        case 11:
            fbinfo.chars_y--;
            break;
        case 12:
            bzero((uint8_t *)fbinfo.buf, fbinfo.buf_size);
            fbinfo.chars_x = 0;
            fbinfo.chars_y = 0;
            break;
        case 13:
            fbinfo.chars_x = 0;
            break;
        default:
            if (c >= 32 && c < 127)
            {
                write_char_at_cursor(c);
                move_cursor_forwards();
            }
            else
            {
                uart_puts(itoa(c, 10));
                uart_putc('\n');
            }
    }

    // scroll up
    if (fbinfo.chars_y < 0)
    {
        uint32_t row_size = fbinfo.pitch * GLYPH_HEIGHT;
        uint32_t all_but_one_rows = row_size * (num_rows - 1);
        memcpyr((uint8_t *)fbinfo.buf + row_size, (uint8_t *)fbinfo.buf, all_but_one_rows);
        bzero((uint8_t *)fbinfo.buf, row_size);
        fbinfo.chars_y++;
    }

    // scroll down
    if (fbinfo.chars_y == num_rows)
    {
        uint32_t row_size = fbinfo.pitch * GLYPH_HEIGHT;
        uint32_t all_but_one_rows = row_size * (num_rows - 1);
        memcpy((uint8_t *)fbinfo.buf, (uint8_t *)fbinfo.buf + row_size, all_but_one_rows);
        bzero((uint8_t *)fbinfo.buf + all_but_one_rows, row_size);
        fbinfo.chars_y--;
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
