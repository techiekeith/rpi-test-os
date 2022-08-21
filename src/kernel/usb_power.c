/*
 * usb_power.c - USB power-on routine using Mailbox channel 0
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../include/kernel/delay.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"

int usb_power_on_via_power_management_channel()
{
    mail_message_t msg;
    msg.data = 8; // I assume this switches on the USB host controller?
    mailbox_send(msg, POWER_MANAGEMENT_CHANNEL);
    delay(150);
    int rv = mailbox_read_with_timeout(POWER_MANAGEMENT_CHANNEL, &msg, 150);
    if (rv == 0)
    {
        debug_printf("Power on USB Host Controller: %d.\r\n", msg.data);
        return (msg.data == 8) ? 0 : -1;
    }
    debug_printf("Power on USB Host Controller failed: %d.\r\n", rv);
    return -1;
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
    debug_printf("Set power state (USB Host Controller) failed: %d.\r\n", rv);
    return -1;
}
