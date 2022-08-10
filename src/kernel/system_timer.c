/*
 * system_timer.c
 */

#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/system_timer.h"

static system_timer_registers_t *system_timer_registers;

void system_timer_set(uint32_t usecs)
{
    system_timer_registers->timer1 = system_timer_registers->counter.low + usecs;
}

__attribute__ ((optimize(0))) void system_timer_delay(uint32_t usecs)
{
    volatile uint64_t start = system_timer_registers->counter.value;
    volatile uint64_t elapsed = 0;
    while (elapsed < usecs)
    {
        elapsed = system_timer_registers->counter.value - start;
    }
}

static void system_timer_irq_handler()
{
    debug_printf("System timer timeout.\r\n");
}

static void system_timer_irq_clearer()
{
    system_timer_registers->control.timer1_matched = 1;
}

uint64_t get_system_timer_counter()
{
    return system_timer_registers->counter.value;
}

void system_timer_init()
{
    system_timer_registers = (system_timer_registers_t *)SYSTEM_TIMER_BASE;
    register_irq_handler(SYSTEM_TIMER_1, system_timer_irq_handler, system_timer_irq_clearer);
}
