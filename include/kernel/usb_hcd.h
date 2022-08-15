/*
 * usb_hcd.h - definitions for the Host Controller Device
 */

#include "../common/stdbool.h"
#include "../common/stdint.h"
#include "peripheral.h"
#include "usb.h"

#pragma once

#define HOST_CONTROLLER_BASE                (PERIPHERAL_BASE + HOST_CONTROLLER_OFFSET)

#define CORE_GLOBAL_OFFSET                  (HOST_CONTROLLER_BASE + 0x0000)
#define HOST_GLOBAL_OFFSET                  (HOST_CONTROLLER_BASE + 0x0400)
#define POWER_CLOCK_OFFSET                  (HOST_CONTROLLER_BASE + 0x0e00)

#define HCD_CORE_OTG_CONTROL                (CORE_GLOBAL_OFFSET + 0x00)
#define HCD_USB_GAHBCFG                     (CORE_GLOBAL_OFFSET + 0x08)
#define HCD_USB                             (CORE_GLOBAL_OFFSET + 0x0c)
#define HCD_RESET                           (CORE_GLOBAL_OFFSET + 0x10)
#define HCD_CORE_INTERRUPT                  (CORE_GLOBAL_OFFSET + 0x14)
#define HCD_CORE_INTERRUPT_MASK             (CORE_GLOBAL_OFFSET + 0x18)
#define HCD_CORE_RECEIVE_SIZE               (CORE_GLOBAL_OFFSET + 0x24)
#define HCD_CORE_NON_PERIODIC_FIFO_SIZE     (CORE_GLOBAL_OFFSET + 0x28)
#define HCD_USER_ID                         (CORE_GLOBAL_OFFSET + 0x3c)
#define HCD_VENDOR_ID                       (CORE_GLOBAL_OFFSET + 0x40)
#define HCD_HARDWARE                        (CORE_GLOBAL_OFFSET + 0x44)

#define HCD_CORE_PERIODIC_HOST_FIFO_SIZE    (CORE_GLOBAL_OFFSET + 0x100)

#define HCD_HOST_CONFIG                     (HOST_GLOBAL_OFFSET + 0x00)
#define HCD_HOST_PORT                       (HOST_GLOBAL_OFFSET + 0x40)
#define HCD_HOST_CHANNEL(x)                 (HOST_GLOBAL_OFFSET + 0x100 + 0x20 * (x))

#define HCD_POWER                           (POWER_CLOCK_OFFSET + 0x00)

#define RECEIVE_FIFO_SIZE                   20480 /* valid values are between 16 and 32768 */
#define NON_PERIODIC_FIFO_SIZE              20480 /* valid values are between 16 and 32768 */
#define PERIODIC_FIFO_SIZE                  20480 /* valid values are between 16 and 32768 */
#define CHANNEL_COUNT                       16
#define REQUEST_TIMEOUT                     5000

typedef enum {
    FLUSH_NON_PERIODIC,
    FLUSH_CHANNEL_1,
    FLUSH_CHANNEL_2,
    FLUSH_CHANNEL_3,
    FLUSH_CHANNEL_4,
    FLUSH_CHANNEL_5,
    FLUSH_CHANNEL_6,
    FLUSH_CHANNEL_7,
    FLUSH_CHANNEL_8,
    FLUSH_CHANNEL_9,
    FLUSH_CHANNEL_10,
    FLUSH_CHANNEL_11,
    FLUSH_CHANNEL_12,
    FLUSH_CHANNEL_13,
    FLUSH_CHANNEL_14,
    FLUSH_CHANNEL_15,
    FLUSH_ALL
} flush_number_t;

typedef enum {
    HNP_SNP_CAPABLE,
    SRP_ONLY_CAPABLE,
    NO_HNP_SNP_CAPABLE,
    SRP_CAPABLE_DEVICE,
    NO_SRP_CAPABLE_DEVICE,
    SRP_CAPABLE_HOST,
    NO_SRP_CAPABLE_HOST
} operating_mode_capabilities_t;

typedef enum {
    SLAVE_ONLY,
    EXTERNAL_DMA,
    INTERNAL_DMA
} architecture_capabilities_t;

typedef enum {
    NOT_SUPPORTED,
    UTMI,
    ULPI,
    UTMI_ULPI
} high_speed_physical_capabilities_t;

typedef enum {
    PHYSICAL_0,
    DEDICATED,
    PHYSICAL_1,
    PHYSICAL_2
} full_speed_physical_capabilities_t;

typedef enum {
    CLOCK_30_60_MHZ,
    CLOCK_48_MHZ,
    CLOCK_6_MHZ
} host_clock_rate_t;

typedef struct {
    bool ses_req_scs: 1;
    bool ses_req: 1;
    bool vb_valid_ov_en: 1;
    bool vb_valid_ov_val: 1;
    bool a_valid_ov_en: 1;
    bool a_valid_ov_val: 1;
    bool b_valid_ov_en: 1;
    bool b_valid_ov_val: 1;
    bool hst_neg_scs: 1;
    bool hnp_req: 1;
    bool host_set_hnp_enable: 1;
    bool dev_hnp_en: 1;
    uint8_t reserved_12_15: 4;
    bool con_id_sts: 1;
    bool dbnc_time: 1;
    bool a_session_valid: 1;
    bool b_session_valid: 1;
    bool otg_version: 1;
    bool reserved_21: 1;
    uint8_t mult_valid_bc: 5;
    bool chirp_en: 1;
    uint8_t reserved_28_31: 4;
} __attribute__ ((__packed__)) hcd_otg_control_t;

typedef struct {
    bool interrupt_enable: 1;
    uint8_t axi_burst_length: 2;
    bool reserved_3: 1;
    bool wait_for_axi_writes: 1;
    bool dma_enable: 1;
    bool reserved_6: 1;
    bool transfer_empty_level: 1;
    bool periodic_transfer_empty_level: 1;
    uint16_t reserved_9_20: 12;
    bool rem_mem_supp: 1;
    bool noti_all_dma_writ: 1;
    bool dma_remainder_mode: 1;
    bool reserved_31: 1;
} __attribute__ ((__packed__)) hcd_ahb_t;

typedef struct {
    uint8_t toutcal: 3; // ???
    bool phy_interface: 1;
    bool mode_select: 1;
    bool fsintf: 1;
    bool phy_sel: 1;
    bool ddr_sel: 1;
    bool srp_capable: 1;
    bool hnp_capable: 1;
    uint8_t usb_trd_tim: 4;
    bool reserved_14: 1;
    bool phy_lpm_clk_sel: 1;
    bool otg_utm_ifs_sel: 1;
    bool ulpi_fsls: 1;
    bool ulpi_auto_res: 1;
    bool ulpi_clk_sus_m: 1;
    bool ulpi_drive_external_vbus: 1;
    bool ulpi_int_vbus_indicator: 1;
    bool ts_dline_pulse_enable: 1;
    bool indicator_complement: 1;
    bool indicator_pass_through: 1;
    bool ulpi_int_prot_dis: 1;
    bool ic_usb_capable: 1;
    bool ic_traffic_pull_remove: 1;
    bool tx_end_delay: 1;
    bool force_host_mode: 1;
    bool force_dev_mode: 1;
} __attribute__ ((__packed__)) hcd_usb_register_t;

typedef struct {
    uint16_t start_address: 16;
    uint16_t depth: 16;
} __attribute__ ((__packed__)) hcd_fifo_size_t;

typedef struct {
    bool core_soft: 1;
    bool h_clk_soft: 1;
    bool host_frame_counter: 1;
    bool in_token_queue_flush: 1;
    bool receive_fifo_flush: 1;
    bool transmit_fifo_flush: 1;
    flush_number_t transmit_fifo_flush_number: 5;
    uint32_t reserved_11_29: 19;
    bool dma_request_signal: 1;
    bool ahb_master_idle: 1;
} __attribute__ ((__packed__)) hcd_reset_t;

typedef struct {
    union {
       struct {
           uint8_t direction0: 2;
           uint8_t direction1: 2;
           uint8_t direction2: 2;
           uint8_t direction3: 2;
           uint8_t direction4: 2;
           uint8_t direction5: 2;
           uint8_t direction6: 2;
           uint8_t direction7: 2;
           uint8_t direction8: 2;
           uint8_t direction9: 2;
           uint8_t direction10: 2;
           uint8_t direction11: 2;
           uint8_t direction12: 2;
           uint8_t direction13: 2;
           uint8_t direction14: 2;
           uint8_t direction15: 2;
       };
       uint32_t word0; /* bits 0-31 */
    };
    union {
        struct {
            operating_mode_capabilities_t operating_mode: 3;
            architecture_capabilities_t architecture: 2;
            bool point_to_point: 1;
            high_speed_physical_capabilities_t high_speed_physical: 2;
            full_speed_physical_capabilities_t full_speed_physical: 2;
            uint8_t device_endpoint_count: 4;
            uint8_t host_channel_count: 4;
            bool supports_periodic_endpoints: 1;
            bool dynamic_fifo: 1;
            bool multi_proc_int: 1;
            bool reserved_53: 1;
            uint8_t non_periodic_queue_depth: 2;
            uint8_t host_periodic_queue_depth: 2;
            uint8_t device_token_queue_depth: 2;
            bool enable_ic_usb: 1;
        };
        uint32_t word1; /* bits 32-63 */
    };
    union {
        struct {
            uint8_t transfer_size_control_width: 4;
            uint8_t packet_size_control_width: 3;
            bool otg_func: 1;
            bool i2c: 1;
            bool vendor_control_interface: 1;
            bool optional_features: 1;
            bool synchronous_reset_type: 1;
            bool adp_support: 1;
            bool otg_enable_hsic: 1;
            bool bc_support: 1;
            bool low_power_mode_enabled: 1;
            uint16_t fifo_depth: 16;
        };
        uint32_t word2; /* bits 64-95 */
    };
    union {
        struct {
            uint8_t periodic_in_endpoint_count: 4;
            bool power_optimisation: 1;
            bool minimum_ahb_frequency: 1;
            bool partial_power_off: 1;
            uint8_t reserved_103_109: 7;
            uint8_t utmi_physical_data_width: 2;
            uint8_t mode_control_endpoint_count: 4;
            bool valid_filter_id_dig_enabled: 1;
            bool vbus_valid_filter_enabled: 1;
            bool valid_filter_a_enabled: 1;
            bool valid_filter_b_enabled: 1;
            bool session_end_filter_enabled: 1;
            bool ded_fifo_en: 1;
            uint8_t in_endpoint_count: 4;
            bool dma_description: 1;
            bool dma_dynamic_description: 1;
        };
        uint32_t word3; /* bits 96-127 */
    };
} __attribute__ ((__packed__)) hcd_hardware_t;

typedef struct {
    host_clock_rate_t clock_rate: 2;
    bool fsls_only : 1;
    uint8_t reserved_3_6: 4;
    bool en_32khz_susp: 1;
    uint8_t res_val_period: 8;
    uint8_t reserved_16_22: 7;
    bool enable_dma_descriptor: 1;
    uint8_t frame_list_entries: 2;
    bool periodic_schedule_enable: 1;
    bool periodic_schedule_status: 1;
    uint8_t reserved_28_30: 3;
    bool chg_time: 1;
} __attribute__ ((__packed__)) hcd_host_configuration_t;

typedef struct {
    bool connect: 1;
    bool connect_detected: 1;
    bool enable: 1;
    bool enable_changed: 1;
    bool over_current: 1;
    bool over_current_charged: 1;
    bool resume: 1;
    bool suspend: 1;
    bool reset: 1;
    bool reserved_9: 1;
    uint8_t port_line_status: 3;
    bool power: 1;
    uint8_t test_control: 4;
    bool speed: 1;
    uint16_t reserved_19_31: 13;
} __attribute__ ((__packed__)) hcd_host_port_t;

typedef struct {
    uint16_t maximum_packet_size: 11;
    uint8_t endpoint_number: 4;
    usb_direction_t endpoint_direction: 1;
    bool reserved_16: 1;
    bool low_speed: 1;
    usb_transfer_type_t type: 2;
    uint8_t packets_per_frame: 2;
    uint8_t device_address: 7;
    bool odd_frame: 1;
    bool disable: 1;
    bool enable: 1;
} __attribute__ ((__packed__)) hcd_host_channel_characteristic_t;

typedef struct {
    uint32_t value;
} __attribute__ ((__packed__)) hcd_host_channel_split_control_t;

typedef struct {
    uint32_t value;
} __attribute__ ((__packed__)) hcd_host_channel_interrupt_t;

typedef struct {
    uint32_t value;
} __attribute__ ((__packed__)) hcd_host_channel_transfer_size_t;

typedef struct {
    hcd_host_channel_characteristic_t characteristic;
    hcd_host_channel_split_control_t split_control;
    hcd_host_channel_interrupt_t interrupt;
    hcd_host_channel_interrupt_t interrupt_mask;
    hcd_host_channel_transfer_size_t transfer_size;
    uint32_t dma_address;
    uint32_t reserved_0x18;
    uint32_t reserved_0x1c;
} __attribute__ ((__packed__)) hcd_host_channel_t;

typedef struct {
    hcd_host_channel_t channel[CHANNEL_COUNT];
} __attribute__ ((__packed__)) hcd_host_channels_t;

int hcd_submit_control_message(usb_device_t *device, usb_pipe_address_t pipe, void *buffer, uint32_t buffer_length,
                               usb_device_request_t *request);
int hcd_init();
int hcd_start();
