/*
 * system_timer.h
 */

#include "../common/stdbool.h"
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
} __attribute__ ((packed)) system_timer_control_reg_t;

#define SYSTEM_TIMER_TICK_INTERVAL      10000
#define MAX_INTERVAL_HANDLERS           20
#define INTERVAL_HANDLER_NAME_LENGTH    15
#define MAX_INTERVAL_HANDLER_ARGS       4

typedef void (*interval_handler_f)(int handle, void **args);

typedef struct {
    uint32_t interval;
    uint32_t counter;
    bool repeat;
    bool enabled;
    char name[INTERVAL_HANDLER_NAME_LENGTH + 1];
    interval_handler_f handler_function;
    int nargs;
    void *args[MAX_INTERVAL_HANDLER_ARGS];
} interval_handler_t;

extern uint64_t system_time;
extern uint32_t skipped_intervals;
extern uint64_t last_now, last_next;
extern interval_handler_t interval_handlers[MAX_INTERVAL_HANDLERS];

void system_timer_set_3_abs(uint32_t when);
void system_timer_busy_wait(uint32_t usecs);
void system_timer_3_irq_clearer(void *unused);
uint64_t get_system_timer_counter();
void system_timer_init();
int register_interval_handler(char *name, uint32_t interval, bool repeat, interval_handler_f handler_function,
                              int nargs, void **args);
void deregister_interval_handler(int handle);
