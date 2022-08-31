/*
 * system_timer.c
 */

#include "../../include/kernel/barrier.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/mmio.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/system_timer.h"

DEBUG_INIT("system_timer");

void system_timer_set(uint32_t usecs)
{
    __dmb();
    mmio_write(SYSTEM_TIMER_COMPARE_1, mmio_read(SYSTEM_TIMER_COUNTER_LOW) + usecs);
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

static void system_timer_irq_handler(void *unused)
{
    (void) unused;
}

void system_timer_irq_clearer(void *unused)
{
    (void) unused;
    system_timer_control_reg_t control;
    __dmb();
    mmio_read_in(SYSTEM_TIMER_CONTROL, &control, 1);
    control.timer1_matched = 1;
    mmio_write_out(SYSTEM_TIMER_CONTROL, &control, 1);
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
    register_irq_handler(SYSTEM_TIMER_1, system_timer_irq_handler, NULL, system_timer_irq_clearer, NULL);
    system_timer_set(1000000);
    DEBUG_END();
}
