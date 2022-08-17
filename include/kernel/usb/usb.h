/*
 * usb.h - USB device related definitions
 */

#pragma once

#include "../../common/stdbool.h"
#include "../../common/stdint.h"
#include "../peripheral.h"

/**
	\brief The maximum number of children a device could have, by implication, this is
	the maximum number of ports a hub supports.

	This is theoretically 255, as 8 bits are used to transfer the port count in
	a hub descriptor. Practically, no hub has more than 10, so we instead allow
	that many. Increasing this number will waste space, but will not have
	adverse consequences up to 255. Decreasing this number will save a little
	space in the HubDevice structure, at the risk of removing support for an
	otherwise valid hub.
*/
#define MAX_CHILDREN_PER_DEVICE         10

/**
	\brief The maximum number of interfaces a device configuration could have.

	This is theoretically 255 as one byte is used to transfer the interface
	count in a configuration descriptor. In practice this is unlikely, so we
	allow an arbitrary 8. Increasing this number wastes (a lot) of space in
	every device structure, but should not have other consequences up to 255.
	Decreasing this number reduces the overheads of the UsbDevice structure, at
	the cost of possibly rejecting support for an otherwise supportable device.
*/
#define MAX_INTERFACES_PER_DEVICE       8

/**
	\brief The maximum number of endpoints a device could have (per interface).

	This is theoretically 16, as four bits are used to transfer the endpoint
	number in certain device requests. This is possible in practice, so we
	allow that many. Decreasing this number reduces the space in each device
	structure considerably, while possible removing support for otherwise valid
	devices. This number should not be greater than 16.
*/
#define MAX_ENDPOINTS_PER_INTERFACE     16

/** The maximum number of devices that can be connected. */
#define MAXIMUM_USB_DEVICES             32

/** The default timeout in ms of control transfers. */
#define CONTROL_MESSAGE_TIMEOUT         10

/**
	\brief Result of a method call.

	Negative results are errors.
	OK is for a general success.
	ERROR_GENERAL is an undisclosed failure.
	ERROR_ARGUMENT is a bad input.
	ERROR_RETRY is a temporary issue that may disappear, the method should be rerun
		without modification (the caller is expected to limit number of retries as
		required).
	ERROR_DEVICE is a more permanent hardware error (a reset procedure should be
		enacted before retrying).
	ERROR_INCOMPATIBLE is a device driver that will not support the detected
		device.
	ERROR_COMPILER is a problem with the configuration of the compiler generating
		unusable code.
	ERROR_MEMORY is used when the memory is exhausted.
	ERROR_TIMEOUT is used when a maximum delay is reached when waiting and an
		operation is unfinished. This does not necessarily mean the operation
		will not finish, just that it is unreasonably slow.
	ERROR_DISCONNECTED is used when a device is disconnected in transfer.
*/
typedef enum {
    OK = 0,
    ERROR_GENERAL = -1,
    ERROR_ARGUMENT = -2,
    ERROR_RETRY = -3,
    ERROR_DEVICE = -4,
    ERROR_INCOMPATIBLE = -5,
    ERROR_COMPILER = -6,
    ERROR_MEMORY = -7,
    ERROR_TIMEOUT = -8,
    ERROR_DISCONNECTED = -9,
    ERROR_NOT_IMPLEMENTED = -10
} usb_call_result_t;

/**
	\brief Transfer type in USB communication.

	Many and various parts of the USB standard use this 2 bit field to indicate
	in what type of transaction to use.
*/
typedef enum {
    USB_TRANSFER_TYPE_CONTROL = 0,
    USB_TRANSFER_TYPE_ISOCHRONOUS = 1,
    USB_TRANSFER_TYPE_BULK = 2,
    USB_TRANSFER_TYPE_INTERRUPT = 3
} usb_transfer_type_t;

/**
	\brief Direction of USB communication.

	Many and various parts of the USB standard use this 1 bit field to indicate
	in which direction information flows.
*/
typedef enum {
    /* Host to device, sometimes "out". */
    HOST_TO_DEVICE = 0,
    /* Device to host, sometimes "in". */
    DEVICE_TO_HOST = 1
} usb_direction_t;

/**
	\brief Transfer size in USB communication.

	Many and various parts of the USB standard use this 2 bit field to indicate
	in what size of transaction to use.
*/
typedef enum {
    USB_PACKET_SIZE_8_BIT = 0,
    USB_PACKET_SIZE_16_BIT = 1,
    USB_PACKET_SIZE_32_BIT = 2,
    USB_PACKET_SIZE_64_BIT = 3
} usb_packet_size_t;

/**
	\brief Speed of USB communication.

	Many and various parts of the USB standard use this 2 bit field to indicate
	in which direction information flows.
*/
typedef enum {
    USB_SPEED_HIGH = 0,
    USB_SPEED_FULL = 1,
    USB_SPEED_LOW = 2
} usb_speed_t;

/**
	\brief Status of a USB device.

	Stores the status of a USB device. Statuses as defined in 9.1 of the USB2.0
	manual.
*/
typedef enum {
    USB_DEVICE_STATUS_ATTACHED,
    USB_DEVICE_STATUS_POWERED,
    USB_DEVICE_STATUS_DEFAULT,
    USB_DEVICE_STATUS_ADDRESSED,
    USB_DEVICE_STATUS_CONFIGURED
} usb_device_status_t;

/**
	\brief Status of a USB transfer.

	Stores the status of the last transfer a USB device did.
*/
typedef enum {
    USB_TRANSFER_OK = 0,
    USB_TRANSFER_ERROR_STALL = 1 << 1,
    USB_TRANSFER_ERROR_BUFFER_ERROR = 1 << 2,
    USB_TRANSFER_ERROR_BABBLE = 1 << 3,
    USB_TRANSFER_ERROR_NO_ACKNOWLEDGE = 1 << 4,
    USB_TRANSFER_ERROR_CRC_ERROR = 1 << 5,
    USB_TRANSFER_ERROR_BIT_ERROR = 1 << 6,
    USB_TRANSFER_ERROR_CONNECTION_ERROR = 1 << 7,
    USB_TRANSFER_ERROR_AHB_ERROR = 1 << 8,
    USB_TRANSFER_ERROR_NOT_YET_ERROR = 1 << 9,
    USB_TRANSFER_PROCESSING = 1 << 31
} usb_transfer_error_t;

/**
	\brief The descriptor type field from the header of USB descriptors.

	The descriptor type in the header of all USB descriptor sturctures defined
	in the USB 2.0 manual in 9.6.
*/
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

/**
	\brief The device descriptor information.

	The device descriptor structure defined in the USB 2.0 manual in 9.6.1.
*/
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

/**
	\brief Start of a device specific data field.

	The first two words of driver data in a UsbDevice. The  DeviceDriver field
	is a code which uniquely identifies the driver that set the driver data
	field (i.e. the lowest driver in the stack above the USB driver). The
	DataSize is the size in bytes of the device specific data field.
*/
typedef struct {
    uint32_t device_driver;
    uint32_t data_size;
} usb_driver_data_header_t;

/**
	\brief The header of USB descriptor information.

	The header of all USB descriptor sturctures defined in the USB 2.0 manual
	in 9.6.
*/
typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
} __attribute__((packed)) usb_descriptor_header_t;

/**
	\brief The device descriptor information.

	The device descriptor structure defined in the USB 2.0 manual in 9.6.1.
*/
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

/**
	\brief The device qualifier descriptor information.

	The device descriptor qualifier sturcture defined in the USB 2.0 manual in
	9.6.2.
*/
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

/**
	\brief The configuration descriptor information.

	The configuration descriptor structure defined in the USB2.0 manual section
	9.6.3.
*/
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
	\brief The other speed configuration descriptor.

	The other speed configuration descriptor defined in the USB2.0 manual section
	9.6.4.

    usb_other_speed_configuration_descriptor_t seems to be identical to usb_configuration_descriptor_t
    save that in AC's code the 'reserved_7' attribute bit has an enum with a definition of 'Valid = 1'.
 */
typedef usb_configuration_descriptor_t usb_other_speed_configuration_descriptor_t;

/**
	\brief The interface descriptor information.

	The interface descriptor structure defined in the USB2.0 manual section
	9.6.5.
*/
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

/**
	\brief The endpoint descriptor information.

	The endpoint descriptor structure defined in the USB2.0 manual section
	9.6.6.
*/
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

/**
	\brief The string descriptor information.

	The string descriptor structure defined in the USB2.0 manual section
	9.6.7.
*/
typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint16_t data[];
} __attribute__((packed)) usb_string_descriptor_t;

/**
	\brief Structure to store the details of a USB device that has been
	detectd.

	Stores the details about a connected USB device. This is not directly part
	of the USB standard, and is instead a mechanism used to control the device
	tree.
*/
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
    usb_call_result_t (*device_child_reset)(struct usb_device *device, struct usb_device *child);
    usb_call_result_t (*device_check_connection)(struct usb_device *device, struct usb_device *child);

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
    if (size == USB_PACKET_SIZE_16_BIT) return 16;
    if (size == USB_PACKET_SIZE_32_BIT) return 32;
    return 64;
}

/**
	\brief Method to attach a particular interface for a particular class.
*/
typedef usb_call_result_t (*usb_attach_f)(usb_device_t *device, uint32_t interface_number);

/**
	\brief Method to attach a particular interface for a particular class.
*/
void attach_driver_for_class(interface_class_t interface_class, usb_attach_f function);

/**
	\brief Allocates memory to a new device.

	Sets the value in the parameter device to the address of a new device
	allocated on the heap, which then has appropriate default values.
*/
usb_call_result_t usb_allocate_device(usb_device_t **devicep);

/**
	\brief Recursively enumerates a new device.

	Recursively enumerates a new device that has been allocated. This assigns
	an address, determines what the device is, and, if it is a hub, will
	configure the device recursively look for new devices. If not, it will
	configure the device with the default configuration.
*/
usb_call_result_t usb_attach_device(usb_device_t *device);

/**
	\brief Sends a control message synchronously to a given device.

	Sends a contorl message synchronously to a given device, then waits for
	completion. If the timeout is reached returns ErrorTimeout. This puts
	device into an inconsistent state, so best not to use it until processing
	is unset.
*/
usb_call_result_t usb_control_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer,
                                      uint32_t buffer_length, usb_device_request_t *request, uint32_t timeout);

/*
	\brief Deallocates the memory and resources of a USB device.

	Recursively deallocates the device and all children. Deallocates any class
	specific data, as well as the device structure itself and releases the
	device number.
*/
void usb_deallocate_device(usb_device_t *device);

/**
	\brief Returns a description for a device.

	Returns a description for a device. This is not read from the device, this
	is just generated given by the driver.
*/
const char *usb_get_description(usb_device_t *device);

/**
	\brief Gets the descriptor for a given device.

	Gets the descriptor for a given device, using the index and language id
	specified. The returned value is not longer than length.
*/
usb_call_result_t usb_get_descriptor(usb_device_t *device, descriptor_type_t descriptor_type, uint8_t index,
                                     uint16_t lang_id, void *buffer, uint32_t length, uint32_t minimum_length,
                                     uint8_t recipient);

/**
	\brief Performs all necessary operationg to start the USB driver.

	Initialises the USB driver by performing necessary interfactions with the
	host controller driver, and enumerating the device tree.
*/
usb_call_result_t usb_init();

/**
	\brief Returns a pointer to the root hub device.

	On a Universal Serial Bus, there exists a root hub. This if often a virtual
	device, and typically represents a one port hub, which is the physical
	universal serial bus for this computer. It is always address 1. It is
	present to allow uniform software manipulation of the universal serial bus
	itself.
*/
usb_device_t *usb_get_root_hub();

/**
	\brief Scans the entire USB tree for changes.

	Recursively calls HubCheckConnection on all ports on all hubs connected to
	the root hub.
*/
void usb_check_for_change();
