/*
 * usb_root_hub.c
 */

#include "../../../include/common/stdlib.h"
#include "../../../include/common/string.h"
#include "../../../include/kernel/io.h"
#include "../../../include/kernel/mmio.h"
#include "../../../include/kernel/system_timer.h"
#include "../../../include/kernel/usb/device/hub.h"
#include "../../../include/kernel/usb/usb_hcd.h"
#include "../../../include/kernel/usb/usb_root_hub.h"

DEBUG_INIT("usb_root_hub");

uint32_t root_hub_device_number;

static usb_device_descriptor_t root_hub_device_descriptor = {
    .descriptor_length = 0x12,
    .descriptor_type = DESCRIPTOR_TYPE_DEVICE,
    .usb_version = 0x0200,
    .device_class = DEVICE_CLASS_HUB,
    .sub_class = 0,
    .protocol = 0,
    .max_packet_size_0 = 8,
    .vendor_id = 0,
    .product_id = 0,
    .version = 0x100,
    .manufacturer = 0,
    .product = 1,
    .serial_number = 0,
    .configuration_count = 1,
};

static hub_configuration_descriptor_t root_hub_configuration_descriptor = {
    .configuration = {
        .descriptor_length = 9,
        .descriptor_type = DESCRIPTOR_TYPE_CONFIGURATION,
        .total_length = 0x19,
        .interface_count = 1,
        .configuration_value = 1,
        .string_index = 0,
        .attributes = {
            .remote_wakeup = false,
            .self_powered = true,
            .reserved_7 = true
        },
        .maximum_power = 0
    },
    .interface = {
        .descriptor_length = 9,
        .descriptor_type = DESCRIPTOR_TYPE_INTERFACE,
        .number = 0,
        .alternate_setting = 0,
        .endpoint_count = 1,
        .interface_class = INTERFACE_CLASS_HUB,
        .sub_class = 0,
        .protocol = 0,
        .string_index = 0
    },
    .endpoint = {
        .descriptor_length = 7,
        .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
        .endpoint_address = {
            .number = 1,
            .direction = DEVICE_TO_HOST
        },
        .attributes = {
            .type = USB_TRANSFER_TYPE_INTERRUPT
        },
        .packet = {
            .max_size = 8
        },
        .interval = 0xff
    }
};

usb_string_descriptor_t root_hub_string_0_descriptor = {
    .descriptor_length = 4,
    .descriptor_type = DESCRIPTOR_TYPE_STRING,
    .data = { 0x0409 }
};

usb_string_descriptor_t root_hub_string_1_descriptor = {
    .descriptor_length = sizeof(u"USB 2.0 Root Hub") + 2,
    .descriptor_type = DESCRIPTOR_TYPE_STRING,
    .data = u"USB 2.0 Root Hub"
};

usb_hub_descriptor_t root_hub_descriptor = {
    .descriptor_length = 9,
    .descriptor_type = DESCRIPTOR_TYPE_HUB,
    .port_count = 1,
    .attributes = {
        .power_switching_mode = HUB_PORT_CONTROL_GLOBAL,
        .compound = false,
        .over_current_protection = HUB_PORT_CONTROL_GLOBAL,
        .think_time = 0,
        .indicators = false
    },
    .power_good_delay = 0,
    .maximum_hub_power = 0,
    .data = { 0x01, 0xff }
};

usb_call_result_t hcd_process_root_hub_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer,
                                               uint32_t buffer_length, usb_device_request_t *request) {
    DEBUG_START("hcd_process_root_hub_message");

    device->error = USB_TRANSFER_PROCESSING;

    if (pipe.type == USB_TRANSFER_TYPE_INTERRUPT) {
        debug_printf("HCD.Hub: RootHub does not support IRQ pipes.\r\n");
        device->error = USB_TRANSFER_ERROR_STALL;
        DEBUG_END();
        return OK;
    }

    uint32_t reply_length = 0;
    usb_call_result_t result = OK;

    hcd_host_port_t host_port;
    hcd_power_t power;

    switch (request->request) {
        case USB_DEVICE_REQUEST_GET_STATUS:
            switch (request->type) {
                case 0x80: // ???
                    *(uint16_t *)buffer = 1;
                    reply_length = 2;
                    break;
                case 0x81: // interface
                case 0x82: // endpoint
                    *(uint16_t *)buffer = 0;
                    reply_length = 2;
                    break;
                case 0xa0: // class
                    *(uint16_t *)buffer = 0;
                    reply_length = 4;
                    break;
                case 0xa3:
                    mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                    *(uint32_t *)buffer = 0;
                    ((hub_port_full_status_t *)buffer)->status.connected = host_port.connect;
                    ((hub_port_full_status_t *)buffer)->status.enabled = host_port.enable;
                    ((hub_port_full_status_t *)buffer)->status.suspended = host_port.suspend;
                    ((hub_port_full_status_t *)buffer)->status.over_current = host_port.over_current;
                    ((hub_port_full_status_t *)buffer)->status.reset = host_port.reset;
                    ((hub_port_full_status_t *)buffer)->status.power = host_port.power;
                    if (host_port.speed == USB_SPEED_HIGH)
                        ((hub_port_full_status_t *)buffer)->status.high_speed_attached = true;
                    if (host_port.speed == USB_SPEED_LOW)
                        ((hub_port_full_status_t *)buffer)->status.low_speed_attached = true;
                    ((hub_port_full_status_t *)buffer)->status.test_mode = host_port.test_control;
                    ((hub_port_full_status_t *)buffer)->change.connected_changed = host_port.connect_detected;
                    ((hub_port_full_status_t *)buffer)->change.enabled_changed = host_port.enable_changed;
                    ((hub_port_full_status_t *)buffer)->change.over_current_changed = host_port.over_current_changed;
                    ((hub_port_full_status_t *)buffer)->change.reset_changed = true;
                    reply_length = 4;
                    break;
                default:
                    device->error = USB_TRANSFER_ERROR_STALL;
            }
            break;
        case USB_DEVICE_REQUEST_CLEAR_FEATURE:
            reply_length = 0;
            switch (request->type) {
                case 0x2:
                case 0x20:
                    break;
                case 0x23:
                    switch ((hcd_hub_port_feature_t) request->value) {
                        case HUB_PORT_FEATURE_ENABLE:
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.enable = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x4); // XXX MAGIC
                            break;
                        case HUB_PORT_FEATURE_SUSPEND:
                            mmio_write(HCD_POWER, NO_BITS);
                            system_timer_busy_wait(5000);
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.resume = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x40); // XXX MAGIC
                            system_timer_busy_wait(100000);
                            host_port.resume = false;
                            host_port.suspend = false;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0xc0); // XXX MAGIC
                            break;
                        case HUB_PORT_FEATURE_POWER:
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.power = false;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x1000); // XXX MAGIC
                            break;
                        case HUB_PORT_FEATURE_CONNECTION_CHANGE:
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.connect_detected = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x2); // XXX MAGIC
                            break;
                        case HUB_PORT_FEATURE_ENABLE_CHANGE:
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.enable_changed = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x8); // XXX MAGIC
                            break;
                        case HUB_PORT_FEATURE_OVER_CURRENT_CHANGE:
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.over_current_changed = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x20); // XXX MAGIC
                            break;
                        default:
                            // do nothing
                            break;
                    }
                    break;
                default:
                    result = ERROR_ARGUMENT;
            }
            break;
        case USB_DEVICE_REQUEST_SET_FEATURE:
            reply_length = 0;
            switch (request->type) {
                case 0x20:
                    break;
                case 0x23:
                    switch ((hcd_hub_port_feature_t) request->value) {
                        case HUB_PORT_FEATURE_RESET:
                            mmio_read_in(HCD_POWER, &power, 1);
                            power.enable_sleep_clock_gating = false;
                            power.stop_p_clock = false;
                            mmio_write_out(HCD_POWER, &power, 1);
                            mmio_write(HCD_POWER, NO_BITS);

                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.suspend = false;
                            host_port.reset = true;
                            host_port.power = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x1180); // XXX MAGIC
                            system_timer_busy_wait(6000);
                            host_port.reset = false;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x1000); // XXX MAGIC
                            break;
                        case HUB_PORT_FEATURE_POWER:
                            mmio_read_in(HCD_HOST_PORT, &host_port, 1);
                            host_port.power = true;
                            mmio_write_with_mask(HCD_HOST_PORT, &host_port, HCD_HOST_PORT_MASK | 0x1000); // XXX MAGIC
                            break;
                        default:
                            // do nothing
                            break;
                    }
                    break;
                default:
                    result = ERROR_ARGUMENT;
            }
            break;
        case USB_DEVICE_REQUEST_SET_ADDRESS:
            reply_length = 0;
            root_hub_device_number = request->value;
            break;
        case USB_DEVICE_REQUEST_GET_DESCRIPTOR:
            switch (request->type) {
                case 0x80:
                    switch ((descriptor_type_t)((request->value >> 8) & 0xff)) {
                        case DESCRIPTOR_TYPE_DEVICE:
                            reply_length = MIN(sizeof(root_hub_device_descriptor), buffer_length, uint32_t);
                            memcpy(buffer, &root_hub_device_descriptor, reply_length);
                            break;
                        case DESCRIPTOR_TYPE_CONFIGURATION:
                            reply_length = MIN(sizeof(root_hub_configuration_descriptor), buffer_length, uint32_t);
                            memcpy(buffer, &root_hub_configuration_descriptor, reply_length);
                            break;
                        case DESCRIPTOR_TYPE_STRING:
                            switch(request->value & 0xff) {
                                case 0x0:
                                    reply_length = MIN(root_hub_string_0_descriptor.descriptor_length,
                                                       buffer_length, uint32_t);
                                    memcpy(buffer, &root_hub_string_0_descriptor, reply_length);
                                    break;
                                case 0x1:
                                    reply_length = MIN(root_hub_string_1_descriptor.descriptor_length,
                                                       buffer_length, uint32_t);
                                    memcpy(buffer, &root_hub_string_1_descriptor, reply_length);
                                    break;
                                default:
                                    reply_length = 0;
                            }
                            break;
                        default:
                            result = ERROR_ARGUMENT;
                    }
                    break;
                case 0xa0:
                    reply_length = MIN(root_hub_descriptor.descriptor_length, buffer_length, uint32_t);
                    memcpy(buffer, &root_hub_descriptor, reply_length);
                    break;
                default:
                    result = ERROR_ARGUMENT;
            }
            break;
        case USB_DEVICE_REQUEST_GET_CONFIGURATION:
            *(uint8_t *)buffer = 0x1;
            reply_length = 1;
            break;
        case USB_DEVICE_REQUEST_SET_CONFIGURATION:
            reply_length = 0;
            break;
        default:
            result = ERROR_ARGUMENT;
    }

    if (result == ERROR_ARGUMENT) device->error |= USB_TRANSFER_ERROR_STALL;
    device->error &= ~USB_TRANSFER_PROCESSING;
    device->last_transfer = reply_length;

    DEBUG_END();
    return OK;
}
