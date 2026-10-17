#include <rtthread.h>
#include <rthw.h>
#include "app_weight.h"
#include "HX711.h"
#include "Flash.h"
#include "Host.h"

#define WEIGHT_READ_MS 3000
#define WEIGHT_THREAD_STACK 1024
#define WEIGHT_THREAD_PRIO 10

static volatile int32_t s_weight = 0;
static volatile uint8_t s_tare_request = 0;

static void app_weight_publish(int32_t weight) {
    rt_base_t level = rt_hw_interrupt_disable();
    s_weight = weight;
    rt_hw_interrupt_enable(level);
}

int32_t app_weight_get(void) {
    int32_t weight;
    rt_base_t level = rt_hw_interrupt_disable();
    weight = s_weight;
    rt_hw_interrupt_enable(level);
    return weight;
}

void app_weight_request_tare(void) { s_tare_request = 1; }

static void app_weight_entry(void *parameter) {
    rt_tick_t last_read = rt_tick_get();
    int32_t weight;

    (void)parameter;
    while (1) {
        if (s_tare_request) {
            HX711_Tare(20);
            Flash_SaveTareOffset(HX711_GetOffset());
            s_tare_request = 0;
            weight = HX711_GetWeight();
            app_weight_publish(weight);
            Host_SendWeight(weight);
        } else if ((rt_tick_get() - last_read) >= rt_tick_from_millisecond(WEIGHT_READ_MS)) {
            last_read = rt_tick_get();
            weight = HX711_GetWeight();
            app_weight_publish(weight);
            Host_SendWeight(weight);
        }

        rt_thread_mdelay(50);
    }
}

rt_thread_t app_weight_start(void) {
    rt_thread_t tid =
        rt_thread_create("weight", app_weight_entry, RT_NULL, WEIGHT_THREAD_STACK, WEIGHT_THREAD_PRIO, 10);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    }
    return tid;
}
