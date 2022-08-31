/*
 * clock.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/arm_timer.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/kernel/mmio.h"

extern uint32_t get_arm_timer_control();

void show_timers()
{
    uint32_t arm_timer_control = get_arm_timer_control();
    if (arm_timer_control)
    {
        printf("ARM: control %p, free_running_counter %d value %d\r\n",
               arm_timer_control, get_arm_timer_free_running_counter(), get_arm_timer_value());
    }
    printf("System: counter %llu compare1 %u compare3 %u\r\n",
           get_system_timer_counter(), mmio_read(SYSTEM_TIMER_COMPARE_1), mmio_read(SYSTEM_TIMER_COMPARE_3));
}
