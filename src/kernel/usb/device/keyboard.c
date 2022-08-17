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
uint32_t keyboard_count __attribute__((aligned(4))) = 0;
uint32_t keyboard_addresses[MAX_KEYBOARDS] = { 0, 0, 0, 0 };
usb_device_t *keyboards[MAX_KEYBOARDS];

void keyboard_load()
{
    DEBUG_START("keyboard_load");

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

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return 0;
}

uint32_t keyboard_get_address(uint32_t index)
{
    DEBUG_START("keyboard_get_address");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return 0;
}

void keyboard_detached(usb_device_t *device)
{
    DEBUG_START("keyboard_detached");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
}

void keyboard_deallocate(usb_device_t *device)
{
    DEBUG_START("keyboard_deallocate");

    debug_printf("KBD: TODO.\r\n"); // TODO

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
    DEBUG_START("keyboard_get_count");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return 0;
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
    DEBUG_START("keyboard_get_modifiers");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return false;
}

uint16_t keyboard_get_key_down(uint32_t keyboard_address, uint32_t index)
{
    DEBUG_START("keyboard_get_modifiers");

    debug_printf("KBD: TODO.\r\n"); // TODO

    DEBUG_END();
    return 0;
}
