/*
 * mem_size.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/board.h"
#include "../../include/kernel/mailbox.h"

volatile size_t peripheral_base;

void discover_peripheral_base(uint32_t board_id)
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
