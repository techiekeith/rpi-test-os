/*
 * usb.c - USB driver
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/common/utf8.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/usb.h"
#include "../../include/kernel/usb_hcd.h"

DEBUG_INIT("usb");

static const char device_speeds[4][16] = { "480 Mb/s", "1.5 Mb/s", "12 Mb/s", "unknown speed" };

static usb_device_t *devices[MAXIMUM_USB_DEVICES];
static usb_call_result_t (*interface_class_attach[INTERFACE_CLASS_ATTACH_COUNT])(usb_device_t *device, uint32_t interface_number);

const char *usb_get_description(usb_device_t *device)
{
    if (device->status == USB_DEVICE_STATUS_ATTACHED)
        return "New Device (not ready)";
    if (device->status == USB_DEVICE_STATUS_POWERED)
        return "Unknown Device (not ready)";
    if (device == devices[0])
        return "USB Root Hub";

    switch (device->descriptor.device_class)
    {
        case DEVICE_CLASS_HUB:
            if (device->descriptor.usb_version == 0x210)
                return "USB 2.1 Hub";
            if (device->descriptor.usb_version == 0x200)
                return "USB 2.0 Hub";
            if (device->descriptor.usb_version == 0x110)
                return "USB 1.1 Hub";
            if (device->descriptor.usb_version == 0x110)
                return "USB 1.0 Hub";
            return "USB Hub";
        case DEVICE_CLASS_VENDOR_SPECIFIC:
            if (device->descriptor.vendor_id == 0x424 && device->descriptor.product_id == 0xec00)
                return "SMSC LAN9512";
            return "Vendor Specific";
        case DEVICE_CLASS_IN_INTERFACE:
            if (device->status == USB_DEVICE_STATUS_CONFIGURED)
            {
                switch (device->interfaces[0].interface_class)
                {
                    case INTERFACE_CLASS_AUDIO:
                        return "USB Audio Device";
                    case INTERFACE_CLASS_COMMUNICATIONS:
                        return "USB CDC Device";
                    case INTERFACE_CLASS_HID:
                        switch (device->interfaces[0].protocol)
                        {
                            case 1:
                                return "USB Keyboard";
                            case 2:
                                return "USB Mouse";
                            default:
                                return "USB HID";
                        }
                    case INTERFACE_CLASS_PHYSICAL:
                        return "USB Physical Device";
                    case INTERFACE_CLASS_IMAGE:
                        return "USB Imaging Device";
                    case INTERFACE_CLASS_PRINTER:
                        return "USB Printer";
                    case INTERFACE_CLASS_MASS_STORAGE:
                        return "USB Mass Storage Device";
                    case INTERFACE_CLASS_HUB:
                        if (device->descriptor.usb_version == 0x210)
                            return "USB 2.1 Hub";
                        if (device->descriptor.usb_version == 0x200)
                            return "USB 2.0 Hub";
                        if (device->descriptor.usb_version == 0x110)
                            return "USB 1.1 Hub";
                        if (device->descriptor.usb_version == 0x110)
                            return "USB 1.0 Hub";
                        return "USB Hub";
                    case INTERFACE_CLASS_CDC_DATA:
                        return "USB CDC-Data Device";
                    case INTERFACE_CLASS_SMART_CARD:
                        return "USB Smart Card";
                    case INTERFACE_CLASS_CONTENT_SECURITY:
                        return "USB Content Security Device";
                    case INTERFACE_CLASS_VIDEO:
                        return "USB Video Device";
                    case INTERFACE_CLASS_PERSONAL_HEALTHCARE:
                        return "USB Healthcare Device";
                    case INTERFACE_CLASS_AUDIO_VIDEO:
                        return "USB AV Device";
                    case INTERFACE_CLASS_DIAGNOSTIC_DEVICE:
                        return "USB Diagnostic Device";
                    case INTERFACE_CLASS_WIRELESS_CONTROLLER:
                        return "USB Wireless Controller";
                    case INTERFACE_CLASS_MISCELLANEOUS:
                        return "USB Miscellaneous Device";
                    case INTERFACE_CLASS_VENDOR_SPECIFIC:
                        return "Vendor Specific";
                    default:
                        return "Generic Device";
                }
            }
            if (device->descriptor.device_class == DEVICE_CLASS_VENDOR_SPECIFIC)
                return "Vendor Specific";
            return "Unconfigured Device";
        default:
            return "Generic Device";
    }
}

static usb_call_result_t usb_control_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer,
                                             uint32_t buffer_length, usb_device_request_t *request, uint32_t timeout)
{
    DEBUG_START("usb_control_message");

    if ((uint32_t)buffer & 3)
    {
        debug_printf("USBD: Warning: buffer is not word aligned.\r\n");
    }

    usb_call_result_t result;
    if ((result = hcd_submit_control_message(device, pipe, buffer, buffer_length, request)) != OK)
    {
        debug_printf("USBD: Failed to send message to %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return result;
    }

    while (timeout-- > 0 && device->error & USB_TRANSFER_ERROR_PROCESSING)
    {
        delay(1000);
    }
    if (device->error & USB_TRANSFER_ERROR_PROCESSING)
    {
        debug_printf("USBD: Message to %s timeout reached.\r\n", usb_get_description(device));
        DEBUG_END();
        return ERROR_TIMEOUT;
    }

    if (device->error & ~USB_TRANSFER_ERROR_PROCESSING)
    {
        if ((device->parent != NULL) && (device->parent->device_check_connection != NULL))
        {
            debug_printf("USBD: Verifying %s is still connected.\r\n", usb_get_description(device));
            if (device->parent->device_check_connection(device->parent, device) != OK)
            {
                debug_printf("USBD: No, %s is disconnected.\r\n", usb_get_description(device));
                return ERROR_DISCONNECTED;
            }
            else
            {
                debug_printf("USBD: Yes, %s is still connected.\r\n", usb_get_description(device));
            }
        }
        DEBUG_END();
        return ERROR_DEVICE;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t usb_get_descriptor(usb_device_t *device, descriptor_type_t descriptor_type, uint8_t index,
                                            uint16_t lang_id, void *buffer, uint32_t length, uint32_t minimum_length,
                                            uint8_t recipient)
{
    DEBUG_START("usb_get_descriptor");

    // Send the control message
    usb_pipe_address_t pipe = {
            .type = USB_TRANSFER_TYPE_CONTROL,
            .speed = device->speed,
            .endpoint = 0,
            .device = device->number,
            .direction = DEVICE_TO_HOST,
            .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
            .request = USB_DEVICE_REQUEST_GET_DESCRIPTOR,
            .type = 0x80 | recipient,
            .value = (uint16_t)(descriptor_type << 8 | index),
            .index = lang_id,
            .length = length
    };
    usb_call_result_t result;
    if ((result = usb_control_message(device, pipe, buffer, length, &request, CONTROL_MESSAGE_TIMEOUT)) != OK)
    {
        debug_printf("USBD: Failed to get descriptor %02x:%02x for device %s.\r\n",
                     descriptor_type, index, usb_get_description(device));
        DEBUG_END();
        return result;
    }
    if (device->last_transfer < minimum_length)
    {
        debug_printf("USBD: Unexpectedly short descriptor (%d/%d) %02x:%02x for device %s.\r\n",
                     device->last_transfer, minimum_length, descriptor_type, index, usb_get_description(device));
        DEBUG_END();
        return ERROR_DEVICE;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t usb_set_address(usb_device_t *device, uint8_t address)
{
    DEBUG_START("usb_set_address");

    if (device->status != USB_DEVICE_STATUS_DEFAULT)
    {
        debug_printf("USBD: Illegal attempt to configure device %s in state %d.\r\n",
                     usb_get_description(device), device->status);
        return ERROR_DEVICE;
    }

    // Send the control message
    usb_pipe_address_t pipe = {
            .type = USB_TRANSFER_TYPE_CONTROL,
            .speed = device->speed,
            .endpoint = 0,
            .device = 0,
            .direction = HOST_TO_DEVICE,
            .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
            .request = USB_DEVICE_REQUEST_SET_ADDRESS,
            .type = 0,
            .value = address
    };
    usb_call_result_t result;
    if ((result = usb_control_message(device, pipe, NULL, 0, &request, CONTROL_MESSAGE_TIMEOUT)) != OK)
    {
        DEBUG_END();
        return result;
    }

    delay(10000); // Hopefully this is long enough...
    device->number = address;
    device->status = USB_DEVICE_STATUS_ADDRESSED;

    DEBUG_END();
    return OK;
}

static void usb_deallocate_device(usb_device_t *device)
{
    DEBUG_START("usb_deallocate_device");

    debug_printf("USBD: Deallocating device %d: %s.\r\n", device->number, usb_get_description(device));

    if (device->device_detached)
    {
        device->device_detached(device);
    }

    if (device->device_deallocate)
    {
        device->device_deallocate(device);
    }

    if (device->parent && device->parent->device_child_detached)
    {
        device->parent->device_child_detached(device->parent, device);
    }

    if (device->status == USB_DEVICE_STATUS_ADDRESSED || device->status == USB_DEVICE_STATUS_CONFIGURED)
    {
        if (device->number > 0 && device->number <= MAXIMUM_USB_DEVICES && devices[device->number - 1] == device)
        {
            devices[device->number - 1] = NULL;
        }
    }

    if (device->full_configuration)
    {
        heap_free((void *)device->full_configuration);
    }

    heap_free(device);

    DEBUG_END();
}

static usb_call_result_t usb_allocate_device(usb_device_t **devicep)
{
    DEBUG_START("usb_allocate_device");

    *devicep = heap_alloc("usb_allocate_device", sizeof(usb_device_t));
    if (!*devicep)
    {
        debug_printf("USBD: Failed to allocate memory for new device.\r\n");
        DEBUG_END();
        return ERROR_MEMORY;
    }

    for (uint32_t number = 0; number < MAXIMUM_USB_DEVICES; number++)
    {
        if (!devices[number])
        {
            devices[number] = *devicep;
            (*devicep)->number = number + 1;
            break;
        }
        // What if we run out of device numbers? Is that even possible?
    }

    debug_printf("USBD: Allocating new device, address %d.\r\n", (*devicep)->number);

    (*devicep)->status = USB_DEVICE_STATUS_ATTACHED;
    (*devicep)->error = USB_TRANSFER_OK;
    (*devicep)->port_number = 0;
    (*devicep)->parent = NULL;
    (*devicep)->driver_data = NULL;
    (*devicep)->full_configuration = NULL;
    (*devicep)->configuration_index = 0xff; // ???
    (*devicep)->device_deallocate = NULL;
    (*devicep)->device_detached = NULL;
    (*devicep)->device_check_connection = NULL;
    (*devicep)->device_check_for_change = NULL;
    (*devicep)->device_child_detached = NULL;
    (*devicep)->device_child_reset = NULL;

    DEBUG_END();
    return OK;
}

static usb_call_result_t usb_get_string(usb_device_t *device, uint8_t string_index, uint16_t lang_id,
                                        void *buffer, uint32_t length)
{
    usb_call_result_t result;
    // From AC's code: Apparently this tends to fail a lot.
    for (uint32_t i = 0; i < 3; i++)
    {
        result = usb_get_descriptor(device, DESCRIPTOR_TYPE_STRING, string_index, lang_id, buffer, length, length, 0);
        if (result == OK)
            break;
    }
    return result;
}

static usb_call_result_t usb_read_string_lang(usb_device_t *device, uint8_t string_index, uint16_t lang_id,
                                              void *buffer, uint32_t length)
{
    DEBUG_START("usb_read_string_lang");

    usb_call_result_t result = usb_get_string(device, string_index, lang_id, buffer, MIN(2, length, uint32_t));

    if (result == OK && device->last_transfer != length)
    {
        result = usb_get_string(device, string_index, lang_id, buffer, MIN(((uint8_t *)buffer)[0], length, uint32_t));
    }

    DEBUG_END();
    return result;
}

static usb_call_result_t usb_read_string(usb_device_t *device, uint8_t string_index, char *buffer, uint32_t length)
{
    if (buffer == NULL || string_index == 0)
        return ERROR_ARGUMENT;

    DEBUG_START("usb_read_device_descriptor");

    usb_call_result_t result;
    uint16_t lang_ids[2];

    result = usb_read_string_lang(device, 0, 0, &lang_ids, 4);
    if (result != OK)
    {
        debug_printf("USBD: Error getting language list from %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return result;
    }
    if (device->last_transfer < 4)
    {
        debug_printf("USBD: Unexpectedly short language list from %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return ERROR_DEVICE;
    }

    usb_string_descriptor_t *descriptor = (usb_string_descriptor_t *)buffer;
    if ((result = usb_read_string_lang(device, string_index, lang_ids[1], descriptor, length)) != OK)
    {
        DEBUG_END();
        return result;
    }

    char *temp_buffer = heap_alloc("usb_read_string", 0x100);
    if (temp_buffer == NULL)
    {
        DEBUG_END();
        return ERROR_MEMORY;
    }
    ucs16_to_utf8(temp_buffer, descriptor->data, 0x100, descriptor->descriptor_length);
    memcpy(buffer, temp_buffer, 0x100);
    heap_free(temp_buffer);

    DEBUG_END();
    return result;
}

static usb_call_result_t usb_read_device_descriptor(usb_device_t *device)
{
    DEBUG_START("usb_read_device_descriptor");

    usb_call_result_t result;
    device->descriptor.max_packet_size_0 = device->speed == USB_SPEED_LOW ? 8 : 64;
    if (device->speed == USB_SPEED_LOW || device->speed == USB_SPEED_FULL)
    {
        if ((result = usb_get_descriptor(device, DESCRIPTOR_TYPE_DEVICE, 0, 0, (void *)&device->descriptor,
                               sizeof(device->descriptor), 8, 0)) != OK)
        {
            DEBUG_END();
            return result;
        }
        if (device->last_transfer == sizeof(usb_device_descriptor_t))
        {
            DEBUG_END();
            return result;
        }
    }
    DEBUG_END();
    return usb_get_descriptor(device, DESCRIPTOR_TYPE_DEVICE, 0, 0, (void *)&device->descriptor,
                              sizeof(device->descriptor), sizeof(device->descriptor), 0);
}

static usb_call_result_t usb_set_configuration(usb_device_t *device, uint8_t configuration)
{
    DEBUG_START("usb_set_configuration");

    usb_call_result_t result;
    if (device->status != USB_DEVICE_STATUS_ADDRESSED)
    {
        debug_printf("USBD: Illegal attempt to configure device %s in state %x.\r\n",
                     usb_get_description(device), device->status);
        DEBUG_END();
        return ERROR_DEVICE;
    }

    usb_pipe_address_t pipe = {
        .type = USB_TRANSFER_TYPE_CONTROL,
        .speed = device->speed,
        .endpoint = 0,
        .device = device->number,
        .direction = HOST_TO_DEVICE,
        .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
        .request = USB_DEVICE_REQUEST_SET_CONFIGURATION,
        .type = 0,
        .value = configuration
    };
    if ((result = usb_control_message(device, pipe, NULL, 0, &request, CONTROL_MESSAGE_TIMEOUT)) != OK)
    {
        DEBUG_END();
        return result;
    }

    device->configuration_index = configuration;
    device->status = USB_DEVICE_STATUS_CONFIGURED;

    DEBUG_END();
    return OK;
}

static usb_call_result_t usb_configure(usb_device_t *device, uint8_t configuration)
{
    DEBUG_START("usb_configure");

    usb_call_result_t result;
    void *full_descriptor;
    usb_descriptor_header_t *header;
    usb_interface_descriptor_t *interface;
    usb_endpoint_descriptor_t *endpoint;
    uint32_t last_interface, last_endpoint;
    bool is_alternate;

    if (device->status != USB_DEVICE_STATUS_ADDRESSED)
    {
        debug_printf("USBD: Illegal attempt to configure device %s in state %x.\r\n",
                     usb_get_description(device), device->status);
        DEBUG_END();
        return ERROR_DEVICE;
    }

    if ((result = usb_get_descriptor(device, DESCRIPTOR_TYPE_CONFIGURATION, configuration, 0,
                                     (void *)&device->configuration, sizeof(device->configuration),
                                     sizeof(device->configuration), 0)) != OK)
    {
        debug_printf("USBD: Failed to retrieve configuration descriptor %x for device %s.\r\n",
                     configuration, usb_get_description(device));
        DEBUG_END();
        return result;
    }

    if ((full_descriptor = heap_alloc("usb_configure", device->configuration.total_length)) == NULL)
    {
        debug_printf("USBD: Failed to allocate space for descriptor.\r\n");
        DEBUG_END();
        return ERROR_MEMORY;
    }

    if ((result = usb_get_descriptor(device, DESCRIPTOR_TYPE_CONFIGURATION, configuration, 0,
                                     full_descriptor, device->configuration.total_length,
                                     device->configuration.total_length, 0)) != OK)
    {
        debug_printf("USBD: Failed to retrieve full configuration descriptor %x for device %s.\r\n",
                     configuration, usb_get_description(device));
        DEBUG_END();
        heap_free(full_descriptor);
        return result;
    }

    device->configuration_index = configuration;
    configuration = device->configuration.configuration_value;

    header = full_descriptor;
    last_interface = MAX_INTERFACES_PER_DEVICE;
    last_endpoint = MAX_ENDPOINTS_PER_INTERFACE;
    is_alternate = false;

    for (header = (usb_descriptor_header_t *)((uint8_t *)header + header->descriptor_length);
         (uint32_t) header - (uint32_t) full_descriptor < device->configuration.total_length;
         header = (usb_descriptor_header_t *)((uint8_t *)header + header->descriptor_length))
    {
        bool header_loop_break = false;
        switch (header->descriptor_type)
        {
            case DESCRIPTOR_TYPE_INTERFACE:
                interface = (usb_interface_descriptor_t *)header;
                if (last_interface == interface->number)
                {
                    is_alternate = true;
                }
                else
                {
                    memcpy((void *)&device->interfaces[last_interface = interface->number], (void *)interface,
                           sizeof(usb_interface_descriptor_t));
                    last_endpoint = 0;
                    is_alternate = false;
                }
                break;
            case DESCRIPTOR_TYPE_ENDPOINT:
                if (is_alternate) break;
                if (last_interface == MAX_INTERFACES_PER_DEVICE
                    || last_endpoint >= device->interfaces[last_interface].endpoint_count)
                {
                    debug_printf("USBD: Unexpected endpoint descriptor in %s.Interface%d.\r\n",
                                 usb_get_description(device), last_interface + 1);
                    break;
                }
                endpoint = (usb_endpoint_descriptor_t *)header;
                memcpy((void *)&device->endpoints[last_interface][last_endpoint++], (void *)endpoint,
                       sizeof(usb_endpoint_descriptor_t));
                break;
            default:
                if (header->descriptor_length == 0) header_loop_break = true;
        }
        if (header_loop_break) break;
        debug_printf("USBD: Descriptor %d length %d, interface %d.\r\n",
                     header->descriptor_type, header->descriptor_length, last_interface);
    }

    /* header_loop_break */
    if ((result = usb_set_configuration(device, configuration)) != OK)
    {
        DEBUG_END();
        heap_free(full_descriptor);
        return result;
    }

    debug_printf("USBD: %s configuration %d. Class %d, subclass %d.\r\n",
                 usb_get_description(device), configuration,
                 device->interfaces[0].interface_class, device->interfaces[0].sub_class);
    device->full_configuration = full_descriptor;

    DEBUG_END();
    return OK;
}

static usb_call_result_t usb_attach_device(usb_device_t *device)
{
    DEBUG_START("usb_attach_device");

    usb_call_result_t result;
    char *buffer;
    uint8_t address = device->number;
    device->number = 0;
    debug_printf("USBD: Scanning %d. %s.\r\n", address, device_speeds[MIN(device->speed, 3, uint8_t)]);

    // Read device descriptor
    if ((result = usb_read_device_descriptor(device)) != OK)
    {
        debug_printf("USBD: Failed to read device descriptor for %d.\r\n", address);
        device->number = address;
        DEBUG_END();
        return result;
    }
    device->status = USB_DEVICE_STATUS_DEFAULT;

    if (device->parent && device->parent->device_child_reset)
    {
        if ((result = device->parent->device_child_reset(device->parent, device)) != OK)
        {
            debug_printf("USBD: Failed to reset port again for new device %s.\r\n", usb_get_description(device));
            device->number = address;
            DEBUG_END();
            return result;
        }
    }

    // Set device address
    if ((result = usb_set_address(device, address)) != OK)
    {
        debug_printf("USBD: Failed to assign address to %d.\r\n", address);
        device->number = address;
        DEBUG_END();
        return result;
    }
    device->number = address;

    // Read device descriptor (again)
    if ((result = usb_read_device_descriptor(device)) != OK)
    {
        debug_printf("USBD: Failed to reread device descriptor for %d.\r\n", address);
        device->number = address;
        DEBUG_END();
        return result;
    }

    debug_printf("USBD: Attach device %s. Address: %d Class: %d Subclass: %d"
                 " USB: %x.%x. %d configurations, %d interfaces.\r\n",
                 usb_get_description(device), address, device->descriptor.device_class, device->descriptor.sub_class,
                 device->descriptor.usb_version >> 8, (device->descriptor.usb_version >> 4) & 0xf,
                 device->descriptor.configuration_count, device->configuration.interface_count);

    // Read and display product, manufacturer, serial number
    buffer = heap_alloc("usb_attach_device", 0x100);
    if (buffer != NULL)
    {
        if (device->descriptor.product != 0)
        {
            result = usb_read_string(device, device->descriptor.product, buffer, 0x100);
            if (result == OK) debug_printf("USBD:  -Product:       %s.\r\n", buffer);
        }
        if (device->descriptor.manufacturer != 0)
        {
            result = usb_read_string(device, device->descriptor.manufacturer, buffer, 0x100);
            if (result == OK) debug_printf("USBD:  -Manufacturer:  %s.\r\n", buffer);
        }
        if (device->descriptor.serial_number != 0)
        {
            result = usb_read_string(device, device->descriptor.serial_number, buffer, 0x100);
            if (result == OK) debug_printf("USBD:  -SerialNumber:  %s.\r\n", buffer);
        }
    }

    debug_printf("USBD:  -VID:PID:       %x:%x v%d.%x.\r\n",
                 device->descriptor.vendor_id, device->descriptor.product_id,
                 device->descriptor.version >> 8, device->descriptor.version & 0xff);

    // Configure device - we only support devices with one configuration for now
    if ((result = usb_configure(device, 0)) != OK)
    {
        debug_printf("USBD: Failed to configure device %x.\n", address);
        DEBUG_END();
        if (buffer != NULL) heap_free(buffer);
        return OK;
    }

    if (buffer != NULL && device->configuration.string_index != 0)
    {
        result = usb_read_string(device, device->configuration.string_index, buffer, 0x100);
        if (result == OK) debug_printf("USBD:  -Configuration: %s.\r\n", buffer);
    }
    if (buffer != NULL) heap_free(buffer);

    // Start driver
    if (device->interfaces[0].interface_class < INTERFACE_CLASS_ATTACH_COUNT
        && interface_class_attach[device->interfaces[0].interface_class] != NULL)
    {
        if ((result = interface_class_attach[device->interfaces[0].interface_class](device, 0)) != OK)
        {
            debug_printf("USBD: Could not start the driver for %s.\r\n", usb_get_description(device));
        }
    }
    else
    {
        debug_printf("USBD: No driver registered for %s.\r\n", usb_get_description(device));
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t usb_attach_root_hub()
{
    DEBUG_START("usb_attach_root_hub");

    usb_device_t *root_hub;
    debug_printf("USBD: Scanning for devices.\r\n");

    if (devices[0] != NULL)
    {
        usb_deallocate_device(devices[0]);
    }
    usb_call_result_t result;
    if ((result = usb_allocate_device(&root_hub)) != OK)
    {
        debug_printf("USBD: Error allocating root hub device.\r\n");
        DEBUG_END();
        return result;
    }
    devices[0]->status = USB_DEVICE_STATUS_POWERED;

    DEBUG_END();
    return usb_attach_device(devices[0]);
}

static void usb_load()
{
    for (uint32_t i = 0; i < MAXIMUM_USB_DEVICES; i++)
        devices[i] = NULL;
    for (uint32_t i = 0; i < INTERFACE_CLASS_ATTACH_COUNT; i++)
        interface_class_attach[i] = NULL;
}

usb_call_result_t usb_init()
{
    DEBUG_START("usb_init");

    usb_load();

    usb_call_result_t result;
    if ((result = hcd_init()) != OK)
    {
        debug_printf("Failed to initialize HCD.\r\n");
        DEBUG_END();
        return result;
    }
    if ((result = hcd_start()) != OK)
    {
        debug_printf("Failed to start HCD.\r\n");
        DEBUG_END();
        return result;
    }
    if ((result = usb_attach_root_hub()) != OK)
    {
        debug_printf("Failed to attach root hub.\r\n");
        DEBUG_END();
        return result;
    }
    DEBUG_END();
    return OK;
}
