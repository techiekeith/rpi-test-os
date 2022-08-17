/*
 * keyboard.h
 */

#pragma once

#include "../../../common/stddef.h"
#include "../../../common/stdint.h"
#include "../usb.h"
#include "hid.h"

/** The DeviceDriver field in UsbDriverDataHeader for keyboard devices. ASCII 'KBD0' (big-endian). */
#define DEVICE_DRIVER_KEYBOARD  0x4B424430

/** The maximum number of keys a keyboard can report at once. Should be
	multiple of 2. */
#define KEYBOARD_MAX_KEYS       6

/**
	\brief The current state of keyboard modifier keys.

	Encapsulates the current state of the keyboard modifier keys. Structure
	mimics the most common keyboard ordering.
*/
typedef struct {
    bool left_control : 1; // @0
    bool left_shift : 1; // @1
    bool left_alt : 1; // @2
    bool left_gui : 1; // the 'windows' key @3
    bool right_control : 1;  // @4
    bool right_shift : 1; // @5
    bool right_alt : 1; // 'alt gr' @6
    bool right_gui : 1; // @7
} __attribute__ ((__packed__)) keyboard_modifiers_t;

/**
	\brief The current state of keyboard LEDs.

	Encapsulates the current state of the keyboard LEDs. Structure mimics the
	most common lights and ordering. Not all keyboards may support all lights.
*/
typedef struct {
    bool number_lock : 1;
    bool caps_lock : 1;
    bool scroll_lock : 1;
    bool compose : 1;
    bool kana : 1;
    bool power : 1;
    bool mute : 1;
    bool shift : 1;
} __attribute__ ((__packed__)) keyboard_leds_t;

/**
	\brief Keyboard specific data.

	The contents of the driver data field for keyboard devices. Placed in
	HidDevice, as this driver is built atop that.
*/
typedef struct {
    /** Standard driver data header. */
    usb_driver_data_header_t header;
    /** Internal - Index in keyboard arrays. */
    uint32_t index;
    /** Number of keys currently held down. */
    uint32_t key_count;
    /** Keys currently held down. */
    uint16_t keys[KEYBOARD_MAX_KEYS];
    /** Modifier keys currently held down. */
    keyboard_modifiers_t modifiers;
    /** Which LEDs this keyboard supports. */
    keyboard_leds_t led_support;
    /** Which fields in the LED report are for what LEDs. */
    hid_parser_field_t *led_fields[8];
    /** Which fields in the Input report are for what modifiers and keys. */
    hid_parser_field_t *key_fields[8 + 1];
    /** The LED report. */
    hid_parser_report_t *led_report;
    /** The input report. */
    hid_parser_report_t *key_report;
} keyboard_device_t;

/**
	\brief Enumerates a device as a keyboard.

	Checks a device already checked by HidAttach to see if it appears to be a
	keyboard, and, if so, builds up necessary information to enable the
	keyboard methods.
*/
usb_call_result_t keyboard_attach(usb_device_t *device, uint32_t interface_number);


/**
	\brief Returns the number of keyboards connected to the system.
*/
uint32_t keyboard_get_count();

/**
	\brief Sets the keyboard LEDs to the state given in LEDs.

	Sets the keyboard LEDs to the state given in LEDs. Unimplemented LEDs are
	ignored silently.
*/
usb_call_result_t keyboard_set_leds(uint32_t keyboardAddress, keyboard_leds_t leds);

/**
	\brief Gets a list of available keyboard LEDs.

	Reads the availablility of keyboard LEDs from the report descriptor. LEDs
	that are present are set to 1, and those than are not are set to 0.
*/
keyboard_leds_t keyboard_get_led_support(uint32_t keyboardAddress);

/**
	\brief Checks a given keyboard.

	Reads back the report from a given keyboard and parses it into the internal
	fields. These can be accessed with KeyboardGet... methods
*/
usb_call_result_t keyboard_poll(uint32_t keyboardAddress);

/**
	\brief Reads the modifier keys from a keyboard.

	Reads back the state of the modifier keys from the last successfully
	received report. Zeros out by default.
*/
keyboard_modifiers_t keyboard_get_modifiers(uint32_t keyboardAddress);

/**
	\brief Returns the number of keys currently held down.

	Reads back the number of keys that were held down in the last report. If
	the keyboard reaches its key limit, this reports the last sensible report
	received.
*/
uint32_t keyboard_get_key_down_count(uint32_t keyboardAddress);

/**
	\brief Returns whether or not a particular key is held down.

	Reads back whether or not a key was held on the last successfully received
	report.
*/
bool keyboard_get_key_is_down(uint32_t keyboardAddress, uint16_t key);

/**
	\brief Returns the nth key that is held down.

	Reads back the number of the nth key that was held down in the last
	successfully received report.
*/
uint16_t keyboard_get_key_down(uint32_t keyboardAddress, uint32_t index);

/**
	\brief Returns the device address of the nth connected keyboard.

	Keyboards that are connected are stored in an array, and this method
	retrieves the nth item from that array. Returns 0 on error.
*/
uint32_t keyboard_get_address(uint32_t index);