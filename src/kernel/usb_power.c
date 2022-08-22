/*
 * usb_power.c - USB power-on routines
 */

#include "../../include/kernel/delay.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"

DEBUG_INIT("usb_power");

int usb_power_on_via_power_management_channel()
{
    DEBUG_START("usb_power_on_via_power_management_channel");
    mail_message_t msg __attribute__((aligned(16)));
    // Cribbed shamelessly from Alex Chadwick's code at https://github.com/Chadderz121/csud
    msg.data = 8; // I assume this switches on the USB host controller
    msg.channel = POWER_MANAGEMENT_CHANNEL;
    mailbox_send(msg, POWER_MANAGEMENT_CHANNEL);
    delay(150);
    mail_message_t result;
//    result = mailbox_read(POWER_MANAGEMENT_CHANNEL);
    int rv = mailbox_read_with_timeout(POWER_MANAGEMENT_CHANNEL, &result, 10000);
    if (rv == 0)
    {
        debug_printf("Power on USB Host Controller: result=%p msg=%p.\r\n", result.as_int, msg.as_int);
        DEBUG_END();
        return (msg.data == 8) ? 0 : -1;
//        return 0;
    }
    debug_printf("Power on USB Host Controller failed: %d.\r\n", rv);
    DEBUG_END();
    return -1;
}

int usb_power_on_via_property_channel()
{
    DEBUG_START("usb_power_on_via_property_channel");
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
        DEBUG_END();
        return (tags[0].value_buffer.data[1]) == 1 ? 0 : -1;
    }
    debug_printf("Set power state (USB Host Controller) failed: %d.\r\n", rv);
    DEBUG_END();
    return -1;
}

int usb_power_on()
{
    DEBUG_START("usb_power_on");
    int rv = usb_power_on_via_property_channel();
    if (rv < 0)
    {
        debug_printf("Power on USB via property channel failed, trying power management channel instead.\r\n");
        rv = usb_power_on_via_power_management_channel();
        if (rv < 0)
        {
            debug_printf("Power on USB via power management channel failed.\r\n");
        }
    }
    DEBUG_END();
    return rv;
}
