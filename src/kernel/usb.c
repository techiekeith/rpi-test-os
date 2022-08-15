/*
 * usb.c - USB driver
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../include/common/stddef.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/usb.h"
#include "../../include/kernel/usb_hcd.h"

DEBUG_INIT("usb");

static usb_device_t *devices[MAXIMUM_USB_DEVICES];

char default_device_description[] = "<placeholder>";
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

static int usb_control_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer, uint32_t buffer_length,
                               usb_device_request_t *request, uint32_t timeout)
{
    DEBUG_START("usb_control_message");

    if ((uint32_t)buffer & 3)
    {
        debug_printf("USBD: Warning: buffer is not word aligned.\r\n");
    }

    if (hcd_submit_control_message(device, pipe, buffer, buffer_length, request))
    {
        debug_printf("USBD: Failed to send message to %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return -1;
    }

    while (timeout-- > 0 && device->error & USB_TRANSFER_ERROR_PROCESSING)
    {
        delay(1000000);
    }
    if (device->error & USB_TRANSFER_ERROR_PROCESSING)
    {
        debug_printf("USBD: Message to %s timeout reached.\r\n", usb_get_description(device));
        DEBUG_END();
        return -1;
    }

    if (device->error & ~USB_TRANSFER_ERROR_PROCESSING)
    {
        if (device->parent && device->parent->device_check_connection)
        {
            debug_printf("USBD: Verifying %s is still connected.\r\n", usb_get_description(device));
            if (device->parent->device_check_connection(device->parent, device))
            {
                debug_printf("USBD: No, %s is disconnected.\r\n", usb_get_description(device));
            }
            else
            {
                debug_printf("USBD: Yes, %s is still connected.\r\n", usb_get_description(device));
            }
        }
        DEBUG_END();
        return -1;
    }

    DEBUG_END();
    return 0;
}

static int usb_get_descriptor(usb_device_t *device, descriptor_type_t descriptor_type, uint8_t index, uint16_t lang_id,
                              void *buffer, uint32_t length, uint32_t minimum_length, uint8_t recipient)
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
    if (usb_control_message(device, pipe, buffer, length, &request, CONTROL_MESSAGE_TIMEOUT))
    {
        debug_printf("USBD: Failed to get descriptor %02x:%02x for device %s.\r\n",
                     descriptor_type, index, usb_get_description(device));
        DEBUG_END();
        return -1;
    }
    if (device->last_transfer < minimum_length)
    {
        debug_printf("USBD: Unexpectedly short descriptor (%d/%d) %02x:%02x for device %s.\r\n",
                     device->last_transfer, minimum_length, descriptor_type, index, usb_get_description(device));
        DEBUG_END();
        return -1;
    }

    DEBUG_END();
    return 0;
}

static int usb_set_address(usb_device_t *device, uint8_t address)
{
    DEBUG_START("usb_set_address");

    if (device->status != USB_DEVICE_STATUS_DEFAULT)
    {
        debug_printf("USBD: Illegal attempt to configure device %s in state %d.\r\n",
                     usb_get_description(device), device->status);
        return -1;
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
    if (usb_control_message(device, pipe, NULL, 0, &request, CONTROL_MESSAGE_TIMEOUT))
    {
        DEBUG_END();
        return -1;
    }

    delay(10000000); // Hopefully this is long enough...
    device->number = address;
    device->status = USB_DEVICE_STATUS_ADDRESSED;

    DEBUG_END();
    return 0;
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

static int usb_allocate_device(usb_device_t **devicep)
{
    DEBUG_START("usb_allocate_device");

    *devicep = heap_alloc("usb", sizeof(usb_device_t));
    if (!*devicep)
    {
        debug_printf("USBD: Failed to allocate memory for new device.\r\n");
        DEBUG_END();
        return -1;
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
    (*devicep)->error = USB_TRANSFER_ERROR_NONE;
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
    return 0;
}

static int usb_read_device_descriptor(usb_device_t *device)
{
    device->descriptor.max_packet_size_0 = device->speed == USB_SPEED_LOW ? 8 : 64;
    if (device->speed == USB_SPEED_LOW || device->speed == USB_SPEED_FULL)
    {
        if (usb_get_descriptor(device, DESCRIPTOR_TYPE_DEVICE, 0, 0, (void *)&device->descriptor,
                               sizeof(device->descriptor), 8, 0))
        {
            return -1;
        }
        if (device->last_transfer == sizeof(usb_device_descriptor_t))
        {
            return 0;
        }
    }
    return usb_get_descriptor(device, DESCRIPTOR_TYPE_DEVICE, 0, 0, (void *)&device->descriptor,
                              sizeof(device->descriptor), sizeof(device->descriptor), 0);
}

const char *device_speeds[4] = { "480 Mb/s", "1.5 Mb/s", "12 Mb/s", "unknown speed" };
static int usb_attach_device(usb_device_t *device)
{
    DEBUG_START("usb_attach_device");

    uint8_t address = device->number;
    device->number = 0;
    debug_printf("USBD: Scanning %d. %s.\r\n", address, device_speeds[device->speed]);

    // Read device descriptor
    if (usb_read_device_descriptor(device))
    {
        debug_printf("USBD: Failed to read device descriptor for %d.\r\n", address);
        device->number = address;
        DEBUG_END();
        return -1;
    }
    device->status = USB_DEVICE_STATUS_DEFAULT;

    if (device->parent && device->parent->device_child_reset)
    {
        if (device->parent->device_child_reset(device->parent, device))
        {
            debug_printf("USBD: Failed to reset port again for new device %s.\r\n", usb_get_description(device));
            device->number = address;
            DEBUG_END();
            return -1;
        }
    }

    // Set device address
    if (usb_set_address(device, address))
    {
        debug_printf("USBD: Failed to assign address to %d.\r\n", address);
        device->number = address;
        DEBUG_END();
        return -1;
    }
    device->number = address;

    // Read device descriptor (again)
    if (usb_read_device_descriptor(device))
    {
        debug_printf("USBD: Failed to reread device descriptor for %d.\r\n", address);
        device->number = address;
        DEBUG_END();
        return -1;
    }

    debug_printf("USBD: Attach device %s. Address: %d Class: %d Subclass: %d"
                 " USB: %x.%x. %d configurations, %d interfaces.\r\n",
                 usb_get_description(device), address, device->descriptor.device_class, device->descriptor.sub_class,
                 device->descriptor.usb_version >> 8, (device->descriptor.usb_version >> 4) & 0xf,
                 device->descriptor.configuration_count, device->configuration.interface_count);

    DEBUG_END();
    return 0;
}

static int usb_attach_root_hub()
{
    DEBUG_START("usb_attach_root_hub");

    usb_device_t *root_hub;
    debug_printf("USBD: Scanning for devices.\r\n");

    if (devices[0] != NULL)
    {
        usb_deallocate_device(devices[0]);
    }
    if (usb_allocate_device(&root_hub))
    {
        debug_printf("USBD: Error allocating root hub device.\r\n");
        DEBUG_END();
        return -1;
    }
    devices[0]->status = USB_DEVICE_STATUS_POWERED;

    DEBUG_END();
    return usb_attach_device(devices[0]);
}

void usb_init()
{
    DEBUG_START("usb_init");
    if (hcd_init())
    {
        debug_printf("Failed to initialize HCD.\r\n");
        DEBUG_END();
        return;
    }
    if (hcd_start())
    {
        debug_printf("Failed to start HCD.\r\n");
        DEBUG_END();
        return;
    }
    if (usb_attach_root_hub())
    {
        debug_printf("Failed to attach root hub.\r\n");
        DEBUG_END();
        return;
    }
    DEBUG_END();
}
