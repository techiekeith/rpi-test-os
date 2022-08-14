/*
 * arm_timer.c
 */

#include "../../include/kernel/barrier.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/arm_timer.h"

DEBUG_INIT("arm_timer");

static volatile arm_timer_registers_t *arm_timer_registers;

void arm_timer_set(uint32_t usecs)
{
    arm_timer_registers->load = usecs;
    __dmb();
}

__attribute__ ((optimize(0))) void arm_timer_delay(uint32_t usecs)
{
    __dmb();
    volatile uint32_t start = arm_timer_registers->free_running_counter;
    __dmb();
    volatile uint32_t elapsed = 0;
    while (elapsed < usecs)
    {
        elapsed = arm_timer_registers->free_running_counter - start;
        __dmb();
    }
}

static void arm_timer_irq_handler()
{
    debug_printf("ARM timer timeout.\r\n");
}

static void arm_timer_irq_clearer()
{
    arm_timer_registers->irq_clear_ack = 0;
    __dmb();
}

uint32_t get_arm_timer_control()
{
    __dmb();
    return arm_timer_registers->control.value;
}

uint32_t get_arm_timer_free_running_counter()
{
    __dmb();
    return arm_timer_registers->free_running_counter;
}

uint32_t get_arm_timer_value()
{
    __dmb();
    return arm_timer_registers->value;
}

void arm_timer_init()
{
    DEBUG_START("arm_timer_init");
    arm_timer_registers = (arm_timer_registers_t *)ARM_TIMER_BASE;
    __dmb();
    if (arm_timer_registers->pre_divider != 0x7d)
    {
        debug_printf("pre_divider: Expected value 0x7d, got value 0x%02x\r\n", arm_timer_registers->pre_divider);
    }
    arm_timer_registers->control.enable = 1;
    arm_timer_registers->control.interrupt_enable = 1;
    arm_timer_registers->control.free_running_counter_enable = 1;
    __dmb();
    register_irq_handler(ARM_TIMER, arm_timer_irq_handler, arm_timer_irq_clearer);
    DEBUG_END();
}
