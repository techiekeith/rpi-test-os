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
#include "../../include/kernel/system_timer.h"
#include "configuration.h"
#include "platform.h"
#include "types.h"

void Bcm2835Load()
{
	LOG_DEBUG("CSUD: Broadcom2835 driver version 0.1.\n");
}

#ifndef TYPE_DRIVER

void MicroDelay(u32 delay_value) {
    system_timer_busy_wait(delay_value);
}

int usb_power_on();
Result PowerOnUsb() {
    return usb_power_on();
}

#endif