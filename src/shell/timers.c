/*
 * clock.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/arm_timer.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/kernel/mmio.h"

extern uint32_t get_arm_timer_control();

void show_timers()
{
    printf("System timer: system_time %llu, skipped_intervals %u, last_now %llu, last_next %llu\r\n",
           system_time, skipped_intervals, last_now, last_next);
    printf("System timer registers: counter %llu compare1 %u compare3 %u\r\n",
           get_system_timer_counter(), mmio_read(SYSTEM_TIMER_COMPARE_1), mmio_read(SYSTEM_TIMER_COMPARE_3));

    uint32_t arm_timer_control = get_arm_timer_control();
    if (arm_timer_control)
    {
        printf("ARM: control %p, free_running_counter %d value %d\r\n",
               arm_timer_control, get_arm_timer_free_running_counter(), get_arm_timer_value());
    }
    printf("\r\nTimer interval handlers:\r\n");
    for (int i = 0; i < MAX_INTERVAL_HANDLERS; i++)
    {
        if (interval_handlers[i].enabled) {
            printf("[%02d] %s: interval %d/100 sec, next in %d/100 sec\r\n", i,
                   interval_handlers[i].name,
                   interval_handlers[i].interval,
                   interval_handlers[i].counter);
        }
    }
}
