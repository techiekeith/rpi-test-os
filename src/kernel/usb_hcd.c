/*
 * usb_hcd.c - USB Host Controller Device driver
 * Cribbed shamelessly from Alex Chadwick's work at https://github.com/Chadderz121/csud
 */

#include "../../include/kernel/io.h"
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/delay.h"
#include "../../include/kernel/mailbox.h"
#include "../../include/kernel/mmio.h"
#include "../../include/kernel/usb_hcd.h"

DEBUG_INIT("usb_hcd");

static bool phy_initialized = false;

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

int hcd_init()
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
    if ((vendor_id & 0xfffff000) == 0x4f542000) // 'OT2.x'
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
        return -1;
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
            return -1;
        }
        if (hardware.high_speed_physical == NOT_SUPPORTED)
        {
            debug_printf("HCD: High speed physical unsupported. Driver incompatible.\r\n");
            DEBUG_END();
            return -1;
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
        return -1;
    }

    DEBUG_END();
    return 0;
}

static int hcd_reset()
{
    DEBUG_START("hcd_reset");

    uint32_t timeout;
    volatile hcd_reset_t reset;

    debug_printf("HCD: Waiting for AHB master idle.\r\n");
    timeout = 0;
    do {
        mmio_read_in(HCD_RESET, &reset, 1);
        if (timeout++ >= 0x100000)
        {
            debug_printf("AHB Master Idle: %d\n", reset.ahb_master_idle);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return -1;
        }
    } while (!reset.ahb_master_idle);

    reset.core_soft = true;
    mmio_write_out(HCD_RESET, &reset, 1);

    debug_printf("HCD: Waiting for AHB master idle, and for core_soft to clear.\r\n");
    timeout = 0;
    do {
        mmio_read_in(HCD_RESET, &reset, 1);
        if (timeout++ >= 0x100000)
        {
            debug_printf("AHB Core Soft: %d\n", reset.core_soft);
            debug_printf("AHB Master Idle: %d\n", reset.ahb_master_idle);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return -1;
        }
    } while (reset.core_soft || !reset.ahb_master_idle);

    DEBUG_END();
    return 0;
}

static int hcd_transmit_fifo_flush(flush_number_t flush_number)
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
        if (timeout++ >= 0x100000)
        {
            debug_printf("Tx FIFO Flush: %d\n", reset.transmit_fifo_flush);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return -1;
        }
    } while (reset.transmit_fifo_flush);

    DEBUG_END();
    return 0;
}

static int hcd_receive_fifo_flush()
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
        if (timeout++ >= 0x100000)
        {
            debug_printf("Tx FIFO Flush: %d\n", reset.receive_fifo_flush);
            debug_printf("HCD: Device hang! (%p)\r\n", reset);
            DEBUG_END();
            return -1;
        }
    } while (reset.receive_fifo_flush);

    DEBUG_END();
    return 0;
}

int hcd_start()
{
    DEBUG_START("hcd_start");

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
    if (hcd_reset())
    {
        debug_printf("Failed to reset HCD.\r\n");
        DEBUG_END();
        return -1;
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
    mmio_read_in(HCD_CORE_RECEIVE_SIZE, &non_periodic_fifo_size, 1);
    mmio_read_in(HCD_CORE_RECEIVE_SIZE, &periodic_host_fifo_size, 1);
    __dmb();
    receive_size.depth = RECEIVE_FIFO_SIZE;
    non_periodic_fifo_size.start_address = RECEIVE_FIFO_SIZE;
    non_periodic_fifo_size.depth = NON_PERIODIC_FIFO_SIZE;
    periodic_host_fifo_size.start_address = RECEIVE_FIFO_SIZE + NON_PERIODIC_FIFO_SIZE;
    periodic_host_fifo_size.depth = PERIODIC_FIFO_SIZE;
    __dmb();
    mmio_write_out(HCD_CORE_RECEIVE_SIZE, &receive_size, 1);
    mmio_write_out(HCD_CORE_RECEIVE_SIZE, &non_periodic_fifo_size, 1);
    mmio_write_out(HCD_CORE_RECEIVE_SIZE, &periodic_host_fifo_size, 1);

    volatile hcd_otg_control_t otg_control;
    mmio_read_in(HCD_CORE_OTG_CONTROL, &otg_control, 1);
    __dmb();
    otg_control.host_set_hnp_enable = true;
    __dmb();
    mmio_write_out(HCD_CORE_OTG_CONTROL, &otg_control, 1);
    debug_printf("HCD: Set HNP: enabled.\r\n");

    // Flush Tx FIFOs
    if (hcd_transmit_fifo_flush(FLUSH_ALL))
    {
        debug_printf("HCD: Failed to flush Tx FIFOs.\r\n");
        DEBUG_END();
        return -1;
    }

    // Flush Rx FIFO
    if (hcd_receive_fifo_flush())
    {
        debug_printf("HCD: Failed to flush Rx FIFO.\r\n");
        DEBUG_END();
        return -1;
    }

    // Halt channels to put them into a known state
    if (!host_config.enable_dma_descriptor)
    {
        // This block seems to be the same as the block below, but without the read back / status check
        for (uint32_t channel = 0; channel < hardware.host_channel_count; channel++)
        {
            hcd_host_channel_characteristic_t characteristic;
            mmio_read_in(HCD_HOST_CHANNEL(channel), &characteristic, 1);
            __dmb();
            characteristic.enable = false;
            characteristic.disable = true;
            characteristic.endpoint_direction = DEVICE_TO_HOST;
            __dmb();
            mmio_write_out(HCD_HOST_CHANNEL(channel), &characteristic, 1);
        }

        // Check channel status (both 'enable' and 'disable' are true here, is that right?)
        for (uint32_t channel = 0; channel < hardware.host_channel_count; channel++)
        {
            hcd_host_channel_characteristic_t characteristic;
            mmio_read_in(HCD_HOST_CHANNEL(channel), &characteristic, 1);
            __dmb();
            characteristic.enable = true;
            characteristic.disable = true;
            characteristic.endpoint_direction = DEVICE_TO_HOST;
            __dmb();
            mmio_write_out(HCD_HOST_CHANNEL(channel), &characteristic, 1);
            uint32_t timeout = 0;
            do {
                mmio_read_in(HCD_HOST_CHANNEL(channel), &characteristic, 1);
                __dmb();
                if (timeout++ >= 0x100000)
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
    delay(50000000); // Hopefully this is long enough...
    host_port.reset = false;
    mmio_write_out(HCD_HOST_PORT, &host_port, 1);
    mmio_read_in(HCD_HOST_PORT, &host_port, 1);

    debug_printf("HCD: Successfully started.\r\n");

    DEBUG_END();
    return 0;
}

int hcd_submit_control_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer, uint32_t buffer_length,
                               usb_device_request_t *request)
{
    DEBUG_START("hcd_submit_control_message");

    // TODO
    debug_printf("TODO\r\n");

    DEBUG_END();
    return 0;
}