/*
 * framebuffer.c
 */

#include "../../../include/kernel/framebuffer.h"
#include "../../../include/kernel/io.h"
#include "../../../include/kernel/mailbox.h"

framebuffer_info_t fbinfo;

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint32_t vwidth;
	uint32_t vheight;
	uint32_t bytes;
	uint32_t depth;
	uint32_t ignorex;
	uint32_t ignorey;
	void * pointer;
	uint32_t size;
} fb_init_t;

fb_init_t fbinit __attribute__((aligned(16)));

int bcm2835_set_display_dimensions(int width, int height, int depth, int char_width, int char_height)
{
	mail_message_t msg;

	fbinit.width = width;
	fbinit.height = height;
	fbinit.vwidth = fbinit.width;
	fbinit.vheight = fbinit.height;
	fbinit.bytes = 0;
	fbinit.depth = depth;
	fbinit.ignorex = 0;
	fbinit.ignorey = 0;
	fbinit.pointer = 0;
	fbinit.size = 0;

	uint32_t data = ((size_t)&fbinit | 0x40000000) >> 4;
	msg.data = data;
	msg.channel = FRAMEBUFFER_CHANNEL;

	mailbox_send(msg, FRAMEBUFFER_CHANNEL);
	msg = mailbox_read(FRAMEBUFFER_CHANNEL);

    debug_printf("Init framebuffer returned %p\r\n", msg);

    fbinfo.width = fbinit.width;
	fbinfo.height = fbinit.height;
    fbinfo.depth = fbinit.depth;
    fbinfo.bpp = fbinit.depth >> 3;
    fbinfo.pitch = fbinfo.width * fbinfo.bpp;
    fbinfo.char_width = char_width;
    fbinfo.char_height = char_height;
	fbinfo.columns = fbinfo.width / char_width;
    fbinfo.rows = fbinfo.height / char_height;
    fbinfo.current_column = 0;
    fbinfo.current_row = 0;
	fbinfo.buf = (void *)((size_t)fbinit.pointer & 0x3fffffff);
	fbinfo.buf_size = fbinit.size;

    debug_printf("Framebuffer allocated, location %p, size %d\r\n", fbinfo.buf, fbinfo.buf_size);

	return 0;
}
