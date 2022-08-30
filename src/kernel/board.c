/*
 * board.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/common/string.h"
#include "../../include/kernel/board.h"
#include "../../include/kernel/mailbox.h"

size_t peripheral_base;
uint64_t serial_number;
uint8_t mac_address[6];

// OUIs: https://gitlab.com/wireshark/wireshark/-/raw/master/manuf
// - B8:27:EB for Raspberry Pi 1, 2 and 3
// - DC:A6:32 for Raspberry Pi 4 and 400
// - E4:5F:01 for later Raspberry Pi 4/400 models? https://forums.raspberrypi.com/viewtopic.php?t=312584

size_t get_peripheral_base(uint32_t board_id)
{
    // Discover the peripheral base address
    switch (board_id)
    {
        case BOARD_ID_RPI0_1:
            peripheral_base = PERIPHERAL_BASE_RPI0_1;
            break;
        case BOARD_ID_RPI2:
        case BOARD_ID_RPI3:
            peripheral_base = PERIPHERAL_BASE_RPI2_3;
            break;
        case BOARD_ID_RPI4:
            peripheral_base = PERIPHERAL_BASE_RPI4;
            break;
        default:
            peripheral_base = PERIPHERAL_BASE_RPI0_1;
            break;
    }
    return peripheral_base;
}

size_t get_mem_size() {
    size_t mem = 0;
    property_message_tag_t tags[3];
    tags[0].proptag = HW_GET_ARM_MEMORY;
    tags[1].proptag = HW_GET_VIDEOCORE_MEMORY;
    tags[2].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv < 0)
    {
        printf("Failed to read memory size via mailbox.\r\n");
    }
    else
    {
        printf("ARM memory: base address %p, size %p bytes.\r\n",
                     tags[0].value_buffer.memory_block.base_address,
                     tags[0].value_buffer.memory_block.size);
        printf("VideoCore memory: base address %p, size %p bytes.\r\n",
                     tags[1].value_buffer.memory_block.base_address,
                     tags[1].value_buffer.memory_block.size);
        mem = tags[0].value_buffer.memory_block.size + tags[1].value_buffer.memory_block.size;
        printf("Total memory size: %p bytes.\r\n", mem);
    }
    return mem;
}

uint64_t get_serial_number() {
    property_message_tag_t tags[2];
    tags[0].proptag = HW_GET_BOARD_SERIAL;
    tags[0].value_buffer.data[0] = 0xffffffff;
    tags[0].value_buffer.data[1] = 0xffffffff;
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv < 0)
    {
        printf("Failed to read serial number via mailbox (error code %d).\r\n", rv);
    }
    if (tags[0].value_buffer.data[0] == 0xffffffff && tags[0].value_buffer.data[1] == 0xffffffff)
    {
        printf("Serial number not set; assigning default serial number.\r\n");
        tags[0].value_buffer.data[0] = 0xc0ffee01;
        tags[0].value_buffer.data[1] = 0x10000000;
    }
    memcpy(&serial_number, &(tags[0].value_buffer), 8);
    printf("Serial number: %016llx.\r\n", serial_number);
    return serial_number;
}

uint8_t *get_mac_address() {
    property_message_tag_t tags[2];
    tags[0].proptag = HW_GET_BOARD_MAC_ADDRESS;
    tags[0].value_buffer.data[0] = 0xffffffff;
    tags[0].value_buffer.data[1] = 0xffffffff;
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv < 0)
    {
        printf("Failed to read MAC address via mailbox (error code %d).\r\n", rv);
    }
    if (tags[0].value_buffer.data[0] == 0xffffffff && tags[0].value_buffer.data[1] == 0xffffffff)
    {
        printf("MAC address not set; assigning MAC address based on serial number.\r\n");
        tags[0].value_buffer.data[0] = 0xc0adde | ((serial_number & 0xff0000) >> 8);
        tags[0].value_buffer.data[1] = ((serial_number & 0xff00) >> 8) | ((serial_number & 0xff) << 8);
    }
    memcpy(mac_address, &(tags[0].value_buffer), 6);
    printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X.\r\n", mac_address[0], mac_address[1],
           mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
    return mac_address;
}
