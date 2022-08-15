/*
 * usb.h - USB device related definitions
 */

#include "../common/stdbool.h"
#include "../common/stdint.h"
#include "peripheral.h"

#pragma once

#define MAX_CHILDREN_PER_DEVICE         10
#define MAX_INTERFACES_PER_DEVICE       8
#define MAX_ENDPOINTS_PER_INTERFACE     16

#define MAXIMUM_USB_DEVICES             32
#define CONTROL_MESSAGE_TIMEOUT         10

typedef enum {
    USB_TRANSFER_TYPE_CONTROL,
    USB_TRANSFER_TYPE_ISOCHRONOUS,
    USB_TRANSFER_TYPE_BULK,
    USB_TRANSFER_TYPE_INTERRUPT
} usb_transfer_type_t;

typedef enum {
    HOST_TO_DEVICE = 0, // aka "Out"
    DEVICE_TO_HOST = 1  // aka "In"
} usb_direction_t;

typedef enum {
    USB_PACKET_SIZE_8_BIT = 0,
    USB_PACKET_SIZE_16_BIT = 1,
    USB_PACKET_SIZE_32_BIT = 2,
    USB_PACKET_SIZE_64_BIT = 3
} usb_packet_size_t;

typedef enum {
    USB_SPEED_HIGH = 0,
    USB_SPEED_FULL = 1,
    USB_SPEED_LOW = 2
} usb_speed_t;

typedef enum {
    USB_DEVICE_STATUS_ATTACHED,
    USB_DEVICE_STATUS_POWERED,
    USB_DEVICE_STATUS_DEFAULT,
    USB_DEVICE_STATUS_ADDRESSED,
    USB_DEVICE_STATUS_CONFIGURED
} usb_device_status_t;

typedef enum {
    USB_TRANSFER_ERROR_NONE = 0,
    USB_TRANSFER_ERROR_STALL = 1 << 1,
    USB_TRANSFER_ERROR_BUFFER_ERROR = 1 << 2,
    USB_TRANSFER_ERROR_BABBLE = 1 << 3,
    USB_TRANSFER_ERROR_NO_ACKNOWLEDGE = 1 << 4,
    USB_TRANSFER_ERROR_CRC_ERROR = 1 << 5,
    USB_TRANSFER_ERROR_BIT_ERROR = 1 << 6,
    USB_TRANSFER_ERROR_CONNECTION_ERROR = 1 << 7,
    USB_TRANSFER_ERROR_AHB_ERROR = 1 << 8,
    USB_TRANSFER_ERROR_NOT_YET_ERROR = 1 << 9,
    USB_TRANSFER_ERROR_PROCESSING = 1 << 31
} usb_transfer_error_t;

typedef enum {
    DESCRIPTOR_TYPE_DEVICE = 1,
    DESCRIPTOR_TYPE_CONFIGURATION = 2,
    DESCRIPTOR_TYPE_STRING = 3,
    DESCRIPTOR_TYPE_INTERFACE = 4,
    DESCRIPTOR_TYPE_ENDPOINT = 5,
    DESCRIPTOR_TYPE_DEVICE_QUALIFIER = 6,
    DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION = 7,
    DESCRIPTOR_TYPE_INTERFACE_POWER = 8,
    DESCRIPTOR_TYPE_HID = 33,
    DESCRIPTOR_TYPE_HID_REPORT = 34,
    DESCRIPTOR_TYPE_HID_PHYSICAL = 35,
    DESCRIPTOR_TYPE_HUB = 41
} descriptor_type_t;

typedef enum {
    DEVICE_CLASS_IN_INTERFACE = 0,
    DEVICE_CLASS_COMMUNICATIONS = 2,
    DEVICE_CLASS_HUB = 9,
    DEVICE_CLASS_DIAGNOSTIC = 0xdc,
    DEVICE_CLASS_MISCELLANEOUS = 0xef,
    DEVICE_CLASS_VENDOR_SPECIFIC = 0xff
} device_class_t;

typedef enum {
    INTERFACE_CLASS_RESERVED = 0,
    INTERFACE_CLASS_AUDIO = 1,
    INTERFACE_CLASS_COMMUNICATIONS = 2,
    INTERFACE_CLASS_HID = 3,
    INTERFACE_CLASS_PHYSICAL = 5,
    INTERFACE_CLASS_IMAGE = 6,
    INTERFACE_CLASS_PRINTER = 7,
    INTERFACE_CLASS_MASS_STORAGE = 8,
    INTERFACE_CLASS_HUB = 9,
    INTERFACE_CLASS_CDC_DATA = 10,
    INTERFACE_CLASS_SMART_CARD = 11,
    INTERFACE_CLASS_CONTENT_SECURITY = 13,
    INTERFACE_CLASS_VIDEO = 14,
    INTERFACE_CLASS_PERSONAL_HEALTHCARE = 15,
    INTERFACE_CLASS_AUDIO_VIDEO = 16,
    INTERFACE_CLASS_DIAGNOSTIC_DEVICE = 0xdc,
    INTERFACE_CLASS_WIRELESS_CONTROLLER = 0xe0,
    INTERFACE_CLASS_MISCELLANEOUS = 0xef,
    INTERFACE_CLASS_APPLICATION_SPECIFIC = 0xfe,
    INTERFACE_CLASS_VENDOR_SPECIFIC = 0xff
} interface_class_t;

typedef enum {
    SYNCHRONIZATION_NONE = 0,
    SYNCHRONIZATION_ASYNCHRONOUS = 1,
    SYNCHRONIZATION_ADAPTIVE = 2,
    SYNCHRONIZATION_SYNCHRONOUS = 3
} usb_endpoint_synchronization_t;

typedef enum {
    USAGE_DATA = 0,
    USAGE_FEEDBACK = 1,
    USAGE_IMPLICIT_FEEDBACK_DATA = 2
} usb_endpoint_usage_t;

typedef enum {
    TRANSACTIONS_NONE = 0,
    TRANSACTIONS_EXTRA1 = 1,
    TRANSACTIONS_EXTRA2 = 2
} usb_endpoint_transactions_t;

typedef struct {
    uint32_t device_driver;
    uint32_t data_size;
} usb_driver_data_header_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint16_t usb_version;
    device_class_t device_class: 8;
    uint8_t sub_class;
    uint8_t protocol;
    uint8_t max_packet_size_0;
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t version;
    uint8_t manufacturer;
    uint8_t product;
    uint8_t serial_number;
    uint8_t configuration_count;
} __attribute__((packed)) usb_device_descriptor_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint16_t usb_version;
    device_class_t device_class: 8;
    uint8_t sub_class;
    uint8_t protocol;
    uint8_t max_packet_size_0;
    uint8_t configuration_count;
    uint8_t reserved_9;
} __attribute__((packed)) usb_device_qualifier_descriptor_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint16_t total_length;
    uint8_t interface_count;
    uint8_t configuration_value;
    uint8_t string_index;
    struct {
        uint8_t reserved_0_4: 5;
        bool remote_wakeup: 1;
        bool self_powered: 1;
        bool reserved_7: 1;
    } __attribute__((packed)) attributes;
    uint8_t maximum_power;
} __attribute__((packed)) usb_configuration_descriptor_t;

/**
 * usb_other_speed_configuration_descriptor_t seems to be identical to usb_configuration_descriptor_t
 * save that in AC's code the 'reserved_7' attribute bit has an enum with a definition of 'Valid = 1'.
 */
typedef usb_configuration_descriptor_t usb_other_speed_configuration_descriptor_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint8_t number;
    uint8_t alternate_setting;
    uint8_t endpoint_count;
    interface_class_t interface_class: 8;
    uint8_t sub_class;
    uint8_t protocol;
    uint8_t string_index;
} __attribute__((packed)) usb_interface_descriptor_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    struct {
        uint8_t number: 4;
        uint8_t reserved_4_6: 3;
        usb_direction_t direction: 1;
    } __attribute__((packed)) endpoint_address;
    struct {
        usb_transfer_type_t type: 2;
        usb_endpoint_synchronization_t synchronization: 2;
        usb_endpoint_usage_t usage: 2;
        uint8_t reserved_6_7: 2;
    } __attribute__((packed)) attributes;
    struct {
        uint16_t max_size: 11;
        usb_endpoint_transactions_t transactions: 2;
        uint8_t reserved_13_15: 3;
    } __attribute__((packed)) packet;
    uint8_t interval;
} __attribute__((packed)) usb_endpoint_descriptor_t;

typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint16_t data[];
} __attribute__((packed)) usb_string_descriptor_t;

struct usb_device {
    uint32_t number;
    usb_speed_t speed;
    usb_device_status_t status;
    volatile uint8_t configuration_index;
    uint8_t port_number;
    volatile usb_transfer_error_t error __attribute__((aligned(4)));

    void (*device_detached)(struct usb_device *device) __attribute__((aligned(4)));
    void (*device_deallocate)(struct usb_device *device);
    void (*device_check_for_change)(struct usb_device *device);
    void (*device_child_detached)(struct usb_device *device, struct usb_device *child);
    int (*device_child_reset)(struct usb_device *device, struct usb_device *child);
    int (*device_check_connection)(struct usb_device *device, struct usb_device *child);

    volatile usb_device_descriptor_t descriptor __attribute__((aligned(4)));
    volatile usb_configuration_descriptor_t configuration __attribute__((aligned(4)));
    volatile usb_interface_descriptor_t interfaces[MAX_INTERFACES_PER_DEVICE] __attribute__((aligned(4)));
    volatile usb_endpoint_descriptor_t endpoints[MAX_INTERFACES_PER_DEVICE][MAX_ENDPOINTS_PER_INTERFACE] __attribute__((aligned(4)));
    struct usb_device *parent __attribute((aligned(4)));
    volatile void *full_configuration;
    volatile usb_driver_data_header_t *driver_data;
    volatile uint32_t last_transfer;
};
typedef struct usb_device usb_device_t;

typedef struct {
    usb_packet_size_t max_size: 2;
    usb_speed_t speed: 2;
    uint8_t endpoint: 4;
    uint8_t device: 8;
    usb_transfer_type_t type: 2;
    usb_direction_t direction: 1;
    uint16_t reserved_19_31: 13;
} __attribute__((packed)) usb_pipe_address_t;

typedef enum {
    USB_DEVICE_REQUEST_GET_STATUS = 0,
    USB_DEVICE_REQUEST_CLEAR_FEATURE = 1,
    USB_DEVICE_REQUEST_SET_FEATURE = 3,
    USB_DEVICE_REQUEST_SET_ADDRESS = 5,
    USB_DEVICE_REQUEST_GET_DESCRIPTOR = 6,
    USB_DEVICE_REQUEST_SET_DESCRIPTOR = 7,
    USB_DEVICE_REQUEST_GET_CONFIGURATION = 8,
    USB_DEVICE_REQUEST_SET_CONFIGURATION = 9,
    USB_DEVICE_REQUEST_GET_INTERFACE = 10,
    USB_DEVICE_REQUEST_SET_INTERFACE = 11,
    USB_DEVICE_REQUEST_SYNCH_FRAME = 12,
    // HID requests
    USB_HID_REQUEST_GET_REPORT = 1,
    USB_HID_REQUEST_GET_IDLE = 2,
    USB_HID_REQUEST_GET_PROTOCOL = 3,
    USB_HID_REQUEST_SET_REPORT = 9,
    USB_HID_REQUEST_SET_IDLE = 10,
    USB_HID_REQUEST_SET_PROTOCOL = 11
} usb_device_request_request_t;

typedef struct {
    uint8_t type;
    usb_device_request_request_t request: 8;
    uint16_t value;
    uint16_t index;
    uint16_t length;
} __attribute__((packed)) usb_device_request_t;

inline usb_packet_size_t packet_size_from_number(uint32_t size)
{
    if (size <= 8) return USB_PACKET_SIZE_8_BIT;
    if (size <= 16) return USB_PACKET_SIZE_16_BIT;
    if (size <= 32) return USB_PACKET_SIZE_32_BIT;
    return USB_PACKET_SIZE_64_BIT;
}

inline uint32_t packet_size_to_number(usb_packet_size_t size)
{
    if (size == USB_PACKET_SIZE_8_BIT) return 8;
    if (size == USB_PACKET_SIZE_8_BIT) return 16;
    if (size == USB_PACKET_SIZE_8_BIT) return 32;
    return 64;
}

void usb_init();
