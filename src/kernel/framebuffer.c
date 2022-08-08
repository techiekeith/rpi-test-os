/*
 * framebuffer.c
 */

#include <common/stdio.h>
#include <kernel/framebuffer.h>
#include <kernel/mailbox.h>
#include <saa505x/glyphs.h>

framebuffer_info_t fbinfo;

int framebuffer_init()
{
    int rv;

    property_message_tag_t tags[5];

    tags[0].proptag = FB_SET_PHYSICAL_DIMENSIONS;
    tags[0].value_buffer.fb_screen_size.width = DISPLAY_WIDTH;
    tags[0].value_buffer.fb_screen_size.height = DISPLAY_HEIGHT;
    tags[1].proptag = FB_SET_VIRTUAL_DIMENSIONS;
    tags[1].value_buffer.fb_screen_size.width = DISPLAY_WIDTH;
    tags[1].value_buffer.fb_screen_size.height = DISPLAY_HEIGHT;
    tags[2].proptag = FB_SET_BITS_PER_PIXEL;
    tags[2].value_buffer.fb_bits_per_pixel = COLORDEPTH;
    tags[3].proptag = FB_SET_PALETTE;
    tags[3].value_buffer.fb_palette.start_index = 0;
    tags[3].value_buffer.fb_palette.num_entries = PALETTE_COLORS;
    for (int palette_entry = 0; palette_entry < PALETTE_COLORS; palette_entry++)
    {
        tags[3].value_buffer.fb_palette.entry[palette_entry] = palette[palette_entry];
    }
    tags[4].proptag = NULL_TAG;

    // Send over the initialization
    rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Set framebuffer properties failed, return code %d\n", rv);
        return -1;
    }

    fbinfo.width = tags[0].value_buffer.fb_screen_size.width;
    fbinfo.height = tags[0].value_buffer.fb_screen_size.height;
    fbinfo.chars_width = fbinfo.width / GLYPH_WIDTH;
    fbinfo.chars_height = fbinfo.height / GLYPH_HEIGHT;
    fbinfo.chars_x = 0;
    fbinfo.chars_y = 0;
    fbinfo.pitch = fbinfo.width * BYTES_PER_PIXEL;

    // request a framebuffer
    tags[0].proptag = FB_ALLOCATE_BUFFER;
    tags[0].value_buffer.fb_screen_size.width = 0;
    tags[0].value_buffer.fb_screen_size.height = 0;
    tags[0].value_buffer.fb_allocate_align = 16;
    tags[1].proptag = NULL_TAG;

    rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Allocate framebuffer failed, return code %d\n", rv);
        return -1;
    }

    fbinfo.buf = tags[0].value_buffer.fb_allocate_res.fb_addr;
    fbinfo.buf_size = tags[0].value_buffer.fb_allocate_res.fb_size;

    debug_printf("Framebuffer allocated, location %p, size %d\n", fbinfo.buf, fbinfo.buf_size);

    return 0;
}
