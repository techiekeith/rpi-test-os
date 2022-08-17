/*
 * hid.h
 */

#pragma once

#include "../../../common/stddef.h"
#include "../../../common/stdint.h"
#include "../usb.h"
#include "hid_report.h"

/* The DeviceDriver field in UsbDriverDataHeader for human interface devices (HIDs). ASCII 'HID0' (big-endian). */
#define DEVICE_DRIVER_HID       0x48494430

typedef enum {
    COUNTRY_NOT_SUPPORTED = 0,
    ARABIC = 1,
    BELGIAN = 2,
    CANADIAN_BILINGUAL = 3,
    CANADIAN_FRENCH = 4,
    CZECH_REPUBLIC = 5,
    DANISH = 6,
    FINNISH = 7,
    FRENCH = 8,
    GERMAN = 9,
    GREEK = 10,
    HEBREW = 11,
    HUNGARY = 12,
    INTERNATIONAL = 13,
    ITALIAN = 14,
    JAPAN = 15,
    KOREAN = 16,
    LATIN_AMERICAN = 17,
    DUTCH = 18,
    NORWEGIAN = 19,
    PERSIAN = 20,
    POLAND = 21,
    PORTUGUESE = 22,
    RUSSIAN = 23,
    SLOVAKIAN = 24,
    SPANISH = 25,
    SWEDISH = 26,
    SWISS_FRENCH = 27,
    SWISS_GERMAN = 28,
    SWITZERLAND = 29,
    TAIWAN = 30,
    TURKISH_Q = 31,
    ENGLISH_UK = 32,
    ENGLISH_US = 33,
    YUGOSLAVIAN = 34,
    TURKISH_F = 35
} hid_country_t;

/**
	\brief The human interface device descriptor information.

	The hid descriptor structure defined in the USB HID 1.11 manual in 6.2.1.
*/
typedef struct {
    uint8_t descriptor_length;
    descriptor_type_t descriptor_type: 8;
    uint16_t hid_version;
    hid_country_t country_code: 8;
    uint8_t descriptor_count;
    struct {
        descriptor_type_t type: 8;
        uint16_t length;
    } __attribute__ ((packed)) optional_descriptors[];
} __attribute__ ((packed)) hid_descriptor_t;

/**
	\brief Hid specific data.

	The contents of the driver data field for hid devices. Chains to
	allow a stacked driver.
*/
typedef struct {
    usb_driver_data_header_t header;
    hid_descriptor_t *descriptor;
    hid_parser_result_t *parser_result;
    usb_driver_data_header_t *driver_data;

    // HID event handlers
    void (*hid_detached)(usb_device_t *device);
    void (*hid_deallocate)(usb_device_t *device);
} hid_device_t;

/**
	\brief Methods to attach an interface of particular hid desktop usage.

	The application desktop usage of the interface is the index into this array
	of methods. The array is populated by ConfigurationLoad().
*/
typedef usb_call_result_t (*hid_attach_f)(usb_device_t *device, uint32_t interface_number);

/**
	\brief Methods to attach an interface of particular hid desktop usage.

	The application desktop usage of the interface is the index into this array
	of methods. The array is populated by ConfigurationLoad().
*/
void hid_usage_attach(uint16_t hid_usage_page, hid_attach_f attach_function);

/**
	\brief Retrieves a hid report.

	Performs a hid get report request as defined in  in the USB HID 1.11 manual
	in 7.2.1.
*/
usb_call_result_t hid_get_report(usb_device_t *device, hid_report_type_t report_type, uint8_t report_id,
                                 uint8_t interface, size_t buffer_length, void *buffer);

/**
	\brief Sends a hid report.

	Performs a hid set report request as defined in  in the USB HID 1.11 manual
	in 7.2.2.
*/
usb_call_result_t hid_set_report(usb_device_t *device, hid_report_type_t report_type, uint8_t report_id,
                                 uint8_t interface, size_t buffer_length, void *buffer);

/**
	\brief Updates the device with the values of a report.

	Writes back the current values of a report in memory to the device.
	Implemented using HidSetReport, not interrupts.
*/
usb_call_result_t hid_write_device(usb_device_t *device, uint8_t report);

/**
	\brief Updates a report with the values from the device.

	Reads the current values of a report from the device into memory. Implemented
	using HidGetReport not interrupts.
*/
usb_call_result_t hid_read_device(usb_device_t *device, uint8_t report);

/**
	\brief Enumerates a device as a HID device.

	Checks a device to see if it appears to be a HID device, and, if so, loads
	its hid and report descriptors to see what it can do.
*/
usb_call_result_t hid_attach(usb_device_t *device, uint32_t interface_number);
