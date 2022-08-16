/*
 * usb_hcd.c - USB Host Controller Device driver
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../include/common/string.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/mailbox.h"
#include "../../include/kernel/mmio.h"
#include "../../include/kernel/usb_hcd.h"
#include "../../include/kernel/usb_root_hub.h"

DEBUG_INIT("usb_hcd");

static bool phy_initialized = false;

static uint8_t data_buffer[1024];

static char *operating_mode_capabilities[8] = {
        "HNP_SNP_CAPABLE",
        "SRP_ONLY_CAPABLE",
        "NO_HNP_SNP_CAPABLE",
        "SRP_CAPABLE_DEVICE",
        "NO_SRP_CAPABLE_DEVICE",
        "SRP_CAPABLE_HOST",
        "NO_SRP_CAPABLE_HOST",
        "<undefined>"
};

static char *architecture_capabilities[4] = {
        "SLAVE_ONLY",
        "EXTERNAL_DMA",
        "INTERNAL_DMA",
        "<undefined>"
};

static char *high_speed_physical_capabilities[4] = {
        "NOT_SUPPORTED",
        "UTMI",
        "ULPI"
        "UTMI_ULPI"
};

static char *full_speed_physical_capabilities[4] = {
        "PHYSICAL_0",
        "DEDICATED",
        "PHYSICAL_1"
        "PHYSICAL_2"
};

static int hcd_power_on()
{
    DEBUG_START("hcd_power_on");
    property_message_tag_t tags[2];
    tags[0].proptag = PWR_SET_STATE;
    tags[0].value_buffer.data[0] = DEVICE_USB_HOST_CONTROLLER;
    tags[0].value_buffer.data[1] = 3;
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv != 0)
    {
        debug_printf("Power on USB Host Controller failed, return code %d\r\n", rv);
    }
    DEBUG_END();
    return rv;
}

usb_call_result_t hcd_init()
{
    DEBUG_START("hcd_init");

    volatile uint32_t vendor_id = mmio_read(HCD_VENDOR_ID);
    volatile uint32_t user_id = mmio_read(HCD_USER_ID);
    volatile hcd_hardware_t hardware;
    mmio_read_in(HCD_HARDWARE, &hardware, 4);
    bool emulated_device = false;
    __dmb();

    // Check hardware capabilities
    debug_printf("vendor_id=%p, user_id=%p\r\n", vendor_id, user_id);
    if ((vendor_id & OT2_VENDOR_ID_MASK) == OT2_VENDOR_ID_MATCH) // 'OT2.x'
    {
        debug_printf("Host Controller Device: %c%c%x.%x (BCM%05x)\r\n",
                     (vendor_id >> 24) & 0xff, (vendor_id >> 16) & 0xff,
                     (vendor_id >> 12) & 0xf, vendor_id & 0xfff,
                     (user_id >> 12) & 0xfffff);
    }
    else
    {
        debug_printf("HCD has an incompatible vendor_id. Driver incompatible.\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }
    if (!user_id)
    {
        debug_printf("warning: user_id is zero, are we running in QEMU?\r\n");
        emulated_device = true;
    }
    debug_printf("HCD: Operating mode: %s [%d], architecture: %s [%d], high_speed_physical: %s [%d],"
                 " full_speed_physical: %s [%d], hardware: [%p %p %p %p]\r\n",
                 operating_mode_capabilities[hardware.operating_mode], hardware.operating_mode,
                 architecture_capabilities[hardware.architecture], hardware.architecture,
                 high_speed_physical_capabilities[hardware.high_speed_physical], hardware.high_speed_physical,
                 full_speed_physical_capabilities[hardware.full_speed_physical], hardware.full_speed_physical,
                 hardware.word0, hardware.word1, hardware.word2, hardware.word3);

    // Ignore compatibility checks for now, since QEMU's emulated HCD doesn't report the expected values.
    // This might cause us problems later, but we'll cross that bridge when we come to it
    if (emulated_device)
    {
        debug_printf("HCD is an emulated device; skipping compatibility checks.\r\n");
    }
    else
    {
        if (hardware.architecture != INTERNAL_DMA)
        {
            debug_printf("HCD: Host architecture is not Internal DMA. Driver incompatible.\r\n");
            DEBUG_END();
            return ERROR_INCOMPATIBLE;
        }
        if (hardware.high_speed_physical == NOT_SUPPORTED)
        {
            debug_printf("HCD: High speed physical unsupported. Driver incompatible.\r\n");
            DEBUG_END();
            return ERROR_INCOMPATIBLE;
        }
    }

    // Disable interrupts
    debug_printf("HCD: Disabling interrupts.\r\n");
    volatile hcd_ahb_t ahb;
    mmio_read_in(HCD_USB_GAHBCFG, &ahb, 1);
    __dmb();
    ahb.interrupt_enable = false;
    __dmb();
    mmio_write_out(HCD_USB_GAHBCFG, &ahb, 1);
    mmio_write(HCD_CORE_INTERRUPT_MASK, 0);

    // Power on USB
    debug_printf("HCD: Powering on USB Host Controller.\r\n");
//    if (bcm2835_hcd_power_on())
    if (hcd_power_on())
    {
        debug_printf("HCD: Failed to power on USB Host Controller.\r\n");
        DEBUG_END();
        return ERROR_INCOMPATIBLE;
    }

    DEBUG_END();
    return OK;
}

static usb_call_result_t hcd_reset()
{
    DEBUG_START("hcd_reset");

    uint32_t timeout;
    volatile hcd_reset_t reset;

    debug_printf("HCD: Waiting for AHB master idle.\r\n");
    timeout = 0;
    do {
        mmio_read_in(HCD_RESET, &reset, 1);
        if (timeout++ >= DEVICE_RESET_TIMEOUT)
        {
            debug_printf("AHB Master Idle: %d\n", reset.ahb_master_idle);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return ERROR_DEVICE;
        }
    } while (!reset.ahb_master_idle);

    reset.core_soft = true;
    mmio_write_out(HCD_RESET, &reset, 1);

    debug_printf("HCD: Waiting for AHB master idle, and for core_soft to clear.\r\n");
    timeout = 0;
    do {
        mmio_read_in(HCD_RESET, &reset, 1);
        if (timeout++ >= DEVICE_RESET_TIMEOUT)
        {
            debug_printf("AHB Core Soft: %d\n", reset.core_soft);
            debug_printf("AHB Master Idle: %d\n", reset.ahb_master_idle);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return ERROR_DEVICE;
        }
    } while (reset.core_soft || !reset.ahb_master_idle);

    DEBUG_END();
    return OK;
}

static usb_call_result_t hcd_transmit_fifo_flush(flush_number_t flush_number)
{
    DEBUG_START("hcd_transmit_fifo_flush");

    if (flush_number == FLUSH_ALL)
    {
        debug_printf("Tx FIFO Flush (ALL)\r\n");
    }
    else if (flush_number == FLUSH_NON_PERIODIC)
    {
        debug_printf("Tx FIFO Flush: (NP)\r\n");
    }
    else
    {
        debug_printf("Tx FIFO Flush: (P%d)\r\n", flush_number);
    }

    hcd_reset_t reset = {
            .transmit_fifo_flush_number = flush_number,
            .transmit_fifo_flush = true
    };
    __dmb();
    mmio_write_out(HCD_RESET, &reset, 1);

    uint32_t timeout = 0;
    do {
        mmio_read_in(HCD_RESET, &reset, 1);
        if (timeout++ >= DEVICE_RESET_TIMEOUT)
        {
            debug_printf("Tx FIFO Flush: %d\n", reset.transmit_fifo_flush);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return ERROR_DEVICE;
        }
    } while (reset.transmit_fifo_flush);

    DEBUG_END();
    return OK;
}

static usb_call_result_t hcd_receive_fifo_flush()
{
    DEBUG_START("hcd_receive_fifo_flush");

    debug_printf("Rx FIFO Flush (ALL)\r\n");

    hcd_reset_t reset = {
            .receive_fifo_flush = true
    };
    __dmb();
    mmio_write_out(HCD_RESET, &reset, 1);

    uint32_t timeout = 0;
    do {
        mmio_read_in(HCD_RESET, &reset, 1);
        if (timeout++ >= DEVICE_RESET_TIMEOUT)
        {
            debug_printf("Tx FIFO Flush: %d\n", reset.receive_fifo_flush);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return ERROR_DEVICE;
        }
    } while (reset.receive_fifo_flush);

    DEBUG_END();
    return OK;
}

usb_call_result_t hcd_start()
{
    DEBUG_START("hcd_start");

    usb_call_result_t result;
    volatile hcd_hardware_t hardware;
    mmio_read_in(HCD_HARDWARE, &hardware, 4);

    // Copied from Alex Chadwick's code, not sure what these flags do
    debug_printf("HCD: Starting Core.\r\n");

    volatile hcd_usb_register_t usb;
    mmio_read_in(HCD_USB, &usb, 1);
    __dmb();
    usb.ulpi_drive_external_vbus = false;
    usb.ts_dline_pulse_enable = false;
    __dmb();
    mmio_write_out(HCD_USB, &usb, 1);

    // Reset HCD
    if ((result = hcd_reset()) != OK)
    {
        debug_printf("Failed to reset HCD.\r\n");
        DEBUG_END();
        return result;
    }

    // Initialize PHY
    if (!phy_initialized)
    {
        debug_printf("HCD: One time phy initialisation.\r\n");
        mmio_read_in(HCD_USB, &usb, 1);
        __dmb();
        usb.mode_select = true; // 1 = UTMI
        debug_printf("HCD: Interface: UTMI+.\r\n");
        usb.ts_dline_pulse_enable = false;
        __dmb();
        mmio_write_out(HCD_USB, &usb, 1);
        phy_initialized = true;
        hcd_reset();
    }

    // Configure UPLI FSLS
    mmio_read_in(HCD_USB, &usb, 1);
    __dmb();
    if (hardware.high_speed_physical == UTMI && hardware.full_speed_physical == DEDICATED)
    {
        debug_printf("HCD: ULPI FSLS configuration: enabled.\r\n");
        usb.ulpi_fsls = true;
        usb.ulpi_clk_sus_m = true;
    }
    else
    {
        debug_printf("HCD: ULPI FSLS configuration: disabled.\r\n");
        usb.ulpi_fsls = false;
        usb.ulpi_clk_sus_m = false;
    }
    __dmb();
    mmio_write_out(HCD_USB, &usb, 1);

    // Configure DMA
    volatile hcd_ahb_t ahb;
    mmio_read_in(HCD_USB_GAHBCFG, &ahb, 1);
    __dmb();
    ahb.dma_enable = true;
    ahb.dma_remainder_mode = false; // 0 = incremental
    __dmb();
    mmio_write_out(HCD_USB_GAHBCFG, &ahb, 1);
    debug_printf("HCD: DMA configuration: enabled.\r\n");

    // Configure HNP/SRP
    mmio_read_in(HCD_USB, &usb, 1);
    __dmb();
    switch (hardware.operating_mode)
    {
        case HNP_SNP_CAPABLE:
            debug_printf("HCD: HNP/SRP configuration: HNP, SRP.\r\n");
            usb.hnp_capable = true;
            usb.srp_capable = true;
            break;
        case SRP_ONLY_CAPABLE:
        case SRP_CAPABLE_DEVICE:
        case SRP_CAPABLE_HOST:
            debug_printf("HCD: HNP/SRP configuration: SRP.\r\n");
            usb.hnp_capable = false;
            usb.srp_capable = true;
            break;
        case NO_HNP_SNP_CAPABLE:
        case NO_SRP_CAPABLE_DEVICE:
        case NO_SRP_CAPABLE_HOST:
            debug_printf("HCD: HNP/SRP configuration: none.\r\n");
            usb.hnp_capable = false;
            usb.srp_capable = false;
            break;
    }
    __dmb();
    mmio_write_out(HCD_USB, &usb, 1);
    debug_printf("HCD: Core started.\r\n");

    // Start host
    debug_printf("HCD: Starting host.\r\n");
    __dmb();
    mmio_write(HCD_POWER, 0);

    // Configure clock rate
    volatile hcd_host_configuration_t host_config;
    mmio_read_in(HCD_HOST_CONFIG, &host_config, 1);
    __dmb();
    if ((hardware.high_speed_physical == ULPI)
        && (hardware.full_speed_physical == DEDICATED)
        && usb.ulpi_fsls)
    {
        debug_printf("HCD: Host clock: 48MHz.\r\n");
        host_config.clock_rate = CLOCK_48_MHZ;
    }
    else
    {
        debug_printf("HCD: Host clock: 30-60MHz.\r\n");
        host_config.clock_rate = CLOCK_30_60_MHZ;
    }
    __dmb();
    mmio_write_out(HCD_HOST_CONFIG, &host_config, 1);

    // Configure FSLS only
    mmio_read_in(HCD_HOST_CONFIG, &host_config, 1);
    __dmb();
    host_config.fsls_only = true;
    __dmb();
    mmio_write_out(HCD_HOST_CONFIG, &host_config, 1);

    // Configure DMA description
    // nothing changes here so not sure why the register is written out again
    volatile uint32_t vendor_id = mmio_read(HCD_VENDOR_ID);
    mmio_read_in(HCD_HOST_CONFIG, &host_config, 1);
    __dmb();
    if (host_config.enable_dma_descriptor == (hardware.dma_description && (vendor_id & 0xfff) >= 0x90a))
    {
        debug_printf("HCD: DMA descriptor: enabled.\r\n");
    }
    else
    {
        debug_printf("HCD: DMA descriptor: disabled.\r\n");
    }
    __dmb();
    mmio_write_out(HCD_HOST_CONFIG, &host_config, 1);

    // FIFO configuration
    debug_printf("HCD: FIFO configuration: Total=%p Rx=%p NPTx=%p PTx=%p.\r\n",
                 RECEIVE_FIFO_SIZE + NON_PERIODIC_FIFO_SIZE + PERIODIC_FIFO_SIZE,
                 RECEIVE_FIFO_SIZE, NON_PERIODIC_FIFO_SIZE, PERIODIC_FIFO_SIZE);
    volatile hcd_fifo_size_t receive_size, non_periodic_fifo_size, periodic_host_fifo_size;
    mmio_read_in(HCD_CORE_RECEIVE_SIZE, &receive_size, 1);
    mmio_read_in(HCD_CORE_NON_PERIODIC_FIFO_SIZE, &non_periodic_fifo_size, 1);
    mmio_read_in(HCD_CORE_PERIODIC_HOST_FIFO_SIZE, &periodic_host_fifo_size, 1);
    __dmb();
    receive_size.depth = RECEIVE_FIFO_SIZE;
    non_periodic_fifo_size.start_address = RECEIVE_FIFO_SIZE;
    non_periodic_fifo_size.depth = NON_PERIODIC_FIFO_SIZE;
    periodic_host_fifo_size.start_address = RECEIVE_FIFO_SIZE + NON_PERIODIC_FIFO_SIZE;
    periodic_host_fifo_size.depth = PERIODIC_FIFO_SIZE;
    __dmb();
    mmio_write_out(HCD_CORE_RECEIVE_SIZE, &receive_size, 1);
    mmio_write_out(HCD_CORE_NON_PERIODIC_FIFO_SIZE, &non_periodic_fifo_size, 1);
    mmio_write_out(HCD_CORE_PERIODIC_HOST_FIFO_SIZE, &periodic_host_fifo_size, 1);

    volatile hcd_otg_control_t otg_control;
    mmio_read_in(HCD_CORE_OTG_CONTROL, &otg_control, 1);
    __dmb();
    otg_control.host_set_hnp_enable = true;
    __dmb();
    mmio_write_out(HCD_CORE_OTG_CONTROL, &otg_control, 1);
    debug_printf("HCD: Set HNP: enabled.\r\n");

    // Flush Tx FIFOs
    if ((result = hcd_transmit_fifo_flush(FLUSH_ALL)) != OK)
    {
        debug_printf("HCD: Failed to flush Tx FIFOs.\r\n");
        DEBUG_END();
        return result;
    }

    // Flush Rx FIFO
    if ((result = hcd_receive_fifo_flush()) != OK)
    {
        debug_printf("HCD: Failed to flush Rx FIFO.\r\n");
        DEBUG_END();
        return result;
    }

    // Halt channels to put them into a known state
    if (!host_config.enable_dma_descriptor)
    {
        // This block seems to be the same as the block below, but without the read back / status check
        for (uint32_t channel = 0; channel < hardware.host_channel_count; channel++)
        {
            hcd_host_channel_characteristic_t characteristic;
            mmio_read_in(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
            __dmb();
            characteristic.enable = false;
            characteristic.disable = true;
            characteristic.endpoint_direction = DEVICE_TO_HOST;
            __dmb();
            mmio_write_out(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
        }

        // Check channel status (both 'enable' and 'disable' are true here, is that right?)
        for (uint32_t channel = 0; channel < hardware.host_channel_count; channel++)
        {
            hcd_host_channel_characteristic_t characteristic;
            mmio_read_in(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
            __dmb();
            characteristic.enable = true;
            characteristic.disable = true;
            characteristic.endpoint_direction = DEVICE_TO_HOST;
            __dmb();
            mmio_write_out(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
            uint32_t timeout = 0;
            do {
                mmio_read_in(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
                __dmb();
                if (timeout++ >= DEVICE_RESET_TIMEOUT)
                {
                    debug_printf("HCD: Unable to clear halt on channel %d. (%p)\r\n", channel, characteristic);
                    break;
                }
            } while(characteristic.enable);
        }
    }

    // Power up port
    hcd_host_port_t host_port;
    mmio_read_in(HCD_HOST_PORT, &host_port, 1);
    __dmb();
    if (!host_port.power)
    {
        debug_printf("HCD: Powering up port.\r\n");
        host_port.power = true;
        mmio_write_out(HCD_HOST_PORT, &host_port, 1);
    }

    debug_printf("HCD: Reset port.\r\n");
    mmio_read_in(HCD_HOST_PORT, &host_port, 1);
    __dmb();
    host_port.reset = true;
    mmio_write_out(HCD_HOST_PORT, &host_port, 1);
    delay(50000); // Hopefully this is long enough...
    host_port.reset = false;
    mmio_write_out(HCD_HOST_PORT, &host_port, 1);
    mmio_read_in(HCD_HOST_PORT, &host_port, 1);

    debug_printf("HCD: Successfully started.\r\n");

    DEBUG_END();
    return OK;
}

static usb_call_result_t hcd_prepare_channel(usb_device_t *device, uint8_t channel, uint32_t length, packet_id_t type,
                                             usb_pipe_address_t *pipe)
{
    DEBUG_START("hcd_prepare_channel");

    hcd_hardware_t hardware;
    mmio_read_in(HCD_HARDWARE, &hardware, 4);
    if (channel > hardware.host_channel_count)
    {
        debug_printf("HCD: Channel %d is not available on this host.\r\n", channel);
        DEBUG_END();
        return ERROR_ARGUMENT;
    }

    // Clear all existing interrupts.
    mmio_write(HCD_HOST_CHANNEL_INTERRUPT(channel), ALL_BITS);

    // Program the channel.
    hcd_host_channel_characteristic_t characteristic = {
        .device_address = pipe->device,
        .endpoint_number = pipe->endpoint,
        .endpoint_direction = pipe->direction,
        .low_speed = (pipe->speed == USB_SPEED_LOW),
        .type = pipe->type,
        .maximum_packet_size = packet_size_to_number(pipe->max_size),
        .enable = false,
        .disable = false
    };
    mmio_write_out(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);

    // Clear split control. (added '&& device->parent' here to prevent any unexpected NPE)
    hcd_host_channel_split_control_t split_control = {};
    if (pipe->speed != USB_SPEED_HIGH && device->parent) {
        split_control.split_enable = true;
        split_control.hub_address = device->parent->number;
        split_control.port_address = device->port_number;
    }
    mmio_write_out(HCD_HOST_CHANNEL_SPLIT_CONTROL(channel), &split_control, 1);

    // Transfer size
    int max_packet_size = (pipe->speed == USB_SPEED_LOW) ? 8 : characteristic.maximum_packet_size;
    hcd_host_channel_transfer_size_t transfer_size = {
        .transfer_size = length,
        .packet_count = (length + max_packet_size - 1) / max_packet_size,
        .packet_id = type
    };
    mmio_write_out(HCD_HOST_CHANNEL_TRANSFER_SIZE(channel), &transfer_size, 1);

    DEBUG_END();
    return OK;
}

void hcd_transmit_channel(uint8_t channel, void *buffer)
{
    DEBUG_START("hcd_transmit_channel");

    hcd_host_channel_split_control_t split_control;
    mmio_read_in(HCD_HOST_CHANNEL_SPLIT_CONTROL(channel), &split_control, 1);
    split_control.complete_split = false;
    mmio_write_out(HCD_HOST_CHANNEL_SPLIT_CONTROL(channel), &split_control, 1);

    if (((uint32_t)buffer & 3) != 0)
    {
        debug_printf("HCD: Transfer buffer %p is not DWORD aligned. Ignored, but dangerous.\r\n", buffer);
    }
    mmio_write(HCD_HOST_CHANNEL_DMA_ADDRESS(channel), (uint32_t)buffer);

    hcd_host_channel_characteristic_t characteristic;
    mmio_read_in(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
    characteristic.packets_per_frame = 1;
    characteristic.enable = true;
    characteristic.disable = false;
    mmio_write_out(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);

    DEBUG_END();
}

static usb_call_result_t hcd_channel_interrupt_to_error(usb_device_t *device, hcd_host_channel_interrupt_t interrupts,
                                                        bool is_complete)
{
    DEBUG_START("hcd_channel_interrupt_to_error");

    usb_call_result_t result = OK;

    if (interrupts.ahb_error) {
        device->error = USB_TRANSFER_ERROR_AHB_ERROR;
        debug_printf("HCD: AHB error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.stall) {
        device->error = USB_TRANSFER_ERROR_STALL;
        debug_printf("HCD: Stall error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.negative_acknowledgement) {
        device->error = USB_TRANSFER_ERROR_NO_ACKNOWLEDGE;
        debug_printf("HCD: NAK error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (!interrupts.acknowledgement) {
        debug_printf("HCD: Transfer was not acknowledged.\r\n");
        result = ERROR_TIMEOUT;
    }
    if (interrupts.not_yet) {
        device->error = USB_TRANSFER_ERROR_NOT_YET_ERROR;
        debug_printf("HCD: Not yet error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.babble_error) {
        device->error = USB_TRANSFER_ERROR_BABBLE;
        debug_printf("HCD: Babble error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.frame_overrun) {
        device->error = USB_TRANSFER_ERROR_BUFFER_ERROR;
        debug_printf("HCD: Frame overrun in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.data_toggle_error) {
        device->error = USB_TRANSFER_ERROR_BIT_ERROR;
        debug_printf("HCD: Data toggle error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.transaction_error) {
        device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
        debug_printf("HCD: Transaction error in transfer.\r\n");
        DEBUG_END();
        return ERROR_DEVICE;
    }
    if (interrupts.transfer_complete && is_complete) {
        device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
        debug_printf("HCD: Transfer did not complete.\r\n");
        result = ERROR_TIMEOUT;
    }

    DEBUG_END();
    return result;
}

static usb_call_result_t hcd_channel_send_wait_one(usb_device_t *device, usb_pipe_address_t *pipe, uint8_t channel,
                                                   void *buffer, uint32_t buffer_length, uint32_t buffer_offset,
                                                   usb_device_request_t *request)
{
    DEBUG_START("hcd_channel_send_wait_one");

    usb_call_result_t result;
    hcd_host_channel_characteristic_t characteristic;
    hcd_host_channel_interrupt_t interrupt;
    hcd_host_channel_split_control_t split_control;
    hcd_host_channel_transfer_size_t transfer_size;
    uint32_t timeout, tries, global_tries, actual_tries;

    for (global_tries = 0, actual_tries = 0; global_tries < 3 && actual_tries < 10; global_tries++, actual_tries++)
    {
        mmio_write(HCD_HOST_CHANNEL_INTERRUPT(channel), ALL_BITS);
        mmio_read_in(HCD_HOST_CHANNEL_TRANSFER_SIZE(channel), &transfer_size, 1);
        mmio_read_in(HCD_HOST_CHANNEL_SPLIT_CONTROL(channel), &split_control, 1);

        hcd_transmit_channel(channel, (uint8_t *)buffer + buffer_offset);

        timeout = 0;
        while(true) {
            if (timeout++ == REQUEST_TIMEOUT)
            {
                debug_printf("HCD: Request to %s has timed out.\r\n", usb_get_description(device));
                device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
                DEBUG_END();
                return ERROR_TIMEOUT;
            }
            mmio_read_in(HCD_HOST_CHANNEL_INTERRUPT(channel), &interrupt, 1);
            if (interrupt.halt) break;
            delay(10);
        }
        mmio_read_in(HCD_HOST_CHANNEL_TRANSFER_SIZE(channel), &transfer_size, 1);

        if (split_control.split_enable)
        {
            if (interrupt.acknowledgement)
            {
                for (tries = 0; tries < 3; tries++)
                {
                    mmio_write(HCD_HOST_CHANNEL_INTERRUPT(channel), ALL_BITS);
                    mmio_read_in(HCD_HOST_CHANNEL_SPLIT_CONTROL(channel), &split_control, 1);
                    split_control.complete_split = true;
                    mmio_write_out(HCD_HOST_CHANNEL_SPLIT_CONTROL(channel), &split_control, 1);
                    mmio_read_in(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);
                    characteristic.enable = true;
                    characteristic.disable = false;
                    mmio_write_out(HCD_HOST_CHANNEL_CHARACTERISTIC(channel), &characteristic, 1);

                    timeout = 0;
                    while(true) {
                        if (timeout++ == REQUEST_TIMEOUT)
                        {
                            debug_printf("HCD: Request split completion to %s has timed out.\r\n",
                                         usb_get_description(device));
                            device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
                            DEBUG_END();
                            return ERROR_TIMEOUT;
                        }
                        mmio_read_in(HCD_HOST_CHANNEL_INTERRUPT(channel), &interrupt, 1);
                        if (interrupt.halt) break;
                        delay(100);
                    }
                    if (interrupt.not_yet) break;
                } // end for (tries = 0; tries < 3; tries++)

                if (tries == 3 || interrupt.negative_acknowledgement || interrupt.transaction_error)
                {
                    delay(25000);
                    continue;
                }

                if ((result = hcd_channel_interrupt_to_error(device, interrupt, false)) != OK)
                {
                    debug_printf("HCD: Control message to %x: %08x %08x.\r\n", *(uint32_t *)pipe,
                                 (uint32_t *)request, (uint32_t *)request + 1);
                    debug_printf("HCD: Request split completion to %s failed.\r\n", usb_get_description(device));
                    DEBUG_END();
                    return result;
                }
            } // end if (interrupt.acknowledgement)
            else if (interrupt.negative_acknowledgement || interrupt.transaction_error)
            {
                global_tries--;
                delay(25000);
                continue;
            }
        } // end if (split_control.split_enable)
        else
        {
            if (hcd_channel_interrupt_to_error(device, interrupt, !split_control.split_enable))
            {
                debug_printf("HCD: Control message to %x: %08x %08x.\r\n", *(uint32_t *)pipe,
                             (uint32_t *)request, (uint32_t *)request + 1);
                debug_printf("HCD: Request split completion to %s failed.\r\n", usb_get_description(device));
                DEBUG_END();
                return ERROR_RETRY;
            }
        }

        break;
    } // end for (global_tries = 0, actual_tries = 0; global_tries < 3 && actual_tries < 10; global_tries++, actual_tries++)

    if (global_tries == 3 || actual_tries == 10)
    {
        debug_printf("HCD: Request to %s has failed 3 times.\r\n", usb_get_description(device));
        if ((result = hcd_channel_interrupt_to_error(device, interrupt, !split_control.split_enable)) != OK)
        {
            debug_printf("HCD: Control message to %x: %08x %08x.\r\n", *(uint32_t *)pipe,
                         (uint32_t *)request, (uint32_t *)request + 1);
            debug_printf("HCD: Request to %s failed.\r\n", usb_get_description(device));
            DEBUG_END();
            return result;
        }
        device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
        return ERROR_TIMEOUT;
    }
    DEBUG_END();
    return OK;
}

static usb_call_result_t hcd_channel_send_wait(usb_device_t *device, usb_pipe_address_t *pipe, uint8_t channel,
                                               void *buffer, uint32_t buffer_length, usb_device_request_t *request,
                                               packet_id_t packet_id)
{
    DEBUG_START("hcd_channel_send_wait");

    usb_call_result_t result;
    int tries = 1;
    int packets = 0;
    bool prepare_done = false;
    hcd_host_channel_transfer_size_t transfer_size;
    do {
        if (!prepare_done)
        {
            if ((result = hcd_prepare_channel(device, channel, buffer_length, packet_id, pipe)) != OK)
            {
                device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
                debug_printf("HCD: Could not prepare data channel to %s.\r\n", usb_get_description(device));
                DEBUG_END();
                return result;
            }
            prepare_done = true;
        }

        int transfer = 0;
        mmio_read_in(HCD_HOST_CHANNEL_TRANSFER_SIZE(channel), &transfer_size, 1);
        packets = transfer_size.packet_count;
        result = hcd_channel_send_wait_one(device, pipe, channel, buffer, buffer_length, transfer, request);
        // If we can retry...
        if (result == ERROR_RETRY)
        {
            if (tries == 3)
            {
                debug_printf("HCD: Failed to send to %s after three attempts.\r\n", usb_get_description(device));
                DEBUG_END();
                return ERROR_TIMEOUT;
            }
            tries++;
            prepare_done = false;
            continue;
        }
        if (result)
        {
            DEBUG_END();
            return result;
        }

        mmio_read_in(HCD_HOST_CHANNEL_TRANSFER_SIZE(channel), &transfer_size, 1);
        transfer = buffer_length - transfer_size.transfer_size;
        if (packets == transfer_size.packet_count) break;
    } while (!prepare_done || transfer_size.packet_count > 0);

    if (packets == transfer_size.packet_count)
    {
        device->error = USB_TRANSFER_ERROR_CONNECTION_ERROR;
        debug_printf("HCD: Transfer to %s got stuck.\r\n", usb_get_description(device));
        DEBUG_END();
        return ERROR_DEVICE;
    }

    if (tries > 1)
    {
        debug_printf("HCD: Transfer to %s succeeded on attempt %d/3.\r\n", usb_get_description(device), tries);
    }

    DEBUG_END();
    return OK;
}

usb_call_result_t hcd_submit_control_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer,
                                             uint32_t buffer_length, usb_device_request_t *request)
{
    DEBUG_START("hcd_submit_control_message");

    hcd_host_channel_transfer_size_t transfer_size;
    usb_pipe_address_t temp_pipe;
    if (pipe.device == root_hub_device_number)
    {
        usb_call_result_t result = hcd_process_root_hub_message(device, pipe, buffer, buffer_length, request);
        DEBUG_END();
        return result;
    }

    // Set device state
    device->error = USB_TRANSFER_ERROR_PROCESSING;
    device->last_transfer = 0;

    // Setup
    temp_pipe.speed = pipe.speed;
    temp_pipe.device = pipe.device;
    temp_pipe.endpoint = pipe.endpoint;
    temp_pipe.max_size = pipe.max_size;
    temp_pipe.type = USB_TRANSFER_TYPE_CONTROL;
    temp_pipe.direction = HOST_TO_DEVICE;
    if (hcd_channel_send_wait(device, &temp_pipe, 0, request, 8, request, PACKET_ID_SETUP))
    {
        debug_printf("HCD: Could not send SETUP to %s.\r\n", usb_get_description(device));
        return OK;
    }

    // Data
    if (buffer != NULL)
    {
        if (pipe.direction == HOST_TO_DEVICE)
        {
            memcpy(data_buffer, buffer, buffer_length);
        }
        temp_pipe.speed = pipe.speed;
        temp_pipe.device = pipe.device;
        temp_pipe.endpoint = pipe.endpoint;
        temp_pipe.max_size = pipe.max_size;
        temp_pipe.type = USB_TRANSFER_TYPE_CONTROL;
        temp_pipe.direction = pipe.direction;
        if (hcd_channel_send_wait(device, &temp_pipe, 0, data_buffer, buffer_length, request, PACKET_ID_DATA1))
        {
            debug_printf("HCD: Could not send DATA to %s.\r\n", usb_get_description(device));
            return OK;
        }

        mmio_read_in(HCD_HOST_CHANNEL_TRANSFER_SIZE(0), &transfer_size, 1);
        if (pipe.direction == DEVICE_TO_HOST)
        {
            if (transfer_size.transfer_size <= buffer_length)
            {
                device->last_transfer = buffer_length - transfer_size.transfer_size;
            }
            else
            {
                debug_printf("HCD: Weird transfer.. %d/%d bytes received.\r\n",
                             transfer_size.transfer_size, buffer_length);
                debug_printf("HCD: Message %08x %08x %08x %08x ...\r\n",
                             data_buffer, data_buffer + 4, data_buffer + 8, data_buffer + 12);
                device->last_transfer = buffer_length;
            }
            memcpy(buffer, data_buffer, device->last_transfer);
        }
        else
        {
            device->last_transfer = buffer_length;
        }
    }

    // Status
    temp_pipe.speed = pipe.speed;
    temp_pipe.device = pipe.device;
    temp_pipe.endpoint = pipe.endpoint;
    temp_pipe.max_size = pipe.max_size;
    temp_pipe.type = USB_TRANSFER_TYPE_CONTROL;
    temp_pipe.direction = (buffer_length == 0 || pipe.direction == HOST_TO_DEVICE) ? DEVICE_TO_HOST : HOST_TO_DEVICE;

    if (hcd_channel_send_wait(device, &temp_pipe, 0, data_buffer, 0, request, PACKET_ID_DATA1))
    {
        debug_printf("HCD: Could not send STATUS to %s.\r\n", usb_get_description(device));
        return OK;
    }

    mmio_read_in(HCD_HOST_CHANNEL_TRANSFER_SIZE(0), &transfer_size, 1);
    if (transfer_size.transfer_size != 0)
    {
        debug_printf("HCD: Warning: non-zero status transfer! (%d).\r\n", transfer_size.transfer_size);
    }

    device->error = USB_TRANSFER_OK;

    DEBUG_END();
    return OK;
}