#include <rtthread.h>
#include "app_display.h"
#include "show.h"

#define DISPLAY_THREAD_STACK_SIZE 1536
#define DISPLAY_THREAD_PRIORITY 11
#define DISPLAY_PERIOD_MS 100

static void app_display_entry(void *parameter) {
    (void)parameter;

    while (1) {
        show();
        rt_thread_mdelay(DISPLAY_PERIOD_MS);
    }
}

rt_thread_t app_display_start(void) {
    rt_thread_t tid =
        rt_thread_create("display", app_display_entry, RT_NULL, DISPLAY_THREAD_STACK_SIZE, DISPLAY_THREAD_PRIORITY, 10);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    }
    return tid;
}
