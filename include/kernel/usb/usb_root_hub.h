/*
 * usb_root_hub.h
 */

#pragma once

#include "../../common/stdint.h"
#include "usb.h"

typedef struct {
    usb_configuration_descriptor_t configuration;
    usb_interface_descriptor_t interface;
    usb_endpoint_descriptor_t endpoint;
} __attribute__ ((packed)) hub_configuration_descriptor_t;

typedef enum {
    HUB_PORT_CONTROL_GLOBAL = 0,
    HUB_PORT_CONTROL_INDIVIDUAL = 1
} hub_port_control_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type;
    uint8_t port_count;
    struct {
        hub_port_control_t power_switching_mode: 2;
        bool compound: 1;
        hub_port_control_t over_current_protection: 2;
        uint8_t think_time: 2;
        bool indicators: 1;
        uint8_t reserved_8_15: 8;
    } __attribute__ ((packed)) attributes;
    uint8_t power_good_delay;
    uint8_t maximum_hub_power;
    uint8_t data[];
} __attribute__ ((packed)) usb_hub_descriptor_t;

/**
	\brief The device number of the root hub.

	We keep track of the device number of the virtual root hub we are
	simulating.
*/
extern uint32_t root_hub_device_number;

/**
	\brief Sends a message to the virtual root hub for processing.

	Passes a message to the virtual root hub for processing. The buffer can be
	both in and out.
*/
usb_call_result_t hcd_process_root_hub_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer,
                                               uint32_t buffer_length, usb_device_request_t *request);
