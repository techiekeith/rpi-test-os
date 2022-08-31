/*
 * framebuffer.c
 */

#include "../../include/common/stdbool.h"
#include "../../include/common/stddef.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"

framebuffer_info_t fbinfo;
fb_init_t fbinit __attribute__((aligned(16)));
uint32_t palette[PALETTE_COLORS];

DEBUG_INIT("framebuffer");

void framebuffer_init()
{
    memset((void *)&fbinfo, 0, sizeof(framebuffer_info_t));
}

rgb_t rgb_685_levels(int i)
{
    int red, green, blue, green_v, blue_v;
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
        red = green = blue = 0;
    }
    rgb_t rgb = {
            .red = red,
            .green = green,
            .blue = blue
    };
    return rgb;
}

rgb_t rgb_685_levels_with_greyscale(int i)
{
    if (i < 240) return rgb_685_levels(i);
    int shade = (i & 15) | ((i & 15) << 4);
    rgb_t rgb = {
        .red = shade,
        .green = shade,
        .blue = shade
    };
    return rgb;
}

rgb_t rgb_685_levels_with_spectrum_colors(int i)
{
    if (i < 240) return rgb_685_levels(i);
    int shade = (i & 8) ? 0xff : 0xcc;
    rgb_t rgb = {
        .red = (i & 2) ? shade : 0,
        .green = (i & 4) ? shade : 0,
        .blue = (i & 1) ? shade : 0
    };
    return rgb;
}

rgb_t rgb_332_bits(int i)
{
    int red, green, blue;
    // Avoid division: ((n * 4663) >> 7) = int(n * 255 / 7) where n is an integer between 0 and 7.
    red = ((i >> 5) * 4663) >> 7;
    green = (((i >> 2) & 7) * 4663) >> 7;
    blue = (i & 3) * 85;
    rgb_t rgb = {
        .red = red,
        .green = green,
        .blue = blue
    };
    return rgb;
}

uint32_t abgr32(rgb_t color)
{
    return 0xff000000 | (color.blue << 16) | (color.green << 8) | color.red;
}

uint32_t argb32(rgb_t color)
{
    return 0xff000000 | (color.red << 16) | (color.green << 8) | color.blue;
}

rgb_t apply_palette_mode(int color_index)
{
    switch(fbinfo.palette_mode)
    {
        case RGB_685_LEVELS:
            return rgb_685_levels(color_index);
        case RGB_685_LEVELS_WITH_GREYSCALE:
            return rgb_685_levels_with_greyscale(color_index);
        case RGB_685_LEVELS_WITH_SPECTRUM_COLORS:
            return rgb_685_levels_with_spectrum_colors(color_index);
        case RGB_332_BITS:
            return rgb_332_bits(color_index);
        default:
            return rgb_685_levels(color_index);
    }
}

uint32_t apply_rgb_mode(rgb_t color)
{
    switch(fbinfo.rgb_mode)
    {
        case ARGB32: /* BGR little-endian */
            return argb32(color);
        case ABGR32: /* RGB little-endian */
            return abgr32(color);
        default:
            return fbinfo.channel_mode ? abgr32(color) : argb32(color);
    }
}

int set_pixel_order(bool rgb)
{
    DEBUG_START("set_pixel_order");

    // Create the message
    property_message_tag_t tags[2];
    tags[0].proptag = FB_SET_PIXEL_ORDER;
    tags[0].value_buffer.pixel_order = rgb ? PIXEL_ORDER_RGB : PIXEL_ORDER_BGR;
    tags[1].proptag = NULL_TAG;

    // Set the pixel order
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Set pixel order failed, return code %d.\r\n", rv);
    }
    else
    {
        fbinfo.rgb = (tags[0].value_buffer.pixel_order == PIXEL_ORDER_RGB);
        debug_printf("Pixel order: %s.\r\n", fbinfo.rgb ? "RGB" : "BGR");
    }

    DEBUG_END();
    return rv;
}

int init_palette()
{
    DEBUG_START("init_palette");

    // Generate the palette
    for (int i = 0; i < PALETTE_COLORS; i++)
    {
        palette[i] = apply_rgb_mode(apply_palette_mode(i));
    }

    // Create the message
    property_message_tag_t tags[3];
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
        debug_printf("Set palette failed, return code %d.\r\n", rv);
    }
    else
    {
        if (tags[0].value_buffer.data[0] != 0)
        {
            debug_printf("Set palette failed, response value %d.\r\n", tags[0].value_buffer.data[0]);
        }
        else
        {
            debug_printf("Color palette set.\r\n");
        }
    }

    DEBUG_END();
    return rv;
}

static int framebuffer_release()
{
    DEBUG_START("framebuffer_release");

    property_message_tag_t tags[2];
    tags[0].proptag = FB_RELEASE_BUFFER;
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Failed to release framebuffer, return code %d.\r\n", rv);
    }

    DEBUG_END();
    return rv;
}

static int framebuffer_allocate()
{
    DEBUG_START("framebuffer_allocate");

    property_message_tag_t tags[2];
    tags[0].proptag = FB_ALLOCATE_BUFFER;
    tags[0].value_buffer.fb_allocate_align = 16;
    tags[1].proptag = NULL_TAG;

    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Failed to allocate framebuffer, return code %d.\r\n", rv);
        DEBUG_END();
        return -1;
    }

    uint32_t allocated_size = tags[0].value_buffer.fb_allocate_res.fb_size;
    if (allocated_size != fbinfo.buf_size)
    {
        debug_printf("WARNING: Allocated size %d != expected size %d! This may not work.\r\n",
                     allocated_size, fbinfo.buf_size);
    }
    fbinfo.buf = tags[0].value_buffer.fb_allocate_res.fb_addr;
    fbinfo.buf_size = allocated_size;

    if ((((size_t)fbinfo.buf) & 0x40000000) && (peripheral_base < 0x40000000))
    {
        debug_printf("WARNING: Framebuffer address %p looks invalid. Trying to correct, but this may not work.\r\n",
                     fbinfo.buf);
        fbinfo.buf = (void *)((size_t)fbinfo.buf & 0x3fffffff);
    }

    debug_printf("Framebuffer allocated, address %p, size %d.\r\n", fbinfo.buf, fbinfo.buf_size);

    DEBUG_END();
    return 0;
}

static int set_display_dimensions_via_framebuffer_channel(uint32_t width, uint32_t height, uint32_t depth)
{
    DEBUG_START("set_display_dimensions_via_framebuffer_channel");

    mail_message_t msg;

    fbinit.physical_width = width;
    fbinit.physical_height = height;
    fbinit.virtual_width = fbinit.physical_width;
    fbinit.virtual_height = fbinit.physical_height;
    fbinit.pitch = 0;
    fbinit.depth = depth;
    fbinit.virtual_x_offset = 0;
    fbinit.virtual_y_offset = 0;
    fbinit.address = 0;
    fbinit.size = 0;

    uint32_t data = ((size_t)&fbinit | 0x40000000) >> 4;
    msg.data = data;
    msg.channel = FRAMEBUFFER_CHANNEL;

    mailbox_send(msg, FRAMEBUFFER_CHANNEL);
    mail_message_t result = mailbox_read(FRAMEBUFFER_CHANNEL);

    debug_printf("Init framebuffer returned %p.\r\n", result.as_int);

    fbinfo.width = fbinit.physical_width;
    fbinfo.height = fbinit.physical_height;
    fbinfo.depth = fbinit.depth;
    fbinfo.bpp = fbinit.depth >> 3;
    fbinfo.pitch = fbinit.pitch;
    fbinfo.buf = (void *)((size_t)fbinit.address & 0x3fffffff);
    fbinfo.buf_size = fbinit.size;

    debug_printf("Framebuffer allocated, address %p, size %d.\r\n", fbinfo.buf, fbinfo.buf_size);

    DEBUG_END();
    return 0;
}

static int set_display_dimensions_via_property_channel(uint32_t width, uint32_t height, uint32_t depth)
{
    DEBUG_START("set_display_dimensions_via_property_channel");

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

    debug_printf("Set framebuffer properties [0]: trying to set physical screen size %d x %d.\r\n",
                 tags[0].value_buffer.fb_screen_size.width,
                 tags[0].value_buffer.fb_screen_size.height);
    debug_printf("Set framebuffer properties [1]: trying to set virtual screen size %d x %d.\r\n",
                 tags[1].value_buffer.fb_screen_size.width,
                 tags[1].value_buffer.fb_screen_size.height);
    debug_printf("Set framebuffer properties [2]: trying to set depth %d bpp.\r\n", tags[2].value_buffer.fb_depth);

    // Send over the initialization
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Set framebuffer properties failed, return code %d.\r\n", rv);
        DEBUG_END();
        return -1;
    }
    debug_printf("Set framebuffer properties [0]: returned physical screen size %d x %d.\r\n",
                 tags[0].value_buffer.fb_screen_size.width,
                 tags[0].value_buffer.fb_screen_size.height);
    debug_printf("Set framebuffer properties [1]: returned virtual screen size %d x %d.\r\n",
                 tags[1].value_buffer.fb_screen_size.width,
                 tags[1].value_buffer.fb_screen_size.height);
    debug_printf("Set framebuffer properties [2]: returned depth %d bpp.\r\n", tags[2].value_buffer.fb_depth);

    tags[0].proptag = FB_GET_PHYSICAL_DISPLAY_SIZE;
    tags[0].value_buffer.fb_screen_size.width = 0;
    tags[0].value_buffer.fb_screen_size.height = 0;
    tags[1].proptag = FB_GET_VIRTUAL_DISPLAY_SIZE;
    tags[1].value_buffer.fb_screen_size.width = 0;
    tags[1].value_buffer.fb_screen_size.height = 0;
    tags[2].proptag = FB_GET_DEPTH;
    tags[2].value_buffer.fb_depth = 0;
    tags[3].proptag = NULL_TAG;

    // Send over the verification messages
    rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Get framebuffer properties failed, return code %d.\r\n", rv);
        DEBUG_END();
        return -1;
    }
    debug_printf("Get framebuffer properties [0]: physical screen size %d x %d.\r\n",
                 tags[0].value_buffer.fb_screen_size.width,
                 tags[0].value_buffer.fb_screen_size.height);
    debug_printf("Get framebuffer properties [1]: virtual screen size %d x %d.\r\n",
                 tags[1].value_buffer.fb_screen_size.width,
                 tags[1].value_buffer.fb_screen_size.height);
    debug_printf("Get framebuffer properties [2]: depth %d bpp.\r\n", tags[2].value_buffer.fb_depth);
    if (tags[0].value_buffer.fb_screen_size.width != width
        || tags[0].value_buffer.fb_screen_size.height != height
        || tags[1].value_buffer.fb_screen_size.width != width
        || tags[1].value_buffer.fb_screen_size.height != height
        || tags[2].value_buffer.fb_depth != depth)
    {
        debug_printf("Actual values do not match expected values.\r\n");
        DEBUG_END();
        return -1;
    }

    // Update our framebuffer_info_t struct
    fbinfo.width = tags[0].value_buffer.fb_screen_size.width;
    fbinfo.height = tags[0].value_buffer.fb_screen_size.height;
    fbinfo.depth = tags[2].value_buffer.fb_depth;
    fbinfo.bpp = fbinfo.depth >> 3;
    fbinfo.pitch = fbinfo.width * fbinfo.bpp;
    fbinfo.buf_size = fbinfo.pitch * fbinfo.height;

    rv = framebuffer_allocate();
    DEBUG_END();
    return rv;
}

int set_display_dimensions(uint32_t width, uint32_t height, uint32_t depth)
{
    DEBUG_START("set_display_dimensions");
    int rv;

    if (fbinfo.mode_discovered)
    {
        if (fbinfo.channel_mode)
        {
            rv = set_display_dimensions_via_framebuffer_channel(width, height, depth);
        }
        else
        {
            rv = set_display_dimensions_via_property_channel(width, height, depth);
        }
    }
    else
    {
        // Discover whether to use property channel or framebuffer channel
        rv = set_display_dimensions_via_property_channel(width, height, depth);
        if (rv != 0)
        {
            debug_printf("Allocate framebuffer via property channel failed, trying framebuffer channel instead.\r\n");
            rv = set_display_dimensions_via_framebuffer_channel(width, height, depth);
            fbinfo.channel_mode = true;
        }
        fbinfo.mode_discovered = true;
    }

    DEBUG_END();
    return rv;
}
