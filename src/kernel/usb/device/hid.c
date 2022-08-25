/*
 * hid.c
 */

#include "../../../../include/common/stdbool.h"
#include "../../../../include/common/stdint.h"
#include "../../../../include/kernel/heap.h"
#include "../../../../include/kernel/io.h"
#include "../../../../include/kernel/usb/usb.h"
#include "../../../../include/kernel/usb/device/hid.h"

DEBUG_INIT("hid");

#define HID_MESSAGE_TIMEOUT     10

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
    debug_printf("Attaching driver for usage page %d: %p.\r\n", hid_usage_page, attach_function);
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

    usb_pipe_address_t pipe = {
        .type = USB_TRANSFER_TYPE_CONTROL,
        .speed = device->speed,
        .endpoint = 0,
        .device = device->number,
        .direction = DEVICE_TO_HOST,
        .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
        .request = USB_HID_REQUEST_GET_REPORT,
        .type = 0xa1, // XXX MAGIC
        .index = interface,
        .value = (uint16_t)report_type << 8 | report_id,
        .length = buffer_length
    };
    usb_call_result_t result = usb_control_message(device, pipe, buffer, buffer_length, &request, HID_MESSAGE_TIMEOUT);

    DEBUG_END();
    return result;
}

usb_call_result_t hid_set_report(usb_device_t *device, hid_report_type_t report_type, uint8_t report_id,
                                 uint8_t interface, uint32_t buffer_length, void *buffer)
{
    DEBUG_START("hid_set_report");

    usb_pipe_address_t pipe = {
            .type = USB_TRANSFER_TYPE_CONTROL,
            .speed = device->speed,
            .endpoint = 0,
            .device = device->number,
            .direction = HOST_TO_DEVICE,
            .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
            .request = USB_HID_REQUEST_SET_REPORT,
            .type = 0x21, // XXX MAGIC
            .index = interface,
            .value = (uint16_t)report_type << 8 | report_id,
            .length = buffer_length
    };
    usb_call_result_t result = usb_control_message(device, pipe, buffer, buffer_length, &request, HID_MESSAGE_TIMEOUT);

    DEBUG_END();
    return result;
}

static void bit_set(void *buffer, uint32_t offset, uint32_t length, uint32_t value)
{
    // TODO
}

static uint32_t bit_get_unsigned(void *buffer, uint32_t offset, uint32_t length)
{
    // TODO
    return 0;
}

static int32_t bit_get_signed(void *buffer, uint32_t offset, uint32_t length)
{
    // TODO
    return 0;
}

usb_call_result_t hid_read_device(usb_device_t *device, uint8_t report_number)
{
    DEBUG_START("hid_read_device");

    hid_device_t *data = (hid_device_t *)device->driver_data;
    hid_parser_result_t *parse = data->parser_result;
    hid_parser_report_t *report = parse->report[report_number];
    uint32_t size = ((report->report_length + 7) / 8);
    usb_call_result_t result;

    if ((report->report_buffer == NULL)
        && ((report->report_buffer = (uint8_t *)heap_alloc("hid_read_device", size)) == NULL))
    {
        DEBUG_END();
        return ERROR_MEMORY;
    }

    if ((result = hid_get_report(device, report->type, report->id, data->parser_result->interface,
                                 size, report->report_buffer)) != OK)
    {
        if (result != ERROR_DISCONNECTED)
            debug_printf("HID: Could not read %s report %d.\r\n", usb_get_description(device), report);
        DEBUG_END();
        return result;
    }

    for (uint32_t i = 0; i < report->field_count; i++)
    {
        hid_parser_field_t *field = &report->fields[i];
        if (field->attributes.variable)
        {
            if (field->logical_minimum < 0)
            {
                field->value.int32 = bit_get_signed(report->report_buffer, field->offset, field->size);
            }
            else
            {
                field->value.uint32 = bit_get_unsigned(report->report_buffer, field->offset, field->size);
            }
        }
        else
        {
            for (uint32_t j = 0; j < field->count; j++)
            {
                uint32_t relative_offset = j * field->size;
                uint32_t offset = field->offset + relative_offset;
                bit_set(field->value.pointer, relative_offset, field->size,
                        (field->logical_minimum < 0) ? bit_get_signed(report->report_buffer, offset, field->size)
                                : bit_get_unsigned(report->report_buffer, offset, field->size));
            }
        }
    }

    DEBUG_END();
    return OK;
}

usb_call_result_t hid_write_device(usb_device_t *device, uint8_t report_number)
{
    DEBUG_START("hid_write_device");

    hid_device_t *data = (hid_device_t *)device->driver_data;
    hid_parser_result_t *parse = data->parser_result;
    hid_parser_report_t *report = parse->report[report_number];
    uint32_t size = ((report->report_length + 7) / 8);
    usb_call_result_t result;

    if ((report->report_buffer == NULL)
        && ((report->report_buffer = (uint8_t *)heap_alloc("hid_read_device", size)) == NULL))
    {
        DEBUG_END();
        return ERROR_MEMORY;
    }

    for (uint32_t i = 0; i < report->field_count; i++)
    {
        hid_parser_field_t *field = &report->fields[i];
        if (field->attributes.variable)
        {
            bit_set(report->report_buffer, field->offset, field->size, field->value.int32);
        }
        else
        {
            for (uint32_t j = 0; j < field->count; j++)
            {
                uint32_t relative_offset = j * field->size;
                uint32_t offset = field->offset + relative_offset;
                bit_set(report->report_buffer, offset, field->size,
                        bit_get_signed(field->value.pointer, relative_offset, field->size));
            }
        }
    }

    if ((result = hid_set_report(device, report->type, report->id, data->parser_result->interface,
                                 size, report->report_buffer)) != OK)
    {
        if (result != ERROR_DISCONNECTED)
            debug_printf("HID: Could not write %s report %d.\r\n", usb_get_description(device), report);
        DEBUG_END();
        return result;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t hid_set_protocol(usb_device_t *device, uint8_t interface, uint16_t protocol)
{
    DEBUG_START("hid_set_protocol");

    usb_pipe_address_t pipe = {
        .type = USB_TRANSFER_TYPE_CONTROL,
        .speed = device->speed,
        .endpoint = 0,
        .device = device->number,
        .direction = HOST_TO_DEVICE,
        .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
        .request = USB_HID_REQUEST_SET_PROTOCOL,
        .type = 0x21, // XXX MAGIC
        .index = interface,
        .value = protocol,
        .length = 0
    };
    usb_call_result_t result = usb_control_message(device, pipe, NULL, 0, &request, HID_MESSAGE_TIMEOUT);

    DEBUG_END();
    return result;
}

static usb_call_result_t hid_parse_report_descriptor(usb_device_t *device, void *descriptor, uint16_t length)
{
    DEBUG_START("hid_parse_report_descriptor");

    debug_printf("TODO\r\n"); // TODO

    DEBUG_END();
    return ERROR_NOT_IMPLEMENTED;
}

static void hid_detached(usb_device_t *device)
{
    if (device->driver_data != NULL)
    {
        hid_device_t *data = (hid_device_t *)device->driver_data;
        if (data->hid_detached != NULL)
        {
            data->hid_detached(device);
        }
    }
}

static void hid_deallocate(usb_device_t *device)
{
    if (device->driver_data != NULL)
    {
        hid_device_t *data = (hid_device_t *)device->driver_data;
        if (data->hid_deallocate != NULL) data->hid_deallocate(device);
        if (data->parser_result != NULL)
        {
            for (uint32_t i = 0; i < data->parser_result->report_count; i++)
            {
                if (data->parser_result->report[i] != NULL)
                {
                    hid_parser_report_t *report = data->parser_result->report[i];
                    if (report->report_buffer != NULL) heap_free(report->report_buffer);
                    for (uint32_t j = 0; j < report->field_count; j++)
                    {
                        if (!report->fields[j].attributes.variable) heap_free(report->fields[j].value.pointer);
                    }
                    heap_free(data->parser_result->report[i]);
                }
            }
            heap_free(data->parser_result);
        }
        heap_free(data);
    }
    device->device_deallocate = NULL;
    device->device_detached = NULL;
}

usb_call_result_t hid_attach(usb_device_t *device, uint32_t interface_number)
{
    DEBUG_START("hid_attach");

    hid_device_t *data;
    hid_descriptor_t *descriptor;
    usb_descriptor_header_t *header;
    void *report_descriptor = NULL;
    usb_call_result_t result;
    uint32_t current_interface;

    if (device->interfaces[interface_number].interface_class != INTERFACE_CLASS_HID)
    {
        DEBUG_END();
        return ERROR_ARGUMENT;
    }

    if (device->interfaces[interface_number].endpoint_count < 1)
    {
        debug_printf("HID: Invalid HID device with no endpoints.\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    if (device->endpoints[interface_number][0].endpoint_address.direction != DEVICE_TO_HOST
        || device->endpoints[interface_number][0].attributes.type != USB_TRANSFER_TYPE_INTERRUPT)
    {
        debug_printf("HID: Invalid HID device with unusual endpoints (0).\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    if ((device->interfaces[interface_number].endpoint_count < 1)
        && ((device->endpoints[interface_number][0].endpoint_address.direction != DEVICE_TO_HOST
            || device->endpoints[interface_number][0].attributes.type != USB_TRANSFER_TYPE_INTERRUPT)))
    {
        debug_printf("HID: Invalid HID device with unusual endpoints (1).\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    if (device->status != USB_DEVICE_STATUS_CONFIGURED)
    {
        debug_printf("HID: Cannot start driver on unconfigured device!\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }

    if (device->interfaces[interface_number].sub_class == 1)
    {
        if (device->interfaces[interface_number].protocol == 1)
            debug_printf("HID: Boot keyboard detected.\r\n");
        else if (device->interfaces[interface_number].protocol == 1)
            debug_printf("HID: Boot mouse detected.\r\n");
        else
            debug_printf("HID: Unknown boot device detected.\r\n");
        debug_printf("HID: Reverting from boot mode to normal HID mode.\r\n");
        if ((result = hid_set_protocol(device, interface_number, 1)) != OK)
        {
            debug_printf("HID: Could not revert from boot mode to normal HID mode.\r\n");
            DEBUG_END();
            return result;
        }
    }

    header = (usb_descriptor_header_t *)device->full_configuration;
    descriptor = NULL;
    current_interface = interface_number + 1;
    do {
        if (header->descriptor_length == 0) break;
        switch (header->descriptor_type)
        {
            case DESCRIPTOR_TYPE_INTERFACE:
                current_interface = ((usb_interface_descriptor_t *)header)->number;
                break;
            case DESCRIPTOR_TYPE_HID:
                if (current_interface == interface_number)
                    descriptor = (void *)header;
                break;
            default:
                break;
        }

        debug_printf("HID: Descriptor %d length %d, interface %d.\r\n",
                     header->descriptor_type, header->descriptor_length, current_interface);

        if (descriptor != NULL) break;
        header = (void *)((uint8_t *)header + header->descriptor_length);
    } while(true);

    if (descriptor == NULL)
    {
        debug_printf("HID: No HID descriptor in %s.Interface%d. Cannot be a HID device.\r\n",
                     usb_get_description(device), interface_number + 1);
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    if (descriptor->hid_version > 0x111)
    {
        debug_printf("HID: Device uses unsupported HID version %x.%x.\r\n",
                     descriptor->hid_version >> 8, descriptor->hid_version & 0xff);
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    device->device_deallocate = hid_deallocate;
    device->device_detached = hid_detached;
    if ((device->driver_data = heap_alloc("hid_attach", sizeof(hid_device_t))) == NULL)
    {
        hid_deallocate(device);
        DEBUG_END();
        return ERROR_MEMORY;
    }
    data = (hid_device_t *)device->driver_data;
    data->descriptor = descriptor;
    data->driver_data = NULL;

    uint16_t report_descriptor_length = descriptor->optional_descriptors[0].length;
    if ((report_descriptor = heap_alloc("hid_attach", report_descriptor_length)) == NULL)
    {
        hid_deallocate(device);
        DEBUG_END();
        return ERROR_MEMORY;
    }
    if ((result = usb_get_descriptor(device, DESCRIPTOR_TYPE_HID_REPORT, 0, interface_number, report_descriptor,
                                     report_descriptor_length, report_descriptor_length, 1)) != OK)
    {
        debug_printf("HID: Could not read report descriptor for %s.Interface%d.\r\n",
                     usb_get_description(device), interface_number + 1);
        heap_free(report_descriptor);
        hid_deallocate(device);
        DEBUG_END();
        return result;
    }
    if ((result = hid_parse_report_descriptor(device, report_descriptor, report_descriptor_length)) != OK)
    {
        debug_printf("HID: Invalid report descriptor for %s.Interface%d.\r\n",
                     usb_get_description(device), interface_number + 1);
        heap_free(report_descriptor);
        hid_deallocate(device);
        DEBUG_END();
        return result;
    }

    heap_free(report_descriptor);
    report_descriptor = NULL;

    data->parser_result->interface = interface_number;
    if (data->parser_result->application.page == HID_UP_GENERIC_DESKTOP_CONTROL
        && (uint16_t)data->parser_result->application.desktop < HID_USAGE_ATTACH_COUNT
        && hid_usage_classes[(uint16_t)data->parser_result->application.desktop] != NULL)
    {
        hid_usage_classes[(uint16_t)data->parser_result->application.desktop](device, interface_number);
    }

    DEBUG_END();
    return OK;
}
