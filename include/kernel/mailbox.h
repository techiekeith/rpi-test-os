/*
 * mailbox.h
 */

#pragma once

#include "../common/stdint.h"
#include "peripheral.h"

/*
 * See https://github.com/raspberrypi/firmware/wiki/Mailboxes
 */
#define MAILBOX_BASE                (peripheral_base + MAILBOX_OFFSET)
#define MAIL0_READ                  (MAILBOX_BASE + 0x00)
#define MAIL0_STATUS                (MAILBOX_BASE + 0x18)
#define MAIL1_WRITE                 (MAILBOX_BASE + 0x20)
#define MAIL1_STATUS                (MAILBOX_BASE + 0x38)
#define POWER_MANAGEMENT_CHANNEL    0
#define FRAMEBUFFER_CHANNEL         1
#define VIRTUAL_UART_CHANNEL        2
#define VCHIQ_CHANNEL               3
#define LED_CHANNEL                 4
#define BUTTON_CHANNEL              5
#define TOUCH_SCREEN_CHANNEL        6
#define PROPERTY_CHANNEL            8
#define ARM_TO_VC_PROPERTY_CHANNEL  PROPERTY_CHANNEL
#define VC_TO_ARM_PROPERTY_CHANNEL  9

typedef union mail_message
{
    struct
    {
        uint8_t channel: 4;
        uint32_t data: 28;
    };
    uint32_t as_int;
} mail_message_t;

typedef union mail_status
{
    struct
    {
        uint32_t reserved: 30;
        uint8_t empty: 1;
        uint8_t full: 1;
    };
    uint32_t as_int;
} mail_status_t;

/*
 * A property message can either be a request, or a response, and a response can be successful or an error
 */
typedef enum
{
    REQUEST = 0x00000000,
    RESPONSE_SUCCESS = 0x80000000,
    RESPONSE_ERROR = 0x80000001
} buffer_req_res_code_t;


/*
 * A buffer that holds many property messages.
 * The last tag must be a 4 byte zero, and then padding to make the whole thing 4 byte aligned
 */
typedef struct
{
    uint32_t size;                      // Size includes the size itself
    buffer_req_res_code_t req_res_code;
    uint32_t tags[1];                    // A concatenated sequence of tags. will use overrun to make large enough
} property_message_buffer_t;

/**
 * A message is identified by a tag. These are some of the possible tags
 */
typedef enum
{
    NULL_TAG = 0,
    VC_GET_FIRMWARE_REVISION = 0x00000001,
    HW_GET_BOARD_MODEL = 0x00010001,
    HW_GET_BOARD_REVISION = 0x00010002,
    HW_GET_BOARD_MAC_ADDRESS = 0x00010003,
    HW_GET_BOARD_SERIAL = 0x00010004,
    HW_GET_ARM_MEMORY = 0x00010005,
    HW_GET_VIDEOCORE_MEMORY = 0x00010006,
    HW_GET_CLOCKS = 0x00010007,
    CFG_GET_COMMAND_LINE = 0x00050001,
    DMA_GET_CHANNELS = 0x00060001,
    PWR_GET_STATE = 0x00020001,
    PWR_GET_TIMING = 0x00020002,
    PWR_SET_STATE = 0x00028001,
    CLK_GET_STATE = 0x00030001,
    CLK_SET_STATE = 0x00038001,
    CLK_GET_RATE = 0x00030002,
    CLK_SET_RATE = 0x00038002,
    CLK_GET_MAX_RATE = 0x00030004,
    CLK_GET_MIN_RATE = 0x00030007,
    CLK_GET_TURBO = 0x00030009,
    CLK_SET_TURBO = 0x00038009,
    CLK_GET_LED_STATUS = 0x00030041,
    CLK_TEST_LED_STATUS = 0x00034041,
    CLK_SET_LED_STATUS = 0x00038041,
    CLK_GET_RATE_MEASURED = 0x00030047,
    CLK_GET_VOLTAGE = 0x00030003,
    CLK_SET_VOLTAGE = 0x00038003,
    CLK_GET_MAX_VOLTAGE = 0x00030005,
    CLK_GET_MIN_VOLTAGE = 0x00030008,
    CLK_GET_TEMPERATURE = 0x00030006,
    CLK_GET_MAX_TEMPERATURE = 0x0003000a,
    CLK_ALLOCATE_MEMORY = 0x0003000c,
    CLK_LOCK_MEMORY = 0x0003000d,
    CLK_UNLOCK_MEMORY = 0x0003000e,
    CLK_RELEASE_MEMORY = 0x0003000f,
    CLK_EXECUTE_CODE = 0x00030010,
    CLK_GET_DISPMANX_RESOURCE_MEM_HANDLE = 0x00030014,
    CLK_GET_EDID_BLOCK = 0x00030020,
    FB_ALLOCATE_BUFFER = 0x00040001,
    FB_RELEASE_BUFFER = 0x00048001,
    FB_BLANK_SCREEN = 0x00040002,
    FB_GET_PHYSICAL_DISPLAY_SIZE = 0x00040003,
    FB_TEST_PHYSICAL_DISPLAY_SIZE = 0x00044003,
    FB_SET_PHYSICAL_DISPLAY_SIZE = 0x00048003,
    FB_GET_VIRTUAL_DISPLAY_SIZE = 0x00040004,
    FB_TEST_VIRTUAL_DISPLAY_SIZE = 0x00044004,
    FB_SET_VIRTUAL_DISPLAY_SIZE = 0x00048004,
    FB_GET_DEPTH = 0x00040005,
    FB_TEST_DEPTH = 0x00044005,
    FB_SET_DEPTH = 0x00048005,
    FB_GET_PIXEL_ORDER = 0x00040006,
    FB_TEST_PIXEL_ORDER = 0x00044006,
    FB_SET_PIXEL_ORDER = 0x00048006,
    FB_GET_ALPHA_MODE = 0x00040007,
    FB_TEST_ALPHA_MODE = 0x00044007,
    FB_SET_ALPHA_MODE = 0x00048007,
    FB_GET_PITCH = 0x00040008,
    FB_GET_VIRTUAL_OFFSET = 0x00040009,
    FB_TEST_VIRTUAL_OFFSET = 0x00044009,
    FB_SET_VIRTUAL_OFFSET = 0x00048009,
    FB_GET_OVERSCAN = 0x0004000a,
    FB_TEST_OVERSCAN = 0x0004400a,
    FB_SET_OVERSCAN = 0x0004800a,
    FB_GET_PALETTE = 0x0004000b,
    FB_TEST_PALETTE = 0x0004400b,
    FB_SET_PALETTE = 0x0004800b,
    FB_SET_CURSOR_INFO = 0x00048010,
    FB_SET_CURSOR_STATE = 0x00048011,
    VC_SET_SCREEN_GAMMA_PI3 = 0x00008012,
} property_tag_t;

/**
 * Structure for mailbox methods.
 */
typedef struct {
    char *method_name;
    property_tag_t tag;
    uint32_t request_size;
    uint32_t response_size;
} mailbox_method_t;

extern mailbox_method_t mailbox_methods[];

/**
 * Device IDs used in PWR_GET_STATE etc
 */
typedef enum
{
    DEVICE_SD_CARD = 0,
    DEVICE_UART0 = 1,
    DEVICE_UART1 = 2,
    DEVICE_USB_HOST_CONTROLLER = 3,
    DEVICE_I2C0 = 4,
    DEVICE_I2C1 = 5,
    DEVICE_I2C2 = 6,
    DEVICE_SPI = 7,
    DEVICE_CCP2TX = 8,
    DEVICE_PI4_UNKNOWN0 = 9,
    DEVICE_PI4_UNKNOWN1 = 10,
} device_id_t;

/**
 * Voltage IDs.
 */
typedef enum
{
    VOLTAGE_ID_RESERVED = 0,
    VOLTAGE_ID_CORE = 1,
    VOLTAGE_ID_SDRAM_C = 2,
    VOLTAGE_ID_SDRAM_P = 3,
    VOLTAGE_ID_SDRAM_I = 2,
} voltage_id_t;

/*
 * Pixel order values.
 */
typedef enum
{
    PIXEL_ORDER_BGR = 0,
    PIXEL_ORDER_RGB = 1,
} pixel_order_t;

/**
 * For each possible tag, we create a struct corresponding to the request value buffer, and the response value buffer
 */

typedef struct
{
    void *base_address;
    uint32_t size;
} memory_block_t;

typedef struct
{
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
    uint8_t b5;
} __attribute__ ((packed)) hw_board_mac_address_t;

typedef struct
{
    void *fb_addr;
    uint32_t fb_size;
} fb_allocate_res_t;

typedef struct
{
    uint32_t width;
    uint32_t height;
} fb_screen_size_t;

typedef struct
{
    uint32_t start_index;
    uint32_t num_entries;
    uint32_t entry[256];
} fb_palette_t;

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t unused;
    uint32_t *pixels;
    uint32_t hotspot_x;
    uint32_t hotspot_y;
} fb_cursor_info_t;

typedef struct
{
    uint32_t enable;
    uint32_t x;
    uint32_t y;
    uint32_t flags;
} fb_cursor_state_t;

/*
 * The value buffer can be any one of these types
 */
typedef union
{
    memory_block_t memory_block;
    hw_board_mac_address_t mac_address;
    uint32_t fb_allocate_align;
    fb_allocate_res_t fb_allocate_res;
    fb_screen_size_t fb_screen_size;
    pixel_order_t pixel_order;
    fb_palette_t fb_palette;
    uint32_t fb_depth;
    uint32_t fb_pitch;
    fb_cursor_info_t fb_cursor_info;
    fb_cursor_state_t fb_cursor_state;
    uint32_t data[256];
} value_buffer_t;

/*
 * A message_buffer can contain any number of these
 */
typedef struct {
    property_tag_t proptag;
    value_buffer_t value_buffer;
} property_message_tag_t;


int mailbox_read_with_timeout(int channel, mail_message_t *data, int timeout);
mail_message_t mailbox_read(int channel);
void mailbox_send(mail_message_t msg, int channel);

/**
 * given an array of tags, will send all of the tags given, and will populate that array with the responses.
 * the given array should end with a "null tag" with the proptag field set to 0.
 * returns 0 on success
 */
mailbox_method_t *get_mailbox_method_by_tag(property_tag_t tag);
mailbox_method_t *get_mailbox_method_by_name(char *method_name);
int send_messages(property_message_tag_t *tags);
