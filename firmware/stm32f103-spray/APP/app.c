#include <rtthread.h>
#include "app.h"
#include "app_control.h"
#include "app_weight.h"
#include "app_display.h"
#include "Host.h"
#include "Servo.h"
#include "Spray.h"
#include "Pump.h"
#include "HX711.h"
#include "Flash.h"
#include "OLED.h"
#include "Delay.h"

#define SCALE 430.0f
#define CMD_THREAD_STACK 1024
#define CMD_THREAD_PRIO 7

static void app_command_entry(void *parameter) {
    (void)parameter;
    while (1) {
        Host_Process();
    }
}

int app_init(void) {
    rt_thread_t tid;
    int32_t tare;

    SERVO_Init();
    Host_Init();
    Spray_Init();
    Pump_Init();
    HX711_Init();
    HX711_SetScale(SCALE);

    if (Flash_LoadTareOffset(&tare)) {
        HX711_SetOffset(tare);
    }

    OLED_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "Spray Robot Arm");

    Spray_OFF();
    Pump_OFF();
    SERVO_LoadOrUnload(1, 1);
    SERVO_LoadOrUnload(2, 1);
    Delay_ms(100);
    SERVO_MoveTimeWrite(1, 400, 1500);
    SERVO_MoveTimeWrite(2, 700, 1500);
    Delay_ms(1700);

    tid = rt_thread_create("command", app_command_entry, RT_NULL, CMD_THREAD_STACK, CMD_THREAD_PRIO, 10);
    if (tid != RT_NULL) rt_thread_startup(tid);

    if (app_control_start() == RT_NULL) return -1;
    if (app_weight_start() == RT_NULL) return -1;
    if (app_display_start() == RT_NULL) return -1;

    return 0;
}
