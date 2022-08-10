/*
 * usb_power.c - USB power-on routine using Mailbox channel 0
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../../include/kernel/mailbox.h"

int bcm2835_usb_power_on()
{
    // Power on the USB.
    mail_message_t msg;
    msg.data = 8; // I assume this denotes the USB host controller?
    mailbox_send(msg, POWER_MANAGEMENT_CHANNEL);
    delay(150);
    msg = mailbox_read(POWER_MANAGEMENT_CHANNEL);

    debug_printf("Power on returned %p\r\n", msg);

    return (message.data != 8);
}
