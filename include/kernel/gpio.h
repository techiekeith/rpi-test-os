/*
 * gpio.h
 */

#pragma once

#include "peripheral.h"

/* The GPIO registers base address. */
#define GPIO_BASE   (peripheral_base + GPIO_OFFSET)

/* The offsets for reach register. */

/* Controls actuation of pull up/down to ALL GPIO pins. */
#define GPPUD       (GPIO_BASE + 0x94)

/* Controls actuation of pull up/down for specific GPIO pin. */
#define GPPUDCLK0   (GPIO_BASE + 0x98)
