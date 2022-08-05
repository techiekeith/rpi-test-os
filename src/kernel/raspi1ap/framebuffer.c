/*
 * framebuffer.c
 */

#include <common/stdio.h>
#include <kernel/delay.h>
#include <kernel/framebuffer.h>
#include <kernel/gpu.h>
#include <kernel/mem.h>
#include <kernel/mailbox.h>

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

int framebuffer_init(void)
{
	mail_message_t msg;

	fbinit.width = 640;
	fbinit.height = 480;
	fbinit.vwidth = fbinit.width;
	fbinit.vheight = fbinit.height;
	fbinit.bytes = 0;
	fbinit.depth = COLORDEPTH;
	fbinit.ignorex = 0;
	fbinit.ignorey = 0;
	fbinit.pointer = 0;
	fbinit.size = 0;

	uint32_t data = ((uint32_t)&fbinit | 0x40000000) >> 4;
	msg.data = data;
	msg.channel = FRAMEBUFFER_CHANNEL;

	mailbox_send(msg, FRAMEBUFFER_CHANNEL);
	delay(150);
	msg = mailbox_read(FRAMEBUFFER_CHANNEL);

	fbinfo.width = fbinit.width;
	fbinfo.height = fbinit.height;
	fbinfo.chars_width = fbinfo.width / GLYPH_WIDTH; 
    fbinfo.chars_height = fbinfo.height / GLYPH_HEIGHT;
    fbinfo.chars_x = 0;
    fbinfo.chars_y = 0;
	fbinfo.pitch = fbinit.bytes;
	fbinfo.buf = (void *)((uint32_t)fbinit.pointer & 0x3fffffff);
	fbinfo.buf_size = fbinit.size;

	return 0;
}
