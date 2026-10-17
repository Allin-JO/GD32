#include <rtthread.h>
#include "app_display.h"
#include "app_control.h"
#include "app_weight.h"
#include "OLED.h"

#define DISPLAY_THREAD_STACK 1024
#define DISPLAY_THREAD_PRIO 11
#define DISPLAY_PERIOD_MS 100

static void app_display_entry(void *parameter) {
    int32_t weight;
    uint32_t shown;

    (void)parameter;
    while (1) {
        weight = app_weight_get();

        OLED_ShowString(2, 1, "Weight:      g");
        if (weight >= 0) {
            shown = (uint32_t)weight;
            OLED_ShowNum(2, 8, shown, 5);
        } else {
            OLED_ShowChar(2, 8, '-');
            OLED_ShowNum(2, 9, (uint32_t)(-weight), 5);
        }

        if (app_control_get_mode()) {
            OLED_ShowString(3, 1, "Mode: WORK   ");
        } else {
            OLED_ShowString(3, 1, "Mode: OFF    ");
        }

        OLED_ShowString(4, 1, "RT-Thread 4.1.1");
        rt_thread_mdelay(DISPLAY_PERIOD_MS);
    }
}

rt_thread_t app_display_start(void) {
    rt_thread_t tid =
        rt_thread_create("display", app_display_entry, RT_NULL, DISPLAY_THREAD_STACK, DISPLAY_THREAD_PRIO, 10);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    }
    return tid;
}
