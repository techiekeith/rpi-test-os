/*
 * clock.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/arm_timer.h"
#include "../../include/kernel/system_timer.h"

extern uint32_t get_arm_timer_control();

void show_timers()
{
    printf("ARM: control %p, free_running_counter %ld value %ld\r\n",
           get_arm_timer_control(), get_arm_timer_free_running_counter(), get_arm_timer_value());
    printf("System: %ld\r\n", get_system_timer_counter());
}
