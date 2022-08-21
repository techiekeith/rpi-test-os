/******************************************************************************
*	platform/arm/broadcom2835.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/arm/broadcom2835.c contains code for the broadcom2835 chip, used 
*	in the Raspberry Pi. Compiled conditionally on LIB_BCM2835=1.
******************************************************************************/
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/mailbox.h"
#include "configuration.h"
#include "platform.h"
#include "types.h"

void Bcm2835Load()
{
	LOG_DEBUG("CSUD: Broadcom2835 driver version 0.1.\n");
}

#ifndef TYPE_DRIVER

void MicroDelay(u32 delay_value) {
    delay(delay_value);
}

Result PowerOnUsb() {
    property_message_tag_t tags[2];
    tags[0].proptag = PWR_SET_STATE;
    tags[0].value_buffer.data[0] = DEVICE_USB_HOST_CONTROLLER;
    tags[0].value_buffer.data[1] = 3; // power on, wait
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv == 0)
    {
        LOG_DEBUGF("USBD: set power state: %.x %.x.\r\n",
                   tags[0].value_buffer.data[0],
                   tags[0].value_buffer.data[1]);
        return tags[0].value_buffer.data[1] == 1 ? OK : ErrorDevice;
    }
    return ErrorDevice;
}

#endif