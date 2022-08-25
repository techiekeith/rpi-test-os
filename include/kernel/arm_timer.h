/*
 * arm_timer.h
 */

#include "peripheral.h"

#define ARM_TIMER_BASE          (peripheral_base + ARM_TIMER_OFFSET)

typedef union
{
    struct
    {
        uint8_t one_shot_mode: 1; /* not supported in the BCM2835 */
        uint8_t counter_width: 1; /* if set, use single 23-bit counter */
        uint8_t pre_scale: 2; /* 00 = no pre-scale, 01 = clock / 16, 10 = clock / 256, 11 = undefined */
        uint8_t not_used_b4: 1;
        uint8_t interrupt_enable: 1;
        uint8_t periodic_mode: 1; /* not supported in the BCM2835 */
        uint8_t enable: 1;
        uint8_t halt_in_debug_mode: 1;
        uint8_t free_running_counter_enable: 1;
        uint8_t not_used_b10_b15: 6;
        uint8_t free_running_counter_pre_scaler; /* frequency is sys_clk / (prescale + 1) */
        uint8_t not_used_b24_b31;
    };
    uint32_t value;
} arm_timer_control_reg_t;

typedef struct
{
    uint32_t load;
    uint32_t value;                     /* read only */
    arm_timer_control_reg_t control;
    uint32_t irq_clear_ack;             /* write only */
    uint32_t raw_irq;                   /* read only */
    uint32_t masked_irq;                /* read only */
    uint32_t reload;
    uint32_t pre_divider;
    uint32_t free_running_counter;      /* read only */
} arm_timer_registers_t;

void arm_timer_set(uint32_t usecs);
void arm_timer_delay(uint32_t usecs);
uint32_t get_arm_timer_free_running_counter();
uint32_t get_arm_timer_value();
void arm_timer_init();
