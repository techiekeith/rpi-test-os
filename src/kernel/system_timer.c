/*
 * system_timer.c
 */

#include "../../include/common/string.h"
#include "../../include/kernel/barrier.h"
#include "../../include/kernel/heap.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/mmio.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/system_timer.h"

DEBUG_INIT("system_timer");

uint64_t system_time;
uint32_t skipped_intervals;
uint64_t last_now, last_next;
interval_handler_t interval_handlers[MAX_INTERVAL_HANDLERS];

static void system_timer_start()
{
    __dmb();
    uint32_t intervals = mmio_read(SYSTEM_TIMER_COUNTER_LOW) / SYSTEM_TIMER_TICK_INTERVAL;
    intervals++;
    mmio_write(SYSTEM_TIMER_COMPARE_1, SYSTEM_TIMER_TICK_INTERVAL * intervals);
    __dmb();
}

void system_timer_set_3_abs(uint32_t when)
{
    __dmb();
    mmio_write(SYSTEM_TIMER_COMPARE_3, when);
    __dmb();
}

void system_timer_busy_wait(uint32_t usecs)
{
    volatile uint64_t start = get_system_timer_counter();
    volatile uint64_t elapsed = 0;
    while (elapsed < usecs)
    {
        elapsed = get_system_timer_counter() - start;
    }
}

static void system_timer_interval_handler(void *unused)
{
    (void) unused;

    for (int i = 0; i < MAX_INTERVAL_HANDLERS; i++)
    {
        if (interval_handlers[i].enabled)
        {
            interval_handlers[i].counter--;
            if (!interval_handlers[i].counter)
            {
                interval_handler_f handler_function = interval_handlers[i].handler_function;
                interval_handlers[i].counter = interval_handlers[i].interval;
                interval_handlers[i].enabled = interval_handlers[i].repeat;
                handler_function();
            }
        }
    }
}

static void system_timer_tick(void *unused)
{
    (void) unused;
    system_timer_control_reg_t control;

    __dmb();
    mmio_read_in(SYSTEM_TIMER_CONTROL, &control, 1);
    control.timer1_matched = 1;
    mmio_write_out(SYSTEM_TIMER_CONTROL, &control, 1);
    __dmb();

    __dmb();
    uint64_t next = (uint64_t) mmio_read(SYSTEM_TIMER_COMPARE_1);
    uint64_t now = (uint64_t) mmio_read(SYSTEM_TIMER_COUNTER_LOW);
    uint64_t mask = (now < next) ? 0x100000000ULL : 0; // Handle counter low word rolling through zero
    now |= mask;

    system_time++;
    next += SYSTEM_TIMER_TICK_INTERVAL;

    while (now + 1000 >= next) /* don't set compare1 too close to current time */
    {
        skipped_intervals++;
        system_time++;
        next += SYSTEM_TIMER_TICK_INTERVAL;
        now = (uint64_t) mmio_read(SYSTEM_TIMER_COUNTER_LOW) | mask;
    }
    mmio_write(SYSTEM_TIMER_COMPARE_1, (uint32_t) next);
    last_now = now;
    last_next = next;
    __dmb();
}

void system_timer_3_irq_clearer(void *unused)
{
    (void) unused;
    system_timer_control_reg_t control;
    __dmb();
    mmio_read_in(SYSTEM_TIMER_CONTROL, &control, 1);
    control.timer3_matched = 1;
    mmio_write_out(SYSTEM_TIMER_CONTROL, &control, 1);
    __dmb();
}

uint64_t get_system_timer_counter()
{
    __dmb();
    uint32_t high1 = mmio_read(SYSTEM_TIMER_COUNTER_HIGH);
    uint32_t low = mmio_read(SYSTEM_TIMER_COUNTER_LOW);
    uint32_t high2 = mmio_read(SYSTEM_TIMER_COUNTER_HIGH);
    if ((high1 != high2) && ((low >> 31) == 0)) high1 = high2;
    return (uint64_t)low | ((uint64_t)high1 << 32LL);
}

void system_timer_init()
{
    DEBUG_START("system_timer_init");
    register_irq_handler(SYSTEM_TIMER_1, system_timer_interval_handler, NULL, system_timer_tick, NULL);
    system_timer_start();
    DEBUG_END();
}

int register_interval_handler(char *name, interval_handler_f handler_function, uint32_t interval, bool repeat)
{
    for (int i = 0; i < MAX_INTERVAL_HANDLERS; i++)
    {
        if (!interval_handlers[i].enabled)
        {
            strncpy(interval_handlers[i].name, name, INTERVAL_HANDLER_NAME_LENGTH);
            interval_handlers[i].name[INTERVAL_HANDLER_NAME_LENGTH] = '\0';
            interval_handlers[i].handler_function = handler_function;
            interval_handlers[i].interval = interval;
            interval_handlers[i].counter = interval;
            interval_handlers[i].repeat = repeat;
            interval_handlers[i].enabled = true;
            return i + 1;
        }
    }
    return 0;
}

void deregister_interval_handler(int handle)
{
    if (handle > 0 && handle <= MAX_INTERVAL_HANDLERS) interval_handlers[handle - 1].enabled = false;
}
