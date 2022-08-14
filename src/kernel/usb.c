/*
 * usb.c - USB driver
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"

static void usb_power_on()
{
    property_message_tag_t tags[2];
    tags[0].proptag = PWR_SET_STATE;
    tags[0].value_buffer.data[0] = DEVICE_USB_HOST_CONTROLLER;
    tags[0].value_buffer.data[1] = 1;
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Power on USB Host Controller failed, return code %d\r\n", rv);
    }
}

static void hcd_init()
{
    // Do something
}

static void hcd_start()
{
    // Do something
}

static void usb_attach_root_hub()
{
    // Do something
}

void usb_init()
{
//    bcm2835_usb_power_on();
    usb_power_on();
    hcd_init();
    hcd_start();
    usb_attach_root_hub();
}
