/*
 * system_timer.h
 */

#include "peripheral.h"

#define SYSTEM_TIMER_BASE           (peripheral_base + SYSTEM_TIMER_OFFSET)

typedef struct {
    uint8_t timer0_matched: 1;
    uint8_t timer1_matched: 1;
    uint8_t timer2_matched: 1;
    uint8_t timer3_matched: 1;
    uint32_t reserved: 28;
} system_timer_control_reg_t;

typedef struct {
    system_timer_control_reg_t control;
    union {
        struct {
            uint32_t low;
            uint32_t high;
        };
        uint64_t value;
    } counter;
    uint32_t timer0;
    uint32_t timer1;
    uint32_t timer2;
    uint32_t timer3;
} system_timer_registers_t;

void system_timer_set(uint32_t usec);
void system_timer_delay(uint32_t usecs);
uint64_t get_system_timer_counter();
void system_timer_init();
