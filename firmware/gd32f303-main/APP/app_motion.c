#include <rtthread.h>
#include "app_motion.h"
#include "cmd.h"
#include "conctrl.h"

#define MOTION_THREAD_STACK_SIZE 1024
#define MOTION_THREAD_PRIORITY 8
#define MOTION_PERIOD_TICKS 10 /* 10 ms */

static void app_motion_entry(void *parameter) {
    rt_tick_t next_tick;
    u8 command;
    int speed_mm_s;

    (void)parameter;
    next_tick = rt_tick_get();

    while (1) {
        /* Take a consistent command snapshot, then run one control cycle. */
        Car_State_Get(&command, &speed_mm_s);
        Car_Control(command, speed_mm_s);
        Motion_Control_Step();

        /* Absolute-period scheduling avoids accumulating execution jitter. */
        rt_thread_delay_until(&next_tick, MOTION_PERIOD_TICKS);
    }
}

rt_thread_t app_motion_start(void) {
    rt_thread_t tid =
        rt_thread_create("motion", app_motion_entry, RT_NULL, MOTION_THREAD_STACK_SIZE, MOTION_THREAD_PRIORITY, 10);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    }
    return tid;
}
