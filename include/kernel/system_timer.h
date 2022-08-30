/*
 * system_timer.h
 */

#include "../common/stdint.h"
#include "peripheral.h"

#define SYSTEM_TIMER_BASE           (peripheral_base + SYSTEM_TIMER_OFFSET)

#define SYSTEM_TIMER_CONTROL        (SYSTEM_TIMER_BASE + 0x00)
#define SYSTEM_TIMER_COUNTER_LOW    (SYSTEM_TIMER_BASE + 0x04)
#define SYSTEM_TIMER_COUNTER_HIGH   (SYSTEM_TIMER_BASE + 0x08)
#define SYSTEM_TIMER_COMPARE_0      (SYSTEM_TIMER_BASE + 0x0c)
#define SYSTEM_TIMER_COMPARE_1      (SYSTEM_TIMER_BASE + 0x10)
#define SYSTEM_TIMER_COMPARE_2      (SYSTEM_TIMER_BASE + 0x14)
#define SYSTEM_TIMER_COMPARE_3      (SYSTEM_TIMER_BASE + 0x18)

typedef struct {
    uint8_t timer0_matched: 1;
    uint8_t timer1_matched: 1;
    uint8_t timer2_matched: 1;
    uint8_t timer3_matched: 1;
    uint32_t reserved: 28;
} __attribute__((packed)) system_timer_control_reg_t;

void system_timer_set(uint32_t usec);
void system_timer_set_3(uint32_t usec);
void system_timer_busy_wait(uint32_t usecs);
uint64_t get_system_timer_counter();
void system_timer_init();
