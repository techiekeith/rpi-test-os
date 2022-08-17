/*
 * hid.c
 */

#include "../../../../include/common/stdbool.h"
#include "../../../../include/common/stdint.h"
#include "../../../../include/kernel/io.h"
#include "../../../../include/kernel/usb/usb.h"
#include "../../../../include/kernel/usb/device/hid.h"

DEBUG_INIT("hid");

#define HID_USAGE_ATTACH_COUNT  10
static hid_attach_f hid_usage_classes[HID_USAGE_ATTACH_COUNT];

void hid_load()
{
    DEBUG_START("hid_load");
    debug_printf("HID: HID driver version 0.1 (derived from CSUD).\r\n");
    attach_driver_for_class(INTERFACE_CLASS_HID, hid_attach);
    DEBUG_END();
}

void hid_usage_attach(uint16_t hid_usage_page, hid_attach_f attach_function)
{
    DEBUG_START("hid_usage_attach");
    if (hid_usage_page < HID_USAGE_ATTACH_COUNT)
        hid_usage_classes[hid_usage_page] = attach_function;
    else
        debug_printf("HID: Usage page %d >= %d, can't attach driver.\r\n", hid_usage_page, HID_USAGE_ATTACH_COUNT);
    DEBUG_END();
}

usb_call_result_t hid_get_report(usb_device_t *device, hid_report_type_t report_type, uint8_t report_id,
                                 uint8_t interface, uint32_t buffer_length, void *buffer)
{
    DEBUG_START("hid_get_report");

    debug_printf("HID: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

usb_call_result_t hid_set_report(usb_device_t *device, hid_report_type_t report_type, uint8_t report_id,
                                 uint8_t interface, uint32_t buffer_length, void *buffer)
{
    DEBUG_START("hid_get_report");

    debug_printf("HID: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

usb_call_result_t hid_write_device(usb_device_t *device, uint8_t report)
{
    DEBUG_START("hid_write_device");

    debug_printf("HID: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

usb_call_result_t hid_read_device(usb_device_t *device, uint8_t report)
{
    DEBUG_START("hid_read_device");

    debug_printf("HID: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

usb_call_result_t hid_attach(usb_device_t *device, uint32_t interface_number)
{
    DEBUG_START("hid_attach");

    debug_printf("HID: TODO.\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}
