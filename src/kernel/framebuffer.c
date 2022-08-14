/*
 * framebuffer.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"

framebuffer_info_t fbinfo;
uint32_t palette[PALETTE_COLORS];

void framebuffer_init()
{
    memset((void *)&fbinfo, 0, sizeof(framebuffer_info_t));
}

int init_palette()
{
    // Generate the palette
    int red, green, blue, green_v, blue_v;
    for (int i = 0; i < PALETTE_COLORS; i++)
    {
        if (i < 240)
        {
            red = (i / 40) * 51;
            green_v = ((i % 40) / 5) * 255;
            blue_v = (i % 5) * 255;
            green = (green_v / 7) + ((green_v % 7) >> 2);
            blue = (blue_v / 4) + ((blue_v % 4) >> 1);
        }
        else
        {
            blue = i & 15;
            blue |= blue << 4;
            red = green = blue;
        }
        palette[i] = red | (green << 8) | (blue << 16) | 0xff000000;
    }

    // Create the message
    property_message_tag_t tags[2];
    tags[0].proptag = FB_SET_PALETTE;
    tags[0].value_buffer.fb_palette.start_index = 0;
    tags[0].value_buffer.fb_palette.num_entries = PALETTE_COLORS;
    for (int palette_entry = 0; palette_entry < PALETTE_COLORS; palette_entry++)
    {
        tags[0].value_buffer.fb_palette.entry[palette_entry] = palette[palette_entry];
    }
    tags[1].proptag = NULL_TAG;

    // Update the palette
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Set palette failed, return code %d\r\n", rv);
    }
    return rv;
}

static int framebuffer_release()
{
    property_message_tag_t tags[2];
    tags[0].proptag = FB_RELEASE_BUFFER;
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Failed to release framebuffer, return code %d\r\n", rv);
    }
    return rv;
}

static int framebuffer_allocate()
{
    property_message_tag_t tags[2];
    tags[0].proptag = FB_ALLOCATE_BUFFER;
    tags[0].value_buffer.fb_screen_size.width = 0;
    tags[0].value_buffer.fb_screen_size.height = 0;
    tags[0].value_buffer.fb_allocate_align = 16;
    tags[1].proptag = NULL_TAG;

    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Allocate framebuffer failed, return code %d\r\n", rv);
        return -1;
    }

    fbinfo.buf = tags[0].value_buffer.fb_allocate_res.fb_addr;
    fbinfo.buf_size = tags[0].value_buffer.fb_allocate_res.fb_size;

    debug_printf("Framebuffer allocated, location %p, size %d\r\n", fbinfo.buf, fbinfo.buf_size);

    return 0;
}

int set_display_dimensions(int width, int height, int depth, int char_width, int char_height)
{
    if (fbinfo.buf)
    {
        framebuffer_release();
        fbinfo.buf = NULL;
    }

    property_message_tag_t tags[4];
    tags[0].proptag = FB_SET_PHYSICAL_DISPLAY_SIZE;
    tags[0].value_buffer.fb_screen_size.width = width;
    tags[0].value_buffer.fb_screen_size.height = height;
    tags[1].proptag = FB_SET_VIRTUAL_DISPLAY_SIZE;
    tags[1].value_buffer.fb_screen_size.width = width;
    tags[1].value_buffer.fb_screen_size.height = height;
    tags[2].proptag = FB_SET_DEPTH;
    tags[2].value_buffer.fb_depth = depth;
    tags[3].proptag = NULL_TAG;

    // Send over the initialization
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Set framebuffer properties failed, return code %d\r\n", rv);
        return -1;
    }

    // Update our framebuffer_info_t struct
    fbinfo.width = tags[0].value_buffer.fb_screen_size.width;
    fbinfo.height = tags[0].value_buffer.fb_screen_size.height;
    fbinfo.depth = tags[2].value_buffer.fb_depth;
    fbinfo.bpp = fbinfo.depth >> 3;
    fbinfo.pitch = fbinfo.width * fbinfo.bpp;
    fbinfo.char_width = char_width;
    fbinfo.char_height = char_height;
    fbinfo.columns = fbinfo.width / fbinfo.char_width;
    fbinfo.rows = fbinfo.height / fbinfo.char_height;
    fbinfo.current_column = 0;
    fbinfo.current_row = 0;

    return framebuffer_allocate();
}
