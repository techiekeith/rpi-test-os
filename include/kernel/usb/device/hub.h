/*
 * hub.h
 */

#pragma once

#include "../../../common/stddef.h"
#include "../../../common/stdint.h"
#include "../usb.h"
#include "../usb_root_hub.h"

/* The DeviceDriver field in UsbDriverDataHeader for hubs. ASCII 'HUB0' (big-endian). */
#define DEVICE_DRIVER_HUB   0x48554230

/**
	\brief Encapsulates the current status of a hub.

	The hub status structure defined in 11.24.2.6 of the USB2.0
	standard.
*/
typedef struct {
    bool local_power: 1;
    bool over_current: 1;
    uint16_t reserved_2_15: 14;
} __attribute__ ((packed)) hub_status_t;

/**
	\brief Encapsulates the change in current status of a hub.

	The hub status change structure defined in 11.24.2.6 of the USB2.0
	standard.
*/
typedef struct {
    bool local_power_changed: 1;
    bool over_current_changed: 1;
    uint16_t reserved_2_15: 14;
} __attribute__ ((packed)) hub_status_change_t;

/**
	\brief Encapsulates the full status of a hub.

	The hub status structure defined in 11.24.2.6 of the USB2.0 standard.
*/
typedef struct {
    hub_status_t status;
    hub_status_change_t change;
} __attribute__ ((packed)) hub_full_status_t;

/**
	\brief Encapsulates the current status of a hub port.

	The hub port status structure defined in 11.24.2.7.1 of the USB2.0
	standard.
*/
typedef struct {
    bool connected: 1;
    bool enabled: 1;
    bool suspended: 1;
    bool over_current: 1;
    bool reset: 1;
    uint8_t reserved_5_7: 3;
    bool power: 1;
    bool low_speed_attached: 1;
    bool high_speed_attached: 1;
    bool test_mode: 1;
    bool indicator_control: 1;
    uint8_t reserved_13_15: 3;
} __attribute__ ((packed)) hub_port_status_t;

/**
	\brief Encapsulates the change in current status of a hub port.

	The hub port status change structure defined in 11.24.2.7.2 of the USB2.0
	standard.
*/
typedef struct {
    bool connected_changed: 1;
    bool enabled_changed: 1;
    bool suspended_changed: 1;
    bool over_current_changed: 1;
    bool reset_changed: 1;
    uint16_t reserved_5_15: 11;
} __attribute__ ((packed)) hub_port_status_change_t;

/**
	\brief Encapsulates the full status of a hub port.

	The hub port status structure defined in 11.24.2.7 of the USB2.0 standard.
*/
typedef struct {
    hub_port_status_t status;
    hub_port_status_change_t change;
} __attribute__ ((packed)) hub_port_full_status_t;

/**
	\brief Hub specific data.

	The contents of the driver data field for hubs.
*/
typedef struct {
    usb_driver_data_header_t *header;
    hub_full_status_t status;
    usb_hub_descriptor_t *descriptor;
    uint32_t max_children;
    hub_port_full_status_t port_status[MAX_CHILDREN_PER_DEVICE];
    usb_device_t *children[MAX_CHILDREN_PER_DEVICE];
} __attribute__ ((packed)) hub_device_t;

/**
	\brief Performs all necessary hub related initialisation.

	Loads a hub device and enumerates its children.
*/
usb_call_result_t hub_attach(usb_device_t *device, uint32_t interface_number);

/**
	\brief Resets a port on a hub.

	Resets a port on a hub. No validation.
*/
usb_call_result_t hub_port_reset(usb_device_t *device, uint8_t port);

/**
	\brief Checks the connection status of a port.

	Checks for a change in the connection status of a port. If it has changed
	performs necessary actions such as enumerating a new device or deallocating
	an old one.
*/
usb_call_result_t hub_check_connection(usb_device_t *device, uint8_t port);

/**
	\brief Checks all hubs for new devices.

	Recursively checks the hub tree for new devices being attached, and for old
	devices being removed.
*/
void hub_recursive_check(usb_device_t *device);
