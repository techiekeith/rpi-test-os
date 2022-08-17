/*
 * keyboard.c
 */

#include "../../../../include/common/stdbool.h"
#include "../../../../include/common/stdint.h"
#include "../../../../include/kernel/io.h"
#include "../../../../include/kernel/usb/usb.h"
#include "../../../../include/kernel/usb/device/keyboard.h"
#include "../../../../include/kernel/usb/device/hid.h"

DEBUG_INIT("keyboard");

#define MAX_KEYBOARDS   4
#define NO_KEY          0xffffffff
static uint32_t keyboard_count __attribute__((aligned(4))) = 0;
static uint32_t keyboard_addresses[MAX_KEYBOARDS] = { 0, 0, 0, 0 };
static usb_device_t *keyboards[MAX_KEYBOARDS];

void keyboard_load()
{
    DEBUG_START("keyboard_load");

    debug_printf("KBD: Keyboard driver version 0.1 (derived from CSUD).\r\n");
    keyboard_count = 0;
    for (uint32_t i = 0; i < MAX_KEYBOARDS; i++)
    {
        keyboard_addresses[i] = 0;
        keyboards[i] = NULL;
    }
    hid_usage_attach(HID_UPD_DESKTOP_KEYBOARD, keyboard_attach);

    DEBUG_END();
}

uint32_t keyboard_index(uint32_t address)
{
    DEBUG_START("keyboard_index");

    if (address == 0) return NO_KEY;

    for (uint32_t i = 0; i < keyboard_count; i++)
    {
        if (keyboard_addresses[i] == address) return i;
    }

    DEBUG_END();
    return NO_KEY;
}

uint32_t keyboard_get_address(uint32_t index)
{
    DEBUG_START("keyboard_get_address");

    if (index > keyboard_count) return 0;

    for (uint32_t i = 0; index >= 0 && i < MAX_KEYBOARDS; i++)
    {
        if ((keyboard_addresses[i] != 0) && (index-- == 0)) return keyboard_addresses[i];
    }

    DEBUG_END();
    return 0;
}

void keyboard_detached(usb_device_t *device)
{
    DEBUG_START("keyboard_detached");

    keyboard_device_t *data = (keyboard_device_t *)((hid_device_t *)device->driver_data)->driver_data;
    if (data != NULL)
    {
        if (keyboard_addresses[data->index] == device->number)
        {
            keyboard_addresses[data->index] = 0;
            keyboard_count--;
            keyboards[data->index] = NULL;
        }
    }

    DEBUG_END();
}

void keyboard_deallocate(usb_device_t *device)
{
    DEBUG_START("keyboard_deallocate");

    keyboard_device_t *data = (keyboard_device_t *)((hid_device_t *)device->driver_data)->driver_data;
    if (data != NULL)
    {
        heap_free(data);
        ((hid_device_t *)device->driver_data)->driver_data = NULL;
    }
    ((hid_device_t *)device->driver_data)->hid_deallocate = NULL;
    ((hid_device_t *)device->driver_data)->hid_detached = NULL;

    DEBUG_END();
}

usb_call_result_t keyboard_attach(usb_device_t *device, uint32_t interface_number)
{
    DEBUG_START("keyboard_attach");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

uint32_t keyboard_get_count()
{
    return keyboard_count;
}

usb_call_result_t keyboard_set_leds(uint32_t keyboard_address, keyboard_leds_t leds)
{
    DEBUG_START("keyboard_set_leds");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

keyboard_leds_t keyboard_get_led_support(uint32_t keyboard_address)
{
    DEBUG_START("keyboard_get_led_support");

    debug_printf("KBD: TODO.\r\n"); // TODO
    keyboard_leds_t dummy = {
            .caps_lock = 0
    };

    DEBUG_END();
    return dummy;
}

usb_call_result_t keyboard_poll(uint32_t keyboard_address)
{
    DEBUG_START("keyboard_poll");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

keyboard_modifiers_t keyboard_get_modifiers(uint32_t keyboard_address)
{
    DEBUG_START("keyboard_get_modifiers");

    debug_printf("KBD: TODO.\r\n"); // TODO
    keyboard_modifiers_t dummy = {
            .left_shift = 0
    };

    DEBUG_END();
    return dummy;
}

bool keyboard_get_key_is_down(uint32_t keyboard_address, uint16_t key)
{
    DEBUG_START("keyboard_get_key_is_down");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return false;
}

uint16_t keyboard_get_key_down(uint32_t keyboard_address, uint32_t index)
{
    DEBUG_START("keyboard_get_key_down");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return 0;
}
