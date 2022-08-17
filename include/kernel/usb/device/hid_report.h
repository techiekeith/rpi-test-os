/*
 * hid_report.h
 */

#pragma once

#include "../../../common/stddef.h"
#include "../../../common/stdint.h"

/**
	\brief The possible types of hid reports.

	The possible hid reports defined in the USB HID 1.11 manual in 7.2.1.
*/
typedef enum {
    HID_REPORT_TYPE_INPUT = 1,
    HID_REPORT_TYPE_OUTPUT = 2,
    HID_REPORT_TYPE_FEATURE = 3
} hid_report_type_t;

typedef enum {
    TAG_MAIN_INPUT = 0x20,
    TAG_MAIN_OUTPUT = 0x24,
    TAG_MAIN_FEATURE = 0x2c,
    TAG_MAIN_COLLECTION = 0x28,
    TAG_MAIN_END_COLLECTION = 0x30,
    TAG_GLOBAL_USAGE_PAGE = 0x1,
    TAG_GLOBAL_LOGICAL_MINIMUM = 0x5,
    TAG_GLOBAL_LOGICAL_MAXIMUM = 0x9,
    TAG_GLOBAL_PHYSICAL_MINIMUM = 0xd,
    TAG_GLOBAL_PHYSICAL_MAXIMUM = 0x11,
    TAG_GLOBAL_UNIT_EXPONENT = 0x15,
    TAG_GLOBAL_UNIT = 0x19,
    TAG_GLOBAL_REPORT_SIZE = 0x1d,
    TAG_GLOBAL_REPORT_ID = 0x21,
    TAG_GLOBAL_REPORT_COUNT = 0x25,
    TAG_GLOBAL_PUSH = 0x29,
    TAG_GLOBAL_POP = 0x2d,
    TAG_LOCAL_USAGE = 0x2,
    TAG_LOCAL_USAGE_MINIMUM = 0x6,
    TAG_LOCAL_USAGE_MAXIMUM = 0xa,
    TAG_LOCAL_DESIGNATOR_INDEX = 0xe,
    TAG_LOCAL_DESIGNATOR_MINIMUM = 0x12,
    TAG_LOCAL_DESIGNATOR_MAXIMUM = 0x16,
    TAG_LOCAL_STRING_INDEX = 0x1e,
    TAG_LOCAL_STRING_MINIMUM = 0x22,
    TAG_LOCAL_STRING_MAXIMUM = 0x26,
    TAG_LOCAL_DELIMITER = 0x2a,
    TAG_LONG = 0x3f
} hid_report_tag_t;

/**
	\brief An item in an hid report descriptor.

	The hid report item structure defined in the USB HID 1.11 manual in
	6.2.2.2. There are two such types defined here, long and short. Since long
	is based upon short. So we can reuse the structure to this effect.
*/
typedef struct {
    uint8_t size: 2;
    hid_report_tag_t tag: 6;
} __attribute__ ((packed)) hid_report_item_t;

/**
	\brief A main input, output or feature in an hid report descriptor.

	The hid main input, output or feature item structure defined in the USB HID
	1.11 manual in 6.2.2.4.
*/
typedef struct {
    bool constant : 1; // Data=0,Constant=1 @0
    bool variable : 1; // Array=0,Variable=1 @1
    bool relative : 1; // Absolute=0,Relative=1 @2
    bool wrap : 1; // NoWrap=0,Wrap=1 @3
    bool non_linear : 1; // Linear=0,NonLinear=1 @4
    bool no_preferred : 1; // PreferredState=0,NoPreferred=1 @5
    bool is_null : 1; // NoNull=0,NullState=1 @6
    bool is_volatile : 1; // NonVolatile=0,Volatile=1 Inputs cannot be volatile @7
    bool buffered_bytes : 1; // BitField=0,BufferedBytes=1 @8
    uint32_t reserved_9_31 : 23; // @9
} __attribute__ ((__packed__)) hid_main_item_t;

/**
	\brief A main collection index in an hid report descriptor.

	The hid main collection index used in the collection item defined in the
	USB HID 1.11 manual in 6.2.2.4.
*/
typedef enum {
    HID_MC_PHYSICAL = 0,
    HID_MC_APPLICATION = 1,
    HID_MC_LOGICAL = 2,
    HID_MC_REPORT = 3,
    HID_MC_NAMED_ARRAY = 4,
    HID_MC_USAGE_SWITCH = 5,
    HID_MC_USAGE_MODIFIER = 6,
} hid_main_collection_t;

/**
	\brief Values of the hid page usage field in a report.

	Values that the hid page usage can take in a hid report descriptor. Defined
	in section 3 table 1 of the HID 1.11 usage tables.
*/
typedef enum {
    HID_UP_UNDEFINED = 0,
    HID_UP_GENERIC_DESKTOP_CONTROL = 1,
    HID_UP_SIMULATION_CONTROL = 2,
    HID_UP_VR_CONTROL = 3,
    HID_UP_SPORT_CONTROL = 4,
    HID_UP_GAME_CONTROL = 5,
    HID_UP_GENERIC_DEVICE_CONTROL = 6,
    HID_UP_KEYBOARD_CONTROL = 7,
    HID_UP_LED = 8,
    HID_UP_BUTTON = 9,
    HID_UP_ORDINAL = 10,
    HID_UP_TELEPHONY = 11,
    HID_UP_CONSUMER = 12,
    HID_UP_DIGITIZER = 13,
    HID_UP_PID_PAGE = 15,
    HID_UP_UNICODE = 16,
    HID_USAGE_PAGE = 0xffff,
} hid_usage_page_t;

/**
	\brief Values of the hid desktop page usage in a report.

	Values that usage numbers in the desktop page represent. Defined in
	section 4 table 6 of the HID 1.11 usage tables. Only items below 0x48 are
	included here for brevity.
*/
typedef enum {
    HID_UPD_DESKTOP_POINT = 1,
    HID_UPD_DESKTOP_MOUSE = 2,
    HID_UPD_DESKTOP_JOYSTICK = 4,
    HID_UPD_DESKTOP_GAME_PAD = 5,
    HID_UPD_DESKTOP_KEYBOARD = 6,
    HID_UPD_DESKTOP_KEYPAD = 7,
    HID_UPD_DESKTOP_MULTI_AXIS_CONTROLLER = 8,
    HID_UPD_DESKTOP_TABLE_PC_CONTROL = 9,
    HID_UPD_DESKTOP_X = 0x30,
    HID_UPD_DESKTOP_Y = 0x31,
    HID_UPD_DESKTOP_Z = 0x32,
    HID_UPD_DESKTOP_RX = 0x33,
    HID_UPD_DESKTOP_RY = 0x34,
    HID_UPD_DESKTOP_RZ = 0x35,
    HID_UPD_DESKTOP_SLIDER = 0x36,
    HID_UPD_DESKTOP_DIAL = 0x37,
    HID_UPD_DESKTOP_WHEEL = 0x38,
    HID_UPD_DESKTOP_HAT_SWITCH = 0x39,
    HID_UPD_DESKTOP_COUNTED_BUFFER = 0x3a,
    HID_UPD_DESKTOP_BYTE_COUNT = 0x3b,
    HID_UPD_DESKTOP_MOTION_WAKEUP = 0x3c,
    HID_UPD_DESKTOP_START = 0x3d,
    HID_UPD_DESKTOP_SELECT = 0x3e,
    HID_UPD_DESKTOP_VX = 0x40,
    HID_UPD_DESKTOP_VY = 0x41,
    HID_UPD_DESKTOP_VZ = 0x42,
    HID_UPD_DESKTOP_VBR_X = 0x43,
    HID_UPD_DESKTOP_VBR_Y = 0x44,
    HID_UPD_DESKTOP_VBR_Z = 0x45,
    HID_UPD_DESKTOP_V_NO = 0x46,
    HID_UPD_DESKTOP_FEATURE_NOTIFICATION = 0x47,
    HID_UPD_DESKTOP_RESOLUTION_MULTIPLIER = 0x48,
    HID_USAGE_PAGE_DESKTOP = 0xffff,
} hid_usage_page_desktop_t;

/**
	\brief Values of the hid keyboard page usage in a report.

	Values that usage numbers in the keyboard page represent. Defined in
	section 10 table 12 of the HID 1.11 usage tables. Key definitions are not
	included here, as this driver, being deliberately awkward, does not handle
	them.
*/
typedef enum {
    HID_UPK_KEYBOARD_ERROR_ROLLOVER = 1,
    HID_UPK_KEYBOARD_POST_FAIL = 2,
    HID_UPK_KEYBOARD_ERROR_UNDEFINED = 3,
    HID_UPK_KEYBOARD_LEFT_CONTROL = 0xe0,
    HID_UPK_KEYBOARD_LEFT_SHIFT = 0xe1,
    HID_UPK_KEYBOARD_LEFT_ALT = 0xe2,
    HID_UPK_KEYBOARD_LEFT_GUI = 0xe3,
    HID_UPK_KEYBOARD_RIGHT_CONTROL = 0xe4,
    HID_UPK_KEYBOARD_RIGHT_SHIFT = 0xe5,
    HID_UPK_KEYBOARD_RIGHT_ALT = 0xe6,
    HID_UPK_KEYBOARD_RIGHT_GUI = 0xe7,
    HID_USAGE_PAGE_KEYBOARD = 0xffff,
} hid_usage_page_keyboard_t;

/**
	\brief Values of the hid led page usage in a report.

	Values that usage numbers in the keyboard page represent. Defined in
	section 11 table 13 of the HID 1.11 usage tables.
*/
typedef enum {
    LED_NUMBER_LOCK = 1,
    LED_CAPS_LOCK = 2,
    LED_SCROLL_LOCK = 3,
    LED_COMPOSE = 4,
    LED_KANA = 5,
    LED_POWER = 6,
    LED_SHIFT = 7,
    LED_MUTE = 9,
    HID_USAGE_PAGE_LED = 0xffff,
} hid_usage_page_led_t;

/**
	\brief A full HID usage tag.

	A full HID usage with both the page and the tag represented in the order it
	can appear in HID report descriptors.
*/
typedef struct {
    union {
        hid_usage_page_desktop_t desktop : 16;
        hid_usage_page_keyboard_t keyboard : 16;
        hid_usage_page_led_t led : 16;
    };
    hid_usage_page_t page : 16;
} __attribute__ ((__packed__)) hid_full_usage_t;

typedef enum {
    SYSTEM_NONE = 0,
    /** Centimeter, Gram, Second, Kelvin, Ampere, Candela */
    STANDARD_LINEAR = 1,
    /** Radian, Gram, Second, Kelvin, Ampere, Candela */
    STANDARD_ROTATION = 2,
    /** Inch, Slug, Second, Fahrenheit, Ampere, Candela */
    ENGLISH_LINEAR = 3,
    /** Degree, Slug, Second, Fahrenheit, Ampere, Candela */
    ENGLISH_ROTATION = 4,
} unit_system_t;

/**
	\brief A HID field units declaration.

	The units of a HID field. This declaration allows the definition of many SI
	units.
*/
typedef struct {
    unit_system_t system : 4;
    int8_t length : 4;
    int8_t mass : 4;
    int8_t time : 4;
    int8_t temperature : 4;
    int8_t current : 4;
    int8_t luminous_intensity : 4;
    uint8_t reserved_28_31 : 4;
} __attribute__ ((__packed__)) hid_unit_t;

/**
	\brief A parsed report field, with value.

	A representation of a fully parsed report field complete with value for
	easy retrieval and setting of values.
*/
typedef struct {
    /** Size in bits of this field. For arrays, this is per element. */
    uint8_t size;
    /** Offset of this field into the report in bits */
    uint8_t offset;
    /** Array fields have a number of individual fields. */
    uint8_t count;
    /** Attributes of this field */
    hid_main_item_t attributes __attribute__((aligned(4)));
    /** Usage of this field. For array elements, this is the first usage, and
        it is assumed sequential values have sequential usage. */
    hid_full_usage_t usage;
    /** Usage of the physical connection this device is in, if present. */
    hid_full_usage_t physical_usage;
    /** The minimum value of this field. */
    int32_t logical_minimum;
    /** The maximum value of this field. */
    int32_t logical_maximum;
    /** The minimum physical quantity represented by this field. */
    int32_t physical_minimum;
    /** The maximum physical quantity represented by this field. */
    int32_t physical_maximum;
    /** The units of this field's physical quantity. */
    hid_unit_t unit;
    /** The base 10 exponenet of this field's physical quantity. */
    int32_t unit_exponent;
    /** Current value of this field. This is the logical value. The value is
        not sign extended. */
    union {
        uint8_t uint8;
        int8_t int8;
        uint16_t uint16;
        int16_t int16;
        uint32_t uint32;
        int32_t int32;
        bool boolean;
        void *pointer;
    } value;
} hid_parser_field_t;

/**
	\brief A parsed report, with values.

	A representation of a fully parsed report complete with value fields for
	easy retrieval and setting of values.
*/
typedef struct {
    /** Which report this is in the parser result. */
    uint8_t index;
    /** There can be multiple fields in each report */
    uint8_t field_count;
    /** Report can have an ID. If not we use 0. */
    uint8_t id;
    /** The type of this report. */
    hid_report_type_t type;
    /** Length of this report in bytes. */
    uint8_t report_length;
    /** The last report received (if not NULL). */
    uint8_t *report_buffer;
    /** Store the fields sequentially */
    hid_parser_field_t fields[] __attribute__((aligned(4)));
} hid_parser_report_t;

/**
	\brief A parsed report descriptor, with values.

	A representation of a fully parsed report descriptor complete with value
	fields for easy retrieval and setting of values.
*/
typedef struct {
    /** Each report descriptor has an application collection with a usage */
    hid_full_usage_t application;
    /** There can be multiple reports */
    uint8_t report_count;
    /** The interface number that HID is available on. */
    uint8_t interface;
    /** Store a pointer to each report. */
    hid_parser_report_t *report[] __attribute__((aligned(4)));
} hid_parser_result_t;

/**
	\brief Retrieves a value from a field.

	Reads the current value at a given index in a HID field.
*/
int32_t hid_get_field_value(hid_parser_field_t *field, uint32_t index);
