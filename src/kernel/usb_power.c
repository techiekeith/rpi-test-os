/*
 * usb_power.c - USB power-on routine using Mailbox channel 0
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../../include/kernel/mailbox.h"

int usb_power_on_via_power_management_channel()
{
    // Power on the USB.
    mail_message_t msg;
    msg.data = 8; // I assume this switches on the USB host controller?
    mailbox_send(msg, POWER_MANAGEMENT_CHANNEL);
    delay(150);
    msg = mailbox_read(POWER_MANAGEMENT_CHANNEL);

    debug_printf("Power on returned %p.\r\n", msg);

    return (message.data == 8) ? 0 : -1;
}

int usb_power_on_via_property_channel()
{
    property_message_tag_t tags[2];
    tags[0].proptag = PWR_SET_STATE;
    tags[0].value_buffer.data[0] = DEVICE_USB_HOST_CONTROLLER;
    tags[0].value_buffer.data[1] = 3; // power on, wait
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv == 0)
    {
        debug_printf("Set power state (USB Host Controller): %d %d.\r\n",
                   tags[0].value_buffer.data[0],
                   tags[0].value_buffer.data[1]);
        return (tags[0].value_buffer.data[1]) == 1 ? 0 : -1;
    }
    return -1;
}
