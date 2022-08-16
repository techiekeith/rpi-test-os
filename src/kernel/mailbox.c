/*
 * mailbox.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"
#include "../../include/kernel/mailbox_methods.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/mmio.h"

DEBUG_INIT("mailbox");

mail_message_t mailbox_read(int channel)
{
    volatile mail_status_t stat;
    volatile mail_message_t res;

    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to recieve
        do
        {
            stat.as_int = mmio_read(MAIL0_STATUS);
        } while (stat.empty);

        // Get the message
        res.as_int = mmio_read(MAIL0_READ);
    } while (res.channel != channel);
    __dmb();

    return res;
}

void mailbox_send(mail_message_t msg, int channel)
{
    volatile mail_status_t stat;
    msg.channel = channel;

    // Make sure you can send mail
    __dmb();
    do
    {
        stat.as_int = mmio_read(MAIL1_STATUS);
    } while (stat.full);

    // send the message
    mmio_write(MAIL1_WRITE, msg.as_int);
}

mailbox_method_t *get_mailbox_method_by_name(char *method_name)
{
    int i;
    for (i = 0; mailbox_methods[i].tag != NULL_TAG && strcmp(mailbox_methods[i].method_name, method_name); i++);
    return mailbox_methods[i].tag == NULL_TAG ? NULL : &mailbox_methods[i];
}

mailbox_method_t *get_mailbox_method_by_tag(property_tag_t tag)
{
    int i;
    for (i = 0; mailbox_methods[i].tag != NULL_TAG && mailbox_methods[i].tag != tag; i++);
    return mailbox_methods[i].tag == NULL_TAG ? NULL : &mailbox_methods[i];
}

static uint32_t get_value_buffer_size(property_tag_t tag)
{
    int i;
    for (i = 0; mailbox_methods[i].tag != NULL_TAG && mailbox_methods[i].tag != tag; i++);
    return mailbox_methods[i].tag == NULL_TAG ? 0 :
            MAX(mailbox_methods[i].request_size, mailbox_methods[i].response_size, uint32_t);
}

int send_messages(property_message_tag_t *tags)
{
    property_message_buffer_t *msg;
    mail_message_t mail;
    uint32_t i, bufpos, len;
    size_t bufsize = 0;

    DEBUG_START("send_messages");

    // Calculate the sizes of each tag
    for (i = 0; tags[i].proptag != NULL_TAG; i++)
    {
        bufsize += get_value_buffer_size(tags[i].proptag) + 3 * sizeof(uint32_t);
    }

    // Add the buffer size, buffer request/response code and buffer end tag sizes
    bufsize += 3 * sizeof(uint32_t);

    // buffer size must be 16 byte aligned
    bufsize += (bufsize % 16) ? 16 - (bufsize % 16) : 0;

    msg = heap_alloc("send_messages", bufsize);
    if (!msg)
    {
        return -1;
    }

    msg->size = bufsize;
    msg->req_res_code = REQUEST;

    // Copy the messages into the buffer
    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++)
    {
        len = get_value_buffer_size(tags[i].proptag);
        msg->tags[bufpos++] = tags[i].proptag;
        msg->tags[bufpos++] = len;
        msg->tags[bufpos++] = 0;
        memcpy(msg->tags+bufpos, &tags[i].value_buffer, len);
        bufpos += len / 4;
    }

    msg->tags[bufpos] = 0;

    // Send the message
    mail.data = ((size_t)msg) >> 4;

    mailbox_send(mail, PROPERTY_CHANNEL);
    delay(150);
    mail = mailbox_read(PROPERTY_CHANNEL);

    debug_printf("mailbox_read returned 0x%ux\r\n", mail);

    if (msg->req_res_code == REQUEST)
    {
        debug_printf("msg still has req_res_code REQUEST\r\n");
        heap_free(msg);
        return 1;
    }
    // Check the response code
    if (msg->req_res_code == RESPONSE_ERROR)
    {
        heap_free(msg);
        return 2;
    }

    
    // Copy the tags back into the array
    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++)
    {
        debug_printf("mailbox_read returned tag 0x%ux\r\n", tags[i].proptag);
        len = get_value_buffer_size(tags[i].proptag);
        bufpos += 3; // skip over the tag bookkeeping info
        memcpy(&tags[i].value_buffer, msg->tags+bufpos, len);
        bufpos += len / 4;
    }

    heap_free(msg);

    DEBUG_END();
    return 0;
}
