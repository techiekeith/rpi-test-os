/*
 * hub.c - USB Hub driver
 */

#include "../../../../include/common/string.h"
#include "../../../../include/kernel/heap.h"
#include "../../../../include/kernel/io.h"
#include "../../../../include/kernel/system_timer.h"
#include "../../../../include/kernel/usb/usb.h"
#include "../../../../include/kernel/usb/usb_hcd.h"
#include "../../../../include/kernel/usb/device/hub.h"

DEBUG_INIT("hub");

static usb_call_result_t hub_read_descriptor(usb_device_t *device)
{
    DEBUG_START("hub_read_descriptor");

    usb_descriptor_header_t header;
    usb_call_result_t result;

    if ((result = usb_get_descriptor(device, DESCRIPTOR_TYPE_HUB, 0, 0, &header, sizeof(header),
                                     sizeof(header), 0x20)) != OK)
    {
        debug_printf("HUB: Failed to read hub descriptor for %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return result;
    }

    hub_device_t *data = (hub_device_t *)device->driver_data;
    if (data->descriptor == NULL
        && (data->descriptor = heap_alloc("hub_read_descriptor", header.descriptor_length)) == NULL)
    {
        debug_printf("HUB: Not enough memory to read hub descriptor for %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return ERROR_MEMORY;
    }

    if ((result = usb_get_descriptor(device, DESCRIPTOR_TYPE_HUB, 0, 0, data->descriptor, header.descriptor_length,
                                     header.descriptor_length, 0x20)) != OK)
    {
        debug_printf("HUB: Failed to read hub descriptor for %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return result;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t hub_get_status(usb_device_t *device)
{
    DEBUG_START("hub_get_status");

    usb_pipe_address_t pipe = {
            .type = USB_TRANSFER_TYPE_CONTROL,
            .speed = device->speed,
            .endpoint = 0,
            .device = device->number,
            .direction = DEVICE_TO_HOST,
            .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
            .request = USB_DEVICE_REQUEST_GET_STATUS,
            .type = 0xa0, // XXX MAGIC
            .length = sizeof(hub_full_status_t)
    };
    usb_call_result_t result = usb_control_message(device,
                                                   pipe,
                                                   &((hub_device_t *) device->driver_data)->status,
                                                   sizeof(hub_full_status_t),
                                                   &request,
                                                   CONTROL_MESSAGE_TIMEOUT);
    if (result != OK)
    {
        DEBUG_END();
        return result;
    }
    if (device->last_transfer < sizeof(hub_full_status_t))
    {
        debug_printf("HUB: Failed to read hub status for %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return ERROR_DEVICE;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t hub_port_get_status(usb_device_t *device, uint8_t port)
{
    DEBUG_START("hub_port_get_status");

    usb_pipe_address_t pipe = {
        .type = USB_TRANSFER_TYPE_CONTROL,
        .speed = device->speed,
        .endpoint = 0,
        .device = device->number,
        .direction = DEVICE_TO_HOST,
        .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
        .request = USB_DEVICE_REQUEST_GET_STATUS,
        .type = 0xa3, // XXX MAGIC
        .index = port + 1,
        .length = sizeof(hub_port_full_status_t)
    };
    usb_call_result_t result = usb_control_message(device,
                                                   pipe,
                                                   &((hub_device_t *) device->driver_data)->port_status[port],
                                                   sizeof(hub_port_full_status_t),
                                                   &request,
                                                   CONTROL_MESSAGE_TIMEOUT);
    if (result != OK)
    {
        DEBUG_END();
        return result;
    }
    if (device->last_transfer < sizeof(hub_port_full_status_t))
    {
        debug_printf("HUB: Failed to read hub port status for %s.Port%d.\r\n", usb_get_description(device), port + 1);
        DEBUG_END();
        return ERROR_DEVICE;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t hub_change_feature(usb_device_t *device, hcd_hub_port_feature_t feature, bool set)
{
    DEBUG_START("hub_change_feature");

    usb_pipe_address_t pipe = {
        .type = USB_TRANSFER_TYPE_CONTROL,
        .speed = device->speed,
        .endpoint = 0,
        .device = device->number,
        .direction = HOST_TO_DEVICE,
        .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
        .request = set ? USB_DEVICE_REQUEST_SET_FEATURE : USB_DEVICE_REQUEST_CLEAR_FEATURE,
        .type = 0x20, // XXX MAGIC
        .value = (uint16_t) feature
    };
    usb_call_result_t result = usb_control_message(device, pipe, NULL, 0, &request, CONTROL_MESSAGE_TIMEOUT);

    DEBUG_END();
    return result;
}

static usb_call_result_t hub_change_port_feature(usb_device_t *device, hcd_hub_port_feature_t feature,
                                                 uint8_t port, bool set)
{
    DEBUG_START("hub_change_port_feature");

    usb_pipe_address_t pipe = {
        .type = USB_TRANSFER_TYPE_CONTROL,
        .speed = device->speed,
        .endpoint = 0,
        .device = device->number,
        .direction = HOST_TO_DEVICE,
        .max_size = packet_size_from_number(device->descriptor.max_packet_size_0)
    };
    usb_device_request_t request = {
        .request = set ? USB_DEVICE_REQUEST_SET_FEATURE : USB_DEVICE_REQUEST_CLEAR_FEATURE,
        .type = 0x23, // XXX MAGIC
        .value = (uint16_t) feature,
        .index = port + 1
    };
    usb_call_result_t result = usb_control_message(device, pipe, NULL, 0, &request, CONTROL_MESSAGE_TIMEOUT);

    DEBUG_END();
    return result;
}

static usb_call_result_t hub_power_on(usb_device_t *device)
{
    DEBUG_START("hub_power_on");

    hub_device_t *data = (hub_device_t *)device->driver_data;
    usb_hub_descriptor_t *hub_descriptor = data->descriptor;

    debug_printf("HUB: Powering on hub %s.\r\n", usb_get_description(device));

    for (uint32_t i = 0; i < data->max_children; i++)
    {
        if (hub_change_port_feature(device, HUB_PORT_FEATURE_POWER, i, true) != OK)
        {
            debug_printf("HUB: Could not power %s.Port%d.\r\n", usb_get_description(device), i + 1);
        }
    }

    system_timer_busy_wait(hub_descriptor->power_good_delay * 2000);

    DEBUG_END();
    return OK;
}

usb_call_result_t hub_port_reset(usb_device_t *device, uint8_t port)
{
    DEBUG_START("hub_port_reset");

    hub_device_t *data = (hub_device_t *)device->driver_data;
    hub_port_full_status_t *port_status = &data->port_status[port];
    usb_call_result_t result;
    uint32_t retry, timeout;

    debug_printf("HUB: Hub reset %s.Port%d.\r\n", usb_get_description(device), port + 1);
    for (retry = 0; retry < 3; retry++)
    {
        if ((result = hub_change_port_feature(device, HUB_PORT_FEATURE_RESET, port, true)) != OK)
        {
            debug_printf("HUB: Failed to reset %s.Port%d.\r\n", usb_get_description(device), port + 1);
            DEBUG_END();
            return result;
        }
        timeout = 0;
        do {
            system_timer_busy_wait(20000);
            if ((result = hub_port_get_status(device, port)) != OK)
            {
                debug_printf("HUB: Hub failed to get status (4) for %s.Port%d.\r\n",
                             usb_get_description(device), port + 1);
                DEBUG_END();
                return result;
            }
            timeout++;
        } while (!port_status->change.reset_changed && !port_status->status.enabled && timeout < 10);

        if (timeout == 10) continue;

        debug_printf("HUB: %s.Port%d Status %x:%x.\r\n", usb_get_description(device), port + 1,
                     *(uint16_t *)&port_status->status, *(uint16_t *)&port_status->change);

        if (port_status->change.connected_changed || !port_status->status.connected)
        {
            DEBUG_END();
            return ERROR_DEVICE;
        }

        if (port_status->status.enabled) break;
    }

    if (retry == 3)
    {
        debug_printf("HUB: Cannot enable %s.Port%d. Please verify the hardware is working.\r\n",
                     usb_get_description(device), port + 1);
        DEBUG_END();
        return ERROR_DEVICE;
    }

    if (hub_change_port_feature(device, HUB_PORT_FEATURE_RESET, port, false) != OK)
    {
        debug_printf("HUB: Failed to clear reset on %s.Port%d.\r\n", usb_get_description(device), port + 1);
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t hub_port_connection_changed(usb_device_t *device, uint8_t port)
{
    DEBUG_START("hub_port_connection_changed");

    hub_device_t *data = (hub_device_t *)device->driver_data;
    hub_port_full_status_t *port_status = &data->port_status[port];
    usb_call_result_t result;

    if ((result = hub_port_get_status(device, port)) != OK)
    {
        debug_printf("HUB: Hub failed to get status (2) for %s.Port%d.\r\n", usb_get_description(device), port + 1);
        DEBUG_END();
        return result;
    }
    debug_printf("HUB: %s.Port%d Status %x:%x.\r\n", usb_get_description(device), port + 1,
                 *(uint16_t *)&port_status->status, *(uint16_t *)&port_status->change);

    if (hub_change_port_feature(device, HUB_PORT_FEATURE_CONNECTION_CHANGE, port, false) != OK)
    {
        debug_printf("HUB: Failed to clear change on %s.Port%d.\r\n", usb_get_description(device), port + 1);
    }

    if ((!port_status->status.connected && !port_status->status.enabled) || data->children[port] != NULL)
    {
        debug_printf("HUB: Disconnected %s.Port%d - %s.\r\n", usb_get_description(device), port + 1,
                     usb_get_description(data->children[port]));
        usb_deallocate_device(data->children[port]);
        data->children[port] = NULL;
        if (!port_status->status.connected)
        {
            DEBUG_END();
            return OK;
        }
    }

    if ((result = hub_port_reset(device, port)) != OK)
    {
        debug_printf("HUB: Could not reset %s.Port%d for new device.\r\n", usb_get_description(device), port + 1);
        DEBUG_END();
        return result;
    }

    if ((result = usb_allocate_device(&data->children[port])) != OK)
    {
        debug_printf("HUB: Could not allocate a new device entry for %s.Port%d.\r\n",
                     usb_get_description(device), port + 1);
        DEBUG_END();
        return result;
    }

    if ((result = hub_port_get_status(device, port)) != OK)
    {
        debug_printf("HUB: Hub failed to get status (3) for %s.Port%d.\r\n", usb_get_description(device), port + 1);
        DEBUG_END();
        return result;
    }

    debug_printf("HUB: %s.Port%d Status %x:%x.\r\n", usb_get_description(device), port + 1,
                 *(uint16_t *)&port_status->status, *(uint16_t *)&port_status->change);

    if (port_status->status.high_speed_attached)
    {
        data->children[port]->speed = USB_SPEED_HIGH;
    }
    else if (port_status->status.low_speed_attached)
    {
        data->children[port]->speed = USB_SPEED_LOW;
    }
    else
    {
        data->children[port]->speed = USB_SPEED_FULL;
    }

    data->children[port]->parent = device;
    data->children[port]->port_number = port;

    if ((result = usb_attach_device(data->children[port])) != OK)
    {
        debug_printf("HUB: Could not connect to new device in %s.Port%d. Disabling.\r\n",
                     usb_get_description(device), port + 1);
        usb_deallocate_device(data->children[port]);
        data->children[port] = NULL;
        if (hub_change_port_feature(device, HUB_PORT_FEATURE_ENABLE, port, false) != OK)
        {
            debug_printf("HUB: Failed to disable %s.Port%d.\r\n", usb_get_description(device), port + 1);
        }
        DEBUG_END();
        return result;
    }

    DEBUG_END();
    return OK;
}

static void hub_detached(usb_device_t *device)
{
    DEBUG_START("hub_detached");

    debug_printf("HUB: %s detached.\r\n", usb_get_description(device));
    hub_device_t *data = (hub_device_t *) device->driver_data;
    if (data != NULL)
    {
        for (uint32_t i = 0; i < data->max_children; i++)
        {
            if (data->children[i] != NULL && data->children[i]->device_detached != NULL)
            {
                data->children[i]->device_detached(data->children[i]);
            }
        }
    }

    DEBUG_END();
}

static void hub_deallocate(usb_device_t *device)
{
    DEBUG_START("hub_deallocate");

    debug_printf("HUB: %s deallocate.\r\n", usb_get_description(device));
    if (device->driver_data != NULL)
    {
        hub_device_t *data = (hub_device_t *) device->driver_data;
        for (uint32_t i = 0; i < data->max_children; i++)
        {
            if (data->children[i] != NULL)
            {
                usb_deallocate_device(data->children[i]);
                data->children[i] = NULL;
            }

            if (data->descriptor != NULL)
            {
                heap_free(data->descriptor);
            }
            heap_free((void *)device->driver_data);
        }
    }
    device->device_deallocate = NULL;
    device->device_detached = NULL;
    device->device_check_for_change = NULL;
    device->device_child_detached = NULL;
    device->device_child_reset = NULL;
    device->device_check_connection = NULL;

    DEBUG_END();
}

static void hub_check_for_change(usb_device_t *device)
{
    DEBUG_START("hub_check_for_change");

    hub_device_t *data = (hub_device_t *)device->driver_data;

    for (uint32_t i = 0; i < data->max_children; i++)
    {
        if (hub_check_connection(device, i) != OK) continue;
        if (data->children[i] != NULL && data->children[i]->device_check_for_change != NULL)
        {
            data->children[i]->device_check_for_change(data->children[i]);
        }
    }

    DEBUG_END();
}

static void hub_child_detached(usb_device_t *device, usb_device_t *child)
{
    DEBUG_START("hub_child_detached");

    hub_device_t *data = (hub_device_t *)device->driver_data;

    if (child->parent == device
        && child->port_number < data->max_children
        && data->children[child->port_number] == child)
    {
        data->children[child->port_number] = NULL;
    }

    DEBUG_END();
}

static usb_call_result_t hub_child_reset(usb_device_t *device, usb_device_t *child)
{
    DEBUG_START("hub_child_reset");

    hub_device_t *data = (hub_device_t *)device->driver_data;

    if (child->parent == device
        && child->port_number < data->max_children
        && data->children[child->port_number] == child)
    {
        usb_call_result_t result = hub_port_reset(device, child->port_number);
        DEBUG_END();
        return result;
    }

    DEBUG_END();
    return ERROR_DEVICE;
}

static usb_call_result_t hub_check_connection_device(usb_device_t *device, usb_device_t *child)
{
    DEBUG_START("hub_check_connection_device");

    hub_device_t *data = (hub_device_t *)device->driver_data;

    if (child->parent == device
        && child->port_number < data->max_children
        && data->children[child->port_number] == child)
    {
        usb_call_result_t result = hub_check_connection(device, child->port_number);
        DEBUG_END();
        return (result == OK) ? (data->children[child->port_number] == child ? OK : ERROR_DISCONNECTED) : result;
    }

    DEBUG_END();
    return ERROR_ARGUMENT;
}

usb_call_result_t hub_check_connection(usb_device_t *device, uint8_t port)
{
    usb_call_result_t result;
    hub_port_full_status_t *port_status;
    hub_device_t *data;
    bool prior_connected_state;

    DEBUG_START("hub_check_connection");

    data = (hub_device_t *)device->driver_data;
    prior_connected_state = data->port_status[port].status.connected;

    if ((result = hub_port_get_status(device, port)) != OK)
    {
        if (result != ERROR_DISCONNECTED)
        {
            debug_printf("HUB: Failed to get hub port status (1) for %s.Port%d.\r\n",
                         usb_get_description(device), port + 1);
        }
        return result;
    }
    port_status = &data->port_status[port];

    if ((device == usb_get_root_hub()) && (prior_connected_state != port_status->status.connected))
    {
        port_status->change.connected_changed = true;
    }

    if (port_status->change.connected_changed)
    {
        hub_port_connection_changed(device, port);
    }

    if (port_status->change.enabled_changed)
    {
        if (hub_change_port_feature(device, HUB_PORT_FEATURE_ENABLE_CHANGE, port, false) != OK)
        {
            debug_printf("HUB: Failed to clear enable change - %s.Port%d.\r\n", usb_get_description(device), port + 1);
        }

        // This may indicate EM interference.
        if (!port_status->status.enabled && port_status->status.connected && data->children[port] != NULL)
        {
            debug_printf("HUB: %s.Port%d has been disabled, but is connected. This can be caused by interference."
                         " Re-enabling!\r\n", usb_get_description(device), port + 1);
            hub_port_connection_changed(device, port);
        }
    }

    if (port_status->status.suspended)
    {
        if (hub_change_port_feature(device, HUB_PORT_FEATURE_SUSPEND, port, false) != OK)
        {
            debug_printf("HUB: Failed to clear suspended port - %s.Port%d.\r\n", usb_get_description(device), port + 1);
        }
    }

    if (port_status->change.over_current_changed)
    {
        if (hub_change_port_feature(device, HUB_PORT_FEATURE_OVER_CURRENT_CHANGE, port, false) != OK)
        {
            debug_printf("HUB: Failed to clear over current port - %s.Port%d.\r\n", usb_get_description(device), port + 1);
        }
        hub_power_on(device);
    }

    if (port_status->change.reset_changed)
    {
        if (hub_change_port_feature(device, HUB_PORT_FEATURE_RESET_CHANGE, port, false) != OK)
        {
            debug_printf("HUB: Failed to clear reset port - %s.Port%d.\r\n", usb_get_description(device), port + 1);
        }
        hub_power_on(device);
    }

    DEBUG_END();
    return OK;
}

usb_call_result_t hub_attach(usb_device_t *device, uint32_t interface_number)
{
    usb_call_result_t result;
    hub_device_t *data;
    usb_hub_descriptor_t *hub_descriptor;
    hub_full_status_t *status;

    DEBUG_START("hub_attach");

    if (device->interfaces[interface_number].endpoint_count != 1)
    {
        debug_printf("HUB: Cannot enumerate hub with multiple endpoints: %d.\r\n",
                     device->interfaces[interface_number].endpoint_count);
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }
    if (device->endpoints[interface_number][0].endpoint_address.direction == HOST_TO_DEVICE)
    {
        debug_printf("HUB: Cannot enumerate hub with only one output endpoint.\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }
    if (device->endpoints[interface_number][0].attributes.type != USB_TRANSFER_TYPE_INTERRUPT)
    {
        debug_printf("HUB: Cannot enumerate hub without interrupt endpoint.\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    device->device_deallocate = hub_deallocate;
    device->device_detached = hub_detached;
    device->device_check_for_change = hub_check_for_change;
    device->device_child_detached = hub_child_detached;
    device->device_child_reset = hub_child_reset;
    device->device_check_connection = hub_check_connection_device;

    if ((device->driver_data = heap_alloc("hub_attach", sizeof(hub_device_t))) == NULL)
    {
        debug_printf("HUB: Cannot allocate hub data. Out of memory.\r\n");
        DEBUG_END();
        return ERROR_MEMORY;
    }

    data = (hub_device_t *)device->driver_data;
    memset(data, 0, sizeof(hub_device_t));
    device->driver_data->data_size = sizeof(hub_device_t);
    device->driver_data->device_driver = DEVICE_DRIVER_HUB;
    for (uint32_t i = 0; i < MAX_CHILDREN_PER_DEVICE; i++)
        data->children[i] = NULL;

    if ((result = hub_read_descriptor(device)) != OK)
    {
        DEBUG_END();
        return result;
    }

    // Number of children
    hub_descriptor = data->descriptor;
    if (hub_descriptor->port_count > MAX_CHILDREN_PER_DEVICE)
    {
        debug_printf("HUB: Hub %s is too big for this driver to handle. Only the first %d ports will be used."
                     " Change MAX_CHILDREN_PER_DEVICE in include/kernel/usb/usb.h to increase this limit.\r\n",
                     usb_get_description(device), MAX_CHILDREN_PER_DEVICE);
        data->max_children = MAX_CHILDREN_PER_DEVICE;
    }
    else
    {
        data->max_children = hub_descriptor->port_count;
    }

    // Power switching mode
    switch (hub_descriptor->attributes.power_switching_mode)
    {
        case HUB_PORT_CONTROL_GLOBAL:
            debug_printf("HUB: Hub power: Global.\r\n");
            break;
        case HUB_PORT_CONTROL_INDIVIDUAL:
            debug_printf("HUB: Hub power: Individual.\r\n");
            break;
        default:
            debug_printf("HUB: Hub power: No power switching.\r\n");
    }

    // Hub nature
    if (hub_descriptor->attributes.compound)
    {
        debug_printf("HUB: Hub nature: Compound.\r\n");
    }
    else
    {
        debug_printf("HUB: Hub nature: Standalone.\r\n");
    }

    // Over current protection
    switch (hub_descriptor->attributes.over_current_protection)
    {
        case HUB_PORT_CONTROL_GLOBAL:
            debug_printf("HUB: Hub power: Global.\r\n");
            break;
        case HUB_PORT_CONTROL_INDIVIDUAL:
            debug_printf("HUB: Hub power: Individual.\r\n");
            break;
        default:
            debug_printf("HUB: Unknown hub over current type %d on %s. Driver incompatible.\r\n",
                         hub_descriptor->attributes.power_switching_mode, usb_get_description(device));
            hub_deallocate(device);
            DEBUG_END();
            return ERROR_INCOMPATIBLE;
    }

    debug_printf("HUB: Hub power to good: %dms.\r\n", hub_descriptor->power_good_delay * 2);
    debug_printf("HUB: Hub current required: %dmA.\r\n", hub_descriptor->maximum_hub_power * 2);
    debug_printf("HUB: Hub ports: %d.\r\n", hub_descriptor->port_count);

    for (uint32_t i = 0; i < data->max_children; i++) {
        if (hub_descriptor->data[(i + 1) >> 3] & 1 << ((i + 1) & 0x7))
            debug_printf("HUB: Hub port %d is not removable.\n", i + 1);
        else
            debug_printf("HUB: Hub port %d is removable.\n", i + 1);
    }

    // Get hub status
    if ((result = hub_get_status(device)) != OK)
    {
        debug_printf("HUB: Failed to get hub status for %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return result;
    }
    status = &data->status;
    debug_printf("HUB: Hub power: %s.\r\n", status->status.local_power ? "Lost" : "Good");
    debug_printf("HUB: Hub over current condition: %s.\r\n", status->status.over_current ? "Yes" : "No");

    // Power on hub
    debug_printf("HUB: Hub powering on.\r\n");
    if ((result = hub_power_on(device)) != OK)
    {
        debug_printf("HUB: Hub failed to power on.\r\n");
        DEBUG_END();
        return result;
    }

    // Get hub status a second time
    if ((result = hub_get_status(device)) != OK)
    {
        debug_printf("HUB: Failed to get hub status for %s.\r\n", usb_get_description(device));
        DEBUG_END();
        return result;
    }
    debug_printf("HUB: Hub power: %s.\r\n", status->status.local_power ? "Lost" : "Good");
    debug_printf("HUB: Hub over current condition: %s.\r\n", status->status.over_current ? "Yes" : "No");

    debug_printf("HUB: %s status %x:%x.\r\n", usb_get_description(device),
                 *(uint16_t *)&status->status, *(uint16_t *)&status->change);

    for (uint8_t port = 0; port < data->max_children; port++)
    {
        hub_check_connection(device, port);
    }

    DEBUG_END();
    return OK;
}

void hub_load()
{
    DEBUG_START("hub_load");

    debug_printf("HUB: Hub driver version 0.1 (derived from CSUD).\r\n");
    attach_driver_for_class(INTERFACE_CLASS_HUB, hub_attach);

    DEBUG_END();
}
